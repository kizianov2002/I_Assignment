#ifndef PRINT_LETTERS_WND_H
#define PRINT_LETTERS_WND_H

#include <QDialog>
#include <QtSql>

#include "s_data_app.h"
#include "ki_exec_query.h"
#include "print_letters_data_wnd.h"

namespace Ui {
class print_letters_wnd;
}

class print_letters_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;    // это - наша база данных
    QSqlQueryModel model_data;
    s_data_app &data_app;
    QSettings &settings;
    exec_SQL mySQL;

    QString data_sql;       // запрос отображаемых данных

    print_letters_data_wnd *print_letters_data_w;

    void refresh_data_tab();
    void refresh_filter_fields();
    void refresh_filter_values();

public:
    explicit print_letters_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent = 0);
    ~print_letters_wnd();

private slots:
    void on_bn_close_clicked();

    void on_bn_refresh_data_clicked();
    void on_rb_new_clicked(bool checked);
    void on_rb_all_clicked(bool checked);
    void on_rb_selected_clicked(bool checked);

    void on_bn_replace_data_clicked();
    void on_bn_add_data_clicked();
    void on_bn_del_data_clicked();

    void on_bn_letter_file_clicked();

    void on_ch_filter_clicked(bool checked);
    //void on_combo_filter_field_activated(const QString &arg1);
    void on_combo_filter_value_activated(const QString &arg1);
    void on_combo_filter_fields_currentIndexChanged(int index);

    void on_bn_generate_clicked();

    void on_combo_filter_values_activated(const QString &arg1);

private:
    Ui::print_letters_wnd *ui;
};

#endif // PRINT_LETTERS_WND_H
