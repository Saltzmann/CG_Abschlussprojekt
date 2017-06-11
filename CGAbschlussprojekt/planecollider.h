#ifndef PLANECOLLIDER_H
#define PLANECOLLIDER_H

#include "icollider.h"
#include <QVector3D>

namespace Physics
{
    class PlaneCollider : public ICollider
    {
    public:
        PlaneCollider();

        // translate the collider to the position of the rigidBody
        virtual void translateCollider(const QVector3D& translation);
        // checks the collision
        virtual CollisionData intersect(ICollider& other);

    private:
        QVector3D m_normal;
        float     m_distance;
    };
}

#endif // PLANECOLLIDER_H
