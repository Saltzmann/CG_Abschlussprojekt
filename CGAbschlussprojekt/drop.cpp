#include "drop.h"


Drop::Drop() {
    qDebug() << "Falscher Konstruktoraufruf! --- Programm bricht ab!";
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
    this->trailTimer = 50;
    this->lastTrailPosX = this->posX;
    this->trailSpreadX = (unsigned short)((float(this->radius)/5.f) + 0.5f);
    float sizeFactor = float(15 + (qrand() % 15)) / 100.f;
    this->trailSize = (unsigned short)((float(this->radius) * sizeFactor) + 0.5f); //dirty style rounding
    this->willSpawn = false;
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
    //kleine Traildrops sollen schneller schrumpfen
    if(parent != nullptr) this->shrinkage *= DROP_SHRINK_FACTOR;
    unsigned short oldRadius = this->radius;
    this->radius = (unsigned short)round(shrinkage);
    //qDebug() << "Radius: " << radius;
    if(radius <= 0) {
        killed = true;
        return;
    }
    //Trailsize update
    this->trailSize -= (oldRadius - radius)/2;

    //Dann bewegen
    unsigned short oldPosY = this->posY;
    //dann mit aktualsierter Geschwindigkeit bewegen
    //MovementChance: je größer desto höher 70 = max combined size
    float movementChance = float(radius*2.f)/(70.f*2.f);
    movementChance = 1 + qrand() % int(1.f/movementChance);
    if(int(movementChance) != 1) movementChance = 0.f;
    this->posY -= (radius*radius*radius)*DROP_SPEED_FACTOR*int(movementChance);//pow³ radius
    this->momentum = oldPosY - posY;
    if(momentum > 0 && radius > 12) trailTimer -= round(1.5*momentum);
    if(trailTimer < 0) {
        this->willSpawn = true;
        //qDebug() << "=========================================================";
        trailTimer = 50;
     }
    //qDebug() << "Momentum: " << this->momentum;
    if(posY < 50.f) killed = true;
}

Drop Drop::produceTrail() {
    unsigned short trailPosX = lastTrailPosX - trailSpreadX + (qrand() % (trailSpreadX*2));
    lastTrailPosX = trailPosX;
    unsigned short trailPosY = this->posY + (this->radius - 2); //-2 damit es so aussieht als ob es von innen kommt
    Drop newTrailDrop = Drop(trailPosX, trailPosY, trailSize, this);
    this->shrinkage -= trailSize/8; //Spontan schrumpfen
    return newTrailDrop;
}
