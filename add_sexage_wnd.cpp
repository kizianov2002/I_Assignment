#include "add_sexage_wnd.h"
#include "ui_add_sexage_wnd.h"

add_sexage_wnd::add_sexage_wnd(int &id_layer, int &layer_type, QDate &layer_date, int &assig_type, QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    id_layer(id_layer),
    layer_type(layer_type),
    layer_date(layer_date),
    assig_type(assig_type),
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::add_sexage_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    switch (assig_type) {
    case 1:
        ui->rb_terr->setChecked(true);
        break;
    case 2:
        ui->rb_reqs->setChecked(true);
        break;
    default:
        ui->rb_all->setChecked(true);
        break;
    }

    switch (layer_type) {
    case 1:
        ui->rb_inko->setChecked(true);
        break;
    case 2:
        ui->rb_tfoms->setChecked(true);
        break;
    default:
        ui->rb_tfoms->setChecked(true);
        break;
    }

    refresh_combo_layer();
}

add_sexage_wnd::~add_sexage_wnd() {
    delete ui;
}


void add_sexage_wnd::refresh_combo_layer() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка регионов
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select id, layer_name, layer_date "
                  "  from assig.layers "
                  " where layer_type=" + QString(ui->rb_inko->isChecked() ? "1" : "2") + " "
                  " order by layer_date DESC ; ";
    query->exec(sql);

    ui->combo_layer->clear();
    layer_ids.clear();
    layer_dates.clear();

    layer_ids.append(-1);
    layer_dates.append(QDate());
    ui->combo_layer->addItem(" - Выберите снимок данных - ", -1);
    while (query->next()) {
        int     id_layer   = query->value(0).toInt();
        QString layer_name = query->value(1).toString();
        QDate   layer_date = query->value(2).toDate();
        layer_ids.append(id_layer);
        layer_dates.append(layer_date);
        ui->combo_layer->addItem(layer_name, id_layer);
    }
    delete query;

    if (ui->combo_layer->count()>1) {
        int n = ui->combo_layer->findData(id_layer);
        if (n<0) {
            id_layer = -1;
            layer_type = (ui->rb_inko->isChecked() ? 1 : 2);
            layer_date = QDate();
        } else {
            ui->combo_layer->setCurrentIndex(n);
            id_layer = ui->combo_layer->currentData().toInt();
            layer_type = (ui->rb_inko->isChecked() ? 1 : 2);
            layer_date = layer_dates.at(n);
        }
    } else {
        id_layer = -1;
        layer_type = (ui->rb_inko->isChecked() ? 1 : 2);
        layer_date = QDate();
    }
    this->setCursor(Qt::ArrowCursor);
}

void add_sexage_wnd::on_combo_layer_currentIndexChanged(int index) {
    if (ui->combo_layer->count()>1) {
        id_layer = ui->combo_layer->currentData().toInt();
        layer_type = (ui->rb_inko->isChecked() ? 1 : 2);
        layer_date = layer_dates.at(index);
    }
}


void add_sexage_wnd::on_rb_inko_clicked(bool checked) {
    refresh_combo_layer();
}
void add_sexage_wnd::on_rb_tfoms_clicked(bool checked) {
    refresh_combo_layer();
}


void add_sexage_wnd::on_bn_process_clicked() {
    if (id_layer<1) {
        QMessageBox::warning(this, "Выберите снимок данных",
                             "Сначала выберите снимок данных для постраения таблицы половозрастного распределения.");
        return;
    }
    layer_type = (ui->rb_inko->isChecked() ? 1 : 2);
    assig_type = (ui->rb_terr->isChecked() ? 1 : (ui->rb_reqs->isChecked() ? 2 : 0) );
    id_layer = ui->combo_layer->currentData().toInt();
    layer_date = layer_dates.at(ui->combo_layer->currentIndex());
    accept();
}
void add_sexage_wnd::on_bn_cansel_clicked() {
    reject();
}

