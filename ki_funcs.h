#ifndef KI_FUNCS
#define KI_FUNCS

#include <QString>
#include <QSqlDatabase>
#include <QTableView>
#include <QSqlQueryModel>


int max(int i, int j);

QString clear_num(QString str);
QString show_num(int num);
QString show_rubles(double num);

QString month_to_str(QDate date);
QString month_to_str_vp(QDate date);
QString date_to_str(QDate date);
QString monthyear_to_str(QDate date);

bool table2csv(QSqlDatabase &db, QString sql, QSqlQueryModel &model, QTableView *table, bool f_open, QString path="", QWidget *parent=NULL, QStringList names_old=QStringList(), QStringList names_new=QStringList());
bool sql2csv(QSqlDatabase &db, QString sql, bool f_open, QString fname_s, QWidget *parent=NULL, QStringList names_old=QStringList(), QStringList names_new=QStringList());

bool read_csvData(QString &text_s, QVector<QStringList> &data_lists, int &data_lists_cnt, QWidget *parent=0);


#endif // KI_FUNCS

