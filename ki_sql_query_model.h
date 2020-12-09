#ifndef KI_SQL_QUERY_MODEL
#define KI_SQL_QUERY_MODEL

#include <QSqlQueryModel>

class ki_persSqlQueryModel : public QSqlQueryModel {
public:
    ki_persSqlQueryModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const;
};

class ki_terrSqlQueryModel : public QSqlQueryModel {
public:
    ki_terrSqlQueryModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const;
};

#endif // KI_SQL_QUERY_MODEL

