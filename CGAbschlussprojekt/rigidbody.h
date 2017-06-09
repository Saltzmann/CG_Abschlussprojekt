#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "collider.h"
#include <QVector3D>

class RigidBody
{
public:
    RigidBody(QVector3D startPos, QVector3D velocity)
        : m_position(startPos), m_velocity(velocity) {}

    // Simple getter
    inline QVector3D getPosition() { return m_position; }
    inline QVector3D getVelocity() { return m_velocity; }

    // Simple setter
    inline void setPosition(const QVector3D& position) { m_position = position; }
    inline void setVelocity(const QVector3D& velocity) { m_velocity = velocity; }

    void move(const float deltaTime);

private:
    QVector3D   m_position;
    QVector3D   m_velocity;
    Collider*   m_collider;
};

#endif // RIGIDBODY_H
