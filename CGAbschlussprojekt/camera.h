#ifndef CAMERA_H
#define CAMERA_H

//Generelle Includes
#include <QObject>
#include <QOpenGLWidget>
//Debug Includes
#include <QDebug>
//Datenstruktur Includes
#include <QPoint>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
//Mouse / Keyboard Input Includes
#include <QCursor>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QMoveEvent>

//Define Regler, für schnellen Zugriff ohne im Code zu wühlen
#define UPDATE_RATE 60
#define INITIAL_CAMERA_OFFSET_Z 2.0f
#define INITIAL_CAMERA_OFFSET_Y 0.f
#define INITIAL_SPEED_FACTOR 0.05f
#define TICKRATE 1000/UPDATE_RATE
#define CAMERA_TURN_SPEED 1.f/5.f

class Camera : public QObject {
    Q_OBJECT
private:
    QOpenGLWidget* _parent;
    //Kameraführungs-Variablen
    float _speedFactor;
    QVector3D _viewOffset;
    QVector3D _viewDirection;
    QVector3D const _upVector;
    QVector3D _rightVector;

    bool _mouseCaptured;

    QPoint _dragStartPos;
    QPoint _windowPos;
    QPoint _midWindowPos;
public:
    //Konstruktor
    Camera(QOpenGLWidget* parent);

    //Bewegungsfunktionen
    inline void moveForward();
    inline void moveBackward();
    inline void moveRight();
    inline void moveLeft();
    inline void moveUp();
    inline void moveDown();
    inline void resetViewDirection();
    inline void resetToDefault();
    inline void turn(QVector2D mouseDelta);
    inline void alterSpeed(float modificator);

    //Kontrollfunktionen
    void freeOrCatchMouse();

    //Extern aufgerufene Update Funktionen für die Attribute
    bool mousePosUpdate(QMouseEvent* event);
    bool mouseWheelUpdate(QWheelEvent* event);
    bool keyPressUpdate(QKeyEvent* event);
    bool windowPosUpdate(QMoveEvent* event);
    bool mousePressUpdate(QMouseEvent *event);
    bool mouseReleaseUpdate(QMouseEvent *event);

    //Return Funktion der Kamera Klasse, die zum Rendern verwendet wird
    void getViewMatrix(QMatrix4x4 &inputMatrix) const;
signals:
    //Notification für updates des Widgets
    void isUpdated();
    void mouseCaptured(bool captured);
};

#endif // CAMERA_H
