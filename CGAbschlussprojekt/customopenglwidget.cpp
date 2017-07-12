#include "customopenglwidget.h"

CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent){
    //Context erstellen
    _context = new QOpenGLContext();
    _context->create();
    Q_ASSERT(_context->isValid());

    //Shader initialisieren
    _defaultShaderProgram = new QOpenGLShaderProgram();
    _textureShaderProgram = new QOpenGLShaderProgram();
    _normalDrawShaderProgram = new QOpenGLShaderProgram();
    _dropShaderProgram = new QOpenGLShaderProgram();

    //Keyboard und Mouse Input Einstellungen
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    //Debug Output Versionsnummer etc.
    QSurfaceFormat fmt = this->format();
    qDebug() << endl << " - - - - - QSurfaceFormat - Informationen - - - - - ";
    qDebug().nospace() << "OpenGL " << fmt.majorVersion() << "." << fmt.minorVersion();
    qDebug().noquote() << "Profile:" << QMetaEnum::fromType<QSurfaceFormat::OpenGLContextProfile>().valueToKey(fmt.profile());
    qDebug().noquote() << "Options:" << QMetaEnum::fromType<QSurfaceFormat::FormatOption>().valueToKeys(fmt.options());
    qDebug().noquote() << "Renderable Type:" << QMetaEnum::fromType<QSurfaceFormat::RenderableType>().valueToKey(fmt.renderableType());
    qDebug().noquote() << "Swap Behavior:" << QMetaEnum::fromType<QSurfaceFormat::SwapBehavior>().valueToKey(fmt.swapBehavior());
    qDebug() << "Swap Interval:" << fmt.swapInterval();
    qDebug() << endl;

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
    connect(_myCamera, SIGNAL(mouseCaptured(bool)),
            this, SLOT(recieveMouseCaptured(bool)));

    //Ordner setzen
    QDir currentDir;
    currentDir.cd("..");
    currentDir.cd("CGAbschlussprojekt");
    QDir::setCurrent(currentDir.canonicalPath());
    qDebug() << "Aktuelles Verzeichnis ist jetzt: ";
    qDebug() << currentDir.canonicalPath() << endl;
}

CustomOpenGLWidget::~CustomOpenGLWidget() {
    for(RenderableObject* x : _myRenderables) {
        delete x;
    }
    delete _myCamera;
    delete _planeModel;
    delete _dropsModel;
    delete _defaultShaderProgram;
    delete _fpsTimer;
    delete _debugLogger;
}

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

void CustomOpenGLWidget::mousePressEvent(QMouseEvent *event) {
    //Weiterleitung MousePressEvent
    //Wenn Event verarbeitet -> accept sonst ignore
   if(_myCamera->mousePressUpdate(event)) {
       event->accept();
       return;
   }
   event->ignore();
}

void CustomOpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    //Weiterleitung MouseReleaseEvent
    //Wenn Event verarbeitet -> accept sonst ignore
   if(_myCamera->mouseReleaseUpdate(event)) {
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
    glEnable(GL_POLYGON_SMOOTH); //Antialiasing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);    

    //Zum Debuggen - PolygonModes switch
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glClearDepth(1.0);
    glClearColor(0.f, 0.f, 0.f, 1.f); //erstmal schwarz, nachher vllt irgendein Blau zwecks Himmel?
    //glClearColor(0.5f, 0.6f, 1.f, 1.f); <- himmelblauisch

    //shader zu shader-Programmen hinzufügen
    // default shader
    _defaultShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/default440.vert");
    _defaultShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/default440.frag");

    // texture shader
    _textureShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/texture440.vert");
    _textureShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/texture440.frag");

    // normal drawing shader
    _normalDrawShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/normalDraw440.vert");
    _normalDrawShaderProgram->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shader/normalDraw440.geom");
    _normalDrawShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/normalDraw440.frag");

    //drop shader
    _dropShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/drop440.vert");
    _dropShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/drop440.frag");

    //Kompiliere und linke die Shader-Programme
    _defaultShaderProgram->link();
    _textureShaderProgram->link();
    _normalDrawShaderProgram->link();
    _dropShaderProgram->link();

    qDebug() << endl << " - - - - - SHADER - COMPILE - INFOS - - - - - ";
    qDebug() << "Default Shader log: " << endl << _defaultShaderProgram->log();
    qDebug() << "Texture Shader log: " << endl << _textureShaderProgram->log();
    qDebug() << "Normal Shader log: " << endl << _normalDrawShaderProgram->log();
    qDebug() << "Drop Shader log: " << endl << _dropShaderProgram->log();
    qDebug() << endl;

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
    qDebug() << message << endl;
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

void CustomOpenGLWidget::counter1Changed(double value) {
    //TODO Kaskadierend an RenderableObjects weiterleiten (bei Bedarf)
}

void CustomOpenGLWidget::counter2Changed(double value) {
    //TODO Kaskadierend an RenderableObjects weiterleiten (bei Bedarf)
}

void CustomOpenGLWidget::recieveMouseCaptured(bool captured) {
    //Einfach nur Signalweiterleitung an GUI
    emit mouseCaptured(captured);
}

void CustomOpenGLWidget::_buildGeometry() {   
    _planeModel = new Model("16to9_rectangle_vertical_tris.obj");
    _dropsModel = new Model("square_vertical_tris.obj");
}

void CustomOpenGLWidget::_createRenderables() {
    QMatrix4x4 ctm;

    //Plane
    ctm.setToIdentity();
    RenderableObject* background = new RenderableObject(ctm,
                                                        _planeModel,
                                                        SHADER_TEXTURE,
                                                        _textureShaderProgram,
                                                        nullptr,
                                                        QVector4D(1.f, 0.f, 0.f, 1.f),
                                                        "background/old_street_800x450.png",
                                                        //"background/alternative_1600x900.png",
                                                        "water/texture-bg.png");
    _myRenderables.push_back(background);

    //Minimal nach vorne schieben und auf 1600x900 Bild anpassen
    ctm.translate(-800.f, -450.f, 0.5f);
    RenderableObject* foreground = new Raindrops(ctm,
                                                 _dropsModel,
                                                 SHADER_DROPS,
                                                 _dropShaderProgram,
                                                 nullptr,
                                                 QVector4D(1.f, 0.f, 0.f, 1.f),
                                                 "background/old_street_800x450.png",
                                                 //"background/alternative_1600x900.png",
                                                 "water/texture-fg.png",
                                                 "drops/drop-alpha.png",
                                                 "drops/drop-color.png");
    _myRenderables.push_back(foreground);
}
