#include "ball.h"
#include <iostream>

#ifndef GRAVITY
#define GRAVITY 9.8f
#endif

Ball::Ball() : Ball(glm::vec3(), glm::vec3(), glm::vec3(0, 1, 0))
{}

Ball::Ball(glm::vec3 pos0, glm::vec3 vel0, glm::vec3 color)
    : m_pos(pos0), m_vel(vel0), m_pos0(pos0), m_vel0(vel0),
      m_color(color), m_gravity(glm::vec3(0.0, -GRAVITY, 0.0)),
      isStopped(true)
{}

Ball::Ball(const Ball &ball)
    : m_pos(ball.m_pos), m_vel(ball.m_vel), m_pos0(ball.m_pos0),
      m_vel0(ball.m_vel0), m_color(ball.m_color),
      m_gravity(ball.m_gravity), isStopped(ball.isStopped)
{}

Ball::~Ball()
{}

glm::vec3 Ball::getPosition()
{
    glm::vec3 screenPos = glm::vec3(0.1 * m_pos[0],
                                    0.1 * m_pos[1],
                                    0.1 * m_pos[2]);
    return screenPos;
}

void Ball::setPosition(glm::vec3 pos)
{
    m_pos = pos;
}

glm::vec3 Ball::getColor()
{
    return m_color;
}

void Ball::setColor(glm::vec3 color)
{
    m_color = color;
}

void Ball::tick(float dT)
{
    if (isStopped)
    {
        return;
    }
    // compute physics
    if (dT >= 1000.f)
    {
        // If the window is minimized, we can end up with huge dT values between ticks,
        // leading to massive acceleration. This should avoid that
        return;
    }

    float scaledTime = 0.003 * dT;

    m_pos += scaledTime * m_vel;
    m_vel += scaledTime * m_gravity;
}

void Ball::reset()
{
    m_pos = m_pos0;
    m_vel = m_vel0;
    isStopped = true;
}

void Ball::pressedStartStop()
{
    isStopped = !isStopped;
}

glm::vec3 Ball::getInitialPosition()
{
    return m_pos0;
}

void Ball::setInitialPosition(glm::vec3 pos0)
{
    m_pos0 = pos0;
    reset();
}

glm::vec3 Ball::getInitialVelocity()
{
    return m_vel0;
}

void Ball::setInitialVelocity(glm::vec3 vel0)
{
    m_vel0 = vel0;
    reset();
}

// void Ball::addForce(glm::vec3 force)
// {
//     m_forces = force;
// }

// void Ball::computeForces()
// {
//     // zero out forces
//     m_forces = glm::vec3();

//     // default is gravity force
//     addForce(m_mass * m_gravity);
// }

