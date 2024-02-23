#include "ball.h"

Ball::Ball() : Ball(glm::vec3(), glm::vec3(0, 1, 0))
{}

Ball::Ball(glm::vec3 pos, glm::vec3 color)
    : m_pos(pos), m_color(color)
{}

Ball::Ball(const Ball &ball)
    : m_pos(ball.m_pos), m_color(ball.m_color)
{}

Ball::~Ball()
{}

glm::vec3 Ball::getPosition()
{
    return m_pos;
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

    moveRight(0.1);
}

void Ball::moveRight(float amount)
{
    m_pos += glm::vec3(amount, 0, 0);
}
