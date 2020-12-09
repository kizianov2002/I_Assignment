#ifndef PRINT_LETTERS_DATA_WND_H
#define PRINT_LETTERS_DATA_WND_H

#include <QDialog>
#include <QtSql>

#include "s_data_app.h"
#include "ki_exec_query.h"

namespace Ui {
class print_letters_data_wnd;
}

class print_letters_data_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlQueryModel model_coefs;
    s_data_app &data_app;
    QSettings &settings;
    exec_SQL mySQL;

    int style;      // 0 - полная замена данных, 1 - добавление данных

    void refresh_file_data();

public:
    explicit print_letters_data_wnd(int style, QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~print_letters_data_wnd();

private slots:
    void on_bn_cansel_clicked();

    void on_bn_load_data_clicked();
    void on_te_data_textChanged();

    void on_bn_data_file_clicked();
    void on_bn_load_data_2_clicked();
    void on_ln_data_file_editingFinished();
    void on_combo_Codeset_activated(int index);

private:
    Ui::print_letters_data_wnd *ui;
};

#endif // PRINT_LETTERS_DATA_WND_H
