#include "connection_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectionWindow w;
    w.show();

    return a.exec();
}
