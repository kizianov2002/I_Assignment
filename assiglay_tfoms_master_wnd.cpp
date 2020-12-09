#include "assiglay_tfoms_master_wnd.h"
#include "ui_assiglay_tfoms_master_wnd.h"

#include <QFileDialog>
#include "ki_funcs.h"

assiglay_tfoms_master_wnd::assiglay_tfoms_master_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::assiglay_tfoms_master_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    assiglay_data_w = NULL;

    ui->pan_wait->setVisible(false);
    ui->lab_process->setText(" -//- ");

    refresh_data_status();
    refresh_import_tab();
    refresh_combo_SQL();
}

assiglay_tfoms_master_wnd::~assiglay_tfoms_master_wnd() {
    delete ui;
}

void assiglay_tfoms_master_wnd::refresh_data_status() {
    this->setCursor(Qt::WaitCursor);

    int cnt_all, cnt_pers_yes, cnt_pers_no, cnt_pol_yes, cnt_pol_no;

    QString sql_cnt_all;
    sql_cnt_all = "select count(*) "
                  "  from assig.import_data ; ";
    QSqlQuery *query_cnt_all = new QSqlQuery(db);
    bool res_cnt_all = query_cnt_all->exec(sql_cnt_all);

    if (!res_cnt_all) {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при подсчёте cnt_all",
                             "При попытке подсчитать cnt_all произошла неожиданная ошибка.\n\n"
                             + query_cnt_all->lastError().driverText() + "\n" + query_cnt_all->lastError().databaseText() + "\n\n"
                             "Операция отменена.");
    } else {
        query_cnt_all->next();
        cnt_all = query_cnt_all->value(0).toInt();
        ui->cnt_all->setText(show_num(cnt_all));
    }
    delete query_cnt_all;


    QString sql_cnt_pers_yes;
    sql_cnt_pers_yes = "select count(*) "
                       "  from assig.import_data "
                       " where id_person is not NULL ; ";
    QSqlQuery *query_cnt_pers_yes = new QSqlQuery(db);
    bool res_cnt_pers_yes = query_cnt_pers_yes->exec(sql_cnt_pers_yes);

    if (!res_cnt_pers_yes) {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при подсчёте cnt_pers_yes",
                             "При попытке подсчитать cnt_pers_yes произошла неожиданная ошибка.\n\n"
                             + query_cnt_pers_yes->lastError().driverText() + "\n" + query_cnt_pers_yes->lastError().databaseText() + "\n\n"
                             "Операция отменена.");
    } else {
        query_cnt_pers_yes->next();
        cnt_pers_yes = query_cnt_pers_yes->value(0).toInt();
        ui->cnt_pers_yes->setText(show_num(cnt_pers_yes));
    }
    delete query_cnt_pers_yes;


    QString sql_cnt_pers_no;
    sql_cnt_pers_no = "select count(*) "
                      "  from assig.import_data "
                      " where id_person is NULL ; ";
    QSqlQuery *query_cnt_pers_no = new QSqlQuery(db);
    bool res_cnt_pers_no = query_cnt_pers_no->exec(sql_cnt_pers_no);

    if (!res_cnt_pers_no) {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при подсчёте cnt_pers_no",
                             "При попытке подсчитать cnt_pers_no произошла неожиданная ошибка.\n\n"
                             + query_cnt_pers_no->lastError().driverText() + "\n" + query_cnt_pers_no->lastError().databaseText() + "\n\n"
                             "Операция отменена.");
    } else {
        query_cnt_pers_no->next();
        cnt_pers_no = query_cnt_pers_no->value(0).toInt();
        ui->cnt_pers_no->setText(show_num(cnt_pers_no));
    }
    delete query_cnt_pers_no;


    QString sql_cnt_pol_yes;
    sql_cnt_pol_yes = "select count(*) "
                       "  from assig.import_data "
                       " where id_polis is not NULL ; ";
    QSqlQuery *query_cnt_pol_yes = new QSqlQuery(db);
    bool res_cnt_pol_yes = query_cnt_pol_yes->exec(sql_cnt_pol_yes);

    if (!res_cnt_pol_yes) {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при подсчёте cnt_pol_yes",
                             "При попытке подсчитать cnt_pol_yes произошла неожиданная ошибка.\n\n"
                             + query_cnt_pol_yes->lastError().driverText() + "\n" + query_cnt_pol_yes->lastError().databaseText() + "\n\n"
                             "Операция отменена.");
    } else {
        query_cnt_pol_yes->next();
        cnt_pol_yes = query_cnt_pol_yes->value(0).toInt();
        ui->cnt_pol_yes->setText(show_num(cnt_pol_yes));
    }
    delete query_cnt_pol_yes;


    QString sql_cnt_pol_no;
    sql_cnt_pol_no = "select count(*) "
                      "  from assig.import_data "
                      " where id_polis is NULL ; ";
    QSqlQuery *query_cnt_pol_no = new QSqlQuery(db);
    bool res_cnt_pol_no = query_cnt_pol_no->exec(sql_cnt_pol_no);

    if (!res_cnt_pol_no) {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при подсчёте cnt_pol_no",
                             "При попытке подсчитать cnt_pol_no произошла неожиданная ошибка.\n\n"
                             + query_cnt_pol_no->lastError().driverText() + "\n" + query_cnt_pol_no->lastError().databaseText() + "\n\n"
                             "Операция отменена.");
    } else {
        query_cnt_pol_no->next();
        cnt_pol_no = query_cnt_pol_no->value(0).toInt();
        ui->cnt_pol_no->setText(show_num(cnt_pol_no));
    }
    delete query_cnt_pol_no;

    QString log = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ">> "
                  " СТАТУС: строк: " + show_num(cnt_all) + ",  "
                  "ID_person: " + show_num(cnt_pers_yes) + " / " + show_num(cnt_pers_no) + ",  "
                  "ID_polis: " + show_num(cnt_pol_yes) + " / " + show_num(cnt_pol_no) + ".  ";
    ui->te_log->append(log);

    QApplication::processEvents();
    this->setCursor(Qt::ArrowCursor);
}


void assiglay_tfoms_master_wnd::refresh_combo_SQL() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка медорганизаций
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select id, sql_name, sql_text "
                  "  from assig.import_sql s "
                  " order by id ; ";
    mySQL.exec(this, sql, QString("Список запросов для подготовки импорта слоя ТФОМС"), *query, true, db, data_app);
    ui->combo_SQL->clear();
    SQL_ids.clear();
    SQL_names.clear();
    SQL_textes.clear();
    while (query->next()) {
        SQL_ids.append(query->value(0).toInt());
        SQL_names.append(query->value(1).toString());
        SQL_textes.append(query->value(2).toString());
        ui->combo_SQL->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void assiglay_tfoms_master_wnd::on_combo_SQL_currentIndexChanged(int index) {
    ui->te_SQL->setText(SQL_textes.at(ui->combo_SQL->currentIndex()));
}


void assiglay_tfoms_master_wnd::refresh_import_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    ui->tab_import->setEnabled(false);
    QApplication::processEvents();

    // подготовим фильтр
    QString filters = "";
    if (ui->ch_filter_person->isChecked()) {
        if (ui->ch_filter_person_yes->isChecked())
            filters += " and i.id_person is not NULL ";
        if (ui->ch_filter_person_no->isChecked())
            filters += " and i.id_person is NULL ";
    }
    if (ui->ch_filter_polis->isChecked()) {
        if (ui->ch_filter_polis_yes->isChecked())
            filters += " and i.id_polis is not NULL ";
        if (ui->ch_filter_polis_no->isChecked())
            filters += " and i.id_polis is NULL ";
    }

    // подсчитаем количество строк
    QString sql_cnt_tab;
    sql_cnt_tab = "select count(*) "
                  "  from assig.import_data i "
                  " where 1=1 #FILTERS# ; ";
    sql_cnt_tab = sql_cnt_tab.replace("#FILTERS#", filters);
    QSqlQuery *query_cnt_tab = new QSqlQuery(db);
    bool res_cnt_tab = query_cnt_tab->exec(sql_cnt_tab);

    if (!res_cnt_tab) {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при подсчёте числа строк в таблице",
                             "При попытке подсчитать число строк в таблице произошла неожиданная ошибка.\n\n"
                             + query_cnt_tab->lastError().driverText() + "\n" + query_cnt_tab->lastError().databaseText() + "\n\n"
                             "Операция отменена.");
    } else {
        query_cnt_tab->next();
        int cnt_tab = query_cnt_tab->value(0).toInt();
        ui->lab_cnt_tab->setText("   " + show_num(cnt_tab));
    }
    delete query_cnt_tab;

    sql_import_tab = "select id, ic_id, sp_id, "
                     "       pol_type, pol_ser, pol_num, pol_begin, pol_end, "
                     "       last_name, first_name, middle_name, birth_date, sex, birth_place, snils, death_date, "
                     "       zip_code, region, district, city, settle, street, house, building, flat, "
                     "       doc_type, doc_ser, doc_num, doc_begin, doc_end, citizenship, "
                     "       a1_med_type, a1_med_org_id, a1_assign_type, a1_assign_date, a1_snils_mt, a1_code_ms, "
                     "       id_person, id_polis, pol_v, "
                     "       subj_name, dstr_name, city_name, nasp_name, strt_name, house_num, corp, "
                     "       code_mo, code_mp, code_ms, snils_mt "
                     "  from assig.import_data i "
                     " where 1=1 #FILTERS# "
                     " order by last_name, first_name, middle_name, birth_date, sex, birth_place, snils, death_date  "
                     " limit " + QString::number(ui->spin_limit->value()) + " ; ";
    sql_import_tab = sql_import_tab.replace("#FILTERS#", filters);

    model_import_tab.setQuery(sql_import_tab,db);
    QString err2 = model_import_tab.lastError().driverText();

    // подключаем модель из БД
    ui->tab_import->setModel(&model_import_tab);

    // обновляем таблицу
    ui->tab_import->reset();

    // задаём ширину колонок
    ui->tab_import->setColumnWidth(  0,  2);     // id
    ui->tab_import->setColumnWidth(  2,120);     // sp_id
    ui->tab_import->setColumnWidth(  8, 70);     // fam
    ui->tab_import->setColumnWidth(  9, 70);     // im
    ui->tab_import->setColumnWidth( 10, 70);     // ot
    ui->tab_import->setColumnWidth( 11, 70);     // date_birth
    ui->tab_import->setColumnWidth( 12, 30);     // sex

    /* // правим заголовки
    model_import_tab.setHeaderData(  0, Qt::Horizontal, ("ID"));
    model_import_tab.setHeaderData(  1, Qt::Horizontal, ("имя среза"));
    model_import_tab.setHeaderData(  2, Qt::Horizontal, ("дата среза"));
    model_import_tab.setHeaderData(  3, Qt::Horizontal, ("тип \nсреза"));
    model_import_tab.setHeaderData(  4, Qt::Horizontal, ("число \nстрок"));*/

    ui->tab_import->repaint();

    ui->tab_import->setEnabled(true);
    QApplication::processEvents();
    this->setCursor(Qt::ArrowCursor);
}


void assiglay_tfoms_master_wnd::on_ch_filter_person_clicked(bool checked) {
    ui->ch_filter_person_yes->setEnabled(checked);
    ui->ch_filter_person_no->setEnabled(checked);
    refresh_import_tab();
}
void assiglay_tfoms_master_wnd::on_ch_filter_polis_clicked(bool checked) {
    ui->ch_filter_polis_yes->setEnabled(checked);
    ui->ch_filter_polis_no->setEnabled(checked);
    refresh_import_tab();
}
void assiglay_tfoms_master_wnd::on_ch_filter_person_yes_clicked(bool checked) {
    ui->ch_filter_person_no->setChecked(!checked);
    refresh_import_tab();
}
void assiglay_tfoms_master_wnd::on_ch_filter_person_no_clicked(bool checked) {
    ui->ch_filter_person_yes->setChecked(!checked);
    refresh_import_tab();
}
void assiglay_tfoms_master_wnd::on_ch_filter_polis_yes_clicked(bool checked) {
    ui->ch_filter_polis_no->setChecked(!checked);
    refresh_import_tab();
}
void assiglay_tfoms_master_wnd::on_ch_filter_polis_no_clicked(bool checked) {
    ui->ch_filter_polis_yes->setChecked(!checked);
    refresh_import_tab();
}

void assiglay_tfoms_master_wnd::on_bn_refresh_clicked() {
    refresh_import_tab();
    refresh_data_status();
}


void assiglay_tfoms_master_wnd::on_ch_limit_clicked(bool checked) {
    ui->spin_limit->setEnabled(checked);
}

void assiglay_tfoms_master_wnd::on_bn_load_clicked() {
    if (QMessageBox::question(this, "Нужно подтверждение", "Вы действительно хотите загрузить новые данные в таблицу импорта ASSIG.IMPORT_DATA ?\n"
                              "При этом из таблицы ASSIG.IMPORT_DATA будут безвозвратно удалены старые данные.",
                              QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
        // отмена операции
        QMessageBox::information(this, "Отмена загрузки данных",
                                 "Загрузка данных отменена пользователем.\n"
                                 "Ничего не изменилоось.");
        return;
    }

    //------------------------------//
    // Запросим фондовский файл CSV //
    //------------------------------//
    QString file_name = QFileDialog::getOpenFileName(this, "Выберите файл из ТФОМС", "d:/temp", tr("CSV-files (*.csv *.txt)"));
    if (file_name.isEmpty()) {
        QMessageBox::information(this, "Файл не выбран",
                                 "Операция отменена пользователем.");
        QApplication::processEvents();
        return;
    }

    QString log = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ">> "
                  " Импорт CSV-файла данных от ТФОМС.  ";
    ui->te_log->append(log);

    QFileInfo fi(file_name);
    QString base_name = fi.baseName();
    if ( base_name.left(7)!="inko_01" ) {
        if (QMessageBox::warning(this, "Неправильное имя файла",
                                 "Имя файла " + base_name + ".CSV не соответствует маске: \n\n"
                                 "inko_01MMYYYY.csv\n\n"
                                 "Всё равно заргузить?",
                                 QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
            this->setCursor(Qt::ArrowCursor);
            QApplication::processEvents();

            return;
        }
    }

    // пробуем открыть файл
    QFile file(file_name);
    if (!file.open(QFile::ReadOnly)) {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка открытия файла",
                             "При попытке открыть выбранный файл произошла ошибка. "
                             "\n\nОперация отменена.");
        QApplication::processEvents();

        return;
    }

    db.transaction();

    // почистим таблицу импорта
    QString sql_clear;
    sql_clear = "TRUNCATE assig.import_data "
                " RESTART IDENTITY ; ";
    QSqlQuery *query_clear = new QSqlQuery(db);
    bool res_clear = query_clear->exec(sql_clear);

    if (!res_clear) {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при очистке таблицы импорта",
                             "При попытке очистить таблицу импорта произошла неожиданная ошибка.\n\n"
                             + query_clear->lastError().driverText() + "\n" + query_clear->lastError().databaseText() + "\n\n"
                             "Загрузка отменена.");
        delete query_clear;
        db.rollback();
        QApplication::processEvents();

        return;
    }
    delete query_clear;
    refresh_import_tab();
    ui->lab_cnt_save->setText("-//-");
    ui->lab_time_all->setText("-//-");
    ui->lab_time->setText("-//-");
    ui->cnt_all->setText("-//-");
    ui->cnt_pers_yes->setText("-//-");
    ui->cnt_pers_no->setText("-//-");
    ui->cnt_pol_yes->setText("-//-");
    ui->cnt_pol_no->setText("-//-");
    ui->tab_import->setEnabled(false);

    ui->pan_wait->setVisible(true);
    ui->lab_process->setText(" загрузка CSV ");
    QApplication::processEvents();

    date0 = QDate::currentDate();
    time0 = QTime::currentTime();


    //------------------------------//
    // Загрузим фондовский файл CSV //
    //------------------------------//
    QStringList data_list;

    // сформируем поток
    QTextStream stream(&file);
    stream.setCodec (QTextCodec::codecForName("UTF-8"));

    // прочитаем первую строку - она нам не нужна
    QString values_str = stream.readLine().replace("\'\'","\"").replace("\'","\"");

    // перебор строк файла
    QString div_fields = ";";
    QString rnd_fields = "\"";
    int n = -1;

    data_list.clear();
    int load_counter = 0;

    QString IC_ID, SP_ID;
    int POL_TYPE;
    QString POL_SER, POL_NUM;
    QDate POL_BEGIN, POL_END;
    QString last_name, first_name, MIDDLE_NAME;
    QDate BIRTH_DATE;
    int SEX;
    QString BIRTH_PLACE;
    QString SNILS;
    QDate DEATH_DATE;
    QString ZIP_CODE, REGION, DISTRICT, CITY, SETTLE, STREET, HOUSE, BUILDING, FLAT;
    int DOC_TYPE;
    QString DOC_SER, DOC_NUM;
    QDate DOC_BEGIN, DOC_END;
    QString CITIZENSHIP;
    int A1_MED_TYPE;
    QString A1_MED_ORG_ID;
    int A1_ASSIGN_TYPE;
    QDate A1_ASSIGN_DATE;
    QString A1_SNILS_MT;
    QString A1_CODE_MS;

    while (true) {
        data_list.clear();
        values_str = stream.readLine().replace("\'\'","\"").replace("\'","\"");
        if (values_str.isEmpty())
            break;

        //-----------------------//
        // разберём список полей //
        //-----------------------//
        QString value;
        int len = values_str.length(), pos = 0;
        int n_value = 0;

        while (pos<len && pos!=-1) {
            int pos1 = values_str.indexOf(div_fields, pos);
            if (pos1<0)  pos1 = len;
            value = values_str.mid(pos, pos1-pos).simplified().trimmed();
            if (value.left(1)==rnd_fields && value.right(1)==rnd_fields)
                value = value.mid(1, value.length()-2);
            if (value.left(1)=="\"")
                value = value.mid(1, value.length()-1);
            if (value.right(1)=="\"")
                value = value.mid(0, value.length()-1);
            if (value=="NULL")
                value = "";
            if (value.right(13)==" 00:00:00.000")
                value = value.left(value.length()-13).trimmed();

            value = value.replace("\"", "").replace("'", "");

            // добавим значение
            data_list.append(value);

            if (pos1<0 || pos1==len) break;
            else pos = pos1 + div_fields.length();

            n_value++;
        }

        if (data_list.size()<34)
            continue;

        IC_ID          = QString("00000" + data_list.at( 0).trimmed().replace("-","").replace(" ","").replace("/","")).right(5);
        SP_ID          = QString("0000000000000000" + data_list.at( 1).trimmed().replace("-","").replace(" ","").replace("/","")).right(16);
        POL_TYPE       = data_list.at( 2).trimmed().toInt();
        POL_SER        = data_list.at( 3).trimmed();
        POL_NUM        = data_list.at( 4).trimmed();
        POL_BEGIN      = QDate::fromString(data_list.at( 5).trimmed().replace('.',"-"), "yyyy-MM-dd");
        POL_END        = QDate::fromString(data_list.at( 6).trimmed().replace('.',"-"), "yyyy-MM-dd");
        last_name      = data_list.at( 7).trimmed().toUpper();
        first_name     = data_list.at( 8).trimmed().toUpper();
        MIDDLE_NAME    = data_list.at( 9).trimmed().toUpper();
        BIRTH_DATE     = QDate::fromString(data_list.at(10).trimmed().replace('.',"-"), "yyyy-MM-dd");
        SEX            = data_list.at(11).trimmed().toInt();
        BIRTH_PLACE    = data_list.at(12).trimmed();
        SNILS          = QString("00000000000" + data_list.at(13).trimmed().replace("-","").replace(" ","").replace("/","")).right(11);
        DEATH_DATE     = QDate::fromString(data_list.at(14).trimmed().replace('.',"-"), "yyyy-MM-dd");
        ZIP_CODE       = data_list.at(15).trimmed();
        REGION         = data_list.at(16).trimmed().toUpper();
        DISTRICT       = data_list.at(17).trimmed().toUpper();
        CITY           = data_list.at(18).trimmed().toUpper();
        SETTLE         = data_list.at(19).trimmed().toUpper();
        STREET         = data_list.at(20).trimmed().toUpper();
        HOUSE          = data_list.at(21).trimmed().toUpper();
        BUILDING       = data_list.at(22).trimmed().toUpper();
        FLAT           = data_list.at(23).trimmed();
        DOC_TYPE       = data_list.at(24).trimmed().toInt();
        DOC_SER        = data_list.at(25).trimmed().toUpper();
        DOC_NUM        = data_list.at(26).trimmed().toUpper();
        DOC_BEGIN      = QDate::fromString(data_list.at(27).trimmed().replace('.',"-"), "yyyy-MM-dd");
        DOC_END        = QDate::fromString(data_list.at(28).trimmed().replace('.',"-"), "yyyy-MM-dd");
        CITIZENSHIP    = data_list.at(29).trimmed().toUpper();
        A1_MED_TYPE    = data_list.at(30).trimmed().toInt();
        A1_MED_ORG_ID  = data_list.at(31).trimmed();
        A1_ASSIGN_TYPE = data_list.at(32).trimmed().toInt();
        A1_ASSIGN_DATE = QDate::fromString(data_list.at(33).trimmed().replace('.',"-"), "yyyy-MM-dd");
        A1_SNILS_MT    = QString("00000000000" + data_list.at(34).trimmed().replace("-","").replace(" ","").replace("/","")).right(11);
        A1_CODE_MS     = data_list.at(35).trimmed();

        if ( POL_SER.isEmpty()
             && !POL_NUM.isEmpty() ) {
            POL_NUM = QString("00000000000" + POL_NUM).right(11);
            POL_SER = POL_NUM.left(4);
            POL_NUM = POL_NUM.right(7);
        }

        int id_import = -1;

        //-----------------------------------------//
        // добавим запись данных в таблицу импорта //
        //-----------------------------------------//
        QString sql_import;
        sql_import = "insert into assig.import_data "
                     " ( ic_id, sp_id, "
                     "   pol_type, pol_ser, pol_num, pol_begin, pol_end, "
                     "   last_name, first_name, middle_name, birth_date, sex, birth_place, snils, death_date, "
                     "   zip_code, region, district, city, settle, street, house, building, flat, "
                     "   doc_type, doc_ser, doc_num, doc_begin, doc_end, citizenship, "
                     "   a1_med_type, a1_med_org_id, a1_assign_type, a1_assign_date, a1_snils_mt, a1_code_ms ) "
                     " values "
                     " ('" + IC_ID       + "', "
                     "  '" + SP_ID       + "', "

                     "   " + QString::number(POL_TYPE) + ", "
                     "  '" + POL_SER     + "', "
                     "  '" + POL_NUM     + "', "
                     "   " + (POL_BEGIN.isValid()  ? ("'" + POL_BEGIN.toString("yyyy-MM-dd")  + "'") : "NULL") + ", "
                     "   " + (POL_END.isValid()    ? ("'" + POL_END.toString("yyyy-MM-dd")    + "'") : "NULL") + ", "

                     "  '" + last_name   + "', "
                     "  '" + first_name  + "', "
                     "  '" + MIDDLE_NAME + "', "
                     "   " + (BIRTH_DATE.isValid() ? ("'" + BIRTH_DATE.toString("yyyy-MM-dd") + "'") : "NULL") + ", "
                     "   " + QString::number(SEX)  + ", "
                     "  '" + BIRTH_PLACE + "', "
                     "  '" + SNILS       + "', "
                     "   " + (DEATH_DATE.isValid() ? ("'" + DEATH_DATE.toString("yyyy-MM-dd") + "'") : "NULL") + ", "

                     "  '" + ZIP_CODE    + "', "
                     "  '" + REGION      + "', "
                     "  '" + DISTRICT    + "', "
                     "  '" + CITY        + "', "
                     "  '" + SETTLE      + "', "
                     "  '" + STREET      + "', "
                     "  '" + HOUSE       + "', "
                     "  '" + BUILDING    + "', "
                     "  '" + FLAT        + "', "

                     "   " + QString::number(DOC_TYPE) + ", "
                     "  '" + DOC_SER     + "', "
                     "  '" + DOC_NUM     + "', "
                     "   " + (DOC_BEGIN.isValid() ? ("'" + DOC_BEGIN.toString("yyyy-MM-dd") + "'") : "NULL") + ", "
                     "   " + (DOC_END.isValid()   ? ("'" + DOC_END.toString("yyyy-MM-dd")   + "'") : "NULL") + ", "
                     "  '" + CITIZENSHIP + "', "

                     "   " + QString::number(A1_MED_TYPE)    + ", "
                     "  '" + A1_MED_ORG_ID                   + "', "
                     "   " + QString::number(A1_ASSIGN_TYPE) + ", "
                     "   " + (A1_ASSIGN_DATE.isValid() ? ("'" + A1_ASSIGN_DATE.toString("yyyy-MM-dd") + "'") : "NULL") + ", "
                     "  '" + A1_SNILS_MT + "', "
                     "  '" + A1_CODE_MS  + "' ) ";
                     " returning id ; ";
        QSqlQuery *query_import = new QSqlQuery(db);
        bool res_import = query_import->exec(sql_import);

        if (!res_import) {
            ui->tab_import->setEnabled(true);
            this->setCursor(Qt::ArrowCursor);
            QMessageBox::warning(this, "Ошибка добавления записи данных в таблицу импорта",
                                 "При попытке добавить запись данных в таблицу импорта произошла неожиданная ошибка.\n\n"
                                 "n = " + QString::number(n) + "\n\n"
                                 + query_import->lastError().driverText() + "\n" + query_import->lastError().databaseText() + "\n\n"
                                 "Загрузка отменена.");
            delete query_import;
            db.rollback();
            ui->pan_wait->setVisible(false);
            ui->lab_process->setText(" -//- ");
            QApplication::processEvents();

            return;
        }
        query_import->next();
        id_import = query_import->value(0).toInt();
        delete query_import;

        n++;

        if ((n%2341)==0) {
            //------------------------------//
            // отображение статуса на экран //
            //------------------------------//
            date = QDate::currentDate();
            time = QTime::currentTime();

            int sec = time0.secsTo(time) + date0.daysTo(date)*24*60*60;
            int sut = sec/86400;
            sec = sec - sut*86400;
            int hou = sec/3600;
            sec = sec - hou*3600;
            int min = sec/60;
            sec = sec - min*60;
            ui->lab_time_all->setText( (sut>0 ? (QString::number(sut)+"д ") : " ") +
                                       QString("00" + QString::number(hou)).right(2) + ":" +
                                       QString("00" + QString::number(min)).right(2) + ":" +
                                       QString("00" + QString::number(sec)).right(2) );
            sec = time1.secsTo(time) + date1.daysTo(date)*24*60*60;
            sut = sec/86400;
            sec = sec - sut*86400;
            hou = sec/3600;
            sec = sec - hou*3600;
            min = sec/60;
            sec = sec - min*60;
            ui->lab_time->setText( (sut>0 ? (QString::number(sut)+"д ") : " ") +
                                   QString("00" + QString::number(hou)).right(2) + ":" +
                                   QString("00" + QString::number(min)).right(2) + ":" +
                                   QString("00" + QString::number(sec)).right(2) );

            date1 = QDate::currentDate();
            time1 = QTime::currentTime();

            ui->lab_cnt_load->setText(base_name + "\n" + show_num(n));
            QApplication::processEvents();
        }
    }
    file.close();
    db.commit();
    ui->lab_cnt_load->setText(base_name + "\n" + show_num(n));
    ui->lab_process->setText(" -//- ");
    ui->pan_wait->setVisible(false);
    ui->tab_import->setEnabled(true);
    this->setCursor(Qt::ArrowCursor);
    QApplication::processEvents();

    QMessageBox::warning(this, "Готово", "Готово.");

    refresh_data_status();
    refresh_import_tab();
}


void assiglay_tfoms_master_wnd::on_bn_SQL_exec_clicked() {
    QString sql_name = ui->combo_SQL->currentText();
    QString sql_text = ui->te_SQL->toPlainText();
    sql_text = sql_text.replace("\n", "");
    QString log = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ">> "
                  " SQL-операция \"" + sql_name + "\".  ";
    ui->te_log->append(log);

    ui->pan_wait->setVisible(true);
    ui->lab_process->setText("Выполнение\nSQL-операции");
    ui->tab_import->setEnabled(false);
    QApplication::processEvents();

    date0 = QDate::currentDate();
    time0 = QTime::currentTime();

    // выполнение произвольного оператора
    db.transaction();
    QSqlQuery *query_SQL = new QSqlQuery(db);
    bool res_SQL = query_SQL->exec(sql_text);

    if (!res_SQL) {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка выполнения SQL-операции",
                             "При попытке выполнить SQL-операцию произошла неожиданная ошибка.\n\n"
                             + query_SQL->lastError().driverText() + "\n" + query_SQL->lastError().databaseText() + "\n\n"
                             "Операция отменена.");
        delete query_SQL;
        db.rollback();
        ui->pan_wait->setVisible(false);
        ui->lab_process->setText(" -//- ");
        ui->tab_import->setEnabled(true);
        QApplication::processEvents();

        return;
    }
    delete query_SQL;
    db.commit();
    ui->pan_wait->setVisible(false);
    ui->lab_process->setText(" -//- ");
    QApplication::processEvents();

    {
        //------------------------------//
        // отображение статуса на экран //
        //------------------------------//
        date = QDate::currentDate();
        time = QTime::currentTime();

        int sec = time0.secsTo(time) + date0.daysTo(date)*24*60*60;
        int sut = sec/86400;
        sec = sec - sut*86400;
        int hou = sec/3600;
        sec = sec - hou*3600;
        int min = sec/60;
        sec = sec - min*60;
        ui->lab_time_all->setText( (sut>0 ? (QString::number(sut)+"д ") : " ") +
                                   QString("00" + QString::number(hou)).right(2) + ":" +
                                   QString("00" + QString::number(min)).right(2) + ":" +
                                   QString("00" + QString::number(sec)).right(2) );
        sec = time1.secsTo(time) + date1.daysTo(date)*24*60*60;
        sut = sec/86400;
        sec = sec - sut*86400;
        hou = sec/3600;
        sec = sec - hou*3600;
        min = sec/60;
        sec = sec - min*60;
        ui->lab_time->setText( (sut>0 ? (QString::number(sut)+"д ") : " ") +
                               QString("00" + QString::number(hou)).right(2) + ":" +
                               QString("00" + QString::number(min)).right(2) + ":" +
                               QString("00" + QString::number(sec)).right(2) );
    }
    ui->tab_import->setEnabled(true);
    this->setCursor(Qt::ArrowCursor);

    QMessageBox::warning(this, "Готово", "Готово.");

    refresh_data_status();
    refresh_import_tab();
}


void assiglay_tfoms_master_wnd::on_bn_save_clicked() {
    QString log = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ">> "
                  " Формирование снимка данных прикрепления ТФОМС. ";
    ui->te_log->append(log);

    // запросим параметры нового слоя
    s_assiglay_data data;
    data.layer_date = QDate(QDate::currentDate().year(),QDate::currentDate().month(),1);
    data.layer_name = "ASSIG_"
            + QString("0000" + QString::number(QDate::currentDate().year())).right(4)
            + QString("00" + QString::number(QDate::currentDate().month())).right(2)
            + "01_TFOMS";
    data.layer_type = 2;

    delete assiglay_data_w;
    assiglay_data_w = new assiglay_data_wnd(data, data_app, this);
    this->setCursor(Qt::WaitCursor);

    if (!assiglay_data_w->exec()) {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Отмена операции",
                             "Действие отменено пользователем.\n"
                             "Ничего не изменилось.");
        return;
    }

    // проверим, можно ли создать такой снимок данных
    QString sql_test = "select count(*) as cnt \n"
                       "  from assig.layers \n"
                       " where (layer_type=" + QString::number(data.layer_type) + " and layer_date='" + data.layer_date.toString("yyyy-MM-dd") + "') \n"
                       "    or layer_name='" + data.layer_name.toUpper() + "' ; ";
    QSqlQuery *query_test = new QSqlQuery(db);
    QString sql = sql_test.replace("\n","");
    bool res_test = query_test->exec(sql);
    if (!res_test) {
        this->setCursor(Qt::ArrowCursor);
        QString err_db = query_test->lastError().databaseText();
        QString err_drv = query_test->lastError().driverText();
        QMessageBox::warning(this, "Ошибка чтения данных",
                             "При попытке проверить существование готового слоя произошла ошибка.\n"
                             + sql_test + "\n\n"
                             + err_db   + "\n"
                             + err_drv  + "\n\n"
                             "Операция отменена.\n");
        delete query_test;
        QApplication::processEvents();
        return;
    }
    int cnt_layers = 0;
    while (query_test->next()) {
        cnt_layers = query_test->value(0).toInt();
    }
    if (cnt_layers>0) {
        QMessageBox::warning(this, "Невозможно создать такой снимок данных",
                             "Уже есть " + QString::number(cnt_layers) + " слоёв с таким же именем или датой.\n"
                             "Имя и дата слоя должны быть уникальными.\n"
                             "Операция отменена.\n");
        delete query_test;
        QApplication::processEvents();
        return;
    }
    delete query_test;

    ui->tab_import->setEnabled(false);
    QApplication::processEvents();

    // -------------------
    // добавим запись слоя
    db.transaction();

    QString sql_layer = "insert into assig.layers(layer_name, layer_date, layer_type) "
                        "values( '" + data.layer_name + "', "
                               " '" + data.layer_date.toString("yyyy-MM-dd") + "', "
                               "  " + QString::number(data.layer_type) + " ) "
                        " returning id ; ";
    QSqlQuery *query_layer = new QSqlQuery(db);
    bool res_layer = mySQL.exec(this,sql_layer,"Создание записи нового слоя",query_layer,false, db, data_app);

    if (!res_layer) {        
        ui->tab_import->setEnabled(true);
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка",
                             "При попытке создать запись нового слоя произошла неожиданная ошибка.\n\n"
                             "Операция отменена.");
        delete query_layer;
        db.rollback();
        QApplication::processEvents();
        return;
    }
    // получим ID слоя
    int id_layer = -1;
    while (query_layer->next())
        id_layer = query_layer->value(0).toInt();

    delete query_layer;


    date0 = QDate::currentDate();
    time0 = QTime::currentTime();


    int     id;
    int     id_person;
    QString fam;
    QString im;
    QString ot;
    int     sex;
    QDate   date_birth;
    QString snils;
    int     id_polis;
    int     pol_v;
    QString pol_ser;
    QString pol_num;
    int     vs_num;
    QString enp;
    QString code_mo;
    QString code_mp;
    QString code_ms;
    QString snils_mt;
    int     assig_type;
    QString assig_way;
    QDate   assig_date;
    QString subj_name;
    QString dstr_name;
    QString city_name;
    QString nasp_name;
    QString strt_name;
    int     house_num;
    QString corp;
    QDateTime dt_ins;
    QDateTime dt_upd;

    ui->pan_wait->setVisible(true);
    ui->lab_process->setText("Формирую снимок\nданных ТФОМС");
    QApplication::processEvents();

    {
        // сформируем снимок данных ТФОМС по содержимому таблицы импорта
        QString sql_import;
        sql_import = "insert into assig.sexages_data "
                     " ( id_layer, id_sexage_line, cell_name, "
                     "   id_person, fam, im, ot, sex, date_birth, snils, "
                     "   id_polis, enp, pol_v, pol_ser, pol_num, vs_num, "
                     "   code_head, code_mo, code_mp, code_ms, snils_mt, "
                     "   assig_type, assig_way, assig_date, "
                     "   subj_name, dstr_name, city_name, nasp_name, strt_name, house, corp, quart ) "

                     "select " + QString::number(id_layer) + ", NULL, NULL, "
                     "       i.id_person, i.last_name as fam, i.first_name as im, i.middle_name as ot, i.sex, i.birth_date, coalesce(i.snils, e.snils) as snils, "
                     "       i.id_polis, i.sp_id as enp, coalesce(i.pol_type, p.pol_v) as pol_v, p.pol_ser, p.pol_num, p.vs_num, "
                     "       i.code_head, i.a1_med_org_id as code_mo, i.code_mp, coalesce(i.a1_code_ms, i.code_ms), coalesce(i.a1_snils_mt, i.snils_mt), "
                     "       case when i.a1_assign_type in(1,2) then i.a1_assign_type else 1 end as assig_type, NULL as assig_way, i.a1_assign_date as assig_date, "
                     "       i.subj_name, i.dstr_name, i.city_name, i.nasp_name, i.strt_name, i.house_num, i.corp, i.quart "
                     "  from assig.import_data i "
                     "       join persons e on(e.id=i.id_person) "
                     "  left join polises p on(p.id=i.id_polis) ; ";
        QSqlQuery *query_import = new QSqlQuery(db);
        bool res_import = query_import->exec(sql_import);

        if (!res_import) {
            ui->tab_import->setEnabled(true);
            this->setCursor(Qt::ArrowCursor);
            QMessageBox::warning(this, "Ошибка при чтении таблицы импорта",
                                 "При попытке прочитать данные из таблицы импорта произошла неожиданная ошибка.\n\n"
                                 + query_import->lastError().driverText() + "\n" + query_import->lastError().databaseText() + "\n\n"
                                 "Операция отменена.");
            delete query_import;
            db.rollback();
            ui->pan_wait->setVisible(false);
            ui->lab_process->setText(" -//- ");
            QApplication::processEvents();

            return;
        }

        // обновим статистику снимка
        QString sql_stat = "update assig.layers l "
                           "   set cnt_recs=(select count(*) from assig.sexages_data d where d.id_layer=" + QString::number(id_layer) + "), "
                           "       cnt_uniq=(select count(distinct d.id_person) from assig.sexages_data d where d.id_layer=" + QString::number(id_layer) + ") "
                           " where l.id=" + QString::number(id_layer) + "; ";
        QSqlQuery *query_stat = new QSqlQuery(db);
        bool res_stat = query_stat->exec(sql_stat);

        if (!res_stat) {
            this->setCursor(Qt::ArrowCursor);
            QMessageBox::warning(this, "Ошибка при подсчёте статистики снимка",
                                 "При подсчёте статистики снимка произошла неожиданная ошибка.\n\n"
                                 + query_stat->lastError().driverText() + "\n" + query_stat->lastError().databaseText() + "\n\n"
                                 "Операция отменена.");
            delete query_stat;
            db.rollback();
            ui->pan_wait->setVisible(false);
            return;
        }
        delete query_stat;

        db.commit();
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Готово", "Готово.");
    }
    ui->pan_wait->setVisible(false);
    ui->lab_process->setText(" -//- ");
    log = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ">> "
                  " Готово. ";
    ui->te_log->append(log);
    ui->tab_import->setEnabled(true);
    QApplication::processEvents();
}


void assiglay_tfoms_master_wnd::on_bn_close_clicked() {
    if (QMessageBox::question(this, "Закрыть мастер?",
                              "Вы действительно хотите закрыть мастер формирования снимков данных прикрепления ТФОМС?",
                              QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::No)
        return;
    close();
}

void assiglay_tfoms_master_wnd::on_bn_toCSV_clicked() {
    bool res_CSV = table2csv(db, sql_import_tab, model_import_tab, ui->tab_import, true,
                             data_app.path_out + QDate::currentDate().toString("yyyy-MM-dd") + "  - Мастер импорта данных (id_person " + (ui->ch_filter_person->isChecked() ? (ui->ch_filter_person_yes->isChecked() ? "ЕСТЬ" : "НЕТ") : "ВСЕ") + ", id_polis " + (ui->ch_filter_polis->isChecked() ? (ui->ch_filter_polis_yes->isChecked() ? "ЕСТЬ" : "НЕТ") : "ВСЕ") + ")",
                             this);
}

void assiglay_tfoms_master_wnd::on_bn_clear_clicked() {

    if (QMessageBox::question(this, "Нужно подтверждение",
                              "Вы действительно хотите удалить данные из таблицы импорта  ASSIG.IMPORT_DATA ?\n"
                              "Это необратимая операция.",
                              QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
        // отмена операции
        QMessageBox::information(this, "Отмена очистки таблицы",
                                 "Очистка таблицы импорта отменена пользователем.\n"
                                 "Ничего не изменилоось.");
        return;
    }

    db.transaction();

    // почистим таблицу импорта
    QString sql_clear;
    sql_clear = "TRUNCATE assig.import_data "
                " RESTART IDENTITY ; ";
    QSqlQuery *query_clear = new QSqlQuery(db);
    bool res_clear = query_clear->exec(sql_clear);

    if (!res_clear) {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при очистке таблицы импорта",
                             "При попытке очистить таблицу импорта произошла неожиданная ошибка.\n\n"
                             + query_clear->lastError().driverText() + "\n" + query_clear->lastError().databaseText() + "\n\n"
                             "Загрузка отменена.");
        delete query_clear;
        db.rollback();
        QApplication::processEvents();

        return;
    }
    delete query_clear;
    db.commit();

    refresh_import_tab();
}
