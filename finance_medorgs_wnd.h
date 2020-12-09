#ifndef finance_medorgs_wnd_H
#define finance_medorgs_wnd_H

#include <QDialog>
#include "finance_mo_coef_wnd.h"

#include "s_data_app.h"
#include "ki_exec_query.h"

namespace Ui {
class finance_medorgs_wnd;
}

class finance_medorgs_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlQueryModel model_finance;
    QSqlQueryModel model_finance_ms;
    exec_SQL mySQL;
    QSettings &settings;
    s_data_app &data_app;

    QList<QDate> sexage_dates;

    finance_mo_coef_wnd *finance_mo_coef_w;

    QString sql_finance;
    QString sql_finance_ms;

    void refresh_combo_sexage_tab();
    void refresh_finance_tab();
    void refresh_finance_ms_tab();

public:
    explicit finance_medorgs_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~finance_medorgs_wnd();

private slots:

    void on_bn_close_clicked();

    void on_bn_refresh_finance_clicked();

    void on_bn_load_sexage_clicked();
    void on_tab_finance_clicked(const QModelIndex &index);
    void on_bn_edit_coef_clicked();
    void on_bn_recalc_tab_clicked();

    void on_bn_toCSV_clicked();
    void on_bn_toCSV_ms_clicked();

    void on_bn_gen_finrep4mo_clicked();
    void on_bn_gen_finrep4ms_clicked();

    void on_bn_now_clicked();

private:
    Ui::finance_medorgs_wnd *ui;
};

#endif // finance_medorgs_wnd_H
