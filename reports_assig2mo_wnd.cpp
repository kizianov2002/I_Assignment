#include "reports_assig2mo_wnd.h"
#include "ui_reports_assig2mo_wnd.h"

#include "ki_funcs.h"

#include <QMessageBox>
#include <QFileDialog>

reports_assig2mo_wnd::reports_assig2mo_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::reports_assig2mo_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    import4csv_w = NULL;

    l41.clear();
    l41.append(400);
    l41.append(100);

    l51.clear();
    l51.append(300);
    l51.append(100);

    l01.clear();
    l01.append(0);
    l01.append(100);

    ui->split_log->setSizes(l51);
    ui->split_medorgs->setSizes(l41);

    ui->ch_live4terr->setChecked(false);
    ui->ch_assig->setChecked(false);
    ui->ch_analize->setChecked(false);
    ui->ch_other_files->setChecked(false);
    on_ch_date_rep_clicked(false);
    on_ch_assig_clicked(false);
    on_ch_analize_clicked(false);
    on_ch_other_files_clicked(false);
    ui->date_rep->setDate(QDate::currentDate());

    time_start = QDateTime();

    refresh_combo_data_source();
    refresh_medorgs_tab();
    refresh_analize_tab();
}

reports_assig2mo_wnd::~reports_assig2mo_wnd() {
    delete ui;
}

void reports_assig2mo_wnd::on_bn_close_clicked() {
    close();
}


void reports_assig2mo_wnd::refresh_combo_data_source() {
    this->setCursor(Qt::WaitCursor);

    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select id, layer_name, layer_date "
                  "  from assig.layers "
                  " where layer_type = 2 "
                  " order by layer_date DESC ; ";
    query->exec(sql);

    ui->combo_data_source->clear();
    ui->combo_data_source->addItem(" - Основной массив данных прикрепления - ", -1);
    while (query->next()) {
        int     id_layer   = query->value(0).toInt();
        QString layer_name = query->value(1).toString();
        ui->combo_data_source->addItem(layer_name, id_layer);
    }
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void reports_assig2mo_wnd::on_ch_live4terr_clicked(bool checked) {
    ui->pan_live4terr->setEnabled(checked);
    ui->ch_live4terr_one4terr->setEnabled(checked);
    ui->ch_live4terr_one4req->setEnabled(checked);
    ui->ch_live4terr_other4terr->setEnabled(checked);
    ui->ch_live4terr_other4req->setEnabled(checked);
    /*if (checked) {
        if (QMessageBox::question(this, "Нужно подтверждение",
                                  "Выборка списка застрахованных, проживающих на территории МО\n - это долгая операция\nПродолжить?\n",
                                  QMessageBox::Yes|QMessageBox::Cancel)==QMessageBox::Cancel) {
            ui->ch_live4terr->setChecked(false);
            ui->pan_live4terr->setEnabled(false);
            ui->ch_live4terr_one4terr->setEnabled(false);
            ui->ch_live4terr_one4req->setEnabled(false);
            ui->ch_live4terr_other4terr->setEnabled(false);
            ui->ch_live4terr_other4req->setEnabled(false);
        }
    }*/
}

void reports_assig2mo_wnd::on_ch_live4terr_one4terr_clicked(bool checked) {
    if ( !ui->ch_live4terr_one4terr->isChecked()
         && !ui->ch_live4terr_one4req->isChecked()
         && !ui->ch_live4terr_other4terr->isChecked()
         && !ui->ch_live4terr_other4req->isChecked()) {
        ui->ch_live4terr_one4terr->setChecked(true);
    }
}
void reports_assig2mo_wnd::on_ch_live4terr_one4req_clicked(bool checked) {
    if ( !ui->ch_live4terr_one4terr->isChecked()
         && !ui->ch_live4terr_one4req->isChecked()
         && !ui->ch_live4terr_other4terr->isChecked()
         && !ui->ch_live4terr_other4req->isChecked()) {
        ui->ch_live4terr_one4terr->setChecked(true);
    }
}
void reports_assig2mo_wnd::on_ch_live4terr_other4terr_clicked(bool checked) {
    if ( !ui->ch_live4terr_one4terr->isChecked()
         && !ui->ch_live4terr_one4req->isChecked()
         && !ui->ch_live4terr_other4terr->isChecked()
         && !ui->ch_live4terr_other4req->isChecked()) {
        ui->ch_live4terr_one4terr->setChecked(true);
    }
}
void reports_assig2mo_wnd::on_ch_live4terr_other4req_clicked(bool checked) {
    if ( !ui->ch_live4terr_one4terr->isChecked()
         && !ui->ch_live4terr_one4req->isChecked()
         && !ui->ch_live4terr_other4terr->isChecked()
         && !ui->ch_live4terr_other4req->isChecked()) {
        ui->ch_live4terr_one4terr->setChecked(true);
    }
}

void reports_assig2mo_wnd::on_ch_assig_clicked(bool checked) {
    ui->lab_RData->setVisible(checked);
    ui->lab_RSource->setVisible(checked);
    ui->pan_RData->setVisible(checked);
    ui->pan_RSource->setVisible(checked);
}


void reports_assig2mo_wnd::on_ch_date_rep_clicked(bool checked) {
    ui->date_rep->setEnabled(checked);
    if (checked && ui->date_rep->date()<QDate(2015,1,1)) {
        ui->date_rep->setDate(QDate::currentDate());
    }
}


void reports_assig2mo_wnd::on_ch_analize_clicked(bool checked) {
    ui->lab_analize->setVisible(checked);
    ui->pan_analize->setVisible(checked);
}

void reports_assig2mo_wnd::on_ch_other_files_clicked(bool checked) {
    ui->lab_other_files->setVisible(checked);
}

void reports_assig2mo_wnd::refresh_analize_tab() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Нет доступа к базе данных", "Потерян доступ к файлу базы данных !");
        return;
    }
    this->setCursor(Qt::WaitCursor);

    QString query_str = "select * from assig.analize_assigs_auto ORDER BY fam, im, ot, date_birth ";

    model_analize_tab.setQuery(query_str,db);
    //QString err2 = model_analize_tab.lastError().driverText();

    // подключаем модель из БД
    ui->tab_analize->setModel(&model_analize_tab);

    // обновляем таблицу
    ui->tab_analize->reset();

    QString sql_cnt = "select count(*) from assig.analize_assigs_auto  ; ";
    QSqlQuery *query_cnt = new QSqlQuery(db);
    query_cnt->exec(sql_cnt);
    query_cnt->next();
    int cnt = query_cnt->value(0).toInt();
    ui->lab_analize_cnt->setText(QString::number(cnt));

    // задаём ширину колонок
    ui->tab_analize->setColumnWidth( 0, 1);
    /*ui->tab_medorg_addresses->setColumnWidth( 1,  1);     // code_terr,
    ui->tab_medorg_addresses->setColumnWidth( 2,180);     // terr,
    ui->tab_medorg_addresses->setColumnWidth( 3,  1);     // code_dstr,
    ui->tab_medorg_addresses->setColumnWidth( 4,180);     // dstr,
    ui->tab_medorg_addresses->setColumnWidth(12,100);     // for_age
    ui->tab_medorg_addresses->setColumnWidth(13,  1);     // id

    // правим заголовки
    model_medorg_addresses.setHeaderData( 0, Qt::Horizontal, "код МО");
    model_medorg_addresses.setHeaderData( 2, Qt::Horizontal, "регион");
    model_medorg_addresses.setHeaderData( 4, Qt::Horizontal, "район");
    model_medorg_addresses.setHeaderData( 6, Qt::Horizontal, "нас.пункт");
    model_medorg_addresses.setHeaderData( 8, Qt::Horizontal, "улица");
    model_medorg_addresses.setHeaderData(12, Qt::Horizontal, "возрастная\nкатегория");*/
    ui->tab_analize->repaint();

    this->setCursor(Qt::ArrowCursor);
}


void reports_assig2mo_wnd::on_line_file_csv_textChanged(const QString &arg1) {
    // проверим, есть ли этот файл
    QFile csv_file(arg1);
    if (csv_file.open(QIODevice::ReadOnly)) {
        csv_file.close();
        ui->bn_import_csv->setEnabled(true);
        //ui->tab_analize->setEnabled(true);
    } else {
        ui->bn_import_csv->setEnabled(false);
        //ui->tab_analize->setEnabled(false);
    }
    refresh_analize_tab();
}

void reports_assig2mo_wnd::on_bn_file_csv_clicked() {
    ui->line_file_csv->setText(QFileDialog::getOpenFileName(this, "Выберите CSV-файл данных из МО",
                                                              ui->line_file_csv->text(), "Таблично-текстовый файл (*.txt *.csv)"));
    on_line_file_csv_textChanged(ui->line_file_csv->text());
}


void reports_assig2mo_wnd::on_bn_import_csv_clicked() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Нет доступа к базе данных", "Потерян доступ к файлу базы данных !");
        return;
    }
    this->setCursor(Qt::WaitCursor);

    // данные прикрепления
    s_data_import data;
    data.file_name = ui->line_file_csv->text();
    data.tab_name = "assig.analize_assigs_auto";

    delete import4csv_w;
    import4csv_w = new import4csv_wnd(db, data_app, data, this);
    import4csv_w->exec();

    refresh_analize_tab();
    this->setCursor(Qt::ArrowCursor);
}

void reports_assig2mo_wnd::on_bn_other_files_clicked() {
    ui->line_other_files->setText(QFileDialog::getExistingDirectory(this, "Выберите папку писем и дополнительных данных", ui->line_other_files->text()));
}
void reports_assig2mo_wnd::on_bn_gen_dir_clicked() {
    ui->line_gen_dir->setText(QFileDialog::getExistingDirectory(this, "Выберите папку для сохранения данных", ui->line_gen_dir->text()));
}





#include "qt_windows.h"
#include "qwindowdefs_win.h"
#include <shellapi.h>

bool reports_assig2mo_wnd::rep_MO_assig ( QString code_mo, QString tab_name, int assig_type, int assig_age,
                                          bool gen_terr, bool gen_live4terr, bool f_live4terr_one4terr, bool f_live4terr_one4req, bool f_live4terr_other4terr, bool f_live4terr_other4req,
                                          bool f_address, bool f_phone, bool f_date_rep, QDate date_rep,
                                          bool gen_addresses, bool gen_assig, bool gen_analize, bool f_other_files, bool f_append_arch,
                                          QString dir_Files, QString dir_GenDir ) {
    this->setCursor(Qt::WaitCursor);

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

    QDir gen_dir(dir_GenDir);
    if (!gen_dir.exists(dir_GenDir)) {
        gen_dir.mkpath(dir_GenDir);
    }
    // подготовим названия файлов
    QString fname_arch = dir_GenDir + "/" + QDate::currentDate().toString("yyyy-MM-dd") + "  -  " + data_app.smo_short + " - " + data_app.filial_name + "  -  " + code_mo + " - mo_assig_data.zip";
    fname_arch = fname_arch.replace("/","\\");
    if ( !f_append_arch     // если архив надо дополнять
         && QFile::exists(fname_arch) ) {
        while (!QFile::remove(fname_arch))
            QMessageBox::warning(this,"Не удалось удалить старый файл", "Не удалось удалить старый файл\n" + fname_arch);
    }

    QProcess *myProcess = new QProcess(NULL);

    // readme.txt - добавим в архив
    QString com = QString("D:/INKO/7-Zip/7z.exe a -tzip \"") + fname_arch + "\" \"" + dir_GenDir + "\\readme.txt" + "\" -mx9";
    int res_com = 0;
    while ((res_com = myProcess->execute(com))!=0) {
        if (QMessageBox::warning(this,
                                 "Ошибка при создании архива",
                                 "Некоторые из файлов отчёта не удалось добавить в отправляемый архив.\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки D:\\TEMP\\\n\nПовторить попытку?",
                                 QMessageBox::Yes|QMessageBox::Abort,
                                 QMessageBox::Abort)==QMessageBox::Abort) {
            break;
        }
    }



    // запись в лог
    QDateTime time_now = QDateTime::currentDateTime();
    QString log_str = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +
                      " - Выгрузка территории обслуживания МО " + code_mo + " :";
    ui->te_log->append(log_str);
    QApplication::processEvents();


    // ---------------------------------- //
    // ---------------------------------- //
    //        формирование данных         //
    // ---------------------------------- //
    // ---------------------------------- //

    db.transaction();

    // запрос территории МО
    QString msg_terr = "Выгрузка территории обслуживания МО - " + code_mo;
    QString fname_terr = dir_GenDir + "/" + code_mo + " - MO_terr.csv";

    if (gen_terr) {
        // ------------------------------
        // проживающие на территории МО
        // ------------------------------
        QString sql_terr = "";

        // найдём ID персон, на этой территории
        sql_terr =  "select distinct ta.ocato, /*coalesce(s.aoid, n.aoid, c.aoid, d.aoid, t.aoid) as aoid,*/ coalesce(s.aoguid, n.aoguid, c.aoguid, d.aoguid, t.aoguid) as aoguid, "
                    "       t.subj, t.offname||' '||t.shortname as subj_name, "
                    "       d.dstr, d.offname||' '||d.shortname as dstr_name, "
                    "       c.city, c.offname||' '||c.shortname as city_name, "
                    "       n.nasp, n.offname||' '||n.shortname as nasp_name, "
                    "       s.strt, s.offname||' '||s.shortname as strt_name, "
                    "       case when ta.house is NULL or ta.house=0 then 'все дома' else '' end as all_street, ta.house, ta.corp, "
                    "       case when ta.from_start=1 then 'с начала улицы' else '' end as from_start, "
                    "       case when ta.to_end=1 then 'до конца улицы' else '' end as to_end, "
                    "       case when ta.side=1 then 'нечётная сторона' when ta.side=2 then 'чётная сторона' else '' end as side, "
                    "       ta.code_mo, ta.code_mp, ta.code_ms, ta.snils_mt, "
                    "       case ta.age when 1 then 'дети' when 2 then 'взрослые' else 'любой возраст' end as age "
                    "  from assig_by_terr ta "
                    "  left join fias.subj t on(t.subj=ta.subj   and t.subj<>0 ) "
                    "  left join fias.dstr d on(d.subj=ta.subj /*and d.subj<>0*/ and d.dstr=ta.dstr   and d.dstr<>0 ) "
                    "  left join fias.city c on(c.subj=ta.subj /*and c.subj<>0*/ and c.dstr=ta.dstr /*and c.dstr<>0*/ and c.city=ta.city   and c.city<>0 ) "
                    "  left join fias.nasp n on(n.subj=ta.subj /*and n.subj<>0*/ and n.dstr=ta.dstr /*and n.dstr<>0*/ and n.city=ta.city /*and n.city<>0*/ and n.nasp=ta.nasp   and n.nasp<>0 ) "
                    "  left join fias.strt s on(s.subj=ta.subj /*and s.subj<>0*/ and s.dstr=ta.dstr /*and s.dstr<>0*/ and s.city=ta.city /*and s.city<>0*/ and s.nasp=ta.nasp /*and s.nasp<>0*/ and s.strt=ta.strt   and s.strt<>0  ) "
                    " where ta.code_mo in(" + codes_mo + ") "
                    //" limit 10 ; ";
                    " order by t.subj, d.dstr, c.city, n.nasp, s.strt, ta.house, ta.corp ; ";

        sql2csv(db, sql_terr, false, fname_terr, this); //, QStringList names_old=QStringList(), QStringList names_new=QStringList());

        // добавим в архив
        QString com = QString("D:/INKO/7-Zip/7z.exe a -tzip \"") + fname_arch + "\" \"" + fname_terr + "\" -mx9";
        int res_com = 0;
        while ((res_com = myProcess->execute(com))!=0) {
            if (QMessageBox::warning(this,
                                     "Ошибка при создании архива",
                                     "Некоторые из файлов отчёта не удалось добавить в отправляемый архив.\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки D:\\TEMP\\\n\nПовторить попытку?",
                                     QMessageBox::Yes|QMessageBox::Abort,
                                     QMessageBox::Abort)==QMessageBox::Abort) {
                break;
            }
        }

        // запись в лог
        QDateTime time_now = QDateTime::currentDateTime();
        log_str = time_now.toString("yyyy-MM-ss hh:mm:ss") +
                  "    - выгрузка территории обслуживания МО завершена.";
        ui->te_log->append(log_str);
        QApplication::processEvents();
    } else {
        log_str = time_now.toString("yyyy-MM-ss hh:mm:ss") + "    - отмена";
        ui->te_log->append(log_str);
    }



    // запись в лог
    log_str = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +
                      " - Выгрузка данных о лицах, проживающих на территории МО " + code_mo + " :";
    ui->te_log->append(log_str);
    QApplication::processEvents();


    // запрос - выгрузка проживающих на территории МО
    QString msg_live4terr = "Выгрузка проживающих на территории МО - " + code_mo;
    QString fname_live4terr = dir_GenDir + "/" + code_mo + " - persons_by_MO_terr.csv";

    if (gen_live4terr) {
        // ------------------------------
        // проживающие на территории МО
        // ------------------------------
        QString sql_live4terr = "";

        QString date_18 = QDate::currentDate().addYears(-18).toString("yyyy-MM-dd");

        QString filter_s = "";
        if (    ui->ch_live4terr_one4terr->isChecked()   && ui->ch_live4terr_one4req->isChecked()                  // все МО
             && ui->ch_live4terr_other4terr->isChecked() && ui->ch_live4terr_other4req->isChecked()) {

        } else if ( (ui->ch_live4terr_one4terr->isChecked() || ui->ch_live4terr_one4req->isChecked()) &&      // только эта МО
                    !ui->ch_live4terr_other4terr->isChecked() && !ui->ch_live4terr_other4req->isChecked() ) {

            filter_s += "   and ( ( a.code_mo in(" + codes_mo + ") ";
            if (!ui->ch_live4terr_one4req->isChecked())        filter_s += "       and a.assig_type=1 ) ";
            else if (!ui->ch_live4terr_one4terr->isChecked())  filter_s += "       and a.assig_type=2 ) ";
            else                                               filter_s += "    ) ";

            filter_s += "       ) ";

        } else if ( !ui->ch_live4terr_one4terr->isChecked() && !ui->ch_live4terr_one4req->isChecked() &&      // только другие МО
                    (ui->ch_live4terr_other4terr->isChecked() || ui->ch_live4terr_other4req->isChecked()) ) {

            filter_s += "   and ( ( a.code_mo not in(" + codes_mo + ") ";
            if (!ui->ch_live4terr_one4req->isChecked())        filter_s += "       and a.assig_type=1 ) ";
            else if (!ui->ch_live4terr_one4terr->isChecked())  filter_s += "       and a.assig_type=2 ) ";
            else                                               filter_s +=  "    ) ";

            filter_s +=  "       ) ";

        } else if ( (ui->ch_live4terr_one4terr->isChecked() || ui->ch_live4terr_one4req->isChecked()) &&      // и эта и другие МО
                    (ui->ch_live4terr_other4terr->isChecked() || ui->ch_live4terr_other4req->isChecked()) ) {

            filter_s += "  and ( ( a.code_mo in(" + codes_mo + ") ";
            if ( ui->ch_live4terr_one4terr->isChecked()
                 && ui->ch_live4terr_one4req->isChecked())     filter_s += "      and a.assig_type in(1,2) ) ";
            else if (ui->ch_live4terr_one4terr->isChecked())   filter_s += "      and a.assig_type=1 ) ";
            else                                               filter_s += "      and a.assig_type=2 ) ";

            filter_s += "     or ( a.code_mo not in(" + codes_mo + ") ";

            if ( ui->ch_live4terr_other4terr->isChecked()
                 && ui->ch_live4terr_other4req->isChecked())   filter_s += "      and a.assig_type in(1,2) ) ";
            else if (ui->ch_live4terr_other4terr->isChecked()) filter_s += "      and a.assig_type=1 ) ";
            else                                               filter_s += "      and a.assig_type=2 ) ";

            filter_s += "      ) ";
        }

        // найдём ID персон, на этой территории
        sql_live4terr =  "TRUNCATE persons_find  "
                         " RESTART IDENTITY ; ";

        sql_live4terr += "insert into persons_find (id_person, comment) "
                         "select distinct q.id_person, mo.code_head as code_mo_terr "
                         "  from( "
                         "       (select e.id as id_person, ta.code_mo as code_mo_terr "
                         "          from "
                         "               persons e "
                         "               left join addresses adr on(adr.id=coalesce(e.id_addr_liv, e.id_addr_reg)) "
                         "               left join link_persons_asg la on(la.id_person=e.id) "
                         "               left join persons_assig a on(a.id=la._id_asg) "
                         // правила терр-прикрепления
                         "               left join assig_by_terr ta "
                         "                  on ( ( ta.sex in(0, e.sex) and ( (ta.age=0) or (e.date_birth>'" + date_18 + "' and  ta.age=1) or (e.date_birth<='" + date_18 + "' and  ta.age=2) ) ) "
                         "                   and ( (ta.subj=adr.subj and ta.dstr=adr.dstr and ta.city=adr.city and ta.nasp=adr.nasp and ta.strt=adr.strt and ta.house=adr.house and ta.corp=adr.corp ) "
                         "                      or (ta.subj=adr.subj and ta.dstr=adr.dstr and ta.city=adr.city and ta.nasp=adr.nasp and ta.strt=adr.strt and ta.house=adr.house and ta.corp= ''      ) "
                         "                      or (ta.subj=adr.subj and ta.dstr=adr.dstr and ta.city=adr.city and ta.nasp=adr.nasp and ta.strt=adr.strt and ta.house<adr.house and ta.corp= ''      and ta.from_start=1 ) "
                         "                      or (ta.subj=adr.subj and ta.dstr=adr.dstr and ta.city=adr.city and ta.nasp=adr.nasp and ta.strt=adr.strt and ta.house>adr.house and ta.corp= ''      and ta.to_end=1     ) "
                         "                      or (ta.subj=adr.subj and ta.dstr=adr.dstr and ta.city=adr.city and ta.nasp=adr.nasp and ta.strt=adr.strt and ta.house= 0        and ta.corp= ''      ) "
                         "                       ) "
                         "                     ) "
                         // условия отбора
                         "         where ( ta.code_mo in(" + codes_mo + ") ) "
                         "               " + filter_s + " "
                         "        ) "
                         "      union "
                         "        ( "
                         // выборка по обобщённым адресам
                         "        select e.id as id_person, ta.code_mo as code_mo_terr "
                         "          from "
                         "               persons e "
                         "               left join addresses adr on(adr.id=coalesce(e.id_addr_liv, e.id_addr_reg)) "
                         "               left join link_persons_asg la on(la.id_person=e.id) "
                         "               left join persons_assig a on(a.id=la._id_asg) "
                         // правила терр-прикрепления
                         "               left join assig_by_terr ta "
                         "                  on ( ( ta.sex in(0, e.sex) and ( (ta.age=0) or (e.date_birth>'" + date_18 + "' and  ta.age=1) or (e.date_birth<='" + date_18 + "' and  ta.age=2) ) ) "
                         "                   and ( (ta.subj=adr.subj and ta.dstr=adr.dstr and ta.city=adr.city and ta.nasp=adr.nasp and ta.strt= 0       and ta.house= 0        and ta.corp= ''      ) "
                         "                      or (ta.subj=adr.subj and ta.dstr=adr.dstr and ta.city=adr.city and ta.nasp= 0       and ta.strt= 0       and ta.house= 0        and ta.corp= ''      ) "
                         "                      or (ta.subj=adr.subj and ta.dstr=adr.dstr and ta.city= 0       and ta.nasp= 0       and ta.strt= 0       and ta.house= 0        and ta.corp= ''      ) "
                         "                      or (ta.subj=adr.subj and ta.dstr= 0       and ta.city= 0       and ta.nasp= 0       and ta.strt= 0       and ta.house= 0        and ta.corp= ''      ) "
                         "                       ) "
                         "                     ) "
                         // условия отбора
                         "         where ( ta.code_mo in(" + codes_mo + ") ) "
                         "               " + filter_s + " "
                         "        ) "
                         "      ) q "
                         "  left join spr_medorgs mo on(mo.code_mo=q.code_mo_terr); ";

        sql_live4terr += "select e.id, e.fam, e.im, e.ot, e.sex, e.date_birth, e.snils, "
                         "       p.pol_v, p.pol_ser, p.pol_num, p.vs_num, p.enp, p.uec_num, p.date_begin, "
                         "       pf.comment as code_mo_terr, "
                         "       a.code_mo, a.assig_type, a.assig_date, a.code_mp, a.code_ms, a.snils_mt, "
                         "       adr.subj, adr.subj_name, "
                         "       adr.dstr, adr.dstr_name, "
                         "       adr.city, adr.city_name, "
                         "       adr.nasp, adr.nasp_name, "
                         "       adr.strt, adr.strt_name, "
                         "       adr.house, adr.corp, adr.quart, "
                         "       adr1.subj as subj1, adr1.subj_name, "
                         "       adr1.dstr as dstr1, adr1.dstr_name, "
                         "       adr1.city as city1, adr1.city_name, "
                         "       adr1.nasp as nasp1, adr1.nasp_name, "
                         "       adr1.strt as strt1, adr1.strt_name, "
                         "       adr1.house, adr1.corp, adr1.quart, "
                         "       e.phone_cell, e.phone_home, e.phone_work "

                         "  from "
                         "      persons_find pf "
                         // дополнительные данные
                         "      left join persons e on(e.id=pf.id_person) "
                         "      left join polises p on(p.id=e.id_polis) "
                         "      left join link_persons_asg la on(la.id_person=e.id) "
                         "      left join persons_assig a on(a.id=la._id_asg) "
                         "      left join addresses adr on(adr.id=e.id_addr_reg) "
                         "      left join addresses adr1 on(adr1.id=e.id_addr_liv) "
                         " ; ";

        sql2csv(db, sql_live4terr, false, fname_live4terr, this);

        // добавим в архив
        QString com = QString("D:/INKO/7-Zip/7z.exe a -tzip \"") + fname_arch + "\" \"" + fname_live4terr + "\" -mx9";
        int res_com = 0;
        while ((res_com = myProcess->execute(com))!=0) {
            if (QMessageBox::warning(this,
                                     "Ошибка при создании архива",
                                     "Некоторые из файлов отчёта не удалось добавить в отправляемый архив.\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки D:\\TEMP\\\n\nПовторить попытку?",
                                     QMessageBox::Yes|QMessageBox::Abort,
                                     QMessageBox::Abort)==QMessageBox::Abort) {
                break;
            }
        }

        // запись в лог
        QDateTime time_now = QDateTime::currentDateTime();
        log_str = time_now.toString("yyyy-MM-ss hh:mm:ss") +
                  "    - выгрузка 'Все проживающие на территории МО' завершена.";
        ui->te_log->append(log_str);
        QApplication::processEvents();
    } else {
        log_str = time_now.toString("yyyy-MM-ss hh:mm:ss") + "    - отмена";
        ui->te_log->append(log_str);
    }



    log_str = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +
                      " - Выгрузка адресов МО " + code_mo + " :";
    ui->te_log->append(log_str);
    QApplication::processEvents();






    // запрос - выгрузка адресов МО
    QString msg_address = "Выгрузка адресов МО - " + code_mo;
    QString fname_address = dir_GenDir + "/" + code_mo + " - MO_addresses - " + QDate::currentDate().toString("yyyy-MM-dd") + ".csv";

    if (gen_addresses) {
        // --------------------------
        // адреса МО
        // --------------------------
        QString sql_address = "";

        sql_address += "select distinct la.code_mo, la.naim as name_mo, ta.code_distr, dstr.naim as dstr, ta.code_nasp, nasp.naim as nasp, ta.code_strt, strt.naim as strt, ta.all_str, ta.n_dom, ta.korp, ta.for_age ";
        sql_address += "  from assig_by_terr ta ";
        sql_address += "       left join lpu la on(la.kod_lpu=ta.code_lpu) ";
        sql_address += "       left join adres_dict subj on(subj.adr_id=ta.code_terr and subj.obj_id=1) ";
        sql_address += "       left join adres_dict dstr on(dstr.adr_id=ta.code_distr and dstr.obj_id=2) ";
        sql_address += "       left join adres_dict nasp on(nasp.adr_id=ta.code_nasp and nasp.obj_id=3) ";
        sql_address += "       left join adres_dict strt on(strt.adr_id=ta.code_strt and strt.obj_id=4) ";
        sql_address += " where la.code_osn is not NULL ";
        sql_address += "   and la.code_osn='" + code_mo + "' ";
        sql_address += " order by dstr.naim, nasp.naim, strt.naim, ta.all_str, ta.n_dom, ta.korp, ta.for_age ; ";

        sql2csv(db, sql_address, false, fname_address, this);

        // добавим в архив
        QString com = QString("D:/INKO/7-Zip/7z.exe a -tzip \"") + fname_arch + "\" \"" + fname_address + "\" -mx9";
        int res_com = 0;
        while ((res_com = myProcess->execute(com))!=0) {
            if (QMessageBox::warning(this,
                                     "Ошибка при создании архива",
                                     "Некоторые из файлов отчёта не удалось добавить в отправляемый архив.\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки D:\\TEMP\\\n\nПовторить попытку?",
                                     QMessageBox::Yes|QMessageBox::Abort,
                                     QMessageBox::Abort)==QMessageBox::Abort) {
                break;
            }
        }

        // запись в лог
        QDateTime time_now = QDateTime::currentDateTime();
        log_str = time_now.toString("yyyy-MM-ss hh:mm:ss") +
                  "    - выгрузка 'Адреса МО' завершена.";
        ui->te_log->append(log_str);
        QApplication::processEvents();
    } else {
        log_str = time_now.toString("yyyy-MM-ss hh:mm:ss") + "    - отмена";
        ui->te_log->append(log_str);
    }



    log_str = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +
                      " - Выгрузка данных о лицах, прикреплённых к МО " + code_mo + " :";
    ui->te_log->append(log_str);
    QApplication::processEvents();

    // запрос - выгрузка прикреплённых к МО
    QString msg_assig = "Выгрузка прикреплённых к МО - " + code_mo;
    QString fname_assig = dir_GenDir + "/" + code_mo + " - MO_persons - " + QDate::currentDate().toString("yyyy-MM-dd") + ".csv";

    if (gen_assig) {
        // --------------------------
        // данные прикреплённых лиц
        // --------------------------
        QString sql_assig = "";

        if (ui->rb_RSourse_main->isChecked()) {
            sql_assig +=     "select distinct e.fam, e.im, e.ot, e.dat_r, e.sex, e.snils, p.oms_rights as pol_type, e.enp, p.blank_ser, p.blank_num, p.temp_number, \n"
                             "       a.code_mo, a.assig_date, a.assig_type, a.code_mp, a.code_ms, a.snils_mt, \n";
            if (f_address)
                sql_assig += "       subj.naim as subj, dstr.naim as dstr, nasp.naim as nasp, strt.naim as strt, p.dom, p.korp, p.kv, \n";
            if (f_phone)
                sql_assig += "       coalesce(p.contact_phone, p.contact_phone_home, p.contact_phone_cellular, p.contact_phone_work) as phone, \n";
            sql_assig +=     "       ' ' as i \n"
                             "  from persons e \n";
            if (f_date_rep)
                sql_assig += "       join persons_assig a on(e.id=a.id_person and a.assig_type>0 and a.assig_date<='" + date_rep.toString("yyyy-MM-dd") + "' and ((a.unsig_date is NULL) or (a.unsig_date>'" + date_rep.toString("yyyy-MM-dd") + "'))) \n";
            else
                sql_assig += "       join persons_assig a on(e.id=a.id_person and a.assig_type>0 and a.is_actual=0) \n";

            sql_assig +=     "  left join spr_medorgs mo on(mo.code_mo=a.code_mo) \n"
                             "  left join polises p on(p.id=e.id_polis) \n";
            if (f_address) {
                sql_assig += "  left join addresses ar on(ar.id=e.id_addr_reg) \n";
            }
            sql_assig +=     " where e.id is not NULL \n";
            sql_assig +=     "   and mo.code_head='" + code_mo + "' \n";
            if (assig_type>0)
                sql_assig += "   and a.assig_type=" + QString::number(assig_type) + " \n";
            if (assig_age==1)
                sql_assig += "   and ADDYEAR(e.dat_r,18)>=CURRENT_DATE \n";
            else if (assig_age==2)
                sql_assig += "   and ADDYEAR(e.dat_r,18)<CURRENT_DATE \n";
            sql_assig +=     " order by e.fam, e.im, e.ot, e.dat_r, e.sex ; ";
        }

        sql2csv(db, sql_assig, false, fname_assig, this); //, QStringList names_old=QStringList(), QStringList names_new=QStringList());
        /*
        wnd_sql2csv *wnd_assig = new wnd_sql2csv(db, this, sql_assig, fname_assig, "\r\n", ";", "", "", msg_assig, msg_assig, true, true);
        int res_assig =  wnd_assig->exec();
        delete wnd_assig;
        */
        // добавим в архив
        QString com = QString("D:/INKO/7-Zip/7z.exe a -tzip \"") + fname_arch + "\" \"" + fname_assig + "\" -mx9";
        int res_com = 0;
        while ((res_com = myProcess->execute(com))!=0) {
            if (QMessageBox::warning(this,
                                     "Ошибка при создании архива",
                                     "Некоторые из файлов отчёта не удалось добавить в отправляемый архив.\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки D:\\TEMP\\\n\nПовторить попытку?",
                                     QMessageBox::Yes|QMessageBox::Abort,
                                     QMessageBox::Abort)==QMessageBox::Abort) {
                break;
            }
        }

        // запись в лог
        QDateTime time_now = QDateTime::currentDateTime();
        log_str = time_now.toString("yyyy-MM-ss hh:mm:ss") +
                  "    - выгрузка 'Текущая численность/выгрузка прикреплённых к МО' завершена.";
        ui->te_log->append(log_str);
        QApplication::processEvents();
    } else {
        log_str = time_now.toString("yyyy-MM-ss hh:mm:ss") + "    - отмена";
        ui->te_log->append(log_str);
    }




    log_str = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +
                      " - Проверка прикрепления застрахованных лиц к МО по списку " + code_mo + " :";
    ui->te_log->append(log_str);
    QApplication::processEvents();

    // запрос - проверка прикрепления к МО по списку
    int id_layer = ui->combo_data_source->currentData().toInt();

    QString msg_analize = "Проверка прикреплённых к МО - " + code_mo + " по списку";
    QString fname_analize = dir_GenDir + "/" + code_mo + " - analize_MO_assig - " + QDate::currentDate().toString("yyyy-MM-dd") + " - по массиву " + (id_layer<0 ? "База ИНКО" : ui->combo_data_source->currentText()) + ".csv";

    if (gen_analize) {
        // --------------------------
        // данные прикреплённых лиц
        // --------------------------
        QString sql_analize = "";
        if (id_layer<0)
            sql_analize = "select * from assig.analize_assigs_proc(cast('" + code_mo + "' as text)) ; ";
        else
            sql_analize = "select * from assig.analize_assigs_proc(" + QString::number(id_layer) + ", cast('" + code_mo + "' as text)) ; ";

        QSqlQuery *query_analize = new QSqlQuery(db);
        bool res_analize = query_analize->exec(sql_analize);
        if (!res_analize) {
            QString err1 = query_analize->lastError().driverText();
            QString err2 = query_analize->lastError().databaseText();
            QMessageBox::warning(this, "Ошибка SQL",
                                 "sql: " + sql_analize + "\n\n"
                                 "driver: " + err1 + "\n"
                                 "db: " + err2);
        }
        delete query_analize;

        QString sql_rezult = "select * from assig.analize_assigs_result order by status; ";
        sql2csv(db, sql_rezult, false, fname_analize, this);

        // добавим в архив
        QString com = QString("D:/INKO/7-Zip/7z.exe a -tzip \"") + fname_arch + "\" \"" + fname_analize + "\" -mx9";
        int res_com = 0;
        while ((res_com = myProcess->execute(com))!=0) {
            if (QMessageBox::warning(this,
                                     "Ошибка при создании архива",
                                     "Некоторые из файлов отчёта не удалось добавить в отправляемый архив.\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки D:\\TEMP\\\n\nПовторить попытку?",
                                     QMessageBox::Yes|QMessageBox::Abort,
                                     QMessageBox::Abort)==QMessageBox::Abort) {
                break;
            }
        }

        // запись в лог
        QDateTime time_now = QDateTime::currentDateTime();
        log_str = time_now.toString("yyyy-MM-ss hh:mm:ss") +
                  "    - выгрузка 'Текущая численность/выгрузка прикреплённых к МО' завершена.";
        ui->te_log->append(log_str);
        QApplication::processEvents();
    } else {
        log_str = time_now.toString("yyyy-MM-ss hh:mm:ss") + "    - отмена";
        ui->te_log->append(log_str);
    }

    db.commit();


    // -------------------------------------------- //
    // -------------------------------------------- //
    //    добавление в выгрузку остальных файлов    //
    // -------------------------------------------- //
    // -------------------------------------------- //
    log_str = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +
                      " - Добавление остальных файлов " + code_mo + " :";
    ui->te_log->append(log_str);
    QApplication::processEvents();

    if (f_other_files) {
        // перебор содержимого папки данных
        QDir dir(dir_OtherData);
        foreach(QString entry, dir.entryList()) {
            if ((entry == ".") || (entry == ".."))
                continue;
            QString fName = QString(dir_OtherData + "\\" + entry).replace("/","\\");
            QFileInfo fi(fName);

            QString fn_up = entry.toUpper();

            // найдём в папке данных все файлы, в названии которых есть код МО
            QString code_mo_s = code_mo;
            QString ss = "ПИСЬМО";
            int pos0 = fn_up.indexOf(ss,0);
            QString sss = "ПОЯСНИТЕЛЬНАЯ ЗАПИСКА";
            int pos1 = fn_up.indexOf(sss,0);
            if ( fi.isFile() &&
                 ( entry.contains(code_mo_s) /*||
                   pos0>=0  ||
                   pos1>=0*/ ) ) {
                // добавим этот файл
                int res_exe = 0;
                QString com = QString("D:/INKO/7-Zip/7z.exe a -tzip \"") + fname_arch + "\" \"" + fName + "\" -mx9";
                int res_com = 0;
                while ((res_com = myProcess->execute(com))!=0) {
                    if ( QMessageBox::warning(this, "Ошибка при добавлении файла",
                                              "Произошла ошибка при попытке добавления файла \n" + fName + "\n" +
                                              "в архив \n" + fname_arch + "\n\n" +
                                              "Повторить попытку?",
                                              QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)==QMessageBox::No ) {
                        break;
                    }
                }
                // запись в лог
                if (res_exe==0)
                    ui->te_log->append("\t - добавлен файл " + fName);
            }

            QString dName = QString(dir_OtherData + "\\" + entry).replace("/","\\");
            QFileInfo di(dName);

            // найдём в папке данных все папки, в названии которых есть код МО
            if (di.isDir() && entry.contains(code_mo)) {
                QDir fDir(dName);

                // переберём все файлы в этой папке
                foreach(QString dEntry, fDir.entryList()) {
                    if ((dEntry == ".") || (dEntry == ".."))
                        continue;
                    QString dfName = dName + "\\" + dEntry.replace("/","\\");
                    QFileInfo dfi(dfName);

                    // переберём в этой папке все файлы
                    if (dfi.isFile()) {
                        // добавим этот файл
                        int res_exe = 0;
                        QString dCom = QString("D:/INKO/7-Zip/7z.exe a -tzip \"") + fname_arch + "\" \"" + dfName + "\" -mx9";
                        while ( (res_exe = myProcess->execute(dCom)) !=0 ) {
                            if ( QMessageBox::warning(this, "Ошибка при добавлении файла",
                                                      "Произошла ошибка при попытке добавления файла \n" + dfName + "\n" +
                                                      "в архив \n" + fname_arch + "\n\n" +
                                                      "Повторить попытку?",
                                                      QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)==QMessageBox::No ) {
                                break;
                            }
                        }
                        // запись в лог
                        if (res_exe==0)
                            ui->te_log->append("\t - добавлен файл " + dfName);
                    }
                }
            }
        }
    }

    /*
    // добавим файл readme.txt
    int res_exe = 0;
    QString fName = dir_GenDir + "\\readme.txt";
    while ( (res_exe = myProcess->execute(QString("D:/INKO/7-Zip/7z.exe a -tzip \"" + fname_arch + "\" " + fName + " -mx9")) !=0 ) {
        if ( QMessageBox::warning(this, "Ошибка при добавлении файла"),
                             "Произошла ошибка при попытке добавления файла \n" + fName + "\n" +
                             "в архив \n" + fname_arch + "\n\n" +
                             "Повторить попытку?"),
                             QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)==QMessageBox::No ) {
            break;
        }
    }
    // запись в лог
    if (res_exe==0)
        ui->te_log->append("\t - добавлен файл " + fName);
    * /
    myProcess->close();
    delete myProcess;





    //удаление временных файлов
    QFile::remove(fname_assig);
    QFile::remove(fname_addresses);
    QFile::remove(fname_renames);
    QFile::remove(fname_analize);


    if (!ui->ch_debug->isChecked()) {
        // перенос архива в папку для хранения
        QString fname_arch_backup = fname_arch;
        fname_arch_backup = fname_arch_backup.replace(dir_GenDir.replace("/","\\"), dir_Backup);
        if (QFile::exists(fname_arch_backup))
            QFile::remove(fname_arch_backup);
        bool res_bk = QFile::copy(fname_arch, fname_arch_backup);
        if (!res_bk) {
            QMessageBox::warning(this, "Внимание! Ошибка резервного копирования"),
                                 "Папка резервного копирования данных не доступна:\n\n") +
                                 dir_Backup +
                                 "Работа прервана. Данные не отправлены."));
            this->setCursor(Qt::ArrowCursor);
            return false;
        }

        // запись в лог
               QDateTime time_now = QDateTime::currentDateTime();
               log_str = time_now.toString("yyyy-MM-ss hh:mm:ss") +
                         " - файл скопироват в папку резервного хранения ";
        ui->te_log->append(log_str + "\n");

        /*
        if (ui->ch_RSendVipnet->isChecked()) {
            // отправка архива по деловой почте в МО
            QString arch_tosend_name = fname_arch;
            arch_tosend_name = arch_tosend_name.replace(dir_GenDir.replace("/","\\"), QString("\\\\for-tfoms\\LPU\\" + code_mo + "\\");
            if (QFile::exists(arch_tosend_name))
                QFile::remove(arch_tosend_name);
            bool res_snd = QFile::copy(fname_arch, arch_tosend_name);

            // запись в лог
               QDateTime time_now = QDateTime::currentDateTime();
               log_str = time_now.toString("yyyy-MM-ss hh:mm:ss") +
                         " - файл отправлен по \"Деловой почте\" на абонпункт МО " +
                         code_mo;
            ui->te_log->append(log_str + "\n");
        }
        * /

        // удаление архива в папке ТЕМР.
        QFile::remove(fname_arch);
    }

    // запись в лог
               QDateTime time_now = QDateTime::currentDateTime();
               log_str = time_now.toString("yyyy-MM-ss hh:mm:ss") +
                         " - выгрузка завершена.");
    ui->te_log->append(log_str + "\n");

    this->setCursor(Qt::ArrowCursor);
    return true;*/

    this->setCursor(Qt::ArrowCursor);


    // ===========================
    // собственно открытие шаблона
    // ===========================
    // открытие полученного ODT
    /*long result = (long)*/ShellExecute(0, 0, reinterpret_cast<const WCHAR*>(dir_GenDir.utf16()), 0, 0, SW_NORMAL);
}

void reports_assig2mo_wnd::on_ch_all_medorgs_clicked(bool checked) {
    if (checked) {
        QItemSelectionModel *sel_model = ui->tab_medorgs->selectionModel();
        sel_model->reset();
        for (int i=0; i<model_medorgs.rowCount(); i++) {
            ui->tab_medorgs->selectRow(i);
            //sel_model->select(model_medorgs.index(1,i), QItemSelectionModel::Select);
            //sel_model->select(model_medorgs.index(2,i), QItemSelectionModel::Select);
        }
        ui->tab_medorgs->setEnabled(false);
    } else {
        //refresh_medorgs_tab();
        ui->tab_medorgs->setEnabled(true);
    }
}

void reports_assig2mo_wnd::on_bn_clear_sel_clicked() {
    QItemSelectionModel *sel_model = ui->tab_medorgs->selectionModel();

    for (int i=0; i<model_medorgs.rowCount(); i++) {
        sel_model->select(model_medorgs.index(i,0),QItemSelectionModel::Deselect);
        sel_model->select(model_medorgs.index(i,1),QItemSelectionModel::Deselect);
        sel_model->select(model_medorgs.index(i,2),QItemSelectionModel::Deselect);
        sel_model->select(model_medorgs.index(i,3),QItemSelectionModel::Deselect);
    }
    ui->tab_medorgs->repaint();
}

void reports_assig2mo_wnd::on_bn_reverse_sel_clicked() {
    QItemSelectionModel *sel_model = ui->tab_medorgs->selectionModel();

    for (int i=0; i<model_medorgs.rowCount(); i++) {
        if (sel_model->isSelected(model_medorgs.index(i,1))) {
            sel_model->select(model_medorgs.index(i,0),QItemSelectionModel::Deselect);
            sel_model->select(model_medorgs.index(i,1),QItemSelectionModel::Deselect);
            sel_model->select(model_medorgs.index(i,2),QItemSelectionModel::Deselect);
            sel_model->select(model_medorgs.index(i,3),QItemSelectionModel::Deselect);

        } else {
            sel_model->select(model_medorgs.index(i,0),QItemSelectionModel::Select);
            sel_model->select(model_medorgs.index(i,1),QItemSelectionModel::Select);
            sel_model->select(model_medorgs.index(i,2),QItemSelectionModel::Select);
            sel_model->select(model_medorgs.index(i,3),QItemSelectionModel::Select);

        }
    }
    ui->tab_medorgs->repaint();
}

void reports_assig2mo_wnd::refresh_medorgs_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    ui->pan_medorgs_UP->setEnabled(false);
    ui->pan_medorgs_DN->setEnabled(false);
    ui->pan_medorgs->setEnabled(false);
    QApplication::processEvents();

    sql_medorgs = "select id, code_mo, name_mo, mo_assig "
                  "  from spr_medorgs "
                  " where mo_assig>0 "
                  " order by code_mo ; ";

    model_medorgs.setQuery(sql_medorgs,db);
    QString err2 = model_medorgs.lastError().driverText();

    // подключаем модель из БД
    ui->tab_medorgs->setModel(&model_medorgs);

    // обновляем таблицу
    ui->tab_medorgs->reset();

    // задаём ширину колонок
    ui->tab_medorgs->setColumnWidth(  0,  2);     // id
    ui->tab_medorgs->setColumnWidth(  1, 50);     // code_mo
    ui->tab_medorgs->setColumnWidth(  2,190);     // name_mo
    ui->tab_medorgs->setColumnWidth(  3, 30);     // name_mo

    // правим заголовки
    model_medorgs.setHeaderData(  0, Qt::Horizontal, ("ID"));
    model_medorgs.setHeaderData(  1, Qt::Horizontal, ("рег. №"));
    model_medorgs.setHeaderData(  2, Qt::Horizontal, ("медицинская организация"));
    model_medorgs.setHeaderData(  3, Qt::Horizontal, ("V"));
    ui->tab_medorgs->repaint();

    on_ch_all_medorgs_clicked(ui->ch_all_medorgs->isChecked());

    ui->pan_medorgs_UP->setEnabled(true);
    ui->pan_medorgs_DN->setEnabled(true);
    ui->pan_medorgs->setEnabled(true);
    QApplication::processEvents();
    this->setCursor(Qt::ArrowCursor);
}

void reports_assig2mo_wnd::on_bn_refresh_medorgs_clicked() {
    refresh_medorgs_tab();
}
void reports_assig2mo_wnd::on_ch_lock_clicked(bool checked) {
    ui->tab_medorgs->setEnabled(!checked);
    ui->bn_reverse_sel->setEnabled(!checked);
    ui->bn_clear_sel->setEnabled(!checked);
    ui->ch_all_medorgs->setEnabled(!checked);

    if (!checked) {
        ui->ch_all_medorgs->setChecked(false);
    }
}
void reports_assig2mo_wnd::on_bn_MSendVipnet_clicked() {
    // спросим папку, где расположены рассылаемые данные
    QString dir_str = QFileDialog::getExistingDirectory(this, "Выберите папку, где расположены рассылаемые данные", ui->line_gen_dir->text(), 0);
    if (dir_str.isEmpty()) {
        QMessageBox::information(this, "Ничего не выбрано", "Папка с данными не выбрана - операция отменена.");
        return;
    }

    // выберем список папок из папки рассылки "Деловой почты"
    QString dir_vipnet_str = ui->line_VipNet->text();
    QDir dir_vipnet(dir_vipnet_str);
    //QDir dir_vipnet("\\\\for-tfoms\\LPU");
    QStringList vip_dirs = dir_vipnet.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    QList<int> vip_ints;
    for (int i=0; i<vip_dirs.size(); i++) {
        int vip_int = 0;
        QDir vip_dir(vip_dirs.at(i));
        QString vip_str = vip_dir.dirName();
        vip_int = vip_str.toInt() + 360000;
        vip_ints.append(vip_int);
    }

    // переберём ZIP-файлы в папке
    QDir dir(dir_str);
    QStringList filters;
    //filters << "*.zip" << "*.7z" << "*.rar" << "*.odt" << "*.ods" << "*.doc" << "*.xls" << "*.jpg";
    QStringList files = dir.entryList(filters,QDir::Files,QDir::NoSort);

    for (int i=0; i<files.size(); i++) {
        bool f_continue = false;
        QFile file(files.at(i));
        QString fname = file.fileName();

        // переберём папки ВипНет
        for (int j=0; j<vip_dirs.size(); j++) {
            QString code_mo = QString::number(vip_ints.at(j));
            if (fname.indexOf(code_mo,0)>=0) {

                // отправим этот файл на Випнет в эту МО
                QString fname_old = dir_str + "\\" + files.at(i);
                QString fname_new = dir_vipnet_str + "\\" + vip_dirs.at(j) + "\\" + fname;
                while (!QFile::copy(fname_old, fname_new)) {
                    if (QMessageBox::warning(this, "Ошибка копирования файла",
                                             "При попытке скопировать файл\n" +
                                             fname_old +
                                             "\n\nв папку рассылки по VipNet\n" +
                                             fname_new +
                                             "\n\nпроизошла ошибка доступа\nПовторить?",
                                             QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::No) {
                        f_continue = true;
                        break;
                    }
                }
                if (f_continue)
                    continue;

                f_continue = false;
            }
        }
        ui->lab_vipnet_cnt->setText(QString::number(i));
        QApplication::processEvents();
    }

    QMessageBox::information(this, "Отправка завершена", "Отправка завершена");
}

void reports_assig2mo_wnd::on_bn_generate_clicked() {
    if ( !ui->tab_medorgs->selectionModel()->hasSelection() ) {
        QMessageBox::warning(this, "Не выбрана ни одна медорганизация",
                             "Не выбрана ни одна медорганизация.\n"
                             "Для запуска процедуры надо выбрать медорганизацию или перечень медорганизаций, для которых делается отчёт.\n\n"
                             "Отмена операции.");
        return;
    }


    // запомним параметры генерации
    bool gen_terr         = ui->ch_terr->isChecked();
    bool gen_live4terr    = ui->ch_live4terr->isChecked();
    bool f_live4terr_one4terr   = ui->ch_live4terr_one4terr->isChecked();
    bool f_live4terr_one4req    = ui->ch_live4terr_one4req->isChecked();
    bool f_live4terr_other4terr = ui->ch_live4terr_other4terr->isChecked();
    bool f_live4terr_other4req  = ui->ch_live4terr_other4terr->isChecked();
    bool gen_assig        = ui->ch_assig->isChecked();
    bool gen_analize      = ui->ch_analize->isChecked();
    bool gen_other_files  = ui->ch_other_files->isChecked();
    bool f_append_arch    = ui->ch_append_arch->isChecked();
    int  assig_type       = ui->combo_assig_type->currentIndex();
    int  age              = ui->combo_age->currentIndex();
    bool f_address        = ui->ch_address->isChecked();
    bool f_phone          = ui->ch_phone->isChecked();
    bool f_date_rep       = ui->ch_date_rep->isChecked();
    bool gen_addresses    = ui->ch_addresses->isChecked();
    QDate date_rep        = ui->date_rep->date();
    QString other_files_s = ui->line_other_files->text();
    QString gen_dir_s     = ui->line_gen_dir->text();



    // папка доп.файлов
    QDir dir_other_files;
    // если надо - проверим папку дополнительных файлов
    if (gen_other_files) {
        dir_other_files.setPath(ui->line_other_files->text());
        if ( !dir_other_files.exists() ) {
            QMessageBox::warning(this, "Не найдена папка с дополнительными файлами",
                                 "Папка с дополнительными данными не найдена или не указана оператором.\n"
                                 "Выберите папку с дополнительными файлами или снимите флажок.\n\n"
                                 "Отмена операции.");
            return;
        }
    }
    // папка генерации - есть всегда
    QString dir_generate_s = ui->line_gen_dir->text();
    QDir dir_generate(dir_generate_s);
    if ( !dir_generate.exists() ) {
        if ( !dir_generate.mkdir(dir_generate_s) ) {
            QMessageBox::warning(this, "Не найдена целевая папка генерации писем",
                                 "Папка для генерации писем не найдена или не указана оператором.\n"
                                 "При попытке создать такую папку получен отказ.\n\n"
                                 "Отмена операции.");
            return;
        }
    }
    /*
    // проверим, надо ли очистить целевую папку генерации
    QStringList files = dir_generate.entryList(QDir::NoDotAndDotDot|QDir::NoSymLinks|QDir::Files);
    if (files.count()>0) {
        if (QMessageBox::question(this, "Папка генерации писем должна быть пустой",
                                  "Выбранная целевая папка писем должна быть пустой.\n"
                                  "  Для продолжения работы всё содержимое папки будет автоматически удалено.\n"
                                  "  В случае отказа - генерация писем не возможна.\n\n"
                                  "Удалить файлы?", QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Yes) {
            // очистим папку
            foreach (QString file, files) {
                if (!QFile::remove(dir_generate_s + "/" + file)) {
                    QMessageBox::warning(this, "Не удалось удалить файл : ",
                                         "При попытке удаления файла \n  " +
                                         file +
                                         "\n  произошла ошибка. \n\n"
                                         "Удалите файл вручную или выберите другую папку для генерации писем.");
                    return;
                }
            }
        } else {
            QMessageBox::warning(this, "Папка генерации писем должна быть пустой",
                                 "Отмена операции.");
            return;
        }
    }
    */

    ui->ch_lock->setChecked(true);
    on_ch_lock_clicked(true);
    ui->lab_count->setText("0");
    ui->te_log->clear();
    ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->  Запущен процесс генераци писем...");
    QApplication::processEvents();

    // подготовка README.TXT
    QString fname_readme = dir_generate_s + "/readme.txt";
    QFile file_readme(fname_readme);
    if (QFile::exists(fname_readme))
        QFile::remove(fname_readme);
    file_readme.open(QFile::WriteOnly);

/*
    bool gen_live4terr    = ui->ch_live4terr->isChecked();
    bool f_live4terr_one4terr   = ui->ch_live4terr_one4terr->isChecked();
    bool f_live4terr_one4req    = ui->ch_live4terr_one4req->isChecked();
    bool f_live4terr_other4terr = ui->ch_live4terr_other4terr->isChecked();
    bool f_live4terr_other4req  = ui->ch_live4terr_other4terr->isChecked();
    bool gen_assig        = ui->ch_assig->isChecked();
    bool gen_analize      = ui->ch_analize->isChecked();
    bool gen_other_files  = ui->ch_other_files->isChecked();
    bool f_append_arch    = ui->ch_append_arch->isChecked();
    int  assig_type       = ui->combo_assig_type->currentIndex();
    int  age              = ui->combo_age->currentIndex();
    bool f_address        = ui->ch_address->isChecked();
    bool f_phone          = ui->ch_phone->isChecked();
    bool f_date_rep       = ui->ch_date_rep->isChecked();
    bool gen_addresses    = ui->ch_addresses->isChecked();
    QDate date_rep        = ui->date_rep->date();
    QString other_files_s = ui->line_other_files->text();
    QString gen_dir_s     = ui->line_gen_dir->text();*/

     file_readme.write("Содержимое архива:\n\n");

     if (gen_terr)              file_readme.write("XXXXXX - MO_terr.csv  -  адреса территории обслуживания медицинской организации.\n");
     if (gen_live4terr)         file_readme.write("XXXXXX - persons_by_MO_terr.csv  -  список лиц, проживающих на территории обслуживания медицинской организации.\n"
                                                  "                                    в том числе : ");

     file_readme.close();

    // ПОЕХАЛИ !
    time_start = QDateTime::currentDateTime();

    // переберём медорганизации
    QItemSelectionModel *sel_model = ui->tab_medorgs->selectionModel();

    int count = 0;

    ui->split_log->setSizes(l01);

    for (int i=0; i<model_medorgs.rowCount(); i++) {
        if (sel_model->isSelected(model_medorgs.index(i,1))) {
            QString code_mo = model_medorgs.data(model_medorgs.index(i, 1), Qt::EditRole).toString();

            ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->  генерация письма в медорганизацию  " + code_mo );
            QApplication::processEvents();

            // собственно генерация отчёта
            bool res = rep_MO_assig ( code_mo, "persons_assig", assig_type, age,
                                      gen_terr, gen_live4terr, f_live4terr_one4terr, f_live4terr_one4req, f_live4terr_other4terr, f_live4terr_other4req,
                                      f_address, f_phone, f_date_rep, date_rep,
                                      gen_addresses, gen_assig, gen_analize, gen_other_files, f_append_arch,
                                      other_files_s, gen_dir_s);

            if (res) {
                // снимем выделение с этой медорганизации
                sel_model->select(model_medorgs.index(i,0),QItemSelectionModel::Deselect);
                sel_model->select(model_medorgs.index(i,1),QItemSelectionModel::Deselect);
                sel_model->select(model_medorgs.index(i,2),QItemSelectionModel::Deselect);
                sel_model->select(model_medorgs.index(i,3),QItemSelectionModel::Deselect);
                //ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->  генерация письма в медорганизацию  " + code_mo + " - ГОТОВО\n");
                //QApplication::processEvents();
            } else {
                // выделение не снимаем - пусть юзер видит с чем ошибка
                ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->  генерация письма в медорганизацию  " + code_mo + " - !! ОТКАЗ !!\n");
                QApplication::processEvents();
            }
            count++;
            ui->lab_count->setText(QString::number(count));
        }
    }

    ui->split_log->setSizes(l51);
    ui->ch_lock->setChecked(false);
    ui->ch_all_medorgs->setChecked(false);
    ui->ch_all_medorgs->setEnabled(true);
    ui->tab_medorgs->setEnabled(true);

    ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " ->  ГОТОВО!  Файлы сохранены в папку - ");
    ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " ->           " + dir_generate_s + "\n");

    QApplication::processEvents();

    QMessageBox::information(this, "Генерация завершена", "Генерация завершена");
    // ГОТОВО !
}
