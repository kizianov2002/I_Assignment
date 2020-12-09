#ifndef ADD_PERSON_WND_H
#define ADD_PERSON_WND_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include <QDate>

#include "s_data_app.h"

namespace Ui {
class add_person_wnd;
}

typedef struct {
    int id;
    QString fam, im, ot, snils, enp;
    int sex, pol_v;
    QDate date_birth, date_reg, date_death;
    bool f_death, f_bomj;
} s_data_person;

class add_person_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных

    s_data_person &data_person;
    s_data_app &data_app;
    bool f_edit;

public:
    explicit add_person_wnd(QSqlDatabase &db, s_data_app &data_app, s_data_person &data_person, bool f_edit=false, QWidget *parent = 0);
    ~add_person_wnd();

private slots:
    void on_bn_close_clicked();

    void on_ch_death_toggled(bool checked);
    void on_ch_bomj_toggled(bool checked);

    void on_bn_save_clicked();

private:
    Ui::add_person_wnd *ui;
};

#endif // ADD_PERSON_WND_H
