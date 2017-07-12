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
//externe includes
#include <modelloader.h>

class Model : public QObject, public QOpenGLExtraFunctions {
    Q_OBJECT
private:
    // A T T R I B U T E

    //Bufferdaten
    GLfloat* _vboData;
    GLuint* _indexData;
    //Bufferlängen
    size_t _vboLength;
    size_t _iboLength;
    //Buffer-Organisations Infos
    unsigned short _vertOffset;
    unsigned short _normOffset;
    unsigned short _texCoordOffset;
    size_t _stride;
    //Buffer handles
    GLuint _vboHandle;
    GLuint _iboHandle;

    //Bool-Flag, ob Model überhaupt Texturkoordinaten hat
    bool _hasTextureCoords;

    //Bool-Flag, ob Model-Laden erfolgreich war
    bool _hasModelLoaded;

    // M E T H O D E N

    //(ausgelagerte) Hilfsfunktionen - hauptsächlich zur Übersichtlichkeit
    void _initializeModelData(QString const &modelFileName);
    void _setUpBuffers(QString const &modelFileName);
public:
    // A T T R I B U T E
    // -

    // M E T H O D E N

    //Parameter-Konstruktor
    Model(QString const &modelFileName = "");

    //Lade Funktionen
    void loadModelFromFile(QString const &modelFileName);

    //Getter die nicht kopieren und auch Manipulation zulassen
    GLfloat* vboData();
    GLuint* indexData();

    //Einfache Getter, die nur kopieren
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
