#include "customopenglwidget.h"

CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent){
    //Shader initialisieren
    _defaultShaderProgram = new QOpenGLShaderProgram();
    _textureShaderProgram = new QOpenGLShaderProgram();
    _meltingShaderProgram = new QOpenGLShaderProgram();
    _normalDrawShaderProgram = new QOpenGLShaderProgram();

    //Keyboard und Mouse Input Einstellungen
    this->setCursor(Qt::BlankCursor);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    //Debug Output Versionsnummer etc.
    QSurfaceFormat fmt = this->format();
    qDebug().nospace() << "OpenGL " << fmt.majorVersion() << "." << fmt.minorVersion();
    qDebug().noquote() << "Profile:" << QMetaEnum::fromType<QSurfaceFormat::OpenGLContextProfile>().valueToKey(fmt.profile());
    qDebug().noquote() << "Options:" << QMetaEnum::fromType<QSurfaceFormat::FormatOption>().valueToKeys(fmt.options());
    qDebug().noquote() << "Renderable Type:" << QMetaEnum::fromType<QSurfaceFormat::RenderableType>().valueToKey(fmt.renderableType());
    qDebug().noquote() << "Swap Behavior:" << QMetaEnum::fromType<QSurfaceFormat::SwapBehavior>().valueToKey(fmt.swapBehavior());
    qDebug() << "Swap Interval:" << fmt.swapInterval();

    //GUI Anzeige/Einstell-Werte initialisieren
    _fpsCounter = 0;
    _fpsTimer = new QTimer(this);
    //fpsCounter connecten
    connect(_fpsTimer, SIGNAL(timeout()),
            this, SLOT(resetFPSCounter()));
    //Timer starten 1 Timeout pro Sekunde für FPS
    _fpsTimer->start(1000);

    //Kamera-Updates empfangen können
    _myCamera = new Camera(this);
    connect(_myCamera, SIGNAL(isUpdated()),
                     this, SLOT(cameraIsUpdated()));

    //Ordner setzen
    QDir currentDir;
    currentDir.cd("..");
    currentDir.cd("CGAbschlussprojekt");
    QDir::setCurrent(currentDir.canonicalPath());
    qDebug() << "Aktuelles Verzeichnis ist jetzt: ";
    qDebug() << currentDir.canonicalPath();
}

CustomOpenGLWidget::~CustomOpenGLWidget() {
    for(RenderableObject* x : _myRenderables) {
        delete x;
    }
    delete _myCamera;
    delete _cubeModel;
    delete _floorModel;
    delete _defaultShaderProgram;
    delete _fpsTimer;
    delete _debugLogger;
}

//! initializeGL() -- Kommentare so hole ich nach! eventuell sogar
/// doxygen / JavaDocs Style
// param:   /
// return:  void
// descr.:  Initializes OpenGL and its states
void CustomOpenGLWidget::wheelEvent(QWheelEvent* event) {
    //Weiterleitung MouseWheelEvents
    //Wenn Event verarbeitet -> accept sonst ignore
    if(_myCamera->mouseWheelUpdate(event)) {
        event->accept();
        return;
    }
    event->ignore();
}

void CustomOpenGLWidget::moveEvent(QMoveEvent *event) {
    //Weiterleitung MoveEvent (relevant für MousePos)
    //Wenn Event verarbeitet -> accept sonst ignore
    if(_myCamera->windowPosUpdate(event)) {
        event->accept();
        return;
    }
    event->ignore();
}

void CustomOpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
    //Weiterleitung MouseMoveEvents
    //Wenn Event verarbeitet -> accept sonst ignore
   if(_myCamera->mousePosUpdate(event)) {
       event->accept();
       return;
   }
   event->ignore();
}

void CustomOpenGLWidget::keyPressEvent(QKeyEvent* event) {
    //Weiterleitung MouseMoveEvents
    //Wenn Event verarbeitet -> accept sonst ignore
   if(_myCamera->keyPressUpdate(event)) {
       event->accept();
       return;
   }
   event->ignore();
}

void CustomOpenGLWidget::initializeGL() {
    //gewisse OpenGL Funktionen aktiveren (Funktion von QT
    initializeOpenGLFunctions();

    //DebugLogger initialisieren (darf nicht in den Konstruktor, weil dort noch kein OpenGL-Kontext vorhanden ist)
    _debugLogger = new QOpenGLDebugLogger(this);

    connect(_debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this,
            SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);

    if (_debugLogger->initialize()) {
        _debugLogger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
        _debugLogger->enableMessages();
    }

    //OpenGL Flags setzen / Funktionen aktivieren
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    //Zum Debuggen - PolygonModes switch
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glClearDepth(1.0);
    glClearColor(0.f, 0.f, 0.f, 1.f); //erstmal schwarz, nachher vllt irgendein Blau zwecks Himmel?
    //glClearColor(0.5f, 0.6f, 1.f, 1.f); <- himmelblauisch

    //shader zu shader-Programmen hinzufügen
    // default shader
    _defaultShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/default440.vert");
    _defaultShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/default440.frag");

    // texture shader
    _textureShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/texture440.vert");
    _textureShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/texture440.frag");

    // normal drawing shader
    _normalDrawShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/normalDraw440.vert");
    _normalDrawShaderProgram->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/normalDraw440.geom");
    _normalDrawShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/normalDraw440.frag");

    //Kompiliere und linke die Shader-Programme
    _defaultShaderProgram->link();
    _textureShaderProgram->link();
    _normalDrawShaderProgram->link();

    qDebug() << "Default Shader log: " << endl << _defaultShaderProgram->log() << endl;
    qDebug() << "Texture Shader log: " << endl << _textureShaderProgram->log() << endl;
    qDebug() << "Melting Shader log: " << endl << _meltingShaderProgram->log() << endl;
    qDebug() << "Normal Shader log: " << endl << _normalDrawShaderProgram->log() << endl;

    //hauseigene Geometrie erstellen
    _buildGeometry();

    _createRenderables();
}

void CustomOpenGLWidget::paintGL() {
    //Clear buffer to set color and alpha
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Benötigte Matrizen bereitstellen
    QMatrix4x4 projectionMatrix, viewMatrix, modelMatrix;

    //Modelmatrix zur Einheitsmatrix machen - wird später modifiziert
    modelMatrix.setToIdentity();

    _myCamera->getViewMatrix(viewMatrix);

    //ProjectionMatrix setzen, bleibt während aller aufrufe der Kinder konstant
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 10000.0f);

    //Alle Render-Wurzel-Elemente ohne Abhängigkeiten starten Render aufruf
    for(RenderableObject* x : _myRenderables) {
        x->render(modelMatrix,
                 viewMatrix,
                 projectionMatrix);
    }

    //1 Durchlauf = 1 Frameupdate -> FPS zählen zum berechnen
    _fpsCounter++;
    emit sendFPSValue(int(_actualFPS));
    this->update();
}

void CustomOpenGLWidget::onMessageLogged(QOpenGLDebugMessage message) {
    //Erkenntnis: Das braucht nur mein Laptop :/
    //if(message.type() == QOpenGLDebugMessage::PerformanceType &&
    //       message.severity() == QOpenGLDebugMessage::LowSeverity) {
    //        return;
    //}
    qDebug() << message;
}

void CustomOpenGLWidget::cameraIsUpdated() {
    //Queued repaint - aber spammt sie nicht
    this->update();
}

void CustomOpenGLWidget::resetFPSCounter() {
    //FPS Counter jede Sekunde zurücksetzen / Anzeigewert anpassen
    _actualFPS = _fpsCounter;
    _fpsCounter = 0;
}

void CustomOpenGLWidget::_buildGeometry() {
    _cubeModel = new Model("cube.obj");

    //_cubeModel->printVBOData();
    //_cubeModel->printIBOData();

    _floorModel = new Model("square.obj");
    _sphereModel = new Model("sphere_low.obj");
}

void CustomOpenGLWidget::_createRenderables() {
    QMatrix4x4 ctm;

    //Cube
    ctm.setToIdentity();
    ctm.translate(0.f, 0.5f, 0.f);
    RenderableObject* cube = new RenderableObject(ctm,
                                                  _cubeModel,
                                                  SHADER_NORMALS,
                                                  _defaultShaderProgram,
                                                  _normalDrawShaderProgram,
                                                  QVector4D(0.5f, 0.5f, 1.f, 1.f));
    _myRenderables.push_back(cube);

    //Sphere
    ctm.setToIdentity();
    ctm.translate(-5.f, 1.f, -5.f);
    RenderableObject* sphere = new RenderableObject(ctm,
                                                    _sphereModel,
                                                    SHADER_NORMALS,
                                                    _defaultShaderProgram,
                                                    _normalDrawShaderProgram,
                                                    QVector4D(0.5f, 0.5f, 1.f, 1.f));
    _myRenderables.push_back(sphere);

    //Floor
    ctm.setToIdentity();
    ctm.scale(8);
    RenderableObject* floor = new RenderableObject(ctm,
                                                   _floorModel,
                                                   SHADER_TEXTURE,
                                                   _textureShaderProgram,
                                                   nullptr,
                                                   QVector4D(1.f, 0.f, 0.f, 1.f),
                                                   "floor_texture_2_1024px.bmp");
    _myRenderables.push_back(floor);
}
