#include "assignment_wnd.h"
#include "ui_assignment_wnd.h"

#include <QSqlError>
#include <QMessageBox>
#include <QInputDialog>

#include "sessions_wnd.h"
#include "ki_funcs.h"
#include "add_person_wnd.h"

#include <QtSql>
#include <QTime>
#include <QTimer>
#include <QFileDialog>
#include <QTextCodec>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QScrollBar>
#include <QModelIndexList>

//#include <winuser.h>


assignment_wnd::assignment_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent)
 : db(db), data_app(data_app), settings(settings), QMainWindow(parent), ui(new Ui::assignment_wnd)
{
    ui->setupUi(this);

    data_app.version = "0.0.1";

    QFont font = this->font();
    data_app.font_size = font.pointSize();

    add_assig_w  = NULL;
    autoassig_w  = NULL;
    show_tab_w   = NULL;
    show_text_w  = NULL;
    sessions_w   = NULL;
    add_medsite_ter_w = NULL;
    snils_list_w = NULL;
    matchTFOMS_w = NULL;
    geodata_w    = NULL;
    old_db_w     = NULL;
    accounts_base_w   = NULL;
    reindex_w    = NULL;
    assiglays_w  = NULL;
    get_date_w   = NULL;
    letters2MO_w = NULL;
    medsites_reg_w    = NULL;
    finance_medorgs_w = NULL;
    add_person_w      = NULL;
    add_assig_rec_w   = NULL;
    reports_assig2mo_w = NULL;
    print_letters_w   = NULL;


    data_pers.id         = -1;
    data_pers.pol_v      = -1;
    data_pers.fam        = "";
    data_pers.im         = "";
    data_pers.ot         = "";
    data_pers.sex        = -1;
    data_pers.date_birth = QDate(1900,1,1);
    data_pers.date_death = QDate(1900,1,1);
    data_pers.snils      = "";
    data_pers.enp        = "";
    data_pers.date_reg   = QDate(1900,1,1);
    data_pers.bomj       = false;
    data_pers.status     = -1;
    data_pers.date_insure_begin     = QDate(1900,1,1);
    data_pers.has_date_insure_begin = false;
    data_pers.date_insure_end       = QDate(1900,1,1);
    data_pers.has_date_insure_end   = false;
    data_pers.id_udl     = -1;
    data_pers.id_drp     = -1;
    data_pers.id_pol     = -1;
    data_pers.id_asg     = -1;


    l10.clear();
    l10.append(100);
    l10.append(0);

    l11.clear();
    l11.append(500);
    l11.append(500);

    l500.clear();
    l500.append(500);
    l500.append(0);
    l500.append(0);

    l501.clear();
    l501.append(400);
    l501.append(0);
    l501.append(70);

    l510.clear();
    l510.append(400);
    l510.append(70);
    l510.append(0);

    l511.clear();
    l511.append(330);
    l511.append(70);
    l511.append(70);

    ui->ch_addr_reg->setVisible(false);
    ui->ch_addr_liv->setVisible(false);

    f_Updater = false;
    ui->bn_guten_morgen->setChecked(false);
    timer_Updater = new QTimer();
    timer_Updater->setInterval(5000);
    connect(timer_Updater, SIGNAL(timeout()), this, SLOT(on_time_Updater()));
    timer_Updater->start();

    if (data_app.ocato!="20000") {
        ui->act_Voronej_addPolises->setText("Доступно только в воронежском сегменте данных");
      //  ui->act_Voronej_reindex->setText("Доступно только в воронежском сегменте данных");
        ui->act_to_acconnts_base->setText("Доступно только в воронежском сегменте данных");
        ui->act_Voronej_addPolises->setEnabled(false);
      //  ui->act_Voronej_reindex->setEnabled(false);
        ui->act_to_acconnts_base->setEnabled(false);
        ui->menu_Voronej->setVisible(false);
    }

    if (data_app.ocato!="14000") {
        ui->act_load_mo_data->setText("Доступно только в белгородском сегменте данных");
        ui->act_load_mo_data->setEnabled(false);
        ui->menu_Belgorod->setVisible(false);
    }

    ui->tabWidget->setCurrentIndex(0);

    refresh_fias_connection();

    // 1
    ui->ch_show_assig->setChecked(false);
    //ui->split_assig->setStretchFactor(0,1);
    //ui->split_assig->setStretchFactor(1,0);
    ui->ch_show_addresses->setChecked(false);
    //ui->split_addresses->setStretchFactor(0,1);
    //ui->split_addresses->setStretchFactor(1,0);
    ui->split_assig->setSizes(l10);
    ui->split_addresses->setSizes(l10);
    ui->ch_assig_type->setChecked(false);
    ui->combo_assig_type->setVisible(false);
    refresh_medorg_pers();
    //refresh_persons_tab();
    ui->tab_persons->setEnabled(false);

    // 2
    refresh_medorg_terr();
    refresh_subj_terr();
    refresh_dstr_terr();
    refresh_city_terr();
    refresh_nasp_terr();
    refresh_strt_terr();
    // собственно таблица
    //refresh_terrs_tab();
    ui->tab_terrs->setEnabled(false);

    // 3
    ui->splitter_main->setStretchFactor(0,4);
    ui->splitter_main->setStretchFactor(1,3);


    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,1);
    ui->split_MO->setStretchFactor(0,4);
    ui->split_MO->setStretchFactor(1,3);
    ui->split_MS->setStretchFactor(0,1);
    ui->split_MS->setStretchFactor(1,1);
    ui->split_mo_ms->setStretchFactor(0,3);
    ui->split_mo_ms->setStretchFactor(1,1);

    this->setCursor(Qt::WaitCursor);
    QApplication::processEvents();
    //QMessageBox::warning(this, ("1"), ("1"));
    refresh_medorgs_tab();
    //QMessageBox::warning(this, ("2"), ("2"));
    refresh_medpart_type();
    //QMessageBox::warning(this, ("2"), ("2"));
    refresh_medsites_tab();
    refresh_medsite_mo();
    refresh_medsite_mp();
    refresh_medsite_type();
    //QMessageBox::warning(this, ("3"), ("3"));
    refresh_medters_tab();
    refresh_medter_mo();
    //refresh_medter_mp();
    //refresh_medter_ms();
    refresh_medter_spec();
    //QMessageBox::warning(this, ("4"), ("4"));

    /*
    // раскладка
    long long lay0 = (long long)ActivateKeyboardLayout(HKL(68748313), 0x00000008);
    long long lay = (long long)GetKeyboardLayout(GetCurrentThreadId());
    if (lay == 68748313) {      //Russian
        ui->bn_layout->setText("Руc");
        QPalette pal = ui->bn_layout->palette();
        pal.setColor(QPalette::Button,QColor(0,255,0,255));
        ui->bn_layout->setPalette(pal);
    } else if(lay == 67699721) {  //English
        ui->bn_layout->setText("Eng");
        QPalette pal = ui->bn_layout->palette();
        pal.setColor(QPalette::Button,QColor(128,128,255,255));
        ui->bn_layout->setPalette(pal);
    } else if(lay == 69338146) {  //Ukrainian
        ui->bn_layout->setText("Укр");
        QPalette pal = ui->bn_layout->palette();
        pal.setColor(QPalette::Button,QColor(255,0,0,255));
        ui->bn_layout->setPalette(pal);
    } else { //unknown language
        ui->bn_layout->setText("???");
        QPalette pal = ui->bn_layout->palette();
        pal.setColor(QPalette::Button,QColor(200,200,200,255));
        ui->bn_layout->setPalette(pal);
    }
    */

    data_assig.ocato   = data_app.ocato;
    data_assig.subj    = 0;
    data_assig.dstr    = 0;
    data_assig.city    = 0;
    data_assig.nasp    = 0;
    data_assig.strt    = 0;
    data_assig.right_type = 1;
    data_assig.house   = -1;
    data_assig.corp    = "";
    data_assig.house2  = -1;
    data_assig.corp2   = "";
    data_assig.sex     = 0;
    data_assig.age     = 0;
    data_assig.code_mo = "";
    data_assig.code_mp = "";
    data_assig.code_ms = "";
    data_assig.snils_mt= "";
    data_assig.from_start= false;
    data_assig.to_end  = false;

/*
    statusLab = new QLabel;
    statusBar()->addWidget(statusLab);
    statusLab->setFont(QFont("Arial",10,75,false));
    QPalette pal = statusLab->palette();
    pal.setColor(QPalette::Text,QColor(0,0,255));
    statusLab->setPalette(pal);
    statusLab->setText("программа: \"Прикрепление персон\", версия:" + data_app.version + ";  сервер: " + data_app.db_host + ";  БД: " + data_app.db_name + ";  версия: " + data_app.db_vers);
*/


    statusLab = new QLabel;
    statusBar()->addWidget(statusLab);
    statusLab->setFont(QFont("Arial",10,75,false));
    QPalette pal = statusLab->palette();
    pal.setColor(QPalette::WindowText,QColor(0,0,128));
    statusLab->setPalette(pal);
    statusLab->setText("программа: \"Прикрепление персон\", версия:" + data_app.version + ";  сервер: " + (data_app.is_tech ? (data_app.host+" / "+data_app.db_host) : data_app.db_host) + ";  БД: " + data_app.db_name + ";  версия: " + data_app.db_vers);

    statusOper = new QLabel;
    statusBar()->addWidget(statusOper);
    statusOper->setFont(QFont("Arial",10,50,false));
    pal = statusOper->palette();
    pal.setColor(QPalette::WindowText,QColor(0,80,0));
    statusOper->setPalette(pal);
    statusOper->setText("ПВП: " + data_app.point_name + ";  оператор: " + data_app.operator_fio + "  (OK)");


    refresh_pers_statuses();

    refresh_combo_assig_source();

    this->setCursor(Qt::ArrowCursor);
    QApplication::processEvents();
}

assignment_wnd::~assignment_wnd() {
    delete ui;
}


void assignment_wnd::on_act_font_1_triggered() {
    QFont font = this->font();
    font.setPointSize(8);
    this->setFont(font);
}

void assignment_wnd::on_act_font_2_triggered() {
    QFont font = this->font();
    font.setPointSize(10);
    this->setFont(font);
}

void assignment_wnd::on_act_font_3_triggered() {
    QFont font = this->font();
    font.setPointSize(13);
    this->setFont(font);
}

void assignment_wnd::on_act_font_4_triggered() {
    QFont font = this->font();
    font.setPointSize(15);
    this->setFont(font);
}

void assignment_wnd::on_bn_font_up_clicked() {
    QFont font = this->font();
    data_app.font_size = font.pointSize();
    data_app.font_size = int(data_app.font_size +1);
    font.setPointSize(data_app.font_size);
    this->setFont(font);
}
void assignment_wnd::on_bn_font_dn_clicked() {
    QFont font = this->font();
    data_app.font_size = font.pointSize();
    data_app.font_size = int(data_app.font_size -1);
    font.setPointSize(data_app.font_size);
    this->setFont(font);
}


// обновление подключения к БД ФИАС
bool assignment_wnd::refresh_fias_connection() {
    // закроем, если есть, старое подключение к БД ФИАС
    if (db_FIAS.isOpen()) {
        db_FIAS.close();
        QSqlDatabase::removeDatabase("FIAS Database");
    }

    // ------------------------------ //
    // создадим подключение к БД ФИАС //
    // ------------------------------ //
    db_FIAS = QSqlDatabase::addDatabase(data_app.FIAS_db_drvr, "FIAS Database");

    db_FIAS.setConnectOptions();
    // сервер, на котором расположена БД - у нас localhost
    db_FIAS.setHostName(data_app.FIAS_db_host);
    // порт
    db_FIAS.setDatabaseName(data_app.FIAS_db_name);
    // имя базы данных
    db_FIAS.setPort(data_app.FIAS_db_port.toInt());
    // имя пользователя - postgres
    db_FIAS.setUserName(data_app.FIAS_db_user);
    // пароль для подключения к БД
    db_FIAS.setPassword(data_app.FIAS_db_pass);

    while (!db_FIAS.open()) {
        // подключение не удалось
        QMessageBox::critical(this,"Ошибка подключения к базе ФИАС",
                              QString("ВНИМАНИЕ!!!\n\n") +
                              "При попытке подключиться к базе ФИАС произошла ошибка.\n"
                              "Позвоните мне по телефону \n\n"
                              "8-920-449-54-88\n\n"
                              "Александр\n\n" +
                              "FIAS_db_drvr = " + data_app.FIAS_db_drvr + "\n" +
                              "FIAS_db_host = " + data_app.FIAS_db_host + "\n" +
                              "FIAS_db_port = " + data_app.FIAS_db_port + "\n" +
                              "FIAS_db_name = " + data_app.FIAS_db_name + "\n" +
                              "FIAS_db_user = " + data_app.FIAS_db_user + "\n" +
                              "FIAS_db_pass = " + data_app.FIAS_db_pass + "\n");
        return false;
    }
    if (db_FIAS.isOpen()) {
        // подключение готово
        return true;
    }
    return false;
}

void assignment_wnd::on_bn_exit_clicked() {
    if (QMessageBox::question(this, "Закрыть программу?", "Закрыть программу?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::Yes) {
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
}

void assignment_wnd::on_act_close_triggered() {
    on_bn_exit_clicked();
}

void assignment_wnd::refresh_medorg_pers() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка медорганизаций
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mo, '(' || code_mo || ') ' || name_mo "
                  "  from spr_medorgs s "
                  " where is_head=1 "
                  " order by code_mo ; ";
    mySQL.exec(this, sql, QString("Справочник медорганизаций для списка персон"), *query, true, db, data_app);
    ui->combo_medorg_pers->clear();
    while (query->next()) {
        ui->combo_medorg_pers->addItem(query->value(1).toString(), query->value(0).toString());
    }
    //ui->combo_medorg_pers->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}


void assignment_wnd::refresh_pers_statuses() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка статусов персоны
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code, '('||code||')  '||text||'  ' "
                  "  from public.spr_pers_status s "
                  " where s.code>=-10 "
                  " order by code desc ; ";
    mySQL.exec(this, sql, QString("Список статусов персоны"), *query, true, db, data_app);
    ui->combo_pers_status->clear();
    //ui->combo_pers_status_f->clear();
    while (query->next()) {
        ui->combo_pers_status->addItem(query->value(1).toString(), query->value(0).toInt());
        //ui->combo_pers_status_f->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    // для лиц с истёкшим УДЛ
    //ui->combo_pers_status_f->addItem(" !!!  Застрахованные с истёкшим УДЛ  !!! ", -11);

    delete query;
    ui->combo_pers_status->setCurrentIndex(-1);
    //ui->combo_pers_status_f->setCurrentIndex(ui->combo_pers_status_f->findData(1));
    this->setCursor(Qt::ArrowCursor);
}


void assignment_wnd::refresh_persons_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    ui->pan_persons->setEnabled(false);

    ui->pan_polises->setEnabled(false);
    ui->pan_addresses->setEnabled(false);
    ui->pan_assig->setEnabled(false);
    QApplication::processEvents();

    QDate date18 = QDate::currentDate().addYears(-18);
    QString date18_s = date18.toString("yyyy-MM-dd");

    sql_pers_flds = "select e.id as id_pers, "
                    "       case p.pol_v when 1 then ' СТАР.' when 2 then ' ВС ' when 3 then ' ЕНП ' else '-???-' end as pol_v, "
                    "       e.fam, e.im, e.ot, "
                    "       case e.sex when 1 then 'муж.' when 2 then ' жен.' else '???' end as sex, "
                    "       e.date_birth, e.date_death, e.snils, e.enp, "
                    "       e.date_reg, "
                    "       case e.bomj when 1 then 'БОМЖ' else ' - ' end as bomj, "
                    "       e.status, e._date_insure_begin, e._date_insure_end, "
                    "       e.id_udl, e.id_drp, e.id_polis, NULL/*la._id_asg*/, "
                    "       /*ar.id as*/ e.id_addr_reg, /*al.id as*/ e.id_addr_liv, "
                    "       e.dt_ins, e.dt_upd ";

    sql_pers_from = "  from persons e "
                    "  left join polises p on(p.id=e.id_polis) "
                    //"  left join polises p on(p.id=(select max(pp.id) from polises pp where pp.id_person=e.id and pp.date_begin<CURRENT_DATE and pp.date_stop is NULL)) "
                    "  left join link_persons_asg la on(la.id_person=e.id) "
                    "  left join persons_assig a on(a.id=la._id_asg) "
                    "  left join addresses ar on(ar.id=e.id_addr_reg) "
                    "  left join addresses al on(al.id=e.id_addr_liv) ";

    if ( (ui->ch_fam_pers->isChecked()  && !ui->ln_fam_pers->text().isEmpty())         ||
         (ui->ch_im_pers->isChecked()   && !ui->ln_im_pers->text().isEmpty())          ||
         (ui->ch_ot_pers->isChecked()   && !ui->ln_ot_pers->text().isEmpty())          ||
         (ui->ch_datr_pers->isChecked() && ui->date_datr_pers->date()>QDate(1920,1,1)) ||
         (ui->ch_sex_pers->isChecked()  && ui->combo_sex_pers->currentIndex()>0)       ||
         (ui->ch_pers_age->isChecked()  && ui->combo_pers_age->currentIndex()>0)       ||
         (ui->ch_vs_num->isChecked()    && !ui->ln_vs_num->text().isEmpty())           ||
         (ui->ch_enp->isChecked()       && !ui->ln_enp->text().isEmpty())              ||
         (ui->ch_snils->isChecked()     && !ui->ln_snils->text().isEmpty())            ||
         //(ui->ch_assig_type->isChecked() ) ||
         ui->ch_medorg_pers->isChecked()                                        ||
         (ui->ch_filter_polis->isChecked()  &&  ui->ch_has_polis->isChecked())  ||
         (ui->ch_filter_polis->isChecked()  &&  ui->ch_no_polis->isChecked() )  ||
         (ui->ch_filter_addrs->isChecked()  &&  ui->ch_addr_reg->isChecked() )  ||
         (ui->ch_filter_addrs->isChecked()  &&  ui->ch_addr_liv->isChecked() )  ||
         (ui->ch_filter_assig->isChecked()  &&  ui->ch_has_assig->isChecked())  ||
         (ui->ch_filter_assig->isChecked()  &&  ui->ch_no_assig->isChecked() )  ) {
        sql_pers_from += " where ";

        bool f_first = true;
        if (ui->ch_medorg_pers->isChecked()) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " a.code_mo='" + ui->combo_medorg_pers->currentData().toString() + "' ";
            f_first = false;
        }
        if (ui->ch_fam_pers->isChecked() && !ui->ln_fam_pers->text().isEmpty()) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " upper(e.fam) like ('" + ui->ln_fam_pers->text().trimmed().simplified().toUpper() + "%')";
            f_first = false;
        }
        if (ui->ch_im_pers->isChecked() && !ui->ln_im_pers->text().isEmpty()) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " upper(e.im) like ('" + ui->ln_im_pers->text().trimmed().simplified().toUpper() + "%')";
            f_first = false;
        }
        if (ui->ch_ot_pers->isChecked() && !ui->ln_ot_pers->text().isEmpty()) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " upper(e.ot) like ('" + ui->ln_ot_pers->text().trimmed().simplified().toUpper() + "%')";
            f_first = false;
        }
        if (ui->ch_datr_pers->isChecked() && ui->date_datr_pers->date()>QDate(1920,1,1)) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " e.date_birth='" + ui->date_datr_pers->date().toString("yyyy-MM-dd") + "' ";
            f_first = false;
        }
        if (ui->ch_sex_pers->isChecked() && ui->combo_sex_pers->currentIndex()>0) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " e.sex=" + QString::number(ui->combo_sex_pers->currentIndex()) + " ";
            f_first = false;
        }
        if (ui->ch_pers_age->isChecked() && ui->combo_pers_age->currentIndex()>0) {
            if (ui->combo_pers_age->currentIndex()>0) {
                if (!f_first)  sql_pers_from += " and ";
                if (ui->combo_pers_age->currentIndex()==1) {
                    sql_pers_from += " e.date_birth>'" + date18_s + "' ";
                } else if (ui->combo_pers_age->currentIndex()==2) {
                    sql_pers_from += " e.date_birth<='" + date18_s + "' ";
                }
                f_first = false;
            }
        }

        if (ui->ch_vs_num->isChecked() && !ui->ln_vs_num->text().isEmpty()) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " cast(p.vs_num as text) like ('" + ui->ln_vs_num->text().trimmed().simplified().toUpper() + "%')";
            f_first = false;
        }
        if (ui->ch_enp->isChecked() && !ui->ln_enp->text().isEmpty()) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " p.enp like ('" + ui->ln_enp->text().trimmed().simplified().toUpper() + "%')";
            f_first = false;
        }
        if (ui->ch_snils->isChecked() && !ui->ln_snils->text().isEmpty()) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " e.snils like ('" + ui->ln_snils->text().trimmed().simplified().toUpper() + "%')";
            f_first = false;
        }

        if (ui->ch_filter_polis->isChecked()  &&  ui->ch_has_polis->isChecked()) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " p.id is not NULL ";
            f_first = false;
        }
        if (ui->ch_filter_polis->isChecked()  &&  ui->ch_no_polis->isChecked()) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " p.id is NULL ";
            f_first = false;
        }

        if (ui->ch_filter_addrs->isChecked()  &&  ui->ch_addr_reg->isChecked()) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " e.id_addr_reg is not NULL ";
            f_first = false;
        }
        if (ui->ch_filter_addrs->isChecked()  &&  ui->ch_addr_liv->isChecked()) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " e.id_addr_liv is not NULL ";
            f_first = false;
        }

        if (ui->ch_filter_assig->isChecked()  &&  ui->ch_has_assig->isChecked()) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " a.code_mo is not NULL ";
            f_first = false;
        }
        if (ui->ch_filter_assig->isChecked()  &&  ui->ch_no_assig->isChecked()) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " a.code_mo is NULL ";
            f_first = false;
        }
        if (!ui->ch_arch->isChecked()) {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " e.status>-100 and e.status<100 ";
            f_first = false;
        } else {
            if (!f_first)  sql_pers_from += " and ";
            sql_pers_from += " (e.status<-100 or e.status>100) ";
            f_first = false;
        }

        sql_pers_from += " ";
    }

    sql_pers_all = sql_pers_flds + sql_pers_from + " order by e.fam, e.im, e.ot, e.date_birth ";

    // подсчитаем число строк
    sql_pers_cnt = "select count(*) as cnt "
                   " " + sql_pers_from + " ; ";
    QSqlQuery *query_count = new QSqlQuery(db);
    bool res_count = mySQL.exec(this, sql_pers_cnt, QString("Подсчёт числа отображаемых строк персон"), *query_count, true, db, data_app);
    if (!res_count) {
        QMessageBox::warning(this, "Ошибка при подсчёте числа отображаемых строк персон", "При подсчёте числа отображаемых строк персон произошла неожиданная ошибка");
        ui->lab_tab_cnt->setText(" -//- ");
    } else {
        query_count->next();
        QString cnt = query_count->value("cnt").toString();
        ui->lab_tab_cnt->setText(cnt);
    }
    delete query_count;

    QApplication::processEvents();

    // ограничение числа отображаемых строк
    sql_pers_lim = sql_pers_all;
    if (ui->ch_tab_limit->isChecked())
        sql_pers_lim += " LIMIT " + QString::number(ui->spin_tab_limit->value()) + " ";

    model_persons.setQuery(sql_pers_lim,db);
    QString err2 = model_persons.lastError().driverText();

    // подключаем модель из БД
    ui->tab_persons->setModel(&model_persons);

    // обновляем таблицу
    ui->tab_persons->reset();

    // задаём ширину колонок
    ui->tab_persons->setColumnWidth(  0,  2);   // e.id as id_pers
    ui->tab_persons->setColumnWidth(  1, 55);   // pol_v
    ui->tab_persons->setColumnWidth(  2,100);   // e.fam
    ui->tab_persons->setColumnWidth(  3,100);   // e.im
    ui->tab_persons->setColumnWidth(  4,120);   // e.ot
    ui->tab_persons->setColumnWidth(  5, 50);   // e.sex
    ui->tab_persons->setColumnWidth(  6,100);   // e.date_birth
    ui->tab_persons->setColumnWidth(  7,  2);   // e.date_death
    ui->tab_persons->setColumnWidth(  8,100);   // e.snils
    ui->tab_persons->setColumnWidth(  9,120);   // e.enp
    ui->tab_persons->setColumnWidth( 10, 80);   // e.date_reg
    ui->tab_persons->setColumnWidth( 11, 50);   // e.bomj
    ui->tab_persons->setColumnWidth( 12, 80);   // e.status
    ui->tab_persons->setColumnWidth( 13,  2);   // e.date_insure_begin
    ui->tab_persons->setColumnWidth( 14,  2);   // e.date_insure_end
    ui->tab_persons->setColumnWidth( 15, 60);   // e.id_udl
    ui->tab_persons->setColumnWidth( 16, 60);   // e.id_drp
    ui->tab_persons->setColumnWidth( 17, 60);   // e.id_pol
    ui->tab_persons->setColumnWidth( 18, 60);   // e.id_asg
    ui->tab_persons->setColumnWidth( 19, 80);  // id_addr_reg
    ui->tab_persons->setColumnWidth( 20, 80);  // id_addr_liv

    // правим заголовки
    model_persons.setHeaderData(  0, Qt::Horizontal, ("ID"));
    model_persons.setHeaderData(  1, Qt::Horizontal, ("полис"));
    model_persons.setHeaderData(  2, Qt::Horizontal, ("фамилия"));
    model_persons.setHeaderData(  3, Qt::Horizontal, ("имя"));
    model_persons.setHeaderData(  4, Qt::Horizontal, ("отчество"));
    model_persons.setHeaderData(  5, Qt::Horizontal, ("пол"));
    model_persons.setHeaderData(  6, Qt::Horizontal, ("дата рождения"));
    model_persons.setHeaderData(  7, Qt::Horizontal, ("дата смерти"));
    model_persons.setHeaderData(  8, Qt::Horizontal, ("СНИЛС"));
    model_persons.setHeaderData(  9, Qt::Horizontal, ("ЕНП"));
    model_persons.setHeaderData( 10, Qt::Horizontal, ("дата\nрегистрац."));
    model_persons.setHeaderData( 11, Qt::Horizontal, ("БОМЖ\n?"));
    model_persons.setHeaderData( 12, Qt::Horizontal, ("статус\nперсоны"));
    ui->tab_persons->repaint();

    ui->pan_persons->setEnabled(true);

    // деактивируем дополнительные компоненты
    ui->lab_pers_status->setEnabled(false);
    ui->ch_pers_status->setEnabled(false);
    ui->combo_pers_status->setCurrentIndex(-1);
    ui->ch_pers_status->setChecked(false);
    ui->ch_insure_begin->setEnabled(false);
    ui->ch_insure_end->setEnabled(false);
    ui->date_insure_begin->setVisible(false);
    ui->date_insure_end->setVisible(false);
    ui->date_insure_begin->setEnabled(false);
    ui->date_insure_end->setEnabled(false);

    QApplication::processEvents();
    ui->tab_persons->setEnabled(true);
    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::on_ln_fam_pers_textChanged(const QString &arg1)
{  ui->ch_fam_pers->setChecked(!(arg1.isEmpty()));  }
void assignment_wnd::on_ln_im_pers_textChanged(const QString &arg1)
{  ui->ch_im_pers->setChecked(!(arg1.isEmpty()));  }
void assignment_wnd::on_ln_ot_pers_textChanged(const QString &arg1)
{  ui->ch_ot_pers->setChecked(!(arg1.isEmpty()));  }
void assignment_wnd::on_date_datr_pers_dateChanged(const QDate &date)
{  ui->ch_datr_pers->setChecked(date>QDate(1920,1,1));  }
void assignment_wnd::on_combo_sex_pers_currentIndexChanged(int index)
{  ui->ch_sex_pers->setChecked(index>0);  }
void assignment_wnd::on_combo_assig_type_currentIndexChanged(int index)
{  }
void assignment_wnd::on_ln_vs_num_textChanged(const QString &arg1)
{  ui->ch_vs_num->setChecked(!(arg1.isEmpty()));  }
void assignment_wnd::on_ln_enp_textChanged(const QString &arg1)
{  ui->ch_enp->setChecked(!(arg1.isEmpty()));  }
void assignment_wnd::on_ln_snils_textChanged(const QString &arg1)
{  ui->ch_snils->setChecked(!(arg1.isEmpty()));  }



void assignment_wnd::on_ch_pers_status_clicked(bool checked) {
    if (checked) {
        QString str = QInputDialog::getText(this,"Введите пароль",
                                            QString("Вы действительно хотитье изменить статус и/или даты страхования выбранной персоны?\n\n"
                                            "Для непосредственной правки статуса застрахованного введите пароль\n") +
                                            (data_app.is_head||data_app.is_tech ? "СТАТУС" : "***")).toUpper();
        if (str!="СТАТУС" && str!="CNFNEC") {
            ui->ch_pers_status->setChecked(false);
            ui->ch_insure_begin->setEnabled(false);
            ui->ch_insure_end->setEnabled(false);
            ui->date_insure_begin->setVisible(false);
            ui->date_insure_end->setVisible(false);
            ui->date_insure_begin->setEnabled(false);
            ui->date_insure_end->setEnabled(false);
            QMessageBox::warning(this,"Отказ","Неправильный пароль");
            return;
        }
    }
    ui->combo_pers_status->setEnabled(checked);
    ui->ch_insure_begin->setEnabled(checked);
    ui->ch_insure_end->setEnabled(checked);
    ui->date_insure_begin->setVisible(checked && ui->ch_insure_begin->isChecked());
    ui->date_insure_end->setVisible(checked && ui->ch_insure_end->isChecked());
    ui->date_insure_begin->setEnabled(checked && ui->ch_insure_begin->isChecked());
    ui->date_insure_end->setEnabled(checked && ui->ch_insure_end->isChecked());
}

void assignment_wnd::on_ch_insure_begin_clicked(bool checked) {
    ui->date_insure_begin->setVisible(checked && ui->ch_insure_begin->isChecked());
    ui->date_insure_begin->setEnabled(checked && ui->ch_insure_begin->isChecked());
    //data_pers.has_date_insure_begin = checked;

    if (!checked) {
        // надо сбросить значение?
        this->setCursor(Qt::WaitCursor);
        // проверим выделенную строку
        QModelIndexList indexes_pers = ui->tab_persons->selectionModel()->selection().indexes();

        if (indexes_pers.size()>0) {
            this->setCursor(Qt::WaitCursor);
            //QModelIndex index_pers = indexes_pers.front();

            db.transaction();

            QSqlQuery *query = new QSqlQuery(db);
            QString sql = "update persons "
                          "   set _date_insure_begin=NULL"
                          " where id=" + QString::number(data_pers.id) + " ; ";
            if (!(mySQL.exec(this, sql, QString("сброс даты начала страхования"), *query, true, db, data_app))) {
                QMessageBox::warning(this, "Ошибка при обновлении записи",
                                     "При обновлении записи произошла неожиданная ошибка.\n" + sql + "\n\nОперация отменена.");

                data_pers.has_date_insure_begin = true;
                ui->date_insure_begin->setDate(data_pers.date_insure_begin);
                ui->date_insure_begin->setVisible(true);
                ui->date_insure_begin->setEnabled(true);
                ui->ch_insure_begin->setChecked(true);
                db.rollback();
                this->setCursor(Qt::ArrowCursor);
                return;
            }
            if (QMessageBox::question(this, "Нужно подтверждение",
                                      "Вы действительно хотите для человека \n\n" + data_pers.fam + " " + data_pers.im + " " + data_pers.ot + " " + "\n"
                                      "сбросить дату начала страхования?",
                                      QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::No) {

                data_pers.has_date_insure_begin = true;
                ui->date_insure_begin->setDate(data_pers.date_insure_begin);
                ui->date_insure_begin->setVisible(true);
                ui->date_insure_begin->setEnabled(true);
                ui->ch_insure_begin->setChecked(true);
                db.rollback();
                this->setCursor(Qt::ArrowCursor);
                return;
            }
            db.commit();

            this->setCursor(Qt::ArrowCursor);
            refresh_persons_tab();
            return;
        } else {
            QMessageBox::warning(this, "Ничего не выбрано",
                                 "Для ручной правки даты страхования персоны надо выбрать строку этого человекав таблице персон.");
        }
        this->setCursor(Qt::ArrowCursor);
    }
}

void assignment_wnd::on_ch_insure_end_clicked(bool checked) {
    ui->date_insure_end->setVisible(checked && ui->ch_insure_end->isChecked());
    ui->date_insure_end->setEnabled(checked && ui->ch_insure_end->isChecked());
    data_pers.has_date_insure_end = checked;

    if (!checked) {
        // надо сбросить значение?
        this->setCursor(Qt::WaitCursor);
        // проверим выделенную строку
        QModelIndexList indexes_pers = ui->tab_persons->selectionModel()->selection().indexes();

        if (indexes_pers.size()>0) {
            this->setCursor(Qt::WaitCursor);
            /*QModelIndex index_pers = indexes_pers.front();*/

            db.transaction();

            QSqlQuery *query = new QSqlQuery(db);
            QString sql = "update persons "
                          "   set _date_insure_end=NULL"
                          " where id=" + QString::number(data_pers.id) + " ; ";
            if (!(mySQL.exec(this, sql, QString("сброс даты начала страхования"), *query, true, db, data_app))) {
                QMessageBox::warning(this, "Ошибка при обновлении записи",
                                     "При обновлении записи произошла неожиданная ошибка.\n" + sql + "\n\nОперация отменена.");

                data_pers.has_date_insure_end = true;
                ui->date_insure_end->setDate(data_pers.date_insure_end);
                ui->date_insure_end->setVisible(true);
                ui->date_insure_end->setEnabled(true);
                ui->ch_insure_end->setChecked(true);
                db.rollback();
                this->setCursor(Qt::ArrowCursor);
                return;
            }
            if (QMessageBox::question(this, "Нужно подтверждение",
                                      "Вы действительно хотите для человека \n\n" + data_pers.fam + " " + data_pers.im + " " + data_pers.ot + " " + "\n"
                                      "сбросить дату окончания страхования?",
                                      QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::No) {

                data_pers.has_date_insure_end = true;
                ui->date_insure_end->setDate(data_pers.date_insure_end);
                ui->date_insure_end->setVisible(true);
                ui->date_insure_end->setEnabled(true);
                ui->ch_insure_end->setChecked(true);
                db.rollback();
                this->setCursor(Qt::ArrowCursor);
                return;
            }
            db.commit();

            this->setCursor(Qt::ArrowCursor);
            refresh_persons_tab();
            return;
        } else {
            QMessageBox::warning(this, "Ничего не выбрано",
                                 "Для ручной правки даты страхования персоны надо выбрать строку этого человекав таблице персон.");
        }
        this->setCursor(Qt::ArrowCursor);
    }
}


void assignment_wnd::on_combo_pers_status_activated(int index) {
    this->setCursor(Qt::WaitCursor);
    // проверим выделенную строку
    QModelIndexList indexes_pers = ui->tab_persons->selectionModel()->selection().indexes();

    if (indexes_pers.size()>0) {
        this->setCursor(Qt::WaitCursor);
        QModelIndex index_pers = indexes_pers.front();

        int status_old = data_pers.status;

        data_pers.status = ui->combo_pers_status->currentData().toInt();
        if (data_pers.status>=0) {
            QMessageBox::warning(this, "Не будет сохранено",
                                 "Вручную можно установить только варианты статуса, соответствующие снятию застрахованного с учёта.");
        } else {
            db.transaction();

            if (QMessageBox::question(this, "Нужно подтверждение",
                                      "Вы действительно хотите для человека \n\n" + data_pers.fam + " " + data_pers.im + " " + data_pers.ot + " " + "\n"
                                      "установить новый статус \n\n\"" + ui->combo_pers_status->currentText() + "\"?",
                                      QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::No) {

                data_pers.status = model_persons.data(model_persons.index(index_pers.row(), 12), Qt::EditRole).toInt();  // persons.status
                ui->combo_pers_status->setCurrentIndex(ui->combo_pers_status->findData(data_pers.status));
                db.rollback();
                this->setCursor(Qt::ArrowCursor);
                return;
            }

            // попробуем закрыть действующие полисы
            QString sql_close_pol;
            sql_close_pol =  "select p.id, p.pol_v, p.vs_num, p.pol_ser, p.pol_num, p.date_begin, p.date_end, p.date_stop ";
            sql_close_pol += " from persons e "
                             "     join polises p on( e.id=p.id_person and "
                             "         /*(date_begin<=CURRENT_DATE) and "
                             "         (p.date_end is NULL or p.date_end>CURRENT_DATE) and*/ "
                             "         (date_stop is NULL or date_stop>CURRENT_DATE) ) "
                             "     left join spr_f008 f on(f.code=p.pol_v)";
            sql_close_pol += " where e.id=" + QString::number(data_pers.id) + " ; ";

            QSqlQuery *query_close_pol = new QSqlQuery(db);
            /*bool res =*/ mySQL.exec(this, sql_close_pol, QString("Проверка полисов застрахованного"), *query_close_pol, true, db, data_app);
            while (query_close_pol->next()) {

                // зададим для полиса date_stop
                QString id_polis= QString::number(query_close_pol->value(0).toInt());
                QString pol_v   = QString::number(query_close_pol->value(1).toInt());
                QString vs_num  = query_close_pol->value(2).toString();
                QString pol_ser = query_close_pol->value(3).toString();
                QString pol_num = query_close_pol->value(4).toString();
                /*QDate date_begin = query_close_pol->value(5).toDate();
                QDate date_end   = query_close_pol->value(6).toDate();
                QDate date_stop  = query_close_pol->value(7).toDate();*/

                QString sql;
                sql += "update polises ";
                sql += " set date_stop=min_date(CURRENT_DATE, date_stop) ";
                sql += " where id=" + id_polis + " ; ";
                QSqlQuery *query = new QSqlQuery(db);
                bool res = mySQL.exec(this, sql, QString("Закрытие полиса в связи с расстрахованием персоны"), *query, true, db, data_app);

                if (!res) {
                    delete query;
                    db.rollback();
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при закрытии полиса", "При закрытии полиса в связи с расстрахованием персоны произошла неожиданная ошибка");
                    return;
                } else {
                    delete query;

                    // обновим статус бланка ВС - сделаем вид, что бланк изъят ... я хз, правильно ли, но ничего другого пока не придумал ...
                    QString sql_v = "update blanks_vs "
                                    "   set status=-2, "
                                    "       date_spent = coalesce(date_spent, CURRENT_DATE) "
                                    " where vs_num='" + vs_num + "' and "
                                    "       id_polis=" + id_polis + " ; ";

                    QSqlQuery *query_v = new QSqlQuery(db);
                    /*bool res_v =*/ mySQL.exec(this, sql_v, QString("Правка статуса бланка ВС"), *query_v, true, db, data_app);
                    delete query_v;

                    // обновим статус бланка полиса - сделаем вид, что бланк изъят ... я хз, правильно ли, но ничего другого пока не придумал ...
                    QString sql_b = "update blanks_pol "
                                    "   set status=-2, "
                                    "       date_spent = coalesce(date_spent, CURRENT_DATE) "
                                    " where pol_ser" + QString(pol_ser.isEmpty() ? " is NULL " : QString("='" + ("0000"+pol_ser).right(4) + "' ")) + " and "
                                    "       pol_num='" + pol_num + "' and "
                                    "       id_polis=" + id_polis + " ; ";

                    QSqlQuery *query_b = new QSqlQuery(db);
                    /*bool res_b =*/ mySQL.exec(this, sql_b, QString("Правка статуса бланка полиса"), *query_b, true, db, data_app);
                    delete query_b;
                }
            }

            if (status_old>0 && ui->combo_pers_status->currentData().toInt()<0) {
                // сформируем событие на закрытие полиса
                QString event_code = "";
                QString event_comment = "";
                switch (ui->combo_pers_status->currentData().toInt()) {
                case -10:
                    event_code = "П022";
                    event_comment = "застрахованный умер";
                    break;
                case -3:
                    event_code = "П021";
                    event_comment = "переезд на другую территорию";
                    break;
                case -2:
                    event_code = "П021";
                    event_comment = "застрахованный сменил СМО";
                    break;
                case -1:
                    event_code = "П025";
                    event_comment = "больше нет действующего полиса";
                    break;
                case -5:
                    event_code = "П023";
                    event_comment = "ФФОМС - нарушение правил страхования";
                    break;
                case -4:
                    event_code = "П024";
                    event_comment = "ТФОМС - закрыт ТФОМС";
                    break;
                case -6:
                    event_code = "П023";
                    event_comment = "ТФОМС - закрыт в связи с дублированием";
                    break;
                default:
                    event_code = "П021";
                    event_comment = " - другие причины - ";
                    break;
                };
                // удалим старые события на закрытие этого полиса
                QString sql_s= "delete from events a "
                               " where id_polis in "
                               "   ( select p.id  from persons e  join polises p on(p.id=e.id_polis) "
                               "     where e.id=" + QString::number(data_pers.id) + " ) "
                               "   and left(event_code,3)='П02' ; "
                // добавим новое событие на закрытие этого полиса
                               "insert into events(id_point, id_operator, id_polis, id_vizit, event_code, event_dt, event_time, status, comment, id_person, id_person_old, id_udl, id_drp) "
                               "select " + QString::number(data_app.id_point) + ", "
                               "       " + QString::number(data_app.id_operator) + ", "
                               "       p.id, "
                               "       -1, "
                               "       '" + event_code + "', "
                               "       '" + QDate::currentDate().toString("yyyy-MM-dd") + "', "
                               "       '" + QTime::currentTime().toString("hh:mm:ss") + "', "
                               "       -1, "
                               "       '" + event_comment + "', "
                               "       e.id, "
                               "       o.id, "
                               "       e.id_udl, "
                               "       e.id_drp "
                               "  from persons e "
                               "       join polises p on(p.id=e.id_polis) "
                               "  left join persons_old o on(o.id_person=e.id) "
                               " where e.id=" + QString::number(data_pers.id) + " ; ";

                QSqlQuery *query_s= new QSqlQuery(db);
                bool res_s= mySQL.exec(this, sql_s, QString("Сформируем событие закрытия полиса"), *query_s, true, db, data_app);
                delete query_s;

                if (!res_s) {
                    delete query_s;
                    db.rollback();
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при генерации события закрытия полиса", "При генерации события закрытия полиса в связи с расстрахованием персоны произошла неожиданная ошибка");
                    return;
                }
            }

            // сохраним новый статус
            QSqlQuery *query = new QSqlQuery(db);
            QString sql = "update persons "
                          " set status=" + QString::number(data_pers.status) + ", "
                          "     _id_last_point=" + QString::number(data_app.id_point) + ", "
                          "     _id_last_operator=" + QString::number(data_app.id_operator) + ", "
                          "     _date_insure_end=min_date(_date_insure_end, CURRENT_DATE) "
                          " where id=" + QString::number(data_pers.id) + " ; ";
            if (!(mySQL.exec(this, sql, QString("Список статусов персоны"), *query, true, db, data_app))) {
                QMessageBox::warning(this, "Ошибка при обновлении записи",
                                     "При обновлении записи произошла неожиданная ошибка.\n" + sql + "\n\nОперация отменена.");
                data_pers.status = model_persons.data(model_persons.index(index_pers.row(), 12), Qt::EditRole).toInt();  // persons.status
                ui->combo_pers_status->setCurrentIndex(ui->combo_pers_status->findData(data_pers.status));
                db.rollback();
                this->setCursor(Qt::ArrowCursor);
                return;
            }

            db.commit();

            this->setCursor(Qt::ArrowCursor);
            refresh_persons_tab();
            return;
        }
    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Для ручной правки статуса персоны надо выбрать строку этого человекав таблице персон.");
    }
    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::on_date_insure_begin_editingFinished() {
    // проверим, есть ли что менять
    if ( data_pers.date_insure_begin!=ui->date_insure_begin->date() &&
         ui->date_insure_begin->date()>QDate(1920,1,1) ) {

        this->setCursor(Qt::WaitCursor);
        // проверим выделенную строку
        QModelIndexList indexes_pers = ui->tab_persons->selectionModel()->selection().indexes();

        if (indexes_pers.size()>0) {
            this->setCursor(Qt::WaitCursor);
            QModelIndex index_pers = indexes_pers.front();

            data_pers.date_insure_begin = ui->date_insure_begin->date();
            if (data_pers.date_insure_begin<QDate(2014,1,1)) {
                QMessageBox::warning(this, "Не будет сохранено",
                                     "Дата начала страхования в системе не может быть раньше 01.01.2014 г.");
            } else {
                // сохраним новый дату
                db.transaction();
                QSqlQuery *query = new QSqlQuery(db);
                QString sql = "update persons "
                              "   set _date_insure_begin=" + (ui->ch_insure_begin->isChecked() ? QString("'" + data_pers.date_insure_begin.toString("yyyy-MM-dd") + "'") : "NULL" ) + ", "
                              "       _id_last_point=" + QString::number(data_app.id_point) + ", "
                              "       _id_last_operator=" + QString::number(data_app.id_operator) + " "
                              " where id=" + QString::number(data_pers.id) + " ; ";
                if (!(mySQL.exec(this, sql, QString("новая дата начала страхования"), *query, true, db, data_app))) {
                    QMessageBox::warning(this, "Ошибка при обновлении записи",
                                         "При обновлении записи произошла неожиданная ошибка.\n" + sql + "\n\nОперация отменена.");

                    data_pers.date_insure_begin = model_persons.data(model_persons.index(index_pers.row(), 13), Qt::EditRole).toDate();  // persons.date_insure_begin
                    ui->date_insure_begin->setDate(data_pers.date_insure_begin);
                    db.rollback();
                    this->setCursor(Qt::ArrowCursor);
                    return;
                }
                if (QMessageBox::question(this, "Нужно подтверждение",
                                          "Вы действительно хотите для человека \n\n" + data_pers.fam + " " + data_pers.im + " " + data_pers.ot + " " + "\n"
                                          "установить новую дату начала страхования \n\n" + ui->date_insure_begin->date().toString("dd.MM.yyyy") + " ?",
                                          QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::No) {

                    data_pers.date_insure_begin = model_persons.data(model_persons.index(index_pers.row(), 14), Qt::EditRole).toDate();  // persons.date_insure_begin
                    ui->date_insure_begin->setDate(data_pers.date_insure_begin);
                    db.rollback();
                    this->setCursor(Qt::ArrowCursor);
                    return;
                }
                db.commit();

                this->setCursor(Qt::ArrowCursor);
                refresh_persons_tab();
                return;
            }
        } else {
            QMessageBox::warning(this, "Ничего не выбрано",
                                 "Для ручной правки даты страхования персоны надо выбрать строку этого человекав таблице персон.");
        }
        this->setCursor(Qt::ArrowCursor);
    }
}

void assignment_wnd::on_date_insure_end_editingFinished() {
    // проверим, есть ли что менять
    if ( data_pers.date_insure_end!=ui->date_insure_end->date() &&
         ui->date_insure_end->date()>QDate(1920,1,1) ) {

        this->setCursor(Qt::WaitCursor);
        // проверим выделенную строку
        QModelIndexList indexes_pers = ui->tab_persons->selectionModel()->selection().indexes();

        if (indexes_pers.size()>0) {
            this->setCursor(Qt::WaitCursor);
            QModelIndex index_pers = indexes_pers.front();

            data_pers.date_insure_end = ui->date_insure_end->date();
            if ( data_pers.date_insure_end<=data_pers.date_insure_begin ||
                 data_pers.date_insure_end>QDate::currentDate() ) {
                QMessageBox::warning(this, "Не будет сохранено",
                                     "Дата окончания страхования в системе не может быть раньше даты начала страхования или позже текущей даты " + QDate::currentDate().toString("dd.MM.yyyy") + " г.");
            } else {
                // сохраним новый дату
                db.transaction();
                QSqlQuery *query = new QSqlQuery(db);
                QString sql = "update persons "
                              "   set _date_insure_end=" + (ui->ch_insure_end->isChecked() ? QString("'" + data_pers.date_insure_end.toString("yyyy-MM-dd") + "'") : "NULL" ) + ", "
                              "       _id_last_point=" + QString::number(data_app.id_point) + ", "
                              "       _id_last_operator=" + QString::number(data_app.id_operator) + " "
                              " where id=" + QString::number(data_pers.id) + " ; ";
                if (!(mySQL.exec(this, sql, QString("новая дата начала страхования"), *query, true, db, data_app))) {
                    QMessageBox::warning(this, "Ошибка при обновлении записи",
                                         "При обновлении записи произошла неожиданная ошибка.\n" + sql + "\n\nОперация отменена.");

                    data_pers.date_insure_end = model_persons.data(model_persons.index(index_pers.row(), 14), Qt::EditRole).toDate();  // persons.date_insure_end
                    ui->date_insure_end->setDate(data_pers.date_insure_end);
                    db.rollback();
                    this->setCursor(Qt::ArrowCursor);
                    return;
                }
                if (QMessageBox::question(this, "Нужно подтверждение",
                                          "Вы действительно хотите для человека \n\n" + data_pers.fam + " " + data_pers.im + " " + data_pers.ot + " " + "\n"
                                          "установить новую дату окончания страхования \n\n" + ui->date_insure_end->date().toString("dd.MM.yyyy") + " ?",
                                          QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::No) {

                    data_pers.date_insure_end = model_persons.data(model_persons.index(index_pers.row(), 14), Qt::EditRole).toDate();  // persons.date_insure_end
                    ui->date_insure_end->setDate(data_pers.date_insure_end);
                    db.rollback();
                    this->setCursor(Qt::ArrowCursor);
                    return;
                }
                db.commit();

                this->setCursor(Qt::ArrowCursor);
                refresh_persons_tab();
                return;
            }
        } else {
            QMessageBox::warning(this, "Ничего не выбрано",
                                 "Для ручной правки даты страхования персоны надо выбрать строку этого человекав таблице персон.");
        }
        this->setCursor(Qt::ArrowCursor);
    }
}



void assignment_wnd::on_bn_clear_filter_pers_clicked() {
    ui->ln_fam_pers->clear();
    ui->ln_im_pers->clear();
    ui->ln_ot_pers->clear();
    ui->date_datr_pers->setDate(QDate(1900,1,1));
    //ui->combo_assig_type->setCurrentIndex(0);
    ui->combo_sex_pers->setCurrentIndex(0);
    ui->combo_medorg_pers->setCurrentIndex(0);

    ui->ln_vs_num->clear();
    ui->ln_enp->clear();
    ui->ln_snils->clear();

    refresh_persons_tab();
}

void assignment_wnd::on_ch_medorg_pers_clicked(bool checked) {
    ui->combo_medorg_pers->setEnabled(checked);
    refresh_persons_tab();
}

void assignment_wnd::on_bn_refresh_pers_clicked()
{  refresh_persons_tab();  }
void assignment_wnd::on_combo_medorg_pers_activated(int index)
{  refresh_persons_tab();  }
void assignment_wnd::on_ln_fam_pers_returnPressed()
{  refresh_persons_tab();  }
void assignment_wnd::on_ln_im_pers_returnPressed()
{  refresh_persons_tab();  }
void assignment_wnd::on_ln_ot_pers_returnPressed()
{  refresh_persons_tab();  }
void assignment_wnd::on_date_datr_pers_editingFinished()
{  refresh_persons_tab();  }
void assignment_wnd::on_combo_sex_pers_activated(const QString &arg1)
{  refresh_persons_tab();  }

void assignment_wnd::on_ln_vs_num_returnPressed()
{  refresh_persons_tab();  }
void assignment_wnd::on_ln_enp_returnPressed()
{  refresh_persons_tab();  }
void assignment_wnd::on_ln_snils_returnPressed()
{  refresh_persons_tab();  }

void assignment_wnd::on_ch_arch_clicked(bool checked)
{  refresh_persons_tab();  }


void assignment_wnd::on_combo_assig_type_activated(const QString &arg1) {
    ui->ch_assig_type->setChecked(true);
    refresh_assigs_tab();
}

void assignment_wnd::on_combo_pers_age_activated(const QString &arg1) {
    ui->ch_pers_age->setChecked(true);
    refresh_persons_tab();
}

void assignment_wnd::on_ch_act_pers_clicked(bool checked)
{  refresh_persons_tab();  }
















void assignment_wnd::refresh_medorgs_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }

    QString medorgs_sql;
    medorgs_sql  = "select id, ocato, code_mo, name_mo, name_mo_full, address, is_head, code_head, phone, fax, email, www, comment, oid_mo, face_phone, face_post, face_fio ";
    medorgs_sql += "  from spr_medorgs ";
    medorgs_sql += " where is_head=1 ";
    QString filter = ui->ln_filter->text();
    if (!filter.trimmed().isEmpty()) {
        medorgs_sql += " and upper(name_mo) like(upper('%" + filter + "%')) ";
    }
    medorgs_sql += " order by ocato, code_mo, is_head ; ";

    model_medorgs.setQuery(medorgs_sql,db);
    QString err2 = model_medorgs.lastError().driverText();

    // подключаем модель из БД
    ui->tab_medorgs->setModel(&model_medorgs);

    // обновляем таблицу
    ui->tab_medorgs->reset();

    // задаём ширину колонок
    ui->tab_medorgs->setColumnWidth( 0,  2);
    ui->tab_medorgs->setColumnWidth( 1, 50);     // ocato,
    ui->tab_medorgs->setColumnWidth( 2, 75);     // code_mo,
    ui->tab_medorgs->setColumnWidth( 3,300);     // name_mo,
    ui->tab_medorgs->setColumnWidth( 4,  2);     // name_mo_full,
    ui->tab_medorgs->setColumnWidth( 5,150);     // address,
    ui->tab_medorgs->setColumnWidth( 6, 30);     // is_head,
    ui->tab_medorgs->setColumnWidth( 7, 75);     // code_head,
    ui->tab_medorgs->setColumnWidth( 8, 99);     // phone,
    ui->tab_medorgs->setColumnWidth( 9, 99);     // fax,
    ui->tab_medorgs->setColumnWidth(10,120);     // email,
    ui->tab_medorgs->setColumnWidth(11,200);     // www,
    ui->tab_medorgs->setColumnWidth(12,200);     // comment
    ui->tab_medorgs->setColumnWidth(13,200);     // oid_mo
    ui->tab_medorgs->setColumnWidth(14, 99);     // face_phone
    ui->tab_medorgs->setColumnWidth(15,100);     // face_post
    ui->tab_medorgs->setColumnWidth(16,100);     // face_fio
    // правим заголовки
    model_medorgs.setHeaderData( 1, Qt::Horizontal, ("ОКАТО"));
    model_medorgs.setHeaderData( 2, Qt::Horizontal, ("код МО"));
    model_medorgs.setHeaderData( 3, Qt::Horizontal, ("название"));
    model_medorgs.setHeaderData( 4, Qt::Horizontal, ("полное название"));
    model_medorgs.setHeaderData( 5, Qt::Horizontal, ("адрес"));
    model_medorgs.setHeaderData( 6, Qt::Horizontal, ("Гл?"));
    model_medorgs.setHeaderData( 7, Qt::Horizontal, ("код глав."));
    model_medorgs.setHeaderData( 8, Qt::Horizontal, ("телефон"));
    model_medorgs.setHeaderData( 9, Qt::Horizontal, ("факс"));
    model_medorgs.setHeaderData(10, Qt::Horizontal, ("e-mail"));
    model_medorgs.setHeaderData(12, Qt::Horizontal, ("примечания"));
    model_medorgs.setHeaderData(13, Qt::Horizontal, ("ОИД МО"));
    ui->tab_medorgs->repaint();

    //data_medorg.id = -1;
    /*data_medorg.code_mo = "";
    //data_medorg.name_mo = "";
    data_medorg.code_mp = "";
    data_medorg.code_ms = "";
    data_medorg.code_mt = "";*/

    refresh_head_mdorgs();

    ui->ln_ocato->setText("");
    ui->ln_code_mo->setText("");
    ui->ln_oid_mo->setText("");
    ui->ln_name_mo->setText("");
    ui->ln_name_mo_full->setText("");
    ui->ln_address->setText("");
    ui->ch_is_head->setChecked(true);
    ui->combo_head_mo->setCurrentIndex(-1);
    ui->ln_phone->setText("");
    ui->ln_face_phone->setText("");
    ui->ln_face_post->setText("");
    ui->ln_face_fio->setText("");
    ui->ln_fax->setText("");
    ui->ln_email->setText("");
    ui->ln_www->setText("");
    ui->ln_comment->setText("");

    //ui->bn_ok->setEnabled(true);

    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::refresh_head_mdorgs() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка медорганизаций
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mo, '(' || code_mo || ') ' || name_mo "
                  "  from spr_medorgs s "
                  " where is_head=1 "
                  " order by code_mo ; ";
    mySQL.exec(this, sql, QString("Справочник медорганизаций"), *query, true, db, data_app);
    ui->combo_head_mo->clear();
    while (query->next()) {
        ui->combo_head_mo->addItem(query->value(1).toString(), query->value(0).toString());
    }
    ui->combo_head_mo->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::on_bn_refresh_clicked() {
    refresh_medorgs_tab();
}

void assignment_wnd::on_bn_add_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medorgs || !ui->tab_medorgs->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица медорганизаций не загружена!\nНажмите кнопку [Обновить] и выберите МО из списка.");
        return;
    }

    // проверка, введен ли код МО
    if (ui->ln_code_mo->text().trimmed().isEmpty() ||
            ui->ln_code_mo->text().trimmed().length()!=6) {
        QMessageBox::warning(this, "Недостаточно данных", "Реестровый номер медорганизации должен состоять из 6 цифр без разделителей и пробелов не может быть пустым.\n\nСохранение отменено.");
        return;
    }

    // проверка, введен ли ОИД МО
    if (!ui->ln_oid_mo->text().trimmed().isEmpty()) {
        if (ui->ln_oid_mo->text().trimmed().length()<10) {
            QMessageBox::warning(this, "Недостаточно данных", "ОИД медорганизации не может быть короче 10 символов.\n\nСохранение отменено.");
            return;
        }
    }

    // проверка, введено ли название МО
    if (ui->ln_name_mo->text().trimmed().simplified().isEmpty()) {
        QMessageBox::warning(this, "Недостаточно данных", "Название медицинской организации не может быть пустым.\n\nСохранение отменено.");
        return;
    }

    // проверим кода на уникальность
    QString sql = "select count(*) from spr_medorgs "
                  " where code_mo='" + ui->ln_code_mo->text().trimmed() + "' ; ";

    QSqlQuery *query = new QSqlQuery(db);
    if (!mySQL.exec(this, sql, "Проверка кода МО на уникальность", *query, true, db, data_app) || !query->next()) {
        QMessageBox::warning(this, "Непредвиденная ошибка", "При попытке проверить код МО на уникальность произошла неожиданная ошибка.\n\nСохранение отменено.");
        delete query;
        return;
    }
    if (query->value(0).toInt()>0) {
        QMessageBox::warning(this,
                              "Такой код медицинской организации уже есть",
                              "В базе данных уже есть медицинская организация с таким же реестровым номером.\nРеестровый номер МО должен быть уникальным.\n\nСохранение отменено.");
        delete query;
        return;
    }

    if (!ui->ln_oid_mo->text().trimmed().isEmpty()) {
        // проверим ОИД на уникальность
        QString sql_oid = "select count(*) from spr_medorgs "
                      " where oid_mo='" + ui->ln_oid_mo->text().trimmed() + "' ; ";

        QSqlQuery *query_oid = new QSqlQuery(db);
        if (!mySQL.exec(this, sql_oid, "Проверка ОИД МО на уникальность", *query_oid, true, db, data_app) || !query_oid->next()) {
            QMessageBox::warning(this, "Непредвиденная ошибка", "При попытке проверить ОИД МО на уникальность произошла неожиданная ошибка.\n\nСохранение отменено.");
            delete query_oid;
            return;
        }
        if (query_oid->value(0).toInt()>0) {
            QMessageBox::warning(this,
                                  "Такой ОИД медицинской организации уже есть",
                                  "В базе данных уже есть медицинская организация с таким же ОИД.\nОИД МО должен быть уникальным.\n\nСохранение отменено.");
            delete query;
            return;
        }
    }


    if (QMessageBox::question(this,
                              "Нужно подтверждение",
                              "Добавить новую медицинскую организацию?\n",
                              QMessageBox::Yes|QMessageBox::Cancel,
                              QMessageBox::Yes)==QMessageBox::Yes) {
        db.transaction();
        // добавим МО
        QString sql = "insert into spr_medorgs (code_mo, name_mo, name_mo_full, address, is_head, code_head, phone, email, fax, www, comment, face_phone, face_post, face_fio) "
                      " values ("
                      "   '" + ui->ln_code_mo->text().trimmed().simplified() + "', "
                      "   '" + ui->ln_oid_mo->text().trimmed().simplified() + "', "
                      "   '" + ui->ln_name_mo->text().trimmed().simplified() + "', "
                      "   '" + ui->ln_name_mo_full->text().trimmed().simplified() + "', "
                      "   '" + ui->ln_address->text().trimmed().simplified() + "', "
                      "   " + QString(ui->ch_is_head->isChecked() ? "1" : "0") + ", "
                      "   " + QString(ui->ch_is_head->isChecked() ? "NULL" : ("'" + ui->combo_head_mo->currentData().toString() + "'")) + ", "
                      "   '" + ui->ln_phone->text().trimmed().simplified() + "', "
                      "   '" + ui->ln_fax->text().trimmed().simplified() + "', "
                      "   '" + ui->ln_email->text().trimmed().simplified() + "', "
                      "   '" + ui->ln_www->text().trimmed().simplified() + "', "
                      "   '" + ui->ln_face_phone->text().trimmed().simplified() + "', "
                      "   '" + ui->ln_face_post->text().trimmed().simplified() + "', "
                      "   '" + ui->ln_face_fio->text().trimmed().simplified() + "', "
                      "   '" + ui->ln_comment->toPlainText().trimmed().simplified() + "') "
                      " returning id ; ";

        QSqlQuery *query = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Добавление новой медорганизации", *query, true, db, data_app) || !query->next()) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую медорганизацию произошла неожиданная ошибка.\n\nСохранение отменено.");
            delete query;
            return;
        }
        //data_medorg.id = query->value(0).toInt();
        //data_medorg.code_mo = ui->ln_code_mo->text();
        //data_medorg.name_mo = ui->ln_name_mo->text();
        delete query;
        db.commit();

        refresh_medorgs_tab();
    }
}

void assignment_wnd::on_tab_medorgs_clicked(const QModelIndex &index) {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medorgs || !ui->tab_medorgs->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица медорганизаций не загружена!\nНажмите кнопку [Обновить] и выберите МО из списка.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medorgs->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        //data_medorg.id = model_medorgs.data(model_medorgs.index(index.row(), 0), Qt::EditRole).toInt();
        //data_medorg.code_mo = model_medorgs.data(model_medorgs.index(index.row(), 2), Qt::EditRole).toString();
        //data_medorg.name_mo = model_medorgs.data(model_medorgs.index(index.row(), 3), Qt::EditRole).toString();

        ui->ln_ocato->setText(model_medorgs.data(model_medorgs.index(index.row(), 1), Qt::EditRole).toString());
        ui->ln_code_mo->setText(model_medorgs.data(model_medorgs.index(index.row(), 2), Qt::EditRole).toString());
        ui->ln_oid_mo->setText(model_medorgs.data(model_medorgs.index(index.row(), 13), Qt::EditRole).toString());
        ui->ln_name_mo->setText(model_medorgs.data(model_medorgs.index(index.row(), 3), Qt::EditRole).toString());
        ui->ln_name_mo_full->setText(model_medorgs.data(model_medorgs.index(index.row(), 4), Qt::EditRole).toString());
        ui->ln_address->setText(model_medorgs.data(model_medorgs.index(index.row(), 5), Qt::EditRole).toString());
        ui->ln_phone->setText(model_medorgs.data(model_medorgs.index(index.row(), 8), Qt::EditRole).toString());
        ui->ln_face_phone->setText(model_medorgs.data(model_medorgs.index(index.row(),14), Qt::EditRole).toString());
        ui->ln_face_post->setText(model_medorgs.data(model_medorgs.index(index.row(),15), Qt::EditRole).toString());
        ui->ln_face_fio->setText(model_medorgs.data(model_medorgs.index(index.row(),16), Qt::EditRole).toString());
        ui->ln_fax->setText(model_medorgs.data(model_medorgs.index(index.row(), 9), Qt::EditRole).toString());
        ui->ln_email->setText(model_medorgs.data(model_medorgs.index(index.row(),10), Qt::EditRole).toString());
        ui->ln_www->setText(model_medorgs.data(model_medorgs.index(index.row(),11), Qt::EditRole).toString());
        ui->ln_comment->setText(model_medorgs.data(model_medorgs.index(index.row(),12), Qt::EditRole).toString());

        bool is_head_mo = model_medorgs.data(model_medorgs.index(index.row(), 6), Qt::EditRole).toInt()==1;
        ui->ch_is_head->setChecked(is_head_mo);
        ui->combo_head_mo->setCurrentIndex(ui->combo_head_mo->findData(model_medorgs.data(model_medorgs.index(index.row(), 7), Qt::EditRole).toString()));
        ui->lab_head_mo->setEnabled(!is_head_mo);
        ui->combo_head_mo->setEnabled(!is_head_mo);
        ui->lab_is_head->setEnabled(is_head_mo);

        ui->combo_medsite_mo->setCurrentIndex(ui->combo_medsite_mo->findData(ui->ln_code_mo->text()));
        refresh_medsite_mp();
        ui->combo_medter_mo->setCurrentIndex(ui->combo_medsite_mo->findData(ui->ln_code_mo->text()));
        //refresh_medter_mp();

    } else {
        //data_medorg.id = -1;
        //data_medorg.code_mo = "";
        //data_medorg.name_mo = "";

        ui->ln_code_mo->setText("");
        ui->ln_name_mo->setText("");
        ui->ln_oid_mo->setText("");
        ui->ln_name_mo_full->setText("");
        ui->ln_address->setText("");
        ui->ch_is_head->setChecked(true);
        ui->combo_head_mo->setCurrentIndex(-1);
        ui->ln_phone->setText("");
        ui->ln_face_phone->setText("");
        ui->ln_face_post->setText("");
        ui->ln_face_fio->setText("");
        ui->ln_fax->setText("");
        ui->ln_email->setText("");
        ui->ln_www->setText("");
        ui->ln_comment->setText("");
    }

    refresh_medparts_tab();
    refresh_medsites_tab();
    refresh_medters_tab();

    //refresh_medsites_tab();
    //refresh_medsite_mo();

    //refresh_medters_tab();
    //refresh_medter_mo();

    if (ui->ch_medsites_mo->isChecked()) {
        refresh_medsites_tab();
    }
    if (ui->ch_medters_mo->isChecked()) {
        refresh_medters_tab();
    }

    refresh_medsite_ters_tab();
}

void assignment_wnd::on_bn_edit_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medorgs || !ui->tab_medorgs->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица медорганизаций не загружена!\nНажмите кнопку [Обновить] и выберите МО из списка.");
        return;
    }

    // проверка, введен ли код организации
    if (ui->ln_code_mo->text().trimmed().simplified().isEmpty()) {
        QMessageBox::warning(this, "Недостаточно данных", "Реестровый номер организации не может быть пустым.\n\nИзменение отменено.");
        return;
    }

    // проверка, введено ли название организации
    if (ui->ln_name_mo->text().trimmed().simplified().isEmpty()) {
        QMessageBox::warning(this, "Недостаточно данных", "Название организации не может быть пустым.\n\nИзменение отменено.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medorgs->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        if (QMessageBox::question(this,
                                  "Нужно подтверждение",
                                  "Изменить данные выбранной медицинской организации?",
                                  QMessageBox::Yes|QMessageBox::Cancel,
                                  QMessageBox::Yes)==QMessageBox::Cancel) {
            return;
        }

        QModelIndex index = indexes.front();
        QString ID = QString::number(model_medorgs.data(model_medorgs.index(index.row(), 0), Qt::EditRole).toInt());


        // проверим кода на уникальность
        QString sql = "select count(*) from spr_medorgs "
                      " where "
                      "    id<>" + ID + " and is_head=1 and "
                      "    code_mo='" + ui->ln_code_mo->text().trimmed() + "' ; ";

        QSqlQuery *query = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Проверка кода МО на уникальность", *query, true, db, data_app) || !query->next()) {
            QMessageBox::warning(this,
                                 "Непредвиденная ошибка",
                                 "При попытке проверить код МО на уникальность произошла неожиданная ошибка.\n\n"
                                 "Изменение отменено.");
            delete query;
            return;
        }
        query->next();
        if (query->value(0).toInt()>0) {
            QMessageBox::warning(this,
                                  "Такой код медицинской организации уже есть",
                                  "В базе данных уже есть медицинская организация с таким же реестровым номером.\n"
                                  "Реестровый номер МО должен быть уникальным.\n\n" +
                                  sql + "\n\n" +
                                  QString::number(query->value(0).toInt()) + "\n\n"
                                  "Изменение отменено.");
            delete query;
            return;
        }

        if (!ui->ln_code_mo->text().trimmed().isEmpty()) {
            // проверим ОИД на уникальность
            QString sql_oid = "select count(*) from spr_medorgs "
                          " where "
                          "    id<>" + ID + " and "
                          "    oid_mo='" + ui->ln_oid_mo->text().trimmed() + "' ; ";

            QSqlQuery *query_oid = new QSqlQuery(db);
            if (!mySQL.exec(this, sql_oid, "Проверка ОИД МО на уникальность", *query_oid, true, db, data_app) || !query_oid->next()) {
                QMessageBox::warning(this,
                                     "Непредвиденная ошибка",
                                     "При попытке проверить ОИД МО на уникальность произошла неожиданная ошибка.\n\n"
                                     "Изменение отменено.");
                delete query_oid;
                return;
            }
            query_oid->next();
            if (query_oid->value(0).toInt()>0) {
                QMessageBox::warning(this,
                                      "Такой ОИД медицинской организации уже есть",
                                      "В базе данных уже есть медицинская организация с таким же ОИД.\n"
                                      "ОИД МО должен быть уникальным.\n\n" +
                                      sql + "\n\n" +
                                      QString::number(query_oid->value(0).toInt()) + "\n\n"
                                      "Изменение отменено.");
                delete query_oid;
                return;
            }
        }


        // проверим названия на уникальность
        QString sql2 ="select count(*) from spr_medorgs "
                      " where "
                      "    id<>" + ID + " and "
                      "    name_mo='" + ui->ln_name_mo->text().trimmed() + "' ; ";

        QSqlQuery *query2 = new QSqlQuery(db);
        if (!mySQL.exec(this, sql2, "Проверка названия МО на уникальность", *query2, true, db, data_app) || !query2->next()) {
            QMessageBox::warning(this,
                                 "Непредвиденная ошибка",
                                 "При попытке проверить названия МО на уникальность произошла неожиданная ошибка.\n\n"
                                 "Изменение отменено.");
            delete query2;
            return;
        }
        query2->next();
        if (query2->value(0).toInt()>0) {
            QMessageBox::warning(this,
                                  "Такой код медицинской организации уже есть",
                                  "В базе данных уже есть медицинская организация с таким же названием.\n"
                                  "Название МО должен быть уникальным.\n\n" +
                                  sql2 + "\n\n" +
                                  QString::number(query2->value(0).toInt()) + "\n\n"
                                  "Изменение отменено.");
            delete query2;
            return;
        }


        db.transaction();
        // добавим фирму
        QString sql_upd = "update spr_medorgs "
                      " set "
                      "   ocato='" + ui->ln_ocato->text().trimmed().simplified() + "', "
                      "   code_mo='" + ui->ln_code_mo->text().trimmed().simplified() + "', "
                      "   oid_mo='" + ui->ln_oid_mo->text().trimmed().simplified() + "', "
                      "   name_mo='" + ui->ln_name_mo->text().trimmed().simplified() + "', "
                      "   name_mo_full='" + ui->ln_name_mo_full->text().trimmed().simplified() + "', "
                      "   address='" + ui->ln_address->text().trimmed().simplified() + "', "
                      "   is_head=" + QString(ui->ch_is_head->isChecked() ? "1" : "0") + ", "
                      "   code_head=" + QString(ui->ch_is_head->isChecked() ? "NULL" : ("'" + ui->combo_head_mo->currentData().toString() + "'")) + ", "
                      "   phone='" + ui->ln_phone->text().trimmed().simplified() + "', "
                      "   fax='" + ui->ln_fax->text().trimmed().simplified() + "', "
                      "   email='" + ui->ln_email->text().trimmed().simplified() + "', "
                      "   www='" + ui->ln_www->text().trimmed().simplified() + "', "
                      "   face_phone='" + ui->ln_face_phone->text().trimmed().simplified() + "', "
                      "   face_post='" + ui->ln_face_post->text().trimmed().simplified() + "', "
                      "   face_fio='" + ui->ln_face_fio->text().trimmed().simplified() + "', "
                      "   comment='" + ui->ln_comment->toPlainText().trimmed().simplified() + "' "
                      " where id=" + ID + " ; ";

        QSqlQuery *query_upd = new QSqlQuery(db);
        if (!mySQL.exec(this, sql_upd, "Обновление медорганизации", *query_upd, true, db, data_app)) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке изменить данные медицинской организации произошла неожиданная ошибка.\n\nИзменение отменено.");
            delete query_upd;
            return;
        }
        //data_medorg.code_mo = ui->ln_code_mo->text();
        //data_medorg.name_mo = ui->ln_name_mo->text();
        delete query_upd;
        db.commit();

        refresh_medorgs_tab();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано", "Ничего не выбрано");
    }
}

void assignment_wnd::on_bn_delete_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medorgs || !ui->tab_medorgs->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица медорганизаций не загружена!\nНажмите кнопку [Обновить] и выберите МО из списка.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medorgs->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        if (QMessageBox::question(this,
                                  "Нужно подтверждение",
                                  "Удалить выбранную запись из базы?\n(Все лица, приписанные к удаляемой фирме будут от неё откреплены)\n",
                                  QMessageBox::Yes|QMessageBox::Cancel,
                                  QMessageBox::Yes)==QMessageBox::Cancel) {
            return;
        }

        QModelIndex index = indexes.front();
        QString ID = QString::number(model_medorgs.data(model_medorgs.index(index.row(), 0), Qt::EditRole).toInt());

        QString sql = "delete from spr_medorgs "
                      " where id=" + ID + " ; ";
        QSqlQuery *query2 = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Удаление медорганизации", *query2, true, db, data_app)) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке удалить медицинскую организацию произошла неожиданная ошибка.\n\nУдаление отменено.");
            delete query2;
            return;
        }
        //data_medorg.id = -1;
        //data_medorg.code_mo = "";
        //data_medorg.name_mo = "";
        delete query2;

        db.commit();

        refresh_medorgs_tab();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано", "Ничего не выбрано");
    }
}

void assignment_wnd::on_bn_ocato_clicked() {
    //ui->ln_ocato->setText(data_app.ocato);
}
/*
void assignment_wnd::on_ln_ocato_editingFinished() {
    ui->bn_ok->setEnabled(false);   }
void assignment_wnd::on_ln_code_mo_editingFinished() {
    ui->bn_ok->setEnabled(false);   }
void assignment_wnd::on_ln_name_mo_editingFinished() {
    ui->bn_ok->setEnabled(false);   }
void assignment_wnd::on_ln_name_mo_full_editingFinished() {
    ui->bn_ok->setEnabled(false);   }
void assignment_wnd::on_combo_head_mo_activated(int index) {
    ui->bn_ok->setEnabled(false);   }
void assignment_wnd::on_ln_phone_editingFinished() {
    ui->bn_ok->setEnabled(false);   }
void assignment_wnd::on_ln_fax_editingFinished() {
    ui->bn_ok->setEnabled(false);   }
void assignment_wnd::on_ln_email_editingFinished() {
    ui->bn_ok->setEnabled(false);   }
void assignment_wnd::on_ln_www_editingFinished() {
    ui->bn_ok->setEnabled(false);   }
void assignment_wnd::on_ln_comment_editingFinished() {
    ui->bn_ok->setEnabled(false);   }
*/
void assignment_wnd::on_ch_is_head_clicked(bool checked) {
    ui->lab_head_mo->setEnabled(!checked);
    ui->combo_head_mo->setEnabled(!checked);
    ui->lab_is_head->setEnabled(checked);
}


void assignment_wnd::refresh_medparts_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }

    QString medparts_sql;
    medparts_sql  = "select s.id, s.code_mo, s.code_mp, s.name_mp, s.type_mp, '('||s.type_mp||') '||t.text as type_mp_text ";
    medparts_sql += "  from spr_medparts s left join spr_medparts_types t on (s.type_mp=t.code) ";

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medorgs->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();
        QString code_mo = model_medorgs.data(model_medorgs.index(index.row(), 2), Qt::EditRole).toString();
        medparts_sql += " where code_mo='" + code_mo + "' ";
    }
    medparts_sql += " order by s.code_mo, s.code_mp ; ";
    //QInputDialog::getText(this, "", "", QLineEdit::Normal, medparts_sql);

    model_medparts.setQuery(medparts_sql,db);
    QString err2 = model_medparts.lastError().driverText();

    // подключаем модель из БД
    ui->tab_medparts->setModel(&model_medparts);

    // обновляем таблицу
    ui->tab_medparts->reset();

    // задаём ширину колонок
    ui->tab_medparts->setColumnWidth( 0,  2);   // id,
    ui->tab_medparts->setColumnWidth( 1, 50);   // code_mo,
    ui->tab_medparts->setColumnWidth( 2, 50);   // code_mp,
    ui->tab_medparts->setColumnWidth( 3,200);   // name_mp,
    ui->tab_medparts->setColumnWidth( 4,  2);   // type_mp,
    ui->tab_medparts->setColumnWidth( 5,200);   // type_mp_text
    // правим заголовки
    model_medparts.setHeaderData( 1, Qt::Horizontal, ("МО"));
    model_medparts.setHeaderData( 2, Qt::Horizontal, ("код"));
    model_medparts.setHeaderData( 4, Qt::Horizontal, ("подразделение"));
    model_medparts.setHeaderData( 5, Qt::Horizontal, ("тип"));
    model_medparts.setHeaderData( 6, Qt::Horizontal, ("тип подразд."));
    ui->tab_medparts->repaint();

    /*data_medpart.id = -1;
    data_medpart.code_mo = "";
    data_medpart.name_mo = "";*/

    refresh_medpart_type();

    ui->ln_medpart_code->setText("");
    ui->ln_medpart_name->setText("");
    //ui->combo_medpart_type->setCurrentIndex(-1);

    //ui->bn_ok->setEnabled(true);

    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::refresh_medpart_type() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка подразделений
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code, '(' || code || ') ' || text "
                  "  from spr_medparts_types t "
                  " order by code ; ";
    //QInputDialog::getText(this, "", "", QLineEdit::Normal, sql);

    mySQL.exec(this, sql, QString("Справочник типов участков для таблицы мед.участков"), *query, true, db, data_app);
    ui->combo_medpart_type->clear();
    while (query->next()) {
        ui->combo_medpart_type->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    //ui->combo_medpart_type->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}


void assignment_wnd::refresh_medsites_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }

    QString medsites_sql;
    medsites_sql  = "select ms.id, ms.code_mo, ms.code_mp, ms.code_ms, /*ms.name_ms*/q.fio_mt, ms.type_ms, '('||ms.type_ms||') '||t.text as type_ms_text, mo.name_mo, mp.name_mp, ms.name_ms ";
    medsites_sql += "  from spr_medsites ms "
                    "  left join spr_medsites_types t on (ms.type_ms=t.code) "
                    "  left join spr_medorgs  mo on(mo.code_mo=ms.code_mo and mo.is_head=1) "
                    "  left join spr_medparts mp on(mp.code_mo=ms.code_mo and mp.code_mp=ms.code_mp) "
                    "  left join ( select r.id_ms, mt.fio_mt, r.date_beg, r.date_end "
                    "                from spr_ms_r_mt r "
                    "                left join spr_medters mt on(mt.id=r.id_mt) "
                    "               where mt.fio_mt is not NULL ) q "
                    "            on(q.id_ms=ms.id and q.date_beg<=CURRENT_DATE and ( q.date_end is NULL or date_end>CURRENT_DATE )) ";

    if (ui->ch_medsites_mo->isChecked()) {
        // проверим выделенную строку
        QModelIndexList indexes = ui->tab_medorgs->selectionModel()->selection().indexes();

        if (indexes.size()>0) {
            QModelIndex index = indexes.front();
            QString code_mo = model_medorgs.data(model_medorgs.index(index.row(), 2), Qt::EditRole).toString();
            medsites_sql += " where mo.code_mo='" + code_mo + "' ";
        }
    }
    if (ui->rb_sort_ms4codes->isChecked())
        medsites_sql += " order by ms.code_mo, ms.code_mp, ms.code_ms ";
    else if (ui->rb_sort_ms4code->isChecked())
        medsites_sql += " order by ms.code_mo, ms.code_ms ";
    else
        medsites_sql += " order by ms.name_ms ";

    model_medsites.setQuery(medsites_sql,db);
    QString err2 = model_medsites.lastError().driverText();

    // подключаем модель из БД
    ui->tab_medsites->setModel(&model_medsites);

    // обновляем таблицу
    ui->tab_medsites->reset();

    // задаём ширину колонок
    ui->tab_medsites->setColumnWidth( 0,  2);   // id,
    ui->tab_medsites->setColumnWidth( 1, 50);   // code_mo,
    ui->tab_medsites->setColumnWidth( 2, 80);   // code_mp,
    ui->tab_medsites->setColumnWidth( 3, 80);   // code_ms,
    ui->tab_medsites->setColumnWidth( 4,170);   // name_ms,
    ui->tab_medsites->setColumnWidth( 5,  2);   // type_ms,
    ui->tab_medsites->setColumnWidth( 6,160);   // type_ms_text
    ui->tab_medsites->setColumnWidth( 7,  2);   // name_mo,
    ui->tab_medsites->setColumnWidth( 8,  2);   // name_mp,
    ui->tab_medsites->setColumnWidth( 9,  2);   // name_mp,
    // правим заголовки
    model_medsites.setHeaderData( 1, Qt::Horizontal, ("МО"));
    model_medsites.setHeaderData( 2, Qt::Horizontal, ("подразд."));
    model_medsites.setHeaderData( 3, Qt::Horizontal, ("участок"));
    model_medsites.setHeaderData( 4, Qt::Horizontal, ("участковый врач"));
    model_medsites.setHeaderData( 5, Qt::Horizontal, ("тип"));
    model_medsites.setHeaderData( 6, Qt::Horizontal, ("тип участка"));
    model_medsites.setHeaderData( 9, Qt::Horizontal, ("название"));
    ui->tab_medsites->repaint();

    /*data_medsite.id = -1;
    data_medsite.code_mo = "";
    data_medsite.name_mo = "";*/

    refresh_medsite_type();

    ui->ln_medsite_code->setText("");
    ui->ln_medsite_name->setText("");
    ui->ln_medsite_mt->setText("");
    //ui->combo_medsite_type->setCurrentIndex(-1);

    //ui->bn_ok->setEnabled(true);

    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::refresh_medsite_type() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка подразделений
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code, '(' || code || ') ' || text "
                  "  from spr_medsites_types t "
                  " order by code ; ";
    //QInputDialog::getText(this, "", "", QLineEdit::Normal, sql);

    mySQL.exec(this, sql, QString("Справочник типов участков для таблицы мед.участков"), *query, true, db, data_app);
    ui->combo_medsite_type->clear();
    while (query->next()) {
        ui->combo_medsite_type->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    //ui->combo_medsite_type->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::on_bn_refresh_medsites_clicked() {
    refresh_medsites_tab();
}

void assignment_wnd::on_tab_medsites_clicked(const QModelIndex &index) {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medsites || !ui->tab_medsites->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица не загружена!\nНажмите кнопку [Поиск] и выберите человека из списка.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medsites->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        //data_medorg.code_ms = model_medorgs.data(model_medorgs.index(index.row(), 3), Qt::EditRole).toString();
        ui->combo_medsite_mo->setCurrentIndex(ui->combo_medsite_mo->findData(model_medsites.data(model_medsites.index(index.row(), 1), Qt::EditRole).toString()));
        //refresh_medsite_mp();
        ui->combo_medsite_mp->setCurrentIndex(ui->combo_medsite_mp->findData(model_medsites.data(model_medsites.index(index.row(), 2), Qt::EditRole).toString()));
        ui->ln_medsite_code->setText(model_medsites.data(model_medsites.index(index.row(), 3), Qt::EditRole).toString());
        ui->ln_medsite_name->setText(model_medsites.data(model_medsites.index(index.row(), 9), Qt::EditRole).toString());
        ui->ln_medsite_mt->setText(model_medsites.data(model_medsites.index(index.row(), 4), Qt::EditRole).toString());
        ui->combo_medsite_type->setCurrentIndex(ui->combo_medsite_type->findData(model_medsites.data(model_medsites.index(index.row(), 5), Qt::EditRole).toString()));
    } else {
        ui->ln_medsite_code->setText("");
        ui->ln_medsite_name->setText("");
        ui->ln_medsite_mt->setText("");
        //ui->combo_medsite_type->setCurrentIndex(-1);
    }

    //ui->combo_medter_mp->setCurrentIndex(ui->combo_medter_mp->findData(model_medsites.data(model_medsites.index(index.row(), 2), Qt::EditRole).toString()));
    //ui->combo_medter_ms->setCurrentIndex(ui->combo_medter_ms->findData(model_medsites.data(model_medsites.index(index.row(), 3), Qt::EditRole).toString()));

    /*if (ui->ch_medters_ms->isChecked()) {
        refresh_medters_tab();
    }*/
    refresh_medsite_ters_tab();
}

void assignment_wnd::on_bn_add_medsite_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medorgs || !ui->tab_medorgs->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица медорганизаций не загружена!\nНажмите кнопку [Обновить] и выберите МО из списка.");
        return;
    }

    if (ui->combo_medsite_type->currentIndex()<0) {
        QMessageBox::warning(this, "Недостаточно данных", "Выберите тип нового участка.\n\nСохранение отменено.");
        return;
    }
    if (ui->ln_medsite_code->text().trimmed().simplified().isEmpty()) {
        QMessageBox::warning(this, "Недостаточно данных", "Введите код участка.\n\nСохранение отменено.");
        return;
    }
    if (ui->ln_medsite_name->text().trimmed().simplified().isEmpty()) {
        QMessageBox::warning(this, "Недостаточно данных", "Введите название участка.\n\nСохранение отменено.");
        return;
    }

    // проверим код на уникальность
    QString sql = "select count(*) from spr_medsites "
                  " where code_mo='" + ui->combo_medsite_mo->currentData().toString() + "' and "
                  "       code_mp='" + ui->combo_medsite_mp->currentData().toString() + "' and "
                  "       code_ms='" + ui->ln_medsite_code->text().trimmed() + "' ; ";

    QSqlQuery *query = new QSqlQuery(db);
    if (!mySQL.exec(this, sql, "Проверка кода участка на уникальность", *query, true, db, data_app) || !query->next()) {
        QMessageBox::warning(this, "Непредвиденная ошибка", "При попытке проверить код участка на уникальность произошла неожиданная ошибка.\n\nСохранение отменено.");
        delete query;
        return;
    }
    if (query->value(0).toInt()>0) {
        QMessageBox::warning(this,
                              "Такой код участка МО уже есть",
                              "В базе данных уже есть участок данного подразделения данной МО с таким же реестровым номером.\nКод участка в подразделении должен быть уникальным.\n\nСохранение отменено.");
        delete query;
        return;
    }


    if (QMessageBox::question(this,
                              "Нужно подтверждение",
                              "Добавить новый участок?\n",
                              QMessageBox::Yes|QMessageBox::Cancel,
                              QMessageBox::Yes)==QMessageBox::Yes) {
        db.transaction();
        // добавим МО
        QString sql = "insert into spr_medsites (code_mo, code_mp, code_ms, name_ms, type_ms) "
                      " values ("
                      "   '" + ui->combo_medsite_mo->currentData().toString() + "', "
                      "   '" + ui->combo_medsite_mp->currentData().toString() + "', "
                      "   '" + ui->ln_medsite_code->text().trimmed().simplified() + "', "
                      "   '" + ui->ln_medsite_name->text().trimmed().simplified() + "', "
                      "   '" + ui->combo_medsite_type->currentData().toString() + "') "
                      " returning id ; ";

        QSqlQuery *query = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Добавление нового участка", *query, true, db, data_app) || !query->next()) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новый участок произошла неожиданная ошибка.\n\nСохранение отменено.");
            delete query;
            return;
        }
        //data_medorg.code_ms = "";
        delete query;
        db.commit();

        refresh_medsites_tab();
    }
}

void assignment_wnd::on_bn_edit_medsite_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medorgs || !ui->tab_medorgs->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица медорганизаций не загружена!\nНажмите кнопку [Обновить] и выберите МО из списка.");
        return;
    }


    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medsites->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        if (QMessageBox::question(this,
                                  "Нужно подтверждение",
                                  "Изменить данные выбранного участка?",
                                  QMessageBox::Yes|QMessageBox::Cancel,
                                  QMessageBox::Yes)==QMessageBox::Cancel) {
            return;
        }

        QModelIndex index = indexes.front();
        QString ID = QString::number(model_medsites.data(model_medsites.index(index.row(), 0), Qt::EditRole).toInt());


        // проверка
        if (ui->combo_medsite_mo->currentIndex()<0) {
            QMessageBox::warning(this, "Недостаточно данных", "Выберите медорганизацию, к которой относится новый участок.\n\nСохранение отменено.");
            return;
        }
        /*if (ui->combo_medsite_mp->currentIndex()<0) {
            QMessageBox::warning(this, "Недостаточно данных", "Выберите подразделение, к которому относится новый участок.\n\nСохранение отменено.");
            return;
        }*/
        if (ui->combo_medsite_type->currentIndex()<0) {
            QMessageBox::warning(this, "Недостаточно данных", "Выберите тип нового участка.\n\nСохранение отменено.");
            return;
        }
        if (ui->ln_medsite_code->text().trimmed().simplified().isEmpty()) {
            QMessageBox::warning(this, "Недостаточно данных", "Введите код участка.\n\nСохранение отменено.");
            return;
        }
        if (ui->ln_medsite_name->text().trimmed().simplified().isEmpty()) {
            QMessageBox::warning(this, "Недостаточно данных", "Введите название участка.\n\nСохранение отменено.");
            return;
        }

        // проверим код на уникальность
        QString sql = "select count(*) from spr_medsites "
                      " where code_mo='" + ui->combo_medsite_mo->currentData().toString() + "' and "
                      "       code_mp='" + ui->ln_medpart_code->text().trimmed() + "' and "
                      "       code_ms='" + ui->ln_medsite_code->text().trimmed() + "' and "
                      "       id<>" + ID + " ; ";

        QSqlQuery *query = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Проверка кода участка на уникальность", *query, true, db, data_app) || !query->next()) {
            QMessageBox::warning(this, "Непредвиденная ошибка", "При попытке проверить код участка на уникальность произошла неожиданная ошибка.\n\nСохранение отменено.");
            delete query;
            return;
        }
        if (query->value(0).toInt()>0) {
            QMessageBox::warning(this,
                                  "Такой код участка МО уже есть",
                                  "В базе данных уже есть участок того же подразделения той же МО, код которого совпадает с введённым.\nКод участка должен быть уникальным в пределаж МО / подразделения МО.\n\nСохранение отменено.");
            delete query;
            return;
        }


        db.transaction();
        // добавим фирму
        QString sql_upd = "update spr_medsites "
                      " set "
                      "   code_mo='" + ui->combo_medsite_mo->currentData().toString() + "', "
                      "   code_mp='" + ui->combo_medsite_mp->currentData().toString() + "', "
                      "   code_ms='" + ui->ln_medsite_code->text().trimmed().simplified() + "', "
                      "   name_ms='" + ui->ln_medsite_name->text().trimmed().simplified() + "', "
                      "   type_ms='" + ui->combo_medsite_type->currentData().toString() + "' "
                      " where id=" + ID + " ; ";

        QSqlQuery *query_upd = new QSqlQuery(db);
        if (!mySQL.exec(this, sql_upd, "Обновление медорганизации", *query_upd, true, db, data_app)) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке изменить данные медицинской организации произошла неожиданная ошибка.\n\nИзменение отменено.");
            delete query_upd;
            return;
        }
        //data_medorg.code_ms = ui->ln_medsite_code->text();
        /*data_medsite.name_mo = ui->ln_name_mo->text();*/
        delete query;
        db.commit();

        refresh_medsites_tab();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано", "Ничего не выбрано");
    }
}

void assignment_wnd::on_ch_medsites_mo_clicked() {
    refresh_medsites_tab();
}

void assignment_wnd::on_bn_delete_medsite_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medsites || !ui->tab_medsites->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица не загружена!\nНажмите кнопку [Поиск] и выберите человека из списка.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medsites->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        if (QMessageBox::question(this,
                                  "Нужно подтверждение",
                                  "Удалить выбранный участок из базы?\n",
                                  QMessageBox::Yes|QMessageBox::Cancel,
                                  QMessageBox::Yes)==QMessageBox::Cancel) {
            return;
        }
        db.transaction();

        QModelIndex index = indexes.front();
        QString ID = QString::number(model_medsites.data(model_medsites.index(index.row(), 0), Qt::EditRole).toInt());


        QString sql = "delete from spr_medsites "
                      " where id=" + ID + " ; ";
        QSqlQuery *query2 = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Удаление участка", *query2, true, db, data_app)) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке удалить участок произошла неожиданная ошибка.\n\nУдаление отменено.");
            delete query2;
            return;
        }
        delete query2;

        db.commit();

        refresh_medsites_tab();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано", "Ничего не выбрано");
    }
}

void assignment_wnd::on_bn_refresh_medters_clicked() {
    refresh_medters_tab();
}


void assignment_wnd::refresh_medters_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }

    QString medters_sql;
    medters_sql  = "select mt.id, mt.code_mo, mt.code_mt, mt.fio_mt, mt.snils_mt, mt.spec_mt, '('||mt.spec_mt||') '||spec.text as spec_mt_text ";
    medters_sql += "  from spr_medters mt "
                   "      left join spr_medters_spec spec on(mt.spec_mt=spec.code) ";

    if (ui->ch_medters_mo->isChecked()) {
        // проверим выделенную строку
        QModelIndexList indexes = ui->tab_medorgs->selectionModel()->selection().indexes();

        if (indexes.size()>0) {
            QModelIndex index = indexes.front();
            QString code_mo = model_medorgs.data(model_medorgs.index(index.row(), 2), Qt::EditRole).toString();
            medters_sql += " where code_mo='" + code_mo + "' ";
        }
    }
    if (ui->rb_sort_mt4fio->isChecked())
        medters_sql += " order by mt.fio_mt ";
    else
        medters_sql += " order by mt.snils_mt ";

    model_medters.setQuery(medters_sql,db);
    QString err2 = model_medsites.lastError().driverText();

    // подключаем модель из БД
    ui->tab_medters->setModel(&model_medters);

    // обновляем таблицу
    ui->tab_medters->reset();

    // задаём ширину колонок
    ui->tab_medters->setColumnWidth( 0,  2);   // id,
    ui->tab_medters->setColumnWidth( 1, 50);   // code_mo,
    //ui->tab_medters->setColumnWidth( 2, 50);   // code_mp,
    //ui->tab_medters->setColumnWidth( 3, 50);   // code_ms,
    ui->tab_medters->setColumnWidth( 2,  2);   // code_mt,
    ui->tab_medters->setColumnWidth( 3,170);   // fio_mt,
    ui->tab_medters->setColumnWidth( 4, 80);   // snils_mt,
    ui->tab_medters->setColumnWidth( 5,  2);   // spec_mt,
    ui->tab_medters->setColumnWidth( 6,240);   // spec_mt_text
    //ui->tab_medters->setColumnWidth( 9,  2);   // prof_mt_text
    //ui->tab_medters->setColumnWidth(10, 50);   // prof_mt_text
    // правим заголовки
    model_medters.setHeaderData( 1, Qt::Horizontal, ("МО"));
    //model_medters.setHeaderData( 2, Qt::Horizontal, ("подр."));
    //model_medters.setHeaderData( 3, Qt::Horizontal, ("уч."));
    model_medters.setHeaderData( 2, Qt::Horizontal, ("код"));
    model_medters.setHeaderData( 3, Qt::Horizontal, ("ФИО"));
    model_medters.setHeaderData( 4, Qt::Horizontal, ("СНИЛС"));
    model_medters.setHeaderData( 5, Qt::Horizontal, ("сп."));
    model_medters.setHeaderData( 6, Qt::Horizontal, ("специализация"));
    //model_medters.setHeaderData(10, Qt::Horizontal, ("проф."));
    ui->tab_medters->repaint();

    /*data_medsite.id = -1;
    data_medsite.code_mo = "";
    data_medsite.name_mo = "";*/

    refresh_medter_spec();

    //ui->combo_medter_mo->setCurrentIndex(-1);
    //ui->combo_medter_mp->setCurrentIndex(-1);
    //ui->combo_medter_ms->setCurrentIndex(-1);
    ui->ln_medter_code->setText("");
    ui->ln_medter_fio->setText("");
    ui->ln_medter_snils->setText("");
    ui->combo_medter_spec->setCurrentIndex(-1);
    //ui->combo_medter_prof->setCurrentIndex(-1);

    //ui->bn_ok->setEnabled(true);

    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::refresh_medter_mo() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка медорганизаций
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mo, '(' || code_mo || ') ' || name_mo "
                  "  from spr_medorgs s "
                  " where is_head=1 "
                  " order by code_mo ; ";
    mySQL.exec(this, sql, QString("Справочник медорганизаций для таблицы врачей"), *query, true, db, data_app);
    ui->combo_medter_mo->clear();
    while (query->next()) {
        ui->combo_medter_mo->addItem(query->value(1).toString(), query->value(0).toString());
    }
    //ui->combo_medter_mo->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

/*void assignment_wnd::refresh_medter_mp() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка подразделений
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mp, '(' || code_mp || ') ' || name_mp "
                  "  from spr_medparts u "
                  " where u.code_mo='" + ui->combo_medter_mo->currentData().toString() + "' "
                  " order by code_mp ; ";
    mySQL.exec(this, sql, QString("Справочник подразделений для таблицы врачей"), *query, true);
    //ui->combo_medter_mp->clear();
    while (query->next()) {
        ui->combo_medter_mp->addItem(query->value(1).toString(), query->value(0).toString());
    }
    //ui->combo_medter_mp->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}*/

/*void assignment_wnd::refresh_medter_ms() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка подразделений
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_ms, '(' || code_ms || ') ' || name_ms "
                  "  from spr_medsites u "
                  " where u.code_mo='" + ui->combo_medter_mo->currentData().toString() + "' "
                  " order by code_ms ; ";
    mySQL.exec(this, sql, QString("Справочник участков для таблицы врачей"), *query, true);
    ui->combo_medter_ms->clear();
    while (query->next()) {
        ui->combo_medter_ms->addItem(query->value(1).toString(), query->value(0).toString());
    }
    //ui->combo_medter_ms->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}*/

void assignment_wnd::refresh_medter_spec() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка подразделений
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code, '(' || code || ') ' || text "
                  "  from spr_medters_spec t "
                  " order by code ; ";
    mySQL.exec(this, sql, QString("Справочник спечиальностей для таблицы врачей"), *query, true, db, data_app);
    ui->combo_medter_spec->clear();
    while (query->next()) {
        ui->combo_medter_spec->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    //ui->combo_medter_spec->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}


void assignment_wnd::on_tab_medters_clicked(const QModelIndex &index) {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medsites || !ui->tab_medsites->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица не загружена!\nНажмите кнопку [Поиск] и выберите человека из списка.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medters->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        //data_medorg.code_mt = model_medorgs.data(model_medorgs.index(index.row(), 4), Qt::EditRole).toString();

        ui->combo_medter_mo->setCurrentIndex(ui->combo_medter_mo->findData(model_medters.data(model_medters.index(index.row(), 1), Qt::EditRole).toString()));
        //refresh_medter_mp();
        //refresh_medter_ms();
        //ui->combo_medter_mp->setCurrentIndex(ui->combo_medter_mp->findData(model_medters.data(model_medters.index(index.row(), 2), Qt::EditRole).toString()));
        //ui->combo_medter_ms->setCurrentIndex(ui->combo_medter_ms->findData(model_medters.data(model_medters.index(index.row(), 3), Qt::EditRole).toString()));
        ui->ln_medter_code->setText(model_medters.data(model_medters.index(index.row(), 2), Qt::EditRole).toString());
        ui->ln_medter_fio->setText(model_medters.data(model_medters.index(index.row(), 3), Qt::EditRole).toString());
        ui->ln_medter_snils->setText(model_medters.data(model_medters.index(index.row(), 4), Qt::EditRole).toString());
        ui->combo_medter_spec->setCurrentIndex(ui->combo_medter_spec->findData(model_medters.data(model_medters.index(index.row(), 5), Qt::EditRole).toString()));
        //ui->combo_medter_prof->setCurrentIndex(model_medters.data(model_medters.index(index.row(), 9), Qt::EditRole).toInt());

    } else {
        //ui->combo_medter_mo->setCurrentIndex(-1);
        //ui->combo_medter_mp->setCurrentIndex(-1);
        //ui->combo_medter_ms->setCurrentIndex(-1);
        ui->ln_medter_code->setText("");
        ui->ln_medter_fio->setText("");
        ui->ln_medter_snils->setText("");
        //ui->combo_medter_spec->setCurrentIndex(-1);
        //ui->combo_medter_prof->setCurrentIndex(0);
    }
}

void assignment_wnd::on_bn_add_medter_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medorgs || !ui->tab_medorgs->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица медорганизаций не загружена!\nНажмите кнопку [Обновить] и выберите МО из списка.");
        return;
    }

    // проверка
    if (ui->combo_medter_mo->currentIndex()<0) {
        QMessageBox::warning(this, "Недостаточно данных", "Выберите медорганизацию, к которой относится новый врач.\n\nСохранение отменено.");
        return;
    }
    /*if (ui->combo_medter_mp->currentIndex()<0) {
        QMessageBox::warning(this, "Недостаточно данных", "Выберите подразделение, к которому относится новый врач.\n\nСохранение отменено.");
        return;
    }*/
    /*if (ui->combo_medter_ms->currentIndex()<0) {
        QMessageBox::warning(this, "Недостаточно данных", "Выберите участок, к которому прикреплён новый врач.\n\nСохранение отменено.");
        return;
    }*/
    if (ui->combo_medter_spec->currentIndex()<0) {
        QMessageBox::warning(this, "Недостаточно данных", "Выберите специализацию нового врача.\n\nСохранение отменено.");
        return;
    }
    /*if (ui->ln_medter_code->text().trimmed().simplified().isEmpty()) {
        QMessageBox::warning(this, "Недостаточно данных", "Введите код врача.\n\nСохранение отменено.");
        return;
    }*/
    if (ui->ln_medter_fio->text().trimmed().simplified().isEmpty()) {
        QMessageBox::warning(this, "Недостаточно данных", "Введите ФИО врача.\n\nСохранение отменено.");
        return;
    }
    /*if (ui->combo_medter_prof->currentIndex()==0) {
        QMessageBox::warning(this, "Недостаточно данных", "Выберите специализацию (врач/медсестра).\n\nСохранение отменено.");
        return;
    }*/
    if (ui->ln_medter_snils->text().trimmed().simplified().isEmpty()) {
        QMessageBox::warning(this, "Недостаточно данных", "Введите СНИЛС врача.\n\nСохранение отменено.");
        return;
    }

    if (ui->ln_medter_snils->text().trimmed().isEmpty()) {
        // СНИЛС не введён - проверки не надо
    } else {
        // проверим СНИЛС на уникальность
        QString sql = "select count(*) from spr_medters "
                      " where snils_mt='" + ui->ln_medter_snils->text().trimmed() + "' ; ";

        QSqlQuery *query = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Проверка СНИЛС врача на уникальность", *query, true, db, data_app) || !query->next()) {
            QMessageBox::warning(this, "Непредвиденная ошибка", "При попытке проверить СНИЛС врача на уникальность произошла неожиданная ошибка.\n\nСохранение отменено.");
            delete query;
            return;
        }
        if (query->value(0).toInt()>0) {
            QMessageBox::warning(this,
                                  "Такой СНИЛС врача уже есть",
                                  "В базе данных уже есть врач с таким же СНИЛС.\nСНИЛС врача должен быть уникальным.\n\nСохранение отменено.");
            delete query;
            return;
        }
        delete query;
    }


    if (QMessageBox::question(this,
                              "Нужно подтверждение",
                              "Добавить нового врача?\n",
                              QMessageBox::Yes|QMessageBox::Cancel,
                              QMessageBox::Yes)==QMessageBox::Yes) {
        db.transaction();
        // добавим МО
        QString sql = "insert into spr_medters (code_mo, /*code_mp, code_ms,*/ code_mt, fio_mt, snils_mt, spec_mt/*, prof_mt*/) "
                      " values ("
                      "   '" + ui->combo_medter_mo->currentData().toString() + "', "
                      //"   '" + ui->combo_medter_mp->currentData().toString() + "', "
                      //"   '" + ui->combo_medter_ms->currentData().toString() + "', "
                      "   '" + ui->ln_medter_code->text().trimmed().simplified() + "', "
                      "   '" + ui->ln_medter_fio->text().trimmed().simplified() + "', "
                      "   '" + ui->ln_medter_snils->text().trimmed().simplified().replace("-","").replace(" ","") + "', "
                      "   '" + ui->combo_medter_spec->currentData().toString() + "') "
                      " returning id ; ";

        QSqlQuery *query = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Добавление нового врача", *query, true, db, data_app) || !query->next()) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить нового врача произошла неожиданная ошибка.\n\nСохранение отменено.");
            delete query;
            return;
        }
        //data_medorg.code_mt = ui->ln_code_mo->text();
        delete query;
        db.commit();

        refresh_medters_tab();
    }
}

void assignment_wnd::on_combo_medter_mo_currentIndexChanged(int index) {
    /*refresh_medter_mp();
    refresh_medter_ms();*/
}

void assignment_wnd::on_bn_edit_medter_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medorgs || !ui->tab_medorgs->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица медорганизаций не загружена!\nНажмите кнопку [Обновить] и выберите МО из списка.");
        return;
    }


    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medters->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        if (QMessageBox::question(this,
                                  "Нужно подтверждение",
                                  "Изменить данные выбранного врача?",
                                  QMessageBox::Yes|QMessageBox::Cancel,
                                  QMessageBox::Yes)==QMessageBox::Cancel) {
            return;
        }

        QModelIndex index = indexes.front();
        QString ID = QString::number(model_medters.data(model_medters.index(index.row(), 0), Qt::EditRole).toInt());


        // проверка
        if (ui->combo_medter_mo->currentIndex()<0) {
            QMessageBox::warning(this, "Недостаточно данных", "Выберите медорганизацию, к которой относится врач.\n\nСохранение отменено.");
            return;
        }
        /*if (ui->combo_medter_mp->currentIndex()<0) {
            QMessageBox::warning(this, "Недостаточно данных", "Выберите подразделение, к которому относится врач.\n\nСохранение отменено.");
            return;
        }*/
        /*if (ui->combo_medter_ms->currentIndex()<0) {
            QMessageBox::warning(this, "Недостаточно данных", "Выберите участок, за которым закреплён врач.\n\nСохранение отменено.");
            return;
        }*/
        if (ui->combo_medter_spec->currentIndex()<0) {
            QMessageBox::warning(this, "Недостаточно данных", "Выберите специализацию врача.\n\nСохранение отменено.");
            return;
        }
        /*if (ui->ln_medter_code->text().trimmed().simplified().isEmpty()) {
            QMessageBox::warning(this, "Недостаточно данных", "Введите код врача.\n\nСохранение отменено.");
            return;
        }*/
        if (ui->ln_medter_snils->text().trimmed().simplified().isEmpty()) {
            QMessageBox::warning(this, "Недостаточно данных", "Введите СНИЛС врача.\n\nСохранение отменено.");
            return;
        }
        /*if (ui->combo_medter_prof->currentIndex()==0) {
            QMessageBox::warning(this, "Недостаточно данных", "Выберите специализацию (врач/медсестра).\n\nСохранение отменено.");
            return;
        }*/
        if (ui->ln_medter_fio->text().trimmed().simplified().isEmpty()) {
            QMessageBox::warning(this, "Недостаточно данных", "Введите ФИО врача.\n\nСохранение отменено.");
            return;
        }

        if (ui->ln_medter_snils->text().trimmed().isEmpty()) {
            // СНИЛС не введён - проверки не надо
        } else {
            // проверим код на уникальность
            QString sql = "select count(*) from spr_medters "
                          " where code_mo='" + ui->combo_medter_mo->currentData().toString() + "' and "
                          "       snils_mt='" + ui->ln_medter_snils->text().trimmed() + "' and "
                          "       id<>" + ID + " ; ";

            QSqlQuery *query = new QSqlQuery(db);
            if (!mySQL.exec(this, sql, "Проверка СНИЛС врача на уникальность", *query, true, db, data_app) || !query->next()) {
                QMessageBox::warning(this, "Непредвиденная ошибка", "При попытке проверить СНИЛС врача на уникальность произошла неожиданная ошибка.\n\nСохранение отменено.");
                delete query;
                return;
            }
            if (query->value(0).toInt()>0) {
                QMessageBox::warning(this,
                                      "Такой СНИЛС врача уже есть",
                                      "В базе данных уже есть врач с таким же СНИЛС.\nСНИЛС врача должен быть уникальным.\n\nСохранение отменено.");
                delete query;
                return;
            }
            delete query;
        }

        db.transaction();
        // добавим фирму
        QString sql_upd = "update spr_medters "
                      " set "
                      "   code_mo='" + ui->combo_medter_mo->currentData().toString() + "', "
                      //"   code_mp='" + ui->combo_medter_mp->currentData().toString() + "', "
                      //"   code_ms='" + ui->combo_medter_ms->currentData().toString() + "', "
                      "   code_mt='" + ui->ln_medter_code->text().trimmed().simplified() + "', "
                      "   fio_mt='" + ui->ln_medter_fio->text().trimmed().simplified() + "', "
                      "   snils_mt='" + ui->ln_medter_snils->text().trimmed().simplified() + "', "
                      "   spec_mt='" + ui->combo_medter_spec->currentData().toString() + "' "
                      " where id=" + ID + " ; ";

        QSqlQuery *query_upd = new QSqlQuery(db);
        if (!mySQL.exec(this, sql_upd, "Обновление врача", *query_upd, true, db, data_app)) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке изменить данные врача произошла неожиданная ошибка.\n\nИзменение отменено.");
            delete query_upd;
            return;
        }
        //data_medorg.code_mt = ui->ln_medter_code->text();
        /*data_medter.code_mo = ui->ln_code_mo->text();
        data_medter.name_mo = ui->ln_name_mo->text();*/
        db.commit();

        refresh_medters_tab();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано", "Ничего не выбрано");
    }
    //ui->bn_ok->setEnabled(true);
}

void assignment_wnd::on_bn_delete_medter_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medters || !ui->tab_medters->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица не загружена!\nНажмите кнопку [Поиск] и выберите человека из списка.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medters->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        if (QMessageBox::question(this,
                                  "Нужно подтверждение",
                                  "Удалить выбранного врача из базы?\n",
                                  QMessageBox::Yes|QMessageBox::Cancel,
                                  QMessageBox::Yes)==QMessageBox::Cancel) {
            return;
        }
        db.transaction();

        QModelIndex index = indexes.front();
        QString ID = QString::number(model_medters.data(model_medters.index(index.row(), 0), Qt::EditRole).toInt());
        QString snils_mt = model_medters.data(model_medters.index(index.row(), 4), Qt::EditRole).toString();


        QString sql = "";

        // открепление персоны от этого врача
        sql += "update persons_assig "
               "   set unsig_date=current_date "
               " where snils_mt='" + snils_mt + "' "
               "   and is_actual=0 ; ";
        sql += "update persons_assig "
               "   set is_actual=is_actual +1 "
               " where snils_mt='" + snils_mt + "' ; ";
        /*sql += "update persons_assig "
               "   set snils_mt=NULL "
               " where snils_mt='" + snils_mt + "' ; ";*/

        sql += "update assig_by_terr "
               "   set snils_mt=NULL "
               " where snils_mt='" + snils_mt + "' ; ";

        sql += "delete from spr_ms_r_mt "
               " where id_mt=" + ID + " ; ";

        sql += "delete from spr_medters "
               " where id=" + ID + " ; ";

        QSqlQuery *query2 = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Удаление врача", *query2, true, db, data_app)) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке удалить запись врача произошла неожиданная ошибка.\n\nУдаление отменено.");
            delete query2;
            return;
        }
        delete query2;

        db.commit();

        refresh_medters_tab();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано", "Ничего не выбрано");
    }
}

void assignment_wnd::on_bn_filter_clicked() {
    on_bn_refresh_clicked();
}

void assignment_wnd::on_bn_clear_clicked() {
    ui->ln_filter->clear();
    on_bn_refresh_clicked();
}

void assignment_wnd::on_ln_filter_editingFinished() {
    on_bn_refresh_clicked();
}

void assignment_wnd::on_bn_add_medpart_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medorgs || !ui->tab_medorgs->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица медорганизаций не загружена!\nНажмите кнопку [Обновить] и выберите МО из списка.");
        return;
    }

    if (ui->combo_medpart_type->currentIndex()<0) {
        QMessageBox::warning(this, "Недостаточно данных", "Выберите тип подразделения.\n\nСохранение отменено.");
        return;
    }
    if (ui->ln_medpart_name->text().trimmed().simplified().isEmpty()) {
        QMessageBox::warning(this, "Недостаточно данных", "Введите код подразделения МО.\n\nСохранение отменено.");
        return;
    }
    if (ui->ln_medpart_name->text().trimmed().simplified().isEmpty()) {
        QMessageBox::warning(this, "Недостаточно данных", "Введите название подразделения МО.\n\nСохранение отменено.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medorgs->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        int id_medorg = model_medorgs.data(model_medorgs.index(index.row(), 0), Qt::EditRole).toInt();
        QString code_mo = model_medorgs.data(model_medorgs.index(index.row(), 2), Qt::EditRole).toString();

        if (QMessageBox::question(this,
                                  "Нужно подтверждение",
                                  "Добавить новое подразделений МО?\n",
                                  QMessageBox::Yes|QMessageBox::Cancel,
                                  QMessageBox::Yes)==QMessageBox::Yes) {
            // проверим код на уникальность
            QString sql_test = "select count(*) from spr_medsites "
                          " where code_mo='" + code_mo + "' and "
                          "       code_mp='" + ui->ln_medpart_code->text().trimmed() + "' ; ";

            QSqlQuery *query_test = new QSqlQuery(db);
            if (!mySQL.exec(this, sql_test, "Проверка кода подразделения на уникальность", *query_test, true, db, data_app) || !query_test->next()) {
                QMessageBox::warning(this, "Непредвиденная ошибка", "При попытке проверить код подразделения на уникальность произошла неожиданная ошибка.\n\nСохранение отменено.");
                delete query_test;
                return;
            }
            if (query_test->value(0).toInt()>0) {
                QMessageBox::warning(this,
                                      "Такой код подразделения МО уже есть",
                                      "В базе данных уже есть подразделение данной МО с таким же кодом.\nКод подразделения должен быть уникальным.\n\nСохранение отменено.");
                delete query_test;
                return;
            }

            db.transaction();
            // добавим МО
            QString sql_ins = "insert into spr_medparts (code_mo, code_mp, name_mp, type_mp) "
                          " values ("
                          "   '" + code_mo + "', "
                          "   '" + ui->ln_medpart_code->text().trimmed().simplified() + "', "
                          "   '" + ui->ln_medpart_name->text().trimmed().simplified() + "', "
                          "    " + QString::number(ui->combo_medpart_type->currentData().toInt()) + " ) "
                          " returning id ; ";

            QSqlQuery *query_ins = new QSqlQuery(db);
            if (!mySQL.exec(this, sql_ins, "Добавление нового подразделения", *query_ins, true, db, data_app) || !query_ins->next()) {
                db.rollback();
                QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новое подразделение МО произошла неожиданная ошибка.\n\nСохранение отменено.");
                delete query_ins;
                return;
            }
            //data_medorg.code_ms = "";
            delete query_ins;
            db.commit();
        }
    }
    refresh_medparts_tab();
    refresh_medsites_tab();
    refresh_medters_tab();
}

void assignment_wnd::on_tab_medparts_clicked(const QModelIndex &index) {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medparts || !ui->tab_medparts->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица подразделений медорганизаций не загружена!\nНажмите кнопку [Обновить] и выберите подразделение МО из списка.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medparts->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        ui->ln_medpart_code->setText(model_medparts.data(model_medparts.index(index.row(), 2), Qt::EditRole).toString());
        ui->ln_medpart_name->setText(model_medparts.data(model_medparts.index(index.row(), 3), Qt::EditRole).toString());
        ui->combo_medpart_type->setCurrentIndex(ui->combo_medpart_type->findData(model_medparts.data(model_medparts.index(index.row(), 4), Qt::EditRole).toString()));

    } else {
        ui->ln_medpart_code->setText("");
        ui->ln_medpart_name->setText("");
        //ui->combo_medpart_type->setCurrentIndex(-1);
    }
    ui->combo_medsite_mp->setCurrentIndex(ui->combo_medsite_mp->findData(model_medparts.data(model_medparts.index(index.row(), 2), Qt::EditRole).toString()));
    //ui->combo_medter_mp->setCurrentIndex(ui->combo_medsite_mp->findData(model_medparts.data(model_medparts.index(index.row(), 2), Qt::EditRole).toString()));

    refresh_medsite_ters_tab();
}

void assignment_wnd::on_bn_edit_medpart_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medparts || !ui->tab_medparts->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица подразделений медорганизаций не загружена!\nНажмите кнопку [Обновить] и выберите подразделение МО из списка.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medparts->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        if (QMessageBox::question(this,
                                  "Нужно подтверждение",
                                  "Изменить данные выбранного подразделения МО?",
                                  QMessageBox::Yes|QMessageBox::Cancel,
                                  QMessageBox::Yes)==QMessageBox::Cancel) {
            return;
        }

        QModelIndex index = indexes.front();
        QString ID = QString::number(model_medparts.data(model_medparts.index(index.row(), 0), Qt::EditRole).toInt());


        // проверка
        if (ui->combo_medpart_type->currentIndex()<0) {
            QMessageBox::warning(this, "Недостаточно данных", "Выберите тип нового подразделения.\n\nСохранение отменено.");
            return;
        }
        if (ui->ln_medpart_code->text().trimmed().simplified().isEmpty()) {
            QMessageBox::warning(this, "Недостаточно данных", "Введите код подразделения.\n\nСохранение отменено.");
            return;
        }
        if (ui->ln_medpart_name->text().trimmed().simplified().isEmpty()) {
            QMessageBox::warning(this, "Недостаточно данных", "Введите название подразделения.\n\nСохранение отменено.");
            return;
        }

        // проверим код на уникальность
        QString sql = "select count(*) from spr_medparts "
                      " where code_mo='" + model_medparts.data(model_medparts.index(index.row(), 2), Qt::EditRole).toString() + "' and "
                      "       code_mp='" + ui->ln_medpart_code->text().trimmed() + "' and "
                      "       id<>" + ID + " ; ";

        QSqlQuery *query = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Проверка кода подразделения на уникальность", *query, true, db, data_app) || !query->next()) {
            QMessageBox::warning(this, "Непредвиденная ошибка", "При попытке проверить код подразделения на уникальность произошла неожиданная ошибка.\n\nСохранение отменено.");
            delete query;
            return;
        }
        if (query->value(0).toInt()>0) {
            QMessageBox::warning(this,
                                  "Такой код подразделения МО уже есть",
                                  "В базе данных уже есть подразделение данной МО с таким же реестровым номером.\nКод подразделения должен быть уникальным.\n\nСохранение отменено.");
            delete query;
            return;
        }


        QString code_mo = model_medparts.data(model_medparts.index(index.row(), 1), Qt::EditRole).toString();

        db.transaction();
        // поравим
        QString sql_upd = "update spr_medparts "
                      " set "
                      "   code_mo='" + code_mo + "', "
                      "   code_mp='" + ui->ln_medpart_code->text().trimmed().simplified() + "', "
                      "   name_mp='" + ui->ln_medpart_name->text().trimmed().simplified() + "', "
                      "   type_mp=" + QString::number(ui->combo_medpart_type->currentData().toInt()) + " "
                      " where id=" + ID + " ; ";

        QSqlQuery *query_upd = new QSqlQuery(db);
        if (!mySQL.exec(this, sql_upd, "Обновление подразделения", *query_upd, true, db, data_app)) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке изменить данные подразделения произошла неожиданная ошибка.\n\nИзменение отменено.");
            delete query_upd;
            return;
        }
        //data_medorg.code_ms = ui->ln_medpart_code->text();
        /*data_medpart.name_mo = ui->ln_name_mo->text();*/
        delete query;
        db.commit();

        refresh_medparts_tab();
        refresh_medsites_tab();
        refresh_medters_tab();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано", "Ничего не выбрано");
    }
}

void assignment_wnd::on_bn_refresh_medparts_clicked() {
    refresh_medparts_tab();
}

void assignment_wnd::on_bn_delete_medpart_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medparts || !ui->tab_medparts->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица не загружена!\nНажмите кнопку [Поиск] и выберите подразделение из списка.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medparts->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        if (QMessageBox::question(this,
                                  "Нужно подтверждение",
                                  "Удалить выбранный участок из базы?\n",
                                  QMessageBox::Yes|QMessageBox::Cancel,
                                  QMessageBox::Yes)==QMessageBox::Cancel) {
            return;
        }
        db.transaction();

        QModelIndex index = indexes.front();
        QString ID = QString::number(model_medparts.data(model_medparts.index(index.row(), 0), Qt::EditRole).toInt());


        QString sql = "delete from spr_medparts "
                      " where id=" + ID + " ; ";
        QSqlQuery *query2 = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Удаление участка", *query2, true, db, data_app)) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке удалить участок произошла неожиданная ошибка.\n\nУдаление отменено.");
            delete query2;
            return;
        }
        delete query2;

        db.commit();

        refresh_medparts_tab();
        refresh_medsites_tab();
        refresh_medters_tab();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано", "Ничего не выбрано");
    }
}


void assignment_wnd::refresh_medsite_mo() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка медорганизаций
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mo, '(' || code_mo || ') ' || name_mo "
                  "  from spr_medorgs s "
                  " where is_head=1 "
                  " order by code_mo ; ";
    mySQL.exec(this, sql, QString("Справочник медорганизаций для таблицы мед.участков"), *query, true, db, data_app);
    ui->combo_medsite_mo->clear();
    while (query->next()) {
        ui->combo_medsite_mo->addItem(query->value(1).toString(), query->value(0).toString());
    }
    //ui->combo_medsite_mo->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::refresh_medsite_mp() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка подразделений
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mp, '(' || code_mp || ') ' || name_mp "
                  "  from spr_medparts u "
                  " where u.code_mo='" + ui->combo_medsite_mo->currentData().toString() + "' "
                  " order by code_mp ; ";
    mySQL.exec(this, sql, QString("Справочник подразделений для таблицы мед.участков"), *query, true, db, data_app);
    ui->combo_medsite_mp->clear();
    while (query->next()) {
        ui->combo_medsite_mp->addItem(query->value(1).toString(), query->value(0).toString());
    }
    //ui->combo_medsite_mp->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::on_ch_medsites_mo_clicked(bool checked) {
    ui->combo_medsite_mo->setEnabled(!checked);
    //ui->combo_medsite_mp->setEnabled(!checked);
}


/*void assignment_wnd::on_ch_medters_ms_clicked(bool checked) {
    if (checked) {
        ui->ch_medters_mo->setChecked(true);
        ui->combo_medter_mo->setEnabled(!checked);
        //ui->combo_medter_mp->setEnabled(!checked);
    }
    ui->combo_medter_ms->setEnabled(!checked);
    refresh_medters_tab();
}*/

void assignment_wnd::on_ch_medters_mo_clicked(bool checked) {
    ui->combo_medter_mo->setEnabled(!checked);
    //if (!checked) {
    //    ui->ch_medters_ms->setChecked(false);
    //    ui->combo_medter_mp->setEnabled(!checked);
    //    ui->combo_medter_ms->setEnabled(!checked);
    //}
    refresh_medters_tab();
}

void assignment_wnd::on_combo_medsite_mo_activated(const QString &arg1) {
    refresh_medsite_mp();
}

void assignment_wnd::on_combo_medter_mo_activated(const QString &arg1) {
    //refresh_medter_mp();
}

/*void assignment_wnd::on_combo_medter_mp_activated(const QString &arg1) {
    refresh_medter_mp();
    refresh_medter_ms();
}*/















// 2

void assignment_wnd::on_bn_refresh_terrs_clicked() {
    refresh_terrs_tab();
}

// получим список используемых кодов участков для этой МО
void assignment_wnd::refresh_combo_medsite_terrs() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка участков
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select q.code_ms, q.code_mp || ' -> ' || q.cnt "
                  "  from "
                  " ( select COALESCE(code_mp,'') as code_mp, COALESCE(code_ms,'') as code_ms, COALESCE(code_ms,'') || '   (' || count(*) || ')' as cnt "
                  "   from assig_by_terr a "
                  "   where code_mo='" + ui->combo_medorg_terrs->currentData().toString() + "' "
                  "   group by code_mo, code_mp, code_ms "
                  " ) q "
                  "order by code_mp, code_ms ; ";

    mySQL.exec(this, sql, QString("Список используемых участков МО для таблицы адресов прикреплений"), *query, true, db, data_app);
    ui->combo_medsite_terrs->clear();
    ui->combo_medsite_terrs->addItem(" - все участки - ", "-1");
    while (query->next()) {
        ui->combo_medsite_terrs->addItem(query->value(1).toString(), query->value(0).toString());
    }
    //ui->combo_medsite_terrs->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::refresh_terrs_tab() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }

    this->setCursor(Qt::WaitCursor);
    QApplication::processEvents();

    QString code_mo = ui->combo_medorg_terrs->currentData().toString();

    //  получим список кодов ЛПУ, соответствующих этой МО
    QString sql_codes = "select code_mo, code_head "
                        "  from spr_medorgs "
                        " where code_mo='" + code_mo + "' or code_head='" + code_mo + "' ; ";
    QSqlQuery *query_codes = new QSqlQuery(db);
    QString codes_mo  = "'-1'";
    query_codes->exec(sql_codes);
    while (query_codes->next()) {
        codes_mo  += ",'" + query_codes->value(0).toString() + "'";
    }

    QString terrs_sql;
    terrs_sql  = "select a.id, a.ocato, " +
                 QString( ui->ch_show_names->isChecked()
                   ? ( " a.subj, t.offname||' '||t.shortname as subj_name, "
                       " a.dstr, d.offname||' '||d.shortname as dstr_name, "
                       " a.city, c.offname||' '||c.shortname as city_name, "
                       " a.nasp, n.offname||' '||n.shortname as nasp_name, "
                       " a.strt, s.offname||' '||s.shortname as strt_name, ")
                   : ( " a.subj, NULL, "
                       " a.dstr, NULL, "
                       " a.city, NULL, "
                       " a.nasp, NULL, "
                       " a.strt, NULL, " )
                 ) +
                       " case a.from_start when 1 then '<<' else NULL end as from_start, a.house, a.corp, case a.to_end when 1 then '>>' else NULL end as to_end, "
                       " a.sex, case a.sex when 1 then 'муж.' when 2 then ' жен.' else '- все -' end as sex_txt, a.age, case a.age when 1 then ' дети' when 2 then 'взросл.' else '- любая -' end as age_txt, "
                       " mos.code_head, a.code_mo, a.code_mp, a.code_ms, a.snils_mt, "
                       " a.comment, a.dt_ins "
                 "  from assig_by_terr a "
                 "  left join spr_medorgs mos on(mos.code_mo=a.code_mo) " +
                 QString( ui->ch_show_names->isChecked()
                   ? ( " left join fias.subj t on(t.subj=a.subj) "
                       " left join fias.dstr d on(d.subj=a.subj and d.dstr=a.dstr) "
                       " left join fias.city c on(c.subj=a.subj and c.dstr=a.dstr and c.city=a.city) "
                       " left join fias.nasp n on(n.subj=a.subj and n.dstr=a.dstr and n.city=a.city and n.nasp=a.nasp) "
                       " left join fias.strt s on(s.subj=a.subj and s.dstr=a.dstr and s.city=a.city and s.nasp=a.nasp and s.strt=a.strt) " )
                   : ( "" )
                 );

    bool is_first = true;
    int cnt_r = 0;
    if (ui->combo_medorg_terrs->currentIndex()>0) {
        if (is_first) {
            terrs_sql += " where ";
            is_first = false;
        } else {
            terrs_sql += " and ";
        }
        terrs_sql += " a.code_mo in (" + codes_mo + ") ";
        cnt_r++;
    } else {
        /*if (is_first) {
            terrs_sql += " where ";
            is_first = false;
        } else {
            terrs_sql += " and ";
        }
        terrs_sql += " a.code_mo='000000' ";*/
    }
    if (ui->combo_subj_terrs->currentIndex()>0) {
        if (is_first) {
            terrs_sql += " where ";
            is_first = false;
        } else {
            terrs_sql += " and ";
        }
        terrs_sql += " a.subj=" + QString::number(ui->combo_subj_terrs->currentData().toInt()) + " ";
        cnt_r++;
    }
    if ( ui->combo_dstr_terrs->currentIndex()>0
         || ui->combo_city_terrs->currentIndex()>0
         || ui->combo_nasp_terrs->currentIndex()>0
         || ui->combo_strt_terrs->currentIndex()>0 ) {
        if (is_first) {
            terrs_sql += " where ";
            is_first = false;
        } else {
            terrs_sql += " and ";
        }
        terrs_sql += " a.dstr=" + QString::number(ui->combo_dstr_terrs->currentData().toInt()) + " ";
        cnt_r++;
    }
    if ( ui->combo_city_terrs->currentIndex()>0
         || ui->combo_nasp_terrs->currentIndex()>0
         || ui->combo_strt_terrs->currentIndex()>0 ) {
        if (is_first) {
            terrs_sql += " where ";
            is_first = false;
        } else {
            terrs_sql += " and ";
        }
        terrs_sql += " a.city=" + QString::number(ui->combo_city_terrs->currentData().toInt()) + " ";
        cnt_r++;
    }
    if ( ui->combo_nasp_terrs->currentIndex()>0
         || ui->combo_strt_terrs->currentIndex()>0 ) {
        if (is_first) {
            terrs_sql += " where ";
            is_first = false;
        } else {
            terrs_sql += " and ";
        }
        terrs_sql += " a.nasp=" + QString::number(ui->combo_nasp_terrs->currentData().toInt()) + " ";
        cnt_r++;
    }
    if (ui->combo_strt_terrs->currentIndex()>0) {
        if (is_first) {
            terrs_sql += " where ";
            is_first = false;
        } else {
            terrs_sql += " and ";
        }
        terrs_sql += " a.strt=" + QString::number(ui->combo_strt_terrs->currentData().toInt()) + " ";
        cnt_r++;
    }
    if (ui->combo_age_terrs->currentIndex()>0) {
        if (is_first) {
            terrs_sql += " where ";
            is_first = false;
        } else {
            terrs_sql += " and ";
        }
        terrs_sql += " a.age<>" + QString::number(3-ui->combo_age_terrs->currentIndex()) + " ";
        cnt_r++;
    }
    if (ui->combo_sex_terrs->currentIndex()>0) {
        if (is_first) {
            terrs_sql += " where ";
            is_first = false;
        } else {
            terrs_sql += " and ";
        }
        terrs_sql += " a.sex<>" + QString::number(3-ui->combo_sex_terrs->currentIndex()) + " ";
        cnt_r++;
    }
    if (ui->combo_medsite_terrs->currentIndex()>0) {
        if (is_first) {
            terrs_sql += " where ";
            is_first = false;
        } else {
            terrs_sql += " and ";
        }
        terrs_sql += " a.code_ms='" + ui->combo_medsite_terrs->currentData().toString() + "' ";
        cnt_r++;
    }
    terrs_sql += " order by " +
                 QString( ui->ch_show_names->isChecked()
                   ? ( " t.offname||' '||t.shortname, "
                       " d.offname||' '||d.shortname, "
                       " c.offname||' '||c.shortname, "
                       " n.offname||' '||n.shortname, "
                       " s.offname||' '||s.shortname, "
                       " a.house, a.corp ")
                   : ( " a.subj, "
                       " a.dstr, "
                       " a.city, "
                       " a.nasp, "
                       " a.strt, "
                       " a.house, a.corp") ) +
                  " ; ";

    /*if (cnt_r<2) {
        // должно быть минимум 2 условия отбора
        this->setCursor(Qt::ArrowCursor);
        return;
    }*/

    //QInputDialog::getText(this, "", "", QLineEdit::Normal, terrs_sql);
    data_app.KLADR_subj = ui->combo_subj_terrs->currentData().toInt();
    data_app.KLADR_dstr = ui->combo_dstr_terrs->currentData().toInt();
    data_app.KLADR_city = ui->combo_city_terrs->currentData().toInt();
    data_app.KLADR_nasp = ui->combo_nasp_terrs->currentData().toInt();
    data_app.KLADR_strt = ui->combo_strt_terrs->currentData().toInt();

    model_terrs.setQuery(terrs_sql,db);
    QString err2 = model_terrs.lastError().driverText();

    // подключаем модель из БД
    ui->tab_terrs->setModel(&model_terrs);

    // обновляем таблицу
    ui->tab_terrs->reset();

    // задаём ширину колонок
    ui->tab_terrs->setColumnWidth(  0,  2);     // id
    ui->tab_terrs->setColumnWidth(  1,  2);     // ocato
    if (ui->ch_show_names->isChecked()) {
        ui->tab_terrs->setColumnWidth(  2,  2);     // subj
        ui->tab_terrs->setColumnWidth(  3,150);     // subj_str
        ui->tab_terrs->setColumnWidth(  4,  2);     // dstr
        ui->tab_terrs->setColumnWidth(  5,150);     // dstr_str
        ui->tab_terrs->setColumnWidth(  6,  2);     // city
        ui->tab_terrs->setColumnWidth(  7,150);     // city_str
        ui->tab_terrs->setColumnWidth(  8,  2);     // nasp
        ui->tab_terrs->setColumnWidth(  9,180);     // nasp_str
        ui->tab_terrs->setColumnWidth( 10,  2);     // strt
        ui->tab_terrs->setColumnWidth( 11,180);     // strt_str
    } else {
        ui->tab_terrs->setColumnWidth(  2, 40);     // subj
        ui->tab_terrs->setColumnWidth(  3,  2);     // subj_str
        ui->tab_terrs->setColumnWidth(  4, 40);     // dstr
        ui->tab_terrs->setColumnWidth(  5,  2);     // dstr_str
        ui->tab_terrs->setColumnWidth(  6, 40);     // city
        ui->tab_terrs->setColumnWidth(  7,  2);     // city_str
        ui->tab_terrs->setColumnWidth(  8, 40);     // nasp
        ui->tab_terrs->setColumnWidth(  9,  2);     // nasp_str
        ui->tab_terrs->setColumnWidth( 10, 40);     // strt
        ui->tab_terrs->setColumnWidth( 11,  2);     // strt_str
    }
    ui->tab_terrs->setColumnWidth( 12, 20);     // from
    ui->tab_terrs->setColumnWidth( 13, 70);     // house
    ui->tab_terrs->setColumnWidth( 14, 50);     // corp
    ui->tab_terrs->setColumnWidth( 15, 20);     // to
    ui->tab_terrs->setColumnWidth( 16,  2);     // sex
    ui->tab_terrs->setColumnWidth( 17, 55);     // sex_text
    ui->tab_terrs->setColumnWidth( 18,  2);     // age
    ui->tab_terrs->setColumnWidth( 19, 55);     // age_text
    ui->tab_terrs->setColumnWidth( 20, 70);     // code_head
    ui->tab_terrs->setColumnWidth( 21, 70);     // code_mo
    ui->tab_terrs->setColumnWidth( 22, 80);     // code_mp
    ui->tab_terrs->setColumnWidth( 23, 80);     // code_ms
    ui->tab_terrs->setColumnWidth( 24, 90);     // snils_mt
    ui->tab_terrs->setColumnWidth( 25,150);     // comment
    ui->tab_terrs->setColumnWidth( 26, 80);     // dt_ins
    // правим заголовки
    model_terrs.setHeaderData(  1, Qt::Horizontal, ("ОКАТО"));
    model_terrs.setHeaderData(  3, Qt::Horizontal, ("регион"));
    model_terrs.setHeaderData(  5, Qt::Horizontal, ("район/улус"));
    model_terrs.setHeaderData(  7, Qt::Horizontal, ("город"));
    model_terrs.setHeaderData(  9, Qt::Horizontal, ("нас.пункт"));
    model_terrs.setHeaderData( 11, Qt::Horizontal, ("улица"));
    model_terrs.setHeaderData( 12, Qt::Horizontal, ("<"));
    model_terrs.setHeaderData( 13, Qt::Horizontal, ("дом"));
    model_terrs.setHeaderData( 14, Qt::Horizontal, ("корп."));
    model_terrs.setHeaderData( 15, Qt::Horizontal, (">"));
    model_terrs.setHeaderData( 16, Qt::Horizontal, ("пол"));
    model_terrs.setHeaderData( 17, Qt::Horizontal, ("пол"));
    model_terrs.setHeaderData( 18, Qt::Horizontal, ("возр."));
    model_terrs.setHeaderData( 19, Qt::Horizontal, ("возр."));
    model_terrs.setHeaderData( 20, Qt::Horizontal, ("глав. МО"));
    model_terrs.setHeaderData( 21, Qt::Horizontal, ("МО"));
    model_terrs.setHeaderData( 22, Qt::Horizontal, ("подразд."));
    model_terrs.setHeaderData( 23, Qt::Horizontal, ("участок"));
    model_terrs.setHeaderData( 24, Qt::Horizontal, ("врач (СНИЛС)"));
    model_terrs.setHeaderData( 25, Qt::Horizontal, ("комментарий"));
    model_terrs.setHeaderData( 26, Qt::Horizontal, ("создана"));
    ui->tab_terrs->repaint();

    /*refresh_head_mdorgs();
    refresh_subj_terr();
    refresh_dstr_terr();
    refresh_city_terr();
    refresh_nasp_terr();
    refresh_strt_terr();*/

    ui->tab_terrs->setEnabled(true);
    this->setCursor(Qt::ArrowCursor);
    QApplication::processEvents();
}

void assignment_wnd::on_combo_medorg_terrs_activated(int index) {
    refresh_combo_medsite_terrs();
    refresh_terrs_tab();
}

void assignment_wnd::on_combo_medsites_terrs_activated(int index) {
    refresh_terrs_tab();
}

void assignment_wnd::on_bn_clear_filter_terrs_clicked() {
    ui->line_strt_filter->clear();
    ui->combo_subj_terrs->setCurrentIndex(0);
    ui->combo_dstr_terrs->setCurrentIndex(0);
    ui->combo_city_terrs->setCurrentIndex(0);
    ui->combo_nasp_terrs->setCurrentIndex(0);
    ui->combo_strt_terrs->setCurrentIndex(0);
    ui->combo_age_terrs->setCurrentIndex(0);
    ui->combo_sex_terrs->setCurrentIndex(0);

    refresh_terrs_tab();
}

void assignment_wnd::refresh_medorg_terr() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка регионов
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mo, '('||code_mo||') '||name_mo "
                  "  from spr_medorgs "
                  " where is_head=1 "
                  " order by code_mo ; ";

    mySQL.exec(this, sql, QString("Справочник медогранизаций"), *query, true, db, data_app);
    ui->combo_medorg_terrs->clear();
    ui->combo_medorg_terrs->addItem(" - выберите МО - ", 0);
    while (query->next()) {
        ui->combo_medorg_terrs->addItem(query->value(1).toString(), query->value(0).toString());
    }
    delete query;
    ui->combo_medorg_terrs->setCurrentIndex(0);

    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::refresh_subj_terr() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка регионов
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select subj, offname||' '||shortname||'  ('||subj||')' "
                  "  from fias.subj "
                  " where aolevel=1 "
                  "   and actstatus=1 "
                  " order by offname||' '||shortname ; ";
    query->exec(sql);

    ui->combo_subj_terrs->clear();
    ui->combo_subj_terrs->addItem(" - все регионы - ", 0);
    while (query->next()) {
        ui->combo_subj_terrs->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    delete query;

    if (data_app.ocato=="20000") {
        ui->combo_subj_terrs->setCurrentIndex(ui->combo_subj_terrs->findData(36, Qt::UserRole));
    } else if (data_app.ocato=="14000") {
        ui->combo_subj_terrs->setCurrentIndex(ui->combo_subj_terrs->findData(31, Qt::UserRole));
    } else if (data_app.ocato=="38000") {
        ui->combo_subj_terrs->setCurrentIndex(ui->combo_subj_terrs->findData(46, Qt::UserRole));
    } else if (data_app.KLADR_subj==0) {
        ui->combo_subj_terrs->setCurrentIndex(0);
    } else {
        ui->combo_subj_terrs->setCurrentIndex(ui->combo_subj_terrs->findData(data_app.KLADR_subj, Qt::UserRole));
    }

    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::refresh_dstr_terr() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка районов выбранного региона
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select dstr, offname||' '||shortname||'  ('||dstr||')' "
                  "  from fias.dstr "
                  " where subj=" + QString::number(ui->combo_subj_terrs->currentData().toInt()) + " "
                  "   and aolevel=3 "
                  "   and actstatus=1 "
                  " order by offname||' '||shortname ";
    query->exec(sql);

    ui->combo_dstr_terrs->clear();
    ui->combo_dstr_terrs->addItem(" - все районы - ", 0);
    while (query->next()) {
        ui->combo_dstr_terrs->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    ui->combo_dstr_terrs->setCurrentIndex(ui->combo_dstr_terrs->findData(data_app.KLADR_dstr, Qt::UserRole));
    delete query;

    if (data_app.ocato=="20000") {
        ui->combo_dstr_terrs->setCurrentIndex(ui->combo_dstr_terrs->findData(0, Qt::UserRole));
    } else if (data_app.ocato=="14000") {
        ui->combo_dstr_terrs->setCurrentIndex(ui->combo_dstr_terrs->findData(0, Qt::UserRole));
    } else if (data_app.ocato=="38000") {
        ui->combo_dstr_terrs->setCurrentIndex(ui->combo_dstr_terrs->findData(0, Qt::UserRole));
    } else if (data_app.KLADR_dstr==0) {
        ui->combo_dstr_terrs->setCurrentIndex(0);
    } else {
        ui->combo_dstr_terrs->setCurrentIndex(ui->combo_dstr_terrs->findData(data_app.KLADR_dstr, Qt::UserRole));
    }

    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::refresh_city_terr() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка городов выбранного района
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select city, offname||' '||shortname||'  ('||city||')' "
                  "  from fias.city "
                  " where subj=" + QString::number(ui->combo_subj_terrs->currentData().toInt()) + " "
                  "   and dstr=" + QString::number(ui->combo_dstr_terrs->currentData().toInt()) + " "
                  "   and aolevel=4 "
                  "   and actstatus=1 "
                  " order by offname||' '||shortname ";
    query->exec(sql);

    ui->combo_city_terrs->clear();
    ui->combo_city_terrs->addItem(" - все города - ", 0);
    while (query->next()) {
        ui->combo_city_terrs->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    ui->combo_city_terrs->setCurrentIndex(ui->combo_city_terrs->findData(data_app.KLADR_city, Qt::UserRole));
    delete query;

    if (data_app.ocato=="20000") {
        ui->combo_city_terrs->setCurrentIndex(ui->combo_city_terrs->findData(1, Qt::UserRole));
    } else if (data_app.ocato=="14000") {
        ui->combo_city_terrs->setCurrentIndex(ui->combo_city_terrs->findData(1, Qt::UserRole));
    } else if (data_app.ocato=="38000") {
        ui->combo_city_terrs->setCurrentIndex(ui->combo_city_terrs->findData(1, Qt::UserRole));
    } else if (data_app.KLADR_city==0) {
        ui->combo_city_terrs->setCurrentIndex(0);
    } else {
        ui->combo_city_terrs->setCurrentIndex(ui->combo_city_terrs->findData(data_app.KLADR_city, Qt::UserRole));
    }

    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::refresh_nasp_terr() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка насенлённых пунктов выбранного города
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select nasp, offname||' '||shortname||'  ('||nasp||')' "
                  "  from fias.nasp "
                  " where subj=" + QString::number(ui->combo_subj_terrs->currentData().toInt()) + " "
                  "   and dstr=" + QString::number(ui->combo_dstr_terrs->currentData().toInt()) + " "
                  "   and city=" + QString::number(ui->combo_city_terrs->currentData().toInt()) + " "
                  "   and aolevel=6 "
                  "   and actstatus=1 "
                  " order by offname||' '||shortname ";
    query->exec(sql);

    ui->combo_nasp_terrs->clear();
    ui->combo_nasp_terrs->addItem(" - все нас.пункты - ", 0);
    while (query->next()) {
        ui->combo_nasp_terrs->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    ui->combo_nasp_terrs->setCurrentIndex(ui->combo_nasp_terrs->findData(data_app.KLADR_nasp, Qt::UserRole));
    delete query;

    if (data_app.ocato=="20000") {
        ui->combo_nasp_terrs->setCurrentIndex(ui->combo_nasp_terrs->findData(0, Qt::UserRole));
    } else if (data_app.ocato=="14000") {
        ui->combo_nasp_terrs->setCurrentIndex(ui->combo_nasp_terrs->findData(0, Qt::UserRole));
    } else if (data_app.ocato=="38000") {
        ui->combo_nasp_terrs->setCurrentIndex(ui->combo_nasp_terrs->findData(0, Qt::UserRole));
    } else if (data_app.KLADR_nasp==0) {
        ui->combo_nasp_terrs->setCurrentIndex(0);
    } else {
        ui->combo_nasp_terrs->setCurrentIndex(ui->combo_nasp_terrs->findData(data_app.KLADR_nasp, Qt::UserRole));
    }

    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::refresh_strt_terr() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка улиц выбранного населённого пункта
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select strt, offname||' '||shortname||'  ('||strt||')' "
                  "  from fias.strt "
                  " where subj=" + QString::number(ui->combo_subj_terrs->currentData().toInt()) + " "
                  "   and dstr=" + QString::number(ui->combo_dstr_terrs->currentData().toInt()) + " "
                  "   and city=" + QString::number(ui->combo_city_terrs->currentData().toInt()) + " "
                  "   and nasp=" + QString::number(ui->combo_nasp_terrs->currentData().toInt()) + " "
                  "   and aolevel=7 "
                  "   and actstatus=1 ";

    if (!ui->line_strt_filter->text().trimmed().isEmpty())
         sql +=   "   and upper(offname) like('%" + ui->line_strt_filter->text().trimmed().toUpper() + "%') ";

    sql +=        " order by offname||' '||shortname ";
    query->exec(sql);

    ui->combo_strt_terrs->clear();
    ui->combo_strt_terrs->addItem(" - все улицы - ", 0);
    while (query->next()) {
        ui->combo_strt_terrs->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    ui->combo_strt_terrs->setCurrentIndex(ui->combo_strt_terrs->findData(data_app.KLADR_strt, Qt::UserRole));
    delete query;

    if (data_app.ocato=="20000") {
        ui->combo_strt_terrs->setCurrentIndex(ui->combo_strt_terrs->findData(0, Qt::UserRole));
    } else if (data_app.ocato=="14000") {
        ui->combo_strt_terrs->setCurrentIndex(ui->combo_strt_terrs->findData(0, Qt::UserRole));
    } else if (data_app.ocato=="38000") {
        ui->combo_strt_terrs->setCurrentIndex(ui->combo_strt_terrs->findData(0, Qt::UserRole));
    } else if (data_app.KLADR_strt==0) {
        ui->combo_strt_terrs->setCurrentIndex(0);
    } else {
        ui->combo_strt_terrs->setCurrentIndex(ui->combo_strt_terrs->findData(data_app.KLADR_strt, Qt::UserRole));
    }

    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::on_combo_subj_terrs_activated(int index) {
    data_app.KLADR_subj = ui->combo_subj_terrs->currentData().toInt();
    refresh_dstr_terr();
    refresh_city_terr();
    refresh_nasp_terr();
    refresh_strt_terr();

    refresh_terrs_tab();
}

void assignment_wnd::on_combo_dstr_terrs_activated(int index) {
    data_app.KLADR_dstr = ui->combo_dstr_terrs->currentData().toInt();
    refresh_city_terr();
    refresh_nasp_terr();
    refresh_strt_terr();

    refresh_terrs_tab();
}

void assignment_wnd::on_combo_city_terrs_activated(int index) {
    data_app.KLADR_city = ui->combo_city_terrs->currentData().toInt();
    refresh_nasp_terr();
    refresh_strt_terr();

    refresh_terrs_tab();
}

void assignment_wnd::on_combo_nasp_terrs_activated(int index) {
    data_app.KLADR_nasp = ui->combo_nasp_terrs->currentData().toInt();
    refresh_strt_terr();
    data_assig.subj    = data_app.KLADR_subj;
    data_assig.dstr    = data_app.KLADR_dstr;
    data_assig.city    = data_app.KLADR_city;
    data_assig.nasp    = data_app.KLADR_nasp;
    data_assig.strt    = data_app.KLADR_strt;

    refresh_terrs_tab();
}

void assignment_wnd::on_combo_strt_terrs_activated(int index) {
    data_app.KLADR_strt = ui->combo_strt_terrs->currentData().toInt();
    data_assig.subj    = data_app.KLADR_subj;
    data_assig.dstr    = data_app.KLADR_dstr;
    data_assig.city    = data_app.KLADR_city;
    data_assig.nasp    = data_app.KLADR_nasp;
    data_assig.strt    = data_app.KLADR_strt;

    refresh_terrs_tab();
}

void assignment_wnd::on_bn_add_terrs_clicked() {
    data_assig.id = -1;

    delete add_assig_w;
    add_assig_w = new add_assig_wnd(db, db_FIAS, data_app, data_assig, settings, ui->te_log, this);

    add_assig_w->exec();

    refresh_terrs_tab();
}

void assignment_wnd::on_tab_terrs_clicked(const QModelIndex &index) {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_terrs || !ui->tab_terrs->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица правил терр-прикрепления не загружена!\nНажмите кнопку [Обновить] и выберите строку из списка.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_terrs->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        this->setCursor(Qt::WaitCursor);

        QModelIndex index = indexes.front();
        data_assig.id      = model_terrs.data(model_terrs.index(index.row(), 0), Qt::EditRole).toInt();

        data_assig.ocato   = model_terrs.data(model_terrs.index(index.row(), 1), Qt::EditRole).toString();
        data_assig.subj    = model_terrs.data(model_terrs.index(index.row(), 2), Qt::EditRole).toInt();
        data_assig.dstr    = model_terrs.data(model_terrs.index(index.row(), 4), Qt::EditRole).toInt();
        data_assig.city    = model_terrs.data(model_terrs.index(index.row(), 6), Qt::EditRole).toInt();
        data_assig.nasp    = model_terrs.data(model_terrs.index(index.row(), 8), Qt::EditRole).toInt();
        data_assig.strt    = model_terrs.data(model_terrs.index(index.row(),10), Qt::EditRole).toInt();

        data_assig.right_type = (model_terrs.data(model_terrs.index(index.row(),12), Qt::EditRole).toInt()>0 &&
                                 model_terrs.data(model_terrs.index(index.row(),15), Qt::EditRole).toInt()>0) ? 4 : 1;

        data_assig.from_start = (model_terrs.data(model_terrs.index(index.row(),12), Qt::EditRole).toInt()>0);
        data_assig.house   = model_terrs.data(model_terrs.index(index.row(),13), Qt::EditRole).toInt();
        data_assig.corp    = model_terrs.data(model_terrs.index(index.row(),14), Qt::EditRole).toString();
        data_assig.house2  = -1;
        data_assig.corp2   = "";
        data_assig.to_end  = (model_terrs.data(model_terrs.index(index.row(),15), Qt::EditRole).toInt()>0);
        data_assig.sex     = model_terrs.data(model_terrs.index(index.row(),17), Qt::EditRole).toInt();
        data_assig.age     = model_terrs.data(model_terrs.index(index.row(),19), Qt::EditRole).toInt();
        data_assig.code_mo = model_terrs.data(model_terrs.index(index.row(),21), Qt::EditRole).toString();
        data_assig.code_mp = model_terrs.data(model_terrs.index(index.row(),22), Qt::EditRole).toString();
        data_assig.code_ms = model_terrs.data(model_terrs.index(index.row(),23), Qt::EditRole).toString();
        data_assig.snils_mt= model_terrs.data(model_terrs.index(index.row(),24), Qt::EditRole).toString();
        data_assig.comment = model_terrs.data(model_terrs.index(index.row(),25), Qt::EditRole).toString();

        this->setCursor(Qt::ArrowCursor);
    }
}


void assignment_wnd::on_bn_edit_terrs_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_terrs || !ui->tab_terrs->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица правил терр-прикрепления не загружена!\nНажмите кнопку [Обновить] и выберите строку из списка.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_terrs->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        this->setCursor(Qt::WaitCursor);

        QModelIndex index = indexes.front();
        data_assig.id      = model_terrs.data(model_terrs.index(index.row(), 0), Qt::EditRole).toInt();

        data_assig.ocato   = model_terrs.data(model_terrs.index(index.row(), 1), Qt::EditRole).toString();
        data_assig.subj    = model_terrs.data(model_terrs.index(index.row(), 2), Qt::EditRole).toInt();
        data_assig.dstr    = model_terrs.data(model_terrs.index(index.row(), 4), Qt::EditRole).toInt();
        data_assig.city    = model_terrs.data(model_terrs.index(index.row(), 6), Qt::EditRole).toInt();
        data_assig.nasp    = model_terrs.data(model_terrs.index(index.row(), 8), Qt::EditRole).toInt();
        data_assig.strt    = model_terrs.data(model_terrs.index(index.row(),10), Qt::EditRole).toInt();

        data_assig.right_type = ( model_terrs.data(model_terrs.index(index.row(),12), Qt::EditRole).toInt()>0 ||
                                  model_terrs.data(model_terrs.index(index.row(),15), Qt::EditRole).toInt()>0 ) ? 4 :
                                  ( (model_terrs.data(model_terrs.index(index.row(),13), Qt::EditRole).isNull() ||
                                     model_terrs.data(model_terrs.index(index.row(),13), Qt::EditRole).toInt()==0) ? 3 : 1 );

        data_assig.from_start = (model_terrs.data(model_terrs.index(index.row(),12), Qt::EditRole).toInt()>0);
        data_assig.house   = model_terrs.data(model_terrs.index(index.row(),13), Qt::EditRole).toInt();
        data_assig.corp    = model_terrs.data(model_terrs.index(index.row(),14), Qt::EditRole).toString();
        data_assig.house2  = -1;
        data_assig.corp2   = "";
        data_assig.to_end  = (model_terrs.data(model_terrs.index(index.row(),15), Qt::EditRole).toInt()>0);
        data_assig.sex     = model_terrs.data(model_terrs.index(index.row(),17), Qt::EditRole).toInt();
        data_assig.age     = model_terrs.data(model_terrs.index(index.row(),19), Qt::EditRole).toInt();
        data_assig.code_mo = model_terrs.data(model_terrs.index(index.row(),21), Qt::EditRole).toString();
        data_assig.code_mp = model_terrs.data(model_terrs.index(index.row(),22), Qt::EditRole).toString();
        data_assig.code_ms = model_terrs.data(model_terrs.index(index.row(),23), Qt::EditRole).toString();
        data_assig.snils_mt= model_terrs.data(model_terrs.index(index.row(),24), Qt::EditRole).toString();
        data_assig.comment = model_terrs.data(model_terrs.index(index.row(),25), Qt::EditRole).toString();

        //QMessageBox::warning(this, "--","h="+QString::number(data_assig.house)+" c="+data_assig.corp+" h2="+QString::number(data_assig.house2)+" c2="+data_assig.corp2);

        delete add_assig_w;
        add_assig_w = new add_assig_wnd(db, db_FIAS, data_app, data_assig, settings, ui->te_log, this);

        add_assig_w->exec();

        this->setCursor(Qt::ArrowCursor);

        refresh_terrs_tab();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано", "Ничего не выбрано");
    }
}

void assignment_wnd::on_tab_terrs_activated(const QModelIndex &index) {
    on_bn_edit_terrs_clicked();
}

void assignment_wnd::on_bn_del_terrs_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_terrs || !ui->tab_terrs->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица правил терр-прикрепления не загружена!\nНажмите кнопку [Обновить] и выберите строку из списка.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_terrs->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        if (QMessageBox::question(this,
                                  "Нужно подтверждение",
                                  "Удалить выбранную строку?",
                                  QMessageBox::Yes|QMessageBox::Cancel,
                                  QMessageBox::Yes)==QMessageBox::Cancel) {
            return;
        }
        this->setCursor(Qt::WaitCursor);

        for (int i=0; i<indexes.size(); i++) {
            QModelIndex index = indexes.at(i);
            if (index.column()!=0)
                continue;
            QString ID = QString::number(model_terrs.data(model_terrs.index(index.row(), 0), Qt::EditRole).toInt());

            QString sql = "delete from assig_by_terr "
                          " where id=" + ID + " ; ";

            //QInputDialog::getText(this, "on_bn_del_terrs_clicked", "", QLineEdit::Normal, sql);

            QSqlQuery *query2 = new QSqlQuery(db);
            if (!mySQL.exec(this, sql, "Удаление медорганизации", *query2, true, db, data_app)) {
                db.rollback();
                QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке удалить медицинскую организацию произошла неожиданная ошибка.\n\nУдаление отменено.");
                delete query2;
                return;
            }
            delete query2;
        }        
        this->setCursor(Qt::ArrowCursor);

        db.commit();
        refresh_terrs_tab();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано", "Ничего не выбрано");
    }
}

void assignment_wnd::on_act_autoassig_triggered() {
    delete  autoassig_w;
    autoassig_w = new autoassig_wnd(db, data_app, settings, this);
    autoassig_w->exec();
}

void assignment_wnd::on_act_sessions_triggered() {
    delete  sessions_w;
    sessions_w = new sessions_wnd(db, data_app, this);
    sessions_w->exec();
}

#include "qt_windows.h"
#include "qwindowdefs_win.h"
#include <shellapi.h>

void assignment_wnd::on_act_help_triggered() {
    QString pathDocument = QApplication::applicationDirPath() + "/INKO_assignment_help.pdf";
    long result = (long)ShellExecute(0, 0, reinterpret_cast<const WCHAR*>(pathDocument.utf16()), 0, 0, SW_NORMAL);
    if (result<32) {
        QMessageBox::warning(this,
                             "Файл справки не найден",
                             "Файл справлки не найден:\n\n" + pathDocument +
                             "\nКод ошибки: " + QString::number(result) +
                             "\n\nПроверьте целостность установочного пакета программы.");
    }
}

void assignment_wnd::on_act_about_triggered() {
    QMessageBox::about(this,
                       "О программе",
                       "ИНКО-МЕД  -  «Прикрепление персон»\n\n"
                       "Утилита для редактирования базы полисов ООО \"МСК «ИНКО-МЕД»\"\nверсия " + data_app.version + "\n\n"
                       "© ООО МСК \"ИНКО-МЕД\", 2015 г.\nРазработчик  -  Кизянов А.Ф."
                       );
}

void assignment_wnd::on_ch_fam_pers_clicked()
{  refresh_persons_tab();  }
void assignment_wnd::on_ch_im_pers_clicked()
{  refresh_persons_tab();  }
void assignment_wnd::on_ch_ot_pers_clicked()
{  refresh_persons_tab();  }
void assignment_wnd::on_ch_datr_pers_clicked()
{  refresh_persons_tab();  }
void assignment_wnd::on_ch_sex_pers_clicked()
{  refresh_persons_tab();  }
void assignment_wnd::on_ch_pers_age_clicked()
{  refresh_persons_tab();  }

void assignment_wnd::on_ch_vs_num_clicked()
{  refresh_persons_tab();  }
void assignment_wnd::on_ch_enp_clicked()
{  refresh_persons_tab();  }
void assignment_wnd::on_ch_snils_clicked()
{  refresh_persons_tab();  }

void assignment_wnd::on_ch_assig_type_clicked() {
    ui->combo_assig_type->setVisible(ui->ch_assig_type->isChecked());
    refresh_assigs_tab();
}


void assignment_wnd::on_bn_refresh_medsite_ters_clicked() {
    refresh_medsite_ters_tab();
}
void assignment_wnd::on_ch_medsite_ters_now_clicked() {
    refresh_medsite_ters_tab();
}

void assignment_wnd::refresh_medsite_ters_tab() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    this->setCursor(Qt::WaitCursor);

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medsites->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();
        QString code_mo = model_medsites.data(model_medsites.index(index.row(), 1), Qt::EditRole).toString();
        QString code_mp = model_medsites.data(model_medsites.index(index.row(), 2), Qt::EditRole).toString();
        QString code_ms = model_medsites.data(model_medsites.index(index.row(), 3), Qt::EditRole).toString();

        QString medsite_ters_sql;
        medsite_ters_sql  = "select r.id_ms, r.id_mt, ms.code_mo, mo.name_mo, ms.code_mp, mp.name_mp, ms.code_ms, ms.name_ms, mt.fio_mt, mt.snils_mt, "
                            "       case r.prof_mt when 0 then '???' when 1 then 'врач' when 2 then 'медс(-б)' when 3 then 'фелд/ак.' end, "
                            "       r.date_beg, r.date_end ";
        medsite_ters_sql += "  from spr_ms_r_mt r "
                            "      join spr_medsites ms on(ms.id=r.id_ms) "
                            "      join spr_medters  mt on(mt.id=r.id_mt) "
                            "      join spr_medparts mp on(mp.code_mo=ms.code_mo and mp.code_mp=ms.code_mp) "
                            "      join spr_medorgs  mo on(mo.code_mo=ms.code_mo and mo.is_head=1) "
                            " where ms.code_mo='" + code_mo + "' "
                            "   and ms.code_mp='" + code_mp + "' "
                            "   and ms.code_ms='" + code_ms + "' ";
        if (ui->ch_medsite_ters_now->isChecked()) {
            medsite_ters_sql += " and (r.date_beg is not NULL and r.date_beg<='" + QDate::currentDate().toString("yyyy-MM-dd") + "') "
                                " and (r.date_end is NULL or r.date_end>'" + QDate::currentDate().toString("yyyy-MM-dd") + "') ";
        }
        medsite_ters_sql += " order by r.prof_mt, mt.fio_mt ; ";

        model_medsite_ters.setQuery(medsite_ters_sql,db);
        QString err2 = model_medsite_ters.lastError().driverText();

        // подключаем модель из БД
        ui->tab_medsite_ters->setModel(&model_medsite_ters);

        // обновляем таблицу
        ui->tab_medsite_ters->reset();

        // задаём ширину колонок
        ui->tab_medsite_ters->setColumnWidth( 0,  2);   // r.id_ms,
        ui->tab_medsite_ters->setColumnWidth( 1,  2);   // r.id_mt,
        ui->tab_medsite_ters->setColumnWidth( 2,  2);   // ms.code_mo,
        ui->tab_medsite_ters->setColumnWidth( 3,  2);   // mo.name_mo,
        ui->tab_medsite_ters->setColumnWidth( 4,  2);   // ms.code_mp,
        ui->tab_medsite_ters->setColumnWidth( 5,  2);   // mp.name_mp,
        ui->tab_medsite_ters->setColumnWidth( 6,  2);   // ms.code_ms,
        ui->tab_medsite_ters->setColumnWidth( 7,  2);   // ms.name_ms,
        ui->tab_medsite_ters->setColumnWidth( 8,150);   // mt.fio_mt,
        ui->tab_medsite_ters->setColumnWidth( 9,  2);   // mt.snils_mt,
        ui->tab_medsite_ters->setColumnWidth(10, 60);   // r.prof_mt,
        ui->tab_medsite_ters->setColumnWidth(11,  2);   // r.date_beg,
        ui->tab_medsite_ters->setColumnWidth(12,  2);   // r.date_end
        // правим заголовки
        model_medsite_ters.setHeaderData( 8, Qt::Horizontal, ("врач"));
        model_medsite_ters.setHeaderData(10, Qt::Horizontal, ("спец."));
        ui->tab_medsite_ters->repaint();
    }

    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::on_bn_add_medsite_ter_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"),
                             ("Нет открытого соединения к базе данных?\n "
                              "Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n "
                              "Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    this->setCursor(Qt::WaitCursor);

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medsites->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        s_data_medsite_ter data_medsite_ter;
        data_medsite_ter.id_ms = model_medsites.data(model_medsites.index(index.row(), 0), Qt::EditRole).toInt();
        data_medsite_ter.id_mt = -1;
        data_medsite_ter.code_mo = model_medsites.data(model_medsites.index(index.row(), 1), Qt::EditRole).toString();
        data_medsite_ter.name_mo = model_medsites.data(model_medsites.index(index.row(), 7), Qt::EditRole).toString();
        data_medsite_ter.code_mp = model_medsites.data(model_medsites.index(index.row(), 2), Qt::EditRole).toString();
        data_medsite_ter.name_mp = model_medsites.data(model_medsites.index(index.row(), 8), Qt::EditRole).toString();
        data_medsite_ter.code_ms = model_medsites.data(model_medsites.index(index.row(), 3), Qt::EditRole).toString();
        data_medsite_ter.name_ms = model_medsites.data(model_medsites.index(index.row(), 4), Qt::EditRole).toString();


        // проверим выделенную строку
        QModelIndexList indexes_mt = ui->tab_medters->selectionModel()->selection().indexes();

        if (indexes_mt.size()>0) {
            QModelIndex index_mt = indexes_mt.front();
            data_medsite_ter.id_mt = model_medters.data(model_medters.index(index_mt.row(), 0), Qt::EditRole).toInt();
        }

        delete add_medsite_ter_w;

        add_medsite_ter_w = new add_medsite_ter_wnd(db, data_app, data_medsite_ter, settings, this);
        add_medsite_ter_w->exec();
        refresh_medsite_ters_tab();

    } else {
        QMessageBox::warning(this, "Не выбран участок", "Выберите участок, в который надо добавить участкового врача.");
    }
    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::on_bn_delete_med_site_ter_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"),
                             ("Нет открытого соединения к базе данных?\n "
                              "Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n "
                              "Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medsite_ters || !ui->tab_medsite_ters->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!",
                             "Таблица не загружена!\nВыберите МО, подразделение, участок и участкового врача.");
        return;
    }
    this->setCursor(Qt::WaitCursor);

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medsite_ters->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        if (QMessageBox::question(this,
                                  "Нужно подтверждение",
                                  "Убрать выбранного участкового врача?\n",
                                  QMessageBox::Yes|QMessageBox::Cancel,
                                  QMessageBox::Yes)==QMessageBox::Cancel) {
            return;
        }
        db.transaction();

        QModelIndex index = indexes.front();
        QString id_ms = QString::number(model_medsite_ters.data(model_medsite_ters.index(index.row(), 0), Qt::EditRole).toInt());
        QString id_mt = QString::number(model_medsite_ters.data(model_medsite_ters.index(index.row(), 1), Qt::EditRole).toInt());

        QString sql = "update spr_ms_r_mt "
                      " set date_end='" + QDate::currentDate().toString("yyyy-MM-dd") + "' "
                      " where id_ms=" + id_ms + " "
                      "   and id_mt=" + id_mt + "; ";
        QSqlQuery *query2 = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Удаление участкового врача", *query2, true, db, data_app)) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке убрать из участка участкового врача произошла неожиданная ошибка.\n\nУдаление отменено.");
            delete query2;
            return;
        }
        delete query2;

        db.commit();
        refresh_medsite_ters_tab();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано", "Ничего не выбрано");
    }
    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::on_bn_consume_med_site_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    if (!ui->tab_medorgs || !ui->tab_medorgs->selectionModel()) {
        QMessageBox::warning(this, "Таблица не загружена!", "Таблица медорганизаций не загружена!\nНажмите кнопку [Обновить] и выберите МО из списка.");
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medsites->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();
        QString id_ms   = QString::number(model_medsites.data(model_medsites.index(index.row(), 0), Qt::EditRole).toInt());
        QString code_mo = model_medsites.data(model_medsites.index(index.row(), 1), Qt::EditRole).toString();
        QString code_mp = model_medsites.data(model_medsites.index(index.row(), 2), Qt::EditRole).toString();
        QString code_ms = model_medsites.data(model_medsites.index(index.row(), 3), Qt::EditRole).toString();
        int id_ms_consumed = -1;

        // запросим пароль
        QString pass = QInputDialog::getText(this, "Введите пароль",
                                             "Для поглощения выбранным участком любого другого врачебного участка введите пароль администратора базы прикреплений:\n"
                                             "(пустая строка означает отказ от операции)", QLineEdit::Password);
        while (pass!="650325" && !pass.isEmpty()) {
            pass = QInputDialog::getText(this, "Введите пароль",
                                         "Для поглощения выбранным участком любого другого врачебного участка введите пароль администратора базы прикреплений:\n"
                                         "(пустая строка означает отказ от операции)", QLineEdit::Password);
        }
        if (pass.isEmpty())
            return;

        // запросим код участка
        QString code_ms_consumed =
                QInputDialog::getText(this, "Введите код поглащаемого участка",
                                      "Введите точное написание кода поглащаемого участка данной МО с учётом пробелов и регистра букв:");

        // получим из базы ID поглащаемого участка
        QString sql = "select id "
                      "  from spr_medsites "
                      " where code_mo='" + code_mo + "' "
                      "   and code_mp='" + code_mp + "' "
                      "   and code_ms='" + code_ms_consumed + "' ; ";

        QSqlQuery *query = new QSqlQuery(db);
        if (!mySQL.exec(this, sql, "Получим ID поглощаемого участка", *query, true, db, data_app) || !query->next()) {
            db.rollback();
            //QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке получить ID поглощаемого участка произошла неожиданная ошибка.\n\nОперация поглощения участка отменена.");
            delete query;
            return;
        }
        id_ms_consumed = query->value(0).toInt();
        delete query;

        // перенесём все связки врачей с удаляемым участком в новый участок
        QString sql_ = "update spr_ms_r_mt "
                      " set id_ms=" + id_ms + " "
                      " where id_ms=" + QString::number(id_ms_consumed) + " ; ";

        QSqlQuery *query_ = new QSqlQuery(db);
        if (!mySQL.exec(this, sql_, "Перенос всех врачей из поглогщаемого на новый участок", *query_, true, db, data_app)) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке перенести всех врачей из поглогщаемого на новый участок произошла неожиданная ошибка.\n\nОперация поглощения участка отменена.");
            delete query_;
            return;
        }
        delete query_;

        // сольём адреса
        QString sql2 = "update assig_by_terr "
                      " set code_ms='" + code_ms + "' "
                      " where code_mo='" + code_mo + "' "
                      "       and code_mp='" + code_mp + "' "
                      "       and code_ms='" + code_ms_consumed + "' ; ";

        QSqlQuery *query2 = new QSqlQuery(db);
        if (!mySQL.exec(this, sql2, "Поглощение участка", *query2, true, db, data_app)) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке поглотить участок произошла неожиданная ошибка.\n\nОперация поглощения участка отменена.");
            delete query2;
            return;
        }
        delete query2;

        // удалим старый участок
        QString sql1 = "delete from spr_medsites "
                      " where code_mo='" + code_mo + "' "
                      "       and code_mp='" + code_mp + "' "
                      "       and code_ms='" + code_ms_consumed + "' ; ";

        QSqlQuery *query1 = new QSqlQuery(db);
        if (!mySQL.exec(this, sql1, "Поглощение участка", *query1, true, db, data_app)) {
            db.rollback();
            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке поглотить участок произошла неожиданная ошибка.\n\nОперация поглощения участка отменена.");
            delete query1;
            return;
        }
        delete query1;

        refresh_medsites_tab();
    }
}

void assignment_wnd::on_combo_medsite_terrs_activated(int index) {
    refresh_terrs_tab();
}

void assignment_wnd::on_combo_age_terrs_activated(int index) {
    refresh_terrs_tab();
}

void assignment_wnd::on_combo_sex_terrs_activated(int index) {
    refresh_terrs_tab();
}

void assignment_wnd::on_bn_delete_medter4snils_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }

    // получим СНИЛС удаляемого врача
    QString str = QInputDialog::getText(this, "", "Введите СНИЛС удаляемого врача в формате ХХХХХХХХХХХ\n (пустая строка - отмена операции)",
                                        QLineEdit::Normal,"", NULL, Qt::Popup, Qt::ImhDigitsOnly);
    QString snils_mt = QString("00000000000") + str;
    snils_mt = snils_mt.right(11);
    if (snils_mt=="00000000000")
        return;


    // получим данные этого врача
    QString sql_test = "select sm.id, sm.code_mo, sm.code_mt, sm.fio_mt, sm.snils_mt, sm.spec_mt, ss.text as spec_text"
                       "  from spr_medters sm "
                       "       left join spr_medters_spec ss on(ss.code=sm.spec_mt) "
                       " where sm.snils_mt='" + snils_mt + "' ; ";
    QSqlQuery *query_test = new QSqlQuery(db);
    if (!mySQL.exec(this, sql_test, "Поиск врача по СНИЛС", *query_test, true, db, data_app)) {
        QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке найти врача по СНИЛС произошла неожиданная ошибка.\n\nУдаление отменено.");
        delete query_test;
        return;
    }

    int id_mt = -1;
    QString code_mo = "XXXXXX";
    QString code_mt = "XXXXXX";
    QString fio_mt  = "XXXXXX";
    QString spec_mt = "XXXXXX";
    if (query_test->size()<1) {
        QMessageBox::critical(this, "Ничего не найдено", "Не удалось найти врача по СНИЛС  " + snils_mt + " .\n\nУдаление отменено.");
        delete query_test;
        //return;
    } else {
        query_test->next();
        id_mt = query_test->value(0).toInt();
        code_mo = query_test->value(1).toString();
        code_mt = query_test->value(2).toString();
        fio_mt  = query_test->value(3).toString();
        spec_mt = query_test->value(6).toString();
        delete query_test;
    }



    // получим число участков этого врача
    QString sql_test2 = "select count(*) "
                       "  from spr_ms_r_mt "
                       " where id_mt='" + QString::number(id_mt) + "' ; ";
    QSqlQuery *query_test2 = new QSqlQuery(db);
    if (!mySQL.exec(this, sql_test2, "Подсчёт участков врача по СНИЛС", *query_test2, true, db, data_app)) {
        QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке сосчитать участки врача по СНИЛС произошла неожиданная ошибка.\n\nУдаление отменено.");
        delete query_test2;
        return;
    }
    query_test2->next();
    int count_ms_r_mt = query_test2->value(0).toInt();
    delete query_test2;



    // получим число адресов этого врача
    QString sql_test3 = "select count(*) "
                        "  from assig_by_terr "
                        " where snils_mt='" + snils_mt + "' ; ";
    QSqlQuery *query_test3 = new QSqlQuery(db);
    if (!mySQL.exec(this, sql_test3, "Подсчёт адресов врача по СНИЛС", *query_test3, true, db, data_app)) {
        QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке сосчитать адреса врача по СНИЛС произошла неожиданная ошибка.\n\nУдаление отменено.");
        delete query_test3;
        return;
    }
    query_test3->next();
    int count_addr_r_mt = query_test3->value(0).toInt();
    delete query_test3;



    // получим число пациентов этого врача
    QString sql_test4 = "select count(*) "
                        "  from persons_assig "
                        " where snils_mt='" + snils_mt + "' "
                        "   and is_actual=0 ; ";
    QSqlQuery *query_test4 = new QSqlQuery(db);
    if (!mySQL.exec(this, sql_test4, "Подсчёт пациентов врача по СНИЛС", *query_test4, true, db, data_app)) {
        QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке сосчитать пациентов врача по СНИЛС произошла неожиданная ошибка.\n\nУдаление отменено.");
        delete query_test4;
        return;
    }
    query_test4->next();
    int count_pers_r_mt = query_test4->value(0).toInt();
    delete query_test4;



    if (QMessageBox::question(this, "Нужно подтверждение",
                              "Вы действительно хотите улюалить запись врача?\n" +
                              fio_mt + ",  СНИЛС: " + snils_mt + ",  МО: " + code_mo + "\n\n"
                              "Врачу сопоставлено:\n" +
                              QString::number(count_ms_r_mt) + " участков\n" +
                              QString::number(count_addr_r_mt) + " адресов\n" +
                              QString::number(count_pers_r_mt) + " пациентов\n",
                              QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
        return;
    }

    db.transaction();
    QString sql = "";

    // открепление персоны от этого врача
    sql += "update persons_assig "
           "   set unsig_date=current_date "
           " where snils_mt='" + snils_mt + "' "
           "   and is_actual=0 ; ";
    sql += "update persons_assig "
           "   set is_actual=is_actual +1 "
           " where snils_mt='" + snils_mt + "' ; ";
    /*sql += "update persons_assig "
           "   set snils_mt=NULL "
           " where snils_mt='" + snils_mt + "' ; ";*/

    sql += "update assig_by_terr "
           "   set snils_mt=NULL "
           " where snils_mt='" + snils_mt + "' ; ";

    sql += "delete from spr_ms_r_mt "
           " where id_mt=" + QString::number(id_mt) + " ; ";

    sql += "delete from spr_medters "
           " where id=" + QString::number(id_mt) + " ; ";

    QSqlQuery *query2 = new QSqlQuery(db);
    if (!mySQL.exec(this, sql, "Удаление врача", *query2, true, db, data_app)) {
        db.rollback();
        QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке удалить запись врача произошла неожиданная ошибка.\n\nУдаление отменено.");
        delete query2;
        return;
    }
    delete query2;

    db.commit();

    refresh_medters_tab();
}

void assignment_wnd::on_act_mt4snils_triggered() {
    // запрпосим список СНИЛС врачей
    s_snils_mt_data data_snils;
    data_snils.snils_list = "";
    delete snils_list_w;
    snils_list_w = new snils_list_wnd(data_snils, this);
    if (!snils_list_w->exec())
        return;

    // выберем данные людей, которых нет в полученном файле
    QString sql_data = "select distinct "
                       "       m.code_mo, m.name_mo, m.phone, t.snils_mt, t.fio_mt, /*t.code_mt, t.spec_mt,*/ ss.text as spec_mt_text "
                       "       #MS_FIELDS# #ADDR_FIELDS# "
                       "  from spr_medters t "
                       "       join spr_medorgs m on(m.code_mo=t.code_mo and m.is_head=1) "
                       "       join spr_medters_spec ss on(ss.code=t.spec_mt) "
                       "       left join persons_assig a on(a.snils_mt=t.snils_mt and a.assig_type in(0,1)) "
                       "       join persons e on(e.id=a.id_person) "
                       "       #ADDR_TABLE# "
                       " where t.snils_mt in(#SNILS_LIST#) "
                       " order by m.code_mo, "
                       "       m.name_mo, t.fio_mt "
                       "       #ADDR_ORDER# ; ";
    sql_data = sql_data.replace("#SNILS_LIST#", data_snils.snils_list.replace("\n", " "));
    sql_data = sql_data.replace("#MS_FIELDS#",  ((data_snils.f_ms) ? ", a.code_mp, a.code_ms" : ""));
    sql_data = sql_data.replace("#ADDR_FIELDS#",((data_snils.f_address) ? ", adr.dstr_name||', '||adr.nasp_name||', '||adr.strt_name||', '||adr.house||', '||adr.corp||', '||adr.quart as address" : ""));
    sql_data = sql_data.replace("#ADDR_ORDER#", ((data_snils.f_address) ? ", adr.dstr_name||', '||adr.nasp_name||', '||adr.strt_name||', '||adr.house||', '||adr.corp||', '||adr.quart" : ""));
    sql_data = sql_data.replace("#ADDR_TABLE#", ((data_snils.f_address) ? "join addresses adr on(adr.id=e.id_addr_reg)" : ""));

    delete show_tab_w;
    this->setCursor(Qt::ArrowCursor);

    show_tab_w = new show_tab_wnd("Сверка списка закстрахованных с ТФОМС", sql_data, db, data_app, this);
    show_tab_w->exec();
}

void assignment_wnd::on_act_matchTFOMS_triggered() {
    delete  matchTFOMS_w;
    matchTFOMS_w = new matchTFOMS_wnd(db, data_app, settings, this);
    matchTFOMS_w->show();
}

void assignment_wnd::on_ch_show_names_clicked(bool checked) {
    if ( checked
         /*&& QMessageBox::question(this, "Нукжно подтверждение",
                                  "Включить отображение названий адресных элементов?\n\n"
                                  "Это сильно замедлит отображение таблицы.",
                                  QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Yes*/ ) {
        refresh_terrs_tab();
        return;
    } else {
        //ui->ch_show_names->setChecked(false);
        refresh_terrs_tab();
    }
}


void assignment_wnd::on_act_map_triggered() {
    delete geodata_w;
    geodata_w = new geodata_wnd(this);
    geodata_w->exec();
}

void assignment_wnd::on_rb_sort_mt4fio_clicked(bool checked) {
    refresh_medters_tab();
}

void assignment_wnd::on_rb_sort_mt4snils_clicked(bool checked) {
    refresh_medters_tab();
}

void assignment_wnd::on_rb_sort_ms4codes_clicked(bool checked) {
    refresh_medsites_tab();
}

void assignment_wnd::on_rb_sort_ms4code_clicked(bool checked) {
    refresh_medsites_tab();
}

void assignment_wnd::on_rb_sort_ms4name_clicked(bool checked) {
    refresh_medsites_tab();
}

void assignment_wnd::on_bn_strt_filter_clear_clicked() {
    ui->line_strt_filter->clear();
    refresh_strt_terr();
}

void assignment_wnd::on_line_strt_filter_editingFinished() {
    refresh_strt_terr();
}

void assignment_wnd::on_combo_strt_terrs_activated(const QString &arg1) {
    //ui->line_strt_filter->clear();
}

void assignment_wnd::on_act_Voronej_addPolises_triggered() {
    delete old_db_w;
    old_db_w = new old_db_wnd(db, db_FIAS, data_app, settings, this);

    old_db_w->exec();
}

void assignment_wnd::on_act_Voronej_reindex_triggered() {
    delete reindex_w;
    reindex_w = new reindex_wnd(db, data_app, settings, this);

    reindex_w->exec();
}

void assignment_wnd::on_act_load_mo_data_triggered() {
m_im:
    // надо выбрать файл, полученный от ТФОМС
    QString fullname = QFileDialog::getOpenFileName(this, "Выберите полученный от МО DBF-файл с данными заявлений на прикрепление",
                                                    data_app.path_in, "DBF-files (*.dbf)"/*, QFileDialog::ReadOnly*/);
    QFileInfo fi(fullname);
    QString filename = fi.fileName();
    QString basename = fi.baseName();
    QString filepath = fi.filePath();

    if (basename.isEmpty())  return;

    // проверка правильности имени файла
    QString S1 = basename.left(2).toUpper();
    QString code_mo = basename.left(8).right(6);
    QString S2 = code_mo.left(2);
    QString S3 = basename.left(10).right(2);
    QString S4 = basename.left(12).right(2);
    if (S1!="ZP") {
        if (QMessageBox::question(this, "Требуется подтверждение",
                                  "Имя выбранного DBF-файла не совпадает со стандартным именем файла имён: \"ZPccccccYYMM.dbf\"\n\nВы действительно хотите загрузить данные из этого файла как информацию о прикреплении застрахованных к медицинской организациии по личному заявлению?",
                                  QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::No)
            goto m_im;
    }
    if (S2!="31") {
        QMessageBox::question(this, "Неправильный реестровый номер МО!",
                              "Полученный реестровый номер медицинской организации\n " + code_mo + "\nне является 6-значным реестровым номером медорганизации Белгородской области!\n"
                              "Обработка данных невозможна.\n\n"
                              "Действие отменено.");
        return;
    }
    QString fullname_new = data_app.path_dbf + S1 + code_mo + ".DBF";
    if (fullname!=fullname_new) {
        // если есть, удалим старую копию файла
        if (QFile::exists(fullname_new)) {
            if (!QFile::remove(fullname_new)) {
                QMessageBox::warning(this, "Ошибка копирования файла",
                                     "При попытке скопировать выбранный DBF-файл в папку источника данных DBF произошла неожиданная ошибка.\n\n"
                                     "Операция отменена.");
                return;
            }
        }
        // файл надо скопировать в папку источника DBF
        if (!QFile::copy(fullname, fullname_new)) {
            QMessageBox::warning(this, "Ошибка копирования файла",
                                 "При попытке скопировать выбранный DBF-файл в папку источника данных DBF произошла неожиданная ошибка.\n\n"
                                 "Операция отменена.");
            return;
        }
    }


    // подключимся к ODBC
    db_ODBC = QSqlDatabase::addDatabase("QODBC", "INKO_DBF");
    db_ODBC.setConnectOptions();
    // имя источника данных
    db_ODBC.setDatabaseName("INKO_DBF");
    while (!db_ODBC.open()) {
        if (QMessageBox::warning(this, "Не найден ODBC-источник данных INKO_DBF",
                                 QString("Источник данных должен использовать папку, указанную в параметре программы \"path_dbf\":\n"
                                 "При попытке подключиться к источнику данных ODBC \"INKO_DBF\" операционная система вернула отказ. \n"
                                 "Этот источник данных необходим для генерации и чтения dbf-файлов обмена с ТФОМС и МО. \n"
                                 "Если такого источника данных нет - создайте его.\n\n")
                                 + data_app.path_dbf + "\n\n"
                                 "Попробовать снова?", QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)==QMessageBox::No) {
            return;
        }
    }

    // выберем все данные
    QString sql_mo = "select fam, im, ot, dr, spolic, npolic, rec_date, uch_id "
                     "  from " + S1 + code_mo + " ";
    //QString sql_mo = "select * "
    //                 "  from " + S1 + code_mo + " ";
    QSqlQuery *query_mo = new QSqlQuery(db_ODBC);
    bool res_mo = query_mo->exec(sql_mo);
    QString err = db_ODBC.lastError().databaseText();
    QString er2 = db_ODBC.lastError().driverText();
    if (!res_mo) {
        delete query_mo;
        QMessageBox::warning(this, "Ошибка чтения данных МО ",
                             "При попытке прочитать данные МО произошла ошибка!\n\n"
                             + sql_mo + "\n\n"
                             "Операция отменена");
        db_ODBC.close();
        QSqlDatabase::removeDatabase("INKO_DBF");
        return;
    }

    // зальём новые данные
    db.transaction();

    int n = 0;

    QSqlRecord rec = query_mo->record();
    while (query_mo->next()) {
        // поля
        QString fam    = query_mo->value(0).toString().trimmed().toUpper();
        QString im     = query_mo->value(1).toString().trimmed().toUpper();
        QString ot     = query_mo->value(2).toString().trimmed().toUpper();
        QDate   dr     = query_mo->value(3).toDate();
        QString spolic = query_mo->value(4).toString().trimmed().toUpper();
        QString npolic = query_mo->value(5).toString().trimmed().toUpper();
        QDate   rec_dt = query_mo->value(6).toDate();
        QString uch_ID = query_mo->value(7).toString();
        QString enp    = (spolic.length()==16 ? spolic : (npolic.length()==16 ? npolic : "") );

        QString sql_ins = "select add_or_find_assig ( "
                          "  cast('" + fam + "' as character varying), "
                          "  cast('" + im  + "' as character varying), "
                          "  cast('" + ot  + "' as character varying), "
                          "  cast('" + dr.toString("yyyy-MM-dd") + "' as date), "
                          "  cast(NULL as smallint), "
                          "  cast(NULL as character varying), "
                          "  cast('" + enp + "' as character varying), "
                          "  cast(NULL as smallint), "
                          "  cast(NULL as character varying), "
                          "  cast(NULL as character varying), "
                          "  cast(NULL as character varying), "
                          "  cast('" + code_mo + "' as character varying), "
                          "  cast(NULL as character varying), "
                          "  cast('0' as character varying), "
                          "  cast('" + uch_ID + "' as character varying), "
                          "  cast(NULL as character varying), "
                          "  cast(NULL as integer), "
                          "  cast('" + rec_dt.toString("yyyy-MM-dd") + "' as date), "
                          "  cast(2 as smallint), "
                          "  cast(NULL as date), "
                          "  'МО - файл новых прикреплений', "
                          "  cast(" + ("'" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "'") + " as timestamp), "
                          "  cast(" + ("'" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "'") + " as timestamp)  "
                          " ); ";
        // добавим сроку
        QSqlQuery *query_ins = new QSqlQuery(db);
        bool res_ins = query_ins->exec(sql_ins);
        QString err = db.lastError().databaseText();
        if (!res_ins) {
            delete query_ins;
            QMessageBox::warning(this, "Ошибка добавления данных от МО",
                                 "При попытке добавить строку данных от МО произошла ошибка!\n\n"
                                 + sql_ins + "\n\n"
                                 "Операция отменена");
            db_ODBC.close();
            QSqlDatabase::removeDatabase("INKO_DBF");
            return;
        }
        n++;

        delete query_ins;
    }
    delete query_mo;
    QMessageBox::warning(this, "Готово",
                         "Загружено " + QString::number(n) + " новых записей.");

    db.commit();
}

void assignment_wnd::on_spin_tab_limit_editingFinished() {
    refresh_persons_tab();
}

void assignment_wnd::on_ch_tab_limit_clicked(bool checked) {
    ui->spin_tab_limit->setEnabled(checked);
    // если фильтр есть - обновим таблицу
    if (checked)  refresh_persons_tab();
}

void assignment_wnd::on_ch_has_address_reg_clicked(bool checked) {
    refresh_persons_tab();
}
void assignment_wnd::on_tab_persons_clicked(const QModelIndex &index) {
    refresh_assigs_tab();
    refresh_polises_tab();
    refresh_addresses_tab();

    data_pers.id         = model_persons.data(model_persons.index(index.row(),  0), Qt::EditRole).toInt();
    data_pers.pol_v      = model_persons.data(model_persons.index(index.row(),  1), Qt::EditRole).toInt();
    data_pers.fam        = model_persons.data(model_persons.index(index.row(),  2), Qt::EditRole).toString();
    data_pers.im         = model_persons.data(model_persons.index(index.row(),  3), Qt::EditRole).toString();
    data_pers.ot         = model_persons.data(model_persons.index(index.row(),  4), Qt::EditRole).toString();
    data_pers.sex        = model_persons.data(model_persons.index(index.row(),  5), Qt::EditRole).toInt();
    data_pers.date_birth = model_persons.data(model_persons.index(index.row(),  6), Qt::EditRole).toDate();
    data_pers.date_death = model_persons.data(model_persons.index(index.row(),  7), Qt::EditRole).toDate();
    data_pers.snils      = model_persons.data(model_persons.index(index.row(),  9), Qt::EditRole).toString();
    data_pers.enp        = model_persons.data(model_persons.index(index.row(),  9), Qt::EditRole).toString();
    data_pers.date_reg   = model_persons.data(model_persons.index(index.row(), 10), Qt::EditRole).toDate();
    data_pers.bomj       = model_persons.data(model_persons.index(index.row(), 11), Qt::EditRole).toBool();
    data_pers.status     = model_persons.data(model_persons.index(index.row(), 12), Qt::EditRole).toInt();

    data_pers.date_insure_begin      = model_persons.data(model_persons.index(index.row(), 13), Qt::EditRole).toDate();
    data_pers.has_date_insure_begin  = model_persons.data(model_persons.index(index.row(), 13), Qt::EditRole).toDate().isValid();
    ui->ch_insure_begin->setChecked ( data_pers.has_date_insure_begin );
    ui->date_insure_begin->setDate  ( data_pers.has_date_insure_begin ? data_pers.date_insure_begin : QDate(1900,1,1) );
    ui->date_insure_begin->setVisible(data_pers.has_date_insure_begin);

    data_pers.date_insure_end      = model_persons.data(model_persons.index(index.row(), 14), Qt::EditRole).toDate();
    data_pers.has_date_insure_end  = model_persons.data(model_persons.index(index.row(), 14), Qt::EditRole).toDate().isValid();
    ui->ch_insure_end->setChecked ( data_pers.has_date_insure_end );
    ui->date_insure_end->setDate  ( data_pers.has_date_insure_end ? data_pers.date_insure_end : QDate(1900,1,1) );
    ui->date_insure_end->setVisible(data_pers.has_date_insure_end);

    data_pers.id_udl  = model_persons.data(model_persons.index(index.row(), 15), Qt::EditRole).toInt();
    data_pers.id_drp  = model_persons.data(model_persons.index(index.row(), 16), Qt::EditRole).toInt();
    data_pers.id_pol  = model_persons.data(model_persons.index(index.row(), 17), Qt::EditRole).toInt();
    data_pers.id_asg  = model_persons.data(model_persons.index(index.row(), 18), Qt::EditRole).toInt();

    ui->ch_insure_begin->setEnabled(false);
    ui->ch_insure_end->setEnabled(false);
    ui->date_insure_begin->setEnabled(false);
    ui->date_insure_end->setEnabled(false);

    ui->lab_pers_status->setEnabled(true);
    ui->ch_pers_status->setEnabled(true);
    ui->ch_pers_status->setChecked(false);
    QApplication::processEvents();

    ui->combo_pers_status->setCurrentIndex(ui->combo_pers_status->findData(data_pers.status));
    ui->combo_pers_status->setEnabled(false);
}




void assignment_wnd::refresh_combo_assig_source() {
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
    ui->combo_pers_status->setCurrentIndex(-1);
    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::refresh_assigs_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    ui->pan_assig->setEnabled(false);

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_persons->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        if (ui->combo_assig_source->currentData()==-1) {
            // для полной таблицы
            this->setCursor(Qt::WaitCursor);
            QModelIndex index = indexes.front();

            int id_person = model_persons.data(model_persons.index(index.row(), 0), Qt::EditRole).toInt();       // e.id

            sql_assig_tab = "select a.id_person, a.id, a.assig_type, "
                            "       case when (a.assig_type is NULL or a.assig_type<1) then '- нет прикрепления -' "
                            "            when (a.assig_type=1) then '(1) по терр.принципу' "
                            "            when (a.assig_type=2) then '(2) по личн.заявлению' "
                            "            else '('||a.assig_type||') ???' end as assig_type, "
                            "       a.assig_date, unsig_date, a.is_actual, "
                            "       a.code_mo, mo.name_mo, a.code_mp, a.code_ms, a.snils_mt, "
                            "       a.dt_ins, a.dt_upd, a.source "
                            "  from persons_assig a "
                            "  left join spr_medorgs mo on(mo.code_mo=a.code_mo) "
                            " where a.id_person=" + QString::number(id_person) + " ";

            if ( ui->ch_assig_type->isChecked()
                 || ui->rb_date_cur->isChecked()
                 || ui->rb_date_assig->isChecked() )  {

                if (ui->ch_assig_type->isChecked())
                    sql_assig_tab += " and a.assig_type=" + QString(ui->combo_assig_type->currentIndex()==0 ? "1" : "2") + " ";

                if (ui->rb_date_cur->isChecked())
                    sql_assig_tab += " and a.assig_date<=CURRENT_DATE "
                                     " and (a.unsig_date is NULL or a.unsig_date>CURRENT_DATE) ";

                if (ui->rb_date_assig->isChecked())
                    sql_assig_tab += " and a.assig_date<='" + ui->date_assig->date().toString("yyyy-MM-dd") + "' "
                                     " and (a.unsig_date is NULL or a.unsig_date>'" + ui->date_assig->date().toString("yyyy-MM-dd") + "') ";

                sql_assig_tab += " ";
            }

            sql_assig_tab += " order by is_actual, assig_date desc, unsig_date desc ";

            model_assig.setQuery(sql_assig_tab,db);
            QString err2 = model_assig.lastError().driverText();

            // подключаем модель из БД
            ui->tab_assig->setModel(&model_assig);

            // обновляем таблицу
            ui->tab_assig->reset();

            // задаём ширину колонок
            ui->tab_assig->setColumnWidth(  0,  2);   // a.id_person,
            ui->tab_assig->setColumnWidth(  1,  2);   // a.id,
            ui->tab_assig->setColumnWidth(  2,  2);   // a.assig_type,
            ui->tab_assig->setColumnWidth(  3,140);   // a.assig_type - text,
            ui->tab_assig->setColumnWidth(  4, 70);   // a.assig_date,
            ui->tab_assig->setColumnWidth(  5, 70);   // unsig_date,
            ui->tab_assig->setColumnWidth(  6, 40);   // a.is_actual,
            ui->tab_assig->setColumnWidth(  7, 45);   // a.code_mo,
            ui->tab_assig->setColumnWidth(  8,200);   // mo.name_mo,
            ui->tab_assig->setColumnWidth(  9, 80);   // a.code_mp,
            ui->tab_assig->setColumnWidth( 10, 80);   // a.code_ms,
            ui->tab_assig->setColumnWidth( 11, 80);   // a.snils_mt
            ui->tab_assig->setColumnWidth( 12,120);   // a.dt_ins
            ui->tab_assig->setColumnWidth( 13,  2);   // a.dt_upd
            ui->tab_assig->setColumnWidth( 14,500);   // a.source

            // правим заголовки
            model_assig.setHeaderData(  3, Qt::Horizontal, ("способ\nприкрепления"));
            model_assig.setHeaderData(  4, Qt::Horizontal, ("дата\nприкрепл."));
            model_assig.setHeaderData(  5, Qt::Horizontal, ("дата\nоткрепл."));
            model_assig.setHeaderData(  6, Qt::Horizontal, ("акт."));
            model_assig.setHeaderData(  7, Qt::Horizontal, ("код\nМО"));
            model_assig.setHeaderData(  8, Qt::Horizontal, ("название МО"));
            model_assig.setHeaderData(  9, Qt::Horizontal, ("подразде-\nление"));
            model_assig.setHeaderData( 10, Qt::Horizontal, ("участок"));
            model_assig.setHeaderData( 11, Qt::Horizontal, ("СНИЛС\nучаст.врача"));
            model_assig.setHeaderData( 12, Qt::Horizontal, ("дата/время\nполучения записи"));
            model_assig.setHeaderData( 13, Qt::Horizontal, ("дата/время\nправки записи"));
            model_assig.setHeaderData( 14, Qt::Horizontal, ("источник"));

        } else if (ui->combo_assig_source->currentData()==-2) {
            // для краткой таблицы
            this->setCursor(Qt::WaitCursor);
            QModelIndex index = indexes.front();

            int id_person = model_persons.data(model_persons.index(index.row(), 0), Qt::EditRole).toInt();       // e.id

            sql_assig_tab = "select a.id_person, a.id, a.assig_type, "
                            "       case when (a.assig_type is NULL or a.assig_type<1) then '- нет прикрепления -' "
                            "            when (a.assig_type=1) then '(1) по терр.принципу' "
                            "            when (a.assig_type=2) then '(2) по личн.заявлению' "
                            "            else '('||a.assig_type||') ???' end as assig_type, "
                            "       a.assig_date, unsig_date, a.is_actual, "
                            "       a.code_mo, mo.name_mo, "
                            "       a.dt_ins, a.dt_upd "
                            "  from persons_assig_mo a "
                            "  left join spr_medorgs mo on(mo.code_mo=a.code_mo) "
                            " where a.id_person=" + QString::number(id_person) + " ";

            if ( ui->ch_assig_type->isChecked()
                 || ui->rb_date_cur->isChecked()
                 || ui->rb_date_assig->isChecked() )  {

                if (ui->ch_assig_type->isChecked())
                    sql_assig_tab += " and a.assig_type=" + QString(ui->combo_assig_type->currentIndex()==0 ? "1" : "2") + " ";

                if (ui->rb_date_cur->isChecked())
                    sql_assig_tab += " and a.assig_date<=CURRENT_DATE "
                                     " and (a.unsig_date is NULL or a.unsig_date>CURRENT_DATE) ";

                if (ui->rb_date_assig->isChecked())
                    sql_assig_tab += " and a.assig_date<='" + ui->date_assig->date().toString("yyyy-MM-dd") + "' "
                                     " and (a.unsig_date is NULL or a.unsig_date>'" + ui->date_assig->date().toString("yyyy-MM-dd") + "') ";

                sql_assig_tab += " ";
            }

            sql_assig_tab += " order by is_actual, assig_date desc, unsig_date desc ";

            model_assig.setQuery(sql_assig_tab,db);
            QString err2 = model_assig.lastError().driverText();

            // подключаем модель из БД
            ui->tab_assig->setModel(&model_assig);

            // обновляем таблицу
            ui->tab_assig->reset();

            // задаём ширину колонок
            ui->tab_assig->setColumnWidth(  0,  2);   // a.id_person,
            ui->tab_assig->setColumnWidth(  1,  2);   // a.id,
            ui->tab_assig->setColumnWidth(  2,  2);   // a.assig_type,
            ui->tab_assig->setColumnWidth(  3,140);   // a.assig_type - text,
            ui->tab_assig->setColumnWidth(  4, 70);   // a.assig_date,
            ui->tab_assig->setColumnWidth(  5, 70);   // unsig_date,
            ui->tab_assig->setColumnWidth(  6, 40);   // a.is_actual,
            ui->tab_assig->setColumnWidth(  7, 45);   // a.code_mo,
            ui->tab_assig->setColumnWidth(  8,200);   // mo.name_mo,
            ui->tab_assig->setColumnWidth(  9,120);   // a.dt_ins
            ui->tab_assig->setColumnWidth( 10,  2);   // a.dt_upd

            // правим заголовки
            model_assig.setHeaderData(  3, Qt::Horizontal, ("способ\nприкрепления"));
            model_assig.setHeaderData(  4, Qt::Horizontal, ("дата\nприкрепл."));
            model_assig.setHeaderData(  5, Qt::Horizontal, ("дата\nоткрепл."));
            model_assig.setHeaderData(  6, Qt::Horizontal, ("акт."));
            model_assig.setHeaderData(  7, Qt::Horizontal, ("код\nМО"));
            model_assig.setHeaderData(  8, Qt::Horizontal, ("название МО"));
            model_assig.setHeaderData(  9, Qt::Horizontal, ("дата/время\nполучения записи"));
            model_assig.setHeaderData( 10, Qt::Horizontal, ("дата/время\nправки записи"));

        } else {
            // для слоёв данных
            this->setCursor(Qt::WaitCursor);
            QModelIndex index = indexes.front();

            int id_layer = ui->combo_assig_source->currentData().toInt();
            int id_person = model_persons.data(model_persons.index(index.row(), 0), Qt::EditRole).toInt();       // e.id

            sql_assig_tab = "select d.id_person, d.id, d.assig_type, "
                            "       case when (d.assig_type is NULL or d.assig_type<1) then '- нет прикрепления -' "
                            "            when (d.assig_type=1) then '(1) по терр.принципу' "
                            "            when (d.assig_type=2) then '(2) по личн.заявлению' "
                            "            else '('||d.assig_type||') ???' end as assig_type, "
                            "       d.assig_date, d.code_mo, mo.name_mo, d.code_mp, d.code_ms, d.snils_mt, "
                            "       d.dt_ins, d.dt_upd, '- снимок данных -' "
                            "  from assig.sexages_data d "
                            "  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                            " where d.id_layer=" + QString::number(id_layer) + ""
                            "   and d.id_person=" + QString::number(id_person) + " ";

            if ( ui->ch_assig_type->isChecked()
                 || ui->rb_date_cur->isChecked()
                 || ui->rb_date_assig->isChecked() )  {

                if (ui->ch_assig_type->isChecked())
                    sql_assig_tab += " and d.assig_type=" + QString(ui->combo_assig_type->currentIndex()==0 ? "1" : "2") + " ";

                if (ui->rb_date_cur->isChecked())
                    sql_assig_tab += " and d.assig_date<=CURRENT_DATE "
                                     " and (d.unsig_date is NULL or d.unsig_date>CURRENT_DATE) ";

                if (ui->rb_date_assig->isChecked())
                    sql_assig_tab += " and d.assig_date<='" + ui->date_assig->date().toString("yyyy-MM-dd") + "' "
                                     " and (d.unsig_date is NULL or d.unsig_date>'" + ui->date_assig->date().toString("yyyy-MM-dd") + "') ";

                sql_assig_tab += " ";
            }

            sql_assig_tab += " order by assig_date desc ";

            model_assig.setQuery(sql_assig_tab,db);
            QString err2 = model_assig.lastError().driverText();

            // подключаем модель из БД
            ui->tab_assig->setModel(&model_assig);

            // обновляем таблицу
            ui->tab_assig->reset();

            // задаём ширину колонок
            ui->tab_assig->setColumnWidth(  0,  2);   // d.id_person,
            ui->tab_assig->setColumnWidth(  1,  2);   // d.id,
            ui->tab_assig->setColumnWidth(  2,  2);   // d.assig_type,
            ui->tab_assig->setColumnWidth(  3,140);   // d.assig_type - text,
            ui->tab_assig->setColumnWidth(  4, 70);   // d.assig_date,
            ui->tab_assig->setColumnWidth(  5, 45);   // d.code_mo,
            ui->tab_assig->setColumnWidth(  6,200);   // mo.name_mo,
            ui->tab_assig->setColumnWidth(  7, 80);   // d.code_mp,
            ui->tab_assig->setColumnWidth(  8, 80);   // d.code_ms,
            ui->tab_assig->setColumnWidth(  9, 80);   // d.snils_mt
            ui->tab_assig->setColumnWidth( 10,120);   // d.dt_ins
            ui->tab_assig->setColumnWidth( 11,  2);   // d.dt_upd
            ui->tab_assig->setColumnWidth( 12,500);   // d.source

            // правим заголовки
            model_assig.setHeaderData(  3, Qt::Horizontal, ("способ\nприкрепления"));
            model_assig.setHeaderData(  4, Qt::Horizontal, ("дата\nприкрепл."));
            model_assig.setHeaderData(  5, Qt::Horizontal, ("код\nМО"));
            model_assig.setHeaderData(  6, Qt::Horizontal, ("название МО"));
            model_assig.setHeaderData(  7, Qt::Horizontal, ("подразде-\nление"));
            model_assig.setHeaderData(  8, Qt::Horizontal, ("участок"));
            model_assig.setHeaderData(  9, Qt::Horizontal, ("СНИЛС\nучаст.врача"));
            model_assig.setHeaderData( 10, Qt::Horizontal, ("дата/время\nполучения записи"));
            model_assig.setHeaderData( 11, Qt::Horizontal, ("дата/время\nправки записи"));
            model_assig.setHeaderData( 12, Qt::Horizontal, ("источник"));
        }

        ui->pan_assig->setEnabled(true);
        QApplication::processEvents();

    }

    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::on_combo_assig_source_activated(int index) {
    refresh_assigs_tab();
}




void assignment_wnd::refresh_polises_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    ui->pan_polises->setEnabled(false);
    QApplication::processEvents();

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_persons->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        this->setCursor(Qt::WaitCursor);
        QModelIndex index = indexes.front();

        int id_person = model_persons.data(model_persons.index(index.row(), 0), Qt::EditRole).toInt();       // e.id

        sql_polises = "select p.id, p.ocato, "
                      "       case p.pol_v when 1 then ' СТАР.' when 2 then ' ВС ' when 3 then ' ЕНП ' else '-???-' end as pol_v, "
                      "       case p.f_polis when 1 then 'бумага' when 2 then 'пластик' else '-???-' end as f_polis, "
                      "       p.vs_num, p.pol_ser, p.pol_num, p.enp, p.uec_num, "
                      "       p.date_begin, p.date_end, p.date_stop, p.date_activate, p.date_get2hand, p.tfoms_id, p.tfoms_date "
                      "  from polises p "
                      " where p.id_person=" + QString::number(id_person) + " "
                      " order by p.date_begin DESC ; ";

        model_polises.setQuery(sql_polises,db);
        QString err2 = model_polises.lastError().driverText();

        // подключаем модель из БД
        ui->tab_polises->setModel(&model_polises);

        // обновляем таблицу
        ui->tab_polises->reset();

        // задаём ширину колонок
        ui->tab_polises->setColumnWidth(  0,  2);   // p.id,
        ui->tab_polises->setColumnWidth(  1, 50);   // p.ocato,
        ui->tab_polises->setColumnWidth(  2, 55);   // case p.pol_v when 1 then ' СТАР.' when 2 then ' ВС ' when 3 then ' ЕНП ' else '-???-' end as pol_v,
        ui->tab_polises->setColumnWidth(  3, 55);   // case p.f_polis when 1 then 'бумага' when 2 then 'пластик' else '-???-' end as f_polis,
        ui->tab_polises->setColumnWidth(  4, 90);   // p.vs_num,
        ui->tab_polises->setColumnWidth(  5, 50);   // p.pol_ser,
        ui->tab_polises->setColumnWidth(  6, 70);   // p.pol_num,
        ui->tab_polises->setColumnWidth(  7,110);   // p.enp,
        ui->tab_polises->setColumnWidth(  8,150);   // p.uec_num,
        ui->tab_polises->setColumnWidth(  9, 80);   // p.date_begin,
        ui->tab_polises->setColumnWidth( 10, 80);   // p.date_end,
        ui->tab_polises->setColumnWidth( 11, 80);   // p.date_stop,
        ui->tab_polises->setColumnWidth( 12, 80);   // p.date_activate,
        ui->tab_polises->setColumnWidth( 13, 80);   // p.date_get2hand,
        ui->tab_polises->setColumnWidth( 14,100);   // p.tfoms_id,
        ui->tab_polises->setColumnWidth( 15,100);   // p.tfoms_date

        // правим заголовки
        model_polises.setHeaderData(  0, Qt::Horizontal, ("ID"));
        model_polises.setHeaderData(  1, Qt::Horizontal, ("ОКАТО"));
        model_polises.setHeaderData(  2, Qt::Horizontal, ("тип\nполиса"));
        model_polises.setHeaderData(  3, Qt::Horizontal, ("форма\nполиса"));
        model_polises.setHeaderData(  4, Qt::Horizontal, ("№ ВС"));
        model_polises.setHeaderData(  5, Qt::Horizontal, ("серия\nполиса"));
        model_polises.setHeaderData(  6, Qt::Horizontal, ("номер\nполиса"));
        model_polises.setHeaderData(  7, Qt::Horizontal, ("ЕНП"));
        model_polises.setHeaderData(  8, Qt::Horizontal, ("№ УЭК"));
        model_polises.setHeaderData(  9, Qt::Horizontal, ("дата\nначала"));
        model_polises.setHeaderData( 10, Qt::Horizontal, ("срок\nдействия"));
        model_polises.setHeaderData( 11, Qt::Horizontal, ("дата\nизъятия"));
        model_polises.setHeaderData( 12, Qt::Horizontal, ("дата\nактивации"));
        model_polises.setHeaderData( 13, Qt::Horizontal, ("выдан\nна руки"));
        model_polises.setHeaderData( 14, Qt::Horizontal, ("ID ТФОМС"));
        model_polises.setHeaderData( 15, Qt::Horizontal, ("дата ТФОМС"));

        ui->pan_polises->setEnabled(true);
        QApplication::processEvents();
    }

    this->setCursor(Qt::ArrowCursor);
}



void assignment_wnd::refresh_addresses_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    ui->pan_addresses->setEnabled(false);
    QApplication::processEvents();

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_persons->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        this->setCursor(Qt::WaitCursor);
        QModelIndex index = indexes.front();

        int id_person = model_persons.data(model_persons.index(index.row(), 0), Qt::EditRole).toInt();       // e.id

        sql_addresses = "select * "
                        "  from "
                        "  ( "
                        "    ( select e.id as id_pers, 1 as code, 'Адрес регистрации' as text, "
                        "             reg.id, reg.ocato, reg.subj, reg.subj_name, reg.dstr, reg.dstr_name, reg.city, reg.city_name, reg.nasp, reg.nasp_name, reg.strt, reg.strt_name, reg.house, reg.corp, reg.quart, reg.kladr, reg.kladr_street, "
                        "             reg.fias_aoid, reg.fias_houseid "
                        "        from persons e"
                        "        left join addresses reg on(e.id_addr_reg=reg.id) "
                        "       where e.id=" + QString::number(id_person) + " "
                        "    ) "
                        "  UNION "
                        "    ( select e.id as id_pers, 2 as code, 'Адрес проживания' as text, "
                        "             liv.id, liv.ocato, liv.subj, liv.subj_name, liv.dstr, liv.dstr_name, liv.city, liv.city_name, liv.nasp, liv.nasp_name, liv.strt, liv.strt_name, liv.house, liv.corp, liv.quart, liv.kladr, liv.kladr_street, "
                        "             liv.fias_aoid, liv.fias_houseid "
                        "        from persons e"
                        "        left join addresses liv on(e.id_addr_liv=liv.id) "
                        "       where e.id=" + QString::number(id_person) + " "
                        "    ) "
                        "  ) q "
                        " order by q.code ; ";

        model_addresses.setQuery(sql_addresses,db);
        QString err2 = model_addresses.lastError().driverText();

        // подключаем модель из БД
        ui->tab_addresses->setModel(&model_addresses);

        // обновляем таблицу
        ui->tab_addresses->reset();

        // задаём ширину колонок
        ui->tab_addresses->setColumnWidth(  0,  2);   // id,
        ui->tab_addresses->setColumnWidth(  1,  2);   // code,
        ui->tab_addresses->setColumnWidth(  2,130);   // text,
        ui->tab_addresses->setColumnWidth(  3,  2);   // id,
        ui->tab_addresses->setColumnWidth(  4, 80);   // ocato,
        ui->tab_addresses->setColumnWidth(  5,  2);   // subj,
        ui->tab_addresses->setColumnWidth(  6,180);   // subj_name,
        ui->tab_addresses->setColumnWidth(  7,  2);   // dstr,
        ui->tab_addresses->setColumnWidth(  8,150);   // dstr_name,
        ui->tab_addresses->setColumnWidth(  9,  2);   // city,
        ui->tab_addresses->setColumnWidth( 10,120);   // city_name,
        ui->tab_addresses->setColumnWidth( 11,  2);   // nasp,
        ui->tab_addresses->setColumnWidth( 12,120);   // nasp_name,
        ui->tab_addresses->setColumnWidth( 13,  2);   // strt,
        ui->tab_addresses->setColumnWidth( 14,150);   // strt_name,
        ui->tab_addresses->setColumnWidth( 15, 50);   // house,
        ui->tab_addresses->setColumnWidth( 16, 50);   // corp,
        ui->tab_addresses->setColumnWidth( 17, 50);   // quart,
        ui->tab_addresses->setColumnWidth( 18,100);   // kladr,
        ui->tab_addresses->setColumnWidth( 19,130);   // kladr_street
        ui->tab_addresses->setColumnWidth( 20,130);   // fias_aoid
        ui->tab_addresses->setColumnWidth( 21,130);   // fias_houseid

        // правим заголовки
        model_addresses.setHeaderData(  1, Qt::Horizontal, ("адрес"));
        model_addresses.setHeaderData(  4, Qt::Horizontal, ("ОКАТО"));
        model_addresses.setHeaderData(  6, Qt::Horizontal, ("регион"));
        model_addresses.setHeaderData(  8, Qt::Horizontal, ("район"));
        model_addresses.setHeaderData( 10, Qt::Horizontal, ("город"));
        model_addresses.setHeaderData( 12, Qt::Horizontal, ("нас. пункт"));
        model_addresses.setHeaderData( 14, Qt::Horizontal, ("улица"));
        model_addresses.setHeaderData( 15, Qt::Horizontal, ("дом"));
        model_addresses.setHeaderData( 16, Qt::Horizontal, ("корпус"));
        model_addresses.setHeaderData( 17, Qt::Horizontal, ("кварт."));
        model_addresses.setHeaderData( 18, Qt::Horizontal, ("КЛАДР"));
        model_addresses.setHeaderData( 19, Qt::Horizontal, ("КЛАДР СТРИТ"));
        model_addresses.setHeaderData( 20, Qt::Horizontal, ("ФИАС aoId"));
        model_addresses.setHeaderData( 21, Qt::Horizontal, ("ФИАС houseId"));

        ui->pan_addresses->setEnabled(true);
        QApplication::processEvents();
    }

    this->setCursor(Qt::ArrowCursor);
}


void assignment_wnd::on_bn_today_clicked() {
    ui->date_assig->setDate(QDate::currentDate());
    refresh_assigs_tab();
}
void assignment_wnd::on_date_assig_editingFinished() {
    ui->rb_date_assig->setChecked(true);
    refresh_assigs_tab();
}
void assignment_wnd::on_rb_date_assig_clicked(bool checked) {
    refresh_assigs_tab();
}
void assignment_wnd::on_rb_date_cur_clicked(bool checked) {
    refresh_assigs_tab();
}
void assignment_wnd::on_rb_date_all_clicked() {
    refresh_assigs_tab();
}

void assignment_wnd::on_ch_show_assig_clicked(bool checked) {
    if (!checked)
        ui->split_assig->setSizes(l10);
    else {
        ui->split_assig->setSizes(l11);
        ui->split_assig->setStretchFactor(0,1);
        ui->split_assig->setStretchFactor(1,1);
        refresh_assigs_tab();
    }
}

void assignment_wnd::on_ch_show_addresses_clicked(bool checked) {
    if ( !ui->ch_show_polises->isChecked()
      && !ui->ch_show_addresses->isChecked() ) {
        ui->split_addresses->setSizes(l500);
        ui->split_addresses->setStretchFactor(0,5);
        ui->split_addresses->setStretchFactor(1,0);
        ui->split_addresses->setStretchFactor(2,0);
    } else
    if ( ui->ch_show_polises->isChecked()
      && !ui->ch_show_addresses->isChecked() ) {
        ui->split_addresses->setSizes(l510);
        ui->split_addresses->setStretchFactor(0,5);
        ui->split_addresses->setStretchFactor(1,1);
        ui->split_addresses->setStretchFactor(2,0);
        refresh_polises_tab();
    } else
    if ( !ui->ch_show_polises->isChecked()
      && ui->ch_show_addresses->isChecked() ) {
        ui->split_addresses->setSizes(l501);
        ui->split_addresses->setStretchFactor(0,5);
        ui->split_addresses->setStretchFactor(1,0);
        ui->split_addresses->setStretchFactor(2,1);
        refresh_addresses_tab();
    } else
    if ( ui->ch_show_polises->isChecked()
      && ui->ch_show_addresses->isChecked() ) {
        ui->split_addresses->setSizes(l511);
        ui->split_addresses->setStretchFactor(0,5);
        ui->split_addresses->setStretchFactor(1,1);
        ui->split_addresses->setStretchFactor(2,1);
        refresh_polises_tab();
        refresh_addresses_tab();
    }
}

void assignment_wnd::on_ch_show_polises_clicked(bool checked) {
    if ( !ui->ch_show_polises->isChecked()
      && !ui->ch_show_addresses->isChecked() ) {
        ui->split_addresses->setSizes(l500);
        ui->split_addresses->setStretchFactor(0,5);
        ui->split_addresses->setStretchFactor(1,0);
        ui->split_addresses->setStretchFactor(2,0);
    } else
    if ( ui->ch_show_polises->isChecked()
      && !ui->ch_show_addresses->isChecked() ) {
        ui->split_addresses->setSizes(l510);
        ui->split_addresses->setStretchFactor(0,5);
        ui->split_addresses->setStretchFactor(1,1);
        ui->split_addresses->setStretchFactor(2,0);
        refresh_polises_tab();
    } else
    if ( !ui->ch_show_polises->isChecked()
      && ui->ch_show_addresses->isChecked() ) {
        ui->split_addresses->setSizes(l501);
        ui->split_addresses->setStretchFactor(0,5);
        ui->split_addresses->setStretchFactor(1,0);
        ui->split_addresses->setStretchFactor(2,1);
        refresh_addresses_tab();
    } else
    if ( ui->ch_show_polises->isChecked()
      && ui->ch_show_addresses->isChecked() ) {
        ui->split_addresses->setSizes(l511);
        ui->split_addresses->setStretchFactor(0,5);
        ui->split_addresses->setStretchFactor(1,1);
        ui->split_addresses->setStretchFactor(2,1);
        refresh_polises_tab();
        refresh_addresses_tab();
    }
}


void assignment_wnd::on_ch_filter_polis_clicked(bool checked) {
    ui->ch_has_polis->setVisible(checked);
    ui->ch_no_polis->setVisible(checked);
    refresh_persons_tab();
}
void assignment_wnd::on_ch_filter_addrs_clicked(bool checked) {
    ui->ch_addr_reg->setVisible(checked);
    ui->ch_addr_liv->setVisible(checked);
    refresh_persons_tab();
}
void assignment_wnd::on_ch_filter_assig_clicked(bool checked) {
    ui->ch_has_assig->setVisible(checked);
    ui->ch_no_assig->setVisible(checked);
    refresh_persons_tab();
}


void assignment_wnd::on_ch_has_polis_clicked(bool checked) {
    ui->ch_no_polis->setChecked(!checked);
    refresh_persons_tab();
}
void assignment_wnd::on_ch_no_polis_clicked(bool checked) {
    ui->ch_has_polis->setChecked(!checked);
    refresh_persons_tab();
}
void assignment_wnd::on_ch_addr_reg_clicked(bool checked) {
    refresh_persons_tab();
}
void assignment_wnd::on_ch_addr_liv_clicked(bool checked) {
    refresh_persons_tab();
}
void assignment_wnd::on_ch_has_assig_clicked(bool checked) {
    ui->ch_no_assig->setChecked(!checked);
    refresh_persons_tab();
}
void assignment_wnd::on_ch_no_assig_clicked(bool checked) {
    ui->ch_has_assig->setChecked(!checked);
    refresh_persons_tab();
}

void assignment_wnd::on_act_to_acconnts_base_triggered() {
    delete accounts_base_w;
    accounts_base_w = new accounts_base_wnd(db, data_app, settings, this);

    accounts_base_w->exec();
}

void assignment_wnd::on_act_kladr_triggered() {

}

void assignment_wnd::on_ln_vs_num_textEdited(const QString &arg1) {
    QString txt0 = arg1.trimmed(), txt = "";
    for (int i=0; i<txt0.size(); ++i) {
        if (txt.length()>=11)
            break;
        QString c = txt0.mid(i, 1);
        if ( c=="0" || c=="1" || c=="2" || c=="3" || c=="4" || c=="5" || c=="6" || c=="7" || c=="8" || c=="9" ) {
            txt += c;
        }
    }
    ui->ln_vs_num->setText(txt);
    ui->ln_vs_num->setCursorPosition(txt.length());
}

void assignment_wnd::on_ln_enp_textEdited(const QString &arg1) {
    QString txt0 = arg1.trimmed(), txt = "";
    for (int i=0; i<txt0.size(); ++i) {
        if (txt.length()>=16)
            break;
        QString c = txt0.mid(i, 1);
        if ( c=="0" || c=="1" || c=="2" || c=="3" || c=="4" || c=="5" || c=="6" || c=="7" || c=="8" || c=="9" ) {
            txt += c;
        }
    }
    ui->ln_enp->setText(txt);
    ui->ln_enp->setCursorPosition(txt.length());
}

void assignment_wnd::on_ln_snils_textEdited(const QString &arg1) {
    QString txt0 = arg1.trimmed(), txt = "";
    for (int i=0; i<txt0.size(); ++i) {
        if (txt.length()>=11)
            break;
        QString c = txt0.mid(i, 1);
        if ( c=="0" || c=="1" || c=="2" || c=="3" || c=="4" || c=="5" || c=="6" || c=="7" || c=="8" || c=="9" ) {
            txt += c;
        }
    }
    ui->ln_snils->setText(txt);
    ui->ln_snils->setCursorPosition(txt.length());
}

void assignment_wnd::on_bn_autoassig_clicked() {
    // получим ID персоны
    QModelIndexList indexes = ui->tab_persons->selectionModel()->selection().indexes();

    int id_person = -1;
    int assig_type = -1;
    int id_assig = -1;

    if (indexes.size()>0) {
        this->setCursor(Qt::WaitCursor);
        QModelIndex index = indexes.front();

        id_person = model_persons.data(model_persons.index(index.row(), 0), Qt::EditRole).toInt();       // e.id
    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Сначала выберите запись персоны, для которой хотите сделать авто-прикрепление по территориальному принципу.");
        return;
    }

    // проверим, можно ли делать автоприкрепление
    QString sql_test = "select id_person, code_mo, code_mp, code_ms, snils_mt, assig_type, assig_way, assig_date, unsig_date, is_actual \n"
                       "  from persons_assig \n"
                       " where id_person=" + QString::number(id_person) + " \n"
                       "   and is_actual=0 \n"
                       "   and assig_date<=CURRENT_DATE \n"
                       "   and (unsig_date is NULL or unsig_date>CURRENT_DATE) ; ";
    QSqlQuery *query_test = new QSqlQuery(db);
    QString sql = sql_test.replace("\n","");
    bool res_test = query_test->exec(sql);
    if (!res_test) {
        this->setCursor(Qt::ArrowCursor);
        QString err_db = query_test->lastError().databaseText();
        QString err_drv = query_test->lastError().driverText();
        delete query_test;
        QMessageBox::warning(this, "Ошибка чтения данных",
                             "При попытке прочитать данные о действующем прикреплении персоны произошла ошибка.\n"
                             + sql_test + "\n\n"
                             + err_db   + "\n"
                             + err_drv  + "\n\n"
                             "Операция отменена.\n");
        return;
    }
    // проверим тип действующего прикрепления
    while (query_test->next())
        assig_type = max(assig_type, query_test->value(5).toInt());

    delete query_test;

    if (assig_type>1) {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Автоприкрепление невозможно",
                             "Выбранная персона уже прикреплена со способом прикрепления " + QString::number(assig_type) + ".\n"
                             "При этом автоматическое прикрепление не возможно.\n\n"
                             "Операция отменена.");
        return;
    }

    // собственно автоприкрепление по терр-принципу
    db.transaction();
    QString sql_assig = "select * from assig_proc(" + QString::number(id_person) + ") ; ";
    QSqlQuery *query_assig = new QSqlQuery(db);
    bool res_assig = mySQL.exec(this, sql_assig, "Автоприкрепление персоны по команде оператора", query_assig, false, db, data_app);

    if (!res_assig) {
        delete query_assig;
        db.rollback();
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка",
                             "При попытке автоматически прикрепить застрахованного по территориальному принципу произошла неожиданная ошибка.\n\n"
                             "Операция отменена.");
        return;
    }
    // проверим, удалось ли прикрепление
    while (query_assig->next())
        id_assig = query_assig->value(0).toInt();

    //обновим связку
    QString sql_link = "select * "
                       "  from update_pers_links_asg(" + QString::number(id_person) + ", NULL) ; ";
    QSqlQuery *query_link = new QSqlQuery(db);
    bool res_link = mySQL.exec(this, sql_link, "Автоприкрепление персоны по команде оператора", query_link, false, db, data_app);

    if (!res_link) {
        delete query_assig;
        delete query_link;
        db.rollback();
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка",
                             "При попытке автоматически прикрепить застрахованного по территориальному принципу произошла неожиданная ошибка.\n\n"
                             "Операция отменена.");
        return;
    }
    delete query_assig;
    delete query_link;

    db.commit();
    this->setCursor(Qt::ArrowCursor);

    if (id_assig==0)
        QMessageBox::warning(this, "Ничего не изменилось",
                             "Процедура автоматического прикрепления ничего не вернула.\n"
                             "Вероятно для адреса регистрации выбранной персоны нет подходящего правила территориального прикрепления.\n");
    if (id_assig>0)
        QMessageBox::warning(this, "Ничего не изменилось",
                             "Процедура автоматического прикрепления вернула ID уже существующей записи прикрепления.\n"
                             "Персона итак уже прикреплена согласно самому подходящему правилу.");
    refresh_persons_tab();
    refresh_assigs_tab();
}

void assignment_wnd::on_act_assiglays_INKO_triggered() {
    delete assiglays_w;
    assiglays_w = new assiglays_wnd(1, db, data_app, settings, this);

    assiglays_w->exec();
}

void assignment_wnd::on_act_assiglays_TFOMS_triggered() {
    delete assiglays_w;
    assiglays_w = new assiglays_wnd(2, db, data_app, settings, this);

    assiglays_w->exec();
}

void assignment_wnd::on_act_assig4dates_triggered() {
    delete get_date_w;
    QDate date1 = QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1), date2 = QDate::currentDate();
    get_date_w = new get_date_wnd("Задайте диапазон дат генерации отчёта",
                                  "Задайте диапазон дат, на котором надо выбрать новых прикреплённых по терр-принципу.",
                                  true, true, true, true, date1, date2, db, data_app, settings);
    if (!get_date_w->exec()) {
        QMessageBox::warning(this, "Отмена операции",
                             "Операция отменена пользователем.");
        return;
    }

    // выберем новые прикрепления за указанный период
    QString sql_data = "select row_number() over() as NN, coalesce(p.enp, e.enp) as enp, a.code_mo, a.code_mp, a.code_ms, a.assig_date, a.snils_mt, a.is_actual, a.unsig_date, "
                       "       e.id, e.fam, e.im, e.ot, e.sex, e.date_birth, e.snils, \n"
                       "       p.pol_v, p.pol_ser, p.pol_num, p.vs_num, p.date_begin, p.date_end \n"
                       "  from persons e \n"
                       "  left join polises p on(p.id=e.id_polis) \n"
                       "  left join persons_assig a on(a.id_person=e.id) \n"
                       "  left join spr_medorgs m on(m.code_mo=a.code_mo) \n"
                       " where a.dt_ins>='#DATE1# 00:00:00' \n"
                       "   and a.dt_upd<='#DATE2# 23:59:59' \n"
                       "   and a.assig_type=1 ; ";
    sql_data = sql_data.replace("#DATE1#", date1.toString("yyyy-MM-dd"));
    sql_data = sql_data.replace("#DATE2#", date2.toString("yyyy-MM-dd"));

    delete show_tab_w;
    this->setCursor(Qt::ArrowCursor);
    QString dates_s = "период с " + date1.toString("dd.MM.yyyy") + " по " + date2.toString("dd.MM.yyyy");
    if (date1==date2)
        dates_s = date1.toString("dd.MM.yyyy");
    show_tab_w = new show_tab_wnd("Новые терр-прикрепления за " + dates_s, sql_data, db, data_app, this);
    show_tab_w->exec();
}

void assignment_wnd::on_act_assig4dates_2_triggered() {
    delete get_date_w;
    QDate date1 = QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1), date2 = QDate::currentDate();
    get_date_w = new get_date_wnd("Задайте диапазон дат генерации отчёта",
                                  "Задайте диапазон дат, на котором надо выбрать новых прикреплённых по личным заявлениям.",
                                  true, true, true, true, date1, date2, db, data_app, settings);
    if (!get_date_w->exec()) {
        QMessageBox::warning(this, "Отмена операции",
                             "Операция отменена пользователем.");
        return;
    }

    // выберем новые прикрепления за указанный период
    QString sql_data = "select row_number() over() as NN, coalesce(p.enp, e.enp) as enp, a.code_mo, a.code_mp, a.code_ms, a.assig_date, a.snils_mt, a.is_actual, a.unsig_date, "
                       "       e.id, e.fam, e.im, e.ot, e.sex, e.date_birth, e.snils, \n"
                       "       p.pol_v, p.pol_ser, p.pol_num, p.vs_num, p.date_begin, p.date_end \n"
                       "  from persons e \n"
                       "  left join polises p on(p.id=e.id_polis) \n"
                       "  left join persons_assig a on(a.id_person=e.id) \n"
                       "  left join spr_medorgs m on(m.code_mo=a.code_mo) \n"
                       " where a.dt_ins>='#DATE1# 00:00:00' \n"
                       "   and a.dt_upd<='#DATE2# 23:59:59' \n"
                       "   and a.assig_type=2 ; ";
    sql_data = sql_data.replace("#DATE1#", date1.toString("yyyy-MM-dd"));
    sql_data = sql_data.replace("#DATE2#", date2.toString("yyyy-MM-dd"));

    delete show_tab_w;
    this->setCursor(Qt::ArrowCursor);
    QString dates_s = "период с " + date1.toString("dd.MM.yyyy") + " по " + date2.toString("dd.MM.yyyy");
    if (date1==date2)
        dates_s = date1.toString("dd.MM.yyyy");
    show_tab_w = new show_tab_wnd("Новые прикрепления по заявлению за " + dates_s, sql_data, db, data_app, this);
    show_tab_w->exec();
}

void assignment_wnd::on_bn_pers_status_auto_clicked() {
    this->setCursor(Qt::WaitCursor);
    // принудительное обновление статуса застрахованного
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select * from update_pers_links(" + QString::number(data_pers.id) + ", NULL) ;";

    if (!(mySQL.exec(this, sql, QString("Обновим статус застрахованного"), *query, true, db, data_app))) {
        QMessageBox::warning(this, "Ошибка при обновлении статауса застрахованного",
                             "При обновлении статуса застрахованного произошла неожиданная ошибка.\n" + sql + "\n\nОперация отменена.");
    } else {
        refresh_persons_tab();
    }
    this->setCursor(Qt::ArrowCursor);
}

void assignment_wnd::on_bn_guten_morgen_toggled(bool checked) {
    if (checked) {
        ui->bn_guten_morgen->setIcon(QIcon(":/moon.png"));
        f_Updater = true;
        /*int n = 0;
        int cnt = 1;

        ui->bn_guten_morgen->setChecked(false);*/
    } else {
        ui->bn_guten_morgen->setIcon(QIcon(":/sun.png"));
        f_Updater = true;
    }
}



void assignment_wnd::on_time_Updater() {
    /*
    if (f_Updater) {
        QString sql_upd = "select * from update_pers_links(CURRENT_DATE, 10) ; ";
        QSqlQuery *query_upd = new QSqlQuery(db);

        int n = ui->lab_Update->text().toInt();
        int cnt = 1;

        ui->bn_guten_morgen->setChecked(true);
        QApplication::processEvents();
        do {
            ui->lab_Update->setText(QString::number(n));
            QApplication::processEvents();

            bool res_upd = query_upd->exec(sql_upd);
            if (res_upd) {
                query_upd->next();
                cnt = query_upd->value(0).toInt();
            } else {
                cnt = -1;
            }
            n++;
        } while (cnt>0);
        ui->bn_guten_morgen->setChecked(false);
        QApplication::processEvents();

        delete query_upd;
    }
    */
}

void assignment_wnd::on_act_letters2MO_triggered() {
    delete letters2MO_w;
    letters2MO_w = new letters2MO_wnd(db, data_app, settings, this);

    letters2MO_w->exec();
}

void assignment_wnd::on_act_medsites_Belgorod_triggered() {
    delete medsites_reg_w;
    medsites_reg_w = new medsites_reg_wnd(db, data_app, settings, this);

    medsites_reg_w->exec();
}

void assignment_wnd::on_act_unassigned_stat_triggered() {
    delete get_date_w;
    QDate date1 = QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1), date2 = QDate::currentDate();
    date1 = date1.addMonths(-1);
    get_date_w = new get_date_wnd("Задайте дату генерации отчёта",
                                  "Задайте дату, по сотоянию на которую надо выбрать статистику не прикреплённых застрахованных ИНКО-МЕД.",
                                  false, true, true, true, date1, date2, db, data_app, settings);
    if (!get_date_w->exec()) {
        QMessageBox::warning(this, "Отмена операции",
                             "Операция отменена пользователем.");
        return;
    }

    // выберем статистику не прикреплённых
    QString sql_data = "select text_why, count(*) /*, id_person, fam, im, ot, date_birth, sex, enp, dt_ins_pers, dt_upd_pers, post_index, ocato, ocato_full, "
                       "       kladr, kladr_street, subj_name, dstr_name, city_name, nasp_name, strt_name, house, corp, quart, subj, dstr, city, nasp, strt, fias_aoid, fias_houseid*/ \n"
                       "  from ( \n"
                       "         select case when e.id_addr_reg is NULL and e.id_addr_liv is NULL \n"
                       "                     then ' 1) нет ни адреса регистрации ни адреса проживания' \n"
                       "                     when e.id_addr_reg is NULL \n"
                       "                     then ' 2) нет адреса регистрации' \n"
                       "                     when addr.kladr is NULL or addr.kladr='' \n"
                       "                     then ' 3) в адресе нет кода КЛАДР' \n"
                       "                     when addr.kladr_street is NULL or addr.kladr_street='' \n"
                       "                     then ' 4) в адресе нет кода КЛАДР_СТРИТ' \n"
                       "                     when addr.subj is NULL or addr.subj=0 \n"
                       "                     then ' 5) в адресе не пределён код субъекта РФ' \n"
                       "                     when addr.subj<>36 \n"
                       "                     then ' 6) код субъекта РФ не равен 36 и нет прикрепления по месту проживания' \n"
                       "                     when (addr.city is NULL or addr.city=0) and (addr.nasp is NULL or addr.nasp=0) \n"
                       "                     then ' 7) в адресе не определены коды ни города ни нас.пункта' \n"
                       "                     when addr.strt is NULL or addr.strt=0 \n"
                       "                     then ' 8) в адресе не определён код улицы' \n"
                       "                     when addr.house is NULL or addr.house=0 \n"
                       "                     then ' 9) в адресе не указан номер дома' \n"
                       "                     when a.id is not NULL and a.code_mo is NULL \n"
                       "                     then ' 0) откреплено ТФОМС' \n"
                       "                else 'новый застрахованный или иное' end as text_why, \n"
                       "                e.id as id_person, e.fam, e.im, e.ot, e.date_birth, e.sex, e.enp, e.dt_ins as dt_ins_pers, e.dt_upd as dt_upd_pers, \n"
                       "                addr.* \n"
                       "           from persons e \n"
                       "           left join persons_assig a on(a.id_person=e.id and a.assig_date<=current_date and (a.unsig_date is NULL or a.unsig_date>current_date) ) \n"
                       "           left join addresses addr on(addr.id=e.id_addr_reg) \n"
                       "          where e.dt_ins<'" + date1.toString("yyyy-MM-dd") + "' \n"
                       "            and (a.id is NULL \n"
                       "             or a.code_mo is NULL) \n"
                       "        ) q \n"
                       " group by q.text_why \n"
                       " order by q.text_why/*, date_birth desc, subj_name, dstr_name, city_name, nasp_name, strt_name, house, corp*/ ; ";

    delete show_tab_w;
    this->setCursor(Qt::ArrowCursor);
    show_tab_w = new show_tab_wnd("Статистика не прикреплённых застрахованных по состоянию на " + date1.toString("dd.MM.yyyy"), sql_data, db, data_app, this);
    show_tab_w->exec();
}

void assignment_wnd::on_act_unassigned_list_triggered() {
    delete get_date_w;
    QDate date1 = QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1), date2 = QDate::currentDate();
    date1 = date1.addMonths(-1);
    get_date_w = new get_date_wnd("Задайте дату генерации отчёта",
                                  "Задайте дату, по сотоянию на которую надо выбрать список не прикреплённых застрахованных ИНКО-МЕД.",
                                  false, true, true, true, date1, date2, db, data_app, settings);
    if (!get_date_w->exec()) {
        QMessageBox::warning(this, "Отмена операции",
                             "Операция отменена пользователем.");
        return;
    }

    // выберем список не прикреплённых
    QString sql_data = "select text_why, id_person, fam, im, ot, date_birth, sex, enp, dt_ins_pers, dt_upd_pers, post_index, ocato, ocato_full,  "
                       "       kladr, kladr_street, subj_name, dstr_name, city_name, nasp_name, strt_name, house, corp, quart, subj, dstr, city, nasp, strt, fias_aoid, fias_houseid \n"
                       "  from ( \n"
                       "         select case when e.id_addr_reg is NULL and e.id_addr_liv is NULL \n"
                       "                     then ' 1) нет ни адреса регистрации ни адреса проживания' \n"
                       "                     when e.id_addr_reg is NULL \n"
                       "                     then ' 2) нет адреса регистрации' \n"
                       "                     when addr.kladr is NULL or addr.kladr='' \n"
                       "                     then ' 3) в адресе нет кода КЛАДР' \n"
                       "                     when addr.kladr_street is NULL or addr.kladr_street='' \n"
                       "                     then ' 4) в адресе нет кода КЛАДР_СТРИТ' \n"
                       "                     when addr.subj is NULL or addr.subj=0 \n"
                       "                     then ' 5) в адресе не пределён код субъекта РФ' \n"
                       "                     when addr.subj<>36 \n"
                       "                     then ' 6) код субъекта РФ не равен 36 и нет прикрепления по месту проживания' \n"
                       "                     when (addr.city is NULL or addr.city=0) and (addr.nasp is NULL or addr.nasp=0) \n"
                       "                     then ' 7) в адресе не определены коды ни города ни нас.пункта' \n"
                       "                     when addr.strt is NULL or addr.strt=0 \n"
                       "                     then ' 8) в адресе не определён код улицы' \n"
                       "                     when addr.house is NULL or addr.house=0 \n"
                       "                     then ' 9) в адресе не указан номер дома' \n"
                       "                     when a.id is not NULL and a.code_mo is NULL \n"
                       "                     then ' 0) прикрепление есть, но без указания кода МО' \n"
                       "                else 'новый застрахованный или иное' end as text_why, \n"
                       "                e.id as id_person, e.fam, e.im, e.ot, e.date_birth, e.sex, e.enp, e.dt_ins as dt_ins_pers, e.dt_upd as dt_upd_pers, \n"
                       "                addr.* \n"
                       "           from persons e \n"
                       "           left join persons_assig a on(a.id_person=e.id and a.assig_date<=current_date and (a.unsig_date is NULL or a.unsig_date>current_date) ) \n"
                       "           left join addresses addr on(addr.id=e.id_addr_reg) \n"
                       "         where e.dt_ins<'" + date1.toString("yyyy-MM-dd") + "' \n"
                       "            and (a.id is NULL \n"
                       "             or a.code_mo is NULL) \n"
                       "        ) q \n"
                       "  order by q.text_why, date_birth desc, subj_name, dstr_name, city_name, nasp_name, strt_name, house, corp ; ";

    delete show_tab_w;
    this->setCursor(Qt::ArrowCursor);
    show_tab_w = new show_tab_wnd("Список не прикреплённых застрахованных по состоянию на " + date1.toString("dd.MM.yyyy"), sql_data, db, data_app, this);
    show_tab_w->exec();
}

void assignment_wnd::on_act_finance_triggered() {
    delete finance_medorgs_w;
    finance_medorgs_w = new finance_medorgs_wnd(db, data_app, settings, this);

    finance_medorgs_w->exec();
}

void assignment_wnd::on_act_assig4_ISZL_triggered() {
    delete get_date_w;
    QDate date1 = QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1), date2 = QDate::currentDate();
    get_date_w = new get_date_wnd("Задайте диапазон дат генерации отчёта",
                                  "Задайте диапазон дат, на котором надо выбрать список новых прикреплённых по терр-принципу.",
                                  true, true, true, true, date1, date2, db, data_app, settings);
    if (!get_date_w->exec()) {
        QMessageBox::warning(this, "Отмена операции",
                             "Операция отменена пользователем.");
        return;
    }

    // выберем новые прикрепления за указанный период
    QString sql_data = "select row_number() over() as NN, coalesce(p.enp, e.enp) as enp, mo.code_head as code_mo/*a.code_mo, a.code_mp, a.code_ms*/, "
                       "       a.assig_date, a.assig_type, /*a.snils_mt,*/ a.is_actual, a.unsig_date, "
                       "       e.id, e.fam, e.im, e.ot, e.sex, e.date_birth, e.snils, "
                       "       p.pol_v, p.pol_ser, p.pol_num, p.vs_num, p.date_begin, p.date_end, evt.event_code, a.source "
                       "  from persons_assig_mo a "
                       "  left join spr_medorgs mo on(mo.code_mo=a.code_mo) "
                       "  left join persons e on(a.id_person=e.id) "
                       "  left join polises p on(p.id=e.id_polis) "
                       "  LEFT JOIN events evt on(evt.id=p.id_evt_add) "
                       " where a.dt_ins>='#DATE1# 00:00:00' "
                       "   and a.dt_ins<='#DATE2# 23:59:59' "
                       "   and a.assig_type=1 "
                       "   and left(a.source,22)<>'ТФОМС - файл изменений' "
                       " order by a.source, a.assig_date  ; ";
    sql_data = sql_data.replace("#DATE1#", date1.toString("yyyy-MM-dd"));
    sql_data = sql_data.replace("#DATE2#", date2.toString("yyyy-MM-dd"));

    QSqlQuery *query_data = new QSqlQuery(db);
    if ( !mySQL.exec(this, sql_data, "Выборка терр-прикреплений за период", *query_data, true, db, data_app) ) {
        QMessageBox::warning(this, "Ошибка при выборе прикреплённых по терр-принципц за период",
                             "При попытке выбрать новых прикреплённых по территориальному принципу за период произошла неожиданныя ошибка.\n\n" +
                             sql_data + "\n\nDB error - " +
                             query_data->lastError().databaseText() +  "\ndriver error - " +
                             query_data->lastError().driverText() + "\n\n"
                             "Действие отменено." );
        delete query_data;
        return;
    }

    // ==========================
    // сформируем отчёт по форме
    // ==========================
    QString fname_template = data_app.path_reports + "rep_new_assigs_by_terr_for_ISZL.ods";
    QString temp_folder = data_app.path_temp + "rep_new_assigs_by_terr_for_ISZL";
    if (!QFile::exists(fname_template)) {
        QMessageBox::warning(this,
                             "Шаблон не найден",
                             "Не найден шаблон печатной формы списка рассылки: \n" + fname_template +
                             "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        return;
    }
    QDir tmp_dir(temp_folder);
    if (tmp_dir.exists(temp_folder) && !tmp_dir.removeRecursively()) {
        QMessageBox::warning(this,
                             "Ошибка при очистке папки",
                             "Не удалось очистить папку временных файлов: \n" + temp_folder +
                             "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        return;
    }
    tmp_dir.mkpath(temp_folder);
    QProcess myProcess;
    QString com = "\"" + data_app.path_arch + "7z.exe\" e \"" + fname_template + "\" -o\"" + temp_folder + "\" content.xml";
    if (myProcess.execute(com)!=0) {
        QMessageBox::warning(this,
                             "Ошибка распаковки шаблона",
                             "Не удалось распаковать файл контента шаблона\n" + fname_template + " -> content.xml"
                             "\nпечатной формы во временную папку\n" + temp_folder +
                             "\n\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки." + "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        return;
    }
    com = "\"" + data_app.path_arch + "7z.exe\" e \"" + fname_template + "\" -o\"" + temp_folder + "\" styles.xml";
    if (myProcess.execute(com)!=0) {
        QMessageBox::warning(this,
                             "Ошибка распаковки шаблона",
                             "Не удалось распаковать файл стилей шаблона \n" + fname_template + " -> styles.xml"
                             "\nпечатной формы во временную папку\n" + temp_folder +
                             "\n\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки." + "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        return;
    }

    // открытие контента шаблона
    QFile f_content(temp_folder + "/content.xml");
    f_content.open(QIODevice::ReadOnly);
    QString s_content = f_content.readAll();
    f_content.close();
    if (s_content.isEmpty()) {
        QMessageBox::warning(this,
                             "Файл контента шаблона пустой",
                             "Файл контента шаблона пустой. Возможно шаблон был испорчен.\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        return;
    }
    // выбор шаблона
    int pos0 = s_content.indexOf("<table:table-row table:style-name=\"ro4\">", 0);
    int pos1 = s_content.indexOf("</table:table-row>", pos0) + 18;
    QString s_row = s_content.mid(pos0, pos1-pos0);

    // правка полей контента шаблона
    QString s_content_new = s_content.left(pos0);
    s_content_new = s_content_new.
                    replace("#DATE_FROM#", date1.toString("dd.MM.yyyy")).
                    replace("#DATE_TO#",   date2.toString("dd.MM.yyyy"));

    QFile f_content_save(temp_folder + "/content1.xml");
    f_content_save.open(QIODevice::WriteOnly);
    f_content_save.write(s_content_new.toUtf8());
    f_content_save.flush();

    int n = 0;
    while (query_data->next()) {
        n++;
        QString s_row_new = s_row;
        //int     n          = query_data->value( 0).isNull() ? -1 : query_data->value( 0).toInt();
        QString enp        = query_data->value( 1).isNull() ? "" : query_data->value( 1).toString();
        QString code_mo    = query_data->value( 2).isNull() ? "" : query_data->value( 2).toString();
        QDate   assig_date = query_data->value( 3).isNull() ? QDate::currentDate() : query_data->value( 3).toDate();
        int     assig_type = query_data->value( 4).isNull() ? 1  : query_data->value( 4).toInt();
        QString fam        = query_data->value( 8).isNull() ? "" : query_data->value( 8).toString();
        QString im         = query_data->value( 9).isNull() ? "" : query_data->value( 9).toString();
        QString ot         = query_data->value(10).isNull() ? "" : query_data->value(10).toString();
        QDate   date_birth = query_data->value(12).isNull() ? QDate::currentDate() : query_data->value(12).toDate();
        QDate   date_begin = query_data->value(18).isNull() ? QDate::currentDate() : query_data->value(18).toDate();
        QString event_code = query_data->value(20).isNull() ? "" : query_data->value(20).toString();
        QString source     = query_data->value(21).isNull() ? "" : query_data->value(21).toString();

        s_row_new = s_row_new.
                    replace("#NN#",         QString::number(n)).
                    replace("#ENP#",        enp).
                    replace("#CODE_MO#",    code_mo).
                    replace("#ASSIG_DATE#", assig_date.toString("dd.MM.yyyy")).
                    replace("#ASSIG_TYPE#", QString::number(assig_type)).
                    replace("#COMMENT#",    source).
                    replace("#FAM#",        fam).
                    replace("#IM#",         im).
                    replace("#OT#",         ot).
                    replace("#DATE_BIRTH#", date_birth.toString("dd.MM.yyyy")).
                    replace("#EVENT#",      event_code).
                    replace("#DATE_BEGIN#", date_begin.toString("dd.MM.yyyy"));

        // сохранение контента шаблона
        f_content_save.write(s_row_new.toUtf8());
        s_row_new.clear();
    }
    delete query_data;

    s_content_new = s_content.right(s_content.size()-pos1);
    f_content_save.write(s_content_new.toUtf8());

    f_content_save.close();

    QFile::remove(temp_folder + "/content.xml");
    QFile::copy(temp_folder + "/content1.xml", temp_folder + "/content.xml");

    // архивация шаблона
    QString dates = date1.toString("yyyy-MM-dd");
    if (date1!=date2)  dates = "период с " + date1.toString("yyyy-MM-dd") + " по " + date2.toString("yyyy-MM-dd");
    QString fname_report = data_app.path_out + "_REPORTS_/" +
            QDate::currentDate().toString("yyyy-MM-dd") + "  -  Новые терр-прикрелпённые за " + dates + ".ods";

    QFile::remove(fname_report);
    while (QFile::exists(fname_report)) {
        if (QMessageBox::warning(this, "Ошибка сохранения списка новых терр-прикреплённых",
                                 "Не удалось сохранить список новых терр-прикреплённых: \n" + fname_template +
                                 "\n\nво временную папку\n" + fname_report +
                                 "\n\nПроверьте, не открыт ли целевой файл в сторонней программе и повторите операцию.",
                                 QMessageBox::Retry|QMessageBox::Abort,
                                 QMessageBox::Retry)==QMessageBox::Abort) {
            this->setCursor(Qt::ArrowCursor);
            return;
        }
        QFile::remove(fname_report);
    }
    if (!QFile::copy(fname_template, fname_report) || !QFile::exists(fname_report)) {
        QMessageBox::warning(this, "Ошибка копирования отчёта",
                             "Не удалось скопировать список новых терр-прикрелпённых: \n" +
                             fname_template + "\n\nво временную папку\n" +
                             fname_report + "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        return;
    }
    com = "\"" + data_app.path_arch + "7z.exe\" a \"" + fname_report + "\" \"" + temp_folder + "/content.xml\"";
    if (myProcess.execute(com)!=0) {
        QMessageBox::warning(this, "Ошибка обновления контента",
                             "При добавлении нового контента в список новых терр-прикреплённых произошла непредвиденная ошибка\n\n"
                             "Опреация прервана.");
    }
    this->setCursor(Qt::ArrowCursor);

    // ===========================
    // открытие полученного ODT
    // ===========================
    long result = (long long)ShellExecute(0, 0, reinterpret_cast<const WCHAR*>(fname_report.utf16()), 0, 0, SW_NORMAL);
}


void assignment_wnd::on_tab_persons_activated(const QModelIndex &index) {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    ui->pan_assig->setEnabled(false);

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_persons->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        s_data_person data;
        data.id    = model_persons.data(model_persons.index(index.row(), 0), Qt::EditRole).toInt();          // e.id
        data.fam   = model_persons.data(model_persons.index(index.row(), 2), Qt::EditRole).toString();       // e.fam
        data.im    = model_persons.data(model_persons.index(index.row(), 3), Qt::EditRole).toString();       // e.im
        data.ot    = model_persons.data(model_persons.index(index.row(), 4), Qt::EditRole).toString();       // e.ot
        data.sex   = model_persons.data(model_persons.index(index.row(), 5), Qt::EditRole).toInt();          // e.sex
        data.date_birth =  model_persons.data(model_persons.index(index.row(), 6), Qt::EditRole).toDate();   // e.date_birth
        data.f_death    = !model_persons.data(model_persons.index(index.row(), 7), Qt::EditRole).isNull();   // e.f_death
        data.date_death =  model_persons.data(model_persons.index(index.row(), 7), Qt::EditRole).toDate();   // e.date_death
        data.snils = model_persons.data(model_persons.index(index.row(), 8), Qt::EditRole).toString();       // e.snils
        data.enp   = model_persons.data(model_persons.index(index.row(), 9), Qt::EditRole).toString();       // e.enp
        data.date_reg   =  model_persons.data(model_persons.index(index.row(),10), Qt::EditRole).toDate();   // e.date_reg
        data.f_bomj     =  model_persons.data(model_persons.index(index.row(),11), Qt::EditRole).toInt();    // e.f_bomj

        QString  sex    = model_persons.data(model_persons.index(index.row(),  5), Qt::EditRole).toString().trimmed();
        if (sex=="муж.")  data.sex  = 1;
        else if (sex==" жен.") data.sex  = 1;
        else              data.sex  = 2;

        QString  pol_type    = model_persons.data(model_persons.index(index.row(),  1), Qt::EditRole).toString().trimmed();
        if (pol_type=="ВС")  data.pol_v  = 2;
        else                 data.pol_v  = 3;

        delete add_person_w;
        add_person_w = new add_person_wnd(db, data_app, data, false, this);

        add_person_w->exec();

    }
}

void assignment_wnd::on_tab_assig_activated(const QModelIndex &index) {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_assig->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        s_data_assig_rec data;

        data.id_table   = ui->combo_assig_source->currentData().toInt();
        data.id_person  = model_assig.data(model_assig.index(index.row(), 0), Qt::EditRole).toInt();          // a.id_person
        data.id         = model_assig.data(model_assig.index(index.row(), 1), Qt::EditRole).toInt();          // a.id
        data.assig_type = model_assig.data(model_assig.index(index.row(), 2), Qt::EditRole).toInt();          // a.assig_type
        data.assig_date = model_assig.data(model_assig.index(index.row(), 4), Qt::EditRole).toDate();         // a.assig_date
        data.f_unsig    =!model_assig.data(model_assig.index(index.row(), 5), Qt::EditRole).isNull();         // !a.unsig_date is NULL
        data.unsig_date = model_assig.data(model_assig.index(index.row(), 5), Qt::EditRole).toDate();         // a.unsig_date
        data.is_actual  = model_assig.data(model_assig.index(index.row(), 6), Qt::EditRole).toInt();          // a.is_actual
        data.code_mo    = model_assig.data(model_assig.index(index.row(), 7), Qt::EditRole).toString();       // e.code_mo
        data.code_mp    = model_assig.data(model_assig.index(index.row(), 9), Qt::EditRole).toString();       // e.code_mp
        data.code_ms    = model_assig.data(model_assig.index(index.row(),10), Qt::EditRole).toString();       // e.code_ms
        data.snils_mt   = model_assig.data(model_assig.index(index.row(),11), Qt::EditRole).toString();       // e.snils_mt
        data.source     = model_assig.data(model_assig.index(index.row(),14), Qt::EditRole).toString();       // e.source

        delete add_assig_rec_w;
        add_assig_rec_w = new add_assig_rec_wnd(db, data, data_app, false, this);

        add_assig_rec_w->exec();

    }
}

void assignment_wnd::on_act_assig2mo_test_triggered() {
    delete reports_assig2mo_w;
    reports_assig2mo_w = new reports_assig2mo_wnd(db, data_app, settings, this);

    reports_assig2mo_w->exec();
}

void assignment_wnd::on_act_addr4dates_triggered() {
    delete get_date_w;
    QDate date1 = QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1), date2 = QDate::currentDate();
    get_date_w = new get_date_wnd("Задайте диапазон дат добавления новых адресов",
                                  "Задайте диапазон дат, на котором надо выбрать новые адреса прикрепления по терр-принципу.",
                                  true, true, true, true, date1, date2, db, data_app, settings);
    if (!get_date_w->exec()) {
        QMessageBox::warning(this, "Отмена операции",
                             "Операция отменена пользователем.");
        return;
    }

    // выберем новые прикрепления за указанный период
    QString sql_data = "select row_number() over() as NN, "
                       "       a.ocato, a.guid, "
                       "       a.subj, t.offname||' '||t.shortname as subj_name, "
                       "       a.dstr, d.offname||' '||d.shortname as dstr_name, "
                       "       a.city, c.offname||' '||c.shortname as coty_name, "
                       "       a.nasp, n.offname||' '||n.shortname as nasp_name, "
                       "       a.strt, s.offname||' '||s.shortname as strt_name, "
                       "       a.from_start, a.to_end, a.side, a.house, a.corp, a.sex, a.age, "
                       "       a.code_mo, a.code_mp, a.code_ms, a.snils_mt, a.comment, "
                       "       a.id_point, p.point_name, a.id_operator, o.oper_fio, "
                       "       a.dt_ins "
                       "  from assig_by_terr a "
                       "  left join fias.subj t on(t.subj=a.subj) "
                       "  left join fias.dstr d on(d.subj=a.subj and d.dstr=a.dstr) "
                       "  left join fias.city c on(c.subj=a.subj and c.dstr=a.dstr and c.city=a.city) "
                       "  left join fias.nasp n on(n.subj=a.subj and n.dstr=a.dstr and n.city=a.city and n.nasp=a.nasp) "
                       "  left join fias.strt s on(s.subj=a.subj and s.dstr=a.dstr and s.city=a.city and s.nasp=a.nasp and s.strt=a.strt) "
                       "  left join points p on(p.id=a.id_point) "
                       "  left join operators o on(o.id=a.id_operator) "
                       " where a.dt_ins>='#DATE1# 00:00:00' "
                       "   and a.dt_ins<='#DATE2# 23:59:59'  ; ";
    sql_data = sql_data.replace("#DATE1#", date1.toString("yyyy-MM-dd"));
    sql_data = sql_data.replace("#DATE2#", date2.toString("yyyy-MM-dd"));

    delete show_tab_w;
    this->setCursor(Qt::ArrowCursor);
    QString dates_s = "период с " + date1.toString("dd.MM.yyyy") + " по " + date2.toString("dd.MM.yyyy");
    if (date1==date2)
        dates_s = date1.toString("dd.MM.yyyy");
    show_tab_w = new show_tab_wnd("Новые адреса терр-прикрепления за " + dates_s, sql_data, db, data_app, this);
    show_tab_w->exec();
}

void assignment_wnd::on_act_letters_gen_triggered() {
    delete print_letters_w;
    print_letters_w = new print_letters_wnd(db, data_app, settings);
    print_letters_w->exec();
}
