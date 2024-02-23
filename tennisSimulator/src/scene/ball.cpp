#include "ball.h"
#include <iostream>

#ifndef GRAVITY
#define GRAVITY 9.8f
#endif

Ball::Ball() : Ball(glm::vec3(), glm::vec3(), glm::vec3(0, 1, 0))
{}

Ball::Ball(glm::vec3 pos, glm::vec3 vel0, glm::vec3 color)
    : m_pos(pos), m_vel(vel0), m_mass(1.0f),
      m_color(color), m_gravity(glm::vec3(0.0, -GRAVITY, 0.0))
{}

Ball::Ball(const Ball &ball)
    : m_pos(ball.m_pos), m_vel(ball.m_vel), m_mass(ball.m_mass),
      m_color(ball.m_color), m_gravity(ball.m_gravity)
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
    // compute physics
    if (dT >= 1000.f) {
        // If the window is minimized, we can end up with huge dT values between ticks,
        // leading to massive acceleration. This should avoid that
        return;
    }

    float scaledTime = 0.003 * dT;

    m_pos += scaledTime * m_vel;
    m_vel += scaledTime * m_gravity;
}

void Ball::moveRight(float amount)
{
    m_pos += glm::vec3(amount, 0, 0);
}

void Ball::moveDown(float amount)
{
    m_pos += glm::vec3(0, amount, 0);
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

