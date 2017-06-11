#include "physicsengine.h"

void PhysicsEngine::addRigidBody(const RigidBody& rb)
{
    m_physicsObjects.push_back(rb);
}

void PhysicsEngine::simulate(float deltaTime)
{
    const QVector::size_type VEC_SIZE = m_physicsObjects.size();

    for (QVector::size_type i = 0; i < VEC_SIZE; ++i)
    {
        m_physicsObjects.at(i).move(deltaTime);
    }
}
