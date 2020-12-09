#include "find_FIAS_wnd.h"
#include "ui_find_FIAS_wnd.h"
#include <QInputDialog>

find_FIAS_wnd::find_FIAS_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::find_FIAS_wnd)
{
    ui->setupUi(this);

}

find_FIAS_wnd::~find_FIAS_wnd() {
    delete ui;
}

QString find_FIAS_wnd::month_to_str(QDate date) {
    QString res;
    switch (date.month()) {
    case 1:
        res += "января ";
        break;
    case 2:
        res += "февраля ";
        break;
    case 3:
        res += "марта ";
        break;
    case 4:
        res += "апреля ";
        break;
    case 5:
        res += "мая ";
        break;
    case 6:
        res += "июня ";
        break;
    case 7:
        res += "июля ";
        break;
    case 8:
        res += "августа ";
        break;
    case 9:
        res += "сентября ";
        break;
    case 10:
        res += "октября ";
        break;
    case 11:
        res += "ноября ";
        break;
    case 12:
        res += "декабря ";
        break;
    default:
        break;
    }
    return res;
}

#include "qt_windows.h"
#include "qwindowdefs_win.h"
#include <shellapi.h>

void find_FIAS_wnd::on_bn_print_clicked() {
}

void find_FIAS_wnd::on_bn_cansel_clicked() {
    close();
}
