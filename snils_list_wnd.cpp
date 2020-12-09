#include "snils_list_wnd.h"
#include "ui_snils_list_wnd.h"
#include <QMessageBox>

snils_list_wnd::snils_list_wnd(s_snils_mt_data &data, QWidget *parent) :
    data(data),
    QDialog(parent),
    ui(new Ui::snils_list_wnd)
{
    ui->setupUi(this);

    ui->ch_mo->setChecked(true);
    ui->ch_ms->setChecked(true);
    ui->ch_mt->setChecked(true);
    ui->ch_address->setChecked(true);
}

snils_list_wnd::~snils_list_wnd() {
    delete ui;
}

void snils_list_wnd::on_bn_cansel_clicked() {
    reject();
}

void snils_list_wnd::on_bn_apply_clicked() {
    QString s = ui->textEdit->toPlainText();
    s = s.replace("-", "").replace("'", "").replace(",", "").replace(".", "").replace("(", "").replace(")", "").replace("[", "").replace("]", "").replace("{", "").replace("}", "").replace("<", "").replace(">", "").replace("\"", "").replace("\t", "").replace(" ", "");
    s = s.replace("\n", "*").trimmed().simplified() + "*";

    if (s==" ")
        QMessageBox::warning(this, "Ничего не введено", "Ничего не введено");

    // нормализуем СНИЛСы
    QString s1 = "";
    while (s.length()>1) {
        int pos = s.indexOf("*");
        if (pos<0)  break;

        if (s1.length()>0)
            s1 += ",\n";

        QString part = s.left(pos);
        s = s.right(s.length()-pos-1).trimmed();
        part = QString(QString("0000000000000") + part).right(11);

        s1 += "'" + part + "'";
    }
    //s = s.replace(" ", "\n");

    ui->textEdit->setText(s1);
    QApplication::processEvents();

    if (QMessageBox::question(this, "Нужно подтверждение", "Обработать введённый список СНИЛС?\n", QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::No)
        return;

    data.f_mo = ui->ch_mo->isChecked();
    data.f_mt = ui->ch_mt->isChecked();
    data.f_ms = ui->ch_ms->isChecked();
    data.f_address = ui->ch_address->isChecked();
    data.snils_list = s1;

    accept();
}
