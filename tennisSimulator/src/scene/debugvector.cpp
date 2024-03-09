#include "debugvector.h"

DebugVector::DebugVector(OpenGLContext *mp_context)
    : Drawable(mp_context), m_pt1(glm::vec3(0, 0, 0)),
      m_pt2(glm::vec3(1, 0, 0))
{}

void DebugVector::create() {

    this->setMode(GL_LINES);

    std::vector<glm::vec3> vertPos {m_pt1, m_pt2};
    std::vector<glm::vec3> vertColor {glm::vec3(1, 0, 0),
                                       glm::vec3(0, 0, 1)};
    std::vector<GLuint> vertIdx {0, 1};

    m_count = vertIdx.size();

    generateIdx();
    bindIdx();
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertIdx.size() * sizeof(GLuint), vertIdx.data(), GL_STATIC_DRAW);

    generatePos();
    bindPos();
    mp_context->glBufferData(GL_ARRAY_BUFFER, vertPos.size() * sizeof(glm::vec3), vertPos.data(), GL_STATIC_DRAW);

    generateCol();
    bindCol();
    mp_context->glBufferData(GL_ARRAY_BUFFER, vertColor.size() * sizeof(glm::vec3), vertColor.data(), GL_STATIC_DRAW);

    vertIdx.clear();
    vertPos.clear();
    vertColor.clear();
}

glm::mat3 DebugVector::getModelMatrix() {

    glm::vec3 pos = m_pt2 - m_pt1;
    glm::mat3 mat = glm::mat3({{0.05, 0, 0},
                               {0, 0.05, 0},
                               {0.25 * pos.x, 0.25 * pos.y, 1}});
    return mat;
}
