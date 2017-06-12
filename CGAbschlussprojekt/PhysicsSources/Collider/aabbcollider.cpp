#include "./PhysicsHeaders/Collider/aabbcollider.h"
#include "./PhysicsHeaders/Collider/colliders.h"
#include <typeinfo>

namespace Physics
{
    // translate the collider to the position of the rigidBody
    void AABBCollider::translateCollider(const QVector3D& translation)
    {
        m_colliderPosition += translation;
    }

    // TODO: implement AABB - Sphere collision detection
    CollisionData intersectSphereCollider(const AABBCollider& first, const SphereCollider& second)
    {
        throw new std::logic_error("Not implemented yet");
    }

    // TODO: test AABB - AABB collision
    CollisionData intersectAABBCollider(const AABBCollider& first, const AABBCollider& second)
    {
        QVector3D distanceLeft = second.getMinPos() - first.getMaxPos();
        QVector3D distanceRight = first.getMinPos() - second.getMaxPos();

        QVector3D distances = QVector3D(std::max(distanceLeft.x(), distanceRight.x()),
                                        std::max(distanceLeft.y(), distanceRight.y()),
                                        std::max(distanceLeft.z(), distanceRight.z()));

        float maxDistance = std::max(distances.x(), std::max(distances.y(), distances.z()));

        return CollisionData(maxDistance < 0, maxDistance);
    }

    // TODO: implement AABB - Plane collision detection
    CollisionData intersectPlaneCollider(const AABBCollider& first, const PlaneCollider& second)
    {
        throw new std::logic_error("Not implemented yet");
    }

    // checks the collision
    CollisionData AABBCollider::intersect(ICollider& other)
    {
        switch(other.getType())
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
            break;
        }
    }
}
