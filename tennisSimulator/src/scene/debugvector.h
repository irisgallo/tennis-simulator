#ifndef DEBUGVECTOR_H
#define DEBUGVECTOR_H

#include "drawable.h"

class DebugVector : public Drawable
{
protected:
    glm::vec3 m_pt1;
    glm::vec3 m_pt2;

public:
    DebugVector(OpenGLContext *mp_context);

    void create() override;
    glm::mat3 getModelMatrix();

    friend class MyGL;
};

#endif // DEBUGVECTOR_H
