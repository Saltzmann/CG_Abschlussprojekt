#include "rigidbody.h"

void RigidBody::move(float deltaTime)
{
    m_rbPosition += C_GRAVITY * deltaTime;
}

void RigidBody::moveTowards(const QVector3D& towards, float deltaTime)
{
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
