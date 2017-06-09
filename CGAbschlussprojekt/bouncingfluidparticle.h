#ifndef PARTICLE_H
#define PARTICLE_H

#include "rigidbody.h"
#include "collisiondata.h"
#include <QVector3D>

class BouncingFluidParticle : public Collider
{
public:
    BouncingFluidParticle(QVector3D position, QVector3D radius)
        : Collider(Collider::TYPE_SPHERE),
          m_center(position),
          m_radius(radius) {}

    CollisionData collisionBfp(const BouncingFluidParticle& other);

private:
    float       m_center;
    float       m_radius;
};

#endif // PARTICLE_H
