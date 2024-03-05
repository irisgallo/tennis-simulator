#ifndef RACQUET_H
#define RACQUET_H

#include "glm_includes.h"
#include "polygon.h"

class Racquet : public Polygon2D
{
protected:
    glm::vec3 m_pos;
    float m_width;
    float m_height;
    float m_deg;

public:
    Racquet(OpenGLContext* mp_context);
    Racquet(OpenGLContext* mp_context, glm::vec3 pos0);

    glm::mat3 getModelMatrix();

    friend class MyGL;
};

#endif // RACQUET_H
