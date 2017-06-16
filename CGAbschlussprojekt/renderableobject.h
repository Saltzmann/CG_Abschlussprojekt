#ifndef RENDERABLEOBJECT_H
#define RENDERABLEOBJECT_H

//Generelle Includes
#include <QObject>
#include <QOpenGLExtraFunctions>
//Debug Includes
#include <QDebug>
//Datenstruktur Includes
#include <QVector>
#include <QMatrix4x4>
//Shader Includes
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
//externe includes
#include <model.h>

//Shader-Code defines
#define SHADER_DEFAULT 0x0001
#define SHADER_TEXTURE 0x0002
#define SHADER_NORMALS 0x0003


class RenderableObject : public QObject, public QOpenGLExtraFunctions {
    Q_OBJECT
private:
    //Referenzen bilden 'Render-Konfiguration'
    //Geometrie
    Model* _model;
    bool _modelHasTextureCoords; //für Performanz nur 1x
    //QMatrix4x4 _ctm; //später hier sinnvoll?

    //Optik
    int _shaderFlag;
    QOpenGLShaderProgram* _shader;
    QOpenGLShaderProgram* _secondShader;
    QVector4D _baseColor;
    QOpenGLTexture* _mainTexture;
    QOpenGLTexture* _secondTexture;
    //QOpenGLTexture* _thirdTexture; ?

    //Rendering Counter
    float _renderingCounter1;
    float _renderingCounter2;

    //Bool-Flags ob überhaupt Texturen vorhanden sind
    bool _hasTexture;
    bool _hasSecondTexture;    

    //Abhängige RenderableObjects
    QVector<RenderableObject*> _children;

    //Temp. Lösung - statische Objekte -> speichert eigene TransformationsMatrix
    QMatrix4x4 _myCTM;

    //(ausgelagerte) Hilfsfunktionen - hauptsächlich zur Übersichtlichkeit
    void _setMainTexture(QString filename);
    void _setSecondTexture(QString filename);

    //verschiedene Renderfunktionen
    void _renderWithDefaultShader(QMatrix4x4 const &parentCTM,
                                  QMatrix4x4 const &viewMatrix,
                                  QMatrix4x4 const &projectionMatrix);
    void _renderWithTextureShader(QMatrix4x4 const &parentCTM,
                                  QMatrix4x4 const &viewMatrix,
                                  QMatrix4x4 const &projectionMatrix);
    void _renderWithNormalsShader(QMatrix4x4 const &parentCTM,
                               QMatrix4x4 const &viewMatrix,
                               QMatrix4x4 const &projectionMatrix);
public:
    //Konstruktor
    RenderableObject(QMatrix4x4 ctm,
                     Model* model,
                     int shaderTypeFlag,
                     QOpenGLShaderProgram* shader,
                     QOpenGLShaderProgram* secondShader,
                     QVector4D const &baseColor,
                     QString const &mainTextureFileName = "",
                     QString const &secondTextureFileName = "");

    //Allgemeine Renderfunktion
    virtual void render(QMatrix4x4 const &parentCTM,
                        QMatrix4x4 const &viewMatrix,
                        QMatrix4x4 const &projectionMatrix);

    //Tree-Aufbau-Funktion
    void addChild(RenderableObject* child);

    //Info Getter
    bool hasTexture() const;
    bool hasSecondTexture() const;
public slots:
    //void update(); //später wahrscheinlich / vielleicht
};

#endif // RENDERABLEOBJECT_H
