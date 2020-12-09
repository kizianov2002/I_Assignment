#include "geodata_wnd.h"
#include "ui_geodata_wnd.h"
#include "myglwidget.h"

geodata_wnd::geodata_wnd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::geodata_wnd)
{
    ui->setupUi(this);

    myGLWidget *w = new myGLWidget;
    ui->layout_geodata->addWidget(w);
}

geodata_wnd::~geodata_wnd() {
    delete ui;
}

void geodata_wnd::on_bn_close_clicked() {
    close();
}

