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
#include <QVector2D>

#include "drop.h"
#include "renderableobject.h"


class Raindrops : public RenderableObject {
    Q_OBJECT
private:
    // A T T R I B U T E
    //Erweiterung von RenderableObject
    QOpenGLTexture* _dropColorTexture;
    QOpenGLTexture* _dropAlphaTexture;
    QOpenGLTexture* _dropShineTexture;

    unsigned int _counter = 0;

    QHash<unsigned int, unsigned short> _dropsSmall; //[width][height] => [width*height]
    QHash<unsigned int, Drop> _dropsBig;
    QList<Drop> _updatedDrops;

    size_t _numberOfBigNonTrailDrops;
    size_t _glassWidth;
    size_t _glassHeight;

    bool _hasDropAlphaTexture;
    bool _hasDropColorTexture;
    bool _hasDropShineTexture;

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

    void _setDropAlphaTexture(QString filename);
    void _setDropColorTexture(QString filename);
    void _setDropShineTexture(QString filename);

public:
    Raindrops(QMatrix4x4 ctm,
              Model* model,
              int shaderTypeFlag,
              QOpenGLShaderProgram* shader,
              QOpenGLShaderProgram* secondShader,
              QVector4D const &baseColor,
              QString const &refractionBackground = "",
              QString const &refractionOverlay = "",
              QString const &dropAlpha = "",
              QString const &dropColor = "",
              QString const &dropShine = "");

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
