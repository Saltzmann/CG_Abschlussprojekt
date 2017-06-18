#ifndef MODEL_H
#define MODEL_H

//Generelle Includes
#include <QObject>
#include <QDir>
#include <QOpenGLExtraFunctions>
//Debug Includes
#include <QDebug>
//Datenstrukturen Includes
#include <QVector>
#include <QVector2D>
#include <QVector3D>
//Geometrie / Vertex / Mesh Includes
//#include <QOpenGLBuffer>
//externe includes
#include <modelloader.h>

//custom defines
#define NUMBER_OF_BUFFERS 4
#define VERTEX_BUFFER 0
#define NORMAL_BUFFER 1
#define TEXCOORD_BUFFER 2
#define INDEX_BUFFER 3

class Model : public QObject, public QOpenGLExtraFunctions {
    Q_OBJECT
private:
    //Model und Buffer-Object bezogene Date
    GLuint _VAO; //Vertex Array Object
    GLuint _Buffers[4];

    QVector<QVector3D> _vertices;
    QVector<QVector3D> _normals;
    QVector<QVector2D> _texCoords;
    QVector<GLuint>    _indices;

    GLfloat* _vboData;
    GLuint* _indexData;
    size_t _vboLength;
    size_t _iboLength;
    unsigned short _vertOffset;
    unsigned short _normOffset;
    unsigned short _texCoordOffset;
    size_t _stride;

    GLuint _vboHandle;
    GLuint _iboHandle;

    bool _hasTextureCoords;

    //Bool-Flag ob Model-Laden erfolgreich war
    bool _hasModelLoaded;

    //(ausgelagerte) Hilfsfunktionen - hauptsächlich zur Übersichtlichkeit
    void _initializeModelData(QString const &modelFileName);
    void _setUpBuffers(QString const &modelFileName);
public:
    //Konstruktor
    Model(QString const &modelFileName = "");

    //Lade Funktionen
    void loadModelFromFile(QString const &modelFileName);

    //Getter die nicht kopieren und auch Manipulation zulassen
    GLfloat* vboData();
    GLuint* indexData();

    //Einfache Getter, die nur kopieren
    GLuint vaoHandle() const;
    int numIndices() const;

    GLuint vboHandle() const;
    GLuint iboHandle() const;
    size_t vboLength() const;
    size_t iboLength() const;
    unsigned short vertOffset() const;
    unsigned short normOffset() const;
    unsigned short texCoordOffset() const;
    size_t stride() const;
    bool hasTextureCoords() const;
    bool hasModelLoaded() const;

    //Debug-Print-funktionen
    void printVBOData();
    void printIBOData();
};

#endif // MODEL_H
