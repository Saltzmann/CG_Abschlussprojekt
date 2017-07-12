#ifndef DROP_H
#define DROP_H

//Debug Includes
#include <QDebug>
//Datenstrukturen Includes
#include <QVector2D>
//externe Includes
#include "options.h"

//BigDrop-Simulation betreffende defines
#define DROP_SHRINK_FACTOR 0.999f
#define DROP_SPEED_FACTOR 0.000025

class Drop {
public:
    // A T T R I B U T E

    //Positions und Aussehensinformationen
    unsigned short posX;
    unsigned short posY;
    unsigned short radius;
    float momentum;
    float shrinkage;

    //Folgetropfen (Trail) bezogene Informationen
    Drop* parent; //wenn != nullptr dann selbst Trail
    bool isNew;
    int trailTimer;
    bool willSpawn;
    unsigned short trailSpreadX;
    unsigned short trailSize;

    //Update-Counters
    unsigned int numberOfUpdates;
    unsigned int numberOfMovedUnits;

    //Drop wird beim nächsten Updatevorgang gelöscht wenn killed == true
    bool killed;

    // M E T H O D E N

    //Dummy sollte nie benutzt werden -
    //wird aber von QHash gebraucht für die Methoden die, falls sie das
    //gesuchte Element nicht finden den Default-Konstruktor aufrufen
    Drop();
    //Parameterkonstruktor
    Drop(unsigned short const &xPos,
         unsigned short const &yPos,
         unsigned short const &radius,
         Drop* parent);

    // von Extern aufgerufene Update-Funktionen
    void update();
    Drop produceTrail();
    //Drop combineWith(Drop const &other); //buggy daher nicht genutzt
};

#endif // DROP_H
