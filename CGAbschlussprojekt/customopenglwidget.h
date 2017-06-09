#ifndef CUSTOMOPENGLWIDGET_H
#define CUSTOMOPENGLWIDGET_H

//Generelle / Vererbungs Includes
#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
//Compatibility includes
#include <QOpenGLFunctions>
//Debug Includes
#include <QDebug>
#include <QOpenGLDebugLogger>
#include <QMetaEnum>
//Mouse / Keyboard Input Includes
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
//Sim/Time includes
#include <QTimer>
//Datenstruktur Includes
#include <QVector>
//Shader Includes
#include <QOpenGLShaderProgram>
//externe includes
#include <camera.h>
#include <model.h>
#include <renderableobject.h>


class CustomOpenGLWidget : public QOpenGLWidget,
                           protected QOpenGLFunctions {
    Q_OBJECT
protected:
    //Kamera
    Camera* _myCamera;

    //Models
    Model* _cubeModel;
    Model* _floorModel;
    Model* _sphereModel;
    //...

    //Zu rendernde Objekte
    QVector<RenderableObject*> _myRenderables;

    //Shader
    QOpenGLShaderProgram* _defaultShaderProgram;

    //GUI Hilfsvariablen
    unsigned int _fpsCounter;
    float _actualFPS;
    QTimer* _fpsTimer;

    //Debug Logger für erweiterte Fehlermeldungen
    QOpenGLDebugLogger* _debugLogger;

    //Hilfsfunktionen
    void _buildGeometry();
    void _createRenderables();
    void _makeFloor(unsigned short sideLengthInVertices);
public:
    //Konstruktor
    CustomOpenGLWidget(QWidget *parent);
    //Destruktor (vermutlich nur fürs Gewissen)
    ~CustomOpenGLWidget();
    //QT OpenGL Overrides
    void initializeGL();
    void paintGL();

    //Mouse und Keyboard-Event Overrides
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
public slots:
    //interne Slots
    void onMessageLogged(QOpenGLDebugMessage message);
    void cameraIsUpdated();

    //GUI Slots
    void resetFPSCounter();
signals:
    //GUI Signals
    void sendFPSValue(int value);
};

#endif // CUSTOMOPENGLWIDGET_H
