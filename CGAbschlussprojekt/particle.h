#ifndef PARTICLE_H
#define PARTICLE_H

#include "rigidbody.h"
#include <QVector3D>

class Particle : public RigidBody
{
public:
    Particle(QVector3D position, QVector3D velocity, QVector3D radius)
        : RigidBody(position, velocity),
          m_position(position),
          m_radius(radius) {}

private:
    float       m_radius;
};

#endif // PARTICLE_H
