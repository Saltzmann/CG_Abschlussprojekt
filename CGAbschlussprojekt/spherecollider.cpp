#include "spherecollider.h"
#include "aabbcollider.h"
#include "planecollider.h"
#include <typeinfo>

SphereCollider::~SphereCollider()
{

}

// translate the collider to the position of the rigidBody
void SphereCollider::translateCollider(const QVector3D& translation)
{
    m_colliderPosition += translation;
}

CollisionData intersectSphereCollider(const SphereCollider& first, const SphereCollider& second)
{
    // max distance between two spheres,
    float maxDistance = first.getRadius() + second.getRadius();
    float curDistance = (first.getPosition() - second.getPosition()).length();

    // TODO: 2. Parameter von CollisionData hier anpassen (könnte falsch sein)
    if (maxDistance > curDistance)
        return CollisionData(true, curDistance - maxDistance);
    return CollisionData(false, curDistance - maxDistance);
}

// TODO: implement Sphere - AABB collision detection
CollisionData intersectAABBCollider(const SphereCollider& first, const AABBCollider& second)
{
    throw new std::logic_error("Not implemented yet");
}

// TODO: implement Sphere - Plane collision detection
CollisionData intersectPlaneCollider(const SphereCollider& first, const PlaneCollider& second)
{
    throw new std::logic_error("Not implemented yet");
}

// checks the collision
CollisionData SphereCollider::intersect(ICollider& other)
{
    switch (other.getType())
    {
    case ICollider::COLLIDER_AABB:
        try { return intersectAABBCollider(*this, dynamic_cast<AABBCollider&>(other)); }
        catch (std::bad_cast) { exit(-10); }
        break;
    case ICollider::COLLIDER_SPHERE:
        try { return intersectSphereCollider(*this, dynamic_cast<SphereCollider&>(other)); }
        catch (std::bad_cast) { exit(-11); }
        break;
    case ICollider::COLLIDER_PLANE:
        try { return intersectPlaneCollider(*this, dynamic_cast<PlaneCollider&>(other)); }
        catch (std::bad_cast) { exit(-12); }
        break;
    default:
        throw new std::logic_error("No known collider");
    }
}
