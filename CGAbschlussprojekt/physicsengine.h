#ifndef PHYISCSENGINE_H
#define PHYISCSENGINE_H

#include "collisiondetectiondata.h"
#include "collisionresponse.h"
#include "rigidbody.h"
#include <QVector>

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

#endif // PHYISCSENGINE_H
