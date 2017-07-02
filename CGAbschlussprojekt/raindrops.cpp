#include "raindrops.h"

Raindrops::Raindrops(QMatrix4x4 ctm,
                     Model* model,
                     int shaderTypeFlag,
                     QOpenGLShaderProgram* shader,
                     QOpenGLShaderProgram* secondShader,
                     QVector4D const &baseColor,
                     QString const &mainTextureFileName,
                     QString const &secondTextureFileName)

                     : RenderableObject (ctm,
                                         model,
                                         shaderTypeFlag,
                                         shader,
                                         secondShader,
                                         baseColor,
                                         mainTextureFileName,
                                         secondTextureFileName) {

   _glassWidth = 1600;
   _glassHeight = 900;

    _dropsSmall.reserve(_maxNumberDroplets);

   //Zufallszahlengenerator seeden
   QTime timeObj;
   qsrand(timeObj.msecsSinceStartOfDay());

   _maxNumberDroplets = 900;
}

void Raindrops::_spawnDroplet() {
    if(_dropsSmall.size() >= _maxNumberDroplets) {
        return;
    }
    QString lookUp;
    do {
        int xPos = qrand() % _glassWidth;
        int yPos = qrand() % _glassHeight;

        lookUp = QString::number(xPos) + "x" + QString::number(yPos);
        //wenn an dieser Stelle breits ein Tröpfchen ist -> erneut versuchen
    } while(!_dropsSmall.contains(lookUp));

    unsigned char radius = 2 + (qrand() %  2);
    _dropsSmall.insert(lookUp, radius);
}

void Raindrops::_deleteDroplets(QPoint location, unsigned char const &radius) {
    QString lookUp;
    float cleaningRadius = radius * _dropletsCleaningRadiusMultiplier;
    //löschen im Kreis
    for(unsigned char xPos = location.x() - cleaningRadius; xPos < location.x() + cleaningRadius; xPos++) {
        for(unsigned char yPos = location.y() - cleaningRadius; yPos < location.y() + cleaningRadius; yPos++) {
            //x² + y² = r² Kreisdarstellung wenn kleiner als r dann löschen
            if(((xPos*xPos) + (yPos*yPos)) < (cleaningRadius*cleaningRadius)) {
                lookUp = QString::number(xPos) + "x" + QString::number(yPos);
                _dropsSmall.remove(lookUp);
            }
        }
    }
}

void Raindrops::render(QMatrix4x4 const &parentCTM,
                       QMatrix4x4 const &viewMatrix,
                       QMatrix4x4 const &projectionMatrix) {

}
