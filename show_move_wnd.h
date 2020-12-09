#ifndef show_move_wnd_H
#define show_move_wnd_H

#include <QDialog>
#include <QSettings>

#include "s_data_app.h"
#include "ki_exec_query.h"

#include "show_tab_wnd.h"

namespace Ui {
class show_move_wnd;
}

class show_move_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlQueryModel model_move_tab;
    exec_SQL mySQL;
    s_data_app &data_app;
    QSettings &settings;

    show_tab_wnd *show_tab_w;

    int id_move;
    int assig_type;
    QString sql_move_tab;

    void refresh_combo_move();
    void refresh_move_tab();

public:
    explicit show_move_wnd(int id_move, QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~show_move_wnd();

private slots:
    void on_bn_close_clicked();

    void on_combo_move_activated(int index);

    void on_bn_refresh_move_clicked();

    void on_bn_toCSV_clicked();

    void on_tab_move_clicked(const QModelIndex &index);

    void on_bn_show_list_clicked();

private:
    Ui::show_move_wnd *ui;
};

#endif // show_move_wnd_H
