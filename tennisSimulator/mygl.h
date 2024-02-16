#ifndef MYGL_H
#define MYGL_H

#include <QOpenGLWidget>

class MyGL : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();
};

#endif // MYGL_H
