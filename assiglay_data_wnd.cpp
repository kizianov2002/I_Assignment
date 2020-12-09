#include "assiglay_data_wnd.h"
#include "ui_assiglay_data_wnd.h"

#include <QMessageBox>
#include <QFileDialog>

assiglay_data_wnd::assiglay_data_wnd(s_assiglay_data &data, s_data_app &data_app, QWidget *parent) :
    data(data),
    data_app(data_app),
    QDialog(parent),
    ui(new Ui::assiglay_data_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    ui->line_layer_name->setText(data.layer_name);
    ui->date_layer->setDate(data.layer_date);

    //if (data.layer_type==2)
    //     ui->line_file->setText(data.file_name);
    //else ui->line_file->setText("");

    //ui->lab_file->setVisible(data.layer_type==2);
    //ui->line_file->setVisible(data.layer_type==2);
    //ui->bn_file->setVisible(data.layer_type==2);

    switch (data.layer_type) {
    case 1:
        this->setWindowTitle("Параметры среза ИНКО");
        break;
    case 2:
        this->setWindowTitle("Параметры среза ТФОМС");
        break;
    default:
        this->setWindowTitle("Параметры среза -???-");
        break;
    }
}

assiglay_data_wnd::~assiglay_data_wnd() {
    delete ui;
}

void assiglay_data_wnd::on_bn_cansel_clicked() {
    reject();
}

void assiglay_data_wnd::on_date_layer_dateChanged(const QDate &date) {
    data.layer_date = QDate(ui->date_layer->date().year(),ui->date_layer->date().month(),1);
    data.layer_name = "ASSIG_"
            + QString("0000" + QString::number(ui->date_layer->date().year())).right(4)
            + QString("00" + QString::number(ui->date_layer->date().month())).right(2)
            + "01_"
            + QString(data.layer_type==1 ? "INKO" : (data.layer_type==2 ? "TFOMS" : "-???-") );
    ui->line_layer_name->setText(data.layer_name);
    ui->date_layer->setDate(data.layer_date);
}

void assiglay_data_wnd::on_bn_save_clicked() {
    data.layer_name = ui->line_layer_name->text();
    data.layer_date = ui->date_layer->date();

    //if (data.layer_type==2)
    //     data.file_name = ui->line_file->text();
    //else data.file_name = "";

    accept();
}

/*
void assiglay_data_wnd::on_bn_file_clicked() {
    QString file_name = QFileDialog::getOpenFileName(this, "Выберите файл из ТФОМС", ui->line_file->text(), tr("CSV-files (*.csv *.txt)"));
    if (!file_name.isEmpty()) {
        ui->line_file->setText(file_name);
    } else {
        QMessageBox::information(this, "Файл не выбран",
                                 "Операция отменена пользователем.");
    }
}
*/
