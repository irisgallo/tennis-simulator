#ifndef BALL_H
#define BALL_H

#include "glm_includes.h"

class Ball
{
protected:
    glm::vec3 m_pos;
    glm::vec3 m_vel;

    glm::vec3 m_pos0;
    glm::vec3 m_vel0;

    glm::vec3 m_color;
    glm::vec3 m_gravity;

    float m_radius;

    bool isStopped;

public:
    Ball();
    Ball(glm::vec3 pos0, glm::vec3 vel0, glm::vec3 color);
    Ball(const Ball &ball);
    virtual ~Ball();

    // To be called by MyGL::tick()
    void tick(float dT);

    void reset();
    void pressedStartStop();

    glm::vec3 getPosition();
    void setPosition(glm::vec3 pos);
    glm::vec3 getColor();
    void setColor(glm::vec3 color);
    float getRadius();

    glm::vec3 getInitialPosition();
    void setInitialPosition(glm::vec3 pos0);
    glm::vec3 getInitialVelocity();
    void setInitialVelocity(glm::vec3 vel0);
    glm::mat3 getModelMatrix();
};

#endif // BALL_H
