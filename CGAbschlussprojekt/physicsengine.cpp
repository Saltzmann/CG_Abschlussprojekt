#include "physicsengine.h"
#include "collisiondata.h"

void PhysicsEngine::addRigidBody(const RigidBody &rb)
{
    m_rigidBodys.push_back(rb);
}

// First to use in a main loop or something like that,
// to update Position
void PhysicsEngine::simulate(const float deltaTime)
{
    for (size_t i = 0; i < m_rigidBodys.size(); ++i)
    {
        m_rigidBodys.at(i).move(deltaTime);
    }
    checkForCollisions();
}

// Second method to call, to check afterwards for the
// collisions and to handle them
void PhysicsEngine::checkForCollisions()
{
    for (size_t i = 0; i < m_rigidBodys.size(); ++i)
    {
        for(size_t j = i + 1; j < m_rigidBodys.size(); ++j)
        {
            CollisionData cd = m_rigidBodys.at(i)
                    .getCollider().collision(m_rigidBodys.at(j));

            if (cd.didCollide())
            {
                // TODO: insert collision response here!
            }
        }
    }
}
