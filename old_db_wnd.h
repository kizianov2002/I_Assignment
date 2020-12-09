#ifndef old_db_wnd_H
#define old_db_wnd_H

#include <QDialog>

#include "s_data_app.h"
#include "ki_exec_query.h"

typedef struct {
    __int64 pol_id, id_person, id_polis, id_blank_vs, id_blank_pol;
    QString id_blank_uec;
} s_data_close;

namespace Ui {
class old_db_wnd;
}

class old_db_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlDatabase &db_FIAS;    // это - наша база данных
    QSqlDatabase db_oldBase;    // это - внешняя база полисов
    exec_SQL mySQL;

    QSettings &settings;
    s_data_app &data_app;

    // начало всей обработки
    QDate date0;
    QTime time0;
    // начало последнего цикла
    QDate date1;
    QTime time1;
    // текущее дата/время
    QDate date;
    QTime time;

    QString old_db_drvr;
    QString old_db_host;
    QString old_db_name;
    QString old_db_port;
    QString old_db_user;
    QString old_db_pass;

    bool try_connect_old_Base();

    int cnt_persons;
    int cnt_persons_docs;
    int cnt_persons_old;
    int cnt_vizits;
    int cnt_events;
    int cnt_polises_close;
    int cnt_polises;
    int cnt_blanks_vs;
    int cnt_blanks_pol;
    int cnt_blanks_uec;
    int cnt_spr_insure_errors;
    int cnt_spr_oksm;
    int cnt_spr_ocato;
    int cnt_spr_ocato_full;
    int cnt_addresses;
    int cnt_assig_by_terr;
    int cnt_persons_assig;
    int cnt_spr_assig_errors;
    int cnt_spr_medorgs;
    int cnt_spr_medparts;
    int cnt_spr_medsites;
    int cnt_spr_medters;
    int cnt_spr_ms_r_mt;
    int cnt_spr_medsites_types;
    int cnt_spr_medters_spec;

    int cnt_load_tfoms;
    int cnt_load_tfoms_2;

    bool try_close_polises();
    bool try_clear_links();
    bool try_load_polises();
    bool try_reload_addres();
    bool try_load_addresses();
    bool try_reload_PersonsOld();
    //bool try_load_assigs();
    int  try_load_tfoms();
    int  try_load_file(QString file_name);

public:
    explicit old_db_wnd(QSqlDatabase &db, QSqlDatabase &db_FIAS, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~old_db_wnd();

private slots:

    void on_bn_close_clicked();
    void on_push_connect_clicked();
    void on_combo_db_drvr_activated(int index);

    void on_bn_load_polises_clicked();
    void on_bn_close_polises_clicked();
    void on_bn_clear_links_clicked();
    void on_bn_reload_addres_clicked();
    void on_bn_load_addresses_clicked();
    //void on_bn_load_assigs_clicked();
    void on_bn_load_tfoms_clicked();

    void on_rb_10_clicked(bool checked);
    void on_rb_100_clicked(bool checked);
    void on_rb_1000_clicked(bool checked);
    void on_rb_10000_clicked(bool checked);
    void on_rb_100000_clicked(bool checked);
    void on_rb_1000000_clicked(bool checked);
    void on_rb_hand_clicked(bool checked);

    void on_bn_FIAS2KLADR_clicked();

    void on_bn_PersonsOld_clicked();

private:
    Ui::old_db_wnd *ui;
};

#endif // old_db_wnd_H
