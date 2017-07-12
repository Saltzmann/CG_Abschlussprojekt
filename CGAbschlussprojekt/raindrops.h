#ifndef RAINDROPS_H
#define RAINDROPS_H

//Generelle Includes
#include <QObject>
#include <QtGlobal>
//Timer Includes
#include <QTime>
#include <QTimer>
//Datenstruktur Includes
#include <QHash>
#include <QPoint>
#include <QList>
#include <QVector2D>
//externe Includes
#include "drop.h"
#include "renderableobject.h"


class Raindrops : public RenderableObject {
    Q_OBJECT
private:
    // A T T R I B U T E

    //Erweiterung von RenderableObject
    QOpenGLTexture* _dropColorTexture;
    QOpenGLTexture* _dropAlphaTexture;

    //Update-Counter
    unsigned int _counter = 0;

    QHash<unsigned int, unsigned short> _dropsSmall;
    QHash<unsigned int, Drop> _dropsBig;
    QList<Drop> _updatedDrops;

    size_t _numberOfBigNonTrailDrops;
    size_t _glassWidth;
    size_t _glassHeight;

    //Kontroll-Flags, ob alle benötigten Texturen vorhanden sind
    bool _hasDropAlphaTexture;
    bool _hasDropColorTexture;

    //Update Timer
    QTimer* _updateTimer;

    // M E T H O D E N

    //interne Hilfsmethoden (hauptsächlich zur Übersichtlichkeit)
    void _spawnDrop();
    inline void _updateDrops();
    void _spawnDroplet();
    void _deleteDroplets(unsigned short const &locationX,
                         unsigned short const &locationY,
                         unsigned short const &radius);
    
    //Hash-Helper
    inline unsigned int _createUintPosHash(unsigned short const &xPos,
                                           unsigned short const &yPos);
    inline unsigned short _retrieveXValueFromHash(unsigned int const &hash);
    inline unsigned short _retrieveYValueFromHash(unsigned int const &hash);

    //Texturen-Helper
    void _setDropAlphaTexture(QString filename);
    void _setDropColorTexture(QString filename);

public:
    // A T T R I B U T E
    // -

    // M E T H O D E N

    //Parameter-Konstruktor (Erweiterung von renderableObject)
    Raindrops(QMatrix4x4 ctm,
              Model* model,
              int shaderTypeFlag,
              QOpenGLShaderProgram* shader,
              QOpenGLShaderProgram* secondShader,
              QVector4D const &baseColor,
              QString const &refractionBackground = "",
              QString const &refractionOverlay = "",
              QString const &dropAlpha = "",
              QString const &dropColor = "");

    //Override der Render-Funktion für diese spezielle Anwendung
    virtual void render(QMatrix4x4 const &parentCTM,
                        QMatrix4x4 const &viewMatrix,
                        QMatrix4x4 const &projectionMatrix);
public slots:
    //Slot für den Update-Timer
    void update();
};

#endif // RAINDROPS_H
