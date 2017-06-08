#include "openglcustomwidget.h"
#include "modelloader.h"

OpenGLCustomWidget::OpenGLCustomWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

// Standard Qt OpenGL Methods
//

// initializeGL()
// param:   /
// return:  void
// descr.:  Initializes OpenGL and its states
void OpenGLCustomWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glClearDepth(1.0f);
    glClearColor(0.5f, 0.0f, 0.0f, 1.0f);

    createGeometry();
}

// paintGL()
// param:   /
// return:  void
// descr.:  Called once per Frame to update the screen
void OpenGLCustomWidget::paintGL()
{

}

// createGeometry()
// param:   /
// return:  void
// descr.:  creates the VBO and IBO for all geometric figures
void OpenGLCustomWidget::createGeometry()
{
    ModelLoader model;
    bool res = model.loadObjectFromFile(":/sphere_high.obj");
        // Wenn erfolgreich, generiere VBO und Index-Array
        if (res) {
        // Frage zu erwartende Array-Längen ab
        vboLength = model.lengthOfSimpleVBO();
        iboLength = model.lengthOfIndexArray();
        // Generiere VBO und Index-Array
        vboData = new GLfloat[vboLength];
        indexData = new GLuint[iboLength];
        model.genSimpleVBO(vboData);
        model.genIndexArray(indexData);
    }
    else {
        throw new std::exception();
    }
}
