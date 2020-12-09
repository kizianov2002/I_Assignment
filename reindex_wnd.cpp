#include "reindex_wnd.h"
#include "ui_reindex_wnd.h"
#include "ki_funcs.h"

reindex_wnd::reindex_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::reindex_wnd)
{
    ui->setupUi(this);
    ui->pan_wait->setVisible(false);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    cnt_links = 0;
    cnt_pol   = 0;
    cnt_udl   = 0;
    cnt_drp   = 0;
    cnt_asg   = 0;

    ui->date->setDate(QDate::currentDate());

    ui->cnt_links->setText(" -//- ");
    ui->cnt_pol->setText(" -//- ");
    ui->cnt_udl->setText(" -//- ");
    ui->cnt_drp->setText(" -//- ");
    ui->cnt_asg->setText(" -//- ");
}

reindex_wnd::~reindex_wnd()
{
    delete ui;
}

void reindex_wnd::on_bn_close_clicked() {
    close();
}

void reindex_wnd::on_ch_links_clicked(bool checked) {
    if (checked) {
        // доступна только полная переиндексация всех записей
        ui->ch_pol->setChecked(false);
        ui->ch_udl->setChecked(false);
        ui->ch_drp->setChecked(false);
        ui->ch_asg->setChecked(false);

        ui->ch_null->setChecked(false);
        ui->ch_null->setEnabled(false);
        ui->ch_all->setChecked(false);
        ui->ch_all->setEnabled(false);
    }
}
void reindex_wnd::on_ch_pol_clicked(bool checked) {
    if (checked) {
        ui->ch_links->setChecked(false);
        //ui->ch_udl->setChecked(false);
        //ui->ch_drp->setChecked(false);
        //ui->ch_asg->setChecked(false);
    }
}
void reindex_wnd::on_ch_udl_clicked(bool checked) {
    if (checked) {
        ui->ch_links->setChecked(false);
        //ui->ch_pol->setChecked(false);
        //ui->ch_drp->setChecked(false);
        //ui->ch_asg->setChecked(false);
    }
}
void reindex_wnd::on_ch_drp_clicked(bool checked) {
    if (checked) {
        ui->ch_links->setChecked(false);
        //ui->ch_pol->setChecked(false);
        //ui->ch_udl->setChecked(false);
        //ui->ch_asg->setChecked(false);
    }
}
void reindex_wnd::on_ch_asg_clicked(bool checked) {
    if (checked) {
        ui->ch_links->setChecked(false);
        //ui->ch_pol->setChecked(false);
        //ui->ch_udl->setChecked(false);
        //ui->ch_drp->setChecked(false);
    }
}

void reindex_wnd::on_ch_date_clicked(bool checked) {
    if (checked) {
        ui->ch_null->setChecked(false);
        ui->ch_old->setChecked(false);
        ui->ch_all->setChecked(false);
    }
}

void reindex_wnd::on_ch_null_clicked(bool checked) {
    if (checked) {
        ui->ch_date->setChecked(false);
        ui->ch_old->setChecked(false);
        ui->ch_all->setChecked(false);
    }
}
void reindex_wnd::on_ch_all_clicked(bool checked) {
    if (checked) {
        ui->ch_date->setChecked(false);
        ui->ch_null->setChecked(false);
        ui->ch_old->setChecked(false);
    }
}

void reindex_wnd::on_bn_process_clicked() {
    ui->bn_close->setEnabled(false);
    ui->bn_process->setEnabled(false);
    ui->pan_wait->setVisible(true);
    ui->bn_pause->setEnabled(true);
    ui->bn_stop->setEnabled(true);
    ui->bn_pause->setChecked(false);
    ui->bn_stop->setChecked(false);

    date0 = QDate::currentDate();
    time0 = QTime::currentTime();

    if (ui->ch_links->isChecked()) update_links();

    if (ui->ch_pol->isChecked())   update_pol();
    if (ui->ch_udl->isChecked())   update_udl();
    if (ui->ch_drp->isChecked())   update_drp();
    if (ui->ch_asg->isChecked())   update_asg();

    ui->bn_close->setEnabled(true);
    ui->bn_process->setEnabled(true);
    ui->pan_wait->setVisible(false);
    ui->bn_pause->setEnabled(false);
    ui->bn_stop->setEnabled(false);
    ui->bn_pause->setChecked(false);
    ui->bn_stop->setChecked(false);
}

bool reindex_wnd::update_links() {
    ui->pan_wait->setVisible(true);
    int lim = 1000;
    int cnt = 0;
    int cnt_sum = 0;
    f_pause = false;
    f_stop = false;


    db.transaction();


    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  подготовка к обновлению связок с полисами");
    QApplication::processEvents();

    // сначала выставим флаги всем персонам, у кого текущий полис не совпадает с полисом по связке
    QString sql;
    sql = "update persons e "
          "  set _update_pol=1 "
          "where e.id in( "
          "      select distinct e.id "
          "        from persons e "
          "        left join ( select pol.id_person, max(pol.id) as id_pol "
          "                      from polises pol "
          "                     where pol.date_begin<=CURRENT_DATE "
          "                       and ( pol.date_stop is NULL "
          "                          or pol.date_stop>CURRENT_DATE ) "
          "                    group by pol.id_person ) q_pol "
          "               on (q_pol.id_person=e.id) "
          "       where _update_pol<>1 "
          "         and e.id_polis<>q_pol.id_pol ); ";

    QSqlQuery *query1 = new QSqlQuery(db);
    bool res = query1->exec(sql);

    if (!res) {
        QMessageBox::warning(this, "Ошибка обновления связок полисов",
                             "При попытке обновить связки полисов произошла неожиданная ошибка.\n\n"
                             + query1->lastError().driverText() + "\n" + query1->lastError().databaseText() + "\n\n"
                             "Запись пропущена.");
        delete query1;
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->bn_process->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return false;
    }
    delete query1;


    db.commit();
    db.transaction();


    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  подготовка к обновлению связок с УДЛ");
    QApplication::processEvents();

    // сначала выставим флаги всем персонам, у кого текущий УДЛ не совпадает с УДЛ по связке
    sql = "update persons e "
          "   set _update_udl=1 "
          "  where e.id in(select distinct e.id "
          "  from persons e "
          "  left join ( select udl.id_person, max(udl.id) as id_udl "
          "                from persons_docs udl "
          "               where udl.status=1 "
          "                 and udl.doc_date<=CURRENT_DATE "
          "                 and ( udl.doc_exp is NULL "
          "                    or udl.doc_exp>CURRENT_DATE )"
          "               group by udl.id_person ) q_udl "
          "         on (q_udl.id_person=e.id) "
          " where _update_udl<>1 "
          "   and e.id_udl<>q_udl.id_udl ) ";

    QSqlQuery *query2 = new QSqlQuery(db);
    res = query2->exec(sql);

    if (!res) {
        QMessageBox::warning(this, "Ошибка обновления связок УДЛ",
                             "При попытке обновить связки УДЛ произошла неожиданная ошибка.\n\n"
                             + query2->lastError().driverText() + "\n" + query2->lastError().databaseText() + "\n\n"
                             "Запись пропущена.");
        delete query2;
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->bn_process->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return false;
    }
    delete query2;


    db.commit();
    db.transaction();


    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  подготовка к обновлению связок с ДРП");
    QApplication::processEvents();

    // сначала выставим флаги всем персонам, у кого текущий ДРП не совпадает с ДРП по связке
    sql = "update persons e "
          "   set _update_drp=1 "
          "  where e.category in (2, 7, 3, 8, 4, 9, 11, 12, 13, 14) "
          "    and e.id in(select distinct e.id "
          "        from persons e "
          "        left join ( select drp.id_person, max(drp.id) as id_drp "
          "                      from persons_docs drp "
          "                     where drp.status=1 "
          "                       and drp.doc_date<=CURRENT_DATE "
          "                       and ( drp.doc_exp is NULL "
          "                          or drp.doc_exp>CURRENT_DATE )"
          "                     group by drp.id_person ) q_drp "
          "          on (q_drp.id_person=e.id) "
          "    where _update_drp<>1 "
          "      and e.id_drp<>q_drp.id_drp ) ";

    QSqlQuery *query3 = new QSqlQuery(db);
    res = query3->exec(sql);

    if (!res) {
        QMessageBox::warning(this, "Ошибка обновления связок ДРП",
                             "При попытке обновить связки ДРП произошла неожиданная ошибка.\n\n"
                             + query3->lastError().driverText() + "\n" + query3->lastError().databaseText() + "\n\n"
                             "Запись пропущена.");
        delete query3;
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->bn_process->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return false;
    }
    delete query3;


    db.commit();
    db.transaction();


    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  подготовка к обновлению связок с прикреплением");
    QApplication::processEvents();

    // сначала выставим флаги всем персонам, у кого текущее прикрепление не совпадает с прикреплением по связке
    sql = "update persons e "
          "   set _update_asg=1 "
          " where e.id in ( "
          "          select distinct la.id_person "
          "            from persons e "
          "            left join link_persons_asg la on(la.id_person=e.id) "
          "            left join ( select asg.id_person, max(asg.id) as id_asg "
          "                          from persons_assig asg "
          "                         where asg.assig_date<=CURRENT_DATE "
          "                           and ( asg.unsig_date is NULL "
          "                              or asg.unsig_date>CURRENT_DATE ) "
          "                        group by asg.id_person ) q_asg "
          "                   on (q_asg.id_person=la.id_person) "
          "           where e._update_asg<>1 "
          "             and la._id_asg<>q_asg.id_asg ); ";

    QSqlQuery *query4 = new QSqlQuery(db);
    res = query4->exec(sql);

    if (!res) {
        QMessageBox::warning(this, "Ошибка обновления связок прикреплений",
                             "При попытке обновить связки прикреплений произошла неожиданная ошибка.\n\n"
                             + query4->lastError().driverText() + "\n" + query4->lastError().databaseText() + "\n\n"
                             "Запись пропущена.");
        delete query4;
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->bn_process->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return false;
    }
    delete query4;


    db.commit();


    do {
        QApplication::processEvents();
        db.transaction();

        if (ui->rb_1->isChecked())  lim=1;
        if (ui->rb_21->isChecked())  lim=21;
        if (ui->rb_231->isChecked())  lim=231;
        if (ui->rb_2341->isChecked())  lim=2341;
        if (ui->rb_23451->isChecked())  lim=23451;

        // подготовим функцию обновления связок
        QString sql;
        sql = "select #FUNC_NAME#("
              " " + (ui->ch_date->isChecked() ? "cast('" + ui->date->date().toString("yyyy-MM-dd") + "' as date) " : QString("CURRENT_DATE")) + ", "
              "  " + QString::number(lim) + ") ; ";

        if (ui->ch_old->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links");
        else if (ui->ch_null->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links_null");
        else if (ui->ch_all->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links_all");

        // вызовем функцию обновления связок в цикле
        ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -   " + sql);
        QApplication::processEvents();

        QSqlQuery *query = new QSqlQuery(db);
        bool res = query->exec(sql);

        if (!res) {
            QMessageBox::warning(this, "Ошибка обновления связок полисов",
                                 "При попытке обновить связки полисов произошла неожиданная ошибка.\n\n"
                                 + query->lastError().driverText() + "\n" + query->lastError().databaseText() + "\n\n"
                                 "Запись пропущена.");
            delete query;
            db.rollback();

            ui->bn_close->setEnabled(true);
            ui->bn_process->setEnabled(true);
            ui->pan_wait->setVisible(false);
            return false;
        }
        query->next();
        cnt = query->value(0).toInt();
        cnt_sum += cnt;
        ui->cnt_links->setText(show_num(cnt_sum));

        delete query;
        db.commit();

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

        if (f_pause) {
            f_pause = false;
            ui->bn_pause->setChecked(false);
            if (QMessageBox::information(this, "Пауза",
                                     "Последняя транзакция закрыта.\n"
                                     "Для приостановки работы просто не закрывайте это окно.\n\n"
                                     "Для продолжения работы нажмите [Yes]\n"
                                     "Для остановки процесса нажмите [No]\n",
                                     QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)==QMessageBox::No) {
                break;
            }
        }
        if (f_stop)  break;
    }
    while ( cnt>0 );

    ui->bn_close->setEnabled(true);
    ui->bn_process->setEnabled(true);
    ui->pan_wait->setVisible(false);
    ui->bn_pause->setEnabled(false);
    ui->bn_stop->setEnabled(false);
    ui->bn_pause->setChecked(false);
    ui->bn_stop->setChecked(false);

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  Готово. Обновлены все связки " + QString::number(cnt_sum) + " персон.");
    return true;
}

bool reindex_wnd::update_pol() {
    ui->pan_wait->setVisible(true);
    int lim = 1000;
    int cnt = 0;
    int cnt_sum = 0;
    f_pause = false;
    f_stop = false;

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  подготовка к обновлению связок с полисами");
    QApplication::processEvents();
    db.transaction();

    // сначала выставим флаги всем персонам, у кого текущий полис не совпадает с полисом по связке
    QString sql;
    sql = "update persons e "
          "   set _update_pol=1 "
          " where _update_pol<>1 "
          "   and e.id_polis<> "
          "       ( select max(pol.id) "
          "           from polises pol "
          "          where pol.id_person=e.id "
          "            and pol.date_begin<=CURRENT_DATE "
          "            and ( pol.date_stop is NULL "
          "               or pol.date_stop>CURRENT_DATE ) ); ";

    QSqlQuery *query = new QSqlQuery(db);
    bool res = query->exec(sql);

    if (!res) {
        QMessageBox::warning(this, "Ошибка обновления связок полисов",
                             "При попытке обновить связки полисов произошла неожиданная ошибка.\n\n"
                             + query->lastError().driverText() + "\n" + query->lastError().databaseText() + "\n\n"
                             "Запись пропущена.");
        delete query;
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->bn_process->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return false;
    }
    delete query;
    db.commit();
    db.transaction();

    do {
        if (ui->rb_1->isChecked())  lim=1;
        if (ui->rb_21->isChecked())  lim=21;
        if (ui->rb_231->isChecked())  lim=231;
        if (ui->rb_2341->isChecked())  lim=2341;
        if (ui->rb_23451->isChecked())  lim=23451;

        // подготовим функцию обновления связок
        sql = "select * "
              "  from #FUNC_NAME#( " + (ui->ch_date->isChecked() ? "cast('" + ui->date->date().toString("yyyy-MM-dd") + "' as date) " : QString("CURRENT_DATE")) + ", "
                                 " " + QString::number(lim) + ") ; ";

        if (ui->ch_old->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links_pol");
        else if (ui->ch_null->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links_pol_null");
        else if (ui->ch_all->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links_pol_all");

        // вызовем функцию обновления связок полисов в цикле
        ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -   " + sql);
        QApplication::processEvents();

        query = new QSqlQuery(db);
        res = query->exec(sql);

        if (!res) {
            QMessageBox::warning(this, "Ошибка обновления связок полисов",
                                 "При попытке обновить связки полисов произошла неожиданная ошибка.\n\n"
                                 + query->lastError().driverText() + "\n" + query->lastError().databaseText() + "\n\n"
                                 "Запись пропущена.");
            delete query;
            db.rollback();

            ui->bn_close->setEnabled(true);
            ui->bn_process->setEnabled(true);
            ui->pan_wait->setVisible(false);
            return false;
        }
        query->next();
        cnt = query->value(0).toInt();
        cnt_sum += cnt;
        ui->cnt_pol->setText(show_num(cnt_sum));

        delete query;
        db.commit();

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

        // контроль кнопок "Пауза" и "Стоп"
        QApplication::processEvents();
        if (ui->bn_pause->hasFocus())
            on_bn_pause_clicked(true);
        QApplication::processEvents();
        if (ui->bn_stop->hasFocus())
            on_bn_stop_clicked(true);
        QApplication::processEvents();

        if (f_pause) {
            f_pause = false;
            ui->bn_pause->setChecked(false);
            if (QMessageBox::information(this, "Пауза",
                                     "Последняя транзакция закрыта.\n"
                                     "Для приостановки работы просто не закрывайте это окно.\n\n"
                                     "Для продолжения работы нажмите [Yes]\n"
                                     "Для остановки процесса нажмите [No]\n",
                                     QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)==QMessageBox::No) {
                break;
            }
        }
        if (f_stop)  break;
    }
    while ( cnt>0 );

    ui->bn_close->setEnabled(true);
    ui->bn_process->setEnabled(true);
    ui->pan_wait->setVisible(false);
    ui->bn_pause->setEnabled(false);
    ui->bn_stop->setEnabled(false);
    ui->bn_pause->setChecked(false);
    ui->bn_stop->setChecked(false);

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  Готово. Обновлено " + QString::number(cnt_sum) + " связок персон и полисов.");
    return true;
}

bool reindex_wnd::update_udl() {
    ui->pan_wait->setVisible(true);
    int lim = 1000;
    int cnt = 0;
    int cnt_sum = 0;
    f_pause = false;
    f_stop = false;

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  подготовка к обновлению связок с УДЛ");
    QApplication::processEvents();
    db.transaction();

    // сначала выставим флаги всем персонам, у кого текущий УДЛ не совпадает с УДЛ по связке
    QString sql;
    sql = "update persons e "
          "   set _update_udl=1 "
          "  where e.id in(select distinct e.id "
          "  from persons e "
          "  left join ( select udl.id_person, max(udl.id) as id_udl "
          "                from persons_docs udl "
          "               where udl.status=1 "
          "                 and udl.doc_date<=CURRENT_DATE "
          "                 and ( udl.doc_exp is NULL "
          "                    or udl.doc_exp>CURRENT_DATE )"
          "               group by udl.id_person ) q_udl "
          "         on (q_udl.id_person=e.id) "
          " where _update_udl<>1 "
          "   and e.id_udl<>q_udl.id_udl ) ";

    QSqlQuery *query = new QSqlQuery(db);
    bool res = query->exec(sql);

    if (!res) {
        QMessageBox::warning(this, "Ошибка обновления связок УДЛ",
                             "При попытке обновить связки УДЛ произошла неожиданная ошибка.\n\n"
                             + query->lastError().driverText() + "\n" + query->lastError().databaseText() + "\n\n"
                             "Запись пропущена.");
        delete query;
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->bn_process->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return false;
    }
    delete query;
    db.commit();
    db.transaction();

    do {
        QApplication::processEvents();
        db.transaction();

        if (ui->rb_1->isChecked())  lim=1;
        if (ui->rb_21->isChecked())  lim=21;
        if (ui->rb_231->isChecked())  lim=231;
        if (ui->rb_2341->isChecked())  lim=2341;
        if (ui->rb_23451->isChecked())  lim=23451;

        // подготовим функцию обновления связок
        QString sql;
        sql = "select #FUNC_NAME#("
              " " + (ui->ch_date->isChecked() ? "cast('" + ui->date->date().toString("yyyy-MM-dd") + "' as date) " : QString("CURRENT_DATE")) + ", "
              " " + QString::number(lim) + ") ; ";

        if (ui->ch_old->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links_udl");
        else if (ui->ch_null->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links_udl_null");
        else if (ui->ch_all->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links_udl_all");

        // вызовем функцию обновления связок полисов в УДЛ
        ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -   " + sql);
        QApplication::processEvents();

        QSqlQuery *query = new QSqlQuery(db);
        bool res = query->exec(sql);

        if (!res) {
            QMessageBox::warning(this, "Ошибка обновления связок УДЛ",
                                 "При попытке обновить связки УДЛ произошла неожиданная ошибка.\n\n"
                                 + query->lastError().driverText() + "\n" + query->lastError().databaseText() + "\n\n"
                                 "Запись пропущена.");
            delete query;
            db.rollback();

            ui->bn_close->setEnabled(true);
            ui->bn_process->setEnabled(true);
            ui->pan_wait->setVisible(false);
            return false;
        }
        query->next();
        cnt = query->value(0).toInt();
        cnt_sum += cnt;
        ui->cnt_udl->setText(show_num(cnt_sum));

        delete query;
        db.commit();

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

        // контроль кнопок "Пауза" и "Стоп"
        QApplication::processEvents();
        if (ui->bn_pause->hasFocus())
            on_bn_pause_clicked(true);
        QApplication::processEvents();
        if (ui->bn_stop->hasFocus())
            on_bn_stop_clicked(true);
        QApplication::processEvents();

        if (f_pause) {
            f_pause = false;
            ui->bn_pause->setChecked(false);
            if (QMessageBox::information(this, "Пауза",
                                     "Последняя транзакция закрыта.\n"
                                     "Для приостановки работы просто не закрывайте это окно.\n\n"
                                     "Для продолжения работы нажмите [Yes]\n"
                                     "Для остановки процесса нажмите [No]\n",
                                     QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)==QMessageBox::No) {
                break;
            }
        }
        if (f_stop)  break;
    }
    while ( cnt>0 );

    ui->bn_close->setEnabled(true);
    ui->bn_process->setEnabled(true);
    ui->pan_wait->setVisible(false);
    ui->bn_pause->setEnabled(false);
    ui->bn_stop->setEnabled(false);
    ui->bn_pause->setChecked(false);
    ui->bn_stop->setChecked(false);

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  Готово. Обновлено " + QString::number(cnt_sum) + " связок персон и УДЛ.");
    return true;
}
bool reindex_wnd::update_drp() {
    ui->pan_wait->setVisible(true);
    int lim = 1000;
    int cnt = 0;
    int cnt_sum = 0;
    f_pause = false;
    f_stop = false;

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  подготовка к обновлению связок с ДРП");
    QApplication::processEvents();
    db.transaction();

    // сначала выставим флаги всем персонам, у кого текущий ДРП не совпадает с ДРП по связке
    QString sql;
    sql = "update persons e "
          "   set _update_drp=1 "
          "  where e.category in (2, 7, 3, 8, 4, 9, 11, 12, 13, 14) "
          "    and e.id in(select distinct e.id "
          "        from persons e "
          "        left join ( select drp.id_person, max(drp.id) as id_drp "
          "                      from persons_docs drp "
          "                     where drp.status=1 "
          "                       and drp.doc_date<=CURRENT_DATE "
          "                       and ( drp.doc_exp is NULL "
          "                          or drp.doc_exp>CURRENT_DATE )"
          "                     group by drp.id_person ) q_drp "
          "          on (q_drp.id_person=e.id) "
          "    where _update_drp<>1 "
          "      and e.id_drp<>q_drp.id_drp ) ";

    QSqlQuery *query = new QSqlQuery(db);
    bool res = query->exec(sql);

    if (!res) {
        QMessageBox::warning(this, "Ошибка обновления связок ДРП",
                             "При попытке обновить связки ДРП произошла неожиданная ошибка.\n\n"
                             + query->lastError().driverText() + "\n" + query->lastError().databaseText() + "\n\n"
                             "Запись пропущена.");
        delete query;
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->bn_process->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return false;
    }
    delete query;
    db.commit();
    db.transaction();

    do {
        QApplication::processEvents();
        db.transaction();

        if (ui->rb_1->isChecked())  lim=1;
        if (ui->rb_21->isChecked())  lim=21;
        if (ui->rb_231->isChecked())  lim=231;
        if (ui->rb_2341->isChecked())  lim=2341;
        if (ui->rb_23451->isChecked())  lim=23451;

        // подготовим функцию обновления связок
        QString sql;
        sql = "select #FUNC_NAME#("
              " " + (ui->ch_date->isChecked() ? "cast('" + ui->date->date().toString("yyyy-MM-dd") + "' as date) " : QString("CURRENT_DATE")) + ", "
              "  " + QString::number(lim) + ") ; ";

        if (ui->ch_old->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links_drp");
        else if (ui->ch_null->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links_drp_null");
        else if (ui->ch_all->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links_drp_all");

        // вызовем функцию обновления связок ДРП в цикле
        ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -   " + sql);
        QApplication::processEvents();

        QSqlQuery *query = new QSqlQuery(db);
        bool res = query->exec(sql);

        if (!res) {
            QMessageBox::warning(this, "Ошибка обновления всех связок",
                                 "При попытке обновить все связки произошла неожиданная ошибка.\n\n"
                                 + query->lastError().driverText() + "\n" + query->lastError().databaseText() + "\n\n"
                                 "Запись пропущена.");
            delete query;
            db.rollback();

            ui->bn_close->setEnabled(true);
            ui->bn_process->setEnabled(true);
            ui->pan_wait->setVisible(false);
            return false;
        }
        query->next();
        cnt = query->value(0).toInt();
        cnt_sum += cnt;
        ui->cnt_drp->setText(show_num(cnt_sum));

        delete query;
        db.commit();

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

        // контроль кнопок "Пауза" и "Стоп"
        QApplication::processEvents();
        if (ui->bn_pause->hasFocus())
            on_bn_pause_clicked(true);
        QApplication::processEvents();
        if (ui->bn_stop->hasFocus())
            on_bn_stop_clicked(true);
        QApplication::processEvents();

        if (f_pause) {
            f_pause = false;
            ui->bn_pause->setChecked(false);
            if (QMessageBox::information(this, "Пауза",
                                     "Последняя транзакция закрыта.\n"
                                     "Для приостановки работы просто не закрывайте это окно.\n\n"
                                     "Для продолжения работы нажмите [Yes]\n"
                                     "Для остановки процесса нажмите [No]\n",
                                     QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)==QMessageBox::No) {
                break;
            }
        }
        if (f_stop)  break;
    }
    while ( cnt>0 );

    ui->bn_close->setEnabled(true);
    ui->bn_process->setEnabled(true);
    ui->pan_wait->setVisible(false);
    ui->bn_pause->setEnabled(false);
    ui->bn_stop->setEnabled(false);
    ui->bn_pause->setChecked(false);
    ui->bn_stop->setChecked(false);

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  Готово. Обновлено " + QString::number(cnt_sum) + " связок персон и ДРП.");
    return true;
}
bool reindex_wnd::update_asg() {
    ui->pan_wait->setVisible(true);
    int lim = 1000;
    int cnt = 0;
    int cnt_sum = 0;
    f_pause = false;
    f_stop = false;

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  подготовка к обновлению связок с прикреплением");
    QApplication::processEvents();
    db.transaction();

    // сначала выставим флаги всем персонам, у кого текущее прикрепление не совпадает с прикреплением по связке
    QString sql;
    sql = "update persons e "
          "   set _update_asg=1 "
          " where e.id in ( "
          "          select distinct la.id_person "
          "            from persons e "
          "            left join link_persons_asg la on(la.id_person=e.id) "
          "            left join ( select asg.id_person, max(asg.id) as id_asg "
          "                          from persons_assig asg "
          "                         where asg.assig_date<=CURRENT_DATE "
          "                           and ( asg.unsig_date is NULL "
          "                              or asg.unsig_date>CURRENT_DATE ) "
          "                        group by asg.id_person ) q_asg "
          "                   on (q_asg.id_person=la.id_person) "
          "           where e._update_asg<>1 "
          "             and la._id_asg<>q_asg.id_asg ); ";

    QSqlQuery *query = new QSqlQuery(db);
    bool res = query->exec(sql);

    if (!res) {
        QMessageBox::warning(this, "Ошибка обновления связок прикреплений",
                             "При попытке обновить связки прикреплений произошла неожиданная ошибка.\n\n"
                             + query->lastError().driverText() + "\n" + query->lastError().databaseText() + "\n\n"
                             "Запись пропущена.");
        delete query;
        db.rollback();

        ui->bn_close->setEnabled(true);
        ui->bn_process->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return false;
    }
    delete query;
    db.commit();
    db.transaction();

    do {
        QApplication::processEvents();
        db.transaction();

        if (ui->rb_1->isChecked())  lim=1;
        if (ui->rb_21->isChecked())  lim=21;
        if (ui->rb_231->isChecked())  lim=231;
        if (ui->rb_2341->isChecked())  lim=2341;
        if (ui->rb_23451->isChecked())  lim=23451;

        // подготовим функцию обновления связок
        sql = "select #FUNC_NAME#("
              " " + (ui->ch_date->isChecked() ? "cast('" + ui->date->date().toString("yyyy-MM-dd") + "' as date) " : QString("CURRENT_DATE")) + ", "
              "  " + QString::number(lim) + ") ; ";

        if (ui->ch_old->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links_asg");
        else if (ui->ch_null->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links_asg_null");
        else if (ui->ch_all->isChecked())
            sql = sql.replace("#FUNC_NAME#", "update_pers_links_asg_all");

        // вызовем функцию обновления связок прикреплений в цикле
        ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -   " + sql);
        QApplication::processEvents();

        query = new QSqlQuery(db);
        res = query->exec(sql);

        if (!res) {
            QMessageBox::warning(this, "Ошибка обновления связок прикреплений",
                                 "При попытке обновить связки прикреплений произошла неожиданная ошибка.\n\n"
                                 + query->lastError().driverText() + "\n" + query->lastError().databaseText() + "\n\n"
                                 "Запись пропущена.");
            delete query;
            db.rollback();

            ui->bn_close->setEnabled(true);
            ui->bn_process->setEnabled(true);
            ui->pan_wait->setVisible(false);
            return false;
        }
        query->next();
        cnt = query->value(0).toInt();
        cnt_sum += cnt;
        ui->cnt_asg->setText(show_num(cnt_sum));

        delete query;
        db.commit();

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

        // контроль кнопок "Пауза" и "Стоп"
        QApplication::processEvents();
        if (ui->bn_pause->hasFocus())
            on_bn_pause_clicked(true);
        QApplication::processEvents();
        if (ui->bn_stop->hasFocus())
            on_bn_stop_clicked(true);
        QApplication::processEvents();

        if (f_pause) {
            f_pause = false;
            ui->bn_pause->setChecked(false);
            if (QMessageBox::information(this, "Пауза",
                                     "Последняя транзакция закрыта.\n"
                                     "Для приостановки работы просто не закрывайте это окно.\n\n"
                                     "Для продолжения работы нажмите [Yes]\n"
                                     "Для остановки процесса нажмите [No]\n",
                                     QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)==QMessageBox::No) {
                break;
            }
        }
        if (f_stop)  break;
    }
    while ( cnt>0 );

    ui->bn_close->setEnabled(true);
    ui->bn_process->setEnabled(true);
    ui->pan_wait->setVisible(false);
    ui->bn_pause->setEnabled(false);
    ui->bn_stop->setEnabled(false);
    ui->bn_pause->setChecked(false);
    ui->bn_stop->setChecked(false);

    ui->te_log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  -  -  Готово. Обновлено " + QString::number(cnt_sum) + " связок персон и прикреплений.");
    return true;
}

void reindex_wnd::on_bn_pause_clicked(bool checked) {
    ui->bn_pause->setChecked(true);
    ui->bn_stop->setChecked(false);
    f_pause = true;
    f_stop = false;
}

void reindex_wnd::on_bn_stop_clicked(bool checked) {
    ui->bn_pause->setChecked(false);
    ui->bn_stop->setChecked(true);
    f_pause = false;
    f_stop = true;
}
