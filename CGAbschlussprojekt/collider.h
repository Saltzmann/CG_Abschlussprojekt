#ifndef COLLIDER_H
#define COLLIDER_H

#include "collisiondata.h"

class Collider
{
public:
    enum
    {
        TYPE_SPHERE,    // Sphere Collider
        TYPE_AABB       // AABB (Axis-Aligned Bounding Box)
                        //  Boxes that can't be rotated
    };

    Collider();

    CollisionData collision(const Collider& other);

    // Simple getter
    inline const int getType() const { return m_type; }

private:
    int m_type;
};

#endif // COLLIDER_H
