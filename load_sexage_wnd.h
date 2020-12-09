#ifndef LOAD_SEXAGE_WND_H
#define LOAD_SEXAGE_WND_H

#include <QDialog>
#include <QSettings>

#include "s_data_app.h"
#include "ki_exec_query.h"

namespace Ui {
class load_sexage_wnd;
}

class load_sexage_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlQueryModel model_sexage_tab;
    exec_SQL mySQL;
    s_data_app &data_app;
    QSettings &settings;

    int id_layer;
    int assig_type;
    int &id_sexage;

    bool f_refresh;
    void refresh_combo_sexage();

public:
    explicit load_sexage_wnd(int &id_sexage, QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~load_sexage_wnd();

private slots:    
    void on_bn_cansel_clicked();
    void on_bn_process_clicked();

    void on_combo_sexage_currentIndexChanged(int index);

private:
    Ui::load_sexage_wnd *ui;
};

#endif // LOAD_SEXAGE_WND_H
