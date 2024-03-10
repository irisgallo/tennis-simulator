#include "debugvector.h"

DebugVector::DebugVector(OpenGLContext *mp_context)
    : Drawable(mp_context), m_pt1(glm::vec3(0, 0, 0)),
    m_pt2(glm::vec3(1, 0, 0))
{}

void DebugVector::create() {

    this->setMode(GL_LINES);

    m_vertPos = {m_pt1, m_pt2};
    m_vertIdx = {0, 1};

    m_count = 2;

    generateIdx();
    bindIdx();
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vertIdx.size() * sizeof(GLuint), m_vertIdx.data(), GL_STATIC_DRAW);

    generatePos();
    bindPos();
    mp_context->glBufferData(GL_ARRAY_BUFFER, m_vertPos.size() * sizeof(glm::vec3), m_vertPos.data(), GL_STATIC_DRAW);

    m_vertIdx.clear();
    m_vertPos.clear();
}

void DebugVector::setColor(glm::vec3 c) {

    if (!bindCol()) {
        generateCol();
    }
    std::vector<glm::vec3> colors(2);
    std::fill(colors.begin(), colors.end(), c);
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
}

glm::mat3 DebugVector::getModelMatrix() {

    glm::vec3 pos = m_pt2 - m_pt1;
    glm::mat3 mat = glm::mat3({{0.05, 0, 0},
                               {0, 0.05, 0},
                               {0.25 * pos.x, 0.25 * pos.y, 1}});
    return mat;
}
