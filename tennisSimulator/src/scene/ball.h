#ifndef BALL_H
#define BALL_H

#include "glm_includes.h"
#include "polygon.h"
#include "racquet.h"

class Ball : public Polygon2D
{
protected:
    glm::vec3 m_pos;
    glm::vec3 m_vel;

    glm::vec3 m_pos0;
    glm::vec3 m_vel0;

    float m_deg; // always starts at zero degrees
    float m_angVel; // probably need to use quaternions later
                    // for now, we're assuming rotation about z-axis
    float m_angVel0;


    glm::vec3 m_gravity;
    float m_radius;
    float m_mass;

    bool isStopped;
    bool hasCollision;

    // for debugging
    glm::vec3 netPoint;

    Racquet* racquet;
    glm::vec2 m_hitVelocity;

private:
    bool detectRacquetCollision();
    bool detectNetCollision();

    void hitBall();

    glm::mat3 generateRotationMatrix(float rad);

public:
    Ball(OpenGLContext* mp_context);
    Ball(OpenGLContext* mp_context, glm::vec3 pos0,
         glm::vec3 vel0, float angVel0);

    // To be called by MyGL::tick()
    void tick(float dT);

    void reset();
    void pressedStartStop();

    glm::mat3 getBallModelMatrix();
    glm::mat3 getCourtModelMatrix();
    glm::mat3 getNetModelMatrix();
    glm::vec3 getOrientation();

signals:
    void sig_sendHitVelocity(double, double);

    friend class MyGL;
};

#endif // BALL_H
