#include "old_db_wnd.h"
#include "ui_old_db_wnd.h"
#include <QMessageBox>
#include <QDesktopWidget>
#include <QFileDialog>

#include "ki_funcs.h"

old_db_wnd::old_db_wnd(QSqlDatabase &db, QSqlDatabase &db_FIAS, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    db(db),
    db_FIAS(db_FIAS),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::old_db_wnd)
{
    ui->setupUi(this);
    ui->pan_wait->setVisible(false);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    cnt_persons            = 0;
    cnt_persons_docs       = 0;
    cnt_persons_old       = 0;
    cnt_vizits             = 0;
    cnt_events             = 0;
    cnt_polises_close      = 0;
    cnt_polises            = 0;
    cnt_blanks_vs          = 0;
    cnt_blanks_pol         = 0;
    cnt_blanks_uec         = 0;
    cnt_spr_insure_errors  = 0;
    cnt_spr_oksm           = 0;
    cnt_spr_ocato          = 0;
    cnt_spr_ocato_full     = 0;
    cnt_addresses          = 0;
    cnt_assig_by_terr      = 0;
    cnt_persons_assig      = 0;
    cnt_spr_assig_errors   = 0;
    cnt_spr_medorgs        = 0;
    cnt_spr_medparts       = 0;
    cnt_spr_medsites       = 0;
    cnt_spr_medters        = 0;
    cnt_spr_ms_r_mt        = 0;
    cnt_spr_medsites_types = 0;
    cnt_spr_medters_spec   = 0;

    ui->bn_FIAS2KLADR->setVisible(false);
    //ui->ch_process_new->setVisible(false);

    ui->cnt_polises_close->setVisible(false);

    ui->cnt_persons->setText(" -//- ");
    ui->cnt_persons_docs->setText(" -//- ");
    ui->cnt_persons_old->setText(" -//- ");
    ui->cnt_vizits->setText(" -//- ");
    ui->cnt_events->setText(" -//- ");
    ui->cnt_polises_close->setText(" -//- ");
    ui->cnt_polises->setText(" -//- ");
    ui->cnt_blanks_vs->setText(" -//- ");
    ui->cnt_blanks_pol->setText(" -//- ");
    ui->cnt_blanks_uec->setText(" -//- ");
    ui->cnt_spr_insure_errors->setText(" -//- ");
    ui->cnt_spr_oksm->setText(" -//- ");
    ui->cnt_spr_ocato->setText(" -//- ");
    ui->cnt_spr_ocato_full->setText(" -//- ");
    ui->cnt_addresses->setText(" -//- ");
    ui->cnt_assig_by_terr->setText(" -//- ");
    ui->cnt_persons_assig->setText(" -//- ");
    ui->cnt_spr_assig_errors->setText(" -//- ");
    ui->cnt_spr_medorgs->setText(" -//- ");
    ui->cnt_spr_medparts->setText(" -//- ");
    ui->cnt_spr_medsites->setText(" -//- ");
    ui->cnt_spr_medters->setText(" -//- ");
    ui->cnt_spr_ms_r_mt->setText(" -//- ");
    ui->cnt_spr_medsites_types->setText(" -//- ");
    ui->cnt_spr_medters_spec->setText(" -//- ");

    // подсчитаем существующие записи
    QSqlQuery *query_cnt = new QSqlQuery(db);
    query_cnt->exec("select count(*) from persons ; ");            query_cnt->next();     ui->cnt_persons_base->setText(            show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from persons_docs ; ");       query_cnt->next();     ui->cnt_persons_docs_base->setText(       show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from persons_old ; ");        query_cnt->next();     ui->cnt_persons_old_base->setText(        show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from vizits ; ");             query_cnt->next();     ui->cnt_vizits_base->setText(             show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from events ; ");             query_cnt->next();     ui->cnt_events_base->setText(             show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from polises ; ");            query_cnt->next();     ui->cnt_polises_base->setText(            show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from blanks_vs ; ");          query_cnt->next();     ui->cnt_blanks_vs_base->setText(          show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from blanks_pol ; ");         query_cnt->next();     ui->cnt_blanks_pol_base->setText(         show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from blanks_uec ; ");         query_cnt->next();     ui->cnt_blanks_uec_base->setText(         show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_insure_errors ; ");  query_cnt->next();     ui->cnt_spr_insure_errors_base->setText(  show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_oksm ; ");           query_cnt->next();     ui->cnt_spr_oksm_base->setText(           show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_ocato ; ");          query_cnt->next();     ui->cnt_spr_ocato_base->setText(          show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_ocato_full ; ");     query_cnt->next();     ui->cnt_spr_ocato_full_base->setText(     show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from addresses ; ");          query_cnt->next();     ui->cnt_addresses_base->setText(          show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from assig_by_terr ; ");      query_cnt->next();     ui->cnt_assig_by_terr_base->setText(      show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from persons_assig ; ");      query_cnt->next();     ui->cnt_persons_assig_base->setText(      show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_assig_errors ; ");   query_cnt->next();     ui->cnt_spr_assig_errors_base->setText(   show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medorgs ; ");        query_cnt->next();     ui->cnt_spr_medorgs_base->setText(        show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medparts ; ");       query_cnt->next();     ui->cnt_spr_medparts_base->setText(       show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medparts ; ");       query_cnt->next();     ui->cnt_spr_medparts_base->setText(       show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medsites ; ");       query_cnt->next();     ui->cnt_spr_medsites_base->setText(       show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medters ; ");        query_cnt->next();     ui->cnt_spr_medters_base->setText(        show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_ms_r_mt ; ");        query_cnt->next();     ui->cnt_spr_ms_r_mt_base->setText(        show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medsites_types ; "); query_cnt->next();     ui->cnt_spr_medsites_types_base->setText( show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medters_spec ; ");   query_cnt->next();     ui->cnt_spr_medters_spec_base->setText(   show_num(query_cnt->value(0).toInt()));

    old_db_drvr = data_app.old_db_drvr;
    old_db_host = data_app.old_db_host;
    old_db_name = data_app.old_db_name;
    old_db_port = data_app.old_db_port;
    old_db_user = data_app.old_db_user;
    old_db_pass = data_app.old_db_pass;

    ui->combo_db_drvr->setCurrentIndex(ui->combo_db_drvr->findText(data_app.old_db_drvr));
    ui->line_db_host->setText(data_app.old_db_host);
    ui->line_db_port->setText(data_app.old_db_port);
    ui->line_db_name->setText(data_app.old_db_name);
    ui->line_db_user->setText(data_app.old_db_user);
    ui->line_db_pass->setText(data_app.old_db_pass);

    try_connect_old_Base();
}

bool old_db_wnd::try_connect_old_Base() {
    QApplication::processEvents();
    // ------------------------------------------------ //
    // откроем новое подключение к внешней базе полисов //
    // ------------------------------------------------ //
    db_oldBase = QSqlDatabase::addDatabase(ui->combo_db_drvr->currentText(), "old Database");

    db_oldBase.setConnectOptions();
    // сервер базы полисов
    if (ui->line_db_host->text()!="polises_host")
        db_oldBase.setHostName(ui->line_db_host->text());
    else db_oldBase.setHostName(data_app.host);
    // порт базы полисов
    if (ui->combo_db_drvr->currentText()!="QIBASE")
        if (!ui->line_db_port->text().isEmpty())
            db_oldBase.setPort(ui->line_db_port->text().toInt());
    // имя базы полисов
    if (!ui->line_db_name->text().isEmpty())
        db_oldBase.setDatabaseName(ui->line_db_name->text());
    // имя пользователя
    if (!ui->line_db_user->text().isEmpty())
        db_oldBase.setUserName(ui->line_db_user->text());
    // пароль пользователя
    if (!ui->line_db_pass->text().isEmpty())
        db_oldBase.setPassword(ui->line_db_pass->text());

    if (!db_oldBase.open()) {
        // подключение не удалось
        ui->bn_load_tfoms->setEnabled(false);
        ui->bn_PersonsOld->setEnabled(false);
        ui->bn_clear_links->setEnabled(false);
        ui->bn_load_polises->setEnabled(false);
        ui->bn_reload_addres->setEnabled(false);
        ui->bn_close_polises->setEnabled(false);
        ui->bn_load_addresses->setEnabled(false);

        QMessageBox::critical(this,"Ошибка подключения к внешней базе полисов",
                              QString("ВНИМАНИЕ!!!\n\n") +
                              "При попытке подключиться к внешней базе полисов произошла ошибка.\n"
                              "Позвоните мне по телефону \n\n"
                              "8-920-449-54-88\n\n"
                              "Александр\n\n" +
                              "old_db_driv = " + ui->combo_db_drvr->currentText() + "\n" +
                              "old_db_host = " + ui->line_db_host->text() + "\n" +
                              "old_db_port = " + ui->line_db_port->text() + "\n" +
                              "old_db_name = " + ui->line_db_name->text() + "\n" +
                              "old_db_user = " + ui->line_db_user->text() + "\n" +
                              "old_db_pass = " + ui->line_db_pass->text() + "\n");
    } else {
        ui->bn_close->setEnabled(true);
        ui->push_connect->setEnabled(true);
        ui->bn_load_tfoms->setEnabled(true);
        ui->bn_PersonsOld->setEnabled(true);
        ui->bn_clear_links->setEnabled(true);
        ui->bn_load_polises->setEnabled(true);
        ui->bn_reload_addres->setEnabled(true);
        ui->bn_close_polises->setEnabled(true);
        ui->bn_load_addresses->setEnabled(true);

        old_db_drvr = ui->combo_db_drvr->currentText();
        old_db_host = ui->line_db_host->text();
        old_db_name = ui->line_db_name->text();
        old_db_port = ui->line_db_port->text();
        old_db_user = ui->line_db_user->text();
        old_db_pass = ui->line_db_pass->text();

        QApplication::processEvents();

        QMessageBox::information(this,"Внешняя база полисов доступна",
                                 QString("Доступ к внешней базе полисов получен.\n"));
    }
    if (db_oldBase.isOpen()) {
        // подключение готово
        return true;
    } else {
        return false;
    }
}

old_db_wnd::~old_db_wnd() {
    delete ui;
}

void old_db_wnd::on_bn_close_clicked() {
    close();
}

void old_db_wnd::on_push_connect_clicked() {
    if (try_connect_old_Base()) {
    }
}

void old_db_wnd::on_combo_db_drvr_activated(int index) {
    if (ui->combo_db_drvr->currentText()=="QIBASE") {
        ui->line_db_host->setText("172.20.250.20");
        ui->line_db_port->setText("");
        ui->line_db_name->setText("inko");
        ui->line_db_user->setText("SYSDBA");
        ui->line_db_pass->setText("masterkey");

    } else if (ui->combo_db_drvr->currentText()=="QPSQL") {
        ui->line_db_host->setText("");
        ui->line_db_port->setText("");
        ui->line_db_name->setText("");
        ui->line_db_user->setText("");
        ui->line_db_pass->setText("");
    }
}


void old_db_wnd::on_bn_clear_links_clicked() {
    ui->pan_wait->setVisible(true);
    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  Удаление устаревших ссылок в старой базе");
    QApplication::processEvents();

    bool res_load = try_clear_links();
    if (!res_load) {
        ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  действие отменено!");
        QMessageBox::warning(this, "Действие отменено", "Действие отменено.\n");
    } else if (!ui->ch_process_all->isChecked()) {
        QMessageBox::information(this, "Готово", "Все устаревшие ссылки удалены.\n");
    }
    ui->pan_wait->setVisible(false);
    QApplication::processEvents();
}

bool old_db_wnd::try_clear_links() {
    ui->lay_load_table->setText("POLICES_import (дохлые ссылки)");
    ui->bn_close->setEnabled(false);
    ui->push_connect->setEnabled(false);
    ui->bn_load_tfoms->setEnabled(false);
    ui->bn_PersonsOld->setEnabled(false);
    ui->bn_clear_links->setEnabled(false);
    ui->bn_load_polises->setEnabled(false);
    ui->bn_reload_addres->setEnabled(false);
    ui->bn_close_polises->setEnabled(false);
    ui->bn_load_addresses->setEnabled(false);
    ui->cnt_polises_close->setVisible(true);
    QApplication::processEvents();

    date0 = QDate::currentDate();
    time0 = QTime::currentTime();

    // подготовка
    date = QDate::currentDate();
    time = QTime::currentTime();
    ui->lab_time_all->setText("00:00:00");
    ui->lab_time->setText("00:00:00");
    QApplication::processEvents();

    int cnt_max = 2;

    if (ui->rb_10->isChecked())  cnt_max = 20;
    else if (ui->rb_100->isChecked())  cnt_max = 200;
    else if (ui->rb_1000->isChecked())  cnt_max = 2000;
    else if (ui->rb_10000->isChecked())  cnt_max = 20000;
    else if (ui->rb_100000->isChecked())  cnt_max = 200000;
    else if (ui->rb_1000000->isChecked())  cnt_max = 2000000;
    else if (ui->rb_hand->isChecked())  cnt_max = ui->spin_hand->value();

    // ------------------------------------------------------ //
    // удалим старые ссылки на записи - закрытые в новой базе //
    // ------------------------------------------------------ //

    db_oldBase.transaction();
    db.transaction();

    QString sql_get;
    sql_get =  // закрывшиеся записи, которые есть в пуле импортированных
               "select distinct e.id "
               "  from persons e "
               " where e.status <= 0"
               "   and e.id is not NULL ; ";


    // выберем порцию данных
    QSqlQuery *query_get = new QSqlQuery(db);
    bool res_get = query_get->exec(sql_get);

    if (!res_get) {
        QMessageBox::warning(this, "Ошибка чтения из таблицы PERSONS",
                             "При попытке прочитать данные персон без действующего полиса из таблицы PERSONS произошла ошибка.\n\n"
                             "Удаление устаревших ссылок во внешенй базе отменено!");
        db_oldBase.rollback();
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->push_connect->setEnabled(true);
        ui->bn_load_tfoms->setEnabled(true);
        ui->bn_PersonsOld->setEnabled(true);
        ui->bn_clear_links->setEnabled(true);
        ui->bn_load_polises->setEnabled(true);
        ui->bn_reload_addres->setEnabled(true);
        ui->bn_close_polises->setEnabled(true);
        ui->bn_load_addresses->setEnabled(true);
        return false;
    }

    QString sql_clr;
    sql_clr = "delete from POLICES_IMPORT_IDS ; ";
    QSqlQuery *query_clr = new QSqlQuery(db_oldBase);
    bool res_clr = query_clr->exec(sql_clr);
    delete query_clr;

    __int64 id;
    QString sql_set;

    int n = 0;
    QSqlQuery *query_set = new QSqlQuery(db_oldBase);
    while (query_get->next()) {
        //------------------//
        // прочитаем данные //
        //------------------//
        id = query_get->value( 0).toInt();

        sql_set =  // добавим ID в таблицу
                   "insert into POLICES_IMPORT_IDS (id)"
                   "values (" + QString::number(id) + ") ; ";

        // выберем порцию данных
        bool res_set = query_set->exec(sql_set);
        n++;

        if (n%2341==0) {
            // отображение статуса на экран
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

            ui->cnt_clear_links->setText("~" + show_num(n) + "~");
            QApplication::processEvents();
        }
    }
    delete query_set;
    ui->cnt_clear_links->setText("~" + show_num(n) + "~");
    QApplication::processEvents();
    db_oldBase.commit();

    // удаляем из таблицы импорта записи, для котрых нет актуальных записей застрахованных в базе INKO_POLISES_VORONEJ
    QString sql_use;
    sql_use = "delete from POLICES_IMPORT "
              " where id_person in(select id from POLICES_IMPORT_IDS) ; ";
    db_oldBase.transaction();
    QSqlQuery *query_use = new QSqlQuery(db_oldBase);
    bool res_use = query_use->exec(sql_use);
    delete query_use;
    db_oldBase.commit();


    // ----------------------------------------------- //
    // удалим ссылки на полисы, закрытые в старой базе //
    // ----------------------------------------------- //

    db_oldBase.transaction();
    db.transaction();

    sql_get =  // открытые записи, которых нет в пуле импортированных
               "select distinct e.id, p.id, bp.id, p.vs_num, p.uec_num "
               "  from persons e "
               "  left join polises p on(p.id=e.id_polis) "
               "  left join blanks_pol bp on(bp.id_polis=p.id) "
               " where e.status > 0 ; ";

    // выберем порцию данных
    res_get = query_get->exec(sql_get);

    if (!res_get) {
        QMessageBox::warning(this, "Ошибка чтения из таблицы PERSONS",
                             "При попытке прочитать данные персон с действующим полисом из таблицы PERSONS произошла ошибка.\n\n"
                             "Удаление устаревших ссылок во внешенй базе отменено!");
        db_oldBase.rollback();
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->push_connect->setEnabled(true);
        ui->bn_load_tfoms->setEnabled(true);
        ui->bn_PersonsOld->setEnabled(true);
        ui->bn_clear_links->setEnabled(true);
        ui->bn_load_polises->setEnabled(true);
        ui->bn_reload_addres->setEnabled(true);
        ui->bn_close_polises->setEnabled(true);
        ui->bn_load_addresses->setEnabled(true);
        return false;
    }

    /**/
    sql_clr = "delete from POLICES_IMPORT_IDS_FULL ; ";
    db_oldBase.transaction();
    query_clr = new QSqlQuery(db_oldBase);
    res_clr = query_clr->exec(sql_clr);
    db_oldBase.commit();
    delete query_clr;

    n = 0;
    __int64 id_person;
    __int64 id_polis;
    __int64 id_blank_pol;
    QString vs_num;
    QString uec_num;

    query_set = new QSqlQuery(db_oldBase);
    while (query_get->next()) {
        //------------------//
        // прочитаем данные //
        //------------------//
        id_person    = query_get->value( 0).toInt();
        id_polis     = query_get->value( 1).toInt();
        id_blank_pol = query_get->value( 2).toInt();
        vs_num       = query_get->value( 3).toString();
        uec_num      = query_get->value( 4).toString();

        sql_set =  // добавим ID в таблицу
                   "insert into POLICES_IMPORT_IDS_FULL (id_person, id_polis, id_blank_pol, vs_num, uec_num) "
                   "values ( " + QString::number(id_person) +
                         " , " + QString::number(id_polis) +
                         " , " + QString::number(id_blank_pol) +
                         " ,'" + vs_num  + "' "
                         " ,'" + uec_num + "' ) ; ";

        // выберем порцию данных
        bool res_set = query_set->exec(sql_set);
        n++;

        if (n%2341==0) {
            // отображение статуса на экран
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

            ui->cnt_clear_links_2->setText("~" + show_num(n) + "~");
            QApplication::processEvents();
        }
    }
    ui->cnt_clear_links_2->setText("~" + show_num(n) + "~");
    QApplication::processEvents();
    delete query_set;
    delete query_get;
    db_oldBase.commit();
    /**/

    // удаляем ссылки, для которых нет действующего полиса в базе полисов
    QString sql_cl2;
    sql_cl2 = "delete from polices_import i "
              "  where i.id_person in "
              "             ( select distinct pif.id_person "
              "                 from polices_import_ids_full pif "
              "                 left join polices_import pi on(pi.id_polis=pif.id_polis) "
              "                 left join polices p on(p.pol_id=pi.pol_id and p.pr_izm=2) "
              "                where p.pol_id is NULL ) ; ";

    QSqlQuery *query_cl2 = new QSqlQuery(db_oldBase);
    bool res_cl2 = query_cl2->exec(sql_cl2);
    delete query_cl2;

    // отметим на перезагрузку записей, для которых нет записей действующих полисов в базе полисов
    QString sql_chk;
    sql_chk = "insert into polices_import(pol_id, id_person, id_polis, id_blank_vs, id_blank_pol, id_blank_uec, flag) "
              "select distinct (-1)*pif.id_person, pif.id_person, pif.id_polis, pif.vs_num, pif.id_blank_pol, pif.uec_num, -1 "
              "  from polices_import_ids_full pif "
              "  left join polices_import pi on(pi.id_polis=pif.id_polis) "
              "  left join polices p on(p.pol_id=pi.pol_id and p.pr_izm=2) "
              " where p.pol_id is NULL ; ";

    QSqlQuery *query_chk = new QSqlQuery(db_oldBase);
    bool res_chk = query_chk->exec(sql_chk);
    delete query_chk;


    /*
    QString sql_add;
    QSqlQuery *query_add = new QSqlQuery(db_oldBase);
    __int64 pol_id;
    while (query_chk->next()) {
        pol_id        = query_chk->value(0).toInt();
        id_person     = query_chk->value(1).toInt();
        id_polis      = query_chk->value(2).toInt();
        id_blank_pol  = query_chk->value(3).toInt();
        vs_num        = query_chk->value(4).toString();
        uec_num       = query_chk->value(5).toString();

        sql_add = "insert into polices_import (pol_id, id_person, id_polis, id_blank_pol, id_blank_vs, id_blank_uec, flag) "
                  "values ( " + QString::number(pol_id)       + ", "
                          " " + QString::number(id_person)    + ", "
                          " " + QString::number(id_polis)     + ", "
                          " " + QString::number(id_blank_pol) + ", "
                          " " + vs_num                        + ", "
                          " " + uec_num                       + " ) ; ";
        res_chk = query_add->exec(sql_add);
        n++;
    }
    delete query_add;
    */

    delete query_chk;
    db_oldBase.commit();

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  Готово (" + QString::number(n) + " строк) !");
    if (ui->ch_process_all->isChecked())
         on_bn_close_polises_clicked();
    else QMessageBox::information(this, "Готово",
                                  "Все устаревшие ссылки удалены.\n\n"
                                  "Обработано " + QString::number(n) + " строк. ");





    ui->lay_load_table->setText("  - // -  ");
    ui->bn_close->setEnabled(true);
    ui->push_connect->setEnabled(true);
    ui->bn_load_tfoms->setEnabled(true);
    ui->bn_PersonsOld->setEnabled(true);
    ui->bn_clear_links->setEnabled(true);
    ui->bn_load_polises->setEnabled(true);
    ui->bn_reload_addres->setEnabled(true);
    ui->bn_close_polises->setEnabled(true);
    ui->bn_load_addresses->setEnabled(true);
    return true;
}


void old_db_wnd::on_bn_close_polises_clicked() {
    ui->pan_wait->setVisible(true);
    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  Закрытие старых полисов");
    QApplication::processEvents();

    bool res_close = try_close_polises();
    if (!res_close) {
        ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  действие отменено!");
        QMessageBox::warning(this, "Действие отменено", "Действие отменено.\n");
    } else if (!ui->ch_process_all->isChecked()) {
        QMessageBox::information(this, "Готово", "Все старые полисы закрыты.\n");
    }
    ui->pan_wait->setVisible(false);
    QApplication::processEvents();
}

bool old_db_wnd::try_close_polises() {
    ui->lay_load_table->setText("POLICES (закрытые полисы)");
    ui->bn_close->setEnabled(false);
    ui->push_connect->setEnabled(false);
    ui->bn_load_tfoms->setEnabled(false);
    ui->bn_PersonsOld->setEnabled(false);
    ui->bn_clear_links->setEnabled(false);
    ui->bn_load_polises->setEnabled(false);
    ui->bn_reload_addres->setEnabled(false);
    ui->bn_close_polises->setEnabled(false);
    ui->bn_load_addresses->setEnabled(false);
    ui->cnt_polises_close->setVisible(true);
    QApplication::processEvents();

    date0 = QDate::currentDate();
    time0 = QTime::currentTime();


    // подсчитаем существующие записи
    QSqlQuery *query_cnt = new QSqlQuery(db);
    query_cnt->exec("select count(*) from persons ; ");            query_cnt->next();     ui->cnt_persons_base->setText(            show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from persons_docs ; ");       query_cnt->next();     ui->cnt_persons_docs_base->setText(       show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from persons_old ; ");        query_cnt->next();     ui->cnt_persons_old_base->setText(        show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from vizits ; ");             query_cnt->next();     ui->cnt_vizits_base->setText(             show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from events ; ");             query_cnt->next();     ui->cnt_events_base->setText(             show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from polises ; ");            query_cnt->next();     ui->cnt_polises_base->setText(            show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from blanks_vs ; ");          query_cnt->next();     ui->cnt_blanks_vs_base->setText(          show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from blanks_pol ; ");         query_cnt->next();     ui->cnt_blanks_pol_base->setText(         show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from blanks_uec ; ");         query_cnt->next();     ui->cnt_blanks_uec_base->setText(         show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_insure_errors ; ");  query_cnt->next();     ui->cnt_spr_insure_errors_base->setText(  show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_oksm ; ");           query_cnt->next();     ui->cnt_spr_oksm_base->setText(           show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_ocato ; ");          query_cnt->next();     ui->cnt_spr_ocato_base->setText(          show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_ocato_full ; ");     query_cnt->next();     ui->cnt_spr_ocato_full_base->setText(     show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from addresses ; ");          query_cnt->next();     ui->cnt_addresses_base->setText(          show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from assig_by_terr ; ");      query_cnt->next();     ui->cnt_assig_by_terr_base->setText(      show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from persons_assig ; ");      query_cnt->next();     ui->cnt_persons_assig_base->setText(      show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_assig_errors ; ");   query_cnt->next();     ui->cnt_spr_assig_errors_base->setText(   show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medorgs ; ");        query_cnt->next();     ui->cnt_spr_medorgs_base->setText(        show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medparts ; ");       query_cnt->next();     ui->cnt_spr_medparts_base->setText(       show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medparts ; ");       query_cnt->next();     ui->cnt_spr_medparts_base->setText(       show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medsites ; ");       query_cnt->next();     ui->cnt_spr_medsites_base->setText(       show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medters ; ");        query_cnt->next();     ui->cnt_spr_medters_base->setText(        show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_ms_r_mt ; ");        query_cnt->next();     ui->cnt_spr_ms_r_mt_base->setText(        show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medsites_types ; "); query_cnt->next();     ui->cnt_spr_medsites_types_base->setText( show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medters_spec ; ");   query_cnt->next();     ui->cnt_spr_medters_spec_base->setText(   show_num(query_cnt->value(0).toInt()));

    // подготовка
    date = QDate::currentDate();
    time = QTime::currentTime();
    ui->lab_time_all->setText("00:00:00");
    ui->lab_time->setText("00:00:00");
    QApplication::processEvents();

    int cnt_max = 2;

    if (ui->rb_10->isChecked())  cnt_max = 20;
    else if (ui->rb_100->isChecked())  cnt_max = 200;
    else if (ui->rb_1000->isChecked())  cnt_max = 2000;
    else if (ui->rb_10000->isChecked())  cnt_max = 20000;
    else if (ui->rb_100000->isChecked())  cnt_max = 200000;
    else if (ui->rb_1000000->isChecked())  cnt_max = 2000000;
    else if (ui->rb_hand->isChecked())  cnt_max = ui->spin_hand->value();

    ui->lab_persons->setEnabled(false);
    ui->lab_persons_docs->setEnabled(false);
    ui->lab_persons_old->setEnabled(false);
    ui->lab_vizits->setEnabled(false);
    ui->lab_events->setEnabled(false);
    ui->lab_polises->setEnabled(false);
    ui->lab_blanks_vs->setEnabled(false);
    ui->lab_blanks_pol->setEnabled(false);
    ui->lab_blanks_uec->setEnabled(false);
    ui->lab_spr_insure_errors->setEnabled(false);
    ui->lab_spr_oksm->setEnabled(false);
    ui->lab_spr_ocato->setEnabled(false);
    ui->lab_spr_ocato_full->setEnabled(false);
    ui->lab_addresses->setEnabled(false);
    ui->lab_assig_by_terr->setEnabled(false);
    ui->lab_persons_assig->setEnabled(false);
    ui->lab_spr_assig_errors->setEnabled(false);
    ui->lab_spr_medorgs->setEnabled(false);
    ui->lab_spr_medparts->setEnabled(false);
    ui->lab_spr_medsites->setEnabled(false);
    ui->lab_spr_medters->setEnabled(false);
    ui->lab_spr_ms_r_mt->setEnabled(false);
    ui->lab_spr_medsites_types->setEnabled(false);
    ui->lab_spr_medters_spec->setEnabled(false);

    ui->cnt_persons->setEnabled(false);
    ui->cnt_persons_docs->setEnabled(false);
    ui->cnt_persons_old->setEnabled(false);
    ui->cnt_vizits->setEnabled(false);
    ui->cnt_events->setEnabled(false);
    ui->cnt_polises->setEnabled(false);
    ui->cnt_polises_close->setEnabled(false);
    ui->cnt_blanks_vs->setEnabled(false);
    ui->cnt_blanks_pol->setEnabled(false);
    ui->cnt_blanks_uec->setEnabled(false);
    ui->cnt_spr_insure_errors->setEnabled(false);
    ui->cnt_spr_oksm->setEnabled(false);
    ui->cnt_spr_ocato->setEnabled(false);
    ui->cnt_spr_ocato_full->setEnabled(false);
    ui->cnt_addresses->setEnabled(false);
    ui->cnt_assig_by_terr->setEnabled(false);
    ui->cnt_persons_assig->setEnabled(false);
    ui->cnt_spr_assig_errors->setEnabled(false);
    ui->cnt_spr_medorgs->setEnabled(false);
    ui->cnt_spr_medparts->setEnabled(false);
    ui->cnt_spr_medsites->setEnabled(false);
    ui->cnt_spr_medters->setEnabled(false);
    ui->cnt_spr_ms_r_mt->setEnabled(false);
    ui->cnt_spr_medsites_types->setEnabled(false);
    ui->cnt_spr_medters_spec->setEnabled(false);

    // запрос данных
    // Как показала практика, самое узкое место -  это чтение данных из FIrebird  Поэтому читаем ОЧЕНЬ большими порциями

    // найдём новые закрытые полисы
    QString sql;
    sql =  // закрывшиеся записи, которые есть в пуле импортированных
           "select first " + QString::number(cnt_max) + " "
           "       p.pol_id, pi.id_person, pi.id_polis, pi.id_blank_vs, pi.id_blank_pol, pi.id_blank_uec "
           "  from polices_import pi "
           "  left join polices p on(pi.pol_id=p.pol_id) "
           " where ( p.pr_izm<>2 and pi.pol_id is not null ) ; ";

    // заготовим переменные
    //__int64 pol_id, id_person, id_polis, id_blank_vs, id_blank_pol;
    //QString id_blank_uec;
    int n = 0;
    QList<s_data_close> lst_data_close;


    db_oldBase.transaction();
    db.transaction();

    QSqlQuery *query = new QSqlQuery(db_oldBase);
    bool res;

    // выберем порцию данных
    res = query->exec(sql);

    if (!res) {
        QMessageBox::warning(this, "Ошибка чтения из таблицы POLICES",
                             "При попытке прочитать очередную порцию данных из таблицы POLICES произошла неожиданная ошибка.\n\n"
                             "Загрузка данных из внешней базы остановлена!");
        db_oldBase.rollback();
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->push_connect->setEnabled(true);
        ui->bn_load_tfoms->setEnabled(true);
        ui->bn_PersonsOld->setEnabled(true);
        ui->bn_clear_links->setEnabled(true);
        ui->bn_load_polises->setEnabled(true);
        ui->bn_reload_addres->setEnabled(true);
        ui->bn_close_polises->setEnabled(true);
        ui->bn_load_addresses->setEnabled(true);
        return false;
    }

    lst_data_close.clear();

    while (query->next()) {
        //------------------//
        // прочитаем данные //
        //------------------//
        s_data_close data;
        data.pol_id        = query->value( 0).toInt();
        data.id_person     = query->value( 1).toInt();
        data.id_polis      = query->value( 2).toInt();
        data.id_blank_vs   = query->value( 3).toInt();
        data.id_blank_pol  = query->value( 4).toInt();
        data.id_blank_uec  = query->value( 5).toString();
        lst_data_close.append(data);
    }
    delete query;


    // обновим транзакции
    db_oldBase.commit();
    db.commit();
    db_oldBase.transaction();
    db.transaction();


    // найдём устаревшие свызки
    sql =  // закрывшиеся записи, которые есть в пуле импортированных
           "select first " + QString::number(cnt_max) + " "
           "       -1, pi.id_person, pi.id_polis, pi.id_blank_vs, pi.id_blank_pol, pi.id_blank_uec "
           "  from polices_import pi "
           " where pol_id<0 "
           "    or flag<0 ; ";

    QSqlQuery *query2 = new QSqlQuery(db_oldBase);
    res = query2->exec(sql);

    if (!res) {
        QMessageBox::warning(this, "Ошибка чтения из таблицы POLICES",
                             "При попытке прочитать очередную порцию данных из таблицы POLICES произошла неожиданная ошибка.\n\n"
                             "Загрузка данных из внешней базы остановлена!");
        db_oldBase.rollback();
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->push_connect->setEnabled(true);
        ui->bn_load_tfoms->setEnabled(true);
        ui->bn_PersonsOld->setEnabled(true);
        ui->bn_clear_links->setEnabled(true);
        ui->bn_load_polises->setEnabled(true);
        ui->bn_reload_addres->setEnabled(true);
        ui->bn_close_polises->setEnabled(true);
        ui->bn_load_addresses->setEnabled(true);
        return false;
    }

    QString err0 = query2->lastError().driverText();
    QString err1 = query2->lastError().databaseText();

    while (query2->next()) {
        //------------------//
        // прочитаем данные //
        //------------------//
        s_data_close data;
        data.pol_id        = query2->value( 0).toInt();
        data.id_person     = query2->value( 1).toInt();
        data.id_polis      = query2->value( 2).toInt();
        data.id_blank_vs   = query2->value( 3).toInt();
        data.id_blank_pol  = query2->value( 4).toInt();
        data.id_blank_uec  = query2->value( 5).toString();
        lst_data_close.append(data);
    }

    delete query2;


    // покажем число полисов на закрытие
    int cnt = lst_data_close.count();
    ui->cnt_close_polises_base->setText(show_num(cnt));
    QApplication::processEvents();


    foreach (s_data_close data, lst_data_close) {
        //---------------------------------------//
        // закроем полис, обновим статус персоны //
        //---------------------------------------//
        QString sql_polis;
        sql_polis = "update polises "
                    "   set date_stop = CURRENT_DATE "
                    " where id=" + QString::number(data.id_polis) + " "
                    "   and ( date_stop is NULL or date_stop>CURRENT_DATE ) ; "

                    "update blanks_pol "
                    "   set status = -5 "
                    " where ( id=" + QString::number(data.id_blank_pol) + " or id_polis=" + QString::number(data.id_polis) + " ) "
                    "   and status>=0 ; "

                    "update blanks_vs "
                    "   set status = -5 "
                    " where ( vs_num=" + QString::number(data.id_blank_vs) + " or id_polis=" + QString::number(data.id_polis) + " ) "
                    "   and status>=0 ; "

                    "update blanks_uec "
                    "   set status = -5 "
                    " where ( uec_num='" + data.id_blank_uec + "' or id_polis=" + QString::number(data.id_polis) + " ) "
                    "   and status>=0 ; ";


        QSqlQuery *query_polis = new QSqlQuery(db);
        bool res_polis = query_polis->exec(sql_polis);

        if (!res_polis) {
            QMessageBox::warning(this, "Ошибка закрытия полиса",
                                 "При попытке закрыть полис произошла неожиданная ошибка.\n\n"
                                 "n = " + QString::number(n) + "\n\n"
                                 + query_polis->lastError().driverText() + "\n" + query_polis->lastError().databaseText() + "\n\n"
                                 "Запись пропущена.");
            delete query_polis;
            db_oldBase.rollback();
            db.rollback();

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_load_tfoms->setEnabled(true);
            ui->bn_PersonsOld->setEnabled(true);
            ui->bn_clear_links->setEnabled(true);
            ui->bn_load_polises->setEnabled(true);
            ui->bn_reload_addres->setEnabled(true);
            ui->bn_close_polises->setEnabled(true);
            ui->bn_load_addresses->setEnabled(true);
            return false;
        }
        delete query_polis;


        QString sql_person;
        sql_person = "select update_pers_links_pol(" + QString::number(data.id_person) + ", CURRENT_DATE) ; ";
        QSqlQuery *query_person = new QSqlQuery(db);
        bool res_person = query_person->exec(sql_person);

        if (!res_person) {
            QMessageBox::warning(this, "Ошибка обновления статуса персоны",
                                 "При попытке обновить статаус персоны произошла неожиданная ошибка.\n\n"
                                 "n = " + QString::number(n) + "\n\n"
                                 + query_person->lastError().driverText() + "\n" + query_person->lastError().databaseText() + "\n\n"
                                 "Запись пропущена.");
            delete query_person;
            db_oldBase.rollback();
            db.rollback();

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_load_tfoms->setEnabled(true);
            ui->bn_PersonsOld->setEnabled(true);
            ui->bn_clear_links->setEnabled(true);
            ui->bn_load_polises->setEnabled(true);
            ui->bn_reload_addres->setEnabled(true);
            ui->bn_close_polises->setEnabled(true);
            ui->bn_load_addresses->setEnabled(true);
            return false;
        }
        cnt_polises_close++;
        delete query_person;


        //------------------------------------------------//
        // запомним во внешней базе факт обработки записи //
        //------------------------------------------------//
        QString sql_link;
        sql_link  = "delete from polices_import "
                    " where (" + QString::number(data.pol_id)       + ">0  and  pol_id="       + QString::number(data.pol_id)       + ") "
                    "    or (" + QString::number(data.id_polis)     + ">0  and  id_polis="     + QString::number(data.id_polis)     + ") "
                    "    or (" + QString::number(data.id_blank_pol) + ">0  and  id_blank_pol=" + QString::number(data.id_blank_pol) + ") "
                    "    or (" + QString::number(data.id_blank_vs)  + ">0  and  id_blank_vs="  + QString::number(data.id_blank_vs)  + ") ; ";
        QSqlQuery *query_link = new QSqlQuery(db_oldBase);
        bool res_link = query_link->exec(sql_link);

        if (!res_link) {
            delete query_link;
            // без проверок
        }

        delete query_link;

        n++;

        if ( (n%21==0)
             || n==cnt_max-1 ) {
            // переоткроем транзакции
            db_oldBase.commit();
            db.commit();
            db_oldBase.transaction();
            db.transaction();

            // отображение статуса на экран
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


            ui->cnt_close_polises->setText(show_num(0-n));
            ui->cnt_persons->setText(show_num(0-cnt_persons));
            ui->cnt_persons_docs->setText(show_num(0-cnt_persons_docs));
            ui->cnt_persons_old->setText(show_num(0-cnt_persons_old));
            ui->cnt_vizits->setText(show_num(0-cnt_vizits));
            ui->cnt_events->setText(show_num(0-cnt_events));
            ui->cnt_polises->setText(show_num(0-cnt_polises));
            ui->cnt_polises_close->setText(show_num(0-cnt_polises_close) + ", ");
            ui->cnt_blanks_vs->setText(show_num(0-cnt_blanks_vs));
            ui->cnt_blanks_pol->setText(show_num(0-cnt_blanks_pol));
            ui->cnt_blanks_uec->setText(show_num(0-cnt_blanks_uec));
            ui->cnt_spr_insure_errors->setText(show_num(0-cnt_spr_insure_errors));
            ui->cnt_spr_oksm->setText(show_num(0-cnt_spr_oksm));
            ui->cnt_spr_ocato->setText(show_num(0-cnt_spr_ocato));
            ui->cnt_spr_ocato_full->setText(show_num(0-cnt_spr_ocato_full));
            ui->cnt_addresses->setText(show_num(0-cnt_addresses));
            ui->cnt_assig_by_terr->setText(show_num(0-cnt_assig_by_terr));
            ui->cnt_persons_assig->setText(show_num(0-cnt_persons_assig));
            ui->cnt_spr_assig_errors->setText(show_num(0-cnt_spr_assig_errors));
            ui->cnt_spr_medorgs->setText(show_num(0-cnt_spr_medorgs));
            ui->cnt_spr_medparts->setText(show_num(0-cnt_spr_medparts));
            ui->cnt_spr_medsites->setText(show_num(0-cnt_spr_medsites));
            ui->cnt_spr_medters->setText(show_num(0-cnt_spr_medters));
            ui->cnt_spr_ms_r_mt->setText(show_num(0-cnt_spr_ms_r_mt));
            ui->cnt_spr_medsites_types->setText(show_num(0-cnt_spr_medsites_types));
            ui->cnt_spr_medters_spec->setText(show_num(0-cnt_spr_medters_spec));

            ui->lab_persons->setEnabled(0<cnt_persons);
            ui->lab_polises->setEnabled(0<cnt_polises);
            ui->lab_blanks_vs->setEnabled(0<cnt_blanks_vs);
            ui->lab_blanks_pol->setEnabled(0<cnt_blanks_pol);
            ui->lab_blanks_uec->setEnabled(0<cnt_blanks_uec);

            ui->cnt_persons->setEnabled(0<cnt_persons);
            ui->cnt_polises->setEnabled(0<cnt_polises);
            ui->cnt_polises_close->setEnabled(0<cnt_polises_close);
            ui->cnt_blanks_vs->setEnabled(0<cnt_blanks_vs);
            ui->cnt_blanks_pol->setEnabled(0<cnt_blanks_pol);
            ui->cnt_blanks_uec->setEnabled(0<cnt_blanks_uec);

            QApplication::processEvents();
        }
    }
    int cnt_all = n;

    if (cnt_all<cnt_max)
        // все записи уже обработаны
        ui->bn_close_polises->setVisible(false);

    ui->cnt_close_polises->setText(show_num(0-n));

    // закроем посделние транзакции
    db_oldBase.commit();
    db.commit();

    lst_data_close.clear();

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  Готово (" + QString::number(n) + " строк) !");
    if (ui->ch_process_all->isChecked())
         on_bn_load_polises_clicked();
    else QMessageBox::information(this, "Готово", "Все старые полисы закрыты.\n\nОбработано " + QString::number(n) + " строк. ");


    ui->lay_load_table->setText("  - // -  ");
    ui->bn_close->setEnabled(true);
    ui->push_connect->setEnabled(true);
    ui->bn_load_tfoms->setEnabled(true);
    ui->bn_PersonsOld->setEnabled(true);
    ui->bn_clear_links->setEnabled(true);
    ui->bn_load_polises->setEnabled(true);
    ui->bn_reload_addres->setEnabled(true);
    ui->bn_close_polises->setEnabled(true);
    ui->bn_load_addresses->setEnabled(true);
    return true;
}


void old_db_wnd::on_bn_load_polises_clicked() {
    ui->pan_wait->setVisible(true);
    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  Загрузка новых полисов");
    QApplication::processEvents();

    bool res_load = try_load_polises();
    if (!res_load) {
        ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  действие отменено!");
        QMessageBox::warning(this, "Действие отменено", "Действие отменено.\n");
    } else if (!ui->ch_process_all->isChecked()) {
        QMessageBox::information(this, "Готово", "Все новые полисы загружены.\n");
    }
    ui->pan_wait->setVisible(false);
    QApplication::processEvents();
}

bool old_db_wnd::try_load_polises() {
    ui->lay_load_table->setText("POLICES");
    ui->bn_close->setEnabled(false);
    ui->push_connect->setEnabled(false);
    ui->bn_load_tfoms->setEnabled(false);
    ui->bn_PersonsOld->setEnabled(false);
    ui->bn_clear_links->setEnabled(false);
    ui->bn_load_polises->setEnabled(false);
    ui->bn_reload_addres->setEnabled(false);
    ui->bn_close_polises->setEnabled(false);
    ui->bn_load_addresses->setEnabled(false);
    QApplication::processEvents();

    date0 = QDate::currentDate();
    time0 = QTime::currentTime();

    // подсчитаем существующие записи
    QSqlQuery *query_cnt = new QSqlQuery(db);
    query_cnt->exec("select count(*) from persons ; ");            query_cnt->next();     ui->cnt_persons_base->setText(            show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from persons_docs ; ");       query_cnt->next();     ui->cnt_persons_docs_base->setText(       show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from persons_old ; ");        query_cnt->next();     ui->cnt_persons_old_base->setText(        show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from vizits ; ");             query_cnt->next();     ui->cnt_vizits_base->setText(             show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from events ; ");             query_cnt->next();     ui->cnt_events_base->setText(             show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from polises ; ");            query_cnt->next();     ui->cnt_polises_base->setText(            show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from blanks_vs ; ");          query_cnt->next();     ui->cnt_blanks_vs_base->setText(          show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from blanks_pol ; ");         query_cnt->next();     ui->cnt_blanks_pol_base->setText(         show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from blanks_uec ; ");         query_cnt->next();     ui->cnt_blanks_uec_base->setText(         show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_insure_errors ; ");  query_cnt->next();     ui->cnt_spr_insure_errors_base->setText(  show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_oksm ; ");           query_cnt->next();     ui->cnt_spr_oksm_base->setText(           show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_ocato ; ");          query_cnt->next();     ui->cnt_spr_ocato_base->setText(          show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_ocato_full ; ");     query_cnt->next();     ui->cnt_spr_ocato_full_base->setText(     show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from addresses ; ");          query_cnt->next();     ui->cnt_addresses_base->setText(          show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from assig_by_terr ; ");      query_cnt->next();     ui->cnt_assig_by_terr_base->setText(      show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from persons_assig ; ");      query_cnt->next();     ui->cnt_persons_assig_base->setText(      show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_assig_errors ; ");   query_cnt->next();     ui->cnt_spr_assig_errors_base->setText(   show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medorgs ; ");        query_cnt->next();     ui->cnt_spr_medorgs_base->setText(        show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medparts ; ");       query_cnt->next();     ui->cnt_spr_medparts_base->setText(       show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medparts ; ");       query_cnt->next();     ui->cnt_spr_medparts_base->setText(       show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medsites ; ");       query_cnt->next();     ui->cnt_spr_medsites_base->setText(       show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medters ; ");        query_cnt->next();     ui->cnt_spr_medters_base->setText(        show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_ms_r_mt ; ");        query_cnt->next();     ui->cnt_spr_ms_r_mt_base->setText(        show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medsites_types ; "); query_cnt->next();     ui->cnt_spr_medsites_types_base->setText( show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medters_spec ; ");   query_cnt->next();     ui->cnt_spr_medters_spec_base->setText(   show_num(query_cnt->value(0).toInt()) );

    // подготовка
    date = QDate::currentDate();
    time = QTime::currentTime();
    ui->lab_time_all->setText("00:00:00");
    ui->lab_time->setText("00:00:00");
    QApplication::processEvents();

    int cnt_max = 2;
    if (ui->rb_10->isChecked())  cnt_max = 20;
    else if (ui->rb_100->isChecked())  cnt_max = 200;
    else if (ui->rb_1000->isChecked())  cnt_max = 2000;
    else if (ui->rb_10000->isChecked())  cnt_max = 20000;
    else if (ui->rb_100000->isChecked())  cnt_max = 200000;
    else if (ui->rb_1000000->isChecked())  cnt_max = 2000000;
    else if (ui->rb_hand->isChecked())  cnt_max = ui->spin_hand->value();

    ui->lab_persons->setEnabled(false);
    ui->lab_persons_docs->setEnabled(false);
    ui->lab_persons_old->setEnabled(false);
    ui->lab_vizits->setEnabled(false);
    ui->lab_events->setEnabled(false);
    ui->lab_polises->setEnabled(false);
    ui->lab_blanks_vs->setEnabled(false);
    ui->lab_blanks_pol->setEnabled(false);
    ui->lab_blanks_uec->setEnabled(false);
    ui->lab_spr_insure_errors->setEnabled(false);
    ui->lab_spr_oksm->setEnabled(false);
    ui->lab_spr_ocato->setEnabled(false);
    ui->lab_spr_ocato_full->setEnabled(false);
    ui->lab_addresses->setEnabled(false);
    ui->lab_assig_by_terr->setEnabled(false);
    ui->lab_persons_assig->setEnabled(false);
    ui->lab_spr_assig_errors->setEnabled(false);
    ui->lab_spr_medorgs->setEnabled(false);
    ui->lab_spr_medparts->setEnabled(false);
    ui->lab_spr_medsites->setEnabled(false);
    ui->lab_spr_medters->setEnabled(false);
    ui->lab_spr_ms_r_mt->setEnabled(false);
    ui->lab_spr_medsites_types->setEnabled(false);
    ui->lab_spr_medters_spec->setEnabled(false);

    ui->cnt_persons->setEnabled(false);
    ui->cnt_persons_docs->setEnabled(false);
    ui->cnt_persons_old->setEnabled(false);
    ui->cnt_vizits->setEnabled(false);
    ui->cnt_events->setEnabled(false);
    ui->cnt_polises->setEnabled(false);
    ui->cnt_blanks_vs->setEnabled(false);
    ui->cnt_blanks_pol->setEnabled(false);
    ui->cnt_blanks_uec->setEnabled(false);
    ui->cnt_spr_insure_errors->setEnabled(false);
    ui->cnt_spr_oksm->setEnabled(false);
    ui->cnt_spr_ocato->setEnabled(false);
    ui->cnt_spr_ocato_full->setEnabled(false);
    ui->cnt_addresses->setEnabled(false);
    ui->cnt_assig_by_terr->setEnabled(false);
    ui->cnt_persons_assig->setEnabled(false);
    ui->cnt_spr_assig_errors->setEnabled(false);
    ui->cnt_spr_medorgs->setEnabled(false);
    ui->cnt_spr_medparts->setEnabled(false);
    ui->cnt_spr_medsites->setEnabled(false);
    ui->cnt_spr_medters->setEnabled(false);
    ui->cnt_spr_ms_r_mt->setEnabled(false);
    ui->cnt_spr_medsites_types->setEnabled(false);
    ui->cnt_spr_medters_spec->setEnabled(false);

    ui->lay_load_table->setText("POLICES\nПодсчёт числа записей");
    QApplication::processEvents();

    QString sql_prep;
    // проверим, есть ли что грузить
    sql_prep = "select COUNT(*) "
               "  from polices  p "
               "  left join polices_import pi on(pi.pol_id=p.pol_id) "
               " where #ONLY_FRESH# "          // временное ограничение - чтобы обработать последние записи
               "   and pr_izm=2 "
               "   and ((p.action_event not in('П021', 'П022', 'П023', 'П024', 'П025')) or (p.action_event is NULL) ) "
               "   and pi.pol_id is NULL ; ";

    sql_prep = sql_prep.replace("#ONLY_FRESH#", ui->ch_process_new->isChecked() ? "p.pol_id>10000000" : "1=1");

    QSqlQuery *query_prep = new QSqlQuery(db_oldBase);
    bool res_prep = query_prep->exec(sql_prep);
    query_prep->next();
    int cnt_prep = query_prep->value(0).toInt();
    delete query_prep;

    if (cnt_prep<1 && !ui->ch_process_all->isChecked()) {
        QMessageBox::information(this, "Нечего грузить",
                                 "В таблице полисов нет ни одной новой записи.\n\n"
                                 "Операция отменена.");

        // все записи уже обработаны
        ui->bn_load_polises->setVisible(false);

        ui->bn_close->setEnabled(true);
        ui->push_connect->setEnabled(true);
        ui->bn_load_tfoms->setEnabled(true);
        ui->bn_PersonsOld->setEnabled(true);
        ui->bn_clear_links->setEnabled(true);
        ui->bn_load_polises->setEnabled(true);
        ui->bn_reload_addres->setEnabled(true);
        ui->bn_close_polises->setEnabled(true);
        ui->bn_load_addresses->setEnabled(true);
        return true;
    }


    // покажем число новых полисов на экране
    ui->cnt_load_polises_base->setText(show_num(cnt_prep));
    QApplication::processEvents();


    ui->lay_load_table->setText("POLICES\nGET data from FIREBIRD");
    QApplication::processEvents();

    // запрос данных
    QString sql;
    // Как показала практика, самое узкое место -
    // это чтение данных из FIrebird
    // Поэтому читаем ОЧЕНЬ большими порциями
    sql  =  "select first " + QString::number(cnt_max) + " "
            "       p.pol_id, p.fam, p.im, p.ot, p.dat_r, coalesce(p.birthplace, p.birthplace_sity, p.birthplace_country) as plc_birth, p.pol, p.snils, p.enp, "
            "       coalesce(oksm.kod, '643') as oksm_c, coalesce(oks1.kod, '643') as oksm_birth, "
            "       p.contact_phone as phone, p.contact_phone_home as phone_home, p.contact_phone_cellular as phone_cell, p.contact_phone_work as phone_work, p.email, "
            "       p.n_papki as id_folder, "
            // старые данные
            "       p.fam_old, p.im_old, p.ot_old, p.dat_r_old, p.doc_type_id_old, p.ser_doc_old, p.nom_doc_old, p.date_issue_old, p.mr_old, "
            // адреса
            "       subj.adr_id as subj, subj.naim as subj_name, subj.kladr_code as subj_kladr, "
            "       dstr.adr_id as dstr, dstr.naim as dstr_name, dstr.kladr_code as dstr_kladr, "
            "       nasp.adr_id as nasp, nasp.naim as nasp_name, nasp.kladr_code as nasp_kladr, "
            "       strt.adr_id as strt, strt.naim as strt_name, strt.kladr_code as strt_kladr, "
            "       p.dom as house, p.korp as corp, p.kv as quart, p.postal_code as post_index, "
            "       coalesce(p.date_registration, p.temp_registr_beg, p.dat_r) as date_reg, p.bomj, "
            "       subj1.adr_id as subj_1, subj1.naim as subj_1_name, subj1.kladr_code as subj_1_kladr, "
            "       dstr1.adr_id as dstr_1, dstr1.naim as dstr_1_name, dstr1.kladr_code as dstr_1_kladr, "
            "       nasp1.adr_id as nasp_1, nasp1.naim as nasp_1_name, nasp1.kladr_code as nasp_1_kladr, "
            "       strt1.adr_id as strt_1, strt1.naim as strt_1_name, strt1.kladr_code as strt_1_kladr, "
            "       p.dom_1 as house_1, p.korp_1 as corp_1, p.kv_1 as quart_1, p.postal_code_1 as post_index_1, "
            // документы
            "       p.doc_type_id as doc_type, p.ser_doc as doc_ser, p.nom_doc as doc_num, p.date_issue as doc_date, p.podrazdcode as org_code, p.podr_issue as doc_org, NULL as doc_exp, "
            "       p.doc_type_id_1 as doc_type_1, p.ser_doc_1 as doc_ser_1, p.nom_doc_1 as doc_num_1, p.date_issue_1 as doc_date_1, NULL as org_code_1, p.podr_issue_1 as doc_org_1, p.temp_registr_end as doc_exp_1, "
            // визит
            "       p.fillreason as r_smo, p.fillreason_ps as r_polis, "
            // полис
            "       p.oms_rights as pol_v, p.temp_number as vs_num, p.dat_vp as date_begin, p.date_end, p.dat_ip as date_stop, "
            "       p.ser_pol, p.nom_pol, p.blank_ser, p.blank_num, "
            "       substring(bn.blank_num from 1 for 4) as blank_ser2, substring(bn.blank_num from 5 for 7) as blank_num2, "
            // событие
            "       p.action_event as event_code, "
            // ФИАС
            "       p.fias_aoid_addressg, p.fias_aohouseid_addressg, p.fias_aoid_addressp, p.fias_aohouseid_addressp, "
            // представитель
            "       p.agent, p.agent_ser_doc, p.agent_num_doc, p.agent_date_issue, p.agent_phone_cellular, p.agent_phone_home, p.agent_phone_work, "
            "       p.workstatus,"
            "       pi.flag "
            "  from polices p "
            "  left join blank_nums bn on(p.last_num = bn.temp_num) "
            "  left join polices_import pi on(pi.pol_id=p.pol_id) "
            "  left join spr_country oksm on(oksm.name=p.citizenship) "
            "  left join spr_country oks1 on(oks1.name=p.birthplace_country) "
            "  left join adres_dict subj on(subj.adr_id=p.kod_t  and subj.obj_id=1) "
            "  left join adres_dict dstr on(dstr.adr_id=p.kod_r  and dstr.obj_id=2) "
            "  left join adres_dict nasp on(nasp.adr_id=p.nas_p  and nasp.obj_id=3) "
            "  left join adres_dict strt on(strt.adr_id=p.kod_ul and strt.obj_id=4) "
            "  left join adres_dict subj1 on(subj1.adr_id=p.kod_t_1  and subj1.obj_id=1) "
            "  left join adres_dict dstr1 on(dstr1.adr_id=p.kod_r_1  and dstr1.obj_id=2) "
            "  left join adres_dict nasp1 on(nasp1.adr_id=p.nas_p_1  and nasp1.obj_id=3) "
            "  left join adres_dict strt1 on(strt1.adr_id=p.kod_ul_1 and strt1.obj_id=4) "
            " where #ONLY_FRESH# "          // временное ограничение - чтобы обработать последние записи
            "   and ( p.pr_izm=2 "          // ещё не учтённый действующий полис
            "      or pi.flag>0 ) "         // полис с изменением данных
            "   and ( (pi.pol_id is NULL) or (pi.flag>0) ) "
            //"   and p.pol_id=10008381 "
            //"   and p.enp='2398389729000961' "
            "   and ( (p.action_event not in('П021', 'П022', 'П023', 'П024', 'П025')) or (p.action_event is NULL) ) "
            " order by p.pol_id ; ";

    sql = sql.replace("#ONLY_FRESH#", ui->ch_process_new->isChecked() ? "p.pol_id>10000000" : "1=1");

    // заготовим переменные
    int pol_id;
    QString fam, im, ot;
    QString fam_norm, im_norm, ot_norm;
    QDate dat_r;
    QString plc_birth, pol;
    int sex;
    QString snils, enp, citizenship, birthplace_country;
    int oksm_birth, oksm_c;
    QString phone, phone_home, phone_cell, phone_work, email;
    int id_folder;
    int category;
    // старые данные
    QString fam_old, im_old, ot_old, plc_birth_old;
    QDate dat_r_old;
    int doc_type_id_old;
    QString ser_doc_old, nom_doc_old;
    QDate date_issue_old;
    // адреса
    int subj, dstr, nasp, strt, house, subj_1, dstr_1, nasp_1, strt_1, house_1, bomj;
    QString subj_name, dstr_name, nasp_name, strt_name, corp, quart, post_index, subj_1_name, dstr_1_name, nasp_1_name, strt_1_name, corp_1, quart_1, post_index_1, addr_reg_text, addr_liv_text;
    QString subj_kladr, dstr_kladr, nasp_kladr, strt_kladr, subj_1_kladr, dstr_1_kladr, nasp_1_kladr, strt_1_kladr;
    QString subj_s, dstr_s, city_s, nasp_s, strt_s, subj_1_s, dstr_1_s, city_1_s, nasp_1_s, strt_1_s;
    QDate date_reg;
    //QString fias_aiod, fias_houseid;
    // документы
    int doc_type, doc_type_1;
    QString doc_ser, doc_num, doc_ser_1, doc_num_1;
    QDate doc_date, doc_date_1, doc_exp, doc_exp_1;
    QString org_code, doc_org, org_code_1, doc_org_1;
    // визит
    int r_smo, r_polis, f_polis, v_method;
    // полис
    QString oms_rights;
    int pol_v, pol_f;
    int vs_num;
    QString uec_num;
    QString blank_ser,  blank_num;
    QString blank_ser2, blank_num2;
    QString pol_ser, pol_num;
    QDate date_begin, date_end, date_stop;
    // событие
    QString event_code;
    // ФИАС
    QString fias_aoid_r, fias_houseid_r, fias_aoid_l, fias_houseid_l;
    // представлитель
    int rep_type;
    QString rep_doc_ser, rep_doc_num;
    QDate rep_doc_date;
    QString rep_phone_cell, rep_phone_home, rep_phone_work;

    __int64 id_addr_reg, id_addr_liv, id_person, id_UDL, id_DRP, id_polis, id_vizit, id_event, id_blank_vs, id_blank_pol;
    QString id_blank_uec;
    int flag;
    bool has_flag;


    db_oldBase.transaction();
    db.transaction();

    // выберем порцию данных
    QSqlQuery *query = new QSqlQuery(db_oldBase);
    bool res = query->exec(sql);

    if (!res) {
        QMessageBox::warning(this, "Ошибка чтения из таблицы POLICES",
                             "При попытке прочитать очередную порцию данных из таблицы POLICES произошла неожиданная ошибка.\n\n"
                             "Загрузка данных из внешней базы остановлена!");
        db_oldBase.rollback();
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->push_connect->setEnabled(true);
        ui->bn_load_tfoms->setEnabled(true);
        ui->bn_PersonsOld->setEnabled(true);
        ui->bn_clear_links->setEnabled(true);
        ui->bn_load_polises->setEnabled(true);
        ui->bn_reload_addres->setEnabled(true);
        ui->bn_close_polises->setEnabled(true);
        ui->bn_load_addresses->setEnabled(true);
        return false;
    }

    ui->lay_load_table->setText("POLICES\nДобавление данных");
    QApplication::processEvents();

    int n = 0;
    while (query->next()) {
        /* вот так некрасиво
        if (n<1000) {
            n++;
            continue;
        }*/

        //------------------//
        // прочитаем данные //
        //------------------//
        pol_id        = query->value( 0).toInt();
        fam           = query->value( 1).toString().trimmed().simplified().toUpper();
        im            = query->value( 2).toString().trimmed().simplified().toUpper().replace("'","\"");
        ot            = query->value( 3).toString().trimmed().simplified().toUpper().replace("'","\"");
        dat_r         = query->value( 4).toDate();
        plc_birth     = query->value( 5).toString().trimmed().simplified().replace("'","\"");
        pol           = query->value( 6).toString().trimmed();
        sex           = (pol.left(1)=="М" ? 1 : 2);
        snils         = query->value( 7).toString().trimmed().replace(" ", "").replace("-", "").replace("'","\"");
        enp           = query->value( 8).toString().trimmed().replace(" ", "").replace("-", "").replace("'","\"");
        oksm_c        = query->value( 9).toInt();
        oksm_birth    = query->value(10).toInt();
        phone         = query->value(11).toString().trimmed().replace(" ","").replace("+7","8").replace("(","").replace(")","").replace("-","").replace("'","\"");
        phone_home    = query->value(12).toString().trimmed().replace(" ","").replace("+7","8").replace("(","").replace(")","").replace("-","").replace("'","\"");
        phone_cell    = query->value(13).toString().trimmed().replace(" ","").replace("+7","8").replace("(","").replace(")","").replace("-","").replace("'","\"");
        phone_work    = query->value(14).toString().trimmed().replace(" ","").replace("+7","8").replace("(","").replace(")","").replace("-","").replace("'","\"");
        email         = query->value(15).toString().trimmed().simplified().replace("'","\"");
        id_folder     = query->value(16).toInt();
        // старые данные
        fam_old       = query->value(17).toString().trimmed().simplified().replace("'","\"");
        im_old        = query->value(18).toString().trimmed().simplified().replace("'","\"");
        ot_old        = query->value(19).toString().trimmed().simplified().replace("'","\"");
        dat_r_old     = query->value(20).toDate();
        doc_type_id_old = query->value(21).toInt();
        ser_doc_old   = query->value(22).toString().trimmed().simplified().replace("'","\"");
        nom_doc_old   = query->value(23).toString().trimmed().simplified().replace("'","\"");
        date_issue_old= query->value(24).toDate();
        plc_birth_old = query->value(25).toString().trimmed().simplified().replace("'","\"");
        // адрес регистрации
        subj          = query->value(26).toInt();
        subj_name     = query->value(27).toString().trimmed().simplified().replace("'","\"");
        subj_kladr    = query->value(28).toString().trimmed().simplified().replace("'","\"");
        dstr          = query->value(29).toInt();
        dstr_name     = query->value(30).toString().trimmed().simplified().replace("'","\"");
        dstr_kladr    = query->value(31).toString().trimmed().simplified().replace("'","\"");
        nasp          = query->value(32).toInt();
        nasp_name     = query->value(33).toString().trimmed().simplified().replace("'","\"");
        nasp_kladr    = query->value(34).toString().trimmed().simplified().replace("'","\"");
        strt          = query->value(35).toInt();
        strt_name     = query->value(36).toString().trimmed().simplified().replace("'","\"");
        strt_kladr    = query->value(37).toString().trimmed().simplified().replace("'","\"");
        house         = query->value(38).toInt();
        corp          = query->value(39).toString().trimmed().simplified().replace("'","\"");
        quart         = query->value(40).toString().trimmed().simplified().replace("'","\"");
        addr_reg_text = subj_name + ", " + dstr_name + ", " + nasp_name + ", " + strt_name + ", " + QString::number(house) + " " + corp + (quart>0 ? (", кв. " + quart) : "");
        // сформируем коды КЛАДР из выделенных компонентов
        subj_s = (!subj_kladr.isEmpty() ? subj_kladr.left(2)   : (!dstr_kladr.isEmpty() ? dstr_kladr.left(2)  : (!nasp_kladr.isEmpty() ? nasp_kladr.left(2)  : (!strt_kladr.isEmpty() ? strt_kladr.left(2) : "00"))));
        dstr_s = (!dstr_kladr.isEmpty() ? dstr_kladr.mid(2,3)  : (!nasp_kladr.isEmpty() ? nasp_kladr.mid(2,3) : (!strt_kladr.isEmpty() ? strt_kladr.mid(2,3) : "000")));
        city_s = (!nasp_kladr.isEmpty() ? nasp_kladr.mid(5,3)  : (!strt_kladr.isEmpty() ? strt_kladr.mid(5,3) : "000"));
        nasp_s = (!nasp_kladr.isEmpty() ? nasp_kladr.mid(8,3)  : (!strt_kladr.isEmpty() ? strt_kladr.mid(8,3) : "000"));
        strt_s = (!strt_kladr.isEmpty() ? strt_kladr.mid(11,4) : "0000");
        subj_kladr = subj_s + "000"  + "000"  + "000"  + "00";
        dstr_kladr = subj_s + dstr_s + "000"  + "000"  + "00";
        nasp_kladr = subj_s + dstr_s + city_s + nasp_s + "00";
        strt_kladr = subj_s + dstr_s + city_s + nasp_s + strt_s + "00";

        post_index    = query->value(41).toString().trimmed().simplified().replace("'","\"");
        date_reg      = query->value(42).toDate();
        bomj          = query->value(43).toInt();
        // адрес проживания
        subj_1        = query->value(44).toInt();
        subj_1_name   = query->value(45).toString().trimmed().simplified().replace("'","\"");
        subj_1_kladr  = query->value(46).toString().trimmed().simplified().replace("'","\"");
        dstr_1        = query->value(47).toInt();
        dstr_1_name   = query->value(48).toString().trimmed().simplified().replace("'","\"");
        dstr_1_kladr  = query->value(49).toString().trimmed().simplified().replace("'","\"");
        nasp_1        = query->value(50).toInt();
        nasp_1_name   = query->value(51).toString().trimmed().simplified().replace("'","\"");
        nasp_1_kladr  = query->value(52).toString().trimmed().simplified().replace("'","\"");
        strt_1        = query->value(53).toInt();
        strt_1_name   = query->value(54).toString().trimmed().simplified().replace("'","\"");
        strt_1_kladr  = query->value(55).toString().trimmed().simplified().replace("'","\"");
        house_1       = query->value(56).toInt();
        corp_1        = query->value(57).toString().trimmed().simplified().replace("'","\"");
        quart_1       = query->value(58).toString().trimmed().simplified().replace("'","\"");
        addr_liv_text = subj_1_name + ", " + dstr_1_name + ", " + nasp_1_name + ", " + strt_1_name + ", " + QString::number(house_1) + " " + corp_1 + (quart_1>0 ? (", кв. " + quart_1) : "");
        // сформируем коды КЛАДР_1 из выделенных компонентов
        subj_1_s = (!subj_1_kladr.isEmpty() ? subj_1_kladr.left(2)   : (!dstr_1_kladr.isEmpty() ? dstr_1_kladr.left(2)  : (!nasp_1_kladr.isEmpty() ? nasp_1_kladr.left(2)  : (!strt_1_kladr.isEmpty() ? strt_1_kladr.left(2) : "00"))));
        dstr_1_s = (!dstr_1_kladr.isEmpty() ? dstr_1_kladr.mid(2,3)  : (!nasp_1_kladr.isEmpty() ? nasp_1_kladr.mid(2,3) : (!strt_1_kladr.isEmpty() ? strt_1_kladr.mid(2,3) : "000")));
        city_1_s = (!nasp_1_kladr.isEmpty() ? nasp_1_kladr.mid(5,3)  : (!strt_1_kladr.isEmpty() ? strt_1_kladr.mid(5,3) : "000"));
        nasp_1_s = (!nasp_1_kladr.isEmpty() ? nasp_1_kladr.mid(8,3)  : (!strt_1_kladr.isEmpty() ? strt_1_kladr.mid(8,3) : "000"));
        strt_1_s = (!strt_1_kladr.isEmpty() ? strt_1_kladr.mid(11,4) : "0000");
        subj_1_kladr = subj_1_s + "000"    + "000"    + "000"    + "00";
        dstr_1_kladr = subj_1_s + dstr_1_s + "000"    + "000"    + "00";
        nasp_1_kladr = subj_1_s + dstr_1_s + city_1_s + nasp_1_s + "00";
        strt_1_kladr = subj_1_s + dstr_1_s + city_1_s + nasp_1_s + strt_1_s + "00";

        post_index_1  = query->value(59).toString().trimmed().simplified().replace("'","\"");
        // документы
        doc_type      = query->value(60).toInt();
        doc_ser       = query->value(61).toString().trimmed().simplified().replace("'","\"");
        doc_num       = query->value(62).toString().trimmed().simplified().replace("'","\"");
        doc_date      = query->value(63).toDate();
        org_code      = query->value(64).toString().trimmed().simplified().replace("'","\"");
        doc_org       = query->value(65).toString().trimmed().simplified().replace("'","\"");
        doc_exp       = query->value(66).toDate();
        doc_type_1    = query->value(67).toInt();
        doc_ser_1     = query->value(68).toString().trimmed().simplified().replace("'","\"");
        doc_num_1     = query->value(69).toString().trimmed().simplified().replace("'","\"");
        doc_date_1    = query->value(70).toDate();
        org_code_1    = query->value(71).toString().trimmed().simplified().replace("'","\"");
        doc_org_1     = query->value(72).toString().trimmed().simplified().replace("'","\"");
        doc_exp_1     = query->value(73).toDate();
        // визит
        r_smo         = query->value(74).toInt();
        r_polis       = query->value(75).toInt();
        // полис
        oms_rights    = query->value(76).toString().trimmed().simplified().replace("'","\"");
        if (oms_rights=="С")  pol_v = 1;  else if (oms_rights=="В")  pol_v = 2;  else pol_v = 3;
        vs_num        = query->value(77).toInt();
        date_begin    = query->value(78).toDate();
        date_end      = query->value(79).toDate();
        date_stop     = query->value(80).toDate();
        pol_ser       = query->value(81).toString().trimmed().simplified().replace("'","\"");
        pol_num       = query->value(82).toString().trimmed().simplified().replace("'","\"");
        blank_ser     = query->value(83).toString().trimmed().simplified().replace("'","\"");
        blank_num     = query->value(84).toString().trimmed().simplified().replace("'","\"");
        blank_ser2    = query->value(85).toString().trimmed().simplified().replace("'","\"");
        blank_num2    = query->value(86).toString().trimmed().simplified().replace("'","\"");
        // событие
        event_code    = query->value(87).toString().trimmed().simplified().replace("'","\"");
        // ФИАС
        fias_aoid_r   = query->value(88).toString().trimmed().simplified().replace(" ","").replace("'","\"");
        fias_houseid_r= query->value(89).toString().trimmed().simplified().replace(" ","").replace("'","\"");
        fias_aoid_l   = query->value(90).toString().trimmed().simplified().replace(" ","").replace("'","\"");
        fias_houseid_l= query->value(91).toString().trimmed().simplified().replace(" ","").replace("'","\"");
        // представитель
        rep_type      = query->value(92).toInt();
        rep_doc_ser   = query->value(93).toString().trimmed().simplified().replace("'","\"");
        rep_doc_num   = query->value(94).toString().trimmed().simplified().replace("'","\"");
        rep_doc_date  = query->value(95).toDate();
        rep_phone_home= query->value(96).toString().trimmed().replace(" ","").replace("+7","8").replace("(","").replace(")","").replace("-","").replace("'","\"");
        rep_phone_cell= query->value(97).toString().trimmed().replace(" ","").replace("+7","8").replace("(","").replace(")","").replace("-","").replace("'","\"");
        rep_phone_work= query->value(98).toString().trimmed().replace(" ","").replace("+7","8").replace("(","").replace(")","").replace("-","").replace("'","\"");
        category      = query->value(99).toInt();
        flag          = query->value(100).toInt();
        has_flag      =!query->value(100).isNull();

        // если такая запись в POLICES_IMPORT уже есть ...
        if (has_flag || flag!=0) {
            // ... удалим старую запись из POLICES_IMPORT
            QString sql_del;
            sql_del = "delete from polices_import "
                      " where (" + QString::number(pol_id)   + ">0  and  pol_id="   + QString::number(pol_id)   + ") "
                      "    or (" + QString::number(id_polis) + ">0  and  id_polis=" + QString::number(id_polis) + ") ; ";
            QSqlQuery *query_del = new QSqlQuery(db_oldBase);
            bool res_del = query_del->exec(sql_del);
            if (!res_del) {
                //delete query_link;
                // без проверок
            }
            delete query_del;
        }

        //-----------//
        // коррекция //
        //-----------//
        fam_norm      = fam;
        im_norm       = im;
        ot_norm       = ot;
        fam_norm      = fam_norm.toUpper().replace("Ё","Е");
        im_norm       = im_norm.toUpper().replace("Ё","Е");
        ot_norm       = ot_norm.toUpper().replace("Ё","Е");
        if (oms_rights.isEmpty())
            oms_rights = "С";
        if (!dat_r.isValid())
            dat_r = QDate(2000,1,1);
        if (!date_reg.isValid())
            date_reg = dat_r;
        if (!snils.isEmpty())
            snils = QString("00000000000" + snils).right(11);
        if (!enp.isEmpty())
            enp = QString("0000000000000000" + enp).right(16);
        if (!phone.isEmpty()) {
            phone = clear_num(phone);
            phone = QString(QString("84730000000").left(11 - phone.length()) + phone).left(20);
        }
        if (!phone_cell.isEmpty()){
            phone_cell = clear_num(phone_cell);
            phone_cell = QString(QString("84730000000").left(11 - phone_cell.length()) + phone_cell).left(20);
        }
        if (!phone_home.isEmpty()){
            phone_home = clear_num(phone_home);
            phone_home = QString(QString("84730000000").left(11 - phone_home.length()) + phone_home).left(20);
        }
        if (!phone_work.isEmpty()){
            phone_work = clear_num(phone_work);
            phone_work = QString(QString("84730000000").left(11 - phone_work.length()) + phone_work).left(20);
        }
        if (doc_type>0 && !doc_date.isValid()) {
            doc_date = dat_r;
        }
        if (oms_rights=="С") {
            blank_ser = pol_ser;
            blank_num = pol_num;
            event_code = "-//-";
        }
        if (blank_ser.isEmpty() || blank_ser=="0000")
            blank_ser = blank_ser2;
        if (blank_num.isEmpty())
            blank_num = blank_num2;
        if (blank_num.length()<7 and blank_num.length()>0)
            blank_num = QString("0000000" + blank_num).right(7);
        if (blank_ser.isEmpty() || blank_ser=="0000")
            blank_ser = "X" + QString("...." + pol_ser).right(3);
        if (blank_num.isEmpty())
            blank_num = "X" + QString("......." + pol_num).right(7);

        if (oms_rights!="С" && blank_ser.length()<4)
            blank_ser = QString("0000" + blank_ser).right(4);

        pol_f = (oms_rights=="Э" ? 2 : 1);
        if (blank_num.length()>17) {
            oms_rights = "К";
            uec_num = blank_num;
            blank_ser = "";
            blank_num = "";
        } else
            uec_num = "";
        f_polis = ( oms_rights=="Э" ? 2 : 1 );
        if (!fias_aoid_r.isEmpty())
            fias_aoid_r = QString("00000000-0000-0000-0000-000000000000").left(36 - fias_aoid_r.length()) + fias_aoid_r;
        if (!fias_houseid_r.isEmpty())
            fias_houseid_r = QString("00000000-0000-0000-0000-000000000000").left(36 - fias_houseid_r.length()) + fias_houseid_r;
        if (!fias_aoid_l.isEmpty())
            fias_aoid_l = QString("00000000-0000-0000-0000-000000000000").left(36 - fias_aoid_l.length()) + fias_aoid_l;
        if (!fias_houseid_l.isEmpty())
            fias_houseid_l = QString("00000000-0000-0000-0000-000000000000").left(36 - fias_houseid_l.length()) + fias_houseid_l;

        id_addr_reg = -1;
        id_addr_liv = -1;
        id_person = -1;
        id_polis = -1;
        id_vizit = -1;
        id_event = -1;
        id_UDL = -1;
        id_DRP = -1;
        id_blank_vs = -1;
        id_blank_pol = -1;
        id_blank_uec = "";

        //------------------------------ //
        // коррекция кодов КЛАДР по ФИАС //
        //------------------------------ //
        if (!fias_aoid_r.isEmpty()) {
            // попробуем найти данные КЛАДР по базе ФИАС
            QString sql_fias_aoid_r;
            sql_fias_aoid_r  = "select code "
                               "  from addrobj "
                               " where aoid = cast('" + fias_aoid_r + "' as uuid) "
                               " limit 1 ; ";
            QSqlQuery *query_fias_aoid_r = new QSqlQuery(db_FIAS);
            bool res_fias_aoid_r = query_fias_aoid_r->exec(sql_fias_aoid_r);
            if (query_fias_aoid_r->next()) {
                QString code = query_fias_aoid_r->value(0).toString();
                if (!code.isEmpty()) {
                    QString region_code = code.mid( 0, 2);
                    QString area_code   = code.mid( 2, 3);
                    QString city_code   = code.mid( 5, 3);
                    QString place_code  = code.mid( 8, 3);
                    QString street_code = code.mid(11, 4);
                    QString act_code    = code.mid(15, 2);
                    // получим коды по КЛАДР и названия
                    subj_kladr = region_code + "000000000" + act_code;
                    dstr_kladr = region_code + area_code + "000000" + act_code;
                    nasp_kladr = region_code + area_code + city_code + place_code + act_code;
                    strt_kladr = region_code + area_code + city_code + place_code + street_code +  act_code;
                }
            }
            delete query_fias_aoid_r;
        }

        if (!fias_aoid_l.isEmpty()) {
            // попробуем найти данные КЛАДР по базе ФИАС
            QString sql_fias_aoid_l;
            sql_fias_aoid_l  = "select code "
                               "  from addrobj "
                               " where aoid = cast('" + fias_aoid_l + "' as uuid) "
                               " limit 1 ; ";
            QSqlQuery *query_fias_aoid_l = new QSqlQuery(db_FIAS);
            bool res_fias_aoid_l = query_fias_aoid_l->exec(sql_fias_aoid_l);
            if (query_fias_aoid_l->next()) {
                QString code = query_fias_aoid_l->value(0).toString();
                if (!code.isEmpty()) {
                    QString region_code = code.mid( 0, 2);
                    QString area_code   = code.mid( 2, 3);
                    QString city_code   = code.mid( 5, 3);
                    QString place_code  = code.mid( 8, 3);
                    QString street_code = code.mid(11, 4);
                    QString act_code    = code.mid(15, 2);
                    // получим коды по КЛАДР и названия
                    subj_1_kladr = region_code + "000000000" + act_code;
                    dstr_1_kladr = region_code + area_code + "000000" + act_code;
                    nasp_1_kladr = region_code + area_code + city_code + place_code + act_code;
                    strt_1_kladr = region_code + area_code + city_code + place_code + street_code +  act_code;
                }
            }
            delete query_fias_aoid_l;
        }

        if ( house>0
             || (!corp.isEmpty()  && corp!="0"  && corp!="."  && corp!=".."  && corp!="-"  && corp!="," )
             || (!quart.isEmpty() && quart!="0" && quart!="++" && quart!="-" && quart!="--" && quart!="." && quart!="..." && quart!="/") ) {
            //----------------------------------//
            // найдём/добавим адрес регистрации //
            //----------------------------------//
            QString sql_addr_reg;
            sql_addr_reg  = "select add_or_find_address "
                            "   ( cast('" + subj_name        + "' as varchar), "
                            "     cast('" + subj_kladr       + "' as varchar), "
                            "     cast('" + dstr_name        + "' as varchar), "
                            "     cast('" + dstr_kladr       + "' as varchar), "
                            "     cast('" + nasp_name        + "' as varchar), "
                            "     cast('" + nasp_kladr       + "' as varchar), "
                            "     cast('" + strt_name        + "' as varchar), "
                            "     cast('" + strt_kladr       + "' as varchar), "
                            "     " + QString::number(house) + ", "
                            "     cast('" + corp             + "' as varchar), "
                            "     cast('" + quart            + "' as varchar), "
                            "     cast('" + post_index       + "' as varchar), "
                            "     cast(" + (fias_aoid_r.isEmpty() ? "NULL" : QString("'" + fias_aoid_r  + "'")) + " as uuid), "
                            "     cast(" + (fias_houseid_r.isEmpty() ? "NULL" : QString("'" + fias_houseid_r  + "'")) + " as uuid)) ; ";
            QSqlQuery *query_addr_reg = new QSqlQuery(db);
            bool res_addr_reg = query_addr_reg->exec(sql_addr_reg);
            query_addr_reg->next();

            id_addr_reg = query_addr_reg->value(0).toInt();
            if (id_addr_reg>0) {
                // найден готовый адрес
            } else if (id_addr_reg<0) {
                // добавлен новый адрес
                id_addr_reg = 0 - id_addr_reg;
                cnt_addresses++;
            } else {
                // !!! ошибка поиска/добавления записи !!!
                QMessageBox::warning(this, "Ошибка добавления адреса регистрации",
                                     "При попытке добавить запись адреса регистрации произошла неожиданная ошибка.\n\n"
                                     "n = " + QString::number(n) + "\n\n"
                                     + query_addr_reg->lastError().driverText() + "\n" + query_addr_reg->lastError().databaseText() + "\n\n"
                                     "Запись пропущена.");
                delete query_addr_reg;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            delete query_addr_reg;
        }


        if ( house_1>0
             || (!corp_1.isEmpty() and corp_1!="0")
             || (!quart_1.isEmpty() and quart!="-" and quart!="0") ) {
            //---------------------------------//
            // найдём/добавим адрес проживания //
            //---------------------------------//
            QString sql_addr_liv;
            sql_addr_liv  = "select add_or_find_address "
                            "   ( cast('" + subj_1_name        + "' as varchar), "
                            "     cast('" + subj_1_kladr       + "' as varchar), "
                            "     cast('" + dstr_1_name        + "' as varchar), "
                            "     cast('" + dstr_1_kladr       + "' as varchar), "
                            "     cast('" + nasp_1_name        + "' as varchar), "
                            "     cast('" + nasp_1_kladr       + "' as varchar), "
                            "     cast('" + strt_1_name        + "' as varchar), "
                            "     cast('" + strt_1_kladr       + "' as varchar), "
                            "     " + QString::number(house_1) + ", "
                            "     cast('" + corp_1             + "' as varchar), "
                            "     cast('" + quart_1            + "' as varchar), "
                            "     cast('" + post_index_1       + "' as varchar), "
                            "     cast(" + (fias_aoid_l.isEmpty() ? "NULL" : QString("'" + fias_aoid_l  + "'")) + " as uuid), "
                            "     cast(" + (fias_houseid_l.isEmpty() ? "NULL" : QString("'" + fias_houseid_l  + "'")) + " as uuid)) ; ";
            QSqlQuery *query_addr_liv = new QSqlQuery(db);
            bool res_addr_liv = query_addr_liv->exec(sql_addr_liv);
            query_addr_liv->next();

            id_addr_liv = query_addr_liv->value(0).toInt();
            if (id_addr_liv>0) {
                // найден готовый адрес
            } else if (id_addr_liv<0) {
                // добавлен новый адрес
                id_addr_liv = 0 - id_addr_liv;
                cnt_addresses++;
            } else {
                // !!! ошибка поиска/добавления записи !!!
                QMessageBox::warning(this, "Ошибка добавления адреса проживания",
                                     "При попытке добавить запись адреса проживания произошла неожиданная ошибка.\n\n"
                                     "n = " + QString::number(n) + "\n\n"
                                     + query_addr_liv->lastError().driverText() + "\n" + query_addr_liv->lastError().databaseText() + "\n\n"
                                     "Запись пропущена.");
                delete query_addr_liv;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            delete query_addr_liv;
        }


        //------------------------//
        // найдём/добавим персону //
        //------------------------//
        QString sql_person;
        sql_person  = "select add_or_find_person "
                      "   ( cast(" + QString::number(category) + " as smallint), "
                      "     cast('" + fam        + "' as varchar), "
                      "     cast('" + im         + "' as varchar), "
                      "     cast('" + ot         + "' as varchar), "
                      "     cast('" + fam_norm   + "' as varchar), "
                      "     cast('" + im_norm    + "' as varchar), "
                      "     cast('" + ot_norm    + "' as varchar), "
                      "     cast("  + (dat_r.isValid() ? QString("'" + dat_r.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                      "     cast('" + plc_birth  + "' as varchar), "
                      "     cast("  + QString::number(sex) + " as smallint), "
                      "     cast('" + snils      + "' as varchar), "
                      "     cast('" + enp        + "' as varchar), "
                      "     " + QString::number(oksm_c) + ", "
                      "     " + QString::number(oksm_birth) + ", "
                      "     cast('" + addr_reg_text + "' as varchar), "
                      "     cast('" + addr_liv_text + "' as varchar), "
                      "     cast('" + phone      + "' as varchar), "
                      "     cast('" + phone_cell + "' as varchar), "
                      "     cast('" + phone_home + "' as varchar), "
                      "     cast('" + phone_work + "' as varchar), "
                      "     cast('" + email      + "' as varchar), "
                      "     cast('" + date_reg.toString("yyyy-MM-dd") + "' as date), "
                      "     " + QString::number(id_addr_reg) + ", "
                      "     " + QString::number(id_addr_liv) + " ) ; ";
        QSqlQuery *query_person = new QSqlQuery(db);
        bool res_person = query_person->exec(sql_person);

        while ( !res_person
                && QMessageBox::warning( this, "Ошибка добавления персоны",
                                         "При попытке добавить запись персоны произошла неожиданная ошибка.\n\n"
                                         "n = " + QString::number(n) + "\n\n"
                                         + query_person->lastError().driverText() + "\n" + query_person->lastError().databaseText() + "\n\n"
                                         "Повторить?",
                                         QMessageBox::Retry|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel ) {
            res_person = query_person->exec(sql_person);
        }

        if (!res_person) {
            delete query_person;
            db_oldBase.rollback();
            db.rollback();

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_load_tfoms->setEnabled(true);
            ui->bn_PersonsOld->setEnabled(true);
            ui->bn_clear_links->setEnabled(true);
            ui->bn_load_polises->setEnabled(true);
            ui->bn_reload_addres->setEnabled(true);
            ui->bn_close_polises->setEnabled(true);
            ui->bn_load_addresses->setEnabled(true);
            return false;
        }
        query_person->next();

        id_person = query_person->value(0).toInt();
        if (id_person>0) {
            // найден готовая запись персоны
        } else if (id_person<0) {
            // добавлена новая запись персоны
            id_person = 0 - id_person;
            cnt_persons++;
        } else {
            // !!! ошибка поиска/добавления записи !!!
        }
        delete query_person;



        //---------------------------------------------//
        // обновим старые данные идертификации персоны //
        //---------------------------------------------//
        QString sql_find = "select max(id) "
                           "  from persons_old "
                           " where id_person=" + QString::number(id_person) + " ; ";
        QSqlQuery *query_find = new QSqlQuery(db);
        bool res_find = query_find->exec(sql_find);
        int id_old_person = -1;
        while (query_find->next())
            if (!query_find->value(0).isNull())
                id_old_person = query_find->value(0).toInt();
        delete query_find;

        if ( id_old_person<=0
             && ( !fam_old.isEmpty()
               || !im_old.isEmpty()
               || !ot_old.isEmpty()
               || dat_r_old.isValid()
               || !plc_birth_old.isEmpty()) ) {
            // запись старых данных ещё не создана - сделаем её
            QString sql_nold = "insert into persons_old (id_person, old_fam, old_im, old_ot, old_date_birth, old_plc_birth, old_enp, old_snils, old_sex) "
                               "values ( " + QString::number(id_person)  + " "
                               "       ,'" + QString (fam_old.isEmpty()   ? fam  : fam_old) + "' " +
                               "       ,'" + QString (im_old.isEmpty()    ? im   : im_old)  + "' " +
                               "       , " + QString (ot_old.isEmpty()    ? (ot.isEmpty()  ? "NULL"  : ("'" + ot + "'"))  : ("'" + ot_old + "'")) +
                               "       ,'" + QString (dat_r_old.isValid() ? dat_r_old.toString("yyyy-MM-dd") : dat_r.toString("yyyy-MM-dd")) + "' " +
                               "       ,'" + QString (plc_birth_old.isEmpty() ? plc_birth : plc_birth_old) + "' " +
                               "       , " + QString (enp.isEmpty()       ? "NULL"               : ("'" + enp + "'")) +
                               "       , " + QString (snils.isEmpty()     ? "NULL"               : ("'" + snils + "'")) +
                               "       , " + QString ((sex==1||sex==2)    ? QString::number(sex) : "1") +  " ) ; ";
            QSqlQuery *query_nold = new QSqlQuery(db);
            bool res_nold = query_nold->exec(sql_nold);

            if (!res_nold) {
                delete query_nold;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            delete query_nold;

        } else if ( id_old_person>0
                    && ( !fam_old.isEmpty()
                      || !im_old.isEmpty()
                      || !ot_old.isEmpty()
                      || dat_r_old.isValid()
                      || !plc_birth_old.isEmpty()) ) {
            // обновим старые денные персоны
            QString sql_old;
            sql_old  = "update persons_old "
                       "   set " +
                       QString (!fam_old.isEmpty()  ? (" old_fam =  '" + fam_old + "', ") : "") +
                       QString (!im_old.isEmpty()   ? (" old_im  =  '" + im_old  + "', ") : "") +
                       QString (!ot_old.isEmpty()   ? (" old_ot =   '" + ot_old  + "', ") : "") +
                       QString (dat_r_old.isValid() ? (" old_date_birth = '" + dat_r_old.toString("yyyy-MM-dd") + "', ") : "") +
                       QString (!plc_birth_old.isEmpty() ? (" old_plc_birth  = '" + plc_birth_old + "' ") : "") +
                       " where id_person = " + QString::number(id_person) + " ; ";
            sql_old = sql_old.replace(",  where", "  where");
            QSqlQuery *query_old = new QSqlQuery(db);
            bool res_old = query_old->exec(sql_old);

            while ( !res_old
                    && QMessageBox::warning( this, "Ошибка добавления старых данных персоны",
                                             "При попытке добавить старые данные персоны произошла неожиданная ошибка.\n\n"
                                             "n = " + QString::number(n) + "\n\n"
                                             + query_old->lastError().driverText() + "\n" + query_old->lastError().databaseText() + "\n\n"
                                             "Повторить?",
                                             QMessageBox::Retry|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel ) {
                res_old = query_old->exec(sql_old);
            }

            if (!res_old) {
                delete query_old;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            delete query_old;
        }







        if (!doc_num.isEmpty()) {
            //--------------------//
            // найдём/добавим УДЛ //
            //--------------------//
            QString sql_UDL;
            sql_UDL  = "select add_or_find_doc "
                       "   ( cast(" + QString::number(doc_type) + " as smallint), "
                       "     cast('" + doc_ser      + "' as varchar), "
                       "     cast('" + doc_num      + "' as varchar), "
                       "     cast(" + (doc_date.isValid() ? QString("'" + doc_date.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                       "     cast(" + (doc_exp.isValid()  ? QString("'" + doc_exp.toString("yyyy-MM-dd")  + "'") : "NULL") + " as date), "
                       "     cast('" + org_code     + "' as varchar), "
                       "     cast('" + doc_org      + "' as varchar), "
                       "     " + QString::number(id_person) + " ) ; ";
            QSqlQuery *query_UDL = new QSqlQuery(db);
            bool res_UDL = query_UDL->exec(sql_UDL);

            while ( !res_UDL
                    && QMessageBox::warning( this, "Ошибка добавления УДЛ",
                                             "При попытке добавить запись УДЛ произошла неожиданная ошибка.\n\n"
                                             "n = " + QString::number(n) + "\n\n"
                                             + query_UDL->lastError().driverText() + "\n" + query_UDL->lastError().databaseText() + "\n\n"
                                             "Повторить?",
                                             QMessageBox::Retry|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel ) {
                res_UDL = query_UDL->exec(sql_UDL);
            }

            if (!res_UDL) {
                delete query_UDL;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            query_UDL->next();

            id_UDL = query_UDL->value(0).toInt();
            if (id_UDL>0) {
                // найден готовый УДЛ650325

            } else if (id_UDL<0) {
                // добавлен новый УДЛ
                id_UDL = 0 - id_UDL;
                cnt_persons_docs++;
            } else {
                // !!! ошибка поиска/добавления записи !!!
            }
            delete query_UDL;
        }


        if (!doc_num_1.isEmpty()) {
            //--------------------//
            // найдём/добавим ДРП //
            //--------------------//
            QString sql_DRP;
            sql_DRP  = "select add_or_find_doc "
                       "   ( cast(" + QString::number(doc_type_1) + " as smallint), "
                       "     cast('" + doc_ser_1  + "' as varchar), "
                       "     cast('" + doc_num_1  + "' as varchar), "
                       "     cast(" + (doc_date_1.isValid() ? QString("'" + doc_date_1.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                       "     cast(" + (doc_exp_1.isValid()  ? QString("'" + doc_exp_1.toString("yyyy-MM-dd")  + "'") : "NULL") + " as date), "
                       "     cast('" + org_code_1 + "' as varchar), "
                       "     cast('" + doc_org_1  + "' as varchar), "
                       "     " + QString::number(id_person) + " ) ; ";
            QSqlQuery *query_DRP = new QSqlQuery(db);
            bool res_DRP = query_DRP->exec(sql_DRP);

            while ( !res_DRP
                    && QMessageBox::warning( this, "Ошибка добавления ДРП",
                                             "При попытке добавить запись ДРП произошла неожиданная ошибка.\n\n"
                                             "n = " + QString::number(n) + "\n\n"
                                             + query_DRP->lastError().driverText() + "\n" + query_DRP->lastError().databaseText() + "\n\n"
                                             "Повторить?",
                                             QMessageBox::Retry|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel ) {
                res_DRP = query_DRP->exec(sql_DRP);
            }

            if (!res_DRP) {
                delete query_DRP;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            query_DRP->next();

            id_DRP = query_DRP->value(0).toInt();
            if (id_DRP>0) {
                // найден готовый ДРП
            } else if (id_DRP<0) {
                // добавлен новый ДРП
                id_DRP = 0 - id_DRP;
                cnt_persons_docs++;
            } else {
                // !!! ошибка поиска/добавления записи !!!
            }
            delete query_DRP;
        }


        if (date_begin.isValid()) {
            //----------------------//
            // найдём/добавим визит //
            //----------------------//
            QString sql_vizit;
            sql_vizit  = "select add_or_find_vizit "
                         "   ( cast(" + (date_begin.isValid() ? QString("'" + date_begin.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                         "     cast(" + (rep_type==0 ? "1" : "2")  + " as smallint), "
                         "     cast(" + QString::number(r_smo)     + " as smallint), "
                         "     cast(" + QString::number(r_polis)   + " as smallint), "
                         "     cast(" + QString::number(f_polis)   + " as smallint), "
                         "     " + QString::number(id_person) + ") ; ";
            QSqlQuery *query_vizit = new QSqlQuery(db);
            bool res_vizit = query_vizit->exec(sql_vizit);

            while ( !res_vizit
                    && QMessageBox::warning( this, "Ошибка добавления визита",
                                             "При попытке добавить запись визита произошла неожиданная ошибка.\n\n"
                                             "n = " + QString::number(n) + "\n\n"
                                             + query_vizit->lastError().driverText() + "\n" + query_vizit->lastError().databaseText() + "\n\n"
                                             "Повторить?",
                                             QMessageBox::Retry|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel ) {
                res_vizit = query_vizit->exec(sql_vizit);
            }

            if (!res_vizit) {
                delete query_vizit;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            query_vizit->next();

            id_vizit = query_vizit->value(0).toInt();
            if (id_vizit>0) {
                // найден готовый визит
            } else if (id_vizit<0) {
                // добавлен новый визит
                id_vizit = 0 - id_vizit;
                cnt_vizits++;
            } else {
                // !!! ошибка поиска/добавления записи !!!
            }
            delete query_vizit;
        }

        bool f_vs=false, f_pol=false, f_uec=false;

        if ( /*pol_v==2
             &&*/ vs_num>1000000
             && vs_num<1000000000) {
            //-------------------------//
            // найдём/добавим бланк ВС //
            //-------------------------//
            QString sql_blank_vs;
            sql_blank_vs  = "select add_or_find_blank_vs "
                            "   ( " + QString::number(vs_num)    + ", "
                            "     " + QString::number(id_person) + ", "
                            "     " + QString::number(id_polis)  + ", "
                            "     cast(" + (date_begin.isValid() ? QString("'" + date_begin.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                            "     cast(" + (date_begin.isValid() ? QString("'" + date_begin.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                            "     cast(1 as smallint)) ; ";
            QSqlQuery *query_blank_vs = new QSqlQuery(db);
            bool res_blank_vs = query_blank_vs->exec(sql_blank_vs);

            while ( !res_blank_vs
                    && QMessageBox::warning( this, "Ошибка добавления бланка ВС",
                                             "При попытке добавить запись бланка ВС произошла неожиданная ошибка.\n\n"
                                             "n = " + QString::number(n) + "\n\n"
                                             + query_blank_vs->lastError().driverText() + "\n" + query_blank_vs->lastError().databaseText() + "\n\n"
                                             "Повторить?",
                                             QMessageBox::Retry|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel ) {
               res_blank_vs = query_blank_vs->exec(sql_blank_vs);
           }

           if (!res_blank_vs) {
                delete query_blank_vs;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            query_blank_vs->next();

            id_blank_vs = query_blank_vs->value(0).toInt();
            if (id_blank_vs>0) {
                // найдена готовая запись
            } else if (id_blank_vs<0) {
                // добавлен новый бланк ВС
                id_blank_vs = 0 - id_blank_vs;
                cnt_blanks_vs++;
            } else {
                // !!! ошибка поиска/добавления записи !!!
                QMessageBox::warning(this, "Ошибка добавления бланка ВС",
                                     "При попытке добавить запись бланка ВС произошла неожиданная ошибка.\n\n"
                                     "n = " + QString::number(n) + "\n\n"
                                     + query_blank_vs->lastError().driverText() + "\n" + query_blank_vs->lastError().databaseText() + "\n\n"
                                     "Запись пропущена.");
                delete query_blank_vs;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            delete query_blank_vs;

            f_vs = true;
        } //else

        if ( pol_v!=2
             && blank_ser.length()>0
             && blank_num.length()>0) {
            //-----------------------------//
            // найдём/добавим бланк полиса //
            //-----------------------------//
            QString sql_blank_pol;
            sql_blank_pol  = "select add_or_find_blank_pol "
                            "   ( cast('" + blank_ser + "' as varchar), "
                            "     cast('" + blank_num + "' as varchar), "
                            "     "       + QString::number(id_person) + ", "
                            "     "       + QString::number(id_polis)  + ", "
                            "     cast("  + (date_begin.isValid() ? QString("'" + date_begin.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                            "     cast("  + QString::number(f_polis)   + " as smallint), "
                            "     cast(NULL as date), "
                            "     cast('" + enp + "' as varchar), "
                            "     cast("  + (date_stop.isValid() ? QString("'"  + date_stop.toString("yyyy-MM-dd")  + "'") : "NULL") + " as date), "
                            "     cast("  + (date_begin.isValid() ? QString("'" + date_begin.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                            "     cast(NULL as varchar), "
                            "     cast(NULL as date), "
                            "     cast(1 as smallint)) ; ";
            QSqlQuery *query_blank_pol = new QSqlQuery(db);
            bool res_blank_pol = query_blank_pol->exec(sql_blank_pol);

            while ( !res_blank_pol
                    && QMessageBox::warning( this, "Ошибка добавления бланка полиса",
                                             "При попытке добавить запись бланка полиса произошла неожиданная ошибка.\n\n"
                                             "n = " + QString::number(n) + "\n\n"
                                             + query_blank_pol->lastError().driverText() + "\n" + query_blank_pol->lastError().databaseText() + "\n\n"
                                             "Повторить?",
                                             QMessageBox::Retry|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel ) {
                res_blank_pol = query_blank_pol->exec(sql_blank_pol);
            }

            if (!res_blank_pol) {
                delete query_blank_pol;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            query_blank_pol->next();

            id_blank_pol = query_blank_pol->value(0).toInt();
            if (id_blank_pol>0) {
                // найдена готовая запись
            } else if (id_blank_pol<0) {
                // добавлен новый бланк полиса
                id_blank_pol = 0 - id_blank_pol;
                cnt_blanks_pol++;
            } else {
                // !!! ошибка поиска/добавления записи !!!
                QMessageBox::warning(this, "Ошибка добавления бланка полиса",
                                     "При попытке добавить запись бланка полиса произошла неожиданная ошибка.\n\n"
                                     "n = " + QString::number(n) + "\n\n"
                                     + query_blank_pol->lastError().driverText() + "\n" + query_blank_pol->lastError().databaseText() + "\n\n"
                                     "Запись пропущена.");
                delete query_blank_pol;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            delete query_blank_pol;

            f_pol = true;
        } //else

        if ( uec_num.length()>15) {
            //-------------------------------------------//
            // найдём/добавим бланк полиса в составе УЭК //
            //-------------------------------------------//
            QString sql_blank_uec;
            sql_blank_uec  = "select add_or_find_blank_uec "
                            "   ( cast('" + uec_num + "' as varchar), "
                            "     "       + QString::number(id_person) + ", "
                            "     "       + QString::number(id_polis)  + ", "
                            "     cast("  + (date_begin.isValid() ? QString("'" + date_begin.toString("yyyy-MM-dd") + "'") : "NULL") + " as date) ) ; ";
            QSqlQuery *query_blank_uec = new QSqlQuery(db);
            bool res_blank_uec = query_blank_uec->exec(sql_blank_uec);

            while ( !res_blank_uec
                    && QMessageBox::warning( this, "Ошибка добавления бланка УЭК",
                                             "При попытке добавить запись бланка УЭК произошла неожиданная ошибка.\n\n"
                                             "n = " + QString::number(n) + "\n\n"
                                             + query_blank_uec->lastError().driverText() + "\n" + query_blank_uec->lastError().databaseText() + "\n\n"
                                             "Повторить?",
                                             QMessageBox::Retry|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel ) {
                res_blank_uec = query_blank_uec->exec(sql_blank_uec);
            }

            if (!res_blank_uec) {
                delete query_blank_uec;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            query_blank_uec->next();

            id_blank_uec = query_blank_uec->value(0).toString();
            if (id_blank_uec.left(1)=="-") {
                // добавлен новый бланк уэк
                id_blank_uec = id_blank_uec.right(id_blank_uec.length()-1);
                cnt_blanks_uec++;
            } else if (!id_blank_uec.isEmpty()) {
                // найдена готовая запись
            } else {
                // !!! ошибка поиска/добавления записи !!!
                QMessageBox::warning(this, "Ошибка добавления бланка УЭК",
                                     "При попытке добавить запись бланка УЭК произошла неожиданная ошибка.\n\n"
                                     "n = " + QString::number(n) + "\n\n"
                                     + query_blank_uec->lastError().driverText() + "\n" + query_blank_uec->lastError().databaseText() + "\n\n"
                                     "Запись пропущена.");
                delete query_blank_uec;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            delete query_blank_uec;

            f_uec = true;
        }

        if (!(f_pol||f_vs||f_uec)){
            // не добавлен ни бланк полиса единого образца ни бланк ВС - это странно ...
            QMessageBox::warning(this, "Нет бланка?",
                                 "Не добавлен ни бланк полиса единого образца, ни бланк ВС, ни бланк УЭК - это странно ...\n\n"
                                 "n = " + QString::number(n));
        }



        //----------------------//
        // найдём/добавим полис //
        //----------------------//
        QString sql_polis;
        sql_polis = "select add_or_find_polis "
                    "   ( cast("  + QString::number(pol_v)  + " as smallint), "
                    "     cast("  + QString::number(pol_f)  + " as smallint), "
                    "     cast('" + oms_rights              + "' as varchar), "
                    "     "       + QString::number(vs_num) + ", "
                    "     cast('" + blank_ser               + "' as varchar), "
                    "     cast('" + blank_num               + "' as varchar), "
                    "     cast('" + enp                     + "' as varchar), "
                    "     cast('" + uec_num                 + "' as varchar), "
                    "     cast("  + (date_begin.isValid() ? QString("'" + date_begin.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                    "     cast("  + (date_end.isValid()   ? QString("'" + date_end.toString("yyyy-MM-dd")   + "'") : "NULL") + " as date), "
                    "     cast("  + (date_stop.isValid()  ? QString("'" + date_stop.toString("yyyy-MM-dd")  + "'") : "NULL") + " as date), "
                    "     cast(NULL as date), "
                    "     cast("  + (date_begin.isValid() ? QString("'" + date_begin.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                    "     cast("  + (date_begin.isValid() ? QString("'" + date_begin.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                    "     NULL, "
                    "     cast(NULL as date), "
                    "     " + QString::number(id_person) + ") ; ";
        QSqlQuery *query_polis = new QSqlQuery(db);
        bool res_polis = query_polis->exec(sql_polis);

        while ( !res_polis
                && QMessageBox::warning( this, "Ошибка добавления полиса",
                                         "При попытке добавить запись полиса произошла неожиданная ошибка.\n\n"
                                         "n = " + QString::number(n) + "\n\n"
                                         + query_polis->lastError().driverText() + "\n" + query_polis->lastError().databaseText() + "\n\n"
                                         "Повторить?",
                                         QMessageBox::Retry|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel ) {
            res_polis = query_polis->exec(sql_polis);
        }

        if (!res_polis) {
            delete query_polis;
            db_oldBase.rollback();
            db.rollback();

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_load_tfoms->setEnabled(true);
            ui->bn_PersonsOld->setEnabled(true);
            ui->bn_clear_links->setEnabled(true);
            ui->bn_load_polises->setEnabled(true);
            ui->bn_reload_addres->setEnabled(true);
            ui->bn_close_polises->setEnabled(true);
            ui->bn_load_addresses->setEnabled(true);
            return false;
        }
        query_polis->next();

        id_polis = query_polis->value(0).toInt();
        if (id_polis>0) {
            // найдена готовая запись
        } else if (id_polis<0) {
            // добавлен новый полис
            id_polis = 0 - id_polis;
            cnt_polises++;
        } else {
            // !!! ошибка поиска/добавления записи !!!
        }
        delete query_polis;


        if (date_begin.isValid()) {
            //------------------------//
            // найдём/добавим событие //
            //------------------------//
            QString sql_event;
            sql_event  = "select add_or_find_event "
                         "   ( cast('" + event_code + "' as varchar), "
                         "     cast('" + date_begin.toString("yyyy-MM-dd") + " 00:00:00' as timestamp), "
                         "     cast('00:00:00' as time), "
                         "     cast('" + date_begin.toString("yyyy-MM-dd") + "' as date), "
                         "     cast(NULL as smallint), "
                         "     " + QString::number(id_person) + ", "
                         "     " + QString::number(id_polis)  + ", "
                         "     " + QString::number(id_vizit)  + ", "
                         "     " + QString::number(id_UDL)    + ", "
                         "     " + QString::number(id_DRP)    + ", "
                         "     cast(0 as smallint)) ; ";
            QSqlQuery *query_event = new QSqlQuery(db);
            bool res_event = query_event->exec(sql_event);

            while ( !res_event
                    && QMessageBox::warning( this, "Ошибка добавления события",
                                             "При попытке добавить запись события произошла неожиданная ошибка.\n\n"
                                             "n = " + QString::number(n) + "\n\n"
                                             + query_event->lastError().driverText() + "\n" + query_event->lastError().databaseText() + "\n\n"
                                             "Повторить?",
                                             QMessageBox::Retry|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel ) {
                res_event = query_event->exec(sql_event);
            }

            if (!res_event) {
                delete query_event;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            query_event->next();

            id_event = query_event->value(0).toInt();
            if (id_event>0) {
                // добавлен новый событие
                cnt_events++;
            } else if (id_event<0) {
                // найдена готовая запись
                id_event = 0 - id_event;
            } else {
                // !!! ошибка поиска/добавления записи !!!
            }
            delete query_event;
        }


        //------------------------------------------------//
        // запомним во внешней базе факт обработки записи //
        //------------------------------------------------//
        QString sql_link;
        sql_link  = "insert into polices_import(pol_id, id_person, id_addr_reg, id_addr_liv, id_vizit, id_event, id_polis, id_blank_vs, id_blank_pol, id_blank_uec) "
                    "values ( "
                    "     " + QString::number(pol_id) + ", "
                    "     " + QString::number(id_person) + ", "
                    "     " + (id_addr_reg>0 ? QString::number(id_addr_reg) : "NULL") + ", "
                    "     " + (id_addr_liv>0 ? QString::number(id_addr_liv) : "NULL") + ", "
                    "     " + QString::number(id_vizit) + ", "
                    "     " + QString::number(id_event) + ", "
                    "     " + QString::number(id_polis) + ", "
                    "     " + QString::number(id_blank_vs) + ", "
                    "     " + QString::number(id_blank_pol)+ ", "
                    "     '" + id_blank_uec + "') ; ";
        QSqlQuery *query_link = new QSqlQuery(db_oldBase);
        bool res_link = query_link->exec(sql_link);
        if (!res_link) {
            //delete query_link;
            // без проверок
        }
        delete query_link;


        //----------------------------//
        // пересчитаем статус персоны //
        //----------------------------//
        QString sql_status;
        sql_status  = "select * from update_pers_links_pol ( " + QString::number(id_person) + ", CURRENT_DATE ) ; ";
        QSqlQuery *query_status = new QSqlQuery(db);
        bool res_status = query_status->exec(sql_status);

        if (!res_status) {
            QMessageBox::warning(this, "Ошибка обновления статуса персоны",
                                 "При попытке обновить статус персоны произошла неожиданная ошибка.\n\n"
                                 "n = " + QString::number(n) + "\n\n"
                                 + query_status->lastError().driverText() + "\n" + query_status->lastError().databaseText() + "\n\n"
                                 "Запись пропущена.");
            delete query_status;
            db_oldBase.rollback();
            db.rollback();

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_load_tfoms->setEnabled(true);
            ui->bn_PersonsOld->setEnabled(true);
            ui->bn_clear_links->setEnabled(true);
            ui->bn_load_polises->setEnabled(true);
            ui->bn_reload_addres->setEnabled(true);
            ui->bn_close_polises->setEnabled(true);
            ui->bn_load_addresses->setEnabled(true);
            return false;
        }
        query_status->next();


        n++;

        if ( n%21==0
             || n==cnt_max-1 ) {
            // отображение статуса на экран
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


            ui->cnt_load_polises->setText(show_num(n));
            ui->cnt_persons->setText(show_num(cnt_persons));
            ui->cnt_persons_docs->setText(show_num(cnt_persons_docs));
            ui->cnt_persons_old->setText(show_num(cnt_persons_old));
            ui->cnt_vizits->setText(show_num(cnt_vizits));
            ui->cnt_events->setText(show_num(cnt_events));
            ui->cnt_polises_close->setText(show_num(0-cnt_polises_close) + ", ");
            ui->cnt_polises->setText(show_num(cnt_polises));
            ui->cnt_blanks_vs->setText(show_num(cnt_blanks_vs));
            ui->cnt_blanks_pol->setText(show_num(cnt_blanks_pol));
            ui->cnt_blanks_uec->setText(show_num(cnt_blanks_uec));
            ui->cnt_spr_insure_errors->setText(show_num(cnt_spr_insure_errors));
            ui->cnt_spr_oksm->setText(show_num(cnt_spr_oksm));
            ui->cnt_spr_ocato->setText(show_num(cnt_spr_ocato));
            ui->cnt_spr_ocato_full->setText(show_num(cnt_spr_ocato_full));
            ui->cnt_addresses->setText(show_num(cnt_addresses));
            ui->cnt_assig_by_terr->setText(show_num(cnt_assig_by_terr));
            ui->cnt_persons_assig->setText(show_num(cnt_persons_assig));
            ui->cnt_spr_assig_errors->setText(show_num(cnt_spr_assig_errors));
            ui->cnt_spr_medorgs->setText(show_num(cnt_spr_medorgs));
            ui->cnt_spr_medparts->setText(show_num(cnt_spr_medparts));
            ui->cnt_spr_medsites->setText(show_num(cnt_spr_medsites));
            ui->cnt_spr_medters->setText(show_num(cnt_spr_medters));
            ui->cnt_spr_ms_r_mt->setText(show_num(cnt_spr_ms_r_mt));
            ui->cnt_spr_medsites_types->setText(show_num(cnt_spr_medsites_types));
            ui->cnt_spr_medters_spec->setText(show_num(cnt_spr_medters_spec));

            ui->lab_persons->setEnabled(0<cnt_persons);
            ui->lab_persons_docs->setEnabled(0<cnt_persons_docs);
            ui->lab_persons_old->setEnabled(0<cnt_persons_old);
            ui->lab_vizits->setEnabled(0<cnt_vizits);
            ui->lab_events->setEnabled(0<cnt_events);
            ui->lab_polises->setEnabled(0<cnt_polises);
            ui->lab_blanks_vs->setEnabled(0<cnt_blanks_vs);
            ui->lab_blanks_pol->setEnabled(0<cnt_blanks_pol);
            ui->lab_blanks_uec->setEnabled(0<cnt_blanks_uec);
            ui->lab_addresses->setEnabled(0<cnt_addresses);

            ui->cnt_persons->setEnabled(0<cnt_persons);
            ui->cnt_persons_docs->setEnabled(0<cnt_persons_docs);
            ui->cnt_persons_old->setEnabled(0<cnt_persons_old);
            ui->cnt_vizits->setEnabled(0<cnt_vizits);
            ui->cnt_events->setEnabled(0<cnt_events);
            ui->cnt_polises->setEnabled(0<cnt_polises);
            ui->cnt_blanks_vs->setEnabled(0<cnt_blanks_vs);
            ui->cnt_blanks_pol->setEnabled(0<cnt_blanks_pol);
            ui->cnt_blanks_uec->setEnabled(0<cnt_blanks_uec);
            ui->cnt_addresses->setEnabled(0<cnt_addresses);

            db.commit();
            db.transaction();
            /*  --  нельзя - ведь у нас открыт запрос :(
            db_oldBase.commit();
            db_oldBase.transaction(); */

            QApplication::processEvents();
        }
    }
    int cnt_all = query->size();

    if (cnt_all<cnt_max)
        // все записи уже обработаны
        ui->bn_load_polises->setVisible(false);

    db_oldBase.commit();
    db.commit();

    ui->cnt_load_polises->setText(show_num(n));

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  Готово (" + QString::number(n) + " строк) !");
    QApplication::processEvents();
    if (ui->ch_process_all->isChecked())
          on_bn_reload_addres_clicked();
    else  QMessageBox::information(this, "Готово", "Все новые полисы загружены.\n\nОбработано " + QString::number(n) + " строк. ");


    ui->lay_load_table->setText("  - // -  ");
    ui->bn_close->setEnabled(true);
    ui->push_connect->setEnabled(true);
    ui->bn_load_tfoms->setEnabled(true);
    ui->bn_PersonsOld->setEnabled(true);
    ui->bn_clear_links->setEnabled(true);
    ui->bn_load_polises->setEnabled(true);
    ui->bn_reload_addres->setEnabled(true);
    ui->bn_close_polises->setEnabled(true);
    ui->bn_load_addresses->setEnabled(true);
    return true;
}



void old_db_wnd::on_bn_reload_addres_clicked() {
    ui->pan_wait->setVisible(true);
    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  Правка адресов регистрации / проживания");
    QApplication::processEvents();

    bool res_load = try_reload_addres();
    if (!res_load) {
        ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  действие отменено!");
        QMessageBox::warning(this, "Действие отменено", "Действие отменено.\n");
    } else if (!ui->ch_process_all->isChecked()) {
        QMessageBox::information(this, "Готово", "Все некорректные адреса обновлены.\n");
    }
    ui->pan_wait->setVisible(false);
    QApplication::processEvents();
}

bool old_db_wnd::try_reload_addres() {
    ui->lay_load_table->setText("ADDRESSES (подготовка)");
    ui->bn_close->setEnabled(false);
    ui->push_connect->setEnabled(false);
    ui->bn_load_tfoms->setEnabled(false);
    ui->bn_PersonsOld->setEnabled(false);
    ui->bn_clear_links->setEnabled(false);
    ui->bn_load_polises->setEnabled(false);
    ui->bn_reload_addres->setEnabled(false);
    ui->bn_close_polises->setEnabled(false);
    ui->bn_load_addresses->setEnabled(false);
    QApplication::processEvents();

    date0 = QDate::currentDate();
    time0 = QTime::currentTime();

    // подсчитаем существующие записи
    QSqlQuery *query_cnt = new QSqlQuery(db);
    query_cnt->exec("select count(*) from persons ; ");            query_cnt->next();     ui->cnt_persons_base->setText(            show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from persons_docs ; ");       query_cnt->next();     ui->cnt_persons_docs_base->setText(       show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from persons_old ; ");        query_cnt->next();     ui->cnt_persons_old_base->setText(        show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from vizits ; ");             query_cnt->next();     ui->cnt_vizits_base->setText(             show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from events ; ");             query_cnt->next();     ui->cnt_events_base->setText(             show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from polises ; ");            query_cnt->next();     ui->cnt_polises_base->setText(            show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from blanks_vs ; ");          query_cnt->next();     ui->cnt_blanks_vs_base->setText(          show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from blanks_pol ; ");         query_cnt->next();     ui->cnt_blanks_pol_base->setText(         show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from blanks_uec ; ");         query_cnt->next();     ui->cnt_blanks_uec_base->setText(         show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_insure_errors ; ");  query_cnt->next();     ui->cnt_spr_insure_errors_base->setText(  show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_oksm ; ");           query_cnt->next();     ui->cnt_spr_oksm_base->setText(           show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_ocato ; ");          query_cnt->next();     ui->cnt_spr_ocato_base->setText(          show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_ocato_full ; ");     query_cnt->next();     ui->cnt_spr_ocato_full_base->setText(     show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from addresses ; ");          query_cnt->next();     ui->cnt_addresses_base->setText(          show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from assig_by_terr ; ");      query_cnt->next();     ui->cnt_assig_by_terr_base->setText(      show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from persons_assig ; ");      query_cnt->next();     ui->cnt_persons_assig_base->setText(      show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_assig_errors ; ");   query_cnt->next();     ui->cnt_spr_assig_errors_base->setText(   show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medorgs ; ");        query_cnt->next();     ui->cnt_spr_medorgs_base->setText(        show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medparts ; ");       query_cnt->next();     ui->cnt_spr_medparts_base->setText(       show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medparts ; ");       query_cnt->next();     ui->cnt_spr_medparts_base->setText(       show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medsites ; ");       query_cnt->next();     ui->cnt_spr_medsites_base->setText(       show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medters ; ");        query_cnt->next();     ui->cnt_spr_medters_base->setText(        show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_ms_r_mt ; ");        query_cnt->next();     ui->cnt_spr_ms_r_mt_base->setText(        show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medsites_types ; "); query_cnt->next();     ui->cnt_spr_medsites_types_base->setText( show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medters_spec ; ");   query_cnt->next();     ui->cnt_spr_medters_spec_base->setText(   show_num(query_cnt->value(0).toInt()) );

    // подготовка
    date = QDate::currentDate();
    time = QTime::currentTime();
    ui->lab_time_all->setText("00:00:00");
    ui->lab_time->setText("00:00:00");
    QApplication::processEvents();

    int cnt_max = 2;
    if (ui->rb_10->isChecked())  cnt_max = 20;
    else if (ui->rb_100->isChecked())  cnt_max = 200;
    else if (ui->rb_1000->isChecked())  cnt_max = 2000;
    else if (ui->rb_10000->isChecked())  cnt_max = 20000;
    else if (ui->rb_100000->isChecked())  cnt_max = 200000;
    else if (ui->rb_1000000->isChecked())  cnt_max = 2000000;
    else if (ui->rb_hand->isChecked())  cnt_max = ui->spin_hand->value();

    ui->lab_persons->setEnabled(false);
    ui->lab_persons_docs->setEnabled(false);
    ui->lab_persons_old->setEnabled(false);
    ui->lab_vizits->setEnabled(false);
    ui->lab_events->setEnabled(false);
    ui->lab_polises->setEnabled(false);
    ui->lab_blanks_vs->setEnabled(false);
    ui->lab_blanks_pol->setEnabled(false);
    ui->lab_blanks_uec->setEnabled(false);
    ui->lab_spr_insure_errors->setEnabled(false);
    ui->lab_spr_oksm->setEnabled(false);
    ui->lab_spr_ocato->setEnabled(false);
    ui->lab_spr_ocato_full->setEnabled(false);
    ui->lab_addresses->setEnabled(false);
    ui->lab_assig_by_terr->setEnabled(false);
    ui->lab_persons_assig->setEnabled(false);
    ui->lab_spr_assig_errors->setEnabled(false);
    ui->lab_spr_medorgs->setEnabled(false);
    ui->lab_spr_medparts->setEnabled(false);
    ui->lab_spr_medsites->setEnabled(false);
    ui->lab_spr_medters->setEnabled(false);
    ui->lab_spr_ms_r_mt->setEnabled(false);
    ui->lab_spr_medsites_types->setEnabled(false);
    ui->lab_spr_medters_spec->setEnabled(false);

    ui->cnt_persons->setEnabled(false);
    ui->cnt_persons_docs->setEnabled(false);
    ui->cnt_persons_old->setEnabled(false);
    ui->cnt_vizits->setEnabled(false);
    ui->cnt_events->setEnabled(false);
    ui->cnt_polises->setEnabled(false);
    ui->cnt_blanks_vs->setEnabled(false);
    ui->cnt_blanks_pol->setEnabled(false);
    ui->cnt_blanks_uec->setEnabled(false);
    ui->cnt_spr_insure_errors->setEnabled(false);
    ui->cnt_spr_oksm->setEnabled(false);
    ui->cnt_spr_ocato->setEnabled(false);
    ui->cnt_spr_ocato_full->setEnabled(false);
    ui->cnt_addresses->setEnabled(false);
    ui->cnt_assig_by_terr->setEnabled(false);
    ui->cnt_persons_assig->setEnabled(false);
    ui->cnt_spr_assig_errors->setEnabled(false);
    ui->cnt_spr_medorgs->setEnabled(false);
    ui->cnt_spr_medparts->setEnabled(false);
    ui->cnt_spr_medsites->setEnabled(false);
    ui->cnt_spr_medters->setEnabled(false);
    ui->cnt_spr_ms_r_mt->setEnabled(false);
    ui->cnt_spr_medsites_types->setEnabled(false);
    ui->cnt_spr_medters_spec->setEnabled(false);

    ui->lay_load_table->setText("ADDRESSES (подсчёт числа записей)");
    QApplication::processEvents();

    QString sql_prep;
    // проверим, есть ли что грузить
    sql_prep = "select COUNT(*) "
               "  from persons e "
               "  left join addresses a on(a.id=e.id_addr_reg) "
               " where e.id_addr_reg is not NULL "
               "   and a.id is NULL ; ";

    QSqlQuery *query_prep = new QSqlQuery(db);
    bool res_prep = query_prep->exec(sql_prep);
    query_prep->next();
    int cnt_prep = query_prep->value(0).toInt();
    delete query_prep;

    if (cnt_prep<1 && !ui->ch_process_all->isChecked()) {
        QMessageBox::information(this, "Нечего грузить",
                                 "Все адреса уже загружены корректно.\n\n"
                                 "Операция отменена.");

        // все записи уже обработаны
        ui->bn_reload_addres->setVisible(false);

        ui->bn_close->setEnabled(true);
        ui->push_connect->setEnabled(true);
        ui->bn_load_tfoms->setEnabled(true);
        ui->bn_PersonsOld->setEnabled(true);
        ui->bn_clear_links->setEnabled(true);
        ui->bn_load_polises->setEnabled(true);
        ui->bn_reload_addres->setEnabled(true);
        ui->bn_close_polises->setEnabled(true);
        ui->bn_load_addresses->setEnabled(true);
        return true;
    }


    // покажем число битых адресов на экране
    ui->cnt_reload_addres_base->setText(show_num(cnt_prep));
    QApplication::processEvents();


    QString sql_mtch;
    QString sql_remk;
    QSqlQuery *query_mtch = new QSqlQuery(db);
    QSqlQuery *query_remk = new QSqlQuery(db_oldBase);
    int id_pers;
    bool res_mtch;
    QList<int> lst;
    bool res_remk;

    // выберем новые записи на перегрузку
    // адрес регистрации
    db_oldBase.transaction();
    sql_mtch = "select e.id "
               "  from persons e "
               "  left join addresses a on(a.id=e.id_addr_reg) "
               " where e.id_addr_reg is not NULL "
               "   and a.id is NULL "
               " limit " + QString::number(cnt_max) + " ; ";
    res_mtch = query_mtch->exec(sql_mtch);
    lst.clear();
    while (query_mtch->next()) {
        id_pers = query_mtch->value(0).toInt();
        lst.append(id_pers);
    }

    // отметим новыые записи на перегрузку
    for (int nn=0; nn<lst.size(); nn++) {
        id_pers = lst.at(nn);
        sql_remk = "update polices_import "
                   "   set id_addr_reg=-2 "
                   " where id_person=" + QString::number(id_pers) + " "
                   "   and ( id_addr_reg is NULL or id_addr_reg<>-2 ) ; ";
        res_remk = query_remk->exec(sql_remk);
    }
    db_oldBase.commit();



    // выберем новые записи на перегрузку
    // адрес проживания
    db_oldBase.transaction();
    sql_mtch = "select e.id "
               "  from persons e "
               "  left join addresses a on(a.id=e.id_addr_liv) "
               " where e.id_addr_liv is not NULL "
               "   and a.id is NULL "
               " limit " + QString::number(cnt_max) + " ; ";
    res_mtch = query_mtch->exec(sql_mtch);
    lst.clear();
    while (query_mtch->next()) {
        int id_pers = query_mtch->value(0).toInt();
        lst.append(id_pers);
    }

    // отметим новыые записи на перегрузку
    for (int nn=0; nn<lst.size(); nn++) {
        id_pers = lst.at(nn);
        sql_remk = "update polices_import "
                   "   set id_addr_liv=-2 "
                   " where id_person=" + QString::number(id_pers) + " "
                   "   and ( id_addr_liv is NULL or id_addr_liv<>-2 ) ; ";
        res_remk = query_remk->exec(sql_remk);
    }
    delete query_mtch;
    delete query_remk;
    db_oldBase.commit();



    ui->lay_load_table->setText("ADDRESSES\nчтение данных из FIREBIRD");
    QApplication::processEvents();

    // запрос данных
    QString sql;
    // Как показала практика, самое узкое место - это чтение данных из FIrebird
    // Поэтому читаем ОЧЕНЬ большими порциями
    sql  =  "select first " + QString::number(cnt_max) + " "
            "       p.pol_id, p.fam, p.im, p.ot, p.dat_r, coalesce(p.birthplace, p.birthplace_sity, p.birthplace_country) as plc_birth, p.pol, p.snils, p.enp, "
            "       coalesce(oksm.kod, '643') as oksm_c, coalesce(oks1.kod, '643') as oksm_birth, "
            "       p.contact_phone as phone, p.contact_phone_home as phone_home, p.contact_phone_cellular as phone_cell, p.contact_phone_work as phone_work, p.email, "
            "       p.n_papki as id_folder, "
            // старые данные
            "       p.fam_old, p.im_old, p.ot_old, p.dat_r_old, p.doc_type_id_old, p.ser_doc_old, p.nom_doc_old, p.date_issue_old, p.mr_old, "
            // адреса
            "       subj.adr_id as subj, subj.naim as subj_name, subj.kladr_code as subj_kladr, "
            "       dstr.adr_id as dstr, dstr.naim as dstr_name, dstr.kladr_code as dstr_kladr, "
            "       nasp.adr_id as nasp, nasp.naim as nasp_name, nasp.kladr_code as nasp_kladr, "
            "       strt.adr_id as strt, strt.naim as strt_name, strt.kladr_code as strt_kladr, "
            "       p.dom as house, p.korp as corp, p.kv as quart, p.postal_code as post_index, "
            "       coalesce(p.date_registration, p.temp_registr_beg, p.dat_r) as date_reg, p.bomj, "
            "       subj1.adr_id as subj_1, subj1.naim as subj_1_name, subj1.kladr_code as subj_1_kladr, "
            "       dstr1.adr_id as dstr_1, dstr1.naim as dstr_1_name, dstr1.kladr_code as dstr_1_kladr, "
            "       nasp1.adr_id as nasp_1, nasp1.naim as nasp_1_name, nasp1.kladr_code as nasp_1_kladr, "
            "       strt1.adr_id as strt_1, strt1.naim as strt_1_name, strt1.kladr_code as strt_1_kladr, "
            "       p.dom_1 as house_1, p.korp_1 as corp_1, p.kv_1 as quart_1, p.postal_code_1 as post_index_1, "
            // документы
            "       p.doc_type_id as doc_type, p.ser_doc as doc_ser, p.nom_doc as doc_num, p.date_issue as doc_date, p.podrazdcode as org_code, p.podr_issue as doc_org, NULL as doc_exp, "
            "       p.doc_type_id_1 as doc_type_1, p.ser_doc_1 as doc_ser_1, p.nom_doc_1 as doc_num_1, p.date_issue_1 as doc_date_1, NULL as org_code_1, p.podr_issue_1 as doc_org_1, p.temp_registr_end as doc_exp_1, "
            // визит
            "       p.fillreason as r_smo, p.fillreason_ps as r_polis, "
            // полис
            "       p.oms_rights as pol_v, p.temp_number as vs_num, p.dat_vp as date_begin, p.date_end, p.dat_ip as date_stop, "
            "       p.ser_pol, p.nom_pol, p.blank_ser, p.blank_num, "
            "       substring(bn.blank_num from 1 for 4) as blank_ser2, substring(bn.blank_num from 5 for 7) as blank_num2, "
            // событие
            "       p.action_event as event_code, "
            // ФИАС
            "       p.fias_aoid_addressg, p.fias_aohouseid_addressg, p.fias_aoid_addressp, p.fias_aohouseid_addressp, "
            // представитель
            "       p.agent, p.agent_ser_doc, p.agent_num_doc, p.agent_date_issue, p.agent_phone_cellular, p.agent_phone_home, p.agent_phone_work, "
            "       p.workstatus, pi.id_person, "
            "       pi.id_addr_reg, pi.id_addr_liv "
            "  from polices p "
            "  left join blank_nums bn on(p.last_num = bn.temp_num) "
            "  left join polices_import pi on(pi.pol_id=p.pol_id) "
            "  left join spr_country oksm on(oksm.name=p.citizenship) "
            "  left join spr_country oks1 on(oks1.name=p.birthplace_country) "
            "  left join adres_dict subj on(subj.adr_id=p.kod_t  and subj.obj_id=1) "
            "  left join adres_dict dstr on(dstr.adr_id=p.kod_r  and dstr.obj_id=2) "
            "  left join adres_dict nasp on(nasp.adr_id=p.nas_p  and nasp.obj_id=3) "
            "  left join adres_dict strt on(strt.adr_id=p.kod_ul and strt.obj_id=4) "
            "  left join adres_dict subj1 on(subj1.adr_id=p.kod_t_1  and subj1.obj_id=1) "
            "  left join adres_dict dstr1 on(dstr1.adr_id=p.kod_r_1  and dstr1.obj_id=2) "
            "  left join adres_dict nasp1 on(nasp1.adr_id=p.nas_p_1  and nasp1.obj_id=3) "
            "  left join adres_dict strt1 on(strt1.adr_id=p.kod_ul_1 and strt1.obj_id=4) "
            " where pi.id_addr_reg=-2 "
            "    or pi.id_addr_liv=-2 "         // помечен на перезагрузку адреса
            //"   and p.pol_id=9966771 "
            //"   and p.enp='2398389729000961' "
            " order by p.pol_id ; ";

    // заготовим переменные
    int pol_id;
    QString fam, im, ot;
    QString fam_norm, im_norm, ot_norm;
    QDate dat_r;
    QString plc_birth, pol;
    int sex;
    QString snils, enp, citizenship, birthplace_country;
    int oksm_birth, oksm_c;
    QString phone, phone_home, phone_cell, phone_work, email;
    int id_folder;
    int category;
    // старые данные
    QString fam_old, im_old, ot_old, plc_birth_old;
    QDate dat_r_old;
    int doc_type_id_old;
    QString ser_doc_old, nom_doc_old;
    QDate date_issue_old;
    // адреса
    int subj, dstr, nasp, strt, house, subj_1, dstr_1, nasp_1, strt_1, house_1, bomj;
    QString subj_name, dstr_name, nasp_name, strt_name, corp, quart, post_index, subj_1_name, dstr_1_name, nasp_1_name, strt_1_name, corp_1, quart_1, post_index_1, addr_reg_text, addr_liv_text;
    QString subj_kladr, dstr_kladr, nasp_kladr, strt_kladr, subj_1_kladr, dstr_1_kladr, nasp_1_kladr, strt_1_kladr;
    QString subj_s, dstr_s, city_s, nasp_s, strt_s, subj_1_s, dstr_1_s, city_1_s, nasp_1_s, strt_1_s;
    QDate date_reg;
    //QString fias_aiod, fias_houseid;
    // документы
    int doc_type, doc_type_1;
    QString doc_ser, doc_num, doc_ser_1, doc_num_1;
    QDate doc_date, doc_date_1, doc_exp, doc_exp_1;
    QString org_code, doc_org, org_code_1, doc_org_1;
    // визит
    int r_smo, r_polis, f_polis, v_method;
    // полис
    QString oms_rights;
    int pol_v, pol_f;
    int vs_num;
    QString uec_num;
    QString blank_ser,  blank_num;
    QString blank_ser2, blank_num2;
    QString pol_ser, pol_num;
    QDate date_begin, date_end, date_stop;
    // событие
    QString event_code;
    // ФИАС
    QString fias_aoid_r, fias_houseid_r, fias_aoid_l, fias_houseid_l;
    // представлитель
    int rep_type;
    QString rep_doc_ser, rep_doc_num;
    QDate rep_doc_date;
    QString rep_phone_cell, rep_phone_home, rep_phone_work;

    __int64 id_addr_reg, id_addr_liv, id_person, id_UDL, id_DRP, id_polis, id_vizit, id_event, id_blank_vs, id_blank_pol;
    QString id_blank_uec;

    db_oldBase.transaction();
    db.transaction();

    // выберем порцию данных
    QSqlQuery *query = new QSqlQuery(db_oldBase);
    bool res = query->exec(sql);

    if (!res) {
        QMessageBox::warning(this, "Ошибка чтения из таблицы POLICES",
                             "При попытке прочитать очередную порцию данных из таблицы POLICES произошла неожиданная ошибка.\n\n"
                             "Загрузка данных из внешней базы остановлена!");
        db_oldBase.rollback();
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->push_connect->setEnabled(true);
        ui->bn_load_tfoms->setEnabled(true);
        ui->bn_PersonsOld->setEnabled(true);
        ui->bn_clear_links->setEnabled(true);
        ui->bn_load_polises->setEnabled(true);
        ui->bn_reload_addres->setEnabled(true);
        ui->bn_close_polises->setEnabled(true);
        ui->bn_load_addresses->setEnabled(true);
        return false;
    }

    ui->lay_load_table->setText("ADDRESSES\nДобавление данных");
    QApplication::processEvents();

    int n = 0;

    while (query->next()) {
        //------------------//
        // прочитаем данные //
        //------------------//
        pol_id        = query->value( 0).toInt();
        fam           = query->value( 1).toString().trimmed().simplified().toUpper();
        im            = query->value( 2).toString().trimmed().simplified().toUpper().replace("'","\"");
        ot            = query->value( 3).toString().trimmed().simplified().toUpper().replace("'","\"");
        dat_r         = query->value( 4).toDate();
        plc_birth     = query->value( 5).toString().trimmed().simplified().replace("'","\"");
        pol           = query->value( 6).toString().trimmed();
        sex           = (pol.left(1)=="М" ? 1 : 2);
        snils         = query->value( 7).toString().trimmed().replace(" ", "").replace("-", "").replace("'","\"");
        enp           = query->value( 8).toString().trimmed().replace(" ", "").replace("-", "").replace("'","\"");
        oksm_c        = query->value( 9).toInt();
        oksm_birth    = query->value(10).toInt();
        phone         = query->value(11).toString().trimmed().replace(" ","").replace("+7","8").replace("(","").replace(")","").replace("-","").replace("'","\"");
        phone_home    = query->value(12).toString().trimmed().replace(" ","").replace("+7","8").replace("(","").replace(")","").replace("-","").replace("'","\"");
        phone_cell    = query->value(13).toString().trimmed().replace(" ","").replace("+7","8").replace("(","").replace(")","").replace("-","").replace("'","\"");
        phone_work    = query->value(14).toString().trimmed().replace(" ","").replace("+7","8").replace("(","").replace(")","").replace("-","").replace("'","\"");
        email         = query->value(15).toString().trimmed().simplified().replace("'","\"");
        id_folder     = query->value(16).toInt();
        // старые данные
        fam_old       = query->value(17).toString().trimmed().simplified().replace("'","\"");
        im_old        = query->value(18).toString().trimmed().simplified().replace("'","\"");
        ot_old        = query->value(19).toString().trimmed().simplified().replace("'","\"");
        dat_r_old     = query->value(20).toDate();
        doc_type_id_old = query->value(21).toInt();
        ser_doc_old   = query->value(22).toString().trimmed().simplified().replace("'","\"");
        nom_doc_old   = query->value(23).toString().trimmed().simplified().replace("'","\"");
        date_issue_old= query->value(24).toDate();
        plc_birth_old = query->value(25).toString().trimmed().simplified().replace("'","\"");
        // адрес регистрации
        subj          = query->value(26).toInt();
        subj_name     = query->value(27).toString().trimmed().simplified().replace("'","\"");
        subj_kladr    = query->value(28).toString().trimmed().simplified().replace("'","\"");
        dstr          = query->value(29).toInt();
        dstr_name     = query->value(30).toString().trimmed().simplified().replace("'","\"");
        dstr_kladr    = query->value(31).toString().trimmed().simplified().replace("'","\"");
        nasp          = query->value(32).toInt();
        nasp_name     = query->value(33).toString().trimmed().simplified().replace("'","\"");
        nasp_kladr    = query->value(34).toString().trimmed().simplified().replace("'","\"");
        strt          = query->value(35).toInt();
        strt_name     = query->value(36).toString().trimmed().simplified().replace("'","\"");
        strt_kladr    = query->value(37).toString().trimmed().simplified().replace("'","\"");
        house         = query->value(38).toInt();
        corp          = query->value(39).toString().trimmed().simplified().replace("'","\"");
        quart         = query->value(40).toString().trimmed().simplified().replace("'","\"");
        addr_reg_text = subj_name + ", " + dstr_name + ", " + nasp_name + ", " + strt_name + ", " + QString::number(house) + " " + corp + (quart>0 ? (", кв. " + quart) : "");
        // сформируем коды КЛАДР из выделенных компонентов
        subj_s = (!subj_kladr.isEmpty() ? subj_kladr.left(2)   : (!dstr_kladr.isEmpty() ? dstr_kladr.left(2)  : (!nasp_kladr.isEmpty() ? nasp_kladr.left(2)  : (!strt_kladr.isEmpty() ? strt_kladr.left(2) : "00"))));
        dstr_s = (!dstr_kladr.isEmpty() ? dstr_kladr.mid(2,3)  : (!nasp_kladr.isEmpty() ? nasp_kladr.mid(2,3) : (!strt_kladr.isEmpty() ? strt_kladr.mid(2,3) : "000")));
        city_s = (!nasp_kladr.isEmpty() ? nasp_kladr.mid(5,3)  : (!strt_kladr.isEmpty() ? strt_kladr.mid(5,3) : "000"));
        nasp_s = (!nasp_kladr.isEmpty() ? nasp_kladr.mid(8,3)  : (!strt_kladr.isEmpty() ? strt_kladr.mid(8,3) : "000"));
        strt_s = (!strt_kladr.isEmpty() ? strt_kladr.mid(11,4) : "0000");
        subj_kladr = subj_s + "000"  + "000"  + "000"  + "00";
        dstr_kladr = subj_s + dstr_s + "000"  + "000"  + "00";
        nasp_kladr = subj_s + dstr_s + city_s + nasp_s + "00";
        strt_kladr = subj_s + dstr_s + city_s + nasp_s + strt_s + "00";

        post_index    = query->value(41).toString().trimmed().simplified().replace("'","\"");
        date_reg      = query->value(42).toDate();
        bomj          = query->value(43).toInt();
        // адрес проживания
        subj_1        = query->value(44).toInt();
        subj_1_name   = query->value(45).toString().trimmed().simplified().replace("'","\"");
        subj_1_kladr  = query->value(46).toString().trimmed().simplified().replace("'","\"");
        dstr_1        = query->value(47).toInt();
        dstr_1_name   = query->value(48).toString().trimmed().simplified().replace("'","\"");
        dstr_1_kladr  = query->value(49).toString().trimmed().simplified().replace("'","\"");
        nasp_1        = query->value(50).toInt();
        nasp_1_name   = query->value(51).toString().trimmed().simplified().replace("'","\"");
        nasp_1_kladr  = query->value(52).toString().trimmed().simplified().replace("'","\"");
        strt_1        = query->value(53).toInt();
        strt_1_name   = query->value(54).toString().trimmed().simplified().replace("'","\"");
        strt_1_kladr  = query->value(55).toString().trimmed().simplified().replace("'","\"");
        house_1       = query->value(56).toInt();
        corp_1        = query->value(57).toString().trimmed().simplified().replace("'","\"");
        quart_1       = query->value(58).toString().trimmed().simplified().replace("'","\"");
        addr_liv_text = subj_1_name + ", " + dstr_1_name + ", " + nasp_1_name + ", " + strt_1_name + ", " + QString::number(house_1) + " " + corp_1 + (quart_1>0 ? (", кв. " + quart_1) : "");
        // сформируем коды КЛАДР_1 из выделенных компонентов
        subj_1_s = (!subj_1_kladr.isEmpty() ? subj_1_kladr.left(2)   : (!dstr_1_kladr.isEmpty() ? dstr_1_kladr.left(2)  : (!nasp_1_kladr.isEmpty() ? nasp_1_kladr.left(2)  : (!strt_1_kladr.isEmpty() ? strt_1_kladr.left(2) : "00"))));
        dstr_1_s = (!dstr_1_kladr.isEmpty() ? dstr_1_kladr.mid(2,3)  : (!nasp_1_kladr.isEmpty() ? nasp_1_kladr.mid(2,3) : (!strt_1_kladr.isEmpty() ? strt_1_kladr.mid(2,3) : "000")));
        city_1_s = (!nasp_1_kladr.isEmpty() ? nasp_1_kladr.mid(5,3)  : (!strt_1_kladr.isEmpty() ? strt_1_kladr.mid(5,3) : "000"));
        nasp_1_s = (!nasp_1_kladr.isEmpty() ? nasp_1_kladr.mid(8,3)  : (!strt_1_kladr.isEmpty() ? strt_1_kladr.mid(8,3) : "000"));
        strt_1_s = (!strt_1_kladr.isEmpty() ? strt_1_kladr.mid(11,4) : "0000");
        subj_1_kladr = subj_1_s + "000"    + "000"    + "000"    + "00";
        dstr_1_kladr = subj_1_s + dstr_1_s + "000"    + "000"    + "00";
        nasp_1_kladr = subj_1_s + dstr_1_s + city_1_s + nasp_1_s + "00";
        strt_1_kladr = subj_1_s + dstr_1_s + city_1_s + nasp_1_s + strt_1_s + "00";

        post_index_1  = query->value(59).toString().trimmed().simplified().replace("'","\"");
        // документы
        doc_type      = query->value(60).toInt();
        doc_ser       = query->value(61).toString().trimmed().simplified().replace("'","\"");
        doc_num       = query->value(62).toString().trimmed().simplified().replace("'","\"");
        doc_date      = query->value(63).toDate();
        org_code      = query->value(64).toString().trimmed().simplified().replace("'","\"");
        doc_org       = query->value(65).toString().trimmed().simplified().replace("'","\"");
        doc_exp       = query->value(66).toDate();
        doc_type_1    = query->value(67).toInt();
        doc_ser_1     = query->value(68).toString().trimmed().simplified().replace("'","\"");
        doc_num_1     = query->value(69).toString().trimmed().simplified().replace("'","\"");
        doc_date_1    = query->value(70).toDate();
        org_code_1    = query->value(71).toString().trimmed().simplified().replace("'","\"");
        doc_org_1     = query->value(72).toString().trimmed().simplified().replace("'","\"");
        doc_exp_1     = query->value(73).toDate();
        // визит
        r_smo         = query->value(74).toInt();
        r_polis       = query->value(75).toInt();
        // полис
        oms_rights    = query->value(76).toString().trimmed().simplified().replace("'","\"");
        if (oms_rights=="С")  pol_v = 1;  else if (oms_rights=="В")  pol_v = 2;  else pol_v = 3;
        vs_num        = query->value(77).toInt();
        date_begin    = query->value(78).toDate();
        date_end      = query->value(79).toDate();
        date_stop     = query->value(80).toDate();
        pol_ser       = query->value(81).toString().trimmed().simplified().replace("'","\"");
        pol_num       = query->value(82).toString().trimmed().simplified().replace("'","\"");
        blank_ser     = query->value(83).toString().trimmed().simplified().replace("'","\"");
        blank_num     = query->value(84).toString().trimmed().simplified().replace("'","\"");
        blank_ser2    = query->value(85).toString().trimmed().simplified().replace("'","\"");
        blank_num2    = query->value(86).toString().trimmed().simplified().replace("'","\"");
        // событие
        event_code    = query->value(87).toString().trimmed().simplified().replace("'","\"");
        // ФИАС
        fias_aoid_r   = query->value(88).toString().trimmed().simplified().replace(" ","").replace("'","\"");
        fias_houseid_r= query->value(89).toString().trimmed().simplified().replace(" ","").replace("'","\"");
        fias_aoid_l   = query->value(90).toString().trimmed().simplified().replace(" ","").replace("'","\"");
        fias_houseid_l= query->value(91).toString().trimmed().simplified().replace(" ","").replace("'","\"");
        // представитель
        rep_type      = query->value(92).toInt();
        rep_doc_ser   = query->value(93).toString().trimmed().simplified().replace("'","\"");
        rep_doc_num   = query->value(94).toString().trimmed().simplified().replace("'","\"");
        rep_doc_date  = query->value(95).toDate();
        rep_phone_home= query->value(96).toString().trimmed().replace(" ","").replace("+7","8").replace("(","").replace(")","").replace("-","").replace("'","\"");
        rep_phone_cell= query->value(97).toString().trimmed().replace(" ","").replace("+7","8").replace("(","").replace(")","").replace("-","").replace("'","\"");
        rep_phone_work= query->value(98).toString().trimmed().replace(" ","").replace("+7","8").replace("(","").replace(")","").replace("-","").replace("'","\"");
        category      = query->value(99).toInt();
        id_person     = query->value(100).toInt();
        id_addr_reg   = query->value(101).toInt();
        id_addr_liv   = query->value(102).toInt();

        //-----------//
        // коррекция //
        //-----------//
        fam_norm      = fam;
        im_norm       = im;
        ot_norm       = ot;
        fam_norm      = fam_norm.toUpper().replace("Ё","Е");
        im_norm       = im_norm.toUpper().replace("Ё","Е");
        ot_norm       = ot_norm.toUpper().replace("Ё","Е");
        if (oms_rights.isEmpty())
            oms_rights = "С";
        if (!dat_r.isValid())
            dat_r = QDate(2000,1,1);
        if (!date_reg.isValid())
            date_reg = dat_r;
        if (!snils.isEmpty())
            snils = QString("00000000000" + snils).right(11);
        if (!enp.isEmpty())
            enp = QString("0000000000000000" + enp).right(16);

        if (fam_old.isEmpty())
            fam_old = fam;
        if (im_old.isEmpty())
            im_old  = im;
        if (ot_old.isEmpty())
            ot_old  = ot;
        if (!dat_r_old.isValid())
            dat_r_old  = dat_r;
        if (plc_birth_old.isEmpty())
            plc_birth_old = plc_birth;

        if (!phone.isEmpty()) {
            phone = clear_num(phone);
            phone = QString(QString("84730000000").left(11 - phone.length()) + phone).left(20);
        }
        if (!phone_cell.isEmpty()){
            phone_cell = clear_num(phone_cell);
            phone_cell = QString(QString("84730000000").left(11 - phone_cell.length()) + phone_cell).left(20);
        }
        if (!phone_home.isEmpty()){
            phone_home = clear_num(phone_home);
            phone_home = QString(QString("84730000000").left(11 - phone_home.length()) + phone_home).left(20);
        }
        if (!phone_work.isEmpty()){
            phone_work = clear_num(phone_work);
            phone_work = QString(QString("84730000000").left(11 - phone_work.length()) + phone_work).left(20);
        }
        if (doc_type>0 && !doc_date.isValid()) {
            doc_date = dat_r;
        }
        if (oms_rights=="С") {
            blank_ser = pol_ser;
            blank_num = pol_num;
            event_code = "-//-";
        }
        if (blank_ser.isEmpty() || blank_ser=="0000")
            blank_ser = blank_ser2;
        if (blank_num.isEmpty())
            blank_num = blank_num2;
        if (blank_ser.isEmpty() || blank_ser=="0000")
            blank_ser = "X" + QString("...." + pol_ser).right(3);
        if (blank_num.isEmpty())
            blank_num = "X" + QString("......." + pol_num).right(7);

        if (oms_rights!="С" && blank_ser.length()<4)
            blank_ser = QString("0000" + blank_ser).right(4);

        pol_f = (oms_rights=="Э" ? 2 : 1);
        if (blank_num.length()>17) {
            oms_rights = "К";
            uec_num = blank_num;
            blank_ser = "";
            blank_num = "";
        } else
            uec_num = "";
        f_polis = ( oms_rights=="Э" ? 2 : 1 );
        if (!fias_aoid_r.isEmpty())
            fias_aoid_r = QString("00000000-0000-0000-0000-000000000000").left(36 - fias_aoid_r.length()) + fias_aoid_r;
        if (!fias_houseid_r.isEmpty())
            fias_houseid_r = QString("00000000-0000-0000-0000-000000000000").left(36 - fias_houseid_r.length()) + fias_houseid_r;
        if (!fias_aoid_l.isEmpty())
            fias_aoid_l = QString("00000000-0000-0000-0000-000000000000").left(36 - fias_aoid_l.length()) + fias_aoid_l;
        if (!fias_houseid_l.isEmpty())
            fias_houseid_l = QString("00000000-0000-0000-0000-000000000000").left(36 - fias_houseid_l.length()) + fias_houseid_l;

        id_polis = -1;
        id_vizit = -1;
        id_event = -1;
        id_UDL = -1;
        id_DRP = -1;
        id_blank_vs = -1;
        id_blank_pol = -1;
        id_blank_uec = "";

        //------------------------------ //
        // коррекция кодов КЛАДР по ФИАС //
        //------------------------------ //
        if (!fias_aoid_r.isEmpty()) {
            // попробуем найти данные КЛАДР по базе ФИАС
            QString sql_fias_aoid_r;
            sql_fias_aoid_r  = "select code "
                               "  from addrobj "
                               " where aoid = cast('" + fias_aoid_r + "' as uuid) "
                               " limit 1 ; ";
            QSqlQuery *query_fias_aoid_r = new QSqlQuery(db_FIAS);
            bool res_fias_aoid_r = query_fias_aoid_r->exec(sql_fias_aoid_r);
            if (query_fias_aoid_r->next()) {
                QString code = query_fias_aoid_r->value(0).toString();
                if (!code.isEmpty()) {
                    QString region_code = code.mid( 0, 2);
                    QString area_code   = code.mid( 2, 3);
                    QString city_code   = code.mid( 5, 3);
                    QString place_code  = code.mid( 8, 3);
                    QString street_code = code.mid(11, 4);
                    QString act_code    = code.mid(15, 2);
                    // получим коды по КЛАДР и названия
                    subj_kladr = region_code + "000000000" + act_code;
                    dstr_kladr = region_code + area_code + "000000" + act_code;
                    nasp_kladr = region_code + area_code + city_code + place_code + act_code;
                    strt_kladr = region_code + area_code + city_code + place_code + street_code +  act_code;
                }
            }
            delete query_fias_aoid_r;
        }

        if (!fias_aoid_l.isEmpty()) {
            // попробуем найти данные КЛАДР по базе ФИАС
            QString sql_fias_aoid_l;
            sql_fias_aoid_l  = "select code "
                               "  from addrobj "
                               " where aoid = cast('" + fias_aoid_l + "' as uuid) "
                               " limit 1 ; ";
            QSqlQuery *query_fias_aoid_l = new QSqlQuery(db_FIAS);
            bool res_fias_aoid_l = query_fias_aoid_l->exec(sql_fias_aoid_l);
            if (query_fias_aoid_l->next()) {
                QString code = query_fias_aoid_l->value(0).toString();
                if (!code.isEmpty()) {
                    QString region_code = code.mid( 0, 2);
                    QString area_code   = code.mid( 2, 3);
                    QString city_code   = code.mid( 5, 3);
                    QString place_code  = code.mid( 8, 3);
                    QString street_code = code.mid(11, 4);
                    QString act_code    = code.mid(15, 2);
                    // получим коды по КЛАДР и названия
                    subj_1_kladr = region_code + "000000000" + act_code;
                    dstr_1_kladr = region_code + area_code + "000000" + act_code;
                    nasp_1_kladr = region_code + area_code + city_code + place_code + act_code;
                    strt_1_kladr = region_code + area_code + city_code + place_code + street_code +  act_code;
                }
            }
            delete query_fias_aoid_l;
        }

        if ( id_addr_reg==-2
             && ( house>0
                  || (!corp.isEmpty()  && corp!="0"  && corp!="."  && corp!=".."  && corp!="-"  && corp!="," )
                  || (!quart.isEmpty() && quart!="0" && quart!="++" && quart!="-" && quart!="--" && quart!="." && quart!="..." && quart!="/") ) ) {
            //----------------------------------//
            // найдём/добавим адрес регистрации //
            //----------------------------------//
            QString sql_addr_reg;
            sql_addr_reg  = "select add_or_find_address "
                            "   ( cast('" + subj_name        + "' as varchar), "
                            "     cast('" + subj_kladr       + "' as varchar), "
                            "     cast('" + dstr_name        + "' as varchar), "
                            "     cast('" + dstr_kladr       + "' as varchar), "
                            "     cast('" + nasp_name        + "' as varchar), "
                            "     cast('" + nasp_kladr       + "' as varchar), "
                            "     cast('" + strt_name        + "' as varchar), "
                            "     cast('" + strt_kladr       + "' as varchar), "
                            "     " + QString::number(house) + ", "
                            "     cast('" + corp             + "' as varchar), "
                            "     cast('" + quart            + "' as varchar), "
                            "     cast('" + post_index       + "' as varchar), "
                            "     cast(" + (fias_aoid_r.isEmpty() ? "NULL" : QString("'" + fias_aoid_r  + "'")) + " as uuid), "
                            "     cast(" + (fias_houseid_r.isEmpty() ? "NULL" : QString("'" + fias_houseid_r  + "'")) + " as uuid)) ; ";
            QSqlQuery *query_addr_reg = new QSqlQuery(db);
            bool res_addr_reg = query_addr_reg->exec(sql_addr_reg);
            query_addr_reg->next();

            id_addr_reg = query_addr_reg->value(0).toInt();
            if (id_addr_reg>0) {
                // найден готовый адрес
            } else if (id_addr_reg<0) {
                // добавлен новый адрес
                id_addr_reg = 0 - id_addr_reg;
                cnt_addresses++;
            } else {
                // !!! ошибка поиска/добавления записи !!!
                QMessageBox::warning(this, "Ошибка добавления адреса регистрации",
                                     "При попытке добавить запись адреса регистрации произошла неожиданная ошибка.\n\n"
                                     "n = " + QString::number(n) + "\n\n"
                                     + query_addr_reg->lastError().driverText() + "\n" + query_addr_reg->lastError().databaseText() + "\n\n"
                                     "Запись пропущена.");
                delete query_addr_reg;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            delete query_addr_reg;
        }



        if ( id_addr_liv==-2
             && ( house_1>0
                  || (!corp_1.isEmpty()  && corp_1!="0"  && corp_1!="."  && corp_1!=".."  && corp_1!="-"  && corp_1!="," )
                  || (!quart_1.isEmpty() && quart_1!="0" && quart_1!="++" && quart_1!="-" && quart_1!="--" && quart_1!="." && quart_1!="..." && quart_1!="/") ) ) {

                  //|| (!corp_1.isEmpty() and corp_1!="0")
                  //|| (!quart_1.isEmpty() and quart!="-" and quart!="0") ) ) {
            //---------------------------------//
            // найдём/добавим адрес проживания //
            //---------------------------------//
            QString sql_addr_liv;
            sql_addr_liv  = "select add_or_find_address "
                            "   ( cast('" + subj_1_name        + "' as varchar), "
                            "     cast('" + subj_1_kladr       + "' as varchar), "
                            "     cast('" + dstr_1_name        + "' as varchar), "
                            "     cast('" + dstr_1_kladr       + "' as varchar), "
                            "     cast('" + nasp_1_name        + "' as varchar), "
                            "     cast('" + nasp_1_kladr       + "' as varchar), "
                            "     cast('" + strt_1_name        + "' as varchar), "
                            "     cast('" + strt_1_kladr       + "' as varchar), "
                            "     " + QString::number(house_1) + ", "
                            "     cast('" + corp_1             + "' as varchar), "
                            "     cast('" + quart_1            + "' as varchar), "
                            "     cast('" + post_index_1       + "' as varchar), "
                            "     cast(" + (fias_aoid_l.isEmpty() ? "NULL" : QString("'" + fias_aoid_l  + "'")) + " as uuid), "
                            "     cast(" + (fias_houseid_l.isEmpty() ? "NULL" : QString("'" + fias_houseid_l  + "'")) + " as uuid)) ; ";
            QSqlQuery *query_addr_liv = new QSqlQuery(db);
            bool res_addr_liv = query_addr_liv->exec(sql_addr_liv);
            query_addr_liv->next();

            id_addr_liv = query_addr_liv->value(0).toInt();
            if (id_addr_liv>0) {
                // найден готовый адрес
            } else if (id_addr_liv<0) {
                // добавлен новый адрес
                id_addr_liv = 0 - id_addr_liv;
                cnt_addresses++;
            } else {
                // !!! ошибка поиска/добавления записи !!!
                QMessageBox::warning(this, "Ошибка добавления адреса проживания",
                                     "При попытке добавить запись адреса проживания произошла неожиданная ошибка.\n\n"
                                     "n = " + QString::number(n) + "\n\n"
                                     + query_addr_liv->lastError().driverText() + "\n" + query_addr_liv->lastError().databaseText() + "\n\n"
                                     "Запись пропущена.");
                delete query_addr_liv;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            delete query_addr_liv;
        }


        //-------------------------//
        // запомним адреса персоны //
        //-------------------------//
        QString sql_saveadr;
        sql_saveadr  = "update persons e "
                       "   set id_addr_reg = " + QString(id_addr_reg>0 ? QString::number(id_addr_reg) : "NULL") + ", "
                       "       id_addr_liv = " + QString(id_addr_liv>0 ? QString::number(id_addr_liv) : "NULL") + " "
                       " where id = " + QString::number(id_person) + " ; ";
        QSqlQuery *query_saveadr = new QSqlQuery(db);
        bool res_saveadr = query_saveadr->exec(sql_saveadr);

        if (!res_saveadr) {
            QMessageBox::warning(this, "Ошибка сохранения адреса персоны",
                                 "При попытке добавить адрес персоны произошла неожиданная ошибка.\n\n"
                                 "n = " + QString::number(n) + "\n\n"
                                 + query_saveadr->lastError().driverText() + "\n" + query_saveadr->lastError().databaseText() + "\n\n"
                                 "Запись пропущена.");
            delete query_saveadr;
            db_oldBase.rollback();
            db.rollback();

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_load_tfoms->setEnabled(true);
            ui->bn_PersonsOld->setEnabled(true);
            ui->bn_clear_links->setEnabled(true);
            ui->bn_load_polises->setEnabled(true);
            ui->bn_reload_addres->setEnabled(true);
            ui->bn_close_polises->setEnabled(true);
            ui->bn_load_addresses->setEnabled(true);
            return false;
        }
        delete query_saveadr;


        //------------------------------------------------//
        // запомним во внешней базе факт обработки записи //
        //------------------------------------------------//
        QString sql_link;
        sql_link  = "update polices_import "
                    "   set id_addr_reg = " + (id_addr_reg>0 ? QString::number(id_addr_reg) : "NULL") + ", "
                    "       id_addr_liv = " + (id_addr_liv>0 ? QString::number(id_addr_liv) : "NULL") + " "
                    " where id_person = " + QString::number(id_person) + " ; ";
        QSqlQuery *query_link = new QSqlQuery(db_oldBase);
        bool res_link = query_link->exec(sql_link);

        if (!res_link) {
            //delete query_link;
            // без проверок
        }
        delete query_link;



        n++;

        if ( n%21==0
             || n==cnt_max-1 ) {
            // отображение статуса на экран
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


            ui->cnt_reload_addres->setText(show_num(n));
            ui->cnt_persons->setText(show_num(cnt_persons));
            ui->cnt_persons_docs->setText(show_num(cnt_persons_docs));
            ui->cnt_persons_old->setText(show_num(cnt_persons_old));
            ui->cnt_vizits->setText(show_num(cnt_vizits));
            ui->cnt_events->setText(show_num(cnt_events));
            ui->cnt_polises_close->setText(show_num(0-cnt_polises_close) + ", ");
            ui->cnt_polises->setText(show_num(cnt_polises));
            ui->cnt_blanks_vs->setText(show_num(cnt_blanks_vs));
            ui->cnt_blanks_pol->setText(show_num(cnt_blanks_pol));
            ui->cnt_blanks_uec->setText(show_num(cnt_blanks_uec));
            ui->cnt_spr_insure_errors->setText(show_num(cnt_spr_insure_errors));
            ui->cnt_spr_oksm->setText(show_num(cnt_spr_oksm));
            ui->cnt_spr_ocato->setText(show_num(cnt_spr_ocato));
            ui->cnt_spr_ocato_full->setText(show_num(cnt_spr_ocato_full));
            ui->cnt_addresses->setText(show_num(cnt_addresses));
            ui->cnt_assig_by_terr->setText(show_num(cnt_assig_by_terr));
            ui->cnt_persons_assig->setText(show_num(cnt_persons_assig));
            ui->cnt_spr_assig_errors->setText(show_num(cnt_spr_assig_errors));
            ui->cnt_spr_medorgs->setText(show_num(cnt_spr_medorgs));
            ui->cnt_spr_medparts->setText(show_num(cnt_spr_medparts));
            ui->cnt_spr_medsites->setText(show_num(cnt_spr_medsites));
            ui->cnt_spr_medters->setText(show_num(cnt_spr_medters));
            ui->cnt_spr_ms_r_mt->setText(show_num(cnt_spr_ms_r_mt));
            ui->cnt_spr_medsites_types->setText(show_num(cnt_spr_medsites_types));
            ui->cnt_spr_medters_spec->setText(show_num(cnt_spr_medters_spec));

            ui->lab_persons->setEnabled(0<cnt_persons);
            ui->lab_persons_docs->setEnabled(0<cnt_persons_docs);
            ui->lab_persons_old->setEnabled(0<cnt_persons_old);
            ui->lab_vizits->setEnabled(0<cnt_vizits);
            ui->lab_events->setEnabled(0<cnt_events);
            ui->lab_polises->setEnabled(0<cnt_polises);
            ui->lab_blanks_vs->setEnabled(0<cnt_blanks_vs);
            ui->lab_blanks_pol->setEnabled(0<cnt_blanks_pol);
            ui->lab_blanks_uec->setEnabled(0<cnt_blanks_uec);
            ui->lab_addresses->setEnabled(0<cnt_addresses);

            ui->cnt_persons->setEnabled(0<cnt_persons);
            ui->cnt_persons_docs->setEnabled(0<cnt_persons_docs);
            ui->cnt_persons_old->setEnabled(0<cnt_persons_old);
            ui->cnt_vizits->setEnabled(0<cnt_vizits);
            ui->cnt_events->setEnabled(0<cnt_events);
            ui->cnt_polises->setEnabled(0<cnt_polises);
            ui->cnt_blanks_vs->setEnabled(0<cnt_blanks_vs);
            ui->cnt_blanks_pol->setEnabled(0<cnt_blanks_pol);
            ui->cnt_blanks_uec->setEnabled(0<cnt_blanks_uec);
            ui->cnt_addresses->setEnabled(0<cnt_addresses);

            db.commit();
            db.transaction();
            //  --  нельзя - ведь у нас открыт запрос :(
            //db_oldBase.commit();
            //db_oldBase.transaction();

            QApplication::processEvents();
        }
    }
    int cnt_all = query->size();
    delete query;

    if (cnt_all<cnt_max)
        // все записи уже обработаны
        ui->bn_reload_addres->setVisible(false);

    db_oldBase.commit();
    db.commit();

    ui->cnt_reload_addres->setText(show_num(n));

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  Готово (" + QString::number(n) + " строк) !");
    QApplication::processEvents();
    if (ui->ch_process_all->isChecked())
          on_bn_load_addresses_clicked();
    else  QMessageBox::information(this, "Готово", "Все некорректные адреса обновлены.\n\nОбработано " + QString::number(n) + " строк. ");


    ui->lay_load_table->setText("  - // -  ");
    ui->bn_close->setEnabled(true);
    ui->push_connect->setEnabled(true);
    ui->bn_load_tfoms->setEnabled(true);
    ui->bn_PersonsOld->setEnabled(true);
    ui->bn_clear_links->setEnabled(true);
    ui->bn_load_polises->setEnabled(true);
    ui->bn_reload_addres->setEnabled(true);
    ui->bn_close_polises->setEnabled(true);
    ui->bn_load_addresses->setEnabled(true);
    return true;
}



void old_db_wnd::on_bn_load_addresses_clicked() {
    ui->pan_wait->setVisible(true);
    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  Загрузка адресной базы");
    QApplication::processEvents();

    bool res_load = try_load_addresses();
    if (!res_load) {
        ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  действие отменено!");
        QMessageBox::warning(this, "Действие отменено", "Действие отменено.\n");
    } else {
        QMessageBox::information(this, "Готово", "Новые адресные данные загружены.\n");
    }
    ui->pan_wait->setVisible(false);
    QApplication::processEvents();
}

bool old_db_wnd::try_load_addresses() {
    ui->lay_load_table->setText("ASSIG_BY_TERR");
    ui->bn_close->setEnabled(false);
    ui->push_connect->setEnabled(false);
    ui->bn_load_tfoms->setEnabled(false);
    ui->bn_PersonsOld->setEnabled(false);
    ui->bn_clear_links->setEnabled(false);
    ui->bn_load_polises->setEnabled(false);
    ui->bn_reload_addres->setEnabled(false);
    ui->bn_close_polises->setEnabled(false);
    ui->bn_load_addresses->setEnabled(false);
    QApplication::processEvents();

    date0 = QDate::currentDate();
    time0 = QTime::currentTime();

    // подсчитаем существующие записи
    QSqlQuery *query_cnt = new QSqlQuery(db);
    query_cnt->exec("select count(*) from persons ; ");            query_cnt->next();     ui->cnt_persons_base->setText(            show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from persons_docs ; ");       query_cnt->next();     ui->cnt_persons_docs_base->setText(       show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from persons_old ; ");        query_cnt->next();     ui->cnt_persons_old_base->setText(        show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from vizits ; ");             query_cnt->next();     ui->cnt_vizits_base->setText(             show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from events ; ");             query_cnt->next();     ui->cnt_events_base->setText(             show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from polises ; ");            query_cnt->next();     ui->cnt_polises_base->setText(            show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from blanks_vs ; ");          query_cnt->next();     ui->cnt_blanks_vs_base->setText(          show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from blanks_pol ; ");         query_cnt->next();     ui->cnt_blanks_pol_base->setText(         show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from blanks_uec ; ");         query_cnt->next();     ui->cnt_blanks_uec_base->setText(         show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_insure_errors ; ");  query_cnt->next();     ui->cnt_spr_insure_errors_base->setText(  show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_oksm ; ");           query_cnt->next();     ui->cnt_spr_oksm_base->setText(           show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_ocato ; ");          query_cnt->next();     ui->cnt_spr_ocato_base->setText(          show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_ocato_full ; ");     query_cnt->next();     ui->cnt_spr_ocato_full_base->setText(     show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from addresses ; ");          query_cnt->next();     ui->cnt_addresses_base->setText(          show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from assig_by_terr ; ");      query_cnt->next();     ui->cnt_assig_by_terr_base->setText(      show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from persons_assig ; ");      query_cnt->next();     ui->cnt_persons_assig_base->setText(      show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_assig_errors ; ");   query_cnt->next();     ui->cnt_spr_assig_errors_base->setText(   show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medorgs ; ");        query_cnt->next();     ui->cnt_spr_medorgs_base->setText(        show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medparts ; ");       query_cnt->next();     ui->cnt_spr_medparts_base->setText(       show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medparts ; ");       query_cnt->next();     ui->cnt_spr_medparts_base->setText(       show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medsites ; ");       query_cnt->next();     ui->cnt_spr_medsites_base->setText(       show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medters ; ");        query_cnt->next();     ui->cnt_spr_medters_base->setText(        show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_ms_r_mt ; ");        query_cnt->next();     ui->cnt_spr_ms_r_mt_base->setText(        show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medsites_types ; "); query_cnt->next();     ui->cnt_spr_medsites_types_base->setText( show_num(query_cnt->value(0).toInt()));
    query_cnt->exec("select count(*) from spr_medters_spec ; ");   query_cnt->next();     ui->cnt_spr_medters_spec_base->setText(   show_num(query_cnt->value(0).toInt()));

    // подготовка
    date = QDate::currentDate();
    time = QTime::currentTime();
    ui->lab_time_all->setText("00:00:00");
    ui->lab_time->setText("00:00:00");
    QApplication::processEvents();

    ui->lab_persons->setEnabled(false);
    ui->lab_persons_docs->setEnabled(false);
    ui->lab_persons_old->setEnabled(false);
    ui->lab_vizits->setEnabled(false);
    ui->lab_events->setEnabled(false);
    ui->lab_polises->setEnabled(false);
    ui->lab_blanks_vs->setEnabled(false);
    ui->lab_blanks_pol->setEnabled(false);
    ui->lab_blanks_uec->setEnabled(false);
    ui->lab_spr_insure_errors->setEnabled(false);
    ui->lab_spr_oksm->setEnabled(false);
    ui->lab_spr_ocato->setEnabled(false);
    ui->lab_spr_ocato_full->setEnabled(false);
    ui->lab_addresses->setEnabled(false);
    ui->lab_assig_by_terr->setEnabled(false);
    ui->lab_persons_assig->setEnabled(false);
    ui->lab_spr_assig_errors->setEnabled(false);
    ui->lab_spr_medorgs->setEnabled(false);
    ui->lab_spr_medparts->setEnabled(false);
    ui->lab_spr_medsites->setEnabled(false);
    ui->lab_spr_medters->setEnabled(false);
    ui->lab_spr_ms_r_mt->setEnabled(false);
    ui->lab_spr_medsites_types->setEnabled(false);
    ui->lab_spr_medters_spec->setEnabled(false);

    ui->cnt_persons->setEnabled(false);
    ui->cnt_persons_docs->setEnabled(false);
    ui->cnt_persons_old->setEnabled(false);
    ui->cnt_vizits->setEnabled(false);
    ui->cnt_events->setEnabled(false);
    ui->cnt_polises->setEnabled(false);
    ui->cnt_blanks_vs->setEnabled(false);
    ui->cnt_blanks_pol->setEnabled(false);
    ui->cnt_blanks_uec->setEnabled(false);
    ui->cnt_spr_insure_errors->setEnabled(false);
    ui->cnt_spr_oksm->setEnabled(false);
    ui->cnt_spr_ocato->setEnabled(false);
    ui->cnt_spr_ocato_full->setEnabled(false);
    ui->cnt_addresses->setEnabled(false);
    ui->cnt_assig_by_terr->setEnabled(false);
    ui->cnt_persons_assig->setEnabled(false);
    ui->cnt_spr_assig_errors->setEnabled(false);
    ui->cnt_spr_medorgs->setEnabled(false);
    ui->cnt_spr_medparts->setEnabled(false);
    ui->cnt_spr_medsites->setEnabled(false);
    ui->cnt_spr_medters->setEnabled(false);
    ui->cnt_spr_ms_r_mt->setEnabled(false);
    ui->cnt_spr_medsites_types->setEnabled(false);
    ui->cnt_spr_medters_spec->setEnabled(false);

    //-------------------------//
    // скопируем адресную базу //
    //-------------------------//
    ui->lay_load_table->setText("ASSIG_BY_TERR");
    QApplication::processEvents();

    // запрос данных
    QString sql;
    sql  =  "select ta.id, '20000' as ocato, "
            "       left(coalesce(subj.kladr_code, dstr.kladr_code),2) as subj, "
            "       right(left(coalesce(dstr.kladr_code, subj.kladr_code), 5),3) as dstr, "
            "       right(left(coalesce(nasp.kladr_code, dstr.kladr_code, subj.kladr_code), 8),3) as city, "
            "       right(left(coalesce(nasp.kladr_code, dstr.kladr_code, subj.kladr_code),11),3) as nasp, "
            "       right(left(strt.kladr_code,15),4) as strt, "
            "       ta.n_dom as house, ta.korp as corp, 0 as sex, ta.for_age as age, 0 as side, "
            "       NULL as from_start, NULL as to_end, "
            "       cast(360000 + ta.code_lpu as varchar(6)) as code_mo, '0' as code_mp, ta.uch as code_ms, NULL as snils_mt, "
            "       NULL as comment "
            "  from assig_by_terr ta "
            "  left join adres_dict subj on(subj.adr_id=ta.code_terr  and subj.obj_id=1) "
            "  left join adres_dict dstr on(dstr.adr_id=ta.code_distr and dstr.obj_id=2) "
            "  left join adres_dict nasp on(nasp.adr_id=ta.code_nasp  and nasp.obj_id=3) "
            "  left join adres_dict strt on(strt.adr_id=ta.code_strt  and strt.obj_id=4) "
            " where ta.import is NULL "
            " order by subj.adr_id, dstr.adr_id, nasp.adr_id, strt.adr_id, ta.n_dom, ta.korp ; ";

    // заготовим переменные
    int id;
    QString ocato;
    int subj, dstr, city, nasp, strt;
    int house;
    QString corp;
    int sex, age, side, from_start, to_end;
    QString code_mo, code_mp, code_ms, snils_mt;
    QString comment;

    __int64 id_assig_by_terr;

    db_oldBase.transaction();
    db.transaction();

    // выберем порцию данных
    QSqlQuery *query = new QSqlQuery(db_oldBase);
    bool res = query->exec(sql);
    int cnt_max = query->size();

    if (!res) {
        QMessageBox::warning(this, "Ошибка чтения из таблицы  ASSIG_BY_TERR",
                             "При попытке прочитать данные из таблицы  ASSIG_BY_TERR  произошла неожиданная ошибка.\n\n"
                             "Загрузка данных из этой таблицы отменена!");
        db_oldBase.rollback();
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->push_connect->setEnabled(true);
        ui->bn_load_tfoms->setEnabled(true);
        ui->bn_PersonsOld->setEnabled(true);
        ui->bn_clear_links->setEnabled(true);
        ui->bn_load_polises->setEnabled(true);
        ui->bn_reload_addres->setEnabled(true);
        ui->bn_close_polises->setEnabled(true);
        ui->bn_load_addresses->setEnabled(true);
        return false;
    }


    // покажем число битых адресов на экране
    ui->cnt_load_addresses_base->setText(show_num(cnt_max));
    QApplication::processEvents();

    int n = 0;

    while (query->next()) {
        //------------------//
        // прочитаем данные //
        //------------------//
        id         = query->value( 0).toInt();
        ocato      = query->value( 1).toString().trimmed();
        subj       = query->value( 2).toInt();
        dstr       = query->value( 3).toInt();
        city       = query->value( 4).toInt();
        nasp       = query->value( 5).toInt();
        strt       = query->value( 6).toInt();
        house      = query->value( 7).toInt();
        corp       = query->value( 8).toString().trimmed();
        sex        = query->value( 9).toInt();
        age        = query->value(10).toInt();
        side       = query->value(11).toInt();
        from_start = query->value(12).toInt();
        to_end     = query->value(13).toInt();
        code_mo    = query->value(14).toString().trimmed();
        code_mp    = query->value(15).toString().trimmed();
        code_ms    = query->value(16).toString().trimmed();
        snils_mt   = query->value(17).toString().trimmed();
        comment    = query->value(18).toString().trimmed().simplified();

        id_assig_by_terr = -1;

        //-------------------------------------//
        // найдём/добавим правило прикрепления //
        //-------------------------------------//
        QString sql_assig;
        sql_assig = "select add_or_find_assig_by_terr "
                    "   ( cast('" + ocato                       + "' as varchar),  "
                    "     cast( " + QString::number(subj)       + "  as smallint), "
                    "     cast( " + QString::number(dstr)       + "  as smallint), "
                    "     cast( " + QString::number(city)       + "  as smallint), "
                    "     cast( " + QString::number(nasp)       + "  as smallint), "
                    "     cast( " + QString::number(strt)       + "  as smallint), "
                    "     cast( " + QString::number(house)      + "  as smallint), "
                    "     cast('" + corp                        + "' as varchar),  "
                    "     cast( " + QString::number(sex)        + "  as smallint), "
                    "     cast( " + QString::number(age)        + "  as smallint), "
                    "     cast( " + QString::number(side)       + "  as smallint), "
                    "     cast( " + QString::number(from_start) + "  as smallint), "
                    "     cast( " + QString::number(to_end)     + "  as smallint), "
                    "     cast('" + code_mo                     + "' as varchar), "
                    "     cast('" + code_mp                     + "' as varchar), "
                    "     cast('" + code_ms                     + "' as varchar), "
                    "     cast('" + snils_mt                    + "' as varchar), "
                    "     cast('" + comment                     + "' as varchar)) ; ";
        QSqlQuery *query_assig = new QSqlQuery(db);
        bool res_assig = query_assig->exec(sql_assig);

        if (!res_assig) {
            QMessageBox::warning(this, "Ошибка добавления записи адресной базы",
                                 "При попытке добавить запись адресной базы произошла неожиданная ошибка.\n\n"
                                 "n = " + QString::number(n) + "\n\n"
                                 + query_assig->lastError().driverText() + "\n" + query_assig->lastError().databaseText() + "\n\n"
                                 "Загрузка отменена.");
            delete query_assig;
            db_oldBase.rollback();
            db_oldBase.transaction();
            db.rollback();
            db.transaction();
            return false;
        }
        query_assig->next();

        id_assig_by_terr = query_assig->value(0).toInt();
        if (id_assig_by_terr>0) {
            // найдена готовая запись прикрепления персоны
        } else if (id_assig_by_terr<0) {
            // добавлена новая запись прикрепления персоны
            id_assig_by_terr = 0 - id_assig_by_terr;
            cnt_assig_by_terr++;
        } else {
            // !!! ошибка поиска/добавления записи !!!
        }
        delete query_assig;

        //------------------------------------------------//
        // запомним во внешней базе факт обработки записи //
        //------------------------------------------------//
        QString sql_link;
        sql_link  = "update ASSIG_BY_TERR "
                    "   set import=1 "
                    " where id=" + QString::number(id) + " ; ";
        QSqlQuery *query_link = new QSqlQuery(db_oldBase);
        bool res_link = query_link->exec(sql_link);

        if (!res_link) {
            delete query_link;
            // без проверок
        }
        delete query_link;

        n++;

        if ( n>0 && n%231==0 ) {
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


            ui->cnt_load_addresses->setText(show_num(n));
            ui->cnt_persons->setText(show_num(cnt_persons));
            ui->cnt_persons_docs->setText(show_num(cnt_persons_docs));
            ui->cnt_persons_old->setText(show_num(cnt_persons_old));
            ui->cnt_vizits->setText(show_num(cnt_vizits));
            ui->cnt_events->setText(show_num(cnt_events));
            ui->cnt_polises->setText(show_num(cnt_polises));
            ui->cnt_blanks_vs->setText(show_num(cnt_blanks_vs));
            ui->cnt_blanks_pol->setText(show_num(cnt_blanks_pol));
            ui->cnt_blanks_uec->setText(show_num(cnt_blanks_uec));
            ui->cnt_spr_insure_errors->setText(show_num(cnt_spr_insure_errors));
            ui->cnt_spr_oksm->setText(show_num(cnt_spr_oksm));
            ui->cnt_spr_ocato->setText(show_num(cnt_spr_ocato));
            ui->cnt_spr_ocato_full->setText(show_num(cnt_spr_ocato_full));
            ui->cnt_addresses->setText(show_num(cnt_addresses));
            ui->cnt_assig_by_terr->setText(show_num(cnt_assig_by_terr));
            ui->cnt_persons_assig->setText(show_num(cnt_persons_assig));
            ui->cnt_spr_assig_errors->setText(show_num(cnt_spr_assig_errors));
            ui->cnt_spr_medorgs->setText(show_num(cnt_spr_medorgs));
            ui->cnt_spr_medparts->setText(show_num(cnt_spr_medparts));
            ui->cnt_spr_medsites->setText(show_num(cnt_spr_medsites));
            ui->cnt_spr_medters->setText(show_num(cnt_spr_medters));
            ui->cnt_spr_ms_r_mt->setText(show_num(cnt_spr_ms_r_mt));
            ui->cnt_spr_medsites_types->setText(show_num(cnt_spr_medsites_types));
            ui->cnt_spr_medters_spec->setText(show_num(cnt_spr_medters_spec));

            ui->lab_assig_by_terr->setEnabled(0<cnt_assig_by_terr);

            ui->cnt_assig_by_terr->setEnabled(0<cnt_assig_by_terr);

            QApplication::processEvents();
        }
    }

    // все записи уже обработаны
    ui->bn_load_addresses->setVisible(false);

    db_oldBase.commit();
    db.commit();

    ui->cnt_load_addresses->setText(show_num(n));

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  Готово (" + QString::number(n) + " строк) !");
    QApplication::processEvents();
    if (ui->ch_process_all->isChecked())
          QMessageBox::information(this, "Готово", "Все данные загружены.");
    else  QMessageBox::information(this, "Готово", "Все данные адресной базы загружены.\n\nОбработано " + QString::number(n) + " строк. ");


    ui->lay_load_table->setText("  - // -  ");
    ui->bn_close->setEnabled(true);
    ui->push_connect->setEnabled(true);
    ui->bn_load_tfoms->setEnabled(true);
    ui->bn_PersonsOld->setEnabled(true);
    ui->bn_clear_links->setEnabled(true);
    ui->bn_load_polises->setEnabled(true);
    ui->bn_reload_addres->setEnabled(true);
    ui->bn_close_polises->setEnabled(true);
    ui->bn_load_addresses->setEnabled(true);
    return true;
}


/*
void old_db_wnd::on_bn_load_assigs_clicked() {
    ui->pan_wait->setVisible(true);
    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  Загрузка среза прикрепления");
    QApplication::processEvents();

    bool res_load = try_load_assigs();
    if (!res_load) {
        ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  действие отменено!");
        QMessageBox::warning(this, "Действие отменено", "Действие отменено.\n");
    } else {
        QMessageBox::information(this, "Готово", "Данные среза прикреплений загружены.\n");
    }
    ui->pan_wait->setVisible(false);
    QApplication::processEvents();
}

bool old_db_wnd::try_load_assigs() {
    QFile f_log(data_app.path_out + "old_db.log");
    if (!f_log.open(QIODevice::Append | QIODevice::Text))
        QMessageBox::warning(this, "Ошибка открытия файла журнала", "Ошибка открытия файла журнала");
    else {
        QTextStream st_log(&f_log);
        st_log.setCodec("Windows-1251");
        st_log << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "  -  try_load_assigs()\n";
        f_log.close();
    }
    ui->bn_close->setEnabled(false);
    ui->push_connect->setEnabled(false);
    ui->bn_load_tfoms->setEnabled(false);
    ui->bn_PersonsOld->setEnabled(false);
    ui->bn_clear_links->setEnabled(false);
    ui->bn_load_polises->setEnabled(false);
    ui->bn_reload_addres->setEnabled(false);
    ui->bn_close_polises->setEnabled(false);
    ui->bn_load_addresses->setEnabled(false);
    QApplication::processEvents();

    date0 = QDate::currentDate();
    time0 = QTime::currentTime();

    //-------------------------------//
    // скопируем данные прикрепления //
    //-------------------------------//

    // получим список фондовских слоёв прикрепления
    QStringList tables = db_oldBase.tables();
    QStringList layers;
    foreach (QString tabname, tables) {
        QString s1 = tabname.left(20);
        QString s2 = tabname.right(7);
        if ( s1==QString("ASSIGNMENT_ALL_MO_20") &&
             s2==QString("01_FOND")) {
            int pos=0;
            for (pos=0; pos<layers.size(); pos++) {
                if (tabname<layers.at(pos))
                    break;
            }
            layers.insert(pos, tabname);
        }
    }

    bool has_unloaded_data = false;


    int cnt_max = 2;

    if (ui->rb_10->isChecked())  cnt_max = 20;
    else if (ui->rb_100->isChecked())  cnt_max = 200;
    else if (ui->rb_1000->isChecked())  cnt_max = 2000;
    else if (ui->rb_10000->isChecked())  cnt_max = 20000;
    else if (ui->rb_100000->isChecked())  cnt_max = 200000;
    else if (ui->rb_1000000->isChecked())  cnt_max = 2000000;
    else if (ui->rb_hand->isChecked())  cnt_max = ui->spin_hand->value();

    int cnt_step = 1;
    if (cnt_max>=10000)  cnt_step = 2341;
    else if (cnt_max>=1000)  cnt_step = 231;
    else if (cnt_max>=100)  cnt_step = 21;


    // для каждого слоя...
    foreach (QString layer, layers) {
        ui->lay_load_table->setText(layer);
        int year = layer.mid(18,4).toInt();
        int mnth = layer.mid(22,2).toInt();
        QDate assig_date = QDate(year, mnth, 1);
        QDate unsig_date = assig_date.addMonths(1).addDays(25);

        if (!f_log.open(QIODevice::Append | QIODevice::Text))
            QMessageBox::warning(this, "Ошибка открытия файла журнала", "Ошибка открытия файла журнала");
        else {
            QTextStream st_log(&f_log);
            st_log.setCodec("Windows-1251");
            st_log << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "  -  -  layer:  " << layer << "\n";
            f_log.close();
        }

        // подсчитаем существующие записи
        QSqlQuery *query_cnt = new QSqlQuery(db);
        query_cnt->exec("select count(*) from persons ; ");            query_cnt->next();     ui->cnt_persons_base->setText(            show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from persons_docs ; ");       query_cnt->next();     ui->cnt_persons_docs_base->setText(       show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from persons_old ; ");        query_cnt->next();     ui->cnt_persons_old_base->setText(        show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from vizits ; ");             query_cnt->next();     ui->cnt_vizits_base->setText(             show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from events ; ");             query_cnt->next();     ui->cnt_events_base->setText(             show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from polises ; ");            query_cnt->next();     ui->cnt_polises_base->setText(            show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from blanks_vs ; ");          query_cnt->next();     ui->cnt_blanks_vs_base->setText(          show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from blanks_pol ; ");         query_cnt->next();     ui->cnt_blanks_pol_base->setText(         show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from blanks_uec ; ");         query_cnt->next();     ui->cnt_blanks_uec_base->setText(         show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from spr_insure_errors ; ");  query_cnt->next();     ui->cnt_spr_insure_errors_base->setText(  show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from spr_oksm ; ");           query_cnt->next();     ui->cnt_spr_oksm_base->setText(           show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from spr_ocato ; ");          query_cnt->next();     ui->cnt_spr_ocato_base->setText(          show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from spr_ocato_full ; ");     query_cnt->next();     ui->cnt_spr_ocato_full_base->setText(     show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from addresses ; ");          query_cnt->next();     ui->cnt_addresses_base->setText(          show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from assig_by_terr ; ");      query_cnt->next();     ui->cnt_assig_by_terr_base->setText(      show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from persons_assig ; ");      query_cnt->next();     ui->cnt_persons_assig_base->setText(      show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from spr_assig_errors ; ");   query_cnt->next();     ui->cnt_spr_assig_errors_base->setText(   show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from spr_medorgs ; ");        query_cnt->next();     ui->cnt_spr_medorgs_base->setText(        show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from spr_medparts ; ");       query_cnt->next();     ui->cnt_spr_medparts_base->setText(       show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from spr_medparts ; ");       query_cnt->next();     ui->cnt_spr_medparts_base->setText(       show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from spr_medsites ; ");       query_cnt->next();     ui->cnt_spr_medsites_base->setText(       show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from spr_medters ; ");        query_cnt->next();     ui->cnt_spr_medters_base->setText(        show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from spr_ms_r_mt ; ");        query_cnt->next();     ui->cnt_spr_ms_r_mt_base->setText(        show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from spr_medsites_types ; "); query_cnt->next();     ui->cnt_spr_medsites_types_base->setText( show_num(query_cnt->value(0).toInt()));
        query_cnt->exec("select count(*) from spr_medters_spec ; ");   query_cnt->next();     ui->cnt_spr_medters_spec_base->setText(   show_num(query_cnt->value(0).toInt()));

        // подготовка
        date = QDate::currentDate();
        time = QTime::currentTime();
        ui->lab_time_all->setText("00:00:00");
        ui->lab_time->setText("00:00:00");
        QApplication::processEvents();

        ui->lab_persons->setEnabled(false);
        ui->lab_persons_docs->setEnabled(false);
        ui->lab_persons_old->setEnabled(false);
        ui->lab_vizits->setEnabled(false);
        ui->lab_events->setEnabled(false);
        ui->lab_polises->setEnabled(false);
        ui->lab_blanks_vs->setEnabled(false);
        ui->lab_blanks_pol->setEnabled(false);
        ui->lab_blanks_uec->setEnabled(false);
        ui->lab_spr_insure_errors->setEnabled(false);
        ui->lab_spr_oksm->setEnabled(false);
        ui->lab_spr_ocato->setEnabled(false);
        ui->lab_spr_ocato_full->setEnabled(false);
        ui->lab_addresses->setEnabled(false);
        ui->lab_assig_by_terr->setEnabled(false);
        ui->lab_persons_assig->setEnabled(false);
        ui->lab_spr_assig_errors->setEnabled(false);
        ui->lab_spr_medorgs->setEnabled(false);
        ui->lab_spr_medparts->setEnabled(false);
        ui->lab_spr_medsites->setEnabled(false);
        ui->lab_spr_medters->setEnabled(false);
        ui->lab_spr_ms_r_mt->setEnabled(false);
        ui->lab_spr_medsites_types->setEnabled(false);
        ui->lab_spr_medters_spec->setEnabled(false);

        ui->cnt_persons->setEnabled(false);
        ui->cnt_persons_docs->setEnabled(false);
        ui->cnt_persons_old->setEnabled(false);
        ui->cnt_vizits->setEnabled(false);
        ui->cnt_events->setEnabled(false);
        ui->cnt_polises->setEnabled(false);
        ui->cnt_blanks_vs->setEnabled(false);
        ui->cnt_blanks_pol->setEnabled(false);
        ui->cnt_blanks_uec->setEnabled(false);
        ui->cnt_spr_insure_errors->setEnabled(false);
        ui->cnt_spr_oksm->setEnabled(false);
        ui->cnt_spr_ocato->setEnabled(false);
        ui->cnt_spr_ocato_full->setEnabled(false);
        ui->cnt_addresses->setEnabled(false);
        ui->cnt_assig_by_terr->setEnabled(false);
        ui->cnt_persons_assig->setEnabled(false);
        ui->cnt_spr_assig_errors->setEnabled(false);
        ui->cnt_spr_medorgs->setEnabled(false);
        ui->cnt_spr_medparts->setEnabled(false);
        ui->cnt_spr_medsites->setEnabled(false);
        ui->cnt_spr_medters->setEnabled(false);
        ui->cnt_spr_ms_r_mt->setEnabled(false);
        ui->cnt_spr_medsites_types->setEnabled(false);
        ui->cnt_spr_medters_spec->setEnabled(false);

        QApplication::processEvents();

        int n = 0;

        QString sql_prep;
        // проверим, есть ли что грузить
        sql_prep  =  "select count(*) "
                     "  from " + layer + " a "
                     " where a.import is NULL ; ";

        QSqlQuery *query_prep = new QSqlQuery(db_oldBase);
        bool res_prep = query_prep->exec(sql_prep);
        query_prep->next();
        int cnt_prep = query_prep->value(0).toInt();
        delete query_prep;

        if (cnt_prep<1 && !ui->ch_process_all->isChecked()) {
            continue;
        } else {
            has_unloaded_data = true;
        }

        //-----------------------------//
        // скопируем снимок данных прикрепления //
        //-----------------------------//

        // запрос данных
        QString sql;
        sql  =  "select first " + QString::number(cnt_max) + " "
                "       a.id, a.fam, a.im, a.ot, a.dat_r, a.sex, a.oms_rights, a.blank_ser, a.blank_nom, a.temp_nom, a.enp, a.snils, "
                // прикрепление
                "       a.code_mo, a.oid_lpu, a.code_mp, a.code_ms, a.snils_mt, a.code_spec, "
                "       a.assig_date, a.assig_type, a.unsig_date, "
                // появление и правка записи
                "       a.dt_ins, a.dt_upd "
                "  from " + layer + " a "
                " where a.import is NULL "
                " order by id ; ";

        // заготовим переменные
        int id;
        QString fam, im, ot;
        QDate dat_r;
        QString pol;
        int sex, pol_v;
        QString oms_rights, blank_ser, blank_num, vs_num, enp, snils;
        // прикрепление
        QString code_mo, oid_lpu, code_mp, code_ms, snils_mt;
        int code_spec, assig_type;
        QDate assig_date1, unsig_date1;
        QDateTime dt_ins, dt_upd;

        __int64 id_assig;

        while (true) {
            db_oldBase.transaction();
            db.transaction();

            // выберем порцию данных
            QSqlQuery *query = new QSqlQuery(db_oldBase);
            bool res = query->exec(sql);

            int n1 = 0;

            if (!res) {
                QMessageBox::warning(this, "Ошибка чтения из таблицы " + layer,
                                     "При попытке прочитать данные из таблицы " + layer + " произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных из этой таблицы отменена!");
                delete query;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }

            n1 = 0;

            while (query->next()) {
                //if (n1==0)
                //    size = query->size();

                //if (n1>=size)
                //    break;

                // прочитаем данные
                id            = query->value( 0).toInt();
                fam           = query->value( 1).toString().trimmed().replace("'","").replace("\"","").simplified().toUpper();
                im            = query->value( 2).toString().trimmed().replace("'","").replace("\"","").simplified().toUpper();
                ot            = query->value( 3).toString().trimmed().replace("'","").replace("\"","").simplified().toUpper();
                dat_r         = query->value( 4).toDate();
                pol           = query->value( 5).toString().replace("'","").replace("\"","").trimmed();
                sex           = (pol.left(1)=="М" ? 1 : 2);
                oms_rights    = query->value( 6).toString().trimmed().replace("'","").replace("\"","").simplified();
                blank_ser     = query->value( 7).toString().trimmed().replace("'","").replace("\"","").simplified().right(4);
                blank_num     = query->value( 8).toString().trimmed().replace("'","").replace("\"","").simplified().right(16);
                vs_num        = query->value( 9).toString().trimmed().replace("'","").replace("\"","").simplified().right(16);
                enp           = query->value(10).toString().trimmed().replace(" ","").replace("'","").replace("\"","").replace("-","").right(16);
                snils         = query->value(11).toString().trimmed().replace(" ","").replace("'","").replace("\"","").replace("-","").right(11);
                // прикрепление
                code_mo       = QString("000000" + query->value(12).toString()).right(6);
                oid_lpu       = query->value(13).toString().trimmed().replace("'","").replace("\"","");
                code_mp       = query->value(14).toString().trimmed().replace("'","").replace("\"","");
                code_ms       = query->value(15).toString().trimmed().replace("'","").replace("\"","").right(24);
                snils_mt      = query->value(16).toString().trimmed().replace(" ","").replace("'","").replace("\"","").replace("-","").right(11);
                code_spec     = query->value(17).toInt();
                assig_date1   = query->value(18).toDate();
                assig_type    = query->value(19).toInt();
                unsig_date1   = query->value(20).toDate();
                dt_ins        = query->value(21).toDateTime();
                dt_upd        = query->value(22).toDateTime();

                if (!assig_date1.isValid()) assig_date1 = assig_date;
                if (oms_rights=="С")  pol_v = 1;  else if (oms_rights=="В")  pol_v = 2;  else pol_v = 3;

                id_assig = -1;


                //------------------------------------//
                // найдём/добавим запись прикрепления //
                //------------------------------------//
                QString sql_assig;
m_ret_assig:
                QSqlQuery *query_assig = new QSqlQuery(db);
                sql_assig = "select add_or_find_assig "
                            " ( cast('" + fam        + "' as varchar), "
                            "   cast('" + im         + "' as varchar), "
                            "   cast('" + ot         + "' as varchar), "
                            "   cast( " + (dat_r.isValid() ? QString("'" + dat_r.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                            "   cast( " + QString::number(sex)   + " as smallint), "
                            "   cast('" + snils      + "' as varchar), "
                            "   cast('" + enp        + "' as varchar), "
                            "   cast( " + QString::number(pol_v) + " as smallint), "
                            "   cast('" + blank_ser  + "' as varchar), "
                            "   cast('" + blank_num  + "' as varchar), "
                            "   cast('" + vs_num     + "' as varchar), "
                            "   cast('" + code_mo    + "' as varchar), "
                            "   cast('" + oid_lpu    + "' as varchar), "
                            "   cast('" + code_mp    + "' as varchar), "
                            "   cast('" + code_ms    + "' as varchar), "
                            "   cast('" + snils_mt   + "' as varchar), "
                            "   cast( " + QString::number(code_spec)         + "  as integer), "
                            "   cast('" + assig_date1.toString("yyyy-MM-dd") + "' as date), "
                            "   cast( " + QString::number(assig_type)        + "  as smallint), "
                            "   cast( " + (unsig_date1.isValid()? QString("'" + unsig_date1.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                            "   'ТФОМС, подтверждённый снимок данных ИНКО', "
                            "   cast( " + (dt_ins.isValid() ? QString("'" + dt_ins.toString("yyyy-MM-dd hh:mm:ss") + "'") : "NULL") + " as timestamp without time zone), "
                            "   cast( " + (dt_upd.isValid() ? QString("'" + dt_upd.toString("yyyy-MM-dd hh:mm:ss") + "'") : "NULL") + " as timestamp without time zone)  "
                            " ); ";
                bool res_assig = query_assig->exec(sql_assig);

                if ( !res_assig ) {
                    if ( QMessageBox::warning(this, "Ошибка добавления записи прикрепления персоны",
                                              "При попытке добавить запись прикрепления персоны произошла неожиданная ошибка.\n\n"
                                              "n = " + QString::number(n) + "\n\n"
                                              + query_assig->lastError().driverText() + "\n" + query_assig->lastError().databaseText() + "\n\n"
                                              "Повторить попытку ? ", QMessageBox::Retry|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Retry ) {
                        delete query_assig;
                        goto m_ret_assig;

                    } else {

                        delete query_assig;

                        db_oldBase.rollback();
                        db.rollback();

                        ui->bn_close->setEnabled(true);
                        ui->push_connect->setEnabled(true);
                        ui->bn_load_tfoms->setEnabled(true);
                        ui->bn_PersonsOld->setEnabled(true);
                        ui->bn_clear_links->setEnabled(true);
                        ui->bn_load_polises->setEnabled(true);
                        ui->bn_reload_addres->setEnabled(true);
                        ui->bn_close_polises->setEnabled(true);
                        ui->bn_load_addresses->setEnabled(true);
                        return false;
                    }
                }
                query_assig->next();

                id_assig = query_assig->value(0).toInt();
                if (id_assig>0) {
                    // добавлена новая запись прикрепления персоны
                    cnt_persons_assig++;
                } else {
                    // !!! ошибка поиска/добавления записи !!!
                }
                delete query_assig;

                n++;

                if ( n>0 && n%cnt_step==0 ) {
                    //size = query->size();
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

                    ui->cnt_load_assigs->setText(show_num(n));
                    ui->cnt_persons->setText(show_num(cnt_persons));
                    ui->cnt_persons_docs->setText(show_num(cnt_persons_docs));
                    ui->cnt_persons_old->setText(show_num(cnt_persons_old));
                    ui->cnt_vizits->setText(show_num(cnt_vizits));
                    ui->cnt_events->setText(show_num(cnt_events));
                    ui->cnt_polises->setText(show_num(cnt_polises));
                    ui->cnt_blanks_vs->setText(show_num(cnt_blanks_vs));
                    ui->cnt_blanks_pol->setText(show_num(cnt_blanks_pol));
                    ui->cnt_blanks_uec->setText(show_num(cnt_blanks_uec));
                    ui->cnt_spr_insure_errors->setText(show_num(cnt_spr_insure_errors));
                    ui->cnt_spr_oksm->setText(show_num(cnt_spr_oksm));
                    ui->cnt_spr_ocato->setText(show_num(cnt_spr_ocato));
                    ui->cnt_spr_ocato_full->setText(show_num(cnt_spr_ocato_full));
                    ui->cnt_addresses->setText(show_num(cnt_addresses));
                    ui->cnt_assig_by_terr->setText(show_num(cnt_assig_by_terr));
                    ui->cnt_persons_assig->setText(show_num(cnt_persons_assig));
                    ui->cnt_spr_assig_errors->setText(show_num(cnt_spr_assig_errors));
                    ui->cnt_spr_medorgs->setText(show_num(cnt_spr_medorgs));
                    ui->cnt_spr_medparts->setText(show_num(cnt_spr_medparts));
                    ui->cnt_spr_medsites->setText(show_num(cnt_spr_medsites));
                    ui->cnt_spr_medters->setText(show_num(cnt_spr_medters));
                    ui->cnt_spr_ms_r_mt->setText(show_num(cnt_spr_ms_r_mt));
                    ui->cnt_spr_medsites_types->setText(show_num(cnt_spr_medsites_types));
                    ui->cnt_spr_medters_spec->setText(show_num(cnt_spr_medters_spec));

                    ui->lab_persons_assig->setEnabled(0<cnt_persons_assig);

                    ui->cnt_persons_assig->setEnabled(0<cnt_persons_assig);

                    if (!f_log.open(QIODevice::Append | QIODevice::Text))
                        QMessageBox::warning(this, "Ошибка открытия файла журнала", "Ошибка открытия файла журнала");
                    else {
                        QTextStream st_log(&f_log);
                        st_log.setCodec("Windows-1251");
                        st_log << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "  -  -  -  " << "  -  " << QString::number(n) << "\n";
                        f_log.close();
                    }

                    QApplication::processEvents();
                }
                n1++;
            }
            //int cnt_all = query->size();
            delete query;


            //------------------------------------------------//
            // запомним во внешней базе факт обработки записи //
            //------------------------------------------------//
            QString sql_link;
            sql_link  = "update " + layer + " "
                        "   set import=1 "
                        " where import is NULL "
                        "   and id<=" + QString::number(id) + " ; ";
            QSqlQuery *query_link = new QSqlQuery(db_oldBase);
            bool res_link = query_link->exec(sql_link);

            if (!res_link) {
                delete query_link;
                // без проверок
            }
            delete query_link;

            db_oldBase.commit();
            db.commit();

            db_oldBase.transaction();
            db.transaction();

            if (n1<cnt_max-1) {
                // все записи слоя обработаны - это был последний фрагмент
                if (!f_log.open(QIODevice::Append | QIODevice::Text))
                    QMessageBox::warning(this, "Ошибка открытия файла журнала", "Ошибка открытия файла журнала");
                else {
                    QTextStream st_log(&f_log);
                    st_log.setCodec("Windows-1251");
                    st_log << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "  -  -  снимок данных загружен\n";
                    f_log.close();
                }

                break;
            }
        }
        // всё что можно уже загружено
        // пометим не загруженные строки слоя как мусор
        QString sql_trash;
        sql_trash  = "update " + layer + " "
                     "   set import=-1 "
                     " where import is NULL ; ";
        QSqlQuery *query_trash = new QSqlQuery(db_oldBase);
        bool res_trash = query_trash->exec(sql_trash);
        delete query_trash;

    }
    if (has_unloaded_data) {
        // все записи уже обработаны
        if (!f_log.open(QIODevice::Append | QIODevice::Text))
            QMessageBox::warning(this, "Ошибка открытия файла журнала", "Ошибка открытия файла журнала");
        else {
            QTextStream st_log(&f_log);
            st_log.setCodec("Windows-1251");
            st_log << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "  -  все данные прикрепления загружены\n";
            f_log.close();
        }

    }
    db_oldBase.commit();
    db.commit();

    ui->bn_load_assigs->setVisible(false);
    QMessageBox::information(this, "Готово",
                             "Все данные прикрепления загружены.\n\n"
                             "Обработано " + QString::number(layers.count()) + " срезов данных. ");
    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  Готово (" + QString::number(layers.count()) + " срезов данных) !");
    QApplication::processEvents();

    ui->lay_load_table->setText("  - // -  ");
    ui->bn_close->setEnabled(true);
    ui->push_connect->setEnabled(true);
    ui->bn_load_tfoms->setEnabled(true);
    ui->bn_PersonsOld->setEnabled(true);
    ui->bn_load_assigs->setEnabled(true);
    ui->bn_clear_links->setEnabled(true);
    ui->bn_load_polises->setEnabled(true);
    ui->bn_reload_addres->setEnabled(true);
    ui->bn_close_polises->setEnabled(true);
    ui->bn_load_addresses->setEnabled(true);
    return true;
}
*/


void old_db_wnd::on_bn_load_tfoms_clicked() {
    ui->pan_wait->setVisible(true);
    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  Загрузка файла изменений от ТФОМС");
    QApplication::processEvents();

    int res_load = try_load_tfoms();
    if (!res_load) {
        ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  действие отменено!");
        QMessageBox::warning(this, "Действие отменено", "Действие отменено.\n");
    }
    ui->pan_wait->setVisible(false);
    QApplication::processEvents();
}

int old_db_wnd::try_load_tfoms() {
    ui->bn_close->setEnabled(false);
    ui->push_connect->setEnabled(false);
    ui->bn_load_tfoms->setEnabled(false);
    ui->bn_PersonsOld->setEnabled(false);
    ui->bn_clear_links->setEnabled(false);
    ui->bn_load_polises->setEnabled(false);
    ui->bn_reload_addres->setEnabled(false);
    ui->bn_close_polises->setEnabled(false);
    ui->bn_load_addresses->setEnabled(false);
    QApplication::processEvents();

    date0 = QDate::currentDate();
    time0 = QTime::currentTime();

    // подготовка
    date = QDate::currentDate();
    time = QTime::currentTime();
    ui->lab_time_all->setText("00:00:00");
    ui->lab_time->setText("00:00:00");
    QApplication::processEvents();

    ui->lay_load_table->setText("Загрузка CSV-файла ТФОМС");
    ui->lab_persons->setEnabled(false);
    ui->lab_persons_docs->setEnabled(false);
    ui->lab_persons_old->setEnabled(false);
    ui->lab_vizits->setEnabled(false);
    ui->lab_events->setEnabled(false);
    ui->lab_polises->setEnabled(false);
    ui->lab_blanks_vs->setEnabled(false);
    ui->lab_blanks_pol->setEnabled(false);
    ui->lab_blanks_uec->setEnabled(false);
    ui->lab_spr_insure_errors->setEnabled(false);
    ui->lab_spr_oksm->setEnabled(false);
    ui->lab_spr_ocato->setEnabled(false);
    ui->lab_spr_ocato_full->setEnabled(false);
    ui->lab_addresses->setEnabled(false);
    ui->lab_assig_by_terr->setEnabled(false);
    ui->lab_persons_assig->setEnabled(false);
    ui->lab_spr_assig_errors->setEnabled(false);
    ui->lab_spr_medorgs->setEnabled(false);
    ui->lab_spr_medparts->setEnabled(false);
    ui->lab_spr_medsites->setEnabled(false);
    ui->lab_spr_medters->setEnabled(false);
    ui->lab_spr_ms_r_mt->setEnabled(false);
    ui->lab_spr_medsites_types->setEnabled(false);
    ui->lab_spr_medters_spec->setEnabled(false);

    ui->cnt_persons->setEnabled(false);
    ui->cnt_persons_docs->setEnabled(false);
    ui->cnt_persons_old->setEnabled(false);
    ui->cnt_vizits->setEnabled(false);
    ui->cnt_events->setEnabled(false);
    ui->cnt_polises->setEnabled(false);
    ui->cnt_blanks_vs->setEnabled(false);
    ui->cnt_blanks_pol->setEnabled(false);
    ui->cnt_blanks_uec->setEnabled(false);
    ui->cnt_spr_insure_errors->setEnabled(false);
    ui->cnt_spr_oksm->setEnabled(false);
    ui->cnt_spr_ocato->setEnabled(false);
    ui->cnt_spr_ocato_full->setEnabled(false);
    ui->cnt_addresses->setEnabled(false);
    ui->cnt_assig_by_terr->setEnabled(false);
    ui->cnt_persons_assig->setEnabled(false);
    ui->cnt_spr_assig_errors->setEnabled(false);
    ui->cnt_spr_medorgs->setEnabled(false);
    ui->cnt_spr_medparts->setEnabled(false);
    ui->cnt_spr_medsites->setEnabled(false);
    ui->cnt_spr_medters->setEnabled(false);
    ui->cnt_spr_ms_r_mt->setEnabled(false);
    ui->cnt_spr_medsites_types->setEnabled(false);
    ui->cnt_spr_medters_spec->setEnabled(false);


    //------------------------------//
    // Запросим фондовский файл CSV //
    //------------------------------//
    QStringList file_names_lst = QFileDialog::getOpenFileNames(this, "Выберите CSV-файл (файлы) от ТФОМС", "d:/temp", "Текстовые файлы данных (*.csv *.txt)");

    if (file_names_lst.isEmpty()) {
        QMessageBox::warning(this, "Файлы не выбраны", "Операция отменена пользователем.");
        return 0;
    }

    // переберём файлы в списке
    int cnt_files = 0;
    int cnt_recs  = 0;
    int cnt_wrong = 0;
    foreach(QString file_name, file_names_lst) {
        int cnt_recs2 = try_load_file(file_name);
        if (cnt_recs2>=0) {
            cnt_files++;
            cnt_recs += cnt_recs2;
        } else cnt_wrong++;

        ui->cnt_files_tfoms_wrn->setText(show_num(cnt_files));
        ui->cnt_files_tfoms_cor->setText(show_num(cnt_wrong));
        ui->cnt_load_tfoms->setText(show_num(cnt_recs));
        QApplication::processEvents();
    }

    ui->cnt_files_tfoms_wrn->setText(show_num(cnt_files));
    ui->cnt_files_tfoms_cor->setText(show_num(cnt_wrong));
    ui->cnt_load_tfoms->setText(show_num(cnt_recs));
    QApplication::processEvents();

    QMessageBox::information ( this, "Готово",
                               "Закружено " + QString::number(cnt_recs) + " строк данных из " + QString::number(cnt_files) + " файлов.\n" +
                               (cnt_wrong>0 ? "Ошибка загрузки " + QString::number(cnt_wrong) + " файлов.\n" : "") );

    return cnt_files;
}


int old_db_wnd::try_load_file(QString file_name) {
    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  " + file_name);
    QApplication::processEvents();

    // выделим имя файла и папку
    QFileInfo fi(file_name);
    QString base_name = fi.baseName() + "." + fi.completeSuffix();
    QString file_path = fi.absoluteFilePath();
    file_path = file_path.left(file_path.length() - base_name.length());

    QStringList data_list;

    if ( base_name.left(6)!="i36065"
         && base_name.left(8)!="MO136065"
         && base_name.left(23)!="_Прикрепление_населения" ) {
        if (QMessageBox::warning(this, "Неправильное имя файла",
                                 "Имя файла " + base_name + ".CSV не соответствует маске: \n\n"
                                 "i36065yyyymmdd.csv\n"
                                 "MO136065yymm * .csv\n"
                                 "_Прикрепление_населения * .csv\n"
                                 "\nВсё равно заргузить?\n(в случае отказа файл будет пропущен)",
                                 QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
            ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  !! Неправильное имя файла !!");
            QApplication::processEvents();
            return -1;
        }
    }

    // пробуем открыть файл
    QFile file(file_name);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, "Ошибка открытия файла",
                             "При попытке открыть выбранный файл произошла ошибка. "
                             "\n\nОперация отменена.");
        ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  !! Ошибка открытия файла !!");
        QApplication::processEvents();
        return -1;
    }

    //------------------------------//
    // Загрузим фондовский файл CSV //
    //------------------------------//
    db.transaction();

    // сформируем поток
    QTextStream stream(&file);
    stream.setCodec (QTextCodec::codecForName("Windows-1251"));

    // прочитаем первую строку - она нам не нужна
    bool f_short_data = false;
    QString values_str = stream.readLine().replace("\'\'","\"").replace("\'","\"");
    if (values_str=="\"No\";\"Код\";\"Ф.И.О.\";\"Д.р.\";\"СМО\";\"ЕНП\";\"МО\";\"Ф.И.О. врача\";\"СНИЛС врача\";\"Участок\";\"Тип прикр\";\"Дата прикр\";\"Изменение внес\";\"Дата изм.\";")
        f_short_data = true;

    // перебор строк файла
    QString div_fields = ";";
    QString rnd_fields = "\"";
    int n = 0;
    cnt_load_tfoms = 0;
    cnt_load_tfoms_2 = 0;

    data_list.clear();
    int load_counter = 0;

    while (true) {
        data_list.clear();
        values_str = stream.readLine().replace("\'\'","\"").replace("\'","\"");
        if (values_str.isEmpty())  break;

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

            value = value.replace("\"", "").replace("'", "");

            // добавим значение
            data_list.append(value);

            if (pos1<0 || pos1==len)  break;
            else  pos = pos1 + div_fields.length();

            n_value++;
        }
        load_counter++;
        cnt_load_tfoms++;

        QString fio;
        QString fam;
        QString im;
        QString ot;
        int     sex;
        QString s_d1;
        QDate   dat_r;
        QString enp;
        int     assig_type;
        QString s_d2;
        QDate   assig_date;
        QString code_mo;
        QString oid_lpu;
        QString code_mp;
        QString code_ms;
        QString snils_mt;
        int     code_spec;
        QString source;

        if (f_short_data) {
            // прочитаем данные в сокращённом формате
            fio         = data_list.at( 2).trimmed().toUpper().replace("Ё","Е");
            int pos0 = fio.indexOf(" ");
            int pos1 = fio.indexOf(" ", pos0+1);
            int pos2 = fio.length();
            fam         = fio.left(pos0);
            im          = fio.mid(pos0+1, pos1-pos0-1);
            ot          = fio.mid(pos1+1, pos2-pos1);
            sex         = 0;
            s_d1        = data_list.at( 3);
            dat_r       = QDate::fromString(s_d1, "dd.MM.yyyy");
            enp         = clear_num(data_list.at( 5));
            assig_type  = data_list.at(10).toInt();
            s_d2        = data_list.at(11);
            assig_date  = QDate::fromString(s_d2, "dd.MM.yyyy");
            code_mo     = data_list.at( 6).left(6);
            oid_lpu     = "";
            code_mp     = "";
            code_ms     = data_list.at( 9);
            snils_mt    = clear_num(data_list.at( 8));
            code_spec   = 0;
            source      = "Скопировано из ИСЗЛ";
        } else {
            fam         = data_list.at( 1).trimmed().toUpper().replace("Ё","Е");
            im          = data_list.at( 2).trimmed().toUpper().replace("Ё","Е");
            ot          = data_list.at( 3).trimmed().toUpper().replace("Ё","Е");
            sex         = data_list.at( 4).toInt();
            s_d1        = data_list.at( 5);
            s_d1        = s_d1.left(4) + "-" + s_d1.left(6).right(2) + "-" + s_d1.right(2);
            dat_r       = QDate::fromString(s_d1, "yyyy-MM-dd");
            enp         = clear_num(data_list.at( 8));
            assig_type  = data_list.at( 9).toInt();
            s_d2        = data_list.at(11);
            s_d2 = s_d2.left(4) + "-" + s_d2.left(6).right(2) + "-" + s_d2.right(2);
            assig_date  = QDate::fromString(s_d2, "yyyy-MM-dd");
            code_mo     = data_list.at(13);
            oid_lpu     = data_list.at(14);
            code_mp     = data_list.at(15);
            code_ms     = data_list.at(16);
            snils_mt    = clear_num(data_list.at(17));
            code_spec   = data_list.at(18).toInt();
            source      = "ТФОМС - файл изменений";
        }

        int id_assig = -1;

        //------------------------------------//
        // найдём/добавим запись прикрепления //
        //------------------------------------//
        QString sql_assig;
        sql_assig = "select add_or_find_assig  "
                    " ( cast('" + fam        + "' as varchar), "
                    "   cast('" + im         + "' as varchar), "
                    "   cast('" + ot         + "' as varchar), "
                    "   cast("  + (dat_r.isValid() ? QString("'" + dat_r.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                    "   cast("  + QString::number(sex)   + " as smallint), "
                    "   cast(NULL as varchar), "
                    "   cast('" + enp        + "' as varchar), "
                    "   cast(NULL as smallint), "
                    "   cast(NULL as varchar), "
                    "   cast(NULL as varchar), "
                    "   cast(NULL as varchar), "
                    "   cast('" + code_mo    + "' as varchar), "
                    "   cast('" + oid_lpu    + "' as varchar), "
                    "   cast('" + code_mp    + "' as varchar), "
                    "   cast('" + code_ms    + "' as varchar), "
                    "   cast('" + snils_mt   + "' as varchar), "
                    "   " + QString::number(code_spec) + ", "
                    "   cast("  + (assig_date.isValid() ? QString("'" + assig_date.toString("yyyy-MM-dd") + "'") : "NULL") + " as date), "
                    "   cast("  + QString::number(assig_type) + " as smallint), "
                    "   cast(NULL as date), "
                    "   cast('" + source.replace("'","") + "' as varchar), "
                    "   cast("  + ("'" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "'") + " as timestamp), "
                    "   cast("  + ("'" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "'") + " as timestamp)  "
                    " ); ";
        QSqlQuery *query_assig = new QSqlQuery(db);
        bool res_assig = query_assig->exec(sql_assig);

        if (!res_assig) {
            QMessageBox::warning(this, "Ошибка добавления записи прикрепления персоны",
                                 "При попытке добавить запись прикрепления персоны произошла неожиданная ошибка.\n\n"
                                 "n = " + QString::number(n) + "\n\n"
                                 + query_assig->lastError().driverText() + "\n" + query_assig->lastError().databaseText() + "\n\n"
                                 "Загрузка отменена. Файл пропущен.");
            delete query_assig;
            //db_oldBase.rollback();
            db.rollback();
            file.close();
            ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  !! Ошибка добавления записи " + " n = " + QString::number(n) + " !!");
            QApplication::processEvents();
            return -1;
        }
        query_assig->next();

        id_assig = query_assig->value(0).toInt();
        if (id_assig>0) {
            // найдена готовая запись прикрепления персоны
        } else if (id_assig<0) {
            // добавлена новая запись прикрепления персоны
            id_assig = 0 - id_assig;
            cnt_persons_assig++;
        } else {
            // !!! ошибка поиска/добавления записи !!!
        }
        delete query_assig;

        n++;

        if ((n%231)==0) {
            //------------------------------//
            // отображение статуса на экран //
            //------------------------------//
            ui->cnt_load_tfoms->setText(show_num(n));

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

            ui->cnt_persons->setText(show_num(cnt_persons));
            ui->cnt_persons_docs->setText(show_num(cnt_persons_docs));
            ui->cnt_persons_old->setText(show_num(cnt_persons_old));
            ui->cnt_vizits->setText(show_num(cnt_vizits));
            ui->cnt_events->setText(show_num(cnt_events));
            ui->cnt_polises->setText(show_num(cnt_polises));
            ui->cnt_blanks_vs->setText(show_num(cnt_blanks_vs));
            ui->cnt_blanks_pol->setText(show_num(cnt_blanks_pol));
            ui->cnt_blanks_uec->setText(show_num(cnt_blanks_uec));
            ui->cnt_spr_insure_errors->setText(show_num(cnt_spr_insure_errors));
            ui->cnt_spr_oksm->setText(show_num(cnt_spr_oksm));
            ui->cnt_spr_ocato->setText(show_num(cnt_spr_ocato));
            ui->cnt_spr_ocato_full->setText(show_num(cnt_spr_ocato_full));
            ui->cnt_addresses->setText(show_num(cnt_addresses));
            ui->cnt_assig_by_terr->setText(show_num(cnt_assig_by_terr));
            ui->cnt_persons_assig->setText(show_num(cnt_persons_assig));
            ui->cnt_spr_assig_errors->setText(show_num(cnt_spr_assig_errors));
            ui->cnt_spr_medorgs->setText(show_num(cnt_spr_medorgs));
            ui->cnt_spr_medparts->setText(show_num(cnt_spr_medparts));
            ui->cnt_spr_medsites->setText(show_num(cnt_spr_medsites));
            ui->cnt_spr_medters->setText(show_num(cnt_spr_medters));
            ui->cnt_spr_ms_r_mt->setText(show_num(cnt_spr_ms_r_mt));
            ui->cnt_spr_medsites_types->setText(show_num(cnt_spr_medsites_types));
            ui->cnt_spr_medters_spec->setText(show_num(cnt_spr_medters_spec));

            ui->lab_spr_insure_errors->setEnabled(0<cnt_spr_insure_errors);
            ui->lab_spr_oksm->setEnabled(0<cnt_spr_oksm);
            ui->lab_spr_ocato->setEnabled(0<cnt_spr_ocato);
            ui->lab_spr_ocato_full->setEnabled(0<cnt_spr_ocato_full);
            ui->lab_assig_by_terr->setEnabled(0<cnt_assig_by_terr);
            ui->lab_persons_assig->setEnabled(0<cnt_persons_assig);
            ui->lab_spr_assig_errors->setEnabled(0<cnt_spr_assig_errors);
            ui->lab_spr_medorgs->setEnabled(0<cnt_spr_medorgs);
            ui->lab_spr_medparts->setEnabled(0<cnt_spr_medparts);
            ui->lab_spr_medsites->setEnabled(0<cnt_spr_medsites);
            ui->lab_spr_medters->setEnabled(0<cnt_spr_medters);
            ui->lab_spr_ms_r_mt->setEnabled(0<cnt_spr_ms_r_mt);
            ui->lab_spr_medsites_types->setEnabled(0<cnt_spr_medsites_types);
            ui->lab_spr_medters_spec->setEnabled(0<cnt_spr_medters_spec);

            ui->cnt_spr_insure_errors->setEnabled(0<cnt_spr_insure_errors);
            ui->cnt_spr_oksm->setEnabled(0<cnt_spr_oksm);
            ui->cnt_spr_ocato->setEnabled(0<cnt_spr_ocato);
            ui->cnt_spr_ocato_full->setEnabled(0<cnt_spr_ocato_full);
            ui->cnt_assig_by_terr->setEnabled(0<cnt_assig_by_terr);
            ui->cnt_persons_assig->setEnabled(0<cnt_persons_assig);
            ui->cnt_spr_assig_errors->setEnabled(0<cnt_spr_assig_errors);
            ui->cnt_spr_medorgs->setEnabled(0<cnt_spr_medorgs);
            ui->cnt_spr_medparts->setEnabled(0<cnt_spr_medparts);
            ui->cnt_spr_medsites->setEnabled(0<cnt_spr_medsites);
            ui->cnt_spr_medters->setEnabled(0<cnt_spr_medters);
            ui->cnt_spr_ms_r_mt->setEnabled(0<cnt_spr_ms_r_mt);
            ui->cnt_spr_medsites_types->setEnabled(0<cnt_spr_medsites_types);
            ui->cnt_spr_medters_spec->setEnabled(0<cnt_spr_medters_spec);

            QApplication::processEvents();

            db.commit();
            db.transaction();
        }
    }
    db.commit();
    file.close();

    ui->bn_close->setEnabled(true);
    ui->push_connect->setEnabled(true);
    ui->bn_load_tfoms->setEnabled(true);
    ui->bn_PersonsOld->setEnabled(true);
    ui->bn_clear_links->setEnabled(true);
    ui->bn_load_polises->setEnabled(true);
    ui->bn_reload_addres->setEnabled(true);
    ui->bn_close_polises->setEnabled(true);
    ui->bn_load_addresses->setEnabled(true);

    // если надо - добавим папку ОБРАБОТАНО
    QString s_save = file_path + "/ОБРАБОТАНО " + QDate::currentDate().toString("yyyy-MM-dd");
    QDir dir_save(s_save);
    if (!dir_save.exists())
        dir_save.mkdir(s_save);
    // перенесём обработанный файл в папку ОБРАБОТАНО
    QFile::copy(file_name, s_save + "/" + base_name);
    QFile::remove(file_name);

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  Готово (" + QString::number(n) + " строк) !");
    QApplication::processEvents();
    return n;
}



void old_db_wnd::on_rb_1000000_clicked(bool checked) {
    ui->spin_hand->setEnabled(ui->rb_hand->isChecked());
}
void old_db_wnd::on_rb_100000_clicked(bool checked) {
    ui->spin_hand->setEnabled(ui->rb_hand->isChecked());
}
void old_db_wnd::on_rb_10000_clicked(bool checked) {
    ui->spin_hand->setEnabled(ui->rb_hand->isChecked());
}
void old_db_wnd::on_rb_1000_clicked(bool checked) {
    ui->spin_hand->setEnabled(ui->rb_hand->isChecked());
}
void old_db_wnd::on_rb_100_clicked(bool checked) {
    ui->spin_hand->setEnabled(ui->rb_hand->isChecked());
}
void old_db_wnd::on_rb_10_clicked(bool checked) {
    ui->spin_hand->setEnabled(ui->rb_hand->isChecked());
}
void old_db_wnd::on_rb_hand_clicked(bool checked) {
    ui->spin_hand->setEnabled(ui->rb_hand->isChecked());
}




void old_db_wnd::on_bn_FIAS2KLADR_clicked() {
    // выберем все адреса без кода КЛАДР
    QString sql_no_kladr;
    sql_no_kladr  = "select id, fias_aoid, fias_houseid "
                    "  from addresses "
                    " where ( (subj is NULL or subj=0) "
                    "      or ((city is NULL or city=0) and (nasp is NULL or nasp=0)) ) "
                    "   and ( fias_aoid is not NULL ) ; ";
    QSqlQuery *query_no_kladr = new QSqlQuery(db);
    bool res_no_kladr = query_no_kladr->exec(sql_no_kladr);

    int n = 0;
    while (query_no_kladr->next()) {
        int id = query_no_kladr->value(0).toInt();
        QString fias_aoid = query_no_kladr->value(1).toString();
        QString fias_houseid = query_no_kladr->value(2).toString();
        //------------------------------ //
        // коррекция кодов КЛАДР по ФИАС //
        //------------------------------ //
        if (!fias_aoid.isEmpty()) {
            // попробуем найти данные КЛАДР по базе ФИАС
            QString sql_fias_aoid;
            sql_fias_aoid  = "select code "
                             "  from addrobj "
                             " where aoid = cast('" + fias_aoid + "' as uuid) "
                             " limit 1 ; ";
            QSqlQuery *query_fias_aoid = new QSqlQuery(db_FIAS);
            bool res_fias_aoid = query_fias_aoid->exec(sql_fias_aoid);

            if (query_fias_aoid->next()) {
                QString code = query_fias_aoid->value(0).toString();
                if (!code.isEmpty()) {
                    QString region_code = code.mid( 0, 2);
                    QString area_code   = code.mid( 2, 3);
                    QString city_code   = code.mid( 5, 3);
                    QString place_code  = code.mid( 8, 3);
                    QString street_code = code.mid(11, 4);
                    QString act_code    = code.mid(15, 2);
                    // получим коды по КЛАДР и названия
                    QString subj_kladr = region_code + "000000000" + act_code;
                    QString dstr_kladr = region_code + area_code + "000000" + act_code;
                    QString nasp_kladr = region_code + area_code + city_code + place_code + act_code;
                    QString strt_kladr = region_code + area_code + city_code + place_code + street_code +  act_code;

                    QString sql_save;
                    sql_save = "update addresses "
                               "   set subj=" + region_code + ", "
                               "       dstr=" + area_code + ", "
                               "       city=" + city_code + ", "
                               "       nasp=" + place_code + ", "
                               "       strt=" + street_code + ", "
                               "       kladr='" + nasp_kladr + "', "
                               "       kladr_street='" + strt_kladr + "' "
                               " where id=" + QString::number(id) + " ; ";
                    QSqlQuery *query_save = new QSqlQuery(db);
                    bool res_save = query_save->exec(sql_save);
                    if (!res_save) {

                    }
                    delete query_save;
                }
            }
            delete query_fias_aoid;
        }
        n++;
        if (n%231==0) {
            ui->cnt_load_addresses->setText(QString::number(n));
            QApplication::processEvents();
        }
    }
    delete query_no_kladr;
}

void old_db_wnd::on_bn_PersonsOld_clicked()  {
    ui->pan_wait->setVisible(true);
    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  Подгрузка старых данных идентификации персон");
    QApplication::processEvents();

    bool res_load = try_reload_PersonsOld();
    if (!res_load) {
        ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  действие отменено!");
        QMessageBox::warning(this, "Действие отменено", "Действие отменено.\n");
    } else if (!ui->ch_process_all->isChecked()) {
        QMessageBox::information(this, "Готово", "Все старые данные идентификации персон подгружены.\n");
    }
    ui->pan_wait->setVisible(false);
    QApplication::processEvents();
}

bool old_db_wnd::try_reload_PersonsOld() {
    ui->lay_load_table->setText("PERSONS_OLD");
    ui->bn_close->setEnabled(false);
    ui->push_connect->setEnabled(false);
    ui->bn_load_tfoms->setEnabled(false);
    ui->bn_PersonsOld->setEnabled(false);
    ui->bn_clear_links->setEnabled(false);
    ui->bn_load_polises->setEnabled(false);
    ui->bn_reload_addres->setEnabled(false);
    ui->bn_close_polises->setEnabled(false);
    ui->bn_load_addresses->setEnabled(false);
    QApplication::processEvents();

    date0 = QDate::currentDate();
    time0 = QTime::currentTime();

    // подсчитаем существующие записи
    QSqlQuery *query_cnt = new QSqlQuery(db);
    query_cnt->exec("select count(*) from persons ; ");            query_cnt->next();     ui->cnt_persons_base->setText(            show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from persons_docs ; ");       query_cnt->next();     ui->cnt_persons_docs_base->setText(       show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from persons_old ; ");        query_cnt->next();     ui->cnt_persons_old_base->setText(        show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from vizits ; ");             query_cnt->next();     ui->cnt_vizits_base->setText(             show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from events ; ");             query_cnt->next();     ui->cnt_events_base->setText(             show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from polises ; ");            query_cnt->next();     ui->cnt_polises_base->setText(            show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from blanks_vs ; ");          query_cnt->next();     ui->cnt_blanks_vs_base->setText(          show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from blanks_pol ; ");         query_cnt->next();     ui->cnt_blanks_pol_base->setText(         show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from blanks_uec ; ");         query_cnt->next();     ui->cnt_blanks_uec_base->setText(         show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_insure_errors ; ");  query_cnt->next();     ui->cnt_spr_insure_errors_base->setText(  show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_oksm ; ");           query_cnt->next();     ui->cnt_spr_oksm_base->setText(           show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_ocato ; ");          query_cnt->next();     ui->cnt_spr_ocato_base->setText(          show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_ocato_full ; ");     query_cnt->next();     ui->cnt_spr_ocato_full_base->setText(     show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from addresses ; ");          query_cnt->next();     ui->cnt_addresses_base->setText(          show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from assig_by_terr ; ");      query_cnt->next();     ui->cnt_assig_by_terr_base->setText(      show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from persons_assig ; ");      query_cnt->next();     ui->cnt_persons_assig_base->setText(      show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_assig_errors ; ");   query_cnt->next();     ui->cnt_spr_assig_errors_base->setText(   show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medorgs ; ");        query_cnt->next();     ui->cnt_spr_medorgs_base->setText(        show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medparts ; ");       query_cnt->next();     ui->cnt_spr_medparts_base->setText(       show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medparts ; ");       query_cnt->next();     ui->cnt_spr_medparts_base->setText(       show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medsites ; ");       query_cnt->next();     ui->cnt_spr_medsites_base->setText(       show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medters ; ");        query_cnt->next();     ui->cnt_spr_medters_base->setText(        show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_ms_r_mt ; ");        query_cnt->next();     ui->cnt_spr_ms_r_mt_base->setText(        show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medsites_types ; "); query_cnt->next();     ui->cnt_spr_medsites_types_base->setText( show_num(query_cnt->value(0).toInt()) );
    query_cnt->exec("select count(*) from spr_medters_spec ; ");   query_cnt->next();     ui->cnt_spr_medters_spec_base->setText(   show_num(query_cnt->value(0).toInt()) );

    // подготовка
    date = QDate::currentDate();
    time = QTime::currentTime();
    ui->lab_time_all->setText("00:00:00");
    ui->lab_time->setText("00:00:00");
    QApplication::processEvents();

    int cnt_max = 2;
    if (ui->rb_10->isChecked())  cnt_max = 20;
    else if (ui->rb_100->isChecked())  cnt_max = 200;
    else if (ui->rb_1000->isChecked())  cnt_max = 2000;
    else if (ui->rb_10000->isChecked())  cnt_max = 20000;
    else if (ui->rb_100000->isChecked())  cnt_max = 200000;
    else if (ui->rb_1000000->isChecked())  cnt_max = 2000000;
    else if (ui->rb_hand->isChecked())  cnt_max = ui->spin_hand->value();

    ui->lab_persons->setEnabled(false);
    ui->lab_persons_docs->setEnabled(false);
    ui->lab_persons_old->setEnabled(false);
    ui->lab_vizits->setEnabled(false);
    ui->lab_events->setEnabled(false);
    ui->lab_polises->setEnabled(false);
    ui->lab_blanks_vs->setEnabled(false);
    ui->lab_blanks_pol->setEnabled(false);
    ui->lab_blanks_uec->setEnabled(false);
    ui->lab_spr_insure_errors->setEnabled(false);
    ui->lab_spr_oksm->setEnabled(false);
    ui->lab_spr_ocato->setEnabled(false);
    ui->lab_spr_ocato_full->setEnabled(false);
    ui->lab_addresses->setEnabled(false);
    ui->lab_assig_by_terr->setEnabled(false);
    ui->lab_persons_assig->setEnabled(false);
    ui->lab_spr_assig_errors->setEnabled(false);
    ui->lab_spr_medorgs->setEnabled(false);
    ui->lab_spr_medparts->setEnabled(false);
    ui->lab_spr_medsites->setEnabled(false);
    ui->lab_spr_medters->setEnabled(false);
    ui->lab_spr_ms_r_mt->setEnabled(false);
    ui->lab_spr_medsites_types->setEnabled(false);
    ui->lab_spr_medters_spec->setEnabled(false);

    ui->cnt_persons->setEnabled(false);
    ui->cnt_persons_docs->setEnabled(false);
    ui->cnt_persons_old->setEnabled(false);
    ui->cnt_vizits->setEnabled(false);
    ui->cnt_events->setEnabled(false);
    ui->cnt_polises->setEnabled(false);
    ui->cnt_blanks_vs->setEnabled(false);
    ui->cnt_blanks_pol->setEnabled(false);
    ui->cnt_blanks_uec->setEnabled(false);
    ui->cnt_spr_insure_errors->setEnabled(false);
    ui->cnt_spr_oksm->setEnabled(false);
    ui->cnt_spr_ocato->setEnabled(false);
    ui->cnt_spr_ocato_full->setEnabled(false);
    ui->cnt_addresses->setEnabled(false);
    ui->cnt_assig_by_terr->setEnabled(false);
    ui->cnt_persons_assig->setEnabled(false);
    ui->cnt_spr_assig_errors->setEnabled(false);
    ui->cnt_spr_medorgs->setEnabled(false);
    ui->cnt_spr_medparts->setEnabled(false);
    ui->cnt_spr_medsites->setEnabled(false);
    ui->cnt_spr_medters->setEnabled(false);
    ui->cnt_spr_ms_r_mt->setEnabled(false);
    ui->cnt_spr_medsites_types->setEnabled(false);
    ui->cnt_spr_medters_spec->setEnabled(false);

    ui->lay_load_table->setText("PERSONS_OLD\nПодсчёт числа записей");
    QApplication::processEvents();

    QString sql_prep;
    // проверим, есть ли что грузить
    sql_prep = "select COUNT(*) "
               "  from persons e "
               "  left join persons_old eo on(eo.id_person=e.id) "
               " where eo.id is NULL ; ";

    QSqlQuery *query_prep = new QSqlQuery(db);
    bool res_prep = query_prep->exec(sql_prep);
    query_prep->next();
    int cnt_prep = query_prep->value(0).toInt();
    delete query_prep;

    if (cnt_prep<1 && !ui->ch_process_all->isChecked()) {
        QMessageBox::information(this, "Нечего грузить",
                                 "В таблице полисов нет ни одной новой записи.\n\n"
                                 "Операция отменена.");

        // все записи уже обработаны
        ui->bn_PersonsOld->setVisible(false);

        ui->bn_close->setEnabled(true);
        ui->push_connect->setEnabled(true);
        ui->bn_load_tfoms->setEnabled(true);
        ui->bn_PersonsOld->setEnabled(true);
        ui->bn_clear_links->setEnabled(true);
        ui->bn_load_polises->setEnabled(true);
        ui->bn_reload_addres->setEnabled(true);
        ui->bn_close_polises->setEnabled(true);
        ui->bn_load_addresses->setEnabled(true);
        return true;
    }


    int n = 0;
/*
    db_oldBase.transaction();

    QString sql_get;
    sql_get =  // записи персон, для котрых нет старых данных
               "select distinct e.id "
               "  from persons e "
               "  left join persons_old eo on(eo.id_person=e.id) "
               " where eo.id is NULL ; ";

    // выберем порцию данных
    QSqlQuery *query_get = new QSqlQuery(db);
    bool res_get = query_get->exec(sql_get);

    if (!res_get) {
        QMessageBox::warning(this, "Ошибка чтения из таблицы PERSONS_OLD",
                             "При попытке прочитать данные персон без действующего полиса из таблицы PERSONS_OLD произошла ошибка.\n\n"
                             "Удаление устаревших ссылок во внешенй базе отменено!");
        db_oldBase.rollback();
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->push_connect->setEnabled(true);
        ui->bn_load_tfoms->setEnabled(true);
        ui->bn_PersonsOld->setEnabled(true);
        ui->bn_clear_links->setEnabled(true);
        ui->bn_load_polises->setEnabled(true);
        ui->bn_reload_addres->setEnabled(true);
        ui->bn_close_polises->setEnabled(true);
        ui->bn_load_addresses->setEnabled(true);
        return;
    }

    QString sql_clr;
    sql_clr = "delete from POLICES_IMPORT_IDS ; ";
    QSqlQuery *query_clr = new QSqlQuery(db_oldBase);
    bool res_clr = query_clr->exec(sql_clr);
    delete query_clr;

    __int64 id;
    QString sql_set;

    QSqlQuery *query_set = new QSqlQuery(db_oldBase);
    while (query_get->next()) {
        //------------------//
        // прочитаем данные //
        //------------------//
        id = query_get->value( 0).toInt();

        sql_set =  // добавим ID в таблицу
                   "insert into POLICES_IMPORT_IDS (id)"
                   "values (" + QString::number(id) + ") ; ";

        // выберем порцию данных
        bool res_set = query_set->exec(sql_set);
        n++;

        if (n%231==0) {
            // отображение статуса на экран
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

            ui->cnt_PersonsOld_base->setText(show_num(n));
            QApplication::processEvents();
        }
    }
    delete query_set;
    ui->cnt_PersonsOld_base->setText(show_num(n));
    QApplication::processEvents();
    db_oldBase.commit();

*/




    // покажем число персон без старых данных на экране
    ui->lay_load_table->setText("PERSONS_OLD\nreload PERSONS_OLD");
    QApplication::processEvents();

    // запрос данных
    QString sql;
    // Как показала практика, самое узкое место -
    // это чтение данных из FIrebird
    // Поэтому читаем ОЧЕНЬ большими порциями
    sql  =  "select /*first " + QString::number(cnt_max) + "*/ "
            "       p.pol_id, p.fam, p.im, p.ot, p.dat_r, coalesce(p.birthplace, p.birthplace_sity, p.birthplace_country) as plc_birth, p.pol, p.snils, p.enp, "
            // старые данные
            "       p.fam_old, p.im_old, p.ot_old, p.dat_r_old, p.doc_type_id_old, p.ser_doc_old, p.nom_doc_old, p.date_issue_old, p.mr_old, "
            // флаг
            "       pi.flag,"
            "       pid.id as id_person "
            "  from POLICES_IMPORT_IDS pid "
            "  left join polices_import pi on(pi.id_person=pid.id) "
            "  left join polices p on(pi.pol_id=p.pol_id)"
            " order by p.pol_id ; ";

    sql = sql.replace("#ONLY_FRESH#", ui->ch_process_new->isChecked() ? "p.pol_id>10000000" : "1=1");

    // заготовим переменные
    int id_person;
    int pol_id;
    QString fam, im, ot;
    QString fam_norm, im_norm, ot_norm;
    QDate dat_r;
    QString plc_birth, pol;
    int sex;
    QString snils, enp;

    // старые данные
    QString fam_old, im_old, ot_old, plc_birth_old;
    QDate dat_r_old;
    int doc_type_id_old;
    QString ser_doc_old, nom_doc_old;
    QDate date_issue_old;

    int flag;
    bool has_flag;


    db_oldBase.transaction();
    db.transaction();

    // выберем порцию данных
    QSqlQuery *query = new QSqlQuery(db_oldBase);
    bool res = query->exec(sql);

    if (!res) {
        QMessageBox::warning(this, "Ошибка чтения из таблицы PERSONS_OLD",
                             "При попытке прочитать очередную порцию данных из таблицы PERSONS_OLD произошла неожиданная ошибка.\n\n"
                             "Загрузка данных из внешней базы остановлена!");
        db_oldBase.rollback();
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->push_connect->setEnabled(true);
        ui->bn_load_tfoms->setEnabled(true);
        ui->bn_PersonsOld->setEnabled(true);
        ui->bn_clear_links->setEnabled(true);
        ui->bn_load_polises->setEnabled(true);
        ui->bn_reload_addres->setEnabled(true);
        ui->bn_close_polises->setEnabled(true);
        ui->bn_load_addresses->setEnabled(true);
        return false;
    }

    ui->lay_load_table->setText("PERSONS_OLD\nДобавление данных");
    QApplication::processEvents();

    n = 0;
    int n_corr = 0;
    while (query->next()) {
        /* вот так некрасиво
        if (n<1000) {
            n++;
            continue;
        }*/

        //------------------//
        // прочитаем данные //
        //------------------//
        pol_id        = query->value( 0).toInt();
        fam           = query->value( 1).toString().trimmed().simplified().toUpper();
        im            = query->value( 2).toString().trimmed().simplified().toUpper().replace("'","\"");
        ot            = query->value( 3).toString().trimmed().simplified().toUpper().replace("'","\"");
        dat_r         = query->value( 4).toDate();
        plc_birth     = query->value( 5).toString().trimmed().simplified().replace("'","\"");
        pol           = query->value( 6).toString().trimmed();
        sex           = (pol.left(1)=="М" ? 1 : 2);
        snils         = query->value( 7).toString().trimmed().replace(" ", "").replace("-", "").replace("'","\"");
        enp           = query->value( 8).toString().trimmed().replace(" ", "").replace("-", "").replace("'","\"");
        // старые данные
        fam_old       = query->value( 9).toString().trimmed().simplified().replace("'","\"");
        im_old        = query->value(10).toString().trimmed().simplified().replace("'","\"");
        ot_old        = query->value(11).toString().trimmed().simplified().replace("'","\"");
        dat_r_old     = query->value(12).toDate();
        doc_type_id_old = query->value(13).toInt();
        ser_doc_old   = query->value(14).toString().trimmed().simplified().replace("'","\"");
        nom_doc_old   = query->value(15).toString().trimmed().simplified().replace("'","\"");
        date_issue_old= query->value(16).toDate();
        plc_birth_old = query->value(17).toString().trimmed().simplified().replace("'","\"");
        // флаг
        flag          = query->value(18).toInt();
        has_flag      =!query->value(18).isNull();
        id_person     = query->value(19).toInt();




        //---------------------------------------------//
        // обновим старые данные идертификации персоны //
        //---------------------------------------------//
        QString sql_find = "select max(id) "
                           "  from persons_old "
                           " where id_person=" + QString::number(id_person) + " ; ";
        QSqlQuery *query_find = new QSqlQuery(db);
        bool res_find = query_find->exec(sql_find);
        int id_old_person = -1;
        while (query_find->next())
            if (!query_find->value(0).isNull())
                id_old_person = query_find->value(0).toInt();
        delete query_find;

        if ( id_old_person<=0
             && ( !fam_old.isEmpty()
               || !im_old.isEmpty()
               || !ot_old.isEmpty()
               || dat_r_old.isValid()
               || !plc_birth_old.isEmpty()) ) {
            // запись старых данных ещё не создана - сделаем её
            QString sql_nold = "insert into persons_old (id_person, old_fam, old_im, old_ot, old_date_birth, old_plc_birth, old_enp, old_snils, old_sex) "
                               "values ( " + QString::number(id_person)  + " "
                               "       ,'" + QString (fam_old.isEmpty()   ? fam  : fam_old) + "' " +
                               "       ,'" + QString (im_old.isEmpty()    ? im   : im_old)  + "' " +
                               "       , " + QString (ot_old.isEmpty()    ? (ot.isEmpty()  ? "NULL"  : ("'" + ot + "'"))  : ("'" + ot_old + "'")) +
                               "       ,'" + QString (dat_r_old.isValid() ? dat_r_old.toString("yyyy-MM-dd") : dat_r.toString("yyyy-MM-dd")) + "' " +
                               "       ,'" + QString (plc_birth_old.isEmpty() ? plc_birth : plc_birth_old) + "' " +
                               "       , " + QString (enp.isEmpty()       ? "NULL"               : ("'" + enp + "'")) +
                               "       , " + QString (snils.isEmpty()     ? "NULL"               : ("'" + snils + "'")) +
                               "       , " + QString ((sex==1||sex==2)    ? QString::number(sex) : "1") +  " ) ; ";
            QSqlQuery *query_nold = new QSqlQuery(db);
            bool res_nold = query_nold->exec(sql_nold);

            while ( !res_nold
                    && QMessageBox::warning( this, "Ошибка добавления старых данных персоны",
                                             "При попытке добавить старые данные персоны произошла неожиданная ошибка.\n\n"
                                             "n = " + QString::number(n) + "\n\n"
                                             + query_nold->lastError().driverText() + "\n" + query_nold->lastError().databaseText() + "\n\n"
                                             "Повторить?",
                                             QMessageBox::Retry|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel ) {
                res_nold = query_nold->exec(sql_nold);
            }
            n_corr++;

            if (!res_nold) {
                delete query_nold;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            delete query_nold;
        } else if ( id_old_person>0
                    && ( !fam_old.isEmpty()
                      || !im_old.isEmpty()
                      || !ot_old.isEmpty()
                      || dat_r_old.isValid()
                      || !plc_birth_old.isEmpty()) ) {
            // обновим старые денные персоны
            QString sql_old;
            sql_old  = "update persons_old "
                       "   set " +
                       QString (!fam_old.isEmpty()  ? (" old_fam =  '" + fam_old + "', ") : "") +
                       QString (!im_old.isEmpty()   ? (" old_im  =  '" + im_old  + "', ") : "") +
                       QString (!ot_old.isEmpty()   ? (" old_ot =   '" + ot_old  + "', ") : "") +
                       QString (dat_r_old.isValid() ? (" old_date_birth = '" + dat_r_old.toString("yyyy-MM-dd") + "' ") : ", ") +
                       QString (!plc_birth_old.isEmpty() ? (" old_plc_birth  = '" + plc_birth_old + "' ") : "") +
                       " where id_person = " + QString::number(id_person) + " ; ";
            sql_old = sql_old.replace(", where", " where").replace(",  where", "  where").replace(",   where", "  where").replace(",    where", "  where");
            QSqlQuery *query_old = new QSqlQuery(db);
            bool res_old = query_old->exec(sql_old);

            while ( !res_old
                    && QMessageBox::warning( this, "Ошибка добавления старых данных персоны",
                                             "При попытке добавить старые данные персоны произошла неожиданная ошибка.\n\n"
                                             "n = " + QString::number(n) + "\n\n"
                                             + query_old->lastError().driverText() + "\n" + query_old->lastError().databaseText() + "\n\n"
                                             "Повторить?",
                                             QMessageBox::Retry|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel ) {
                res_old = query_old->exec(sql_old);
            }
            n_corr++;

            if (!res_old) {
                delete query_old;
                db_oldBase.rollback();
                db.rollback();

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_load_tfoms->setEnabled(true);
                ui->bn_PersonsOld->setEnabled(true);
                ui->bn_clear_links->setEnabled(true);
                ui->bn_load_polises->setEnabled(true);
                ui->bn_reload_addres->setEnabled(true);
                ui->bn_close_polises->setEnabled(true);
                ui->bn_load_addresses->setEnabled(true);
                return false;
            }
            delete query_old;
        }




        n++;

        if ( n%2341==0
             || n==cnt_max-1 ) {
            // отображение статуса на экран
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


            ui->cnt_PersonsOld->setText(show_num(n));
            ui->cnt_PersonsOld_2->setText(show_num(n_corr));
            ui->cnt_persons->setText(show_num(cnt_persons));
            ui->cnt_persons_docs->setText(show_num(cnt_persons_docs));
            ui->cnt_persons_old->setText(show_num(cnt_persons_old));
            ui->cnt_vizits->setText(show_num(cnt_vizits));
            ui->cnt_events->setText(show_num(cnt_events));
            ui->cnt_polises_close->setText(show_num(0-cnt_polises_close) + ", ");
            ui->cnt_polises->setText(show_num(cnt_polises));
            ui->cnt_blanks_vs->setText(show_num(cnt_blanks_vs));
            ui->cnt_blanks_pol->setText(show_num(cnt_blanks_pol));
            ui->cnt_blanks_uec->setText(show_num(cnt_blanks_uec));
            ui->cnt_spr_insure_errors->setText(show_num(cnt_spr_insure_errors));
            ui->cnt_spr_oksm->setText(show_num(cnt_spr_oksm));
            ui->cnt_spr_ocato->setText(show_num(cnt_spr_ocato));
            ui->cnt_spr_ocato_full->setText(show_num(cnt_spr_ocato_full));
            ui->cnt_addresses->setText(show_num(cnt_addresses));
            ui->cnt_assig_by_terr->setText(show_num(cnt_assig_by_terr));
            ui->cnt_persons_assig->setText(show_num(cnt_persons_assig));
            ui->cnt_spr_assig_errors->setText(show_num(cnt_spr_assig_errors));
            ui->cnt_spr_medorgs->setText(show_num(cnt_spr_medorgs));
            ui->cnt_spr_medparts->setText(show_num(cnt_spr_medparts));
            ui->cnt_spr_medsites->setText(show_num(cnt_spr_medsites));
            ui->cnt_spr_medters->setText(show_num(cnt_spr_medters));
            ui->cnt_spr_ms_r_mt->setText(show_num(cnt_spr_ms_r_mt));
            ui->cnt_spr_medsites_types->setText(show_num(cnt_spr_medsites_types));
            ui->cnt_spr_medters_spec->setText(show_num(cnt_spr_medters_spec));

            ui->lab_persons->setEnabled(0<cnt_persons);
            ui->lab_persons_docs->setEnabled(0<cnt_persons_docs);
            ui->lab_persons_old->setEnabled(0<cnt_persons_old);
            ui->lab_vizits->setEnabled(0<cnt_vizits);
            ui->lab_events->setEnabled(0<cnt_events);
            ui->lab_polises->setEnabled(0<cnt_polises);
            ui->lab_blanks_vs->setEnabled(0<cnt_blanks_vs);
            ui->lab_blanks_pol->setEnabled(0<cnt_blanks_pol);
            ui->lab_blanks_uec->setEnabled(0<cnt_blanks_uec);
            ui->lab_addresses->setEnabled(0<cnt_addresses);

            ui->cnt_persons->setEnabled(0<cnt_persons);
            ui->cnt_persons_docs->setEnabled(0<cnt_persons_docs);
            ui->cnt_persons_old->setEnabled(0<cnt_persons_old);
            ui->cnt_vizits->setEnabled(0<cnt_vizits);
            ui->cnt_events->setEnabled(0<cnt_events);
            ui->cnt_polises->setEnabled(0<cnt_polises);
            ui->cnt_blanks_vs->setEnabled(0<cnt_blanks_vs);
            ui->cnt_blanks_pol->setEnabled(0<cnt_blanks_pol);
            ui->cnt_blanks_uec->setEnabled(0<cnt_blanks_uec);
            ui->cnt_addresses->setEnabled(0<cnt_addresses);

            db.commit();
            db.transaction();
            /*  --  нельзя - ведь у нас открыт запрос :(
            db_oldBase.commit();
            db_oldBase.transaction(); */

            QApplication::processEvents();
        }
    }
    int cnt_all = query->size();

    if (cnt_all<cnt_max)
        // все записи уже обработаны
        ui->bn_PersonsOld->setVisible(false);

    db_oldBase.commit();
    db.commit();

    ui->cnt_PersonsOld->setText(show_num(n));
    ui->cnt_PersonsOld_2->setText(show_num(n_corr));

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  Готово (" + QString::number(n) + " строк) !");
    QApplication::processEvents();
    if (ui->ch_process_all->isChecked())
          on_bn_load_addresses_clicked();
    else  QMessageBox::information(this, "Готово", "Все старые данные застрахованных загружены.\n\nОбработано " + QString::number(n) + " строк. ");


    ui->lay_load_table->setText("  - // -  ");
    ui->bn_close->setEnabled(true);
    ui->push_connect->setEnabled(true);
    ui->bn_load_tfoms->setEnabled(true);
    ui->bn_PersonsOld->setEnabled(true);
    ui->bn_clear_links->setEnabled(true);
    ui->bn_load_polises->setEnabled(true);
    ui->bn_reload_addres->setEnabled(true);
    ui->bn_close_polises->setEnabled(true);
    ui->bn_load_addresses->setEnabled(true);
    return true;
}
