#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <QVector>

class PhysicsEngine
{
public:
    PhysicsEngine();

private:
    QVector<RigidBody> physicObjects;
};

#endif // PHYSICSENGINE_H
