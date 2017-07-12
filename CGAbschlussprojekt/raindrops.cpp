#include "raindrops.h"

Raindrops::Raindrops(QMatrix4x4 ctm,
                     Model* model,
                     int shaderTypeFlag,
                     QOpenGLShaderProgram* shader,
                     QOpenGLShaderProgram* secondShader,
                     QVector4D const &baseColor,
                     QString const &refractionBackground,
                     QString const &refractionOverlay,
                     QString const &dropAlpha,
                     QString const &dropColor)

                     : RenderableObject (ctm,
                                         model,
                                         shaderTypeFlag,
                                         shader,
                                         secondShader,
                                         baseColor,
                                         refractionBackground,
                                         refractionOverlay) {

    //Renderable Object - Erweiterungen
    _hasDropColorTexture = false;
    _hasDropAlphaTexture = false;

    //Wenn angegeben (muss) Alpha-Textur setzen
    if(dropAlpha.length() != 0) {
        _setDropAlphaTexture(dropAlpha);
    }
    else {
        Q_ASSERT(false);
    }

    //Wenn angegeben (muss) Color/Refractions-Textur setzen
    if(dropColor.length() != 0) {
        _setDropColorTexture(dropColor);
    }
    else {
        Q_ASSERT(false);
    }

    //Optionen setzen
    _glassWidth = 1600;
    _glassHeight = 900;
    _numberOfBigNonTrailDrops = 0;

    //Speicher in Hashes reservieren, um
    _dropsSmall.reserve(Options::maxNumberDroplets);
    _dropsBig.reserve(Options::maxNumberNonTrailDrops + 300);

    //Zufallszahlengenerator seeden
    QTime timeObj;
    timeObj = QTime::currentTime();
    int msecs = timeObj.msecsSinceStartOfDay();
    //qDebug() << "Zeit seit Beginn des Tages in Millisekunden: " << msecs << " (für random seeding)";
    qsrand(msecs);

    //Timer connecten und starten
    _updateTimer = new QTimer(this);
    connect(_updateTimer, SIGNAL(timeout()),
            this, SLOT(update()));
    _updateTimer->start(1000/60); //Rendern ausgelegt für 60 fps -> daher 60hz updatefrequenz
}

void Raindrops::_setDropAlphaTexture(QString filename) {
    //(ausgelagerte) Hilfsfunktion zum Setzen der DropAlpha-Textur
    _dropAlphaTexture = new QOpenGLTexture(QImage(":/textures/" + filename).mirrored());
    _dropAlphaTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _dropAlphaTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    _dropAlphaTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    Q_ASSERT(_dropAlphaTexture->textureId() != 0); //Würde Fehler bedeuten
    _hasDropAlphaTexture = true;
    qDebug() << endl << "DropAlpha-Textur: " << filename << " geladen" << endl << endl;
}

void Raindrops::_setDropColorTexture(QString filename) {
    //(ausgelagerte) Hilfsfunktion zum Setzen der DropColor-Textur
    _dropColorTexture = new QOpenGLTexture(QImage(":/textures/" + filename).mirrored());
    _dropColorTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _dropColorTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    _dropColorTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    Q_ASSERT(_dropColorTexture->textureId() != 0); //Würde Fehler bedeuten
    _hasDropColorTexture = true;
    qDebug() << endl << "DropColor-Textur: " << filename << " geladen" << endl << endl;
}

void Raindrops::_spawnDroplet() {
    if(_dropsSmall.size() >= Options::maxNumberDroplets) return;
    //50px Rand
    unsigned int lookUp;
    do {
        int xPos = Options::upperSpawnBorderDroplets + qrand() % (_glassWidth  - Options::lowerSpawnBorderDroplets);
        int yPos = Options::upperSpawnBorderDroplets + qrand() % (_glassHeight - Options::lowerSpawnBorderDroplets);
        lookUp = _createUintPosHash(xPos, yPos);
        //wenn an dieser Stelle breits ein Tröpfchen ist -> erneut versuchen
    } while(_dropsSmall.contains(lookUp));

    unsigned short radius = Options::dropletsMinR + (qrand() %  int(Options::dropletsMaxR - Options::dropletsMinR));
    _dropsSmall.insert(lookUp, radius);
}

void Raindrops::_deleteDroplets(unsigned short const &locationX,
                                unsigned short const &locationY,
                                unsigned short const &radius) {
    unsigned int lookUp;
    unsigned short cleaningRadius = round(float(radius) * Options::dropletsCleaningRadiusMultiplier);

    //Umgebung durch iterieren und wenn kleiner als CleaningRadius entfernt dann löschen
    for(unsigned short xCheckPos = locationX - cleaningRadius; xCheckPos < locationX + cleaningRadius; xCheckPos++) {
        for(unsigned short yCheckPos = locationY + cleaningRadius; yCheckPos > locationY - cleaningRadius; yCheckPos--) {
            //x² + y² = r² Kreisdarstellung wenn kleiner als r dann löschen
            short diffX = locationX - xCheckPos;
            short diffY = locationY - yCheckPos;
            if(float((diffX*diffX) + (diffY*diffY)) <= (cleaningRadius*cleaningRadius)) {
                lookUp = _createUintPosHash(xCheckPos, yCheckPos);
                _dropsSmall.remove(lookUp);
            }
        }
    }
}

void Raindrops::_spawnDrop() {
    if(_numberOfBigNonTrailDrops >= Options::maxNumberNonTrailDrops) return;
    unsigned short xPos, yPos, radius;
    //Wenn kein Parent (==nullptr) dann komplett neuer Tropfen
    //spawn mit Werten von Options beschränkt
    xPos = Options::upperSpawnBorderDrops + qrand() % (_glassWidth  - Options::lowerSpawnBorderDrops);
    yPos = Options::upperSpawnBorderDrops + qrand() % (_glassHeight - Options::lowerSpawnBorderDrops);
    radius = Options::minR + (qrand() % int(Options::maxR - Options::minR));
    Drop newDrop = Drop(xPos, yPos, radius, nullptr);
    unsigned int hashValue = _createUintPosHash(xPos,yPos);
    _dropsBig.insert(hashValue, newDrop);
    _numberOfBigNonTrailDrops++;
}

void Raindrops::_updateDrops() {
    //qDebug() << "updateDrops";
    QList<unsigned int> keyList = _dropsBig.uniqueKeys();
    unsigned int lengthOfKeyList = keyList.length();
    for(unsigned int i = 0; i < lengthOfKeyList; i++) {

        unsigned int locationHash = keyList[i];
        Drop d = _dropsBig.take(locationHash);

        //smalldrops an alter Stelle löschen
        _deleteDroplets(d.posX, d.posY, d.radius);

        d.update();

        if(!d.killed) {
            /* Kollisionscheck mit anderen Tropfen - da BUGGY nicht verwendet
             *
            //Auf Kollision prüfen (_dropCombindedMaxRadius um Tropfen herum)
            unsigned int lookUp;
            for(unsigned short xCheckPos = d.posX - _dropCombindedMaxRadius*2; xCheckPos < d.posX + _dropCombindedMaxRadius*2; xCheckPos++) {
                for(unsigned short yCheckPos = d.posY + _dropCombindedMaxRadius*2; yCheckPos > d.posY - _dropCombindedMaxRadius*2; yCheckPos--) {
                    //ATM QUADRATISCHER CHECK
                    //-------------------
                    //Circulärer Check
                    // | | |
                    // V V V
                    //x² + y² = r² Kreisdarstellung wenn kleiner als r dann löschen
                    //short diffX = d.posX - xCheckPos;
                    //short diffY = d.posY - yCheckPos;
                    //if(float((diffX*diffX) + (diffY*diffY)) <= (d.radius+1*d.radius+1)) {
                        lookUp = _createUintPosHash(xCheckPos, yCheckPos);
                        //Wenn an der Stelle ein Tropfen gefunden wurde:
                        if(_dropsBig.find(lookUp) != _dropsBig.end()) {
                            Drop dropToCheck = _dropsBig.value(lookUp);
                            if(dropToCheck.isNew || dropToCheck.parent == &d) continue; //Gerade erst gespawned
                            short xDistance = dropToCheck.posX - d.posX;
                            short yDistance = dropToCheck.posY - d.posY;
                            short combindedRadii = dropToCheck.radius + d.radius; // - 1; //-1 damit sie näher an einander sein müssen
                            qDebug() << "x2: " << (xDistance*xDistance);
                            qDebug() << "y2: " << (yDistance*yDistance);
                            qDebug() << "r2: " << (combindedRadii*combindedRadii);
                            qDebug() << ((xDistance*xDistance) + (yDistance*yDistance) < (combindedRadii*combindedRadii));
                            if((xDistance*xDistance) + (yDistance*yDistance) < (combindedRadii*combindedRadii)) {
                                //=> können kombiniert werden
                                _dropsBig.remove(lookUp);
                                d = d.combineWith(dropToCheck);
                                //Update Keylist (weil Item gelöscht)
                                keyList = _dropsBig.uniqueKeys();
                                lengthOfKeyList = keyList.length();
                                qDebug() << "Länge der Liste: " << lengthOfKeyList;
                            }
                        }
                    //}
                }
            }
            */

            //Wenn Traildrop gespawnt werden soll
            if(d.willSpawn) {
                Drop newTrailDrop = d.produceTrail();
                if(newTrailDrop.radius <= 0) continue;
                _updatedDrops.push_back(newTrailDrop);
                d.willSpawn = false;
            }
            //Wieder einfügen mit neuer Pos
            _updatedDrops.push_back(d);
        }
        else {
            //Wenn nicht TrailDrop gekillt wurde;
            if(d.parent == nullptr) _numberOfBigNonTrailDrops--;
        }
    }
    //Neue Drops hinzufügen
    for(Drop newD : _updatedDrops) {
        _dropsBig.insert(_createUintPosHash(newD.posX, newD.posY), newD);
    }
    _updatedDrops.clear();
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
    //NOTE hier kontinuerliche ctm Änderungen
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
    int unifViewMatrix = 4;
    int unifModelMatrix = 8;

    //Matrix Berechnungen fertig nun shader konfigurieren
    _shader->bind();

    // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte, Normalen und Textur
    int attrVertices = 0;
    int attrNorms = 1;
    int attrTexCoords = 2;

    // Aktiviere die Verwendung der Attribute-Arrays
    _shader->enableAttributeArray(attrVertices);
    _shader->enableAttributeArray(attrNorms);
    Q_ASSERT(_model->hasTextureCoords());
    _shader->enableAttributeArray(attrTexCoords);

    // Fülle die Attribute-Buffer mit den korrekten Daten
    size_t model_stride = _model->stride(); //reduziert von 3 auf 1 Aufruf (implizit uint in int ist aktzeptabel - kein Werteverlust)
    _shader->setAttributeBuffer(attrVertices, GL_FLOAT, _model->vertOffset(), 4, model_stride); //VertexPositionen
    _shader->setAttributeBuffer(attrNorms, GL_FLOAT, _model->normOffset(), 4, model_stride); //VertexNormalen
    _shader->setAttributeBuffer(attrTexCoords, GL_FLOAT, _model->texCoordOffset(), 4, model_stride); //TexturCoordinaten

    //Alle Texturen einbinden und an shader übergeben
    Q_ASSERT(_hasDropAlphaTexture && _hasDropColorTexture && _hasTexture && _hasSecondTexture);
    _mainTexture->bind(0);
    _shader->setUniformValue("refractionBackground", 0);

    _secondTexture->bind(1);
    _shader->setUniformValue("refractionOverlay", 1);

    _dropAlphaTexture->bind(2);
    _shader->setUniformValue("dropAlpha", 2);

    _dropColorTexture->bind(3);
    _shader->setUniformValue("dropColor", 3);

    //Einstellungen machen
    glDepthMask(GL_FALSE); //z-Buffer auf readonly stellen
    glEnable(GL_BLEND); //Farbmischung aktivieren
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Farbmischmethode festlegen
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    _shader->setUniformValue(unifProjMatrix, projectionMatrix); //projektionsMatrix (const)
    _shader->setUniformValue(unifViewMatrix, viewMatrix); //viewMatrix ("const")

    //Big Drops zeichnen, unabhängige Parameter bei jedem DrawCall neu gesetzt
    for(Drop d : _dropsBig.values()) {
        //qDebug() << "XPOS: " << d.posX << " YPOS: " << d.posY << " SHRINK: " << d.shrinkage;
        ctm.translate(d.posX, d.posY, 0);
        ctm.scale(d.radius);
        ctm.scale(1.0f, 1.5f);
        _shader->setUniformValue(unifModelMatrix, ctm); //modelMatrix (immer abhängig vom gerade zu rendernden RenderableObject)
        QVector2D dropLocationLowerLeftCorner = QVector2D((float)d.posX - float(d.radius), (float)d.posY - float(d.radius)*1.5f);
        //qDebug() << "NormDrop - x: " << normalizedDropLocation.x() << " y: " << normalizedDropLocation.y();
        _shader->setUniformValue("dropLocation", dropLocationLowerLeftCorner);
        _shader->setUniformValue("planeWidth", _glassWidth);
        _shader->setUniformValue("planeHeight", _glassHeight);
        _shader->setUniformValue("dropRadius", (int)d.radius);
        glDrawElements(GL_TRIANGLES, _model->iboLength(), GL_UNSIGNED_INT, 0);
        ctm = combindedCTM; //zurücksetzen
    }

    //Small Drops zeichnen, unabhängige Parameter bei jedem DrawCall neu gesetzt
    for(unsigned int locationHash : _dropsSmall.uniqueKeys()) {
        unsigned short xPos = _retrieveXValueFromHash(locationHash);
        unsigned short yPos = _retrieveYValueFromHash(locationHash);
        ctm.translate(xPos, yPos, 0);
        int radius = _dropsSmall.value(locationHash);
        ctm.scale(radius);
        ctm.scale(1.0f, 1.5f);
        _shader->setUniformValue(unifModelMatrix, ctm); //modelMatrix (immer abhängig vom gerade zu rendernden RenderableObject)
        QVector2D dropLocationLowerLeftCorner = QVector2D((float) xPos - float(radius), (float) yPos - float(radius)*1.5f);
        _shader->setUniformValue("dropLocation", dropLocationLowerLeftCorner);
        _shader->setUniformValue("planeWidth", _glassWidth);
        _shader->setUniformValue("planeHeight", _glassHeight);
        _shader->setUniformValue("dropRadius", radius);
        glDrawElements(GL_TRIANGLES, _model->iboLength(), GL_UNSIGNED_INT, 0);
        ctm = combindedCTM; //zurücksetzen
    }

    // Deaktiviere die Verwendung der Attribute-Arrays
    _shader->disableAttributeArray(attrVertices);
    _shader->disableAttributeArray(attrNorms);
    _shader->disableAttributeArray(attrTexCoords);

    //Shader lösen
    _shader->release();

    // Löse die Texturen aus dem OpenGL-Kontext
    _mainTexture->release();
    _secondTexture->release();
    _dropAlphaTexture->release();
    _dropColorTexture->release();

    //VBO und IBO vom Kontext lösen
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Einstellungen zurücksetzen
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

    //TODO Spawnraten hier anpassen
    if(_counter > 300 && _counter % 120 == 0) _spawnDrop();
    if(_counter % 2) _spawnDroplet();
    _counter++;
}

void Raindrops::update() {
    //qDebug() << "Update called";
    _updateDrops();
}


