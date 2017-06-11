#include "rigidbody.h"

void RigidBody::move(float deltaTime)
{
    QVector3D translated = m_rbPosition - m_rbOldPosition;
    m_rbOldPosition = m_rbPosition;
    // Bewege den Collider mit dem Objekt mit
    m_rbCollider->translateCollider(translated);
    m_rbPosition += C_GRAVITY * deltaTime;
}

void RigidBody::moveTowards(const QVector3D& towards, float deltaTime)
{
    // Berechne, wie sich der Vektor bewegt hat
    QVector3D translated = m_rbPosition - m_rbOldPosition;
    m_rbOldPosition = m_rbPosition;
    // Bewege den Collider mit dem Objekt mit
    m_rbCollider->translateCollider(translated);
    m_rbPosition += (C_GRAVITY + towards) * deltaTime;
}

void RigidBody::addForce(const QVector3D& force, float deltaTime)
{
    m_rbForce += force * deltaTime;
}

void RigidBody::clearForce()
{
    m_rbForce *= 0.0f;
}
