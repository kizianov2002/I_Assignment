#ifndef find_FIAS_wnd_H
#define find_FIAS_wnd_H

#include <QDialog>

#include "s_data_app.h"

#include "ki_exec_query.h"

namespace Ui {
class find_FIAS_wnd;
}

class find_FIAS_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    exec_SQL mySQL;

    QSettings &settings;
    s_data_app &data_app;

    QString month_to_str(QDate date);

public:
    explicit find_FIAS_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~find_FIAS_wnd();

private slots:
    void on_bn_print_clicked();

    void on_bn_cansel_clicked();
private:
    Ui::find_FIAS_wnd *ui;
};

#endif // find_FIAS_wnd_H
