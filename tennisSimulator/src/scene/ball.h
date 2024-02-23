#ifndef BALL_H
#define BALL_H

#include "glm_includes.h"
#include "polygon.h"

class Ball
{
protected:
    glm::vec3 m_pos;
    Polygon2D* m_geometry;

public:
    Ball();
    Ball(glm::vec3 pos, Polygon2D* p);
    Ball(const Ball &b);
    virtual ~Ball();

    // To be called by MyGL::tick()
    void tick(float dT);

    void moveRightLocal(float amount);
    void moveRightGlobal(float amount);

};

#endif // BALL_H
