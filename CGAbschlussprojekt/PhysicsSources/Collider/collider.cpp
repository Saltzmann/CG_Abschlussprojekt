#include "icollider.h"

void Collider::moveCollider(const QVector3D &direction)
{
    m_colliderPosition += direction;
}
