#ifndef SNILS_LIST_WND_H
#define SNILS_LIST_WND_H

#include <QDialog>

typedef struct {
    QString snils_list;
    bool f_mo, f_mt, f_ms, f_address;
} s_snils_mt_data;

namespace Ui {
class snils_list_wnd;
}

class snils_list_wnd : public QDialog
{
    Q_OBJECT

    s_snils_mt_data &data;

public:
    explicit snils_list_wnd(s_snils_mt_data &data, QWidget *parent = 0);
    ~snils_list_wnd();

private slots:
    void on_bn_cansel_clicked();

    void on_bn_apply_clicked();

private:
    Ui::snils_list_wnd *ui;
};

#endif // SNILS_LIST_WND_H
