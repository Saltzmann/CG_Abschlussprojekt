#ifndef AABBCOLLIDER_H
#define AABBCOLLIDER_H

#include "icollider.h"

// TODO: Größe der Collider anpassbar machen

namespace Physics
{
    // Axis Aligned Bounding Box
    class AABBCollider : public ICollider
    {
    public:
        AABBCollider(const QVector3D& min, const QVector3D& max)
            : ICollider(ICollider::COLLIDER_AABB),
              m_minPosition(min),
              m_maxPosition(max)
        { }

        virtual ~AABBCollider();


        // translate the collider to the position of the rigidBody
        virtual void translateCollider(const QVector3D& translation);
        // checks the collision
        virtual CollisionData intersect(ICollider& other);

        // Simple getters
        inline QVector3D getMinPos() const { return m_minPosition; }
        inline QVector3D getMaxPos() const { return m_maxPosition; }

    private:
        QVector3D m_minPosition;
        QVector3D m_maxPosition;
    };
}

#endif // AABBCOLLIDER_H
