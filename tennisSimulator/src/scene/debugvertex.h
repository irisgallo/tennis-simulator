#ifndef DEBUGVERTEX_H
#define DEBUGVERTEX_H

#include "drawable.h"

class DebugVertex : public Drawable
{
protected:
    glm::vec3 m_pos;

private:
    std::vector<glm::vec3> m_vertPos;
    std::vector<GLuint> m_vertIdx;
    unsigned int m_numVertices;

public:
    DebugVertex(OpenGLContext *mp_context);

    void create() override;
    void setColor(glm::vec3 c);

    glm::mat3 getModelMatrix();

    friend class MyGL;
};

#endif // DEBUGVERTEX_H
