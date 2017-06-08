#include "openglcustomwidget.h"

OpenGLCustomWidget::OpenGLCustomWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void OpenGLCustomWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glClearDepth(1.0f);
    glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
}

void OpenGLCustomWidget::paintGL()
{

}

void OpenGLCustomWidget::resizeGL(int w, int h)
{

}
