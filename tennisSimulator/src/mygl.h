#ifndef MYGL_H
#define MYGL_H

#include "openglcontext.h"
#include <shaderprogram.h>
#include <scene/polygon.h>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

class MyGL : public OpenGLContext
{
    Q_OBJECT

private:
    ShaderProgram prog_flat;// A shader program that uses "flat" reflection (no shadowing at all)

    Polygon2D m_geomCircle;

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif // MYGL_H
