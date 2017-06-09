#include "customopenglwidget.h"

CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent){
    //Shader initialisieren
    _defaultShaderProgram = new QOpenGLShaderProgram();

    //Keyboard und Mouse Input Einstellungsn
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
    _myCamera = new Camera();
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

    //OpenGL Flags setzen / Funktionen aktivieren
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glClearDepth(1.0f);
    glClearColor(0.25f, 0.25f, 1.f, 0.5f); //erstmal schwarz, nachher vllt irgendein Blau zwecks Himmel?

    //shader zu shader-Programmen hinzufügen
    // default shader
    _defaultShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/default440.vert");
    _defaultShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/default440.frag");

    //Kompiliere und linke die Shader-Programme
    _defaultShaderProgram->link();

    //DebugLogger initialisieren (darf nicht in den Konstruktor, weil dort noch kein OpenGL-Kontext vorhanden ist)
    _debugLogger = new QOpenGLDebugLogger(this);

    connect(_debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this,
            SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);

    if (_debugLogger->initialize()) {
        _debugLogger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
        _debugLogger->enableMessages();
    }

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
    projectionMatrix.perspective(60.0, 16.0/9.0, 0.1, 10000.0);

    //Alle Render-Wurzel-Elemente ohne Abhängigkeiten starten Render aufruf
    for(RenderableObject* x : _myRenderables) {
        x->render(modelMatrix,
                 viewMatrix,
                 projectionMatrix);
    }

    //1 Durchlauf = 1 Frameupdate -> FPS zählen zum berechnen
    _fpsCounter++;
    emit sendFPSValue(_actualFPS);
    this->update();
}

void CustomOpenGLWidget::onMessageLogged(QOpenGLDebugMessage message) {
    if(message.type() == QOpenGLDebugMessage::PerformanceType &&
           message.severity() == QOpenGLDebugMessage::LowSeverity) {
            return;
        }
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
    //_cubeModel = new Model("hardcoded obj-File");

    QFileInfo fi;
    fi.setFile("floor.obj");
    //if(!fi.exists()) {
    _makeFloor(5);
    //}
    //else {
    //  qDebug() << "floor.obj existiert bereits!";
    //}

    //_floorModel = new Model("floor.obj");

    _sphereModel = new Model("sphere_high.obj");
}

void CustomOpenGLWidget::_createRenderables() {
    //RenderableObject* cube = new RenderableObject(_cubeModel,
    //                                              _defaultShaderProgram);
    //_myRenderables.push_back(cube);

    RenderableObject* sphere = new RenderableObject(_sphereModel,
                                                    _defaultShaderProgram);
    _myRenderables.push_back(sphere);

    //RenderableObject* floor = new RenderableObject(_floorModel,
    //                                               _defaultShaderProgram);
    //_myRenderables.push_back(floor);
}

void CustomOpenGLWidget::_makeFloor(unsigned short sideLengthInVertices) {
    unsigned int totalNumberOfVertices = sideLengthInVertices * sideLengthInVertices;
    QVector<QVector3D> vertices(totalNumberOfVertices); //sideLength x sideLength Felder
    QVector<QVector3D> normals(1); //weil Fläche sonst totalNumberOfVertices)
    QVector<QVector2D> texCoords(totalNumberOfVertices);
    //[AnzahlFlächen = (sideLength-1)² * 2 (bei Triangles)][v/vt/vn][3 Datensätze bei Triangles]
    unsigned short numberOfFaces = (sideLengthInVertices-1) * (sideLengthInVertices-1) * 2;
    QVector<QVector<QVector<GLint>>> faces(numberOfFaces);

    //Mit for Schleife füllen erstellen durch Vectoren ist erstmal einfacher
    float horizontalX = 0.f;
    float horizontalZ = 0.f;
    //Vorgezogen weil nur 1 Wert, da überall gleich bei Fläche
    normals[0] = QVector3D(0.f, 1.f, 0.f);
    //QVector3D shift =  QVector3D(-1.f, 0.f, -1.f) * (float(sideLengthInVertices) / 2.f); //"Zentrieren"
    for(unsigned short i = 0; i < totalNumberOfVertices; i++) {
        vertices[i] = QVector3D(horizontalX, 0.f, horizontalZ);// - shift;
        //normals[i] = QVector3D(0.f, 1.f, 0.f); //für kompliziertere Formen hier
        texCoords[i] = QVector2D(horizontalX, horizontalZ) / float(sideLengthInVertices - 1); //müsste so gehen TextureCoords gehen ja von 0-1
        horizontalX++;
        if((int)horizontalX % sideLengthInVertices == 0) {
            horizontalZ++;
            horizontalX = 0;
        }
    }

    unsigned short numOfEntry = 1;
    for(int i = 0; i < numberOfFaces; i += 2) {

        if((numOfEntry + 1) % sideLengthInVertices == 0) { //aka wenn am Ende der Zeile
            numOfEntry++;
        }

        faces[i].resize(3);

        faces[i][0].resize(3);
        faces[i][0][0] = numOfEntry;
        faces[i][0][1] = numOfEntry;
        faces[i][0][2] = 1;

        faces[i][1].resize(3);
        faces[i][1][0] = (numOfEntry + sideLengthInVertices);
        faces[i][1][1] = (numOfEntry + sideLengthInVertices);
        faces[i][1][2] = 1;

        faces[i][2].resize(3);
        faces[i][2][0] = (numOfEntry + sideLengthInVertices + 1);
        faces[i][2][1] = (numOfEntry + sideLengthInVertices + 1);
        faces[i][2][2] = 1;

        faces[i+1].resize(3);

        faces[i+1][0].resize(3);
        faces[i+1][0][0] = (numOfEntry + sideLengthInVertices + 1);
        faces[i+1][0][1] = (numOfEntry + sideLengthInVertices + 1);
        faces[i+1][0][2] = 1;

        faces[i+1][1].resize(3);
        faces[i+1][1][0] = (numOfEntry + 1);
        faces[i+1][1][1] = (numOfEntry + 1);
        faces[i+1][1][2] = 1;

        faces[i+1][2].resize(3);
        faces[i+1][2][0] = numOfEntry;
        faces[i+1][2][1] = numOfEntry;
        faces[i+1][2][2] = 1;

        numOfEntry++;
    }

    Model::createModelFileFromArrays(vertices, normals, texCoords, faces, "floor"); //SoonTM TODO
}
