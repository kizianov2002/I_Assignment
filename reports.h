#ifndef REPORTS_H
#define REPORTS_H

#include <QDialog>

namespace Ui {
class reports;
}

class reports : public QDialog
{
    Q_OBJECT

public:
    explicit reports(QWidget *parent = 0);
    ~reports();

private:
    Ui::reports *ui;
};

#endif // REPORTS_H
