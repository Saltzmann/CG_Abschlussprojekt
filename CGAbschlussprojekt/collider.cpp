#include "collider.h"

CollisionData Collider::collision(const Collider& other)
{
    switch(m_type)
    {
    case TYPE_SPHERE:
        switch(other.getType())
        {
        case TYPE_SPHERE:
            // add collision response
            break;
        case TYPE_AABB:
            // add collision response
            break;
        }

        break;
    case TYPE_AABB:
        switch(other.getType())
        {
        case TYPE_SPHERE:
            // add collision response
            break;
        case TYPE_AABB:
            // add collision response
            break;
        }

        break;
    }
}
