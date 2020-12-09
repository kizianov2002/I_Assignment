#ifndef ASSIGLAYS_WND_H
#define ASSIGLAYS_WND_H

#include <QDialog>

#include "s_data_app.h"
#include "ki_exec_query.h"

#include "assiglay_data_wnd.h"
#include "assiglay_tfoms_master_wnd.h"
#include "add_sexage_wnd.h"
#include "add_move_wnd.h"
#include "add_matrix_wnd.h"
#include "show_sexage_wnd.h"
#include "load_sexage_wnd.h"
#include "show_move_wnd.h"
#include "show_tab_wnd.h"

namespace Ui {
class assiglays_wnd;
}

class assiglays_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlQueryModel model_assiglays;
    QSqlQueryModel model_sexages;
    QSqlQueryModel model_moves;
    exec_SQL mySQL;
    int layers_type;
    QSettings &settings;
    s_data_app &data_app;

    QList<int> l500, l510, l501, l511;

    QString sql_assiglays, sql_sexages, sql_moves;

    assiglay_data_wnd *assiglay_data_w;
    assiglay_tfoms_master_wnd *assiglay_tfoms_master_w;
    add_sexage_wnd *add_sexage_w;
    add_move_wnd *add_move_w;
    add_matrix_wnd *add_matrix_w;
    show_sexage_wnd *show_sexage_w;
    load_sexage_wnd *load_sexage_w;
    show_move_wnd *show_move_w;
    show_tab_wnd *show_tab_w;

    QStringList code_mo_list;

    // начало всей обработки
    QDate date0;
    QTime time0;
    // начало последнего цикла
    QDate date1;
    QTime time1;
    // текущее дата/время
    QDate date;
    QTime time;

public:
    explicit assiglays_wnd(int layers_type, QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~assiglays_wnd();

    void refresh_assiglays_tab();
    void refresh_sexages_tab();
    void refresh_moves_tab();

private slots:
    void on_bn_close_clicked();
    void on_bn_refresh_assiglays_clicked();
    void on_combo_layers_type_currentIndexChanged(int index);

    void on_bn_add_inko_clicked();
    void on_bn_add_tfoms_clicked();    
    void on_bn_delete_assiglay_clicked();

    void on_tab_assiglays_clicked(const QModelIndex &index);
    void on_ch_show_sexages_clicked(bool checked);
    void on_ch_show_moves_clicked(bool checked);

    void on_bn_add_sexage_clicked();    
    void on_bn_open_sexage_clicked();
    void on_bn_delete_sexage_clicked();

    void on_bn_add_move_clicked();

    void on_bn_delete_move_clicked();

    void on_bn_open_move_clicked();

    void on_bn_matrix_clicked();

    void on_bn_load_TFOMS_clicked();

    void on_bn_match_assiglays_clicked();

    void on_ch_match_fond_clicked(bool checked);

    void on_ch_match_inko_clicked(bool checked);

    void on_ch_diff_assig_clicked(bool checked);

    void on_ch_unassigned_clicked(bool checked);

    void on_bn_match_unassig_clicked();

private:
    Ui::assiglays_wnd *ui;
};

#endif // ASSIGLAYS_WND_H
