#ifndef OPENGLCUSTOMWIDGET_H
#define OPENGLCUSTOMWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>

class OpenGLCustomWidget : public QOpenGLWidget
{
public:
    OpenGLCustomWidget(QWidget *parent);

protected:
    void    initializeGL();
    void    paintGL();

private:
    void    createGeometry();

    // Member Variables
    unsigned int    vboLength;
    unsigned int    iboLength;

    GLfloat*        vboData;
    GLuint*         indexData;
};

#endif // OPENGLCUSTOMWIDGET_H
