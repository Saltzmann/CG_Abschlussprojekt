#ifndef COLLIDER_H
#define COLLIDER_H


class Collider
{
public:
    enum
    {
        TYPE_SPHERE,    // Sphere Collider
        TYPE_AABB       // AABB (Axis-Aligned Bounding Box)
                        //  Boxes that cant be rotated
    };

    Collider();

private:
    int m_type;
};

#endif // COLLIDER_H
