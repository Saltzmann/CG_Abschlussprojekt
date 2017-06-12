#ifndef PHYISCSENGINE_H
#define PHYISCSENGINE_H

#include "./PhysicsHeaders/Collider/collisiondetectiondata.h"
#include "./PhysicsHeaders/Collider/collisionresponse.h"
#include "./PhysicsHeaders/Engine/rigidbody.h"
#include <QVector>
//#include <QTimer>

namespace Physics
{
    class PhysicsEngine
    {
    public:
        PhysicsEngine() { }

        void addRigidBody(RigidBody& rb);
        void simulate(float deltaTime);
        // Reagiere auf checkForCollisions mit m_collisionResponse Methoden
        void checkForCollisions();

    private:
        QVector<RigidBody> m_physicsObjects;
        CollisionDetectionData m_collisionDetectionData;
        CollisionResponse m_collisionResponse;
    };
}

#endif // PHYISCSENGINE_H
