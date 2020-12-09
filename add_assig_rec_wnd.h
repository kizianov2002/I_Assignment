#ifndef ADD_ASSIG_REC_WND_H
#define ADD_ASSIG_REC_WND_H

#include <QDialog>
#include <QDate>
#include <QSqlDatabase>
#include "ki_exec_query.h"

namespace Ui {
class add_assig_rec_wnd;
}

typedef struct {
    int id, id_person;
    int id_table;
    int assig_type, is_actual;
    QDate assig_date, unsig_date;
    bool f_unsig;
    QString code_mo, code_mp, code_ms, snils_mt, source;
} s_data_assig_rec;

class add_assig_rec_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    exec_SQL mySQL;

    s_data_app &data_app;
    s_data_assig_rec &data_assig_rec;
    bool f_edit;

    void refresh_combo_assig_source();
    void refresh_combo_medorgs();
    void refresh_combo_medparts();
    void refresh_combo_medsites();
    void refresh_combo_medters();

public:
    explicit add_assig_rec_wnd(QSqlDatabase &db, s_data_assig_rec &data_assig_rec, s_data_app &data_app, bool f_edit=false, QWidget *parent = 0);
    ~add_assig_rec_wnd();

private slots:
    void on_bn_close_clicked();

    void on_ch_unsig_toggled(bool checked);

    void on_bn_save_clicked();

private:
    Ui::add_assig_rec_wnd *ui;
};

#endif // ADD_ASSIG_REC_WND_H
