#include "model.h"

Model::Model(QString const &modelFileName) : _vbo(QOpenGLBuffer::VertexBuffer),
                                             _ibo(QOpenGLBuffer::IndexBuffer) {
    //Initiales Model aus Datei laden
    if(modelFileName.length() != 0) {
        loadModelFromFile(modelFileName);
    }
}

void Model::_initializeVBOs(QString const &modelFileName) {
    // Lade Modell aus Datei
    ModelLoader model;
    QDir currentDir;
    //SoonTM nächster Versuch für einen relativen Pfad
    //qDebug() << "CurrentDir CanonicalPath = " << currentDir.canonicalPath();
    std::string absolutePath = currentDir.canonicalPath().toStdString() + "/" + modelFileName.toStdString();
    bool res = model.loadObjectFromFile(absolutePath);
    //bool res = model.loadObjectFromFile("C:/Users/Tobias/Documents/GitHub/CG_Abschlussprojekt/CGAbschlussprojekt/sphere_high.obj");

    _hasTextureCoords = model.hasTextureCoordinates();
    qDebug() << "File: " << modelFileName << " hat Textur-Koordinaten = " << _hasTextureCoords;
    //Q_ASSERT(model.hasTextureCoordinates()); //wenn einkommentiert MUSS das Model Texturkoordinaten haben (auch wenn später abgefangen)
    if (res) { //Wenn erfolgreich, generiere VBO und Index-Array
        // Frage zu erwartende Array-Längen ab
        _vboLength = model.lengthOfVBO();
        _iboLength = model.lengthOfIndexArray();
        // Initialisiere VBO und Index-Array
        _vboData = new GLfloat[_vboLength];
        _indexData = new GLuint[_iboLength];
        //Generiere VBO und IBO Data mit vertices, normals, texCoords
        model.genVBO(_vboData);
        model.genIndexArray(_indexData);
        qDebug() << "Model laden erfolgreich!";
    }
    else {
        // Modell konnte nicht geladen werden
        qDebug() << "Model laden fehlgeschlagen!";
        _hasModelLoaded = false;
        Q_ASSERT(false); //gewollter Programmabbruch
    }
    //Versatz und Schrittweiten-Anpassung mit und ohne Texturkoordinaten
    if(_hasTextureCoords) {
        _vertOffset = 0;
        _normOffset = _vertOffset + 4 * sizeof(GLfloat);
        _texCoordOffset = _vertOffset + 8 * sizeof(GLfloat);
        _stride = 12 * sizeof(GLfloat);
    }
    else {
        _vertOffset = 0;
        _normOffset = _vertOffset + 4 * sizeof(GLfloat);
        _texCoordOffset = 0;
        _stride = 8 * sizeof(GLfloat);
    }

    //Schreibe Daten in Buffer-Objekte (unsigned int to int implizit ist ok hier)
    _vbo.allocate(_vboData, sizeof(GLfloat) * _vboLength);
    _ibo.allocate(_indexData, sizeof(GLuint) * _iboLength);

    //Laden als erfolgreich markieren
    _hasModelLoaded = true;
}

void Model::_fillBuffers(QString const &modelFileName) {
    //VBO und IBO zum ersten mal erstellen und für die Datenspeicherung
    //an den aktuellen Kontext binden

    _vbo.create();
    _ibo.create();

    _vbo.bind();
    _ibo.bind();

    _vbo.setUsagePattern(QOpenGLBuffer::StaticDraw); //TODO zu ändern später bei uns
    _ibo.setUsagePattern(QOpenGLBuffer::StaticDraw); //static draw optimiert für keine Änderungen an VBO dynamic für Veränderungen

    _initializeVBOs(modelFileName);

    _vbo.release();
    _ibo.release();
}

void Model::loadModelFromFile(QString const &modelFileName)  {
    if(_vbo.isCreated()) {
        _vbo.destroy();
    }
    if(_ibo.isCreated()) {
        _ibo.destroy();
    }
    _hasModelLoaded = false;
    _fillBuffers(modelFileName);
}

//Getter die nicht kopieren und auch Manipulation zulassen
QOpenGLBuffer* Model::getVBOBufferPtr() {
    return &_vbo;
}
QOpenGLBuffer* Model::getIBOBufferPtr() {
    return &_ibo;
}
GLfloat* Model::vboData() {
    return _vboData;
}
GLuint* Model::indexData() {
    return _indexData;
}

//Einfache Getter, die nur kopieren
size_t Model::vboLength() const {
    return _vboLength;
}
size_t Model::iboLength() const {
    return _iboLength;
}
unsigned short Model::vertOffset() const {
    return _vertOffset;
}
unsigned short Model::normOffset() const {
    return _normOffset;
}
unsigned short Model::texCoordOffset() const {
    return _texCoordOffset;
}
size_t Model::stride() const {
    return _stride;
}
bool Model::hasTextureCoords() const {
    return _hasTextureCoords;
}
bool Model::hasModelLoaded() const {
    return _hasModelLoaded;
}

void Model::printVBOData() {
    if(_hasModelLoaded) {
        if(_hasTextureCoords) {
            qDebug() << "Model mit Textur-Koordinaten";
        }
        else {
            qDebug() << "Model ohne Textur-Koordinaten";
        }
        for(unsigned int i = 0; i < _vboLength; i++) {
            switch(i % 12) {
            case  0:
                qDebug() << "_vboData[" << i << "] : " << "V.x " << _vboData[i];
                break;
            case  1:
                qDebug() << "_vboData[" << i << "] : " << "V.y " << _vboData[i];
                break;
            case  2:
                qDebug() << "_vboData[" << i << "] : " << "V.z " << _vboData[i];
                break;
            case  3:
                qDebug() << "_vboData[" << i << "] : " << "V.w " << _vboData[i];
                break;
            case  4:
                qDebug() << "_vboData[" << i << "] : " << "N.x " << _vboData[i];
                break;
            case  5:
                qDebug() << "_vboData[" << i << "] : " << "N.y " << _vboData[i];
                break;
            case  6:
                qDebug() << "_vboData[" << i << "] : " << "N.z " << _vboData[i];
                break;
            case  7:
                qDebug() << "_vboData[" << i << "] : " << "N.w " << _vboData[i];
                break;
            case  8:
                qDebug() << "_vboData[" << i << "] : " << "T.x " << _vboData[i];
                break;
            case  9:
                qDebug() << "_vboData[" << i << "] : " << "T.y " << _vboData[i];
                break;
            case 10:
                qDebug() << "_vboData[" << i << "] : " << "T.z " << _vboData[i];
                break;
            case 11:
                qDebug() << "_vboData[" << i << "] : " << "T.w " << _vboData[i];
                break;
            }
        }
    }
}

void Model::printIBOData() {
    if(_hasModelLoaded) {
        for(unsigned int i = 0; i < _iboLength; i++) {
            qDebug() << "_indexData[" << i << "] : " << _indexData[i];
        }
    }
}
