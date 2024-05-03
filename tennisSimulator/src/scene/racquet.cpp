#include "racquet.h"

Racquet::Racquet(OpenGLContext* mp_context)
    : Racquet(mp_context, glm::vec3())
{}

Racquet::Racquet(OpenGLContext* mp_context, glm::vec3 pos0)
    : Polygon2D(mp_context, 4), m_pos(pos0), m_vel(glm::vec3()),
      m_accel(glm::vec3()), m_width(4.97), m_height(21.3), m_deg(0.0),
      m_mass(0.3), closestPoint(glm::vec3()), closestNormal(glm::vec3()) {

    // Vertex positions
    glm::vec3 p(0.25f, 0.f, 0.5f);

    float deg = glm::radians(360.f / 4);
    for (int i = 0; i < 4; i++) {
        glm::mat3 M = glm::rotate(glm::mat3(), i * deg);
        m_vertPos.push_back(M * p);
    }
    // Indices for triangulation
    m_vertIdx = { 0, 1, 2, 0, 2, 3 };
}


glm::mat3 Racquet::getModelMatrix() {

    // rotate
    float rad = glm::radians(m_deg);
    float cos = glm::cos(rad);
    float sin = glm::sin(rad);
    glm::vec3 c0 = {cos, sin, 0};
    glm::vec3 c1 = {-sin, cos, 0};
    glm::vec3 c2 = {0, 0, 1};
    glm::mat3 rotate = glm::mat3(c0, c1, c2);

    // translate
    glm::vec3 pos = m_pos;
    pos *= 0.1;
    glm::mat3 translate = glm::mat3({1, 0, 0}, {0, 1, 0}, pos);

    // scale
    glm::mat3 scale = glm::mat3({0.7, 0, 0}, {0, 3.0, 0}, {0, 0, 1});

    // rotate
    rad = glm::radians(45.0);
    cos = glm::cos(rad);
    sin = glm::sin(rad);
    c0 = {cos, sin, 0};
    c1 = {-sin, cos, 0};
    c2 = {0, 0, 1};
    glm::mat3 rotate2 = glm::mat3(c0, c1, c2);

    return translate * rotate * scale * rotate2;
}


void Racquet::rotateCounterclockwise() {

    m_deg += 5.f;
}

void Racquet::rotateClockwise() {

    m_deg -= 5.f;
}
