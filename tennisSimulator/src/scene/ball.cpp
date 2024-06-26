#include "ball.h"
#include <iostream>

#ifndef GRAVITY
#define GRAVITY 9.8f
#endif

#ifndef PI
#define PI 3.14159265f
#endif

// density of air
#ifndef RHO
#define RHO 1.293
#endif

Ball::Ball(OpenGLContext* mp_context)
    : Ball(mp_context, glm::vec3(), glm::vec3(), 0.f)
{}


Ball::Ball(OpenGLContext* mp_context, glm::vec3 pos0,
           glm::vec3 vel0, float angVel0)
    : Polygon2D(mp_context, 20), m_pos(pos0), m_vel(glm::vec3()),
      m_pos0(pos0), m_vel0(vel0), m_deg(0.f), m_angVel(angVel0),
      m_angVel0(angVel0), m_radius(3.5), m_mass(0.057), f_gravity(),
      f_drag(), f_lift(), f_total(), isStopped(true),
      hasCollision(false), netPoint(glm::vec3()), racquet(nullptr),
      m_hitVelocity(glm::vec3())
{}


void Ball::tick(float dT) {

    float scaledTime = 0.003 * dT;

    if (detectRacquetCollision() && !hasCollision) {
        hitBall();
        hasCollision = true;

        // update ang velocity
        // Compute torque from racquet on ball
        glm::vec3 racquetForce = racquet->m_mass * racquet->m_accel;
        glm::vec3 torque = glm::cross(racquetForce, racquet->closestNormal);
        float momentOfInertia = (2.0/3.0) * m_mass * m_radius * m_radius;
        glm::vec3 anglVelocity  = torque / (momentOfInertia * scaledTime);
        m_angVel = anglVelocity[2];
        if (m_angVel > 1000.0) {
            m_angVel = 1000.0;
        } else if (m_angVel < -1000.0) {
            m_angVel = -1000.0;
        }
    }

    if (isStopped) {
        return;
    }
    // compute physics
    if (dT >= 1000.f) {
        // avoids huge dT values between ticks
        return;
    }

    // compute bounce, aka the intersection with tennis court (which lies at y = -81)
    // these are placeholders for bounce and and screen boundaries for now
    if (m_pos.y - m_radius <= -81.0 && m_pos.y - m_radius >= 80.0) {
        m_vel.y = 0;
        m_pos += scaledTime * m_vel;
        hasCollision = false;
        m_angVel *= 1;
    } else if (m_pos.y - m_radius <= -81 || m_pos.y - m_radius >= 133.33) {
        m_vel.y *= -1;
        m_pos += scaledTime * m_vel;
        hasCollision = false;

        // A tennis ball always has topsin after it bounces.
        // So if the ball is moving to the right, we want counterclockwise spin
        // If the ball is moving to the left, we want clockwise spin
        if ((m_vel.x > 0 && m_angVel > 0) || (m_vel.x < 0 && m_angVel < 0)) {
            m_angVel = -m_angVel;
        }
        m_vel.y *= 0.8;

        return;
    }
    if (m_pos.x + m_radius >= 199.0 || m_pos.x - m_radius <= -199.0) {
        m_vel.x *= -1;
        m_pos += scaledTime * m_vel;
        hasCollision = false;
        return;
    }

    // detect net collision
    if (detectNetCollision()) {
        if (m_pos.x == netPoint.x) {
            m_vel.y *= -1;
        }
        if (m_pos.y == netPoint.y) {
            m_vel.x *= -1;
        }

        m_pos += scaledTime * m_vel;
        hasCollision = false;
        return;
    }

    computeForces();

    m_pos += scaledTime * m_vel;
    m_vel += scaledTime * (f_total / (m_mass * 100));
    m_deg += scaledTime * m_angVel;
    if (m_deg >= 360.0) {
        m_deg -= 360.0;
    }

    racquet->m_accel = dT * racquet->m_vel;
    racquet->m_accel *= 0.08;
}


void Ball::reset() {

    m_pos = m_pos0;
    m_vel = m_vel0;
    m_deg = 0.f;
    m_angVel = m_angVel0;
    isStopped = true;
    hasCollision = false;
}


void Ball::pressedStartStop() {

    isStopped = !isStopped;
}


glm::mat3 Ball::getBallModelMatrix() {

    // translate
    glm::vec3 pos = m_pos;
    pos *= 0.1;
    glm::mat3 translate = glm::mat3({1, 0, 0}, {0, 1, 0}, pos);

    // scale
    glm::mat3 scale = glm::mat3({0.7, 0, 0}, {0, 0.7, 0}, {0, 0, 1});

    return translate * scale;
}


glm::mat3 Ball::getCourtModelMatrix() {

    // translate
    glm::vec3 pos = glm::vec3(0.f, -110.f, 0.f);
    pos *= 0.1;
    glm::mat3 translate = glm::mat3({1, 0, 0}, {0, 1, 0}, pos);

    // scale
    glm::mat3 scale = glm::mat3({57.0, 0, 0}, {0, 8.0, 0}, {0, 0, 1});

    // rotate
    float rad = glm::radians(45.0);
    glm::mat3 rotate = generateRotationMatrix(rad);

    return translate * scale * rotate;
}


glm::mat3 Ball::getNetModelMatrix() {

    // translate
    glm::vec3 pos = glm::vec3(0.f, -68.f, 0.f);
    pos *= 0.1;
    glm::mat3 translate = glm::mat3({1, 0, 0}, {0, 1, 0}, pos);

    // scale
    glm::mat3 scale = glm::mat3({1.0, 0, 0}, {0, 4.0, 0}, {0, 0, 1});

    // rotate
    float rad = glm::radians(45.0);
    glm::mat3 rotate = generateRotationMatrix(rad);

    return translate * scale * rotate;
}


glm::vec3 Ball::getOrientation() {

    float rad = glm::radians(m_deg);
    return glm::vec3(glm::cos(rad), glm::sin(rad), 0.f);
}


glm::mat3 Ball::generateRotationMatrix(float rad)
{
    return glm::mat3({glm::cos(rad), glm::sin(rad), 0.f},
                     {-glm::sin(rad), glm::cos(rad), 0.f},
                     {0.f, 0.f, 1.f});
}


bool Ball::detectRacquetCollision() {

    // If the racquet is rotated, we need to rotate the position of
    // the ball with the opposite, but equal, angle amount w.r.t the
    // racquet's center so we can perform collision detection as if
    // the racquet was not rotated.

    // Compute rotated position of ball
    glm::vec3 ballPos = m_pos;
    glm::vec3 racPos = racquet->m_pos;
    float deg = racquet->m_deg;
    float rad = glm::radians(deg);
    glm::mat3 rotation = generateRotationMatrix(-rad);
    glm::vec3 rotatedBall = rotation * (ballPos - racPos) + racPos;

    // find the closest point on the racquet to the ball
    float width = racquet->m_width;
    float height = racquet->m_height;
    glm::vec3 closestPoint = glm::vec3();
    int normalDeg = -1;

    // find x coordinate
    if (rotatedBall.x < racPos.x - (width / 2.0)) {
        closestPoint.x = racPos.x - (width / 2.0);
        normalDeg = 180;
    } else if (rotatedBall.x > racPos.x + (width / 2.0)) {
        closestPoint.x = racPos.x + (width / 2.0);
        normalDeg = 0;
    } else {
        closestPoint.x = rotatedBall.x;
        normalDeg = -1;
    }

    // find y coordinate
    if (rotatedBall.y < racPos.y - (height / 2.0)) {
        closestPoint.y = racPos.y - (height / 2.0);
        switch (normalDeg) {
            case 0:
                normalDeg = 315;
                break;
            case 180:
                normalDeg = 225;
                break;
            default:
                normalDeg = 270;
        }
    } else if (rotatedBall.y > racPos.y + (height / 2.0)) {
        closestPoint.y = racPos.y + (height / 2.0);
        switch (normalDeg) {
        case 0:
            normalDeg = 45;
            break;
        case 180:
            normalDeg = 135;
            break;
        default:
            normalDeg = 90;
        }
    } else {
        closestPoint.y = rotatedBall.y;
        if (normalDeg == -1) {
            normalDeg = 0;
        }
    }

    // rotate back closestPoint
    rotation = generateRotationMatrix(rad);
    racquet->closestPoint = rotation * (closestPoint - racPos) + racPos;

    // rotate back closestNormal
    normalDeg += deg;
    float nRad = glm::radians(normalDeg * 1.0);
    racquet->closestNormal = glm::vec3(glm::cos(nRad), glm::sin(nRad), 1);

    // check if closestPoint is inside the circle
    glm::vec2 dist = glm::vec2(racquet->closestPoint.x - m_pos.x,
                               racquet->closestPoint.y - m_pos.y);

    float len = glm::length(dist);
    if (len <= m_radius) {
        setColor(glm::vec3(0.594, 1.0, 0.594));
        return true;
    }

    setColor(glm::vec3(0.84f, 1.0f, 0.15f));
    return false;
}


bool Ball::detectNetCollision() {

    // find the closest point on the net to the ball
    float width = 7.1;
    float height = 28.4;
    glm::vec3 netPos = glm::vec3(0.f, -68.0, 0.f);
    glm::vec3 closestPoint = glm::vec3();

    // find x coordinate
    if (m_pos.x < netPos.x - (width / 2.0)) {
        closestPoint.x = netPos.x - (width / 2.0);
    } else if (m_pos.x > netPos.x + (width / 2.0)) {
        closestPoint.x = netPos.x + (width / 2.0);
    } else {
        closestPoint.x = m_pos.x;
    }

    // find y coordinate
    if (m_pos.y < netPos.y - (height / 2.0)) {
        closestPoint.y = netPos.y - (height / 2.0);
    } else if (m_pos.y > netPos.y + (height / 2.0)) {
        closestPoint.y = netPos.y + (height / 2.0);
    } else {
        closestPoint.y = m_pos.y;
    }

    netPoint = closestPoint;

    // check if closestPoint is inside the circle
    glm::vec2 dist = glm::vec2(closestPoint.x - m_pos.x,
                               closestPoint.y - m_pos.y);

    float len = glm::length(dist);
    if (len <= m_radius + 0.1) {
        return true;
    }

    return false;
}


void Ball::hitBall() {

    // Convert ball's velocity to the racquet's frame of reference
    // take the masses of the ball and racquet into account
    float ratio = racquet->m_mass / m_mass;
    glm::vec3 ballVelR = (m_vel + (-racquet->m_mass * ratio)) / (ratio + 1);

    // Compute angle between ball's velocity and the racquet's normal
    float dot = glm::dot(racquet->closestNormal, -ballVelR);
    dot /= (glm::length(ballVelR) * glm::length(racquet->closestNormal));
    float angle = glm::acos(dot);
    float deg = glm::degrees(angle);
    if (deg >= 90) {
        deg -= 90;
    }
    angle = glm::radians(deg);

    // Rotate ball's velocity vector about closestPoint, following law of reflection
    glm::mat3 rotation = generateRotationMatrix(-2 * angle);
    glm::vec3 cp = racquet->closestPoint;
    glm::vec3 newVelR = rotation * (-ballVelR);

    // Transform ball's new velocity back to world frame of reference
    glm::vec3 newVel = newVelR + racquet->m_vel;

    m_vel = newVel;

    m_hitVelocity = glm::vec2(m_vel.x, m_vel.y);
}

void Ball::computeForces() {

    // compute gravity
    f_gravity = glm::vec3(0, -GRAVITY * (m_mass * 100), 0);

    // compute drag
    float c_d = 0.55;

    // modify drag coefficient to account for ball "fuzziness". The greater the spin on the
    // ball, the smaller the coefficient... for a minimum of 0.5.
    if (abs(m_angVel) >= 100.0) {
        c_d = -0.0002 * m_angVel + 0.55;
    } else if (abs(m_angVel) >= 500.0) {
        c_d = 0.47;
    }

    float radius = m_radius * 0.03;
    float drag = 0.5 * c_d * PI * radius * radius * RHO * glm::length2(m_vel);
    f_drag = -drag * glm::normalize(m_vel);

    // compute lift/magnus force
    float c_l = 0.25;
    glm::vec3 rotation = glm::vec3(0, 0, 1);
    float lift = 0.5 * c_l * PI * radius * RHO * (glm::radians(m_angVel)) * glm::length(m_vel);
    f_lift = lift * glm::cross(glm::normalize(f_drag), rotation);

    f_total = f_gravity + f_drag + f_lift;
}
