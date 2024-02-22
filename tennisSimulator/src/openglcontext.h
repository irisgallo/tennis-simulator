// taken from CIS 4600 Mini Minecraft source code

#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#include <QOpenGLWidget>
#include <QTimer>
#include <QOpenGLExtraFunctions>

class OpenGLContext
    : public QOpenGLWidget,
      public QOpenGLExtraFunctions
{

public:
    OpenGLContext(QWidget *parent);
    ~OpenGLContext();

    void debugContextVersion();
    void printGLErrorLog();
    void printLinkInfoLog(int prog);
    void printShaderInfoLog(int shader);
};

#endif // OPENGLCONTEXT_H
