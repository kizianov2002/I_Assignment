#ifndef MEDSITES_REG_WND_H
#define MEDSITES_REG_WND_H

#include <QDialog>
#include <QtSql>

#include "s_data_app.h"
#include "ki_exec_query.h"

namespace Ui {
class medsites_reg_wnd;
}

class medsites_reg_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlDatabase db_ODBC;    // это - база данных ODBC для импорта DBF
    QSqlQueryModel model_register;
    QSqlQueryModel model_medsites;
    QSqlQueryModel model_medparts;
    exec_SQL mySQL;
    QSettings &settings;
    s_data_app &data_app;

    void refresh_register();
    void refresh_medparts();
    void refresh_medsites();

public:
    explicit medsites_reg_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~medsites_reg_wnd();

private slots:
    void on_bn_close_clicked();

    void on_bn_refresh_register_clicked();
    void on_bn_refresh_medsites_clicked();
    void on_bn_refresh_medparts_clicked();

    void on_tab_register_clicked(const QModelIndex &index);
    void on_tab_medparts_clicked(const QModelIndex &index);
    void on_tab_medsites_clicked(const QModelIndex &index);

    void on_bn_load_regiaster_clicked();

    void on_bn_auto_analize_clicked();

    void on_bn_use_medpart_clicked();

    void on_bn_use_medsite_clicked();

    void on_line_filter_ms_editingFinished();

    void on_bn_clear_filter_ms_clicked();

    void on_ch_no_medpart_clicked(bool checked);
    void on_ch_no_medsite_clicked(bool checked);
    void on_ch_no_medter_clicked(bool checked);

    void on_bn_Apply2DB_clicked();

private:
    Ui::medsites_reg_wnd *ui;
};

#endif // MEDSITES_REG_WND_H
