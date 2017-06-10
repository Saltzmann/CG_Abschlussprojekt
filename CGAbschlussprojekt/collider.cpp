#include "collider.h"
#include "bouncingfluidparticle.h"

CollisionData Collider::collision(const Collider& other)
{
    switch(m_type)
    {
    case TYPE_SPHERE:
        switch(other.getType())
        {
        case TYPE_SPHERE:
            BouncingFluidParticle* bfp = (BouncingFluidParticle*)this;
            return bfp->collisionBfp((BouncingFluidParticle&)other);
            break;
        case TYPE_AABB:
            exit(-1); // Not implemented yet
            break;
        }

        break;
    case TYPE_AABB:
        switch(other.getType())
        {
        case TYPE_SPHERE:
            exit(-1); // Not implemented yet
            break;
        case TYPE_AABB:
            exit(-1); // Not implemented yet
            break;
        }

        break;
    }
}
