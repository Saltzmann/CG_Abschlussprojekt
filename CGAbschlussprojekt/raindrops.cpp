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

    //Optionen setzen
    _glassWidth = 1600;
    _glassHeight = 900;
    _maxNumberDroplets = 15000;

    _dropsSmall.reserve(_maxNumberDroplets);

    for(int i = 0; i < _maxNumberDroplets; i++) {
        _spawnDroplet();
    }

    //Zufallszahlengenerator seeden
    QTime timeObj;
    timeObj = QTime::currentTime();
    int msecs = timeObj.msecsSinceStartOfDay();
    //qDebug() << "Zeit seit Beginn des Tages in Millisekunden: " << msecs << " (für random seeding)";
    qsrand(msecs);
}

/*
void RenderableObject::_setSecondTexture(QString filename) {
    //(ausgelagerte) Hilfsfunktion zum Setzen der Zweit-Textur
    _secondTexture = new QOpenGLTexture(QImage(":/textures/" + filename).mirrored());
    _secondTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _secondTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    Q_ASSERT(_secondTexture->textureId() != 0); //Würde Fehler bedeuten
    _hasSecondTexture = true;
    qDebug() << endl << "2. Textur: " << filename << " geladen" << endl << endl;
}

void RenderableObject::_setSecondTexture(QString filename) {
    //(ausgelagerte) Hilfsfunktion zum Setzen der Zweit-Textur
    _secondTexture = new QOpenGLTexture(QImage(":/textures/" + filename).mirrored());
    _secondTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _secondTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    Q_ASSERT(_secondTexture->textureId() != 0); //Würde Fehler bedeuten
    _hasSecondTexture = true;
    qDebug() << endl << "2. Textur: " << filename << " geladen" << endl << endl;
}
*/
void Raindrops::_spawnDroplet() {
    if(_dropsSmall.size() >= _maxNumberDroplets) {
        return;
    }
    //50px Rand
    unsigned int lookUp;
    do {
        int xPos = 50 + qrand() % (_glassWidth - 100);
        int yPos = 50 + qrand() % (_glassHeight - 100);

        lookUp = _createUintPosHash(xPos, yPos);
        //wenn an dieser Stelle breits ein Tröpfchen ist -> erneut versuchen
    } while(_dropsSmall.contains(lookUp));

    unsigned char radius = _dropletsMinR + (qrand() %  (_dropletsMaxR - _dropletsMinR));
    _dropsSmall.insert(lookUp, radius);
}

void Raindrops::_deleteDroplets(QPoint location, unsigned char const &radius) {
    unsigned int lookUp;
    float cleaningRadius = radius * _dropletsCleaningRadiusMultiplier;
    //löschen im Kreis (0.5f damit richtig aufgerundet wird)
    for(unsigned char xPos = location.x() - cleaningRadius; xPos < location.x() + cleaningRadius + 0.5f; xPos++) {
        for(unsigned char yPos = location.y() - cleaningRadius; yPos < location.y() + cleaningRadius + 0.5f; yPos++) {
            //x² + y² = r² Kreisdarstellung wenn kleiner als r dann löschen
            if(((xPos*xPos) + (yPos*yPos)) < (cleaningRadius*cleaningRadius)) {
                lookUp = _createUintPosHash(xPos, yPos);
                _dropsSmall.remove(lookUp);
            }
        }
    }
}

void Raindrops::_spawnDrop(Drop* parent) {
    //TODO Radius, Momentum,
    unsigned short xPos, yPos, radius;
    //Wenn kein Parent (==nullptr) dann komplett neuer Tropfen
    if(parent == nullptr) {
        xPos = 50 + qrand() % (_glassWidth - 100);
        yPos = 50 + qrand() % (_glassHeight - 100);
        radius = _minR + (qrand() % (_maxR - _minR));
    }
    else { //Ansonsten Werte abhängig von Parent
        xPos = parent->posX + (qrand() % parent->radius/2);
        yPos = parent->posY + (qrand() % parent->radius/2) + parent->momentum;
        //gibt Wert zwischen oberer und unter Grenze aus mit 3 Nachkommastellen bei 0.x 2 bei x.0 usw
        radius = parent->radius *
                (_trailScaleRangeSmall + float(qrand() %
                int((_trailScaleRangeBig - _trailScaleRangeSmall)*1000)) / 1000.f);
    }
    Drop newDrop = Drop(xPos, yPos, radius, parent);
    unsigned int hashValue = _createUintPosHash(xPos,yPos);
    _dropsBig.insert(hashValue, newDrop);
}

unsigned int Raindrops::_createUintPosHash(unsigned short const &xPos, unsigned short const &yPos) {
    //Aufbau: XXXX0YYYY
    unsigned int hash = yPos;
    hash += (xPos * 100000); //
    return hash;
}

unsigned short Raindrops::_retrieveXValueFromHash(unsigned int const &hash) {
    //elemeniert irrelevante Stellen
    return hash / 100000;
}

unsigned short Raindrops::_retrieveYValueFromHash(unsigned int const &hash) {
    //liest letzten 4 Stellen aus
    return hash % 10000;
}

void Raindrops::render(QMatrix4x4 const &parentCTM,
                       QMatrix4x4 const &viewMatrix,
                       QMatrix4x4 const &projectionMatrix) {
    //TODO CTM Matrix Transformationen
    QMatrix4x4 ctm, combindedCTM;
    combindedCTM = parentCTM * _myCTM;
    ctm = combindedCTM;

    for(RenderableObject* x : _children) {
        x->render(ctm,
                  viewMatrix,
                  projectionMatrix);
    }

    //VBO und IBO an den Kontext binden
    glBindBuffer(GL_ARRAY_BUFFER, _model->vboHandle());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _model->iboHandle());

    //Matrix Locations für den Shader
    int unifProjMatrix = 0;
    int unifViewMatrix = 1;
    int unifModelMatrix = 2;
    int unifColor = 3;

    //Matrix Berechnungen fertig nun shader konfigurieren
    _shader->bind();

    // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte, Normalen und Textur
    int attrVertices = 0;
    int attrNorms = 1;
    //int attrTexCoords = 2;

    // Aktiviere die Verwendung der Attribute-Arrays
    _shader->enableAttributeArray(attrVertices);
    _shader->enableAttributeArray(attrNorms);
    //if(_model->hasTextureCoords()) {
    //   _shader->enableAttributeArray(attrTexCoords);
    //}

    // Fülle die Attribute-Buffer mit den korrekten Daten
    size_t model_stride = _model->stride(); //reduziert von 3 auf 1 Aufruf (implizit uint in int ist aktzeptabel - kein Werteverlust)
    _shader->setAttributeBuffer(attrVertices, GL_FLOAT, _model->vertOffset(), 4, model_stride); //VertexPositionen
    _shader->setAttributeBuffer(attrNorms, GL_FLOAT, _model->normOffset(), 4, model_stride); //VertexNormalen
    //if(_modelHasTextureCoords) {
    //    _shader->setAttributeBuffer(attrTexCoords, GL_FLOAT, _model->texCoordOffset(), 4, model_stride); //TexturCoordinaten
    //}

    //Haupt-Textur (und Zweit-Textur) binden und an shader übergeben
    //_mainTexture->bind(0);
    //_shader->setUniformValue("diffuseMap", 0);

    //if(_hasSecondTexture) {
    //    _secondTexture->bind(1);
    //    _shader->setUniformValue("blendMap", 1);
    //}

    //Einstellungen machen
    glDepthMask(GL_FALSE); //z-Buffer auf readonly stellen
    glEnable(GL_BLEND); //Farbmischung aktivieren
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Farbmischmethode festlegen
    //glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    _shader->setUniformValue(unifProjMatrix, projectionMatrix); //projektionsMatrix (const)
    _shader->setUniformValue(unifViewMatrix, viewMatrix); //viewMatrix ("const")
    _shader->setUniformValue(unifColor, _baseColor);

    for(unsigned int locationHash : _dropsSmall.uniqueKeys()) {
        unsigned short xPos = _retrieveXValueFromHash(locationHash);
        unsigned short yPos = _retrieveYValueFromHash(locationHash);
        ctm.translate(xPos, yPos, 0);
        ctm.scale(_dropsSmall.value(locationHash));
        _shader->setUniformValue(unifModelMatrix, ctm); //modelMatrix (immer abhängig vom gerade zu rendernden RenderableObject)
        glDrawElements(GL_TRIANGLES, _model->iboLength(), GL_UNSIGNED_INT, 0);
        ctm = combindedCTM; //zurücksetzen
    }

    //Element zeichnen lassen (implizit uint in int ist aktzeptabel - kein Werteverlust)
    //glDrawElements(GL_TRIANGLES, _model->iboLength(), GL_UNSIGNED_INT, 0);

    //PolygonMode auf default setzen
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Deaktiviere die Verwendung der Attribute-Arrays
    _shader->disableAttributeArray(attrVertices);
    _shader->disableAttributeArray(attrNorms);
    //if(_modelHasTextureCoords) {
    //    _shader->disableAttributeArray(attrTexCoords);
    //}

    //Shader lösen
    _shader->release();

    // Löse die Textur aus dem OpenGL-Kontext
    //if(_hasTexture) {
    //   _mainTexture->release();
    //}
    //if(_hasSecondTexture) {
    //    _secondTexture->release();
    //}

    //VBO und IBO vom Kontext lösen
   glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Einstellungen zurücksetzen
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

    //_spawnDroplet();
    //testCounter++;
}
