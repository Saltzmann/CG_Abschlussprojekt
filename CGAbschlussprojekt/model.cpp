#include "model.h"

Model::Model(QString const &modelFileName) {
    initializeOpenGLFunctions();
    //Initiales Model aus Datei laden
    if(modelFileName.length() != 0) {
        loadModelFromFile(modelFileName);
    }
}

void Model::_initializeModelData(QString const &modelFileName) {
    // Lade Modell aus Datei
    ModelLoader model;
    QDir currentDir;
    std::string absolutePath = currentDir.canonicalPath().toStdString() + "/" + modelFileName.toStdString();
    bool res = model.loadObjectFromFile(absolutePath);

    _hasTextureCoords = model.hasTextureCoordinates();
    qDebug() << endl << " - - - - - Model loading... - - - - -";
    qDebug() << "File: " << modelFileName << " hat Textur-Koordinaten = " << _hasTextureCoords;

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
        qDebug() << "Model laden erfolgreich!" << endl << endl;
    }
    else {
        // Modell konnte nicht geladen werden
        qDebug() << "Model laden fehlgeschlagen!" << endl << endl;
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

    //Laden als erfolgreich markieren
    _hasModelLoaded = true;
}

void Model::_setUpBuffers(QString const &modelFileName) {
    //VBO und IBO zum ersten mal erstellen und für die Datenspeicherung
    //an den aktuellen Kontext binden

    glGenBuffers(1, &_vboHandle);
    glGenBuffers(1, &_iboHandle);

    glBindBuffer(GL_ARRAY_BUFFER, _vboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboHandle);

    _initializeModelData(modelFileName);

    //Model muss nun geladen sein
    if(!_hasModelLoaded) throw new std::exception();

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _vboLength, _vboData, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _iboLength, _indexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::loadModelFromFile(QString const &modelFileName)  {
    _hasModelLoaded = false;
    _setUpBuffers(modelFileName);
}

//Getter die nicht kopieren und auch Manipulation zulassen
GLuint Model::vboHandle() const {
    return _vboHandle;
}
GLuint Model::iboHandle() const {
    return _iboHandle;
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
        qDebug() << endl << " - - - - - VBOData - - - - - ";
        if(_hasTextureCoords) {
            qDebug() << endl << "Model mit Textur-Koordinaten" << endl;
        }
        else {
            qDebug() << endl << "Model ohne Textur-Koordinaten" << endl;
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
        qDebug() << endl << " - - - - - IndexData - - - - - " << endl;
        for(unsigned int i = 0; i < _iboLength; i++) {
            qDebug() << "_indexData[" << i << "] : " << _indexData[i];
        }
    }
}
