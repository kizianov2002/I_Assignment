#include "show_tab_wnd.h"
#include "ui_show_tab_wnd.h"
#include <QFileDialog>

show_tab_wnd::show_tab_wnd(QString header, QString &sql, QSqlDatabase &db, s_data_app &data_app, QWidget *parent) :
    header(header),
    sql(sql),
    db(db),
    data_app(data_app),
    QDialog(parent),
    ui(new Ui::show_tab_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    ui->pan_wait->setVisible(false);

    sql = sql.replace(";"," ");
    this->setWindowTitle(header);
    refresh_tab();
}

show_tab_wnd::~show_tab_wnd() {
    delete ui;
}

void show_tab_wnd::refresh_tab() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }

    this->setCursor(Qt::WaitCursor);
    ui->lab_wait->setText("...  ПОДОЖДИТЕ, ИДЁТ ЧТЕНИЕ ДАННЫХ  ...");
    ui->pan_wait->setVisible(true);
    ui->tab_sql->setEnabled(false);
    QApplication::processEvents();

    model_tab.setQuery(sql,db);
    QString err2 = model_tab.lastError().driverText();

    // подключаем модель из БД
    ui->tab_sql->setModel(&model_tab);

    // обновляем таблицу
    ui->tab_sql->reset();
    ui->tab_sql->repaint();


    // подсчёт числа строк
    QSqlQuery *query = new QSqlQuery(db);
    QString sql_cnt = "select count(*) from (" + sql + ") q ;";

    if (!(mySQL.exec(this, sql_cnt, QString("Подсчёт числа строк в выборке"), *query, true, db, data_app))) {
        QMessageBox::warning(this, "Ошибка при подсчёте числа строк в выборке",
                             "При подсчёте числа строк в выборке произошла неожиданная ошибка.\n" + sql_cnt + "\n\nОперация отменена.");
    }
    query->next();
    int cnt = query->value(0).toInt();
    ui->lab_cnt->setText(QString::number(cnt));
    delete query;

    ui->pan_wait->setVisible(false);
    ui->tab_sql->setEnabled(true);
    QApplication::processEvents();
    this->setCursor(Qt::ArrowCursor);
}

void show_tab_wnd::on_bn_close_clicked() {
    close();
}

#include "qt_windows.h"
#include "qwindowdefs_win.h"
#include <shellapi.h>

void show_tab_wnd::on_bn_toCSV_clicked() {
    QString fname_s = data_app.path_out + QDate::currentDate().toString("yyyy-MM-dd") + "  -  " + data_app.smo_short + " - " + data_app.filial_name + "  -  " + this->header + ".csv";
    fname_s = QFileDialog::getSaveFileName(this, "Куда сохранить файл?",
                                           fname_s,
                                           "файлы данных (*.csv);;простой текст (*.txt)");
    if (fname_s.isEmpty()) {
        QMessageBox::warning(this,
                             "Не выбрано имя файла",
                             "Имя целевого файла не задано.\n\n"
                             "Операция отменена.");
        return;
    }
    if (QFile::exists(fname_s)) {
        if (QMessageBox::question(this,
                                  "Удалить старый файл?",
                                  "Файл уже существует.\n"
                                  "Удалить старый файл для того чтобы сохранить новый?",
                                  QMessageBox::Yes|QMessageBox::Cancel)==QMessageBox::Yes) {
            if (!QFile::remove(fname_s)) {
                QMessageBox::warning(this,
                                     "Ошибка при удалении старого файла",
                                     "При удалении старого файла произошла непредвиденная ошибка.\n\n"
                                     "Операция отменена.");
                return;
            }
        }
    }

    ui->lab_wait->setText("...  ПОДОЖДИТЕ, ИДЁТ СОХРАНЕНИЕ ДАННЫХ  ...");
    ui->pan_wait->setVisible(true);
    ui->tab_sql->setEnabled(false);
    QApplication::processEvents();

    // сохраним данные в файл
    QFile file;
    file.setFileName(fname_s);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this,
                             "Ошибка создания файла",
                             "При создании целевого файла произошла ошибка.\n\n"
                             "Операция отменена.");
        ui->pan_wait->setVisible(false);
        ui->tab_sql->setEnabled(true);
        QApplication::processEvents();
        return;
    }

    // подготовим поток для сохранения файла
    QTextStream stream(&file);
    QString codec_name = "Windows-1251";
    QTextCodec *codec = QTextCodec::codecForName(codec_name.toLocal8Bit());
    stream.setCodec(codec);


    QString head;
    int cnt_head = model_tab.columnCount();
    for (int i=0; i<cnt_head; i++) {
        if (ui->tab_sql->columnWidth(i)>1) {
            head += model_tab.headerData(i,Qt::Horizontal).toString().replace("\n"," ");
            if (i<cnt_head-1)  head += ";";
        }
    }
    stream << head << "\n";

    // переберём полученные данные и сохраним в файл
    QSqlQuery *query_export = new QSqlQuery(db);
    query_export->exec(sql);
    while (query_export->next()) {
        for (int j=0; j<cnt_head; j++) {
            if (ui->tab_sql->columnWidth(j)>5) {
                /*QString value = query_export->value(j).toString().trimmed().replace(";",",");
                if (!value.isEmpty()) {
                    if (j==51)  stream << "'";
                    stream << value;
                    if (j==51)  stream << "'";
                }
                if (j<cnt_head-1)  stream << ";";*/

                QString value;
                if (query_export->value(j).isNull()) {
                    value = "";
                } else if (query_export->value(j).type()==QMetaType::QDate) {
                    value = query_export->value(j).toDate().toString("dd.MM.yyyy");
                } else if ( query_export->value(j).type()==QMetaType::Int
                            || query_export->value(j).type()==QMetaType::Long
                            || query_export->value(j).type()==QMetaType::LongLong
                            || query_export->value(j).type()==QMetaType::ULong
                            || query_export->value(j).type()==QMetaType::ULongLong ) {
                    qlonglong n = query_export->value(j).toLongLong();
                    if (n==0)  value = "";
                    else  value = QString::number(n);
                } else {
                    value = query_export->value(j).toString().trimmed().replace(";",",");
                }
                if (!value.isEmpty()) {
                    //stream << "'";
                    stream << value;
                    //stream << "'";
                }
                if (j<cnt_head-1)  stream << ";";
            }
        }
        stream << "\n";
    }
    file.close();

    ui->pan_wait->setVisible(false);
    ui->tab_sql->setEnabled(true);
    QApplication::processEvents();

    // ===========================
    // собственно открытие шаблона
    // ===========================
    // открытие полученного ODT
    /*long result = (long)*/ShellExecute(0, 0, reinterpret_cast<const WCHAR*>(fname_s.utf16()), 0, 0, SW_NORMAL);
}
