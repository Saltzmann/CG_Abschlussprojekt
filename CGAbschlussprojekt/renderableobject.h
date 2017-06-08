#ifndef RENDERABLEOBJECT_H
#define RENDERABLEOBJECT_H

//Generelle Includes
#include <QObject>
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

class RenderableObject : public QObject {
    Q_OBJECT
private:
    //Referenzen bilden 'Render-Konfiguration'
    //Geometrie
    Model* _model;
    //QMatrix4x4 _ctm; //später hier sinnvoll?
    //Optik
    QOpenGLShaderProgram* _shader;
    QOpenGLTexture* _mainTexture;
    QOpenGLTexture* _secondTexture;
    //Bool-Flags ob überhaupt Texturen vorhanden sind
    bool _hasTexture;
    bool _hasSecondTexture;
    //QOpenGLTexture* _thirdTexture; ?
    //Abhängige RenderableObjects
    QVector<RenderableObject*> _children;
    //(ausgelagerte) Hilfsfunktionen - hauptsächlich zur Übersichtlichkeit
    void _setMainTexture(QString filename);
    void _setSecondTexture(QString filename);
public:
    //Konstruktor
    RenderableObject(Model* model,
                     QOpenGLShaderProgram* shader,
                     QString const &mainTextureFileName = "",
                     QString const &secondTextureFileName = "");
    virtual void render(QMatrix4x4 ctm,
                        QMatrix4x4 const &viewMatrix,
                        QMatrix4x4 const &projectionMatrix);
public slots:
    //void update(); //später wahrscheinlich
};

#endif // RENDERABLEOBJECT_H
