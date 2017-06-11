#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H

#include "icollider.h"

class SphereCollider : public ICollider
{
public:
    SphereCollider(float radius)
        : ICollider(ICollider::COLLIDER_SPHERE),
          m_radius(radius)
    { }

    virtual ~SphereCollider();

    // translate the collider to the position of the rigidBody
    virtual void translateCollider(const QVector3D& translation);
    // checks the collision
    virtual CollisionData intersect(ICollider& other);


    // simple getter
    inline float getRadius() const { return m_radius; }

    // simple setter
    inline void setRadius(float rad) { m_radius = rad; }

private:
    float   m_radius;
};

#endif // SPHERECOLLIDER_H
