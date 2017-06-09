#include "physicsengine.h"

void PhysicsEngine::addRigidBody(const RigidBody &rb)
{
    m_rigidBodys.push_back(rb);
}

void PhysicsEngine::simulate(const float deltaTime)
{
    for (size_t i = 0; i < m_rigidBodys.size(); ++i)
        m_rigidBodys.at(i).move(deltaTime);
}
