#ifndef COLLISIONDATA_H
#define COLLISIONDATA_H


class CollisionData
{
public:
    CollisionData(bool collided, float diff)
        : isCollided(collided), difference(diff) {}

private:
    const bool  isCollided;
    const float difference;
};

#endif // COLLISIONDATA_H
