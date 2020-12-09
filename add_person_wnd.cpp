#include "add_person_wnd.h"
#include "ui_add_person_wnd.h"

#include "QMessageBox"

add_person_wnd::add_person_wnd(QSqlDatabase &db, s_data_app &data_app, s_data_person &data_person, bool f_edit, QWidget *parent) :
    db(db),
    data_app(data_app),
    data_person(data_person),
    f_edit(f_edit),
    QDialog(parent),
    ui(new Ui::add_person_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    // выставим активность
    ui->line_fam->setReadOnly(!f_edit);
    ui->line_im->setReadOnly(!f_edit);
    ui->line_ot->setReadOnly(!f_edit);
    ui->line_enp->setReadOnly(!f_edit);
    ui->line_snils->setReadOnly(!f_edit);

    ui->combo_sex->setEnabled(f_edit);
    ui->combo_pol_v->setEnabled(f_edit);

    ui->date_birth->setReadOnly(!f_edit);
    ui->date_reg->setReadOnly(!f_edit);
    ui->date_death->setReadOnly(!f_edit);

    ui->ch_death->setEnabled(f_edit);
    ui->ch_bomj->setEnabled(f_edit);

    // отобразим данные
    ui->line_fam->setText(data_person.fam);
    ui->line_im->setText(data_person.im);
    ui->line_ot->setText(data_person.ot);
    ui->line_enp->setText(data_person.enp);
    ui->line_snils->setText(data_person.snils);

    ui->combo_sex->setCurrentIndex(data_person.sex);
    ui->combo_pol_v->setCurrentIndex(data_person.pol_v);

    ui->date_birth->setDate(data_person.date_birth);
    ui->date_reg->setDate(data_person.date_reg);
    ui->date_death->setDate(data_person.date_death);

    ui->ch_death->setChecked(data_person.f_death);
    ui->date_death->setVisible(data_person.f_death);
    ui->ch_bomj->setChecked(data_person.f_bomj);

    if (f_edit) {
        setWindowTitle("Данные персоны");
    } else {
        setWindowTitle("Данные персоны (только просмотр)");
    }
}

add_person_wnd::~add_person_wnd() {
    delete ui;
}

void add_person_wnd::on_ch_death_toggled(bool checked) {
    ui->date_death->setVisible(checked);
}

void add_person_wnd::on_ch_bomj_toggled(bool checked) {
    ui->date_reg->setEnabled(!checked);
}

void add_person_wnd::on_bn_close_clicked() {
    reject();
}

void add_person_wnd::on_bn_save_clicked() {
    if (!f_edit) {
        QMessageBox::information(this, "Ничего не изменилось",
                                 "Данные открыты без возможности правки.\n\n"
                                 "Ничего не изменилось.");
        reject();

    } else if (data_person.id<=0) {
        // добавим новую запись
        accept();

    } else {
        // обновим запись
        accept();

    }
}
