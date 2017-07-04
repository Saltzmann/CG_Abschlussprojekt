#ifndef DROP_H
#define DROP_H

//Generelle / Vererbungs Includes
//#include <QObject>
//Debug Includes
#include <QtGlobal>
#include <QDebug>

#define DROP_SHRINK_FACTOR 0.999f
#define DROP_SPEED_FACTOR 0.001f

class Drop {
public:
    //Variablen
    unsigned short posX;
    unsigned short posY;
    unsigned short radius;
    unsigned short lastTrailPosX;
    unsigned short trailSpreadX;
    unsigned short trailSize;
    //unsigned short spreadY;
    float momentum;
    //unsigned short momentumX;
    //float lastSpawn;
    //float ticksToNextSpawn;
    Drop* parent;
    bool  isNew;
    bool  killed;
    float shrinkage;
    int trailTimer;
    bool willSpawn;


    //Dummy sollte nie benutzt werden - wird aber von QHash gebraucht
    Drop();
    //Methoden
    Drop(unsigned short const &xPos,
         unsigned short const &yPos,
         unsigned short const &radius,
         Drop* parent);

    void update();
    Drop produceTrail();
    //void combine(Drop &other);
};

#endif // DROP_H
