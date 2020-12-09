#ifndef show_sexage_wnd_H
#define show_sexage_wnd_H

#include <QDialog>
#include <QSettings>

#include "s_data_app.h"
#include "ki_exec_query.h"

#include "show_tab_wnd.h"

namespace Ui {
class show_sexage_wnd;
}

class show_sexage_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlQueryModel model_sexage_tab;
    exec_SQL mySQL;
    s_data_app &data_app;
    QSettings &settings;

    show_tab_wnd *show_tab_w;

    int id_layer;
    int assig_type;
    int id_sexage;
    QString sql_sexage_tab;

    void refresh_combo_sexage();
    void refresh_sexage_tab();

public:
    explicit show_sexage_wnd(int id_sexage, QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~show_sexage_wnd();

private slots:
    void on_bn_close_clicked();

    void on_combo_sexage_activated(int index);

    void on_bn_refresh_sexage_clicked();

    void on_bn_toCSV_clicked();

    void on_tab_sexage_clicked(const QModelIndex &index);

    void on_bn_show_list_clicked();

private:
    Ui::show_sexage_wnd *ui;
};

#endif // show_sexage_wnd_H
