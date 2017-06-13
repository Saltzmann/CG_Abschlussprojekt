#ifndef PHYISCSENGINE_H
#define PHYISCSENGINE_H

#include "./PhysicsHeaders/Collider/collisiondetectiondata.h"
#include "./PhysicsHeaders/Collider/collisionresponse.h"
#include "./PhysicsHeaders/Engine/rigidbody.h"
#include "timedelta.h"
#include <QVector>

// Singleton Class
namespace Physics
{
    class PhysicsEngine
    {
    public:
        PhysicsEngine* getInstance();

        void addRigidBody(RigidBody& rb);
        void simulate();
        // Reagiere auf checkForCollisions mit m_collisionResponse Methoden
        void checkForCollisions();
        void handleCollisions();

        // Simple getter
        inline CollisionDetectionData& getCDData() { return m_collisionDetectionData; }

    private:
        PhysicsEngine()
            :   m_collisionDetectionData(),
                m_collisionResponse(this->getCDData()),
                m_timeDelta(TimeDelta::getInstance())
        { }

        QVector<RigidBody>      m_physicsObjects;
        CollisionDetectionData  m_collisionDetectionData;
        CollisionResponse       m_collisionResponse;
        TimeDelta*              m_timeDelta;
    };
}

#endif // PHYISCSENGINE_H
