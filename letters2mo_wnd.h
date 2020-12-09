#ifndef LETTERS2MO_WND_H
#define LETTERS2MO_WND_H

#include <QDialog>

#include "s_data_app.h"
#include "ki_exec_query.h"

namespace Ui {
class letters2MO_wnd;
}

class letters2MO_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlQueryModel model_medorgs;
    exec_SQL mySQL;
    QSettings &settings;
    s_data_app &data_app;

    QList<int> l41, l51, l01;
    int layer_type;
    int id_layer_old, id_layer_new;
    QList<int>   layer_ids;
    QList<QDate> layer_dates;

    QString sql_medorgs;

    void refresh_medorgs_tab();
    void refresh_combo_layers();
    void refresh_combo_sexage_tab();
    void refresh_combo_move_tab();

    QDateTime time_start;

    bool gen_letter2MO( QString layer_old, int id_layer_old, QDate date_layer_old, QString layer_new, int id_layer_new, QDate date_layer_new,
                        QDate date_report, QDate date_print, QString official_num,
                        QString sexage_tab, int id_sexage_tab, QString move_tab, int id_move_tab,
                        bool f_count, bool f_assig, bool f_unsig, bool f_address, bool f_phones,
                        bool f_other_files, QDir &dir_other_files, QDir &dir_generate, bool f_backup, QDir &dir_backup,
                        QString code_mo, QString fname_readme, QString fname_moverep);

public:
    explicit letters2MO_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~letters2MO_wnd();

private slots:
    void on_rb_inko_clicked(bool checked);
    void on_rb_tfoms_clicked(bool checked);

    void on_combo_layer_new_currentIndexChanged(int index);
    void on_combo_layer_old_currentIndexChanged(int index);

    void on_bn_other_files_clicked();
    void on_bn_gen_dir_clicked();
    void on_bn_backup_dir_clicked();

    void on_bn_refresh_medorgs_clicked();
    void on_ch_all_medorgs_clicked(bool checked);

    void on_ch_other_files_clicked(bool checked);
    void on_ch_backup_clicked(bool checked);
    void on_bn_clear_sel_clicked();
    void on_bn_reverse_sel_clicked();
    void on_ch_lock_clicked(bool checked);

    void on_bn_generate_clicked();
    void on_bn_MVipNet_clicked();
    void on_bn_MSendVipnet_clicked();

    void on_bn_close_clicked();

private:
    Ui::letters2MO_wnd *ui;
};

#endif // LETTERS2MO_WND_H
