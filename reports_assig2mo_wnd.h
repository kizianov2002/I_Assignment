#ifndef ASSIG2MO_TEST_WND_H
#define ASSIG2MO_TEST_WND_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQueryModel>

#include "import4csv_wnd.h"
#include "s_data_app.h"

namespace Ui {
class reports_assig2mo_wnd;
}

class reports_assig2mo_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlQueryModel model_analize_tab;
    QSqlQueryModel model_medorgs;
    s_data_app &data_app;
    QSettings &settings;

    import4csv_wnd *import4csv_w;

    QList<int> l41, l51, l01;
    QString sql_medorgs;

    QString dir_OUT;
    QString dir_OtherData;

    QDateTime time_start;

    void refresh_combo_data_source();
    void refresh_medorgs_tab();
    void refresh_analize_tab();

public:
    explicit reports_assig2mo_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~reports_assig2mo_wnd();

private slots:
    void on_ch_live4terr_clicked(bool checked);
    void on_ch_live4terr_one4terr_clicked(bool checked);
    void on_ch_live4terr_one4req_clicked(bool checked);
    void on_ch_live4terr_other4terr_clicked(bool checked);
    void on_ch_live4terr_other4req_clicked(bool checked);
    void on_ch_assig_clicked(bool checked);
    void on_ch_date_rep_clicked(bool checked);
    void on_ch_analize_clicked(bool checked);
    void on_ch_other_files_clicked(bool checked);
    void on_bn_file_csv_clicked();
    void on_line_file_csv_textChanged(const QString &arg1);
    void on_bn_import_csv_clicked();
    void on_bn_other_files_clicked();
    void on_bn_gen_dir_clicked();
    bool rep_MO_assig( QString code_mo, QString tab_name, int assig_type, int assig_age,
                       bool gen_terr, bool gen_live4terr, bool f_live4terr_one4terr, bool f_live4terr_one4req, bool f_live4terr_other4terr, bool f_live4terr_other4req,
                       bool f_address, bool f_phone, bool f_date_rep, QDate date_rep,
                       bool gen_addresses, bool gen_assig, bool gen_analize, bool f_other_files, bool f_append_arch,
                       QString dir_Files, QString dir_GenDir);

    void on_bn_refresh_medorgs_clicked();
    void on_ch_all_medorgs_clicked(bool checked);
    void on_bn_clear_sel_clicked();
    void on_bn_reverse_sel_clicked();
    void on_ch_lock_clicked(bool checked);
    void on_bn_MSendVipnet_clicked();

    void on_bn_generate_clicked();

    void on_bn_close_clicked();

private:
    Ui::reports_assig2mo_wnd *ui;
};

#endif // ASSIG2MO_TEST_WND_H
