#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "rigidbody.h"
#include <QVector>
#include <QVector3D>

class PhysicsEngine
{
public:
    PhysicsEngine(QVector3D gravity)
        : m_gravity(gravity) { }

    void addRigidBody(const RigidBody& rb);
    void simulate(const float deltaTime);
    void checkForCollisions();

private:
    QVector<RigidBody> m_rigidBodys;
    QVector3D          m_gravity;
};

#endif // PHYSICSENGINE_H
