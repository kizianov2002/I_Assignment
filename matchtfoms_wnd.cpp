#include "matchtfoms_wnd.h"
#include "ui_matchtfoms_wnd.h"

matchTFOMS_wnd::matchTFOMS_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::matchTFOMS_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    ui->split_dn->setStretchFactor(0,2);
    ui->split_dn->setStretchFactor(1,1);

    refresh_combo_files();
    refresh_tab_file();
    refresh_tab_assigs();
    refresh_combo_medorg();
    refresh_data_assig();
    refresh_combo_medpart();
    refresh_combo_medsite();
    refresh_combo_medter();
}

matchTFOMS_wnd::~matchTFOMS_wnd() {
    delete ui;
}

void matchTFOMS_wnd::on_bn_close_clicked() {
    close();
}

void matchTFOMS_wnd::on_bn_tab_file_refresh_clicked() {
    refresh_tab_file();
    refresh_tab_assigs();
    refresh_data_assig();
}

void matchTFOMS_wnd::on_combo_files_currentIndexChanged(int index) {
    refresh_tab_file();
    refresh_tab_assigs();
    refresh_data_assig();
}

void matchTFOMS_wnd::refresh_combo_files() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка участков
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select id, date_send||'  -  '||file_name as file_name "
                  "  from files_out_assig "
                  " order by '('||date_send||')  '||file_name desc; ";

    mySQL.exec(this, sql, QString("Список файлов"), *query, true, db, data_app);
    ui->combo_files->clear();
    ui->combo_files->addItem(" - выберите файл - ", -1);
    while (query->next()) {
        ui->combo_files->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void matchTFOMS_wnd::refresh_combo_medorg() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка медорганизаций
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mo, '('||code_mo||')  '||name_mo "
                  "  from spr_medorgs "
                  " where is_head=1 "
                  " order by code_mo ; ";

    mySQL.exec(this, sql, QString("Список медорганизаций"), *query, true, db, data_app);
    ui->combo_medorg->clear();
    ui->combo_medorg->addItem(" - выберите медорганизацию - ", -1);
    while (query->next()) {
        ui->combo_medorg->addItem(query->value(1).toString(), query->value(0).toString());
    }
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void matchTFOMS_wnd::refresh_combo_medpart() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка подразделений медорганизации
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mp, '('||code_mp||')  '||name_mp "
                  "  from spr_medparts "
                  " where code_mo='" + ui->combo_medorg->currentData().toString() + "' "
                  " order by code_mp ; ";

    mySQL.exec(this, sql, QString("Список подразделений медорганизации"), *query, true, db, data_app);
    ui->combo_medpart->clear();
    ui->combo_medpart->addItem(" - выберите подразделение - ", -1);
    while (query->next()) {
        ui->combo_medpart->addItem(query->value(1).toString(), query->value(0).toString());
    }
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void matchTFOMS_wnd::refresh_combo_medsite() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка врачебных участков медорганизации
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_ms, '('||code_ms||')  '||name_ms "
                  "  from spr_medsites "
                  " where code_mo='" + ui->combo_medorg->currentData().toString() + "' "
                  "   and code_mp='" + ui->combo_medpart->currentData().toString() + "' "
                  " order by code_ms ; ";

    mySQL.exec(this, sql, QString("Список участков медорганизации"), *query, true, db, data_app);
    ui->combo_medsite->clear();
    ui->combo_medsite->addItem(" - выберите врачебный участок - ", -1);
    while (query->next()) {
        ui->combo_medsite->addItem(query->value(1).toString(), query->value(0).toString());
    }
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void matchTFOMS_wnd::refresh_combo_medter() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка врачебных участков медорганизации
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select snils_mt, '('||snils_mt||')  '||fio_mt "
                  "  from spr_medters "
                  " where code_mo='" + ui->combo_medorg->currentData().toString() + "' "
                  " order by fio_mt ; ";

    mySQL.exec(this, sql, QString("Список врачей медорганизации"), *query, true, db, data_app);
    ui->combo_medter->clear();
    ui->combo_medter->addItem(" - выберите участкового врача - ", -1);
    while (query->next()) {
        ui->combo_medter->addItem(query->value(1).toString(), query->value(0).toString());
    }
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void matchTFOMS_wnd::refresh_tab_file() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }

    QDate date18 = QDate::currentDate().addYears(-18);
    QString date18_s = date18.toString("yyyy-MM-dd");

    QString pers_sql;
    pers_sql  = "select id, n, id_person, pol_id, enp, fam, im, ot, date_birth, plc_birth, doc_type, doc_sernum, doc_date, doc_org, snils, assig_type, assig_way, assig_date, unsig_date, code_mo, code_mp, code_ms, snils_mt, oid_mo, prof_mt, action_event "
                "  from frecs_out_assig "
                " where id_file_assig=" + QString::number(ui->combo_files->currentData().toInt()) + " ; ";

    model_file.setQuery(pers_sql,db);
    QString err2 = model_file.lastError().driverText();

    // подключаем модель из БД
    ui->tab_file->setModel(&model_file);

    // обновляем таблицу
    ui->tab_file->reset();

    // задаём ширину колонок
    ui->tab_file->setColumnWidth(  0,  2);   // id
    ui->tab_file->setColumnWidth(  1, 40);   // n,
    ui->tab_file->setColumnWidth(  2,  2);   // id_person,
    ui->tab_file->setColumnWidth(  3,100);   // pol_id,
    ui->tab_file->setColumnWidth(  4,110);   // enp,
    ui->tab_file->setColumnWidth(  5, 90);   // fam,
    ui->tab_file->setColumnWidth(  6, 70);   // im,
    ui->tab_file->setColumnWidth(  7, 80);   // ot,
    ui->tab_file->setColumnWidth(  8, 70);   // date_birth,
    ui->tab_file->setColumnWidth(  9,  2);   // plc_birth,
    ui->tab_file->setColumnWidth( 10,  2);   // doc_type,
    ui->tab_file->setColumnWidth( 11,  2);   // doc_sernum,
    ui->tab_file->setColumnWidth( 12,  2);   // doc_date,
    ui->tab_file->setColumnWidth( 13,  2);   // doc_org,
    ui->tab_file->setColumnWidth( 14,  2);   // snils,
    ui->tab_file->setColumnWidth( 15, 40);   // assig_type,
    ui->tab_file->setColumnWidth( 16,  2);   // assig_way,
    ui->tab_file->setColumnWidth( 17, 70);   // assig_date,
    ui->tab_file->setColumnWidth( 18,  2);   // unsig_date,
    ui->tab_file->setColumnWidth( 19, 60);   // code_mo,
    ui->tab_file->setColumnWidth( 20,100);   // code_mp,
    ui->tab_file->setColumnWidth( 21,100);   // code_ms,
    ui->tab_file->setColumnWidth( 22, 90);   // snils_mt,
    ui->tab_file->setColumnWidth( 23,  2);   // oid_mo,
    ui->tab_file->setColumnWidth( 24, 40);   // prof_mt,
    ui->tab_file->setColumnWidth( 25, 40);   // action_event

    // правим заголовки
    /*
    model_pers.setHeaderData( 0, Qt::Horizontal, ("ID"));        // e.id as id_pers
    model_pers.setHeaderData( 1, Qt::Horizontal, ("фамилия"));        // e.fam
    model_pers.setHeaderData( 2, Qt::Horizontal, ("имя"));        // e.im
    model_pers.setHeaderData( 3, Qt::Horizontal, ("отчество"));        // e.ot
    model_pers.setHeaderData( 4, Qt::Horizontal, ("пол"));        // e.sex
    model_pers.setHeaderData( 5, Qt::Horizontal, ("дата рожд."));        // e.date_birth
    model_pers.setHeaderData( 6, Qt::Horizontal, ("дата смер."));        // e.date_death
    model_pers.setHeaderData( 7, Qt::Horizontal, ("СНИЛС"));        // e.snils
    model_pers.setHeaderData( 8, Qt::Horizontal, ("id_assig"));        // a.id as id_assig
    model_pers.setHeaderData( 9, Qt::Horizontal, ("код МО"));        // a.code_mo
    model_pers.setHeaderData( 10, Qt::Horizontal, ("подразд."));        // a.code_mp
    model_pers.setHeaderData( 11, Qt::Horizontal, ("участок"));        // a.code_ms
    model_pers.setHeaderData( 12, Qt::Horizontal, ("врач"));        // a.snils_mt
    model_pers.setHeaderData( 13, Qt::Horizontal, ("способ"));        // a.assig_type
    model_pers.setHeaderData( 14, Qt::Horizontal, ("дата прик"));        // a.assig_date
    model_pers.setHeaderData( 15, Qt::Horizontal, ("дата откр"));        // a.unsig_date
    model_pers.setHeaderData( 16, Qt::Horizontal, ("акт."));        // a.is_actual
    model_pers.setHeaderData( 17, Qt::Horizontal, ("дата рег."));        // e.date_reg
    model_pers.setHeaderData( 18, Qt::Horizontal, ("БОМЖ"));        // e.bomj
    model_pers.setHeaderData( 19, Qt::Horizontal, ("ОКАТО рег"));        // reg.ocato as ocato_reg
    model_pers.setHeaderData( 20, Qt::Horizontal, ("КЛАДР рег"));        // reg.kladr as kladr_reg
    model_pers.setHeaderData( 21, Qt::Horizontal, ("КЛАДР_СТРТ рег"));        // reg.kladr_street as kladr_street_reg
    model_pers.setHeaderData( 22, Qt::Horizontal, ("регион рег"));        // subj_name as subj_name_reg
    model_pers.setHeaderData( 23, Qt::Horizontal, ("район рег"));        // dstr_name as dstr_name_reg
    model_pers.setHeaderData( 24, Qt::Horizontal, ("город рег"));        // city_name as city_name_reg
    model_pers.setHeaderData( 25, Qt::Horizontal, ("нас.п рег"));        // nasp_name as nasp_name_reg
    model_pers.setHeaderData( 26, Qt::Horizontal, ("улица рег"));        // strt_name as strt_name_reg
    model_pers.setHeaderData( 27, Qt::Horizontal, ("дом рег"));        // strt_name as strt_name_reg
    model_pers.setHeaderData( 28, Qt::Horizontal, ("к. рег"));        // strt_name as strt_name_reg
    model_pers.setHeaderData( 29, Qt::Horizontal, ("кв. рег"));        // strt_name as strt_name_reg
    */
    ui->tab_file->repaint();

    this->setCursor(Qt::ArrowCursor);
}

void matchTFOMS_wnd::on_bn_refresh_tab_assigs_clicked() {
    refresh_tab_assigs();
    refresh_data_assig();
}
void matchTFOMS_wnd::on_tab_file_clicked(const QModelIndex &index) {
    refresh_tab_assigs();
    refresh_data_assig();
}

void matchTFOMS_wnd::on_combo_medorg_activated(int index) {
    refresh_combo_medpart();
    refresh_combo_medsite();
    refresh_combo_medter();
}

void matchTFOMS_wnd::on_combo_medpart_activated(int index) {
    refresh_combo_medsite();
    refresh_combo_medter();
}

void matchTFOMS_wnd::refresh_tab_assigs() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_file || !ui->tab_file->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица с данными файла по прикреплению не загружена!\nНажмите кнопку [Обновить] и выберите строку из списка.");
        return;
    }
    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_file->selectionModel()->selection().indexes();
    int id_person = -1;

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();
        id_person = model_file.data(model_file.index(index.row(), 2), Qt::EditRole).toInt();
    }


    QString sql_assigs;
    sql_assigs  = "select a.id, a.id_person, a.code_mo, mo.name_mo, a.code_mp, mp.name_mp, a.code_ms, ms.name_ms, a.snils_mt, mt.fio_mt, a.assig_type, a.assig_way, a.assig_date, a.unsig_date, a.is_actual "
                  "  from persons_assig a "
                  "  left join spr_medorgs  mo on(mo.code_mo=a.code_mo and mo.is_head=1) "
                  "  left join spr_medparts mp on(mp.code_mo=a.code_mo and mp.code_mp=a.code_mp) "
                  "  left join spr_medsites ms on(ms.code_mo=a.code_mo and ms.code_mp=a.code_mp and ms.code_ms=a.code_ms) "
                  "  left join spr_medters  mt on(mt.code_mo=a.code_mo and mt.code_mp=a.code_mp and mt.code_ms=a.code_ms and mt.snils_mt=a.snils_mt) "
                  " where a.id_person=" + QString::number(id_person) + " "
                  " order by a.is_actual ; ";

    model_assigs.setQuery(sql_assigs, db);
    QString err2 = model_assigs.lastError().driverText();

    // подключаем модель из БД
    ui->tab_assigs->setModel(&model_assigs);

    // обновляем таблицу
    ui->tab_assigs->reset();

    // задаём ширину колонок
    ui->tab_assigs->setColumnWidth(  0,  2);   // a.id,
    ui->tab_assigs->setColumnWidth(  1,  2);   // a.id_person,
    ui->tab_assigs->setColumnWidth(  2, 60);   // a.code_mo,
    ui->tab_assigs->setColumnWidth(  3,150);   // mo.name_mo,
    ui->tab_assigs->setColumnWidth(  4,  2);   // a.code_mp,
    ui->tab_assigs->setColumnWidth(  5,100);   // mp.name_mp,
    ui->tab_assigs->setColumnWidth(  6,  2);   // a.code_ms,
    ui->tab_assigs->setColumnWidth(  7, 80);   // ms.name_ms,
    ui->tab_assigs->setColumnWidth(  8, 90);   // a.snils_mt,
    ui->tab_assigs->setColumnWidth(  9,  2);   // mt.fio_mt,
    ui->tab_assigs->setColumnWidth( 10, 40);   // a.assig_type,
    ui->tab_assigs->setColumnWidth( 12, 40);   // a.assig_way,
    ui->tab_assigs->setColumnWidth( 13, 80);   // a.assig_date,
    ui->tab_assigs->setColumnWidth( 14, 80);   // a.unsig_date,
    ui->tab_assigs->setColumnWidth( 15, 40);   // a.is_actual

    // правим заголовки
    /*
    model_pers.setHeaderData( 0, Qt::Horizontal, ("ID"));        // e.id as id_pers
    model_pers.setHeaderData( 1, Qt::Horizontal, ("фамилия"));        // e.fam
    model_pers.setHeaderData( 2, Qt::Horizontal, ("имя"));        // e.im
    model_pers.setHeaderData( 3, Qt::Horizontal, ("отчество"));        // e.ot
    model_pers.setHeaderData( 4, Qt::Horizontal, ("пол"));        // e.sex
    model_pers.setHeaderData( 5, Qt::Horizontal, ("дата рожд."));        // e.date_birth
    model_pers.setHeaderData( 6, Qt::Horizontal, ("дата смер."));        // e.date_death
    model_pers.setHeaderData( 7, Qt::Horizontal, ("СНИЛС"));        // e.snils
    model_pers.setHeaderData( 8, Qt::Horizontal, ("id_assig"));        // a.id as id_assig
    model_pers.setHeaderData( 9, Qt::Horizontal, ("код МО"));        // a.code_mo
    */
    ui->tab_assigs->repaint();

    this->setCursor(Qt::ArrowCursor);
}

void matchTFOMS_wnd::refresh_data_assig() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_assigs || !ui->tab_assigs->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица с данными прикрепления застрахованного пуста!\nВыберите строку файла (сверху).");
        return;
    }
    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_assigs->selectionModel()->selection().indexes();
    int id_assig = -1;

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();
        id_assig = model_assigs.data(model_assigs.index(index.row(), 2), Qt::EditRole).toInt();
        int n_mo = ui->combo_medorg ->findData(model_assigs.data(model_assigs.index(index.row(), 2), Qt::EditRole).toString());
        ui->combo_medorg ->setCurrentIndex(n_mo>=0 ? n_mo : 0);
        refresh_combo_medpart();
        refresh_combo_medsite();
        refresh_combo_medter();
        int n_mp = ui->combo_medpart->findData(model_assigs.data(model_assigs.index(index.row(), 4), Qt::EditRole).toString());
        ui->combo_medpart->setCurrentIndex(n_mp>=0 ? n_mp : 0);
        refresh_combo_medsite();
        refresh_combo_medter();
        int n_ms = ui->combo_medsite->findData(model_assigs.data(model_assigs.index(index.row(), 6), Qt::EditRole).toString());
        ui->combo_medsite->setCurrentIndex(n_ms>=0 ? n_ms : 0);
        refresh_combo_medter();
        int n_mt = ui->combo_medter ->findData(model_assigs.data(model_assigs.index(index.row(), 8), Qt::EditRole).toString());
        ui->combo_medter ->setCurrentIndex(n_mt>=0 ? n_mt : 0);
        int assig_type = model_assigs.data(model_assigs.index(index.row(),10), Qt::EditRole).toInt();
        ui->combo_assig_type->setCurrentIndex(assig_type>0 ? assig_type : 0);
        int assig_way = model_assigs.data(model_assigs.index(index.row(),11), Qt::EditRole).toInt();
        ui->combo_assig_way->setCurrentIndex(assig_way>=0 ? assig_way : 0);
        ui->date_assig->setDate(model_assigs.data(model_assigs.index(index.row(),12), Qt::EditRole).toDate());

        ui->ch_date_assig->setChecked(!model_assigs.data(model_assigs.index(index.row(),12), Qt::EditRole).isNull());
        ui->date_assig->setVisible(!model_assigs.data(model_assigs.index(index.row(),12), Qt::EditRole).isNull());
        ui->date_assig->setDate(model_assigs.data(model_assigs.index(index.row(),12), Qt::EditRole).toDate());

        ui->ch_date_unsig->setChecked(!model_assigs.data(model_assigs.index(index.row(),13), Qt::EditRole).isNull());
        ui->date_unsig->setVisible(!model_assigs.data(model_assigs.index(index.row(),13), Qt::EditRole).isNull());
        ui->date_unsig->setDate(model_assigs.data(model_assigs.index(index.row(),13), Qt::EditRole).toDate());
    } else {
        ui->combo_medorg ->setCurrentIndex(0);
        refresh_combo_medpart();
        refresh_combo_medsite();
        refresh_combo_medter();
        ui->combo_medpart->setCurrentIndex(0);
        ui->combo_medsite->setCurrentIndex(0);
        ui->combo_medter ->setCurrentIndex(0);
        ui->combo_assig_type->setCurrentIndex(0);
        ui->combo_assig_way->setCurrentIndex(0);
        ui->ch_date_assig->setChecked(false);
        ui->date_assig->setVisible(false);
        ui->date_assig->setDate(QDate::currentDate());
        ui->ch_date_unsig->setChecked(false);
        ui->date_unsig->setVisible(false);
        ui->date_unsig->setDate(QDate::currentDate());
    }
}

void matchTFOMS_wnd::on_tab_assigs_clicked(const QModelIndex &index) {
    refresh_data_assig();
}

void matchTFOMS_wnd::on_ch_date_assig_clicked(bool checked) {
    ui->date_assig->setVisible(checked);
}
void matchTFOMS_wnd::on_ch_date_unsig_clicked(bool checked) {
    ui->date_unsig->setVisible(checked);
}


void matchTFOMS_wnd::on_bn_add_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }

    // проверка, введен ли код МО
    if (ui->combo_medorg->currentData().toString().isEmpty()) {
        QMessageBox::warning(this, "Недостаточно данных",
                             "Выберите медицинскую организацию, к которой осуществляется прикрепление.");
        return;
    }

    // проверка, введена ли дата прикрепления
    if (!ui->ch_date_assig->isChecked() ||
         ui->date_assig->date()<QDate(2015,1,1) ||
         ui->date_assig->date()>QDate::currentDate() )
    {
        QMessageBox::warning(this, "Недостаточно данных",
                             "Дата прикепления не может быть раньше 01.01.2000 \nили позже текущего дня.");
        return;
    }

    // проверка способа прикрепления
    if (ui->combo_assig_type->currentIndex()==0) {
        if (QMessageBox::warning(this, "Не указан способ прикрепления",
                                 "Не указан способ прикрепления.\n"
                                 "Выбрать способ прикрепления \"По территориальному принципу\" ? ",
                                 QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Yes)==QMessageBox::Yes) {
            ui->combo_assig_type->setCurrentIndex(1);
        } else {
            return;
        }
    }

    // проверим выделенную строку прикрепления
    QModelIndexList indexes = ui->tab_assigs->selectionModel()->selection().indexes();
    int id_person = -1;
    int id_assig  = -1;

    if (indexes.size()>0) {
        if (QMessageBox::warning(this, "Нужно подтверждение",
                                 "Вы действительно хотите добавить новую строку записи прикрепления застрахованного к МО с автоматическим закрытием предыдущей строки прикрепления с (!!!) УДАЛЕНИЕМ ВСЕХ ЗАПИСЕЙ ПРИКРЕПЛЕНИЯ ПОСЛЕ УКАЗАННОЙ ДАТЫ ПРИКРЕПЛЕНИЯ (!!!), \nпри необходимости ? \n\n"
                                 "Осторожно, это необратимая операция. ",
                                 QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Yes)==QMessageBox::Cancel) {
            return;
        }

        QModelIndex index = indexes.front();
        id_person = model_assigs.data(model_assigs.index(index.row(), 1), Qt::EditRole).toInt();
        id_assig = model_assigs.data(model_assigs.index(index.row(), 0), Qt::EditRole).toInt();

        // проверка, введен ли способ прикреплния
        if (ui->combo_medorg->currentData().toString().isEmpty()) {
            if (QMessageBox::warning(this,
                                     "Способ прикрепления не указан",
                                     "Способ прикрепления не указан.\nПрололжить сохранение?\n",
                                     QMessageBox::Yes|QMessageBox::Cancel,
                                     QMessageBox::Yes)!=QMessageBox::Yes) {
                return;
            }
        }
        db.transaction();

        QString  code_mo = ui->combo_medorg->currentData().toString().left(6);
        QString  code_mp = ui->combo_medpart->currentData().toString().left(64);
        QString  code_ms = ui->combo_medsite->currentData().toString().left(64);
        QString snils_mt = ui->combo_medter->currentData().toString().trimmed().simplified().replace(" ","").replace("-","").left(11);
        int   assig_type = ui->combo_assig_type->currentIndex();
        QDate assig_date = ui->date_assig->date();


        // сравнение действующего и нового прикреплений
        QString sql_old;
        sql_old += "select a.code_mo, a.code_mp, a.code_ms, a.snils_mt, a.assig_type, a.assig_date "
                   " from persons_assig a "
                   " where a.id_person=" + QString::number(id_person) + " and a.is_actual=0 ; ";

        QSqlQuery *query_old = new QSqlQuery(db);
        if ( mySQL.exec(this, sql_old, "Сравнение действующего и нового прикреплений", *query_old, true, db, data_app) &&
             query_old->size()>0 &&
             query_old->next() ) {
            QString  code_mo_old = query_old->value(0).toString().simplified().trimmed();
            QString  code_mp_old = query_old->value(1).toString().simplified().trimmed();
            QString  code_ms_old = query_old->value(2).toString().simplified().trimmed();
            QString snils_mt_old = query_old->value(3).toString().simplified().trimmed();
            int   assig_type_old = query_old->value(4).toInt();
            QDate assig_date_old = query_old->value(5).toDate();

            if ( code_mo==code_mo_old &&
                 code_mp==code_mp_old &&
                 code_ms==code_ms_old &&
                 snils_mt==snils_mt_old &&
                 assig_type==assig_type_old &&
                 assig_date==assig_date_old) {

                // такое прикрепление уже есть - пропустим
                db.rollback();
                delete query_old;
                return;
            }
        }

        // зададим дату открепления прошлого прикрепления
        QString sql_close = "delete from persons_assig "
                            " where id_person=" + QString::number(id_person) + " "
                            "   and (assig_date is NULL or assig_date>='" + assig_date.toString("yyyy-MM-dd") + "') ; ";
                            "update persons_assig "
                            "   set unsig_date='" + QDate::currentDate().toString("yyyy-MM-dd") + "'  "
                            " where id_person=" + QString::number(id_person) + " "
                            "   and unsig_date is NULL ; ";

        QSqlQuery *query_close = new QSqlQuery(db);
        if (!mySQL.exec(this, sql_close, "Закрытие старого прикрепления", *query_close, true, db, data_app)) {
            db.rollback();
            QMessageBox::critical(this,
                                  "Непредвиденная ошибка",
                                  "При попытке задания даты открепления старого прикрепления произошла неожиданная ошибка.\n\n"
                                  "Сохранение отменено.");
            delete query_close;
            return;
        }

        // добавим новое прикрепление
        QString sql = "update persons_assig "
                      "   set is_actual = is_actual +1 "
                      " where id_person = " + QString::number(id_person) + " ; "

                      "insert into persons_assig (id_person, code_mo, code_mp, code_ms, snils_mt, assig_type, assig_way, assig_date, unsig_date, is_actual) "
                      " values "
                      " ( "  + QString::number(id_person) +
                      "  ,'" + code_mo + "' "
                      "  ,'" + code_mp + "' "
                      "  ,'" + code_ms + "' "
                      "  ,'" + snils_mt + "' "
                      "  ,"  + QString::number(assig_type) +
                      "  ,NULL "
                      "  ," + (ui->ch_date_assig->isChecked() ? QString("'" + ui->date_assig->date().toString("yyyy-MM-dd") + "'") : "NULL") + " "
                      "  ," + (ui->ch_date_unsig->isChecked() ? QString("'" + ui->date_unsig->date().toString("yyyy-MM-dd") + "'") : "NULL") + " "
                      "  ,0 ) "
                      " returning id; ";

        QSqlQuery *query = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Добавление прикрепления к МО", *query, true, db, data_app) || !query->next()) {
            db.rollback();
            QMessageBox::critical(this,
                                  "Непредвиденная ошибка",
                                  "При попытке добавить прикрепление застрахованного к МО произошла неожиданная ошибка.\n\n"
                                  "Сохранение отменено.");
            delete query;
            return;
        }

        db.commit();
    } else {
        QMessageBox::warning(this, "Ничего не выбрано", "Ничего не выбрано");
    }

    // обновим связку персоны с прикреплением
    QString sql_link = "select update_pers_links(" + QString::number(id_person) + ", NULL) ; ";
    QSqlQuery *query_link = new QSqlQuery(db);
    if (!mySQL.exec(this, sql_link, "Обновление связки с прикреплением застрахованного", *query_link, true, db, data_app))
        return;
    delete query_link;

    refresh_tab_assigs();
    refresh_data_assig();
}

void matchTFOMS_wnd::on_bn_delete_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }

    // проверим выделенную строку прикрепления
    QModelIndexList indexes = ui->tab_assigs->selectionModel()->selection().indexes();
    int id_person = -1;
    int id_assig  = -1;

    if (indexes.size()>0) {
        if (QMessageBox::warning(this, "Нужно подтверждение",
                                 "Вы действительно хотите удалить выбранную строку записи прикрепления застрахованного к МО ? \n\n"
                                 "Осторожно, это необратимая операция. ",
                                 QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Yes)==QMessageBox::Cancel) {
            return;
        }

        QModelIndex index = indexes.front();
        id_person = model_assigs.data(model_assigs.index(index.row(), 1), Qt::EditRole).toInt();
        id_assig = model_assigs.data(model_assigs.index(index.row(), 0), Qt::EditRole).toInt();

        db.transaction();

        // закроем прикрепление
        QString sql = "update persons_assig "
                      "   set unsig_date='" + QDate::currentDate().toString("yyyy-MM-dd") + "' "
                      " where id=" + QString::number(id_assig) + " ; ";

        QSqlQuery *query = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Закрытие прикрепления к МО", *query, true, db, data_app) || !query->next()) {
            db.rollback();
            QMessageBox::critical(this,
                                  "Непредвиденная ошибка",
                                  "При попытке закрыть прикрепление застрахованного к МО произошла неожиданная ошибка.\n\n"
                                  "Сохранение отменено.");
            delete query;
            return;
        }

        db.commit();
    } else {
        QMessageBox::warning(this, "Ничего не выбрано", "Ничего не выбрано");
    }

    // обновим связку персоны с прикреплением
    QString sql_link = "select update_pers_links(" + QString::number(id_person) + ", NULL) ; ";
    QSqlQuery *query_link = new QSqlQuery(db);
    if (!mySQL.exec(this, sql_link, "Обновление связки с прикреплением застрахованного", *query_link, true, db, data_app))
        return;
    delete query_link;

    refresh_tab_assigs();
    refresh_data_assig();
}

void matchTFOMS_wnd::on_bn_edit_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }

    // проверка, введен ли код МО
    if (ui->combo_medorg->currentData().toString().isEmpty()) {
        QMessageBox::warning(this, "Недостаточно данных",
                             "Выберите медицинскую организацию, к которой осуществляется прикрепление.");
        return;
    }

    // проверка, введена ли дата прикрепления
    if (!ui->ch_date_assig->isChecked() ||
         ui->date_assig->date()<QDate(2015,1,1) ||
         ui->date_assig->date()>QDate::currentDate() )
    {
        QMessageBox::warning(this, "Недостаточно данных",
                             "Дата прикепления не может быть раньше 01.01.2000 \nили позже текущего дня.");
        return;
    }

    // проверка способа прикрепления
    if (ui->combo_assig_type->currentIndex()==0) {
        if (QMessageBox::warning(this, "Не указан способ прикрепления",
                                 "Не указан способ прикрепления.\n"
                                 "Выбрать способ прикрепления \"По территориальному принципу\" ? ",
                                 QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Yes)==QMessageBox::Yes) {
            ui->combo_assig_type->setCurrentIndex(1);
        } else {
            return;
        }
    }

    // проверим выделенную строку прикрепления
    QModelIndexList indexes = ui->tab_assigs->selectionModel()->selection().indexes();
    int id_person = -1;
    int id_assig  = -1;
    int is_actual = -1;

    if (indexes.size()>0) {
        if (QMessageBox::warning(this, "Нужно подтверждение",
                                 "Вы действительно хотите изменить выбранную строку записи прикрепления застрахованного к МО ? \n\n"
                                 "Осторожно, это необратимая операция. ",
                                 QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Yes)==QMessageBox::Cancel) {
            return;
        }

        QModelIndex index = indexes.front();
        id_person = model_assigs.data(model_assigs.index(index.row(), 1), Qt::EditRole).toInt();
        id_assig  = model_assigs.data(model_assigs.index(index.row(), 0), Qt::EditRole).toInt();
        is_actual = model_assigs.data(model_assigs.index(index.row(),15), Qt::EditRole).toInt();

        // проверка, введен ли способ прикреплния
        if (ui->combo_medorg->currentData().toString().isEmpty()) {
            if (QMessageBox::warning(this,
                                     "Способ прикрепления не указан",
                                     "Способ прикрепления не указан.\nПрололжить сохранение?\n",
                                     QMessageBox::Yes|QMessageBox::Cancel,
                                     QMessageBox::Yes)!=QMessageBox::Yes) {
                return;
            }
        }
        db.transaction();

        QString  code_mo = ui->combo_medorg->currentData().toString().left(6);
        QString  code_mp = ui->combo_medpart->currentData().toString().left(64);
        QString  code_ms = ui->combo_medsite->currentData().toString().left(64);
        QString snils_mt = ui->combo_medter->currentData().toString().trimmed().simplified().replace(" ","").replace("-","").left(11);
        int   assig_type = ui->combo_assig_type->currentIndex();
        int   assig_way  = ui->combo_assig_way ->currentIndex();
        QDate assig_date = ui->date_assig->date();
        QDate unsig_date = ui->date_unsig->date();

        // исправим прикрепление
        QString sql = "update persons_assig "
                      "   set code_mo='" + code_mo + "' "
                          " , code_mp='" + code_mp + "' "
                          " , code_ms='" + code_ms + "' "
                          " , snils_mt='" + snils_mt + "' "
                          " , assig_type=" + (assig_type>0 ? QString::number(assig_type) : "0") +
                          " , assig_way="  + (assig_way>0  ? QString::number(assig_way)  : "0") +
                          " , assig_date=" + (ui->ch_date_assig->isChecked() ? QString("'" + ui->date_assig->date().toString("yyyy-MM-dd") + "'") : "NULL") + " "
                          " , unsig_date=" + (ui->ch_date_unsig->isChecked() ? QString("'" + ui->date_unsig->date().toString("yyyy-MM-dd") + "'") : "NULL") + " "
                          " , status=" + (is_actual==0 ? "1" : "0") +
                      " where id=" + QString::number(id_assig) + " ";

        QSqlQuery *query = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Добавление прикрепления к МО", *query, true, db, data_app)) {
            db.rollback();
            QMessageBox::critical(this,
                                  "Непредвиденная ошибка",
                                  "При попытке добавить прикрепление застрахованного к МО произошла неожиданная ошибка.\n\n"
                                  "Сохранение отменено.");
            delete query;
            return;
        }

        db.commit();
    } else {
        QMessageBox::warning(this, "Ничего не выбрано", "Ничего не выбрано");
    }

    // обновим связку персоны с прикреплением
    QString sql_link = "select update_pers_links(" + QString::number(id_person) + ", NULL) ; ";
    QSqlQuery *query_link = new QSqlQuery(db);
    if (!mySQL.exec(this, sql_link, "Обновление связки с прикреплением застрахованного", *query_link, true, db, data_app))
        return;
    delete query_link;

    refresh_tab_assigs();
    refresh_data_assig();
}

void matchTFOMS_wnd::on_bn_auto_clicked()
{}

#include "qt_windows.h"
#include "qwindowdefs_win.h"
#include <shellapi.h>

void matchTFOMS_wnd::on_bn_save_MO_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if ( ui->combo_files->currentIndex()<0
         || ui->combo_files->currentData().toInt()<0 ) {
        QMessageBox::warning(this, "Файл не выбран!",
                             "В всписке файлов выберите файл, который хотите выгрузить на диск.");
        return;
    }

        QString sql_sel = "select a.file_name, a.bin "
                          "  from files_out_assig a "
                          " where a.id=" + QString::number(ui->combo_files->currentData().toInt()) + " ; ";

        QSqlQuery *query_sel = new QSqlQuery(db);
        if (!mySQL.exec(this, sql_sel, "Получим MO-файл", *query_sel, true, db, data_app)) {
            QMessageBox::critical(this, "Непредвиденная ошибка",
                                  "При попытке получить MO-файл произошла неожиданная ошибка.\n\n"
                                  "Действие отменено.");
            delete query_sel;
            return;
        }
        if (query_sel->next()) {
            QString fname_res = /*data_app.path_temp*/ "d:/temp/" + query_sel->value(0).toString() + ".dbf";
            //QString str = QString::fromLocal8Bit(query_sel->value(1).toByteArray());
            QByteArray arr = query_sel->value(1).toByteArray();
            //QString str = QString::toLocal8Bit(arr);

            QFile f(fname_res);

            f.remove();
            if (!f.open(QIODevice::WriteOnly)) {
                QMessageBox::warning(this,
                                     "Ошибка создания файла",
                                     "При сохранении целевого MO-файла произошла ошибка.\n\n"
                                     "Операция отменена.");
                return;
            }
            f.write(arr);
            f.close();
            long result = (long long)ShellExecute(0, 0, reinterpret_cast<const WCHAR*>(fname_res.utf16()), 0, 0, SW_NORMAL);
        } else {
            QMessageBox::warning(this, "Файл не найден",
                                 "MO-файл не найден.");
        }
}
