#ifndef COLLISIONDATA_H
#define COLLISIONDATA_H


class CollisionData
{
public:
    CollisionData(bool collided, float diff)
        : m_didCollide(collided), m_difference(diff) {}

    inline const bool  didCollide() { return m_didCollide; }
    inline const float getDifference() { return m_difference; }

private:
    const bool  m_didCollide;
    const float m_difference;
};

#endif // COLLISIONDATA_H
