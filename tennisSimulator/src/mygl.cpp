#include "mygl.h"
#include <glm_includes.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QDateTime>

MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      prog_flat(this),
      m_geomCircle(this, 20),
      m_geomSquare(this, 4),
      m_displayPoint(this, 10),
      prevMSecs(0)
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    // Tell the timer to redraw 60 times per second
    m_timer.start(16);
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();

    glDeleteVertexArrays(1, &vao);
    m_geomCircle.destroy();
    m_geomSquare.destroy();
    m_displayPoint.destroy();
}

void MyGL::initializeGL()
{
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

    m_geomCircle.create();
    m_geomSquare.create();
    m_displayPoint.create();

    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

    // initalize our tennis ball and racquet
    m_ball = Ball(glm::vec3(-120.f, 0.f, 0.f),
                  glm::vec3(15.f, 30.f, 0.f),
                  glm::vec3(0.84f, 1.0f, 0.15f));
    m_racquet = Racquet(glm::vec3(-130.f, 0.f, 0.f),
                        glm::vec3(0.8, 0.8, 0.8));
    point = m_ball.getPosition();

    sendSignals(m_ball.getInitialPosition(), m_ball.getInitialVelocity());
}

void MyGL::resizeGL(int w, int h)
{
    glm::mat3 viewMat = glm::scale(glm::mat3(), glm::vec2(0.1, 0.15));

    // Upload the view matrix to our shader (i.e. onto the graphics card)
    prog_flat.setViewMatrix(viewMat);

    printGLErrorLog();
}

void MyGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // background color
    glClearColor(0.23f, 0.44f, 0.62f, 1);

    bool isColliding = detectCollision();
    if (isColliding)
    {
        m_geomCircle.setColor(glm::vec3(0.969, 0.512, 0.473));
    }
    else
    {
        m_geomCircle.setColor(m_ball.getColor());
    }

    // circle
    //m_geomCircle.setColor(m_ball.getColor());
    prog_flat.setModelMatrix(m_ball.getModelMatrix());
    prog_flat.draw(*this, m_geomCircle);

    // rectangle
    m_geomSquare.setColor(m_racquet.getColor());
    prog_flat.setModelMatrix(m_racquet.getModelMatrix());
    prog_flat.draw(*this, m_geomSquare);

    m_displayPoint.setColor(glm::vec3(1, 0, 0));

    glm::mat3 mat = glm::mat3({{0.2, 0, 0},
                               {0, 0.2, 0},
                               {0.1 * point.x, 0.1 * point.y, 1}});
    prog_flat.setModelMatrix(mat);
    prog_flat.draw(*this, m_displayPoint);
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
}

void MyGL::slot_setStartStop()
{
    m_ball.pressedStartStop();
}

void MyGL::slot_reset()
{
    m_ball.reset();
}

void MyGL::slot_setPX(double px)
{
    glm::vec3 pos0 = m_ball.getInitialPosition();
    pos0[0] = px;
    m_ball.setInitialPosition(pos0);
}

void MyGL::slot_setPY(double py)
{
    glm::vec3 pos0 = m_ball.getInitialPosition();
    pos0[1] = py;
    m_ball.setInitialPosition(pos0);
}

void MyGL::slot_setVX(double vx)
{
    glm::vec3 vel0 = m_ball.getInitialVelocity();
    vel0[0] = vx;
    m_ball.setInitialVelocity(vel0);
}

void MyGL::slot_setVY(double vy)
{
    glm::vec3 vel0 = m_ball.getInitialVelocity();
    vel0[1] = vy;
    m_ball.setInitialVelocity(vel0);
}

void MyGL::sendSignals(glm::vec3 pos0, glm::vec3 vel0)
{
    emit sig_sendPos(pos0[0], pos0[1]);
    emit sig_sendVel(vel0[0], vel0[1]);
}

// screen space is ([0:900], [0:600])
// world space is ([-200:200], [-133:133])
void MyGL::mousePressEvent(QMouseEvent *event)
{
    float xAtPress = (event->position().x() - 450.0) * (4.0 / 9.0);
    float yAtPress = (event->position().y() - 300.0) * (-4.0 / 9.0);

    std::cerr << "x: " << xAtPress << ", y: " << yAtPress << "\n";

    event->accept();
}

void MyGL::mouseMoveEvent(QMouseEvent *event)
{
    float xAtMove = (event->position().x() - 450.0) * (4.0 / 9.0);
    float yAtMove = (event->position().y() - 300.0) * (-4.0 / 9.0);

    m_racquet.setPosition(glm::vec3(xAtMove, yAtMove, 0.f));

    event->accept();
}

bool MyGL::detectCollision()
{
    // If the racquet is rotated, we need to rotate the position of
    // the ball with the opposite, but equal, angle amount w.r.t the
    // racquet's center so we can perform collision detection as if
    // the racquet was not rotated.

    // Compute rotated position of ball
    glm::vec3 ballPos = m_ball.getPosition();
    glm::vec3 racPos = m_racquet.getPosition();
    float rad = glm::radians(m_racquet.getAngle());

    glm::mat3 rotation = glm::mat3({glm::cos(rad), glm::sin(rad), 0.f},
                                   {-glm::sin(rad), glm::cos(rad), 0.f},
                                   {0.f, 0.f, 1.f});
    glm::vec3 rotatedBall = rotation * (ballPos - racPos) + racPos;

    // find the closest point on the racquet to the ball

    float width = m_racquet.getWidth();
    float height = m_racquet.getHeight();
    glm::vec3 closestPoint = glm::vec3();

    if (rotatedBall.x < racPos.x - (width / 2.0))
    {
        closestPoint.x = racPos.x - (width / 2.0);
    }
    else if (rotatedBall.x > racPos.x + (width / 2.0))
    {
        closestPoint.x = racPos.x + (width / 2.0);
    }
    else
    {
        closestPoint.x = rotatedBall.x;
    }

    if (rotatedBall.y < racPos.y - (height / 2.0))
    {
        closestPoint.y = racPos.y - (height / 2.0);
    }
    else if (rotatedBall.y > racPos.y + (height / 2.0))
    {
        closestPoint.y = racPos.y + (height / 2.0);
    }
    else
    {
        closestPoint.y = rotatedBall.y;
    }

    point = closestPoint;

    // check if closestPoint is inside the circle
    glm::vec2 dist = glm::vec2(closestPoint.x - rotatedBall.x,
                               closestPoint.y - rotatedBall.y);

    float len = glm::length(dist);
    float radius = m_ball.getRadius();

    if (len <= radius)
    {
        return true;
    }

    return false;
}
