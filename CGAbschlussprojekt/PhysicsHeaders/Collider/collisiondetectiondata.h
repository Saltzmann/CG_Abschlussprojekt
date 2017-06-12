#ifndef COLLISIONDETECTIONDATA_H
#define COLLISIONDETECTIONDATA_H

#include "./PhysicsHeaders/Collider/icollider.h"
#include "./PhysicsHeaders/Engine/rigidbody.h"
#include <QMultiMap>

namespace Physics
{
    class CollisionDetectionData
    {
    public:
        CollisionDetectionData() {}
        ~CollisionDetectionData();

        void addCollisionPair(RigidBody* first, RigidBody* second);
        void removeCollisionPair(RigidBody* first, RigidBody* second);

        // Simple getter
        inline QMultiMap<RigidBody*, RigidBody*>& getCDMap()
            { return m_collisionDetectionMap; }

    private:
        QMultiMap<RigidBody*, RigidBody*> m_collisionDetectionMap;
    };
}

#endif // COLLISIONDETECTIONDATA_H
