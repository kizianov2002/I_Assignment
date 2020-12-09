#include "print_letters_wnd.h"
#include "ui_print_letters_wnd.h"
#include <QFileDialog>

print_letters_wnd::print_letters_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::print_letters_wnd)
{
    ui->setupUi(this);

    print_letters_data_w = NULL;

    refresh_data_tab();

    refresh_filter_fields();
    refresh_filter_values();
}

print_letters_wnd::~print_letters_wnd() {
    delete ui;
}

void print_letters_wnd::on_bn_close_clicked() {
    close();
}

void print_letters_wnd::refresh_filter_fields() {
    this->setCursor(Qt::WaitCursor);

    for (int i=1; i<51; i++) {
        ui->combo_filter_fields->addItem( "#DATA" + QString("00" + QString::number(i)).right(2) + "#", "data" + QString("00" + QString::number(i)).right(2) );
    }
    ui->combo_filter_fields->setCurrentIndex(-1);

    this->setCursor(Qt::ArrowCursor);
}

void print_letters_wnd::refresh_filter_values() {
    this->setCursor(Qt::WaitCursor);

    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "";
    sql += QString("select distinct #FIELD# "
                   "  from reports.letters_data_tab "
                   " order by #FIELD# ; ");
    sql = sql.replace("#FIELD#", ui->combo_filter_fields->currentData().toString());
    query->exec( sql );
    ui->combo_filter_values->clear();

    // заполнение списка пунктов выдачи
    int n = 0;
    while (query->next()) {
        ui->combo_filter_values->addItem( query->value(0).toString(), n );
        n++;
    }
    query->finish();
    delete query;
    ui->combo_filter_values->setCurrentIndex(-1);

    this->setCursor(Qt::ArrowCursor);
}

void print_letters_wnd::refresh_data_tab() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    this->setCursor(Qt::WaitCursor);

    data_sql  = "select id, dt_gen, data01, data02, data03, data04, data05, data06, data07, data08, data09, data10, "
                                  " data11, data12, data13, data14, data15, data16, data17, data18, data19, data20, "
                                  " data21, data22, data23, data24, data25, data26, data27, data28, data29, data30, "
                                  " data31, data32, data33, data34, data35, data36, data37, data38, data39, data40, "
                                  " data41, data42, data43, data44, data45, data46, data47, data48, data49, data50  "
                "  from reports.letters_data_tab "
                " where 1=1 " +
                QString( ui->rb_new->isChecked() ? " and dt_gen is NULL " : " " ) +
                QString( ui->ch_filter->isChecked() &&
                         !ui->combo_filter_fields->currentText().isEmpty() &&
                         !ui->combo_filter_values->currentText().isEmpty() ? QString(" and " + ui->combo_filter_fields->currentData().toString() + "='" + ui->combo_filter_values->currentText() + "' ")
                                                                           : QString(" ") ) +
                " order by id ; ";

    model_data.setQuery(data_sql,db);
    QString err2 = model_data.lastError().driverText();

    // подключаем модель из БД
    ui->tab_data->setModel(&model_data);

    // обновляем таблицу
    ui->tab_data->reset();

    // задаём ширину колонок
    ui->tab_data->setColumnWidth( 0, 35);   // id,
    ui->tab_data->setColumnWidth( 1,110);   // dt_gen

    ui->tab_data->setColumnWidth( 2,200);   // data01
    ui->tab_data->setColumnWidth( 3,150);   // data02
    ui->tab_data->setColumnWidth( 4,150);   // data03
    ui->tab_data->setColumnWidth( 5,150);   // data04
    ui->tab_data->setColumnWidth( 6,150);   // data05
    ui->tab_data->setColumnWidth( 7,150);   // data06
    ui->tab_data->setColumnWidth( 8,150);   // data07
    ui->tab_data->setColumnWidth( 9,150);   // data08
    ui->tab_data->setColumnWidth(10,150);   // data09
    ui->tab_data->setColumnWidth(11,150);   // data10

    ui->tab_data->setColumnWidth(12,150);   // data01
    ui->tab_data->setColumnWidth(13,150);   // data02
    ui->tab_data->setColumnWidth(14,150);   // data03
    ui->tab_data->setColumnWidth(15,150);   // data04
    ui->tab_data->setColumnWidth(16,150);   // data05
    ui->tab_data->setColumnWidth(17,150);   // data06
    ui->tab_data->setColumnWidth(18,150);   // data07
    ui->tab_data->setColumnWidth(19,150);   // data08
    ui->tab_data->setColumnWidth(20,150);   // data09
    ui->tab_data->setColumnWidth(21,150);   // data10

    ui->tab_data->setColumnWidth(22,150);   // data01
    ui->tab_data->setColumnWidth(23,150);   // data02
    ui->tab_data->setColumnWidth(24,150);   // data03
    ui->tab_data->setColumnWidth(25,150);   // data04
    ui->tab_data->setColumnWidth(26,150);   // data05
    ui->tab_data->setColumnWidth(27,150);   // data06
    ui->tab_data->setColumnWidth(28,150);   // data07
    ui->tab_data->setColumnWidth(29,150);   // data08
    ui->tab_data->setColumnWidth(30,150);   // data09
    ui->tab_data->setColumnWidth(31,150);   // data10

    ui->tab_data->setColumnWidth(32,150);   // data01
    ui->tab_data->setColumnWidth(33,150);   // data02
    ui->tab_data->setColumnWidth(34,150);   // data03
    ui->tab_data->setColumnWidth(35,150);   // data04
    ui->tab_data->setColumnWidth(36,150);   // data05
    ui->tab_data->setColumnWidth(37,150);   // data06
    ui->tab_data->setColumnWidth(38,150);   // data07
    ui->tab_data->setColumnWidth(39,150);   // data08
    ui->tab_data->setColumnWidth(40,150);   // data09
    ui->tab_data->setColumnWidth(41,150);   // data10

    ui->tab_data->setColumnWidth(42,150);   // data01
    ui->tab_data->setColumnWidth(43,150);   // data02
    ui->tab_data->setColumnWidth(44,150);   // data03
    ui->tab_data->setColumnWidth(45,150);   // data04
    ui->tab_data->setColumnWidth(46,150);   // data05
    ui->tab_data->setColumnWidth(47,150);   // data06
    ui->tab_data->setColumnWidth(48,150);   // data07
    ui->tab_data->setColumnWidth(49,150);   // data08
    ui->tab_data->setColumnWidth(50,150);   // data09
    ui->tab_data->setColumnWidth(51,150);   // data10
    // правим заголовки
    model_data.setHeaderData( 0, Qt::Horizontal, ("NN"));
    model_data.setHeaderData( 1, Qt::Horizontal, ("дата\nгенерации"));
    model_data.setHeaderData( 2, Qt::Horizontal, ("#DATA01#"));
    model_data.setHeaderData( 3, Qt::Horizontal, ("#DATA02#"));
    model_data.setHeaderData( 4, Qt::Horizontal, ("#DATA03#"));
    model_data.setHeaderData( 5, Qt::Horizontal, ("#DATA04#"));
    model_data.setHeaderData( 6, Qt::Horizontal, ("#DATA05#"));
    model_data.setHeaderData( 7, Qt::Horizontal, ("#DATA06#"));
    model_data.setHeaderData( 8, Qt::Horizontal, ("#DATA07#"));
    model_data.setHeaderData( 9, Qt::Horizontal, ("#DATA08#"));
    model_data.setHeaderData(10, Qt::Horizontal, ("#DATA09#"));
    model_data.setHeaderData(11, Qt::Horizontal, ("#DATA10#"));

    model_data.setHeaderData(12, Qt::Horizontal, ("#DATA11#"));
    model_data.setHeaderData(13, Qt::Horizontal, ("#DATA12#"));
    model_data.setHeaderData(14, Qt::Horizontal, ("#DATA13#"));
    model_data.setHeaderData(15, Qt::Horizontal, ("#DATA14#"));
    model_data.setHeaderData(16, Qt::Horizontal, ("#DATA15#"));
    model_data.setHeaderData(17, Qt::Horizontal, ("#DATA16#"));
    model_data.setHeaderData(18, Qt::Horizontal, ("#DATA17#"));
    model_data.setHeaderData(19, Qt::Horizontal, ("#DATA18#"));
    model_data.setHeaderData(20, Qt::Horizontal, ("#DATA19#"));
    model_data.setHeaderData(21, Qt::Horizontal, ("#DATA20#"));

    model_data.setHeaderData(22, Qt::Horizontal, ("#DATA21#"));
    model_data.setHeaderData(23, Qt::Horizontal, ("#DATA22#"));
    model_data.setHeaderData(24, Qt::Horizontal, ("#DATA23#"));
    model_data.setHeaderData(25, Qt::Horizontal, ("#DATA24#"));
    model_data.setHeaderData(26, Qt::Horizontal, ("#DATA25#"));
    model_data.setHeaderData(27, Qt::Horizontal, ("#DATA26#"));
    model_data.setHeaderData(28, Qt::Horizontal, ("#DATA27#"));
    model_data.setHeaderData(29, Qt::Horizontal, ("#DATA28#"));
    model_data.setHeaderData(30, Qt::Horizontal, ("#DATA29#"));
    model_data.setHeaderData(31, Qt::Horizontal, ("#DATA30#"));

    model_data.setHeaderData(32, Qt::Horizontal, ("#DATA31#"));
    model_data.setHeaderData(33, Qt::Horizontal, ("#DATA32#"));
    model_data.setHeaderData(34, Qt::Horizontal, ("#DATA33#"));
    model_data.setHeaderData(35, Qt::Horizontal, ("#DATA34#"));
    model_data.setHeaderData(36, Qt::Horizontal, ("#DATA35#"));
    model_data.setHeaderData(37, Qt::Horizontal, ("#DATA36#"));
    model_data.setHeaderData(38, Qt::Horizontal, ("#DATA37#"));
    model_data.setHeaderData(39, Qt::Horizontal, ("#DATA38#"));
    model_data.setHeaderData(40, Qt::Horizontal, ("#DATA39#"));
    model_data.setHeaderData(41, Qt::Horizontal, ("#DATA40#"));

    model_data.setHeaderData(42, Qt::Horizontal, ("#DATA41#"));
    model_data.setHeaderData(43, Qt::Horizontal, ("#DATA42#"));
    model_data.setHeaderData(44, Qt::Horizontal, ("#DATA43#"));
    model_data.setHeaderData(45, Qt::Horizontal, ("#DATA44#"));
    model_data.setHeaderData(46, Qt::Horizontal, ("#DATA45#"));
    model_data.setHeaderData(47, Qt::Horizontal, ("#DATA46#"));
    model_data.setHeaderData(48, Qt::Horizontal, ("#DATA47#"));
    model_data.setHeaderData(49, Qt::Horizontal, ("#DATA48#"));
    model_data.setHeaderData(50, Qt::Horizontal, ("#DATA49#"));
    model_data.setHeaderData(51, Qt::Horizontal, ("#DATA50#"));
    ui->tab_data->repaint();

    // число строк
    QSqlQuery *query_cnt = new QSqlQuery(db);
    QString sql_cnt = "select count(*) "
                      "  from reports.letters_data_tab "
                      " where 1=1 " +
                      QString( ui->rb_new->isChecked() ? " and dt_gen is NULL "
                                                       : " " ) +
                      QString( ui->ch_filter->isChecked() &&
                               !ui->combo_filter_fields->currentText().isEmpty() &&
                               !ui->combo_filter_values->currentText().isEmpty() ? QString(" and " + ui->combo_filter_fields->currentData().toString() + "='" + ui->combo_filter_values->currentText() + "' ")
                                                                                 : QString(" ") ) + " ; ";

    mySQL.exec(this, sql_cnt, "Число строк данных для генерации писем", query_cnt, false, db, data_app);

    if (query_cnt->next()) {
        int cnt = query_cnt->value(0).toInt();
        ui->lab_cnt->setText(QString::number(cnt));
    } else {
        ui->lab_cnt->setText("-//-");
    }

    this->setCursor(Qt::ArrowCursor);
}

void print_letters_wnd::on_bn_refresh_data_clicked() {
    refresh_data_tab();
}

void print_letters_wnd::on_bn_replace_data_clicked() {
    delete print_letters_data_w;
    print_letters_data_w = new print_letters_data_wnd(0, db, data_app, settings, this);
    print_letters_data_w->exec();

    refresh_data_tab();
}

void print_letters_wnd::on_bn_add_data_clicked() {
    delete print_letters_data_w;
    print_letters_data_w = new print_letters_data_wnd(1, db, data_app, settings, this);
    print_letters_data_w->exec();

    refresh_data_tab();
}

void print_letters_wnd::on_bn_del_data_clicked() {
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "truncate reports.letters_data_tab RESTART IDENTITY ; ";

    mySQL.exec(this, sql, "Очистка старых данных для генерации писем", query, false, db, data_app);

    refresh_data_tab();
}

void print_letters_wnd::on_bn_letter_file_clicked() {
    QString s = QFileDialog::getOpenFileName(this, "Выберите файл шаблона для генерации писем",
                                             data_app.path_reports + "Шаблоны_писем",
                                             tr("документы OpenOffice (*.odt *.ods *.odg)"));
    s = s.trimmed();
    if (!s.isEmpty())  ui->ln_letter_file->setText(s);
}


#include "qt_windows.h"
#include "qwindowdefs_win.h"
#include <shellapi.h>

void print_letters_wnd::on_bn_generate_clicked() {

    // подготовим имена файлов и папку для сохранения писем
    QString fname_template = ui->ln_letter_file->text();
    QFileInfo fi_template(fname_template);
    QString file_nam;
    QString file_dir;
    QString file_ext;
    QString file_dir_std;
    QString file_nam_std;

    if ( fname_template.isEmpty()
         || !fi_template.exists() ) {
        QMessageBox::warning(this, "Файл шаблона не найден",
                             "Перед запуском процесса генерации необходимо задать файл шаблона.\n\n"
                             "Файл шаблона должен быть в формате OpenOffice/LibreOffice.\n"
                             "Для вставки в текст файла полей из таблицы, в файле должны присутствовать ключевые слова #DATA01#, #DATA02#... (символы # обязательны и обрамляют ключевое слово без пробелов)\n\n"
                             "Выберите файл шаблона и повторите попытку.");
        this->setCursor(Qt::ArrowCursor);
        return;
    }

    file_nam  = fi_template.baseName();
    file_dir  = fi_template.dir().path() + "/";
    file_ext  = "." + fi_template.completeSuffix();
    file_dir_std = data_app.path_reports + "Шаблоны_писем/";
    file_nam_std = fi_template.baseName();

    if (file_dir!=file_dir_std) {
        if (QMessageBox::question(this, "Шаблон письма расположен не в стандартной папке хранения шаблонов",
                                  "Шаблон письма расположен не в той папке, в которой программа хранит шаблоны писем по умолчанию.\n\n"
                                  "Скопировать шаблон в стандартную папку?\n "
                                  "Это упростит его поиск при дальнейшем использовании.\n",
                                  QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)==QMessageBox::Yes) {
            file_nam_std = file_nam + file_ext;

            bool f_copy = true;
            while (QFile::exists(file_dir_std + file_nam_std)) {
                if (QMessageBox::question(this, "Шаблон письма с таким именем уже существует",
                                          "Имя копируемого шаблона письма совпадает с именем другого файла шаблона в стандартной папке хранения шаблонов писем.\n\n"
                                          "Заменить старый файл новым?\n "
                                          "В случае отказа генерация писем продолжится из нестандартной папки размещения шаблона.\n",
                                          QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)==QMessageBox::No) {
                    f_copy = false;
                    break;
                } else {
                    f_copy = true;
                    break;
                }
            }
            if (f_copy) {
                QFile::remove(file_dir_std + file_nam_std);
                QFile::copy(fname_template, file_dir_std + file_nam_std);
            }
        }
    }

    QString folder_save = data_app.path_out + "ПИСЬМА " + QDate::currentDate().toString("yyyy-MM-dd");
    QDir    dir_save(folder_save);
    if (!dir_save.exists()) {
        dir_save.mkpath(folder_save);
    }

    // для варианта "сформировать письма для всех строк"
    if ( ui->rb_all->isChecked()
         || ui->rb_new->isChecked() ) {

        db.transaction();

        QSqlQuery *query = new QSqlQuery(db);
        mySQL.exec(this, this->data_sql, "запрос отображаемых данных для генерации писем", query, false, db, data_app);

        int n = 0;
        while (query->next()) {
            int id = query->value( 0).toInt();
            QDateTime dt_gen = query->value( 1).toDateTime();

            QString data01 = query->value( 2).toString();
            QString data02 = query->value( 3).toString();
            QString data03 = query->value( 4).toString();
            QString data04 = query->value( 5).toString();
            QString data05 = query->value( 6).toString();
            QString data06 = query->value( 7).toString();
            QString data07 = query->value( 8).toString();
            QString data08 = query->value( 9).toString();
            QString data09 = query->value(10).toString();
            QString data10 = query->value(11).toString();

            QString data11 = query->value(12).toString();
            QString data12 = query->value(13).toString();
            QString data13 = query->value(14).toString();
            QString data14 = query->value(15).toString();
            QString data15 = query->value(16).toString();
            QString data16 = query->value(17).toString();
            QString data17 = query->value(18).toString();
            QString data18 = query->value(19).toString();
            QString data19 = query->value(20).toString();
            QString data20 = query->value(21).toString();

            QString data21 = query->value(22).toString();
            QString data22 = query->value(23).toString();
            QString data23 = query->value(24).toString();
            QString data24 = query->value(25).toString();
            QString data25 = query->value(26).toString();
            QString data26 = query->value(27).toString();
            QString data27 = query->value(28).toString();
            QString data28 = query->value(29).toString();
            QString data29 = query->value(30).toString();
            QString data30 = query->value(31).toString();

            QString data31 = query->value(32).toString();
            QString data32 = query->value(33).toString();
            QString data33 = query->value(34).toString();
            QString data34 = query->value(35).toString();
            QString data35 = query->value(36).toString();
            QString data36 = query->value(37).toString();
            QString data37 = query->value(38).toString();
            QString data38 = query->value(39).toString();
            QString data39 = query->value(40).toString();
            QString data40 = query->value(41).toString();

            QString data41 = query->value(42).toString();
            QString data42 = query->value(43).toString();
            QString data43 = query->value(44).toString();
            QString data44 = query->value(45).toString();
            QString data45 = query->value(46).toString();
            QString data46 = query->value(47).toString();
            QString data47 = query->value(48).toString();
            QString data48 = query->value(49).toString();
            QString data49 = query->value(50).toString();
            QString data50 = query->value(51).toString();

            // ------------------------------ //
            // собственно формирование письма //
            // ------------------------------ //
            QString temp_folder = data_app.path_temp + file_nam;
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

            // правка полей контента шаблона
            QString s_content_new = s_content;
            s_content_new = s_content_new.
                    replace("#DATA01#", data01).
                    replace("#DATA02#", data02).
                    replace("#DATA03#", data03).
                    replace("#DATA04#", data04).
                    replace("#DATA05#", data05).
                    replace("#DATA06#", data06).
                    replace("#DATA07#", data07).
                    replace("#DATA08#", data08).
                    replace("#DATA09#", data09).
                    replace("#DATA10#", data10).

                    replace("#DATA11#", data11).
                    replace("#DATA12#", data12).
                    replace("#DATA13#", data13).
                    replace("#DATA14#", data14).
                    replace("#DATA15#", data15).
                    replace("#DATA16#", data16).
                    replace("#DATA17#", data17).
                    replace("#DATA18#", data18).
                    replace("#DATA19#", data19).
                    replace("#DATA20#", data20).

                    replace("#DATA21#", data21).
                    replace("#DATA22#", data22).
                    replace("#DATA23#", data23).
                    replace("#DATA24#", data24).
                    replace("#DATA25#", data25).
                    replace("#DATA26#", data26).
                    replace("#DATA27#", data27).
                    replace("#DATA28#", data28).
                    replace("#DATA29#", data29).
                    replace("#DATA30#", data30).

                    replace("#DATA31#", data31).
                    replace("#DATA32#", data32).
                    replace("#DATA33#", data33).
                    replace("#DATA34#", data34).
                    replace("#DATA35#", data35).
                    replace("#DATA36#", data36).
                    replace("#DATA37#", data37).
                    replace("#DATA38#", data38).
                    replace("#DATA39#", data39).
                    replace("#DATA40#", data40).

                    replace("#DATA41#", data41).
                    replace("#DATA42#", data42).
                    replace("#DATA43#", data43).
                    replace("#DATA44#", data44).
                    replace("#DATA45#", data45).
                    replace("#DATA46#", data46).
                    replace("#DATA47#", data47).
                    replace("#DATA48#", data48).
                    replace("#DATA49#", data49).
                    replace("#DATA50#", data50);

            // сохранение контента шаблона
            QFile f_content_save(temp_folder + "/content1.xml");
            f_content_save.open(QIODevice::WriteOnly);
            f_content_save.write(s_content_new.toUtf8());
            f_content_save.flush();
            f_content_save.close();

            QFile::remove(temp_folder + "/content.xml");
            QFile::copy(temp_folder + "/content1.xml", temp_folder + "/content.xml");
m_ret:
            // имя сохраняемого файла
            QString fname_report = folder_save + "/" + file_nam + " - " + QDate::currentDate().toString("yyyy-MM-dd") + " - " + data01 + " " + data02 + " " + data03 + file_ext;

            // проверим, свободно ли имя
            if (QFile::exists(fname_report)) {
                int r = QMessageBox::warning(this, "Повторение имени файла",
                                             "Файл с таким именем уже есть:\n" +
                                             fname_report + "\n\n"
                                             "Возможные варианты действий:\n" +
                                             "  [Ok] - заменить старый файл новым\n"
                                             "  [SaveAll] - добавить к имени файла символ \"#\" и повторить попытку\n"
                                             "  [Cansel] - не сохранять этот файл",
                                             QMessageBox::Ok|QMessageBox::SaveAll|QMessageBox::Cancel,
                                             QMessageBox::Ok);
                if (r==QMessageBox::Cancel)
                    continue;
                if (r==QMessageBox::SaveAll) {
                    file_ext = "#" + file_ext;
                    goto m_ret;
                }

            }

            // архивация шаблона

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

            // запомним факт генерации письма
            QSqlQuery *query_save = new QSqlQuery(db);
            QString sql_save = "update reports.letters_data_tab "
                               "   set dt_gen = CURRENT_TIMESTAMP "
                               " where id = " + QString::number(id) + "; ";

            mySQL.exec(this, sql_save, "Очистка старых данных для генерации писем", query_save, false, db, data_app);

            n++;
            if (n%21==0) {
                ui->lab_cnt_2->setText(QString::number(n));
                QApplication::processEvents();
            }
        }

        db.commit();

        // ===========================
        // открытие папки с данными
        // ===========================
        long result = (long long)ShellExecute(0, 0, reinterpret_cast<const WCHAR*>(folder_save.utf16()), 0, 0, SW_NORMAL);

    } else {
        // пока нетууу :(((
    }

    refresh_data_tab();
}

void print_letters_wnd::on_rb_new_clicked(bool checked) {
    refresh_data_tab();
}
void print_letters_wnd::on_rb_all_clicked(bool checked) {
    refresh_data_tab();
}
void print_letters_wnd::on_rb_selected_clicked(bool checked) {
    refresh_data_tab();
}
void print_letters_wnd::on_combo_filter_value_activated(const QString &arg1) {
    refresh_data_tab();
}


void print_letters_wnd::on_ch_filter_clicked(bool checked) {
    ui->pan_filter->setEnabled(checked);
    if (checked) {
        refresh_filter_values();
    }
}
void print_letters_wnd::on_combo_filter_fields_currentIndexChanged(int index) {
    refresh_filter_values();
    refresh_data_tab();
}

void print_letters_wnd::on_combo_filter_values_activated(const QString &arg1) {
    refresh_data_tab();
}
