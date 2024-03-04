#include "ball.h"
#include <iostream>

#ifndef GRAVITY
#define GRAVITY 9.8f
#endif

Ball::Ball() : Ball(glm::vec3(), glm::vec3(), glm::vec3(0))
{}

Ball::Ball(glm::vec3 pos0, glm::vec3 vel0, glm::vec3 color)
    : m_pos(pos0), m_vel(vel0), m_pos0(pos0), m_vel0(vel0),
      m_color(color), m_gravity(glm::vec3(0.0, -GRAVITY, 0.0)),
      m_radius(3.5), isStopped(true), netPoint(glm::vec3())
{}

Ball::Ball(const Ball &ball)
    : m_pos(ball.m_pos), m_vel(ball.m_vel), m_pos0(ball.m_pos0),
      m_vel0(ball.m_vel0), m_color(ball.m_color),
      m_gravity(ball.m_gravity), m_radius(ball.m_radius),
      isStopped(ball.isStopped), netPoint(ball.netPoint)
{}

Ball::~Ball()
{}

void Ball::tick(float dT)
{
    if (isStopped)
    {
        return;
    }
    // compute physics
    if (dT >= 1000.f)
    {
        // avoids huge dT values between ticks
        return;
    }

    float scaledTime = 0.003 * dT;

    // compute intersection with tennis court (which lies at y = -81.1)
    // these are placeholders for bounce and and a right boundary for now
    if (m_pos.y - m_radius <= -81.1)
    {
        m_vel.y *= -1;
        m_pos += scaledTime * m_vel;
        return;
    }
    if (m_pos.x + m_radius >= 199.0)
    {
        m_vel.x *= -1;
        m_pos += scaledTime * m_vel;
        return;
    }
    // detect net collision
    if (detectNetCollision())
    {
        m_color = glm::vec3(0, 1, 0);
        m_vel.x *= -1;
        m_pos += scaledTime * m_vel;
        return;
    }

    m_color = glm::vec3(0.84f, 1.0f, 0.15f);
    m_pos += scaledTime * m_vel;
    m_vel += scaledTime * m_gravity;
}

bool Ball::detectNetCollision()
{
    // find the closest point on the racquet to the ball

    float width = 7.1;
    float height = 28.4;
    glm::vec3 netPos = glm::vec3(0.f, -68.0, 0.f);
    glm::vec3 closestPoint = glm::vec3();

    if (m_pos.x < netPos.x - (width / 2.0))
    {
        closestPoint.x = netPos.x - (width / 2.0);
    }
    else if (m_pos.x > netPos.x + (width / 2.0))
    {
        closestPoint.x = netPos.x + (width / 2.0);
    }
    else
    {
        closestPoint.x = m_pos.x;
    }

    if (m_pos.y < netPos.y - (height / 2.0))
    {
        closestPoint.y = netPos.y - (height / 2.0);
    }
    else if (m_pos.y > netPos.y + (height / 2.0))
    {
        closestPoint.y = netPos.y + (height / 2.0);
    }
    else
    {
        closestPoint.y = m_pos.y;
    }

    netPoint = closestPoint;

    // check if closestPoint is inside the circle
    glm::vec2 dist = glm::vec2(closestPoint.x - m_pos.x,
                               closestPoint.y - m_pos.y);

    float len = glm::length(dist);

    if (len <= m_radius)
    {
        return true;
    }

    return false;
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

float Ball::getRadius()
{
    return m_radius;
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

glm::mat3 Ball::getModelMatrix()
{
    // translate
    glm::vec3 pos = getPosition();
    pos *= 0.1;
    glm::mat3 translate = glm::mat3({1, 0, 0}, {0, 1, 0}, pos);

    // scale
    glm::mat3 scale = glm::mat3({0.7, 0, 0}, {0, 0.7, 0}, {0, 0, 1});

    return translate * scale;
}

glm::vec3 Ball::getNetPoint()
{
    return netPoint;
}

