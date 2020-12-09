#ifndef import4csv_wnd_H
#define import4csv_wnd_H

#include <QDialog>
#include <QtSql/QSqlTableModel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>

#include "s_data_app.h"

typedef struct {
    QString file_name;
    QString tab_name;
} s_data_import;

namespace Ui {
    class import4csv_wnd;
}

class import4csv_wnd : public QDialog
{
    Q_OBJECT
    QSqlDatabase &db;
    s_data_import &data_import;
    s_data_app &data_app;

    QStringList lines_10;

    void refresh_combo_Table();
    void get_fileData();
    void refresh_tables();
    bool load_csvData(QString file_name);
    bool import_csvData(QString tab_name);

    QStringList* data_lists[50];
    int data_lists_cnt;

    QList<QLineEdit*> list_line_headers;
    QList<QTextEdit*> list_text_dataStr;
    QList<QComboBox*> list_comb_formats;
    QList<QComboBox*> list_comb_columns;
public:
    explicit import4csv_wnd(QSqlDatabase &db, s_data_app &data_app, s_data_import &data_import, QWidget *parent = 0);
    ~import4csv_wnd();

private slots:
    void on_bn_CSV_clicked();

    void on_combo_Codeset_activated(int index);

    void on_line_CSV_textChanged(const QString &arg1);

    void on_combo_Capts_activated(int index);

    void on_combo_Capts_currentIndexChanged(int index);

    void on_bn_close_clicked();

    void on_bn_load_data_clicked();

    void on_bn_Import_clicked();

private:
    Ui::import4csv_wnd *ui;
};

#endif // import4csv_wnd_H
