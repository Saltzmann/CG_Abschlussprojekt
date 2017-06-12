#include "./PhysicsHeaders/Collider/collisionresponse.h"
#include "./PhysicsHeaders/Collider/icollider.h"
#include "./PhysicsHeaders/Collider/colliders.h"
#include "./PhysicsHeaders/Engine/rigidbody.h"
#include <QMultiMap>

namespace Physics
{
    void CollisionResponse::response()
    {
        QMultiMap<RigidBody*, RigidBody*>& CDMap = this->m_CDDataRef.getCDMap();
        QMultiMap<RigidBody*, RigidBody*>::iterator CDMapIt = CDMap.begin();

        for (; CDMapIt != CDMap.end(); ++CDMapIt)
        {
            RigidBody* first = const_cast<RigidBody*>(CDMapIt.key());
            RigidBody* second = const_cast<RigidBody*>(CDMapIt.value());

            first->setVelocity(first->getVelocity() * -1);
            second->setVelocity(second->getVelocity() * -1);
        }

        CDMap.clear();
    }
}
