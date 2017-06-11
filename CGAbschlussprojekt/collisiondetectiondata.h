#ifndef COLLISIONDETECTIONDATA_H
#define COLLISIONDETECTIONDATA_H

#include "icollider.h"
#include <QMultiMap>

class CollisionDetectionData
{
public:
    CollisionDetectionData() {}
    ~CollisionDetectionData();

    void addCollisionPair(ICollider* first, ICollider* second);
    void removeCollisionPair(ICollider* first, ICollider* second);

    // Simple getter
    inline QMultiMap<ICollider*, ICollider*> getCDMap()
        { return m_collisionDetectionMap; }

private:
    QMultiMap<ICollider*, ICollider*> m_collisionDetectionMap;
};

#endif // COLLISIONDETECTIONDATA_H
