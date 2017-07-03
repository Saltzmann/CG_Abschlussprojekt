#ifndef DROP_H
#define DROP_H

//Generelle / Vererbungs Includes
//#include <QObject>
//Debug Includes
#include <QtGlobal>
#include <QDebug>

#define DROP_SHRINK_FACTOR 0.9999f
#define DROP_SPEED_FACTOR 0.005f

class Drop {
public:
    //Variablen
    unsigned short posX;
    unsigned short posY;
    unsigned short radius;
    //unsigned short spreadX;
    //unsigned short spreadY;
    float momentum;
    //unsigned short momentumX;
    //float lastSpawn;
    //float ticksToNextSpawn;
    Drop* parent;
    bool  isNew;
    bool  killed;
    float shrinkage;
    unsigned int trailTimer;


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
