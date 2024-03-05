#include "ball.h"
#include <iostream>

#ifndef GRAVITY
#define GRAVITY 9.8f
#endif

Ball::Ball(OpenGLContext* mp_context)
    : Ball(mp_context, glm::vec3(), glm::vec3())
{}

Ball::Ball(OpenGLContext* mp_context, glm::vec3 pos0, glm::vec3 vel0)
    : Polygon2D(mp_context, 20), m_pos(pos0), m_vel(vel0),
      m_pos0(pos0), m_vel0(vel0),
      m_gravity(glm::vec3(0.0, -GRAVITY, 0.0)),
      m_radius(3.5), isStopped(true), racquetPoint(glm::vec3()),
      netPoint(glm::vec3()), racquet(nullptr)
{}

void Ball::tick(float dT)
{
    detectRacquetCollision();

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
        setColor(glm::vec3(0, 1, 0));
        m_vel.x *= -1;
        m_pos += scaledTime * m_vel;
        return;
    }

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

glm::mat3 Ball::getBallModelMatrix()
{
    // translate
    glm::vec3 pos = m_pos;
    pos *= 0.1;
    glm::mat3 translate = glm::mat3({1, 0, 0}, {0, 1, 0}, pos);

    // scale
    glm::mat3 scale = glm::mat3({0.7, 0, 0}, {0, 0.7, 0}, {0, 0, 1});

    return translate * scale;
}

glm::mat3 Ball::getCourtModelMatrix()
{
    // translate
    glm::vec3 pos = glm::vec3(0.f, -110.f, 0.f);
    pos *= 0.1;
    glm::mat3 translate = glm::mat3({1, 0, 0}, {0, 1, 0}, pos);

    // scale
    glm::mat3 scale = glm::mat3({57.0, 0, 0}, {0, 8.0, 0}, {0, 0, 1});

    // rotate
    float rad = glm::radians(45.0);
    float cos = glm::cos(rad);
    float sin = glm::sin(rad);
    glm::vec3 c0 = {cos, sin, 0};
    glm::vec3 c1 = {-sin, cos, 0};
    glm::vec3 c2 = {0, 0, 1};
    glm::mat3 rotate = glm::mat3(c0, c1, c2);

    return translate * scale * rotate;
}

glm::mat3 Ball::getNetModelMatrix()
{
    // translate
    glm::vec3 pos = glm::vec3(0.f, -68.f, 0.f);
    pos *= 0.1;
    glm::mat3 translate = glm::mat3({1, 0, 0}, {0, 1, 0}, pos);

    // scale
    glm::mat3 scale = glm::mat3({1.0, 0, 0}, {0, 4.0, 0}, {0, 0, 1});

    // rotate
    float rad = glm::radians(45.0);
    float cos = glm::cos(rad);
    float sin = glm::sin(rad);
    glm::vec3 c0 = {cos, sin, 0};
    glm::vec3 c1 = {-sin, cos, 0};
    glm::vec3 c2 = {0, 0, 1};
    glm::mat3 rotate = glm::mat3(c0, c1, c2);

    return translate * scale * rotate;
}


bool Ball::detectRacquetCollision()
{
    // If the racquet is rotated, we need to rotate the position of
    // the ball with the opposite, but equal, angle amount w.r.t the
    // racquet's center so we can perform collision detection as if
    // the racquet was not rotated.

    // Compute rotated position of ball
    glm::vec3 ballPos = m_pos;
    glm::vec3 racPos = racquet->m_pos;
    float deg = racquet->m_deg;
    float rad = glm::radians(deg);

    glm::mat3 rotation = glm::mat3({glm::cos(rad), glm::sin(rad), 0.f},
                                   {-glm::sin(rad), glm::cos(rad), 0.f},
                                   {0.f, 0.f, 1.f});
    glm::vec3 rotatedBall = rotation * (ballPos - racPos) + racPos;

    // find the closest point on the racquet to the ball

    float width = racquet->m_width;
    float height = racquet->m_height;
    glm::vec3 closestPoint = glm::vec3();

    if (rotatedBall.x < racPos.x - (width / 2.0))
    {
        closestPoint.x = racPos.x - (width / 2.0);
    }
    else if (rotatedBall.x > racPos.x + (width / 2.0))
    {
        closestPoint.x = racPos.x + (width / 2.0);
    }
    else
    {
        closestPoint.x = rotatedBall.x;
    }

    if (rotatedBall.y < racPos.y - (height / 2.0))
    {
        closestPoint.y = racPos.y - (height / 2.0);
    }
    else if (rotatedBall.y > racPos.y + (height / 2.0))
    {
        closestPoint.y = racPos.y + (height / 2.0);
    }
    else
    {
        closestPoint.y = rotatedBall.y;
    }

    racquetPoint = closestPoint;

    // check if closestPoint is inside the circle
    glm::vec2 dist = glm::vec2(closestPoint.x - rotatedBall.x,
                               closestPoint.y - rotatedBall.y);

    float len = glm::length(dist);
    float radius = m_radius;

    if (len <= radius)
    {
        setColor(glm::vec3(0.969, 0.512, 0.473));
        return true;
    }

    setColor(glm::vec3(0.84f, 1.0f, 0.15f));
    return false;
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


