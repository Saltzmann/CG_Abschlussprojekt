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

    if (res) { 
        unsigned int* sizes = nullptr; //sizes = [vertArraySize, normArraySize, texCArraySize, indexArraySize] (4)
        sizes = model.genSOA(_vertices, _normals, _texCoords, _indices); //"leere" pointer rein -> arrays raus

        if(sizes == nullptr) throw new std::exception;

        //Vertex Buffer aufsetzen
        glBindBuffer(GL_ARRAY_BUFFER, _Buffers[VERTEX_BUFFER]);

        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices[0]) * sizes[0], _vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);


        //"Normals" Buffer aufsetzen
        glBindBuffer(GL_ARRAY_BUFFER, _Buffers[NORMAL_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_normals[0]) * sizes[1], _normals, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);

        //"Texture-Coordinate" Buffer aufsetzen
        glBindBuffer(GL_ARRAY_BUFFER, _Buffers[TEXCOORD_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_texCoords[0]) * sizes[2], _texCoords, GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(TEXCOORD_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);

        //"Index" Buffer aufsetzen
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _Buffers[INDEX_BUFFER]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * sizes[3], _indices, GL_STATIC_DRAW);

        qDebug() << "Model laden erfolgreich!" << endl << endl;
    }
    else {
        // Modell konnte nicht geladen werden
        qDebug() << "Model laden fehlgeschlagen!" << endl << endl;
        _hasModelLoaded = false;
        Q_ASSERT(false); //gewollter Programmabbruch
    }   

    //Laden als erfolgreich markieren
    _hasModelLoaded = true;
}

void Model::_setUpBuffers(QString const &modelFileName) {
    //Generiere VAO
    glGenVertexArrays(1, &_VAO);
    //Binde VAO
    glBindVertexArray(_VAO);
    
    //"unter-Buffer" erstellen
    glGenBuffers(NUMBER_OF_BUFFERS, _Buffers);

    _initializeModelData(modelFileName);

    //Model muss nun geladen sein
    if(!_hasModelLoaded) throw new std::exception();

    //Löse VAO
    glBindVertexArray(0);
}

void Model::loadModelFromFile(QString const &modelFileName)  {
    //Model als nicht mehr geladen markieren
    _hasModelLoaded = false;

    //Löscht VAO nicht gebundene Buffer
    glDeleteVertexArrays(1, &_VAO);
    //Gebundene Buffer löschen
    glDeleteBuffers(4, _Buffers);

    //alte Daten komplett rausschmeißen, falls noch vorhanden
    delete[] _vertices;
    _vertices = nullptr;
    delete[] _normals;
    _normals = nullptr;
    delete[] _texCoords;
    _texCoords = nullptr;
    delete[] _indices;
    _indices = nullptr;

    //Neue Daten laden
    _setUpBuffers(modelFileName);
}

//Getter die nicht kopieren und auch Manipulation zulassen
GLuint Model::vaoHandle() const {
    return _VAO;
}
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
int Model::numIndices() const {
    return _indices.length();
}
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
