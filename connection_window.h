#ifndef ConnectionWindow_H
#define ConnectionWindow_H

#include <QMainWindow>
#include <QtSql/QSqlQuery>
#include <QSettings>
#include "assignment_wnd.h"
#include "s_data_app.h"

#include "ki_exec_query.h"

namespace Ui {
    class ConnectionWindow;
}

class ConnectionWindow : public QMainWindow
{
    Q_OBJECT
    s_data_app data_app;
    exec_SQL mySQL;

    QSqlDatabase db;
    QSqlQuery my_query;
    QSettings *settings;
    QSettings *winreg;

    assignment_wnd *assignment_w;

public:
    explicit ConnectionWindow(QWidget *parent = 0);
    ~ConnectionWindow();

    void refresh_connection();
    void refresh_points();
    void refresh_operators(int id_point);

private slots:
    void on_tc_pass_returnPressed();

    void on_bn_save_DB_params_clicked();

    void on_combo_point_activated(int index);

    void on_tc_op_pass_returnPressed();

    void on_bn_start_clicked();

    void on_ch_change_pass_clicked(bool checked);

    void on_combo_operator_activated(const QString &arg1);

    void on_tc_op_pass_editingFinished();

    void on_bn_exit_clicked();

    void on_combo_operator_currentIndexChanged(const QString &arg1);

    void on_bn_messages_clicked();

    void on_combo_filial_activated(const QString &arg1);

    void on_combo_driver_currentIndexChanged(const QString &arg1);

private:
    Ui::ConnectionWindow *ui;
};

#endif // ConnectionWindow_H
