#ifndef assiglay_data_WND_H
#define assiglay_data_WND_H

#include <QDialog>
#include <QDate>

#include "s_data_app.h"

namespace Ui {
class assiglay_data_wnd;
}

typedef struct {
    QString layer_name;
    QDate layer_date;
    int layer_type;
    //QString file_name;
} s_assiglay_data;

class assiglay_data_wnd : public QDialog
{
    Q_OBJECT
    s_assiglay_data &data;
    s_data_app &data_app;

public:
    explicit assiglay_data_wnd(s_assiglay_data &data, s_data_app &data_app, QWidget *parent = 0);
    ~assiglay_data_wnd();

private slots:
    void on_bn_cansel_clicked();

    void on_bn_save_clicked();

    void on_date_layer_dateChanged(const QDate &date);

    //void on_bn_file_clicked();

private:
    Ui::assiglay_data_wnd *ui;
};

#endif // assiglay_data_WND_H
