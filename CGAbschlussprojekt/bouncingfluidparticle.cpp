#include "bouncingfluidparticle.h"
#include <typeinfo>

CollisionData BouncingFluidParticle::collisionBfp(const BouncingFluidParticle& other)
{
    float vectorInBetween = (m_center - other.getCenter()).length();
    float minDistance = m_radius + other.getRadius();

    if (vectorInBetween < minDistance)
        return CollisionData(true, vectorInBetween - minDistance);
    return CollisionData(false, vectorInBetween - minDistance);
}
