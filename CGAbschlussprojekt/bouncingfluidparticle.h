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

    // Simple getter
    inline QVector3D getCenter() { return m_center; }
    inline float getRadius() { return m_radius; }

    // Collision Detection for other Particles
    CollisionData collisionBfp(const BouncingFluidParticle& other);

private:
    QVector3D   m_center;
    float       m_radius;
};

#endif // PARTICLE_H
