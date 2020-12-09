#include "myglwidget.h"

myGLWidget::myGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(30);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start();
}


void myGLWidget::initializeGL() {
    // Set up the rendering context, define display lists etc.:
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)this->width()/(GLfloat)this->height(),0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
}


void myGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-1.5f,0.0f,-6.0f);
    glRotatef(rtri,0.0f,1.0f,0.0f);

    // здесь рисуется кристалл из треугольников
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f( 1.0f,-1.0f, 1.0f);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f( 1.0f,-1.0f, 1.0f);
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f( 1.0f,-1.0f, -1.0f);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f( 1.0f,-1.0f, -1.0f);
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(-1.0f,-1.0f, -1.0f);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);
    glEnd();
    glLoadIdentity();
    glTranslatef(1.5f,0.0f,-7.0f);
    glRotatef(rquad,1.0f,1.0f,1.0f);

    // здесь рисуется куб
    glBegin(GL_QUADS);
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f( 1.0f, 1.0f,-1.0f);
        glVertex3f(-1.0f, 1.0f,-1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f( 1.0f, 1.0f, 1.0f);
        glColor3f(1.0f,0.5f,0.0f);
        glVertex3f( 1.0f,-1.0f, 1.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f( 1.0f,-1.0f,-1.0f);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f( 1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f, 1.0f);
        glColor3f(1.0f,1.0f,0.0f);
        glVertex3f( 1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f, 1.0f,-1.0f);
        glVertex3f( 1.0f, 1.0f,-1.0f);
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);
        glColor3f(1.0f,0.0f,1.0f);
        glVertex3f( 1.0f, 1.0f,-1.0f);
        glVertex3f( 1.0f, 1.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f,-1.0f);
        glEnd();
    rtri+=0.7f;
    rquad-=0.75f;
}


void myGLWidget::resizeGL(int w, int h) {
    if (h==0)
        h=1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
}


