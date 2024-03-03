#ifndef MYGL_H
#define MYGL_H

#include "openglcontext.h"
#include "scene/ball.h"
#include "scene/racquet.h"
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
    Polygon2D m_geomSquare;

    // // debugging purposes
    Polygon2D m_displayPoint;
    glm::vec3 point;

    Ball m_ball;
    Racquet m_racquet;

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.

    QTimer m_timer; // Timer linked to tick(). Fires approximately 60 times per second.
    qint64 prevMSecs;

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void sendSignals(glm::vec3 pos0, glm::vec3 vel0);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool detectCollision();

private slots:
    void tick(); // Slot that gets called ~60 times per second by m_timer firing.

public slots:
    void slot_setStartStop();
    void slot_reset();
    void slot_setPX(double px);
    void slot_setPY(double py);
    void slot_setVX(double vx);
    void slot_setVY(double vy);

signals:
    void sig_sendPos(double, double);
    void sig_sendVel(double, double);
};

#endif // MYGL_H
