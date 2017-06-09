#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector3D>

class Particle
{
public:
    Particle(QVector3D center, QVector3D radius)
        : m_center(center), m_radius(radius) {}

private:
    QVector3D   m_center;
    float       m_radius;
};

#endif // PARTICLE_H
