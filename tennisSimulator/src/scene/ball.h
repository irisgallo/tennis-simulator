#ifndef BALL_H
#define BALL_H

#include "glm_includes.h"

class Ball
{
protected:
    glm::vec3 m_pos;
    glm::vec3 m_vel;
    glm::vec3 m_forces;

    float m_mass;
    glm::vec3 m_color;
    glm::vec3 m_gravity;

public:
    Ball();
    Ball(glm::vec3 pos, glm::vec3 vel0, glm::vec3 color);
    Ball(const Ball &ball);
    virtual ~Ball();

    glm::vec3 getPosition();
    void setPosition(glm::vec3 pos);
    glm::vec3 getColor();
    void setColor(glm::vec3 color);

    // To be called by MyGL::tick()
    void tick(float dT);
    void moveRight(float amount);
    void moveDown(float amount);
    // void addForce(glm::vec3 force);
    // void computeForces();
};

#endif // BALL_H
