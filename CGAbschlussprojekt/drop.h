#ifndef DROP_H
#define DROP_H

//Generelle / Vererbungs Includes
//#include <QObject>
//Debug Includes
#include <QtGlobal>
#include <QDebug>
#include <QVector2D>

#define DROP_SHRINK_FACTOR 0.999f
#define DROP_SPEED_FACTOR 0.00001

namespace Options {
    float const minR = 50.f; //10
    float const maxR = 100.f; //40
    float const dropletsMinR = 5.f;
    float const dropletsMaxR = 10.f;
    float const dropCombindedMaxRadius = 350.f;
    float const dropletsCleaningRadiusMultiplier = 0.25f;
    float const trailTimer = 50.f;
    size_t const maxNumberDroplets = 7500; //7500; //5k-10k
    size_t const maxNumberNonTrailDrops = 100; //100ish
    size_t const upperSpawnBorderDrops = 150;
    size_t const lowerSpawnBorderDrops = 150;
    size_t const upperSpawnBorderDroplets = 100;
    size_t const lowerSpawnBorderDroplets = 100;
}

class Drop {
public:
    //Variablen
    unsigned short posX;
    unsigned short posY;
    unsigned short radius;
    unsigned short trailSpreadX;
    unsigned short trailSize;
    unsigned int numberOfUpdates;
    unsigned int numberOfMovedPixels;
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
    Drop combineWith(Drop const &other);
};

#endif // DROP_H
