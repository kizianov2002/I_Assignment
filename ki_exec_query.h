#ifndef EXEC_SQL
#define EXEC_SQL

#include <QtSql>
#include <QMessageBox>
#include <QRegExp>

#include "s_data_app.h"

class exec_SQL {
public:
    // проверка соединения с базой данных
    bool test_connect(QWidget *parent, QSqlQuery &query) {
        QString sql = " select count(*) from db_recent ;";
        bool res = query.exec(sql);
        if (!res || !query.next()) {
            QMessageBox::critical(parent,
                                  "Ошибка подключения к базе данных!",
                                  "В процессе стандартной проверки подключения к базе данных произошла ошибка.\n"
                                  "Возможно, нарушено соединение с сервером базы полисов.\n"
                                  "Проверьте соединение с сетью Интернет и работоспособность сервера полисов.");
            query.clear();
            return false;
        }
        if (query.value(0).toInt()<1) {
            QMessageBox::critical(parent,
                                  "Нарушена структура базы данных!",
                                  "В процессе стандартной проверки базы данных, в базе данных не найдены необходимые служебные структуры.\n"
                                  "Возможно, программа подключена не к той базе данных, структура  "
                                  "базы данных нарушена или соединение с базой данных нарушео по техническим причинам.");
            query.clear();
            return false;
        }
        query.clear();
        return true;
    }

    // проверка соединения с базой данных
    bool init_update(QWidget *parent, QSqlQuery &query, s_data_app &data_app) {
        QString sql = "truncate db_recent ; "
                      "insert into db_recent(ocato, id_point, id_operator, dt_act) "
                      "values( '" + data_app.ocato + "', "
                      "         " + QString::number(data_app.id_point) + ", "
                      "         " + QString::number(data_app.id_operator) + ", "
                      "         CURRENT_TIMESTAMP ) ; ";
        bool res = query.exec(sql);
        if (!res || !query.next()) {
            QMessageBox::critical(parent,
                                  "Ошибка инициализации пишущего запроса!",
                                  "В процессе инициализации пишущего запроса произошла ошибка.\n"
                                  "Возможно, нарушеа структура таблицы db_recent базы полисов.\n"
                                  "Проверьте соединение с сетью Интернет и работоспособность сервера полисов.");
            query.clear();
            return false;
        }
        query.clear();
        return true;
    }

    //bool myBYTEA(QString &fname, QString &res_str);     // перевод массива байт в строку bytea PostgreSQL
    bool myBYTEA(QString &fname, QString& res_str) {
        QString ms = "0123456789ABCDEF";
        QFile f(fname);
        if (!f.open(QIODevice::ReadOnly)) {
            return false;
        }
        res_str = "\\x";
        int m = 0;
        char cd[1024];
        while (!f.atEnd()) {
            int n = f.read(cd, 1024);
            for (int i=0; i<n; i++) {
                unsigned int v = (unsigned char)(cd[i]);
                int n1 = v>>4;
                int n2 = v%16;
                res_str += ms.at(n1);
                res_str += ms.at(n2);
            }
            m++;
        }
        return true;
    }

    bool exec(QWidget *parent, QString &sql, QString caption, QSqlQuery &query, bool f_ret, QSqlDatabase &db, s_data_app &data_app) {
        // проверка стабильности соединения с БД
        if (!test_connect(parent, query))
            return false;

        // запись кода ОКАТО в db_recent для пишущих запросов
        QRegExp exp("\b(insert|update|delete|truncate)\b");
        if (exp.indexIn(sql)>=0) {
            if (!init_update(parent, query, data_app))
                return false;
        }
        // собственно пробуем выполнить запрос sql
        bool res = false;
        if (f_ret) {
            do {
                res = query.exec(sql);
            } while (!res &&
                     QMessageBox::warning (parent, "Нестабильное соединение или ошибка SQL ...", caption + "\n\nПри выполнении запроса SQL произошла ошибка.\nПроверьте соединения с базой данных ИНКО-МЕД, проверьте соединение с Интернет. Попробуйте повторить запрос.\nЕсли ошибка будет повторяться - снимите скриншот с этого окна и со старшего окна, и направьте разаработчику с описанием ситуации, в которой произошла ошибка.\n\nЗапрос\n" +
                                           sql + "\n\nОшибка\ndriver: " +
                                           query.lastError().databaseText() + "\nDB: " +
                                           query.lastError().driverText() + "\n\nТелефон для связи с разработчиком: 8(920)449-54-88 \n\nПовторить?",
                                           QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)==QMessageBox::Yes);
        } else {
            res = query.exec(sql);
            if (!res) {
                QMessageBox::critical (parent, "Нестабильное соединение или ошибка SQL", caption + "\n\nПри выполнении запроса SQL произошла ошибка.\n\nЗапрос\n" +
                                       sql + "\n\nОшибка\n" +
                                       query.lastError().databaseText() + "\n" +
                                       query.lastError().driverText() + "\n\nТелефон для связи с разработчиком: 8(920)449-54-88 \n\nПовторить?");
            }
        }
        return res;
    }

    bool exec(QWidget *parent, QString &sql, QString caption, QSqlQuery *query, bool f_ret, QSqlDatabase &db, s_data_app &data_app) {
        return exec(parent, sql, caption, *query, f_ret, db, data_app);
    }
};


#endif // EXEC_SQL
