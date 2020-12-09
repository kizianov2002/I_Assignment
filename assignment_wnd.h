#ifndef ASSIGNMENT_WND_H
#define ASSIGNMENT_WND_H

#include <QMainWindow>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QTimer>

#include "ki_sql_query_model.h"
#include "add_assig_wnd.h"
#include "ki_exec_query.h"
#include "s_data_app.h"
#include "s_data_pers.h"
#include "autoassig_wnd.h"
#include "show_tab_wnd.h"
#include "show_text_wnd.h"
#include "sessions_wnd.h"
#include "add_medsite_ter_wnd.h"
#include "snils_list_wnd.h"
#include "matchTFOMS_wnd.h"
#include "geodata_wnd.h"
#include "old_db_wnd.h"
#include "accounts_base_wnd.h"
#include "reindex_wnd.h"
#include "assiglays_wnd.h"
#include "get_date_wnd.h"
#include "letters2mo_wnd.h"
#include "medsites_reg_wnd.h"
#include "finance_medorgs_wnd.h"
#include "add_person_wnd.h"
#include "add_assig_rec_wnd.h"
#include "import4csv_wnd.h"
#include "reports_assig2mo_wnd.h"
#include "print_letters_wnd.h"

namespace Ui {
class assignment_wnd;
}

class assignment_wnd : public QMainWindow
{
    Q_OBJECT
    QSqlDatabase &db;        // это - наша база данных
    QSqlDatabase db_FIAS;    // это - база данных ФИАС
    QSqlDatabase db_ODBC;    // это - база данных ODBC для импорта DBF
    s_data_app &data_app;
    s_data_pers data_pers;
    QSettings &settings;

    QList<int> l10, l11, l500, l510, l501, l511;

    sessions_wnd *sessions_w;
    autoassig_wnd *autoassig_w;
    show_tab_wnd *show_tab_w;
    show_text_wnd *show_text_w;
    snils_list_wnd *snils_list_w;
    matchTFOMS_wnd *matchTFOMS_w;
    geodata_wnd *geodata_w;
    old_db_wnd *old_db_w;
    accounts_base_wnd *accounts_base_w;
    reindex_wnd *reindex_w;
    assiglays_wnd *assiglays_w;
    get_date_wnd *get_date_w;
    letters2MO_wnd *letters2MO_w;
    medsites_reg_wnd *medsites_reg_w;
    finance_medorgs_wnd *finance_medorgs_w;
    add_person_wnd *add_person_w;
    add_assig_rec_wnd *add_assig_rec_w;
    import4csv_wnd *import4csv_w;
    reports_assig2mo_wnd *reports_assig2mo_w;
    print_letters_wnd *print_letters_w;

    //QSqlQueryModel model_persons;
    ki_persSqlQueryModel model_persons;
    QSqlQueryModel model_assig;
    QSqlQueryModel model_polises;
    QSqlQueryModel model_addresses;
    ki_terrSqlQueryModel model_terrs;
    QSqlQueryModel model_medorgs;
    QSqlQueryModel model_medparts;
    QSqlQueryModel model_medters;
    QSqlQueryModel model_medsites;
    QSqlQueryModel model_medsite_ters;
    exec_SQL mySQL;

    QLabel *statusLab;
    QLabel *statusOper;

    s_data_assig data_assig;
    add_assig_wnd *add_assig_w;
    add_medsite_ter_wnd *add_medsite_ter_w;

    QString sql_pers_flds;
    QString sql_pers_from;
    QString sql_pers_all;
    QString sql_pers_lim;
    QString sql_pers_cnt;

    QString sql_assig_tab;
    QString sql_polises;
    QString sql_addresses;

    bool refresh_fias_connection();

    // 1
    void refresh_medorg_pers();
    void refresh_persons_tab();
    void refresh_combo_assig_source();
    void refresh_assigs_tab();
    void refresh_polises_tab();
    void refresh_addresses_tab();

    // 2
    void refresh_medorg_terr();
    void refresh_combo_medsite_terrs();

    void refresh_terrs_tab();
    void refresh_subj_terr();
    void refresh_dstr_terr();
    void refresh_city_terr();
    void refresh_nasp_terr();
    void refresh_strt_terr();

    // 3
    void refresh_medorgs_tab();
    void refresh_head_mdorgs();

    void refresh_medparts_tab();
    void refresh_medpart_type();

    void refresh_medters_tab();
    void refresh_medter_mo();
    //void refresh_medter_mp();
    //void refresh_medter_ms();
    void refresh_medter_spec();

    void refresh_medsites_tab();
    void refresh_medsite_mo();
    void refresh_medsite_mp();
    void refresh_medsite_type();

    void refresh_medsite_ters_tab();


    QTimer *timer_Updater;
    bool f_Updater;

public:
    explicit assignment_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~assignment_wnd();

private slots:
    void on_time_Updater();

    void on_bn_exit_clicked();
    void on_act_close_triggered();

    // 1
    void on_bn_refresh_pers_clicked();

    void on_ln_fam_pers_textChanged(const QString &arg1);
    void on_ln_im_pers_textChanged(const QString &arg1);
    void on_ln_ot_pers_textChanged(const QString &arg1);
    void on_date_datr_pers_dateChanged(const QDate &date);
    void on_combo_sex_pers_currentIndexChanged(int index);
    void on_combo_assig_type_currentIndexChanged(int index);    
    void on_ln_vs_num_textChanged(const QString &arg1);
    void on_ln_enp_textChanged(const QString &arg1);
    void on_ln_snils_textChanged(const QString &arg1);

    void on_bn_clear_filter_pers_clicked();

    void on_ln_fam_pers_returnPressed();
    void on_ln_im_pers_returnPressed();
    void on_ln_ot_pers_returnPressed();    
    void on_ln_vs_num_returnPressed();
    void on_ln_enp_returnPressed();
    void on_ln_snils_returnPressed();

    void on_date_datr_pers_editingFinished();

    void on_combo_sex_pers_activated(const QString &arg1);

    void on_combo_assig_type_activated(const QString &arg1);
    void on_combo_pers_age_activated(const QString &arg1);

    // 2

    // 3
    void on_bn_refresh_clicked();
    void on_bn_add_clicked();
    void on_bn_edit_clicked();
    void on_tab_medorgs_clicked(const QModelIndex &index);
    void on_bn_delete_clicked();
    void on_bn_ocato_clicked();

    /*void on_ln_ocato_editingFinished();
    void on_ln_code_mo_editingFinished();
    void on_ln_name_mo_editingFinished();
    void on_ln_name_mo_full_editingFinished();
    void on_combo_head_mo_activated(int index);
    void on_ln_phone_editingFinished();
    void on_ln_fax_editingFinished();
    void on_ln_email_editingFinished();
    void on_ln_www_editingFinished();
    void on_ln_comment_editingFinished();*/

    void on_ch_is_head_clicked(bool checked);
    void on_bn_refresh_medsites_clicked();
    void on_tab_medsites_clicked(const QModelIndex &index);
    void on_bn_add_medsite_clicked();
    void on_bn_edit_medsite_clicked();
    void on_ch_medsites_mo_clicked();
    void on_bn_delete_medsite_clicked();
    void on_bn_refresh_medters_clicked();
    void on_tab_medters_clicked(const QModelIndex &index);
    void on_bn_add_medter_clicked();
    void on_combo_medter_mo_currentIndexChanged(int index);
    void on_bn_edit_medter_clicked();
    void on_bn_delete_medter_clicked();
    void on_bn_filter_clicked();
    void on_bn_clear_clicked();
    void on_ln_filter_editingFinished();

    void on_bn_add_medpart_clicked();

    void on_tab_medparts_clicked(const QModelIndex &index);

    void on_bn_edit_medpart_clicked();

    void on_bn_refresh_medparts_clicked();

    void on_bn_delete_medpart_clicked();

    void on_ch_medsites_mo_clicked(bool checked);

    void on_ch_medters_mo_clicked(bool checked);

    //void on_ch_medters_ms_clicked(bool checked);

    void on_combo_medsite_mo_activated(const QString &arg1);

    void on_combo_medter_mo_activated(const QString &arg1);

    //void on_combo_medter_mp_activated(const QString &arg1);

    void on_ch_act_pers_clicked(bool checked);

    void on_bn_refresh_terrs_clicked();

    void on_combo_medorg_terrs_activated(int index);

    void on_combo_medsites_terrs_activated(int index);

    void on_bn_clear_filter_terrs_clicked();

    void on_combo_subj_terrs_activated(int index);
    void on_combo_dstr_terrs_activated(int index);
    void on_combo_city_terrs_activated(int index);
    void on_combo_nasp_terrs_activated(int index);
    void on_combo_strt_terrs_activated(int index);

    void on_bn_add_terrs_clicked();
    void on_bn_edit_terrs_clicked();
    void on_bn_del_terrs_clicked();

    void on_ch_medorg_pers_clicked(bool checked);
    void on_combo_medorg_pers_activated(int index);

    void on_tab_terrs_activated(const QModelIndex &index);
    void on_tab_terrs_clicked(const QModelIndex &index);

    void on_act_autoassig_triggered();

    void on_act_sessions_triggered();

    void on_act_help_triggered();
    void on_act_about_triggered();

    void on_ch_fam_pers_clicked();
    void on_ch_im_pers_clicked();
    void on_ch_ot_pers_clicked();
    void on_ch_datr_pers_clicked();
    void on_ch_sex_pers_clicked();
    void on_ch_pers_age_clicked();

    void on_ch_vs_num_clicked();
    void on_ch_enp_clicked();
    void on_ch_snils_clicked();

    void on_ch_assig_type_clicked();

    void on_bn_refresh_medsite_ters_clicked();

    void on_ch_medsite_ters_now_clicked();

    void on_bn_add_medsite_ter_clicked();

    void on_bn_delete_med_site_ter_clicked();

    void on_bn_consume_med_site_clicked();

    void on_combo_medsite_terrs_activated(int index);

    void on_combo_age_terrs_activated(int index);

    void on_combo_sex_terrs_activated(int index);

    void on_bn_delete_medter4snils_clicked();

    void on_act_mt4snils_triggered();

    void on_act_matchTFOMS_triggered();

    void on_ch_show_names_clicked(bool checked);

    void on_act_map_triggered();

    void on_rb_sort_mt4fio_clicked(bool checked);

    void on_rb_sort_mt4snils_clicked(bool checked);

    void on_rb_sort_ms4codes_clicked(bool checked);

    void on_rb_sort_ms4code_clicked(bool checked);

    void on_rb_sort_ms4name_clicked(bool checked);

    void on_bn_strt_filter_clear_clicked();

    void on_line_strt_filter_editingFinished();

    void on_combo_strt_terrs_activated(const QString &arg1);

    void on_act_Voronej_addPolises_triggered();

    void on_act_load_mo_data_triggered();

    void on_spin_tab_limit_editingFinished();

    void on_ch_tab_limit_clicked(bool checked);

    void on_ch_has_address_reg_clicked(bool checked);

    void on_tab_persons_clicked(const QModelIndex &index);

    void on_bn_today_clicked();

    void on_date_assig_editingFinished();

    void on_rb_date_assig_clicked(bool checked);

    void on_rb_date_cur_clicked(bool checked);

    void on_rb_date_all_clicked();

    void on_ch_show_assig_clicked(bool checked);

    void on_ch_show_addresses_clicked(bool checked);

    void on_ch_show_polises_clicked(bool checked);

    void on_ch_filter_polis_clicked(bool checked);
    void on_ch_filter_addrs_clicked(bool checked);
    void on_ch_filter_assig_clicked(bool checked);

    void on_ch_has_polis_clicked(bool checked);
    void on_ch_no_polis_clicked(bool checked);
    void on_ch_addr_reg_clicked(bool checked);
    void on_ch_addr_liv_clicked(bool checked);
    void on_ch_has_assig_clicked(bool checked);
    void on_ch_no_assig_clicked(bool checked);

    void on_act_to_acconnts_base_triggered();

    void on_act_kladr_triggered();

    void on_act_Voronej_reindex_triggered();

    void on_ln_vs_num_textEdited(const QString &arg1);

    void on_ln_enp_textEdited(const QString &arg1);

    void on_ln_snils_textEdited(const QString &arg1);

    void on_bn_autoassig_clicked();

    void on_act_assiglays_INKO_triggered();

    void on_act_assiglays_TFOMS_triggered();

    void on_act_assig4dates_triggered();
    void on_act_assig4dates_2_triggered();

    void refresh_pers_statuses();
    void on_ch_pers_status_clicked(bool checked);
    void on_ch_insure_begin_clicked(bool checked);
    void on_ch_insure_end_clicked(bool checked);
    void on_combo_pers_status_activated(int index);
    void on_date_insure_begin_editingFinished();
    void on_date_insure_end_editingFinished();

    void on_bn_pers_status_auto_clicked();

    void on_bn_guten_morgen_toggled(bool checked);

    void on_act_letters2MO_triggered();

    void on_act_medsites_Belgorod_triggered();

    void on_act_unassigned_stat_triggered();

    void on_act_unassigned_list_triggered();

    void on_act_finance_triggered();

    void on_act_assig4_ISZL_triggered();

    void on_ch_arch_clicked(bool checked);

    void on_combo_assig_source_activated(int index);

    void on_tab_persons_activated(const QModelIndex &index);

    void on_tab_assig_activated(const QModelIndex &index);

    void on_act_assig2mo_test_triggered();

    void on_act_font_1_triggered();

    void on_act_font_2_triggered();

    void on_act_font_3_triggered();

    void on_act_font_4_triggered();

    void on_bn_font_up_clicked();

    void on_bn_font_dn_clicked();

    void on_act_addr4dates_triggered();

    void on_act_letters_gen_triggered();

private:
    Ui::assignment_wnd *ui;
};

#endif // ASSIGNMENT_WND_H
