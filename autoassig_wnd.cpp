#include "autoassig_wnd.h"
#include "ui_autoassig_wnd.h"
#include <QInputDialog>
#include "ki_funcs.h"

autoassig_wnd::autoassig_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::autoassig_wnd)
{
    ui->setupUi(this);

    //on_bn_date_cur_clicked();
    ui->ch_unsig_if_fail->setEnabled(true);
    ui->ch_unsig_if_fail->setChecked(false);

    if (data_app.ocato=="20000") {
        ui->ch_assig4age->setChecked(false);
    }
}

autoassig_wnd::~autoassig_wnd() {
    delete ui;
}

QString autoassig_wnd::month_to_str(QDate date) {
    QString res;
    switch (date.month()) {
    case 1:
        res += "января ";
        break;
    case 2:
        res += "февраля ";
        break;
    case 3:
        res += "марта ";
        break;
    case 4:
        res += "апреля ";
        break;
    case 5:
        res += "мая ";
        break;
    case 6:
        res += "июня ";
        break;
    case 7:
        res += "июля ";
        break;
    case 8:
        res += "августа ";
        break;
    case 9:
        res += "сентября ";
        break;
    case 10:
        res += "октября ";
        break;
    case 11:
        res += "ноября ";
        break;
    case 12:
        res += "декабря ";
        break;
    default:
        break;
    }
    return res;
}

#include "qt_windows.h"
#include "qwindowdefs_win.h"
#include <shellapi.h>

void autoassig_wnd::on_bn_print_clicked() {
    /*
    // распаковка шаблона реестра
    QString fname_template = data_app.path_reports + "rep_assig_sexage_tab.ods";
    QString temp_folder = data_app.path_reports + "rep_assig_sexage_tab";
    if (!QFile::exists(fname_template)) {
        QMessageBox::warning(this,
                             "Шаблон не найден",
                             "Не найден шаблон печатной формы половозрастной таблицы прикреплённых к МО: \n" + fname_template +
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

    // ===========================
    // правка контента шаблона
    // ===========================
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
    int pos0 = s_content.indexOf("<table:table-row table:style-name=\"ro6\">", 0);
    int pos1 = s_content.indexOf("</table:table-row>", pos0) + 18;
    QString s_row = s_content.mid(pos0, pos1-pos0);

    // правка полей контента шаблона
    QString s_content_new = s_content.left(pos0);




    // пороговые даты
    QDate date_cur = ui->date_cur->date();
    QDate date_05 = date_cur.addYears( -5);
    QDate date_18 = date_cur.addYears(-18);
    QDate date_55 = date_cur.addYears(-55);
    QDate date_60 = date_cur.addYears(-60);
    QString date_str = date_cur.toString("yy-MM-yyyy");

    // получим список районов
    QString sql_dstrs;
    //sql_dstrs = "select m.id, m.code_mo, m.text from assig_medorgs m  order by m.id asc ; ";

    sql_dstrs += "select distinct m.id, m.code_mo, m.name_mo ";
    sql_dstrs += " from persons e ";
    sql_dstrs += "      left join polises p on ( p.id_person=e.id and p.pol_date_begin<='" + date_str + "' and (p.pol_date_end is NULL or p.pol_date_end>='" + date_str + "') and (p.pol_date_stop is NULL or p.pol_date_stop>='" + date_str + "') ) ";
    sql_dstrs += "      left join persons_assig asg on ( asg.id_person=e.id and asg.is_actual=0 ) ";
    sql_dstrs += "      left join spr_medorgs m on ( m.code_mo=asg.code_mo and m.is_head=1 ) ";
    sql_dstrs += " where m.code_mo is not NULL and p.id is not NULL and  p.pol_date_begin<='" + date_str + "' and  (p.pol_date_stop is NULL or p.pol_date_stop>'" + date_str + "')  ; ";

    QSqlQuery *query_dstrs = new QSqlQuery(db);
    bool res_dstr = mySQL.exec(this, sql_dstrs, "Запросим список медорганизаций, к которым может прикрепляться население", *query_dstrs, true);
    if (!res_dstr || query_dstrs->size()<=0) {
        QMessageBox::warning(this, "Список медорганизаций не получен", "При выборе полного списка медорганизаций произошла неожиданная ошибка.");
        delete query_dstrs;
        return;
    };

    // выберем список персон
    QString sql_pers = "select id from persons p ; ";
    QSqlQuery *query_pers = new QSqlQuery(db);
    bool res_pers = mySQL.exec(this, sql_pers, QString("Выбор полного списка застрахованных"), *query_pers, true);
    if (!res_pers || query_pers->size()<=0) {
        QMessageBox::warning(this, "Список персон не получен", "При выборе полного списка застрахованных произошла неожиданная ошибка.");
        delete query_pers;
        return;
    }



    // суммирование по колонкам
    int sm_4 =   0;
    int sw_4 =   0;
    int sm517 =  0;
    int sw517 =  0;
    int sm1859 = 0;
    int sw1854 = 0;
    int sm60_ =  0;
    int sw55_ =  0;
    int ssum =   0;

    int n = 0;

    // переберём медорганизации
    while (query_dstrs->next()) {

        int i_mo = query_dstrs->value(0).toInt();
        QString code_mo = query_dstrs->value(1).toString();
        QString text = query_dstrs->value(2).toString();

        int m_4 =   0;
        int w_4 =   0;
        int m517 =  0;
        int w517 =  0;
        int m1859 = 0;
        int w1854 = 0;
        int m60_ =  0;
        int w55_ =  0;
        int sum =   0;

        // переберём персоны
        query_pers->first();
        int m = 0;
        while (query_pers->next()) {
            if (m==0) {
                query_pers->first();
            }
            m++;

            QString id_person = QString::number(query_pers->value(0).toInt());

            // выберем данные этого человека, как прикреплённого к данной МО
            QString sql = "";
            sql += "select e.id, "
                         " e.sex,"
                         " e.date_birth ";
            sql += " from  persons e "
                         " left join polises p on ( p.id_person=e.id and p.pol_date_begin<='" + date_str + "' and (p.pol_date_end is NULL or p.pol_date_end>='" + date_str + "') and (p.pol_date_stop is NULL or p.pol_date_stop>='" + date_str + "') ) "
                         " left join persons_assig asg on ( asg.id_person=e.id and asg.is_actual=0 ) "
                         " left join spr_medorgs m on ( m.code_mo=asg.code_mo and m.is_head=1 ) ";
            sql += " where asg.code_mo='" + code_mo + "' and "
                         " e.id=" + id_person + " and "
                         " p.id is not NULL and "
                         " p.pol_date_begin<='" + date_str + "' and "
                         " (p.pol_date_stop is NULL or p.pol_date_stop>'" + date_str + "') ; ";

            QSqlQuery *query = new QSqlQuery(db);
            bool res = mySQL.exec(this, sql, "Запросим персону, прикреплённую к данной МО", *query, true);

            if (res && query->size()>0) {
                query->next();
                // получим данне застрахованного
                int sex = query->value(1).toInt();
                QDate date_birth = query->value(2).toDate();

                // добавим запись к счётчикам
                if (sex==1) {
                    if (date_birth>date_05) {
                        m_4++;
                        sm_4++;
                    } else if (date_birth>date_18) {
                        m517++;
                        sm517++;
                    } else if (date_birth>date_60) {
                        m1859++;
                        sm1859++;
                    } else {
                        m60_++;
                        sm60_++;
                    }
                } else {
                    if (date_birth>date_05) {
                        w_4++;
                        sw_4++;
                    } else if (date_birth>date_18) {
                        w517++;
                        sw517++;
                    } else if (date_birth>date_55) {
                        w1854++;
                        sw1854++;
                    } else {
                        w55_++;
                        sw55_++;
                    }
                }
                sum++;
                ssum++;
            }
        };

        n++;
        QString s_row_new = s_row;
        s_row_new = s_row_new.
                replace("#N#",    QString::number(n)).
                replace("#TEXT#", text).
                replace("#MO#",   code_mo).
                replace("#M_4#",  (m_4>0  ? QString::number(m_4)  : " ")).
                replace("#W_4#",  (w_4>0  ? QString::number(w_4)  : " ")).
                replace("#M517#", (m517>0 ? QString::number(m517) : " ")).
                replace("#W517#", (w517>0 ? QString::number(w517) : " ")).
                replace("#M1859#",(m1859>0? QString::number(m1859): " ")).
                replace("#W1854#",(w1854>0? QString::number(w1854): " ")).
                replace("#M60_#", (m60_>0 ? QString::number(m60_) : " ")).
                replace("#W55_#", (w55_>0 ? QString::number(w55_) : " ")).
                replace("#SUM#",  QString::number(sum));
        s_content_new += s_row_new;
    }
    s_content_new += s_content.right(s_content.size()-pos1);

    s_content_new = s_content_new.
            replace("#SM_4#",  QString::number(sm_4)).
            replace("#SW_4#",  QString::number(sw_4)).
            replace("#SM517#", QString::number(sm517)).
            replace("#SW517#", QString::number(sw517)).
            replace("#SM1859#",QString::number(sm1859)).
            replace("#SW1854#",QString::number(sw1854)).
            replace("#SM60_#", QString::number(sm60_)).
            replace("#SW55_#", QString::number(sw55_)).
            replace("#SSUM#",  QString::number(ssum)).

            replace("#SMO_NAME#", data_app.smo_name).
            replace("#DATE_REP#", date_cur.toString("dd.MM.yyyy")).
            replace("#TERR_NAME_RP#", data_app.terr_name_rp).

            replace("#FILIAL_DIRECTOR#",data_app.filial_director).
            replace("#FILIAL_DIRECTOR_2#",data_app.filial_director_2).
            replace("#FILIAL_DIRECTOR_FIO#",data_app.filial_director_fio);

    delete query_dstrs;

    // сохранение контента шаблона
    QFile f_content_save(temp_folder + "/content.xml");
    f_content_save.open(QIODevice::WriteOnly);
    f_content_save.write(s_content_new.toUtf8());
    f_content_save.close();


    // ===========================
    // архивация шаблона
    // ===========================
    QString fname_res = data_app.path_temp + "assig_sexage_all_mo_tab.ods";
    QFile::remove(fname_res);
    while (QFile::exists(fname_res)) {
        if (QMessageBox::warning(this,
                                 "Ошибка сохранения журнала ВС",
                                 "Не удалось сохранить форму половозрастной таблицы прикреплённых к МО: \n" + fname_template +
                                 "\n\nво временную папку\n" + fname_res +
                                 "\n\nПроверьте, не открыт ли целевой файл в сторонней программе и повторите операцию.",
                                 QMessageBox::Retry|QMessageBox::Abort,
                                 QMessageBox::Retry)==QMessageBox::Abort) {
            this->setCursor(Qt::ArrowCursor);
            return;
        }
        QFile::remove(fname_res);
    }
    if (!QFile::copy(fname_template, fname_res) || !QFile::exists(fname_res)) {
        QMessageBox::warning(this, "Ошибка копирования отчёта", "Не удалось скопировать форму половозрастной таблицы прикреплённых к МО: \n" + fname_template + "\n\nво временную папку\n" + fname_res + "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        return;
    }
    com = "\"" + data_app.path_arch + "7z.exe\" a \"" + fname_res + "\" \"" + temp_folder + "/content.xml\"";
    if (myProcess.execute(com)!=0) {
        QMessageBox::warning(this,
                             "Ошибка обновления контента",
                             "При добавлении нового контента в форму половозрастной таблицы прикреплённых к МО произошла непредвиденная ошибка\n\nОпреация прервана.");
    }

    // ===========================
    // собственно открытие шаблона
    // ===========================
    // открытие полученного ODT
    long result = (long)ShellExecute(0, 0, reinterpret_cast<const WCHAR*>(fname_res.utf16()), 0, 0, SW_NORMAL);

    //close();
    */
}


void autoassig_wnd::on_ch_assig_1_clicked(bool checked) {
    if (!checked) {
        ui->ch_assig_2->setChecked(false);
        ui->ch_assig_3->setChecked(false);
        ui->ch_assig_4->setChecked(false);
    }
    ui->bn_assig->setEnabled (ui->ch_assig_1->isChecked() ||
                              ui->ch_assig4age->isChecked() ||
                              ui->ch_assig_2->isChecked() ||
                              ui->ch_assig_3->isChecked() ||
                              ui->ch_assig_4->isChecked() );
}

void autoassig_wnd::on_ch_assig4age_clicked(bool checked) {
    if (!checked) {
        ui->ch_assig_2->setChecked(false);
        ui->ch_assig_3->setChecked(false);
        ui->ch_assig_4->setChecked(false);
    }
    ui->bn_assig->setEnabled (ui->ch_assig_1->isChecked() ||
                              ui->ch_assig4age->isChecked() ||
                              ui->ch_assig_2->isChecked() ||
                              ui->ch_assig_3->isChecked() ||
                              ui->ch_assig_4->isChecked() );
}

void autoassig_wnd::on_ch_assig_2_clicked(bool checked) {
    if (checked) {
        ui->ch_assig_1->setChecked(true);
    }
    if (!checked) {
        ui->ch_assig_3->setChecked(false);
        ui->ch_assig_4->setChecked(false);
    }
    ui->bn_assig->setEnabled (ui->ch_assig_1->isChecked() ||
                              ui->ch_assig4age->isChecked() ||
                              ui->ch_assig_2->isChecked() ||
                              ui->ch_assig_3->isChecked() ||
                              ui->ch_assig_4->isChecked() );
}

void autoassig_wnd::on_ch_assig_3_clicked(bool checked) {
    if (checked) {
        ui->ch_assig_1->setChecked(true);
        ui->ch_assig_2->setChecked(true);
    }
    if (!checked) {
        ui->ch_assig_4->setChecked(false);
    }
    ui->bn_assig->setEnabled (ui->ch_assig_1->isChecked() ||
                              ui->ch_assig4age->isChecked() ||
                              ui->ch_assig_2->isChecked() ||
                              ui->ch_assig_3->isChecked() ||
                              ui->ch_assig_4->isChecked() );
}

void autoassig_wnd::on_ch_assig_4_clicked(bool checked) {
    if (checked) {
        ui->ch_assig_1->setChecked(true);
        ui->ch_assig_2->setChecked(true);
        ui->ch_assig_3->setChecked(true);
    }
    ui->bn_assig->setEnabled (ui->ch_assig_1->isChecked() ||
                              ui->ch_assig4age->isChecked() ||
                              ui->ch_assig_2->isChecked() ||
                              ui->ch_assig_3->isChecked() ||
                              ui->ch_assig_4->isChecked() );
}

void autoassig_wnd::on_ch_unsig_1_clicked(bool checked) {
    if (!checked) {
        ui->ch_unsig_2->setChecked(false);
        ui->ch_unsig_3->setChecked(false);
    }
    ui->bn_unsig->setEnabled (ui->ch_unsig_1->isChecked() ||
                              ui->ch_unsig_2->isChecked() ||
                              ui->ch_unsig_3->isChecked() );
}

void autoassig_wnd::on_ch_unsig_2_clicked(bool checked) {
    if (checked) {
        ui->ch_unsig_1->setChecked(true);
    }
    if (!checked) {
        ui->ch_unsig_3->setChecked(false);
    }
    ui->bn_unsig->setEnabled (ui->ch_unsig_1->isChecked() ||
                              ui->ch_unsig_2->isChecked() ||
                              ui->ch_unsig_3->isChecked() );
}

void autoassig_wnd::on_ch_unsig_3_clicked(bool checked) {
    if (checked) {
        ui->ch_unsig_1->setChecked(true);
        ui->ch_unsig_2->setChecked(true);
    }
    ui->bn_unsig->setEnabled (ui->ch_unsig_1->isChecked() ||
                              ui->ch_unsig_2->isChecked() ||
                              ui->ch_unsig_3->isChecked() );
}

void autoassig_wnd::on_ch_clear_data_1_clicked(bool checked) {
    if (!checked) {
        ui->ch_clear_data_2->setChecked(false);
        ui->ch_clear_data_3->setChecked(false);
    }
    ui->bn_clear_data->setEnabled (ui->ch_clear_data_1->isChecked() ||
                                   ui->ch_clear_data_2->isChecked() ||
                                   ui->ch_clear_data_3->isChecked() );
}

void autoassig_wnd::on_ch_clear_data_2_clicked(bool checked) {
    if (checked) {
        ui->ch_clear_data_1->setChecked(true);
    }
    if (!checked) {
        ui->ch_clear_data_3->setChecked(false);
    }
    ui->bn_clear_data->setEnabled (ui->ch_clear_data_1->isChecked() ||
                                   ui->ch_clear_data_2->isChecked() ||
                                   ui->ch_clear_data_3->isChecked() );
}

void autoassig_wnd::on_ch_clear_data_3_clicked(bool checked) {
    if (checked) {
        ui->ch_clear_data_1->setChecked(true);
        ui->ch_clear_data_2->setChecked(true);
    }
    ui->bn_clear_data->setEnabled (ui->ch_clear_data_1->isChecked() ||
                                   ui->ch_clear_data_2->isChecked() ||
                                   ui->ch_clear_data_3->isChecked() );
}

void autoassig_wnd::on_bn_cansel_clicked() {
    close();
}

void autoassig_wnd::on_bn_assig_clicked() {
    ui->lab_cnt->setText("0 / 0");
    ui->lab_cnt_2->setText("0");

    // запрос подтверждения
    QString txt = "Выполнить автоматическую замену прикрепления всех застрахованных:\n\n";
    if (ui->ch_assig_1->isChecked()) {
        txt += " 1) не прикреплённых к МО\n";
    }
    if (ui->ch_assig4age->isChecked()) {
        txt += " 2) прикрепленных с нарушением ограничения по возрасту\n";
    }
    if (ui->ch_assig_2->isChecked()) {
        txt += " 3) прикреплённых без указания способа прикрепления\n";
    }
    if (ui->ch_assig_3->isChecked()) {
        txt += " 4) прикреплённых по территориальному принципу\n";
    }
    if (ui->ch_assig_4->isChecked()) {
        txt += " 5) прикреплённых по дичным заявлениям\n";
    }
    if (QMessageBox::question(this, "Нужно подтверждение", txt, QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
        return;
    }

    // выберем людей для прикрепления
    QString sql_pers = "select e.id, a.code_mo, a.code_mp, a.code_ms, a.snils_mt, "
                       "       e.id_addr_reg, ar.subj, ar.dstr, ar.city, ar.nasp, ar.strt, ar.house, ar.corp, ar.quart, "
                       "       e.id_addr_liv, al.subj, al.dstr, al.city, al.nasp, al.strt, al.house, al.corp, al.quart, "
                       "       e.sex, e.date_birth, coalesce(e.enp, p.enp) as enp "
                       "  from persons e "
                       "  left join polises p on(p.id=e.id_polis) "
                       "  left join persons_assig a on(a.id_person=e.id and a.assig_date<=CURRENT_DATE and (a.unsig_date is NULL or a.unsig_date>CURRENT_DATE)) "
                       "  left join addresses ar on(ar.id=e.id_addr_reg) "
                       "  left join addresses al on(al.id=e.id_addr_liv) "
                       "  left join spr_medorgs mo on(mo.code_mo=a.code_mo) "
                       " where e.status>0 "
                       "   and ( (1=2) ";
    if (ui->ch_assig_1->isChecked())
            sql_pers+= "      or (a.code_mo is NULL) ";
    if (ui->ch_assig4age->isChecked())
            sql_pers+= "      or (mo.mo_assig_age<>0  and  mo.mo_assig_age<>(case when e.date_birth>'1999-12-13' then 1 else 2 end)) ";
    if (ui->ch_assig_2->isChecked())
            sql_pers+= "      or (a.code_mo is not NULL and (a.assig_type is NULL or a.assig_type=0)) ";
    if (ui->ch_assig_3->isChecked())
            sql_pers+= "      or (a.code_mo is not NULL and (a.assig_type is not NULL and a.assig_type=1)) ";
    if (ui->ch_assig_4->isChecked())
            sql_pers+= "      or (a.code_mo is not NULL and (a.assig_type is not NULL and a.assig_type=2)) ";
    sql_pers += ") ; ";


    ui->bn_cansel->setEnabled(false);


    QSqlQuery *query_pers = new QSqlQuery(db);
    if ( mySQL.exec(this, sql_pers, "Массовое терр-прикрепление", *query_pers, true, db, data_app) ) {

        db.transaction();

        QString cur_date = QDate::currentDate().toString("yyyy-MM-dd");
        int n = 0;
        int n_assig = 0;
        int n_decline = 0;
        ui->te_log->appendPlainText("Автоматическое прикрепление ...");
        if (ui->ch_unsig_if_fail->isChecked()) {
            ui->te_log->appendPlainText("  ... с откреплением, если автоприкрепление не удалось.");
        }
        QApplication::processEvents();

        while (query_pers->next()) {
            n++;
            ui->lab_cnt_2->setText(QString::number(n));
            QApplication::processEvents();

            int id_person = query_pers->value(0).toInt();
            QString code_mo = query_pers->value(1).toString();
            QString code_mp = query_pers->value(2).toString();
            QString code_ms = query_pers->value(3).toString();
            QString snils_mt = query_pers->value(4).toString();

            int sex = query_pers->value(23).toInt();
            QDate date_birth = query_pers->value(24).toDate();
            QString enp = query_pers->value(25).toString();

            /*
            // проверка автоприкрепления по адресу регистрации
            QString sql_auto = "";
            int subj, dstr, city, nasp, strt, house;
            QString corp;
            int age = 0;
            if (date_birth>QDate::currentDate().addYears(-18))
                age = 1;
            else if (date_birth<=QDate::currentDate().addYears(-18))
                age = 2;
            else age = 0;

            if ( !(query_pers->value(5).isNull()) ) {
                subj = query_pers->value(6).toInt();
                dstr = query_pers->value(7).toInt();
                city = query_pers->value(8).toInt();
                nasp = query_pers->value(9).toInt();
                strt = query_pers->value(10).toInt();
                house = query_pers->value(11).toInt();
                corp = query_pers->value(12).toString();

                // запросим параметры автоприкрепления
                sql_auto = "select * "
                           "  from assig_by_terr "
                           " where id in( select * "
                                        " from assig_test( " + QString::number(subj) + ", "
                                                         " " + QString::number(dstr) + ", "
                                                         " " + QString::number(city) + ", "
                                                         " " + QString::number(nasp) + ", "
                                                         " " + QString::number(strt) + ", "
                                                         " " + QString::number(house) + ", "
                                                         " '" + corp + "', "
                                                         " " + QString::number(sex) + ", ";
                sql_auto += QString::number(age);
                sql_auto += ") ) ; ";
            };

            // проверка автоприкрепления по адресу проживания
            QString sql_auto1 = "";
            int subj1, dstr1, city1, nasp1, strt1, house1;
            QString corp1;

            if ( !(query_pers->value(14).isNull()) ) {
                subj1 = query_pers->value(15).toInt();
                dstr1 = query_pers->value(16).toInt();
                city1 = query_pers->value(17).toInt();
                nasp1 = query_pers->value(18).toInt();
                strt1 = query_pers->value(19).toInt();
                house1 = query_pers->value(20).toInt();
                corp1 = query_pers->value(21).toString();

                // запросим параметры автоприкрепления
                sql_auto1 = "select * "
                            "  from assig_by_terr "
                            " where id in( select * "
                                          " from assig_test( " + QString::number(subj1) + ", "
                                                           " " + QString::number(dstr1) + ", "
                                                           " " + QString::number(city1) + ", "
                                                           " " + QString::number(nasp1) + ", "
                                                           " " + QString::number(strt1) + ", "
                                                           " " + QString::number(house1) + ", "
                                                           " '" + corp1 + "', "
                                                           " " + QString::number(sex) + ", ";
                sql_auto1 += QString::number(age);
                sql_auto1 += ") ) ; ";
            }
            */

            // проверим, можно ли делать автоприкрепление
            int assig_type = 0;
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

            // если терр-прикрепление допустимо ...
            if ( !enp.isEmpty()     // если ЕНП уже известен   ---   ki - пока не знаю, надо ли так делать...
              && assig_type<2) {    // и способ прикрепления не указан или по территории (можно обновить)

                // ... собственно прикрепление
                QString sql = "select assig_proc(" + QString::number(id_person) + ") ; ";
                QSqlQuery *query = new QSqlQuery(db);

                if (!mySQL.exec(this, sql, "Добавление прикрепления к МО", *query, true, db, data_app) || !query->next()) {
                    db.rollback();
                    ui->te_log->appendPlainText(" !!! ОТКАЗ !!!   не удалось добавить новое прикрепление");
                    QApplication::processEvents();
                    QMessageBox::critical(this,
                                          "Непредвиденная ошибка",
                                          "При попытке добавить прикрепление застрахованного к МО произошла неожиданная ошибка.\n\n"
                                          "Сохранение отменено.");
                    delete query;
                    ui->bn_cansel->setEnabled(true);
                    return;
                }
                int id_assig = query->value(0).toInt();
                if (id_assig<0) {
                    // добавлена новая запись
                    n_assig++;
                } else if (id_assig>0) {
                    // такое прикрепление уже есть
                } else {
                    n_decline++;
                }
                delete query;
            }



            /*

            QSqlQuery *query_auto = new QSqlQuery(db);
            QSqlQuery *query_auto1 = new QSqlQuery(db);
            bool res_auto  = !sql_auto.isEmpty()  && mySQL.exec(this, sql_auto,  "Автоматическое терр-прикрепление персоны", *query_auto,  true, db, data_app);
            bool res_auto1 = !sql_auto1.isEmpty() && mySQL.exec(this, sql_auto1, "Автоматическое терр-прикрепление персоны", *query_auto1, true, db, data_app);

            if ( res_auto && query_auto->next()) {
                int id_assig = query_auto->value(0).toInt();

                QString code_mo = query_auto->value(11).toString();
                QString code_mp = query_auto->value(12).toString();
                QString code_ms = query_auto->value(13).toString();
                QString snils_mt= query_auto->value(14).toString();
                int assig_type = 1;

                QString code_mo_old;
                QString code_mp_old;
                QString code_ms_old;
                QString snils_mt_old;
                int assig_type_old;


                // проверка связи врача с таблицей участков
                QString sql_medter;
                sql_medter += "select mt.snils_mt "
                              "  from spr_ms_r_mt r "
                              "       join spr_medsites ms on(ms.id=r.id_ms) "
                              "       join spr_medters  mt on(mt.id=r.id_mt) "
                              " where ms.code_mo='" + code_mo + "' and "
                              "       ms.code_mp='" + code_mp + "' and "
                              "       ms.code_ms='" + code_ms + "' "
                              " order by r.prof_mt ASC ; ";

                QSqlQuery *query_medter = new QSqlQuery(db);
                if ( mySQL.exec(this, sql_medter, "Проверка терапевта по справочнику участков", *query_medter, true, db, data_app) &&
                     query_medter->size()>0 &&
                     query_medter->next() ) {
                    QString snils = query_medter->value(0).toString().simplified().trimmed();
                    if (snils.size()==11) {
                        snils_mt = snils;
                    }
                }
                delete query_medter;


                // сравнение действующего и нового прикреплений
                QString sql_old;
                sql_old += "select a.code_mo, a.code_mp, a.code_ms, a.snils_mt, a.assig_type "
                           "  from persons_assig a "
                           " where a.id_person=" + QString::number(id_person) + " and a.assig_date<=CURRENT_DATE and (a.unsig_date is NULL or a.unsig_date>CURRENT_DATE) ; ";

                QSqlQuery *query_old = new QSqlQuery(db);
                if ( mySQL.exec(this, sql_old, "Сравнение действующего и нового прикреплений", *query_old, true, db, data_app) &&
                     query_old->size()>0 &&
                     query_old->next() ) {
                    code_mo_old = query_old->value(0).toString();
                    code_mp_old = query_old->value(1).toString();
                    code_ms_old = query_old->value(2).toString();
                    snils_mt_old = query_old->value(3).toString();
                    assig_type_old = query_old->value(4).toInt();

                    if ( code_mo==code_mo_old &&
                         code_mp==code_mp_old &&
                         code_ms==code_ms_old &&
                         snils_mt==snils_mt_old &&
                         assig_type==assig_type_old) {

                        // такое прикрепление уже есть - пропустим
                        n_decline++;
                        delete query_old;
                        continue;
                    }
                }
                delete query_old;

                // добавим терр-прикрепление
                // зададим дату открепления прошлого прикрепления
                QString sql_close = "update persons_assig "
                                    "   set unsig_date='" + QDate::currentDate().toString("yyyy-MM-dd") + "'  "
                                    " where id_person=" + QString::number(id_person) + " and unsig_date is NULL ; ";


                QSqlQuery *query_close = new QSqlQuery(db);
                if (!mySQL.exec(this, sql_close, "Закрытие старого прикрепления", *query_close, true, db, data_app)) {
                    db.rollback();
                    ui->te_log->appendPlainText(" !!! ОТКАЗ !!!   не удалось закрыть старые прикрепления");
                    QApplication::processEvents();
                    QMessageBox::critical(this,
                                          "Непредвиденная ошибка",
                                          "При попытке задания даты открепления старого прикрепления произошла неожиданная ошибка.\n\n"
                                          "Сохранение отменено.");
                    delete query_close;
                    delete query_auto;
                    delete query_auto1;
                    delete query_pers;
                    ui->bn_cansel->setEnabled(true);
                    return;
                }
                delete query_close;


                // добавим новое прикрепление
                QString sql = "update persons_assig "
                              "   set is_actual = is_actual +1 "
                              " where id_person = " + QString::number(id_person) + " ; "

                              "insert into persons_assig (id_person, code_mo, code_mp, code_ms, snils_mt, assig_type, assig_way, assig_date, unsig_date, is_actual) "
                              " values "
                              " ( "  + QString::number(id_person) +
                              "  ,'" + code_mo.left(6) + "' "
                              "  ,'" + code_mp.left(64) + "' "
                              "  ,'" + code_ms.left(64) + "' "
                              "  ,'" + snils_mt.trimmed().simplified().replace(" ","").replace("-","").left(11) + "' "
                              "  ,"  + QString::number(assig_type) +
                              "  ,NULL "
                              "  ,'" + cur_date + "' "
                              "  ,NULL,0 ) "
                              " returning id; ";



                QSqlQuery *query = new QSqlQuery(db);
                if (!mySQL.exec(this, sql, "Добавление прикрепления к МО", *query, true, db, data_app) || !query->next()) {
                    db.rollback();
                    ui->te_log->appendPlainText(" !!! ОТКАЗ !!!   не удалось добавить новое прикрепление");
                    QApplication::processEvents();
                    QMessageBox::critical(this,
                                          "Непредвиденная ошибка",
                                          "При попытке добавить прикрепление застрахованного к МО произошла неожиданная ошибка.\n\n"
                                          "Сохранение отменено.");
                    delete query;
                    delete query_auto;
                    delete query_auto1;
                    delete query_pers;
                    ui->bn_cansel->setEnabled(true);
                    return;
                }
                delete query;


                bool res = reg_assig_by_terr_ok(data_app.ocato, subj, dstr, city, nasp, strt, house, corp, sex, age);
                n_assig++;

            } else if ( res_auto1 && query_auto1->next()) {
                int id_assig = query_auto1->value(0).toInt();

                QString code_mo = query_auto1->value(11).toString();
                QString code_mp = query_auto1->value(12).toString();
                QString code_ms = query_auto1->value(13).toString();
                QString snils_mt= query_auto1->value(14).toString();
                int assig_type = 1;

                QString code_mo_old;
                QString code_mp_old;
                QString code_ms_old;
                QString snils_mt_old;
                int assig_type_old;


                // проверка связи врача с таблицей участков
                QString sql_medter;
                sql_medter += "select mt.snils_mt "
                              "  from spr_ms_r_mt r "
                              "       join spr_medsites ms on(ms.id=r.id_ms) "
                              "       join spr_medters  mt on(mt.id=r.id_mt) "
                              " where ms.code_mo='" + code_mo + "' and "
                              "       ms.code_mp='" + code_mp + "' and "
                              "       ms.code_ms='" + code_ms + "' "
                              " order by r.prof_mt ASC ; ";

                QSqlQuery *query_medter = new QSqlQuery(db);
                if ( mySQL.exec(this, sql_medter, "Проверка терапевта по справочнику участков", *query_medter, true, db, data_app) &&
                     query_medter->size()>0 &&
                     query_medter->next() ) {
                    QString snils = query_medter->value(0).toString().simplified().trimmed();
                    if (snils.size()==11) {
                        snils_mt = snils;
                    }
                }
                delete query_medter;


                // сравнение действующего и нового прикреплений
                QString sql_old;
                sql_old += "select a.code_mo, a.code_mp, a.code_ms, a.snils_mt, a.assig_type "
                           "  from persons_assig a "
                           " where a.id_person=" + QString::number(id_person) + " and a.assig_date<=CURRENT_DATE and (a.unsig_date is NULL or a.unsig_date>CURRENT_DATE) ; ";

                QSqlQuery *query_old = new QSqlQuery(db);
                if ( mySQL.exec(this, sql_old, "Сравнение действующего и нового прикреплений", *query_old, true, db, data_app) &&
                     query_old->size()>0 &&
                     query_old->next() ) {
                    code_mo_old = query_old->value(0).toString();
                    code_mp_old = query_old->value(1).toString();
                    code_ms_old = query_old->value(2).toString();
                    snils_mt_old = query_old->value(3).toString();
                    assig_type_old = query_old->value(4).toInt();

                    if ( code_mo==code_mo_old &&
                         code_mp==code_mp_old &&
                         code_ms==code_ms_old &&
                         snils_mt==snils_mt_old &&
                         assig_type==assig_type_old) {

                        // такое прикрепление уже есть - пропустим
                        n_decline++;
                        delete query_old;
                        continue;
                    }
                }
                delete query_old;

                // добавим терр-прикрепление
                // зададим дату открепления прошлого прикрепления
                QString sql_close = "update persons_assig "
                                    "   set unsig_date='" + QDate::currentDate().toString("yyyy-MM-dd") + "'  "
                                    " where id_person=" + QString::number(id_person) + " and unsig_date is NULL ; ";


                QSqlQuery *query_close = new QSqlQuery(db);
                if (!mySQL.exec(this, sql_close, "Закрытие старого прикрепления", *query_close, true, db, data_app)) {
                    db.rollback();
                    ui->te_log->appendPlainText(" !!! ОТКАЗ !!!   не удалось закрыть старые прикрепления");
                    QApplication::processEvents();
                    QMessageBox::critical(this,
                                          "Непредвиденная ошибка",
                                          "При попытке задания даты открепления старого прикрепления произошла неожиданная ошибка.\n\n"
                                          "Сохранение отменено.");
                    delete query_close;
                    delete query_auto;
                    delete query_auto1;
                    delete query_pers;
                    ui->bn_cansel->setEnabled(true);
                    return;
                }
                delete query_close;


                // добавим новое прикрепление
                QString sql = "update persons_assig "
                              "   set is_actual = is_actual +1 "
                              " where id_person = " + QString::number(id_person) + " ; "

                              "insert into persons_assig (id_person, code_mo, code_mp, code_ms, snils_mt, assig_type, assig_way, assig_date, unsig_date, is_actual) "
                              " values "
                              " ( "  + QString::number(id_person) +
                              "  ,'" + code_mo.left(6) + "' "
                              "  ,'" + code_mp.left(64) + "' "
                              "  ,'" + code_ms.left(64) + "' "
                              "  ,'" + snils_mt.trimmed().simplified().replace(" ","").replace("-","").left(11) + "' "
                              "  ,"  + QString::number(assig_type) +
                              "  ,NULL "
                              "  ,'" + cur_date + "' "
                              "  ,NULL,0 ) "
                              " returning id; ";



                QSqlQuery *query = new QSqlQuery(db);
                if (!mySQL.exec(this, sql, "Добавление прикрепления к МО", *query, true, db, data_app) || !query->next()) {
                    db.rollback();
                    ui->te_log->appendPlainText(" !!! ОТКАЗ !!!   не удалось добавить новое прикрепление");
                    QApplication::processEvents();
                    QMessageBox::critical(this,
                                          "Непредвиденная ошибка",
                                          "При попытке добавить прикрепление застрахованного к МО произошла неожиданная ошибка.\n\n"
                                          "Сохранение отменено.");
                    delete query;
                    delete query_auto;
                    delete query_auto1;
                    delete query_pers;
                    ui->bn_cansel->setEnabled(true);
                    return;
                }
                delete query;


                bool res = reg_assig_by_terr_ok(data_app.ocato, subj1, dstr1, city1, nasp1, strt1, house1, corp1, sex, age);
                n_assig++;


            } else {
                // правило автоприкрепления не найдено - отмена
                n_decline ++;
                if (!sql_auto.isEmpty())  reg_assig_by_terr_gap(data_app.ocato, subj,  dstr,  city,  nasp,  strt,  house,  corp,  sex, age);
                if (!sql_auto1.isEmpty()) reg_assig_by_terr_gap(data_app.ocato, subj1, dstr1, city1, nasp1, strt1, house1, corp1, sex, age);

                // при необходимости - отменим старое прикрепление
                if (ui->ch_unsig_if_fail->isChecked()) {

                    // зададим дату открепления прошлого прикрепления
                    QString sql_close = "update persons_assig "
                                        "   set unsig_date='" + QDate::currentDate().toString("yyyy-MM-dd") + "'  "
                                        " where id_person=" + QString::number(id_person) + " and unsig_date is NULL ; ";

                    QSqlQuery *query_close = new QSqlQuery(db);
                    mySQL.exec(this, sql_close, "Закрытие старого прикрепления", *query_close, true, db, data_app);
                    delete query_close;
                }
            }

            delete query_auto;
            delete query_auto1;
            */



            //if ( (n_assig+n_decline)/9==0 ) {
            ui->lab_cnt->setText(QString::number(n_assig) + " / " + QString::number(n_decline));
            QApplication::processEvents();
            //}
        }
        delete query_pers;

        ui->lab_cnt->setText(QString::number(n_assig) + " / " + QString::number(n_decline) + " - upd_links_asg()");
        ui->lab_cnt_2->setText(QString::number(n));
        QApplication::processEvents();

        // обновим связки персон с прикреплением
        ui->te_log->appendPlainText("Обновим связки персон ...");
        QApplication::processEvents();

        QString sql_link = "select * from update_pers_links_asg(CURRENT_DATE, 10); ";
        QSqlQuery *query_link = new QSqlQuery(db);
        if (!mySQL.exec(this, sql_link, "обновим связки персон с прикреплением", *query_link, true, db, data_app)) {
            db.rollback();
            ui->te_log->appendPlainText(" !!! ОТКАЗ !!!   не удалось обновить связки персон с прикреплением");
            QApplication::processEvents();
            QMessageBox::critical(this,
                                  "Непредвиденная ошибка",
                                  "При попытке обновить связки персон с прикреплением произошла неожиданная ошибка.\n\n"
                                  "Сохранение отменено.");
            delete query_link;
            ui->bn_cansel->setEnabled(true);
            return;
        }

        db.commit();
        ui->te_log->appendPlainText("\n ... успешно завершено\n");
        ui->te_log->appendPlainText(" прикреплено: " + QString::number(n_assig) + ";  отказано: " + QString::number(n_decline));
        ui->te_log->appendPlainText("");
    }

    QMessageBox::information(this, "Готово", "Готово.");
    ui->bn_cansel->setEnabled(true);
}

void autoassig_wnd::on_bn_unsig_clicked() {
    // запрос подтверждения
    QString txt = "Вы уверены, что хотите отменить прикрепления для всех застрахованных:\n\n";
    if (ui->ch_unsig_1->isChecked()) {
        txt += " 1) прикреплённых без указания способа прикрепления\n";
    }
    if (ui->ch_unsig_2->isChecked()) {
        txt += " 2) прикреплённых по территориальному принципу\n";
    }
    if (ui->ch_unsig_3->isChecked()) {
        txt += " 3) прикреплённых по дичным заявлениям\n";
    }
    if (QMessageBox::warning(this, "ВНИМАНИЕ !!!", txt, QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
        return;
    }

    // выберем людей для прикрепления
    QString sql_pers = "select e.id, a.code_mo, a.code_mp, a.code_ms, a.snils_mt, "
                       "       e.id_addr_reg, ar.subj, ar.dstr, ar.city, ar.nasp, ar.strt, ar.house, ar.corp, ar.quart, "
                       "       e.id_addr_liv, al.subj, al.dstr, al.city, al.nasp, al.strt, al.house, al.corp, al.quart, "
                       "       e.sex, e.date_birth "
                       " from persons e "
                       "      left join persons_assig a on(a.id_person=e.id and a.is_actual=0) "
                       "      left join addresses ar on(ar.id=e.id_addr_reg) "
                       "      left join addresses al on(al.id=e.id_addr_liv) "
                       " where ";
    if (ui->ch_unsig_1->isChecked()) {
        sql_pers += " (a.code_mo is not NULL and (a.assig_type is NULL or a.assig_type=0)) ";
    }
    if (ui->ch_unsig_2->isChecked()) {
        sql_pers += " or (a.code_mo is not NULL and (a.assig_type is not NULL and a.assig_type=1)) ";
    }
    if (ui->ch_unsig_3->isChecked()) {
        sql_pers += " or (a.code_mo is not NULL and (a.assig_type is not NULL and a.assig_type=2)) ";
    }
    sql_pers += " ; ";



    QSqlQuery *query_pers = new QSqlQuery(db);
    if ( mySQL.exec(this, sql_pers, "Массовое открепление", *query_pers, true, db, data_app) ) {

        db.transaction();
        QString cur_date = QDate::currentDate().toString("yyyy-MM-dd");
        ui->te_log->appendPlainText("Массовое открепление ...");
        int n_unsig = 0;

        while (query_pers->next()) {
            int id_person = query_pers->value(0).toInt();

            // зададим дату открепления
            QString sql_close = "update persons_assig "
                                "   set unsig_date='" + QDate::currentDate().toString("yyyy-MM-dd") + "'  "
                                " where id_person=" + QString::number(id_person) + " and unsig_date is NULL ; ";
            QSqlQuery *query_close = new QSqlQuery(db);
            if (!mySQL.exec(this, sql_close, "Закрытие старого прикрепления", *query_close, true, db, data_app)) {
                db.rollback();
                ui->te_log->appendPlainText(" !!! ОТКАЗ !!!   не удалось закрыть прикрепления");
                QMessageBox::critical(this,
                                      "Непредвиденная ошибка",
                                      "При попытке задания даты открепления произошла неожиданная ошибка.\n\n"
                                      "Сохранение отменено.");
                delete query_close;
                return;
            }

            // это делается в триггере
            /*// "состарим" прикрепления
            QString sql_old = "update persons_assig "
                              "   set is_actual=is_actual +1 "
                              " where id_person=" + QString::number(id_person) + " ; ";

            QSqlQuery *query_old = new QSqlQuery(db);
            if (!mySQL.exec(this, sql_old, "Состарим старые прикрепления", *query_old, true)) {
                db.rollback();
                ui->te_log->appendPlainText(" !!! ОТКАЗ !!!   не удалось \"состарить\" прикрепления");
                QMessageBox::critical(this,
                                      "Непредвиденная ошибка",
                                      "При попытке \"состарить\" прикрепления произошла неожиданная ошибка.\n\n"
                                      "Сохранение отменено.");
                delete query_old;
                return;
            }
            */
            n_unsig++;
        }

        db.commit();
        ui->te_log->appendPlainText("\n ... успешно завершено\n");
        ui->te_log->appendPlainText(" откреплено: " + QString::number(n_unsig));
        ui->te_log->appendPlainText("");
    }
}


bool autoassig_wnd::reg_assig_by_terr_gap(QString ocato, int subj, int dstr, int city, int nasp, int strt, int house, QString corp, int sex, int age) {
    // проверим наличие готовой записи в таблице
    QString sql_tst =
            "select count(*) "
            " from assig_by_terr_gaps g "
            " where g.subj="  + QString::number(subj) +
            "   and g.dstr="  + QString::number(dstr) +
            "   and g.city="  + QString::number(city) +
            "   and g.nasp="  + QString::number(nasp) +
            "   and g.strt="  + QString::number(strt) +
            "   and g.house=" + QString::number(house)+
            "   and g.sex="   + QString::number(sex)  +
            "   and g.age="   + QString::number(age)  +
            "   and g.corp='" + corp + "' ; ";

    QSqlQuery* query_tst = new QSqlQuery(db);
    bool res_tst = mySQL.exec(this, sql_tst, QString("Проверка на наличие готовой записи в таблице необработанных автоприкреплений"), *query_tst, true, db, data_app);
    if (!res_tst) {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::critical(this,
                              "Ошибка при проверке на наличие готовой записи в таблице необраблоанных автоприкреплений!",
                              "При проверке на наличие готовой записи в таблице необработанных автоприкреплений произошла непредвиденная ошибка!\n");
        //data_polis.is_ok = false;
        delete query_tst;
        return false;
    }
    if (query_tst->size()>0 && query_tst->next()) {
        int cnt = query_tst->value(0).toInt();
        if (cnt<=0) {
            // добавим запись в таблицу необработанных автоприкреплений
            QString sql_add =
                    "insert into assig_by_terr_gaps(ocato, subj, dstr, city, nasp, strt, house, corp, sex, age, id_point, id_operator, dt_ins) "
                    " values('" + ocato + "', " +
                    "        " + QString::number(subj) + ", " +
                    "        " + QString::number(dstr) + ", " +
                    "        " + QString::number(city) + ", " +
                    "        " + QString::number(nasp) + ", " +
                    "        " + QString::number(strt) + ", " +
                    "        " + QString::number(house)+ ", " +
                    "        '" + corp + "', "
                    "        " + QString::number(sex)  + ", " +
                    "        " + QString::number(age)  + ", " +
                    "        " + QString::number(data_app.id_point) + ", " +
                    "        " + QString::number(data_app.id_operator)+ ", " +
                    "        CURRENT_TIMESTAMP) ; ";

            QSqlQuery* query_add = new QSqlQuery(db);
            bool res_add = mySQL.exec(this, sql_add, QString("Добавление записи в таблицу необработанных автоприкреплений"), *query_add, true, db, data_app);
            if (!res_add) {
                this->setCursor(Qt::ArrowCursor);
                QMessageBox::critical(this,
                                      "Ошибка при добавлении записи в таблицу необработанных автоприкреплений!",
                                      "При добавлении записи в таблицу необработанных автоприкреплений произошла непредвиденная ошибка!\n");
                delete query_add;
                return false;
            }
        } else {
            // добавление записи не требуется
        }
    } else {
        // запрос почему-то ничего не вернул ...
        // это странно :(
    }
    return true;
}

bool autoassig_wnd::reg_assig_by_terr_ok(QString ocato, int subj, int dstr, int city, int nasp, int strt, int house, QString corp, int sex, int age) {
    // проверим наличие готовой записи в таблице
    QString sql_del =
            "delete "
            " from assig_by_terr_gaps g "
            " where g.subj="  + QString::number(subj) +
            "   and g.dstr="  + QString::number(dstr) +
            "   and g.city="  + QString::number(city) +
            "   and g.nasp="  + QString::number(nasp) +
            "   and g.strt="  + QString::number(strt) +
            "   and g.house=" + QString::number(house)+
            "   and g.sex="   + QString::number(sex)  +
            "   and g.age="   + QString::number(age)  +
            "   and g.corp='" + corp + "' ; ";

    QSqlQuery* query_del = new QSqlQuery(db);
    bool res_del = mySQL.exec(this, sql_del, QString("Удаление записей из таблицы необработанных автоприкреплений"), *query_del, true, db, data_app);
    if (!res_del) {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::critical(this,
                              "Ошибка при удалении записей из таблицы необработанных автоприкреплений!",
                              "При удалении записей из таблицы необработанных автоприкреплений произошла непредвиденная ошибка!\n");
        //data_polis.is_ok = false;
        delete query_del;
        return false;
    }
    return true;
}
