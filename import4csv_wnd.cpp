#include "import4csv_wnd.h"
#include "ui_import4csv_wnd.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QTextCodec>
#include <QSqlQuery>

import4csv_wnd::import4csv_wnd(QSqlDatabase &db, s_data_app &data_app, s_data_import &data_import, QWidget *parent) :
    db(db), data_import(data_import), data_app(data_app),
    QDialog(parent), ui(new Ui::import4csv_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    // проверим разрешение экрана
    QDesktopWidget *scr_widget = QApplication::desktop();
    int screen_w = scr_widget->width();
    int screen_h = scr_widget->height();
    this->setGeometry(QRect(100, 40, screen_w-200, screen_h-140));

    for(int i=0; i<50; i++)
        data_lists[i] = NULL;
    data_lists_cnt = 0;

    ui->pan_noData->show();
    ui->pan_rdData->hide();
    ui->pan_chData->hide();
    ui->pan_import->hide();
    ui->bn_Import->setEnabled(false);
    QApplication::processEvents();

    /*list_line_headers.append(ui->line_header_0);
    list_text_dataStr.append(ui->text_dataStr_0);
    list_comb_formats.append(ui->combo_fieldFmt_0);
    list_comb_columns.append(ui->combo_column_0);*/

    ui->combo_Capts->setCurrentIndex(1);
    ui->line_CSV->setText(data_import.file_name);
    ui->combo_Table->setCurrentIndex(ui->combo_Table->findText(data_import.tab_name));

    get_fileData();
    refresh_tables();
}

import4csv_wnd::~import4csv_wnd() {
    delete ui;
}

void import4csv_wnd::refresh_combo_Table() {
    QStringList tables = db.tables();

    ui->combo_Table->clear();

    foreach (QString tabname, tables) {
        ui->combo_Table->addItem(tabname);
    }
}

void import4csv_wnd::refresh_tables() {
    QStringList tables  = db.tables();
    QStringList tables2;
    foreach (QString tabname, tables) {
        if ( tabname.left(1)!="_" ) {;
            int pos=0;
            for (pos=0; pos<tables2.size(); pos++) {
                if (tabname<tables2.at(pos))
                    break;
            }
            tables2.insert(pos, tabname);
    }   }

    ui->combo_Table->clear();
    int n = 0;
    foreach (QString tabname, tables2) {
        ui->combo_Table->addItem(tabname);
        if (data_import.tab_name==tabname)
            n = ui->combo_Table->count();
    }
    ui->combo_Table->setCurrentIndex(n-1);
}

void import4csv_wnd::on_bn_CSV_clicked() {
    ui->line_CSV->setText(QFileDialog::getOpenFileName(this, ("Выберите CSV-файл данных для загрузки"),
                                                       ui->line_CSV->text(), ("Таблично-текстовый файл (*.txt *.csv)")));
    get_fileData();
    ui->bn_load_data->setEnabled(true);
}

void import4csv_wnd::get_fileData() {
    // пробуем открыть файл
    QFile csv_file(ui->line_CSV->text());

    if (csv_file.open(QIODevice::ReadOnly)) {
        QTextStream csv_stream(&csv_file);
        QTextCodec *codec = QTextCodec::codecForName(ui->combo_Codeset->currentText().toLocal8Bit());
        csv_stream.setCodec(codec);
        csv_file.open(QIODevice::ReadOnly);
        QString csv_string = csv_stream.read(3000).replace("\'\'","\"").replace("\'","\"");

        // проверим число точек с запятой
        int cnt_semidot = 0;
        {   int pos = 0;   while((pos=csv_string.indexOf(";", pos+1))>0)    cnt_semidot++;   }
        // проверим число запятых
        int cnt_semi = 0;
        {   int pos = 0;   while((pos=csv_string.indexOf(",", pos+1))>0)    cnt_semi++;   }
        // проверим число табуляций
        int cnt_tab = 0;
        {   int pos = 0;   while((pos=csv_string.indexOf("\t", pos+1))>0)   cnt_tab++;   }

        // проверим число \r\n
        int cnt_crcn = 0;
        {   int pos = 0;   while((pos=csv_string.indexOf("\r\n", pos+1))>0) cnt_crcn++;   }
        // проверим число \n
        int cnt_cn = 0;
        {   int pos = 0;   while((pos=csv_string.indexOf("\n", pos+1))>0)   cnt_cn++;   }
        // проверим число \r
        int cnt_cr = 0;
        {   int pos = 0;   while((pos=csv_string.indexOf("\r", pos+1))>0)   cnt_cr++;   }

        // проверим число апострофов
        int cnt_1qote = 0;
        {   int pos = 0;   while((pos=csv_string.indexOf("\'", pos+1))>0)   cnt_1qote++;   }
        // проверим число кавычек
        int cnt_2qote = 0;
        {   int pos = 0;   while((pos=csv_string.indexOf("\"", pos+1))>0)   cnt_2qote++;   }

        // ----------------------------
        // настройка выпадающих списков
        // ----------------------------
        // разделитель полей
        if (cnt_semidot>cnt_semi && cnt_semidot>cnt_tab)
            ui->combo_FieldsDiv->setCurrentIndex(0);
        if (cnt_tab>cnt_semi && cnt_tab>cnt_semidot)
            ui->combo_FieldsDiv->setCurrentIndex(1);
        if (cnt_semi>cnt_semidot && cnt_semi>cnt_tab)
            ui->combo_FieldsDiv->setCurrentIndex(2);

        // разделитель строк
        int n_lines = 0;
        if (cnt_crcn==cnt_cn && cnt_crcn==cnt_cr) {
            ui->combo_LinesDiv->setCurrentIndex(0);
            n_lines = cnt_crcn; }
        if (cnt_cn>cnt_crcn && cnt_cn>cnt_cr) {
            ui->combo_LinesDiv->setCurrentIndex(1);
            n_lines = cnt_cn; }
        if (cnt_cr>cnt_crcn && cnt_cr>cnt_cn) {
            ui->combo_LinesDiv->setCurrentIndex(2);
            n_lines = cnt_cr; }

        // обрамление полей
        if (cnt_2qote<n_lines && cnt_1qote<n_lines)
            ui->combo_FieldsRnd->setCurrentIndex(0);
        if (cnt_2qote>=n_lines && cnt_2qote>cnt_1qote)
            ui->combo_FieldsRnd->setCurrentIndex(1);
        if (cnt_1qote>=n_lines && cnt_1qote>cnt_2qote)
            ui->combo_FieldsRnd->setCurrentIndex(2);

        csv_file.close();
        QFile csv_file2(ui->line_CSV->text());
        QTextStream csv_stream2(&csv_file2);
        QTextCodec *codec2 = QTextCodec::codecForName(ui->combo_Codeset->currentText().toLocal8Bit());
        csv_stream2.setCodec(codec);
        csv_file2.open(QIODevice::ReadOnly);

        // загрузим первые 10 строк
        lines_10.clear();
        for (int i=0; i<10; i++)
             lines_10.append(csv_stream2.readLine());

        if ( ui->combo_Capts->currentIndex()>0 )
             ui->line_Capts->setText(lines_10.at(ui->combo_Capts->currentIndex()-1));

        csv_file2.close();
    }
}

void import4csv_wnd::on_combo_Codeset_activated(int index) {
    get_fileData();
}

void import4csv_wnd::on_line_CSV_textChanged(const QString &arg1) {
    get_fileData();
}

void import4csv_wnd::on_combo_Capts_activated(int index) {
    if ( ui->combo_Capts->currentIndex()>0 )
         ui->line_Capts->setText(lines_10.at(ui->combo_Capts->currentIndex()-1));
    else ui->line_Capts->clear();
}

void import4csv_wnd::on_combo_Capts_currentIndexChanged(int index) {
    ui->spin_DataLines->setValue(index+1);
}

void import4csv_wnd::on_bn_close_clicked() {
    close();
}



bool import4csv_wnd::load_csvData(QString file_name) {
    ui->pan_noData->hide();
    ui->pan_rdData->show();
    ui->pan_chData->hide();
    ui->pan_import->hide();
    ui->bn_Import->setEnabled(false);
    QApplication::processEvents();

    // очистим массив данных
    for (int i=0; (i<data_lists_cnt && i<50); i++) {
        data_lists[i]->clear();
        delete data_lists[i];
    }
    data_lists_cnt = 0;

    // пробуем открыть файл
    QFile file(file_name);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, ("Ошибка открытия файла"),
                             ("При попытке открыть выбранный файл произошла ошибка. \nОперация отменена."));
        return false;
    }

    // сформируем поток
    QTextStream stream(&file);
    stream.setCodec (QTextCodec::codecForName(ui->combo_Codeset->currentText().toLocal8Bit()));

    QString div_fields = "\t";
    switch (ui->combo_FieldsDiv->currentIndex()) {
    case 0: div_fields = ";";
        break;
    case 1: div_fields = "\t";
        break;
    case 2: div_fields = ",";
        break;
    }

    QString div_lines = "\r\n";
    switch (ui->combo_LinesDiv->currentIndex()) {
    case 0: div_lines = "\r\n";
        break;
    case 1: div_lines = "\n";
        break;
    case 2: div_lines = "\r";
        break;
    }

    QString rnd_fields = "";
    switch (ui->combo_FieldsRnd->currentIndex()) {
    case 0: rnd_fields = "";
        break;
    case 1: rnd_fields = "\"";
        break;
    case 2: rnd_fields = "\'";
        break;
    }

    // перебор строк файла
    int line_counter = 0;
    while (true) {
        QString values_str = stream.readLine().replace("\'\'","\"").replace("\'","\"");;
        if (values_str.isEmpty())
            break;

        // разберём список полей
        QString value;
        int len = values_str.length(), pos = 0;
        int n_value = 0;
        /*QStringList values;
        values.clear();

        QString sql_data_headers = "";
        QString sql_data_values = "";*/

        while (pos<len && pos!=-1) {
            int pos1 = values_str.indexOf(div_fields, pos);
            if (pos1<0)  pos1 = len;
            value = values_str.mid(pos, pos1-pos).simplified().trimmed();
            if (value.left(1)==rnd_fields && value.right(1)==rnd_fields)
                value = value.mid(1, value.length()-2);
            if (value.left(1)=="\"")
                value = value.mid(1, value.length()-1);
            if (value.right(1)=="\"")
                value = value.mid(0, value.length()-1);

            value = value.replace("\"", "").replace("'", "");

            // если надо - добавим столбец
            while (data_lists_cnt<=n_value) {
                data_lists[data_lists_cnt] = new QStringList();
                data_lists_cnt++;
            }

            // добавим значение
            QStringList *list = data_lists[n_value];
            list->append(value);

            if (pos1<0 || pos1==len) break;
            else pos = pos1 + div_fields.length();

            n_value++;
        }
        line_counter++;

        if (line_counter%13==0) {
            ui->lab_rdData->setText(" ( " + QString::number(line_counter) + " ) ");
            QApplication::processEvents();
        }
    }

    return true;
}


void import4csv_wnd::on_bn_load_data_clicked() {
    if (load_csvData(ui->line_CSV->text()) && data_lists_cnt>0) {
        int win_width = this->width();
        int maxWidth = (win_width-30)/data_lists_cnt;
        if (maxWidth<120)  maxWidth = 120;

        // удалим старые компоненты
        for (int i=list_line_headers.count()-1; i>=0; i--)
            list_line_headers.removeAt(i);
        for (int i=list_text_dataStr.count()-1; i>=0; i--)
            list_text_dataStr.removeAt(i);
        for (int i=list_comb_formats.count()-1; i>=0; i--)
            list_comb_formats.removeAt(i);
        for (int i=list_comb_columns.count()-1; i>=0; i--)
            list_comb_columns.removeAt(i);

        // получим список полей целевой таблицы
        QStringList tabFields;
        QString table = ui->combo_Table->currentText();
        int dot = table.indexOf(".", 0);
        QString schema = "";
        if (dot>0) {
            schema = table.left(dot);
            table  = table.right(table.length() - dot - 1);
        }
        QString sql_fields = "SELECT column_name, table_name, ordinal_position "
                             "  FROM information_schema.columns "
                             " WHERE table_name='" + table + "' "
                                   + QString(schema.isEmpty() ? "" : QString(" and table_schema='" + schema + "' ") ) +
                             " order by ordinal_position ; ";
        /*
        "select RDB$FIELD_NAME "
        "  from rdb$relation_fields "
        " where RDB$RELATION_NAME = '" + ui->combo_Table->currentText() + "' ; ";
        */
        QSqlQuery *query_fields = new QSqlQuery(db);
        tabFields.append((" - нет - "));
        if (query_fields->exec(sql_fields)) {
            while (query_fields->next()) {
                QString field_str = query_fields->value(0).toString().trimmed();
                if (field_str!="ID")  tabFields.append(field_str);
        }   }

        // сформируем компоненты
        for (int i=0; i<data_lists_cnt; i++) {
            if (i==0)  {
                QLabel *l0 = new QLabel(("заголовки данных"));
                l0->setMaximumWidth(maxWidth);
                ui->grid_data->addWidget(l0, 0, i, 1, 1);  }
            QLineEdit *w0 = new QLineEdit(this);
            w0->setMaximumWidth(maxWidth);
            w0->setReadOnly(true);
            list_line_headers.append(w0);
            ui->grid_data->addWidget(w0, 1, i, 1, 1);
            if (ui->combo_Capts->currentIndex()==0)
                 w0->setText(QString("поле № ") + QString::number(i));
            else if (data_lists[i]->size()>(ui->combo_Capts->currentIndex()-1))
                w0->setText(data_lists[i]->at(ui->combo_Capts->currentIndex()-1));

            if (i==0)  {
                QLabel *l1 = new QLabel(("данные"));
                l1->setMaximumWidth(maxWidth);
                ui->grid_data->addWidget(l1, 2, i, 1, 1);  }
            QTextEdit *w1 = new QTextEdit(this);
            w1->setMaximumWidth(maxWidth);
            w1->setWordWrapMode(QTextOption::NoWrap);
            w1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            w1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            list_text_dataStr.append(w1);
            ui->grid_data->addWidget(w1, 3, i, 1, 1);
            int min_k = ui->spin_DataLines->value()-1;
            int max_k = ui->spin_DataLines->value()-1+ui->spin_ViewLines->value()-1;
            if (max_k>data_lists[0]->count())  max_k = data_lists[0]->count()-1;
            for (int k=min_k; k<max_k; k++)
                if (data_lists[i]->size()>k)
                    w1->append(data_lists[i]->at(k));

            if (i==0)  {
                QLabel *l3 = new QLabel(("вставить в \nполе таблицы"));
                l3->setMaximumWidth(maxWidth);
                ui->grid_data->addWidget(l3, 4, i, 1, 1);
            }
            QComboBox *w3 = new QComboBox(this);
            w3->setMaximumWidth(maxWidth);
            w3->setMaxVisibleItems(50);
            list_comb_columns.append(w3);
            ui->grid_data->addWidget(w3, 5, i, 1, 1);
            w3->clear();
            w3->addItems(tabFields);
            w3->setCurrentIndex(w3->findText(w0->text()));
            if (w3->currentIndex()<0)
                w3->setCurrentIndex(i+1);

            QComboBox *w2 = new QComboBox(this);
            w2->setMaximumWidth(maxWidth);
            w2->setMaxVisibleItems(50);
            list_comb_formats.append(w2);
            ui->grid_data->addWidget(w2, 6, i, 1, 1);
            w2->clear();
            w2->addItem(("текст (в кавычках)"));
            w2->addItem(("число (как есть)"));
            w2->addItem(("ГГГГММДД (Бурляев)"));
            w2->addItem(("ГГГГ-ММ-ДД (евро)"));
            w2->addItem(("ДД.ММ.ГГГГ (рус)"));
            w2->addItem(("ММ/ДД/ГГГГ (США)"));
            w2->addItem(("ДД.ММ.ГГ (19xx г.)"));
            w2->addItem(("ДД.ММ.ГГ (20xx г.)"));
            w2->setCurrentIndex(0);
        }
        ui->pan_noData->hide();
        ui->pan_rdData->hide();
        ui->pan_chData->show();
        ui->pan_import->hide();
        ui->bn_Import->setEnabled(true);
        QApplication::processEvents();
    } else {
        ui->pan_noData->show();
        ui->pan_rdData->hide();
        ui->pan_chData->hide();
        ui->pan_import->hide();
        ui->bn_Import->setEnabled(true);
        QApplication::processEvents();

        QMessageBox::warning(this, ("Ничего не загружено"),
                             ("В открываемом файле нет ни одной строки данных. \nОперация отменена."));
    }
}

void import4csv_wnd::on_bn_Import_clicked() {
    ui->pan_noData->hide();
    ui->pan_rdData->hide();
    ui->pan_chData->hide();
    ui->pan_import->show();
    ui->bn_Import->setEnabled(false);
    QApplication::processEvents();

    db.transaction();

    if ( ui->ch_ClearTab->isChecked()
         && QMessageBox::question(this, ("Нужно подтверждение"),
                                  ("Вы действительно хотите удалить все данные, ранее загруженные в эту таблицу?"),
                                  QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes) {
        QString sql_clear = "delete from " + ui->combo_Table->currentText() + " ; ";
        QSqlQuery *query_clear = new QSqlQuery(db);
        if (!query_clear->exec(sql_clear)) {
            delete query_clear;
            QMessageBox::warning(this, ("Ошибка очистки целевой таблицы"),
                                 ("Ошибка очистки целевой таблицы. Импорт данных отменён.") +
                                 sql_clear);
            db.rollback();

            ui->pan_noData->hide();
            ui->pan_rdData->hide();
            ui->pan_chData->show();
            ui->pan_import->hide();
            ui->bn_Import->setEnabled(true);
            QApplication::processEvents();

            return;
        }
        delete query_clear;
    }

    int ln_min = ui->spin_DataLines->value()-1;
    int ln_max = data_lists[0]->count();
    for(int ln=ln_min; ln<ln_max; ln++) {
        // подготовим списки для импорта
        QString insert_fields = "";
        QString insert_values = "";

        for(int fl=0; fl<data_lists_cnt; fl++) {
            int col_n = list_comb_columns.at(fl)->currentIndex();
            if (col_n>0) {
                // это поле надо добавить
                if (!insert_fields.isEmpty())  insert_fields += ", ";
                insert_fields += list_comb_columns.at(fl)->currentText().trimmed();

                if (!insert_values.isEmpty())  insert_values += ", ";
                if (data_lists[fl]->at(ln).trimmed().isEmpty()) {
                    insert_values += "NULL";
                } else {
                    QString value = data_lists[fl]->at(ln).trimmed();
                    switch (list_comb_formats.at(fl)->currentIndex()) {
                    case 1: insert_values += value;
                        break;
                    case 2: insert_values += "\'" + value.left(4) + "-" + value.mid(4,2) + "-" + value.right(2) + "\'";
                        break;
                    case 3: insert_values += "\'" + value + "\'";
                        break;
                    case 4: insert_values += "\'" + value.right(4) + "-" + value.mid(3,2) + "-" + value.left(2) + "\'";
                        break;
                    case 5: insert_values += "\'" + value.right(4) + "-" + value.left(2) + "-" + value.mid(3,2) + "\'";
                        break;
                    case 6: insert_values += "\'19" + value.right(2) + "-" + value.mid(3,2) + "-" + value.left(2) + "\'";
                        break;
                    case 7: insert_values += "\'20" + value.right(2) + "-" + value.mid(3,2) + "-" + value.left(2) + "\'";
                        break;
                    default: insert_values += "\'" + value + "\'";
                    }
                }
            }
        }

        if (insert_fields.isEmpty()) {
            QMessageBox::warning(this, ("Нечего добавлять"),
                                 ("Для импорта не выбрано ни одного поля. Импорт данных отменён.\n") );
            db.rollback();

            ui->pan_noData->hide();
            ui->pan_rdData->hide();
            ui->pan_chData->show();
            ui->pan_import->hide();
            ui->bn_Import->setEnabled(true);
            QApplication::processEvents();

            return;
        }

        QString sql_insert =
                "insert into " + ui->combo_Table->currentText() + "( " + insert_fields + " ) "
                " values ( " + insert_values + " ); ";
        QSqlQuery *query_insert = new QSqlQuery(db);
        while (!query_insert->exec(sql_insert)) {
            delete query_insert;
            int ret_f = QMessageBox::warning(this, ("Ошибка добавления новой записи"),
                                             "Ошибка добавления новой записи. Импорт данных отменён.\n\n"
                                             "Строка " + QString::number(ln) + "\n\n"
                                             "Возможные ошибки:\n"
                                             "    -  неправильно выбран тип данных\n"
                                             "    -  выбранный тип данных не совместим с типом целевой колонки\n"
                                             "    -  одновременная вставка нескольких значений в одну и туже колонку\n\n"
                                             "Поправьте параметры импорта данных и повторите попытку.\n\n" +
                                             sql_insert,
                                             QMessageBox::Retry|QMessageBox::Ignore|QMessageBox::Abort, QMessageBox::Ignore);
            if (ret_f==QMessageBox::Abort) {
                db.rollback();

                ui->pan_noData->hide();
                ui->pan_rdData->hide();
                ui->pan_chData->show();
                ui->pan_import->hide();
                ui->bn_Import->setEnabled(true);
                QApplication::processEvents();

                return;
            }
            if (ret_f==QMessageBox::Ignore) {
                break;
            }
            if (ret_f==QMessageBox::Retry) {
                QSqlQuery *query_insert = new QSqlQuery(db);
            }
        }
        delete query_insert;

        if (ln%231==0) {
            ui->lab_import->setText(" ( " + QString::number(ln) + " ) ");
            QApplication::processEvents();
        }

        if (ln%2310==0) {
            db.commit();
            db.transaction();
        }
    }
    db.commit();

    if (data_import.file_name.isEmpty()) {
        ui->ch_ClearTab->setChecked(false);
        ui->bn_load_data->setEnabled(false);
        ui->bn_Import->setEnabled(false);

        QMessageBox::information(this, ("Готово"), ("Готово"));
    } else {
        close();
    }
}
