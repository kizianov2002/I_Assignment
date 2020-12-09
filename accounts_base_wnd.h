#ifndef accounts_base_wnd_H
#define accounts_base_wnd_H

#include <QDialog>

#include "s_data_app.h"
#include "ki_exec_query.h"

namespace Ui {
class accounts_base_wnd;
}

class accounts_base_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlDatabase db_accBase;    // это - внешняя база отдела счетов
    exec_SQL mySQL;

    QSettings &settings;
    s_data_app &data_app;

    QDate date0;
    QTime time0;

    QDate date;
    QTime time;

    QString acc_db_drvr;
    QString acc_db_host;
    QString acc_db_name;
    QString acc_db_port;
    QString acc_db_user;
    QString acc_db_pass;

    bool try_connect_acc_Base();

    bool try_load_polises();

public:
    explicit accounts_base_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~accounts_base_wnd();

private slots:

    void on_bn_close_clicked();
    void on_push_connect_clicked();
    void on_combo_db_drvr_activated(int index);

    void on_bn_copy_data_clicked();
    bool try_copy_data();

    void on_rb_1000_clicked(bool checked);
    void on_rb_10000_clicked(bool checked);
    void on_rb_100000_clicked(bool checked);
    void on_rb_1000000_clicked(bool checked);
    void on_rb_hand_clicked(bool checked);

    void on_bn_all_tabs_clicked(bool checked);
    void on_bn_all_clear_clicked(bool checked);
    void on_bn_all_all_clicked(bool checked);

    void on_ch_ASSIG_BY_TERR_clear_clicked(bool checked);
    void on_ch_BLANKS_VS_clear_clicked(bool checked);
    void on_ch_BLANKS_POL_clear_clicked(bool checked);
    void on_ch_BLANKS_UEC_clear_clicked(bool checked);
    void on_ch_EVENTS_clear_clicked(bool checked);
    void on_ch_VIZITS_clear_clicked(bool checked);
    void on_ch_POLISES_clear_clicked(bool checked);
    void on_ch_PERSONS_clear_clicked(bool checked);
    void on_ch_ADDRESSES_clear_clicked(bool checked);
    void on_ch_PERSONS_OLD_clear_clicked(bool checked);
    void on_ch_PERSONS_DOCS_clear_clicked(bool checked);
    void on_ch_PERSONS_ASSIGS_clear_clicked(bool checked);

    void on_ch_ASSIG_BY_TERR_all_clicked(bool checked);
    void on_ch_BLANKS_VS_all_clicked(bool checked);
    void on_ch_BLANKS_POL_all_clicked(bool checked);
    void on_ch_BLANKS_UEC_all_clicked(bool checked);
    void on_ch_EVENTS_all_clicked(bool checked);
    void on_ch_VIZITS_all_clicked(bool checked);
    void on_ch_POLISES_all_clicked(bool checked);
    void on_ch_PERSONS_all_clicked(bool checked);
    void on_ch_ADDRESSES_all_clicked(bool checked);
    void on_ch_PERSONS_OLD_all_clicked(bool checked);
    void on_ch_PERSONS_DOCS_all_clicked(bool checked);
    void on_ch_PERSONS_ASSIGS_all_clicked(bool checked);

private:
    Ui::accounts_base_wnd *ui;
};

#endif // accounts_base_wnd_H
