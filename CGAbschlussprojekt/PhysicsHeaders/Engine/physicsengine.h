#ifndef PHYISCSENGINE_H
#define PHYISCSENGINE_H

#include "./PhysicsHeaders/Collider/collisiondetectiondata.h"
#include "./PhysicsHeaders/Collider/collisionresponse.h"
#include "./PhysicsHeaders/Engine/rigidbody.h"
#include <QVector>
// TODO: implement QTimer to remove the deltaTime out of the simulate Method
//#include <QTimer>

namespace Physics
{
    class PhysicsEngine
    {
    public:
        PhysicsEngine()
            :   m_collisionDetectionData(),
                m_collisionResponse(this->getCDData())
        { }

        void addRigidBody(RigidBody& rb);
        void simulate(float deltaTime);
        // Reagiere auf checkForCollisions mit m_collisionResponse Methoden
        void checkForCollisions();
        void handleCollisions();

        // Simple getter
        inline CollisionDetectionData& getCDData() { return m_collisionDetectionData; }

    private:
        QVector<RigidBody> m_physicsObjects;
        CollisionDetectionData m_collisionDetectionData;
        CollisionResponse m_collisionResponse;
    };
}

#endif // PHYISCSENGINE_H
