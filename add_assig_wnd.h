#ifndef ADD_ASSIG_WND_H
#define ADD_ASSIG_WND_H

#include <QDialog>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include "ki_exec_query.h"
#include "s_data_app.h"
#include "s_data_assig.h"
#include <QTextEdit>

namespace Ui {
class add_assig_wnd;
}

class add_assig_wnd : public QDialog
{
    Q_OBJECT

    QSqlDatabase &db;
    QSqlDatabase &db_FIAS;
    s_data_app &data_app;
    s_data_assig &data_assig;
    QSettings &settings;
    QTextEdit *te_log;
    exec_SQL mySQL;

    void update_data();
    void update_right_type();

    void refresh_subj_terr();
    void refresh_dstr_terr();
    void refresh_city_terr();
    void refresh_nasp_terr();
    void refresh_strt_terr();

    void refresh_mo();
    void refresh_mp();
    void refresh_ms();
    void refresh_mt();

    bool try_house (QString ocato, int subj, int dstr, int city, int nasp, int strt, int house, QString corp, int sex, int age, QString code_mo, QString code_mp, QString code_ms, QString snils_mt, bool from_start, bool to_end);
    bool add_house (QString ocato, int subj, int dstr, int city, int nasp, int strt, int house, QString corp, int sex, int age, QString code_mo, QString code_mp, QString code_ms, QString snils_mt, bool from_start, bool to_end);
    //bool del_house (QString ocato, int subj, int dstr, int city, int nasp, int strt, int house, QString corp, int sex, int age, QString code_mo, QString code_mp, QString code_ms, QString snils_mt);
    bool del_house (int id);
    bool del_house (QString ocato, int subj, int dstr, int city, int nasp, int strt, int house, QString corp, int sex, int age, QString code_mo, QString code_mp, QString code_ms, QString snils_mt, bool from_start, bool to_end);

    bool try_all_street (QString ocato, int subj, int dstr, int city, int nasp, int strt, int sex, int age, QString code_mo, QString code_mp, QString code_ms, QString snils_mt);
    bool add_all_street (QString ocato, int subj, int dstr, int city, int nasp, int strt, int sex, int age, QString code_mo, QString code_mp, QString code_ms, QString snils_mt);

public:
    explicit add_assig_wnd(QSqlDatabase &db, QSqlDatabase &db_FIAS, s_data_app &data_app, s_data_assig &data_assig, QSettings &settings, QTextEdit *te_log, QWidget *parent = 0);
    ~add_assig_wnd();

    QString log;

private slots:
    void on_rb_one_house_clicked();
    void on_rb_line_houses_clicked();
    void on_rb_all_street_clicked();
    void on_rb_open_line_clicked();    
    void on_rb_numlist_clicked();

    /*void on_combo_mo_activated(int index);
    void on_combo_mp_activated(int index);
    void on_combo_ms_activated(int index);
    void on_combo_mt_activated(int index);*/

    void on_combo_mo_currentIndexChanged(int index);
    void on_combo_mp_currentIndexChanged(int index);
    void on_combo_ms_currentIndexChanged(int index);

    void on_combo_subj_terrs_activated(int index);
    void on_combo_dstr_terrs_activated(int index);
    void on_combo_city_terrs_activated(int index);
    void on_combo_nasp_terrs_activated(int index);
    void on_combo_strt_terrs_activated(int index);

    void on_buttonBox_accepted();

    void on_line_corp_textEdited(const QString &arg1);
    void on_line_corp2_textEdited(const QString &arg1);

    void on_bn_ok_clicked();

    void on_bn_cansel_clicked();

    void on_bn_add_clicked();

    void on_ch_from_start_clicked(bool checked);

    void on_ch_to_end_clicked(bool checked);

    void on_line_numlist_editingFinished();

private:
    Ui::add_assig_wnd *ui;
};

#endif // ADD_ASSIG_WND_H
