#include "letters2mo_wnd.h"
#include "ui_letters2mo_wnd.h"

#include <QFileDialog>
#include <QItemSelectionModel>

#include "ki_funcs.h"

letters2MO_wnd::letters2MO_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::letters2MO_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    l41.clear();
    l41.append(400);
    l41.append(100);

    l51.clear();
    l51.append(500);
    l51.append(100);

    l01.clear();
    l01.append(0);
    l01.append(100);

    ui->split_log->setSizes(l51);
    ui->split_medorgs->setSizes(l41);
    ui->date_current->setDate(QDate::currentDate());
    ui->date_print->setDate(QDate::currentDate());

    refresh_medorgs_tab();
    refresh_combo_layers();
    ui->combo_layer_new->setCurrentIndex(1);
}

letters2MO_wnd::~letters2MO_wnd() {
    delete ui;
}

void letters2MO_wnd::on_bn_close_clicked() {
    close();
}

void letters2MO_wnd::on_rb_inko_clicked(bool checked) {
    refresh_combo_layers();
    ui->combo_layer_new->setCurrentIndex(1);
}

void letters2MO_wnd::on_rb_tfoms_clicked(bool checked) {
    refresh_combo_layers();
    ui->combo_layer_new->setCurrentIndex(1);
}

void letters2MO_wnd::refresh_combo_layers() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающих списков снимков
    layer_type = (ui->rb_inko->isChecked() ? 1 : 2);

    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select id, layer_name, layer_date "
                  "  from assig.layers "
                  " where layer_type=" + QString::number(layer_type) + " "
                  " order by layer_date DESC ; ";
    query->exec(sql);

    ui->combo_layer_old->clear();
    ui->combo_layer_new->clear();
    layer_ids.clear();
    layer_dates.clear();

    layer_ids.append(-1);
    layer_dates.append(QDate());
    ui->combo_layer_old->addItem(" - Выберите старый снимок данных - ", -1);
    ui->combo_layer_new->addItem(" - Выберите новый снимок данных - ", -1);
    while (query->next()) {
        int     id_layer   = query->value(0).toInt();
        QString layer_name = query->value(1).toString();
        QDate   layer_date = query->value(2).toDate();
        layer_ids.append(id_layer);
        layer_dates.append(layer_date);
        ui->combo_layer_old->addItem(layer_name, id_layer);
        ui->combo_layer_new->addItem(layer_name, id_layer);
    }
    delete query;

    if (ui->combo_layer_old->count()>1) {
        int n = ui->combo_layer_new->findData(id_layer_new);

        if (n>0 && ui->combo_layer_old->count()>n+1) {
            ui->combo_layer_new->setCurrentIndex(n);
            id_layer_new = layer_ids.at(n);

            ui->combo_layer_old->setCurrentIndex(n+1);
            id_layer_old = layer_ids.at(n+1);
        } else {
            ui->combo_layer_old->setCurrentIndex(0);
            id_layer_old = -1;
        }
    } else {
        id_layer_old = -1;
        id_layer_new = -1;
    }
    this->setCursor(Qt::ArrowCursor);
}

void letters2MO_wnd::refresh_combo_sexage_tab() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка медорганизаций
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "SELECT s.id, 'SEXAGE '||l.layer_date||' - '||(case s.assig_type when 1 then ' по терр.' when 2 then ' по заявл.' else ' -все-' end) as sexage_name, "
                  "       l.layer_date, l.layer_type, s.assig_type "
                  "  FROM assig.sexages s "
                  "  LEFT JOIN assig.layers l ON(l.id=s.id_layer) "
                  " WHERE s.id_layer=" + QString::number(id_layer_new) + " "
                  " ORDER BY l.layer_date DESC, s.assig_type ; ";
    mySQL.exec(this, sql, "Список половозрастных таблиц", *query, true, db, data_app);
    ui->combo_sexage_tab->clear();
    while (query->next()) {
        ui->combo_sexage_tab->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void letters2MO_wnd::refresh_combo_move_tab() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка медорганизаций
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "SELECT m.id, 'MOVE '||lo.layer_date||' - '||ln.layer_date||' - '||(case m.assig_type when 1 then ' по терр.' when 2 then ' по заявл.' else ' -все-' end) as move_name, "
                  "      m.assig_type "
                  "  FROM assig.moves m "
                  "  LEFT JOIN assig.layers lo ON(lo.id=m.id_layer_old) "
                  "  LEFT JOIN assig.layers ln ON(ln.id=m.id_layer_new) "
                  " WHERE m.id_layer_new=" + QString::number(id_layer_new) + " "
                  " ORDER BY ln.layer_date DESC, m.assig_type desc ; ";
    mySQL.exec(this, sql, "Список таблиц движения", *query, true, db, data_app);
    ui->combo_move_tab->clear();
    while (query->next()) {
        ui->combo_move_tab->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void letters2MO_wnd::on_combo_layer_new_currentIndexChanged(int index) {
    layer_type = (ui->rb_inko->isChecked() ? 1 : 2);
    int n = ui->combo_layer_new->currentIndex();
    ui->date_report->setDate(layer_dates.at(index));

    if (ui->combo_layer_old->count()>n+1) {
        ui->combo_layer_old->setCurrentIndex(n+1);
        id_layer_old = layer_ids.at(n+1);

        refresh_combo_sexage_tab();
        refresh_combo_move_tab();
    } else {
        /*ui->combo_layer_old->setCurrentIndex(0);
        id_layer_old = -1;*/
    }
}

void letters2MO_wnd::on_combo_layer_old_currentIndexChanged(int index) {
    layer_type = (ui->rb_inko->isChecked() ? 1 : 2);
    int n = ui->combo_layer_old->currentIndex();

    if (n>1 && layer_ids.size()>n) {
        ui->combo_layer_new->setCurrentIndex(n-1);
        id_layer_new = layer_ids.at(n-1);

        refresh_combo_sexage_tab();
        refresh_combo_move_tab();
    } else {
        /*ui->combo_layer_new->setCurrentIndex(0);
        id_layer_new = -1;*/
    }
}

void letters2MO_wnd::on_bn_other_files_clicked() {
    QString s = QFileDialog::getExistingDirectory(this, "Выберите папку дополнительных данных", ui->line_other_files->text());
    s = s.trimmed();
    if (!s.isEmpty())  ui->line_other_files->setText(s);
}

void letters2MO_wnd::on_bn_gen_dir_clicked() {
    QString s = QFileDialog::getExistingDirectory(this, "Выберите папку для сохранения данных", ui->line_gen_dir->text());
    s = s.trimmed();
    if (!s.isEmpty())  ui->line_gen_dir->setText(s);
}

void letters2MO_wnd::on_bn_backup_dir_clicked() {
    QString s = QFileDialog::getExistingDirectory(this, "Выберите папку для сохранения резервной копии данных", ui->line_backup_dir->text());
    s = s.trimmed();
    if (!s.isEmpty())  ui->line_backup_dir->setText(s);
}


void letters2MO_wnd::refresh_medorgs_tab() {
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
    ui->tab_medorgs->setColumnWidth(  2,220);     // name_mo
    ui->tab_medorgs->setColumnWidth(  3, 30);     // mo_assig

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

void letters2MO_wnd::on_bn_refresh_medorgs_clicked() {
    refresh_medorgs_tab();
}

void letters2MO_wnd::on_ch_all_medorgs_clicked(bool checked) {
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

void letters2MO_wnd::on_bn_clear_sel_clicked() {
    QItemSelectionModel *sel_model = ui->tab_medorgs->selectionModel();

    for (int i=0; i<model_medorgs.rowCount(); i++) {
        sel_model->select(model_medorgs.index(i,0),QItemSelectionModel::Deselect);
        sel_model->select(model_medorgs.index(i,1),QItemSelectionModel::Deselect);
        sel_model->select(model_medorgs.index(i,2),QItemSelectionModel::Deselect);
        sel_model->select(model_medorgs.index(i,3),QItemSelectionModel::Deselect);
    }
    ui->tab_medorgs->repaint();
}

void letters2MO_wnd::on_bn_reverse_sel_clicked() {
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

void letters2MO_wnd::on_ch_lock_clicked(bool checked) {
    ui->tab_medorgs->setEnabled(!checked);
    ui->bn_reverse_sel->setEnabled(!checked);
    ui->bn_clear_sel->setEnabled(!checked);
    ui->ch_all_medorgs->setEnabled(!checked);

    if (!checked) {
        ui->ch_all_medorgs->setChecked(false);
    }
}


void letters2MO_wnd::on_ch_other_files_clicked(bool checked) {
    ui->line_other_files->setEnabled(checked);
    ui->bn_other_files->setEnabled(checked);
    ui->lab_other_files->setEnabled(checked);
}

void letters2MO_wnd::on_ch_backup_clicked(bool checked) {
    ui->line_backup_dir->setEnabled(checked);
    ui->bn_backup_dir->setEnabled(checked);
}




void letters2MO_wnd::on_bn_generate_clicked() {
    if ( !ui->tab_medorgs->selectionModel()->hasSelection() ) {
        QMessageBox::warning(this, "Не выбрана ни одна медорганизация",
                             "Не выбрана ни одна медорганизация.\n"
                             "Для запуска процедуры надо выбрать медорганизацию или перечень медорганизаций, для которых делается отчёт.\n\n"
                             "Отмена операции.");
        return;
    }
    if ( ui->combo_layer_new->currentIndex()<1
         || ui->combo_layer_new->currentText().isEmpty() ) {
        QMessageBox::warning(this, "Не выбран новый снимок данных",
                             "Не выбран новый снимок данных.\n\n"
                             "Отмена операции.");
        return;
    }
    if ( ui->combo_layer_old->currentIndex()<1
         || ui->combo_layer_old->currentText().isEmpty() ) {
        QMessageBox::warning(this, "Не выбран старый снимок данных",
                             "Не выбран старый снимок данных.\n\n"
                             "Отмена операции.");
        return;
    }
    if ( ui->date_report->date()<QDate(2017,1,1)
         || ui->date_report->date()>QDate::currentDate() ) {
        QMessageBox::warning(this, "Укажите дату, на которую делаются письма",
                             "Укажите дату, по состоянию на которую формируются письма.\n"
                             "Возможно это 1-5 число текущего месяца.\n\n"
                             "Отмена операции.");
        return;
    }
    if ( ui->date_print->date()<QDate(2017,1,1)
         || ui->date_print->date()>QDate::currentDate() ) {
        QMessageBox::warning(this, "Укажите дату генерации писем",
                             "Укажите дату генерации писем.\n"
                             "Это может быть текущая дата.\n\n"
                             "Отмена операции.");
        return;
    }
    if (ui->line_official_num->text().isEmpty()) {
        QMessageBox::warning(this, "Не указан регистрационный номер письма",
                             "Укажите регистрационный номер для генерируемых писем.\n"
                             "Номер можно узнать у секретаря.\n\n"
                             "Отмена операции.");
        return;
    }
    if ( ui->combo_sexage_tab->currentIndex()<0
         || ui->combo_sexage_tab->currentText().isEmpty() ) {
        QMessageBox::warning(this, "Не выбраа таблица половозрастного распределения застрахованных",
                             "Выберите одну из доступных таблиц половозрастного распределелния застрахованных для нового снимка данных.\n\n"
                             "Если таблица ещё не создана - сначала создайте таблицу в окне снимков данных, а потом повторите попытку генерации писем.");
        return;
    }
    if ( ui->combo_move_tab->currentIndex()<0
         || ui->combo_move_tab->currentText().isEmpty() ) {
        QMessageBox::warning(this, "Не выбраа таблица движения прикреплённых застрахованных",
                             "Выберите одну из доступных таблиц движения прикреплённых застрахованных для старого и нового снимка данных.\n\n"
                             "Если таблица ещё не создана - сначала создайте таблицу в окне снимков данных, а потом повторите попытку генерации писем.");
        return;
    }
    if ( ui->ch_phones->isChecked() ) {
        if (QMessageBox::warning(this, "Нужно подтверждение",
                                 "Вы действительно хотите включить в выгружаемые списки информацию о телефонах застрахованных?\n\n"
                                 "Это персональные данные. Без особой необходимости - откажитесь от их рассылки.",
                                 QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel ) {
            ui->ch_phones->setChecked(false);
        }
    }

    // папка доп.файлов
    bool f_other_files = ui->ch_other_files->isChecked();
    QDir dir_other_files;
    // если надо - проверим папку дополнительных файлов
    if (f_other_files) {
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

    // папка резервной копии
    bool f_backup = ui->ch_backup->isChecked();
    QDir dir_backup;
    // если надо - проверим папку резервной копии
    if (f_backup) {
        QString dir_backup_s = ui->line_backup_dir->text();
        dir_backup.setPath(dir_backup_s);
        if ( !dir_backup.exists() ) {
            QMessageBox::warning(this, "Не найдена папка для резервного копирования писем",
                                 "Папка для резервного копирования писем не найдена или не указана оператором.\n"
                                 "Выберите папку для резервного копирования писем или снимите флажок.\n\n"
                                 "Отмена операции.");
            return;
        }
    }

    ui->ch_lock->setChecked(true);
    on_ch_lock_clicked(true);
    ui->lab_count->setText("0");
    ui->te_log->clear();
    ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->  Запущен процесс генераци писем...");
    QApplication::processEvents();


    // запишем README.txt
    QString fname_readme = dir_generate_s + "/readme.txt";

    QFile f_readme(fname_readme);
    if (QFile::exists(fname_readme))
        QFile::remove(fname_readme);
    if (f_readme.open(QIODevice::WriteOnly))
        f_readme.write(ui->te_readmy->toPlainText().toLocal8Bit());
    f_readme.close();

    ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->  создан файл READ_ME.txt");
    QApplication::processEvents();


    // подготовка MOVE_REPORT.csv
    QString fname_moverep = dir_generate_s + "/move_report.csv";
    QFile file_move_rep(fname_moverep);
    if (QFile::exists(fname_moverep))
        QFile::remove(fname_moverep);
    if (file_move_rep.open(QIODevice::WriteOnly)) {
        file_move_rep.write(QString("CODE_MO\t"
                                    "NAME_MO\t"
                                    "OLD\t"
                                    "ASSIG\t"
                                    "UNSIG\t"
                                    "NEW\t"
                                    "SNILS_MT\n").toLocal8Bit());
        file_move_rep.close();
    }

    ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->  создан файл MOVE_REPORT.csv\n");
    QApplication::processEvents();


    // запомним данные слоёв
    QString layer_old    = ui->combo_layer_old->currentText();
    int id_layer_old     = ui->combo_layer_old->currentData().toInt();
    QDate date_layer_old = layer_dates.at(ui->combo_layer_old->currentIndex());
    QString layer_new    = ui->combo_layer_new->currentText();
    int id_layer_new     = ui->combo_layer_new->currentData().toInt();
    QDate date_layer_new = layer_dates.at(ui->combo_layer_new->currentIndex());
    // запомним атрибуты письма
    QDate date_report = ui->date_report->date();
    QDate date_print  = ui->date_print->date();
    QString official_num = ui->line_official_num->text();
    // запомним данные используемых таблиц
    QString sexage_tab = ui->combo_sexage_tab->currentText();
    int id_sexage_tab  = ui->combo_sexage_tab->currentData().toInt();
    QString move_tab   = ui->combo_move_tab->currentText();
    int id_move_tab    = ui->combo_move_tab->currentData().toInt();
    // запомним параметры генерации
    bool f_count   = ui->ch_count->isChecked();
    bool f_assig   = ui->ch_assig->isChecked();
    bool f_unsig   = ui->ch_unsig->isChecked();
    bool f_address = ui->ch_address->isChecked();
    bool f_phones  = ui->ch_phones->isChecked();


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

            // собственно генерация письма
            bool res = gen_letter2MO( layer_old, id_layer_old, date_layer_old, layer_new, id_layer_new, date_layer_new,
                                      date_report, date_print, official_num,
                                      sexage_tab, id_sexage_tab, move_tab, id_move_tab,
                                      f_count, f_assig, f_unsig, f_address, f_phones,
                                      f_other_files, dir_other_files, dir_generate, f_backup, dir_backup,
                                      code_mo, fname_readme, fname_moverep);

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
    // удлим шаблон файла README.txt
    QFile::remove(fname_readme);

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



bool letters2MO_wnd::gen_letter2MO( QString layer_old, int id_layer_old, QDate date_layer_old, QString layer_new, int id_layer_new, QDate date_layer_new,
                                    QDate date_report, QDate date_print, QString official_num,
                                    QString sexage_tab, int id_sexage_tab, QString move_tab, int id_move_tab,
                                    bool f_count, bool f_assig, bool f_unsig, bool f_address, bool f_phones,
                                    bool f_other_files, QDir &dir_other_files, QDir &dir_generate, bool f_backup, QDir &dir_backup,
                                    QString code_mo, QString fname_readme, QString fname_moverep ) {

    QString fname_arch = dir_generate.path() + "/" + date_report.toString("yyyy-MM-dd") + " - " + code_mo + " - assig_letter_to_mo.zip";
    if ( QFile::exists(fname_arch) ) {
        while (!QFile::remove(fname_arch))
            QMessageBox::warning(this,("Не удалось удалить старый файл"),
                                 ("Не удалось удалить старый файл\n")+fname_arch);
    }

    QProcess *myProcess = new QProcess(NULL);


    // запись в лог
    ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->    -    добавлен файл - README.txt ... ");
    QApplication::processEvents();

    // readme.txt - добавим в архив
    QString com = data_app.path_arch + "7z.exe a -tzip \"" + fname_arch + "\" \"" + fname_readme + "\" -mx9";
    int res_com = 0;
    while ((res_com = myProcess->execute(com))!=0) {
        if (QMessageBox::warning(this, "Ошибка при создании архива",
                                 "Файл README.txt не удалось добавить в архив. \n"
                                 "Это первая операция - при этом архив создаётся с нуля.\n\n"
                                 "Проверьте наличие и доступность программного интерфейса 7Z и доступность папки генерации данных\n"
                                 + dir_generate.path() + "\n\n"
                                 "Повторить попытку?",
                                 QMessageBox::Yes|QMessageBox::Abort,
                                 QMessageBox::Abort)==QMessageBox::Abort) {
            break;
        }
    }
    // запись в лог
    if (res_com==0) {
        //ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->         готово.");
        QApplication::processEvents();
    } else {
        ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->         !! ОТКАЗ !!");
        QApplication::processEvents();
    }


    // получим данные медорганизации
    QString sql_medorg = "SELECT m.id, code_mo, name_mo, name_mo_full, "
                         "       mo_assig, mo_chif_state, mo_chif_state_dp, mo_chif_famio, mo_chif_famio_dp, mo_chif_greetings, mo_contract_num, mo_contract_date, mo_letter_text,"
                         "       mo_assig_type "
                         "  FROM spr_medorgs m "
                         " WHERE m.code_mo='" + code_mo + "' ; ";
    QSqlQuery *query_medorg = new QSqlQuery(db);
    mySQL.exec(this, sql_medorg, "Получим данные медорганизации", *query_medorg, true, db, data_app);
    if (!query_medorg->next()) {
        QMessageBox::warning(this, "Данные медицинской организации не найдены",
                             "При попытке получить данные медицинской организации с кодом " + code_mo + " произошла ошибка \n" + sql_medorg +
                             "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);

        ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->         !! МО НЕ НАЙДЕНА !!");
        QApplication::processEvents();
        return false;
    }
    int     id_medorg         = query_medorg->value( 0).toInt();
    QString name_mo           = query_medorg->value( 2).toString();
    QString name_mo_full      = query_medorg->value( 3).toString();
    int     mo_assig          = query_medorg->value( 4).toInt();
    QString mo_chif_state     = query_medorg->value( 5).toString();
    QString mo_chif_state_dp  = query_medorg->value( 6).toString();
    QString mo_chif_famio     = query_medorg->value( 7).toString();
    QString mo_chif_famio_dp  = query_medorg->value( 8).toString();
    QString mo_chif_greetings = query_medorg->value( 9).toString();
    QString mo_contract_num   = query_medorg->value(10).toString();
    QDate   mo_contract_date  = query_medorg->value(11).toDate();
    QString mo_letter_text    = query_medorg->value(12).toString();
    int     mo_assig_type     = query_medorg->value(13).toInt();

    delete query_medorg;


    // ---------------------------------- //
    // ---------------------------------- //
    //        формирование данных         //
    // ---------------------------------- //
    // ---------------------------------- //


    db.transaction();


    // --------------
    // 1 - письма
    // --------------

    QString rep_folder =  data_app.path_reports;
    // распаковка шаблона
    QString rep_template = rep_folder + "rep_assig_letter_full.odt";
    if (!QFile::exists(rep_template)) {
        QMessageBox::warning(this,
                             ("Шаблон не найден"),
                             ("Не найден шаблон печатной формы заявления: \n") + rep_template +
                             ("\n\nОперация прервана."));
        this->setCursor(Qt::ArrowCursor);
        return false;
    }

    QString tmp_dir_s = dir_generate.path();
    QDir tmp_dir = dir_generate;
    if (tmp_dir.exists(tmp_dir_s)) {
    } else {
        tmp_dir.mkpath(tmp_dir_s);
    }


    // удалим старые версии файлов
    if ( QFile::exists(tmp_dir_s + "\\content.xml") ) {
        while (!QFile::remove(tmp_dir_s + "\\content.xml"))
            QMessageBox::warning(this,"Не удалось удалить старый файл",
                                 "Не удалось удалить старый файл\n"+tmp_dir_s + "\\content.xml");
    }
    if ( QFile::exists(tmp_dir_s + "\\styles.xml") ) {
        while (!QFile::remove(tmp_dir_s + "\\styles.xml"))
            QMessageBox::warning(this,"Не удалось удалить старый файл",
                                 "Не удалось удалить старый файл\n"+tmp_dir_s + "\\styles.xml");
    }


    com = "\"" + data_app.path_arch + "7z.exe\" e \"" + rep_template + "\" -o\"" + tmp_dir_s + "\" content.xml";
    com = com.replace("/", "\\");
    if (myProcess->execute(com)!=0) {
        QMessageBox::warning(this,
                             "Ошибка распаковки шаблона",
                             "Не удалось распаковать файл контента шаблона\n" + rep_template + " -> content.xml" +
                             "\nпечатной формы во временную папку\n" + tmp_dir_s +
                             "\n\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки." +
                             "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        this->setCursor(Qt::ArrowCursor);
        return false;
    }
    com = "\"" + data_app.path_arch + "7z.exe\" e \"" + rep_template + "\" -o\"" + tmp_dir_s + "\" styles.xml";
    com = com.replace("/", "\\");
    if (myProcess->execute(com)!=0) {
        QMessageBox::warning(this, "Ошибка распаковки шаблона",
                             "Не удалось распаковать файл стилей шаблона \n" + rep_template + " -> styles.xml" +
                             "\nпечатной формы во временную папку\n" + tmp_dir_s +
                             "\n\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки." +
                             "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        this->setCursor(Qt::ArrowCursor);
        return false;
    }


    // ===========================
    // правка контента шаблона
    // ===========================
    QFile f_content(tmp_dir_s + "/content.xml");

    QTextStream stream(&f_content);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    stream.setCodec(codec);

    f_content.open(QIODevice::ReadOnly);
    QString s_content = stream.readAll();

    f_content.close();
    if (s_content.isEmpty()) {
        QMessageBox::warning(this,
                             "Файл контента пустой",
                             "Файл контента пустой. Возможно шаблон был испорчен.\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        return false;
    }



    // // ---------- // //
    // //   письмо   // //
    // // ---------- // //

    // запись в лог
    ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->    -    сформируем официальное письмо ... ");
    QApplication::processEvents();

    QString field;
    QString value;

    // логотип
    field = "#LOGO1#";
    value = data_app.smo_logo1;
    s_content = s_content.replace(field,value);
    field = "#LOGO2#";
    value = data_app.smo_logo2;
    s_content = s_content.replace(field,value);
    field = "#LOGO3#";
    value = data_app.smo_logo3;
    s_content = s_content.replace(field,value);


    // данные писем
    field = "#print_date#";
    value = date_print.toString("dd.MM.yyyy");
    s_content = s_content.replace(field,value);
    field = "#print_date_text#";
    value = date_to_str(date_print);
    s_content = s_content.replace(field,value);

    field = "#report_date#";
    value = date_report.toString("dd.MM.yyyy");
    s_content = s_content.replace(field,value);
    field = "#report_date_text#";
    value = date_to_str(date_report);
    s_content = s_content.replace(field,value);

    QDate date_month = date_report.addMonths(-1);
    field = "#in_month_year#";
    value = monthyear_to_str(date_month);
    s_content = s_content.replace(field,value);

    field = "#doc_reg_code#";
    value = official_num;
    s_content = s_content.replace(field,value);


    // данные МО
    field = "#mo_name_short#";
    value = mo_letter_text;
    s_content = s_content.replace(field,value);
    field = "#mo_name#";
    value = mo_letter_text;
    s_content = s_content.replace(field,value);
    field = "#mo_master_rp#";
    value = mo_chif_state_dp;
    s_content = s_content.replace(field,value);
    field = "#mo_master_i_o_fam_rp#";
    value = mo_chif_famio_dp;
    s_content = s_content.replace(field,value);
    field = "#mo_master_greetings#";
    value = mo_chif_greetings;
    s_content = s_content.replace(field,value);

    field = "#contract_num#";
    value = mo_contract_num;
    s_content = s_content.replace(field,value);
    field = "#contract_date#";
    value = mo_contract_date.toString("dd.MM.yyyy");
    s_content = s_content.replace(field,value);


    // данные СМО
    field = "#inko_address#";
    value = data_app.smo_address;
    s_content = s_content.replace(field,value);
    field = "#inko_master#";
    value = data_app.smo_master_state;
    s_content = s_content.replace(field,value);
    field = "#inko_master_name#";
    value = data_app.smo_master_FamIO;
    s_content = s_content.replace(field,value);


    // данные испольнителей
    field = "#DOER1_FamIO#";
    value = data_app.letter_doer1_FamIO;
    s_content = s_content.replace(field,value);
    field = "#DOER1_PHONE#";
    value = data_app.letter_doer1_phone;
    s_content = s_content.replace(field,value);

    field = "#DOER2_FamIO#";
    value = data_app.letter_doer2_FamIO;
    s_content = s_content.replace(field,value);
    field = "#DOER2_PHONE#";
    value = data_app.letter_doer2_phone;
    s_content = s_content.replace(field,value);


    // // ---------------------- // //
    // // половозрастная таблица // //
    // // ---------------------- // //
    ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->    -    -    половозрастное распределение ... ");
    QApplication::processEvents();

    // прочитаем данные половозрастной
    QString sql_sexage = "SELECT id, id_sexage, code_head, m_0_0, m_1_4, m_5_17, m_18_59, m_60_, m_sum, w_0_0, w_1_4, w_5_17, w_18_54, w_55_, w_sum, sum, snils_mt "
                         "  FROM assig.sexages_lines "
                         " WHERE id_sexage = " + QString::number(id_sexage_tab) + " "
                         "   AND code_head = '" + code_mo + "' "
                         " LIMIT 1 ; ";
    QSqlQuery *query_sexage = new QSqlQuery(db);
    mySQL.exec(this, sql_sexage, "Получим данные половозрастной", *query_sexage, true, db, data_app);
    if (!query_sexage->next()) {
        QMessageBox::warning(this, "Данные половозрастной не найдены",
                             "При попытке получить данные половозрастной с ID " + QString::number(id_sexage_tab) + " произошла ошибка \n" + sql_sexage +
                             "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);

        ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->         !! ПОЛОВОЗРАСТНАЯ НЕ НАЙДЕНА !!");
        QApplication::processEvents();
        return false;
    }
    int     id         = query_sexage->value( 0).toInt();
    QString code_head  = query_sexage->value( 2).toString();
    int     m_0_0      = query_sexage->value( 3).toInt();
    int     m_1_4      = query_sexage->value( 4).toInt();
    int     m_5_17     = query_sexage->value( 5).toInt();
    int     m_18_59    = query_sexage->value( 6).toInt();
    int     m_60_      = query_sexage->value( 7).toInt();
    int     m_sum      = query_sexage->value( 8).toInt();
    int     w_0_0      = query_sexage->value( 9).toInt();
    int     w_1_4      = query_sexage->value(10).toInt();
    int     w_5_17     = query_sexage->value(11).toInt();
    int     w_18_54    = query_sexage->value(12).toInt();
    int     w_55_      = query_sexage->value(13).toInt();
    int     w_sum      = query_sexage->value(14).toInt();
    int     sum        = query_sexage->value(15).toInt();
    int     snils_mt   = query_sexage->value(16).toInt();
    int     snils_mt_2 = snils_mt;

    delete query_sexage;

    field = "#all#";
    if (sum>0) {
        QString value = QString::number(sum);
        s_content = s_content.replace(field,value);
    } else s_content = s_content.replace(field,"0");

    field = "#m0_4#";
    if ((m_0_0 + m_1_4)>0) {
        QString value = QString::number(m_0_0 + m_1_4);
        s_content = s_content.replace(field,value);
    } else s_content = s_content.replace(field,"0");

    field = "#w0_4#";
    if ((w_0_0 + w_1_4)>0) {
        QString value = QString::number(w_0_0 + w_1_4);
        s_content = s_content.replace(field,value);
    } else s_content = s_content.replace(field,"0");

    field = "#m5_17#";
    if (m_5_17>0) {
        QString value = QString::number(m_5_17);
        s_content = s_content.replace(field,value);
    } else s_content = s_content.replace(field,"0");

    field = "#w5_17#";
    if (w_5_17>0) {
        QString value = QString::number(w_5_17);
        s_content = s_content.replace(field,value);
    } else s_content = s_content.replace(field,"0");

    field = "#m18_59#";
    if (m_18_59>0) {
        QString value = QString::number(m_18_59);
        s_content = s_content.replace(field,value);
    } else s_content = s_content.replace(field,"0");

    field = "#w18_54#";
    if (w_18_54>0) {
        QString value = QString::number(w_18_54);
        s_content = s_content.replace(field,value);
    } else s_content = s_content.replace(field,"0");

    field = "#m60_#";
    if (m_60_>0) {
        QString value = QString::number(m_60_);
        s_content = s_content.replace(field,value);
    } else s_content = s_content.replace(field,"0");

    field = "#w55_#";
    if (w_55_>0) {
        QString value = QString::number(w_55_);
        s_content = s_content.replace(field,value);
    } else s_content = s_content.replace(field,"0");


    // // ------------------ // //
    // //  таблица движения  // //
    // // ------------------ // //
    ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->    -    -    таблица движения ... ");
    QApplication::processEvents();

    // прочитаем данные таблицы движения
    QString sql_move = "SELECT id, id_move, code_head, cnt_old, cnt_add, cnt_dec, cnt_new "
                       "  FROM assig.moves_lines "
                       " WHERE id_move = " + QString::number(id_move_tab) + " "
                       "   AND code_head = '" + code_mo + "' "
                       " LIMIT 1 ; ";
    QSqlQuery *query_move = new QSqlQuery(db);
    mySQL.exec(this, sql_move, "Получим данные таблицы движения", *query_move, true, db, data_app);
    if (!query_move->next()) {
        QMessageBox::warning(this, "Данные таблицы движения не найдены",
                             "При попытке получить данные таблицы движения с ID " + QString::number(id_move_tab) + " произошла ошибка \n" + sql_move +
                             "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);

        ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->         !! ТАБЛИЦА ДВИЖЕНИЯ НЕ НАЙДЕНА !!");
        QApplication::processEvents();
        return false;
    }
    int cnt_old = query_move->value( 3).toInt();
    int cnt_add = query_move->value( 4).toInt();
    int cnt_dec = query_move->value( 5).toInt();
    int cnt_new = query_move->value( 6).toInt();

    delete query_move;

    if (mo_assig_type==2) {
        // ------------------------------------------ //
        // если прикрепление к МО только по 2 типу... //
        // ------------------------------------------ //
        field = "#move_old#";
        if (cnt_old>0) {
            // берём значение OLD из таблицы движения
            QString value = QString::number(cnt_old);
            s_content = s_content.replace(field,value);
        } else s_content = s_content.replace(field,"0");

        field = "#move_new#";
        if (sum>0) {
            // берём полную численность по половозрастной
            QString value = QString::number(sum);
            s_content = s_content.replace(field,value);
        } else s_content = s_content.replace(field,"0");

        // подгонка данных движения
        int cnt_dec_ = cnt_old - sum     + cnt_add;
        int cnt_add_ = sum     - cnt_old + cnt_dec;
        if (cnt_dec_>=0) {
            // правим значение DEC
            field = "#move_out#";
            if (cnt_dec_>0) {
                QString value = QString::number(cnt_dec_);
                s_content = s_content.replace(field,value);
            } else s_content = s_content.replace(field,"0");

            field = "#move_in#";
            if (cnt_add>0) {
                QString value = QString::number(cnt_add);
                s_content = s_content.replace(field,value);
            } else s_content = s_content.replace(field,"0");

        } else if (cnt_add_>=0) {
            // правим значение ADD
            field = "#move_out#";
            if (cnt_dec>0) {
                QString value = QString::number(cnt_dec);
                s_content = s_content.replace(field,value);
            } else s_content = s_content.replace(field,"0");

            field = "#move_in#";
            if (cnt_add_>0) {
                QString value = QString::number(cnt_add_);
                s_content = s_content.replace(field,value);
            } else s_content = s_content.replace(field,"0");

        } else if (cnt_old>=sum) {
            // есть только DEC
            cnt_dec_ = cnt_old - sum;
            cnt_add_ = 0;

            field = "#move_out#";
            if (cnt_dec_>0) {
                QString value = QString::number(cnt_dec_);
                s_content = s_content.replace(field,value);
            } else s_content = s_content.replace(field,"0");

            field = "#move_in#";
            if (cnt_add_>0) {
                QString value = QString::number(cnt_add_);
                s_content = s_content.replace(field,value);
            } else s_content = s_content.replace(field,"0");

        } else {
            // есть только ADD
            cnt_dec_ = 0;
            cnt_add_ = sum - cnt_old;

            field = "#move_out#";
            if (cnt_dec_>0) {
                QString value = QString::number(cnt_dec_);
                s_content = s_content.replace(field,value);
            } else s_content = s_content.replace(field,"0");

            field = "#move_in#";
            if (cnt_add_>0) {
                QString value = QString::number(cnt_add_);
                s_content = s_content.replace(field,value);
            } else s_content = s_content.replace(field,"0");
        }

        field = "#snils_mt#";
        if (snils_mt>0) {
            // берём полное количество застрахованных со СНИЛС
            QString value = QString::number(snils_mt);
            s_content = s_content.replace(field,value);
        } else s_content = s_content.replace(field,"0");

    } else {
        // ------------------------------------------ //
        //         ... для остальных МО ...           //
        // ------------------------------------------ //
        field = "#move_old#";
        if (cnt_old>0) {
            // берём значение OLD из таблицы движения
            QString value = QString::number(cnt_old);
            s_content = s_content.replace(field,value);
        } else s_content = s_content.replace(field,"0");

        field = "#move_new#";
        if (cnt_new>0) {
            // берём значение NEW из таблицы движения
            QString value = QString::number(cnt_new);
            s_content = s_content.replace(field,value);
        } else s_content = s_content.replace(field,"0");

        field = "#move_out#";
        if (cnt_dec>0) {
            // берём значение DEC из таблицы движения
            QString value = QString::number(cnt_dec);
            s_content = s_content.replace(field,value);
        } else s_content = s_content.replace(field,"0");

        field = "#move_in#";
        if (cnt_add>0) {
            // берём значение ADD из таблицы движения
            QString value = QString::number(cnt_add);
            s_content = s_content.replace(field,value);
        } else s_content = s_content.replace(field,"0");

        field = "#snils_mt#";
        if ( sum>0 ) {
            // берём пропорциональную долю
            float up = (float(snils_mt)/sum);
            snils_mt_2 = up*cnt_new;
            QString value = QString::number(snils_mt_2);
            s_content = s_content.replace(field,value);
        } else s_content = s_content.replace(field,"0");
    }

    QFile file_move_rep(dir_generate.path() + "\\move_report.csv");
    file_move_rep.open(QIODevice::Append);
    file_move_rep.write(QString(code_mo + "\t" + mo_letter_text + "\t" +
                                QString::number(cnt_old) + "\t" +
                                QString::number(cnt_add) + "\t" +
                                QString::number(cnt_dec) + "\t" +
                                QString::number(cnt_new) + "\t" +
                                QString::number(snils_mt_2) + "\n" ).toLocal8Bit());
    file_move_rep.close();


    // сохранение контента шаблона
    QFile f_content_save(tmp_dir_s + "/content.xml");

    QTextStream stream_save(&f_content_save);
    QTextCodec *codec_save = QTextCodec::codecForName("UTF-8");
    stream_save.setCodec(codec_save);
    f_content_save.open(QIODevice::WriteOnly);
    stream_save << s_content;

    f_content_save.close();

    // ===========================
    // архивация шаблона
    // ===========================
    QString fname_lettr;
    fname_lettr = dir_generate.path() + "/" + code_mo + " - " + name_mo + " - " + date_report.toString("yyyy-MM-dd") + ".odt";
    fname_lettr = fname_lettr.replace("«", "#").replace("»", "#").replace("\"", "#");

    QFile::remove(fname_lettr);
    while (QFile::exists(fname_lettr)) {
        if (QMessageBox::warning(this,
                                 ("Ошибка сохранения письма"),
                                 ("Не удалось сохранить печатную форму письма: \n") + fname_lettr +
                                 ("\n\nПроверьте, не открыт ли целевой файл в сторонней программе и повторите операцию."),
                                 QMessageBox::Retry|QMessageBox::Abort,
                                 QMessageBox::Retry)==QMessageBox::Abort) {
            this->setCursor(Qt::ArrowCursor);
            return false;
        }
        QFile::remove(fname_lettr);
    }
    if (!QFile::copy(rep_template, fname_lettr) || !QFile::exists(fname_lettr)) {
        QMessageBox::warning(this,
                             ("Ошибка копирования шаблона"),
                             ("Не удалось скопировать шаблон печатной формы заявления: \n") + rep_template +
                             ("\n\nв целевую папку генерации \n") + fname_lettr +
                             ("\n\nОперация прервана."));
        this->setCursor(Qt::ArrowCursor);
        return false;
    }
    com = "\"" + data_app.path_arch + "7z.exe\" a \"" + fname_lettr + "\" \"" + tmp_dir_s + "/content.xml\"";
    com = com.replace("/", "\\");
    res_com = myProcess->execute(com);
    if (res_com!=0) {
        QMessageBox::warning(this,
                             ("Ошибка обновления контента"),
                             ("При добавлении нового контента в шаблон печатной формы заявления произошла непредвиденная ошибка\n\nОпреация прервана."));
    }


    // добавим в архив
    QString fname_lettr2 = fname_lettr.replace("«", "#").replace("»", "#").replace("\"", "#");
    com = "\"" + data_app.path_arch + "7z.exe\" a -tzip \"" + fname_arch + "\" \"" + fname_lettr2 + "\" -mx9";
    while (myProcess->execute(com)!=0) {
        if (QMessageBox::warning(this,
                                 ("Ошибка при создании архива"),
                                 ("Некоторые из файлов отчёта не удалось добавить в отправляемый архив.\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки D:\\TEMP\\\n\nПовторить попытку?"),
                                 QMessageBox::Yes|QMessageBox::Abort,
                                 QMessageBox::Abort)==QMessageBox::Abort) {
            break;
        }
    }


    // в журнал
    ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->    -    добавлен файл - официальное письмо в МО  " + code_mo + ".");
    QApplication::processEvents();


    // // ------------------------------------------------- // //
    // //  Текущая численность/выгрузка прикреплённых к МО  // //
    // // ------------------------------------------------- // //
    QString msg_count   = "Текущая численность/выгрузка прикреплённых к МО - " + code_mo;
    QString fname_count = dir_generate.path() + "/" + code_mo + " - persons_tab - " + date_report.toString("yyyy-MM-dd") + ".csv";

    if (f_count) {
        // ------------------
        // 2 - численность
        // ------------------
        QString sql_count = "select distinct d.fam, d.im, d.ot, d.sex, d.date_birth, d.snils, d.pol_v, d.vs_num, d.enp, d.pol_ser, d.pol_num, "
                            "       d.assig_type, d.assig_way, d.assig_date, d.code_head, d.code_mo, d.code_mp, d.code_ms, d.snils_mt ";
        if (f_address)
            sql_count   +=  "     , d.subj_name, d.dstr_name, d.nasp_name, d.strt_name, d.house, d.corp, d.quart \n";

        if (f_phones)
            sql_count   +=  "     , coalesce(e.phone_cell, e.phone_home, e.phone_work) as phone \n";

        sql_count       +=  "  from assig.sexages s \n"
                            "       join assig.sexages_lines sl on(sl.id_sexage=s.id) \n"
                            "       join assig.sexages_data  d  on(d.id_sexage_line=sl.id) \n";
        if (f_phones)
            sql_count   +=  "  left join persons e on(d.id_person=e.id) \n";

        sql_count       +=  " where sl.id_sexage=" + QString::number(id_sexage_tab) + " \n"
                            "   and sl.code_head='" + code_mo + "' \n"
                            " order by d.fam, d.im, d.ot, d.date_birth, d.sex ; \n";

        bool res = sql2csv(db, sql_count, false, fname_count, this);

        // добавим в архив
        QString com = "\"" + data_app.path_arch + "7z.exe\" a -tzip \"" + fname_arch + "\" \"" + fname_count + "\" -mx9";
        int res_com = 0;
        while ((res_com = myProcess->execute(com))!=0) {
            if (QMessageBox::warning(this, "Ошибка при создании архива",
                                     "Не удалось добавить файл " + fname_count + " \n  в архив " + fname_arch + ".\n"
                                     "Проверьте наличие и доступность программного интерфейса 7Z и доступность папки.\n\n"
                                     "Повторить попытку?",
                                     QMessageBox::Yes|QMessageBox::Abort,
                                     QMessageBox::Abort)==QMessageBox::Abort) {
                break;
            }
        }

        // в журнал
        ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->    -    добавлен файл - текущая численность.");
        QApplication::processEvents();
    }


    // // ------------------------------------ // //
    // //  Новые прикреплённые к МО за период  // //
    // // ------------------------------------ // //
    QString msg_assig   = "Новые прикреплённые к МО за период - " + code_mo;
    QString fname_assig = dir_generate.path() + "/" + code_mo + " - assig_tab - " + date_layer_old.toString("yyyy-MM-dd") + " - " + date_layer_new.toString("yyyy-MM-dd") + ".csv";

    if (f_assig) {
        // --------------
        // 3 - пришло
        // --------------
        QString sql_assig= "select distinct d.fam, d.im, d.ot, d.date_birth, d.sex, d.snils, d.pol_v, d.vs_num, d.enp, d.pol_ser, d.pol_num, \n"
                           "       d.assig_type_old, d.assig_way_old, d.assig_date_old, d.code_head_old, d.code_mo_old, d.code_mp_old, d.code_ms_old, d.snils_mt_old, \n"
                           "       d.assig_type_new, d.assig_way_new, d.assig_date_new, d.code_head_new, d.code_mo_new, d.code_mp_new, d.code_ms_new, d.snils_mt_new  \n";
        if (f_address)
            sql_assig  +=  "     , d.subj_name, d.dstr_name, d.nasp_name, d.strt_name, d.house, d.corp, d.quart \n";

        if (f_phones)
            sql_assig  +=  "     , coalesce(e.phone_cell, e.phone_home, e.phone_work) as phone \n";

        sql_assig      +=  "  from assig.moves m \n"
                           "       join assig.moves_lines ml on(ml.id_move=m.id) \n"
                           "       join assig.moves_data  d  on(d.id_move_line=ml.id) \n";
        if (f_phones)
            sql_assig  +=  "       join persons e on(e.id=m.id_person) \n";

        sql_assig      +=  " where ml.id_move=" + QString::number(id_move_tab) + " \n"
                           "   and ml.code_head='" + code_mo + "' "
                           "   and d.cell_name='cnt_add' \n"
                           " order by d.fam, d.im, d.ot, d.date_birth, d.sex ; \n";

        bool res = sql2csv(db, sql_assig, false, fname_assig, this);

        // добавим в архив
        QString com = "\"" + data_app.path_arch + "7z.exe\" a -tzip \"" + fname_arch + "\" \"" + fname_assig + "\" -mx9";

        int res_com = 0;
        while ((res_com = myProcess->execute(com))!=0) {
            if (QMessageBox::warning(this, "Ошибка при создании архива",
                                     "Не удалось добавить файл " + fname_assig + " \n  в архив " + fname_arch + ".\n"
                                     "Проверьте наличие и доступность программного интерфейса 7Z и доступность папки.\n\n"
                                     "Повторить попытку?",
                                     QMessageBox::Yes|QMessageBox::Abort,
                                     QMessageBox::Abort)==QMessageBox::Abort) {
                break;
            }
        }

        // в журнал
        ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->    -    добавлен файл - прикреплённые за период.");
        QApplication::processEvents();
    }


    // // ------------------------------ // //
    // //  Откреплённые от МО за период  // //
    // // ------------------------------ // //
    QString msg_unsig   = "Новые прикреплённые к МО за период - " + code_mo;
    QString fname_unsig = dir_generate.path() + "/" + code_mo + " - unsig_tab - " + date_layer_old.toString("yyyy-MM-dd") + " - " + date_layer_new.toString("yyyy-MM-dd") + ".csv";

    if (f_unsig) {
        // --------------
        // 3 - ушло
        // --------------
        QString sql_unsig= "select distinct d.fam, d.im, d.ot, d.date_birth, d.sex, d.snils, d.pol_v, d.vs_num, d.enp, d.pol_ser, d.pol_num, \n"
                           "       d.assig_type_old, d.assig_way_old, d.assig_date_old, d.code_head_old, d.code_mo_old, d.code_mp_old, d.code_ms_old, d.snils_mt_old, \n"
                           "       d.assig_type_new, d.assig_way_new, d.assig_date_new, d.code_head_new, d.code_mo_new, d.code_mp_new, d.code_ms_new, d.snils_mt_new  \n";
        if (f_address)
            sql_unsig  +=  "     , d.subj_name, d.dstr_name, d.nasp_name, d.strt_name, d.house, d.corp, d.quart \n";

        if (f_phones)
            sql_unsig  +=  "     , coalesce(e.phone_cell, e.phone_home, e.phone_work) as phone \n";

        sql_unsig      +=  "  from assig.moves m \n"
                           "       join assig.moves_lines ml on(ml.id_move=m.id) \n"
                           "       join assig.moves_data  d  on(d.id_move_line=ml.id) \n";
        if (f_phones)
            sql_unsig  +=  "       join persons e on(e.id=m.id_person) \n";

        sql_unsig      +=  " where ml.id_move=" + QString::number(id_move_tab) + " \n"
                           "   and ml.code_head='" + code_mo + "' "
                           "   and d.cell_name='cnt_dec' \n"
                           " order by d.fam, d.im, d.ot, d.date_birth, d.sex ; \n";

        bool res = sql2csv(db, sql_unsig, false, fname_unsig, this);

        // добавим в архив
        QString com = "\"" + data_app.path_arch + "7z.exe\" a -tzip \"" + fname_arch + "\" \"" + fname_unsig + "\" -mx9";

        int res_com = 0;
        while ((res_com = myProcess->execute(com))!=0) {
            if (QMessageBox::warning(this, "Ошибка при создании архива",
                                     "Не удалось добавить файл " + fname_unsig + " \n  в архив " + fname_arch + ".\n"
                                     "Проверьте наличие и доступность программного интерфейса 7Z и доступность папки.\n\n"
                                     "Повторить попытку?",
                                     QMessageBox::Yes|QMessageBox::Abort,
                                     QMessageBox::Abort)==QMessageBox::Abort) {
                break;
            }
        }

        // в журнал
        ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->    -    добавлен файл - откреплённые за период.");
        QApplication::processEvents();
    }


    db.commit();


    // // ---------------------------------------- // //
    // //  добавление в выгрузку остальных файлов  // //
    // // ---------------------------------------- // //
    if (f_other_files) {
        ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->    добавим дополнительные файлы");


        // перебор содержимого папки данных
        QString code_all = code_mo.left(3) + "000";

        QDir dir(dir_other_files);
        foreach(QString entry, dir.entryList()) {
            if ((entry == ".") || (entry == ".."))
                continue;
            QString fName = QString(dir_other_files.path() + "\\" + entry).replace("/","\\");
            QFileInfo fi(fName);

            QString fn_up = entry.toUpper();

            // найдём в папке данных все файлы, в названии которых есть код МО
            if ( fi.isFile() &&
                 ( entry.contains(code_mo)
                   || entry.contains(code_all) ) ) {
                // добавим этот файл
                int res_exe = 0;
                QString com = "\"" + data_app.path_arch + "7z.exe\" a -tzip \"" + fname_arch + "\" \"" + fName + "\" -mx9";
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
                    ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->    -    добавлен файл " + fName);
            }

            QString dName = QString(dir_other_files.path() + "\\" + entry).replace("/","\\");
            QFileInfo di(dName);

            // найдём в папке данных все папки, в названии которых есть код МО
            if ( di.isDir() &&
                 ( entry.contains(code_mo)
                   || entry.contains(code_all) ) ) {
                QDir fDir(dName);

                // запись в лог
                ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->    -    проверим папку " + dName);

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
                        QString dCom = "\"" + data_app.path_arch + "7z.exe\" a -tzip \"" + fname_arch + "\" \"" + dfName + "\" -mx9";
                        while ( (res_exe = myProcess->execute(dCom)) !=0 ) {
                            if ( QMessageBox::warning(this, ("Ошибка при добавлении файла"),
                                                 ("Произошла ошибка при попытке добавления файла \n") + dfName + "\n" +
                                                 ("в архив \n") + fname_arch + "\n\n" +
                                                 ("Повторить попытку?"),
                                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)==QMessageBox::No ) {
                                break;
                            }
                        }
                        // запись в лог
                        if (res_exe==0)
                            ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->    -    -     добавлен файл " + dfName);
                    }
                }
            }
        }
    }

    myProcess->close();
    delete myProcess;


    //удаление временных файлов

    // удалим временные файлы
    bool res_1 = QFile::remove(tmp_dir_s + "/content.xml");
    bool res_2 = QFile::remove(tmp_dir_s + "/styles.xml");
    bool res_3 = QFile::remove(fname_lettr);
    bool res_4 = QFile::remove(fname_count);
    bool res_5 = QFile::remove(fname_assig);
    bool res_6 = QFile::remove(fname_unsig);


    if (f_backup) {
        // перенос архива в папку для хранения
        QString fname_arch_backup = fname_arch;
        fname_arch_backup.replace(dir_generate.path(), dir_backup.path());
        if (QFile::exists(fname_arch_backup))
            QFile::remove(fname_arch_backup);
        bool res_bk = QFile::copy(fname_arch, fname_arch_backup);
        if (!res_bk) {
            QMessageBox::warning(this, "Внимание! Ошибка резервного копирования",
                                 "Папка резервного копирования данных не доступна:\n" +
                                 dir_backup.path() + "\n\n"
                                 "Работа прервана. Данные не скопированы.");
            this->setCursor(Qt::ArrowCursor);
            return false;
        }
    }

    // запись в лог
    //ui->te_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "  ->    -    готово.");
    //QApplication::processEvents();

    this->setCursor(Qt::ArrowCursor);
    return true;
}


void letters2MO_wnd::on_bn_MVipNet_clicked() {
    ui->line_VipNet->setText(QFileDialog::getExistingDirectory(this, "Выберите папку для резервирования данных", ui->line_VipNet->text()));
}

void letters2MO_wnd::on_bn_MSendVipnet_clicked() {
    // спросим папку, где расположены рассылаемые данные
    QStringList files = QFileDialog::getOpenFileNames(this, "Выберите файлы данных для рассылки", ui->line_gen_dir->text(),
                                                      "Архивы с письмами (*.zip *.7z *.rar);;"
                                                      "Не упакованные письма или данные (*.pdf *.odt *.doc, *.docx *.csv *.xls *.xlsx);;"
                                                      "Любые файлы (*.*)");
    if (files.isEmpty()) {
        QMessageBox::information(this, "Ничего не выбрано", "Файлы для рассылки не выбраны - операция отменена.");
        return;
    }

    // переберём ZIP-файлы в папке
    //QDir dir(dir_str);
    //QStringList filters;
    //filters << "*.zip" << "*.7z" << "*.rar" << "*.odt" << "*.ods" << "*.doc" << "*.xls" << "*.jpg";
    //QStringList files = dir.entryList(filters,QDir::Files,QDir::NoSort);

    // выберем список папок из папки рассылки "Деловой почты"
    QString dir_vipnet_str = ui->line_VipNet->text();
    QDir dir_vipnet(dir_vipnet_str);
    QStringList vip_dirs = dir_vipnet.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    QList<int> vip_ints;
    for (int i=0; i<vip_dirs.size(); i++) {
        int vip_int = 0;
        QDir vip_dir(vip_dirs.at(i));
        QString vip_str = vip_dir.dirName();
        vip_int = vip_str.toInt() + 360000;
        vip_ints.append(vip_int);
    }

    for (int i=0; i<files.size(); i++) {
        bool f_continue = false;
        QFileInfo file(files.at(i));
        QString fname = file.fileName();
        QString fbase = file.baseName() + "." + file.completeSuffix();

        // переберём папки ВипНет
        for (int j=0; j<vip_dirs.size(); j++) {
            QString code_mo = QString::number(vip_ints.at(j));
            if (fname.indexOf(code_mo,0)>=0) {

                // отправим этот файл на Випнет в эту МО
                QString fname_old = /*dir_str + "\\" +*/ files.at(i);
                QString fname_new = dir_vipnet_str + "\\" + vip_dirs.at(j) + "\\" + fbase;
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
