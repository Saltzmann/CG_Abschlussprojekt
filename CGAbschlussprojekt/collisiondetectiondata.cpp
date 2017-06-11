#include "collisiondetectiondata.h"

CollisionDetectionData::~CollisionDetectionData()
{
    m_collisionDetectionMap.clear();
}

void CollisionDetectionData::addCollisionPair(ICollider* first,
                                              ICollider* second)
{
   m_collisionDetectionMap.insert(first, second);
}

void CollisionDetectionData::removeCollisionPair(ICollider* first,
                                                 ICollider* second)
{
    m_collisionDetectionMap.remove(first, second);
}
