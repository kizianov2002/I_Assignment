#ifndef MATCHTFOMS_WND_H
#define MATCHTFOMS_WND_H

#include <QDialog>

#include "s_data_app.h"

#include "ki_exec_query.h"

namespace Ui {
class matchTFOMS_wnd;
}

class matchTFOMS_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;        // это - наша база данных
    QSqlQueryModel model_file;
    QSqlQueryModel model_assigs;
    s_data_app &data_app;
    QSettings &settings;

    exec_SQL mySQL;

    void refresh_combo_files();
    void refresh_tab_file();
    void refresh_tab_assigs();
    void refresh_data_assig();
    void refresh_combo_medorg();
    void refresh_combo_medpart();
    void refresh_combo_medsite();
    void refresh_combo_medter();

public:
    explicit matchTFOMS_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~matchTFOMS_wnd();

private slots:
    void on_bn_close_clicked();

    void on_combo_files_currentIndexChanged(int index);

    void on_bn_tab_file_refresh_clicked();
    void on_tab_file_clicked(const QModelIndex &index);

    void on_bn_refresh_tab_assigs_clicked();
    void on_tab_assigs_clicked(const QModelIndex &index);

    void on_combo_medorg_activated(int index);
    void on_combo_medpart_activated(int index);

    void on_ch_date_unsig_clicked(bool checked);
    void on_ch_date_assig_clicked(bool checked);

    void on_bn_add_clicked();
    void on_bn_edit_clicked();
    void on_bn_auto_clicked();
    void on_bn_delete_clicked();

    void on_bn_save_MO_clicked();

private:
    Ui::matchTFOMS_wnd *ui;
};

#endif // MATCHTFOMS_WND_H
