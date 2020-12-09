#ifndef FINANCE_MO_coef_WND_H
#define FINANCE_MO_coef_WND_H

#include <QDialog>
#include <QtSql>

#include "s_data_app.h"
#include "ki_exec_query.h"

namespace Ui {
class finance_mo_coef_wnd;
}

class finance_mo_coef_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlQueryModel model_coefs;
    s_data_app &data_app;
    QSettings &settings;
    exec_SQL mySQL;

    int id_coefs;
    QString &code_mo/*, &name_mo*/;
    QDate date_begin, date_end;
    //QDate &date;
    //double &tarif, &m_0_0, &w_0_0, &m_1_4, &w_1_4, &m_5_17, &w_5_17, &m_18_59, &w_18_54, &m_60_, &w_55_, &coef_dif;

    void refresh_combo_medorg();
    void refresh_coefs_tab();

public:
    explicit finance_mo_coef_wnd(QString &code_mo, QSqlDatabase &db, s_data_app &data_app, QSettings &settings/*, QString &name_mo, QDate &date, double &tarif, double &m_0_0, double &w_0_0, double &m_1_4, double &w_1_4, double &m_5_17, double &w_5_17, double &m_18_59, double &w_18_54, double &m_60_, double &w_55_, double &coef_dif*/, QWidget *parent = 0);
    ~finance_mo_coef_wnd();
private slots:
    void on_bn_cansel_clicked();
    void on_combo_medorg_activated(int index);
    void on_tab_coefs_clicked(const QModelIndex &index);

    void on_bn_save_coef_clicked();

    void on_bn_refresh_coefs_tab_clicked();

    void on_bn_delete_coef_clicked();

    void on_bn_today_clicked();

    void on_tab_coefs_activated(const QModelIndex &index);

    void on_bn_all_mo_clicked();

private:
    Ui::finance_mo_coef_wnd *ui;
};

#endif // FINANCE_MO_coef_WND_H
