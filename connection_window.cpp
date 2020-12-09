#include "connection_window.h"
#include "ui_connection_window.h"
#include <QMessageBox>

ConnectionWindow::ConnectionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConnectionWindow)
{
    ui->setupUi(this);
    ui->tc_pass->setEchoMode(QLineEdit::Password);
    ui->tc_op_pass->setEchoMode(QLineEdit::Password);    
    ui->lab_start->setVisible(false);

    // версия программы
    data_app.version = "0.2.3";

    // обнулим дочерние окна
    assignment_w = NULL;

    data_app.id_operator = -1;
    data_app.id_point    = -1;
    data_app.id_session  = -1;
    data_app.terminal_id = -1;

    // ini-файл
    data_app.ini_path = QApplication::applicationDirPath() + "/inko_assignment.ini";
    settings = new QSettings(data_app.ini_path, QSettings::IniFormat , NULL);
    // [polisesDB]
    data_app.drvr = settings->value("polisesDB/drvr", "QPSQL").toString();
    data_app.host = settings->value("polisesDB/host", "localhost").toString();
    data_app.port = settings->value("polisesDB/port", 5432).toInt();
    data_app.base = settings->value("polisesDB/base", "inko_polises").toString();
    data_app.user = settings->value("polisesDB/user", "postgres").toString();
    data_app.pass = settings->value("polisesDB/pass", "650325").toString();
    //QMessageBox::information(this, "info", "host=" + data_app.host + " port=" + QString::number(data_app.port) + " base=" + data_app.base + " user=" + data_app.user + " pass=" + data_app.pass + " " );
    // [flags]
    data_app.f_readonly = settings->value("flags/readonly", true).toBool();
    data_app.is_tech    = settings->value("flags/is_tech",  (true)).toBool();
    data_app.is_head    = settings->value("flags/is_head",  (true)).toBool();
    data_app.is_locl    = settings->value("flags/is_locl",  (true)).toBool();
    data_app.is_call    = settings->value("flags/is_call",  (true)).toBool();
    // [KLADR]
    data_app.ocato_full = settings->value("KLADR/ocato", "").toString();
    data_app.KLADR_subj = settings->value("KLADR/subj", "").toString().toInt();
    data_app.KLADR_dstr = settings->value("KLADR/dstr", "").toString().toInt();
    data_app.KLADR_city = settings->value("KLADR/city", "").toString().toInt();
    data_app.KLADR_nasp = settings->value("KLADR/nasp", "").toString().toInt();
    data_app.KLADR_strt = settings->value("KLADR/strt", "").toString().toInt();
    data_app.KLADR_house= settings->value("KLADR/house", 1).toInt();
    data_app.KLADR_corp = settings->value("KLADR/corp", "").toString();
    // [paths]
    data_app.path_install = QString::fromUtf8(settings->value("paths/path_install", "D:/INKO/").toByteArray());
    data_app.path_arch = QString::fromUtf8(settings->value("paths/path_arch", data_app.path_install + "7-Zip/").toByteArray());
    data_app.path_reports = QString::fromUtf8(settings->value("paths/path_reports", data_app.path_install + "reports/").toByteArray());
    data_app.path_temp = QString::fromUtf8(settings->value("paths/path_temp", "D:/TEMP/").toByteArray());
    data_app.path_foto = QString::fromUtf8(settings->value("paths/path_foto", "D:/TEMP/").toByteArray());
    data_app.path_sign = QString::fromUtf8(settings->value("paths/path_sign", "D:/TEMP/").toByteArray());
    data_app.path_out = QString::fromUtf8(settings->value("paths/path_out", "D:/INKO/_OUT_/").toByteArray());
    data_app.path_in = QString::fromUtf8(settings->value("paths/path_in", "D:/INKO/_IN_/").toByteArray());
    data_app.path_out_backup = QString::fromUtf8(settings->value("paths/path_out_backup", "D:/INKO/_OUT_/backup/").toByteArray());
    data_app.path_in_backup = QString::fromUtf8(settings->value("paths/path_in_backup", "D:/INKO/_IN_/backup/").toByteArray());
    data_app.path_dbf = QString::fromUtf8(settings->value("paths/path_dbf", "D:/INKO/_DBF_/").toByteArray());

    // [smo]
    data_app.ocato            = settings->value("smo/terr_ocato", "14000").toString();
    data_app.ocato_text       = QString::fromUtf8(settings->value("smo/terr_ocato_text", "Белгородская область").toByteArray());
    data_app.smo_ogrn         = settings->value("smo/smo_ogrn", "1043600049041").toString();
    data_app.smo_regnum       = settings->value("smo/smo_regnum", "46006").toString();
    data_app.smo_logo1        = QString::fromUtf8(settings->value("smo/smo_logo1",        "308000, г.Белгород, ул. Н.Островского, д.22").toByteArray());
    data_app.smo_logo2        = QString::fromUtf8(settings->value("smo/smo_logo2",        "Телефон   (4722) 20-28-76,   8-800-100-36-03").toByteArray());
    data_app.smo_logo3        = QString::fromUtf8(settings->value("smo/smo_logo3",        "inkobelgorod@inko-med.ru, www.inko-med.ru").toByteArray());
    data_app.smo_short        = QString::fromUtf8(settings->value("smo/smo_short",        "ИНКО-МЕД").toByteArray());
    data_app.smo_name         = QString::fromUtf8(settings->value("smo/smo_name",         "OOO \"Медицинская страховая компания \"ИНКО-МЕД\"").toByteArray());
    data_app.smo_name_full    = QString::fromUtf8(settings->value("smo/smo_name_full",    "Общество с ограниченной ответственностью \"Медицинская страховая компания \"ИНКО-МЕД\"").toByteArray());
    data_app.smo_name_full_rp = QString::fromUtf8(settings->value("smo/smo_name_full_rp", "Медицинскую страховую компанию \"ИНКО-МЕД\"").toByteArray());
    data_app.smo_director     = QString::fromUtf8(settings->value("smo/smo_director",     "Серёгина А.А.").toByteArray());
    data_app.smo_oms_chif     = QString::fromUtf8(settings->value("smo/smo_oms_chif",     "Проценко Елена Александровна").toByteArray());
    data_app.smo_address      = QString::fromUtf8(settings->value("smo/smo_address",      "394018, г. Воронеж, ул. Платонова, д. 14, тел. (473)2-550-170, 2-298-025").toByteArray());
    data_app.smo_contact_info = QString::fromUtf8(settings->value("smo/smo_contact_info", "Телефон контакт-центра: 8 800 100 36 03").toByteArray());
    data_app.smo_contact_info = "Телефон контакт-центра: 8-800-100-36-03";
    data_app.smo_master_state = QString::fromUtf8(settings->value("smo/smo_master_name",  "Заместитель генерального директора по экономике и финансам").toByteArray());
    data_app.smo_master_FamIO = QString::fromUtf8(settings->value("smo/smo_master_fio",   "С.П. Куршина").toByteArray());

    data_app.letter_doer1_FamIO   = QString::fromUtf8(settings->value("smo/letter_doer1_FamIO",   "Чалая К.В.").toByteArray());
    data_app.letter_doer1_phone   = QString::fromUtf8(settings->value("smo/letter_doer1_phone",   "255-01-70, доб. 3305").toByteArray());
    data_app.letter_doer2_FamIO   = QString::fromUtf8(settings->value("smo/letter_doer2_FamIO",   "Кизянов А.Ф.").toByteArray());
    data_app.letter_doer2_phone   = QString::fromUtf8(settings->value("smo/letter_doer2_phone",   "255-01-70, доб. 3338").toByteArray());

    /*// [smo]
    data_app.ocato = settings->value("smo/terr_ocato", "14000").toString();
    data_app.ocato_text = QString::fromUtf8(settings->value("smo/terr_ocato_text", "Белгородская область").toByteArray());
    data_app.smo_ogrn = settings->value("smo/smo_ogrn", "1043600049041").toString();
    data_app.smo_regnum = settings->value("smo/smo_regnum", "46006").toString();
    data_app.smo_logo1 = QString::fromUtf8(settings->value("smo/smo_logo1", "308000, г.Белгород, ул. Н.Островского, д.22").toByteArray());
    data_app.smo_logo2 = QString::fromUtf8(settings->value("smo/smo_logo2", "Телефон   (4722) 20-28-76,   8-800-100-36-03").toByteArray());
    data_app.smo_logo3 = QString::fromUtf8(settings->value("smo/smo_logo3", "inkobelgorod@inko-med.ru, www.inko-med.ru").toByteArray());
    data_app.smo_name = QString::fromUtf8(settings->value("smo/smo_name", "OOO \"Медицинская страховая компания \"ИНКО-МЕД\"").toByteArray());
    data_app.smo_name_full = QString::fromUtf8(settings->value("smo/smo_name_full", "Общество с ограниченной ответственностью \"Медицинская страховая компания \"ИНКО-МЕД\"").toByteArray());
    data_app.smo_name_full_rp = QString::fromUtf8(settings->value("smo/smo_name_full_rp", "Медицинскую страховую компанию \"ИНКО-МЕД\"").toByteArray());
    data_app.smo_director = QString::fromUtf8(settings->value("smo/smo_director", "Серёгина А.А.").toByteArray());
    data_app.smo_oms_chif = QString::fromUtf8(settings->value("smo/smo_oms_chif", " ").toByteArray());
    data_app.smo_address = QString::fromUtf8(settings->value("smo/smo_address", "394018, г. Воронеж, ул. Платонова, д. 14, тел. (473)2-550-170, 2-298-025").toByteArray());
    data_app.smo_contact_info = QString::fromUtf8(settings->value("smo/smo_contact_info", "Телефон горячей линии: 8 800 100 36 03").toByteArray());
    data_app.point_regnum = settings->value("smo/point_regnum", QString(data_app.ocato=="38000" ? "001" : "101")).toString();
    data_app.point_name = QString::fromUtf8(settings->value("smo/point_name", " - тестовый пункт выдачи - ").toByteArray());
    data_app.operator_fio = QString::fromUtf8(settings->value("smo/operator_fio", " - ИНКО-тест - ").toByteArray());
    // [filial]
    data_app.terr_name          = QString::fromUtf8(settings->value("filial/terr_name",          (data_app.ocato=="38000" ? "Белгородская область" : "Курская область")).toByteArray());
    data_app.terr_name_rp       = QString::fromUtf8(settings->value("filial/terr_name_rp",       (data_app.ocato=="38000" ? "Белгородской области" : "Курской области")).toByteArray());
    data_app.tfoms_director     = QString::fromUtf8(settings->value("filial/tfoms_director",     (data_app.ocato=="38000" ? "" : "Директор  ТФОМС Курской области")).toByteArray());
    data_app.tfoms_director_2   = QString::fromUtf8(settings->value("filial/tfoms_director_2",   (data_app.ocato=="38000" ? "" : "")).toByteArray());
    data_app.tfoms_director_fio = QString::fromUtf8(settings->value("filial/tfoms_director_fio", (data_app.ocato=="38000" ? "" : "Курцев А. В.")).toByteArray());
    data_app.filial_director    = QString::fromUtf8(settings->value("filial/filial_director",    (data_app.ocato=="38000" ? "" : "Заместитель генерального директора")).toByteArray());
    data_app.filial_director_2  = QString::fromUtf8(settings->value("filial/filial_director_2",  (data_app.ocato=="38000" ? "" : "директор Курского филиала ООО \"МСК \"ИНКО-МЕД\"")).toByteArray());
    data_app.filial_director_fio= QString::fromUtf8(settings->value("filial/filial_director_fio",(data_app.ocato=="38000" ? "" : "Чаленко А. И.")).toByteArray());
    // [assig]
    data_app.csv_headers = settings->value("assig/csv_headers", (data_app.ocato=="38000" ? false : true)).toBool();
    // [xml]
    data_app.xml_vers = settings->value("xml/xml_vers", (data_app.ocato=="38000" ? "2.1a" : "2.1")).toString();
    data_app.snils_format = settings->value("xml/snils_format", (data_app.ocato=="38000" ? 1 : 0)).toInt();
    data_app.split_by_ot = settings->value("xml/split_by_ot", (data_app.ocato=="38000" ? 1 : 0)).toInt();
    data_app.send_p021_in_p060 = settings->value("xml/send_p021_in_p060", (data_app.ocato=="38000" ? 1 : 0)).toInt();
    data_app.pol_start_date_by_vs = settings->value("xml/pol_start_date_by_vs", (data_app.ocato=="38000" ? 1 : 0)).toInt();
    data_app.f_zip_packs = settings->value("xml/zip_packs", (data_app.ocato=="38000" ? false : true)).toBool();
    // [VS]
    data_app.vs_blank_width_mm = settings->value("VS/blank_width_mm", 205).toInt();
    data_app.vs_blank_height_mm = settings->value("VS/blank_height_mm", 295).toInt();
    data_app.vs_field_left_mm = settings->value("VS/field_left_mm", 18).toInt();
    data_app.vs_field_right_mm = settings->value("VS/field_right_mm", 5).toInt();
    data_app.vs_field_top_mm = settings->value("VS/field_top_mm", 4).toInt();
    data_app.vs_field_bottom_mm = settings->value("VS/field_bottom_mm", 5).toInt();*/
    // [filials]
    ui->combo_filial->clear();
    data_app.filials_list = "kursk;belg;voro;test";//settings->value("filials/filials_list", "kursk;belg;test").toString();
    if (data_app.filials_list.indexOf("kursk")>=0) {
        data_app.kursk_drvr = settings->value("kursk/drvr", "QPSQL").toString();
        data_app.kursk_host = settings->value("kursk/host", "172.20.250.15").toString();
        data_app.kursk_port = settings->value("kursk/port", 5432).toInt();
        data_app.kursk_base = settings->value("kursk/base", "inko_polises_kursk").toString();
        data_app.kursk_user = settings->value("kursk/user", "belg").toString();
        data_app.kursk_pass = settings->value("kursk/pass", "belg").toString();
        ui->combo_filial->addItem("филиал, г. Курск", 8);
    }
    if (data_app.filials_list.indexOf("belg")>=0) {
        data_app.belg_drvr = settings->value("belg/drvr", "QPSQL").toString();
        data_app.belg_host = settings->value("belg/host", "172.20.250.15").toString();
        data_app.belg_port = settings->value("belg/port", 5432).toInt();
        data_app.belg_base = settings->value("belg/base", "inko_polises_belgorod").toString();
        data_app.belg_user = settings->value("belg/user", "belg").toString();
        data_app.belg_pass = settings->value("belg/pass", "belg").toString();
        ui->combo_filial->addItem("филиал, г. Белгород", 64);
    }
    if (data_app.filials_list.indexOf("voro")>=0) {
        data_app.voro_drvr = settings->value("voro/drvr", "QIBASE").toString();
        data_app.voro_host = settings->value("voro/host", "127.0.0.1").toString();
        data_app.voro_port = settings->value("voro/port", 1).toInt();
        data_app.voro_base = settings->value("voro/base", "inko_polises_voronej").toString();
        data_app.voro_user = settings->value("voro/user", "SYSDBA").toString();
        data_app.voro_pass = settings->value("voro/pass", "masterkey").toString();
        ui->combo_filial->addItem("прикрепление, г. Воронеж", 1);
    }
    if (data_app.filials_list.indexOf("work")>=0) {
        data_app.work_drvr = settings->value("work/drvr", "QPSQL").toString();
        data_app.work_host = settings->value("work/host", "127.0.0.1").toString();
        data_app.work_port = settings->value("work/port", 5432).toInt();
        data_app.work_base = settings->value("work/base", "inko_polises").toString();
        data_app.work_user = settings->value("work/user", "postgres").toString();
        data_app.work_pass = settings->value("work/pass", "650325").toString();
        ui->combo_filial->addItem(" - рабочая база - ", -1);
    }

    ui->frame_bn_messages->setVisible(!(data_app.f_readonly));
    ui->text_comments->setReadOnly(data_app.f_readonly);

    this->move(100,100);
    refresh_connection();
    ui->tc_op_pass->setFocus();
}

ConnectionWindow::~ConnectionWindow() {
    delete ui;
}


void ConnectionWindow::refresh_connection() {
    ui->lab_DB_no->hide();
    ui->lab_DB_ok->hide();

    settings->sync();

    if (settings->status()==0) {
        // загрузим из инишника параметры подключения и проч.
        ui->combo_driver->setCurrentIndex(ui->combo_driver->findText(data_app.drvr));
        ui->tc_host->setText(data_app.host);
        ui->spin_port->setValue(data_app.port);
        ui->tc_base->setText(data_app.base);
        ui->tc_user->setText(data_app.user);
        ui->tc_pass->setText(data_app.pass);

        ui->ch_change_pass->setVisible(!data_app.f_readonly);
    }

    // создадим подключение к базе обновлений
    db = QSqlDatabase::addDatabase(ui->combo_driver->currentText(), "My Database");

    db.setConnectOptions();
    // сервер, на котором расположена БД - у нас localhost
    db.setHostName(ui->tc_host->text());
    // порт
    db.setDatabaseName(ui->tc_base->text());
    // имя базы данных
    if (data_app.drvr!="QIBASE")
        db.setPort(ui->spin_port->value());
    // имя пользователя - postgres
    db.setUserName(ui->tc_user->text());
    // пароль для подключения к БД
    db.setPassword(ui->tc_pass->text());

    while (!db.open()) {
        // подключение не удалось
        ui->lab_DB_no->show();
        ui->lab_DB_ok->hide();
        ui->pan_DB_params->show();
        ui->pan_login->hide();
        ui->pan_messages->hide();
        //ui->plain_log->hide();
        QString pass_ = data_app.pass;
        if (QMessageBox::question(this,
                                  "Подключение не удалось",
                                  "Подключение не удалось\n\nDB: " + db.lastError().databaseText() +
                                  "\n\ndriver: "+db.lastError().driverText()+
                                  "\n\nПараметры: drv=" + data_app.drvr + ", h=" + data_app.host + ":" + QString::number(data_app.port) + ", u=" + data_app.user + ", p=" + pass_.replace(QRegExp("[A-Z,a-z,0-9]"),"*") +
                                  "\n\nПовторить попытку?",
                                  QMessageBox::Yes|QMessageBox::No)==QMessageBox::No) {
            break;
        }

    }
    if (db.isOpen()) {
        // подключение готово
        ui->lab_DB_no->hide();
        ui->lab_DB_ok->show();
        ui->pan_DB_params->hide();
        ui->pan_login->show();
        ui->pan_messages->show();
        if (this->windowState()==Qt::WindowFullScreen) {
            //this->resize(1000,800);
        } else {
            this->resize(1000,800);
        }
        //ui->plain_log->hide();



        // запись терминала
        QString sql_terminal = "select id "
                               " from db_terminals "
                               " where name='INKO_ASSIGNMENT  --  " + data_app.point_name + "  --  " + data_app.operator_fio + "' ; ";
        QSqlQuery *query_terminal = new QSqlQuery(db);
        /*bool res_terminal =*/ mySQL.exec(this, sql_terminal, "Получим ID терминала", *query_terminal, true, db, data_app);
        if (!query_terminal->next()) {
            sql_terminal = "insert into db_terminals(name) "
                           " values('INKO_ASSIGNMENT  --  " + data_app.point_name + "  --  " + data_app.operator_fio + "') "
                           " returning id ; ";
            /*res_terminal =*/ mySQL.exec(this, sql_terminal, "Добавим терминал", *query_terminal, true, db, data_app);
        };
        data_app.terminal_id = query_terminal->value(0).toInt();



        // закрытие устаревших сессий
        QString sql_close_old = "";
        if (data_app.drvr=="QPSQL")
            sql_close_old = "update db_sessions "
                            "   set status=-2, "
                            "       dt_end=CURRENT_TIMESTAMP "
                            " where (dt_last + '1 hour'::INTERVAL)<CURRENT_TIMESTAMP ; ";
        else
            sql_close_old = "update db_sessions "
                            "   set status=-2, "
                            "       dt_end=CURRENT_TIMESTAMP "
                            " where (CURRENT_TIMESTAMP - dt_last)>15*60 ; ";

        QSqlQuery *query_close_old = new QSqlQuery(db);
        bool res_close_old = mySQL.exec(this, sql_close_old, "Закроем сессию", *query_close_old, true, db, data_app);
        delete query_close_old;

        sql_close_old = "update db_sessions "
                        "   set status=-2,"
                        "       dt_end=CURRENT_TIMESTAMP "
                        " where id_terminal=" + QString::number(data_app.terminal_id) + " ; ";
        query_close_old = new QSqlQuery(db);
        res_close_old = mySQL.exec(this, sql_close_old, "Закроем сессию", *query_close_old, true, db, data_app);
        delete query_close_old;


        // дата/время подключения терминала
        sql_terminal = "update db_terminals "
                       " set dt_last=CURRENT_TIMESTAMP "
                       " where id=" + QString::number(data_app.terminal_id) + " ; ";
        /*res_terminal =*/ mySQL.exec(this, sql_terminal, "Добавим терминал", *query_terminal, true, db, data_app);

        // сформируем запись сессии
        QString sql_session = "insert into db_sessions(id_point, id_operator, id_terminal, dt_start, dt_last, status, dt_end) "
                              " values(" + QString::number(data_app.id_point) + ", " + QString::number(data_app.id_operator) + ", " + QString::number(data_app.terminal_id) + ", CURRENT_TIMESTAMP, CURRENT_TIMESTAMP, 1, NULL) "
                              " returning id ; ";
        QSqlQuery *query_session = new QSqlQuery(db);
        /*bool res_session =*/ mySQL.exec(this, sql_session, "Добавим сессию", *query_session, true, db, data_app);
        query_session->next();
        data_app.id_session = query_session->value(0).toInt();

        refresh_points();
        ui->tc_op_pass->setText("");

        // загрузим описание
        QString sql_load = "select text from messages where version='" + data_app.version + "' ; ";
        QSqlQuery *query_load = new QSqlQuery(db);
        bool res_load = mySQL.exec(this, sql_load, "Загрузка описания программы", *query_load, true, db, data_app);
        QString s = "версия программы : " + data_app.version + "\n\n";
        while (query_load->next()) {
            s += query_load->value(0).toString() + "\n\n";
        }
        ui->text_comments->setPlainText(s);
        delete query_load;



        QString sql;
        QSqlQuery query = QSqlQuery(db);
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='terr_ocato' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.ocato= s.trimmed();

        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='terr_ocato_text' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.ocato_text= s.trimmed();

        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_short' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.smo_short= s.trimmed();



        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_master_state' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.smo_master_state= s.trimmed();

        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_master_FamIO' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.smo_master_FamIO= s.trimmed();




        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='filial_name' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.filial_name= s.trimmed();

        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='terr_name_rp' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.terr_name_rp= s.trimmed();



        // получим параметры подключения к базе ФИАС
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='fias_flag' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().toLower().trimmed();
        data_app.FIAS_flag = !(s.replace("false","").replace("0","").trimmed().isEmpty());

        sql = "select d.name, d.\"value\" "
              "  from db_data d "
              " where d.name='fias_db_drvr' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.FIAS_db_drvr = s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='fias_db_host' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.FIAS_db_host = s.trimmed();

        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='fias_db_name' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.FIAS_db_name = s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='fias_db_port' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.FIAS_db_port = s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='fias_db_user' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.FIAS_db_user = s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='fias_db_pass' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.FIAS_db_pass = s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='fias_db_admn' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.FIAS_db_admn = s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='fias_db_admp' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.FIAS_db_admp = s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='fias_db_connstring' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.FIAS_db_connstring= s.trimmed();

        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='old_db_drvr' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.old_db_drvr= s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='old_db_host' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.old_db_host= s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='old_db_port' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.old_db_port= s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='old_db_name' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.old_db_name= s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='old_db_user' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.old_db_user= s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='old_db_pass' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.old_db_pass= s.trimmed();




        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='acc_db_drvr' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.acc_db_drvr= s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='acc_db_host' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.acc_db_host= s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='acc_db_port' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.acc_db_port= s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='acc_db_name' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.acc_db_name= s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='acc_db_user' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.acc_db_user= s.trimmed();
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='acc_db_pass' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s += "  " + query.value(1).toString();
        data_app.acc_db_pass= s.trimmed();




        // реестер
        data_app.id_point = ui->combo_point->currentData().toInt();

        winreg = new QSettings(QSettings::UserScope, "МСК 'ИНКО-МЕД'", "ИНКО-МЕД - Прикрепление персон", NULL);
        data_app.terminal_id  = winreg->value(data_app.db_host + " - " + data_app.db_name + " - terminal_num", -1).toInt();
        data_app.terminal_name = winreg->value(data_app.db_host + " - " + data_app.db_name + " - terminal_name", " ").toString();

        db.transaction();

        // если новый терминал - получим новый terminal_num из базы
        if (data_app.terminal_id<0) {
            QString sql_terminal = "insert into terminals(terminal_name, status, id_point, dt_last) "
                                   "values (' ', 1, " + QString::number(data_app.id_point) + ", CURRENT_TIMESTAMP) "
                                   " returning id ; ";
            QSqlQuery *query_terminal = new QSqlQuery(db);
            if ( !query_terminal->exec(sql_terminal) ||
                 !query_terminal->next() ) {

                delete query_terminal;
                db.rollback();

                QMessageBox::warning(this, "Ошибка регистрации терминала",
                                     "При попытке регистрации нового терминала произошла ошибка");
            } else {
                data_app.terminal_id = query_terminal->value(0).toInt();
                data_app.terminal_name = " ";
                data_app.terminal_status = 1;

                winreg->setValue(data_app.db_host + " - " + data_app.db_name + " - terminal_num", data_app.terminal_id);
                winreg->setValue(data_app.db_host + " - " + data_app.db_name + " - terminal_name", data_app.terminal_name);

                winreg->sync();

                delete query_terminal;
                db.commit();
            }
        } else {
            // терминал уже известен - обновим его данные из базы
            QString sql_terminal = "select id_point, terminal_name, status "
                                   "  from terminals "
                                   " where id=" + QString::number(data_app.terminal_id) + " ";
            QSqlQuery *query_terminal = new QSqlQuery(db);
            if ( !query_terminal->exec(sql_terminal) ) {

                delete query_terminal;
                db.rollback();

                QMessageBox::warning(this, "Ошибка чтения данных терминала",
                                     "При попытке чтения данных терминала произошла ошибка");
            } else {
                query_terminal->next();

                data_app.terminal_id_point = query_terminal->value(0).toInt();
                data_app.terminal_name = query_terminal->value(1).toString();
                data_app.terminal_status = query_terminal->value(2).toInt();

                winreg->setValue(data_app.db_host + " - " + data_app.db_name + " - terminal_num", data_app.terminal_id);
                winreg->setValue(data_app.db_host + " - " + data_app.db_name + " - terminal_name", data_app.terminal_name);

                winreg->sync();

                delete query_terminal;
                db.commit();
            }
        }

    } else {
        this->resize(300,300);
    }

    if (ui->ch_change_pass->isChecked()) {
        ui->pan_DB_params->show();
    }

    // ini-файл
    // [polisesDB]
    settings->beginGroup("polisesDB");
    settings->setValue("drvr", data_app.drvr);
    settings->setValue("host", data_app.host);
    settings->setValue("port", data_app.port);
    settings->setValue("base", data_app.base);
    settings->setValue("user", data_app.user);
    settings->setValue("pass", data_app.pass);
    settings->endGroup();
    // [resent]
    settings->beginGroup("resent");
    settings->setValue("point", ui->combo_point->currentText().toUtf8());
    settings->setValue("operator", ui->combo_operator->currentText().toUtf8());
    settings->endGroup();
    // [KLADR]
    settings->beginGroup("KLADR");
    settings->setValue("ocato",data_app.ocato_full.toUtf8());
    settings->setValue("subj", QString::number(data_app.KLADR_subj));
    settings->setValue("dstr", QString::number(data_app.KLADR_dstr));
    settings->setValue("city", QString::number(data_app.KLADR_city));
    settings->setValue("nasp", QString::number(data_app.KLADR_nasp));
    settings->setValue("strt", QString::number(data_app.KLADR_strt));
    settings->setValue("house",QString::number(data_app.KLADR_house));
    settings->setValue("corp", data_app.KLADR_corp.toUtf8());
    settings->endGroup();
    // [flags]
    settings->beginGroup("flags");
    settings->setValue("readonly", data_app.f_readonly);
    settings->setValue("is_tech", data_app.is_tech);
    settings->setValue("is_head", data_app.is_head);
    settings->setValue("is_locl", data_app.is_locl);
    settings->setValue("is_call", data_app.is_call);
    settings->endGroup();
    // [paths]
    /*settings->beginGroup("paths");
    settings->setValue("path_install", data_app.path_install.toUtf8());
    settings->setValue("path_arch", data_app.path_arch.toUtf8());
    settings->setValue("path_reports", data_app.path_reports.toUtf8());
    settings->setValue("path_temp", data_app.path_temp.toUtf8());
    settings->setValue("path_foto", data_app.path_foto.toUtf8());
    settings->setValue("path_sign", data_app.path_sign.toUtf8());
    settings->setValue("path_in", data_app.path_in.toUtf8());
    settings->setValue("path_out", data_app.path_out.toUtf8());
    settings->setValue("path_in_backup", data_app.path_in_backup.toUtf8());
    settings->setValue("path_out_backup", data_app.path_out_backup.toUtf8());
    settings->endGroup();
    // [smo]
    settings->beginGroup("smo");
    settings->setValue("terr_ocato", data_app.ocato);
    settings->setValue("terr_ocato_text", data_app.ocato_text.toUtf8());
    settings->setValue("smo_ogrn", data_app.smo_ogrn);
    settings->setValue("smo_regnum", data_app.smo_regnum);
    settings->setValue("smo_logo1", data_app.smo_logo1.toUtf8());
    settings->setValue("smo_logo2", data_app.smo_logo2.toUtf8());
    settings->setValue("smo_logo3", data_app.smo_logo3.toUtf8());
    settings->setValue("smo_name", data_app.smo_name.toUtf8());
    settings->setValue("smo_name_full", data_app.smo_name_full.toUtf8());
    settings->setValue("smo_director", data_app.smo_director.toUtf8());
    settings->setValue("smo_oms_chif", data_app.smo_oms_chif.toUtf8());
    settings->setValue("smo_name_full_rp", data_app.smo_name_full_rp.toUtf8());
    settings->setValue("smo_address", data_app.smo_address.toUtf8());
    settings->setValue("smo_contact_info", data_app.smo_contact_info.toUtf8());
    settings->setValue("point_regnum", data_app.point_regnum);
    settings->setValue("point_name", data_app.point_name.toUtf8());
    settings->setValue("operator_fio", ui->combo_operator->currentText().toUtf8());
    settings->endGroup();
    // [filial]
    settings->beginGroup("filial");
    settings->setValue("terr_name", data_app.terr_name);
    settings->setValue("terr_name_rp", data_app.terr_name_rp);
    settings->setValue("tfoms_director", data_app.tfoms_director);
    settings->setValue("tfoms_director_2", data_app.tfoms_director_2);
    settings->setValue("tfoms_director_fio", data_app.tfoms_director_fio);
    settings->setValue("filial_director", data_app.filial_director);
    settings->setValue("filial_director_2", data_app.filial_director_2);
    settings->setValue("filial_director_fio", data_app.filial_director_fio);
    settings->endGroup();
    // [assig]
    settings->beginGroup("assig");
    settings->setValue("csv_headers", data_app.csv_headers);
    settings->endGroup();
    // [xml]
    settings->beginGroup("xml");
    settings->setValue("xml_vers", data_app.xml_vers);
    settings->setValue("snils_format", QString::number(data_app.snils_format));
    settings->setValue("split_by_ot", QString::number(data_app.split_by_ot));
    settings->setValue("send_p021_in_p060", QString::number(data_app.send_p021_in_p060));
    settings->setValue("pol_start_date_by_vs", QString::number(data_app.pol_start_date_by_vs));
    settings->setValue("zip_packs", data_app.f_zip_packs);
    settings->endGroup();
    // [VS]
    settings->beginGroup("VS");
    //settings->setValue("inko_name", data_app.vs_inko_name);
    //settings->setValue("inko_address", data_app.vs_inko_address);
    settings->setValue("blank_width_mm", QString::number(data_app.vs_blank_width_mm));
    settings->setValue("blank_height_mm", QString::number(data_app.vs_blank_height_mm));
    settings->setValue("field_left_mm", QString::number(data_app.vs_field_left_mm));
    settings->setValue("field_right_mm", QString::number(data_app.vs_field_right_mm));
    settings->setValue("field_top_mm", QString::number(data_app.vs_field_top_mm));
    settings->setValue("field_bottom_mm", QString::number(data_app.vs_field_bottom_mm));
    settings->endGroup();*/
    // [req_ch_smo]
    //settings->beginGroup("req_ch_smo");
    //settings->setValue("req_smo_ogrn", data_app.req_smo_ogrn);
    //settings->endGroup();
    // [notice_pol]
    //settings->beginGroup("notice_pol");
    //settings->endGroup();
    // [filials]
    settings->beginGroup("filials");
    settings->setValue("filials_list", data_app.filials_list);
    settings->endGroup();
    if (data_app.filials_list.indexOf("kursk")>=0) {
        // [kursk]
        settings->beginGroup("kursk");
        settings->setValue("drvr", data_app.kursk_drvr);
        settings->setValue("host", data_app.kursk_host);
        settings->setValue("port", QString::number(data_app.kursk_port));
        settings->setValue("base", data_app.kursk_base);
        settings->setValue("user", data_app.kursk_user);
        settings->setValue("pass", data_app.kursk_pass);
        settings->endGroup();
    }
    if (data_app.filials_list.indexOf("belg")>=0) {
        // [belg]
        settings->beginGroup("belg");
        settings->setValue("drvr", data_app.belg_drvr);
        settings->setValue("host", data_app.belg_host);
        settings->setValue("port", QString::number(data_app.belg_port));
        settings->setValue("base", data_app.belg_base);
        settings->setValue("user", data_app.belg_user);
        settings->setValue("pass", data_app.belg_pass);
        settings->endGroup();
    }
    if (data_app.filials_list.indexOf("voro")>=0) {
        // [test]
        settings->beginGroup("voro");
        settings->setValue("drvr", data_app.voro_drvr);
        settings->setValue("host", data_app.voro_host);
        settings->setValue("port", QString::number(data_app.voro_port));
        settings->setValue("base", data_app.voro_base);
        settings->setValue("user", data_app.voro_user);
        settings->setValue("pass", data_app.voro_pass);
        settings->endGroup();
    }
    if (data_app.filials_list.indexOf("work")>=0) {
        // [work]
        settings->beginGroup("work");
        settings->setValue("drvr", data_app.work_drvr);
        settings->setValue("host", data_app.work_host);
        settings->setValue("port", QString::number(data_app.work_port));
        settings->setValue("base", data_app.work_base);
        settings->setValue("user", data_app.work_user);
        settings->setValue("pass", data_app.work_pass);
        settings->endGroup();
    }

    // [smo]
    settings->beginGroup("smo");
    settings->setValue("terr_ocato", data_app.ocato);
    settings->setValue("terr_ocato_text", data_app.ocato_text);
    settings->setValue("smo_ogrn", data_app.smo_ogrn);
    settings->setValue("smo_regnum", data_app.smo_regnum);
    settings->setValue("smo_logo1", data_app.smo_logo1);
    settings->setValue("smo_logo2", data_app.smo_logo2);
    settings->setValue("smo_logo3", data_app.smo_logo3);
    settings->setValue("smo_short", data_app.smo_short);
    settings->setValue("smo_name", data_app.smo_name);
    settings->setValue("smo_name_full", data_app.smo_name_full);
    settings->setValue("smo_name_full_rp", data_app.smo_name_full_rp);
    settings->setValue("smo_director", data_app.smo_director);
    settings->setValue("smo_master_state", data_app.smo_master_state);
    settings->setValue("smo_master_FamIO", data_app.smo_master_FamIO);
    settings->setValue("smo_oms_chif", data_app.smo_oms_chif);
    settings->setValue("smo_address", data_app.smo_address);
    settings->setValue("smo_contact_info", data_app.smo_contact_info);

    settings->setValue("letter_doer1_FamIO", data_app.letter_doer1_FamIO);
    settings->setValue("letter_doer1_phone", data_app.letter_doer1_phone);
    settings->setValue("letter_doer2_FamIO", data_app.letter_doer2_FamIO);
    settings->setValue("letter_doer2_phone", data_app.letter_doer2_phone);
    settings->endGroup();


    settings->sync();

    // TEMP
    /*QDir p_temp(data_app.path_temp);
    if (!p_temp.exists()) p_temp.mkpath(data_app.path_temp);
    // FOTO
    QDir p_foto(data_app.path_foto);
    if (!p_foto.exists()) p_foto.mkpath(data_app.path_foto);
    // SIGN
    QDir p_sign(data_app.path_sign);
    if (!p_sign.exists()) p_sign.mkpath(data_app.path_sign);
    // IN
    QDir p_in(data_app.path_in);
    if (!p_in.exists()) p_in.mkpath(data_app.path_in);
    // OUT
    QDir p_out(data_app.path_out);
    if (!p_out.exists()) p_out.mkpath(data_app.path_out);
    // IN_BACKUP
    QDir p_in_backup(data_app.path_in_backup);
    if (!p_in_backup.exists()) p_in_backup.mkpath(data_app.path_in_backup);
    // OUT_BACKUP
    QDir p_out_backup(data_app.path_out_backup);
    if (!p_out_backup.exists()) p_out_backup.mkpath(data_app.path_out_backup);
    // INSTALL
    // ARCH
    QDir p_arch(data_app.path_arch);
    if (!p_arch.exists()) QMessageBox::warning(this, "Отсутствует модуль!", "ВНИМАНИЕ!\nОтстутствует предопределённый путь к папке модуля архивирования:\n" + data_app.path_arch);
*/
    // REPORTS
    QDir p_reports(data_app.path_reports);
    if (!p_reports.exists()) p_reports.mkpath(data_app.path_reports);
    // REPORTS/Шаблоны_писем
    QDir p_reports_letters(data_app.path_reports + "Шаблоны_писем");
    if (!p_reports_letters.exists()) p_reports_letters.mkpath(data_app.path_reports + "Шаблоны_писем");
    // OUT
    QDir p_out(data_app.path_out);
    if (!p_out.exists())
        if (!p_out.mkpath(data_app.path_out))
            QMessageBox::warning(this, "Отсутствует модуль!", "ВНИМАНИЕ!\nОтстутствует предопределённый путь к папке отчётов:\n" + data_app.path_out);
    // REPORTS/_FIN_REPS_
    QDir p_fin_reps(data_app.path_out + "_FIN_REPS_/");
    if (!p_fin_reps.exists())
        if (!p_fin_reps.mkpath(data_app.path_out + "_FIN_REPS_/"))
            QMessageBox::warning(this, "Отсутствует модуль!", "ВНИМАНИЕ!\nОтстутствует предопределённый путь к папке фин-отчётов\n" + data_app.path_out + "_FIN_REPS_/");

    QSqlQuery query = QSqlQuery(db);
    QString sql;
    QString s;

    // проверка сервера
    sql = "select d.name, d.\"value\" "
          "  from db_data d  "
          " where d.name='host_name' ; ";
    bool res_h = query.exec( sql );
    s = "";
    int n = query.size();
    while (query.next()) {
        s += "  " + query.value(1).toString();
    }
    ui->lab_host->setText(s);
    data_app.db_host = s.trimmed();

    // проверка имени БД
    sql = "select d.name, d.\"value\"   from db_data d   where d.name='DB_name' ; ";
    bool res_db = query.exec( sql );
    s = "";
    n = query.size();
    while (query.next()) {
        s += "  " + query.value(1).toString();
    }
    data_app.db_name = s.trimmed();

    // проверка версии
    sql = "select d.name, d.\"value\"   from db_data d   where d.name='DB_version' ; ";
    bool res_v = query.exec( sql );
    s = "";
    int m = query.size();
    while (query.next()) {
        s += "  " + query.value(1).toString();
    }
    ui->lab_vers->setText(s);
    data_app.db_vers = s.trimmed();
}

void ConnectionWindow::refresh_points() {
    this->setCursor(Qt::WaitCursor);

    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "";
    sql += QString("select p.id, '('||p.point_regnum||') '||p.point_name as name"
                   " from points p "
                   " order by p.point_regnum ; ");
    query->exec( sql );
    ui->combo_point->clear();

    // заполнение списка пунктов выдачи
    while (query->next()) {
        ui->combo_point->addItem( query->value(1).toString(),
                                  query->value(0).toInt() );
    }
    query->finish();
    delete query;

    // восстановим прежнее значение
    if (settings->value("resent/point", "").toString()!="") {
        ui->combo_point->setCurrentIndex(ui->combo_point->findText(QString::fromUtf8(settings->value("resent/point", " ").toByteArray())));
    } else {
        ui->combo_point->setCurrentIndex(0);
    }
    data_app.point_name = ui->combo_point->currentText();

    this->setCursor(Qt::ArrowCursor);

    refresh_operators(ui->combo_point->currentData().toInt());
}

void ConnectionWindow::refresh_operators(int id_point) {
    this->setCursor(Qt::WaitCursor);

    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "";
    sql += QString("select u.id, u.oper_fio ");
    sql += QString(" from operators u ");
    sql += (id_point==-1) ? (QString(" ")) : (QString(" where u.id_point=") + QString::number(id_point));
    sql += QString(" order by u.user_name ; ");
    query->exec( sql );
    ui->combo_operator->clear();

    // заполнение списка операторов
    while (query->next()) {
        ui->combo_operator->addItem( query->value(1).toString(),
                                     query->value(0).toInt() );
    }
    query->finish();
    delete query;

    // восстановим прежнее значение
    if (settings->value("resent/operator", "").toString()!="") {
        ui->combo_operator->setCurrentIndex(ui->combo_operator->findText(QString::fromUtf8(settings->value("resent/operator", " ").toByteArray())));
    } else {
        ui->combo_operator->setCurrentIndex(0);
    }

    this->setCursor(Qt::ArrowCursor);
}

void ConnectionWindow::on_tc_pass_returnPressed() {
    on_bn_save_DB_params_clicked();
}

void ConnectionWindow::on_bn_save_DB_params_clicked() {
    data_app.drvr = ui->combo_driver->currentText();
    data_app.host = ui->tc_host->text();
    data_app.port = ui->spin_port->value();
    data_app.base = ui->tc_base->text();
    data_app.user = ui->tc_user->text();
    data_app.pass = ui->tc_pass->text();

    // сохраним параметры подключения к базе обновлений
    settings->beginGroup("polisesDB");
    settings->setValue("drvr", data_app.drvr);
    settings->setValue("host", data_app.host);
    settings->setValue("port", data_app.port);
    settings->setValue("base", data_app.base);
    settings->setValue("user", data_app.user);
    settings->setValue("pass", data_app.pass);
    settings->endGroup();

    settings->sync();

    // обновим подключение к базе обновлений
    refresh_connection();
    //this->resize(QSize(350,200));
}

void ConnectionWindow::on_combo_point_activated(int index) {
    refresh_operators(ui->combo_point->itemData(ui->combo_point->currentIndex()).toInt());
    data_app.point_name = ui->combo_point->currentText();
    QString name = ui->combo_point->currentText();
    data_app.point_regnum = QString("000" + QString(name.mid(name.indexOf("(")+1, name.indexOf(")")-name.indexOf("(")-1))).right(3);
}

void ConnectionWindow::on_tc_op_pass_returnPressed() {
    on_bn_start_clicked();
}

void ConnectionWindow::on_bn_start_clicked() {
    ui->bn_start->setEnabled(false);

    // проверим пароль оператора
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "";
    sql += QString("select u.id, u.user_pass, u.oper_rights "
                   " from operators u "
                   " where u.id=");
    sql += QString::number(ui->combo_operator->currentData().toInt()) + " ; ";
    query->exec( sql );
    query->next();
    QString pass = query->value(1).toString();
    int rights = query->value(2).toInt();
    if (rights<1 || rights>4)
        rights = 4;

    if (pass==ui->tc_op_pass->text()) {
        ui->tc_op_pass->setText("");
        QApplication::processEvents();


        // получим данне филиала
        QString sql, s;
        QSqlQuery query(db);

        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_ogrn' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.smo_ogrn = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_regnum' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.smo_regnum = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_short' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.smo_short = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_name' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.smo_name = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_name_full' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.smo_name_full = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_name_full_rp' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.smo_name_full_rp = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_director' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.smo_director = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_oms_chif' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.smo_oms_chif = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_address' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.smo_address = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_contact_info' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.smo_contact_info = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_logo1' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.smo_logo1 = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_logo2' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.smo_logo2 = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='smo_logo3' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.smo_logo3 = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='terr_name' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.terr_name = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='terr_name_rp' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.terr_name_rp = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='tfoms_director' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.tfoms_director = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='tfoms_director_2' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.tfoms_director_2 = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='tfoms_director_fio' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.tfoms_director_fio = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='tfoms_chif' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.tfoms_chif = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='filial_name' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.filial_name = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='filial_name_rp' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.filial_name_rp = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='filial_city' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.filial_city = s;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='filial_by_city' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.filial_by_city = s;
        //QString filial_by_city_tp;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='filial_by_city_tp' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.filial_by_city_tp = s;
        //QString filial_director;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='filial_director' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.filial_director = s;
        //QString filial_director_2;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='filial_director_2' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.filial_director_2 = s;
        //QString filial_director_fio;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='filial_director_fio' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.filial_director_fio = s;
        //QString filial_chif;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='filial_chif' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.filial_chif = s;
        //QString filial_phone;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='filial_phone' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.filial_phone = s;
        //QString filial_phone_2;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='filial_phone_2' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.filial_phone_2 = s;
        //QString filial_email;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='filial_email' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.filial_email = s;
        //QString filial_email_2;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='filial_email_2' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.filial_email_2 = s;
        //QString ocato;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='terr_ocato' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.ocato = s;
        //QString ocato_text;
        sql = "select d.name, d.\"value\" "
              "  from db_data d  "
              " where d.name='terr_ocato_text' ; ";
        s = "";
        query.exec(sql);
        query.next();
        s = query.value(1).toString().trimmed();
        data_app.ocato_text = s;


        // [polisesDB]
        settings->beginGroup("polisesDB");
        settings->setValue("drvr", data_app.drvr);
        settings->setValue("host", data_app.host);
        settings->setValue("port", data_app.port);
        settings->setValue("base", data_app.base);
        settings->setValue("user", data_app.user);
        settings->setValue("pass", data_app.pass);
        settings->endGroup();
        // [resent]
        settings->beginGroup("resent");
        settings->setValue("point", ui->combo_point->currentText().toUtf8());
        settings->setValue("operator", ui->combo_operator->currentText().toUtf8());
        settings->endGroup();
        // [flags]
        data_app.is_tech = (rights==1);
        data_app.is_head = (rights==2);
        data_app.is_locl = (rights==3);
        data_app.is_call = (rights==4);
        settings->beginGroup("flags");
        settings->setValue("readonly", data_app.f_readonly);
        settings->setValue("is_tech", data_app.is_tech);
        settings->setValue("is_head", data_app.is_head);
        settings->setValue("is_locl", data_app.is_locl);
        settings->setValue("is_call", data_app.is_call);
        settings->endGroup();
        //[KLADR]
        settings->beginGroup("KLADR");
        settings->setValue("ocato",data_app.ocato_full.toUtf8());
        settings->setValue("subj", QString::number(data_app.KLADR_subj));
        settings->setValue("dstr", QString::number(data_app.KLADR_dstr));
        settings->setValue("city", QString::number(data_app.KLADR_city));
        settings->setValue("nasp", QString::number(data_app.KLADR_nasp));
        settings->setValue("strt", QString::number(data_app.KLADR_strt));
        settings->setValue("house",QString::number(data_app.KLADR_house));
        settings->setValue("corp", data_app.KLADR_corp.toUtf8());
        settings->endGroup();
        // [paths]
        /*settings->beginGroup("paths");
        settings->setValue("path_install", data_app.path_install.toUtf8());
        settings->setValue("path_arch", data_app.path_arch.toUtf8());
        settings->setValue("path_reports", data_app.path_reports.toUtf8());
        settings->setValue("path_temp", data_app.path_temp.toUtf8());
        settings->setValue("path_foto", data_app.path_foto.toUtf8());
        settings->setValue("path_sign", data_app.path_sign.toUtf8());
        settings->setValue("path_in", data_app.path_in.toUtf8());
        settings->setValue("path_out", data_app.path_out.toUtf8());
        settings->setValue("path_in_backup", data_app.path_in_backup.toUtf8());
        settings->setValue("path_out_backup", data_app.path_out_backup.toUtf8());
        settings->endGroup();
        // [smo]
        settings->beginGroup("smo");
        settings->setValue("terr_ocato", data_app.ocato);
        settings->setValue("terr_ocato_text", data_app.ocato_text.toUtf8());
        settings->setValue("smo_ogrn", data_app.smo_ogrn);
        settings->setValue("smo_regnum", data_app.smo_regnum);
        settings->setValue("smo_logo1", data_app.smo_logo1.toUtf8());
        settings->setValue("smo_logo2", data_app.smo_logo2.toUtf8());
        settings->setValue("smo_logo3", data_app.smo_logo3.toUtf8());
        settings->setValue("smo_name", data_app.smo_name.toUtf8());
        settings->setValue("point_regnum", data_app.point_regnum.toUtf8());
        settings->setValue("smo_name_full", data_app.smo_name_full.toUtf8());
        settings->setValue("smo_name_full_rp", data_app.smo_name_full_rp.toUtf8());
        settings->setValue("smo_director", data_app.smo_director.toUtf8());
        settings->setValue("smo_oms_chif", data_app.smo_oms_chif.toUtf8());
        settings->setValue("smo_address", data_app.smo_address.toUtf8());
        settings->setValue("smo_contact_info", data_app.smo_contact_info.toUtf8());
        settings->setValue("point_name", data_app.point_name.toUtf8());
        settings->setValue("operator_fio", data_app.operator_fio.toUtf8());
        settings->endGroup();
        // [assig]
        settings->beginGroup("assig");
        settings->setValue("csv_headers", data_app.csv_headers);
        settings->endGroup();
        // [filial]
        settings->beginGroup("filial");
        settings->setValue("terr_name",          data_app.terr_name);
        settings->setValue("terr_name_rp",       data_app.terr_name_rp);
        settings->setValue("tfoms_director",     data_app.tfoms_director);
        settings->setValue("tfoms_director_2",   data_app.tfoms_director_2);
        settings->setValue("tfoms_director_fio", data_app.tfoms_director_fio);
        settings->setValue("filial_director",    data_app.filial_director);
        settings->setValue("filial_director_2",  data_app.filial_director_2);
        settings->setValue("filial_director_fio",data_app.filial_director_fio);
        settings->endGroup();
        // [xml]
        settings->beginGroup("xml");
        settings->setValue("xml_vers", data_app.xml_vers);
        settings->setValue("snils_format", QString::number(data_app.snils_format));
        settings->setValue("split_by_ot", QString::number(data_app.split_by_ot));
        settings->setValue("send_p021_in_p060", QString::number(data_app.send_p021_in_p060));
        settings->setValue("pol_start_date_by_vs", QString::number(data_app.pol_start_date_by_vs));
        settings->setValue("zip_packs", data_app.f_zip_packs);
        settings->endGroup();
        // [VS]
        settings->beginGroup("VS");
        //settings->setValue("inko_name", data_app.vs_inko_name);
        //settings->setValue("inko_address", data_app.vs_inko_address);
        settings->setValue("blank_width_mm", QString::number(data_app.vs_blank_width_mm));
        settings->setValue("blank_height_mm", QString::number(data_app.vs_blank_height_mm));
        settings->setValue("field_left_mm", QString::number(data_app.vs_field_left_mm));
        settings->setValue("field_right_mm", QString::number(data_app.vs_field_right_mm));
        settings->setValue("field_top_mm", QString::number(data_app.vs_field_top_mm));
        settings->setValue("field_bottom_mm", QString::number(data_app.vs_field_bottom_mm));
        settings->endGroup();
        // [req_ch_smo]
        //settings->beginGroup("req_ch_smo");
        //settings->endGroup();
        // [notice_pol]
        //settings->beginGroup("notice_pol");
        //settings->endGroup();*/
        // [filials]
        settings->beginGroup("filials");
        settings->setValue("filials_list", data_app.filials_list);
        settings->endGroup();
        if (data_app.filials_list.indexOf("kursk")>=0) {
            // [kursk]
            settings->beginGroup("kursk");
            settings->setValue("drvr", data_app.kursk_drvr);
            settings->setValue("host", data_app.kursk_host);
            settings->setValue("port", QString::number(data_app.kursk_port));
            settings->setValue("base", data_app.kursk_base);
            settings->setValue("user", data_app.kursk_user);
            settings->setValue("pass", data_app.kursk_pass);
            settings->endGroup();
        }
        if (data_app.filials_list.indexOf("belg")>=0) {
            // [belg]
            settings->beginGroup("belg");
            settings->setValue("drvr", data_app.belg_drvr);
            settings->setValue("host", data_app.belg_host);
            settings->setValue("port", QString::number(data_app.belg_port));
            settings->setValue("base", data_app.belg_base);
            settings->setValue("user", data_app.belg_user);
            settings->setValue("pass", data_app.belg_pass);
            settings->endGroup();
        }
        if (data_app.filials_list.indexOf("voro")>=0) {
            // [test]
            settings->beginGroup("voro");
            settings->setValue("drvr", data_app.voro_drvr);
            settings->setValue("host", data_app.voro_host);
            settings->setValue("port", QString::number(data_app.voro_port));
            settings->setValue("base", data_app.voro_base);
            settings->setValue("user", data_app.voro_user);
            settings->setValue("pass", data_app.voro_pass);
            settings->endGroup();
        }
        if (data_app.filials_list.indexOf("work")>=0) {
            // [work]
            settings->beginGroup("work");
            settings->setValue("drvr", data_app.work_drvr);
            settings->setValue("host", data_app.work_host);
            settings->setValue("port", QString::number(data_app.work_port));
            settings->setValue("base", data_app.work_base);
            settings->setValue("user", data_app.work_user);
            settings->setValue("pass", data_app.work_pass);
            settings->endGroup();
        }

        // [smo]
        settings->beginGroup("smo");
        settings->setValue("terr_ocato", data_app.ocato);
        settings->setValue("terr_ocato_text", data_app.ocato_text);
        settings->setValue("smo_ogrn", data_app.smo_ogrn);
        settings->setValue("smo_regnum", data_app.smo_regnum);
        settings->setValue("smo_logo1", data_app.smo_logo1);
        settings->setValue("smo_logo2", data_app.smo_logo2);
        settings->setValue("smo_logo3", data_app.smo_logo3);
        settings->setValue("smo_short", data_app.smo_short);
        settings->setValue("smo_name", data_app.smo_name);
        settings->setValue("smo_name_full", data_app.smo_name_full);
        settings->setValue("smo_name_full_rp", data_app.smo_name_full_rp);
        settings->setValue("smo_director", data_app.smo_director);
        settings->setValue("smo_master_state", data_app.smo_master_state);
        settings->setValue("smo_master_FamIO", data_app.smo_master_FamIO);
        settings->setValue("smo_oms_chif", data_app.smo_oms_chif);
        settings->setValue("smo_address", data_app.smo_address);
        settings->setValue("smo_contact_info", data_app.smo_contact_info);

        settings->setValue("letter_doer1_FamIO", data_app.letter_doer1_FamIO);
        settings->setValue("letter_doer1_phone", data_app.letter_doer1_phone);
        settings->setValue("letter_doer2_FamIO", data_app.letter_doer2_FamIO);
        settings->setValue("letter_doer2_phone", data_app.letter_doer2_phone);
        settings->endGroup();


        settings->sync();

        data_app.id_point = ui->combo_point->currentData().toInt();
        data_app.id_operator = ui->combo_operator->currentData().toInt();
        data_app.operator_fio = ui->combo_operator->currentText();

        ui->lab_start->setVisible(true);
        QApplication::processEvents();

        delete assignment_w;
        assignment_w = new assignment_wnd(db, data_app, *settings);

        //QMessageBox::warning(this, QString("go"), QString("go"));
        assignment_w->setWindowState( assignment_w->windowState() | Qt::WindowMaximized);
        QApplication::processEvents();
        assignment_w->show();

        this->hide();

    } else {
        QMessageBox::warning(this, QString("Неверный пароль"), QString("Неверный пароль\n"));
        ui->tc_op_pass->setText("");
        ui->bn_start->setEnabled(true);
        //this->resize(300,100);
        return;
    }
    ui->bn_start->setEnabled(true);
}

void ConnectionWindow::on_ch_change_pass_clicked(bool checked) {
    ui->pan_DB_params->setVisible(checked);
    on_combo_filial_activated("");
}

void ConnectionWindow::on_combo_operator_activated(const QString &arg1) {
    ui->tc_op_pass->setText("");
}

void ConnectionWindow::on_tc_op_pass_editingFinished() {
    //on_bn_start_clicked();
}

void ConnectionWindow::on_bn_exit_clicked() {
    if (data_app.id_session>0) {
        // закрытие сессии
        QString sql_close = "update db_sessions "
                            " set status=-1,"
                            "     dt_end=CURRENT_TIMESTAMP "
                            " where id=" + QString::number(data_app.id_session) + " ; ";
        QSqlQuery *query_close = new QSqlQuery(db);
        bool res_close = mySQL.exec(this, sql_close, "Закроем сессию", *query_close, true, db, data_app);
    }

    QApplication::closeAllWindows();
}

void ConnectionWindow::on_combo_operator_currentIndexChanged(const QString &arg1) {
    data_app.operator_fio = arg1;
}

void ConnectionWindow::on_bn_messages_clicked() {
    // удалим старое описание
    QString sql_del = "delete from messages where version='" + data_app.version + "' ; ";
    QSqlQuery *query_del = new QSqlQuery(db);
    bool res_del = mySQL.exec(this, sql_del, "Очистка старого описания программы", *query_del, true, db, data_app);

    // сохраним текст в базу
    QString s = ui->text_comments->toPlainText();
    s = s.right(s.size()-s.indexOf("\n\n")-2);
    QString sql_ins = "insert into messages(version, text) "
                      "values('" + data_app.version + "','" + s + "') ; ";
    QSqlQuery *query_ins = new QSqlQuery(db);
    bool res_ins = mySQL.exec(this, sql_ins, "Сохранение нового описания программы", *query_ins, true, db, data_app);
}

void ConnectionWindow::on_combo_filial_activated(const QString &arg1) {
    switch (ui->combo_filial->currentData().toInt()) {
    case -1: {
            ui->combo_driver->setCurrentIndex(ui->combo_driver->findText(data_app.work_drvr));
            ui->tc_host->setText(data_app.work_host);
            ui->spin_port->setValue(data_app.work_port);
            ui->tc_base->setText(data_app.work_base);
            ui->tc_user->setText(data_app.work_user);
            ui->tc_pass->setText(data_app.work_pass);
        }
        break;
    case 1: {
            ui->combo_driver->setCurrentIndex(ui->combo_driver->findText(data_app.voro_drvr));
            ui->tc_host->setText(data_app.voro_host);
            ui->spin_port->setValue(data_app.voro_port);
            ui->tc_base->setText(data_app.voro_base);
            ui->tc_user->setText(data_app.voro_user);
            ui->tc_pass->setText(data_app.voro_pass);
        }
        break;
    case 8: {
            ui->combo_driver->setCurrentIndex(ui->combo_driver->findText(data_app.kursk_drvr));
            ui->tc_host->setText(data_app.kursk_host);
            ui->spin_port->setValue(data_app.kursk_port);
            ui->tc_base->setText(data_app.kursk_base);
            ui->tc_user->setText(data_app.kursk_user);
            ui->tc_pass->setText(data_app.kursk_pass);
        }
        break;
    case 64: {
            ui->combo_driver->setCurrentIndex(ui->combo_driver->findText(data_app.belg_drvr));
            ui->tc_host->setText(data_app.belg_host);
            ui->spin_port->setValue(data_app.belg_port);
            ui->tc_base->setText(data_app.belg_base);
            ui->tc_user->setText(data_app.belg_user);
            ui->tc_pass->setText(data_app.belg_pass);
        }
        break;
    default:
        break;
    }
}

void ConnectionWindow::on_combo_driver_currentIndexChanged(const QString &arg1) {
    ui->spin_port->setEnabled(arg1!="QIBASE");
}
