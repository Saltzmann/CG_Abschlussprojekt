#ifndef COLLIDER_H
#define COLLIDER_H

#include "collisiondata.h"
#include <QVector3D>

namespace Physics
{
    class ICollider
    {
    public:
        ICollider(int collider_type)
            : m_colliderType(collider_type),
              m_colliderPosition(QVector3D(0.0f, 0.0f, 0.0f))
        { }

        virtual ~ICollider() { }

        enum
        {
            COLLIDER_AABB,
            COLLIDER_SPHERE,
            COLLIDER_PLANE
        };

        // translate the collider to the position of the rigidBody
        virtual void translateCollider(const QVector3D& translation) = 0;
        // checks the collision
        virtual CollisionData intersect(ICollider& other) = 0;

        // Basic getter
        inline QVector3D getPosition() const { return m_colliderPosition; }
        inline int       getType() const { return m_colliderType; }

    protected:
        // Position is the center of the collider
        int         m_colliderType;
        QVector3D   m_colliderPosition;

    };
}

#endif // COLLIDER_H
