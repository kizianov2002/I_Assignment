#include "finance_mo_coef_wnd.h"
#include "ui_finance_mo_coef_wnd.h"

#include <QMessageBox>

finance_mo_coef_wnd::finance_mo_coef_wnd(QString &code_mo, QSqlDatabase &db, s_data_app &data_app, QSettings &settings,  /*QString &name_mo, double &tarif, double &m_0_0, double &w_0_0, double &m_1_4, double &w_1_4, double &m_5_17, double &w_5_17, double &m_18_59, double &w_18_54, double &m_60_, double &w_55_, double &coef_dif,*/ QWidget *parent) :
    code_mo(code_mo),
    db(db),
    data_app(data_app),
    settings(settings),
    /*name_mo(name_mo),
    date(date),
    tarif(tarif),
    m_0_0(m_0_0),
    w_0_0(w_0_0),
    m_1_4(m_1_4),
    w_1_4(w_1_4),
    m_5_17(m_5_17),
    w_5_17(w_5_17),
    m_18_59(m_18_59),
    w_18_54(w_18_54),
    m_60_(m_60_),
    w_55_(w_55_),
    coef_dif(coef_dif),*/
    QDialog(parent),
    ui(new Ui::finance_mo_coef_wnd)
{
    ui->setupUi(this);

    id_coefs = -1;
    date_begin = QDate::currentDate();
    date_end = QDate();

    refresh_combo_medorg();
    if (!code_mo.isEmpty())
        ui->combo_medorg->setCurrentIndex(ui->combo_medorg->findData(code_mo));

    refresh_coefs_tab();
}

finance_mo_coef_wnd::~finance_mo_coef_wnd() {
    delete ui;
}

void finance_mo_coef_wnd::on_bn_cansel_clicked() {
    if (QMessageBox::question(this, "Нужно подтверждение",
                              "Закрыть окно правки коэффициентов?\n",
                              QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::No)
        return;
    reject();
}

void finance_mo_coef_wnd::refresh_combo_medorg() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка медорганизаций
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mo, '(' || code_mo || ') ' || name_mo "
                  "  from spr_medorgs s "
                  " where mo_assig=1 "
                  " order by code_mo ; ";
    mySQL.exec(this, sql, QString("Справочник медорганизаций для таблицы коэффициентов"), *query, true, db, data_app);
    ui->combo_medorg->clear();
    ui->combo_medorg->addItem(" - все медорганизации - ", " - все - ");
    while (query->next()) {
        ui->combo_medorg->addItem(query->value(1).toString(), query->value(0).toString());
    }
    ui->combo_medorg->setCurrentIndex(0);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void finance_mo_coef_wnd::on_bn_today_clicked() {
    ui->date->setDate(QDate::currentDate());
}



void finance_mo_coef_wnd::refresh_coefs_tab() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }

    this->setCursor(Qt::WaitCursor);
    QApplication::processEvents();

    QString coefs_sql;
    coefs_sql  = "select id, "
                 "       code_mo, "
                 "       date_begin, "
                 "       date_end, "
                 "       tarif, "
                 "       coef_m_0_0, "
                 "       coef_w_0_0, "
                 "       coef_m_1_4, "
                 "       coef_w_1_4, "
                 "       coef_m_5_17, "
                 "       coef_w_5_17, "
                 "       coef_m_18_59, "
                 "       coef_w_18_54, "
                 "       coef_m_60_, "
                 "       coef_w_55_, "
                 "       coef_dif "
                 "  from assig.coef_finance_medorgs_tab c " ;

    bool is_first = true;
    if (ui->combo_medorg->currentIndex()>0) {
        if (is_first) {
            coefs_sql += " where ";
            is_first = false;
        } else
            coefs_sql += " and ";
        coefs_sql += " c.code_mo='" + ui->combo_medorg->currentData().toString() + "' ";
    } else {  }
    coefs_sql += " order by c.code_mo, c.date_begin desc ; ";

    model_coefs.setQuery(coefs_sql,db);
    QString err2 = model_coefs.lastError().driverText();

    // подключаем модель из БД
    ui->tab_coefs->setModel(&model_coefs);

    // обновляем таблицу
    ui->tab_coefs->reset();

    // задаём ширину колонок
    ui->tab_coefs->setColumnWidth(  0,  2);     //  id
    ui->tab_coefs->setColumnWidth(  1, 45);     //  code_mo,
    ui->tab_coefs->setColumnWidth(  2, 70);     //  date_begin,
    ui->tab_coefs->setColumnWidth(  3, 70);     //  date_end,
    ui->tab_coefs->setColumnWidth(  4, 45);     //  tarif,
    ui->tab_coefs->setColumnWidth(  5, 50);     //  coef_m_0_0,
    ui->tab_coefs->setColumnWidth(  6, 50);     //  coef_w_0_0,
    ui->tab_coefs->setColumnWidth(  7, 50);     //  coef_m_1_4,
    ui->tab_coefs->setColumnWidth(  8, 50);     //  coef_w_1_4,
    ui->tab_coefs->setColumnWidth(  9, 50);     //  coef_m_5_17,
    ui->tab_coefs->setColumnWidth( 10, 50);     //  coef_w_5_17,
    ui->tab_coefs->setColumnWidth( 11, 60);     //  coef_m_18_59,
    ui->tab_coefs->setColumnWidth( 12, 60);     //  coef_w_18_54,
    ui->tab_coefs->setColumnWidth( 13, 55);     //  coef_m_60_,
    ui->tab_coefs->setColumnWidth( 14, 55);     //  coef_w_55_,
    ui->tab_coefs->setColumnWidth( 15, 80);     //  coef_dif

    // правим заголовки
    model_coefs.setHeaderData(  0, Qt::Horizontal, ("ID"));
    model_coefs.setHeaderData(  1, Qt::Horizontal, ("код"));
    model_coefs.setHeaderData(  2, Qt::Horizontal, ("дата\nначала"));
    model_coefs.setHeaderData(  3, Qt::Horizontal, ("дата\nокончания"));
    model_coefs.setHeaderData(  4, Qt::Horizontal, ("тариф"));
    model_coefs.setHeaderData(  5, Qt::Horizontal, ("коэфф.\n0-0  М"));
    model_coefs.setHeaderData(  6, Qt::Horizontal, ("коэфф.\n0-0  Ж"));
    model_coefs.setHeaderData(  7, Qt::Horizontal, ("коэфф.\n1-4  М"));
    model_coefs.setHeaderData(  8, Qt::Horizontal, ("коэфф.\n1-4  Ж"));
    model_coefs.setHeaderData(  9, Qt::Horizontal, ("коэфф.\n5-17  М"));
    model_coefs.setHeaderData( 10, Qt::Horizontal, ("коэфф.\n5-17  Ж"));
    model_coefs.setHeaderData( 11, Qt::Horizontal, ("коэфф.\n18-59  М"));
    model_coefs.setHeaderData( 12, Qt::Horizontal, ("коэфф.\n18-54  Ж"));
    model_coefs.setHeaderData( 13, Qt::Horizontal, ("коэфф.\n60<  М"));
    model_coefs.setHeaderData( 14, Qt::Horizontal, ("коэфф.\n55<  Ж"));
    model_coefs.setHeaderData( 15, Qt::Horizontal, ("коэфф.\nдифференц."));
    ui->tab_coefs->repaint();

    ui->date->setDate(QDate::currentDate());

    this->setCursor(Qt::ArrowCursor);
}


void finance_mo_coef_wnd::on_tab_coefs_clicked(const QModelIndex &index) {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_coefs->selectionModel()->selection().indexes();
    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        id_coefs             = model_coefs.data(model_coefs.index(index.row(), 0), Qt::EditRole).toInt();
        code_mo              = model_coefs.data(model_coefs.index(index.row(), 1), Qt::EditRole).toString();
        date_begin           = model_coefs.data(model_coefs.index(index.row(), 2), Qt::EditRole).toDate();
        date_end             = model_coefs.data(model_coefs.index(index.row(), 3), Qt::EditRole).toDate();

        double  tarif        = model_coefs.data(model_coefs.index(index.row(), 4), Qt::EditRole).toDouble();
        double  coef_m_0_0   = model_coefs.data(model_coefs.index(index.row(), 5), Qt::EditRole).toDouble();
        double  coef_w_0_0   = model_coefs.data(model_coefs.index(index.row(), 6), Qt::EditRole).toDouble();
        double  coef_m_1_4   = model_coefs.data(model_coefs.index(index.row(), 7), Qt::EditRole).toDouble();
        double  coef_w_1_4   = model_coefs.data(model_coefs.index(index.row(), 8), Qt::EditRole).toDouble();
        double  coef_m_5_17  = model_coefs.data(model_coefs.index(index.row(), 9), Qt::EditRole).toDouble();
        double  coef_w_5_17  = model_coefs.data(model_coefs.index(index.row(),10), Qt::EditRole).toDouble();
        double  coef_m_18_59 = model_coefs.data(model_coefs.index(index.row(),11), Qt::EditRole).toDouble();
        double  coef_w_18_54 = model_coefs.data(model_coefs.index(index.row(),12), Qt::EditRole).toDouble();
        double  coef_m_60_   = model_coefs.data(model_coefs.index(index.row(),13), Qt::EditRole).toDouble();
        double  coef_w_55_   = model_coefs.data(model_coefs.index(index.row(),14), Qt::EditRole).toDouble();
        double  coef_dif     = model_coefs.data(model_coefs.index(index.row(),15), Qt::EditRole).toDouble();


        ui->lab_code_mo->setText(code_mo);
        ui->date->setDate(date_begin);

        ui->spin_tarif   ->setValue(tarif);
        ui->spin_m_0_0   ->setValue(coef_m_0_0);
        ui->spin_w_0_0   ->setValue(coef_w_0_0);
        ui->spin_m_1_4   ->setValue(coef_m_1_4);
        ui->spin_w_1_4   ->setValue(coef_w_1_4);
        ui->spin_m_5_17  ->setValue(coef_m_5_17);
        ui->spin_w_5_17  ->setValue(coef_w_5_17);
        ui->spin_m_18_59 ->setValue(coef_m_18_59);
        ui->spin_w_18_54 ->setValue(coef_w_18_54);
        ui->spin_m_60_   ->setValue(coef_m_60_);
        ui->spin_w_55_   ->setValue(coef_w_55_);
        ui->spin_coef_dif->setValue(coef_dif);
    } else {
        id_coefs = -1;
        ui->lab_code_mo->setText(" - ??? - ");
        ui->date->setDate(QDate::currentDate());

        ui->spin_tarif   ->setValue(0);
        ui->spin_m_0_0   ->setValue(0);
        ui->spin_w_0_0   ->setValue(0);
        ui->spin_m_1_4   ->setValue(0);
        ui->spin_w_1_4   ->setValue(0);
        ui->spin_m_5_17  ->setValue(0);
        ui->spin_w_5_17  ->setValue(0);
        ui->spin_m_18_59 ->setValue(0);
        ui->spin_w_18_54 ->setValue(0);
        ui->spin_m_60_   ->setValue(0);
        ui->spin_w_55_   ->setValue(0);
        ui->spin_coef_dif->setValue(0);
    }
}

void finance_mo_coef_wnd::on_combo_medorg_activated(int index) {
    refresh_coefs_tab();
    ui->lab_code_mo->setText(ui->combo_medorg->currentData().toString());

    /*
    if (index<1) {
        // без коэффициента дифференциации
        ui->label_14->setEnabled(false);
        ui->spin_coef_dif->setEnabled(false);
    } else {
        ui->label_14->setEnabled(true);
        ui->spin_coef_dif->setEnabled(true);
    }
    */
}

void finance_mo_coef_wnd::on_bn_save_coef_clicked() {
    db.transaction();

    // проверка правильности задания даты
    if (ui->date->date()==QDate::currentDate()) {
        if (QMessageBox::question(this, "Дата начала действия - сегодня?",
                                  "Заданная дата начала действия записи совпадает с сегодняшним числом!\n"
                                  "Продолжить?\n", QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
            QMessageBox::information(this, "Действие отменено пользователем", "Действие отменено пользователем.\nДаныне в базе не изменились.");
            db.rollback();
            return;
        }
    }

    date_begin = ui->date->date();
    date_end = QDate();

    // проверка выбора МО
    if (ui->combo_medorg->currentIndex()<1) {
        if (QMessageBox::question(this, "Медорганизация не выбюрана",
                                  "Медицинская организация не выбрана и сделанные изменения будут применены сразу ко всем МО в списке!\n"
                                  "Продолжить?\n", QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
            QMessageBox::information(this, "Действие отменено пользователем", "Действие отменено пользователем.\nДаныне в базе не изменились.");
            db.rollback();
            return;
        }
        code_mo = "";
    } else {
        code_mo = ui->combo_medorg->currentData().toString();
    }

    // проверим наличие перекрытых записей
    QString sql_test = "select count(*) cnt"
                       "  from assig.coef_finance_medorgs_tab c "
                       " where date_begin>='" + date_begin.toString("yyyy-MM-dd") + "' ";
    if (!code_mo.isEmpty())
        sql_test  +=   "   and code_mo='" + code_mo + "' ; ";

    QSqlQuery *query_test = new QSqlQuery(db);
    bool res_test = mySQL.exec(this, sql_test, QString("проверка на перекрытие существующих данных"), *query_test, true, db, data_app);
    if (!res_test) {
        delete query_test;
        this->setCursor(Qt::ArrowCursor);
        db.rollback();
        QMessageBox::warning(this, "Ошибка при проверке на перекрытие существующих данных",
                             "При проверке на перекрытие существующих данных произошла неожиданная ошибка");
        return;
    }
    query_test->next();
    int cnt = query_test->value(0).toInt();
    delete query_test;
    if (cnt>0) {
        if (QMessageBox::question(this, "Перекрытие существующих данных!",
                                  "Внимание!\n\n"
                                  "При добавлении новой записи будет удалено " + QString::number(cnt) +
                                  " строк уже существующих данных с датой начала действия не раньше " + date_begin.toString("dd.MM.yyyy") + "\n\n"
                                  "Продолжить?\n", QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
            QMessageBox::information(this, "Действие отменено пользователем", "Действие отменено пользователем.\nДаныне в базе не изменились.");
            db.rollback();
            return;
        }

        // удалим перекрытые записи
        QString sql_del = "delete from assig.coef_finance_medorgs_tab "
                          " where date_begin>='" + date_begin.toString("yyyy-MM-dd") + "' ";
        if (!code_mo.isEmpty())
            sql_del  +=   "   and code_mo='" + code_mo + "' ; ";

        QSqlQuery *query_del = new QSqlQuery(db);
        bool res_del = mySQL.exec(this, sql_del, QString("удаление перекрытых записей"), *query_del, true, db, data_app);
        if (!res_del) {
            delete query_del;
            this->setCursor(Qt::ArrowCursor);
            db.rollback();
            QMessageBox::warning(this, "Ошибка при удалении перекрытых записей",
                                 "При удалении перекрытых записей произошла неожиданная ошибка");
            return;
        }
        delete query_del;
    }

    // закроем не закрытые записи
    QString sql_close = "update assig.coef_finance_medorgs_tab c "
                        "   set date_end='" + date_begin.toString("yyyy-MM-dd") + "' "
                        " where date_end is NULL ";
    if (!code_mo.isEmpty())
        sql_close  +=   "   and code_mo='" + code_mo + "' ; ";

    QSqlQuery *query_close = new QSqlQuery(db);
    bool res_close = mySQL.exec(this, sql_close, QString("закрытие не закрытых записей"), *query_close, true, db, data_app);
    if (!res_close) {
        delete query_close;
        this->setCursor(Qt::ArrowCursor);
        db.rollback();
        QMessageBox::warning(this, "Ошибка при закрытии не закрытых записей",
                             "При закрытии не закрытых записей произошла неожиданная ошибка");
        return;
    }
    delete query_close;

    // добавим новую запись/записи
    QString sql_add;
    if (!code_mo.isEmpty()) {
        // добавим новую запись для одной МО
        sql_add = "insert into assig.coef_finance_medorgs_tab (code_mo, date_begin, date_end, tarif, coef_m_0_0, coef_w_0_0, coef_m_1_4, coef_w_1_4, coef_m_5_17, coef_w_5_17, coef_m_18_59, coef_w_18_54, coef_m_60_, coef_w_55_, coef_dif) "
                  "values ('" + code_mo                                     + "', "
                  "        '" + date_begin.toString("yyyy-MM-dd")           + "', "
                  "         NULL, "
                  "         " + QString::number(ui->spin_tarif->value())    + ", "
                  "         " + QString::number(ui->spin_m_0_0->value())    + ", "
                  "         " + QString::number(ui->spin_w_0_0->value())    + ", "
                  "         " + QString::number(ui->spin_m_1_4->value())    + ", "
                  "         " + QString::number(ui->spin_w_1_4->value())    + ", "
                  "         " + QString::number(ui->spin_m_5_17->value())   + ", "
                  "         " + QString::number(ui->spin_w_5_17->value())   + ", "
                  "         " + QString::number(ui->spin_m_18_59->value())  + ", "
                  "         " + QString::number(ui->spin_w_18_54->value())  + ", "
                  "         " + QString::number(ui->spin_m_60_->value())    + ", "
                  "         " + QString::number(ui->spin_w_55_->value())    + ", "
                  "         " + QString::number(ui->spin_coef_dif->value()) + ") "
                  " ; ";
    } else {
        // добавим записи для всех МО, к которым делается прикрепление
        // !!!  коэфффициент дифференциации = 1  !!!
        sql_add = "insert into assig.coef_finance_medorgs_tab (code_mo, date_begin, date_end, tarif, coef_m_0_0, coef_w_0_0, coef_m_1_4, coef_w_1_4, coef_m_5_17, coef_w_5_17, coef_m_18_59, coef_w_18_54, coef_m_60_, coef_w_55_, coef_dif) "
                  "select mo.code_mo, "
                  "      '" + date_begin.toString("yyyy-MM-dd")           + "', "
                  "       NULL, "
                  "       " + QString::number(ui->spin_tarif->value())    + ", "
                  "       " + QString::number(ui->spin_m_0_0->value())    + ", "
                  "       " + QString::number(ui->spin_w_0_0->value())    + ", "
                  "       " + QString::number(ui->spin_m_1_4->value())    + ", "
                  "       " + QString::number(ui->spin_w_1_4->value())    + ", "
                  "       " + QString::number(ui->spin_m_5_17->value())   + ", "
                  "       " + QString::number(ui->spin_w_5_17->value())   + ", "
                  "       " + QString::number(ui->spin_m_18_59->value())  + ", "
                  "       " + QString::number(ui->spin_w_18_54->value())  + ", "
                  "       " + QString::number(ui->spin_m_60_->value())    + ", "
                  "       " + QString::number(ui->spin_w_55_->value())    + ", "
                  "       1 "
                  "  from spr_medorgs mo "
                  " where mo.mo_assig=1 "
                  " ; ";
        // скопируем значение коэффициента дифференциации из предыдущей записи !!!
        sql_add+= "UPDATE assig.coef_finance_medorgs_tab c "
                  "   set coef_dif = (select cc.coef_dif from assig.coef_finance_medorgs_tab cc where cc.code_mo=c.code_mo and c.date_begin=cc.date_end limit 1) "
                  " where c.date_end is NULL "
                  "   and c.coef_dif=1 "
                  "   and (select cc.coef_dif from assig.coef_finance_medorgs_tab cc where cc.code_mo=c.code_mo and c.date_begin=cc.date_end limit 1) is not NULL "
                  " ; ";
    }
    QSqlQuery *query_add = new QSqlQuery(db);
    bool res_add = mySQL.exec(this, sql_add, QString("добавление записи/записей коэффиициентов"), *query_add, true, db, data_app);
    if (!res_add) {
        delete query_add;
        this->setCursor(Qt::ArrowCursor);
        db.rollback();
        QMessageBox::warning(this, "Ошибка при добавлении записи/записей коэффиициентов",
                             "При добавлении записи/записей коэффиициентов произошла неожиданная ошибка");
        return;
    }
    delete query_add;

    // все данные загружены
    db.commit();
    refresh_coefs_tab();
}

void finance_mo_coef_wnd::on_bn_refresh_coefs_tab_clicked() {
    refresh_coefs_tab();
}

void finance_mo_coef_wnd::on_bn_delete_coef_clicked() {
    db.transaction();

    if (QMessageBox::question(this, "Нужно подтверждение",
                              "Вы действительно хотите удалить выбранную запись?\n"
                              "Это необратимая операция.\n\n"
                              "Продолжить?\n", QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
        QMessageBox::information(this, "Действие отменено пользователем", "Действие отменено пользователем.\nДаныне в базе не изменились.");
        db.rollback();
        return;
    }

    // удалим перекрытые записи
    QString sql_del = "delete from assig.coef_finance_medorgs_tab "
                      " where code_mo='" + code_mo + "' "
                      "   and date_begin>='" + date_begin.toString("yyyy-MM-dd") + "' ; ";
    QSqlQuery *query_del = new QSqlQuery(db);
    bool res_del = mySQL.exec(this, sql_del, QString("удаление перекрытых записей"), *query_del, true, db, data_app);
    if (!res_del) {
        delete query_del;
        this->setCursor(Qt::ArrowCursor);
        db.rollback();
        QMessageBox::warning(this, "Ошибка при удалении перекрытых записей",
                             "При удалении перекрытых записей произошла неожиданная ошибка");
        return;
    }
    delete query_del;
}

void finance_mo_coef_wnd::on_tab_coefs_activated(const QModelIndex &index)  {
    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_coefs->selectionModel()->selection().indexes();
    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        // выберем МО по коду
        code_mo = model_coefs.data(model_coefs.index(index.row(), 1), Qt::EditRole).toString();
        ui->combo_medorg->setCurrentIndex(ui->combo_medorg->findData(code_mo));
        refresh_coefs_tab();
    }
}

void finance_mo_coef_wnd::on_bn_all_mo_clicked() {
    ui->combo_medorg->setCurrentIndex(0);
    refresh_coefs_tab();
}
