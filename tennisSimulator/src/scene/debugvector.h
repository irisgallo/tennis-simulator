#ifndef DEBUGVECTOR_H
#define DEBUGVECTOR_H

#include "drawable.h"

class DebugVector : public Drawable
{
protected:
    glm::vec3 m_pt1;
    glm::vec3 m_pt2;

private:
    std::vector<glm::vec3> m_vertPos;
    std::vector<GLuint> m_vertIdx;

public:
    DebugVector(OpenGLContext *mp_context);

    void create() override;
    void setColor(glm::vec3 c);
    glm::mat3 getModelMatrix();

    friend class MyGL;
};

#endif // DEBUGVECTOR_H
