#include "accounts_base_wnd.h"
#include "ui_accounts_base_wnd.h"
#include <QMessageBox>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QDate>

#include "ki_funcs.h"

accounts_base_wnd::accounts_base_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::accounts_base_wnd)
{
    ui->setupUi(this);
    ui->pan_wait->setVisible(false);

    acc_db_drvr = data_app.acc_db_drvr;
    acc_db_host = data_app.acc_db_host;
    acc_db_name = data_app.acc_db_name;
    acc_db_port = data_app.acc_db_port;
    acc_db_user = data_app.acc_db_user;
    acc_db_pass = data_app.acc_db_pass;

    ui->combo_db_drvr->setCurrentIndex(ui->combo_db_drvr->findText(data_app.acc_db_drvr));
    ui->line_db_host->setText(data_app.acc_db_host);
    ui->line_db_port->setText(data_app.acc_db_port);
    ui->line_db_name->setText(data_app.acc_db_name);
    ui->line_db_user->setText(data_app.acc_db_user);
    ui->line_db_pass->setText(data_app.acc_db_pass);

    try_connect_acc_Base();
}

bool accounts_base_wnd::try_connect_acc_Base() {
    QApplication::processEvents();
    // ------------------------------------------------ //
    // откроем новое подключение к внешней базе полисов //
    // ------------------------------------------------ //
    db_accBase = QSqlDatabase::addDatabase(ui->combo_db_drvr->currentText(), "acc Database");

    db_accBase.setConnectOptions();
    // сервер базы полисов
    if (ui->line_db_host->text()!="polises_host")
        db_accBase.setHostName(ui->line_db_host->text());
    else db_accBase.setHostName(data_app.host);
    // порт базы полисов
    if (ui->combo_db_drvr->currentText()!="QIBASE")
        if (!ui->line_db_port->text().isEmpty())
            db_accBase.setPort(ui->line_db_port->text().toInt());
    // имя базы полисов
    if (!ui->line_db_name->text().isEmpty())
        db_accBase.setDatabaseName(ui->line_db_name->text());
    // имя пользователя
    if (!ui->line_db_user->text().isEmpty())
        db_accBase.setUserName(ui->line_db_user->text());
    // пароль пользователя
    if (!ui->line_db_pass->text().isEmpty())
        db_accBase.setPassword(ui->line_db_pass->text());

    if (!db_accBase.open()) {
        // подключение не удалось
        ui->bn_copy_data->setEnabled(false);

        QMessageBox::critical(this,"Ошибка подключения к внешней базе полисов",
                              QString("ВНИМАНИЕ!!!\n\n") +
                              "При попытке подключиться к внешней базе полисов произошла ошибка.\n"
                              "Позвоните мне по телефону \n\n"
                              "8-920-449-54-88\n\n"
                              "Александр\n\n" +
                              "acc_db_driv = " + ui->combo_db_drvr->currentText() + "\n" +
                              "acc_db_host = " + ui->line_db_host->text() + "\n" +
                              "acc_db_port = " + ui->line_db_port->text() + "\n" +
                              "acc_db_name = " + ui->line_db_name->text() + "\n" +
                              "acc_db_user = " + ui->line_db_user->text() + "\n" +
                              "acc_db_pass = " + ui->line_db_pass->text() + "\n");
    } else {
        ui->bn_close->setEnabled(true);
        ui->push_connect->setEnabled(true);
        ui->bn_copy_data->setEnabled(true);

        acc_db_drvr = ui->combo_db_drvr->currentText();
        acc_db_host = ui->line_db_host->text();
        acc_db_name = ui->line_db_name->text();
        acc_db_port = ui->line_db_port->text();
        acc_db_user = ui->line_db_user->text();
        acc_db_pass = ui->line_db_pass->text();

        QApplication::processEvents();

        QMessageBox::information(this,"Внешняя база полисов доступна",
                                 QString("Доступ к внешней базе полисов получен.\n"));
    }
    if (db_accBase.isOpen()) {
        // подключение готово
        return true;
    } else {
        return false;
    }
}

accounts_base_wnd::~accounts_base_wnd() {
    delete ui;
}

void accounts_base_wnd::on_bn_close_clicked() {
    close();
}

void accounts_base_wnd::on_push_connect_clicked() {
    if (try_connect_acc_Base()) {
    }
}

void accounts_base_wnd::on_combo_db_drvr_activated(int index) {
    if (ui->combo_db_drvr->currentText()=="QIBASE") {
        ui->line_db_host->setText("172.20.250.20");
        ui->line_db_port->setText("");
        ui->line_db_name->setText("INKO_ASSIG");
        ui->line_db_user->setText("SYSDBA");
        ui->line_db_pass->setText("masterkey");

    } else if (ui->combo_db_drvr->currentText()=="QPSQL") {
        ui->line_db_host->setText("172.20.250.15");
        ui->line_db_port->setText("5432");
        ui->line_db_name->setText("inko_polises_voronej");
        ui->line_db_user->setText("belg");
        ui->line_db_pass->setText("belg");
    }
}


void accounts_base_wnd::on_rb_hand_clicked(bool checked) {
    ui->spin_hand->setEnabled(ui->rb_hand->isChecked());
}
void accounts_base_wnd::on_rb_1000000_clicked(bool checked) {
    ui->spin_hand->setEnabled(ui->rb_hand->isChecked());
}
void accounts_base_wnd::on_rb_100000_clicked(bool checked) {
    ui->spin_hand->setEnabled(ui->rb_hand->isChecked());
}
void accounts_base_wnd::on_rb_10000_clicked(bool checked) {
    ui->spin_hand->setEnabled(ui->rb_hand->isChecked());
}
void accounts_base_wnd::on_rb_1000_clicked(bool checked) {
    ui->spin_hand->setEnabled(ui->rb_hand->isChecked());
}







void accounts_base_wnd::on_bn_all_tabs_clicked(bool checked) {
    ui->ch_ASSIG_BY_TERR->setChecked(checked);
    ui->ch_BLANKS_VS->setChecked(checked);
    ui->ch_BLANKS_POL->setChecked(checked);
    ui->ch_BLANKS_UEC->setChecked(checked);
    ui->ch_EVENTS->setChecked(checked);
    ui->ch_VIZITS->setChecked(checked);
    ui->ch_POLISES->setChecked(checked);
    ui->ch_PERSONS->setChecked(checked);
    ui->ch_ADDRESSES->setChecked(checked);
    ui->ch_PERSONS_OLD->setChecked(checked);
    ui->ch_PERSONS_DOCS->setChecked(checked);
    ui->ch_PERSONS_ASSIG->setChecked(checked);
}

void accounts_base_wnd::on_bn_all_clear_clicked(bool checked) {
    ui->ch_ASSIG_BY_TERR_all->setEnabled(!checked);
    ui->ch_BLANKS_VS_all->setEnabled(!checked);
    ui->ch_BLANKS_POL_all->setEnabled(!checked);
    ui->ch_BLANKS_UEC_all->setEnabled(!checked);
    ui->ch_EVENTS_all->setEnabled(!checked);
    ui->ch_VIZITS_all->setEnabled(!checked);
    ui->ch_POLISES_all->setEnabled(!checked);
    ui->ch_PERSONS_all->setEnabled(!checked);
    ui->ch_ADDRESSES_all->setEnabled(!checked);
    ui->ch_PERSONS_OLD_all->setEnabled(!checked);
    ui->ch_PERSONS_DOCS_all->setEnabled(!checked);
    ui->ch_PERSONS_ASSIG_all->setEnabled(!checked);
    ui->bn_all_all->setEnabled(!checked);

    if (checked) {
        ui->ch_ASSIG_BY_TERR_all->setChecked(false);
        ui->ch_BLANKS_VS_all->setChecked(false);
        ui->ch_BLANKS_POL_all->setChecked(false);
        ui->ch_BLANKS_UEC_all->setChecked(false);
        ui->ch_EVENTS_all->setChecked(false);
        ui->ch_VIZITS_all->setChecked(false);
        ui->ch_POLISES_all->setChecked(false);
        ui->ch_PERSONS_all->setChecked(false);
        ui->ch_ADDRESSES_all->setChecked(false);
        ui->ch_PERSONS_OLD_all->setChecked(false);
        ui->ch_PERSONS_DOCS_all->setChecked(false);
        ui->ch_PERSONS_ASSIG_all->setChecked(false);
        ui->bn_all_all->setEnabled(false);
    }

    ui->ch_ASSIG_BY_TERR_clear->setEnabled(true);
    ui->ch_BLANKS_VS_clear->setEnabled(true);
    ui->ch_BLANKS_POL_clear->setEnabled(true);
    ui->ch_BLANKS_UEC_clear->setEnabled(true);
    ui->ch_EVENTS_clear->setEnabled(true);
    ui->ch_VIZITS_clear->setEnabled(true);
    ui->ch_POLISES_clear->setEnabled(true);
    ui->ch_PERSONS_clear->setEnabled(true);
    ui->ch_ADDRESSES_clear->setEnabled(true);
    ui->ch_PERSONS_OLD_clear->setEnabled(true);
    ui->ch_PERSONS_DOCS_clear->setEnabled(true);
    ui->ch_PERSONS_ASSIG_clear->setEnabled(true);
    ui->bn_all_clear->setEnabled(true);

    ui->ch_ASSIG_BY_TERR_clear->setChecked(checked);
    ui->ch_BLANKS_VS_clear->setChecked(checked);
    ui->ch_BLANKS_POL_clear->setChecked(checked);
    ui->ch_BLANKS_UEC_clear->setChecked(checked);
    ui->ch_EVENTS_clear->setChecked(checked);
    ui->ch_VIZITS_clear->setChecked(checked);
    ui->ch_POLISES_clear->setChecked(checked);
    ui->ch_PERSONS_clear->setChecked(checked);
    ui->ch_ADDRESSES_clear->setChecked(checked);
    ui->ch_PERSONS_OLD_clear->setChecked(checked);
    ui->ch_PERSONS_DOCS_clear->setChecked(checked);
    ui->ch_PERSONS_ASSIG_clear->setChecked(checked);
}

void accounts_base_wnd::on_bn_all_all_clicked(bool checked) {
    ui->ch_ASSIG_BY_TERR_clear->setEnabled(!checked);
    ui->ch_BLANKS_VS_clear->setEnabled(!checked);
    ui->ch_BLANKS_POL_clear->setEnabled(!checked);
    ui->ch_BLANKS_UEC_clear->setEnabled(!checked);
    ui->ch_EVENTS_clear->setEnabled(!checked);
    ui->ch_VIZITS_clear->setEnabled(!checked);
    ui->ch_POLISES_clear->setEnabled(!checked);
    ui->ch_PERSONS_clear->setEnabled(!checked);
    ui->ch_ADDRESSES_clear->setEnabled(!checked);
    ui->ch_PERSONS_OLD_clear->setEnabled(!checked);
    ui->ch_PERSONS_DOCS_clear->setEnabled(!checked);
    ui->ch_PERSONS_ASSIG_clear->setEnabled(!checked);
    ui->bn_all_clear->setEnabled(!checked);

    if (checked) {
        ui->ch_ASSIG_BY_TERR_clear->setChecked(false);
        ui->ch_BLANKS_VS_clear->setChecked(false);
        ui->ch_BLANKS_POL_clear->setChecked(false);
        ui->ch_BLANKS_UEC_clear->setChecked(false);
        ui->ch_EVENTS_clear->setChecked(false);
        ui->ch_VIZITS_clear->setChecked(false);
        ui->ch_POLISES_clear->setChecked(false);
        ui->ch_PERSONS_clear->setChecked(false);
        ui->ch_ADDRESSES_clear->setChecked(false);
        ui->ch_PERSONS_OLD_clear->setChecked(false);
        ui->ch_PERSONS_DOCS_clear->setChecked(false);
        ui->ch_PERSONS_ASSIG_clear->setChecked(false);
        ui->bn_all_clear->setEnabled(false);
    }
    ui->ch_ASSIG_BY_TERR_all->setEnabled(true);
    ui->ch_BLANKS_VS_all->setEnabled(true);
    ui->ch_BLANKS_POL_all->setEnabled(true);
    ui->ch_BLANKS_UEC_all->setEnabled(true);
    ui->ch_EVENTS_all->setEnabled(true);
    ui->ch_VIZITS_all->setEnabled(true);
    ui->ch_POLISES_all->setEnabled(true);
    ui->ch_PERSONS_all->setEnabled(true);
    ui->ch_ADDRESSES_all->setEnabled(true);
    ui->ch_PERSONS_OLD_all->setEnabled(true);
    ui->ch_PERSONS_DOCS_all->setEnabled(true);
    ui->ch_PERSONS_ASSIG_all->setEnabled(true);
    ui->bn_all_all->setEnabled(true);

    ui->ch_ASSIG_BY_TERR_all->setChecked(checked);
    ui->ch_BLANKS_VS_all->setChecked(checked);
    ui->ch_BLANKS_POL_all->setChecked(checked);
    ui->ch_BLANKS_UEC_all->setChecked(checked);
    ui->ch_EVENTS_all->setChecked(checked);
    ui->ch_VIZITS_all->setChecked(checked);
    ui->ch_POLISES_all->setChecked(checked);
    ui->ch_PERSONS_all->setChecked(checked);
    ui->ch_ADDRESSES_all->setChecked(checked);
    ui->ch_PERSONS_OLD_all->setChecked(checked);
    ui->ch_PERSONS_DOCS_all->setChecked(checked);
    ui->ch_PERSONS_ASSIG_all->setChecked(checked);
}





void accounts_base_wnd::on_ch_ASSIG_BY_TERR_clear_clicked(bool checked) {
    if (checked) {
        ui->ch_ASSIG_BY_TERR_all->setChecked(false);
        ui->ch_ASSIG_BY_TERR_all->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_BLANKS_VS_clear_clicked(bool checked) {
    if (checked) {
        ui->ch_BLANKS_VS_all->setChecked(false);
        ui->ch_BLANKS_VS_all->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_BLANKS_POL_clear_clicked(bool checked) {
    if (checked) {
        ui->ch_BLANKS_POL_all->setChecked(false);
        ui->ch_BLANKS_POL_all->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_BLANKS_UEC_clear_clicked(bool checked) {
    if (checked) {
        ui->ch_BLANKS_UEC_all->setChecked(false);
        ui->ch_BLANKS_UEC_all->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_EVENTS_clear_clicked(bool checked) {
    if (checked) {
        ui->ch_EVENTS_all->setChecked(false);
        ui->ch_EVENTS_all->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_VIZITS_clear_clicked(bool checked) {
    if (checked) {
        ui->ch_VIZITS_all->setChecked(false);
        ui->ch_VIZITS_all->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_POLISES_clear_clicked(bool checked) {
    if (checked) {
        ui->ch_POLISES_all->setChecked(false);
        ui->ch_POLISES_all->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_PERSONS_clear_clicked(bool checked) {
    if (checked) {
        ui->ch_PERSONS_all->setChecked(false);
        ui->ch_PERSONS_all->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_ADDRESSES_clear_clicked(bool checked) {
    if (checked) {
        ui->ch_ADDRESSES_all->setChecked(false);
        ui->ch_ADDRESSES_all->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_PERSONS_OLD_clear_clicked(bool checked) {
    if (checked) {
        ui->ch_PERSONS_OLD_all->setChecked(false);
        ui->ch_PERSONS_OLD_all->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_PERSONS_DOCS_clear_clicked(bool checked) {
    if (checked) {
        ui->ch_PERSONS_DOCS_all->setChecked(false);
        ui->ch_PERSONS_DOCS_all->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_PERSONS_ASSIGS_clear_clicked(bool checked) {
    if (checked) {
        ui->ch_PERSONS_ASSIG_all->setChecked(false);
        ui->ch_PERSONS_ASSIG_all->setEnabled(false);
    }
}





void accounts_base_wnd::on_ch_ASSIG_BY_TERR_all_clicked(bool checked) {
    if (checked) {
        ui->ch_ASSIG_BY_TERR_clear->setChecked(false);
        ui->ch_ASSIG_BY_TERR_clear->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_BLANKS_VS_all_clicked(bool checked) {
    if (checked) {
        ui->ch_BLANKS_VS_clear->setChecked(false);
        ui->ch_BLANKS_VS_clear->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_BLANKS_POL_all_clicked(bool checked) {
    if (checked) {
        ui->ch_BLANKS_POL_clear->setChecked(false);
        ui->ch_BLANKS_POL_clear->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_BLANKS_UEC_all_clicked(bool checked) {
    if (checked) {
        ui->ch_BLANKS_UEC_clear->setChecked(false);
        ui->ch_BLANKS_UEC_clear->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_EVENTS_all_clicked(bool checked) {
    if (checked) {
        ui->ch_EVENTS_clear->setChecked(false);
        ui->ch_EVENTS_clear->setEnabled(false);
    }
}

void accounts_base_wnd::on_ch_VIZITS_all_clicked(bool checked) {
    if (checked) {
        ui->ch_VIZITS_clear->setChecked(false);
        ui->ch_VIZITS_clear->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_POLISES_all_clicked(bool checked) {
    if (checked) {
        ui->ch_POLISES_clear->setChecked(false);
        ui->ch_POLISES_clear->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_PERSONS_all_clicked(bool checked) {
    if (checked) {
        ui->ch_PERSONS_clear->setChecked(false);
        ui->ch_PERSONS_clear->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_ADDRESSES_all_clicked(bool checked) {
    if (checked) {
        ui->ch_ADDRESSES_clear->setChecked(false);
        ui->ch_ADDRESSES_clear->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_PERSONS_OLD_all_clicked(bool checked) {
    if (checked) {
        ui->ch_PERSONS_OLD_clear->setChecked(false);
        ui->ch_PERSONS_OLD_clear->setEnabled(false);
    }
}

void accounts_base_wnd::on_ch_PERSONS_DOCS_all_clicked(bool checked) {
    if (checked) {
        ui->ch_PERSONS_DOCS_clear->setChecked(false);
        ui->ch_PERSONS_DOCS_clear->setEnabled(false);
    }
}
void accounts_base_wnd::on_ch_PERSONS_ASSIGS_all_clicked(bool checked) {
    if (checked) {
        ui->ch_PERSONS_ASSIG_clear->setChecked(false);
        ui->ch_PERSONS_ASSIG_clear->setEnabled(false);
    }
}






void accounts_base_wnd::on_bn_copy_data_clicked() {
    ui->pan_wait->setVisible(true);
    QApplication::processEvents();

    bool res_load = try_copy_data();
    if (!res_load)
        QMessageBox::warning(this, "Действие отменено", "Действие отменено.\n");
    else
        QMessageBox::information(this, "Готово", "Все данные загружены.\n");

    ui->pan_wait->setVisible(false);
    QApplication::processEvents();
}

bool accounts_base_wnd::try_copy_data() {
    ui->te_log->clear();
    ui->bn_close->setEnabled(false);
    ui->push_connect->setEnabled(false);
    ui->bn_copy_data->setEnabled(false);
    QApplication::processEvents();

    date0 = QDate::currentDate();
    time0 = QTime::currentTime();

    // подготовка
    date = QDate::currentDate();
    time = QTime::currentTime();
    ui->lab_time_all->setText("00:00:00");
    ui->lab_time->setText("00:00:00");
    QApplication::processEvents();

    int cnt_step = 1;
    if (ui->rb_21->isChecked())  cnt_step = 21;
    else if (ui->rb_231->isChecked())  cnt_step = 231;
    else if (ui->rb_2341->isChecked())  cnt_step = 2341;
    else if (ui->rb_23451->isChecked())  cnt_step = 23451;
    else if (ui->rb_hand->isChecked())  cnt_step = ui->spin_hand->value();


    QString sql_clear;
    QSqlQuery *query_clear;
    bool res_clear;

    QString sql_del;
    QSqlQuery *query_del;
    bool res_del;

    QString sql_cnt;
    QSqlQuery *query_cnt;
    bool res_cnt;

    QString sql;
    QSqlQuery *query;
    bool res;

    QString sql_add;
    QSqlQuery *query_add;
    bool res_add;

    QString sql_upd;
    QSqlQuery *query_upd;
    bool res_upd;


    /////////////////////////
    // // ASSIG_BY_TERR // //
    /////////////////////////
    if (ui->ch_ASSIG_BY_TERR->isChecked()) {
        ui->lab_table->setText("ASSIG_BY_TERR");
        ui->te_log->append("ASSIG_BY_TERR");
        QApplication::processEvents();

        db_accBase.transaction();
        db.transaction();


        if (ui->ch_ASSIG_BY_TERR_all->isChecked()) {
            // почистим таблицу
            sql_clear = "DELETE FROM ASSIG_BY_TERR ; ";
            query_clear = new QSqlQuery(db_accBase);
            res_clear = query_clear->exec(sql_clear);
            query_clear->next();
            delete query_clear;

            // запрос данных
            sql  =  "select id, ocato, "
                    "       subj, dstr, city, nasp, strt, house, "
                    "       corp, "
                    "       sex, age, from_start, to_end, side, "
                    "       code_mo, code_mp, code_ms, snils_mt, "
                    "       comment "
                    "  from assig_by_terr "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы ASSIG_BY_TERR",
                                     "При попытке прочитать очередную порцию данных из таблицы ASSIG_BY_TERR произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        } else {
            // запрос данных
            sql  =  "select id, ocato, "
                    "       subj, dstr, city, nasp, strt, house, "
                    "       corp, "
                    "       sex, age, from_start, to_end, side, "
                    "       code_mo, code_mp, code_ms, snils_mt, "
                    "       comment "
                    "  from assig_by_terr "
                    " where _update2acc is NULL "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы ASSIG_BY_TERR",
                                     "При попытке прочитать очередную порцию данных из таблицы ASSIG_BY_TERR произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        }

        // заготовим переменные
        int id_assig;
        QString ocato;
        int subj, dstr, city, nasp, strt, house;
        QString corp;
        int sex, age, from_start, to_end, side;
        QString code_mo, code_mp, code_ms, snils_mt;
        QString comment;

        int n = 0;

        while (query->next()) {
            //------------------//
            // прочитаем данные //
            //------------------//
            id_assig   = query->value( 0).toInt();
            ocato      = query->value( 1).toString();
            subj       = query->value( 2).toInt();
            dstr       = query->value( 3).toInt();
            city       = query->value( 4).toInt();
            nasp       = query->value( 5).toInt();
            strt       = query->value( 6).toInt();
            house      = query->value( 7).toInt();
            corp       = query->value( 8).toString();
            sex        = query->value( 9).toInt();
            age        = query->value(10).toInt();
            from_start = query->value(11).toInt();
            to_end     = query->value(12).toInt();
            side       = query->value(13).toInt();
            code_mo    = query->value(14).toString();
            code_mp    = query->value(15).toString();
            code_ms    = query->value(16).toString();
            snils_mt   = query->value(17).toString();
            comment    = query->value(18).toString();

            //----------------//
            // добавим запись //
            //----------------//
            if (!ui->ch_ASSIG_BY_TERR_all->isChecked()) {
                sql_del  = "DELETE FROM ASSIG_BY_TERR "
                           " WHERE id=" + QString::number(id_assig) + " ; ";
                query_del = new QSqlQuery(db_accBase);
                res_del = query_del->exec(sql_del);
                if (!res_del) {
                    db.rollback();
                    db_accBase.rollback();
                    QMessageBox::warning(this, "Ошибка удаления старой записи в таблице ASSIG_BY_TERR",
                                         "При попытке удалить старую запись в таблице ASSIG_BY_TERR произошла неожиданная ошибка.\n\n"
                                         "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                    ui->bn_close->setEnabled(true);
                    ui->push_connect->setEnabled(true);
                    ui->bn_copy_data->setEnabled(true);
                    return false;
                }
                delete query_del;
            }

            sql_add  = "INSERT INTO ASSIG_BY_TERR(id, ocato, "
                       "       subj, dstr, city, nasp, strt, house, "
                       "       corp, "
                       "       sex, age, from_start, to_end, side, "
                       "       code_mo, code_mp, code_ms, snils_mt, "
                       "       comment) "
                       "VALUES ("    + QString::number(id_assig) + " "
                                ",'" + ocato                     + "'"
                                ", " + QString::number(subj)     + " "
                                ", " + QString::number(dstr)     + " "
                                ", " + QString::number(city)     + " "
                                ", " + QString::number(nasp)     + " "
                                ", " + QString::number(strt)     + " "
                                ", " + QString::number(house)    + " "
                                ",'" + corp                      + "'"
                                ", " + QString::number(sex)      + " "
                                ", " + QString::number(age)      + " "
                                ", " + QString::number(from_start) + " "
                                ", " + QString::number(to_end)   + " "
                                ", " + QString::number(side)     + " "
                                ",'" + code_mo                   + "'"
                                ",'" + code_mp                   + "'"
                                ",'" + code_ms                   + "'"
                                ",'" + snils_mt                  + "'"
                                ",'" + comment                   + "'"
                                ") ; ";
            query_add = new QSqlQuery(db_accBase);
            res_add = query_add->exec(sql_add);
            if (!res_add) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка переноса данных таблицы ASSIG_BY_TERR",
                                     "При попытке перенести в базу данных для отдела счетов данные таблицы ASSIG_BY_TERR произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_add->next();

            delete query_add;

            n++;

            if ( n%cnt_step==0 ) {
                //------------------------------//
                // отображение статуса на экран //
                //------------------------------//
                QDate date2 = QDate::currentDate();
                QTime time2 = QTime::currentTime();

                int sec = time0.secsTo(time2) + date0.daysTo(date2)*24*60*60;
                int sut = sec/86400;
                sec = sec - sut*86400;
                int hou = sec/3600;
                sec = sec - hou*3600;
                int min = sec/60;
                sec = sec - min*60;
                ui->lab_time_all->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                           QString("00" + QString::number(hou)).right(2) + ":" +
                                           QString("00" + QString::number(min)).right(2) + ":" +
                                           QString("00" + QString::number(sec)).right(2) );

                sec = time.secsTo(time2) + date.daysTo(date2)*24*60*60;
                sut = sec/86400;
                sec = sec - sut*86400;
                hou = sec/3600;
                sec = sec - hou*3600;
                min = sec/60;
                sec = sec - min*60;
                ui->lab_time->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                       QString("00" + QString::number(hou)).right(2) + ":" +
                                       QString("00" + QString::number(min)).right(2) + ":" +
                                       QString("00" + QString::number(sec)).right(2) );
                date = QDate::currentDate();
                time = QTime::currentTime();

                ui->lab_count->setText(show_num(n));
                if (ui->rb_21->isChecked())  cnt_step = 21;
                else if (ui->rb_231->isChecked())  cnt_step = 231;
                else if (ui->rb_2341->isChecked())  cnt_step = 2341;
                else if (ui->rb_23451->isChecked())  cnt_step = 23451;
                else if (ui->rb_hand->isChecked())  cnt_step = ui->spin_hand->value();
                QApplication::processEvents();
            }
        }
        delete query;
        QApplication::processEvents();

        // добавим связки
        sql_upd = "UPDATE ASSIG_BY_TERR "
                  "   set _update2acc = 2 "
                  " where _update2acc is NULL ";
        query_upd = new QSqlQuery(db);
        res_upd = query_upd->exec(sql_upd);
        if (!res_upd) {
            db.rollback();
            db_accBase.rollback();
            QMessageBox::warning(this, "Ошибка отметки статуса таблицы ASSIG_BY_TERR",
                                 "При попытке пометить данные таблицы ASSIG_BY_TERR как успешно перенесённые произошла неожиданная ошибка.\n\n"
                                 "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_copy_data->setEnabled(true);
            return false;
        }


        db.commit();
        db_accBase.commit();
        ui->te_log->setText(ui->te_log->toPlainText() + " - ok (" + show_num(n) + ")");


        if (ui->ch_ASSIG_BY_TERR_clear->isChecked()) {
            // ------------------------ //
            // Удалим устаревшие записи //
            // ------------------------ //
            db.transaction();
            db_accBase.transaction();

            // Cначала очистим таблицу _IDS_TAB
            sql_del = "DELETE FROM \"_IDS_TAB\" ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка при очистке таблицы _IDS_TAB",
                                     "При попытке очистить таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;
            // Зальём в таблицу _IDS_TAB идентификаторы записей, которые есть в исходной базе...
            sql  =  "select id "
                    "  from assig_by_terr ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                delete query;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения ID из таблицы ASSIG_BY_TERR",
                                     "При попытке прочитать действующие ID из таблицы ASSIG_BY_TERR произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            int cntr = 0;
            while (query->next()) {
                int id = query->value(0).toInt();
                sql_add = "INSERT INTO \"_IDS_TAB\" (ID) "
                          "VALUES (" + QString::number(id) + ") ; ";
                query_add = new QSqlQuery(db_accBase);
                {
                    res_add = query_add->exec(sql_add);
                    if (!res_add) {
                        delete query_add;
                        delete query;
                        db.rollback();
                        db_accBase.commit();
                        db_accBase.rollback();
                        QMessageBox::warning(this, "Ошибка добавления записи в таблицу _IDS_TAB",
                                             "При попытке добавить запись в таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                             "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                        ui->bn_close->setEnabled(true);
                        ui->push_connect->setEnabled(true);
                        ui->bn_copy_data->setEnabled(true);
                        return false;
                    }
                } while (!res_add);
                delete query_add;

                cntr++;
                if (cntr%2341==0) {
                    db_accBase.commit();
                    db_accBase.transaction();
                    ui->lab_count->setText("IDs  " + show_num(cntr));
                    QApplication::processEvents();
                }
            }
            db_accBase.commit();
            db_accBase.transaction();
            ui->lab_count->setText("IDs  " + show_num(cntr));
            QApplication::processEvents();
            delete query;
            // ...посчитаем число ID которых нет в загруженной таблице _IDS_TAB
            sql_cnt = "SELECT count(*)"
                      "  FROM ASSIG_BY_TERR a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_cnt = new QSqlQuery(db_accBase);
            res_cnt = query_cnt->exec(sql_cnt);
            if (!res_cnt) {
                delete query_cnt;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из ASSIG_BY_TERR",
                                     "При попытке удалить устаревшие записи из ASSIG_BY_TERR произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_cnt->next();
            int cnt_to_delete = query_cnt->value(0).toInt();
            delete query_cnt;
            // ...и удалим записи, ID которых нет в загруженной таблице _IDS_TAB
            sql_del = "DELETE FROM ASSIG_BY_TERR a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из ASSIG_BY_TERR",
                                     "При попытке удалить устаревшие записи из ASSIG_BY_TERR произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;

            db.commit();
            db_accBase.commit();
            ui->te_log->setText(ui->te_log->toPlainText() + " - удалено " + show_num(cnt_to_delete) + " устаревших записей");
        }


        ui->ch_ASSIG_BY_TERR->setChecked(false);
        ui->ch_ASSIG_BY_TERR_clear->setChecked(false);
        ui->ch_ASSIG_BY_TERR_all->setChecked(false);
    }


    /////////////////////
    // // BLANKS_VS // //
    /////////////////////
    if (ui->ch_BLANKS_VS->isChecked()) {
        ui->lab_table->setText("BLANKS_VS");
        ui->te_log->append("BLANKS_VS");
        QApplication::processEvents();

        db_accBase.transaction();
        db.transaction();

        if (ui->ch_BLANKS_VS_all->isChecked()) {
            // почистим таблицу
            sql_clear = "DELETE FROM BLANKS_VS ; ";
            query_clear = new QSqlQuery(db_accBase);
            res_clear = query_clear->exec(sql_clear);
            query_clear->next();
            delete query_clear;

            // запрос данных
            sql  =  "select vs_num, id_person, id_polis, id_operator, id_point, status, "
                    "       date_add, date_spent "
                    "  from BLANKS_VS vs_num "
                    " order by vs_num ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы BLANKS_VS",
                                     "При попытке прочитать очередную порцию данных из таблицы BLANKS_VS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        } else {
            // запрос данных
            sql  =  "select vs_num, id_person, id_polis, id_operator, id_point, status, "
                    "       date_add, date_spent "
                    "  from BLANKS_VS vs_num "
                    " where _update2acc is NULL "
                    " order by vs_num ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы BLANKS_VS",
                                     "При попытке прочитать очередную порцию данных из таблицы BLANKS_VS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        }

        // заготовим переменные
        int vs_num, id_person, id_polis, id_operator, id_point, status;
        QDate date_add, date_spent;

        int n = 0;

        while (query->next()) {
            //------------------//
            // прочитаем данные //
            //------------------//
            vs_num      = query->value( 0).toInt();
            id_person   = query->value( 1).toInt();
            id_polis    = query->value( 2).toInt();
            id_operator = query->value( 3).toInt();
            id_point    = query->value( 4).toInt();
            status      = query->value( 5).toInt();
            date_add    = query->value( 6).toDate();
            date_spent  = query->value( 7).toDate();

            //----------------//
            // добавим запись //
            //----------------//
            if (!ui->ch_BLANKS_VS_all->isChecked()) {
                sql_del  = "DELETE FROM BLANKS_VS "
                           " WHERE vs_num=" + QString::number(vs_num) + " ; ";
                query_del = new QSqlQuery(db_accBase);
                res_del = query_del->exec(sql_del);
                if (!res_del) {
                    db.rollback();
                    db_accBase.rollback();
                    QMessageBox::warning(this, "Ошибка удаления старой записи в таблице BLANKS_VS",
                                         "При попытке удалить старую запись в таблице BLANKS_VS произошла неожиданная ошибка.\n\n"
                                         "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                    ui->bn_close->setEnabled(true);
                    ui->push_connect->setEnabled(true);
                    ui->bn_copy_data->setEnabled(true);
                    return false;
                }
                delete query_del;
            }

            sql_add  = "INSERT INTO BLANKS_VS "
                       "       ( vs_num, id_person, id_polis, id_operator, id_point, status, "
                       "         date_add, date_spent) "
                       "VALUES ("    + QString::number(vs_num)         + " "
                                ", " + QString::number(id_person)      + " "
                                ", " + QString::number(id_polis)       + " "
                                ", " + QString::number(id_operator)    + " "
                                ", " + QString::number(id_point)       + " "
                                ", " + QString::number(status)         + " "
                                ", " + ( date_add.isNull()   ? "NULL" : ("'" + date_add.toString("yyyy-MM-dd")   + "'") ) + " "
                                ", " + ( date_spent.isNull() ? "NULL" : ("'" + date_spent.toString("yyyy-MM-dd") + "'") ) + " "
                                ") ; ";
            query_add = new QSqlQuery(db_accBase);
            res_add = query_add->exec(sql_add);
            if (!res_add) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка переноса данных таблицы BLANKS_VS",
                                     "При попытке перенести в базу данных для отдела счетов данные таблицы BLANKS_VS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_add->next();

            delete query_add;

            n++;

            if ( n%cnt_step==0 ) {
                //------------------------------//
                // отображение статуса на экран //
                //------------------------------//
                QDate date2 = QDate::currentDate();
                QTime time2 = QTime::currentTime();

                int sec = time0.secsTo(time2) + date0.daysTo(date2)*24*60*60;
                int sut = sec/86400;
                sec = sec - sut*86400;
                int hou = sec/3600;
                sec = sec - hou*3600;
                int min = sec/60;
                sec = sec - min*60;
                ui->lab_time_all->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                           QString("00" + QString::number(hou)).right(2) + ":" +
                                           QString("00" + QString::number(min)).right(2) + ":" +
                                           QString("00" + QString::number(sec)).right(2) );

                sec = time.secsTo(time2) + date.daysTo(date2)*24*60*60;
                sut = sec/86400;
                sec = sec - sut*86400;
                hou = sec/3600;
                sec = sec - hou*3600;
                min = sec/60;
                sec = sec - min*60;
                ui->lab_time->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                       QString("00" + QString::number(hou)).right(2) + ":" +
                                       QString("00" + QString::number(min)).right(2) + ":" +
                                       QString("00" + QString::number(sec)).right(2) );
                date = QDate::currentDate();
                time = QTime::currentTime();

                ui->lab_count->setText(show_num(n));
                if (ui->rb_21->isChecked())  cnt_step = 21;
                else if (ui->rb_231->isChecked())  cnt_step = 231;
                else if (ui->rb_2341->isChecked())  cnt_step = 2341;
                else if (ui->rb_23451->isChecked())  cnt_step = 23451;
                else if (ui->rb_hand->isChecked())  cnt_step = ui->spin_hand->value();
                QApplication::processEvents();


                // переоткроем транзакию для сохранения обработанных данных
                db_accBase.commit();
                db.commit();
                db_accBase.transaction();
                db.transaction();
            }
        }
        delete query;
        QApplication::processEvents();

        // добавим связки
        sql_upd = "UPDATE BLANKS_VS "
                  "   set _update2acc = 2 "
                  " where _update2acc is NULL ";
        query_upd = new QSqlQuery(db);
        res_upd = query_upd->exec(sql_upd);
        if (!res_upd) {
            db.rollback();
            db_accBase.rollback();
            QMessageBox::warning(this, "Ошибка отметки статуса таблицы BLANKS_VS",
                                 "При попытке пометить данные таблицы BLANKS_VS как успешно перенесённые произошла неожиданная ошибка.\n\n"
                                 "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_copy_data->setEnabled(true);
            return false;
        } else  {
            db.commit();
        }

        db_accBase.commit();
        ui->te_log->setText(ui->te_log->toPlainText() + " - ok (" + show_num(n) + ")");


        if (ui->ch_BLANKS_VS_clear->isChecked()) {
            // ------------------------ //
            // Удалим устаревшие записи //
            // ------------------------ //
            db.transaction();
            db_accBase.transaction();

            // Cначала очистим таблицу _IDS_TAB
            sql_del = "DELETE FROM \"_IDS_TAB\" ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка при очистке таблицы _IDS_TAB",
                                     "При попытке очистить таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;
            // Зальём в таблицу _IDS_TAB идентификаторы записей, которые есть в исходной базе...
            sql  =  "select vs_num "
                    "  from BLANKS_VS ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                delete query;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения ID из таблицы BLANKS_VS",
                                     "При попытке прочитать действующие ID из таблицы BLANKS_VS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            int cntr = 0;
            while (query->next()) {
                int id = query->value(0).toInt();
                sql_add = "INSERT INTO \"_IDS_TAB\" (ID) "
                          "VALUES (" + QString::number(id) + ") ; ";
                query_add = new QSqlQuery(db_accBase);
                {
                    res_add = query_add->exec(sql_add);
                    if (!res_add) {
                        delete query_add;
                        delete query;
                        db.rollback();
                        db_accBase.commit();
                        db_accBase.rollback();
                        QMessageBox::warning(this, "Ошибка добавления записи в таблицу _IDS_TAB",
                                             "При попытке добавить запись в таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                             "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                        ui->bn_close->setEnabled(true);
                        ui->push_connect->setEnabled(true);
                        ui->bn_copy_data->setEnabled(true);
                        return false;
                    }
                } while (!res_add);
                delete query_add;

                cntr++;
                if (cntr%2341==0) {
                    db_accBase.commit();
                    db_accBase.transaction();
                    ui->lab_count->setText("IDs  " + show_num(cntr));
                    QApplication::processEvents();
                }
            }
            db_accBase.commit();
            db_accBase.transaction();
            ui->lab_count->setText("IDs  " + show_num(cntr));
            QApplication::processEvents();
            delete query;
            // ...посчитаем число ID которых нет в загруженной таблице _IDS_TAB
            sql_cnt = "SELECT count(*)"
                      "  FROM BLANKS_VS a "
                      " WHERE a.vs_num NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_cnt = new QSqlQuery(db_accBase);
            res_cnt = query_cnt->exec(sql_cnt);
            if (!res_cnt) {
                delete query_cnt;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из BLANKS_VS",
                                     "При попытке удалить устаревшие записи из BLANKS_VS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_cnt->next();
            int cnt_to_delete = query_cnt->value(0).toInt();
            delete query_cnt;
            // ...и удалим записи, ID которых нет в загруженной таблице _IDS_TAB
            sql_del = "DELETE FROM BLANKS_VS a "
                      " WHERE a.vs_num NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из BLANKS_VS",
                                     "При попытке удалить устаревшие записи из BLANKS_VS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;

            db.commit();
            db_accBase.commit();
            ui->te_log->setText(ui->te_log->toPlainText() + " - удалено " + show_num(cnt_to_delete) + " устаревших записей");
        }


        ui->ch_BLANKS_VS->setChecked(false);
        ui->ch_BLANKS_VS_clear->setChecked(false);
        ui->ch_BLANKS_VS_all->setChecked(false);
    }


    //////////////////////
    // // BLANKS_POL // //
    //////////////////////
    if (ui->ch_BLANKS_POL->isChecked()) {
        ui->lab_table->setText("BLANKS_POL");
        ui->te_log->append("BLANKS_POL");
        QApplication::processEvents();

        db_accBase.transaction();
        db.transaction();

        if (ui->ch_BLANKS_POL_all->isChecked()) {
            // почистим таблицу
            sql_clear = "DELETE FROM BLANKS_POL ; ";
            query_clear = new QSqlQuery(db_accBase);
            res_clear = query_clear->exec(sql_clear);
            query_clear->next();
            delete query_clear;

            // запрос данных
            sql  =  "select id, id_person, id_polis, id_operator, id_point, status, f_polis, "
                    "       pol_ser, pol_num, enp, "
                    "       date_add, date_spent, date_scan_enp, date_exp, date_sms "
                    "  from BLANKS_POL "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы BLANKS_POL",
                                     "При попытке прочитать очередную порцию данных из таблицы BLANKS_POL произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        } else {
            // запрос данных
            sql  =  "select id, id_person, id_polis, id_operator, id_point, status, f_polis, "
                    "       pol_ser, pol_num, enp, "
                    "       date_add, date_spent, date_scan_enp, date_exp, date_sms "
                    "  from BLANKS_POL "
                    " where _update2acc is NULL "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы BLANKS_POL",
                                     "При попытке прочитать очередную порцию данных из таблицы BLANKS_POL произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        }
        // заготовим переменные
        int id_blank, id_person, id_polis, id_operator, id_point, status, f_polis;
        QString pol_ser, pol_num, enp;
        QDate date_add, date_spent, date_scan_enp, date_exp, date_sms;

        int n = 0;

        while (query->next()) {
            //------------------//
            // прочитаем данные //
            //------------------//
            id_blank      = query->value( 0).toInt();
            id_person     = query->value( 1).toInt();
            id_polis      = query->value( 2).toInt();
            id_operator   = query->value( 3).toInt();
            id_point      = query->value( 4).toInt();
            status        = query->value( 5).toInt();
            f_polis       = query->value( 6).toInt();
            pol_ser       = query->value( 7).toString();
            pol_num       = query->value( 8).toString();
            enp           = query->value( 9).toString();
            date_add      = query->value(10).toDate();
            date_spent    = query->value(11).toDate();
            date_scan_enp = query->value(12).toDate();
            date_exp      = query->value(13).toDate();
            date_sms      = query->value(14).toDate();

            //----------------//
            // добавим запись //
            //----------------//
            if (!ui->ch_BLANKS_POL_all->isChecked()) {
                sql_del  = "DELETE FROM BLANKS_POL "
                           " WHERE id=" + QString::number(id_blank) + " ; ";
                query_del = new QSqlQuery(db_accBase);
                res_del = query_del->exec(sql_del);
                if (!res_del) {
                    db.rollback();
                    db_accBase.rollback();
                    QMessageBox::warning(this, "Ошибка удаления старой записи в таблице BLANKS_POL",
                                         "При попытке удалить старую запись в таблице BLANKS_POL произошла неожиданная ошибка.\n\n"
                                         "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                    ui->bn_close->setEnabled(true);
                    ui->push_connect->setEnabled(true);
                    ui->bn_copy_data->setEnabled(true);
                    return false;
                }
                delete query_del;
            }

            sql_add  = "INSERT INTO BLANKS_POL "
                       "       ( id, id_person, id_polis, id_operator, id_point, status, f_polis, "
                       "         pol_ser, pol_num, enp, "
                       "         date_add, date_spent, date_scan_enp, date_exp, date_sms) "
                       "VALUES ("    + QString::number(id_blank)    + " "
                                ", " + QString::number(id_person)   + " "
                                ", " + QString::number(id_polis)    + " "
                                ", " + QString::number(id_operator) + " "
                                ", " + QString::number(id_point)    + " "
                                ", " + QString::number(status)      + " "
                                ", " + QString::number(f_polis)     + " "
                                ",'" + pol_ser                      + "'"
                                ",'" + pol_num                      + "'"
                                ",'" + enp                          + "'"
                                ", " + ( date_add.isNull()      ? "NULL" : ("'" + date_add.toString("yyyy-MM-dd")      + "'") ) + " "
                                ", " + ( date_spent.isNull()    ? "NULL" : ("'" + date_spent.toString("yyyy-MM-dd")    + "'") ) + " "
                                ", " + ( date_scan_enp.isNull() ? "NULL" : ("'" + date_scan_enp.toString("yyyy-MM-dd") + "'") ) + " "
                                ", " + ( date_exp.isNull()      ? "NULL" : ("'" + date_exp.toString("yyyy-MM-dd")      + "'") ) + " "
                                ", " + ( date_sms.isNull()      ? "NULL" : ("'" + date_sms.toString("yyyy-MM-dd")      + "'") ) + " "
                                ") ; ";
            query_add = new QSqlQuery(db_accBase);
            res_add = query_add->exec(sql_add);
            if (!res_add) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка переноса данных таблицы BLANKS_POL",
                                     "При попытке перенести в базу данных для отдела счетов данные таблицы BLANKS_POL произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_add->next();

            delete query_add;

            n++;

            if ( n%cnt_step==0 ) {
                //------------------------------//
                // отображение статуса на экран //
                //------------------------------//
                QDate date2 = QDate::currentDate();
                QTime time2 = QTime::currentTime();

                int sec = time0.secsTo(time2) + date0.daysTo(date2)*24*60*60;
                int sut = sec/86400;
                sec = sec - sut*86400;
                int hou = sec/3600;
                sec = sec - hou*3600;
                int min = sec/60;
                sec = sec - min*60;
                ui->lab_time_all->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                           QString("00" + QString::number(hou)).right(2) + ":" +
                                           QString("00" + QString::number(min)).right(2) + ":" +
                                           QString("00" + QString::number(sec)).right(2) );

                sec = time.secsTo(time2) + date.daysTo(date2)*24*60*60;
                sut = sec/86400;
                sec = sec - sut*86400;
                hou = sec/3600;
                sec = sec - hou*3600;
                min = sec/60;
                sec = sec - min*60;
                ui->lab_time->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                       QString("00" + QString::number(hou)).right(2) + ":" +
                                       QString("00" + QString::number(min)).right(2) + ":" +
                                       QString("00" + QString::number(sec)).right(2) );
                date = QDate::currentDate();
                time = QTime::currentTime();

                ui->lab_count->setText(show_num(n));
                if (ui->rb_21->isChecked())  cnt_step = 21;
                else if (ui->rb_231->isChecked())  cnt_step = 231;
                else if (ui->rb_2341->isChecked())  cnt_step = 2341;
                else if (ui->rb_23451->isChecked())  cnt_step = 23451;
                else if (ui->rb_hand->isChecked())  cnt_step = ui->spin_hand->value();
                QApplication::processEvents();


                // переоткроем транзакию для сохранения обработанных данных
                db_accBase.commit();
                db.commit();
                db_accBase.transaction();
                db.transaction();
            }
        }
        delete query;
        QApplication::processEvents();

        // добавим связки
        sql_upd = "UPDATE BLANKS_POL "
                  "   set _update2acc = 2 "
                  " where _update2acc is NULL ";
        query_upd = new QSqlQuery(db);
        res_upd = query_upd->exec(sql_upd);
        if (!res_upd) {
            db.rollback();
            db_accBase.rollback();
            QMessageBox::warning(this, "Ошибка отметки статуса таблицы BLANKS_POL",
                                 "При попытке пометить данные таблицы BLANKS_POL как успешно перенесённые произошла неожиданная ошибка.\n\n"
                                 "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_copy_data->setEnabled(true);
            return false;
        } else  {
            db.commit();
        }

        db_accBase.commit();
        ui->te_log->setText(ui->te_log->toPlainText() + " - ok (" + show_num(n) + ")");


        if (ui->ch_BLANKS_POL_clear->isChecked()) {
            // ------------------------ //
            // Удалим устаревшие записи //
            // ------------------------ //
            db.transaction();
            db_accBase.transaction();

            // Cначала очистим таблицу _IDS_TAB
            sql_del = "DELETE FROM \"_IDS_TAB\" ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка при очистке таблицы _IDS_TAB",
                                     "При попытке очистить таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;
            // Зальём в таблицу _IDS_TAB идентификаторы записей, которые есть в исходной базе...
            sql  =  "select id "
                    "  from BLANKS_POL ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                delete query;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения ID из таблицы BLANKS_POL",
                                     "При попытке прочитать действующие ID из таблицы BLANKS_POL произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            int cntr = 0;
            while (query->next()) {
                int id = query->value(0).toInt();
                sql_add = "INSERT INTO \"_IDS_TAB\" (ID) "
                          "VALUES (" + QString::number(id) + ") ; ";
                query_add = new QSqlQuery(db_accBase);
                {
                    res_add = query_add->exec(sql_add);
                    if (!res_add) {
                        delete query_add;
                        delete query;
                        db.rollback();
                        db_accBase.commit();
                        db_accBase.rollback();
                        QMessageBox::warning(this, "Ошибка добавления записи в таблицу _IDS_TAB",
                                             "При попытке добавить запись в таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                             "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                        ui->bn_close->setEnabled(true);
                        ui->push_connect->setEnabled(true);
                        ui->bn_copy_data->setEnabled(true);
                        return false;
                    }
                } while (!res_add);
                delete query_add;

                cntr++;
                if (cntr%2341==0) {
                    db_accBase.commit();
                    db_accBase.transaction();
                    ui->lab_count->setText("IDs  " + show_num(cntr));
                    QApplication::processEvents();
                }
            }
            db_accBase.commit();
            db_accBase.transaction();
            ui->lab_count->setText("IDs  " + show_num(cntr));
            QApplication::processEvents();
            delete query;
            // ...посчитаем число ID которых нет в загруженной таблице _IDS_TAB
            sql_cnt = "SELECT count(*)"
                      "  FROM BLANKS_POL a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_cnt = new QSqlQuery(db_accBase);
            res_cnt = query_cnt->exec(sql_cnt);
            if (!res_cnt) {
                delete query_cnt;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из BLANKS_POL",
                                     "При попытке удалить устаревшие записи из BLANKS_POL произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_cnt->next();
            int cnt_to_delete = query_cnt->value(0).toInt();
            delete query_cnt;
            // ...и удалим записи, ID которых нет в загруженной таблице _IDS_TAB
            sql_del = "DELETE FROM BLANKS_POL a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из BLANKS_POL",
                                     "При попытке удалить устаревшие записи из BLANKS_POL произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;

            db.commit();
            db_accBase.commit();
            ui->te_log->setText(ui->te_log->toPlainText() + " - удалено " + show_num(cnt_to_delete) + " устаревших записей");
        }


        ui->ch_BLANKS_POL->setChecked(false);
        ui->ch_BLANKS_POL_clear->setChecked(false);
        ui->ch_BLANKS_POL_all->setChecked(false);
    }


    /////////////////////
    // // BLANKS_UEC // //
    /////////////////////
    if (ui->ch_BLANKS_UEC->isChecked()) {
        ui->lab_table->setText("BLANKS_UEC");
        ui->te_log->append("BLANKS_UEC");
        QApplication::processEvents();

        db_accBase.transaction();
        db.transaction();

        if (ui->ch_BLANKS_UEC_all->isChecked()) {
            // почистим таблицу
            sql_clear = "DELETE FROM BLANKS_UEC ; ";
            query_clear = new QSqlQuery(db_accBase);
            res_clear = query_clear->exec(sql_clear);
            query_clear->next();
            delete query_clear;

            // запрос данных
            sql  =  "select uec_num, id_person, id_polis, id_operator, status, "
                    "       date_add "
                    "  from BLANKS_UEC "
                    " order by uec_num ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы BLANKS_UEC",
                                     "При попытке прочитать очередную порцию данных из таблицы BLANKS_UEC произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        } else {
            // запрос данных
            sql  =  "select uec_num, id_person, id_polis, id_operator, status, "
                    "       date_add "
                    "  from BLANKS_UEC "
                    " where _update2acc is NULL "
                    " order by uec_num ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы BLANKS_UEC",
                                     "При попытке прочитать очередную порцию данных из таблицы BLANKS_UEC произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        }

        // заготовим переменные
        QString uec_num;
        int id_person, id_polis, id_operator, status;
        QDate date_add;

        int n = 0;

        while (query->next()) {
            //------------------//
            // прочитаем данные //
            //------------------//
            uec_num     = query->value( 0).toString();
            id_person   = query->value( 1).toInt();
            id_polis    = query->value( 2).toInt();
            id_operator = query->value( 3).toInt();
            status      = query->value( 4).toInt();
            date_add    = query->value( 5).toDate();

            //----------------//
            // добавим запись //
            //----------------//
            if (!ui->ch_BLANKS_UEC_all->isChecked()) {
                sql_del  = "DELETE FROM BLANKS_UEC "
                           " WHERE uec_num='" + uec_num + "' ; ";
                query_del = new QSqlQuery(db_accBase);
                res_del = query_del->exec(sql_del);
                if (!res_del) {
                    db.rollback();
                    db_accBase.rollback();
                    QMessageBox::warning(this, "Ошибка удаления старой записи в таблице BLANKS_UEC",
                                         "При попытке удалить старую запись в таблице BLANKS_UEC произошла неожиданная ошибка.\n\n"
                                         "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                    ui->bn_close->setEnabled(true);
                    ui->push_connect->setEnabled(true);
                    ui->bn_copy_data->setEnabled(true);
                    return false;
                }
                delete query_del;
            }

            sql_add  = "INSERT INTO BLANKS_UEC "
                       "       ( uec_num, id_person, id_polis, id_operator, status, "
                       "         date_add ) "
                       "VALUES ('"   + uec_num                      + "'"
                                ", " + QString::number(id_person)   + " "
                                ", " + QString::number(id_polis)    + " "
                                ", " + QString::number(id_operator) + " "
                                ", " + QString::number(status)      + " "
                                ", " + ( date_add.isNull()   ? "NULL" : ("'" + date_add.toString("yyyy-MM-dd")   + "'") ) + " "
                                ") ; ";
            query_add = new QSqlQuery(db_accBase);
            res_add = query_add->exec(sql_add);
            if (!res_add) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка переноса данных таблицы BLANKS_UEC",
                                     "При попытке перенести в базу данных для отдела счетов данные таблицы BLANKS_UEC произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_add->next();

            delete query_add;

            n++;

            if ( n%cnt_step==0 ) {
                //------------------------------//
                // отображение статуса на экран //
                //------------------------------//
                QDate date2 = QDate::currentDate();
                QTime time2 = QTime::currentTime();

                int sec = time0.secsTo(time2) + date0.daysTo(date2)*24*60*60;
                int sut = sec/86400;
                sec = sec - sut*86400;
                int hou = sec/3600;
                sec = sec - hou*3600;
                int min = sec/60;
                sec = sec - min*60;
                ui->lab_time_all->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                           QString("00" + QString::number(hou)).right(2) + ":" +
                                           QString("00" + QString::number(min)).right(2) + ":" +
                                           QString("00" + QString::number(sec)).right(2) );

                sec = time.secsTo(time2) + date.daysTo(date2)*24*60*60;
                sut = sec/86400;
                sec = sec - sut*86400;
                hou = sec/3600;
                sec = sec - hou*3600;
                min = sec/60;
                sec = sec - min*60;
                ui->lab_time->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                       QString("00" + QString::number(hou)).right(2) + ":" +
                                       QString("00" + QString::number(min)).right(2) + ":" +
                                       QString("00" + QString::number(sec)).right(2) );
                date = QDate::currentDate();
                time = QTime::currentTime();

                ui->lab_count->setText(show_num(n));
                if (ui->rb_21->isChecked())  cnt_step = 21;
                else if (ui->rb_231->isChecked())  cnt_step = 231;
                else if (ui->rb_2341->isChecked())  cnt_step = 2341;
                else if (ui->rb_23451->isChecked())  cnt_step = 23451;
                else if (ui->rb_hand->isChecked())  cnt_step = ui->spin_hand->value();
                QApplication::processEvents();


                // переоткроем транзакию для сохранения обработанных данных
                db_accBase.commit();
                db.commit();
                db_accBase.transaction();
                db.transaction();
            }
        }
        delete query;
        QApplication::processEvents();

        // добавим связки
        sql_upd = "UPDATE BLANKS_UEC "
                  "   set _update2acc = 2 "
                  " where _update2acc is NULL ";
        query_upd = new QSqlQuery(db);
        res_upd = query_upd->exec(sql_upd);
        if (!res_upd) {
            db.rollback();
            db_accBase.rollback();
            QMessageBox::warning(this, "Ошибка отметки статуса таблицы BLANKS_UEC",
                                 "При попытке пометить данные таблицы BLANKS_UEC как успешно перенесённые произошла неожиданная ошибка.\n\n"
                                 "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_copy_data->setEnabled(true);
            return false;
        } else  {
            db.commit();
        }

        db_accBase.commit();
        ui->te_log->setText(ui->te_log->toPlainText() + " - ok (" + show_num(n) + ")");


        if (ui->ch_BLANKS_UEC_clear->isChecked()) {
            // ------------------------ //
            // Удалим устаревшие записи //
            // ------------------------ //
            db.transaction();
            db_accBase.transaction();

            // Cначала очистим таблицу _UECS_TAB
            sql_del = "DELETE FROM \"_UECS_TAB\" ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка при очистке таблицы _UECS_TAB",
                                     "При попытке очистить таблицу _UECS_TAB произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;
            // Зальём в таблицу _UECS_TAB идентификаторы записей, которые есть в исходной базе...
            sql  =  "select UEC_NUM "
                    "  from BLANKS_UEC ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                delete query;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения ID из таблицы BLANKS_UEC",
                                     "При попытке прочитать действующие ID из таблицы BLANKS_UEC произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            int cntr = 0;
            while (query->next()) {
                QString uec_num = query->value(0).toString();
                sql_add = "INSERT INTO \"_UECS_TAB\" (UEC_NUM) "
                          "VALUES ('" + uec_num + "') ; ";
                query_add = new QSqlQuery(db_accBase);
                {
                    res_add = query_add->exec(sql_add);
                    if (!res_add) {
                        delete query_add;
                        delete query;
                        db.rollback();
                        db_accBase.commit();
                        db_accBase.rollback();
                        QMessageBox::warning(this, "Ошибка добавления записи в таблицу _UECS_TAB",
                                             "При попытке добавить запись в таблицу _UECS_TAB произошла неожиданная ошибка.\n\n"
                                             "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                        ui->bn_close->setEnabled(true);
                        ui->push_connect->setEnabled(true);
                        ui->bn_copy_data->setEnabled(true);
                        return false;
                    }
                } while (!res_add);
                delete query_add;

                cntr++;
                if (cntr%2341==0) {
                    db_accBase.commit();
                    db_accBase.transaction();
                    ui->lab_count->setText("uec_nums  " + show_num(cntr));
                    QApplication::processEvents();
                }
            }
            db_accBase.commit();
            db_accBase.transaction();
            ui->lab_count->setText("IDs  " + show_num(cntr));
            QApplication::processEvents();
            delete query;
            // ...посчитаем число ID которых нет в загруженной таблице _UECS_TAB
            sql_cnt = "SELECT count(*)"
                      "  FROM BLANKS_UEC a "
                      " WHERE a.UEC_NUM NOT IN(SELECT i.uec_num FROM \"_UECS_TAB\" i) ; ";
            query_cnt = new QSqlQuery(db_accBase);
            res_cnt = query_cnt->exec(sql_cnt);
            if (!res_cnt) {
                delete query_cnt;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из BLANKS_UEC",
                                     "При попытке удалить устаревшие записи из BLANKS_UEC произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_cnt->next();
            int cnt_to_delete = query_cnt->value(0).toInt();
            delete query_cnt;
            // ...и удалим записи, ID которых нет в загруженной таблице _UECS_TAB
            sql_del = "DELETE FROM BLANKS_UEC a "
                      " WHERE a.uec_num NOT IN(SELECT i.uec_num FROM \"_UECS_TAB\" i) ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из BLANKS_UEC",
                                     "При попытке удалить устаревшие записи из BLANKS_UEC произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;

            db.commit();
            db_accBase.commit();
            ui->te_log->setText(ui->te_log->toPlainText() + " - удалено " + show_num(cnt_to_delete) + " устаревших записей");
        }


        ui->ch_BLANKS_UEC->setChecked(false);
        ui->ch_BLANKS_UEC_clear->setChecked(false);
        ui->ch_BLANKS_UEC_all->setChecked(false);
    }


    //////////////////
    // // EVENTS // //
    //////////////////
    if (ui->ch_EVENTS->isChecked()) {
        ui->lab_table->setText("EVENTS");
        ui->te_log->append("EVENTS");
        QApplication::processEvents();

        db_accBase.transaction();
        db.transaction();

        if (ui->ch_EVENTS_all->isChecked()) {
            // почистим таблицу
            sql_clear = "DELETE FROM EVENTS ; ";
            query_clear = new QSqlQuery(db_accBase);
            res_clear = query_clear->exec(sql_clear);
            query_clear->next();
            delete query_clear;

            // запрос данных
            sql  =  "select id, id_point, id_operator, id_person, id_polis, id_vizit, id_udl, id_drp, status, "
                    "       event_code, comment, "
                    "       event_dt, "
                    "       event_time, "
                    "       event_send "
                    "  from EVENTS "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы EVENTS",
                                     "При попытке прочитать очередную порцию данных из таблицы EVENTS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        } else {
            // запрос данных
            sql  =  "select id, id_point, id_operator, id_person, id_polis, id_vizit, id_udl, id_drp, status, "
                    "       event_code, comment, "
                    "       event_dt, "
                    "       event_time, "
                    "       event_send "
                    "  from EVENTS "
                    " where _update2acc is NULL "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы EVENTS",
                                     "При попытке прочитать очередную порцию данных из таблицы EVENTS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        }

        // заготовим переменные
        int id_event, id_point, id_operator, id_person, id_polis, id_vizit, id_udl, id_drp, status;
        QString event_code, comment;
        QDateTime event_dt;
        QTime event_time;
        QDate event_send;
        QDateTime dt_ins, dt_upd;

        int n = 0;

        while (query->next()) {
            //------------------//
            // прочитаем данные //
            //------------------//
            id_event    = query->value( 0).toInt();
            id_point    = query->value( 1).toInt();
            id_operator = query->value( 2).toInt();
            id_person   = query->value( 3).toInt();
            id_polis    = query->value( 4).toInt();
            id_vizit    = query->value( 5).toInt();
            id_udl      = query->value( 6).toInt();
            id_drp      = query->value( 7).toInt();
            status      = query->value( 8).toInt();
            event_code  = query->value( 9).toString();
            comment     = query->value(10).toString();
            event_dt    = query->value(11).toDateTime();
            event_time  = query->value(12).toTime();
            event_send  = query->value(13).toDate();
            dt_ins      = query->value(14).toDateTime();
            dt_upd      = query->value(15).toDateTime();

            //----------------//
            // добавим запись //
            //----------------//
            if (!ui->ch_EVENTS_all->isChecked()) {
                sql_del  = "DELETE FROM EVENTS "
                           " WHERE id=" + QString::number(id_event) + " ; ";
                query_del = new QSqlQuery(db_accBase);
                res_del = query_del->exec(sql_del);
                if (!res_del) {
                    db.rollback();
                    db_accBase.rollback();
                    QMessageBox::warning(this, "Ошибка удаления старой записи в таблице EVENTS",
                                         "При попытке удалить старую запись в таблице EVENTS произошла неожиданная ошибка.\n\n"
                                         "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                    ui->bn_close->setEnabled(true);
                    ui->push_connect->setEnabled(true);
                    ui->bn_copy_data->setEnabled(true);
                    return false;
                }
                delete query_del;
            }

            sql_add  = "INSERT INTO EVENTS "
                       "       ( id, id_point, id_operator, id_person, id_polis, id_vizit, id_udl, id_drp, status, "
                       "         event_code, comment, "
                       "         event_dt, event_time, event_send, dt_ins, dt_upd ) "
                       "VALUES ("    + QString::number(id_event)    + " "
                                ", " + QString::number(id_point)    + " "
                                ", " + QString::number(id_operator) + " "
                                ", " + QString::number(id_person)   + " "
                                ", " + QString::number(id_polis)    + " "
                                ", " + QString::number(id_vizit)    + " "
                                ", " + QString::number(id_udl)      + " "
                                ", " + QString::number(id_drp)      + " "
                                ", " + QString::number(status)      + " "
                                ",'" + event_code                   + "'"
                                ",'" + comment                      + "'"
                                ", " + ( event_dt.isNull()    ? "NULL" : ("'" + event_dt.toString("yyyy-MM-dd hh:mm:ss") + "'") ) + " "
                                ", " + ( event_time.isNull()  ? "NULL" : ("'" + event_time.toString("hh:mm:ss")          + "'") ) + " "
                                ", " + ( event_send.isNull()  ? "NULL" : ("'" + event_send.toString("yyyy-MM-dd")        + "'") ) + " "
                                ", " + ( dt_ins.isNull() ? "NULL" : ("'" + dt_ins.toString("yyyy-MM-dd hh:mm:ss")  + "'") ) + " "
                                ", " + ( dt_upd.isNull() ? "NULL" : ("'" + dt_upd.toString("yyyy-MM-dd hh:mm:ss")  + "'") ) + " "
                                ") ; ";
            query_add = new QSqlQuery(db_accBase);
            res_add = query_add->exec(sql_add);
            if (!res_add) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка переноса данных таблицы EVENTS",
                                     "При попытке перенести в базу данных для отдела счетов данные таблицы EVENTS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_add->next();

            delete query_add;

            n++;

            if ( n%cnt_step==0 ) {
                //------------------------------//
                // отображение статуса на экран //
                //------------------------------//
                QDate date2 = QDate::currentDate();
                QTime time2 = QTime::currentTime();

                int sec = time0.secsTo(time2) + date0.daysTo(date2)*24*60*60;
                int sut = sec/86400;
                sec = sec - sut*86400;
                int hou = sec/3600;
                sec = sec - hou*3600;
                int min = sec/60;
                sec = sec - min*60;
                ui->lab_time_all->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                           QString("00" + QString::number(hou)).right(2) + ":" +
                                           QString("00" + QString::number(min)).right(2) + ":" +
                                           QString("00" + QString::number(sec)).right(2) );

                sec = time.secsTo(time2) + date.daysTo(date2)*24*60*60;
                sut = sec/86400;
                sec = sec - sut*86400;
                hou = sec/3600;
                sec = sec - hou*3600;
                min = sec/60;
                sec = sec - min*60;
                ui->lab_time->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                       QString("00" + QString::number(hou)).right(2) + ":" +
                                       QString("00" + QString::number(min)).right(2) + ":" +
                                       QString("00" + QString::number(sec)).right(2) );
                date = QDate::currentDate();
                time = QTime::currentTime();

                ui->lab_count->setText(show_num(n));
                if (ui->rb_21->isChecked())  cnt_step = 21;
                else if (ui->rb_231->isChecked())  cnt_step = 231;
                else if (ui->rb_2341->isChecked())  cnt_step = 2341;
                else if (ui->rb_23451->isChecked())  cnt_step = 23451;
                else if (ui->rb_hand->isChecked())  cnt_step = ui->spin_hand->value();
                QApplication::processEvents();


                // переоткроем транзакию для сохранения обработанных данных
                db_accBase.commit();
                db.commit();
                db_accBase.transaction();
                db.transaction();
            }
        }
        delete query;
        QApplication::processEvents();

        // добавим связки
        sql_upd = "UPDATE EVENTS "
                  "   set _update2acc = 2 "
                  " where _update2acc is NULL ";
        query_upd = new QSqlQuery(db);
        res_upd = query_upd->exec(sql_upd);
        if (!res_upd) {
            db.rollback();
            db_accBase.rollback();
            QMessageBox::warning(this, "Ошибка отметки статуса таблицы EVENTS ",
                                 "При попытке пометить данные таблицы EVENTS как успешно перенесённые произошла неожиданная ошибка.\n\n"
                                 "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_copy_data->setEnabled(true);
            return false;
        } else  {
            db.commit();
        }

        db_accBase.commit();
        ui->te_log->setText(ui->te_log->toPlainText() + " - ok (" + show_num(n) + ")");


        if (ui->ch_EVENTS_clear->isChecked()) {
            // ------------------------ //
            // Удалим устаревшие записи //
            // ------------------------ //
            db.transaction();
            db_accBase.transaction();

            // Cначала очистим таблицу _IDS_TAB
            sql_del = "DELETE FROM \"_IDS_TAB\" ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка при очистке таблицы _IDS_TAB",
                                     "При попытке очистить таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;
            // Зальём в таблицу _IDS_TAB идентификаторы записей, которые есть в исходной базе...
            sql  =  "select id "
                    "  from EVENTS ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                delete query;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения ID из таблицы EVENTS",
                                     "При попытке прочитать действующие ID из таблицы EVENTS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            int cntr = 0;
            while (query->next()) {
                int id = query->value(0).toInt();
                sql_add = "INSERT INTO \"_IDS_TAB\" (ID) "
                          "VALUES (" + QString::number(id) + ") ; ";
                query_add = new QSqlQuery(db_accBase);
                {
                    res_add = query_add->exec(sql_add);
                    if (!res_add) {
                        delete query_add;
                        delete query;
                        db.rollback();
                        db_accBase.commit();
                        db_accBase.rollback();
                        QMessageBox::warning(this, "Ошибка добавления записи в таблицу _IDS_TAB",
                                             "При попытке добавить запись в таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                             "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                        ui->bn_close->setEnabled(true);
                        ui->push_connect->setEnabled(true);
                        ui->bn_copy_data->setEnabled(true);
                        return false;
                    }
                } while (!res_add);
                delete query_add;

                cntr++;
                if (cntr%2341==0) {
                    db_accBase.commit();
                    db_accBase.transaction();
                    ui->lab_count->setText("IDs  " + show_num(cntr));
                    QApplication::processEvents();
                }
            }
            db_accBase.commit();
            db_accBase.transaction();
            ui->lab_count->setText("IDs  " + show_num(cntr));
            QApplication::processEvents();
            delete query;
            // ...посчитаем число ID которых нет в загруженной таблице _IDS_TAB
            sql_cnt = "SELECT count(*)"
                      "  FROM EVENTS a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_cnt = new QSqlQuery(db_accBase);
            res_cnt = query_cnt->exec(sql_cnt);
            if (!res_cnt) {
                delete query_cnt;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из EVENTS",
                                     "При попытке удалить устаревшие записи из EVENTS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_cnt->next();
            int cnt_to_delete = query_cnt->value(0).toInt();
            delete query_cnt;
            // ...и удалим записи, ID которых нет в загруженной таблице _IDS_TAB
            sql_del = "DELETE FROM EVENTS a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из EVENTS",
                                     "При попытке удалить устаревшие записи из EVENTS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;

            db.commit();
            db_accBase.commit();
            ui->te_log->setText(ui->te_log->toPlainText() + " - удалено " + show_num(cnt_to_delete) + " устаревших записей");
        }


        ui->ch_EVENTS->setChecked(false);
        ui->ch_EVENTS_clear->setChecked(false);
        ui->ch_EVENTS_all->setChecked(false);
    }


    //////////////////
    // // VIZITS // //
    //////////////////
    if (ui->ch_VIZITS->isChecked()) {
        ui->lab_table->setText("VIZITS");
        ui->te_log->append("VIZITS");
        QApplication::processEvents();

        db_accBase.transaction();
        db.transaction();

        if (ui->ch_VIZITS_all->isChecked()) {
            // почистим таблицу
            sql_clear = "DELETE FROM VIZITS ; ";
            query_clear = new QSqlQuery(db_accBase);
            res_clear = query_clear->exec(sql_clear);
            query_clear->next();
            delete query_clear;

            // запрос данных
            sql  =  "select id, id_person, id_point, id_operator, v_method, has_petition, r_smo, r_polis, f_polis, "
                    "       v_date "
                    "  from VIZITS "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы VIZITS",
                                     "При попытке прочитать очередную порцию данных из таблицы VIZITS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        } else {
            // запрос данных
            sql  =  "select id, id_person, id_point, id_operator, v_method, has_petition, r_smo, r_polis, f_polis, "
                    "       v_date "
                    "  from VIZITS "
                    " where _update2acc is NULL "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы VIZITS",
                                     "При попытке прочитать очередную порцию данных из таблицы VIZITS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        }

        // заготовим переменные
        int id_vizit, id_person, id_point, id_operator, v_method, has_petition, r_smo, r_polis, f_polis;
        QDate v_date;
        QDateTime dt_ins, dt_upd;

        int n = 0;

        while (query->next()) {
            //------------------//
            // прочитаем данные //
            //------------------//
            id_vizit     = query->value( 0).toInt();
            id_person    = query->value( 1).toInt();
            id_point     = query->value( 2).toInt();
            id_operator  = query->value( 3).toInt();
            v_method     = query->value( 4).toInt();
            has_petition = query->value( 5).toInt();
            r_smo        = query->value( 6).toInt();
            r_polis      = query->value( 7).toInt();
            f_polis      = query->value( 8).toInt();
            v_date       = query->value( 9).toDate();
            dt_ins       = query->value(10).toDateTime();
            dt_upd       = query->value(11).toDateTime();

            //----------------//
            // добавим запись //
            //----------------//
            if (!ui->ch_VIZITS_all->isChecked()) {
                sql_del  = "DELETE FROM VIZITS "
                           " WHERE id=" + QString::number(id_vizit) + " ; ";
                query_del = new QSqlQuery(db_accBase);
                res_del = query_del->exec(sql_del);
                if (!res_del) {
                    db.rollback();
                    db_accBase.rollback();
                    QMessageBox::warning(this, "Ошибка удаления старой записи в таблице VIZITS",
                                         "При попытке удалить старую запись в таблице VIZITS произошла неожиданная ошибка.\n\n"
                                         "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                    ui->bn_close->setEnabled(true);
                    ui->push_connect->setEnabled(true);
                    ui->bn_copy_data->setEnabled(true);
                    return false;
                }
                delete query_del;
            }

            sql_add  = "INSERT INTO VIZITS "
                       "       ( id, id_person, id_point, id_operator, v_method, has_petition, r_smo, r_polis, f_polis, "
                       "         v_date, dt_ins, dt_upd ) "
                       "VALUES ("    + QString::number(id_vizit)       + " "
                                ", " + QString::number(id_person)      + " "
                                ", " + QString::number(id_point)       + " "
                                ", " + QString::number(id_operator)    + " "
                                ", " + QString::number(v_method)       + " "
                                ", " + QString::number(has_petition)   + " "
                                ", " + QString::number(r_smo)          + " "
                                ", " + QString::number(r_polis)        + " "
                                ", " + QString::number(f_polis)        + " "
                                ", " + ( v_date.isNull() ? "NULL" : ("'" + v_date.toString("yyyy-MM-dd")  + "'") ) + " "
                                ", " + ( dt_ins.isNull() ? "NULL" : ("'" + dt_ins.toString("yyyy-MM-dd hh:mm:ss")  + "'") ) + " "
                                ", " + ( dt_upd.isNull() ? "NULL" : ("'" + dt_upd.toString("yyyy-MM-dd hh:mm:ss")  + "'") ) + " "
                                ") ; ";
            query_add = new QSqlQuery(db_accBase);
            res_add = query_add->exec(sql_add);
            if (!res_add) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка переноса данных таблицы VIZITS",
                                     "При попытке перенести в базу данных для отдела счетов данные таблицы VIZITS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_add->next();

            delete query_add;

            n++;

            if ( n%cnt_step==0 ) {
                //------------------------------//
                // отображение статуса на экран //
                //------------------------------//
                QDate date2 = QDate::currentDate();
                QTime time2 = QTime::currentTime();

                int sec = time0.secsTo(time2) + date0.daysTo(date2)*24*60*60;
                int sut = sec/86400;
                sec = sec - sut*86400;
                int hou = sec/3600;
                sec = sec - hou*3600;
                int min = sec/60;
                sec = sec - min*60;
                ui->lab_time_all->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                           QString("00" + QString::number(hou)).right(2) + ":" +
                                           QString("00" + QString::number(min)).right(2) + ":" +
                                           QString("00" + QString::number(sec)).right(2) );

                sec = time.secsTo(time2) + date.daysTo(date2)*24*60*60;
                sut = sec/86400;
                sec = sec - sut*86400;
                hou = sec/3600;
                sec = sec - hou*3600;
                min = sec/60;
                sec = sec - min*60;
                ui->lab_time->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                       QString("00" + QString::number(hou)).right(2) + ":" +
                                       QString("00" + QString::number(min)).right(2) + ":" +
                                       QString("00" + QString::number(sec)).right(2) );
                date = QDate::currentDate();
                time = QTime::currentTime();

                ui->lab_count->setText(show_num(n));
                if (ui->rb_21->isChecked())  cnt_step = 21;
                else if (ui->rb_231->isChecked())  cnt_step = 231;
                else if (ui->rb_2341->isChecked())  cnt_step = 2341;
                else if (ui->rb_23451->isChecked())  cnt_step = 23451;
                else if (ui->rb_hand->isChecked())  cnt_step = ui->spin_hand->value();
                QApplication::processEvents();


                // переоткроем транзакию для сохранения обработанных данных
                db_accBase.commit();
                db.commit();
                db_accBase.transaction();
                db.transaction();
            }
        }
        delete query;
        QApplication::processEvents();

        // добавим связки
        sql_upd = "UPDATE VIZITS "
                  "   set _update2acc = 2 "
                  " where _update2acc is NULL ";
        query_upd = new QSqlQuery(db);
        res_upd = query_upd->exec(sql_upd);
        if (!res_upd) {
            db.rollback();
            db_accBase.rollback();
            QMessageBox::warning(this, "Ошибка отметки статуса таблицы VIZITS",
                                 "При попытке пометить данные таблицы VIZITS как успешно перенесённые произошла неожиданная ошибка.\n\n"
                                 "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_copy_data->setEnabled(true);
            return false;
        } else  {
            db.commit();
        }

        db_accBase.commit();
        ui->te_log->setText(ui->te_log->toPlainText() + " - ok (" + show_num(n) + ")");


        if (ui->ch_VIZITS_clear->isChecked()) {
            // ------------------------ //
            // Удалим устаревшие записи //
            // ------------------------ //
            db.transaction();
            db_accBase.transaction();

            // Cначала очистим таблицу _IDS_TAB
            sql_del = "DELETE FROM \"_IDS_TAB\" ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка при очистке таблицы _IDS_TAB",
                                     "При попытке очистить таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;
            // Зальём в таблицу _IDS_TAB идентификаторы записей, которые есть в исходной базе...
            sql  =  "select id "
                    "  from VIZITS ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                delete query;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения ID из таблицы VIZITS",
                                     "При попытке прочитать действующие ID из таблицы VIZITS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            int cntr = 0;
            while (query->next()) {
                int id = query->value(0).toInt();
                sql_add = "INSERT INTO \"_IDS_TAB\" (ID) "
                          "VALUES (" + QString::number(id) + ") ; ";
                query_add = new QSqlQuery(db_accBase);
                {
                    res_add = query_add->exec(sql_add);
                    if (!res_add) {
                        delete query_add;
                        delete query;
                        db.rollback();
                        db_accBase.commit();
                        db_accBase.rollback();
                        QMessageBox::warning(this, "Ошибка добавления записи в таблицу _IDS_TAB",
                                             "При попытке добавить запись в таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                             "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                        ui->bn_close->setEnabled(true);
                        ui->push_connect->setEnabled(true);
                        ui->bn_copy_data->setEnabled(true);
                        return false;
                    }
                } while (!res_add);
                delete query_add;

                cntr++;
                if (cntr%2341==0) {
                    db_accBase.commit();
                    db_accBase.transaction();
                    ui->lab_count->setText("IDs  " + show_num(cntr));
                    QApplication::processEvents();
                }
            }
            db_accBase.commit();
            db_accBase.transaction();
            ui->lab_count->setText("IDs  " + show_num(cntr));
            QApplication::processEvents();
            delete query;
            // ...посчитаем число ID которых нет в загруженной таблице _IDS_TAB
            sql_cnt = "SELECT count(*)"
                      "  FROM VIZITS a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_cnt = new QSqlQuery(db_accBase);
            res_cnt = query_cnt->exec(sql_cnt);
            if (!res_cnt) {
                delete query_cnt;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из VIZITS",
                                     "При попытке удалить устаревшие записи из VIZITS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_cnt->next();
            int cnt_to_delete = query_cnt->value(0).toInt();
            delete query_cnt;
            // ...и удалим записи, ID которых нет в загруженной таблице _IDS_TAB
            sql_del = "DELETE FROM VIZITS a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из VIZITS",
                                     "При попытке удалить устаревшие записи из VIZITS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;

            db.commit();
            db_accBase.commit();
            ui->te_log->setText(ui->te_log->toPlainText() + " - удалено " + show_num(cnt_to_delete) + " устаревших записей");
        }


        ui->ch_VIZITS->setChecked(false);
        ui->ch_VIZITS_clear->setChecked(false);
        ui->ch_VIZITS_all->setChecked(false);
    }


    ///////////////////
    // // POLISES // //
    ///////////////////
    if (ui->ch_POLISES->isChecked()) {
        ui->lab_table->setText("POLISES");
        ui->te_log->append("POLISES");
        QApplication::processEvents();

        db_accBase.transaction();
        db.transaction();

        if (ui->ch_POLISES_all->isChecked()) {
            // почистим таблицу
            sql_clear = "DELETE FROM POLISES ; ";
            query_clear = new QSqlQuery(db_accBase);
            res_clear = query_clear->exec(sql_clear);
            query_clear->next();
            delete query_clear;

            // запрос данных
            sql  =  "select id, id_old_polis, id_person, "
                    "       ocato, ogrn_smo, "
                    "       r_smo, r_polis, f_polis, pol_v, in_erp, "
                    "       vs_num, "
                    "       pol_ser, pol_num, uec_num, enp, "
                    "       date_begin, date_end, date_stop, date_sms_pol, date_activate, date_get2hand, "
                    "       dt_ins, dt_upd "
                    "  from POLISES "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы POLISES",
                                     "При попытке прочитать очередную порцию данных из таблицы POLISES произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        } else {
            // запрос данных
            sql  =  "select id, id_old_polis, id_person, "
                    "       ocato, ogrn_smo, "
                    "       r_smo, r_polis, f_polis, pol_v, in_erp, "
                    "       vs_num, "
                    "       pol_ser, pol_num, uec_num, enp, "
                    "       date_begin, date_end, date_stop, date_sms_pol, date_activate, date_get2hand "
                    "  from POLISES "
                    " where _update2acc is NULL "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы POLISES",
                                     "При попытке прочитать очередную порцию данных из таблицы POLISES произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        }

        // заготовим переменные
        int id_polis, id_old_polis, id_person;
        QString ocato, ogrn_smo;
        int r_smo, r_polis, f_polis, pol_v, in_erp;
        int vs_num;
        QString pol_ser, pol_num, uec_num, enp;
        QDate date_begin, date_end, date_stop, date_sms_pol, date_activate, date_get2hand;
        QDateTime dt_ins, dt_upd;

        int n = 0;

        while (query->next()) {
            //------------------//
            // прочитаем данные //
            //------------------//
            id_polis      = query->value( 0).toInt();
            id_old_polis  = query->value( 1).toInt();
            id_person     = query->value( 2).toInt();
            ocato         = query->value( 3).toString();
            ogrn_smo      = query->value( 4).toString();
            r_smo         = query->value( 5).toInt();
            r_polis       = query->value( 6).toInt();
            f_polis       = query->value( 7).toInt();
            pol_v         = query->value( 8).toInt();
            in_erp        = query->value( 9).toInt();
            vs_num        = query->value(10).toInt();
            pol_ser       = query->value(11).toString();
            pol_num       = query->value(12).toString();
            uec_num       = query->value(13).toString();
            enp           = query->value(14).toString();
            date_begin    = query->value(15).toDate();
            date_end      = query->value(16).toDate();
            date_stop     = query->value(17).toDate();
            date_sms_pol  = query->value(18).toDate();
            date_activate = query->value(19).toDate();
            date_get2hand = query->value(20).toDate();
            dt_ins        = query->value(21).toDateTime();
            dt_upd        = query->value(22).toDateTime();

            //----------------//
            // добавим запись //
            //----------------//
            if (!ui->ch_POLISES_all->isChecked()) {
                sql_del  = "DELETE FROM POLISES "
                           " WHERE id=" + QString::number(id_polis) + " ; ";
                query_del = new QSqlQuery(db_accBase);
                res_del = query_del->exec(sql_del);
                if (!res_del) {
                    db.rollback();
                    db_accBase.rollback();
                    QMessageBox::warning(this, "Ошибка удаления старой записи в таблице POLISES",
                                         "При попытке удалить старую запись в таблице POLISES произошла неожиданная ошибка.\n\n"
                                         "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                    ui->bn_close->setEnabled(true);
                    ui->push_connect->setEnabled(true);
                    ui->bn_copy_data->setEnabled(true);
                    return false;
                }
                delete query_del;
            }

            sql_add  = "INSERT INTO POLISES "
                       "       ( id, id_old_polis, id_person, "
                       "         ocato, ogrn_smo, "
                       "         r_smo, r_polis, f_polis, pol_v, in_erp, "
                       "         vs_num, pol_ser, pol_num, uec_num, enp, "
                       "         date_begin, date_end, date_stop, date_sms_pol, date_activate, date_get2hand, "
                       "         dt_ins, dt_upd ) "
                       "VALUES ("    + QString::number(id_polis)    + " "
                                ", " + QString::number(id_old_polis)+ " "
                                ", " + QString::number(id_person)   + " "
                                ",'" + ocato                        + "'"
                                ",'" + ogrn_smo                     + "'"
                                ", " + QString::number(r_smo)       + " "
                                ", " + QString::number(r_polis)     + " "
                                ", " + QString::number(f_polis)     + " "
                                ", " + QString::number(pol_v)       + " "
                                ", " + QString::number(in_erp)      + " "
                                ", " + QString::number(vs_num)      + " "
                                ",'" + pol_ser                      + "'"
                                ",'" + pol_num                      + "'"
                                ",'" + uec_num                      + "'"
                                ",'" + enp                          + "'"
                                ", " + ( date_begin.isNull()    ? "NULL" : ("'" + date_begin.toString("yyyy-MM-dd")    + "'") ) + " "
                                ", " + ( date_end.isNull()      ? "NULL" : ("'" + date_end.toString("yyyy-MM-dd")      + "'") ) + " "
                                ", " + ( date_stop.isNull()     ? "NULL" : ("'" + date_stop.toString("yyyy-MM-dd")     + "'") ) + " "
                                ", " + ( date_sms_pol.isNull()  ? "NULL" : ("'" + date_sms_pol.toString("yyyy-MM-dd")  + "'") ) + " "
                                ", " + ( date_activate.isNull() ? "NULL" : ("'" + date_activate.toString("yyyy-MM-dd") + "'") ) + " "
                                ", " + ( date_get2hand.isNull() ? "NULL" : ("'" + date_get2hand.toString("yyyy-MM-dd") + "'") ) + " "
                                ", " + ( dt_ins.isNull() ? "NULL" : ("'" + dt_ins.toString("yyyy-MM-dd hh:mm:ss")  + "'") ) + " "
                                ", " + ( dt_upd.isNull() ? "NULL" : ("'" + dt_upd.toString("yyyy-MM-dd hh:mm:ss")  + "'") ) + " "
                                ") ; ";
            query_add = new QSqlQuery(db_accBase);
            res_add = query_add->exec(sql_add);
            if (!res_add) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка переноса данных таблицы POLISES",
                                     "При попытке перенести в базу данных для отдела счетов данные таблицы POLISES произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_add->next();

            delete query_add;

            n++;

            if ( n%cnt_step==0 ) {
                //------------------------------//
                // отображение статуса на экран //
                //------------------------------//
                QDate date2 = QDate::currentDate();
                QTime time2 = QTime::currentTime();

                int sec = time0.secsTo(time2) + date0.daysTo(date2)*24*60*60;
                int sut = sec/86400;
                sec = sec - sut*86400;
                int hou = sec/3600;
                sec = sec - hou*3600;
                int min = sec/60;
                sec = sec - min*60;
                ui->lab_time_all->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                           QString("00" + QString::number(hou)).right(2) + ":" +
                                           QString("00" + QString::number(min)).right(2) + ":" +
                                           QString("00" + QString::number(sec)).right(2) );

                sec = time.secsTo(time2) + date.daysTo(date2)*24*60*60;
                sut = sec/86400;
                sec = sec - sut*86400;
                hou = sec/3600;
                sec = sec - hou*3600;
                min = sec/60;
                sec = sec - min*60;
                ui->lab_time->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                       QString("00" + QString::number(hou)).right(2) + ":" +
                                       QString("00" + QString::number(min)).right(2) + ":" +
                                       QString("00" + QString::number(sec)).right(2) );
                date = QDate::currentDate();
                time = QTime::currentTime();

                ui->lab_count->setText(show_num(n));
                if (ui->rb_21->isChecked())  cnt_step = 21;
                else if (ui->rb_231->isChecked())  cnt_step = 231;
                else if (ui->rb_2341->isChecked())  cnt_step = 2341;
                else if (ui->rb_23451->isChecked())  cnt_step = 23451;
                else if (ui->rb_hand->isChecked())  cnt_step = ui->spin_hand->value();
                QApplication::processEvents();


                // переоткроем транзакию для сохранения обработанных данных
                db_accBase.commit();
                db.commit();
                db_accBase.transaction();
                db.transaction();
            }
        }
        delete query;
        QApplication::processEvents();

        // добавим связки
        sql_upd = "UPDATE POLISES "
                  "   set _update2acc = 2 "
                  " where _update2acc is NULL ";
        query_upd = new QSqlQuery(db);
        res_upd = query_upd->exec(sql_upd);
        if (!res_upd) {
            db.rollback();
            db_accBase.rollback();
            QMessageBox::warning(this, "Ошибка отметки статуса таблицы POLISES ",
                                 "При попытке пометить данные таблицы POLISES как успешно перенесённые произошла неожиданная ошибка.\n\n"
                                 "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_copy_data->setEnabled(true);
            return false;
        } else  {
            db.commit();
        }

        db_accBase.commit();
        ui->te_log->setText(ui->te_log->toPlainText() + " - ok (" + show_num(n) + ")");


        if (ui->ch_POLISES_clear->isChecked()) {
            // ------------------------ //
            // Удалим устаревшие записи //
            // ------------------------ //
            db.transaction();
            db_accBase.transaction();

            // Cначала очистим таблицу _IDS_TAB
            sql_del = "DELETE FROM \"_IDS_TAB\" ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка при очистке таблицы _IDS_TAB",
                                     "При попытке очистить таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;
            // Зальём в таблицу _IDS_TAB идентификаторы записей, которые есть в исходной базе...
            sql  =  "select id "
                    "  from POLISES ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                delete query;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения ID из таблицы POLISES",
                                     "При попытке прочитать действующие ID из таблицы POLISES произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            int cntr = 0;
            while (query->next()) {
                int id = query->value(0).toInt();
                sql_add = "INSERT INTO \"_IDS_TAB\" (ID) "
                          "VALUES (" + QString::number(id) + ") ; ";
                query_add = new QSqlQuery(db_accBase);
                {
                    res_add = query_add->exec(sql_add);
                    if (!res_add) {
                        delete query_add;
                        delete query;
                        db.rollback();
                        db_accBase.commit();
                        db_accBase.rollback();
                        QMessageBox::warning(this, "Ошибка добавления записи в таблицу _IDS_TAB",
                                             "При попытке добавить запись в таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                             "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                        ui->bn_close->setEnabled(true);
                        ui->push_connect->setEnabled(true);
                        ui->bn_copy_data->setEnabled(true);
                        return false;
                    }
                } while (!res_add);
                delete query_add;

                cntr++;
                if (cntr%2341==0) {
                    db_accBase.commit();
                    db_accBase.transaction();
                    ui->lab_count->setText("IDs  " + show_num(cntr));
                    QApplication::processEvents();
                }
            }
            db_accBase.commit();
            db_accBase.transaction();
            ui->lab_count->setText("IDs  " + show_num(cntr));
            QApplication::processEvents();
            delete query;
            // ...посчитаем число ID которых нет в загруженной таблице _IDS_TAB
            sql_cnt = "SELECT count(*)"
                      "  FROM POLISES a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_cnt = new QSqlQuery(db_accBase);
            res_cnt = query_cnt->exec(sql_cnt);
            if (!res_cnt) {
                delete query_cnt;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из POLISES",
                                     "При попытке удалить устаревшие записи из POLISES произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_cnt->next();
            int cnt_to_delete = query_cnt->value(0).toInt();
            delete query_cnt;
            // ...и удалим записи, ID которых нет в загруженной таблице _IDS_TAB
            sql_del = "DELETE FROM POLISES a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из POLISES",
                                     "При попытке удалить устаревшие записи из POLISES произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;

            db.commit();
            db_accBase.commit();
            ui->te_log->setText(ui->te_log->toPlainText() + " - удалено " + show_num(cnt_to_delete) + " устаревших записей");
        }


        ui->ch_POLISES->setChecked(false);
        ui->ch_POLISES_clear->setChecked(false);
        ui->ch_POLISES_all->setChecked(false);
    }


    ///////////////////
    // // PERSONS // //
    ///////////////////
    if (ui->ch_PERSONS->isChecked()) {
        ui->lab_table->setText("PERSONS");
        ui->te_log->append("PERSONS");
        QApplication::processEvents();

        db_accBase.transaction();
        db.transaction();

        if (ui->ch_PERSONS_all->isChecked()) {
            // почистим таблицу
            sql_clear = "DELETE FROM PERSONS ; ";
            query_clear = new QSqlQuery(db_accBase);
            res_clear = query_clear->exec(sql_clear);
            query_clear->next();
            delete query_clear;

            // запрос данных
            sql  =  "select id, category, rep_id, rep_relation, sex, oksm_birth, oksm_c, bomj, id_addr_reg, id_addr_liv, polis_post_way, status, id_folder, id_insure_agent1, id_insure_agent2, id_udl, id_drp, id_polis, la._id_asg, "
                    "       fam, im, ot, fam_norm, im_norm, ot_norm, dost, snils, enp, phone_cell, phone_home, phone_work, phone_code, email, "
                    "       date_birth, date_death, date_reg "
                    "  from PERSONS e "
                    "  left join link_persons_asg la on(la.id_person=e.id) "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы PERSONS",
                                     "При попытке прочитать очередную порцию данных из таблицы PERSONS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        } else {
            // запрос данных
            sql  =  "select id, category, rep_id, rep_relation, sex, oksm_birth, oksm_c, bomj, id_addr_reg, id_addr_liv, polis_post_way, status, id_folder, id_insure_agent1, id_insure_agent2, id_udl, id_drp, id_polis, la._id_asg, "
                    "       fam, im, ot, fam_norm, im_norm, ot_norm, dost, snils, enp, phone_cell, phone_home, phone_work, phone_code, email, "
                    "       date_birth, date_death, date_reg "
                    "  from PERSONS e "
                    "  left join link_persons_asg la on(la.id_person=e.id) "
                    " where _update2acc is NULL "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы PERSONS",
                                     "При попытке прочитать очередную порцию данных из таблицы PERSONS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        }

        // заготовим переменные
        int id_person, category, rep_id, rep_relation, sex, oksm_birth, oksm_c, bomj, id_addr_reg, id_addr_liv, polis_post_way, status, id_folder, id_insure_agent1, id_insure_agent2, id_udl, id_drp, id_polis, id_assig;
        QString fam, im, ot, fam_norm, im_norm, ot_norm, dost, snils, enp, phone_cell, phone_home, phone_work, phone_code, email;
        QDate date_birth, date_death, date_reg;
        QDateTime dt_ins, dt_upd;

        int n = 0;

        while (query->next()) {
            //------------------//
            // прочитаем данные //
            //------------------//
            id_person        = query->value( 0).toInt();
            category         = query->value( 1).toInt();
            rep_id           = query->value( 2).toInt();
            rep_relation     = query->value( 3).toInt();
            sex              = query->value( 4).toInt();
            oksm_birth       = query->value( 5).toInt();
            oksm_c           = query->value( 6).toInt();
            bomj             = query->value( 7).toInt();
            id_addr_reg      = query->value( 8).toInt();
            id_addr_liv      = query->value( 9).toInt();
            polis_post_way   = query->value(10).toInt();
            status           = query->value(11).toInt();
            id_folder        = query->value(12).toInt();
            id_insure_agent1 = query->value(13).toInt();
            id_insure_agent2 = query->value(14).toInt();
            //id_udl           = query->value(15).toInt();
            //id_drp           = query->value(16).toInt();
            //id_polis         = query->value(17).toInt();
            //id_assig         = query->value(18).toInt();
            fam              = query->value(19).toString();
            im               = query->value(20).toString();
            ot               = query->value(21).toString();
            fam_norm         = query->value(22).toString();
            im_norm          = query->value(23).toString();
            ot_norm          = query->value(24).toString();
            dost             = query->value(25).toString();
            snils            = query->value(26).toString();
            enp              = query->value(27).toString();
            phone_cell       = query->value(28).toString();
            phone_home       = query->value(29).toString();
            phone_work       = query->value(30).toString();
            phone_code       = query->value(31).toString();
            email            = query->value(32).toString();
            date_birth       = query->value(33).toDate();
            date_death       = query->value(34).toDate();
            date_reg         = query->value(35).toDate();
            dt_ins           = query->value(36).toDateTime();
            dt_upd           = query->value(37).toDateTime();


            //----------------//
            // добавим запись //
            //----------------//
            if (!ui->ch_PERSONS_all->isChecked()) {
                sql_del  = "DELETE FROM PERSONS "
                           " WHERE id=" + QString::number(id_person) + " ; ";
                query_del = new QSqlQuery(db_accBase);
                res_del = query_del->exec(sql_del);
                if (!res_del) {
                    db.rollback();
                    db_accBase.rollback();
                    QMessageBox::warning(this, "Ошибка удаления старой записи в таблице PERSONS",
                                         "При попытке удалить старую запись в таблице PERSONS произошла неожиданная ошибка.\n\n"
                                         "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                    ui->bn_close->setEnabled(true);
                    ui->push_connect->setEnabled(true);
                    ui->bn_copy_data->setEnabled(true);
                    return false;
                }
                delete query_del;
            }

            sql_add  = "INSERT INTO PERSONS "
                       "       ( id, category, rep_id, rep_relation, sex, oksm_birth, oksm_c, bomj, id_addr_reg, id_addr_liv, "
                       "         polis_post_way, status, id_folder, id_insure_agent1, id_insure_agent2, /*id_udl, id_drp, id_polis, id_assig,*/ "
                       "         fam, im, ot, fam_norm, im_norm, ot_norm, "
                       "         dost, snils, enp, "
                       "         phone_cell, phone_home, phone_work, phone_code, email, "
                       "         date_birth, date_death, date_reg, dt_ins, dt_upd ) "
                       "VALUES ("    + QString::number(id_person)        + " "
                                ", " + QString::number(category)         + " "
                                ", " + QString::number(rep_id)           + " "
                                ", " + QString::number(rep_relation)     + " "
                                ", " + QString::number(sex)              + " "
                                ", " + QString::number(oksm_birth)       + " "
                                ", " + QString::number(oksm_c)           + " "
                                ", " + QString::number(bomj)             + " "
                                ", " + QString::number(id_addr_reg)      + " "
                                ", " + QString::number(id_addr_liv)      + " "
                                ", " + QString::number(polis_post_way)   + " "
                                ", " + QString::number(status)           + " "
                                ", " + QString::number(id_folder)        + " "
                                ", " + QString::number(id_insure_agent1) + " "
                                ", " + QString::number(id_insure_agent2) + " "
                                //", " + QString::number(id_udl)           + " "
                                //", " + QString::number(id_drp)           + " "
                                //", " + QString::number(id_polis)         + " "
                                //", " + QString::number(id_assig)         + " "
                                ",'" + fam                               + "'"
                                ",'" + im                                + "'"
                                ",'" + ot                                + "'"
                                ",'" + fam_norm                          + "'"
                                ",'" + im_norm                           + "'"
                                ",'" + ot_norm                           + "'"
                                ",'" + dost                              + "'"
                                ",'" + snils                             + "'"
                                ",'" + enp                               + "'"
                                ",'" + phone_cell                        + "'"
                                ",'" + phone_home                        + "'"
                                ",'" + phone_work                        + "'"
                                ",'" + phone_code                        + "'"
                                ",'" + email                             + "'"
                                ", " + ( date_birth.isNull()  ? "NULL" : ("'" + date_birth.toString("yyyy-MM-dd")  + "'") ) + " "
                                ", " + ( date_death.isNull()  ? "NULL" : ("'" + date_death.toString("yyyy-MM-dd")  + "'") ) + " "
                                ", " + ( date_reg.isNull()    ? "NULL" : ("'" + date_reg.toString("yyyy-MM-dd")    + "'") ) + " "
                                ", " + ( dt_ins.isNull() ? "NULL" : ("'" + dt_ins.toString("yyyy-MM-dd hh:mm:ss")  + "'") ) + " "
                                ", " + ( dt_upd.isNull() ? "NULL" : ("'" + dt_upd.toString("yyyy-MM-dd hh:mm:ss")  + "'") ) + " "
                                ") "
                       " returning id ; ";
            query_add = new QSqlQuery(db_accBase);
            res_add = query_add->exec(sql_add);
            if (!res_add) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка переноса данных таблицы PERSONS",
                                     "При попытке перенести в базу данных для отдела счетов данные таблицы PERSONS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_add->next();

            delete query_add;

            n++;

            if ( n%cnt_step==0 ) {
                //------------------------------//
                // отображение статуса на экран //
                //------------------------------//
                QDate date2 = QDate::currentDate();
                QTime time2 = QTime::currentTime();

                int sec = time0.secsTo(time2) + date0.daysTo(date2)*24*60*60;
                int sut = sec/86400;
                sec = sec - sut*86400;
                int hou = sec/3600;
                sec = sec - hou*3600;
                int min = sec/60;
                sec = sec - min*60;
                ui->lab_time_all->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                           QString("00" + QString::number(hou)).right(2) + ":" +
                                           QString("00" + QString::number(min)).right(2) + ":" +
                                           QString("00" + QString::number(sec)).right(2) );

                sec = time.secsTo(time2) + date.daysTo(date2)*24*60*60;
                sut = sec/86400;
                sec = sec - sut*86400;
                hou = sec/3600;
                sec = sec - hou*3600;
                min = sec/60;
                sec = sec - min*60;
                ui->lab_time->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                       QString("00" + QString::number(hou)).right(2) + ":" +
                                       QString("00" + QString::number(min)).right(2) + ":" +
                                       QString("00" + QString::number(sec)).right(2) );
                date = QDate::currentDate();
                time = QTime::currentTime();

                ui->lab_count->setText(show_num(n));
                if (ui->rb_21->isChecked())  cnt_step = 21;
                else if (ui->rb_231->isChecked())  cnt_step = 231;
                else if (ui->rb_2341->isChecked())  cnt_step = 2341;
                else if (ui->rb_23451->isChecked())  cnt_step = 23451;
                else if (ui->rb_hand->isChecked())  cnt_step = ui->spin_hand->value();
                QApplication::processEvents();


                // переоткроем транзакию для сохранения обработанных данных
                db_accBase.commit();
                db.commit();
                db_accBase.transaction();
                db.transaction();
            }
        }
        delete query;
        QApplication::processEvents();

        // добавим связки
        sql_upd = "UPDATE PERSONS "
                  "   set _update2acc = 2 "
                  " where _update2acc is NULL ";
        query_upd = new QSqlQuery(db);
        res_upd = query_upd->exec(sql_upd);
        if (!res_upd) {
            db.rollback();
            db_accBase.rollback();
            QMessageBox::warning(this, "Ошибка отметки статуса таблицы PERSONS ",
                                 "При попытке пометить данные таблицы PERSONS как успешно перенесённые произошла неожиданная ошибка.\n\n"
                                 "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_copy_data->setEnabled(true);
            return false;
        } else  {
            db.commit();
        }

        db_accBase.commit();
        ui->te_log->setText(ui->te_log->toPlainText() + " - ok (" + show_num(n) + ")");


        if (ui->ch_PERSONS_clear->isChecked()) {
            // ------------------------ //
            // Удалим устаревшие записи //
            // ------------------------ //
            db.transaction();
            db_accBase.transaction();

            // Cначала очистим таблицу _IDS_TAB
            sql_del = "DELETE FROM \"_IDS_TAB\" ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка при очистке таблицы _IDS_TAB",
                                     "При попытке очистить таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;
            // Зальём в таблицу _IDS_TAB идентификаторы записей, которые есть в исходной базе...
            sql  =  "select id "
                    "  from PERSONS ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                delete query;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения ID из таблицы PERSONS",
                                     "При попытке прочитать действующие ID из таблицы PERSONS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            int cntr = 0;
            while (query->next()) {
                int id = query->value(0).toInt();
                sql_add = "INSERT INTO \"_IDS_TAB\" (ID) "
                          "VALUES (" + QString::number(id) + ") ; ";
                query_add = new QSqlQuery(db_accBase);
                {
                    res_add = query_add->exec(sql_add);
                    if (!res_add) {
                        delete query_add;
                        delete query;
                        db.rollback();
                        db_accBase.commit();
                        db_accBase.rollback();
                        QMessageBox::warning(this, "Ошибка добавления записи в таблицу _IDS_TAB",
                                             "При попытке добавить запись в таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                             "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                        ui->bn_close->setEnabled(true);
                        ui->push_connect->setEnabled(true);
                        ui->bn_copy_data->setEnabled(true);
                        return false;
                    }
                } while (!res_add);
                delete query_add;

                cntr++;
                if (cntr%2341==0) {
                    db_accBase.commit();
                    db_accBase.transaction();
                    ui->lab_count->setText("IDs  " + show_num(cntr));
                    QApplication::processEvents();
                }
            }
            db_accBase.commit();
            db_accBase.transaction();
            ui->lab_count->setText("IDs  " + show_num(cntr));
            QApplication::processEvents();
            delete query;
            // ...посчитаем число ID которых нет в загруженной таблице _IDS_TAB
            sql_cnt = "SELECT count(*)"
                      "  FROM PERSONS a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_cnt = new QSqlQuery(db_accBase);
            res_cnt = query_cnt->exec(sql_cnt);
            if (!res_cnt) {
                delete query_cnt;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из PERSONS",
                                     "При попытке удалить устаревшие записи из PERSONS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_cnt->next();
            int cnt_to_delete = query_cnt->value(0).toInt();
            delete query_cnt;
            // ...и удалим записи, ID которых нет в загруженной таблице _IDS_TAB
            sql_del = "DELETE FROM PERSONS a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из PERSONS",
                                     "При попытке удалить устаревшие записи из PERSONS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;

            db.commit();
            db_accBase.commit();
            ui->te_log->setText(ui->te_log->toPlainText() + " - удалено " + show_num(cnt_to_delete) + " устаревших записей");
        }


        ui->ch_PERSONS->setChecked(false);
        ui->ch_PERSONS_clear->setChecked(false);
        ui->ch_PERSONS_all->setChecked(false);
    }


    /////////////////////
    // // ADDRESSES // //
    /////////////////////
    if (ui->ch_ADDRESSES->isChecked()) {
        ui->lab_table->setText("ADDRESSES");
        ui->te_log->append("ADDRESSES");
        QApplication::processEvents();

        db_accBase.transaction();
        db.transaction();

        if (ui->ch_ADDRESSES_all->isChecked()) {
            // почистим таблицу
            sql_clear = "DELETE FROM addresses ; ";
            query_clear = new QSqlQuery(db_accBase);
            res_clear = query_clear->exec(sql_clear);
            query_clear->next();
            delete query_clear;

            // запрос данных
            sql  =  "select id, post_index, ocato, ocato_full, kladr, kladr_street, "
                    "       subj, dstr, city, nasp, strt, house, quart, fias_aoid, fias_houseid, "
                    "       subj_name, dstr_name, city_name, nasp_name, strt_name "
                    "  from ADDRESSES "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы ADDRESSES",
                                     "При попытке прочитать очередную порцию данных из таблицы ADDRESSES произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        } else {
            // запрос данных
            sql  =  "select id, post_index, ocato, ocato_full, kladr, kladr_street, "
                    "       subj, dstr, city, nasp, strt, house, quart, fias_aoid, fias_houseid, "
                    "       subj_name, dstr_name, city_name, nasp_name, strt_name "
                    "  from addresses "
                    " where _update2acc is NULL "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы ADDRESSES",
                                     "При попытке прочитать очередную порцию данных из таблицы ADDRESSES произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        }

        // заготовим переменные
        int id_address;
        QString post_index, ocato, ocato_full, kladr, kladr_street;
        int subj, dstr, city, nasp, strt, house;
        QString quart;
        QString fias_aoid, fias_houseid;
        QString subj_name, dstr_name, city_name, nasp_name, strt_name;

        int n = 0;

        while (query->next()) {
            //------------------//
            // прочитаем данные //
            //------------------//
            id_address   = query->value( 0).toInt();
            post_index   = query->value( 1).toString();
            ocato        = query->value( 2).toString();
            ocato_full   = query->value( 3).toString();
            kladr        = query->value( 4).toString();
            kladr_street = query->value( 5).toString();
            subj         = query->value( 6).toInt();
            dstr         = query->value( 7).toInt();
            city         = query->value( 8).toInt();
            nasp         = query->value( 9).toInt();
            strt         = query->value(10).toInt();
            house        = query->value(11).toInt();
            quart        = query->value(12).toString();
            fias_aoid    = query->value(13).toString();
            fias_houseid = query->value(14).toString();
            subj_name      = query->value(15).toString();
            dstr_name      = query->value(16).toString();
            city_name      = query->value(17).toString();
            nasp_name      = query->value(18).toString();
            strt_name      = query->value(19).toString();

            //----------------//
            // добавим запись //
            //----------------//
            if (!ui->ch_ADDRESSES_all->isChecked()) {
                sql_del  = "DELETE FROM ADDRESSES "
                           " WHERE id=" + QString::number(id_address) + " ; ";
                query_del = new QSqlQuery(db_accBase);
                res_del = query_del->exec(sql_del);
                /*if (!res_del) {
                    db.rollback();
                    db_accBase.rollback();
                    QMessageBox::warning(this, "Ошибка удаления старой записи в таблице ADDRESSES",
                                         "При попытке удалить старую запись в таблице ADDRESSES произошла неожиданная ошибка.\n\n"
                                         "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                    ui->bn_close->setEnabled(true);
                    ui->push_connect->setEnabled(true);
                    ui->bn_copy_data->setEnabled(true);
                    return false;
                }*/
                delete query_del;
            }

            sql_add  = "INSERT INTO ADDRESSES(id, post_index, ocato, ocato_full, kladr, kladr_street, "
                       "       subj, dstr, city, nasp, strt, house, quart, fias_aoid, fias_houseid,"
                       "       subj_name, dstr_name, city_name, nasp_name, strt_name) "
                       "VALUES ("    + QString::number(id_address) + " "
                                ",'" + post_index             + "'"
                                ",'" + ocato                  + "'"
                                ",'" + ocato_full             + "'"
                                ",'" + kladr                  + "'"
                                ",'" + kladr_street           + "'"
                                ", " + QString::number(subj)  + " "
                                ", " + QString::number(dstr)  + " "
                                ", " + QString::number(city)  + " "
                                ", " + QString::number(nasp)  + " "
                                ", " + QString::number(strt)  + " "
                                ", " + QString::number(house) + " "
                                ",'" + quart                  + "'"
                                ",'" + fias_aoid              + "'"
                                ",'" + fias_houseid           + "'"
                                ",'" + subj_name                + "'"
                                ",'" + dstr_name                + "'"
                                ",'" + city_name                + "'"
                                ",'" + nasp_name                + "'"
                                ",'" + strt_name                + "'"
                                ") ; ";
            query_add = new QSqlQuery(db_accBase);
            res_add = query_add->exec(sql_add);
            if (!res_add) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка переноса данных таблицы ADDRESSES",
                                     "При попытке перенести в базу данных для отдела счетов данные таблицы ADDRESSES произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_add->next();

            delete query_add;

            n++;

            if ( n%cnt_step==0 ) {
                //------------------------------//
                // отображение статуса на экран //
                //------------------------------//
                QDate date2 = QDate::currentDate();
                QTime time2 = QTime::currentTime();

                int sec = time0.secsTo(time2) + date0.daysTo(date2)*24*60*60;
                int sut = sec/86400;
                sec = sec - sut*86400;
                int hou = sec/3600;
                sec = sec - hou*3600;
                int min = sec/60;
                sec = sec - min*60;
                ui->lab_time_all->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                           QString("00" + QString::number(hou)).right(2) + ":" +
                                           QString("00" + QString::number(min)).right(2) + ":" +
                                           QString("00" + QString::number(sec)).right(2) );

                sec = time.secsTo(time2) + date.daysTo(date2)*24*60*60;
                sut = sec/86400;
                sec = sec - sut*86400;
                hou = sec/3600;
                sec = sec - hou*3600;
                min = sec/60;
                sec = sec - min*60;
                ui->lab_time->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                       QString("00" + QString::number(hou)).right(2) + ":" +
                                       QString("00" + QString::number(min)).right(2) + ":" +
                                       QString("00" + QString::number(sec)).right(2) );
                date = QDate::currentDate();
                time = QTime::currentTime();

                ui->lab_count->setText(show_num(n));
                if (ui->rb_21->isChecked())  cnt_step = 21;
                else if (ui->rb_231->isChecked())  cnt_step = 231;
                else if (ui->rb_2341->isChecked())  cnt_step = 2341;
                else if (ui->rb_23451->isChecked())  cnt_step = 23451;
                else if (ui->rb_hand->isChecked())  cnt_step = ui->spin_hand->value();
                QApplication::processEvents();


                // переоткроем транзакию для сохранения обработанных данных
                db_accBase.commit();
                db.commit();
                db_accBase.transaction();
                db.transaction();
            }
        }
        delete query;
        QApplication::processEvents();

        // добавим связки
        sql_upd = "UPDATE ADDRESSES "
                  "   set _update2acc = 2 "
                  " where _update2acc is NULL ";
        query_upd = new QSqlQuery(db);
        res_upd = query_upd->exec(sql_upd);
        if (!res_upd) {
            db.rollback();
            db_accBase.rollback();
            QMessageBox::warning(this, "Ошибка отметки статуса таблицы ADDRESSES",
                                 "При попытке пометить данные таблицы ADDRESSES как успешно перенесённые произошла неожиданная ошибка.\n\n"
                                 "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_copy_data->setEnabled(true);
            return false;
        } else  {
            db.commit();
        }

        db_accBase.commit();
        ui->te_log->setText(ui->te_log->toPlainText() + " - ok (" + show_num(n) + ")");


        if (ui->ch_ADDRESSES_clear->isChecked()) {
            // ------------------------ //
            // Удалим устаревшие записи //
            // ------------------------ //
            db.transaction();
            db_accBase.transaction();

            // Cначала очистим таблицу _IDS_TAB
            sql_del = "DELETE FROM \"_IDS_TAB\" ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка при очистке таблицы _IDS_TAB",
                                     "При попытке очистить таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;
            // Зальём в таблицу _IDS_TAB идентификаторы записей, которые есть в исходной базе...
            sql  =  "select id "
                    "  from ADDRESSES ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                delete query;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения ID из таблицы ADDRESSES",
                                     "При попытке прочитать действующие ID из таблицы ADDRESSES произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            int cntr = 0;
            while (query->next()) {
                int id = query->value(0).toInt();
                sql_add = "INSERT INTO \"_IDS_TAB\" (ID) "
                          "VALUES (" + QString::number(id) + ") ; ";
                query_add = new QSqlQuery(db_accBase);
                {
                    res_add = query_add->exec(sql_add);
                    if (!res_add) {
                        delete query_add;
                        delete query;
                        db.rollback();
                        db_accBase.commit();
                        db_accBase.rollback();
                        QMessageBox::warning(this, "Ошибка добавления записи в таблицу _IDS_TAB",
                                             "При попытке добавить запись в таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                             "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                        ui->bn_close->setEnabled(true);
                        ui->push_connect->setEnabled(true);
                        ui->bn_copy_data->setEnabled(true);
                        return false;
                    }
                } while (!res_add);
                delete query_add;

                cntr++;
                if (cntr%2341==0) {
                    db_accBase.commit();
                    db_accBase.transaction();
                    ui->lab_count->setText("IDs  " + show_num(cntr));
                    QApplication::processEvents();
                }
            }
            db_accBase.commit();
            db_accBase.transaction();
            ui->lab_count->setText("IDs  " + show_num(cntr));
            QApplication::processEvents();
            delete query;
            // ...посчитаем число ID которых нет в загруженной таблице _IDS_TAB
            sql_cnt = "SELECT count(*)"
                      "  FROM ADDRESSES a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_cnt = new QSqlQuery(db_accBase);
            res_cnt = query_cnt->exec(sql_cnt);
            if (!res_cnt) {
                delete query_cnt;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из ADDRESSES",
                                     "При попытке удалить устаревшие записи из ADDRESSES произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_cnt->next();
            int cnt_to_delete = query_cnt->value(0).toInt();
            delete query_cnt;
            // ...и удалим записи, ID которых нет в загруженной таблице _IDS_TAB
            sql_del = "DELETE FROM ADDRESSES a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из ADDRESSES",
                                     "При попытке удалить устаревшие записи из ADDRESSES произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;

            db.commit();
            db_accBase.commit();
            ui->te_log->setText(ui->te_log->toPlainText() + " - удалено " + show_num(cnt_to_delete) + " устаревших записей");
        }


        ui->ch_ADDRESSES->setChecked(false);
        ui->ch_ADDRESSES_clear->setChecked(false);
        ui->ch_ADDRESSES_all->setChecked(false);
    }



    ///////////////////////
    // // PERSONS_OLD // //
    ///////////////////////
    if (ui->ch_PERSONS_OLD->isChecked()) {
        ui->lab_table->setText("PERSONS_OLD");
        ui->te_log->append("PERSONS_OLD");
        QApplication::processEvents();

        db_accBase.transaction();
        db.transaction();

        if (ui->ch_PERSONS_OLD_all->isChecked()) {
            // почистим таблицу
            sql_clear = "DELETE FROM PERSONS_OLD ; ";
            query_clear = new QSqlQuery(db_accBase);
            res_clear = query_clear->exec(sql_clear);
            query_clear->next();
            delete query_clear;

            // запрос данных
            sql  =  "select id, id_person, old_fam, old_im, old_ot, "
                    "       old_date_birth, old_plc_birth, old_enp, old_sex, old_snils "
                    "  from PERSONS_OLD "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы PERSONS_OLD",
                                     "При попытке прочитать очередную порцию данных из таблицы PERSONS_OLD произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        } else {
            // запрос данных
            sql  =  "select id, id_person, old_fam, old_im, old_ot, "
                    "       old_date_birth, old_plc_birth, old_enp, old_sex, old_snils "
                    "  from PERSONS_OLD "
                    " where _update2acc is NULL "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы PERSONS_OLD",
                                     "При попытке прочитать очередную порцию данных из таблицы PERSONS_OLD произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        }

        // заготовим переменные
        int id_old_person, id_person, old_sex;
        QString old_fam, old_im, old_ot, old_plc_birth, old_enp, old_snils;
        QDate old_date_birth;

        int n = 0;

        while (query->next()) {
            //------------------//
            // прочитаем данные //
            //------------------//
            id_old_person  = query->value( 0).toInt();
            id_person      = query->value( 1).toInt();
            old_fam        = query->value( 2).toString();
            old_im         = query->value( 3).toString();
            old_ot         = query->value( 4).toString();
            old_date_birth = query->value( 5).toDate();
            old_plc_birth  = query->value( 6).toString();
            old_enp        = query->value( 7).toString();
            old_sex        = query->value( 8).toInt();
            old_snils      = query->value( 9).toString();

            //----------------//
            // добавим запись //
            //----------------//
            if (!ui->ch_PERSONS_OLD_all->isChecked()) {
                sql_del  = "DELETE FROM PERSONS_OLD "
                           " WHERE id=" + QString::number(id_old_person) + " ; ";
                query_del = new QSqlQuery(db_accBase);
                res_del = query_del->exec(sql_del);
                /*if (!res_del) {
                    db.rollback();
                    db_accBase.rollback();
                    QMessageBox::warning(this, "Ошибка удаления старой записи в таблице PERSONS_OLD",
                                         "При попытке удалить старую запись в таблице PERSONS_OLD произошла неожиданная ошибка.\n\n"
                                         "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                    ui->bn_close->setEnabled(true);
                    ui->push_connect->setEnabled(true);
                    ui->bn_copy_data->setEnabled(true);
                    return false;
                }*/
                delete query_del;
            }

            sql_add  = "INSERT INTO PERSONS_OLD(id, id_person, old_fam, old_im, old_ot, "
                       "       old_date_birth, old_plc_birth, old_enp, old_sex, old_snils) "
                       "VALUES ("    + QString::number(id_old_person)        + " "
                                ", " + QString::number(id_person)            + " "
                                ",'" + old_fam                               + "'"
                                ",'" + old_im                                + "'"
                                ",'" + old_ot                                + "'"
                                ",'" + old_date_birth.toString("yyyy-MM-dd") + "'"
                                ",'" + old_plc_birth                         + "'"
                                ",'" + old_enp                               + "'"
                                ",'" + QString::number(old_sex)              + "'"
                                ",'" + old_snils                             + "'"
                                ") ; ";
            query_add = new QSqlQuery(db_accBase);
            res_add = query_add->exec(sql_add);
            if (!res_add) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка переноса данных таблицы PERSONS_OLD",
                                     "При попытке перенести в базу данных для отдела счетов данные таблицы PERSONS_OLD произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_add->next();

            delete query_add;

            n++;

            if ( n%cnt_step==0 ) {
                //------------------------------//
                // отображение статуса на экран //
                //------------------------------//
                QDate date2 = QDate::currentDate();
                QTime time2 = QTime::currentTime();

                int sec = time0.secsTo(time2) + date0.daysTo(date2)*24*60*60;
                int sut = sec/86400;
                sec = sec - sut*86400;
                int hou = sec/3600;
                sec = sec - hou*3600;
                int min = sec/60;
                sec = sec - min*60;
                ui->lab_time_all->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                           QString("00" + QString::number(hou)).right(2) + ":" +
                                           QString("00" + QString::number(min)).right(2) + ":" +
                                           QString("00" + QString::number(sec)).right(2) );

                sec = time.secsTo(time2) + date.daysTo(date2)*24*60*60;
                sut = sec/86400;
                sec = sec - sut*86400;
                hou = sec/3600;
                sec = sec - hou*3600;
                min = sec/60;
                sec = sec - min*60;
                ui->lab_time->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                       QString("00" + QString::number(hou)).right(2) + ":" +
                                       QString("00" + QString::number(min)).right(2) + ":" +
                                       QString("00" + QString::number(sec)).right(2) );
                date = QDate::currentDate();
                time = QTime::currentTime();

                ui->lab_count->setText(show_num(n));
                if (ui->rb_21->isChecked())  cnt_step = 21;
                else if (ui->rb_231->isChecked())  cnt_step = 231;
                else if (ui->rb_2341->isChecked())  cnt_step = 2341;
                else if (ui->rb_23451->isChecked())  cnt_step = 23451;
                else if (ui->rb_hand->isChecked())  cnt_step = ui->spin_hand->value();
                QApplication::processEvents();


                // переоткроем транзакию для сохранения обработанных данных
                db_accBase.commit();
                db.commit();
                db_accBase.transaction();
                db.transaction();
            }
        }
        delete query;
        QApplication::processEvents();

        // добавим связки
        sql_upd = "UPDATE PERSONS_OLD "
                  "   set _update2acc = 2 "
                  " where _update2acc is NULL ";
        query_upd = new QSqlQuery(db);
        res_upd = query_upd->exec(sql_upd);
        if (!res_upd) {
            db.rollback();
            db_accBase.rollback();
            QMessageBox::warning(this, "Ошибка отметки статуса таблицы PERSONS_OLD",
                                 "При попытке пометить данные таблицы PERSONS_OLD как успешно перенесённые произошла неожиданная ошибка.\n\n"
                                 "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_copy_data->setEnabled(true);
            return false;
        } else  {
            db.commit();
        }

        db_accBase.commit();
        ui->te_log->setText(ui->te_log->toPlainText() + " - ok (" + show_num(n) + ")");


        if (ui->ch_PERSONS_OLD_clear->isChecked()) {
            // ------------------------ //
            // Удалим устаревшие записи //
            // ------------------------ //
            db.transaction();
            db_accBase.transaction();

            // Cначала очистим таблицу _IDS_TAB
            sql_del = "DELETE FROM \"_IDS_TAB\" ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка при очистке таблицы _IDS_TAB",
                                     "При попытке очистить таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;
            // Зальём в таблицу _IDS_TAB идентификаторы записей, которые есть в исходной базе...
            sql  =  "select id "
                    "  from PERSONS_OLD ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                delete query;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения ID из таблицы PERSONS_OLD",
                                     "При попытке прочитать действующие ID из таблицы PERSONS_OLD произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            int cntr = 0;
            while (query->next()) {
                int id = query->value(0).toInt();
                sql_add = "INSERT INTO \"_IDS_TAB\" (ID) "
                          "VALUES (" + QString::number(id) + ") ; ";
                query_add = new QSqlQuery(db_accBase);
                {
                    res_add = query_add->exec(sql_add);
                    if (!res_add) {
                        delete query_add;
                        delete query;
                        db.rollback();
                        db_accBase.commit();
                        db_accBase.rollback();
                        QMessageBox::warning(this, "Ошибка добавления записи в таблицу _IDS_TAB",
                                             "При попытке добавить запись в таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                             "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                        ui->bn_close->setEnabled(true);
                        ui->push_connect->setEnabled(true);
                        ui->bn_copy_data->setEnabled(true);
                        return false;
                    }
                } while (!res_add);
                delete query_add;

                cntr++;
                if (cntr%2341==0) {
                    db_accBase.commit();
                    db_accBase.transaction();
                    ui->lab_count->setText("IDs  " + show_num(cntr));
                    QApplication::processEvents();
                }
            }
            db_accBase.commit();
            db_accBase.transaction();
            ui->lab_count->setText("IDs  " + show_num(cntr));
            QApplication::processEvents();
            delete query;
            // ...посчитаем число ID которых нет в загруженной таблице _IDS_TAB
            sql_cnt = "SELECT count(*)"
                      "  FROM PERSONS_OLD a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_cnt = new QSqlQuery(db_accBase);
            res_cnt = query_cnt->exec(sql_cnt);
            if (!res_cnt) {
                delete query_cnt;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из PERSONS_OLD",
                                     "При попытке удалить устаревшие записи из PERSONS_OLD произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_cnt->next();
            int cnt_to_delete = query_cnt->value(0).toInt();
            delete query_cnt;
            // ...и удалим записи, ID которых нет в загруженной таблице _IDS_TAB
            sql_del = "DELETE FROM PERSONS_OLD a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из PERSONS_OLD",
                                     "При попытке удалить устаревшие записи из PERSONS_OLD произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;

            db.commit();
            db_accBase.commit();
            ui->te_log->setText(ui->te_log->toPlainText() + " - удалено " + show_num(cnt_to_delete) + " устаревших записей");
        }


        ui->ch_PERSONS_OLD->setChecked(false);
        ui->ch_PERSONS_OLD_clear->setChecked(false);
        ui->ch_PERSONS_OLD_all->setChecked(false);
    }


    /////////////////////////
    // // PERSONS_DOCS // //
    /////////////////////////
    if (ui->ch_PERSONS_DOCS->isChecked()) {
        ui->lab_table->setText("PERSONS_DOCS");
        ui->te_log->append("PERSONS_DOCS");
        QApplication::processEvents();

        db_accBase.transaction();
        db.transaction();

        if (ui->ch_PERSONS_DOCS_all->isChecked()) {
            // почистим таблицу
            sql_clear = "DELETE FROM PERSONS_DOCS ; ";
            query_clear = new QSqlQuery(db_accBase);
            res_clear = query_clear->exec(sql_clear);
            query_clear->next();
            delete query_clear;

            // запрос данных
            sql  =  "select id, id_person, doc_type, status, "
                    "       doc_ser, doc_num, doc_org, org_code, "
                    "       doc_date, doc_exp "
                    "  from PERSONS_DOCS "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы PERSONS_DOCS",
                                     "При попытке прочитать очередную порцию данных из таблицы PERSONS_DOCS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        } else {
            // запрос данных
            sql  =  "select id, id_person, doc_type, status, "
                    "       doc_ser, doc_num, doc_org, org_code, "
                    "       doc_date, doc_exp, dt_ins, dt_upd "
                    "  from PERSONS_DOCS "
                    " where _update2acc is NULL "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы PERSONS_DOCS",
                                     "При попытке прочитать очередную порцию данных из таблицы PERSONS_DOCS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        }

        // заготовим переменные
        int id_doc, id_person, doc_type, status;
        QString doc_ser, doc_num, doc_org, org_code;
        QDate doc_date, doc_exp;
        QDateTime dt_ins, dt_upd;

        int n = 0;

        while (query->next()) {
            //------------------//
            // прочитаем данные //
            //------------------//
            id_doc    = query->value( 0).toInt();
            id_person = query->value( 1).toInt();
            doc_type  = query->value( 2).toInt();
            status    = query->value( 3).toInt();
            doc_ser   = query->value( 4).toString();
            doc_num   = query->value( 5).toString();
            doc_org   = query->value( 6).toString();
            org_code  = query->value( 7).toString();
            doc_date  = query->value( 8).toDate();
            doc_exp   = query->value( 9).toDate();
            dt_ins    = query->value(10).toDateTime();
            dt_upd    = query->value(11).toDateTime();

            //----------------//
            // добавим запись //
            //----------------//
            if (!ui->ch_PERSONS_DOCS_all->isChecked()) {
                sql_del  = "DELETE FROM PERSONS_DOCS "
                           " WHERE id=" + QString::number(id_doc) + " ; ";
                query_del = new QSqlQuery(db_accBase);
                res_del = query_del->exec(sql_del);
                if (!res_del) {
                    db.rollback();
                    db_accBase.rollback();
                    QMessageBox::warning(this, "Ошибка удаления старой записи в таблице PERSONS_DOCS",
                                         "При попытке удалить старую запись в таблице PERSONS_DOCS произошла неожиданная ошибка.\n\n"
                                         "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                    ui->bn_close->setEnabled(true);
                    ui->push_connect->setEnabled(true);
                    ui->bn_copy_data->setEnabled(true);
                    return false;
                }
                delete query_del;
            }

            sql_add  = "INSERT INTO PERSONS_DOCS(id, id_person, doc_type, status, "
                       "       doc_ser, doc_num, doc_org, org_code, "
                       "       doc_date, doc_exp, dt_ins, dt_upd) "
                       "VALUES ("    + QString::number(id_doc)    + " "
                                ", " + QString::number(id_person) + " "
                                ", " + QString::number(doc_type)  + " "
                                ", " + QString::number(status)    + " "
                                ",'" + doc_ser                    + "'"
                                ",'" + doc_num                    + "'"
                                ",'" + doc_org                    + "'"
                                ",'" + org_code                   + "'"
                                ", " + ( doc_date.isNull() ? "NULL" : ("'" + doc_date.toString("yyyy-MM-dd") + "'") ) + " "
                                ", " + ( doc_exp.isNull()  ? "NULL" : ("'" + doc_exp.toString("yyyy-MM-dd")  + "'") ) + " "
                                ", " + ( dt_ins.isNull()   ? "NULL" : ("'" + dt_ins.toString("yyyy-MM-dd hh:mm:ss") + "'") ) + " "
                                ", " + ( dt_upd.isNull()   ? "NULL" : ("'" + dt_upd.toString("yyyy-MM-dd hh:mm:ss") + "'") ) + " "
                                ") ; ";
            query_add = new QSqlQuery(db_accBase);
            res_add = query_add->exec(sql_add);
            if (!res_add) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка переноса данных таблицы PERSONS_DOCS",
                                     "При попытке перенести в базу данных для отдела счетов данные таблицы PERSONS_DOCS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_add->next();

            delete query_add;

            n++;

            if ( n%cnt_step==0 ) {
                //------------------------------//
                // отображение статуса на экран //
                //------------------------------//
                QDate date2 = QDate::currentDate();
                QTime time2 = QTime::currentTime();

                int sec = time0.secsTo(time2) + date0.daysTo(date2)*24*60*60;
                int sut = sec/86400;
                sec = sec - sut*86400;
                int hou = sec/3600;
                sec = sec - hou*3600;
                int min = sec/60;
                sec = sec - min*60;
                ui->lab_time_all->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                           QString("00" + QString::number(hou)).right(2) + ":" +
                                           QString("00" + QString::number(min)).right(2) + ":" +
                                           QString("00" + QString::number(sec)).right(2) );

                sec = time.secsTo(time2) + date.daysTo(date2)*24*60*60;
                sut = sec/86400;
                sec = sec - sut*86400;
                hou = sec/3600;
                sec = sec - hou*3600;
                min = sec/60;
                sec = sec - min*60;
                ui->lab_time->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                       QString("00" + QString::number(hou)).right(2) + ":" +
                                       QString("00" + QString::number(min)).right(2) + ":" +
                                       QString("00" + QString::number(sec)).right(2) );
                date = QDate::currentDate();
                time = QTime::currentTime();

                ui->lab_count->setText(show_num(n));
                if (ui->rb_21->isChecked())  cnt_step = 21;
                else if (ui->rb_231->isChecked())  cnt_step = 231;
                else if (ui->rb_2341->isChecked())  cnt_step = 2341;
                else if (ui->rb_23451->isChecked())  cnt_step = 23451;
                else if (ui->rb_hand->isChecked())  cnt_step = ui->spin_hand->value();
                QApplication::processEvents();


                // переоткроем транзакию для сохранения обработанных данных
                db_accBase.commit();
                db.commit();
                db_accBase.transaction();
                db.transaction();
            }
        }
        delete query;
        QApplication::processEvents();

        // добавим связки
        sql_upd = "UPDATE PERSONS_DOCS "
                  "   set _update2acc = 2 "
                  " where _update2acc is NULL ";
        query_upd = new QSqlQuery(db);
        res_upd = query_upd->exec(sql_upd);
        if (!res_upd) {
            db.rollback();
            db_accBase.rollback();
            QMessageBox::warning(this, "Ошибка отметки статуса таблицы PERSONS_DOCS",
                                 "При попытке пометить данные таблицы PERSONS_DOCS как успешно перенесённые произошла неожиданная ошибка.\n\n"
                                 "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

            ui->bn_close->setEnabled(true);
            ui->push_connect->setEnabled(true);
            ui->bn_copy_data->setEnabled(true);
            return false;
        } else  {
            db.commit();
        }

        db_accBase.commit();
        ui->te_log->setText(ui->te_log->toPlainText() + " - ok (" + show_num(n) + ")");


        if (ui->ch_PERSONS_DOCS_clear->isChecked()) {
            // ------------------------ //
            // Удалим устаревшие записи //
            // ------------------------ //
            db.transaction();
            db_accBase.transaction();

            // Cначала очистим таблицу _IDS_TAB
            sql_del = "DELETE FROM \"_IDS_TAB\" ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка при очистке таблицы _IDS_TAB",
                                     "При попытке очистить таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;
            // Зальём в таблицу _IDS_TAB идентификаторы записей, которые есть в исходной базе...
            sql  =  "select id "
                    "  from PERSONS_DOCS ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                delete query;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения ID из таблицы PERSONS_DOCS",
                                     "При попытке прочитать действующие ID из таблицы PERSONS_DOCS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            int cntr = 0;
            while (query->next()) {
                int id = query->value(0).toInt();
                sql_add = "INSERT INTO \"_IDS_TAB\" (ID) "
                          "VALUES (" + QString::number(id) + ") ; ";
                query_add = new QSqlQuery(db_accBase);
                {
                    res_add = query_add->exec(sql_add);
                    if (!res_add) {
                        delete query_add;
                        delete query;
                        db.rollback();
                        db_accBase.commit();
                        db_accBase.rollback();
                        QMessageBox::warning(this, "Ошибка добавления записи в таблицу _IDS_TAB",
                                             "При попытке добавить запись в таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                             "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                        ui->bn_close->setEnabled(true);
                        ui->push_connect->setEnabled(true);
                        ui->bn_copy_data->setEnabled(true);
                        return false;
                    }
                } while (!res_add);
                delete query_add;

                cntr++;
                if (cntr%2341==0) {
                    db_accBase.commit();
                    db_accBase.transaction();
                    ui->lab_count->setText("IDs  " + show_num(cntr));
                    QApplication::processEvents();
                }
            }
            db_accBase.commit();
            db_accBase.transaction();
            ui->lab_count->setText("IDs  " + show_num(cntr));
            QApplication::processEvents();
            delete query;
            // ...посчитаем число ID которых нет в загруженной таблице _IDS_TAB
            sql_cnt = "SELECT count(*)"
                      "  FROM PERSONS_DOCS a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_cnt = new QSqlQuery(db_accBase);
            res_cnt = query_cnt->exec(sql_cnt);
            if (!res_cnt) {
                delete query_cnt;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из PERSONS_DOCS",
                                     "При попытке удалить устаревшие записи из PERSONS_DOCS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_cnt->next();
            int cnt_to_delete = query_cnt->value(0).toInt();
            delete query_cnt;
            // ...и удалим записи, ID которых нет в загруженной таблице _IDS_TAB
            sql_del = "DELETE FROM PERSONS_DOCS a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из PERSONS_DOCS",
                                     "При попытке удалить устаревшие записи из PERSONS_DOCS произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;

            db.commit();
            db_accBase.commit();
            ui->te_log->setText(ui->te_log->toPlainText() + " - удалено " + show_num(cnt_to_delete) + " устаревших записей");
        }


        ui->ch_PERSONS_DOCS->setChecked(false);
        ui->ch_PERSONS_DOCS_clear->setChecked(false);
        ui->ch_PERSONS_DOCS_all->setChecked(false);
    }


    /////////////////////////
    // // PERSONS_ASSIG // //
    /////////////////////////
    if (ui->ch_PERSONS_ASSIG->isChecked()) {
        ui->lab_table->setText("PERSONS_ASSIG");
        ui->te_log->append("PERSONS_ASSIG");
        QApplication::processEvents();

        db_accBase.transaction();
        db.transaction();

        if (ui->ch_PERSONS_ASSIG_all->isChecked()) {
            // почистим таблицу
            sql_clear = "DELETE FROM PERSONS_ASSIG ; ";
            query_clear = new QSqlQuery(db_accBase);
            res_clear = query_clear->exec(sql_clear);
            query_clear->next();
            delete query_clear;

            // запрос данных
            sql  =  "select id, id_person, assig_type, assig_way, is_actual, status, "
                    "       code_mo, code_mp, code_ms, snils_mt, "
                    "       assig_date, unsig_date "
                    "  from PERSONS_ASSIG "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы PERSONS_ASSIG",
                                     "При попытке прочитать очередную порцию данных из таблицы PERSONS_ASSIG произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        } else {
            // запрос данных
            sql  =  "select id, id_person, assig_type, assig_way, is_actual, status, "
                    "       code_mo, code_mp, code_ms, snils_mt, "
                    "       assig_date, unsig_date "
                    "  from PERSONS_ASSIG "
                    " where _update2acc is NULL "
                    " order by id ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения из таблицы PERSONS_ASSIG",
                                     "При попытке прочитать очередную порцию данных из таблицы PERSONS_ASSIG произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
        }

        // заготовим переменные
        int id_assig, id_person, assig_type, assig_way, is_actual, status;
        QString code_mo, code_mp, code_ms, snils_mt;
        QDate assig_date, unsig_date;
        QDateTime dt_ins, dt_upd;

        int n = 0;

        while (query->next()) {
            //------------------//
            // прочитаем данные //
            //------------------//
            id_assig     = query->value( 0).toInt();
            id_person    = query->value( 1).toInt();
            assig_type   = query->value( 2).toInt();
            assig_way    = query->value( 3).toInt();
            is_actual    = query->value( 4).toInt();
            status       = query->value( 5).toInt();
            code_mo      = query->value( 6).toString();
            code_mp      = query->value( 7).toString();
            code_ms      = query->value( 8).toString();
            snils_mt     = query->value( 9).toString();
            assig_date   = query->value(10).toDate();
            unsig_date   = query->value(11).toDate();
            dt_ins       = query->value(12).toDateTime();
            dt_upd       = query->value(13).toDateTime();

            //----------------//
            // добавим запись //
            //----------------//
            if (!ui->ch_PERSONS_ASSIG_all->isChecked()) {
                sql_del  = "DELETE FROM PERSONS_ASSIG "
                           " WHERE id=" + QString::number(id_assig) + " ; ";
                query_del = new QSqlQuery(db_accBase);
                res_del = query_del->exec(sql_del);
                if (!res_del) {
                    db.rollback();
                    db_accBase.rollback();
                    QMessageBox::warning(this, "Ошибка удаления старой записи в таблице PERSONS_ASSIG",
                                         "При попытке удалить старую запись в таблице PERSONS_ASSIG произошла неожиданная ошибка.\n\n"
                                         "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                    ui->bn_close->setEnabled(true);
                    ui->push_connect->setEnabled(true);
                    ui->bn_copy_data->setEnabled(true);
                    return false;
                }
                delete query_del;
            }

            sql_add  = "INSERT INTO PERSONS_ASSIG "
                       "       ( id, id_person, assig_type, assig_way, is_actual, status, "
                       "         code_mo, code_mp, code_ms, snils_mt, "
                       "         assig_date, unsig_date, dt_ins, dt_upd ) "
                       "VALUES ("    + QString::number(id_assig)   + " "
                                ", " + QString::number(id_person)  + " "
                                ", " + QString::number(assig_type) + " "
                                ", " + QString::number(assig_way)  + " "
                                ", " + QString::number(is_actual)  + " "
                                ", " + QString::number(status)     + " "
                                ",'" + code_mo                     + "'"
                                ",'" + code_mp                     + "'"
                                ",'" + code_ms                     + "'"
                                ",'" + snils_mt                    + "'"
                                ", " + ( assig_date.isNull() ? "NULL"  : ("'" + assig_date.toString("yyyy-MM-dd") + "'") ) + " "
                                ", " + ( unsig_date.isNull() ? "NULL"  : ("'" + unsig_date.toString("yyyy-MM-dd") + "'") ) + " "
                                ", " + ( dt_ins.isNull() ? "NULL" : ("'" + dt_ins.toString("yyyy-MM-dd hh:mm:ss") + "'") ) + " "
                                ", " + ( dt_upd.isNull() ? "NULL" : ("'" + dt_upd.toString("yyyy-MM-dd hh:mm:ss") + "'") ) + " "
                                ") ; ";
            query_add = new QSqlQuery(db_accBase);
            res_add = query_add->exec(sql_add);
            if (!res_add) {
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка переноса данных таблицы PERSONS_ASSIG",
                                     "При попытке перенести в базу данных для отдела счетов данные таблицы PERSONS_ASSIG произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_add;

            // добавим связки
            sql_upd = "UPDATE PERSONS_ASSIG "
                      "   set _update2acc = 2 "
                      " where ID=" + QString::number(id_assig) + " ; ";
            query_upd = new QSqlQuery(db);
            res_upd = query_upd->exec(sql_upd);
            delete query_upd;

            n++;

            if ( n%cnt_step==0 ) {
                db.commit();
                db.transaction();
                db_accBase.commit();
                db_accBase.transaction();

                //------------------------------//
                // отображение статуса на экран //
                //------------------------------//
                QDate date2 = QDate::currentDate();
                QTime time2 = QTime::currentTime();

                int sec = time0.secsTo(time2) + date0.daysTo(date2)*24*60*60;
                int sut = sec/86400;
                sec = sec - sut*86400;
                int hou = sec/3600;
                sec = sec - hou*3600;
                int min = sec/60;
                sec = sec - min*60;
                ui->lab_time_all->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                           QString("00" + QString::number(hou)).right(2) + ":" +
                                           QString("00" + QString::number(min)).right(2) + ":" +
                                           QString("00" + QString::number(sec)).right(2) );

                sec = time.secsTo(time2) + date.daysTo(date2)*24*60*60;
                sut = sec/86400;
                sec = sec - sut*86400;
                hou = sec/3600;
                sec = sec - hou*3600;
                min = sec/60;
                sec = sec - min*60;
                ui->lab_time->setText( (sut>0 ? (QString::number(sut)+"d") : " ") +
                                       QString("00" + QString::number(hou)).right(2) + ":" +
                                       QString("00" + QString::number(min)).right(2) + ":" +
                                       QString("00" + QString::number(sec)).right(2) );
                date = QDate::currentDate();
                time = QTime::currentTime();

                ui->lab_count->setText(show_num(n));
                if (ui->rb_21->isChecked())  cnt_step = 21;
                else if (ui->rb_231->isChecked())  cnt_step = 231;
                else if (ui->rb_2341->isChecked())  cnt_step = 2341;
                else if (ui->rb_23451->isChecked())  cnt_step = 23451;
                else if (ui->rb_hand->isChecked())  cnt_step = ui->spin_hand->value();
                QApplication::processEvents();

            }
        }
        delete query;
        QApplication::processEvents();

        db.commit();
        db.transaction();
        db_accBase.commit();
        db_accBase.transaction();

        ui->te_log->setText(ui->te_log->toPlainText() + " - ok (" + show_num(n) + ")");


        if (ui->ch_PERSONS_ASSIG_clear->isChecked()) {
            // ------------------------ //
            // Удалим устаревшие записи //
            // ------------------------ //
            db.transaction();
            db_accBase.transaction();

            // Cначала очистим таблицу _IDS_TAB
            sql_del = "DELETE FROM \"_IDS_TAB\" ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка при очистке таблицы _IDS_TAB",
                                     "При попытке очистить таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;
            // Зальём в таблицу _IDS_TAB идентификаторы записей, которые есть в исходной базе...
            sql  =  "select id "
                    "  from PERSONS_ASSIG ; ";
            query = new QSqlQuery(db);
            res = query->exec(sql);
            if (!res) {
                delete query;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка чтения ID из таблицы PERSONS_ASSIG",
                                     "При попытке прочитать действующие ID из таблицы PERSONS_ASSIG произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            int cntr = 0;
            while (query->next()) {
                int id = query->value(0).toInt();
                sql_add = "INSERT INTO \"_IDS_TAB\" (ID) "
                          "VALUES (" + QString::number(id) + ") ; ";
                query_add = new QSqlQuery(db_accBase);
                {
                    res_add = query_add->exec(sql_add);
                    if (!res_add) {
                        delete query_add;
                        delete query;
                        db.rollback();
                        db_accBase.commit();
                        db_accBase.rollback();
                        QMessageBox::warning(this, "Ошибка добавления записи в таблицу _IDS_TAB",
                                             "При попытке добавить запись в таблицу _IDS_TAB произошла неожиданная ошибка.\n\n"
                                             "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                        ui->bn_close->setEnabled(true);
                        ui->push_connect->setEnabled(true);
                        ui->bn_copy_data->setEnabled(true);
                        return false;
                    }
                } while (!res_add);
                delete query_add;

                cntr++;
                if (cntr%2341==0) {
                    db_accBase.commit();
                    db_accBase.transaction();
                    ui->lab_count->setText("IDs  " + show_num(cntr));
                    QApplication::processEvents();
                }
            }
            db_accBase.commit();
            db_accBase.transaction();
            ui->lab_count->setText("IDs  " + show_num(cntr));
            QApplication::processEvents();
            delete query;
            // ...посчитаем число ID которых нет в загруженной таблице _IDS_TAB
            sql_cnt = "SELECT count(*)"
                      "  FROM PERSONS_ASSIG a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_cnt = new QSqlQuery(db_accBase);
            res_cnt = query_cnt->exec(sql_cnt);
            if (!res_cnt) {
                delete query_cnt;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из PERSONS_ASSIG",
                                     "При попытке удалить устаревшие записи из PERSONS_ASSIG произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            query_cnt->next();
            int cnt_to_delete = query_cnt->value(0).toInt();
            delete query_cnt;
            // ...и удалим записи, ID которых нет в загруженной таблице _IDS_TAB
            sql_del = "DELETE FROM PERSONS_ASSIG a "
                      " WHERE a.id NOT IN(SELECT i.id FROM \"_IDS_TAB\" i) ; ";
            query_del = new QSqlQuery(db_accBase);
            res_del = query_del->exec(sql_del);
            if (!res_del) {
                delete query_del;
                db.rollback();
                db_accBase.rollback();
                QMessageBox::warning(this, "Ошибка удаления устаревших записей из PERSONS_ASSIG",
                                     "При попытке удалить устаревшие записи из PERSONS_ASSIG произошла неожиданная ошибка.\n\n"
                                     "Загрузка данных во внешнюю базу для отдела счетов остановлена!");

                ui->bn_close->setEnabled(true);
                ui->push_connect->setEnabled(true);
                ui->bn_copy_data->setEnabled(true);
                return false;
            }
            delete query_del;

            db.commit();
            db_accBase.commit();
            ui->te_log->setText(ui->te_log->toPlainText() + " - удалено " + show_num(cnt_to_delete) + " устаревших записей");
        }


        ui->ch_PERSONS_ASSIG->setChecked(false);
        ui->ch_PERSONS_ASSIG_clear->setChecked(false);
        ui->ch_PERSONS_ASSIG_all->setChecked(false);
    }

    ui->bn_close->setEnabled(true);
    ui->push_connect->setEnabled(true);
    ui->bn_copy_data->setEnabled(true);
    return true;
}
