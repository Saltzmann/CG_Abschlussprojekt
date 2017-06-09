#ifndef COLLISIONDATA_H
#define COLLISIONDATA_H


class CollisionData
{
public:
    CollisionData(bool collided, float diff)
        : didCollide(collided), difference(diff) {}

private:
    const bool  didCollide;
    const float difference;
};

#endif // COLLISIONDATA_H
