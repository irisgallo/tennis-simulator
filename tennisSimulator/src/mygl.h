#ifndef MYGL_H
#define MYGL_H

#include <QOpenGLWidget>

class MyGL : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif // MYGL_H
