#ifndef MYGL_H
#define MYGL_H

#include "openglcontext.h"
#include "scene/ball.h"
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
    Ball m_ball;

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.

    QTimer m_timer; // Timer linked to tick(). Fires approximately 60 times per second.
    qint64 prevMSecs;

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private slots:
    void tick(); // Slot that gets called ~60 times per second by m_timer firing.

public slots:
    void slot_setStartStop();
};

#endif // MYGL_H
