#ifndef CUSTOMOPENGLWIDGET_H
#define CUSTOMOPENGLWIDGET_H

//Generelle / Vererbungs Includes
#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLContext>
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
#include <QMoveEvent>
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
#include <raindrops.h>

class CustomOpenGLWidget : public QOpenGLWidget,
                           protected QOpenGLFunctions {
    Q_OBJECT
protected:
    // A T T R I B U T E

    //OpenGL Context
    QOpenGLContext* _context;

    //Kamera
    Camera* _myCamera;

    //Models
    Model* _planeModel;
    Model* _dropsModel;

    //Zu rendernde Objekte
    QVector<RenderableObject*> _myRenderables;

    //Shader
    QOpenGLShaderProgram* _defaultShaderProgram;
    QOpenGLShaderProgram* _textureShaderProgram;
    QOpenGLShaderProgram* _normalDrawShaderProgram;
    QOpenGLShaderProgram* _dropShaderProgram;

    //GUI Hilfsvariablen
    unsigned int _fpsCounter;
    float _actualFPS;
    QTimer* _fpsTimer;

    //Debug Logger für erweiterte Fehlermeldungen
    QOpenGLDebugLogger* _debugLogger;

    // M E T H O D E N

    //Hilfsfunktionen
    void _buildGeometry();
    void _createRenderables();
public:
    // A T T R I B U T E
    // -

    // M E T H O D E N

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
    void moveEvent(QMoveEvent *event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
public slots:
    //interne Slots
    void onMessageLogged(QOpenGLDebugMessage message);
    void cameraIsUpdated();

    //GUI Slots
    void resetFPSCounter();
    void counter1Changed(double value);
    void counter2Changed(double value);
    void recieveMouseCaptured(bool captured);
signals:
    //GUI Signals
    void sendFPSValue(int value);
    void mouseCaptured(bool captured);
};

#endif // CUSTOMOPENGLWIDGET_H
