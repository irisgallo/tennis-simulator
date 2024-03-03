#ifndef RACQUET_H
#define RACQUET_H

#include "glm_includes.h"

class Racquet
{
protected:
    glm::vec3 m_pos;
    glm::vec3 m_color;

public:
    Racquet();
    Racquet(glm::vec3 pos0, glm::vec3 color);
    Racquet(const Racquet &racquet);
    virtual ~Racquet();

    glm::vec3 getPosition();
    void setPosition(glm::vec3 pos);
    glm::vec3 getColor();
    void setColor(glm::vec3 color);

    glm::mat3 getModelMatrix();
};

#endif // RACQUET_H
