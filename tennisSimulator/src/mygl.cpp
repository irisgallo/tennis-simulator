#include "mygl.h"
#include <glm_includes.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QDateTime>

MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      prog_flat(this),
      m_ball(this, glm::vec3(-150.f, 0.f, 0.f),
               glm::vec3(40.f, 10.f, 0.f), 30.f),
      m_racquet(this, glm::vec3(-160.f, -20.f, 0.f)),
      m_geomCourt(this, 4),
      m_geomNet(this, 4),
      racquetClosestPoint(this),
      netClosestPoint(this),
      racquetNormal(this),
      ballOrientation(this),
      gravityForce(this),
      dragForce(this),
      liftForce(this),
      prevMSecs(0)
{

    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    // Tell the timer to redraw 60 times per second
    m_timer.start(16);
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL() {

    makeCurrent();

    glDeleteVertexArrays(1, &vao);

    m_ball.destroy();
    m_racquet.destroy();
    m_geomCourt.destroy();
    m_geomNet.destroy();

    racquetClosestPoint.destroy();
    netClosestPoint.destroy();
    racquetNormal.destroy();
    ballOrientation.destroy();

    gravityForce.destroy();
    dragForce.destroy();
    liftForce.destroy();
}


void MyGL::initializeGL() {

    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.6, 0, 1, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    m_ball.create();
    m_racquet.create();
    m_geomCourt.create();
    m_geomNet.create();

    racquetClosestPoint.create();
    netClosestPoint.create();
    racquetNormal.create();
    ballOrientation.create();

    gravityForce.create();
    dragForce.create();
    liftForce.create();

    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

    m_ball.racquet = &m_racquet;
    racquetClosestPoint.m_pos = m_racquet.m_pos;
    netClosestPoint.m_pos = glm::vec3(0.f, -68.f, 0.f);
    racquetNormal.m_pos = m_racquet.m_pos;
    ballOrientation.m_pos = m_ball.m_pos + (m_ball.m_radius * m_ball.getOrientation());

    sendSignals(m_ball.m_pos0, m_ball.m_vel0, m_ball.m_angVel0);
}


void MyGL::resizeGL(int w, int h) {

    glm::mat3 viewMat = glm::scale(glm::mat3(), glm::vec2(0.1, 0.15));

    // Upload the view matrix to our shader (i.e. onto the graphics card)
    prog_flat.setViewMatrix(viewMat);

    printGLErrorLog();
}


void MyGL::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // background color
    glClearColor(0.23f, 0.44f, 0.62f, 1);

    // ball
    prog_flat.setModelMatrix(m_ball.getBallModelMatrix());
    prog_flat.draw(*this, m_ball);

    // racquet
    m_racquet.setColor(glm::vec3(0.898f, 0.840, 1.f));
    prog_flat.setModelMatrix(m_racquet.getModelMatrix());
    prog_flat.draw(*this, m_racquet);

    // tennis court/net
    m_geomCourt.setColor(glm::vec3(0.8, 0.8, 0.8));
    prog_flat.setModelMatrix(m_ball.getCourtModelMatrix());
    prog_flat.draw(*this, m_geomCourt);
    m_geomNet.setColor(glm::vec3(0.8, 0.8, 0.8));
    prog_flat.setModelMatrix(m_ball.getNetModelMatrix());
    prog_flat.draw(*this, m_geomNet);

    // debug point visuals
    racquetClosestPoint.m_pos = m_racquet.closestPoint;
    racquetClosestPoint.setColor(glm::vec3(1, 0, 0));
    prog_flat.setModelMatrix(racquetClosestPoint.getModelMatrix());
    prog_flat.draw(*this, racquetClosestPoint);

    netClosestPoint.m_pos = m_ball.netPoint;
    netClosestPoint.setColor(glm::vec3(0, 1, 0));
    prog_flat.setModelMatrix(netClosestPoint.getModelMatrix());
    prog_flat.draw(*this, netClosestPoint);

    racquetNormal.m_pos = m_racquet.closestNormal;
    racquetNormal.m_pos *= 10;
    racquetNormal.m_pos += m_racquet.closestPoint;
    racquetNormal.setColor(glm::vec3(1, 0, 0));
    prog_flat.setModelMatrix(racquetNormal.getModelMatrix());
    prog_flat.draw(*this, racquetNormal);

    ballOrientation.m_pos = m_ball.m_pos + (m_ball.m_radius * m_ball.getOrientation());
    ballOrientation.setColor(glm::vec3(1, 0.45, 0.91));
    prog_flat.setModelMatrix(ballOrientation.getModelMatrix());
    prog_flat.draw(*this, ballOrientation);

    gravityForce.m_pos = m_ball.m_pos + m_ball.f_gravity;
    gravityForce.setColor(glm::vec3(0, 1, 0));
    prog_flat.setModelMatrix(gravityForce.getModelMatrix());
    prog_flat.draw(*this, gravityForce);

    dragForce.m_pos = m_ball.m_pos + m_ball.f_drag;
    dragForce.setColor(glm::vec3());
    prog_flat.setModelMatrix(dragForce.getModelMatrix());
    prog_flat.draw(*this, dragForce);

    liftForce.m_pos = m_ball.m_pos + m_ball.f_lift;
    liftForce.setColor(glm::vec3(0, 0, 1));
    prog_flat.setModelMatrix(liftForce.getModelMatrix());
    prog_flat.draw(*this, liftForce);
}


// MyGL's constructor links tick() to a timer that fires 60 times per second.
// We're treating MyGL as our game engine class, so we're going to perform
// all per-frame actions here, such as performing physics updates on all
// entities in the scene.
void MyGL::tick() {

    update(); // Calls paintGL() as part of a larger QOpenGLWidget pipeline

    qint64 currMSecs = QDateTime::currentMSecsSinceEpoch();
    qint64 dT = currMSecs - prevMSecs;
    prevMSecs = currMSecs;

    m_ball.tick(dT);

    emit sig_sendHitVelocity(m_ball.m_hitVelocity.x,
                             m_ball.m_hitVelocity.y);
}


void MyGL::slot_setStartStop() {

    m_ball.pressedStartStop();
}


void MyGL::slot_reset() {

    m_ball.reset();
}


void MyGL::slot_setPX(double px) {

    m_ball.m_pos0[0] = px;
    m_ball.reset();
}


void MyGL::slot_setPY(double py) {

    m_ball.m_pos0[1] = py;
    m_ball.reset();
}


void MyGL::slot_setVX(double vx) {

    m_ball.m_vel0[0] = vx;
    m_ball.reset();
}


void MyGL::slot_setVY(double vy) {

    m_ball.m_vel0[1] = vy;
    m_ball.reset();
}


void MyGL::slot_setAV(double av) {

    m_ball.m_angVel0 = av;
    m_ball.reset();
}


void MyGL::sendSignals(glm::vec3 pos0, glm::vec3 vel0, float angVel0) {

    emit sig_sendPos(pos0[0], pos0[1]);
    emit sig_sendVel(vel0[0], vel0[1]);
    emit sig_sendAngVel(angVel0);
}


// screen space is ([0:900], [0:600])
// world space is ([-200:200], [-133:133])
void MyGL::mousePressEvent(QMouseEvent *event) {

    float xAtPress = (event->position().x() - 450.0) * (4.0 / 9.0);
    float yAtPress = (event->position().y() - 300.0) * (-4.0 / 9.0);

    std::cerr << "x: " << xAtPress << ", y: " << yAtPress << "\n";

    event->accept();
}

void MyGL::mouseMoveEvent(QMouseEvent *event) {

    float xAtMove = (event->position().x() - 450.0) * (4.0 / 9.0);
    float yAtMove = (event->position().y() - 300.0) * (-4.0 / 9.0);

    glm::vec3 currPos = glm::vec3(xAtMove, yAtMove, 0.f);

    m_racquet.m_vel = (currPos - m_racquet.m_pos) * 6.f;
    m_racquet.m_pos = currPos;

    event->accept();
}


void MyGL::keyPressEvent(QKeyEvent *event) {

    switch (event->key()) {
        case Qt::Key_Space:
            m_ball.pressedStartStop();
            break;
        case Qt::Key_D:
            m_racquet.rotateCounterclockwise();
            break;
        case Qt::Key_A:
            m_racquet.rotateClockwise();
            break;
    }
}
