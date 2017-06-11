#ifndef COLLISIONDATA_H
#define COLLISIONDATA_H


class CollisionData
{
public:
    CollisionData(bool didCollide, float posDifference)
        : m_didCollide(didCollide),
          m_positionDifference(posDifference)
    { }

    // simple getters
    inline bool  didCollide() { return m_didCollide; }
    inline float getDifference() { return m_positionDifference; }

private:
    const bool  m_didCollide;
    const float m_positionDifference;
};

#endif // COLLISIONDATA_H
