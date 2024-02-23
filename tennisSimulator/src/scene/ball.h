#ifndef BALL_H
#define BALL_H

#include "glm_includes.h"

class Ball
{
protected:
    glm::vec3 m_pos;
    glm::vec3 m_vel;
    //glm::vec3 m_forces;

    glm::vec3 m_pos0;
    glm::vec3 m_vel0;

    glm::vec3 m_color;
    glm::vec3 m_gravity;

    bool isStopped;

public:
    Ball();
    Ball(glm::vec3 pos0, glm::vec3 vel0, glm::vec3 color);
    Ball(const Ball &ball);
    virtual ~Ball();

    glm::vec3 getPosition();
    void setPosition(glm::vec3 pos);
    void setVelocity(glm::vec3 vel);
    glm::vec3 getColor();
    void setColor(glm::vec3 color);

    // To be called by MyGL::tick()
    void tick(float dT);
    // void addForce(glm::vec3 force);
    // void computeForces();

    void reset();
    void pressedStartStop();
};

#endif // BALL_H
