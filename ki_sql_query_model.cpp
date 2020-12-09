#include "ki_sql_query_model.h"
#include <QTextCodec>
#include <QDate>

ki_persSqlQueryModel::ki_persSqlQueryModel(QObject *parent) : QSqlQueryModel(parent)
 { }

QVariant ki_persSqlQueryModel::data(const QModelIndex &index, int role) const {
    QVariant value = QSqlQueryModel::data(index, role);

    switch (role) {
        case Qt::DisplayRole: // Данные для отображения
        if (index.column()==12) {
            if (value.toInt()==0) {
                return QString ("~ новый");
            } else if (value.toInt()==1) {
                return QString ("+ ВС");
            } else if (value.toInt()==2) {
                return QString ("+ полис");
            } else if (value.toInt()==3) {
                return QString ("+ перестр.");
            } else if (value.toInt()==4) {
                return QString ("+ без пол.");
            } else if (value.toInt()==-1) {
                return QString ("- нет ");
            } else if (value.toInt()==-2) {
                return QString ("- др.СМО");
            } else if (value.toInt()==-3) {
                return QString ("- др.терр.");
            } else if (value.toInt()==-4) {
                return QString ("- закрыт ТФ");
            } else if (value.toInt()==-5) {
                return QString ("- отказ ФФ");
            } else if (value.toInt()==-6) {
                return QString ("- дубль");
            } else if (value.toInt()==-10) {
                return QString ("- умер");

            } else if (value.toInt()==11) {
                return QString ("!УДЛ! ВС");
            } else if (value.toInt()==12) {
                return QString ("!УДЛ! полис");
            } else if (value.toInt()==13) {
                return QString ("!УДЛ! перестр.");
            } else if (value.toInt()==14) {
                return QString ("!УДЛ! без пол.");

            } else if (value.toInt()==-666) {
                return QString ("- УДАЛЁН -");
            } else if (value.toInt()>=500) {
                return QString ("в архив (" + QString::number(value.toInt()) + ") ");
            } else if (value.toInt()<=-500) {
                return QString ("! АРХИВ (" + QString::number(value.toInt()) + ") ");

            } else {
                return QString (" ? (" + QString::number(value.toInt()) + ") ");
            }
        }
    }
    return value;
}


ki_terrSqlQueryModel::ki_terrSqlQueryModel(QObject *parent) : QSqlQueryModel(parent)
 { }

QVariant ki_terrSqlQueryModel::data(const QModelIndex &index, int role) const {
    QVariant value = QSqlQueryModel::data(index, role);

    switch (role) {
        case Qt::DisplayRole: // Данные для отображения
        if (index.column()==2) {
            if (value.toInt()==0) {
                return QString ("-???-");
            } else  return value;
        }
        if (index.column()==4) {
            if (value.toInt()==0) {
                return QString ("-все-");
            } else  return value;
        }
        if (index.column()==6) {
            if (value.toInt()==0) {
                return QString ("-все-");
            } else  return value;
        }
        if (index.column()==8) {
            if (value.toInt()==0) {
                return QString ("-все-");
            } else  return value;
        }
        if (index.column()==10) {
            if (value.toInt()==0) {
                return QString ("-все-");
            } else  return value;
        }


        if (index.column()==3) {
            if (value.toString().isEmpty()) {
                return QString ("- ??? -");
            } else if (value.toString()==" ") {
                return QString ("- ??? -");
            } else  return value;
        }
        if (index.column()==5) {
            if (value.toString().isEmpty()) {
                return QString ("- все районы -");
            } else if (value.toString()==" ") {
                return QString ("- все районы -");
            } else  return value;
        }
        if (index.column()==7) {
            if (value.toString().isEmpty()) {
                return QString ("- все города -");
            } else if (value.toString()==" ") {
                return QString ("- все города -");
            } else  return value;
        }
        if (index.column()==9) {
            if (value.toString().isEmpty()) {
                return QString ("- все нас.пункты -");
            } else if (value.toString()==" ") {
                return QString ("- все нас.пункты -");
            } else  return value;
        }
        if (index.column()==11) {
            if (value.toString().isEmpty()) {
                return QString ("- все улицы -");
            } else if (value.toString()==" ") {
                return QString ("- все улицы -");
            } else  return value;
        }


        if (index.column()==12) {
            if (value.toInt()>0) {
                return QString ("<");
            } else {
                return QString (" ");
            }
        }
        if (index.column()==13) {
            if (value.toInt()==0) {
                QVariant value1 = QSqlQueryModel::data(this->index(index.row(), 12, QModelIndex()), role);
                QVariant value2 = QSqlQueryModel::data(this->index(index.row(), 14, QModelIndex()), role);
                QVariant value3 = QSqlQueryModel::data(this->index(index.row(), 15, QModelIndex()), role);
                if ( value1.toInt()==0
                     && value2.toString()==""
                     && value3.toInt()==0 )
                return QString ("-все дома-");
            } else  return value;
        }
        if (index.column()==15) {
            if (value.toInt()>0) {
                return QString (">");
            } else {
                return QString (" ");
            }
        }
    }
    return value;
}
