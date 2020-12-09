#ifndef autoassig_wnd_H
#define autoassig_wnd_H

#include <QDialog>

#include "s_data_app.h"
#include "ki_exec_query.h"

namespace Ui {
class autoassig_wnd;
}

class autoassig_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    exec_SQL mySQL;

    QSettings &settings;
    s_data_app &data_app;

    QString month_to_str(QDate date);

public:
    explicit autoassig_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~autoassig_wnd();

    bool reg_assig_by_terr_ok (QString ocato, int subj, int dstr, int city, int nasp, int strt, int house, QString corp, int sex, int age);
    bool reg_assig_by_terr_gap(QString ocato, int subj, int dstr, int city, int nasp, int strt, int house, QString corp, int sex, int age);

private slots:
    void on_bn_print_clicked();

    void on_bn_cansel_clicked();

    void on_ch_assig_1_clicked(bool checked);
    void on_ch_assig4age_clicked(bool checked);
    void on_ch_assig_2_clicked(bool checked);
    void on_ch_assig_3_clicked(bool checked);
    void on_ch_assig_4_clicked(bool checked);

    void on_ch_unsig_1_clicked(bool checked);
    void on_ch_unsig_2_clicked(bool checked);
    void on_ch_unsig_3_clicked(bool checked);

    void on_ch_clear_data_1_clicked(bool checked);
    void on_ch_clear_data_2_clicked(bool checked);
    void on_ch_clear_data_3_clicked(bool checked);

    void on_bn_assig_clicked();

    void on_bn_unsig_clicked();

private:
    Ui::autoassig_wnd *ui;
};

#endif // autoassig_wnd_H
