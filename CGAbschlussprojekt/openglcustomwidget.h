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
    void    resizeGL(int w, int h);

private:

};

#endif // OPENGLCUSTOMWIDGET_H
