#ifndef geodata_wnd_H
#define geodata_wnd_H

#include <QDialog>

namespace Ui {
class geodata_wnd;
}

class geodata_wnd : public QDialog
{
    Q_OBJECT

public:
    explicit geodata_wnd(QWidget *parent = 0);
    ~geodata_wnd();

private slots:
    void on_bn_close_clicked();

private:
    Ui::geodata_wnd *ui;
};

#endif // geodata_wnd_H
