#include "./PhysicsHeaders/Collider/collisiondetectiondata.h"

namespace Physics
{
    CollisionDetectionData::~CollisionDetectionData()
    {
        m_collisionDetectionMap.clear();
    }

    void CollisionDetectionData::addCollisionPair(RigidBody* first,
                                                  RigidBody* second)
    {
       m_collisionDetectionMap.insert(first, second);
    }

    void CollisionDetectionData::removeCollisionPair(RigidBody* first,
                                                     RigidBody* second)
    {
        m_collisionDetectionMap.remove(first, second);
    }
}
