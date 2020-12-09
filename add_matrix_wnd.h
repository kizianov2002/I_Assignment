#ifndef ADD_MATRIX_H
#define ADD_MATRIX_H

#include <QDialog>

#include "s_data_app.h"
#include "ki_exec_query.h"

namespace Ui {
class add_matrix_wnd;
}

class add_matrix_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    exec_SQL mySQL;
    QSettings &settings;
    s_data_app &data_app;

    int &layer_type;
    int &assig_type;
    int &id_layer_old;
    QDate &layer_date_old;
    int &id_layer_new;
    QDate &layer_date_new;

    QList<int>   layer_ids;
    QList<QDate> layer_dates;
    void refresh_combo_layers();

public:
    explicit add_matrix_wnd(int &layer_type, int &assig_type, int &id_layer_old, QDate &layer_date_old, int &id_layer_new, QDate &layer_date_new, QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~add_matrix_wnd();

private slots:
    void on_rb_inko_clicked(bool checked);
    void on_rb_tfoms_clicked(bool checked);

    void on_combo_layer_old_currentIndexChanged(int index);
    void on_combo_layer_new_currentIndexChanged(int index);

    void on_bn_cansel_clicked();
    void on_bn_process_clicked();

private:
    Ui::add_matrix_wnd *ui;
};

#endif // ADD_MATRIX_H
