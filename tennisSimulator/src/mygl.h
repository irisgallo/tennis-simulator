#ifndef MYGL_H
#define MYGL_H

#include "openglcontext.h"
#include "scene/ball.h"
#include "scene/debugvertex.h"
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

    Ball m_ball;
    Racquet m_racquet;
    Polygon2D m_geomCourt;
    Polygon2D m_geomNet;

    // debugging
    DebugVertex racquetClosestPoint;
    DebugVertex netClosestPoint;
    DebugVertex racquetNormal;
    DebugVertex ballOrientation;

    DebugVertex gravityForce;
    DebugVertex dragForce;
    DebugVertex liftForce;

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.

    QTimer m_timer; // Timer linked to tick(). Fires approximately 60 times per second.
    qint64 prevMSecs;

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void sendSignals(glm::vec3 pos0, glm::vec3 vel0, float angVel0);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void tick(); // Slot that gets called ~60 times per second by m_timer firing.

public slots:
    void slot_setStartStop();
    void slot_reset();
    void slot_setPX(double px);
    void slot_setPY(double py);
    void slot_setVX(double vx);
    void slot_setVY(double vy);
    void slot_setAV(double av);

signals:
    void sig_sendPos(double, double);
    void sig_sendVel(double, double);
    void sig_sendAngVel(double);
    void sig_sendHitVelocity(double, double);
};

#endif // MYGL_H
