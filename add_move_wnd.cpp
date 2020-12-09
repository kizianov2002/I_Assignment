#include "add_move_wnd.h"
#include "ui_add_move_wnd.h"

add_move_wnd::add_move_wnd(int &layer_type, int &assig_type, int &id_layer_old, QDate &layer_date_old, int &id_layer_new, QDate &layer_date_new, QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    layer_type(layer_type),
    assig_type(assig_type),
    id_layer_old(id_layer_old),
    layer_date_old(layer_date_old),
    id_layer_new(id_layer_new),
    layer_date_new(layer_date_new),
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::add_move_wnd)
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

    refresh_combo_layers();
}

add_move_wnd::~add_move_wnd() {
    delete ui;
}


void add_move_wnd::refresh_combo_layers() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающих списков снимков
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select id, layer_name, layer_date "
                  "  from assig.layers "
                  " where layer_type=" + QString(ui->rb_inko->isChecked() ? "1" : "2") + " "
                  " order by layer_date DESC ; ";
    query->exec(sql);

    ui->combo_layer_old->clear();
    ui->combo_layer_new->clear();
    layer_ids.clear();
    layer_dates.clear();

    layer_ids.append(-1);
    layer_dates.append(QDate());
    ui->combo_layer_old->addItem(" - Выберите старый снимок данных - ", -1);
    ui->combo_layer_new->addItem(" - Выберите новый снимок данных - ", -1);
    while (query->next()) {
        int     id_layer   = query->value(0).toInt();
        QString layer_name = query->value(1).toString();
        QDate   layer_date = query->value(2).toDate();
        layer_ids.append(id_layer);
        layer_dates.append(layer_date);
        ui->combo_layer_old->addItem(layer_name, id_layer);
        ui->combo_layer_new->addItem(layer_name, id_layer);
    }
    delete query;

    layer_type = (ui->rb_inko->isChecked() ? 1 : 2);

    if (ui->combo_layer_old->count()>1) {
        int n = ui->combo_layer_new->findData(id_layer_new);

        if (n>0 && ui->combo_layer_old->count()>n+1) {
            ui->combo_layer_new->setCurrentIndex(n);
            id_layer_new = layer_ids.at(n);
            layer_date_new = layer_dates.at(n);

            ui->combo_layer_old->setCurrentIndex(n+1);
            id_layer_old = layer_ids.at(n+1);
            layer_date_old = layer_dates.at(n+1);
        } else {
            ui->combo_layer_old->setCurrentIndex(0);
            id_layer_old = -1;
            layer_date_old = QDate();
        }
    } else {
        layer_type = (ui->rb_inko->isChecked() ? 1 : 2);
        id_layer_old = -1;
        layer_date_old = QDate();
        id_layer_new = -1;
        layer_date_new = QDate();
    }
    this->setCursor(Qt::ArrowCursor);
}

void add_move_wnd::on_combo_layer_new_currentIndexChanged(int index) {
    layer_type = (ui->rb_inko->isChecked() ? 1 : 2);
    int n = ui->combo_layer_new->currentIndex();    

    if (ui->combo_layer_old->count()>n+1) {
        ui->combo_layer_old->setCurrentIndex(n+1);
        id_layer_old = layer_ids.at(n+1);
        layer_date_old = layer_dates.at(n+1);
    } else {
        /*ui->combo_layer_old->setCurrentIndex(0);
        id_layer_old = -1;
        layer_date_old = QDate();*/
    }
}

void add_move_wnd::on_combo_layer_old_currentIndexChanged(int index) {
    layer_type = (ui->rb_inko->isChecked() ? 1 : 2);
    int n = ui->combo_layer_old->currentIndex();

    if (n>1 && layer_ids.size()>n) {
        ui->combo_layer_new->setCurrentIndex(n-1);
        id_layer_new = layer_ids.at(n-1);
        layer_date_new = layer_dates.at(n-1);
    } else {
        /*ui->combo_layer_new->setCurrentIndex(0);
        id_layer_new = -1;
        layer_date_new = QDate();*/
    }
}


void add_move_wnd::on_rb_inko_clicked(bool checked) {
    refresh_combo_layers();
}
void add_move_wnd::on_rb_tfoms_clicked(bool checked) {
    refresh_combo_layers();
}


void add_move_wnd::on_bn_process_clicked() {
    if (id_layer_old<1) {
        QMessageBox::warning(this, "Выберите старый снимок данных",
                             "Сначала выберите старый снимок данных для постраения таблицы движений прикреплённых застрахованных.\n"
                             "Исправьте данные и повторите попытку.");
        return;
    }
    if (id_layer_new<1) {
        QMessageBox::warning(this, "Выберите новый снимок данных",
                             "Сначала выберите новый снимок данных для постраения таблицы движений прикреплённых застрахованных.\n"
                             "Исправьте данные и повторите попытку.");
        return;
    }
    bool f_inko = ui->rb_inko->isChecked();
    layer_type = (f_inko ? 1 : 2);
    bool f_terr = ui->rb_terr->isChecked();
    bool f_reqs = ui->rb_reqs->isChecked();
    bool f_all  = ui->rb_all->isChecked();
    assig_type = (f_terr ? 1 : (f_reqs ? 2 : 0) );
    id_layer_old = ui->combo_layer_old->currentData().toInt();
    layer_date_old = layer_dates.at(ui->combo_layer_old->currentIndex());
    id_layer_new = ui->combo_layer_new->currentData().toInt();
    layer_date_new = layer_dates.at(ui->combo_layer_new->currentIndex());
    if (layer_date_old>=layer_date_new) {
        QMessageBox::warning(this, "Старый снимок должен быть старше нового",
                             "Старый снимок должен быть старше нового.\n"
                             "Исправьте данные и повторите попытку.");
        return;
    }
    accept();
}
void add_move_wnd::on_bn_cansel_clicked() {
    reject();
}

