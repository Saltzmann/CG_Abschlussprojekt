#ifndef DROP_H
#define DROP_H

//Generelle / Vererbungs Includes
//#include <QObject>
//Debug Includes
#include <QDebug>

class Drop {
public:
    unsigned short posX;
    unsigned short posY;
    unsigned short radius;
    //unsigned short spreadX;
    //unsigned short spreadY;
    unsigned short momentum;
    //unsigned short momentumX;
    //float lastSpawn;
    float ticksToNextSpawn;
    Drop* parent;
    bool  isNew;
    bool  killed;
    float shrink;
public:
    Drop(unsigned short const &xPos,
         unsigned short const &yPos,
         unsigned short const &radius,
         Drop* parent);
};

#endif // DROP_H
