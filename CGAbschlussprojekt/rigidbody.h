#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "icollider.h"
#include "rigidbodyconstants.h"
#include <QVector3D>

// Füge den RigidBody als Membervariable einer Klasse hinzu, um diesem
//  physikalische Eigenschaften zugänglich zu machen

class RigidBody
{
public:
    RigidBody() { }
    RigidBody(ICollider* collider,
              const QVector3D& pos,
              const QVector3D& vel,
              float mass)
        : m_rbCollider(collider),
          m_rbPosition(pos),
          m_rbVelocity(vel),
          m_rbMass(mass)
    { }

    ~RigidBody();

    void move(float deltaTime);
    void moveTowards(const QVector3D& towards, float deltaTime);
    void addForce(const QVector3D& force, float deltaTime); // addForce & moveTowards should be the same? TODO
    void clearForce();

    // Simple getters
    inline QVector3D  getPosition() const { return m_rbPosition; }
    inline QVector3D  getVelocity() const { return m_rbVelocity; }
    inline float      getMass()     const { return m_rbMass; }
    inline ICollider& getCollider() const { return *m_rbCollider; }

    // Simple setters
    inline void setPosition(const QVector3D& pos) { m_rbPosition = pos; }
    inline void setVelocity(const QVector3D& vel) { m_rbVelocity = vel; }
    inline void setMass(float mass) { m_rbMass = mass; }

private:
    ICollider* m_rbCollider;

    QVector3D  m_rbPosition;
    QVector3D  m_rbOldPosition;
    QVector3D  m_rbVelocity;
    QVector3D  m_rbForce; // = || != velocity? TODO
    float      m_rbMass;
};

#endif // RIGIDBODY_H
