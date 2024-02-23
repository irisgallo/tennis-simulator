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

    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

    // initalize our tennis ball
    m_ball = Ball(glm::vec3(-40, 0, 0), glm::vec3(15, 30, 0),
                  glm::vec3(0.84f, 1.0f, 0.15f));

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

    glm::vec3 c0 = {1, 0, 0};
    glm::vec3 c1 = {0, 1, 0};
    glm::vec3 c2 = m_ball.getPosition();
    glm::mat3 mat = glm::mat3(c0, c1, c2);

    m_geomCircle.setColor(m_ball.getColor());
    prog_flat.setModelMatrix(mat);
    prog_flat.draw(*this, m_geomCircle);
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
