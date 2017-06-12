#include "./PhysicsHeaders/Engine/physicsengine.h"
#include "./PhysicsHeaders/Collider/collisiondata.h"

namespace Physics
{
    void PhysicsEngine::addRigidBody(RigidBody& rb)
    {
        m_physicsObjects.push_back(rb);
    }

    void PhysicsEngine::simulate(float deltaTime)
    {
        const QVector<RigidBody>::size_type VEC_SIZE = m_physicsObjects.size();

        for (QVector<RigidBody>::size_type i = 0; i < VEC_SIZE; ++i)
        {
            const_cast<RigidBody&>(m_physicsObjects.at(i)).move(deltaTime);
        }
    }

    void PhysicsEngine::checkForCollisions()
    {
        const QVector<RigidBody>::size_type VEC_SIZE = m_physicsObjects.size();

        for (QVector<RigidBody>::size_type i = 0; i < VEC_SIZE; ++i)
        {
            for (QVector<RigidBody>::size_type j = i + 1; j < VEC_SIZE; ++j)
            {
                CollisionData cd = const_cast<RigidBody&>(m_physicsObjects.at(i)).getCollider()
                        ->intersect(*(const_cast<RigidBody&>(m_physicsObjects.at(j)).getCollider()));

                if (cd.didCollide())
                    m_collisionDetectionData.addCollisionPair(
                                &const_cast<RigidBody&>(m_physicsObjects.at(i)),
                                &const_cast<RigidBody&>(m_physicsObjects.at(j)));
            }
        }
    }

    // TODO: think about inserting collision response inside checkForCollisions - Method
    void PhysicsEngine::handleCollisions()
    {
        m_collisionResponse.response();
    }
}


