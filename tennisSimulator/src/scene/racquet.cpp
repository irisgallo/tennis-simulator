#include "racquet.h"

Racquet::Racquet() : Racquet(glm::vec3(), glm::vec3())
{}

Racquet::Racquet(glm::vec3 pos0, glm::vec3 color)
    : m_pos(pos0), m_color(color), m_width(7.1),
      m_height(28.4), m_deg(0.0)
{}

Racquet::Racquet(const Racquet &racquet)
    : m_pos(racquet.m_pos), m_color(racquet.m_color),
      m_width(racquet.m_width), m_height(racquet.m_height),
      m_deg(racquet.m_deg)
{}

Racquet::~Racquet()
{}

glm::vec3 Racquet::getPosition()
{
    return m_pos;
}

void Racquet::setPosition(glm::vec3 pos)
{
    m_pos = pos;
}

glm::vec3 Racquet::getColor()
{
    return m_color;
}

void Racquet::setColor(glm::vec3 color)
{
    m_color = color;
}

float Racquet::getWidth()
{
    return m_width;
}

float Racquet::getHeight()
{
    return m_height;
}

float Racquet::getAngle()
{
    return m_deg;
}

void Racquet::setAngle(float deg)
{
    m_deg = deg;
}

glm::mat3 Racquet::getModelMatrix()
{
    // translate
    glm::vec3 pos = getPosition();
    pos *= 0.1;
    glm::mat3 mat0 = glm::mat3({1, 0, 0}, {0, 1, 0}, pos);

    // rotate
    float rad = glm::radians(45.0);
    float cos = glm::cos(rad);
    float sin = glm::sin(rad);
    glm::vec3 c0 = {cos, sin, 0};
    glm::vec3 c1 = {-sin, cos, 0};
    glm::vec3 c2 = {0, 0, 1};
    glm::mat3 mat1 = glm::mat3(c0, c1, c2);

    // scale
    glm::mat3 mat2 = glm::mat3({1, 0, 0}, {0, 4.0, 0}, {0, 0, 1});

    return mat0 * mat2 * mat1;
}
