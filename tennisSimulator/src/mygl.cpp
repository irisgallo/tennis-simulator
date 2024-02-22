#include "mygl.h"
#include <glm_includes.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>


MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent), prog_flat(this), m_geomCircle(this, 20)
{
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
    glClearColor(0, 0, 1, 1);

    prog_flat.setModelMatrix(glm::mat3());
    m_geomCircle.setColor(glm::vec3(0,1,0));
    prog_flat.draw(*this, m_geomCircle);
}
