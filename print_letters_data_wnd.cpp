#include "print_letters_data_wnd.h"
#include "ui_print_letters_data_wnd.h"

#include <QMessageBox>
#include <QFileDialog>
#include "ki_funcs.h"

print_letters_data_wnd::print_letters_data_wnd(int style, QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    style(style),
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::print_letters_data_wnd)
{
    ui->setupUi(this);

    switch (style) {
    case 0:
        ui->lab_comment->setText("  Будет сохранено только ");
        ui->lab_comment_2->setText(" новых строк");
        break;
    default:
        ui->lab_comment->setText("  К данным будет добавлено ещё ");
        ui->lab_comment_2->setText(" строк");
        break;
    }
}

print_letters_data_wnd::~print_letters_data_wnd() {
    delete ui;
}

void print_letters_data_wnd::on_bn_cansel_clicked() {
    close();
}

void print_letters_data_wnd::on_te_data_textChanged() {
    QStringList lst = ui->te_data->toPlainText().trimmed().split("\n");
    int cnt = lst.count();
    ui->lab_cnt->setText(QString::number(cnt));
}

void print_letters_data_wnd::on_bn_load_data_clicked() {
    bool f_uniq = ui->ch_uniq_data->isChecked();

    // разбор CSV
    QStringList col_names;
    int data_lists_cnt = 50;
    QVector<QStringList> data_lists;
    for (int i=0; i<data_lists_cnt; i++) {
        data_lists.append(QStringList());
    }

    QString text_s = ui->te_data->toPlainText();
    text_s = text_s.replace(";", "\t");

    if (read_csvData(text_s, data_lists, data_lists_cnt, this)) {

        db.transaction();

        // если надо - удалим старые данные
        if (style==0) {
            QSqlQuery *query = new QSqlQuery(db);
            QString sql = "truncate reports.letters_data_tab RESTART IDENTITY ; ";

            mySQL.exec(this, sql, "Удаление старых данных для генерации писем", query, false, db, data_app);
        }

        // собственно загрузка данных
        QString data01, data02, data03, data04, data05, data06, data07, data08, data09, data10;
        QString data11, data12, data13, data14, data15, data16, data17, data18, data19, data20;
        QString data21, data22, data23, data24, data25, data26, data27, data28, data29, data30;
        QString data31, data32, data33, data34, data35, data36, data37, data38, data39, data40;
        QString data41, data42, data43, data44, data45, data46, data47, data48, data49, data50;

        int n = 0;
        for (int i=0; i<data_lists[0].size(); i++) {
            // прочитаем значения колонок
            data01  = data_lists[ 0].at(i);
            data02  = data_lists[ 1].at(i);
            data03  = data_lists[ 2].at(i);
            data04  = data_lists[ 3].at(i);
            data05  = data_lists[ 4].at(i);
            data06  = data_lists[ 5].at(i);
            data07  = data_lists[ 6].at(i);
            data08  = data_lists[ 7].at(i);
            data09  = data_lists[ 8].at(i);
            data10  = data_lists[ 9].at(i);

            data11  = data_lists[10].at(i);
            data12  = data_lists[11].at(i);
            data13  = data_lists[12].at(i);
            data14  = data_lists[13].at(i);
            data15  = data_lists[14].at(i);
            data16  = data_lists[15].at(i);
            data17  = data_lists[16].at(i);
            data18  = data_lists[17].at(i);
            data19  = data_lists[18].at(i);
            data20  = data_lists[19].at(i);

            data21  = data_lists[20].at(i);
            data22  = data_lists[21].at(i);
            data23  = data_lists[22].at(i);
            data24  = data_lists[23].at(i);
            data25  = data_lists[24].at(i);
            data26  = data_lists[25].at(i);
            data27  = data_lists[26].at(i);
            data28  = data_lists[27].at(i);
            data29  = data_lists[28].at(i);
            data30  = data_lists[29].at(i);

            data31  = data_lists[30].at(i);
            data32  = data_lists[31].at(i);
            data33  = data_lists[32].at(i);
            data34  = data_lists[33].at(i);
            data35  = data_lists[34].at(i);
            data36  = data_lists[35].at(i);
            data37  = data_lists[36].at(i);
            data38  = data_lists[37].at(i);
            data39  = data_lists[38].at(i);
            data40  = data_lists[39].at(i);

            data41  = data_lists[40].at(i);
            data42  = data_lists[41].at(i);
            data43  = data_lists[42].at(i);
            data44  = data_lists[43].at(i);
            data45  = data_lists[44].at(i);
            data46  = data_lists[45].at(i);
            data47  = data_lists[46].at(i);
            data48  = data_lists[47].at(i);
            data49  = data_lists[48].at(i);
            data50  = data_lists[49].at(i);

            if (f_uniq) {
                // проверка на уникальность данных
                QSqlQuery *query_tst = new QSqlQuery(db);
                QString sql_tst  =  "select count(*)"
                                    "  from reports.letters_data_tab "
                                    " where data01 = '" + data01 + "' "
                                    "   and data02 = '" + data02 + "' "
                                    "   and data03 = '" + data03 + "' "
                                    "   and data04 = '" + data04 + "' "
                                    "   and data05 = '" + data05 + "' "
                                    "   and data06 = '" + data06 + "' "
                                    "   and data07 = '" + data07 + "' "
                                    "   and data08 = '" + data08 + "' "
                                    "   and data09 = '" + data09 + "' "
                                    "   and data10 = '" + data10 + "' "

                                    "   and data11 = '" + data11 + "' "
                                    "   and data12 = '" + data12 + "' "
                                    "   and data13 = '" + data13 + "' "
                                    "   and data14 = '" + data14 + "' "
                                    "   and data15 = '" + data15 + "' "
                                    "   and data16 = '" + data16 + "' "
                                    "   and data17 = '" + data17 + "' "
                                    "   and data18 = '" + data18 + "' "
                                    "   and data19 = '" + data19 + "' "
                                    "   and data20 = '" + data20 + "' "

                                    "   and data21 = '" + data21 + "' "
                                    "   and data22 = '" + data22 + "' "
                                    "   and data23 = '" + data23 + "' "
                                    "   and data24 = '" + data24 + "' "
                                    "   and data25 = '" + data25 + "' "
                                    "   and data26 = '" + data26 + "' "
                                    "   and data27 = '" + data27 + "' "
                                    "   and data28 = '" + data28 + "' "
                                    "   and data29 = '" + data29 + "' "
                                    "   and data30 = '" + data30 + "' "

                                    "   and data31 = '" + data31 + "' "
                                    "   and data32 = '" + data32 + "' "
                                    "   and data33 = '" + data33 + "' "
                                    "   and data34 = '" + data34 + "' "
                                    "   and data35 = '" + data35 + "' "
                                    "   and data36 = '" + data36 + "' "
                                    "   and data37 = '" + data37 + "' "
                                    "   and data38 = '" + data38 + "' "
                                    "   and data39 = '" + data39 + "' "
                                    "   and data40 = '" + data40 + "' "

                                    "   and data41 = '" + data41 + "' "
                                    "   and data42 = '" + data42 + "' "
                                    "   and data43 = '" + data43 + "' "
                                    "   and data44 = '" + data44 + "' "
                                    "   and data45 = '" + data45 + "' "
                                    "   and data46 = '" + data46 + "' "
                                    "   and data47 = '" + data47 + "' "
                                    "   and data48 = '" + data48 + "' "
                                    "   and data49 = '" + data49 + "' "
                                    "   and data50 = '" + data50 + "' "
                                    "; ";

                mySQL.exec(this, sql_tst, "Проверка данных письма на уникальность", query_tst, false, db, data_app);

                int cnt_tst = query_tst->value(0).toInt();
                if (cnt_tst>0)
                    continue;
            }

            // добавление данных
            QSqlQuery *query_add = new QSqlQuery(db);
            QString sql_add = "insert into reports.letters_data_tab (data01, data02, data03, data04, data05, data06, data07, data08, data09, data10, "
                                                                   " data11, data12, data13, data14, data15, data16, data17, data18, data19, data20, "
                                                                   " data21, data22, data23, data24, data25, data26, data27, data28, data29, data30, "
                                                                   " data31, data32, data33, data34, data35, data36, data37, data38, data39, data40, "
                                                                   " data41, data42, data43, data44, data45, data46, data47, data48, data49, data50) "
                              "values ( '" + data01 + "' "
                                    " , '" + data02 + "' "
                                    " , '" + data03 + "' "
                                    " , '" + data04 + "' "
                                    " , '" + data05 + "' "
                                    " , '" + data06 + "' "
                                    " , '" + data07 + "' "
                                    " , '" + data08 + "' "
                                    " , '" + data09 + "' "
                                    " , '" + data10 + "' "

                                    " , '" + data11 + "' "
                                    " , '" + data12 + "' "
                                    " , '" + data13 + "' "
                                    " , '" + data14 + "' "
                                    " , '" + data15 + "' "
                                    " , '" + data16 + "' "
                                    " , '" + data17 + "' "
                                    " , '" + data18 + "' "
                                    " , '" + data19 + "' "
                                    " , '" + data20 + "' "

                                    " , '" + data21 + "' "
                                    " , '" + data22 + "' "
                                    " , '" + data23 + "' "
                                    " , '" + data24 + "' "
                                    " , '" + data25 + "' "
                                    " , '" + data26 + "' "
                                    " , '" + data27 + "' "
                                    " , '" + data28 + "' "
                                    " , '" + data29 + "' "
                                    " , '" + data30 + "' "

                                    " , '" + data31 + "' "
                                    " , '" + data32 + "' "
                                    " , '" + data33 + "' "
                                    " , '" + data34 + "' "
                                    " , '" + data35 + "' "
                                    " , '" + data36 + "' "
                                    " , '" + data37 + "' "
                                    " , '" + data38 + "' "
                                    " , '" + data39 + "' "
                                    " , '" + data40 + "' "

                                    " , '" + data41 + "' "
                                    " , '" + data42 + "' "
                                    " , '" + data43 + "' "
                                    " , '" + data44 + "' "
                                    " , '" + data45 + "' "
                                    " , '" + data46 + "' "
                                    " , '" + data47 + "' "
                                    " , '" + data48 + "' "
                                    " , '" + data49 + "' "
                                    " , '" + data50 + "' ) ; ";

            mySQL.exec(this, sql_add, "Добавление строки данных для генерации писем", query_add, false, db, data_app);

            n++;
            if (n%3==0) {
                ui->lab_cnt->setText(QString::number(n));
                QApplication::processEvents();
            }
        }

        db.commit();
        QMessageBox::information(this, "Данные загружены",
                                 "Данные загружены.");
        accept();

    } else {
        db.rollback();
        QMessageBox::warning(this, "Ошибка чтения данных",
                             "При попытке прочитать данные из текстового поля произошла ошибка.\n"
                             "Операция отменена. \n"
                             "Данные не изменились.");
    }
}

void print_letters_data_wnd::on_bn_data_file_clicked() {
    QString s = QFileDialog::getOpenFileName(this, "Выберите файл данных для генерации писем",
                                             data_app.path_temp,
                                             tr("текстовые файлы с разделителями (*.csv *.txt)"));
    s = s.trimmed();
    if (!s.isEmpty()) {
        ui->ln_data_file->setText(s);
        refresh_file_data();
    }
}

void print_letters_data_wnd::on_ln_data_file_editingFinished() {
    refresh_file_data();
}

void print_letters_data_wnd::on_combo_Codeset_activated(int index) {
    refresh_file_data();
}

void print_letters_data_wnd::refresh_file_data() {
    ui->te_data_2->clear();

    // пробуем открыть файл
    QFile csv_file(ui->ln_data_file->text());

    if (csv_file.open(QIODevice::ReadOnly)) {
        QTextStream csv_stream(&csv_file);
        QTextCodec *codec = QTextCodec::codecForName(ui->combo_Codeset->currentText().toLocal8Bit());
        csv_stream.setCodec(codec);
        csv_file.open(QIODevice::ReadOnly);

        // загрузим первые 50 строк
        for (int i=0; i<50; i++) {
            QString s = csv_stream.readLine().trimmed();
            if (!s.isEmpty())
                ui->te_data_2->appendPlainText(s);
        }

        csv_file.close();
    }
}

void print_letters_data_wnd::on_bn_load_data_2_clicked() {
    QString str_preview = ui->te_data->toPlainText().trimmed();
    if (str_preview.isEmpty()) {
        QMessageBox::warning(this, "Не выбран файл с данными?",
                             "Файл с данными не выбран, имеет не правильный формат или не доступен для чтения.\n"
                             "Выберите корректный файл с данными и повторите попытку.\n"
                             "Операция отменена. \n"
                             "Данные не изменились.");
        return;
    }

    bool f_uniq = ui->ch_uniq_data->isChecked();

    // разбор CSV
    QStringList col_names;
    int data_lists_cnt = 10;
    QVector<QStringList> data_lists;
    for (int i=0; i<data_lists_cnt; i++) {
        data_lists.append(QStringList());
    }

    // попробуем прочитать данные из файла
    QFile csv_file(ui->ln_data_file->text());

    if (csv_file.open(QIODevice::ReadOnly)) {
        QTextStream csv_stream(&csv_file);
        QTextCodec *codec = QTextCodec::codecForName(ui->combo_Codeset->currentText().toLocal8Bit());
        csv_stream.setCodec(codec);
        csv_file.open(QIODevice::ReadOnly);

        QString text_s = csv_stream.readAll();
        text_s = text_s.replace(";", "\t");

        if (read_csvData(text_s, data_lists, data_lists_cnt, this)) {

            db.transaction();

            // если надо - удалим старые данные
            if (style==0) {
                QSqlQuery *query = new QSqlQuery(db);
                QString sql = "truncate reports.letters_data_tab RESTART IDENTITY ; ";

                mySQL.exec(this, sql, "Удаление старых данных для генерации писем", query, false, db, data_app);
            }

            // собственно загрузка данных
            QString data01, data02, data03, data04, data05, data06, data07, data08, data09, data10;
            QString data11, data12, data13, data14, data15, data16, data17, data18, data19, data20;
            QString data21, data22, data23, data24, data25, data26, data27, data28, data29, data30;
            QString data31, data32, data33, data34, data35, data36, data37, data38, data39, data40;
            QString data41, data42, data43, data44, data45, data46, data47, data48, data49, data50;

            int n = 0;
            for (int i=0; i<data_lists[0].size(); i++) {
                // прочитаем значения колонок
                data01  = data_lists[ 0].at(i);
                data02  = data_lists[ 1].at(i);
                data03  = data_lists[ 2].at(i);
                data04  = data_lists[ 3].at(i);
                data05  = data_lists[ 4].at(i);
                data06  = data_lists[ 5].at(i);
                data07  = data_lists[ 6].at(i);
                data08  = data_lists[ 7].at(i);
                data09  = data_lists[ 8].at(i);
                data10  = data_lists[ 9].at(i);

                data11  = data_lists[10].at(i);
                data12  = data_lists[11].at(i);
                data13  = data_lists[12].at(i);
                data14  = data_lists[13].at(i);
                data15  = data_lists[14].at(i);
                data16  = data_lists[15].at(i);
                data17  = data_lists[16].at(i);
                data18  = data_lists[17].at(i);
                data19  = data_lists[18].at(i);
                data20  = data_lists[19].at(i);

                data21  = data_lists[20].at(i);
                data22  = data_lists[21].at(i);
                data23  = data_lists[22].at(i);
                data24  = data_lists[23].at(i);
                data25  = data_lists[24].at(i);
                data26  = data_lists[25].at(i);
                data27  = data_lists[26].at(i);
                data28  = data_lists[27].at(i);
                data29  = data_lists[28].at(i);
                data30  = data_lists[29].at(i);

                data31  = data_lists[30].at(i);
                data32  = data_lists[31].at(i);
                data33  = data_lists[32].at(i);
                data34  = data_lists[33].at(i);
                data35  = data_lists[34].at(i);
                data36  = data_lists[35].at(i);
                data37  = data_lists[36].at(i);
                data38  = data_lists[37].at(i);
                data39  = data_lists[38].at(i);
                data40  = data_lists[39].at(i);

                data41  = data_lists[40].at(i);
                data42  = data_lists[41].at(i);
                data43  = data_lists[42].at(i);
                data44  = data_lists[43].at(i);
                data45  = data_lists[44].at(i);
                data46  = data_lists[45].at(i);
                data47  = data_lists[46].at(i);
                data48  = data_lists[47].at(i);
                data49  = data_lists[48].at(i);
                data50  = data_lists[49].at(i);

                if (f_uniq) {
                    // проверка на уникальность данных
                    QSqlQuery *query_tst = new QSqlQuery(db);
                    QString sql_tst = "select count(*)"
                                      "  from reports.letters_data_tab "
                                      " where data01 = '" + data01 + "' "
                                      "   and data02 = '" + data02 + "' "
                                      "   and data03 = '" + data03 + "' "
                                      "   and data04 = '" + data04 + "' "
                                      "   and data05 = '" + data05 + "' "
                                      "   and data06 = '" + data06 + "' "
                                      "   and data07 = '" + data07 + "' "
                                      "   and data08 = '" + data08 + "' "
                                      "   and data09 = '" + data09 + "' "
                                      "   and data10 = '" + data10 + "' "

                                      "   and data11 = '" + data11 + "' "
                                      "   and data12 = '" + data12 + "' "
                                      "   and data13 = '" + data13 + "' "
                                      "   and data14 = '" + data14 + "' "
                                      "   and data15 = '" + data15 + "' "
                                      "   and data16 = '" + data16 + "' "
                                      "   and data17 = '" + data17 + "' "
                                      "   and data18 = '" + data18 + "' "
                                      "   and data19 = '" + data19 + "' "
                                      "   and data20 = '" + data20 + "' "

                                      "   and data21 = '" + data21 + "' "
                                      "   and data22 = '" + data22 + "' "
                                      "   and data23 = '" + data23 + "' "
                                      "   and data24 = '" + data24 + "' "
                                      "   and data25 = '" + data25 + "' "
                                      "   and data26 = '" + data26 + "' "
                                      "   and data27 = '" + data27 + "' "
                                      "   and data28 = '" + data28 + "' "
                                      "   and data29 = '" + data29 + "' "
                                      "   and data30 = '" + data30 + "' "

                                      "   and data31 = '" + data31 + "' "
                                      "   and data32 = '" + data32 + "' "
                                      "   and data33 = '" + data33 + "' "
                                      "   and data34 = '" + data34 + "' "
                                      "   and data35 = '" + data35 + "' "
                                      "   and data36 = '" + data36 + "' "
                                      "   and data37 = '" + data37 + "' "
                                      "   and data38 = '" + data38 + "' "
                                      "   and data39 = '" + data39 + "' "
                                      "   and data40 = '" + data40 + "' "

                                      "   and data41 = '" + data41 + "' "
                                      "   and data42 = '" + data42 + "' "
                                      "   and data43 = '" + data43 + "' "
                                      "   and data44 = '" + data44 + "' "
                                      "   and data45 = '" + data45 + "' "
                                      "   and data46 = '" + data46 + "' "
                                      "   and data47 = '" + data47 + "' "
                                      "   and data48 = '" + data48 + "' "
                                      "   and data49 = '" + data49 + "' "
                                      "   and data50 = '" + data50 + "' "
                                      "; ";

                    mySQL.exec(this, sql_tst, "Проверка данных письма на уникальность", query_tst, false, db, data_app);

                    int cnt_tst = query_tst->value(0).toInt();
                    if (cnt_tst>0)
                        continue;
                }

                // добавление данных
                QSqlQuery *query_add = new QSqlQuery(db);
                QString sql_add = "insert into reports.letters_data_tab (data01, data02, data03, data04, data05, data06, data07, data08, data09, data10, "
                                                                       " data11, data12, data13, data14, data15, data16, data17, data18, data19, data20, "
                                                                       " data21, data22, data23, data24, data25, data26, data27, data28, data29, data30, "
                                                                       " data31, data32, data33, data34, data35, data36, data37, data38, data39, data40, "
                                                                       " data41, data42, data43, data44, data45, data46, data47, data48, data49, data50) "
                                  "values ( '" + data01 + "' "
                                        " , '" + data02 + "' "
                                        " , '" + data03 + "' "
                                        " , '" + data04 + "' "
                                        " , '" + data05 + "' "
                                        " , '" + data06 + "' "
                                        " , '" + data07 + "' "
                                        " , '" + data08 + "' "
                                        " , '" + data09 + "' "
                                        " , '" + data10 + "' "

                                        " , '" + data11 + "' "
                                        " , '" + data12 + "' "
                                        " , '" + data13 + "' "
                                        " , '" + data14 + "' "
                                        " , '" + data15 + "' "
                                        " , '" + data16 + "' "
                                        " , '" + data17 + "' "
                                        " , '" + data18 + "' "
                                        " , '" + data19 + "' "
                                        " , '" + data20 + "' "

                                        " , '" + data21 + "' "
                                        " , '" + data22 + "' "
                                        " , '" + data23 + "' "
                                        " , '" + data24 + "' "
                                        " , '" + data25 + "' "
                                        " , '" + data26 + "' "
                                        " , '" + data27 + "' "
                                        " , '" + data28 + "' "
                                        " , '" + data29 + "' "
                                        " , '" + data30 + "' "

                                        " , '" + data31 + "' "
                                        " , '" + data32 + "' "
                                        " , '" + data33 + "' "
                                        " , '" + data34 + "' "
                                        " , '" + data35 + "' "
                                        " , '" + data36 + "' "
                                        " , '" + data37 + "' "
                                        " , '" + data38 + "' "
                                        " , '" + data39 + "' "
                                        " , '" + data40 + "' "

                                        " , '" + data41 + "' "
                                        " , '" + data42 + "' "
                                        " , '" + data43 + "' "
                                        " , '" + data44 + "' "
                                        " , '" + data45 + "' "
                                        " , '" + data46 + "' "
                                        " , '" + data47 + "' "
                                        " , '" + data48 + "' "
                                        " , '" + data49 + "' "
                                        " , '" + data50 + "' ) ; ";

                mySQL.exec(this, sql_add, "Добавление строки данных для генерации писем", query_add, false, db, data_app);

                n++;
                if (n%3==0) {
                    ui->lab_cnt->setText(QString::number(n));
                    QApplication::processEvents();
                }
            }

            db.commit();
            QMessageBox::information(this, "Данные загружены",
                                     "Данные загружены.");
            accept();

        } else {
            db.rollback();
            QMessageBox::warning(this, "Ошибка чтения данных",
                                 "При попытке прочитать данные из текстового поля произошла ошибка.\n"
                                 "Операция отменена. \n"
                                 "Данные не изменились.");
        }
    } else {
        db.rollback();
        QMessageBox::warning(this, "Ошибка открытия файла",
                             "При попытке открыть файл произошла ошибка.\n"
                             "Операция отменена. \n"
                             "Данные не изменились.");
    }

}
