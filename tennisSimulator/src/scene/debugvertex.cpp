#include "debugvertex.h"

DebugVertex::DebugVertex(OpenGLContext *mp_context)
    : Drawable(mp_context), m_pos(glm::vec3()),
    m_vertPos(), m_vertIdx(), m_numVertices(10)
{
    // Vertex positions
    glm::vec3 p(0.25f, 0.f, 0.5f);

    float deg = glm::radians(360.f / 10);
    for (int i = 0; i < 10; i++)
    {
        glm::mat3 M = glm::rotate(glm::mat3(), i * deg);
        m_vertPos.push_back(M * p);
    }
    // Indices for triangulation
    int n = 8;
    for (int i = 0; i < n; i++)
    {
        m_vertIdx.push_back(0);
        m_vertIdx.push_back(i+1);
        m_vertIdx.push_back(i+2);
    }
}

void DebugVertex::create()
{
    m_count = m_vertIdx.size();
    m_numVertices = m_vertPos.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(GLuint), m_vertIdx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), m_vertPos.data(), GL_STATIC_DRAW);

    m_vertIdx.clear();
    m_vertPos.clear();
}

void DebugVertex::setColor(glm::vec3 c)
{
    if (!bindCol())
    {
        generateCol();
    }
    std::vector<glm::vec3> colors(m_numVertices);
    std::fill(colors.begin(), colors.end(), c);
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
}

glm::mat3 DebugVertex::getModelMatrix()
{
    glm::mat3 mat = glm::mat3({{0.2, 0, 0},
                               {0, 0.2, 0},
                               {0.1 * m_pos.x, 0.1 * m_pos.y, 1}});
    return mat;
}
