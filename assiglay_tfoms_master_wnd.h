#ifndef ASSIGLAY_TFOMS_MASTER_H
#define ASSIGLAY_TFOMS_MASTER_H

#include <QDialog>

#include "s_data_app.h"

#include "ki_exec_query.h"
#include "assiglay_data_wnd.h"

namespace Ui {
class assiglay_tfoms_master_wnd;
}

class assiglay_tfoms_master_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlQueryModel model_import_tab;
    s_data_app &data_app;
    exec_SQL mySQL;
    QSettings &settings;

    assiglay_data_wnd *assiglay_data_w;

    // начало всей обработки
    QDate date0;
    QTime time0;
    // начало последнего цикла
    QDate date1;
    QTime time1;
    // текущее дата/время
    QDate date;
    QTime time;

    QString sql_import_tab;

    QList<int>  SQL_ids;
    QStringList SQL_names;
    QStringList SQL_textes;

public:
    explicit assiglay_tfoms_master_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~assiglay_tfoms_master_wnd();

    void refresh_import_tab();
    void refresh_combo_SQL();
    void refresh_data_status();

private slots:
    void on_bn_refresh_clicked();

    void on_ch_limit_clicked(bool checked);
    void on_ch_filter_person_clicked(bool checked);
    void on_ch_filter_polis_clicked(bool checked);
    void on_ch_filter_person_yes_clicked(bool checked);
    void on_ch_filter_person_no_clicked(bool checked);
    void on_ch_filter_polis_yes_clicked(bool checked);
    void on_ch_filter_polis_no_clicked(bool checked);

    void on_bn_load_clicked();

    void on_combo_SQL_currentIndexChanged(int index);
    void on_bn_SQL_exec_clicked();

    void on_bn_save_clicked();

    void on_bn_close_clicked();

    void on_bn_toCSV_clicked();

    void on_bn_clear_clicked();

private:
    Ui::assiglay_tfoms_master_wnd *ui;
};

#endif // ASSIGLAY_TFOMS_MASTER_H
