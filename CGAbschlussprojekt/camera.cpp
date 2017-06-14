#include "camera.h"

Camera::Camera(QOpenGLWidget *parent) : _upVector(0.f, 1.f, 0.f) {
    //Parent und Fensterpositionen initialisieren
    _parent = parent;
    _windowPos = _parent->pos();
    _midWindowPos = QPoint(_windowPos.x() + _parent->width()/2, _windowPos.y() + _parent->height()/2);
    //Kamera initialisieren
    _speedFactor = INITIAL_SPEED_FACTOR;
    _viewOffset = QVector3D(0.f, INITIAL_CAMERA_OFFSET_Y, INITIAL_CAMERA_OFFSET_Z);
    _viewDirection = QVector3D(0.f, 0.f, -1.f);
    _rightVector = QVector3D(1.f, 0.f, 0.f);
    emit isUpdated();
}

void Camera::moveForward() {
    _viewOffset += _viewDirection * _speedFactor;
    emit isUpdated();
}

void Camera::moveBackward() {
    _viewOffset -= _viewDirection * _speedFactor;
    emit isUpdated();
}

void Camera::moveLeft() {
    _viewOffset -= _rightVector * _speedFactor;
    emit isUpdated();
}

void Camera::moveRight() {
    _viewOffset += _rightVector * _speedFactor;
    emit isUpdated();
}

void Camera::moveUp() {
    _viewOffset += _upVector * _speedFactor;
    emit isUpdated();
}

void Camera::moveDown() {
    _viewOffset -= _upVector * _speedFactor;
    emit isUpdated();
}

void Camera::resetViewDirection() {
    _viewDirection = QVector3D(0.f, 0.f, -1.f);
    emit isUpdated();
}

void Camera::resetToDefault() {
    _viewOffset = QVector3D(0.f, INITIAL_CAMERA_OFFSET_Y, INITIAL_CAMERA_OFFSET_Z);
    _viewDirection = QVector3D(0.f, 0.f, -1.f);
    _speedFactor = INITIAL_SPEED_FACTOR;
    emit isUpdated();
}

void Camera::turn(QVector2D mouseDelta) {
    //TODO vermutlich redundant - hatte ich mir mal was bei gedacht...
    _rightVector = QVector3D::crossProduct(_viewDirection, _upVector);

    QMatrix4x4 rotationMatrix;
    rotationMatrix.setToIdentity();
    rotationMatrix.rotate(-mouseDelta.x() * CAMERA_TURN_SPEED, _upVector);
    rotationMatrix.rotate(-mouseDelta.y() * CAMERA_TURN_SPEED, _rightVector);

    _viewDirection = QVector3D(rotationMatrix * _viewDirection.toVector4D()).normalized();
    _rightVector = QVector3D::crossProduct(_viewDirection, _upVector).normalized();
    emit isUpdated();
}

void Camera::alterSpeed(float modificator) {
    _speedFactor += modificator;
    if(_speedFactor < 0) { //Werte unter 0 würden die Steuerung invertieren daher reset auf 0
        _speedFactor = 0.f;
    }
}

bool Camera::mousePosUpdate(QMouseEvent* event) {
    QPoint currentMousePosition = event->globalPos();
    QVector2D mouseDelta =  QVector2D(currentMousePosition - _midWindowPos);
    if(mouseDelta.length() == 0.f) {
        return false;
    }
    this->turn(mouseDelta);
    QCursor::setPos(_midWindowPos);
    return true;
}

bool Camera::mouseWheelUpdate(QWheelEvent* event) {
    int numDegrees = (event->angleDelta() / 8).y(); // durch 8 weil dann Angabe in Grad (* -1) für speed + nach oben

    if (numDegrees != 0) {
       int numSteps = numDegrees / 15; //Jeder Step = 15°
       //0.05 als Modifikator damit man nicht zu schnell zoomt
       //Wert ist auf Touchpad ausgelegt, eventuell TODO für Mausrad anpassen
       this->alterSpeed(float(numSteps) * 0.05f);
       return true;
    }
    return false;
}

bool Camera::windowPosUpdate(QMoveEvent *event) {
    //Falls Positionsänderung
    if(event->oldPos() != _windowPos) {
        _windowPos = event->pos();
        //Update midWindowPos
        _midWindowPos = QPoint(_windowPos.x() + _parent->width()/2, _windowPos.y() + _parent->height()/2);
        return true;
    }
    return false;
}

bool Camera::keyPressUpdate(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_W: //nach vorne
        this->moveForward();
        break;
    case Qt::Key_S: //nach hinten
        this->moveBackward();
        break;
    case Qt::Key_D: //nach rechts (strafe)
        this->moveRight();
        break;
    case Qt::Key_A: //nach links (strafe)
        this->moveLeft();
        break;
    case Qt::Key_R: //nach oben (strafe)
        this->moveUp();
        break;
    case Qt::Key_F: //nach unten (strafe)
        this->moveDown();
        break;
    case Qt::Key_N: //Nur Blickrichtung zurücksetzen
        this->resetViewDirection();
        break;
    case Qt::Key_B: //Kamera kommplett zurücksetzen
        this->resetToDefault();
        break;
    default:
        return false;
    }
    return true;
}

void Camera::getViewMatrix(QMatrix4x4 &inputMatrix) const {
    //Matrix wird als Referenz verändert
    inputMatrix.setToIdentity();
    inputMatrix.lookAt(_viewOffset, _viewOffset + _viewDirection, _upVector);
}
