#ifndef REINDEX_WND_H
#define REINDEX_WND_H

#include <QDialog>

#include "s_data_app.h"
#include "ki_exec_query.h"

namespace Ui {
class reindex_wnd;
}

class reindex_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
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

    int cnt_links;
    int cnt_pol;
    int cnt_udl;
    int cnt_drp;
    int cnt_asg;

    bool f_pause;
    bool f_stop;

    bool update_links();
    bool update_pol();
    bool update_udl();
    bool update_drp();
    bool update_asg();

public:
    explicit reindex_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~reindex_wnd();

private slots:
    void on_bn_close_clicked();

    void on_ch_links_clicked(bool checked);
    void on_ch_pol_clicked(bool checked);
    void on_ch_udl_clicked(bool checked);
    void on_ch_drp_clicked(bool checked);
    void on_ch_asg_clicked(bool checked);

    void on_ch_date_clicked(bool checked);
    void on_ch_null_clicked(bool checked);
    void on_ch_all_clicked(bool checked);

    void on_bn_process_clicked();

    void on_bn_pause_clicked(bool checked);
    void on_bn_stop_clicked(bool checked);

private:
    Ui::reindex_wnd *ui;
};

#endif // REINDEX_WND_H
