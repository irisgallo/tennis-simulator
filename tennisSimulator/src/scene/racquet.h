#ifndef RACQUET_H
#define RACQUET_H

#include "glm_includes.h"

class Racquet
{
protected:
    glm::vec3 m_pos;
    glm::vec3 m_color;

    float m_width;
    float m_height;
    float m_deg;

public:
    Racquet();
    Racquet(glm::vec3 pos0, glm::vec3 color);
    Racquet(const Racquet &racquet);
    virtual ~Racquet();

    glm::vec3 getPosition();
    void setPosition(glm::vec3 pos);
    glm::vec3 getColor();
    void setColor(glm::vec3 color);
    float getWidth();
    float getHeight();
    float getAngle();
    void setAngle(float deg);

    glm::mat3 getModelMatrix();

};

#endif // RACQUET_H
