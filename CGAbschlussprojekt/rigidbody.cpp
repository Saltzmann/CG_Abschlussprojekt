#include "rigidbody.h"

void RigidBody::move(const float deltaTime)
{
    m_position += (m_gravity + m_velocity) * deltaTime;
}
