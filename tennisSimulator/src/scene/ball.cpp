#include "ball.h"

Ball::Ball() : Ball(glm::vec3(), nullptr)
{}

Ball::Ball(glm::vec3 pos, Polygon2D* p)
    : m_pos(pos), m_geometry(p)
{}

Ball::Ball(const Ball &b)
    : m_pos(b.m_pos), m_geometry(b.m_geometry)
{}

Ball::~Ball()
{}

void tick(float dT)
{
    // TODO
    return;
}

void moveRightLocal(float amount)
{
    // TODO
    return;
}

void moveRightGlobal(float amount)
{
    // TODO
    return;
}
