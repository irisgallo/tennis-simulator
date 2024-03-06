#ifndef RACQUET_H
#define RACQUET_H

#include "glm_includes.h"
#include "polygon.h"

class Racquet : public Polygon2D
{
protected:
    glm::vec3 m_pos;
    glm::vec3 m_vel;

    float m_width;
    float m_height;
    float m_deg;

    glm::vec3 closestPoint;
    glm::vec3 closestNormal; // the normal of the racquet at the closestPoint



public:
    Racquet(OpenGLContext* mp_context);
    Racquet(OpenGLContext* mp_context, glm::vec3 pos0);

    glm::mat3 getModelMatrix();
    void rotateCounterclockwise();
    void rotateClockwise();

    friend class MyGL;
    friend class Ball;
};

#endif // RACQUET_H
