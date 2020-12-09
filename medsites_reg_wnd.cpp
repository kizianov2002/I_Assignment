#include "medsites_reg_wnd.h"
#include "ui_medsites_reg_wnd.h"

#include <QFileDialog>
#include <QDesktopWidget>
#include <QItemSelectionModel>

#include "ki_funcs.h"

medsites_reg_wnd::medsites_reg_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::medsites_reg_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    QList<int> l523;
    l523.append(550);
    l523.append(150);
    l523.append(300);

    // проверим разрешение экрана
    QDesktopWidget *scr_widget = QApplication::desktop();
    int screen_w = scr_widget->width();
    int screen_h = scr_widget->height();

    this->move(100,50);
    this->resize(screen_w-200, screen_h-150);

    ui->split_register->setSizes(l523);

    refresh_register();
}

medsites_reg_wnd::~medsites_reg_wnd() {
    delete ui;
}

void medsites_reg_wnd::on_bn_close_clicked() {
    if (QMessageBox::question(this, "Закрыть окно?", "Закрыть окно?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::Yes) {
        close();
    }
}

void medsites_reg_wnd::refresh_register() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Нет доступа к базе данных",
                             "Нет открытого соединения к базе данных?\n"
                             " Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n"
                             " Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику.");
        return;
    }
    ui->tab_register->setEnabled(false);
    QApplication::processEvents();

    QString sql_where =  " where 1=1 ";
    if ( ui->ch_no_medpart->isChecked()
         || ui->ch_no_medsite->isChecked()
         || ui->ch_no_medter->isChecked() ) {
        sql_where += " and (";
        bool first=true;
        if ( ui->ch_no_medpart->isChecked() ) {
            if (!first) { sql_where += " or ";  first = false; }
            sql_where += " t.id_mp is NULL ";
        };
        if ( ui->ch_no_medsite->isChecked() ) {
            if (!first) { sql_where += " or ";  first = false; }
            sql_where += " t.id_ms is NULL ";
        };
        if ( ui->ch_no_medter->isChecked() ) {
            if (!first) { sql_where += " or ";  first = false; }
            sql_where += " t.id_mt is NULL ";
        };
        sql_where += " ) ";
    }

    QString sql_cnt;
    sql_cnt       = "select count(*) "
                    "  from tfoms_medsites_reg t "
                    "  left join spr_medsites_types s on(s.code=t.type_ms) ";
    sql_cnt      += sql_where;

    QSqlQuery *query_cnt = new QSqlQuery(db);
    bool res_cnt = query_cnt->exec(sql_cnt);
    QString er_1 = db.lastError().databaseText();
    QString er_2 = db.lastError().driverText();
    if (!res_cnt || !query_cnt->next()) {
        db.rollback();
        delete query_cnt;
        QMessageBox::warning(this, "Ошибка подсчёта числа строк реестра ",
                             "При попытке число строк реестра произошла ошибка!\n"
                             "\n db: " + er_1 +
                             "\n driver: " + er_2 +
                             "\n\n sql: " + sql_cnt +
                             "\n\n Операция отменена");
        return;
    }
    int cnt = query_cnt->value(0).toInt();
    ui->lab_cnt_reg->setText(QString::number(cnt));

    QString sql_register;
    sql_register  = "select t.id, t.code_mo, t.code_mp, t.code_ms, t.type_ms, s.text as type_ms_text, t.snils_mt, "
                    "       t.id_mo, t.id_mp, t.id_ms, t.id_mt "
                    "  from tfoms_medsites_reg t "
                    "  left join spr_medsites_types s on(s.code=t.type_ms) ";
    sql_register += sql_where;
    sql_register += " order by t.code_mo, t.code_mp, t.code_ms, t.type_ms, t.snils_mt ; ";

    model_register.setQuery(sql_register, db);
    QString err2 = model_register.lastError().driverText();

    // подключаем модель из БД
    ui->tab_register->setModel(&model_register);

    // обновляем таблицу
    ui->tab_register->reset();

    // задаём ширину колонок
    ui->tab_register->setColumnWidth(  0,  2);   // t.id,
    ui->tab_register->setColumnWidth(  1, 50);   // t.code_mo,
    ui->tab_register->setColumnWidth(  2, 75);   // t.code_mp,
    ui->tab_register->setColumnWidth(  3,200);   // t.code_ms,
    ui->tab_register->setColumnWidth(  4,  2);   // t.type_ms,
    ui->tab_register->setColumnWidth(  5,150);   // s.text as type_ms_text,
    ui->tab_register->setColumnWidth(  6,100);   // t.snils_mt
    ui->tab_register->setColumnWidth(  7, 45);   // t.id_mo
    ui->tab_register->setColumnWidth(  8, 45);   // t.id_mp
    ui->tab_register->setColumnWidth(  9, 45);   // t.id_ms
    ui->tab_register->setColumnWidth( 10, 45);   // t.id_mt

    // правим заголовки
    model_register.setHeaderData( 0, Qt::Horizontal, ("ID"));             // t.id,
    model_register.setHeaderData( 1, Qt::Horizontal, ("код\nМО"));        // t.code_mo,
    model_register.setHeaderData( 2, Qt::Horizontal, ("код\nподразд."));  // t.code_mp,
    model_register.setHeaderData( 3, Qt::Horizontal, ("код\nучастка"));   // t.code_ms,
    model_register.setHeaderData( 5, Qt::Horizontal, ("тип\nучастка"));   // s.text as type_ms_text,
    model_register.setHeaderData( 6, Qt::Horizontal, ("СНИЛС\nврача"));   // t.snils_mt
    ui->tab_register->repaint();

    ui->tab_register->setEnabled(true);
    QApplication::processEvents();
    this->setCursor(Qt::ArrowCursor);

    refresh_medparts();
}

void medsites_reg_wnd::refresh_medparts() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Нет доступа к базе данных",
                             "Нет открытого соединения к базе данных?\n"
                             " Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n"
                             " Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику.");
        return;
    }
    ui->tab_medparts->setEnabled(false);
    QApplication::processEvents();

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_register->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        this->setCursor(Qt::WaitCursor);
        QModelIndex index = indexes.front();

        QString code_mo = model_register.data(model_register.index(index.row(), 1), Qt::EditRole).toString();

        QString sql_cnt;
        sql_cnt       = "select count(*) "
                        "  from spr_medparts t "
                        " where t.code_mo='" + code_mo + "' ; ";

        QSqlQuery *query_cnt = new QSqlQuery(db);
        bool res_cnt = query_cnt->exec(sql_cnt);
        QString er_1 = db.lastError().databaseText();
        QString er_2 = db.lastError().driverText();
        if (!res_cnt || !query_cnt->next()) {
            db.rollback();
            delete query_cnt;
            QMessageBox::warning(this, "Ошибка подсчёта числа подразделений ",
                                 "При попытке подсчитать число подразделений произошла ошибка!\n"
                                 "\n db: " + er_1 +
                                 "\n driver: " + er_2 +
                                 "\n\n sql: " + sql_cnt +
                                 "\n\n Операция отменена");
            return;
        }
        int cnt = query_cnt->value(0).toInt();
        ui->lab_cnt_mp->setText(QString::number(cnt));

        QString sql_medparts;
        sql_medparts  = "select t.id, t.code_mo, t.code_mp "
                        "  from spr_medparts t "
                        " where t.code_mo='" + code_mo + "' "
                        " order by t.code_mo, t.code_mp ; ";

        model_medparts.setQuery(sql_medparts, db);
        QString err2 = model_medparts.lastError().driverText();

        // подключаем модель из БД
        ui->tab_medparts->setModel(&model_medparts);

        // обновляем таблицу
        ui->tab_medparts->reset();

        // задаём ширину колонок
        ui->tab_medparts->setColumnWidth( 0,  2);   // t.id,
        ui->tab_medparts->setColumnWidth( 1, 50);   // t.code_mo,
        ui->tab_medparts->setColumnWidth( 2,150);   // t.code_mp

        // правим заголовки
        model_medparts.setHeaderData( 0, Qt::Horizontal, ("ID"));                // t.id,
        model_medparts.setHeaderData( 1, Qt::Horizontal, ("код\nМО"));           // t.code_mo,
        model_medparts.setHeaderData( 2, Qt::Horizontal, ("код\nподразд."));     // t.code_mp
        ui->tab_medparts->repaint();

        ui->tab_medparts->setEnabled(true);
        QApplication::processEvents();
    }
    ui->pan_medparts_DN->setEnabled(false);
    this->setCursor(Qt::ArrowCursor);

    refresh_medsites();
}

void medsites_reg_wnd::refresh_medsites() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Нет доступа к базе данных",
                             "Нет открытого соединения к базе данных?\n"
                             " Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n"
                             " Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику.");
        return;
    }
    ui->tab_medsites->setEnabled(false);
    QApplication::processEvents();

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_register->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        this->setCursor(Qt::WaitCursor);
        QModelIndex index = indexes.front();

        QString code_mo = model_register.data(model_register.index(index.row(), 1), Qt::EditRole).toString();

        QString sql_cnt;
        sql_cnt       = "select count(*) "
                        "  from spr_medsites t "
                        "  left join spr_medsites_types s on(s.code=t.type_ms) "
                        " where t.code_mo='" + code_mo + "' "
                        "   and upper(t.code_ms) like('%" + ui->line_filter_ms->text().trimmed().toUpper() + "%') ";

        QSqlQuery *query_cnt = new QSqlQuery(db);
        bool res_cnt = query_cnt->exec(sql_cnt);
        QString er_1 = db.lastError().databaseText();
        QString er_2 = db.lastError().driverText();
        if (!res_cnt || !query_cnt->next()) {
            db.rollback();
            delete query_cnt;
            QMessageBox::warning(this, "Ошибка подсчёта числа участков ",
                                 "При попытке подсчитать число участков произошла ошибка!\n"
                                 "\n db: " + er_1 +
                                 "\n driver: " + er_2 +
                                 "\n\n sql: " + sql_cnt +
                                 "\n\n Операция отменена");
            return;
        }
        int cnt = query_cnt->value(0).toInt();
        ui->lab_cnt_ms->setText(QString::number(cnt));

        QString sql_medparts;
        sql_medparts  = "select t.id, t.code_mo, t.code_mp "
                        "  from spr_medparts t "
                        " where t.code_mo='" + code_mo + "' "
                        " order by t.code_mo, t.code_mp ; ";

        QString sql_medsites;
        sql_medsites  = "select t.id, t.code_mo, t.code_mp, t.code_ms, t.type_ms, s.text as type_ms_text, t.name_ms "
                        "  from spr_medsites t "
                        "  left join spr_medsites_types s on(s.code=t.type_ms) "
                        " where t.code_mo='" + code_mo + "' "
                        "   and upper(t.code_ms) like('%" + ui->line_filter_ms->text().trimmed().toUpper() + "%') "
                        " order by t.code_mo, t.code_mp, t.code_ms, t.type_ms, t.name_ms ; ";

        model_medsites.setQuery(sql_medsites, db);
        QString err2 = model_medsites.lastError().driverText();

        // подключаем модель из БД
        ui->tab_medsites->setModel(&model_medsites);

        // обновляем таблицу
        ui->tab_medsites->reset();

        // задаём ширину колонок
        ui->tab_medsites->setColumnWidth(  0,  2);   // t.id,
        ui->tab_medsites->setColumnWidth(  1, 50);   // t.code_mo,
        ui->tab_medsites->setColumnWidth(  2, 75);   // t.code_mp,
        ui->tab_medsites->setColumnWidth(  3,150);   // t.code_ms,
        ui->tab_medsites->setColumnWidth(  4,  2);   // t.type_ms,
        ui->tab_medsites->setColumnWidth(  5,100);   // s.text as type_ms_text,
        ui->tab_medsites->setColumnWidth(  6, 75);   // t.snils_mt

        // правим заголовки
        model_medsites.setHeaderData( 0, Qt::Horizontal, ("ID"));                // t.id,
        model_medsites.setHeaderData( 1, Qt::Horizontal, ("код\nМО"));           // t.code_mo,
        model_medsites.setHeaderData( 2, Qt::Horizontal, ("код\nподразд."));     // t.code_mp,
        model_medsites.setHeaderData( 3, Qt::Horizontal, ("код\nучастка"));      // t.code_ms,
        model_medsites.setHeaderData( 5, Qt::Horizontal, ("тип\nучастка"));      // s.text as type_ms_text,
        model_medsites.setHeaderData( 6, Qt::Horizontal, ("название\nучастка")); // t.name_ms
        ui->tab_medsites->repaint();

        ui->tab_medsites->setEnabled(true);
        QApplication::processEvents();
    }
    ui->pan_medsites_DN->setEnabled(false);
    this->setCursor(Qt::ArrowCursor);
}

void medsites_reg_wnd::on_bn_refresh_register_clicked() {
    refresh_register();
}

void medsites_reg_wnd::on_bn_refresh_medsites_clicked() {
    refresh_medsites();
}

void medsites_reg_wnd::on_bn_refresh_medparts_clicked() {
    refresh_medparts();
}

void medsites_reg_wnd::on_tab_register_clicked(const QModelIndex &index) {
    refresh_medparts();
    refresh_medsites();
}

void medsites_reg_wnd::on_bn_load_regiaster_clicked() {
m_im:
    // надо выбрать файл, полученный от ТФОМС
    QString fullname = QFileDialog::getOpenFileName(this, "Выберите полученный от ТФОМС DBF-файл реестра врачебных участков",
                                                    data_app.path_in, "DBF-files (*.dbf)");
    QFileInfo fi(fullname);
    QString filename = fi.fileName();
    QString basename = fi.baseName();
    QString filepath = fi.filePath();

    if (basename.isEmpty())  return;

    // проверка правильности имени файла
    QString S1 = basename.left(3).toUpper();
    QString code_smo = basename.left(8).right(5);
    QString S2 = basename.left(13).right(4);
    QString S3 = basename.left(16).right(2);
    QString S4 = basename.left(19).right(2);
    if (S1!="UCH") {
        if (QMessageBox::question(this, "Требуется подтверждение",
                                  "Имя выбранного DBF-файла не совпадает со стандартным именем файла имён: \"UCHSSSSS_YYYY_MM_DD_x.dbf\"\n\n"
                                  "Вы действительно хотите загрузить данные из этого файла как информацию о прикреплении застрахованных к медицинской организациии по личному заявлению?",
                                  QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::No)
            goto m_im;
    }
    if (code_smo!="31001") {
        QMessageBox::question(this, "Неправильный реестровый номер СМО!",
                              "Полученный реестровый номер медицинской СТРАХОВОЙ КОМПАНИИ\n"
                              " " + code_smo + "\n"
                              "не является 5-значным реестровым номером филиала \"МСК \"ИНКО-МЕД\" в Белгородской области!\n"
                              "Обработка данных невозможна.\n\n"
                              "Действие отменено.");
        return;
    }
    QString tab_name = S1 + "_" + S2.right(2) + S3;
    QString fullname_new = data_app.path_dbf + tab_name + ".DBF";
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
                                 "Источник данных должен использовать папку, указанную в параметре программы \"path_dbf\":\n"
                                 "При попытке подключиться к источнику данных ODBC \"INKO_DBF\" операционная система вернула отказ. \n"
                                 "Этот источник данных необходим для генерации и чтения dbf-файлов обмена с ТФОМС и МО. \n"
                                 "Если такого источника данных нет - создайте его.\n\n"
                                 + data_app.path_dbf + "\n\n"
                                 "Попробовать снова?", QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)==QMessageBox::No) {
            return;
        }
    }

    // удалим старые данные
    QString sql_del = "delete from tfoms_medsites_reg ; ";
    QSqlQuery *query_del = new QSqlQuery(db);
    bool res_del = query_del->exec(sql_del);
    QString err = db.lastError().databaseText();
    if (!res_del) {
        delete query_del;
        QMessageBox::warning(this, "Ошибка при очистке таблицы импорта",
                             "При попытке очистить таблицу импорта произошла ошибка!\n\n"
                             + sql_del + "\n\n"
                             "Операция отменена");
        db_ODBC.close();
        QSqlDatabase::removeDatabase("INKO_DBF");
        return;
    }
    delete query_del;


    // выберем все данные
    QString sql_ms = "select MCOD, LPU, UCH_ID, TYP, SNILS_VR, DATEBEG "
                     "  from " + tab_name + " ";
    QSqlQuery *query_ms = new QSqlQuery(db_ODBC);
    bool res_ms = query_ms->exec(sql_ms);
    QString er1 = db_ODBC.lastError().databaseText();
    QString er2 = db_ODBC.lastError().driverText();
    if (!res_ms) {
        delete query_ms;
        QMessageBox::warning(this, "Ошибка чтения данных МО ",
                             "При попытке прочитать данные МО произошла ошибка!\n\n"
                             + sql_ms + "\n\n"
                             "Операция отменена");
        db_ODBC.close();
        QSqlDatabase::removeDatabase("INKO_DBF");
        return;
    }

    // зальём новые данные
    db.transaction();
    int n = 0;

    QSqlRecord rec = query_ms->record();
    while (query_ms->next()) {
        // поля
        QString code_mo  = query_ms->value(0).toString().trimmed().toUpper();
        QString code_mp  = query_ms->value(1).toString().trimmed().toUpper();
        QString code_ms  = query_ms->value(2).toString().trimmed().toUpper();
        int     typ__ms  = query_ms->value(3).toInt();
        int     type_ms = 2;
        switch (typ__ms) {
        case 1: type_ms = 2;
            break;
        case 2: type_ms = 1;
            break;
        case 3: type_ms = 5;
            break;
        case 4: type_ms = 3;
            break;
        }
        QString snils_mt = query_ms->value(4).toString().trimmed().toUpper();
        QDate   date_beg = query_ms->value(5).toDate();

        // добавим строку
        QString sql_ins = "insert into tfoms_medsites_reg (code_mo, code_mp, code_ms, type_ms, snils_mt, date_beg) "
                          "values ('" + code_mo  + "', "
                          "        '" + code_mp  + "', "
                          "        '" + code_ms  + "', "
                          "         " + QString::number(type_ms) + ", "
                          "        '" + snils_mt + "', "
                          "        '" + date_beg.toString("yyyy-MM-dd") + "' ); ";
        QSqlQuery *query_ins = new QSqlQuery(db);
        bool res_ins = query_ins->exec(sql_ins);
        QString err = db.lastError().databaseText();
        if (!res_ins) {
            delete query_ins;
            QMessageBox::warning(this, "Ошибка добавления строки реестра участков от ТФОМС",
                                 "При попытке добавить строку реестра участков от ТФОМС произошла ошибка!\n\n"
                                 + sql_ins + "\n\n"
                                 "Операция отменена");
            db_ODBC.close();
            QSqlDatabase::removeDatabase("INKO_DBF");
            return;
        }
        n++;

        delete query_ins;
    }
    delete query_ms;
    QMessageBox::warning(this, "Готово",
                         "Загружено " + QString::number(n) + " новых записей.");

    db.commit();

    refresh_register();
}

void medsites_reg_wnd::on_bn_auto_analize_clicked() {
    db.transaction();

    // по возможности автоматически сопоставим строки реестра существующим записям медорганизаций, подразделений и участков
    QString sql_link = "update tfoms_medsites_reg r "
                       "   set id_mo = (select mo.id from spr_medorgs  mo where mo.code_mo=r.code_mo limit 1), "
                       "       id_mp = (select mp.id from spr_medparts mp where mp.code_mo=r.code_mo and mp.code_mp=r.code_mp limit 1), "
                       "       id_ms = (select ms.id from spr_medsites ms where ms.code_mo=r.code_mo and ms.code_mp=r.code_mp and ms.code_ms=r.code_ms limit 1), "
                       "       id_mt = (select mt.id from spr_medters  mt where mt.code_mo=r.code_mo and mt.snils_mt=r.snils_mt limit 1) ; "

                       "update tfoms_medsites_reg r "
                       "   set id_ms = (select ms.id from spr_medsites ms where ms.code_mo=r.code_mo and ms.code_ms=r.code_ms limit 1) "
                       " where id_ms is NULL ; ";
    QSqlQuery *query_link = new QSqlQuery(db);
    bool res_link = query_link->exec(sql_link);
    QString err = db.lastError().databaseText();
    if (!res_link) {
        delete query_link;
        QMessageBox::warning(this, "Ошибка автосвязывания строк реестра с существующими данными",
                             "При попытке связать строки реестра участков от ТФОМС с существующими данными произошла ошибка!\n\n"
                             + sql_link + "\n\n"
                             "Операция отменена");
        db.rollback();
        return;
    }
    delete query_link;
    db.commit();
}

void medsites_reg_wnd::on_tab_medparts_clicked(const QModelIndex &index) {
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Нет доступа к базе данных",
                             "Нет открытого соединения к базе данных?\n"
                             " Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n"
                             " Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику.");
        return;
    }
    QApplication::processEvents();

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medparts->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        ui->pan_medparts_DN->setEnabled(true);
    }
}

void medsites_reg_wnd::on_tab_medsites_clicked(const QModelIndex &index) {
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Нет доступа к базе данных",
                             "Нет открытого соединения к базе данных?\n"
                             " Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n"
                             " Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику.");
        return;
    }
    QApplication::processEvents();

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_medsites->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        ui->pan_medsites_DN->setEnabled(true);
    }
}

void medsites_reg_wnd::on_bn_use_medpart_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Нет доступа к базе данных",
                             "Нет открытого соединения к базе данных?\n"
                             " Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n"
                             " Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику.");
        return;
    }
    QApplication::processEvents();

    // проверим выделенную строку подразделения
    QModelIndexList indexes_medparts = ui->tab_medparts->selectionModel()->selection().indexes();
    this->setCursor(Qt::WaitCursor);

    if (indexes_medparts.size()>0) {
        QModelIndex index_medpart = indexes_medparts.front();
        int id_medpart = model_medparts.data(model_medparts.index(index_medpart.row(), 0), Qt::EditRole).toInt();

        // проверим выделенную строку реестра
        QModelIndexList indexes_register = ui->tab_register->selectionModel()->selection().indexes();

        db.transaction();

        foreach (QModelIndex index_register, indexes_register) {
            int id_register = model_register.data(model_register.index(index_register.row(), 0), Qt::EditRole).toInt();

            // запишем код подразделения в выбранные строки реестра
            QString sql_link = "update tfoms_medsites_reg r "
                               "   set id_mp = " + QString::number(id_medpart) + " "
                               " where r.id = " + QString::number(id_register) + " ; ";
            QSqlQuery *query_link = new QSqlQuery(db);
            bool res_link = query_link->exec(sql_link);
            QString err = db.lastError().databaseText();
            if (!res_link) {
                delete query_link;
                QMessageBox::warning(this, "Ошибка связывания строки реестра с существующими данными",
                                     "При попытке связать строки реестра участков от ТФОМС с существующим подразделением произошла ошибка!\n\n"
                                     + sql_link + "\n\n"
                                     "Операция отменена");
                db.rollback();
                return;
            }
            delete query_link;
        }
        db.commit();
    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Сначала выберите подразделение, которое хотите сопоставить выбранным участкам в реестре ТФОМС");
        return;
    }
    refresh_register();
}

void medsites_reg_wnd::on_bn_use_medsite_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Нет доступа к базе данных",
                             "Нет открытого соединения к базе данных?\n"
                             " Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n"
                             " Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику.");
        return;
    }
    QApplication::processEvents();

    // проверим выделенную строку участка
    QModelIndexList indexes_medsites = ui->tab_medsites->selectionModel()->selection().indexes();
    this->setCursor(Qt::WaitCursor);

    if (indexes_medsites.size()>0) {
        QModelIndex index_medsite = indexes_medsites.front();
        int id_medsite = model_medsites.data(model_medsites.index(index_medsite.row(), 0), Qt::EditRole).toInt();

        // проверим выделенную строку реестра
        QModelIndexList indexes_register = ui->tab_register->selectionModel()->selection().indexes();

        db.transaction();

        foreach (QModelIndex index_register, indexes_register) {
            int id_register = model_register.data(model_register.index(index_register.row(), 0), Qt::EditRole).toInt();

            // запишем код подразделения в выбранные строки реестра
            QString sql_link = "update tfoms_medsites_reg r "
                               "   set id_ms = " + QString::number(id_medsite) + " "
                               " where r.id = " + QString::number(id_register) + " ; ";
            QSqlQuery *query_link = new QSqlQuery(db);
            bool res_link = query_link->exec(sql_link);
            QString err = db.lastError().databaseText();
            if (!res_link) {
                delete query_link;
                QMessageBox::warning(this, "Ошибка связывания строки реестра с существующими данными",
                                     "При попытке связать строки реестра участков от ТФОМС с существующим участком произошла ошибка!\n\n"
                                     + sql_link + "\n\n"
                                     "Операция отменена");
                db.rollback();
                return;
            }
            delete query_link;
        }
        db.commit();
    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Сначала выберите подразделение, которое хотите сопоставить выбранным участкам в реестре ТФОМС");
        return;
    }
    refresh_register();
}

void medsites_reg_wnd::on_line_filter_ms_editingFinished() {
    refresh_medsites();
}

void medsites_reg_wnd::on_bn_clear_filter_ms_clicked() {
    ui->line_filter_ms->clear();
    refresh_medsites();
}

void medsites_reg_wnd::on_ch_no_medpart_clicked(bool checked) {
    refresh_register();
}
void medsites_reg_wnd::on_ch_no_medsite_clicked(bool checked) {
    refresh_register();
}
void medsites_reg_wnd::on_ch_no_medter_clicked(bool checked) {
    refresh_register();
}

void medsites_reg_wnd::on_bn_Apply2DB_clicked() {
    if (QMessageBox::question(this, "Требуется подтверждение",
                              "Вы действительно хотите применить данные веестра в базу данных?\n"
                              "Это необратимая операция\n\n",
                              QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::No) {
        return;
    }

    db.transaction();

    // обновим данные подразделений по реестру
    QString sql_link;
    sql_link += "update spr_medparts mp "
                "   set code_mo = (select r1.code_mo from tfoms_medsites_reg r1 where mp.id=r1.id_mp limit 1) "
                " where mp.id in(select distinct r.id_mp from tfoms_medsites_reg r where r.id_mp is not NULL) "
                "   and (select r2.code_mo from tfoms_medsites_reg r2 where mp.id=r2.id_mp limit 1) is not NULL ; ";
    sql_link += "update spr_medparts mp "
                "   set code_mp = (select r1.code_mp from tfoms_medsites_reg r1 where mp.id=r1.id_mp limit 1) "
                " where mp.id in(select distinct r.id_mp from tfoms_medsites_reg r where r.id_mp is not NULL) "
                "   and (select r2.code_mp from tfoms_medsites_reg r2 where mp.id=r2.id_mp limit 1) is not NULL ; ";

    // обновим данные участков по реестру
    sql_link += "update spr_medsites ms "
                "   set code_mo = (select r1.code_mo from tfoms_medsites_reg r1 where ms.id=r1.id_ms limit 1) "
                " where ms.id in(select distinct r.id_ms from tfoms_medsites_reg r where r.id_ms is not NULL) "
                "   and (select r2.code_mo from tfoms_medsites_reg r2 where ms.id=r2.id_ms limit 1) is not NULL ; ";
    sql_link += "update spr_medsites ms "
                "   set code_mp = (select r1.code_mp from tfoms_medsites_reg r1 where ms.id=r1.id_ms limit 1) "
                " where ms.id in(select distinct r.id_ms from tfoms_medsites_reg r where r.id_ms is not NULL) "
                "   and (select r2.code_mp from tfoms_medsites_reg r2 where ms.id=r2.id_ms limit 1) is not NULL ; ";
    sql_link += "update spr_medsites ms "
                "   set code_ms = (select r1.code_ms from tfoms_medsites_reg r1 where ms.id=r1.id_ms limit 1) "
                " where ms.id in(select distinct r.id_ms from tfoms_medsites_reg r where r.id_ms is not NULL) "
                "   and (select r2.code_ms from tfoms_medsites_reg r2 where ms.id=r2.id_ms limit 1) is not NULL ; ";
    sql_link += "update spr_medsites ms "
                "   set type_ms = (select r1.type_ms from tfoms_medsites_reg r1 where ms.id=r1.id_ms limit 1) "
                " where ms.id in(select distinct r.id_ms from tfoms_medsites_reg r where r.id_ms is not NULL) "
                "   and (select r2.type_ms from tfoms_medsites_reg r2 where ms.id=r2.id_ms limit 1) is not NULL ; ";

    // добавим недостающие подразделения
    sql_link += "insert into spr_medparts(code_mo, code_mp, name_mp, type_mp) "
                "select distinct r.code_mo, r.code_mp, r.code_mp, 1 "
                "  from tfoms_medsites_reg r "
                " where r.id_mp is NULL "
                "   and (select count(*) from spr_medparts mp where mp.code_mo=r.code_mo and mp.code_mp=r.code_mp)=0 ; ";
    // добавим недостающие участки
    sql_link += "insert into spr_medsites(code_mo, code_mp, code_ms, name_ms, type_ms) "
                "select distinct r.code_mo, r.code_mp, r.code_ms, r.code_ms, r.type_ms "
                "  from tfoms_medsites_reg r "
                " where r.id_ms is NULL "
                "   and (select count(*) from spr_medsites ms where ms.code_mo=r.code_mo and ms.code_mp=r.code_mp and ms.code_ms=r.code_ms)=0 ; ";
    // добавим недостающие записи врачей
    sql_link += "insert into spr_medters(code_mo, code_mt, fio_mt, snils_mt, spec_mt) "
                "select distinct r.code_mo, NULL, NULL, r.snils_mt, 0 "
                "  from tfoms_medsites_reg r "
                " where r.id_mt is NULL "
                "   and r.snils_mt<>'00000000000' "
                "   and (select count(*) from spr_medters mt where mt.code_mo=r.code_mo and mt.snils_mt=r.snils_mt)=0 ; ";


    QSqlQuery *query_link = new QSqlQuery(db);
    bool res_link = query_link->exec(sql_link);
    QString err = db.lastError().databaseText();
    if (!res_link) {
        delete query_link;
        QMessageBox::warning(this, "Ошибка добавления новых участков и подразделений",
                             "При попытке добавлении новых участков и подразделений произошла ошибка!\n\n"
                             + sql_link + "\n\n"
                             "Операция отменена");
        db.rollback();
        return;
    }
    delete query_link;

    db.commit();
}
