#include "drop.h"


Drop::Drop() {
    qDebug() << "Falscher Konstruktoaufruf! --- Programm bricht ab!";
    Q_ASSERT(false);
}

Drop::Drop(unsigned short const &xPos,
           unsigned short const &yPos,
           unsigned short const &radius,
           Drop *parent) {

    this->posX = xPos;
    this->posY = yPos;
    this->radius = radius;
    this->momentum = 0;
    //momentumX;
    //lastSpawn;
    this->trailTimer = 999999;
    this->parent = parent;
    this->isNew = true;
    this->killed = false;
    this->shrinkage = (float)radius;
}

void Drop::update() {
    //4 Aufgaben: Bewegen, schrumpfen, Spur hinterlassen, kleine Tropfen löschen
    //hier Bewegen und schrumpfen
    //erst schrumpfen:
    this->shrinkage *= DROP_SHRINK_FACTOR;
    this->radius = (unsigned short)round(shrinkage);
    if(radius <= 0) {
        killed = true;
        return;
    }
    unsigned short oldPosY = this->posY;
    //dann mit aktualsierter Geschwindigkeit bewegen
    //MovementChance: je größer desto höher
    float movementChance = float(radius)/70.f;
    movementChance = 1 + qrand() % int(1.f/movementChance);
    if(int(movementChance) != 1) movementChance = 0.f;
    this->posY -= radius*radius*DROP_SPEED_FACTOR*int(movementChance);
    this->momentum = oldPosY - posY;
    if(posY < 50.f) killed = true;
}

Drop Drop::produceTrail() {
    unsigned short trailPosX = this->posX - (this->radius/2) + (qrand() % this->radius);
    unsigned short trailPosY = this->posY + (this->radius - 2); //-2 damit es so aussieht als ob es von innen kommt
    unsigned short trailRadius = this->radius * (unsigned short)(round(float(10 + (qrand() % 40)) / 100.f));
    Drop newTrailDrop = Drop(trailPosX, trailPosY, trailRadius, this);
    return newTrailDrop;
}
