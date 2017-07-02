#ifndef DROP_H
#define DROP_H

//Generelle / Vererbungs Includes
//#include <QObject>
//Debug Includes
#include <QDebug>

class Drop {
public:
    float _posX;
    float _posY;
    float _radius;
    float _spreadX;
    float _spreadY;
    float _momentum;
    float _momentumX;
    float _lastSpawn;
    float _nextSpawn;
    Drop* _parent;
    bool  _isNew;
    bool  _killed;
    float _shrink;

public:
    Drop();
};

#endif // DROP_H
