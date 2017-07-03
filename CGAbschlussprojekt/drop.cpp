#include "drop.h"


Drop::Drop(unsigned short const &xPos,
           unsigned short const &yPos,
           unsigned short const &radius,
           Drop *parent) {

    posX = xPos;
    posY = yPos;
    this->radius = radius;
    momentum = 0;
    //momentumX;
    //lastSpawn;
    ticksToNextSpawn = 99999.f;
    this->parent = parent;
    isNew = true;
    killed = false;
    shrink = this->radius * 0.1f;
}
