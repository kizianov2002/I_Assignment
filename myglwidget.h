#ifndef MYGLWIDGET
#define MYGLWIDGET

#include <QGLWidget>
#include <QTimer>
#include <GL/glu.h>

class myGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit myGLWidget(QWidget *parent = 0);

private:
    void initializeGL(); // функция инициализации OpenGL
    void resizeGL(int w, int h); //функция обработки изменения размера вывода
    void paintGL(); // функция прорисовки
private:
    GLfloat rtri;   // парачка переменных для рисования
    GLfloat rquad;

};

#endif // MYGLWIDGET

