#include "add_assig_rec_wnd.h"
#include "ui_add_assig_rec_wnd.h"

#include "QMessageBox"
#include "QSqlQuery"

add_assig_rec_wnd::add_assig_rec_wnd(QSqlDatabase &db, s_data_assig_rec &data_assig_rec, s_data_app &data_app, bool f_edit, QWidget *parent) :
    db(db),
    data_assig_rec(data_assig_rec),
    data_app(data_app),
    f_edit(f_edit),
    QDialog(parent),
    ui(new Ui::add_assig_rec_wnd)
{
    ui->setupUi(this);    

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    if (f_edit) {
        setWindowTitle("Данные прикрепления");
    } else {
        setWindowTitle("Данные прикрепления (только просмотр)");
    }

    refresh_combo_assig_source();
    ui->combo_assig_source->setCurrentIndex(ui->combo_assig_source->findData(data_assig_rec.id_table));
    ui->combo_assig_type->setCurrentIndex(data_assig_rec.assig_type);
    ui->date_assig->setDate(data_assig_rec.assig_date);
    ui->ch_unsig->setChecked(data_assig_rec.f_unsig);
    ui->date_unsig->setDate(data_assig_rec.unsig_date);
    ui->spin_actual->setValue(data_assig_rec.is_actual);

    refresh_combo_medorgs();
    ui->combo_medorg->setCurrentIndex(ui->combo_medorg->findData(data_assig_rec.code_mo));
    refresh_combo_medparts();
    ui->combo_medpart->setCurrentIndex(ui->combo_medpart->findData(data_assig_rec.code_mp));
    refresh_combo_medsites();
    ui->combo_medsite->setCurrentIndex(ui->combo_medsite->findData(data_assig_rec.code_ms));
    refresh_combo_medters();
    ui->combo_medter->setCurrentIndex(ui->combo_medter->findData(data_assig_rec.snils_mt));
    ui->te_source->setText(data_assig_rec.source);
}

add_assig_rec_wnd::~add_assig_rec_wnd() {
    delete ui;
}


void add_assig_rec_wnd::refresh_combo_assig_source() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка слоёв прикрепления
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select id, layer_name||'  ('||layer_date||')' "
                  "  from assig.layers l "
                  " order by layer_name desc ; ";
    mySQL.exec(this, sql, QString("Список слоёв прикрепления"), *query, true, db, data_app);
    ui->combo_assig_source->clear();
    ui->combo_assig_source->addItem("ИНКО - прикрепление подробно", -1);
    ui->combo_assig_source->addItem("ИНКО - прикрепление к МО", -2);
    while (query->next()) {
        ui->combo_assig_source->addItem("Снимок данных - " + query->value(1).toString(), query->value(0).toInt());
    }

    delete query;
    ui->combo_assig_source->setCurrentIndex(-1);
    this->setCursor(Qt::ArrowCursor);
}

void add_assig_rec_wnd::refresh_combo_medorgs() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mo, '(' || code_mo || ') ' || name_mo "
                  "  from spr_medorgs s "
                  " where is_head=1 "
                  " order by code_mo ; ";
    mySQL.exec(this, sql, QString("Справочник медорганизаций"), *query, true, db, data_app);
    ui->combo_medorg->clear();
    while (query->next()) {
        ui->combo_medorg->addItem(query->value(1).toString(), query->value(0).toString());
    }
    ui->combo_medorg->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void add_assig_rec_wnd::refresh_combo_medparts() {
    QString code_mo = ui->combo_medorg->currentData().toString();
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mp, '(' || code_mp || ') ' || name_mp "
                  "  from spr_medparts s "
                  " where code_mo='" + code_mo + "' "
                  " order by code_mp ; ";
    mySQL.exec(this, sql, QString("Справочник подразделений медорганизации"), *query, true, db, data_app);
    ui->combo_medpart->clear();
    ui->combo_medpart->addItem("(0)  - без подразделения -", "0");
    while (query->next()) {
        ui->combo_medpart->addItem(query->value(1).toString(), query->value(0).toString());
    }
    ui->combo_medpart->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void add_assig_rec_wnd::refresh_combo_medsites() {
    QString code_mo = ui->combo_medorg->currentData().toString();
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mp, code_ms, '(' ||code_mp||', '||code_ms|| ') ' || name_ms "
                  "  from spr_medsites s "
                  " where code_mo='" + code_mo + "' "
                  " order by code_mp, code_ms ; ";
    mySQL.exec(this, sql, QString("Справочник участков медорганизации"), *query, true, db, data_app);
    ui->combo_medsite->clear();
    ui->combo_medsite->addItem("(0)  - без участка -", "0");
    while (query->next()) {
        ui->combo_medsite->addItem(query->value(2).toString(), query->value(1).toString());
    }
    ui->combo_medsite->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void add_assig_rec_wnd::refresh_combo_medters() {
    QString code_mo = ui->combo_medorg->currentData().toString();
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select snils_mt, '(' || snils_mt || ') ' || fio_mt "
                  "  from spr_medters s "
                  " where code_mo='" + code_mo + "' "
                  " order by fio_mt ; ";
    mySQL.exec(this, sql, QString("Справочник врачей медорганизации"), *query, true, db, data_app);
    ui->combo_medter->clear();
    ui->combo_medter->addItem("  - без участкового врача - ", "0");
    while (query->next()) {
        ui->combo_medter->addItem(query->value(1).toString(), query->value(0).toString());
    }
    ui->combo_medter->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}


void add_assig_rec_wnd::on_ch_unsig_toggled(bool checked) {
    ui->date_unsig->setEnabled(checked);
}

void add_assig_rec_wnd::on_bn_close_clicked() {
    reject();
}

void add_assig_rec_wnd::on_bn_save_clicked() {
    if (!f_edit) {
        QMessageBox::information(this, "Ничего не изменилось",
                                 "Данные открыты без возможности правки.\n\n"
                                 "Ничего не изменилось.");
        reject();

    } else if (data_assig_rec.id<=0) {
        // добавим новую запись
        accept();

    } else {
        // обновим запись
        accept();

    }
}
