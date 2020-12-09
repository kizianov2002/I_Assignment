#ifndef ADD_SEXAGE_H
#define ADD_SEXAGE_H

#include <QDialog>

#include "s_data_app.h"
#include "ki_exec_query.h"

namespace Ui {
class add_sexage_wnd;
}

class add_sexage_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    exec_SQL mySQL;
    QSettings &settings;
    s_data_app &data_app;

    int &id_layer;
    int &layer_type;
    QDate &layer_date;
    int &assig_type;

    QList<int>   layer_ids;
    QList<QDate> layer_dates;
    void refresh_combo_layer();

public:
    explicit add_sexage_wnd(int &id_layer, int &layer_type, QDate &layer_date, int &assig_type, QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~add_sexage_wnd();

private slots:
    void on_rb_inko_clicked(bool checked);
    void on_rb_tfoms_clicked(bool checked);

    void on_combo_layer_currentIndexChanged(int index);

    void on_bn_cansel_clicked();
    void on_bn_process_clicked();

private:
    Ui::add_sexage_wnd *ui;
};

#endif // ADD_SEXAGE_H
