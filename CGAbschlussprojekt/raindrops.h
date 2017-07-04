#ifndef RAINDROPS_H
#define RAINDROPS_H

#include <QHash>
#include <QVector>
#include <QPoint>
#include <QtGlobal>
#include <QTime>
#include <QTimer>
#include <QPair>
#include <QObject>
#include <QList>

#include "drop.h"
#include "renderableobject.h"


class Raindrops : public RenderableObject {
    Q_OBJECT
private:
    /*
    //Default Options
    //Große
    int _minR; //10
    int _maxR; //40
    size_t _maxDrops; //900
    float _rainChance; //0.3
    float _rainLimit; //3
    //kleine
    float _dropletsRate; //50
    float _dropletsSizeSmall; //2
    float _dropletsSizeBig; //4
    float _dropletsCleaningRadiusMultiplier; //0.43
    bool _raining; //true
    float _globalTimeScale; //1
    float _trailRate; //1
    bool _autoShrink; //true
    float _spawnAreaSmall; //-0.1
    float _spawnAreaBig; //0.95
    float _trailScaleRangeSmall; //0.2
    float _trailScaleRangeBig; //0.5
    float _collisionRadius; //0.65
    float _collisionRadiusIncrease; //0.01
    float _dropFallMultiplier; //1
    float _collisionBoostMultiplier; //0.05
    float _collisionBoost; //1
    //Prototype (TODO Types)
    float _dropColor; //:null,
    float _dropAlpha; //:null,
    float _canvas; //:null,
    float _ctx; //:null,
    float _width; //:0,
    float _height; //:0,
    float _scale; //:0,
    float _dropletsPixelDensity; //:1, //Anpassung auf Planegröße
    float _droplets; //:null,
    float _dropletsCtx; //:null,
    float _dropletsCounter; //:0,
    QVector<Drop>  _drops; //:null, //Array
    float _dropsGfx; //:null, //Array
    float _clearDropletsGfx; //:null,
    float _textureCleaningIterations; //:0,
    float _lastRender; //:null,
    */
    //Options
    unsigned short const _minR = 10; //10
    unsigned short const _maxR = 30; //40
    unsigned short const _dropletsMinR = 3;
    unsigned short const _dropletsMaxR = 7;
    float const _dropletsCleaningRadiusMultiplier = 0.8f; //0.43

    // A T T R I B U T E
    //Erweiterung von RenderableObject
    //QOpenGLTexture* _refractionBackground;
    //QOpenGLTexture* _refractionFront;

    unsigned int testCounter = 0;

    QHash<unsigned int, unsigned char> _dropsSmall; //[width][height] => [width*height]
    QHash<unsigned int, Drop> _dropsBig;
    QList<Drop> _updatedDrops;

    size_t _numberOfBigNonTrailDrops;
    size_t _maxNumberDroplets;
    size_t _maxNumberNonTrailDrops;
    size_t _glassWidth;
    size_t _glassHeight;


    //Update Timer
    QTimer* _updateTimer;

    //Methoden
    void _spawnDroplet();
    void _deleteDroplets(unsigned short const &locationX,
                         unsigned short const &locationY,
                         unsigned short const &radius);

    void _spawnDrop();
    inline void _updateDrops();
    
    //Helper
    inline unsigned int _createUintPosHash(unsigned short const &xPos,
                                           unsigned short const &yPos);
    inline unsigned short _retrieveXValueFromHash(unsigned int const &hash);
    inline unsigned short _retrieveYValueFromHash(unsigned int const &hash);

public:
    Raindrops(QMatrix4x4 ctm,
              Model* model,
              int shaderTypeFlag,
              QOpenGLShaderProgram* shader,
              QOpenGLShaderProgram* secondShader,
              QVector4D const &baseColor,
              QString const &mainTextureFileName = "",
              QString const &secondTextureFileName = "");

    //Override - muss sein TODO
    virtual void render(QMatrix4x4 const &parentCTM,
                        QMatrix4x4 const &viewMatrix,
                        QMatrix4x4 const &projectionMatrix);

    /*
    //Methods TODO
    void drawDrop(); //NOTE benötigt?
    void addDrop();
    void updateRain();
    void createDrop();
    void clearDrops();
    void clearTexture();
    void updateDroplets();
    void updateDrops();
    */
public slots:
    void update();
signals:
};

#endif // RAINDROPS_H
