#include "assiglays_wnd.h"
#include "ui_assiglays_wnd.h"

#include "ki_funcs.h"

assiglays_wnd::assiglays_wnd(int layers_type, QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    layers_type(layers_type),
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::assiglays_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    assiglay_data_w = NULL;
    assiglay_tfoms_master_w = NULL;
    add_sexage_w = NULL;
    show_sexage_w = NULL;
    load_sexage_w = NULL;
    add_move_w = NULL;
    add_matrix_w = NULL;
    show_move_w = NULL;
    show_tab_w = NULL;

    l500.clear();
    l500.append(300);
    l500.append(0);
    l500.append(0);

    l501.clear();
    l501.append(200);
    l501.append(0);
    l501.append(100);

    l510.clear();
    l510.append(200);
    l510.append(100);
    l510.append(0);

    l511.clear();
    l511.append(200);
    l511.append(100);
    l511.append(100);

    ui->combo_layers_type->setCurrentIndex(layers_type);
    ui->pan_wait->setVisible(false);

    switch (layers_type) {
    case 1:  ui->bn_add_inko->setVisible(true);
        ui->bn_add_tfoms->setVisible(false);
        break;
    case 2:  ui->bn_add_inko->setVisible(false);
        ui->bn_add_tfoms->setVisible(true);
        break;
    }

    ui->split_assiglays->setSizes(l511);

    on_bn_refresh_assiglays_clicked();
}

assiglays_wnd::~assiglays_wnd() {
    delete ui;
}

void assiglays_wnd::on_bn_close_clicked() {
    if (QMessageBox::question(this, "Закрыть окно?",
                              "Вы действительно хотите закрыть окно снимков прикрепления?",
                              QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::No)
        return;
    close();
}

void assiglays_wnd::on_combo_layers_type_currentIndexChanged(int index) {
    this->layers_type = index;
    switch (index) {
    case 1:  ui->bn_add_inko->setVisible(true);
        ui->bn_add_tfoms->setVisible(false);
        break;
    case 2:  ui->bn_add_inko->setVisible(false);
        ui->bn_add_tfoms->setVisible(true);
        break;
    }
    on_bn_refresh_assiglays_clicked();
}

void assiglays_wnd::on_bn_refresh_assiglays_clicked() {
    refresh_assiglays_tab();
}

void assiglays_wnd::refresh_assiglays_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    ui->pan_assiglays->setEnabled(false);
    ui->pan_sexages->setEnabled(false);
    ui->pan_moves->setEnabled(false);
    ui->pan_DN->setEnabled(false);

    ui->pan_sexages->setEnabled(false);
    ui->pan_moves->setEnabled(false);
    QApplication::processEvents();

    QString layers_types = "(0,1,2)";

    switch (ui->combo_layers_type->currentIndex()) {
    case 1:  layers_types = "(1)";
        break;
    case 2:  layers_types = "(2)";
        break;
    }

    sql_assiglays = "select l.id, l.layer_name, l.layer_date, l.layer_type, "
                    "       case l.layer_type when 1 then 'ИНКО' when 2 then 'ТФОМС' else '-???-' end as type, "
                    "       l.cnt_recs, l.cnt_uniq, "
                    "       q_sexages.cnt, q_moves.cnt, "
                    "       l.dt_ins, l.dt_upd "
                    "  from assig.layers l "
                    //  "  left join assig.sexages_data d on(d.id_layer=l.id) "
                    "  left join (select s.id_layer, count(*) as cnt from assig.sexages s group by s.id_layer) q_sexages on(q_sexages.id_layer=l.id) "
                    "  left join (select m.id_layer_new, count(*) as cnt from assig.moves m group by m.id_layer_new) q_moves on(q_moves.id_layer_new=l.id) "
                    " where l.layer_type in " + layers_types + " "
                    " order by layer_date desc, layer_type desc ; ";
    
    model_assiglays.setQuery(sql_assiglays,db);
    QString err2 = model_assiglays.lastError().driverText();

    // подключаем модель из БД
    ui->tab_assiglays->setModel(&model_assiglays);

    // обновляем таблицу
    ui->tab_assiglays->reset();

    // задаём ширину колонок
    ui->tab_assiglays->setColumnWidth(  0,  2);     // id
    ui->tab_assiglays->setColumnWidth(  1,250);     // layer_name
    ui->tab_assiglays->setColumnWidth(  2, 90);     // layer_date
    ui->tab_assiglays->setColumnWidth(  3,  2);     // layer_type
    ui->tab_assiglays->setColumnWidth(  4, 60);     // layer_type
    ui->tab_assiglays->setColumnWidth(  5,100);     // l.cnt_recs
    ui->tab_assiglays->setColumnWidth(  6, 70);     // l.cnt_uniq
    ui->tab_assiglays->setColumnWidth(  7, 80);     // q_sexages.cnt
    ui->tab_assiglays->setColumnWidth(  8, 80);     // q_moves.cnt
    ui->tab_assiglays->setColumnWidth(  9,  2);     // dt_ins
    ui->tab_assiglays->setColumnWidth( 10,  2);     // dt_upd

    // правим заголовки
    model_assiglays.setHeaderData(  0, Qt::Horizontal, ("ID"));
    model_assiglays.setHeaderData(  1, Qt::Horizontal, ("имя снимка данных"));
    model_assiglays.setHeaderData(  2, Qt::Horizontal, ("дата снимка \nданных"));
    model_assiglays.setHeaderData(  3, Qt::Horizontal, ("тип \nснимка"));
    model_assiglays.setHeaderData(  4, Qt::Horizontal, ("тип \nснимка"));
    model_assiglays.setHeaderData(  5, Qt::Horizontal, ("число строк\nданных"));
    model_assiglays.setHeaderData(  6, Qt::Horizontal, ("число \nперсон"));
    model_assiglays.setHeaderData(  7, Qt::Horizontal, ("таблицы \nполовозраст."));
    model_assiglays.setHeaderData(  8, Qt::Horizontal, ("таблицы \nдвижения"));
    ui->tab_assiglays->repaint();

    ui->pan_assiglays->setEnabled(true);
    ui->pan_sexages->setEnabled(true);
    ui->pan_moves->setEnabled(true);
    ui->pan_DN->setEnabled(true);
    QApplication::processEvents();
    this->setCursor(Qt::ArrowCursor);
}

void assiglays_wnd::on_bn_add_inko_clicked() {
    ui->lab_wait->setText("   ... ПОДОЖДИТЕ, РАБОТАЮ ...   ");
    ui->pan_wait->setVisible(true);
    ui->lab_time_all->setText("00:00:00");
    ui->lab_time->setText("00:00:00");
    ui->lab_cnt->setText(" -//- ");
    QApplication::processEvents();

    s_assiglay_data data;
    data.layer_date = QDate(QDate::currentDate().year(),QDate::currentDate().month(),1);
    data.layer_name = "ASSIG_"
            + QString("0000" + QString::number(QDate::currentDate().year())).right(4)
            + QString("00" + QString::number(QDate::currentDate().month())).right(2)
            + "01_INKO";
    data.layer_type = 1;

    delete assiglay_data_w;
    assiglay_data_w = new assiglay_data_wnd(data, data_app, this);
    this->setCursor(Qt::WaitCursor);

    if (assiglay_data_w->exec()) {
        ui->pan_assiglays->setEnabled(false);
        ui->pan_sexages->setEnabled(false);
        ui->pan_moves->setEnabled(false);
        ui->pan_DN->setEnabled(false);
        ui->pan_wait->setVisible(true);
        QApplication::processEvents();

        // проверим, можно ли создать такой снимок данных
        QString sql_test = "select count(*) as cnt \n"
                           "  from assig.layers \n"
                           " where (layer_type=" + QString::number(data.layer_type) + " and layer_date='" + data.layer_date.toString("yyyy-MM-dd") + "') \n"
                           "    or layer_name='" + data.layer_name.toUpper() + "' ; ";
        QSqlQuery *query_test = new QSqlQuery(db);
        QString sql = sql_test.replace("\n","");
        bool res_test = query_test->exec(sql);
        if (!res_test) {
            QString err_db = query_test->lastError().databaseText();
            QString err_drv = query_test->lastError().driverText();
            delete query_test;
            this->setCursor(Qt::ArrowCursor);
            QMessageBox::warning(this, "Ошибка чтения данных",
                                 "При попытке проверить существование готового снимка произошла ошибка.\n"
                                 + sql_test + "\n\n"
                                 + err_db   + "\n"
                                 + err_drv  + "\n\n"
                                 "Операция отменена.\n");
            ui->pan_assiglays->setEnabled(true);
            ui->pan_sexages->setEnabled(true);
            ui->pan_moves->setEnabled(true);
            ui->pan_DN->setEnabled(true);
            ui->pan_wait->setVisible(false);
            QApplication::processEvents();

            return;
        }
        int cnt_layers = 0;
        while (query_test->next()) {
            cnt_layers = query_test->value(0).toInt();
        }
        if (cnt_layers>0) {
            delete query_test;
            this->setCursor(Qt::ArrowCursor);
            QMessageBox::warning(this, "Невозможно создать такой снимок данных",
                                 "Уже есть " + QString::number(cnt_layers) + " снимков с таким же именем или датой.\n"
                                 "Имя и дата снимка должны быть уникальными.\n"
                                 "Операция отменена.\n");
            ui->pan_assiglays->setEnabled(true);
            ui->pan_sexages->setEnabled(true);
            ui->pan_moves->setEnabled(true);
            ui->pan_DN->setEnabled(true);
            ui->pan_wait->setVisible(false);
            QApplication::processEvents();

            return;
        }
        delete query_test;

        // добавим запись снимка
        db.transaction();
        QString sql_layer = "insert into assig.layers(layer_name, layer_date, layer_type) "
                            "values( '" + data.layer_name + "', "
                                   " '" + data.layer_date.toString("yyyy-MM-dd") + "', "
                                   "  " + QString::number(data.layer_type) + " ) "
                            " returning id ; ";
        QSqlQuery *query_layer = new QSqlQuery(db);
        bool res_layer = mySQL.exec(this,sql_layer,"Создание записи нового снимка",query_layer,false, db, data_app);

        if (!res_layer) {
            delete query_layer;
            db.rollback();
            this->setCursor(Qt::ArrowCursor);
            QMessageBox::warning(this, "Ошибка",
                                 "При попытке создать запись нового снимка произошла неожиданная ошибка.\n\n"
                                 "Операция отменена.");
            ui->pan_assiglays->setEnabled(true);
            ui->pan_sexages->setEnabled(true);
            ui->pan_moves->setEnabled(true);
            ui->pan_DN->setEnabled(true);
            ui->pan_wait->setVisible(false);
            QApplication::processEvents();

            return;
        }
        // получим ID снимка
        int id_layer = -1;
        while (query_layer->next())
            id_layer = query_layer->value(0).toInt();

        // зальём данные прикрепления в таблицу для хранения снимков прикрепления
        QString sql_data = "insert into assig.sexages_data "
                           "     ( id_layer, id_sexage_line, cell_name, "
                           "       id_person, fam, im, ot, sex, date_birth, snils, "
                           "       id_polis, pol_v, vs_num, enp, pol_ser, pol_num, "
                           "       assig_type, assig_way, assig_date, code_mo, code_head, code_mp, code_ms, snils_mt, "
                           "       subj_name, dstr_name, city_name, nasp_name, strt_name, house, corp, quart ) "
                           "select " + QString::number(id_layer) + ", NULL, NULL, "
                           "       e.id as id_person, e.fam, e.im, e.ot, e.sex, e.date_birth, e.snils, "
                           "       p.id as id_polis, p.pol_v, p.vs_num, p.enp, p.pol_ser, p.pol_num, "
                           "       a.assig_type, a.assig_way, a.assig_date, a.code_mo, mo.code_head, a.code_mp, a.code_ms, a.snils_mt, "
                           "       adr.subj_name, adr.dstr_name, adr.city_name, adr.nasp_name, adr.strt_name, adr.house, adr.corp, adr.quart "
                           "  from persons e "
                           "  left join polises p on(p.id=( select * from id_person_pol(e.id, '" + data.layer_date.toString("yyyy-MM-dd") + "')) ) "
                          //"  left join link_persons_asg la on(la.id_person=e.id) "
                           "  left join persons_assig a on( a.id=(select max(aa.id) "
                           "                                        from persons_assig aa "
                           "                                       where aa.id_person=e.id "
                           "                                         and ( aa.assig_date<='" + data.layer_date.toString("yyyy-MM-dd") + "' ) "
                           "                                         and ( aa.unsig_date is NULL "
                           "                                            or aa.unsig_date>'" + data.layer_date.toString("yyyy-MM-dd") + "')) ) "
                           "  left join spr_medorgs mo on(mo.code_mo=a.code_mo) "
                           "  left join addresses adr on(adr.id=e.id_addr_reg) "
                           " where p.id is not NULL "
                           "   /*and a.id is not NULL*/ ; ";    // в снимок ИНКО попадает всё застрахованное наследение, независимо от прикрепления
        QSqlQuery *query_data = new QSqlQuery(db);
        bool res_data = mySQL.exec(this, sql_data, "Формирования снимка прикрепления на указанную дату", query_data,false, db, data_app);

        if (!res_data) {
            delete query_layer;
            delete query_data;
            db.rollback();
            this->setCursor(Qt::ArrowCursor);
            QMessageBox::warning(this, "Ошибка",
                                 "При попытке сформировать данные снимка прикрепления произошла неожиданная ошибка.\n\n"
                                 "Операция отменена.");
            ui->pan_assiglays->setEnabled(true);
            ui->pan_sexages->setEnabled(true);
            ui->pan_moves->setEnabled(true);
            ui->pan_DN->setEnabled(true);
            ui->pan_wait->setVisible(false);
            QApplication::processEvents();

            return;
        }
        delete query_layer;
        delete query_data;

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
            ui->pan_assiglays->setEnabled(true);
            ui->pan_sexages->setEnabled(true);
            ui->pan_moves->setEnabled(true);
            ui->pan_DN->setEnabled(true);
            ui->pan_wait->setVisible(false);
            return;
        }
        delete query_stat;


        db.commit();
        this->setCursor(Qt::ArrowCursor);
        on_bn_refresh_assiglays_clicked();


        //------------------------------//
        // отображение статуса на экран //
        //------------------------------//
        ui->lab_cnt->setText(" -//- ");

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

        QApplication::processEvents();

        refresh_assiglays_tab();
        refresh_sexages_tab();
        refresh_moves_tab();
    } else {
        QMessageBox::information(this, "Отмена операции",
                                 "Действие отменено пользователем");
    }
    ui->pan_assiglays->setEnabled(true);
    ui->pan_sexages->setEnabled(true);
    ui->pan_moves->setEnabled(true);
    ui->pan_DN->setEnabled(true);
    ui->pan_wait->setVisible(false);
    this->setCursor(Qt::ArrowCursor);
    QApplication::processEvents();
}



void assiglays_wnd::on_bn_add_tfoms_clicked() {
    delete assiglay_tfoms_master_w;
    assiglay_tfoms_master_w = new assiglay_tfoms_master_wnd(db, data_app, settings, this);

    this->setCursor(Qt::WaitCursor);
    ui->pan_assiglays->setEnabled(false);
    ui->pan_sexages->setEnabled(false);
    ui->pan_moves->setEnabled(false);
    ui->pan_DN->setEnabled(false);
    ui->pan_wait->setVisible(true);
    ui->lab_wait->setText("   ... ПОДОЖДИТЕ, РАБОТАЮ ...   ");
    ui->pan_wait->setVisible(true);
    QApplication::processEvents();

    assiglay_tfoms_master_w->exec();
    refresh_assiglays_tab();
    refresh_sexages_tab();
    refresh_moves_tab();

    ui->pan_assiglays->setEnabled(true);
    ui->pan_sexages->setEnabled(true);
    ui->pan_moves->setEnabled(true);
    ui->pan_DN->setEnabled(true);
    ui->pan_wait->setVisible(false);
    this->setCursor(Qt::ArrowCursor);
    QApplication::processEvents();
}

void assiglays_wnd::on_ch_show_sexages_clicked(bool checked) {
    if ( !ui->ch_show_sexages->isChecked()
      && !ui->ch_show_moves->isChecked() ) {
        ui->split_assiglays->setSizes(l500);
        ui->split_assiglays->setStretchFactor(0,5);
        ui->split_assiglays->setStretchFactor(1,0);
        ui->split_assiglays->setStretchFactor(2,0);
    } else
    if ( ui->ch_show_sexages->isChecked()
      && !ui->ch_show_moves->isChecked() ) {
        ui->split_assiglays->setSizes(l510);
        ui->split_assiglays->setStretchFactor(0,5);
        ui->split_assiglays->setStretchFactor(1,1);
        ui->split_assiglays->setStretchFactor(2,0);
        refresh_sexages_tab();
    } else
    if ( !ui->ch_show_sexages->isChecked()
      && ui->ch_show_moves->isChecked() ) {
        ui->split_assiglays->setSizes(l501);
        ui->split_assiglays->setStretchFactor(0,5);
        ui->split_assiglays->setStretchFactor(1,0);
        ui->split_assiglays->setStretchFactor(2,1);
        refresh_moves_tab();
    } else
    if ( ui->ch_show_sexages->isChecked()
      && ui->ch_show_moves->isChecked() ) {
        ui->split_assiglays->setSizes(l511);
        ui->split_assiglays->setStretchFactor(0,5);
        ui->split_assiglays->setStretchFactor(1,1);
        ui->split_assiglays->setStretchFactor(2,1);
        refresh_sexages_tab();
        refresh_moves_tab();
    }
}

void assiglays_wnd::on_ch_show_moves_clicked(bool checked) {
    if ( !ui->ch_show_sexages->isChecked()
      && !ui->ch_show_moves->isChecked() ) {
        ui->split_assiglays->setSizes(l500);
        ui->split_assiglays->setStretchFactor(0,5);
        ui->split_assiglays->setStretchFactor(1,0);
        ui->split_assiglays->setStretchFactor(2,0);
    } else
    if ( ui->ch_show_sexages->isChecked()
      && !ui->ch_show_moves->isChecked() ) {
        ui->split_assiglays->setSizes(l510);
        ui->split_assiglays->setStretchFactor(0,5);
        ui->split_assiglays->setStretchFactor(1,1);
        ui->split_assiglays->setStretchFactor(2,0);
        refresh_sexages_tab();
    } else
    if ( !ui->ch_show_sexages->isChecked()
      && ui->ch_show_moves->isChecked() ) {
        ui->split_assiglays->setSizes(l501);
        ui->split_assiglays->setStretchFactor(0,5);
        ui->split_assiglays->setStretchFactor(1,0);
        ui->split_assiglays->setStretchFactor(2,1);
        refresh_moves_tab();
    } else
    if ( ui->ch_show_sexages->isChecked()
      && ui->ch_show_moves->isChecked() ) {
        ui->split_assiglays->setSizes(l511);
        ui->split_assiglays->setStretchFactor(0,5);
        ui->split_assiglays->setStretchFactor(1,1);
        ui->split_assiglays->setStretchFactor(2,1);
        refresh_sexages_tab();
        refresh_moves_tab();
    }
}


void assiglays_wnd::refresh_sexages_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    ui->pan_sexages->setEnabled(false);
    QApplication::processEvents();

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_assiglays->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        int id_assiglay = model_assiglays.data(model_assiglays.index(index.row(), 0), Qt::EditRole).toInt();       // e.id

        sql_sexages = "select s.id, s.id_layer, "
                      "       l.layer_name, l.layer_date, l.layer_type, case when l.layer_type=1 then 'ИНКО' else 'ТФОМС' end as layer_type_text, "
                      "       s.assig_type, case s.assig_type when 1 then 'по терр.' when 2 then 'по заявл.' else '- все -' end, "
                      "       s.cnt_lines, s.cnt_recs, s.cnt_uniq, s.source "
                      "  from assig.sexages s "
                      "  left join assig.layers l on(l.id=s.id_layer) "
                      " where s.id_layer=" + QString::number(id_assiglay) + " ; ";

        model_sexages.setQuery(sql_sexages,db);
        QString err2 = model_sexages.lastError().driverText();

        // подключаем модель из БД
        ui->tab_sexages->setModel(&model_sexages);

        // обновляем таблицу
        ui->tab_sexages->reset();

        // задаём ширину колонок
        ui->tab_sexages->setColumnWidth(  0,  2);   // s.id,
        ui->tab_sexages->setColumnWidth(  1,  2);   // s.id_layer,
        ui->tab_sexages->setColumnWidth(  2,140);   // l.layer_name,
        ui->tab_sexages->setColumnWidth(  3,100);   // l.layer_date,
        ui->tab_sexages->setColumnWidth(  4,  2);   // l.layer_type,
        ui->tab_sexages->setColumnWidth(  5, 70);   // case when l.layer_type=1 then 'ИНКО' else 'ТФОМС' end as layer_type,
        ui->tab_sexages->setColumnWidth(  6,  2);   // assig_type
        ui->tab_sexages->setColumnWidth(  7, 70);   // case s.assig_type when 1 then 'терр.' when 2 then 'заяв.' else '- все -' end
        ui->tab_sexages->setColumnWidth(  8, 90);   // cnt_lines
        ui->tab_sexages->setColumnWidth(  9, 90);   // cnt_recs
        ui->tab_sexages->setColumnWidth( 10, 65);   // cnt_uniq

        // правим заголовки
        model_sexages.setHeaderData(  1, Qt::Horizontal, ("ID снимка"));
        model_sexages.setHeaderData(  2, Qt::Horizontal, ("название \nснимка данных"));
        model_sexages.setHeaderData(  3, Qt::Horizontal, ("дата снимка \nданных"));
        model_sexages.setHeaderData(  4, Qt::Horizontal, ("тип\nснимка"));
        model_sexages.setHeaderData(  5, Qt::Horizontal, ("тип\nснимка"));
        model_sexages.setHeaderData(  7, Qt::Horizontal, ("способ\nприкрепл."));
        model_sexages.setHeaderData(  8, Qt::Horizontal, ("число строк\nв таблице"));
        model_sexages.setHeaderData(  9, Qt::Horizontal, ("число строк\nданных"));
        model_sexages.setHeaderData( 10, Qt::Horizontal, ("число\nперсон"));

        ui->pan_sexages->setEnabled(true);
        QApplication::processEvents();
    }
    int cnt_rows = model_sexages.rowCount();
    if (cnt_rows>0) {
        ui->bn_add_sexage->setEnabled((true));
        ui->bn_open_sexage->setEnabled(true);
        ui->bn_load_TFOMS->setEnabled(true);
        ui->bn_delete_sexage->setEnabled(true);
    } else {
        ui->bn_add_sexage->setEnabled(true);
        ui->bn_open_sexage->setEnabled(false);
        ui->bn_load_TFOMS->setEnabled(false);
        ui->bn_delete_sexage->setEnabled(false);
    }

    this->setCursor(Qt::ArrowCursor);
}


void assiglays_wnd::refresh_moves_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Нет доступа к базе данных",
                             "Нет открытого соединения к базе данных?\n "
                             "Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n "
                             "Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику.");
        return;
    }
    ui->pan_moves->setEnabled(false);
    QApplication::processEvents();

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_assiglays->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        int id_assiglay = model_assiglays.data(model_assiglays.index(index.row(), 0), Qt::EditRole).toInt();       // e.id

        sql_moves = "select m.id, m.id_layer_old, l0.layer_date as layer_date_old, case when l0.layer_type=1 then 'ИНКО' else 'ТФОМС' end as layer_type_old, "
                    "       m.id_layer_new, l1.layer_date as layer_date_new, case when l1.layer_type=1 then 'ИНКО' else 'ТФОМС' end as layer_type_new, "
                    "       m.assig_type, case m.assig_type when 1 then 'по терр.' when 2 then 'по заявл.' else '- все -' end, "
                    "       m.cnt_lines, m.source "
                    "  from assig.moves  m "
                    "  left join assig.layers l0 on(l0.id=m.id_layer_old) "
                    "  left join assig.layers l1 on(l1.id=m.id_layer_new) "
                    " where m.id_layer_new=" + QString::number(id_assiglay) + " ; ";

        model_moves.setQuery(   sql_moves,db);
        QString err2 = model_moves.lastError().driverText();

        // подключаем модель из БД
        ui->tab_moves->setModel(&model_moves);

        // обновляем таблицу
        ui->tab_moves->reset();

        // задаём ширину колонок
        ui->tab_moves->setColumnWidth(  0,  2);   // m.id,
        ui->tab_moves->setColumnWidth(  1,  2);   // m.id_layer_old,
        ui->tab_moves->setColumnWidth(  2,110);   // l0.layer_date_old,
        ui->tab_moves->setColumnWidth(  3, 90);   // case when l0.layer_type=1 then 'ИНКО' else 'ТФОМС' end as layer_type_old,
        ui->tab_moves->setColumnWidth(  4,  2);   // m.id_layer_new,
        ui->tab_moves->setColumnWidth(  5,110);   // l1.layer_date_new,
        ui->tab_moves->setColumnWidth(  6, 90);   // case when l1.layer_type=1 then 'ИНКО' else 'ТФОМС' end as layer_type_new,
        ui->tab_moves->setColumnWidth(  7,  2);   // m.assig_type,
        ui->tab_moves->setColumnWidth(  8, 70);   // case m.assig_type when 1 then 'терр.' when 2 then 'заяв.' else '- все -' end,
        ui->tab_moves->setColumnWidth(  9, 90);   // count(s.code_mo) as cnt_medorgs

        // правим заголовки
        model_moves.setHeaderData(  1, Qt::Horizontal, ("ID старого\nснимка"));
        model_moves.setHeaderData(  2, Qt::Horizontal, ("дата старого \nснимка данных"));
        model_moves.setHeaderData(  3, Qt::Horizontal, ("тип старого\nснимка"));
        model_moves.setHeaderData(  4, Qt::Horizontal, ("ID нового\nснимка"));
        model_moves.setHeaderData(  5, Qt::Horizontal, ("дата нового \nснимка данных"));
        model_moves.setHeaderData(  6, Qt::Horizontal, ("тип нового \nснимка"));
        model_moves.setHeaderData(  8, Qt::Horizontal, ("способ\nприкрепл."));
        model_moves.setHeaderData(  9, Qt::Horizontal, ("число строк\nв таблице"));

        ui->pan_moves->setEnabled(true);
        QApplication::processEvents();
    }
    int cnt_rows = model_moves.rowCount();
    if (cnt_rows>0) {
        ui->bn_add_move->setEnabled((true));
        ui->bn_open_move->setEnabled(true);
        ui->bn_delete_move->setEnabled(true);
    } else {
        ui->bn_add_move->setEnabled(true);
        ui->bn_open_move->setEnabled(false);
        ui->bn_delete_move->setEnabled(false);
    }

    this->setCursor(Qt::ArrowCursor);
}

void assiglays_wnd::on_tab_assiglays_clicked(const QModelIndex &index) {
    refresh_sexages_tab();
    refresh_moves_tab();
}

void assiglays_wnd::on_bn_add_sexage_clicked() {
    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_assiglays->selectionModel()->selection().indexes();
    this->setCursor(Qt::WaitCursor);

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        int id_layer = model_assiglays.data(model_assiglays.index(index.row(), 0), Qt::EditRole).toInt();       // e.id
        int layer_type = model_assiglays.data(model_assiglays.index(index.row(), 3), Qt::EditRole).toInt();     // assiglay_type
        int assig_type = 0;     // assig_type
        QDate layer_date = model_assiglays.data(model_assiglays.index(index.row(), 2), Qt::EditRole).toDate();     // assiglay_date

        delete add_sexage_w;
        add_sexage_w = new add_sexage_wnd(id_layer, layer_type, layer_date, assig_type, db, data_app, settings, this);

        if (add_sexage_w->exec()) {
            // проверим, можно ли добавить новую таблицу с такими параметрами
            QString sql_test = "select count(*) "
                               "  from assig.sexages s "
                               " where s.id_layer="   + QString::number(id_layer)   + " "
                               "   and s.assig_type=" + QString::number(assig_type) + " ; ";
            QSqlQuery *query_test = new QSqlQuery(db);
            bool res_test = query_test->exec(sql_test);
            if (!res_test) {
                this->setCursor(Qt::ArrowCursor);
                QMessageBox::warning(this, "Ошибка при проверке возможности добавления половозрастной",
                                     "При проверке возможности добавления половозрастной произошла неожиданная ошибка.\n\n"
                                     + query_test->lastError().driverText() + "\n" + query_test->lastError().databaseText() + "\n\n"
                                     "Операция отменена.");
                delete query_test;
                return;
            }
            query_test->next();
            int cnt = query_test->value(0).toInt();
            delete query_test;
            if (cnt>0) {
                this->setCursor(Qt::ArrowCursor);
                QMessageBox::warning(this, "Такая таблица уже есть - отказ",
                                     "В базе данных уже есть табллица половозрастного распределения застрахованных для этого снимка (ID_LAYER=" + QString::number(id_layer) + ") со способом прикрепления - " + QString::number(assig_type) + ".\n\n"
                                     "Параметры таблиц половозрастного распределения не должны повторяться.\n"
                                     "Измените параметры или удалите старую таблицу.\n\n"
                                     "Операция отменена.");
                return;
            }


            // рассчитаем половозрастное распределение застрахованных по снимку
            ui->pan_assiglays->setEnabled(false);
            ui->pan_sexages->setEnabled(false);
            ui->pan_moves->setEnabled(false);
            ui->pan_DN->setEnabled(false);
            ui->pan_wait->setVisible(true);
            ui->lab_wait->setText("   ... ПОДОЖДИТЕ, РАБОТАЮ ...   ");
            QApplication::processEvents();

            db.transaction();

            QString assig_type_d = " ";
            bool f_assig_type_d = false;
            switch (assig_type) {
            case 1:
                assig_type_d = " and d.assig_type=1 ";
                f_assig_type_d = true;
                break;
            case 2:
                assig_type_d = " and d.assig_type=2 ";
                f_assig_type_d = true;
                break;
            default:
                assig_type_d = " and d.assig_type=0 ";
                f_assig_type_d = false;
                break;
            }

            QString assig_type_s = " ";
            bool f_assig_type_s = false;
            switch (assig_type) {
            case 1:
                assig_type_s = " and d.assig_type=1 ";
                f_assig_type_s = true;
                break;
            case 2:
                assig_type_s = " and d.assig_type=2 ";
                f_assig_type_s = true;
                break;
            default:
                assig_type_s = " ";
                f_assig_type_s = false;
                break;
            }

            // добавим запись таблицы
            QString sql_sexage = "insert into assig.sexages "
                                 "       ( id_layer, assig_type )"
                                 "values ( " + QString::number(id_layer) + ", "
                                 "         " + QString::number(assig_type) + "  ) "
                                 "returning id ; ";
            QSqlQuery *query_sexage = new QSqlQuery(db);
            bool res_sexage = query_sexage->exec(sql_sexage);

            if (!res_sexage) {
                this->setCursor(Qt::ArrowCursor);
                QMessageBox::warning(this, "Ошибка при создании записи половозрастной",
                                     "При создании записи половозрастной произошла неожиданная ошибка.\n\n"
                                     + query_sexage->lastError().driverText() + "\n" + query_sexage->lastError().databaseText() + "\n\n"
                                     "Операция отменена.");
                delete query_sexage;
                db.rollback();
                ui->pan_wait->setVisible(false);
                return;
            }
            query_sexage->next();
            int id_sexage = query_sexage->value(0).toInt();
            delete query_sexage;

            // пороговые даты
            QDate date_0  = layer_date;
            QDate date_1  = date_0.addYears(-1);
            QDate date_5  = date_0.addYears(-5);
            QDate date_18 = date_0.addYears(-18);
            QDate date_55 = date_0.addYears(-55);
            QDate date_60 = date_0.addYears(-60);

            QString sql_mo_list;
            // если надо - обновим коды головных медрганизаций
            sql_mo_list = "update assig.sexages_data d "
                          "   set code_head = (select mo.code_head from spr_medorgs mo where mo.code_mo=d.code_mo) "
                          " where d.id_layer=" + QString::number(id_layer) + " "
                          "   and d.code_head is NULL ; ";

            // получим список головных медорганизаций, к которым есть прикреплённые лица
            sql_mo_list+= "select distinct d.code_head "
                          "  from assig.sexages_data d "
                          " where d.id_layer=" + QString::number(id_layer) + " "
                          "   and d.code_head is not NULL "
                          " order by d.code_head ; ";
            QSqlQuery *query_mo_list = new QSqlQuery(db);
            bool res_mo_list = query_mo_list->exec(sql_mo_list);

            if (!res_mo_list) {
                this->setCursor(Qt::ArrowCursor);
                QMessageBox::warning(this, "Ошибка при получении списка медорганизаций",
                                     "При получении списка медорганизаций произошла неожиданная ошибка.\n\n"
                                     + query_mo_list->lastError().driverText() + "\n" + query_mo_list->lastError().databaseText() + "\n\n"
                                     "Операция отменена.");
                delete query_mo_list;
                db.rollback();
                ui->pan_wait->setVisible(false);
                return;
            }

            int mo_cnt = query_mo_list->size() +1;  // +1 - для учёта записей без прикрепления

            date0 = QDate::currentDate();
            time0 = QTime::currentTime();


            // переберём полученные медорганизации
            int mo_n = 0;
            bool is_first = true;
            do {
                mo_n++;
                QString code_head = "NULL";
                if (is_first) {
                    code_head = "NULL";
                } else {
                    code_head = query_mo_list->value(0).toString();
                }
                is_first = false;

                int cnt_m_0_0, cnt_m_1_4, cnt_m_5_17, cnt_m_18_59, cnt_m_60_, cnt_m_sum;
                int cnt_w_0_0, cnt_w_1_4, cnt_w_5_17, cnt_w_18_54, cnt_w_55_, cnt_w_sum;
                int cnt_sum, cnt_snils_mt;


                // ============================= //
                // добавим строку половозрастной //
                // ============================= //
                QString sql_new_line = "insert into assig.sexages_lines "
                                       "       ( id_sexage, id_layer, code_head )"
                                       "values ( " + QString::number(id_sexage) + ", "
                                       "         " + QString::number(id_layer) + ", "
                                       "         " + (code_head=="NULL" ? " NULL " : ("'" + code_head + "'")) + " ) "
                                       "returning id ; ";
                QSqlQuery *query_new_line = new QSqlQuery(db);
                bool res_new_line = query_new_line->exec(sql_new_line);

                if (!res_new_line) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении списка медорганизаций",
                                         "При получении списка медорганизаций произошла неожиданная ошибка.\n\n"
                                         + query_new_line->lastError().driverText() + "\n" + query_new_line->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_new_line;
                    db.rollback();
                    ui->pan_wait->setVisible(false);
                    return;
                }
                query_new_line->next();
                int id_sexage_line = query_new_line->value(0).toInt();
                delete query_new_line;


                // ===== //
                // m_0_0 //
                // ===== //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", М от 0 до 1 ...   ");
                QApplication::processEvents();

                QString sql_m_0_0 = "select count(distinct d.id_person) as cnt_m_0_0 "
                                    "  from assig.sexages_data d "
                                    //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                    " where d.id_layer=" + QString::number(id_layer) + " "
                                    "   and d.sex=1 "
                                    "   and d.date_birth<='" + date_0.toString("yyyy-MM-dd") + "' "
                                    "   and d.date_birth> '" + date_1.toString("yyyy-MM-dd") + "' "
                                    "   and d.code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " " + assig_type_s +
                                    " ; ";
                QSqlQuery *query_m_0_0 = new QSqlQuery(db);
                bool res_m_0_0 = query_m_0_0->exec(sql_m_0_0);

                if (!res_m_0_0) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении числа мужчин от 0 до 1",
                                         "При получении числа мужчин от 0 до 1 произошла неожиданная ошибка.\n\n"
                                         + query_m_0_0->lastError().driverText() + "\n" + query_m_0_0->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_m_0_0;
                    delete query_mo_list;
                    db.rollback();
                    return;
                }
                query_m_0_0->next();
                cnt_m_0_0 = query_m_0_0->value(0).toInt();
                delete query_m_0_0;

                if (cnt_m_0_0>0) {
                    // сформируем список m_0_0
                    QString sql_m_0_0_list = "update assig.sexages_data d "
                                             "   set id_sexage_line=" + QString::number(id_sexage_line) + ", "
                                             "       cell_name='m_0_0' "

                                             " where id_layer=" + QString::number(id_layer) + " "
                                             "   and sex=1 "
                                             "   and date_birth<='" + date_0.toString("yyyy-MM-dd") + "' "
                                             "   and date_birth> '" + date_1.toString("yyyy-MM-dd") + "' "
                                             "   and code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " ; ";
                    QSqlQuery *query_m_0_0_list = new QSqlQuery(db);
                    bool res_m_0_0_list = query_m_0_0_list->exec(sql_m_0_0_list);

                    if (!res_m_0_0_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании списка   мужчин от 0 до 1",
                                             "При формировании списка мужчин от 0 до 1 произошла неожиданная ошибка.\n\n"
                                             + query_m_0_0_list->lastError().driverText() + "\n" + query_m_0_0_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_m_0_0_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_m_0_0_list;
                }


                // ===== //
                // m_1_4 //
                // ===== //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", М от 1 до 5 ...   ");
                QApplication::processEvents();

                QString sql_m_1_4 = "select count(distinct d.id_person) as cnt_m_1_4 "
                                    "  from assig.sexages_data d "
                                    //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                    " where d.id_layer=" + QString::number(id_layer) + " "
                                    "   and d.sex=1 "
                                    "   and d.date_birth<='" + date_1.toString("yyyy-MM-dd") + "' "
                                    "   and d.date_birth> '" + date_5.toString("yyyy-MM-dd") + "' "
                                    "   and d.code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " " + assig_type_s +
                                    " ; ";
                QSqlQuery *query_m_1_4 = new QSqlQuery(db);
                bool res_m_1_4 = query_m_1_4->exec(sql_m_1_4);

                if (!res_m_1_4) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении числа мужчин от 1 до 5",
                                         "При получении числа мужчин от 1 до 5 произошла неожиданная ошибка.\n\n"
                                         + query_m_1_4->lastError().driverText() + "\n" + query_m_1_4->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_m_1_4;
                    delete query_mo_list;
                    db.rollback();
                    ui->pan_wait->setVisible(false);
                    return;
                }
                query_m_1_4->next();
                cnt_m_1_4 = query_m_1_4->value(0).toInt();
                delete query_m_1_4;

                if (cnt_m_1_4>0) {
                    // сформируем список m_1_4
                    QString sql_m_1_4_list = "update assig.sexages_data d "
                                             "   set id_sexage_line=" + QString::number(id_sexage_line) + ", "
                                             "       cell_name='m_1_4' "

                                             " where id_layer=" + QString::number(id_layer) + " "
                                             "   and sex=1 "
                                             "   and date_birth<='" + date_1.toString("yyyy-MM-dd") + "' "
                                             "   and date_birth> '" + date_5.toString("yyyy-MM-dd") + "' "
                                             "   and code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " ; ";
                    QSqlQuery *query_m_1_4_list = new QSqlQuery(db);
                    bool res_m_1_4_list = query_m_1_4_list->exec(sql_m_1_4_list);

                    if (!res_m_1_4_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании списка мужчин от 1 до 5",
                                             "При формировании списка мужчин от 1 до 5 произошла неожиданная ошибка.\n\n"
                                             + query_m_1_4_list->lastError().driverText() + "\n" + query_m_1_4_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_m_1_4_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_m_1_4_list;
                }


                // ====== //
                // m_5_17 //
                // ====== //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", М от 5 до 18 ...   ");
                QApplication::processEvents();

                QString sql_m_5_17 = "select count(distinct d.id_person) as cnt_m_5_17 "
                                     "  from assig.sexages_data d "
                                     //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                     " where d.id_layer=" + QString::number(id_layer) + " "
                                     "   and d.sex=1 "
                                     "   and d.date_birth<='" + date_5.toString("yyyy-MM-dd") + "' "
                                     "   and d.date_birth> '" + date_18.toString("yyyy-MM-dd") + "' "
                                     "   and d.code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " " + assig_type_s +
                                     " ; ";
                QSqlQuery *query_m_5_17 = new QSqlQuery(db);
                bool res_m_5_17 = query_m_5_17->exec(sql_m_5_17);

                if (!res_m_5_17) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении числа мужчин от 5 до 18",
                                         "При получении числа мужчин от 5 до 18 произошла неожиданная ошибка.\n\n"
                                         + query_m_5_17->lastError().driverText() + "\n" + query_m_5_17->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_m_5_17;
                    delete query_mo_list;
                    db.rollback();
                    ui->pan_wait->setVisible(false);
                    return;
                }
                query_m_5_17->next();
                cnt_m_5_17 = query_m_5_17->value(0).toInt();
                delete query_m_5_17;

                if (cnt_m_5_17>0) {
                    // сформируем список m_5_17
                    QString sql_m_5_17_list = "update assig.sexages_data d "
                                              "   set id_sexage_line=" + QString::number(id_sexage_line) + ", "
                                              "       cell_name='m_5_17' "

                                              " where id_layer=" + QString::number(id_layer) + " "
                                              "   and sex=1 "
                                              "   and date_birth<='" + date_5.toString("yyyy-MM-dd") + "' "
                                              "   and date_birth> '" + date_18.toString("yyyy-MM-dd") + "' "
                                              "   and code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " ; ";
                    QSqlQuery *query_m_5_17_list = new QSqlQuery(db);
                    bool res_m_5_17_list = query_m_5_17_list->exec(sql_m_5_17_list);

                    if (!res_m_5_17_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании списка мужчин от 5 до 18",
                                             "При формировании списка мужчин от 5 до 18 произошла неожиданная ошибка.\n\n"
                                             + query_m_5_17_list->lastError().driverText() + "\n" + query_m_5_17_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_m_5_17_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_m_5_17_list;
                }


                // ======= //
                // m_18_59 //
                // ======= //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", М от 18 до 60 ...   ");
                QApplication::processEvents();

                QString sql_m_18_59 = "select count(distinct d.id_person) as cnt_m_18_59 "
                                      "  from assig.sexages_data d "
                                      //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                      " where d.id_layer=" + QString::number(id_layer) + " "
                                      "   and d.sex=1 "
                                      "   and d.date_birth<='" + date_18.toString("yyyy-MM-dd") + "' "
                                      "   and d.date_birth> '" + date_60.toString("yyyy-MM-dd") + "' "
                                      "   and d.code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " " + assig_type_s +
                                      " ; ";
                QSqlQuery *query_m_18_59 = new QSqlQuery(db);
                bool res_m_18_59 = query_m_18_59->exec(sql_m_18_59);

                if (!res_m_18_59) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении числа мужчин от 18 до 60",
                                         "При получении числа мужчин от 18 до 60 произошла неожиданная ошибка.\n\n"
                                         + query_m_18_59->lastError().driverText() + "\n" + query_m_18_59->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_m_18_59;
                    delete query_mo_list;
                    db.rollback();
                    ui->pan_wait->setVisible(false);
                    return;
                }
                query_m_18_59->next();
                cnt_m_18_59 = query_m_18_59->value(0).toInt();
                delete query_m_18_59;

                if (cnt_m_18_59>0) {
                    // сформируем список m_18_59
                    QString sql_m_18_59_list = "update assig.sexages_data d "
                                               "   set id_sexage_line=" + QString::number(id_sexage_line) + ", "
                                               "       cell_name='m_18_59' "

                                               " where id_layer=" + QString::number(id_layer) + " "
                                               "   and sex=1 "
                                               "   and date_birth<='" + date_18.toString("yyyy-MM-dd") + "' "
                                               "   and date_birth> '" + date_60.toString("yyyy-MM-dd") + "' "
                                               "   and code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " ; ";
                    QSqlQuery *query_m_18_59_list = new QSqlQuery(db);
                    bool res_m_18_59_list = query_m_18_59_list->exec(sql_m_18_59_list);

                    if (!res_m_18_59_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании списка мужчин от 18 до 60",
                                             "При формировании списка мужчин от 18 до 60 произошла неожиданная ошибка.\n\n"
                                             + query_m_18_59_list->lastError().driverText() + "\n" + query_m_18_59_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_m_18_59_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_m_18_59_list;
                }


                // ===== //
                // m_60_ //
                // ===== //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", М от 60 до ~~ ...   ");
                QApplication::processEvents();

                QString sql_m_60_ = "select count(distinct d.id_person) as cnt_m_60_ "
                                    "  from assig.sexages_data d "
                                    //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                    " where d.id_layer=" + QString::number(id_layer) + " "
                                    "   and d.sex=1 "
                                    "   and d.date_birth<='" + date_60.toString("yyyy-MM-dd") + "' "
                                    "   and d.code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " " + assig_type_s +
                                    " ; ";
                QSqlQuery *query_m_60_ = new QSqlQuery(db);
                bool res_m_60_ = query_m_60_->exec(sql_m_60_);

                if (!res_m_60_) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении числа мужчин старше 60",
                                         "При получении числа мужчин старше 60 произошла неожиданная ошибка.\n\n"
                                         + query_m_60_->lastError().driverText() + "\n" + query_m_60_->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_m_60_;
                    delete query_mo_list;
                    db.rollback();
                    ui->pan_wait->setVisible(false);
                    return;
                }
                query_m_60_->next();
                cnt_m_60_ = query_m_60_->value(0).toInt();
                delete query_m_60_;

                if (cnt_m_60_>0) {
                    // сформируем список m_60_
                    QString sql_m_60_list = "update assig.sexages_data d "
                                            "   set id_sexage_line=" + QString::number(id_sexage_line) + ", "
                                            "       cell_name='m_60_' "

                                            " where id_layer=" + QString::number(id_layer) + " "
                                            "   and sex=1 "
                                            "   and date_birth<='" + date_60.toString("yyyy-MM-dd") + "' "
                                            "   and code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " ; ";
                    QSqlQuery *query_m_60_list = new QSqlQuery(db);
                    bool res_m_60_list = query_m_60_list->exec(sql_m_60_list);

                    if (!res_m_60_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании списка мужчин от 60 до ~~",
                                             "При формировании списка мужчин от 60 до ~~ произошла неожиданная ошибка.\n\n"
                                             + query_m_60_list->lastError().driverText() + "\n" + query_m_60_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_m_60_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_m_60_list;
                }


                // ===== //
                // w_0_0 //
                // ===== //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", Ж от 0 до 1 ...   ");
                QApplication::processEvents();

                QString sql_w_0_0 = "select count(distinct d.id_person) as cnt_w_0_0 "
                                    "  from assig.sexages_data d "
                                    //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                    " where d.id_layer=" + QString::number(id_layer) + " "
                                    "   and d.sex=2 "
                                    "   and d.date_birth<='" + date_0.toString("yyyy-MM-dd") + "' "
                                    "   and d.date_birth> '" + date_1.toString("yyyy-MM-dd") + "' "
                                    "   and d.code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " " + assig_type_s +
                                    " ; ";
                QSqlQuery *query_w_0_0 = new QSqlQuery(db);
                bool res_w_0_0 = query_w_0_0->exec(sql_w_0_0);

                if (!res_w_0_0) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении числа женщин от 0 до 1",
                                         "При получении числа женщин от 0 до 1 произошла неожиданная ошибка.\n\n"
                                         + query_w_0_0->lastError().driverText() + "\n" + query_w_0_0->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_w_0_0;
                    delete query_mo_list;
                    db.rollback();
                    ui->pan_wait->setVisible(false);
                    return;
                }
                query_w_0_0->next();
                cnt_w_0_0 = query_w_0_0->value(0).toInt();
                delete query_w_0_0;

                if (cnt_w_0_0>0) {
                    // сформируем список w_0_0
                    QString sql_w_0_0_list = "update assig.sexages_data d "
                                             "   set id_sexage_line=" + QString::number(id_sexage_line) + ", "
                                             "       cell_name='w_0_0' "

                                             " where id_layer=" + QString::number(id_layer) + " "
                                             "   and sex=2 "
                                             "   and date_birth<='" + date_0.toString("yyyy-MM-dd") + "' "
                                             "   and date_birth> '" + date_1.toString("yyyy-MM-dd") + "' "
                                             "   and code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " ; ";
                    QSqlQuery *query_w_0_0_list = new QSqlQuery(db);
                    bool res_w_0_0_list = query_w_0_0_list->exec(sql_w_0_0_list);

                    if (!res_w_0_0_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании списка женщин от 0 до 1",
                                             "При формировании списка женщин от 0 до 1 произошла неожиданная ошибка.\n\n"
                                             + query_w_0_0_list->lastError().driverText() + "\n" + query_w_0_0_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_w_0_0_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_w_0_0_list;
                }


                // ===== //
                // w_1_4 //
                // ===== //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", Ж от 1 до 5 ...   ");
                QApplication::processEvents();

                QString sql_w_1_4 = "select count(distinct d.id_person) as cnt_w_1_4 "
                                    "  from assig.sexages_data d "
                                    //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                    " where d.id_layer=" + QString::number(id_layer) + " "
                                    "   and d.sex=2 "
                                    "   and d.date_birth<='" + date_1.toString("yyyy-MM-dd") + "' "
                                    "   and d.date_birth> '" + date_5.toString("yyyy-MM-dd") + "' "
                                    "   and d.code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " " + assig_type_s +
                                    " ; ";
                QSqlQuery *query_w_1_4 = new QSqlQuery(db);
                bool res_w_1_4 = query_w_1_4->exec(sql_w_1_4);

                if (!res_w_1_4) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении числа женщин от 1 до 5",
                                         "При получении числа женщин от 1 до 5 произошла неожиданная ошибка.\n\n"
                                         + query_w_1_4->lastError().driverText() + "\n" + query_w_1_4->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_w_1_4;
                    delete query_mo_list;
                    db.rollback();
                    ui->pan_wait->setVisible(false);
                    return;
                }
                query_w_1_4->next();
                cnt_w_1_4 = query_w_1_4->value(0).toInt();
                delete query_w_1_4;

                if (cnt_w_1_4>0) {
                    // сформируем список w_1_4
                    QString sql_w_1_4_list = "update assig.sexages_data d "
                                             "   set id_sexage_line=" + QString::number(id_sexage_line) + ", "
                                             "       cell_name='w_1_4' "

                                             " where id_layer=" + QString::number(id_layer) + " "
                                             "   and sex=2 "
                                             "   and date_birth<='" + date_1.toString("yyyy-MM-dd") + "' "
                                             "   and date_birth> '" + date_5.toString("yyyy-MM-dd") + "' "
                                             "   and code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " " + assig_type_s +
                                             " ; ";
                    QSqlQuery *query_w_1_4_list = new QSqlQuery(db);
                    bool res_w_1_4_list = query_w_1_4_list->exec(sql_w_1_4_list);

                    if (!res_w_1_4_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании списка женщин от 1 до 5",
                                             "При формировании списка женщин от 1 до 5 произошла неожиданная ошибка.\n\n"
                                             + query_w_1_4_list->lastError().driverText() + "\n" + query_w_1_4_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_w_1_4_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_w_1_4_list;
                }


                // ====== //
                // w_5_17 //
                // ====== //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", Ж от 5 до 18 ...   ");
                QApplication::processEvents();

                QString sql_w_5_17 = "select count(distinct d.id_person) as cnt_w_5_17 "
                                     "  from assig.sexages_data d "
                                     //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                     " where d.id_layer=" + QString::number(id_layer) + " "
                                     "   and d.sex=2 "
                                     "   and d.date_birth<='" + date_5.toString("yyyy-MM-dd") + "' "
                                     "   and d.date_birth> '" + date_18.toString("yyyy-MM-dd") + "' "
                                     "   and d.code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " ; ";
                QSqlQuery *query_w_5_17 = new QSqlQuery(db);
                bool res_w_5_17 = query_w_5_17->exec(sql_w_5_17);

                if (!res_w_5_17) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении числа женщин от 5 до 18",
                                         "При получении числа женщин от 5 до 18 произошла неожиданная ошибка.\n\n"
                                         + query_w_5_17->lastError().driverText() + "\n" + query_w_5_17->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_w_5_17;
                    delete query_mo_list;
                    db.rollback();
                    ui->pan_wait->setVisible(false);
                    return;
                }
                query_w_5_17->next();
                cnt_w_5_17 = query_w_5_17->value(0).toInt();
                delete query_w_5_17;

                if (cnt_w_5_17>0) {
                    // сформируем список w_5_17
                    QString sql_w_5_17_list = "update assig.sexages_data d "
                                              "   set id_sexage_line=" + QString::number(id_sexage_line) + ", "
                                              "       cell_name='w_5_17' "

                                              " where id_layer=" + QString::number(id_layer) + " "
                                              "   and sex=2 "
                                              "   and date_birth<='" + date_5.toString("yyyy-MM-dd") + "' "
                                              "   and date_birth> '" + date_18.toString("yyyy-MM-dd") + "' "
                                              "   and code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " " + assig_type_s +
                                              " ; ";
                    QSqlQuery *query_w_5_17_list = new QSqlQuery(db);
                    bool res_w_5_17_list = query_w_5_17_list->exec(sql_w_5_17_list);

                    if (!res_w_5_17_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании списка женщин от 5 до 18",
                                             "При формировании списка женщин от 5 до 18 произошла неожиданная ошибка.\n\n"
                                             + query_w_5_17_list->lastError().driverText() + "\n" + query_w_5_17_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_w_5_17_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_w_5_17_list;
                }


                // ======= //
                // w_18_54 //
                // ======= //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", Ж от 18 до 55 ...   ");
                QApplication::processEvents();

                QString sql_w_18_54 = "select count(distinct d.id_person) as cnt_w_18_54 "
                                      "  from assig.sexages_data d "
                                      //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                      " where d.id_layer=" + QString::number(id_layer) + " "
                                      "   and d.sex=2 "
                                      "   and d.date_birth<='" + date_18.toString("yyyy-MM-dd") + "' "
                                      "   and d.date_birth> '" + date_55.toString("yyyy-MM-dd") + "' "
                                      "   and d.code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " ; ";
                QSqlQuery *query_w_18_54 = new QSqlQuery(db);
                bool res_w_18_54 = query_w_18_54->exec(sql_w_18_54);

                if (!res_w_18_54) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении числа женщин от 18 до 55",
                                         "При получении числа женщин от 18 до 55 произошла неожиданная ошибка.\n\n"
                                         + query_w_18_54->lastError().driverText() + "\n" + query_w_18_54->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_w_18_54;
                    delete query_mo_list;
                    db.rollback();
                    ui->pan_wait->setVisible(false);
                    return;
                }
                query_w_18_54->next();
                cnt_w_18_54 = query_w_18_54->value(0).toInt();
                delete query_w_18_54;

                if (cnt_w_18_54>0) {
                    // сформируем список w_18_54
                    QString sql_w_18_54_list = "update assig.sexages_data d "
                                               "   set id_sexage_line=" + QString::number(id_sexage_line) + ", "
                                               "       cell_name='w_18_54' "

                                               " where id_layer=" + QString::number(id_layer) + " "
                                               "   and sex=2 "
                                               "   and date_birth<='" + date_18.toString("yyyy-MM-dd") + "' "
                                               "   and date_birth> '" + date_55.toString("yyyy-MM-dd") + "' "
                                               "   and code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " " + assig_type_s +
                                               " ; ";
                    QSqlQuery *query_w_18_54_list = new QSqlQuery(db);
                    bool res_w_18_54_list = query_w_18_54_list->exec(sql_w_18_54_list);

                    if (!res_w_18_54_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании списка женщин от 18 до 55",
                                             "При формировании списка женщин от 18 до 55 произошла неожиданная ошибка.\n\n"
                                             + query_w_18_54_list->lastError().driverText() + "\n" + query_w_18_54_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_w_18_54_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_w_18_54_list;
                }


                // ===== //
                // w_55_ //
                // ===== //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", Ж от 55 до ~~ ...   ");
                QApplication::processEvents();

                QString sql_w_55_ = "select count(distinct d.id_person) as cnt_w_55_ "
                                    "  from assig.sexages_data d "
                                    //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                    " where d.id_layer=" + QString::number(id_layer) + " "
                                    "   and d.sex=2 "
                                    "   and d.date_birth<='" + date_55.toString("yyyy-MM-dd") + "' "
                                    "   and d.code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " ; ";
                QSqlQuery *query_w_55_ = new QSqlQuery(db);
                bool res_w_55_ = query_w_55_->exec(sql_w_55_);

                if (!res_w_55_) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении числа женщин старше 55",
                                         "При получении числа женщин старше 55 произошла неожиданная ошибка.\n\n"
                                         + query_w_55_->lastError().driverText() + "\n" + query_w_55_->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_w_55_;
                    delete query_mo_list;
                    db.rollback();
                    ui->pan_wait->setVisible(false);
                    return;
                }
                query_w_55_->next();
                cnt_w_55_ = query_w_55_->value(0).toInt();
                delete query_w_55_;

                if (cnt_w_55_>0) {
                    // сформируем список w_55_
                    QString sql_w_55_list = "update assig.sexages_data d "
                                            "   set id_sexage_line=" + QString::number(id_sexage_line) + ", "
                                            "       cell_name='w_55_' "

                                            " where id_layer=" + QString::number(id_layer) + " "
                                            "   and sex=2 "
                                            "   and date_birth<='" + date_55.toString("yyyy-MM-dd") + "' "
                                            "   and code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " " + assig_type_s +
                                            " ; ";
                    QSqlQuery *query_w_55_list = new QSqlQuery(db);
                    bool res_w_55_list = query_w_55_list->exec(sql_w_55_list);

                    if (!res_w_55_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании списка женщин от 55 до ~~",
                                             "При формировании списка женщин от 55 до ~~ произошла неожиданная ошибка.\n\n"
                                             + query_w_55_list->lastError().driverText() + "\n" + query_w_55_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_w_55_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_w_55_list;
                }


                // ======== //
                // snils_mt //
                // ======== //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", с отметкой СНИЛС врача ...   ");
                QApplication::processEvents();

                QString sql_snils_mt = "select count(distinct d.id_person) as cnt_snils_mt "
                                       "  from assig.sexages_data d "
                                       //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                       " where d.id_layer=" + QString::number(id_layer) + " "
                                       "   and d.code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " "
                                       "   and d.snils_mt is not NULL "
                                       "   and d.snils_mt<>'0' ; ";
                QSqlQuery *query_snils_mt = new QSqlQuery(db);
                bool res_snils_mt = query_snils_mt->exec(sql_snils_mt);

                if (!res_snils_mt) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении числа персон с отметкой СНИЛС врача",
                                         "При получении числа персон с отметкой СНИЛС врача произошла неожиданная ошибка.\n\n"
                                         + query_snils_mt->lastError().driverText() + "\n" + query_snils_mt->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_snils_mt;
                    delete query_mo_list;
                    db.rollback();
                    ui->pan_wait->setVisible(false);
                    return;
                }
                query_snils_mt->next();
                cnt_snils_mt = query_snils_mt->value(0).toInt();
                delete query_snils_mt;

                /*
                 *  нефиг
                if (cnt_snils_mt>0) {
                    // сформируем список snils_mt
                    QString sql_snils_mt_list = "insert into assig.sexages_data "
                                                "     ( id_layer_sexage, cell_name, "
                                                "       id_person, fam, im, ot, date_birth, sex, snils, "
                                                "       pol_v, enp, vs_num, pol_ser, pol_num, "
                                                "       assig_type, assig_date, code_mo, code_mp, code_ms, snils_mt, "
                                                "       subj_name, dstr_name, city_name, nasp_name, strt_name, house, corp ) "

                                                "select " + QString::number(id_sexage_line) + ", 'snils_mt', d.id_person, d.fam, d.im, d.ot, d.date_birth, d.sex, d.snils, "
                                                "       d.pol_v, d.enp, d.vs_num, d.pol_ser, d.pol_num, "
                                                "       d.assig_type, d.assig_date, mo.code_head, d.code_mp, d.code_ms, d.snils_mt, "
                                                "       d.subj_name, d.dstr_name, d.city_name, d.nasp_name, d.strt_name, d.house, d.corp "
                                                "  from assig.sexages_data d "
                                                //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "

                                                " where d.id_layer=" + QString::number(id_layer) + " "
                                                "   and d.code_head" + (code_head=="NULL" ? " is NULL " : ("='" + code_head + "'")) + " "
                                                "   and d.snils_mt is not NULL "
                                                "   and d.snils_mt<>'0' ";
                                                " order by d.fam, d.im, d.ot, d.date_birth, d.sex, d.snils ; ";
                    QSqlQuery *query_snils_mt_list = new QSqlQuery(db);
                    bool res_snils_mt_list = query_snils_mt_list->exec(sql_snils_mt_list);

                    if (!res_snils_mt_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании списка лиц с отметкой СНИЛС врача",
                                             "При формировании списка лиц с отметкой СНИЛС врача произошла неожиданная ошибка.\n\n"
                                             + query_snils_mt_list->lastError().driverText() + "\n" + query_snils_mt_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_snils_mt_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_snils_mt_list;
                }
                */

                cnt_m_sum = cnt_m_0_0 + cnt_m_1_4 + cnt_m_5_17 + cnt_m_18_59 + cnt_m_60_;
                cnt_w_sum = cnt_w_0_0 + cnt_w_1_4 + cnt_w_5_17 + cnt_w_18_54 + cnt_w_55_;
                cnt_sum   = cnt_m_sum + cnt_w_sum;


                // ============================================= //
                // сохраним строчку половозрастной в базу данных //
                // ============================================= //
                QString sql_save_line = "update assig.sexages_lines "
                                        "   set m_0_0="    + QString::number(cnt_m_0_0)    + ", "
                                        "       m_1_4="    + QString::number(cnt_m_1_4)    + ", "
                                        "       m_5_17="   + QString::number(cnt_m_5_17)   + ", "
                                        "       m_18_59="  + QString::number(cnt_m_18_59)  + ", "
                                        "       m_60_="    + QString::number(cnt_m_60_)    + ", "
                                        "       m_sum="    + QString::number(cnt_m_sum)    + ", "
                                        "       w_0_0="    + QString::number(cnt_w_0_0)    + ", "
                                        "       w_1_4="    + QString::number(cnt_w_1_4)    + ", "
                                        "       w_5_17="   + QString::number(cnt_w_5_17)   + ", "
                                        "       w_18_54="  + QString::number(cnt_w_18_54)  + ", "
                                        "       w_55_="    + QString::number(cnt_w_55_)    + ", "
                                        "       w_sum="    + QString::number(cnt_w_sum)    + ", "
                                        "       sum="      + QString::number(cnt_sum)      + ", "
                                        "       snils_mt=" + QString::number(cnt_snils_mt) + " "
                                        " where id=" + QString::number(id_sexage_line) + " ; ";
                QSqlQuery *query_save_line = new QSqlQuery(db);
                bool res_save_line = query_save_line->exec(sql_save_line);

                if (!res_save_line) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при сохранении строчки половозрастной в базу данных",
                                         "При сохранении строчки половозрастной в базу данных произошла неожиданная ошибка.\n\n"
                                         + query_save_line->lastError().driverText() + "\n" + query_save_line->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_save_line;
                    delete query_mo_list;
                    db.rollback();
                    ui->pan_wait->setVisible(false);
                    return;
                }
                delete query_save_line;


                //------------------------------//
                // отображение статуса на экран //
                //------------------------------//
                ui->lab_cnt->setText(" -//- ");

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

                QApplication::processEvents();

            } while (query_mo_list->next());
            delete query_mo_list;


            // обновим статистику таблицы
            ui->lab_wait->setText("   ... обновление статистики половозрастной таблицы ...   ");
            QApplication::processEvents();

            QString sql_stat = "update assig.sexages s "
                               "   set cnt_lines=(select count(*) from assig.sexages_lines sl where sl.id_sexage=" + QString::number(id_sexage) + "), "
                               "       cnt_recs=(select count(*) from assig.sexages_data sd where sd.id_sexage_line in(select sl.id from assig.sexages_lines sl where sl.id_sexage=" + QString::number(id_sexage) + ") ), "
                               "       cnt_uniq=(select count(distinct sd.id_person) from assig.sexages_data sd where sd.id_sexage_line in(select sl.id from assig.sexages_lines sl where sl.id_sexage=" + QString::number(id_sexage) + ") ) "
                               " where s.id=" + QString::number(id_sexage) + " ; ";
            QSqlQuery *query_stat = new QSqlQuery(db);
            bool res_stat = query_stat->exec(sql_stat);

            if (!res_stat) {
                this->setCursor(Qt::ArrowCursor);
                QMessageBox::warning(this, "Ошибка при подсчёте статистики половозрастной таблицы",
                                     "При подсчёте статистики половозрастной таблицы произошла неожиданная ошибка.\n\n"
                                     + query_stat->lastError().driverText() + "\n" + query_stat->lastError().databaseText() + "\n\n"
                                     "Операция отменена.");
                delete query_stat;
                db.rollback();
                ui->pan_wait->setVisible(false);
                return;
            }
            delete query_stat;


            db.commit();

            ui->pan_assiglays->setEnabled(true);
            ui->pan_sexages->setEnabled(true);
            ui->pan_moves->setEnabled(true);
            ui->pan_DN->setEnabled(true);
            ui->pan_wait->setVisible(false);
            QApplication::processEvents();

        } else {
            QMessageBox::information(this, "Отмена операции",
                                     "Генерация таблицы половозрастного распределения отменена пользователем.\n"
                                     "Ничего не изменилось.");
        }
    } else {
        QMessageBox::information(this, "Ничего не выбрано",
                                 "Для продолжения операции сначал выберите снимок, по которому хотите построить таблицу половозрастного распределения застрахованных, в разрезе МО.");
    }
    refresh_assiglays_tab();
    refresh_sexages_tab();
    refresh_moves_tab();
    this->setCursor(Qt::ArrowCursor);
}

void assiglays_wnd::on_bn_open_sexage_clicked() {
    int id_sexage = -1;

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_sexages->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        this->setCursor(Qt::WaitCursor);
        QModelIndex index = indexes.front();

        id_sexage = model_sexages.data(model_sexages.index(index.row(), 0), Qt::EditRole).toInt();       // e.id

        delete show_sexage_w;
        show_sexage_w = new show_sexage_wnd(id_sexage, db, data_app, settings, this);
        this->setCursor(Qt::ArrowCursor);

        show_sexage_w->exec();
    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Выберите строку таблицы половозрастного распреледения, данные которой хотите увидеть.");
    }
}

void assiglays_wnd::on_bn_delete_sexage_clicked() {
    int id_sexage = -1;

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_sexages->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        this->setCursor(Qt::WaitCursor);
        QModelIndex index = indexes.front();

        id_sexage = model_sexages.data(model_sexages.index(index.row(), 0), Qt::EditRole).toInt();       // e.id

        if (QMessageBox::question(this, "Нужно подтверждение",
                                  "Вы действительно хотите удалить выбранную таблицу половозрастного распределения застрахованных?\n\n"
                                  "Это необратимая операция.",
                                  QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
            QMessageBox::warning(this, "Отмена удаления",
                                 "Удаление таблицы отменено пользователем.\n"
                                 "Ничего не изменилось.");
            return;
        }

        // собственно удаление данных таблицы
        QString sql_delete = "delete from assig.sexages_data d "
                             " where d.id in "
                             "       ( select sl.id "
                             "           from assig.sexages_lines sl "
                             "          where sl.id_sexage=" + QString::number(id_sexage) + " ) ; "

                             "delete from assig.sexages_lines ls "
                             " where ls.id_sexage=" + QString::number(id_sexage) + " ; "

                             "delete from assig.sexages s "
                             " where s.id=" + QString::number(id_sexage) + " ; ";
        QSqlQuery *query_delete = new QSqlQuery(db);
        bool res_delete = query_delete->exec(sql_delete);

        if (!res_delete) {
            this->setCursor(Qt::ArrowCursor);
            QMessageBox::warning(this, "Ошибка при удалении таблицы половозрастного распределения",
                                 "При удалении таблицы половозрастного распределения произошла неожиданная ошибка.\n\n"
                                 + query_delete->lastError().driverText() + "\n" + query_delete->lastError().databaseText() + "\n\n"
                                 "Операция отменена.");
            delete query_delete;
            db.rollback();
            return;
        }
        delete query_delete;
        this->setCursor(Qt::ArrowCursor);

        refresh_assiglays_tab();
        refresh_sexages_tab();
        refresh_moves_tab();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Выберите таблицу половозрастного распреледения для удаления.");
    }
}

void assiglays_wnd::on_bn_delete_assiglay_clicked() {
    int id_assiglay = -1;

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_assiglays->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        this->setCursor(Qt::WaitCursor);
        QModelIndex index = indexes.front();

        id_assiglay = model_assiglays.data(model_assiglays.index(index.row(), 0), Qt::EditRole).toInt();       // e.id

        if (QMessageBox::question(this, "Нужно подтверждение",
                                  "Вы действительно хотите удалить выбранный снимок данных прикрепления застрахованных?\n\n"
                                  "Это необратимая операция.",
                                  QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
            QMessageBox::warning(this, "Отмена удаления",
                                 "Удаление снимка данных отменено пользователем.\n"
                                 "Ничего не изменилось.");
            return;
        }

        db.transaction();

        // собственно удаление данных таблицы
        QString sql_delete = // удалим списки половозрастной таблицы
                             "delete from assig.sexages_data "
                             " where id_sexage_line in( select ls.id "
                             "                from assig.sexages_lines ls "
                             "               where ls.id_layer=" + QString::number(id_assiglay) + " ) ; "
                             // удалим остальные данные
                             "delete from assig.sexages_data "
                             " where id_layer=" + QString::number(id_assiglay) + " ; "
                             // удалим половозрастную таблицу
                             "delete from assig.sexages_lines "
                             " where id_layer=" + QString::number(id_assiglay) + " ; "
                             "delete from assig.sexages "
                             " where id_layer=" + QString::number(id_assiglay) + " ; "
                             // удалим списки движения застрахованных
                             "delete from assig.moves_data "
                             " where id in( select ls.id "
                             "                from assig.moves_lines  ls "
                             "               where ls.id_layer_old=" + QString::number(id_assiglay) + " "
                             "                  or ls.id_layer_new=" + QString::number(id_assiglay) + " ) ; "
                             // удалим таблицу движения застрахованных
                             "delete from assig.moves_lines "
                             " where id_layer_old=" + QString::number(id_assiglay) + " "
                             "    or id_layer_new=" + QString::number(id_assiglay) + " ; "
                             "delete from assig.moves "
                             " where id_layer_old=" + QString::number(id_assiglay) + " "
                             "    or id_layer_new=" + QString::number(id_assiglay) + " ; "
                             // удалим содержимое снимка
                             "delete from assig.sexages_lines "
                             " where id_layer=" + QString::number(id_assiglay) + " ; "
                             // удалим запись снимка
                             "delete from assig.layers "
                             " where id=" + QString::number(id_assiglay) + " ; ";
        QSqlQuery *query_delete = new QSqlQuery(db);
        bool res_delete = query_delete->exec(sql_delete);

        if (!res_delete) {
            this->setCursor(Qt::ArrowCursor);
            QMessageBox::warning(this, "Ошибка при удалении снимка данных прикреплния",
                                 "При удалении снимка данных прикреплния произошла неожиданная ошибка.\n\n"
                                 + query_delete->lastError().driverText() + "\n" + query_delete->lastError().databaseText() + "\n\n"
                                 "Операция отменена.");
            delete query_delete;
            db.rollback();
            return;
        }
        delete query_delete;
        db.commit();
        this->setCursor(Qt::ArrowCursor);

        refresh_assiglays_tab();
        refresh_sexages_tab();
        refresh_moves_tab();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Выберите строку снимка данных прикреплния для удаления.");
    }
}

void assiglays_wnd::on_bn_add_move_clicked() {
    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_assiglays->selectionModel()->selection().indexes();
    this->setCursor(Qt::WaitCursor);

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        int layer_type = model_assiglays.data(model_assiglays.index(index.row(), 3), Qt::EditRole).toInt();     // assiglay_type
        int assig_type = 2;
        int id_layer_old = -1;
        QDate layer_date_old = QDate();
        int id_layer_new = model_assiglays.data(model_assiglays.index(index.row(), 0), Qt::EditRole).toInt();
        QDate layer_date_new = model_assiglays.data(model_assiglays.index(index.row(), 2), Qt::EditRole).toDate();

        delete add_move_w;
        add_move_w = new add_move_wnd(layer_type, assig_type, id_layer_old, layer_date_old, id_layer_new, layer_date_new, db, data_app, settings, this);

        if (add_move_w->exec()) {
            // проверим, можно ли добавить новую таблицу с такими параметрами
            QString sql_test = "select count(*) "
                               "  from assig.moves m "
                               " where m.id_layer_new="   + QString::number(id_layer_new)   + " "
                               "   and m.assig_type=" + QString::number(assig_type) + " ; ";
            QSqlQuery *query_test = new QSqlQuery(db);
            bool res_test = query_test->exec(sql_test);
            if (!res_test) {
                this->setCursor(Qt::ArrowCursor);
                QMessageBox::warning(this, "Ошибка при проверке возможности добавления таблицы движения",
                                     "При проверке возможности добавления таблицы движения произошла неожиданная ошибка.\n\n"
                                     + query_test->lastError().driverText() + "\n" + query_test->lastError().databaseText() + "\n\n"
                                     "Операция отменена.");
                delete query_test;
                return;
            }
            query_test->next();
            int cnt = query_test->value(0).toInt();
            delete query_test;
            if (cnt>0) {
                this->setCursor(Qt::ArrowCursor);
                QMessageBox::warning(this, "Такая таблица уже есть - отказ",
                                     "В базе данных уже есть табллица движения застрахованных для этого снимка (ID_LAYER=" + QString::number(id_layer_new) + ") со способом прикрепления - " + QString::number(assig_type) + ".\n\n"
                                     "Параметры таблиц движения не должны повторяться.\n"
                                     "Измените параметры или удалите старую таблицу.\n\n"
                                     "Операция отменена.");
                return;
            }


            // рассчитаем движение прикреплённых застрахованных по снимку
            ui->pan_assiglays->setEnabled(false);
            ui->pan_sexages->setEnabled(false);
            ui->pan_moves->setEnabled(false);
            ui->pan_DN->setEnabled(false);
            ui->pan_wait->setVisible(true);
            ui->lab_wait->setText("   ... ПОДОЖДИТЕ, РАБОТАЮ ...   ");
            QApplication::processEvents();

            db.transaction();

            // добавим запись таблицы
            QString sql_move = "insert into assig.moves "
                               "       ( id_layer_old, id_layer_new, assig_type )"
                               "values ( " + QString::number(id_layer_old) + ", "
                               "         " + QString::number(id_layer_new) + ", "
                               "         " + QString::number(assig_type) + " ) "
                               "returning id ; ";
            QSqlQuery *query_move = new QSqlQuery(db);
            bool res_move = query_move->exec(sql_move);

            if (!res_move) {
                this->setCursor(Qt::ArrowCursor);
                QMessageBox::warning(this, "Ошибка при создании таблицы движения",
                                     "При создании таблицы движения произошла неожиданная ошибка.\n\n"
                                     + query_move->lastError().driverText() + "\n" + query_move->lastError().databaseText() + "\n\n"
                                     "Операция отменена.");
                delete query_move;
                db.rollback();
                ui->pan_wait->setVisible(false);
                return;
            }
            query_move->next();
            int id_move = query_move->value(0).toInt();
            delete query_move;


            QString assig_type_d = " ";
            bool f_assig_type_d = false;
            switch (assig_type) {
            case 1:
                assig_type_d = " and d.assig_type=1 ";
                f_assig_type_d = true;
                break;
            case 2:
                assig_type_d = " and d.assig_type=2 ";
                f_assig_type_d = true;
                break;
            default:
                assig_type_d = " and d.assig_type=0 ";
                f_assig_type_d = false;
                break;
            }


            QString assig_type_s = " ";
            bool f_assig_type_s = false;
            switch (assig_type) {
            case 1:
                assig_type_s = " and d.assig_type=1 ";
                f_assig_type_s = true;
                break;
            case 2:
                assig_type_s = " and d.assig_type=2 ";
                f_assig_type_s = true;
                break;
            default:
                assig_type_s = " ";
                f_assig_type_s = false;
                break;
            }

            // получим список головных медорганизаций, к которым есть прикреплённые лица
            QString sql_mo_list = "select code_head "
                                  "  from "
                                  "  ( select distinct code_head "
                                  "      from "
                                  "       ( ( select mo.code_head "
                                  "             from assig.sexages_data d "
                                  "             left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                  "            where d.id_layer=" + QString::number(id_layer_old) + " "
                                  "              and mo.code_head is not NULL "
                                  "              #ASSIG_TYPE+# "
                                  "         ) "
                                  "        union "
                                  "         ( select mo.code_head "
                                  "             from assig.sexages_data d "
                                  "             left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                  "            where d.id_layer=" + QString::number(id_layer_new) + " "
                                  "              and mo.code_head is not NULL "
                                  "              #ASSIG_TYPE+# "
                                  "         ) "
                                  "       ) q "
                                  "  ) q "
                                  " order by code_head ; ";
            sql_mo_list = sql_mo_list.replace("#ASSIG_TYPE#", assig_type_d).
                    replace("#ASSIG_TYPE+#", assig_type_s);

            QSqlQuery *query_mo_list = new QSqlQuery(db);
            bool res_mo_list = query_mo_list->exec(sql_mo_list);

            if (!res_mo_list) {
                this->setCursor(Qt::ArrowCursor);
                QMessageBox::warning(this, "Ошибка при получении списка медорганизаций",
                                     "При получении списка медорганизаций произошла неожиданная ошибка.\n\n"
                                     + query_mo_list->lastError().driverText() + "\n" + query_mo_list->lastError().databaseText() + "\n\n"
                                     "Операция отменена.");
                delete query_mo_list;
                db.rollback();
                ui->pan_wait->setVisible(false);
                return;
            }

            int mo_cnt = query_mo_list->size();

            date0 = QDate::currentDate();
            time0 = QTime::currentTime();


            // переберём полученные медорганизации
            int mo_n = 0;
            bool is_first = true;
            while (query_mo_list->next()) {
                mo_n++;
                QString code_head = query_mo_list->value(0).toString();
                int cnt_old, cnt_add, cnt_dec, cnt_new;


                // =============================== //
                // добавим строку таблицы движения //
                // =============================== //
                QString sql_new_line = "insert into assig.moves_lines  "
                                       "       ( id_move, id_layer_old, id_layer_new, code_head, assig_type )"
                                       "values ( " + QString::number(id_move) + ", "
                                       "         " + QString::number(id_layer_old) + ", "
                                       "         " + QString::number(id_layer_new) + ", "
                                       "         " + (code_head=="NULL" ? " NULL " : ("'" + code_head + "'")) + ", "
                                       "         " + QString::number(assig_type) + " ) "
                                       "returning id ; ";
                QSqlQuery *query_new_line = new QSqlQuery(db);
                bool res_new_line = query_new_line->exec(sql_new_line);

                if (!res_new_line) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении списка медорганизаций",
                                         "При получении списка медорганизаций произошла неожиданная ошибка.\n\n"
                                         + query_new_line->lastError().driverText() + "\n" + query_new_line->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_new_line;
                    db.rollback();
                    ui->pan_wait->setVisible(false);
                    return;
                }
                query_new_line->next();
                int id_move_line = query_new_line->value(0).toInt();
                delete query_new_line;


                // === //
                // old //
                // === //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", прикреплённые месяц назад ...   ");
                QApplication::processEvents();

                QString sql_old = "select count(distinct d.id_person) as cnt_old "
                                  "  from assig.sexages_data d "
                                  //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "

                                  " where d.id_layer=" + QString::number(id_layer_old) + " "
                                  "   and d.code_head='" + code_head + "' "
                                  "   #ASSIG_TYPE+# ; ";
                sql_old = sql_old.replace("#ASSIG_TYPE#", assig_type_d).
                        replace("#ASSIG_TYPE+#", assig_type_s);

                QSqlQuery *query_old = new QSqlQuery(db);
                bool res_old = query_old->exec(sql_old);

                if (!res_old) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении старого числа прикреплённых",
                                         "При получении старого числа прикреплённых произошла неожиданная ошибка.\n\n"
                                         + query_old->lastError().driverText() + "\n" + query_old->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_old;
                    delete query_mo_list;
                    db.rollback();
                    return;
                }
                query_old->next();
                cnt_old = query_old->value(0).toInt();
                delete query_old;

                /*
                 * нефиг
                if (cnt_old>0) {
                    // сформируем список m_old
                    QString sql_old_list = "insert into assig.moves_data "
                                           "     ( id_move, cell_name, "
                                           "       id_person, fam, im, ot, sex, date_birth, snils, "
                                           "       id_polis, pol_v, enp, vs_num, pol_ser, pol_num, "
                                           "       assig_type_old, assig_date_old, code_mo_old, code_head_old, code_mp_old, code_ms_old, snils_mt_old, "
                                           "       assig_type_new, assig_date_new, code_mo_new, code_head_new, code_mp_new, code_ms_new, snils_mt_new, "
                                           "       subj_name, dstr_name, city_name, nasp_name, strt_name, house, corp ) "

                                           "select " + QString::number(id_move_line) + ", 'cnt_old', "
                                           "       d.id_person, d.fam, d.im, d.ot, d.sex, d.date_birth, d.snils, "
                                           "       d.id_polis, d.pol_v, d.enp, d.vs_num, d.pol_ser, d.pol_num, "
                                           "       d.assig_type,  d.assig_date,  d.code_mo,  d.code_head,  d.code_mp,  d.code_ms,  d.snils_mt,  "
                                           "       d1.assig_type, d1.assig_date, d1.code_mo, d1.code_head, d1.code_mp, d1.code_ms, d1.snils_mt, "
                                           "       d.subj_name, d.dstr_name, d.city_name, d.nasp_name, d.strt_name, d.house, d.corp "
                                           "  from assig.sexages_data d "
                                           //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                           "  left join assig.sexages_data d1 on(d1.id_layer=" + QString::number(id_layer_new) + " and d1.id_person=d.id_person) "
                                           //"  left join spr_medorgs mo1 on(mo1.code_mo=d1.code_mo) "

                                           " where d.id_layer=" + QString::number(id_layer_old) + " "
                                           "   and d.code_head='" + code_head + "' "
                                           "   #ASSIG_TYPE# "
                                           " order by d.fam, d.im, d.ot, d.date_birth, d.sex, d.snils ; ";
                    sql_old_list = sql_old_list.replace("#ASSIG_TYPE#", assig_type_d);

                    QSqlQuery *query_old_list = new QSqlQuery(db);
                    bool res_old_list = query_old_list->exec(sql_old_list);

                    if (!res_old_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании старого списка прикреплённых",
                                             "При формировании старого списка прикреплённых произошла неожиданная ошибка.\n\n"
                                             + query_old_list->lastError().driverText() + "\n" + query_old_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_old_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_old_list;
                }
                */


                // === //
                // new //
                // === //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", прикреплённые в этом месяце ...   ");
                QApplication::processEvents();

                QString sql_new = "select count(distinct d.id_person) as cnt_new "
                                  "  from assig.sexages_data d "
                                  //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "

                                  " where d.id_layer=" + QString::number(id_layer_new) + " "
                                  "   and d.code_head='" + code_head + "' "
                                  "   #ASSIG_TYPE+# ; ";
                sql_new = sql_new.replace("#ASSIG_TYPE#", assig_type_d).
                        replace("#ASSIG_TYPE+#", assig_type_s);

                QSqlQuery *query_new = new QSqlQuery(db);
                bool res_new = query_new->exec(sql_new);

                if (!res_new) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении старого числа прикреплённых",
                                         "При получении старого числа прикреплённых произошла неожиданная ошибка.\n\n"
                                         + query_new->lastError().driverText() + "\n" + query_new->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_new;
                    delete query_mo_list;
                    db.rollback();
                    return;
                }
                query_new->next();
                cnt_new = query_new->value(0).toInt();
                delete query_new;

                /*
                 * нефиг
                if (cnt_new>0) {
                    // сформируем список m_new
                    QString sql_new_list = "insert into assig.moves_data "
                                           "     ( id_move, cell_name, "
                                           "       id_person, fam, im, ot, sex, date_birth, snils, "
                                           "       id_polis, pol_v, enp, vs_num, pol_ser, pol_num, "
                                           "       assig_type_old, assig_date_old, code_mo_old, code_head_old, code_mp_old, code_ms_old, snils_mt_old, "
                                           "       assig_type_new, assig_date_new, code_mo_new, code_head_new, code_mp_new, code_ms_new, snils_mt_new, "
                                           "       subj_name, dstr_name, city_name, nasp_name, strt_name, house, corp ) "

                                           "select " + QString::number(id_move_line) + ", 'cnt_new', "
                                           "       d.id_person, d.fam, d.im, d.ot, d.sex, d.date_birth, d.snils, "
                                           "       d.id_polis, d.pol_v, d.enp, d.vs_num, d.pol_ser, d.pol_num, "
                                           "       d.assig_type,  d.assig_date,  d.code_mo,  d.code_head,  d.code_mp,  d.code_ms,  d.snils_mt,  "
                                           "       d1.assig_type, d1.assig_date, d1.code_mo, d1.code_head, d1.code_mp, d1.code_ms, d1.snils_mt, "
                                           "       d.subj_name, d.dstr_name, d.city_name, d.nasp_name, d.strt_name, d.house, d.corp "

                                           "  from assig.sexages_data d "
                                           //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                           "  left join assig.sexages_data d0 on(d0.id_layer=" + QString::number(id_layer_old) + " and d0.id_person=d.id_person) "
                                           //"  left join spr_medorgs mo0 on(mo0.code_mo=d0.code_mo) "

                                           " where d.id_layer=" + QString::number(id_layer_new) + " "
                                           "   and d.code_head='" + code_head + "' "
                                           "   #ASSIG_TYPE# "
                                           " order by d.fam, d.im, d.ot, d.date_birth, d.sex, d.snils ; ";
                    sql_new_list = sql_new_list.replace("#ASSIG_TYPE#", assig_type_d);

                    QSqlQuery *query_new_list = new QSqlQuery(db);
                    bool res_new_list = query_new_list->exec(sql_new_list);

                    if (!res_new_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании старого списка прикреплённых",
                                             "При формировании старого списка прикреплённых произошла неожиданная ошибка.\n\n"
                                             + query_new_list->lastError().driverText() + "\n" + query_new_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_new_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_new_list;
                }
                */


                // === //
                // add //
                // === //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", прикреплённые к МО за месяц ...   ");
                QApplication::processEvents();

                QString sql_add = "select count(distinct d.id_person) as cnt_add "
                                  "  from assig.sexages_data d "
                                  //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                  "  left join assig.sexages_data d0 on(d0.id_layer=" + QString::number(id_layer_old) + " and d0.id_person=d.id_person) "
                                  //"  left join spr_medorgs mo0 on(mo0.code_mo=d0.code_mo) "

                                  " where d.id_layer=" + QString::number(id_layer_new) + " "
                                  "   and d.code_head='" + code_head + "' "
                                  "   #ASSIG_TYPE+# "
                                  "   and ( d0.code_head is NULL"
                                  "         or d0.code_head<>'" + code_head + "' "
                                  "         or (d0.code_head='" + code_head + "' and (d0.assig_type is NULL or d0.assig_type<>d.assig_type)) ) ; ";
                sql_add = sql_add.replace("#ASSIG_TYPE#", assig_type_d).
                        replace("#ASSIG_TYPE+#", assig_type_s);

                QSqlQuery *query_add = new QSqlQuery(db);
                bool res_add = query_add->exec(sql_add);

                if (!res_add) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении числа новых прикреплённых лиц",
                                         "При получении числа новых прикреплённых лиц произошла неожиданная ошибка.\n\n"
                                         + query_add->lastError().driverText() + "\n" + query_add->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_add;
                    delete query_mo_list;
                    db.rollback();
                    return;
                }
                query_add->next();
                cnt_add = query_add->value(0).toInt();
                delete query_add;

                if (cnt_add>0) {
                    // сформируем список m_add
                    QString sql_add_list = // удалим старый список
                                           "delete from assig.moves_data d "
                                           " where d.id_layer_old=" + QString::number(id_layer_old) + " "
                                           "   and d.id_layer_new=" + QString::number(id_layer_new) + " "
                                           "   and d.code_head_new='" + code_head + "' "
                                           "   and d.cell_name='cnt_add' "
                                           "   #ASSIG_TYPE# ; "

                                           // добавим новый список
                                           "insert into assig.moves_data "
                                           "     ( assig_type, id_layer_old, id_layer_new, id_move_line, cell_name, "
                                           "       id_person, fam, im, ot, sex, date_birth, snils, "
                                           "       id_polis, pol_v, enp, vs_num, pol_ser, pol_num, "
                                           "       assig_type_old, assig_date_old, code_mo_old, code_head_old, code_mp_old, code_ms_old, snils_mt_old, "
                                           "       assig_type_new, assig_date_new, code_mo_new, code_head_new, code_mp_new, code_ms_new, snils_mt_new, "
                                           "       subj_name, dstr_name, city_name, nasp_name, strt_name, house, corp, quart ) "

                                           "select " + QString::number(assig_type) + ", " + QString::number(id_layer_old) + ", " + QString::number(id_layer_new) + ", " + QString::number(id_move_line) + ", 'cnt_add', "
                                           "       d.id_person, d.fam, d.im, d.ot, d.sex, d.date_birth, d.snils, "
                                           "       d.id_polis, d.pol_v, d.enp, d.vs_num, d.pol_ser, d.pol_num, "
                                           "       d0.assig_type, d0.assig_date, d0.code_mo, d0.code_head, d0.code_mp, d0.code_ms, d0.snils_mt, "
                                           "       d.assig_type,  d.assig_date,  d.code_mo,  d.code_head,  d.code_mp,  d.code_ms,  d.snils_mt,  "
                                           "       d.subj_name, d.dstr_name, d.city_name, d.nasp_name, d.strt_name, d.house, d.corp, d.quart "

                                           "  from assig.sexages_data d "
                                           "  left join assig.sexages_data d0 on(d0.id_layer=" + QString::number(id_layer_old) + " and d0.id_person=d.id_person) "

                                           " where d.id_layer=" + QString::number(id_layer_new) + " "
                                           "   and d.code_head='" + code_head + "' "
                                           "   #ASSIG_TYPE+# "
                                           "   and ( d0.code_head is NULL"
                                           "         or d0.code_head<>'" + code_head + "' "
                                           "         or (d0.code_head='" + code_head + "' and (d0.assig_type is NULL or d0.assig_type<>d.assig_type)) ) ";
                                           //" order by d.fam, d.im, d.ot, d.date_birth, d.sex, d.snils ; ";
                    sql_add_list = sql_add_list.replace("#ASSIG_TYPE#", assig_type_d).
                            replace("#ASSIG_TYPE+#", assig_type_s);

                    QSqlQuery *query_add_list = new QSqlQuery(db);
                    bool res_add_list = query_add_list->exec(sql_add_list);

                    if (!res_add_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании списка новых прикреплённых лиц",
                                             "При формировании списка новых прикреплённых лиц произошла неожиданная ошибка.\n\n"
                                             + query_add_list->lastError().driverText() + "\n" + query_add_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_add_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_add_list;
                }


                // === //
                // dec //
                // === //
                ui->lab_wait->setText("   ... (" + QString::number(mo_n) + "/" + QString::number(mo_cnt) + ")  " + code_head + ", откреплённые от МО за месяц ...   ");
                QApplication::processEvents();

                QString sql_dec = "select count(distinct d.id_person) as cnt_dec "
                                  "  from assig.sexages_data d "
                                  //"  left join spr_medorgs mo on(mo.code_mo=d.code_mo) "
                                  "  left join assig.sexages_data d1 on(d1.id_layer=" + QString::number(id_layer_new) + " and d1.id_person=d.id_person) "
                                  //"  left join spr_medorgs mo1 on(mo1.code_mo=d1.code_mo) "

                                  " where d.id_layer=" + QString::number(id_layer_old) + " "
                                  "   and d.code_head='" + code_head + "' "
                                  "   #ASSIG_TYPE+# "
                                  "   and ( d1.code_head is NULL"
                                  "         or d1.code_head<>'" + code_head + "' "
                                  "         or (d1.code_head='" + code_head + "' and (d1.assig_type is NULL or d1.assig_type<>d.assig_type)) ) ; ";
                sql_dec = sql_dec.replace("#ASSIG_TYPE#", assig_type_d).
                        replace("#ASSIG_TYPE+#", assig_type_s);

                QSqlQuery *query_dec = new QSqlQuery(db);
                bool res_dec = query_dec->exec(sql_dec);

                if (!res_dec) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при получении числа новых прикреплённых",
                                         "При получении числа новых прикреплённых произошла неожиданная ошибка.\n\n"
                                         + query_dec->lastError().driverText() + "\n" + query_dec->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_dec;
                    delete query_mo_list;
                    db.rollback();
                    return;
                }
                query_dec->next();
                cnt_dec = query_dec->value(0).toInt();
                delete query_dec;

                if (cnt_dec>0) {
                    // сформируем список m_dec
                    QString sql_dec_list = // удалим старый список
                                           "delete from assig.moves_data d "
                                           " where d.id_layer_old=" + QString::number(id_layer_old) + " "
                                           "   and d.id_layer_new=" + QString::number(id_layer_new) + " "
                                           "   and d.code_head_old='" + code_head + "' "
                                           "   and d.cell_name='cnt_dec' "
                                           "   #ASSIG_TYPE# ; "

                                           // добавим новый список
                                           "insert into assig.moves_data "
                                           "     ( assig_type, id_layer_old, id_layer_new, id_move_line, cell_name, "
                                           "       id_person, fam, im, ot, sex, date_birth, snils, "
                                           "       id_polis, pol_v, enp, vs_num, pol_ser, pol_num, "
                                           "       assig_type_old, assig_date_old, code_mo_old, code_head_old, code_mp_old, code_ms_old, snils_mt_old, "
                                           "       assig_type_new, assig_date_new, code_mo_new, code_head_new, code_mp_new, code_ms_new, snils_mt_new, "
                                           "       subj_name, dstr_name, city_name, nasp_name, strt_name, house, corp, quart ) "

                                           "select " + QString::number(assig_type) + ", " + QString::number(id_layer_old) + ", " + QString::number(id_layer_new) + ", " + QString::number(id_move_line) + ", 'cnt_dec', "
                                           "       d.id_person, d.fam, d.im, d.ot, d.sex, d.date_birth, d.snils, "
                                           "       d.id_polis, d.pol_v, d.enp, d.vs_num, d.pol_ser, d.pol_num, "
                                           "       d.assig_type,  d.assig_date,  d.code_mo,  d.code_head,  d.code_mp,  d.code_ms,  d.snils_mt,  "
                                           "       d1.assig_type, d1.assig_date, d1.code_mo, d1.code_head, d1.code_mp, d1.code_ms, d1.snils_mt, "
                                           "       d.subj_name, d.dstr_name, d.city_name, d.nasp_name, d.strt_name, d.house, d.corp, d.quart "

                                           "  from assig.sexages_data d "
                                           "  left join assig.sexages_data d1 on(d1.id_layer=" + QString::number(id_layer_new) + " and d1.id_person=d.id_person) "

                                           " where d.id_layer=" + QString::number(id_layer_old) + " "
                                           "   and d.code_head='" + code_head + "' "
                                           "   #ASSIG_TYPE+# "
                                           "   and ( d1.code_head is NULL"
                                           "         or d1.code_head<>'" + code_head + "' "
                                           "         or (d1.code_head='" + code_head + "' and (d1.assig_type is NULL or d1.assig_type<>d.assig_type)) ) ";
                                          // " order by d.fam, d.im, d.ot, d.date_birth, d.sex, d.snils ; ";
                    sql_dec_list = sql_dec_list.replace("#ASSIG_TYPE#", assig_type_d).
                            replace("#ASSIG_TYPE+#", assig_type_s);

                    QSqlQuery *query_dec_list = new QSqlQuery(db);
                    bool res_dec_list = query_dec_list->exec(sql_dec_list);

                    if (!res_dec_list) {
                        this->setCursor(Qt::ArrowCursor);
                        QMessageBox::warning(this, "Ошибка при формировании списка откреплённых от МО за месяц",
                                             "При формировании списка новых прикреплённых произошла неожиданная ошибка.\n\n"
                                             + query_dec_list->lastError().driverText() + "\n" + query_dec_list->lastError().databaseText() + "\n\n"
                                             "Операция отменена.");
                        delete query_dec_list;
                        delete query_mo_list;
                        db.rollback();
                        ui->pan_wait->setVisible(false);
                        return;
                    }
                    delete query_dec_list;
                }






                // ============================================= //
                // сохраним строчку половозрастной в базу данных //
                // ============================================= //
                QString sql_save_line = "update assig.moves_lines  "
                                        "   set cnt_old=" + QString::number(cnt_old) + ", "
                                        "       cnt_add=" + QString::number(cnt_add) + ", "
                                        "       cnt_dec=" + QString::number(cnt_dec) + ", "
                                        "       cnt_new=" + QString::number(cnt_new) + " "
                                        " where id="  + QString::number(id_move_line) + " ; ";
                QSqlQuery *query_save_line = new QSqlQuery(db);
                bool res_save_line = query_save_line->exec(sql_save_line);

                if (!res_save_line) {
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox::warning(this, "Ошибка при сохранении строчки таблицы движения прикреплённых застрахованных в базу данных",
                                         "При сохранении строчки таблицы движения прикреплённых застрахованных в базу данных произошла неожиданная ошибка.\n\n"
                                         + query_save_line->lastError().driverText() + "\n" + query_save_line->lastError().databaseText() + "\n\n"
                                         "Операция отменена.");
                    delete query_save_line;
                    delete query_mo_list;
                    db.rollback();
                    ui->pan_wait->setVisible(false);
                    return;
                }
                delete query_save_line;


                //------------------------------//
                // отображение статуса на экран //
                //------------------------------//
                ui->lab_cnt->setText(" -//- ");

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

                QApplication::processEvents();

            };
            delete query_mo_list;

            // обновим статистику таблицы
            ui->lab_wait->setText("   ... обновление статистики таблицы движения ...   ");
            QApplication::processEvents();

            QString sql_stat = "update assig.moves m "
                               "   set cnt_lines=(select count(*) from assig.moves_lines ml where ml.id_move=" + QString::number(id_move) + ") "
                               " where m.id=" + QString::number(id_move) + " ; ";
            QSqlQuery *query_stat = new QSqlQuery(db);
            bool res_stat = query_stat->exec(sql_stat);

            if (!res_stat) {
                this->setCursor(Qt::ArrowCursor);
                QMessageBox::warning(this, "Ошибка при подсчёте статистики таблицы движения",
                                     "При подсчёте статистики таблицы движения произошла неожиданная ошибка.\n\n"
                                     + query_stat->lastError().driverText() + "\n" + query_stat->lastError().databaseText() + "\n\n"
                                     "Операция отменена.");
                delete query_stat;
                db.rollback();
                ui->pan_wait->setVisible(false);
                return;
            }
            delete query_stat;


            db.commit();

            ui->pan_assiglays->setEnabled(true);
            ui->pan_sexages->setEnabled(true);
            ui->pan_moves->setEnabled(true);
            ui->pan_DN->setEnabled(true);
            ui->pan_wait->setVisible(false);
            QApplication::processEvents();

        } else {
            QMessageBox::information(this, "Отмена операции",
                                     "Генерация таблицы движения прикреплённых застрахованных отменена пользователем.\n"
                                     "Ничего не изменилось.");
        }
    } else {
        QMessageBox::information(this, "Ничего не выбрано",
                                 "Для продолжения операции сначал выберите снимок, по которому хотите построить таблицу движения прикреплённых застрахованных, в разрезе МО.");
    }
    refresh_assiglays_tab();
    refresh_sexages_tab();
    refresh_moves_tab();
    this->setCursor(Qt::ArrowCursor);
}

void assiglays_wnd::on_bn_delete_move_clicked() {
    int id_move = -1;

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_moves->selectionModel()->selection().indexes();
    this->setCursor(Qt::WaitCursor);

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        id_move = model_moves.data(model_moves.index(index.row(), 0), Qt::EditRole).toInt();       // id

        if (QMessageBox::question(this, "Нужно подтверждение",
                                  "Вы действительно хотите удалить выбранную таблицу движения прикреплённых лиц?\n\n"
                                  "Это необратимая операция.",
                                  QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
            QMessageBox::warning(this, "Отмена удаления",
                                 "Удаление таблицы отменено пользователем.\n"
                                 "Ничего не изменилось.");
            return;
        }

        // собственно удаление данных таблицы
        QString sql_delete = "delete from assig.moves_data d "
                             " where d.id_move_line in "
                             "       ( select ml.id "
                             "           from assig.moves_lines ml "
                             "          where ml.id_move=" + QString::number(id_move) + " ) ; "

                             "delete from assig.moves_lines ml "
                             " where ml.id_move=" + QString::number(id_move) + " ; "

                             "delete from assig.moves  m "
                             " where m.id=" + QString::number(id_move) + " ; ";
        QSqlQuery *query_delete = new QSqlQuery(db);
        bool res_delete = query_delete->exec(sql_delete);

        if (!res_delete) {
            this->setCursor(Qt::ArrowCursor);
            QMessageBox::warning(this, "Ошибка при удалении таблицы движения прикреплённых лиц",
                                 "При удалении таблицы движения прикреплённых лиц произошла неожиданная ошибка.\n\n"
                                 + query_delete->lastError().driverText() + "\n" + query_delete->lastError().databaseText() + "\n\n"
                                 "Операция отменена.");
            delete query_delete;
            db.rollback();
            return;
        }
        delete query_delete;

        refresh_assiglays_tab();
        refresh_sexages_tab();
        refresh_moves_tab();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Выберите таблицу движения прикреплённых лиц для удаления.");
    }
    this->setCursor(Qt::ArrowCursor);
}

void assiglays_wnd::on_bn_open_move_clicked() {
    int id_move = -1;

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_moves->selectionModel()->selection().indexes();
    this->setCursor(Qt::WaitCursor);

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        id_move = model_moves.data(model_moves.index(index.row(), 0), Qt::EditRole).toInt();       // e.id

        delete show_move_w;
        show_move_w = new show_move_wnd(id_move, db, data_app, settings, this);

        show_move_w->exec();
    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Выберите строку таблицы движения прикреплённого населения, данные которой хотите увидеть.");
    }
    this->setCursor(Qt::ArrowCursor);
}

void assiglays_wnd::on_bn_matrix_clicked() {
    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_assiglays->selectionModel()->selection().indexes();
    this->setCursor(Qt::WaitCursor);

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        int layer_type = model_assiglays.data(model_assiglays.index(index.row(), 3), Qt::EditRole).toInt();     // assiglay_type
        int assig_type = 2;
        int id_layer_old = -1;
        QDate layer_date_old = QDate();
        int id_layer_new = model_assiglays.data(model_assiglays.index(index.row(), 0), Qt::EditRole).toInt();
        QDate layer_date_new = model_assiglays.data(model_assiglays.index(index.row(), 2), Qt::EditRole).toDate();

        delete add_matrix_w;
        add_matrix_w = new add_matrix_wnd(layer_type, assig_type, id_layer_old, layer_date_old, id_layer_new, layer_date_new, db, data_app, settings, this);

        if (add_matrix_w->exec()) {
            // рассчитаем движение прикреплённых застрахованных по снимку
            ui->pan_assiglays->setEnabled(false);
            ui->pan_sexages->setEnabled(false);
            ui->pan_moves->setEnabled(false);
            ui->pan_DN->setEnabled(false);
            ui->pan_wait->setVisible(true);
            ui->lab_wait->setText("   ... ПОДОЖДИТЕ, ИДЁТ ПОСТРОЕНИЕ МАТРИЦЫ ...   ");
            QApplication::processEvents();

            db.transaction();

            QString sql_mo_list;
            code_mo_list.clear();

            // получим список головных медорганизаций, к которым может делаться прикрепление застрахованных
            sql_mo_list+= "select * from "
                          "  ( select mo.code_mo, mo.name_mo  "
                          "      from spr_medorgs mo "
                          "     where mo.mo_assig>0 "
                          "   union "
                          "    select 'нет', 'не прикреплен' "
                          "  ) q_mo "
                          " order by q_mo.code_mo ; ";

            QSqlQuery *query_mo_list = new QSqlQuery(db);
            bool res_mo_list = query_mo_list->exec(sql_mo_list);

            if (!res_mo_list) {
                this->setCursor(Qt::ArrowCursor);
                QMessageBox::warning(this, "Ошибка при получении списка медорганизаций",
                                     "При получении списка медорганизаций произошла неожиданная ошибка.\n\n"
                                     + query_mo_list->lastError().driverText() + "\n" + query_mo_list->lastError().databaseText() + "\n\n"
                                     "Операция отменена.");
                delete query_mo_list;
                db.rollback();
                ui->pan_assiglays->setEnabled(true);
                ui->pan_sexages->setEnabled(true);
                ui->pan_moves->setEnabled(true);
                ui->pan_DN->setEnabled(true);
                ui->pan_wait->setVisible(false);
                return;
            }
            int mo_cnt = 0;
            while (query_mo_list->next()) {
                code_mo_list.append(query_mo_list->value(0).toString());
                mo_cnt++;
            }
            delete query_mo_list;


            // фильтр по способу прикрепления
            QString assig_type_d = " ";
            bool f_assig_type_d = false;
            switch (assig_type) {
            case 1:
                assig_type_d = " and d_n.assig_type=1 ";
                f_assig_type_d = true;
                break;
            case 2:
                assig_type_d = " and d_n.assig_type=2 ";
                f_assig_type_d = true;
                break;
            default:
                assig_type_d = " and d_n.assig_type=0 ";
                f_assig_type_d = false;
                break;
            }

            QString assig_type_s = " ";
            bool f_assig_type_s = false;
            switch (assig_type) {
            case 1:
                assig_type_s = " and d_n.assig_type=1 ";
                f_assig_type_s = true;
                break;
            case 2:
                assig_type_s = " and d_n.assig_type=2 ";
                f_assig_type_s = true;
                break;
            default:
                assig_type_s = " ";
                f_assig_type_s = false;
                break;
            }


            // запрос-ячейка
            QString sql_flds_cell =  " , q_#CODE_HEAD_NEW#.cnt_cell as n_#CODE_HEAD_NEW# ";

            QString sql_from_cell =  " left join ( "
                                     "  select d_o.code_head as code_mo_old, d_n.code_head as code_mo_new, count(distinct d_n.id_person) as cnt_cell "
                                     "    from assig.sexages_data d_n "
                                     "    left join assig.sexages_data d_o on(d_o.id_layer=" + QString::number(id_layer_old) + " and d_o.id_person=d_n.id_person) "
                                     "   where d_n.id_layer=" + QString::number(id_layer_new) + " "
                                     "     and d_n.code_head='#CODE_HEAD_NEW#'"
                                     "     #ASSIG_TYPE+# "
                                     "   group by d_o.code_head, d_n.code_head "
                                     " ) q_#CODE_HEAD_NEW# on(coalesce(q_#CODE_HEAD_NEW#.code_mo_old, 'нет')=q_mo.code_mo ) ";

            QString sql_from_null =  " left join ( "
                                     "  select d_o.code_head as code_mo_old, d_n.code_head as code_mo_new, count(distinct d_n.id_person) as cnt_cell "
                                     "    from assig.sexages_data d_n "
                                     "    left join assig.sexages_data d_o on(d_o.id_layer=" + QString::number(id_layer_old) + " and d_o.id_person=d_n.id_person) "
                                     "   where d_n.id_layer=" + QString::number(id_layer_new) + " "
                                     "     and d_n.code_head is NULL "
                                     "   group by d_o.code_head, d_n.code_head "
                                     " ) q_#CODE_HEAD_NEW# on(coalesce(q_#CODE_HEAD_NEW#.code_mo_old, 'нет')=q_mo.code_mo ) ";

            date0 = QDate::currentDate();
            time0 = QTime::currentTime();

            int mo_n = 0;

            QString sql_flds = "select row_number() OVER(), q_mo.code_mo, q_mo.name_mo ";
            QString sql_from = "from "
                               " ( select * from "
                               "   ( select mo.code_mo, mo.name_mo  "
                               "       from spr_medorgs mo "
                               "      where mo.mo_assig>0 "
                               "    union "
                               "     select 'нет', 'не прикреплен' "
                               "   ) q_mo "
                               "   order by q_mo.code_mo "
                               " ) q_mo ";

            QString sql_matrix;

            // выберем старую МО
            int n_new=0;
            foreach (QString code_mo_new, code_mo_list) {
                //n_new++;  if (n_new>3)  break;

                // список полей
                QString sql_flds_n = sql_flds_cell;
                sql_flds_n = sql_flds_n.
                        replace("#CODE_HEAD_NEW#", code_mo_new).
                        replace("#ASSIG_TYPE#", assig_type_d).
                        replace("#ASSIG_TYPE+#", assig_type_s);
                sql_flds += sql_flds_n;

                // список таблиц
                QString sql_from_n = sql_from_cell;
                if (code_mo_new=="нет") sql_from_n = sql_from_null;
                sql_from_n = sql_from_n.
                        replace("#CODE_HEAD_NEW#", code_mo_new).
                        replace("#ASSIG_TYPE#", assig_type_d).
                        replace("#ASSIG_TYPE+#", assig_type_s);
                sql_from += sql_from_n;

                mo_n++;
            }

            sql_matrix = sql_flds + " " + sql_from;

            /*delete show_tab_w;
            this->setCursor(Qt::ArrowCursor);
            show_tab_w = new show_tab_wnd(, sql_matrix, db, data_app, this);
            show_tab_w->exec();*/
            bool res = sql2csv(db, sql_matrix, true,
                               data_app.path_out + "_ASSIG_DATA_/" + QDate::currentDate().toString("yyyy-MM-dd") + "  -  " + data_app.smo_short + " - " + data_app.filial_name + "  -  "
                               + "Матрица  на " + layer_date_old.toString("yyyy-MM-dd") + " - " + layer_date_new.toString("yyyy-MM-dd")
                               + QString(assig_type==2 ? "  по заявлениям" : (assig_type==1 ? "  по территории" : "  вся")) + QString(layer_type==2 ? "  (ТФОМС)" : "")
                               + ".csv",
                               this);
            this->setCursor(Qt::ArrowCursor);
            db.commit();
            ui->pan_assiglays->setEnabled(true);
            ui->pan_sexages->setEnabled(true);
            ui->pan_moves->setEnabled(true);
            ui->pan_DN->setEnabled(true);
            ui->pan_wait->setVisible(false);

            QMessageBox::information(this, "Готово",
                                     "Готово.");

        } else {
            QMessageBox::warning(this, "Отмена формирования МАТРИЦЫ",
                                 "Действие отменео пользователем.");
        }
    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Выберите снимок данных, для которго хотите построить МАТРИЦУ движения застрахованных в разрезе МО.");
    }
    ui->pan_assiglays->setEnabled(true);
    ui->pan_sexages->setEnabled(true);
    ui->pan_moves->setEnabled(true);
    ui->pan_DN->setEnabled(true);
    ui->pan_wait->setVisible(false);
}

void assiglays_wnd::on_bn_load_TFOMS_clicked() {
    int id_sexage = -1;

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_sexages->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        this->setCursor(Qt::WaitCursor);
        QModelIndex index = indexes.front();

        id_sexage = model_sexages.data(model_sexages.index(index.row(), 0), Qt::EditRole).toInt();

        delete load_sexage_w;
        load_sexage_w = new load_sexage_wnd(id_sexage, db, data_app, settings, this);

        if (load_sexage_w->exec()) {
            delete show_sexage_w;
            show_sexage_w = new show_sexage_wnd(id_sexage, db, data_app, settings, this);
            show_sexage_w->exec();

            refresh_sexages_tab();
        } else {
            QMessageBox::warning(this, "Отмена операции",
                                 "Подгрузка таблицы половозрастного от ТФОМС отменена пользователем.\n\n"
                                 "Ничего не изменилось.");
        };
        this->setCursor(Qt::ArrowCursor);

    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Выберите строку таблицы половозрастного распреледения, данные которой хотите увидеть.");
    }
}

void assiglays_wnd::on_bn_match_assiglays_clicked() {
    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_assiglays->selectionModel()->selection().indexes();
    this->setCursor(Qt::WaitCursor);
    ui->pan_wait->setVisible(true);
    ui->lab_wait->setText("   ... СРАВНЕНИЕ СНИМКОВ  ИНКО и ТФОМС ...   ");
    QApplication::processEvents();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        int id_layer = model_assiglays.data(model_assiglays.index(index.row(), 0), Qt::EditRole).toInt();       // e.id
        int layer_type = model_assiglays.data(model_assiglays.index(index.row(), 3), Qt::EditRole).toInt();     // assiglay_type
        QDate layer_date = model_assiglays.data(model_assiglays.index(index.row(), 2), Qt::EditRole).toDate();     // assiglay_date
        QString layer_name = model_assiglays.data(model_assiglays.index(index.row(), 1), Qt::EditRole).toString();     // assiglay_name

        int inko_id_layer, fond_id_layer;
        QDate inko_layer_date, fond_layer_date;
        QString inko_layer_name, fond_layer_name;

        switch (layer_type) {
        case 2: {
                fond_id_layer = id_layer;
                fond_layer_date = layer_date;
                fond_layer_name = layer_name;

                // поищем парный снимок ИНКО
                QString sql_lay2 = "select id, layer_name, layer_date, layer_type "
                                   "  from assig.layers "
                                   " where (layer_type=1 and layer_date='" + fond_layer_date.toString("yyyy-MM-dd") + "') ; ";
                QSqlQuery *query_lay2 = new QSqlQuery(db);
                bool res_lay2 = query_lay2->exec(sql_lay2);
                if (!res_lay2 || !query_lay2->next()) {
                    QString err_db = query_lay2->lastError().databaseText();
                    QString err_drv = query_lay2->lastError().driverText();
                    delete query_lay2;
                    this->setCursor(Qt::ArrowCursor);
                    ui->pan_wait->setVisible(false);
                    QMessageBox::warning(this, "Ошибка при поиске парного снимка",
                                         "При попытке найти снимок ИНКО парный выбранному снимку ТФОМС произошла ошибка.\n"
                                         + sql_lay2 + "\n\n"
                                         + err_db   + "\n"
                                         + err_drv  + "\n\n"
                                         "Операция отменена.\n");
                    return;
                }
                inko_id_layer = query_lay2->value(0).toInt();
                inko_layer_name = query_lay2->value(1).toString();
                inko_layer_date = query_lay2->value(2).toDate();
            }
            break;
        default: {
                inko_id_layer = id_layer;
                inko_layer_date = layer_date;
                inko_layer_name = layer_name;

                // поищем парный снимок ТФОМС
                QString sql_lay2 = "select id, layer_name, layer_date, layer_type "
                                   "  from assig.layers "
                                   " where (layer_type=2 and layer_date='" + inko_layer_date.toString("yyyy-MM-dd") + "') ; ";
                QSqlQuery *query_lay2 = new QSqlQuery(db);
                bool res_lay2 = query_lay2->exec(sql_lay2);
                if (!res_lay2 || !query_lay2->next()) {
                    QString err_db = query_lay2->lastError().databaseText();
                    QString err_drv = query_lay2->lastError().driverText();
                    delete query_lay2;
                    this->setCursor(Qt::ArrowCursor);
                    ui->pan_wait->setVisible(false);
                    QMessageBox::warning(this, "Ошибка при поиске парного снимка",
                                         "При попытке найти снимок ТФОМС парный выбранному снимку ИНКО произошла ошибка.\n"
                                         + sql_lay2 + "\n\n"
                                         + err_db   + "\n"
                                         + err_drv  + "\n\n"
                                         "Операция отменена.\n");
                    return;
                }
                fond_id_layer = query_lay2->value(0).toInt();
                fond_layer_name = query_lay2->value(1).toString();
                fond_layer_date = query_lay2->value(2).toDate();
            }
            break;
        }

        // выберем разницу двух снимков
        QString sql_data = "select case when s1.code_mo=s2.code_mo then ' + прикрепление совпадает' "
                           "            when s1.code_mo is not NULL and s2.code_mo is not NULL and s1.code_mo <> s2.code_mo then ' + прикрепление к разным МО' "
                           "            when s1.code_mo is not NULL and s2.code_mo is NULL then ' - ИНКО - прикрепление есть,  ТФОМС - прикрепления нет' "
                           "            when s1.code_mo is NULL and s2.code_mo is not NULL then ' - ИНКО - прикрепления нет,  ТФОМС - прикрепление есть' "
                           "            when s1.code_mo is NULL and s2.code_mo is NULL then ' + прикрепления нет ни в ИНКО, ни в ТФОМС' "
                           "            end as text, "
                           "       s1.id_person, s1.fam, s1.im, s1.ot, s1.date_birth, s1.enp, "
                           "       s1.assig_type as assig_type_inko, s1.assig_date as assig_date_inko, s1.code_mo as code_mo_inko, s1.code_head as code_head_inko, s1.code_mp as code_mp_inko, s1.code_ms as code_ms_inko, s1.snils_mt as snils_mt_inko, "
                           "       s2.assig_type as assig_type_fond, s2.assig_date as assig_date_fond, s2.code_mo as code_mo_fond, s2.code_head as code_head_fond, s2.code_mp as code_mp_fond, s2.code_ms as code_ms_fond, s2.snils_mt as snils_mt_fond  "
                           "  from assig.sexages_data s1 "
                           "  left join persons e on(e.id=s1.id_person) "
                           "  left join polises p on(p.id=s1.id_polis) "
                           "  left join assig.sexages_data s2 on(s2.id_person=s1.id_person  and  s2.id_layer=" + QString::number(fond_id_layer) + "  and  s2.code_mo is not NULL  and  s2.id_polis is not NULL ) "
                           " where s1.id_layer=" + QString::number(inko_id_layer) + " "
                           "   and ( p.date_stop is NULL or p.date_stop< ) "
                           "   and s1.code_mo is not NULL "
                           "   and s1.id_polis is not NULL "
                           "   and ( 1=2 ";
        if (ui->ch_diff_assig->isChecked())
            sql_data  +=   "      or (s1.code_mo is not NULL and s2.code_mo is not NULL and s1.code_mo <> s2.code_mo and coalesce(s1.code_head, '0') <> coalesce(s2.code_head, '0') ) ";
        if (ui->ch_match_fond->isChecked())
            sql_data  +=   "      or (s1.code_mo is not NULL and s2.code_mo is NULL) ";
        if (ui->ch_match_inko->isChecked())
            sql_data  +=   "      or (s1.code_mo is NULL and s2.code_mo is not NULL) ";
        if (ui->ch_unassigned->isChecked())
            sql_data  +=   "      or (s1.code_mo is NULL and s2.code_mo is NULL) ";

        sql_data  +=       "       )"
                           " order by s1.code_mo is NULL, s2.code_mo is NULL, s1.fam, s1.im, s1.ot, s1.date_birth ; ";

        this->setCursor(Qt::ArrowCursor);
        delete show_tab_w;

        show_tab_w = new show_tab_wnd("Сравнение снимков прикрепления ИНКО и ТФОМС за " + layer_date.toString("yyyy-MM-dd"), sql_data, db, data_app, this);
        show_tab_w->exec();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Выберите снимок данных, который надо сравнить с парным ему снимком данных ТФОМС/ИНКО.");
    }
    this->setCursor(Qt::ArrowCursor);
    ui->pan_wait->setVisible(false);
}

void assiglays_wnd::on_ch_match_fond_clicked(bool checked) {
    ui->bn_match_assiglays->setEnabled ( ui->ch_match_fond->isChecked()
                                      || ui->ch_match_inko->isChecked()
                                      || ui->ch_diff_assig->isChecked()
                                      || ui->ch_unassigned->isChecked() );
}
void assiglays_wnd::on_ch_match_inko_clicked(bool checked) {
    ui->bn_match_assiglays->setEnabled ( ui->ch_match_fond->isChecked()
                                      || ui->ch_match_inko->isChecked()
                                      || ui->ch_diff_assig->isChecked()
                                      || ui->ch_unassigned->isChecked() );
}
void assiglays_wnd::on_ch_diff_assig_clicked(bool checked) {
    ui->bn_match_assiglays->setEnabled ( ui->ch_match_fond->isChecked()
                                      || ui->ch_match_inko->isChecked()
                                      || ui->ch_diff_assig->isChecked()
                                      || ui->ch_unassigned->isChecked() );
}
void assiglays_wnd::on_ch_unassigned_clicked(bool checked) {
    ui->bn_match_assiglays->setEnabled ( ui->ch_match_fond->isChecked()
                                      || ui->ch_match_inko->isChecked()
                                      || ui->ch_diff_assig->isChecked()
                                      || ui->ch_unassigned->isChecked() );
}

void assiglays_wnd::on_bn_match_unassig_clicked() {
    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_assiglays->selectionModel()->selection().indexes();
    this->setCursor(Qt::WaitCursor);
    ui->pan_wait->setVisible(true);
    ui->lab_wait->setText("   ... НЕ ПРИКРЕПЛЁННЫЕ ЖИТЕЛИ РЕГИОНА ...   ");
    QApplication::processEvents();

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        int id_layer = model_assiglays.data(model_assiglays.index(index.row(), 0), Qt::EditRole).toInt();       // e.id
        int layer_type = model_assiglays.data(model_assiglays.index(index.row(), 3), Qt::EditRole).toInt();     // assiglay_type
        QDate layer_date = model_assiglays.data(model_assiglays.index(index.row(), 2), Qt::EditRole).toDate();     // assiglay_date
        QString layer_name = model_assiglays.data(model_assiglays.index(index.row(), 1), Qt::EditRole).toString();     // assiglay_name

        int inko_id_layer, fond_id_layer;
        QDate inko_layer_date, fond_layer_date;
        QString inko_layer_name, fond_layer_name;

        // выберем данные для снимка ИНКО
        QString sql_data = "select 'не прикреплённый житель региона - снимок ИНКО' as text, "
                           "       s1.id_person, s1.fam, s1.im, s1.ot, s1.date_birth, s1.enp, "
                           "       s1.assig_type as assig_type_inko, s1.assig_date as assig_date_inko, s1.code_mo as code_mo_inko, s1.code_head as code_head_inko, s1.code_mp as code_mp_inko, s1.code_ms as code_ms_inko, s1.snils_mt as snils_mt_inko, "
                           "       ar.id as id_addr_reg, ar.subj, ar.subj_name, ar.dstr, ar.dstr_name, ar.city, ar.city_name, ar.nasp, ar.nasp_name, ar.strt, ar.strt_name, "
                           "       al.id as id_addr_liv, al.subj, al.subj_name, al.dstr, al.dstr_name, al.city, al.city_name, al.nasp, al.nasp_name, al.strt, al.strt_name "
                           "  from assig.sexages_data s1 "
                           "  left join persons e on(e.id=s1.id_person) "
                           "  left join addresses ar on(ar.id=e.id_addr_reg) "
                           "  left join addresses al on(al.id=e.id_addr_liv) "
                           " where s1.id_layer=" + QString::number(id_layer) + " "
                           "   and ar.subj=(select subj from db_recent limit 1) "
                           "   and s1.code_mo is NULL "
                           " order by s1.fam, s1.im, s1.ot, s1.date_birth ; ";

        if (layer_type==2) {
            // для фондовского снимка - особый запрос
            fond_id_layer = id_layer;
            fond_layer_date = layer_date;
            fond_layer_name = layer_name;

            // поищем парный снимок ИНКО
            QString sql_lay2 = "select id, layer_name, layer_date, layer_type "
                               "  from assig.layers "
                               " where (layer_type=1 and layer_date='" + fond_layer_date.toString("yyyy-MM-dd") + "') ; ";
            QSqlQuery *query_lay2 = new QSqlQuery(db);
            bool res_lay2 = query_lay2->exec(sql_lay2);
            if (!res_lay2 || !query_lay2->next()) {
                QString err_db = query_lay2->lastError().databaseText();
                QString err_drv = query_lay2->lastError().driverText();
                delete query_lay2;
                this->setCursor(Qt::ArrowCursor);
                ui->pan_wait->setVisible(false);
                QMessageBox::warning(this, "Ошибка при поиске парного снимка",
                                     "При попытке найти снимок ИНКО парный выбранному снимку ТФОМС произошла ошибка.\n"
                                     + sql_lay2 + "\n\n"
                                     + err_db   + "\n"
                                     + err_drv  + "\n\n"
                                     "Операция отменена.\n");
                return;
            }
            inko_id_layer = query_lay2->value(0).toInt();
            inko_layer_name = query_lay2->value(1).toString();
            inko_layer_date = query_lay2->value(2).toDate();

            // выберем данные из снимка ИНКО, парного этому снимку фонда
            sql_data = "select 'не прикреплённый житель региона - снимок ТФОМС' as text, "
                       "       s1.id_person, s1.fam, s1.im, s1.ot, s1.date_birth, s1.enp, "
                       "       s1.assig_type as assig_type_inko, s1.assig_date as assig_date_inko, s1.code_mo as code_mo_inko, s1.code_head as code_head_inko, s1.code_mp as code_mp_inko, s1.code_ms as code_ms_inko, s1.snils_mt as snils_mt_inko, "
                       "       s2.assig_type as assig_type_fond, s2.assig_date as assig_date_fond, s2.code_mo as code_mo_fond, s2.code_head as code_head_fond, s2.code_mp as code_mp_fond, s2.code_ms as code_ms_fond, s2.snils_mt as snils_mt_fond, "
                       "       ar.id as id_addr_reg, ar.subj, ar.subj_name, ar.dstr, ar.dstr_name, ar.city, ar.city_name, ar.nasp, ar.nasp_name, ar.strt, ar.strt_name, "
                       "       al.id as id_addr_liv, al.subj, al.subj_name, al.dstr, al.dstr_name, al.city, al.city_name, al.nasp, al.nasp_name, al.strt, al.strt_name "
                       "  from assig.sexages_data s1 "
                       "  left join assig.sexages_data s2 on(s2.id_person=s1.id_person  and  s2.id_layer=" + QString::number(fond_id_layer) + ") "
                       "  left join persons e on(e.id=s1.id_person) "
                       "  left join addresses ar on(ar.id=e.id_addr_reg) "
                       "  left join addresses al on(al.id=e.id_addr_liv) "
                       " where s1.id_layer=" + QString::number(inko_id_layer) + " "
                       "   and ar.subj=(select subj from db_recent limit 1) "
                       "   and s2.code_mo is NULL "
                       " order by s1.fam, s1.im, s1.ot, s1.date_birth ; ";
        }

        this->setCursor(Qt::ArrowCursor);
        delete show_tab_w;

        show_tab_w = new show_tab_wnd("Не прикреплённые жители региона - снимок " + QString(layer_type==2 ? "ТФОМС" : "ИНКО") + " за " + layer_date.toString("yyyy-MM-dd"), sql_data, db, data_app, this);
        show_tab_w->exec();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Выберите снимок данных, по которому надо проверить прикрепление жителей региона.");
    }
    this->setCursor(Qt::ArrowCursor);
    ui->pan_wait->setVisible(false);
}
