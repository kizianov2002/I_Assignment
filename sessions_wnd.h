#ifndef sessions_wnd_H
#define sessions_wnd_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class sessions_wnd;
}

#include "s_data_app.h"
#include "ki_exec_query.h"

class sessions_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlQueryModel model_sessions;
    exec_SQL mySQL;
    s_data_app &data_app;

    void refresh_sessions_tab();

public:
    explicit sessions_wnd(QSqlDatabase &db, s_data_app &data_app, QWidget *parent = 0);
    ~sessions_wnd();

private slots:
    void on_bn_close_clicked();

    void on_bn_refresh_clicked();

    void on_ch_open_clicked();

private:
    Ui::sessions_wnd *ui;
};

#endif // sessions_wnd_H
