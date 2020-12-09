#include "finance_medorgs_wnd.h"
#include "ui_finance_medorgs_wnd.h"

#include "ki_funcs.h"

finance_medorgs_wnd::finance_medorgs_wnd(QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::finance_medorgs_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    finance_mo_coef_w = NULL;

    ui->pan_wait->setVisible(false);
    ui->split_finance->setStretchFactor(0,4);
    ui->split_finance->setStretchFactor(0,1);

    ui->date_cur->setDate(QDate(1800,1,1));
    ui->date_calc->setDate(QDate::currentDate());

    refresh_combo_sexage_tab();
    refresh_finance_tab();
    refresh_finance_ms_tab();
}

finance_medorgs_wnd::~finance_medorgs_wnd() {
    delete ui;
}
void finance_medorgs_wnd::refresh_combo_sexage_tab() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка половозрастных
    QString sql = "SELECT s.id, 'SEXAGE '||l.layer_date||' - '||(case s.assig_type when 1 then ' по терр.' when 2 then ' по заявл.' else ' -все-' end) as sexage_name, "
                  "       l.layer_date, l.layer_type, s.assig_type "
                  "  FROM assig.sexages s "
                  "  LEFT JOIN assig.layers l ON(l.id=s.id_layer) "
                  " ORDER BY l.layer_date DESC, s.assig_type ; ";
    QSqlQuery *query = new QSqlQuery(db);
    mySQL.exec(this, sql, "Список половозрастных таблиц", *query, true, db, data_app);
    sexage_dates.clear();
    ui->combo_sexage_tab->clear();
    while (query->next()) {
        ui->combo_sexage_tab->addItem(query->value(1).toString(), query->value(0).toInt());
        sexage_dates.append( query->value(2).toDate());
    }
    delete query;
    ui->combo_sexage_tab->setCurrentIndex(-1);
    this->setCursor(Qt::ArrowCursor);
}

void finance_medorgs_wnd::on_bn_refresh_finance_clicked() {
    refresh_finance_tab();
    refresh_finance_ms_tab();
}

void finance_medorgs_wnd::refresh_finance_tab() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    this->setEnabled(false);
    QApplication::processEvents();

    QString date_calc_s = ui->date_calc->date().toString("yyyy-MM-dd");

    sql_finance = "select t.id, t.code_mo, t.name_mo, 'X', "
                  "       t.cnt_m_0_0, t.cnt_w_0_0, t.cnt_m_1_4, t.cnt_w_1_4, t.cnt_m_5_17, t.cnt_w_5_17, t.cnt_m_18_59, t.cnt_w_18_54, t.cnt_m_60_, t.cnt_w_55_, t.cnt_sum, 'X', "
                  "       c.tarif, 'X', "
                  "       c.coef_m_0_0, c.coef_w_0_0, c.coef_m_1_4, c.coef_w_1_4, c.coef_m_5_17, c.coef_w_5_17, c.coef_m_18_59, c.coef_w_18_54, c.coef_m_60_, c.coef_w_55_, 'X', "
                  "       t.norm_m_0_0, t.norm_w_0_0, t.norm_m_1_4, t.norm_w_1_4, t.norm_m_5_17, t.norm_w_5_17, t.norm_m_18_59, t.norm_w_18_54, t.norm_m_60_, t.norm_w_55_, 'X', "
                  "       c.coef_dif, 'X', "
                  "       t.nKdif_m_0_0, t.nKdif_w_0_0, t.nKdif_m_1_4, t.nKdif_w_1_4, t.nKdif_m_5_17, t.nKdif_w_5_17, t.nKdif_m_18_59, t.nKdif_w_18_54, t.nKdif_m_60_, t.nKdif_w_55_, 'X', "
                  "       t.fin_m_0_0, t.fin_w_0_0, t.fin_m_1_4, t.fin_w_1_4, t.fin_m_5_17, t.fin_w_5_17, t.fin_m_18_59, t.fin_w_18_54, t.fin_m_60_, t.fin_w_55_, t.fin_sum "
                  "  from assig.calc_finance_medorgs_tab t "
                  "  left join assig.coef_finance_medorgs_tab c on(c.code_mo=t.code_mo and c.date_begin<='" + date_calc_s + "' and (c.date_end is NULL or c.date_end>'" + date_calc_s + "') ) "
                  " order by t.id ; ";

    model_finance.setQuery(sql_finance,db);
    QString err2 = model_finance.lastError().driverText();

    // подключаем модель из БД
    ui->tab_finance->setModel(&model_finance);

    // обновляем таблицу
    ui->tab_finance->reset();

    // задаём ширину колонок
    ui->tab_finance->setColumnWidth(  0,  2);     // id
    ui->tab_finance->setColumnWidth(  1, 50);     // code_mo
    ui->tab_finance->setColumnWidth(  2,200);     // name_mo
    ui->tab_finance->setColumnWidth(  3,  2);     // 'X'
    ui->tab_finance->setColumnWidth(  4, 35);     // cnt_m_0_0
    ui->tab_finance->setColumnWidth(  5, 35);     // cnt_w_0_0
    ui->tab_finance->setColumnWidth(  6, 35);     // cnt_m_1_4
    ui->tab_finance->setColumnWidth(  7, 35);     // cnt_w_1_4
    ui->tab_finance->setColumnWidth(  8, 35);     // cnt_m_5_17
    ui->tab_finance->setColumnWidth(  9, 35);     // cnt_w_5_17
    ui->tab_finance->setColumnWidth( 10, 35);     // cnt_m_18_59
    ui->tab_finance->setColumnWidth( 11, 35);     // cnt_w_18_54
    ui->tab_finance->setColumnWidth( 12, 35);     // cnt_m_60_
    ui->tab_finance->setColumnWidth( 13, 35);     // cnt_w_55_
    ui->tab_finance->setColumnWidth( 14, 40);     // cnt_sum
    ui->tab_finance->setColumnWidth( 15,  2);     // 'X'
    ui->tab_finance->setColumnWidth( 16, 40);     // tarif
    ui->tab_finance->setColumnWidth( 17,  2);     // 'X'
    ui->tab_finance->setColumnWidth( 18, 50);     // coef_m_0_0
    ui->tab_finance->setColumnWidth( 19, 50);     // coef_w_0_0
    ui->tab_finance->setColumnWidth( 20, 50);     // coef_m_1_4
    ui->tab_finance->setColumnWidth( 21, 50);     // coef_w_1_4
    ui->tab_finance->setColumnWidth( 22, 50);     // coef_m_5_17
    ui->tab_finance->setColumnWidth( 23, 50);     // coef_w_5_17
    ui->tab_finance->setColumnWidth( 24, 50);     // coef_m_18_59
    ui->tab_finance->setColumnWidth( 25, 50);     // coef_w_18_54
    ui->tab_finance->setColumnWidth( 26, 50);     // coef_m_60_
    ui->tab_finance->setColumnWidth( 27, 50);     // coef_w_55_
    ui->tab_finance->setColumnWidth( 28,  2);     // 'X'
    ui->tab_finance->setColumnWidth( 29, 50);     // norm_m_0_0
    ui->tab_finance->setColumnWidth( 30, 50);     // norm_w_0_0
    ui->tab_finance->setColumnWidth( 31, 50);     // norm_m_1_4
    ui->tab_finance->setColumnWidth( 32, 50);     // norm_w_1_4
    ui->tab_finance->setColumnWidth( 33, 50);     // norm_m_5_17
    ui->tab_finance->setColumnWidth( 34, 50);     // norm_w_5_17
    ui->tab_finance->setColumnWidth( 35, 50);     // norm_m_18_59
    ui->tab_finance->setColumnWidth( 36, 50);     // norm_w_18_54
    ui->tab_finance->setColumnWidth( 37, 50);     // norm_m_60_
    ui->tab_finance->setColumnWidth( 38, 50);     // norm_w_55_
    ui->tab_finance->setColumnWidth( 39,  2);     // 'X'
    ui->tab_finance->setColumnWidth( 40, 55);     // coef_dif
    ui->tab_finance->setColumnWidth( 41,  2);     // 'X'
    ui->tab_finance->setColumnWidth( 42, 50);     // nKdif_m_0_0
    ui->tab_finance->setColumnWidth( 43, 50);     // nKdif_w_0_0
    ui->tab_finance->setColumnWidth( 44, 50);     // nKdif_m_1_4
    ui->tab_finance->setColumnWidth( 45, 50);     // nKdif_w_1_4
    ui->tab_finance->setColumnWidth( 46, 50);     // nKdif_m_5_17
    ui->tab_finance->setColumnWidth( 47, 50);     // nKdif_w_5_17
    ui->tab_finance->setColumnWidth( 48, 50);     // nKdif_m_18_59
    ui->tab_finance->setColumnWidth( 49, 50);     // nKdif_w_18_54
    ui->tab_finance->setColumnWidth( 50, 50);     // nKdif_m_60_
    ui->tab_finance->setColumnWidth( 51, 50);     // nKdif_w_55_
    ui->tab_finance->setColumnWidth( 52,  2);     // 'X'
    ui->tab_finance->setColumnWidth( 53, 55);     // fin_m_0_0
    ui->tab_finance->setColumnWidth( 54, 55);     // fin_w_0_0
    ui->tab_finance->setColumnWidth( 55, 55);     // fin_m_1_4
    ui->tab_finance->setColumnWidth( 56, 55);     // fin_w_1_4
    ui->tab_finance->setColumnWidth( 57, 55);     // fin_m_5_17
    ui->tab_finance->setColumnWidth( 58, 55);     // fin_w_5_17
    ui->tab_finance->setColumnWidth( 59, 55);     // fin_m_18_59
    ui->tab_finance->setColumnWidth( 60, 55);     // fin_w_18_54
    ui->tab_finance->setColumnWidth( 61, 55);     // fin_m_60_
    ui->tab_finance->setColumnWidth( 62, 55);     // fin_w_55_
    ui->tab_finance->setColumnWidth( 63, 60);     // fin_sum

    // правим заголовки
    model_finance.setHeaderData(  0, Qt::Horizontal, ("ID"));
    model_finance.setHeaderData(  1, Qt::Horizontal, ("Код МО"));          // code_mo
    model_finance.setHeaderData(  2, Qt::Horizontal, ("Наименование МО")); // name_mo
    model_finance.setHeaderData(  3, Qt::Horizontal, ("Х"));     // 'X'
    model_finance.setHeaderData(  4, Qt::Horizontal, ("число\nМ\n0-1"));   // cnt_m_0_0
    model_finance.setHeaderData(  5, Qt::Horizontal, ("число\nЖ\n0-1"));   // cnt_w_0_0
    model_finance.setHeaderData(  6, Qt::Horizontal, ("число\nМ\n1-4"));   // cnt_m_1_4
    model_finance.setHeaderData(  7, Qt::Horizontal, ("число\nЖ\n1-4"));   // cnt_w_1_4
    model_finance.setHeaderData(  8, Qt::Horizontal, ("число\nМ\n5-17"));  // cnt_m_5_17
    model_finance.setHeaderData(  9, Qt::Horizontal, ("число\nЖ\n5-17"));  // cnt_w_5_17
    model_finance.setHeaderData( 10, Qt::Horizontal, ("число\nМ\n18-59")); // cnt_m_18_59
    model_finance.setHeaderData( 11, Qt::Horizontal, ("число\nЖ\n18-54")); // cnt_w_18_54
    model_finance.setHeaderData( 12, Qt::Horizontal, ("число\nМ\n60 <"));  // cnt_m_60_
    model_finance.setHeaderData( 13, Qt::Horizontal, ("число\nЖ\n55 <"));  // cnt_w_55_
    model_finance.setHeaderData( 14, Qt::Horizontal, ("число\nСумма\n"));  // cnt_sum
    model_finance.setHeaderData( 15, Qt::Horizontal, ("Х"));     // 'X'
    model_finance.setHeaderData( 16, Qt::Horizontal, ("Тариф"));      // tarif
    model_finance.setHeaderData( 17, Qt::Horizontal, ("Х"));     // 'X'
    model_finance.setHeaderData( 18, Qt::Horizontal, ("коэффиц.\nМ\n0-1"));   // coef_m_0_0
    model_finance.setHeaderData( 19, Qt::Horizontal, ("коэффиц.\nЖ\n0-1"));   // coef_w_0_0
    model_finance.setHeaderData( 20, Qt::Horizontal, ("коэффиц.\nМ\n1-4"));   // coef_m_1_4
    model_finance.setHeaderData( 21, Qt::Horizontal, ("коэффиц.\nЖ\n1-4"));   // coef_w_1_4
    model_finance.setHeaderData( 22, Qt::Horizontal, ("коэффиц.\nМ\n5-17"));  // coef_m_5_17
    model_finance.setHeaderData( 23, Qt::Horizontal, ("коэффиц.\nЖ\n5-17"));  // coef_w_5_17
    model_finance.setHeaderData( 24, Qt::Horizontal, ("коэффиц.\nМ\n18-59")); // coef_m_18_59
    model_finance.setHeaderData( 25, Qt::Horizontal, ("коэффиц.\nЖ\n18-54")); // coef_w_18_54
    model_finance.setHeaderData( 26, Qt::Horizontal, ("коэффиц.\nМ\n59 <"));  // coef_m_60_
    model_finance.setHeaderData( 27, Qt::Horizontal, ("коэффиц.\nЖ\n54 <"));  // coef_w_55_
    model_finance.setHeaderData( 28, Qt::Horizontal, ("Х"));    // 'X'
    model_finance.setHeaderData( 29, Qt::Horizontal, ("нрматив\nМ\n0-1"));   // norm_m_0_0
    model_finance.setHeaderData( 30, Qt::Horizontal, ("нрматив\nЖ\n0-1"));   // norm_w_0_0
    model_finance.setHeaderData( 31, Qt::Horizontal, ("нрматив\nМ\n1-4"));   // norm_m_1_4
    model_finance.setHeaderData( 32, Qt::Horizontal, ("нрматив\nЖ\n1-4"));   // norm_w_1_4
    model_finance.setHeaderData( 33, Qt::Horizontal, ("нрматив\nМ\n5-17"));  // norm_m_5_17
    model_finance.setHeaderData( 34, Qt::Horizontal, ("нрматив\nЖ\n5-17"));  // norm_w_5_17
    model_finance.setHeaderData( 35, Qt::Horizontal, ("нрматив\nМ\n18-59")); // norm_m_18_59
    model_finance.setHeaderData( 36, Qt::Horizontal, ("нрматив\nЖ\n18-54")); // norm_w_18_54
    model_finance.setHeaderData( 37, Qt::Horizontal, ("нрматив\nМ\n60 <"));  // norm_m_60_
    model_finance.setHeaderData( 38, Qt::Horizontal, ("нрматив\nЖ\n55 <"));  // norm_w_55_
    model_finance.setHeaderData( 39, Qt::Horizontal, ("Х"));    // 'X'
    model_finance.setHeaderData( 40, Qt::Horizontal, ("Коэффиц.\nдифферен-\nциации"));    // coef_dif
    model_finance.setHeaderData( 41, Qt::Horizontal, ("Х"));    // 'X'
    model_finance.setHeaderData( 42, Qt::Horizontal, ("норм*КЗ\nМ\n0-1"));   // nKdif_m_0_0
    model_finance.setHeaderData( 43, Qt::Horizontal, ("норм*КЗ\nЖ\n0-1"));   // nKdif_w_0_0
    model_finance.setHeaderData( 44, Qt::Horizontal, ("норм*КЗ\nМ\n1-4"));   // nKdif_m_1_4
    model_finance.setHeaderData( 45, Qt::Horizontal, ("норм*КЗ\nЖ\n1-4"));   // nKdif_w_1_4
    model_finance.setHeaderData( 46, Qt::Horizontal, ("норм*КЗ\nМ\n5-17"));  // nKdif_m_5_17
    model_finance.setHeaderData( 47, Qt::Horizontal, ("норм*КЗ\nЖ\n5-17"));  // nKdif_w_5_17
    model_finance.setHeaderData( 48, Qt::Horizontal, ("норм*КЗ\nМ\n18-59")); // nKdif_m_18_59
    model_finance.setHeaderData( 49, Qt::Horizontal, ("норм*КЗ\nЖ\n18-54")); // nKdif_w_18_54
    model_finance.setHeaderData( 50, Qt::Horizontal, ("норм*КЗ\nМ\n60 <"));  // nKdif_m_60_
    model_finance.setHeaderData( 51, Qt::Horizontal, ("норм*КЗ\nЖ\n55 <"));  // nKdif_w_55_
    model_finance.setHeaderData( 52, Qt::Horizontal, ("Х"));    // 'X'
    model_finance.setHeaderData( 53, Qt::Horizontal, ("финансир.\nза месяц\nМ 0-1"));   // fin_m_0_0
    model_finance.setHeaderData( 54, Qt::Horizontal, ("финансир.\nза месяц\nЖ 0-1"));   // fin_w_0_0
    model_finance.setHeaderData( 55, Qt::Horizontal, ("финансир.\nза месяц\nМ 1-4"));   // fin_m_1_4
    model_finance.setHeaderData( 56, Qt::Horizontal, ("финансир.\nза месяц\nЖ 1-4"));   // fin_w_1_4
    model_finance.setHeaderData( 57, Qt::Horizontal, ("финансир.\nза месяц\nМ 5-17"));  // fin_m_5_17
    model_finance.setHeaderData( 58, Qt::Horizontal, ("финансир.\nза месяц\nЖ 5-17"));  // fin_w_5_17
    model_finance.setHeaderData( 59, Qt::Horizontal, ("финансир.\nза месяц\nМ 18-59")); // fin_m_18_59
    model_finance.setHeaderData( 60, Qt::Horizontal, ("финансир.\nза месяц\nЖ 18-54")); // fin_w_18_54
    model_finance.setHeaderData( 61, Qt::Horizontal, ("финансир.\nза месяц\nМ 60 <"));  // fin_m_60_
    model_finance.setHeaderData( 62, Qt::Horizontal, ("финансир.\nза месяц\nЖ 55 <"));  // fin_w_55_
    model_finance.setHeaderData( 63, Qt::Horizontal, ("СУММА"));     // fin_sum
    ui->tab_finance->repaint();

    this->setEnabled(true);
    ui->lab_code_mo->setText("-//-");
    QApplication::processEvents();
}


void finance_medorgs_wnd::refresh_finance_ms_tab() {
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    this->setEnabled(false);
    QApplication::processEvents();

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_finance->selectionModel()->selection().indexes();

    QString code_mo = "";

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        code_mo = model_finance.data(model_finance.index(index.row(), 1), Qt::EditRole).toString();       // code_mo
    }

    sql_finance_ms = "select id, code_head, name_head, code_mo, name_mo, code_mp, name_mp, code_ms, name_ms, type_ms, 'X', "
                  "       cnt_m_0_0, cnt_w_0_0, cnt_m_1_4, cnt_w_1_4, cnt_m_5_17, cnt_w_5_17, cnt_m_18_59, cnt_w_18_54, cnt_m_60_, cnt_w_55_, cnt_sum, 'X', "
                  "       fin_m_0_0, fin_w_0_0, fin_m_1_4, fin_w_1_4, fin_m_5_17, fin_w_5_17, fin_m_18_59, fin_w_18_54, fin_m_60_, fin_w_55_, fin_sum "
                  "  from assig.calc_finance_medsites_tab ";
    if (!code_mo.isEmpty())
        sql_finance_ms += " where code_mo='" + code_mo + "'";
    sql_finance_ms += " order by code_head, code_mo, code_mp, code_ms ; ";

    model_finance_ms.setQuery(sql_finance_ms,db);
    QString err2 = model_finance_ms.lastError().driverText();

    // подключаем модель из БД
    ui->tab_finance_ms->setModel(&model_finance_ms);

    // обновляем таблицу
    ui->tab_finance_ms->reset();

    // задаём ширину колонок
    ui->tab_finance_ms->setColumnWidth(  0,  2);     // id
    ui->tab_finance_ms->setColumnWidth(  1, 50);     // code_head
    ui->tab_finance_ms->setColumnWidth(  2,  2);     // name_head
    ui->tab_finance_ms->setColumnWidth(  3, 50);     // code_mo
    ui->tab_finance_ms->setColumnWidth(  4,200);     // name_mo
    ui->tab_finance_ms->setColumnWidth(  5, 50);     // code_mp
    ui->tab_finance_ms->setColumnWidth(  6,  2);     // name_mp
    ui->tab_finance_ms->setColumnWidth(  7,100);     // code_ms
    ui->tab_finance_ms->setColumnWidth(  8,  2);     // name_ms
    ui->tab_finance_ms->setColumnWidth(  9, 50);     // type_ms
    ui->tab_finance_ms->setColumnWidth( 10,  2);     // 'X'
    ui->tab_finance_ms->setColumnWidth( 11, 35);     // cnt_m_0_0
    ui->tab_finance_ms->setColumnWidth( 12, 35);     // cnt_w_0_0
    ui->tab_finance_ms->setColumnWidth( 13, 35);     // cnt_m_1_4
    ui->tab_finance_ms->setColumnWidth( 14, 35);     // cnt_w_1_4
    ui->tab_finance_ms->setColumnWidth( 15, 35);     // cnt_m_5_17
    ui->tab_finance_ms->setColumnWidth( 16, 35);     // cnt_w_5_17
    ui->tab_finance_ms->setColumnWidth( 17, 35);     // cnt_m_18_59
    ui->tab_finance_ms->setColumnWidth( 18, 35);     // cnt_w_18_54
    ui->tab_finance_ms->setColumnWidth( 19, 35);     // cnt_m_60_
    ui->tab_finance_ms->setColumnWidth( 20, 35);     // cnt_w_55_
    ui->tab_finance_ms->setColumnWidth( 21, 40);     // cnt_sum
    ui->tab_finance_ms->setColumnWidth( 22,  2);     // 'X'
    ui->tab_finance_ms->setColumnWidth( 23, 55);     // fin_m_0_0
    ui->tab_finance_ms->setColumnWidth( 24, 55);     // fin_w_0_0
    ui->tab_finance_ms->setColumnWidth( 25, 55);     // fin_m_1_4
    ui->tab_finance_ms->setColumnWidth( 26, 55);     // fin_w_1_4
    ui->tab_finance_ms->setColumnWidth( 27, 55);     // fin_m_5_17
    ui->tab_finance_ms->setColumnWidth( 28, 55);     // fin_w_5_17
    ui->tab_finance_ms->setColumnWidth( 29, 55);     // fin_m_18_59
    ui->tab_finance_ms->setColumnWidth( 30, 55);     // fin_w_18_54
    ui->tab_finance_ms->setColumnWidth( 31, 55);     // fin_m_60_
    ui->tab_finance_ms->setColumnWidth( 32, 55);     // fin_w_55_
    ui->tab_finance_ms->setColumnWidth( 33, 60);     // fin_sum

    // правим заголовки
    model_finance_ms.setHeaderData(  0, Qt::Horizontal, ("ID"));
    model_finance_ms.setHeaderData(  1, Qt::Horizontal, ("Код\nгл.МО"));                 // code_head
    model_finance_ms.setHeaderData(  2, Qt::Horizontal, ("Головная МО"));                // name_head
    model_finance_ms.setHeaderData(  3, Qt::Horizontal, ("Код МО"));                     // code_mo
    model_finance_ms.setHeaderData(  4, Qt::Horizontal, ("Наименование МО"));            // name_mo
    model_finance_ms.setHeaderData(  5, Qt::Horizontal, ("Код\nподр."));                 // code_mp
    model_finance_ms.setHeaderData(  6, Qt::Horizontal, ("Наименование подразделения")); // name_mp
    model_finance_ms.setHeaderData(  7, Qt::Horizontal, ("Код\nучастка"));               // code_ms
    model_finance_ms.setHeaderData(  8, Qt::Horizontal, ("Наименование участка"));       // name_ms
    model_finance_ms.setHeaderData(  9, Qt::Horizontal, ("тип\nучастка"));                // type_ms
    model_finance_ms.setHeaderData( 10, Qt::Horizontal, ("Х"));     // 'X'
    model_finance_ms.setHeaderData( 11, Qt::Horizontal, ("число\nМ\n0-1"));   // cnt_m_0_0
    model_finance_ms.setHeaderData( 12, Qt::Horizontal, ("число\nЖ\n0-1"));   // cnt_w_0_0
    model_finance_ms.setHeaderData( 13, Qt::Horizontal, ("число\nМ\n1-4"));   // cnt_m_1_4
    model_finance_ms.setHeaderData( 14, Qt::Horizontal, ("число\nЖ\n1-4"));   // cnt_w_1_4
    model_finance_ms.setHeaderData( 15, Qt::Horizontal, ("число\nМ\n5-17"));  // cnt_m_5_17
    model_finance_ms.setHeaderData( 16, Qt::Horizontal, ("число\nЖ\n5-17"));  // cnt_w_5_17
    model_finance_ms.setHeaderData( 17, Qt::Horizontal, ("число\nМ\n18-59")); // cnt_m_18_59
    model_finance_ms.setHeaderData( 18, Qt::Horizontal, ("число\nЖ\n18-54")); // cnt_w_18_54
    model_finance_ms.setHeaderData( 19, Qt::Horizontal, ("число\nМ\n60 <"));  // cnt_m_60_
    model_finance_ms.setHeaderData( 20, Qt::Horizontal, ("число\nЖ\n55 <"));  // cnt_w_55_
    model_finance_ms.setHeaderData( 21, Qt::Horizontal, ("число\nСумма\n"));  // cnt_sum
    model_finance_ms.setHeaderData( 22, Qt::Horizontal, ("Х"));     // 'X'
    model_finance_ms.setHeaderData( 23, Qt::Horizontal, ("финансир.\nза месяц\nМ 0-1"));   // fin_m_0_0
    model_finance_ms.setHeaderData( 24, Qt::Horizontal, ("финансир.\nза месяц\nЖ 0-1"));   // fin_w_0_0
    model_finance_ms.setHeaderData( 25, Qt::Horizontal, ("финансир.\nза месяц\nМ 1-4"));   // fin_m_1_4
    model_finance_ms.setHeaderData( 26, Qt::Horizontal, ("финансир.\nза месяц\nЖ 1-4"));   // fin_w_1_4
    model_finance_ms.setHeaderData( 27, Qt::Horizontal, ("финансир.\nза месяц\nМ 5-17"));  // fin_m_5_17
    model_finance_ms.setHeaderData( 28, Qt::Horizontal, ("финансир.\nза месяц\nЖ 5-17"));  // fin_w_5_17
    model_finance_ms.setHeaderData( 29, Qt::Horizontal, ("финансир.\nза месяц\nМ 18-59")); // fin_m_18_59
    model_finance_ms.setHeaderData( 30, Qt::Horizontal, ("финансир.\nза месяц\nЖ 18-54")); // fin_w_18_54
    model_finance_ms.setHeaderData( 31, Qt::Horizontal, ("финансир.\nза месяц\nМ 60 <"));  // fin_m_60_
    model_finance_ms.setHeaderData( 32, Qt::Horizontal, ("финансир.\nза месяц\nЖ 55 <"));  // fin_w_55_
    model_finance_ms.setHeaderData( 33, Qt::Horizontal, ("СУММА"));     // fin_sum
    ui->tab_finance_ms->repaint();

    this->setEnabled(true);
    QApplication::processEvents();
}

void finance_medorgs_wnd::on_bn_close_clicked() {
    if (QMessageBox::question(this, "Закрыть окно?",
                              "Вы действительно хотите закрыть окно расчёта финансирования?",
                              QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::No)
        return;
    close();
}

void finance_medorgs_wnd::on_bn_load_sexage_clicked() {
    if (ui->combo_sexage_tab->currentIndex()<0) {
        QMessageBox::warning(this, "Выберите таблицу половозрастного распределения",
                             "Выберите таблицу половозрастного распределения, данные которой хотите загрузить для расчёта оплаты подушевых счетов для АПП");
        return;
    }

    db.transaction();
    this->setEnabled(false);
    QApplication::processEvents();

    int id_sexage = ui->combo_sexage_tab->currentData().toInt();

    // ---------------------------------------- //
    // копирование данных половозрастной для МО //
    // ---------------------------------------- //

    QString sql;
    // подтянем данные медорганизаций из справочника
    sql  = "update assig.calc_finance_medorgs_tab f "
           "   set name_mo=(select mo.name_mo from spr_medorgs mo where mo.code_mo=f.code_mo limit 1) ; "

    // подтянем данные половозрастной из выбранной таблицы половозрастного распределения
           "update assig.calc_finance_medorgs_tab f "
           "   set cnt_m_0_0  =(select tl.m_0_0   from assig.sexages_lines tl where tl.id_sexage=" + QString::number(id_sexage) + " and tl.code_head=f.code_mo), "
           "       cnt_w_0_0  =(select tl.w_0_0   from assig.sexages_lines tl where tl.id_sexage=" + QString::number(id_sexage) + " and tl.code_head=f.code_mo), "
           "       cnt_m_1_4  =(select tl.m_1_4   from assig.sexages_lines tl where tl.id_sexage=" + QString::number(id_sexage) + " and tl.code_head=f.code_mo), "
           "       cnt_w_1_4  =(select tl.w_1_4   from assig.sexages_lines tl where tl.id_sexage=" + QString::number(id_sexage) + " and tl.code_head=f.code_mo), "
           "       cnt_m_5_17 =(select tl.m_5_17  from assig.sexages_lines tl where tl.id_sexage=" + QString::number(id_sexage) + " and tl.code_head=f.code_mo), "
           "       cnt_w_5_17 =(select tl.w_5_17  from assig.sexages_lines tl where tl.id_sexage=" + QString::number(id_sexage) + " and tl.code_head=f.code_mo), "
           "       cnt_m_18_59=(select tl.m_18_59 from assig.sexages_lines tl where tl.id_sexage=" + QString::number(id_sexage) + " and tl.code_head=f.code_mo), "
           "       cnt_w_18_54=(select tl.w_18_54 from assig.sexages_lines tl where tl.id_sexage=" + QString::number(id_sexage) + " and tl.code_head=f.code_mo), "
           "       cnt_m_60_  =(select tl.m_60_   from assig.sexages_lines tl where tl.id_sexage=" + QString::number(id_sexage) + " and tl.code_head=f.code_mo), "
           "       cnt_w_55_  =(select tl.w_55_   from assig.sexages_lines tl where tl.id_sexage=" + QString::number(id_sexage) + " and tl.code_head=f.code_mo), "
           "       cnt_sum    =(select tl.sum     from assig.sexages_lines tl where tl.id_sexage=" + QString::number(id_sexage) + " and tl.code_head=f.code_mo), "
           "       fin_m_0_0   = NULL, "
           "       fin_w_0_0   = NULL, "
           "       fin_m_1_4   = NULL, "
           "       fin_w_1_4   = NULL, "
           "       fin_m_5_17  = NULL, "
           "       fin_w_5_17  = NULL, "
           "       fin_m_18_59 = NULL, "
           "       fin_w_18_54 = NULL, "
           "       fin_m_60_   = NULL, "
           "       fin_w_55_   = NULL, "
           "       fin_sum     = NULL ; ";

    QSqlQuery *query = new QSqlQuery(db);
    bool res = mySQL.exec(this, sql, "Подтянем данные половозрастной в таблицу для расчёта финансирования", *query, true, db, data_app);
    if (!res) {
        db.rollback();
        this->setEnabled(true);
        QApplication::processEvents();
        QMessageBox::warning(this, "Ошибка при чтении данных половозрастной", "При чтении данных половозрастной произошла неожиданная ошибка");
        return;
    }
    delete query;



    // ---------------------------------------------- //
    // копирование данных половозрастной для участков //
    // ---------------------------------------------- //

    QString sql_ms;
    // подтянем данные участков из справочников
    sql_ms ="delete from assig.calc_finance_medsites_tab f ;"
            // добавим все участки МО
            "insert into assig.calc_finance_medsites_tab (code_head, name_head, code_mo, name_mo, code_mp, name_mp, code_ms, name_ms, type_ms) "
            "select mh.code_mo, mh.name_mo, mo.code_mo, mo.name_mo, mp.code_mp, mp.name_mp, ms.code_ms, ms.name_ms, ms.type_ms "
            "  from assig.calc_finance_medorgs_tab t "
            "  left join spr_medorgs mo on(mo.code_mo=t.code_mo) "
            "  left join spr_medorgs mh on(mh.code_mo=mo.code_head and mh.is_head=1) "
            "  left join spr_medsites ms on(ms.code_mo=mo.code_mo) "
            "  left join spr_medparts mp on(mp.code_mo=ms.code_mo and mp.code_mp=ms.code_mp) "
            " order by mh.code_mo, mo.code_mo, mp.code_mp, ms.code_ms ; "
            // добавим строку "без участка"
            "insert into assig.calc_finance_medsites_tab (code_head, name_head, code_mo, name_mo, code_mp, name_mp, code_ms, name_ms, type_ms) "
            "select distinct mh.code_mo, mh.name_mo, mo.code_mo, mo.name_mo, cast(NULL as text), cast(NULL as text), ' - без участка - ', cast(NULL as text), cast(NULL as smallint) "
            "  from assig.calc_finance_medorgs_tab t "
            "  left join spr_medorgs mo on(mo.code_mo=t.code_mo) "
            "  left join spr_medorgs mh on(mh.code_mo=mo.code_head and mh.is_head=1) "
            "  left join spr_medsites ms on(ms.code_mo=mo.code_mo) "
            "  left join spr_medparts mp on(mp.code_mo=ms.code_mo and mp.code_mp=ms.code_mp) "
            " order by mh.code_mo, mo.code_mo ; "
            // добавим строку ИТОГО
            "insert into assig.calc_finance_medsites_tab (code_head, name_head, code_mo, name_mo, code_mp, name_mp, code_ms, name_ms, type_ms) "
            "select distinct mh.code_mo, mh.name_mo, mo.code_mo, mo.name_mo, cast(NULL as text), cast(NULL as text), ' Итого', cast(NULL as text), cast(NULL as smallint) "
            "  from assig.calc_finance_medorgs_tab t "
            "  left join spr_medorgs mo on(mo.code_mo=t.code_mo) "
            "  left join spr_medorgs mh on(mh.code_mo=mo.code_head and mh.is_head=1) "
            "  left join spr_medsites ms on(ms.code_mo=mo.code_mo) "
            "  left join spr_medparts mp on(mp.code_mo=ms.code_mo and mp.code_mp=ms.code_mp) "
            " order by mh.code_mo, mo.code_mo ; "

            // подтянем данные половозрастной из выбранной таблицы половозрастного распределения
            "update assig.calc_finance_medsites_tab f "
            "   set cnt_m_0_0  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo /*and d.code_mp=f.code_mp*/ and d.code_ms=f.code_ms and d.cell_name='m_0_0') q ),  "
            "       cnt_w_0_0  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo /*and d.code_mp=f.code_mp*/ and d.code_ms=f.code_ms and d.cell_name='w_0_0') q ),  "
            "       cnt_m_1_4  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo /*and d.code_mp=f.code_mp*/ and d.code_ms=f.code_ms and d.cell_name='m_1_4') q ),  "
            "       cnt_w_1_4  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo /*and d.code_mp=f.code_mp*/ and d.code_ms=f.code_ms and d.cell_name='w_1_4') q ),  "
            "       cnt_m_5_17 =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo /*and d.code_mp=f.code_mp*/ and d.code_ms=f.code_ms and d.cell_name='m_5_17') q ), "
            "       cnt_w_5_17 =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo /*and d.code_mp=f.code_mp*/ and d.code_ms=f.code_ms and d.cell_name='w_5_17') q ), "
            "       cnt_m_18_59=(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo /*and d.code_mp=f.code_mp*/ and d.code_ms=f.code_ms and d.cell_name='m_18_59') q ),"
            "       cnt_w_18_54=(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo /*and d.code_mp=f.code_mp*/ and d.code_ms=f.code_ms and d.cell_name='w_18_54') q ),"
            "       cnt_m_60_  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo /*and d.code_mp=f.code_mp*/ and d.code_ms=f.code_ms and d.cell_name='m_60_') q ),  "
            "       cnt_w_55_  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo /*and d.code_mp=f.code_mp*/ and d.code_ms=f.code_ms and d.cell_name='w_55_') q ),  "
            "       cnt_sum    =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo /*and d.code_mp=f.code_mp*/ and d.code_ms=f.code_ms) q ) ; "
            // сформируем строку "без участка"
            "update assig.calc_finance_medsites_tab f "
            "   set cnt_m_0_0  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='m_0_0'   and d.code_ms is NULL) q ), "
            "       cnt_w_0_0  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='w_0_0'   and d.code_ms is NULL) q ), "
            "       cnt_m_1_4  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='m_1_4'   and d.code_ms is NULL) q ), "
            "       cnt_w_1_4  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='w_1_4'   and d.code_ms is NULL) q ), "
            "       cnt_m_5_17 =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='m_5_17'  and d.code_ms is NULL) q ), "
            "       cnt_w_5_17 =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='w_5_17'  and d.code_ms is NULL) q ), "
            "       cnt_m_18_59=(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='m_18_59' and d.code_ms is NULL) q ), "
            "       cnt_w_18_54=(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='w_18_54' and d.code_ms is NULL) q ), "
            "       cnt_m_60_  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='m_60_'   and d.code_ms is NULL) q ), "
            "       cnt_w_55_  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='w_55_'   and d.code_ms is NULL) q ), "
            "       cnt_sum    =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo ) q ) "
            " where code_ms is NULL ; "
            // сформируем строку ИТОГО
            "update assig.calc_finance_medsites_tab f "
            "   set cnt_m_0_0  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='m_0_0'  ) q ),  "
            "       cnt_w_0_0  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='w_0_0'  ) q ),  "
            "       cnt_m_1_4  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='m_1_4'  ) q ),  "
            "       cnt_w_1_4  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='w_1_4'  ) q ),  "
            "       cnt_m_5_17 =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='m_5_17' ) q ), "
            "       cnt_w_5_17 =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='w_5_17' ) q ), "
            "       cnt_m_18_59=(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='m_18_59') q ),"
            "       cnt_w_18_54=(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='w_18_54') q ),"
            "       cnt_m_60_  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='m_60_'  ) q ),  "
            "       cnt_w_55_  =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo  and d.cell_name='w_55_'  ) q ),  "
            "       cnt_sum    =(select q.cnt  from (select count(*) as cnt from assig.sexages_lines sl left join assig.sexages_data d on(d.id_sexage_line=sl.id) where sl.id_sexage=" + QString::number(id_sexage) + " and d.code_mo=f.code_mo ) q ) "
            " where code_ms=' Итого' ; ";

    QSqlQuery *query_ms = new QSqlQuery(db);
    bool res_ms = mySQL.exec(this, sql_ms, "Подтянем данные половозрастной в таблицу для расчёта финансирования по участкам", *query_ms, true, db, data_app);
    if (!res_ms) {
        db.rollback();
        this->setEnabled(true);
        QApplication::processEvents();
        QMessageBox::warning(this, "Ошибка при чтении данных половозрастной по участкам", "При чтении данных половозрастной по участкам произошла неожиданная ошибка");
        return;
    }
    delete query_ms;

    ui->date_cur->setDate(sexage_dates.at(ui->combo_sexage_tab->currentIndex()));

    db.commit();
    this->setEnabled(true);
    QApplication::processEvents();

    refresh_finance_tab();
    refresh_finance_ms_tab();
}

void finance_medorgs_wnd::on_tab_finance_clicked(const QModelIndex &index) {
    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_finance->selectionModel()->selection().indexes();

    ui->bn_edit_coef->setEnabled(false);

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        int id = model_finance.data(model_finance.index(index.row(), 0), Qt::EditRole).toInt();       // e.id
        QString code_mo = model_finance.data(model_finance.index(index.row(), 1), Qt::EditRole).toString();     // code_mo
        QString name_mo = model_finance.data(model_finance.index(index.row(), 2), Qt::EditRole).toString();     // name_mo

        ui->lab_code_mo->setText(code_mo);

        if (code_mo!=" Итого")
            ui->bn_edit_coef->setEnabled(true);

    } else {
        ui->lab_code_mo->setText("-//-");
    }

    refresh_finance_ms_tab();
}

void finance_medorgs_wnd::on_bn_edit_coef_clicked() {
    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_finance->selectionModel()->selection().indexes();
    this->setCursor(Qt::WaitCursor);

    if (indexes.size()>0) {
        QModelIndex index = indexes.front();

        int id = model_finance.data(model_finance.index(index.row(), 0), Qt::EditRole).toInt();       // e.id
        QString code_mo = model_finance.data(model_finance.index(index.row(),1), Qt::EditRole).toString();    // code_mo
        QString name_mo = model_finance.data(model_finance.index(index.row(),2), Qt::EditRole).toString();    // name_mo

        QDate date = QDate::currentDate();

        double tarif   = model_finance.data(model_finance.index(index.row(), 16), Qt::EditRole).toDouble();   // tarif

        double m_0_0   = model_finance.data(model_finance.index(index.row(), 18), Qt::EditRole).toDouble();   // tarif
        double w_0_0   = model_finance.data(model_finance.index(index.row(), 19), Qt::EditRole).toDouble();   // tarif
        double m_1_4   = model_finance.data(model_finance.index(index.row(), 20), Qt::EditRole).toDouble();   // tarif
        double w_1_4   = model_finance.data(model_finance.index(index.row(), 21), Qt::EditRole).toDouble();   // tarif
        double m_5_17  = model_finance.data(model_finance.index(index.row(), 22), Qt::EditRole).toDouble();   // tarif
        double w_5_17  = model_finance.data(model_finance.index(index.row(), 23), Qt::EditRole).toDouble();   // tarif
        double m_18_59 = model_finance.data(model_finance.index(index.row(), 24), Qt::EditRole).toDouble();   // tarif
        double w_18_54 = model_finance.data(model_finance.index(index.row(), 25), Qt::EditRole).toDouble();   // tarif
        double m_60_   = model_finance.data(model_finance.index(index.row(), 26), Qt::EditRole).toDouble();   // tarif
        double w_55_   = model_finance.data(model_finance.index(index.row(), 27), Qt::EditRole).toDouble();   // tarif
        double coef_dif= model_finance.data(model_finance.index(index.row(), 40), Qt::EditRole).toDouble();   // tarif

        delete finance_mo_coef_w;
        finance_mo_coef_w = new finance_mo_coef_wnd(code_mo, db, data_app, settings/*, name_mo, date, tarif, m_0_0, w_0_0, m_1_4, w_1_4, m_5_17, w_5_17, m_18_59, w_18_54, m_60_, w_55_, coef_dif*/, this);

        /*if (*/finance_mo_coef_w->exec(); /*) {
            // обновим данные в таблице

            db.transaction();
            this->setCursor(Qt::WaitCursor);

            QString sql;
            sql  = "update assig.calc_finance_medorgs_tab f "
                   "   set tarif =" + QString::number(tarif) + ", "
                   "       coef_m_0_0 ="   + QString::number(m_0_0)   + ", "
                   "       coef_w_0_0 ="   + QString::number(w_0_0)   + ", "
                   "       coef_m_1_4 ="   + QString::number(m_1_4)   + ", "
                   "       coef_w_1_4 ="   + QString::number(w_1_4)   + ", "
                   "       coef_m_5_17 ="  + QString::number(m_5_17)  + ", "
                   "       coef_w_5_17 ="  + QString::number(w_5_17)  + ", "
                   "       coef_m_18_59 =" + QString::number(m_18_59) + ", "
                   "       coef_w_18_54 =" + QString::number(w_18_54) + ", "
                   "       coef_m_60_ ="   + QString::number(m_60_)   + ", "
                   "       coef_w_55_ ="   + QString::number(w_55_)   + ", "
                   "       coef_coef_dif ="+ QString::number(coef_dif)    + "  "
                   " where f.id=" + QString::number(id) + " ; ";

            QSqlQuery *query = new QSqlQuery(db);
            bool res = mySQL.exec(this, sql, "Запишем новые коэффициенты в таблицу для расчёта финансирования", *query, true, db, data_app);
            if (!res) {
                db.rollback();
                this->setCursor(Qt::ArrowCursor);
                QMessageBox::warning(this, "Ошибка при сохранении новых коэффициентов в таблицу для расчёта финансирования",
                                     "При сохранении ноых коэффициентов в таблицу для расчёта финансирования произошла неожиданная ошибка");
                return;
            }
            delete query;

            db.commit();
            this->setCursor(Qt::ArrowCursor);

            refresh_finance_tab();
            refresh_finance_ms_tab();
        } else {
            QMessageBox::information(this, "Действие отменено пользователем",
                                     "Дейсвие отменено пользователем.\n"
                                     "Ничего не изменилось.");
        }*/
    } else {
        QMessageBox::information(this, "Ничего не выбрано",
                                 "Выберите строку медорганизации, коэффициенты и тариф которой хотите изменить.");
    }
    this->setCursor(Qt::ArrowCursor);
}

void finance_medorgs_wnd::on_bn_recalc_tab_clicked() {
    // пересчитаем таблицу
    db.transaction();

    QString sql;
    QSqlQuery *query = new QSqlQuery(db);
    bool res;

    QString date_calc_s = ui->date_calc->date().toString("yyyy-MM-dd");

    // --------------------------------------------------
    // Подушевой норматив по каждой половозрастной группе
    sql  = "update assig.calc_finance_medorgs_tab f "
           "   set norm_m_0_0   = tarif * (select c.coef_m_0_0   from assig.coef_finance_medorgs_tab c where c.code_mo=f.code_mo and c.date_begin<='" + date_calc_s + "' and (c.date_end is NULL or c.date_end>'" + date_calc_s + "')  limit 1), "
           "       norm_w_0_0   = tarif * (select c.coef_w_0_0   from assig.coef_finance_medorgs_tab c where c.code_mo=f.code_mo and c.date_begin<='" + date_calc_s + "' and (c.date_end is NULL or c.date_end>'" + date_calc_s + "')  limit 1), "
           "       norm_m_1_4   = tarif * (select c.coef_m_1_4   from assig.coef_finance_medorgs_tab c where c.code_mo=f.code_mo and c.date_begin<='" + date_calc_s + "' and (c.date_end is NULL or c.date_end>'" + date_calc_s + "')  limit 1), "
           "       norm_w_1_4   = tarif * (select c.coef_w_1_4   from assig.coef_finance_medorgs_tab c where c.code_mo=f.code_mo and c.date_begin<='" + date_calc_s + "' and (c.date_end is NULL or c.date_end>'" + date_calc_s + "')  limit 1), "
           "       norm_m_5_17  = tarif * (select c.coef_m_5_17  from assig.coef_finance_medorgs_tab c where c.code_mo=f.code_mo and c.date_begin<='" + date_calc_s + "' and (c.date_end is NULL or c.date_end>'" + date_calc_s + "')  limit 1), "
           "       norm_w_5_17  = tarif * (select c.coef_w_5_17  from assig.coef_finance_medorgs_tab c where c.code_mo=f.code_mo and c.date_begin<='" + date_calc_s + "' and (c.date_end is NULL or c.date_end>'" + date_calc_s + "')  limit 1), "
           "       norm_m_18_59 = tarif * (select c.coef_m_18_59 from assig.coef_finance_medorgs_tab c where c.code_mo=f.code_mo and c.date_begin<='" + date_calc_s + "' and (c.date_end is NULL or c.date_end>'" + date_calc_s + "')  limit 1), "
           "       norm_w_18_54 = tarif * (select c.coef_w_18_54 from assig.coef_finance_medorgs_tab c where c.code_mo=f.code_mo and c.date_begin<='" + date_calc_s + "' and (c.date_end is NULL or c.date_end>'" + date_calc_s + "')  limit 1), "
           "       norm_m_60_   = tarif * (select c.coef_m_60_   from assig.coef_finance_medorgs_tab c where c.code_mo=f.code_mo and c.date_begin<='" + date_calc_s + "' and (c.date_end is NULL or c.date_end>'" + date_calc_s + "')  limit 1), "
           "       norm_w_55_   = tarif * (select c.coef_w_55_   from assig.coef_finance_medorgs_tab c where c.code_mo=f.code_mo and c.date_begin<='" + date_calc_s + "' and (c.date_end is NULL or c.date_end>'" + date_calc_s + "')  limit 1) "
           " ; ";

    res = mySQL.exec(this, sql, "Расчитаем подушевой норматив", *query, true, db, data_app);
    if (!res) {
        db.rollback();
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при расчёте подушевого норматива",
                             "При расчёте подушевого норматива произошла неожиданная ошибка");
        return;
    }

    // --------------------------------------------------------------
    // Подушевой норматив финансирования с учетом коэффициента затрат
    sql  = "update assig.calc_finance_medorgs_tab f "
           "   set nkdif_m_0_0   = coef_dif * norm_m_0_0, "
           "       nkdif_w_0_0   = coef_dif * norm_w_0_0, "
           "       nkdif_m_1_4   = coef_dif * norm_m_1_4, "
           "       nkdif_w_1_4   = coef_dif * norm_w_1_4, "
           "       nkdif_m_5_17  = coef_dif * norm_m_5_17, "
           "       nkdif_w_5_17  = coef_dif * norm_w_5_17, "
           "       nkdif_m_18_59 = coef_dif * norm_m_18_59, "
           "       nkdif_w_18_54 = coef_dif * norm_w_18_54, "
           "       nkdif_m_60_   = coef_dif * norm_m_60_, "
           "       nkdif_w_55_   = coef_dif * norm_w_55_ "
           " ; ";

    res = mySQL.exec(this, sql, "Расчитаем норматив финансирования с учетом коэффициента затрат", *query, true, db, data_app);
    if (!res) {
        db.rollback();
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при расчёте подушевого финансирования с учетом коэффициента затрат",
                             "При расчёте финансирования с учетом коэффициента затрат произошла неожиданная ошибка");
        return;
    }

    // -----------------------------
    // Сумма финансирования за месяц
    sql  = "update assig.calc_finance_medorgs_tab f "
           "   set fin_m_0_0   = cnt_m_0_0   * nkdif_m_0_0 , "
           "       fin_w_0_0   = cnt_w_0_0   * nkdif_w_0_0 , "
           "       fin_m_1_4   = cnt_m_1_4   * nkdif_m_1_4 , "
           "       fin_w_1_4   = cnt_w_1_4   * nkdif_w_1_4 , "
           "       fin_m_5_17  = cnt_m_5_17  * nkdif_m_5_17 , "
           "       fin_w_5_17  = cnt_w_5_17  * nkdif_w_5_17 , "
           "       fin_m_18_59 = cnt_m_18_59 * nkdif_m_18_59 , "
           "       fin_w_18_54 = cnt_w_18_54 * nkdif_w_18_54 , "
           "       fin_m_60_   = cnt_m_60_   * nkdif_m_60_ , "
           "       fin_w_55_   = cnt_w_55_   * nkdif_w_55_  "
           " ; ";

    res = mySQL.exec(this, sql, "Расчитаем сумму финансирования за месяц", *query, true, db, data_app);
    if (!res) {
        db.rollback();
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при расчёте суммы финансирования за месяц",
                             "При расчёте суммы финансирования за месяц произошла неожиданная ошибка");
        return;
    }

    // -----
    // ВСЕГО
    sql  = "update assig.calc_finance_medorgs_tab f "
           "   set fin_sum  =  fin_m_0_0 + fin_w_0_0 + fin_m_1_4 + fin_w_1_4 + fin_m_5_17 + fin_w_5_17 + fin_m_18_59 + fin_w_18_54 + fin_m_60_ + fin_w_55_ "
           " ; ";

    res = mySQL.exec(this, sql, "Расчитаем общую сумму", *query, true, db, data_app);
    if (!res) {
        db.rollback();
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при расчёте общей суммы",
                             "При расчёте общей суммы произошла неожиданная ошибка");
        return;
    }

    // ------------------------
    // общее суммирование ИТОГО
    sql  = "update assig.calc_finance_medorgs_tab f "
           "   set cnt_m_0_0   = (select sum(ff.cnt_m_0_0)   from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       cnt_w_0_0   = (select sum(ff.cnt_w_0_0)   from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       cnt_m_1_4   = (select sum(ff.cnt_m_1_4)   from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       cnt_w_1_4   = (select sum(ff.cnt_w_1_4)   from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       cnt_m_5_17  = (select sum(ff.cnt_m_5_17)  from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       cnt_w_5_17  = (select sum(ff.cnt_w_5_17)  from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       cnt_m_18_59 = (select sum(ff.cnt_m_18_59) from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       cnt_w_18_54 = (select sum(ff.cnt_w_18_54) from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       cnt_m_60_   = (select sum(ff.cnt_m_60_)   from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       cnt_w_55_   = (select sum(ff.cnt_w_55_)   from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       cnt_sum     = (select sum(ff.cnt_sum)     from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       "
           "       tarif  = NULL, "
           "       "/*
           "       coef_m_0_0   = NULL, "
           "       coef_w_0_0   = NULL, "
           "       coef_m_1_4   = NULL, "
           "       coef_w_1_4   = NULL, "
           "       coef_m_5_17  = NULL, "
           "       coef_w_5_17  = NULL, "
           "       coef_m_18_59 = NULL, "
           "       coef_w_18_54 = NULL, "
           "       coef_m_60_   = NULL, "
           "       coef_w_55_   = NULL, "
           "       "*/
           "       norm_m_0_0   = NULL, "
           "       norm_w_0_0   = NULL, "
           "       norm_m_1_4   = NULL, "
           "       norm_w_1_4   = NULL, "
           "       norm_m_5_17  = NULL, "
           "       norm_w_5_17  = NULL, "
           "       norm_m_18_59 = NULL, "
           "       norm_w_18_54 = NULL, "
           "       norm_m_60_   = NULL, "
           "       norm_w_55_   = NULL, "
           "       "
           "       coef_dif  = NULL, "
           "       "
           "       nkdif_m_0_0   = NULL, "
           "       nkdif_w_0_0   = NULL, "
           "       nkdif_m_1_4   = NULL, "
           "       nkdif_w_1_4   = NULL, "
           "       nkdif_m_5_17  = NULL, "
           "       nkdif_w_5_17  = NULL, "
           "       nkdif_m_18_59 = NULL, "
           "       nkdif_w_18_54 = NULL, "
           "       nkdif_m_60_   = NULL, "
           "       nkdif_w_55_   = NULL, "
           "       "
           "       fin_m_0_0   = (select sum(ff.fin_m_0_0)   from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       fin_w_0_0   = (select sum(ff.fin_w_0_0)   from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       fin_m_1_4   = (select sum(ff.fin_m_1_4)   from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       fin_w_1_4   = (select sum(ff.fin_w_1_4)   from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       fin_m_5_17  = (select sum(ff.fin_m_5_17)  from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       fin_w_5_17  = (select sum(ff.fin_w_5_17)  from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       fin_m_18_59 = (select sum(ff.fin_m_18_59) from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       fin_w_18_54 = (select sum(ff.fin_w_18_54) from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       fin_m_60_   = (select sum(ff.fin_m_60_)   from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       fin_w_55_   = (select sum(ff.fin_w_55_)   from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого'), "
           "       fin_sum     = (select sum(ff.fin_sum)     from  assig.calc_finance_medorgs_tab ff where ff.code_mo<>' Итого')  "
           "       "
           " where f.code_mo=' Итого'; ";

    res = mySQL.exec(this, sql, "Расчитаем сумму ИТОГО", *query, true, db, data_app);
    if (!res) {
        db.rollback();
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при расчёте суммы ИТОГО",
                             "При расчёте суммы ИТОГО произошла неожиданная ошибка");
        return;
    }



    // ------------------- //
    // РАСЧЁТ ДЛЯ УЧАСТКОВ //
    // ------------------- //

    // -----------------------------
    // Сумма финансирования за месяц
    sql  = "update assig.calc_finance_medsites_tab f "
           "   set fin_m_0_0   = cnt_m_0_0   * (select ff.nkdif_m_0_0   from assig.calc_finance_medorgs_tab ff  where ff.code_mo=f.code_mo  limit 1), "
           "       fin_w_0_0   = cnt_w_0_0   * (select ff.nkdif_w_0_0   from assig.calc_finance_medorgs_tab ff  where ff.code_mo=f.code_mo  limit 1), "
           "       fin_m_1_4   = cnt_m_1_4   * (select ff.nkdif_m_1_4   from assig.calc_finance_medorgs_tab ff  where ff.code_mo=f.code_mo  limit 1), "
           "       fin_w_1_4   = cnt_w_1_4   * (select ff.nkdif_w_1_4   from assig.calc_finance_medorgs_tab ff  where ff.code_mo=f.code_mo  limit 1), "
           "       fin_m_5_17  = cnt_m_5_17  * (select ff.nkdif_m_5_17  from assig.calc_finance_medorgs_tab ff  where ff.code_mo=f.code_mo  limit 1), "
           "       fin_w_5_17  = cnt_w_5_17  * (select ff.nkdif_w_5_17  from assig.calc_finance_medorgs_tab ff  where ff.code_mo=f.code_mo  limit 1), "
           "       fin_m_18_59 = cnt_m_18_59 * (select ff.nkdif_m_18_59 from assig.calc_finance_medorgs_tab ff  where ff.code_mo=f.code_mo  limit 1), "
           "       fin_w_18_54 = cnt_w_18_54 * (select ff.nkdif_w_18_54 from assig.calc_finance_medorgs_tab ff  where ff.code_mo=f.code_mo  limit 1), "
           "       fin_m_60_   = cnt_m_60_   * (select ff.nkdif_m_60_   from assig.calc_finance_medorgs_tab ff  where ff.code_mo=f.code_mo  limit 1), "
           "       fin_w_55_   = cnt_w_55_   * (select ff.nkdif_w_55_   from assig.calc_finance_medorgs_tab ff  where ff.code_mo=f.code_mo  limit 1) "
           " ; ";

    res = mySQL.exec(this, sql, "Расчитаем сумму финансирования по участкам за месяц", *query, true, db, data_app);
    if (!res) {
        db.rollback();
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при расчёте суммы финансирования по участкам за месяц",
                             "При расчёте суммы финансирования по участкам за месяц произошла неожиданная ошибка");
        return;
    }

    // -----
    // ВСЕГО
    sql  = "update assig.calc_finance_medsites_tab f "
           "   set fin_sum  =  fin_m_0_0 + fin_w_0_0 + fin_m_1_4 + fin_w_1_4 + fin_m_5_17 + fin_w_5_17 + fin_m_18_59 + fin_w_18_54 + fin_m_60_ + fin_w_55_ "
           " ; ";

    res = mySQL.exec(this, sql, "Расчитаем общую сумму по участкам", *query, true, db, data_app);
    if (!res) {
        db.rollback();
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Ошибка при расчёте общей суммы по участкам",
                             "При расчёте общей суммы по участкам произошла неожиданная ошибка");
        return;
    }




    delete query;

    db.commit();
    this->setCursor(Qt::ArrowCursor);

    refresh_finance_tab();
    refresh_finance_ms_tab();
}

void finance_medorgs_wnd::on_bn_toCSV_clicked() {
    bool res_CSV = table2csv(db, sql_finance, model_finance, ui->tab_finance, true,
                             data_app.path_out + QDate::currentDate().toString("yyyy-MM-dd") + "  -  " + data_app.smo_short + " - " + data_app.filial_name + "  -  " +
                             "Оплата подушевых счетов по АПП ",
                             this);
}

void finance_medorgs_wnd::on_bn_toCSV_ms_clicked() {
    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_finance->selectionModel()->selection().indexes();
    QString code_mo = "";
    if (indexes.size()>0) {
        QModelIndex index = indexes.front();
        code_mo = model_finance.data(model_finance.index(index.row(), 1), Qt::EditRole).toString();       // code_mo
    }

    bool res_CSV = table2csv(db, sql_finance_ms, model_finance_ms, ui->tab_finance_ms, true,
                             data_app.path_out + QDate::currentDate().toString("yyyy-MM-dd") + "  -  " + data_app.smo_short + " - " + data_app.filial_name + "  -  " +
                             "Оплата подушевых счетов по АПП  (в разрезе врачебных участков" + (code_mo.isEmpty() ? ")" : (", для медорганизации " + code_mo) + ")"),
                             this);
}


#include "qt_windows.h"
#include "qwindowdefs_win.h"
#include <shellapi.h>


// теперь сформируем отчёт по финансированию в разрезе МО
void finance_medorgs_wnd::on_bn_gen_finrep4mo_clicked() {
    if (ui->date_cur->date()<QDate(2014,1,1)) {
        QMessageBox::warning(this,
                             "Дата фыормирования данных не введена",
                             "Задайте дату формирования данных вручную и повторите попытку.");
        return;
    }
    QDate cur_date = ui->date_cur->date();

    ui->pan_wait->setVisible(true);
    ui->pan_finance_DN->setEnabled(false);
    QApplication::processEvents();

    // распаковка шаблона
    QString rep_folder = data_app.path_reports;
    QString tmp_folder = data_app.path_out + "_REPORTS_/temp/";
    QString rep_template = rep_folder + "fin_medorgs.odt";

    if (!QFile::exists(rep_template)) {
        QMessageBox::warning(this,
                             "Шаблон не найден",
                             "Не найден шаблон печатной формы акта сверки прикрепления застрахованных: \n" + rep_template +
                             "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        ui->pan_finance_DN->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return;
    }
    QDir tmp_dir(tmp_folder);
    if (tmp_dir.exists(tmp_folder) && !tmp_dir.removeRecursively()) {
        QMessageBox::warning(this,
                             "Ошибка при очистке папки",
                             "Не удалось очистить папку временных файлов: \n" + tmp_folder +
                             "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        ui->pan_finance_DN->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return;
    }
    tmp_dir.mkpath(tmp_folder);
    QProcess myProcess;
    QString com = "\"" + data_app.path_arch + "7z.exe\" e \"" + rep_template + "\" -o\"" + tmp_folder + "\" content.xml";
    if (myProcess.execute(com)!=0) {
        QMessageBox::warning(this,
                             "Ошибка распаковки шаблона",
                             "Не удалось распаковать файл контента шаблона\n" + rep_template + " -> content.xml"
                             "\nпечатной формы во временную папку\n" + tmp_folder +
                             "\n\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки." + "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        ui->pan_finance_DN->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return;
    }
    com = "\"" + data_app.path_arch + "7z.exe\" e \"" + rep_template + "\" -o\"" + tmp_folder + "\" styles.xml";
    if (myProcess.execute(com)!=0) {
        QMessageBox::warning(this,
                             "Ошибка распаковки шаблона",
                             "Не удалось распаковать файл стилей шаблона \n" + rep_template + " -> styles.xml"
                             "\nпечатной формы во временную папку\n" + tmp_folder +
                             "\n\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки." + "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        ui->pan_finance_DN->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return;
    }

    // ===========================
    // правка контента шаблона
    // ===========================
    // открытие контента шаблона
    QFile f_content(tmp_folder + "/content.xml");
    f_content.open(QIODevice::ReadOnly);
    QString s_content = f_content.readAll();
    f_content.close();
    if (s_content.isEmpty()) {
        QMessageBox::warning(this,
                             "Файл контента шаблона пустой",
                             "Файл контента шаблона пустой. Возможно шаблон был испорчен.\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        ui->pan_finance_DN->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return;
    }
    // выбор шаблона
    int pos_start = 0;

    int pos_mo_start0 = s_content.indexOf("<office:body>", pos_start) + 13;
    int pos_mo_start1 = s_content.indexOf("</table:table-header-rows>", pos_mo_start0) + 26;

    int pos_mp_start0 = s_content.indexOf("<table:table-row>", pos_mo_start1);
    int pos_mp_start1 = s_content.indexOf("</table:table-row>", pos_mp_start0) + 18;

    int pos_type_ms_start0 = s_content.indexOf("<table:table-row>", pos_mp_start1);
    int pos_type_ms_start1 = s_content.indexOf("</table:table-row>", pos_type_ms_start0) + 18;

    int pos_ms0 = s_content.indexOf("<table:table-row>", pos_type_ms_start1);
    int pos_ms1 = s_content.indexOf("</table:table-row>", pos_ms0) + 18;

    int pos_type_ms_end0 = s_content.indexOf("<table:table-row>", pos_ms1);
    int pos_type_ms_end1 = s_content.indexOf("</table:table-row>", pos_type_ms_end0) + 18;

    int pos_mp_end0= s_content.indexOf("<table:table-row>", pos_type_ms_end1);
    int pos_mp_end1 = s_content.indexOf("</table:table-row>", pos_mp_end0) + 18;

    int pos_mo_end0 = s_content.indexOf("<table:table-row>", pos_mp_end1);
    int pos_mo_end1 = s_content.indexOf("</office:text>", pos_mo_end0);

    int pos_end = s_content.size();

    QString s_start         = s_content.mid ( pos_start          , pos_mo_start0      - pos_start          );
    QString s_mo_start      = s_content.mid ( pos_mo_start0      , pos_mo_start1      - pos_mo_start0      );
    QString s_mp_start      = s_content.mid ( pos_mp_start0      , pos_mp_start1      - pos_mp_start0      );
    QString s_type_ms_start = s_content.mid ( pos_type_ms_start0 , pos_type_ms_start1 - pos_type_ms_start0 );
    QString s_ms            = s_content.mid ( pos_ms0            , pos_ms1            - pos_ms0            );
    QString s_type_ms_end   = s_content.mid ( pos_type_ms_end0   , pos_type_ms_end1   - pos_type_ms_end0   );
    QString s_mp_end        = s_content.mid ( pos_mp_end0        , pos_mp_end1        - pos_mp_end0        );
    QString s_mo_end        = s_content.mid ( pos_mo_end0        , pos_mo_end1        - pos_mo_end0        );
    QString s_end           = s_content.mid ( pos_mo_end1        , pos_end            - pos_mo_end1        );


    // --------------------------------------------------------- //
    // выберем список медорганзаций, к которым есть прикрепдение //
    // --------------------------------------------------------- //
    QString sql_mos = "select distinct code_mo "
                      "  from assig.calc_finance_medsites_tab "
                      " where cnt_sum>0 "
                      " order by code_mo ; ";
    QSqlQuery *query_mos = new QSqlQuery(db);
    QString sql = sql_mos.replace("\n","");
    bool res_mos = query_mos->exec(sql);
    if (!res_mos) {
        delete query_mos;
        QMessageBox::warning(this, "Ошибка выборки списка медорганизаций",
                             "При попытке выбрать список медорганизаций произошла ошибка.\n"
                             + sql_mos + "\n\n"
                             "Операция отменена.\n");
        this->setCursor(Qt::ArrowCursor);
        ui->pan_finance_DN->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return;
    }
    while (query_mos->next()) {
        QString code_mo = query_mos->value(0).toString();






        // ----------------------- //
        // сформируем начало файла //
        // ----------------------- //
        QFile f_content_save(tmp_folder + "/content.xml");
        QString s_content_new = s_start;
        QString s_new_line;

        // выборка данных
        QString sql_data = "select id, code_mo, name_mo, type_ms, code_mp, name_mp, code_ms, name_ms, "
                           "       cnt_sum, fin_sum "
                           "  from assig.calc_finance_medsites_tab "
                           " where code_ms<>' Итого'"
                           "   and code_mo='" + code_mo + "' "
                           " order by code_mo, type_ms, code_mp, code_ms ; ";
        QSqlQuery *query_data = new QSqlQuery(db);

        QString code_mo_old = "", name_mo_old = "", code_mp_old = "", name_mp_old = "", code_ms_old = "", type_ms_text_old = "";
        int     type_ms_old = -1;
        int     count_type_ms, count_mp, count_mo;
        double  fin_type_ms, fin_mp, fin_mo;

        count_type_ms = 0;  count_mp = 0;  count_mo = 0;
        fin_type_ms = 0;    fin_mp = 0;    fin_mo = 0;

        int id;
        QString name_mo, code_mp, name_mp, code_ms, name_ms;
        int type_ms = -1;
        QString type_ms_text;

        int n_medorg = 1;

        if (mySQL.exec(this, sql_data, "Получим данные для отчёта", query_data, true, db, data_app)) {
            while (query_data->next()) {
                id        = query_data->value( 0).toInt();
                name_mo   = query_data->value( 2).toString();
                type_ms   = query_data->value( 3).toInt();
                code_mp   = query_data->value( 4).toString();
                name_mp   = query_data->value( 5).toString();
                code_ms   = query_data->value( 6).toString();
                name_ms   = query_data->value( 7).toString();

                switch (type_ms) {
                case 1:
                    type_ms_text = "педиатрические участки";
                    break;
                case 2:
                    type_ms_text = "терапевтические участки";
                    break;
                case 3:
                    type_ms_text = "ВОП (взрослые, дети)";
                    break;
                case 4:
                    type_ms_text = "ВОП (только дети)";
                    break;
                case 5:
                    type_ms_text = "ВОП (только взрослые)";
                    break;
                default:
                    type_ms_text = " ??? ";
                    break;
                }

                int    count_ms = query_data->value( 8).toInt();
                double fin_ms   = query_data->value( 9).toDouble();

                // ------------------------------------------------------
                // если сменился тип участков в пределах подрзделения ...
                if ( !code_mo_old.isEmpty()  //&&  code_head_old==code_head
                    && type_ms_old>0  &&  type_ms_old!=type_ms ) {

                    // закроем предыдущий тип участков
                    QString s_new_line = s_type_ms_end;
                    s_new_line = s_new_line
                            .replace("#TYPE_MS#",     type_ms_text_old)
                            .replace("#TYPE_MS_CNT#", QString::number(count_type_ms))
                            .replace("#TYPE_MS_PAY#", show_rubles(fin_type_ms));
                    s_content_new += s_new_line;

                    // сбросим сумматоры по типу участков
                    count_type_ms = 0;  fin_type_ms = 0;
                }

                // ----------------------------------------------
                // если сменилось подразделение в пределах МО ...
                if ( !code_mo_old.isEmpty()  //&&  code_head_old==code_head
                    && code_mp_old>0  &&  code_mp_old!=code_mp ) {

                    // закроем предыдущее подразделение
                    QString s_new_line = s_mp_end;
                    s_new_line = s_new_line
                            .replace("#CODE_MP#", code_mp_old)
                            .replace("#NAME_MP#", name_mp_old)
                            .replace("#MP_CNT#",  QString::number(count_mp))
                            .replace("#MP_PAY#",  show_rubles(fin_mp));
                    s_content_new += s_new_line;

                    // сбросим сумматоры по МО
                    count_mp = 0;  fin_mp = 0;
                    count_type_ms = 0;  fin_type_ms = 0;
                }

                // ---------------------------------
                // если сменилась медорганизация ...
                if ( !code_mo_old.isEmpty()  &&  code_mo_old!=code_mo ) {

                    // закроем предыдущую
                    QString s_new_line = s_mo_end;
                    s_new_line = s_new_line
                            .replace("#ALL_CNT#", QString::number(count_mo))
                            .replace("#ALL_PAY#", show_rubles(fin_mo));
                    s_content_new += s_new_line;

                    // сбросим сумматоры
                    count_mo = 0;  fin_mo = 0;
                    count_mp = 0;  fin_mp = 0;
                    count_type_ms = 0;  fin_type_ms = 0;

                    n_medorg++;
                }


                // --------------------------------------
                // если МО изменилось и новая МО есть ...
                if ( code_mo_old!=code_mo
                    && !code_mo.isEmpty() ) {
                    // ... добавим нововую МО
                    s_new_line = s_mo_start;
                    s_new_line = s_new_line
                            .replace("#CODE_MO#", code_mo)
                            .replace("#NAME_MO#", name_mo)
                            .replace("#DATE_REP#", cur_date.toString("dd.MM.yyyy"));
                    s_content_new += s_new_line;
                }

                // ------------------------------------------------------------
                // если подразделение изменилось и новое подразделение есть ...
                if ( (code_mo_old!=code_mo || code_mp_old!=code_mp)
                    && !code_mp.isEmpty()) {
                    // ... добавим новое подразделение
                    s_new_line = s_mp_start;
                    s_new_line = s_new_line
                            .replace("#CODE_MP#", code_mp)
                            .replace("#NAME_MP#", name_mp);
                    s_content_new += s_new_line;
                }

                // ---------------------------------------------------------
                // если тип участков изменился и новый тип участков есть ...
                if ( (code_mo_old!=code_mo || code_mp_old!=code_mp || type_ms_old!=type_ms)
                    && type_ms>0) {
                    // ... добавим новый тип участков
                    s_new_line = s_type_ms_start;
                    s_new_line = s_new_line
                            .replace("#TYPE_MS#", type_ms_text);
                    s_content_new += s_new_line;
                }


                // ----------------------------------------------
                // если участок измнился и новый участок есть ...
                if (!code_ms.isEmpty()) {

                    // ... добавим новый участок
                    s_new_line = s_ms;
                    s_new_line = s_new_line
                            .replace("#CODE_MS#", code_ms)
                            .replace("#NAME_MS#", name_ms)
                            .replace("#MS_CNT#", QString::number(count_ms))
                            .replace("#MS_PAY#", show_rubles(fin_ms));
                    s_content_new += s_new_line;

                    // суумматоры
                    count_type_ms += count_ms;  fin_type_ms += fin_ms;
                    count_mp      += count_ms;  fin_mp      += fin_ms;
                    count_mo      += count_ms;  fin_mo      += fin_ms;
                }

                // --------------------------------- //
                // запомним последние значения кодов //
                type_ms_old  = type_ms;
                code_mp_old  = code_mp;
                name_mp_old  = name_mp;
                code_mo_old  = code_mo;
                name_mo_old  = name_mo;
                type_ms_text_old = type_ms_text;
            }

            // ------------------------------ //
            // закроем последний тип участков //
            QString s_new_line = s_type_ms_end;
            s_new_line = s_new_line
                    .replace("#TYPE_MS#",     type_ms_text)
                    .replace("#TYPE_MS_CNT#", QString::number(count_type_ms))
                    .replace("#TYPE_MS_PAY#", show_rubles(fin_type_ms));
            s_content_new += s_new_line;

            // ------------------------------- //
            // закроем последнее подразделение //
            s_new_line = s_mp_end;
            s_new_line = s_new_line
                    .replace("#CODE_MP#", code_mp)
                    .replace("#NAME_MP#", name_mp)
                    .replace("#MP_CNT#",  QString::number(count_mp))
                    .replace("#MP_PAY#",  show_rubles(fin_mp));
            s_content_new += s_new_line;

            // -------------------------------- //
            // закроем последнюю медорганизацию //
            s_new_line = s_mo_end;
            s_new_line = s_new_line
                    .replace("#ALL_CNT#", QString::number(count_mo))
                    .replace("#ALL_PAY#", show_rubles(fin_mo));
            s_content_new += s_new_line;

            // --------------------- //
            // закроем файл контента //
            s_content_new += s_end;


            delete query_data;

            // сохранение контента шаблона
            f_content_save.open(QIODevice::WriteOnly);
            f_content_save.write(s_content_new.toUtf8());
            f_content_save.close();


            // ===========================
            // архивация шаблона
            // ===========================
            QString fname_res = data_app.path_out + "_FIN_REPS_/" + QDate::currentDate().toString("yyyy-MM-dd") + "  -  " + data_app.smo_short + " - " + data_app.filial_name + "  -  " + code_mo + " - Отчёт по финансированию МО в разрезе участков.odt";

            QFile::remove(fname_res);
            while (QFile::exists(fname_res)) {
                if (QMessageBox::warning(this,
                                         "Ошибка сохранения акта",
                                         "Не удалось сохранить сформированный отчёт: \n" + rep_template +
                                         "\n\nво временную папку\n" + fname_res +
                                         "\n\nПроверьте, не открыт ли целевой файл в сторонней программе и повторите операцию.",
                                         QMessageBox::Retry|QMessageBox::Abort,
                                         QMessageBox::Retry)==QMessageBox::Abort) {
                    this->setCursor(Qt::ArrowCursor);
                    ui->pan_finance_DN->setEnabled(true);
                    ui->pan_wait->setVisible(false);
                    return;
                }
                QFile::remove(fname_res);
            }
            if (!QFile::copy(rep_template, fname_res) || !QFile::exists(fname_res)) {
                QMessageBox::warning(this, "Ошибка копирования шаблона",
                                     "Не удалось скопировать сформированный отчёт: \n" + rep_template +
                                     "\n\nво временную папку\n" + fname_res +
                                     "\n\nОперация прервана.");
                this->setCursor(Qt::ArrowCursor);
                ui->pan_finance_DN->setEnabled(true);
                ui->pan_wait->setVisible(false);
                return;
            }
            com = "\"" + data_app.path_arch + "7z.exe\" a \"" + fname_res + "\" \"" + tmp_folder + "/content.xml\"";
            if (myProcess.execute(com)!=0) {
                QMessageBox::warning(this,
                                     "Ошибка обновления контента",
                                     "При добавлении нового контента в шаблон печатной формы заявления произошла непредвиденная ошибка\n\n"
                                     "Опреация прервана.");
            }
        }
    }

    // ===========================
    // собственно открытие шаблона
    // ===========================
    // открытие полученного ODT
    QString s = data_app.path_out + "_FIN_REPS_/";
    long result = (long long)ShellExecute(0, 0, reinterpret_cast<const WCHAR*>(s.utf16()), 0, 0, SW_NORMAL);
    this->setCursor(Qt::ArrowCursor);
    if (result<32) {
        QMessageBox::warning(this,
                             "Ошибка при открытии папки с отчётами",
                             "Папка со сформированными отчётами не найдена:\n\n" + s +
                             "\nКод ошибки: " + QString::number(result) +
                             "\n\nПопробуйте открыть эту папку вручную.\n\n"
                             "Если папка не создана или ошибка будет повторяться - обратитесь к разработчику.");
    }

    this->setCursor(Qt::ArrowCursor);
    ui->pan_finance_DN->setEnabled(true);
    ui->pan_wait->setVisible(false);
}


void finance_medorgs_wnd::on_bn_gen_finrep4ms_clicked() {
    if (ui->date_cur->date()<QDate(2014,1,1)) {
        QMessageBox::warning(this,
                             "Дата фыормирования данных не введена",
                             "Задайте дату формирования данных вручную и повторите попытку.");
        return;
    }

    ui->pan_wait->setVisible(true);
    ui->pan_finance_DN->setEnabled(false);
    QApplication::processEvents();

    // распаковка шаблона
    QString rep_folder = data_app.path_reports;
    QString tmp_folder = data_app.path_out + "_REPORTS_/temp/";
    QString rep_template = rep_folder + "fin_medsites.odt";

    if (!QFile::exists(rep_template)) {
        QMessageBox::warning(this,
                             "Шаблон не найден",
                             "Не найден шаблон печатной формы акта сверки прикрепления застрахованных: \n" + rep_template +
                             "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        ui->pan_finance_DN->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return;
    }
    QDir tmp_dir(tmp_folder);
    if (tmp_dir.exists(tmp_folder) && !tmp_dir.removeRecursively()) {
        QMessageBox::warning(this,
                             "Ошибка при очистке папки",
                             "Не удалось очистить папку временных файлов: \n" + tmp_folder +
                             "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        ui->pan_finance_DN->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return;
    }
    tmp_dir.mkpath(tmp_folder);
    QProcess myProcess;
    QString com = "\"" + data_app.path_arch + "7z.exe\" e \"" + rep_template + "\" -o\"" + tmp_folder + "\" content.xml";
    if (myProcess.execute(com)!=0) {
        QMessageBox::warning(this,
                             "Ошибка распаковки шаблона",
                             "Не удалось распаковать файл контента шаблона\n" + rep_template + " -> content.xml"
                             "\nпечатной формы во временную папку\n" + tmp_folder +
                             "\n\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки." + "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        ui->pan_finance_DN->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return;
    }
    com = "\"" + data_app.path_arch + "7z.exe\" e \"" + rep_template + "\" -o\"" + tmp_folder + "\" styles.xml";
    if (myProcess.execute(com)!=0) {
        QMessageBox::warning(this,
                             "Ошибка распаковки шаблона",
                             "Не удалось распаковать файл стилей шаблона \n" + rep_template + " -> styles.xml"
                             "\nпечатной формы во временную папку\n" + tmp_folder +
                             "\n\nПроверьте наличие и доступность программного интерфейса 7Z и доступность временной папки." + "\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        ui->pan_finance_DN->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return;
    }

    // ===========================
    // правка контента шаблона
    // ===========================
    // открытие контента шаблона
    QFile f_content(tmp_folder + "/content.xml");
    f_content.open(QIODevice::ReadOnly);
    QString s_content = f_content.readAll();
    f_content.close();
    if (s_content.isEmpty()) {
        QMessageBox::warning(this,
                             "Файл контента шаблона пустой",
                             "Файл контента шаблона пустой. Возможно шаблон был испорчен.\n\nОперация прервана.");
        this->setCursor(Qt::ArrowCursor);
        ui->pan_finance_DN->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return;
    }
    // выбор шаблона
    int pos_start = 0;

    int pos_head_start0 = s_content.indexOf("<office:text>", pos_start) + 13;
    int pos_head_start1 = s_content.indexOf("</table:table-header-rows>", pos_head_start0) + 26;

    int pos_mo_start0 = s_content.indexOf("<table:table-row table:style-name=\"Таблица2.2\">", pos_head_start1);
    int pos_mo_start1 = s_content.indexOf("</table:table-row>", pos_mo_start0) + 18;

    int pos_ms_start0 = s_content.indexOf("<table:table-row table:style-name=\"Таблица2.2\">", pos_mo_start1);
    int pos_ms_start1 = s_content.indexOf("</table:table-row>", pos_ms_start0) + 18;

    int pos_age_start0 = s_content.indexOf("<table:table-row table:style-name=\"Таблица2.2\">", pos_ms_start1);
    int pos_age_start1 = s_content.indexOf("</table:table-row>", pos_age_start0) + 18;

    int pos_sex0 = s_content.indexOf("<table:table-row table:style-name=\"Таблица2.2\">", pos_age_start1);
    int pos_sex1 = s_content.indexOf("</table:table-row>", pos_sex0) + 18;

    int pos_age_end0 = s_content.indexOf("<table:table-row table:style-name=\"Таблица2.2\">", pos_sex1);
    int pos_age_end1 = s_content.indexOf("</table:table-row>", pos_age_end0) + 18;

    int pos_ms_end0 = s_content.indexOf("<table:table-row table:style-name=\"Таблица2.2\">", pos_age_end1);
    int pos_ms_end1 = s_content.indexOf("</table:table-row>", pos_ms_end0) + 18;

    int pos_mo_end0 = s_content.indexOf("<table:table-row table:style-name=\"Таблица2.2\">", pos_ms_end1);
    int pos_mo_end1 = s_content.indexOf("</table:table-row>", pos_mo_end0) + 18;

    int pos_head_end0 = s_content.indexOf("<table:table-row table:style-name=\"Таблица2.2\">", pos_mo_end1);
    int pos_head_end1 = s_content.indexOf("</office:text>", pos_head_end0);

    int pos_end = s_content.size();

    QString s_start       = s_content.mid ( pos_start        , pos_head_start0  - pos_start        );
    QString s_head_start  = s_content.mid ( pos_head_start0  , pos_head_start1  - pos_head_start0  );
    QString s_mo_start    = s_content.mid ( pos_mo_start0    , pos_mo_start1    - pos_mo_start0    );
    QString s_ms_start    = s_content.mid ( pos_ms_start0    , pos_ms_start1    - pos_ms_start0    );
    QString s_age_start   = s_content.mid ( pos_age_start0   , pos_age_start1   - pos_age_start0   );
    QString s_sex         = s_content.mid ( pos_sex0         , pos_sex1         - pos_sex0         );
    QString s_age_end     = s_content.mid ( pos_age_end0     , pos_age_end1     - pos_age_end0     );
    QString s_ms_end      = s_content.mid ( pos_ms_end0      , pos_ms_end1      - pos_ms_end0      );
    QString s_mo_end      = s_content.mid ( pos_mo_end0      , pos_mo_end1      - pos_mo_end0      );
    QString s_head_end    = s_content.mid ( pos_head_end0    , pos_head_end1    - pos_head_end0    );
    QString s_end         = s_content.mid ( pos_head_end1    , pos_end          - pos_head_end1    );


    // ------------------------------------------------------------------ //
    // выберем список головных медорганзаций, к которым есть прикрепдение //
    // ------------------------------------------------------------------ //
    QString sql_heads = "select distinct code_head "
                        "  from assig.calc_finance_medsites_tab "
                        " where cnt_sum>0 "
                        " order by code_head ; ";
    QSqlQuery *query_heads = new QSqlQuery(db);
    QString sql = sql_heads.replace("\n","");
    bool res_heads = query_heads->exec(sql);
    if (!res_heads) {
        delete query_heads;
        QMessageBox::warning(this, "Ошибка выборки списка медорганизаций",
                             "При попытке выбрать список медорганизаций произошла ошибка.\n"
                             + sql_heads + "\n\n"
                             "Операция отменена.\n");
        this->setCursor(Qt::ArrowCursor);
        ui->pan_finance_DN->setEnabled(true);
        ui->pan_wait->setVisible(false);
        return;
    }
    while (query_heads->next()) {
        QString code_head = query_heads->value(0).toString();






        // ----------------------- //
        // сформируем начало файла //
        // ----------------------- //
        QFile f_content_save(tmp_folder + "/content.xml");
        QString s_content_new = s_start;
        QString s_new_line;

        // выборка данных
        QString sql_data = "select id, code_head, name_head, code_mo, name_mo, code_mp, name_mp, code_ms, name_ms, type_ms, "
                           "       cnt_m_0_0, cnt_w_0_0, cnt_m_1_4, cnt_w_1_4, cnt_m_5_17, cnt_w_5_17, cnt_m_18_59, cnt_w_18_54, cnt_m_60_, cnt_w_55_, cnt_sum, "
                           "       fin_m_0_0, fin_w_0_0, fin_m_1_4, fin_w_1_4, fin_m_5_17, fin_w_5_17, fin_m_18_59, fin_w_18_54, fin_m_60_, fin_w_55_, fin_sum "
                           "  from assig.calc_finance_medsites_tab "
                           " where code_ms<>' Итого'"
                           "   and code_head='" + code_head + "' "
                           " order by code_head, code_mo, code_mp, code_ms "
                           " /*limit 100*/ ; ";
        QSqlQuery *query_data = new QSqlQuery(db);

        QString code_head_old = "", code_mo_old = "", code_mp_old = "", code_ms_old = "";
        int     count_age, count_ms, count_mo, count_head;
        double  pay_amb_age, pay_amb_ms, pay_amb_mo, pay_amb_head;
        double  pay_stim_age, pay_stim_ms, pay_stim_mo, pay_stim_head;
        double  pay_fap_age, pay_fap_ms, pay_fap_mo, pay_fap_head;
        double  pay_maint_age, pay_maint_ms, pay_maint_mo, pay_maint_head;
        double  pay_all_age, pay_all_ms, pay_all_mo, pay_all_head;

        count_age = 0;  pay_amb_age = 0;  pay_stim_age = 0;  pay_fap_age = 0;  pay_maint_age = 0;  pay_all_age = 0;
        count_ms = 0;   pay_amb_ms = 0;   pay_stim_ms = 0;   pay_fap_ms = 0;   pay_maint_ms = 0;   pay_all_ms = 0;
        count_mo = 0;   pay_amb_mo = 0;   pay_stim_mo = 0;   pay_fap_mo = 0;   pay_maint_mo = 0;   pay_all_mo = 0;
        count_head = 0; pay_amb_head = 0; pay_stim_head = 0; pay_fap_head = 0; pay_maint_head = 0; pay_all_head = 0;

        int id;
        QString name_head, code_mo, name_mo, code_mp, name_mp, code_ms, name_ms;
        int type_ms;

        int n_medhead = 1;

        if (mySQL.exec(this, sql_data, "Получим данные для отчёта", query_data, true, db, data_app)) {
            while (query_data->next()) {
                id        = query_data->value( 0).toInt();
                //code_head = query_data->value( 1).toString();
                name_head = query_data->value( 2).toString();
                code_mo   = query_data->value( 3).toString();
                name_mo   = query_data->value( 4).toString();
                code_mp   = query_data->value( 5).toString();
                name_mp   = query_data->value( 6).toString();
                code_ms   = query_data->value( 7).toString();
                name_ms   = query_data->value( 8).toString();
                type_ms   = query_data->value( 9).toInt();

                int cnt_m_0_0  = query_data->value(10).toInt();
                int cnt_w_0_0  = query_data->value(11).toInt();
                int cnt_m_1_4  = query_data->value(12).toInt();
                int cnt_w_1_4  = query_data->value(13).toInt();
                int cnt_m_5_17 = query_data->value(14).toInt();
                int cnt_w_5_17 = query_data->value(15).toInt();
                int cnt_m_18_59= query_data->value(16).toInt();
                int cnt_w_18_54= query_data->value(17).toInt();
                int cnt_m_60_  = query_data->value(18).toInt();
                int cnt_w_55_  = query_data->value(19).toInt();
                int cnt_sum    = query_data->value(20).toInt();

                double fin_m_0_0  = query_data->value(21).toDouble();
                double fin_w_0_0  = query_data->value(22).toDouble();
                double fin_m_1_4  = query_data->value(23).toDouble();
                double fin_w_1_4  = query_data->value(24).toDouble();
                double fin_m_5_17 = query_data->value(25).toDouble();
                double fin_w_5_17 = query_data->value(26).toDouble();
                double fin_m_18_59= query_data->value(27).toDouble();
                double fin_w_18_54= query_data->value(28).toDouble();
                double fin_m_60_  = query_data->value(29).toDouble();
                double fin_w_55_  = query_data->value(30).toDouble();
                double fin_sum    = query_data->value(31).toDouble();

                // пренесём застрахованных пограничных возрастов в правильные диапазоны
                if ( (type_ms==1 || type_ms==4)
                  && (cnt_m_18_59>0 || cnt_w_18_54>0 || fin_m_18_59>0 || fin_w_18_54>0) ) {
                    cnt_m_5_17 += cnt_m_18_59;
                    cnt_m_18_59 = 0;
                    cnt_w_5_17 += cnt_w_18_54;
                    cnt_w_18_54 = 0;
                    fin_m_5_17 += fin_m_18_59;
                    fin_m_18_59 = 0;
                    fin_w_5_17 += fin_w_18_54;
                    fin_w_18_54 = 0;
                }
                if ( (type_ms==2 || type_ms==5)
                  && (cnt_m_5_17>0 || cnt_w_5_17>0 || fin_m_5_17>0 || fin_w_5_17>0) ) {
                    cnt_m_18_59 += cnt_m_5_17;
                    cnt_m_5_17 = 0;
                    cnt_w_18_54 += cnt_w_5_17;
                    cnt_w_5_17 = 0;
                    fin_m_18_59 += fin_m_5_17;
                    fin_m_5_17 = 0;
                    fin_w_18_54 += fin_w_5_17;
                    fin_w_5_17 = 0;
                }

                // ------------------------------------------------
                // если сменился участок в пределах МО и юрлица ...
                if ( !code_head_old.isEmpty()  //&&  code_head_old==code_head
                    && !code_mo_old.isEmpty()  //&&  code_mo_old==code_mo
                    && !code_ms_old.isEmpty()  &&  (code_ms_old!=code_ms || code_mp_old!=code_mp) ) {

                    // закроем предыдущий участок
                    QString s_new_line = s_ms_end;
                    s_new_line = s_new_line
                            .replace("#CODE_MP#",   code_mp_old)
                            .replace("#CODE_MS#",   code_ms_old)
                            .replace("#COUNT3#",    QString::number(count_ms))
                            .replace("#PAY_AMB3#",  show_rubles(pay_amb_ms))
                            .replace("#PAY_STIM3#", show_rubles(pay_stim_ms))
                            .replace("#PAY_FAP3#",  show_rubles(pay_fap_ms))
                            .replace("#PAY_MAINT3#",show_rubles(pay_maint_ms))
                            .replace("#PAY_ALL3#",  show_rubles(pay_all_ms));
                    s_content_new += s_new_line;

                    // сбросим сумматоры по участку
                    count_ms = 0;  pay_amb_ms = 0;  pay_stim_ms = 0;  pay_fap_ms = 0;  pay_maint_ms = 0;  pay_all_ms = 0;
                }

                // ---------------------------------------
                // если сменилась МО в пределах юрлица ...
                if ( !code_head_old.isEmpty()  //&&  code_head_old==code_head
                    && !code_mo_old.isEmpty()  &&  code_mo_old!=code_mo ) {

                    // закроем предыдущюю МО
                    QString s_new_line = s_mo_end;
                    s_new_line = s_new_line
                            .replace("#CODE_MO#",   code_mo)
                            .replace("#NAME_MO#",   name_mo)
                            .replace("#COUNT4#",    QString::number(count_mo))
                            .replace("#PAY_AMB4#",  show_rubles(pay_amb_mo))
                            .replace("#PAY_STIM4#", show_rubles(pay_stim_mo))
                            .replace("#PAY_FAP4#",  show_rubles(pay_fap_mo))
                            .replace("#PAY_MAINT4#",show_rubles(pay_maint_mo))
                            .replace("#PAY_ALL4#",  show_rubles(pay_all_mo));
                    s_content_new += s_new_line;

                    // сбросим сумматоры по МО
                    count_mo = 0;  pay_amb_mo = 0;  pay_stim_mo = 0;  pay_fap_mo = 0;  pay_maint_mo = 0;  pay_all_mo = 0;
                }

                // -------------------------
                // если сменилось юрлицо ...
                if ( !code_head_old.isEmpty()  &&  code_head_old!=code_head ) {

                    // закроем предыдущее юрлицо
                    QString s_new_line = s_head_end;
                    s_new_line = s_new_line
                            .replace("#COUNT5#",    QString::number(count_head))
                            .replace("#PAY_AMB5#",  show_rubles(pay_amb_head))
                            .replace("#PAY_STIM5#", show_rubles(pay_stim_head))
                            .replace("#PAY_FAP5#",  show_rubles(pay_fap_head))
                            .replace("#PAY_MAINT5#",show_rubles(pay_maint_head))
                            .replace("#PAY_ALL5#",  show_rubles(pay_all_head));
                    s_content_new += s_new_line;

                    // сбросим сумматоры
                    count_head = 0;  pay_amb_head = 0;  pay_stim_head = 0;  pay_fap_head = 0;  pay_maint_head = 0;  pay_all_head = 0;

                    // обновим номер таблицы
                    //s_content_new = s_content_new.replace("table:name=\"Таблица2\"","table:name=\"ТаблиЦЦАА_" + QString::number(n_medhead) + "\"");

                    n_medhead++;
                }


                // ----------------------------------------------
                // если юрлицо изменилось и новое юрлицо есть ...
                if ( code_head_old!=code_head
                    && !code_head.isEmpty() ) {
                    // ... добавим новое юрлицо
                    s_new_line = s_head_start;
                    s_new_line = s_new_line
                            .replace("#CODE_HEAD#", code_head)
                            .replace("#NAME_HEAD#", name_head);
                    s_content_new += s_new_line;
                }

                // -------------------------------------
                // если МО изменилась и новая МО есть ...
                if ( (code_head_old!=code_head || code_mo_old!=code_mo)
                    && !code_mo.isEmpty()) {
                    // ... добавим новую МО
                    s_new_line = s_mo_start;
                    s_new_line = s_new_line
                            .replace("#CODE_MO#", code_mo)
                            .replace("#NAME_MO#", name_mo);
                    s_content_new += s_new_line;
                }


                // ----------------------------------------------
                // если участок измнился и новый участок есть ...
                if (!code_ms.isEmpty()) {

                    // ... добавим новый участок
                    s_new_line = s_ms_start;
                    s_new_line = s_new_line
                            .replace("#CODE_MP#", code_mp)
                            .replace("#CODE_MS#", code_ms);
                    s_content_new += s_new_line;

                    // ------------------------------------------------------------ //
                    // в зависимости от типа участка - добавим возрастные категории //
                    // ------------------------------------------------------------ //

                    // --------- //
                    // до 1 года //
                    if ( type_ms!=2 && type_ms!=5             // не терапевт и не ВОП для взросых
                         && (cnt_m_0_0>0 || cnt_w_0_0>0) ) {  // есть дети до 1 года, М или Ж

                        s_new_line = s_age_start;
                        s_new_line = s_new_line
                                .replace("#AGE#", "до 1 года");
                        s_content_new += s_new_line;

                        // сбросим сумматоры
                        count_age = 0;  pay_amb_age = 0;  pay_stim_age = 0;  pay_fap_age = 0;  pay_maint_age = 0;  pay_all_age = 0;

                        if (cnt_m_0_0>0 ) {
                            // до 1 года - муж
                            s_new_line = s_sex;
                            s_new_line = s_new_line
                                    .replace("#SEX#",      "муж")
                                    .replace("#COUNT#",    QString::number(cnt_m_0_0))
                                    .replace("#PAY_AMB#",  show_rubles(fin_m_0_0))
                                    .replace("#PAY_STIM#", "0,00")
                                    .replace("#PAY_FAP#",  "0,00")
                                    .replace("#PAY_MAINT#","0,00")
                                    .replace("#PAY_ALL#",  show_rubles(fin_m_0_0));
                            s_content_new += s_new_line;

                            // суумматоры
                            count_age  += cnt_m_0_0;  pay_amb_age  += fin_m_0_0;  pay_stim_age  += 0;  pay_fap_age  += 0;  pay_maint_age  += 0;  pay_all_age  += fin_m_0_0;
                            count_ms   += cnt_m_0_0;  pay_amb_ms   += fin_m_0_0;  pay_stim_ms   += 0;  pay_fap_ms   += 0;  pay_maint_ms   += 0;  pay_all_ms   += fin_m_0_0;
                            count_mo   += cnt_m_0_0;  pay_amb_mo   += fin_m_0_0;  pay_stim_mo   += 0;  pay_fap_mo   += 0;  pay_maint_mo   += 0;  pay_all_mo   += fin_m_0_0;
                            count_head += cnt_m_0_0;  pay_amb_head += fin_m_0_0;  pay_stim_head += 0;  pay_fap_head += 0;  pay_maint_head += 0;  pay_all_head += fin_m_0_0;
                        }
                        if (cnt_w_0_0>0 ) {
                            // до 1 года - жен
                            s_new_line = s_sex;
                            s_new_line = s_new_line
                                    .replace("#SEX#",      "жен")
                                    .replace("#COUNT#",    QString::number(cnt_w_0_0))
                                    .replace("#PAY_AMB#",  show_rubles(fin_w_0_0))
                                    .replace("#PAY_STIM#", "0,00")
                                    .replace("#PAY_FAP#",  "0,00")
                                    .replace("#PAY_MAINT#","0,00")
                                    .replace("#PAY_ALL#",  show_rubles(fin_w_0_0));
                            s_content_new += s_new_line;

                            // суумматоры
                            count_age  += cnt_w_0_0;  pay_amb_age  += fin_w_0_0;  pay_stim_age  += 0;  pay_fap_age  += 0;  pay_maint_age  += 0;  pay_all_age  += fin_w_0_0;
                            count_ms   += cnt_w_0_0;  pay_amb_ms   += fin_w_0_0;  pay_stim_ms   += 0;  pay_fap_ms   += 0;  pay_maint_ms   += 0;  pay_all_ms   += fin_w_0_0;
                            count_mo   += cnt_w_0_0;  pay_amb_mo   += fin_w_0_0;  pay_stim_mo   += 0;  pay_fap_mo   += 0;  pay_maint_mo   += 0;  pay_all_mo   += fin_w_0_0;
                            count_head += cnt_w_0_0;  pay_amb_head += fin_w_0_0;  pay_stim_head += 0;  pay_fap_head += 0;  pay_maint_head += 0;  pay_all_head += fin_w_0_0;
                        }
                        // до 1 года, закрытие - сумматоры
                        s_new_line = s_age_end;
                        s_new_line = s_new_line
                                .replace("#AGE#",       "до 1 года")
                                .replace("#COUNT2#",    QString::number(count_age))
                                .replace("#PAY_AMB2#",  show_rubles(pay_amb_age))
                                .replace("#PAY_STIM2#", show_rubles(pay_stim_age))
                                .replace("#PAY_FAP2#",  show_rubles(pay_fap_age))
                                .replace("#PAY_MAINT2#",show_rubles(pay_maint_age))
                                .replace("#PAY_ALL2#",  show_rubles(pay_all_age));
                        s_content_new += s_new_line;
                    }

                    // ------------- //
                    // от 1 до 4 лет //
                    if ( type_ms!=2 && type_ms!=5               // не терапевт и не ВОП для взросых
                         && (cnt_m_1_4>0 || cnt_w_1_4>0) ) {    // есть дети от 1 до 4 лет, М или Ж

                        s_new_line = s_age_start;
                        s_new_line = s_new_line
                                .replace("#AGE#", "от 1 до 4 лет");
                        s_content_new += s_new_line;

                        // сбросим сумматоры
                        count_age = 0;  pay_amb_age = 0;  pay_stim_age = 0;  pay_fap_age = 0;  pay_maint_age = 0;  pay_all_age = 0;

                        if (cnt_m_1_4>0 ) {
                            // от 1 до 4 лет - муж
                            s_new_line = s_sex;
                            s_new_line = s_new_line
                                    .replace("#SEX#",      "муж")
                                    .replace("#COUNT#",    QString::number(cnt_m_1_4))
                                    .replace("#PAY_AMB#",  show_rubles(fin_m_1_4))
                                    .replace("#PAY_STIM#", "0,00")
                                    .replace("#PAY_FAP#",  "0,00")
                                    .replace("#PAY_MAINT#","0,00")
                                    .replace("#PAY_ALL#",  show_rubles(fin_m_1_4));
                            s_content_new += s_new_line;

                            // суумматоры
                            count_age  += cnt_m_1_4;  pay_amb_age  += fin_m_1_4;  pay_stim_age  += 0;  pay_fap_age  += 0;  pay_maint_age  += 0;  pay_all_age  += fin_m_1_4;
                            count_ms   += cnt_m_1_4;  pay_amb_ms   += fin_m_1_4;  pay_stim_ms   += 0;  pay_fap_ms   += 0;  pay_maint_ms   += 0;  pay_all_ms   += fin_m_1_4;
                            count_mo   += cnt_m_1_4;  pay_amb_mo   += fin_m_1_4;  pay_stim_mo   += 0;  pay_fap_mo   += 0;  pay_maint_mo   += 0;  pay_all_mo   += fin_m_1_4;
                            count_head += cnt_m_1_4;  pay_amb_head += fin_m_1_4;  pay_stim_head += 0;  pay_fap_head += 0;  pay_maint_head += 0;  pay_all_head += fin_m_1_4;
                        }
                        if (cnt_w_1_4>0 ) {
                            // от 1 до 4 лет - жен
                            s_new_line = s_sex;
                            s_new_line = s_new_line
                                    .replace("#SEX#",      "жен")
                                    .replace("#COUNT#",    QString::number(cnt_w_1_4))
                                    .replace("#PAY_AMB#",  show_rubles(fin_w_1_4))
                                    .replace("#PAY_STIM#", "0,00")
                                    .replace("#PAY_FAP#",  "0,00")
                                    .replace("#PAY_MAINT#","0,00")
                                    .replace("#PAY_ALL#",  show_rubles(fin_w_1_4));
                            s_content_new += s_new_line;

                            // суумматоры
                            count_age  += cnt_w_1_4;  pay_amb_age  += fin_w_1_4;  pay_stim_age  += 0;  pay_fap_age  += 0;  pay_maint_age  += 0;  pay_all_age  += fin_w_1_4;
                            count_ms   += cnt_w_1_4;  pay_amb_ms   += fin_w_1_4;  pay_stim_ms   += 0;  pay_fap_ms   += 0;  pay_maint_ms   += 0;  pay_all_ms   += fin_w_1_4;
                            count_mo   += cnt_w_1_4;  pay_amb_mo   += fin_w_1_4;  pay_stim_mo   += 0;  pay_fap_mo   += 0;  pay_maint_mo   += 0;  pay_all_mo   += fin_w_1_4;
                            count_head += cnt_w_1_4;  pay_amb_head += fin_w_1_4;  pay_stim_head += 0;  pay_fap_head += 0;  pay_maint_head += 0;  pay_all_head += fin_w_1_4;
                        }
                        // от 1 до 4 лет, закрытие - сумматоры
                        s_new_line = s_age_end;
                        s_new_line = s_new_line
                                .replace("#AGE#",       "от 1 до 4 лет")
                                .replace("#COUNT2#",    QString::number(count_age))
                                .replace("#PAY_AMB2#",  show_rubles(pay_amb_age))
                                .replace("#PAY_STIM2#", show_rubles(pay_stim_age))
                                .replace("#PAY_FAP2#",  show_rubles(pay_fap_age))
                                .replace("#PAY_MAINT2#",show_rubles(pay_maint_age))
                                .replace("#PAY_ALL2#",  show_rubles(pay_all_age));
                        s_content_new += s_new_line;
                    }

                    // -------------- //
                    // от 5 до 17 лет //
                    if ( type_ms!=2 && type_ms!=5               // не терапевт и не ВОП для взросых
                         && (cnt_m_5_17>0 || cnt_w_5_17>0) ) {  // есть дети от 5 до 17 лет, М или Ж

                        s_new_line = s_age_start;
                        s_new_line = s_new_line
                                .replace("#AGE#", "от 5 до 17 лет");
                        s_content_new += s_new_line;

                        // сбросим сумматоры
                        count_age = 0;  pay_amb_age = 0;  pay_stim_age = 0;  pay_fap_age = 0;  pay_maint_age = 0;  pay_all_age = 0;

                        if (cnt_m_5_17>0 ) {
                            // от 5 до 17 лет - муж
                            s_new_line = s_sex;
                            s_new_line = s_new_line
                                    .replace("#SEX#",      "муж")
                                    .replace("#COUNT#",    QString::number(cnt_m_5_17))
                                    .replace("#PAY_AMB#",  show_rubles(fin_m_5_17))
                                    .replace("#PAY_STIM#", "0,00")
                                    .replace("#PAY_FAP#",  "0,00")
                                    .replace("#PAY_MAINT#","0,00")
                                    .replace("#PAY_ALL#",  show_rubles(fin_m_5_17));
                            s_content_new += s_new_line;

                            // суумматоры
                            count_age  += cnt_m_5_17;  pay_amb_age  += fin_m_5_17;  pay_stim_age  += 0;  pay_fap_age  += 0;  pay_maint_age  += 0;  pay_all_age  += fin_m_5_17;
                            count_ms   += cnt_m_5_17;  pay_amb_ms   += fin_m_5_17;  pay_stim_ms   += 0;  pay_fap_ms   += 0;  pay_maint_ms   += 0;  pay_all_ms   += fin_m_5_17;
                            count_mo   += cnt_m_5_17;  pay_amb_mo   += fin_m_5_17;  pay_stim_mo   += 0;  pay_fap_mo   += 0;  pay_maint_mo   += 0;  pay_all_mo   += fin_m_5_17;
                            count_head += cnt_m_5_17;  pay_amb_head += fin_m_5_17;  pay_stim_head += 0;  pay_fap_head += 0;  pay_maint_head += 0;  pay_all_head += fin_m_5_17;
                        }
                        if (cnt_w_5_17>0 ) {
                            // от 5 до 17 лет - жен
                            s_new_line = s_sex;
                            s_new_line = s_new_line
                                    .replace("#SEX#",      "жен")
                                    .replace("#COUNT#",    QString::number(cnt_w_5_17))
                                    .replace("#PAY_AMB#",  show_rubles(fin_w_5_17))
                                    .replace("#PAY_STIM#", "0,00")
                                    .replace("#PAY_FAP#",  "0,00")
                                    .replace("#PAY_MAINT#","0,00")
                                    .replace("#PAY_ALL#",  show_rubles(fin_w_5_17));
                            s_content_new += s_new_line;

                            // суумматоры
                            count_age  += cnt_w_5_17;  pay_amb_age  += fin_w_5_17;  pay_stim_age  += 0;  pay_fap_age  += 0;  pay_maint_age  += 0;  pay_all_age  += fin_w_5_17;
                            count_ms   += cnt_w_5_17;  pay_amb_ms   += fin_w_5_17;  pay_stim_ms   += 0;  pay_fap_ms   += 0;  pay_maint_ms   += 0;  pay_all_ms   += fin_w_5_17;
                            count_mo   += cnt_w_5_17;  pay_amb_mo   += fin_w_5_17;  pay_stim_mo   += 0;  pay_fap_mo   += 0;  pay_maint_mo   += 0;  pay_all_mo   += fin_w_5_17;
                            count_head += cnt_w_5_17;  pay_amb_head += fin_w_5_17;  pay_stim_head += 0;  pay_fap_head += 0;  pay_maint_head += 0;  pay_all_head += fin_w_5_17;
                        }
                        // от 5 до 17 лет, закрытие - сумматоры
                        s_new_line = s_age_end;
                        s_new_line = s_new_line
                                .replace("#AGE#",       "от 5 до 17 лет")
                                .replace("#COUNT2#",    QString::number(count_age))
                                .replace("#PAY_AMB2#",  show_rubles(pay_amb_age))
                                .replace("#PAY_STIM2#", show_rubles(pay_stim_age))
                                .replace("#PAY_FAP2#",  show_rubles(pay_fap_age))
                                .replace("#PAY_MAINT2#",show_rubles(pay_maint_age))
                                .replace("#PAY_ALL2#",  show_rubles(pay_all_age));
                        s_content_new += s_new_line;
                    }

                    // --------------- //
                    // от 18 до 59 лет //
                    if ( type_ms!=1 && type_ms!=4      // не педиатор и не ВОП для детей
                         && (cnt_m_18_59>0) ) {        // есть дети от 18 до 59 лет, М

                        s_new_line = s_age_start;
                        s_new_line = s_new_line
                                .replace("#AGE#", "от 18 до 59 лет");
                        s_content_new += s_new_line;

                        // сбросим сумматоры
                        count_age = 0;  pay_amb_age = 0;  pay_stim_age = 0;  pay_fap_age = 0;  pay_maint_age = 0;  pay_all_age = 0;

                        if (cnt_m_18_59>0 ) {
                            // от 18 до 59 лет - муж
                            s_new_line = s_sex;
                            s_new_line = s_new_line
                                    .replace("#SEX#",      "муж")
                                    .replace("#COUNT#",    QString::number(cnt_m_18_59))
                                    .replace("#PAY_AMB#",  show_rubles(fin_m_18_59))
                                    .replace("#PAY_STIM#", "0,00")
                                    .replace("#PAY_FAP#",  "0,00")
                                    .replace("#PAY_MAINT#","0,00")
                                    .replace("#PAY_ALL#",  show_rubles(fin_m_18_59));
                            s_content_new += s_new_line;

                            // суумматоры
                            count_age  += cnt_m_18_59;  pay_amb_age  += fin_m_18_59;  pay_stim_age  += 0;  pay_fap_age  += 0;  pay_maint_age  += 0;  pay_all_age  += fin_m_18_59;
                            count_ms   += cnt_m_18_59;  pay_amb_ms   += fin_m_18_59;  pay_stim_ms   += 0;  pay_fap_ms   += 0;  pay_maint_ms   += 0;  pay_all_ms   += fin_m_18_59;
                            count_mo   += cnt_m_18_59;  pay_amb_mo   += fin_m_18_59;  pay_stim_mo   += 0;  pay_fap_mo   += 0;  pay_maint_mo   += 0;  pay_all_mo   += fin_m_18_59;
                            count_head += cnt_m_18_59;  pay_amb_head += fin_m_18_59;  pay_stim_head += 0;  pay_fap_head += 0;  pay_maint_head += 0;  pay_all_head += fin_m_18_59;
                        }
                        // от 18 до 59 лет, закрытие - сумматоры
                        s_new_line = s_age_end;
                        s_new_line = s_new_line
                                .replace("#AGE#",       "от 18 до 59 лет")
                                .replace("#COUNT2#",    QString::number(count_age))
                                .replace("#PAY_AMB2#",  show_rubles(pay_amb_age))
                                .replace("#PAY_STIM2#", show_rubles(pay_stim_age))
                                .replace("#PAY_FAP2#",  show_rubles(pay_fap_age))
                                .replace("#PAY_MAINT2#",show_rubles(pay_maint_age))
                                .replace("#PAY_ALL2#",  show_rubles(pay_all_age));
                        s_content_new += s_new_line;
                    }

                    // --------------- //
                    // от 18 до 54 лет //
                    if ( type_ms!=1 && type_ms!=4      // не педиатор и не ВОП для детей
                         && (cnt_w_18_54>0) ) {        // есть дети от 18 до 54 лет, Ж

                        s_new_line = s_age_start;
                        s_new_line = s_new_line
                                .replace("#AGE#", "от 18 до 54 лет");
                        s_content_new += s_new_line;

                        // сбросим сумматоры
                        count_age = 0;  pay_amb_age = 0;  pay_stim_age = 0;  pay_fap_age = 0;  pay_maint_age = 0;  pay_all_age = 0;

                        if (cnt_w_18_54>0 ) {
                            // от 18 до 54 лет - жен
                            s_new_line = s_sex;
                            s_new_line = s_new_line
                                    .replace("#SEX#",      "жен")
                                    .replace("#COUNT#",    QString::number(cnt_w_18_54))
                                    .replace("#PAY_AMB#",  show_rubles(fin_w_18_54))
                                    .replace("#PAY_STIM#", "0,00")
                                    .replace("#PAY_FAP#",  "0,00")
                                    .replace("#PAY_MAINT#","0,00")
                                    .replace("#PAY_ALL#",  show_rubles(fin_w_18_54));
                            s_content_new += s_new_line;

                            // суумматоры
                            count_age  += cnt_w_18_54;  pay_amb_age  += fin_w_18_54;  pay_stim_age  += 0;  pay_fap_age  += 0;  pay_maint_age  += 0;  pay_all_age  += fin_w_18_54;
                            count_ms   += cnt_w_18_54;  pay_amb_ms   += fin_w_18_54;  pay_stim_ms   += 0;  pay_fap_ms   += 0;  pay_maint_ms   += 0;  pay_all_ms   += fin_w_18_54;
                            count_mo   += cnt_w_18_54;  pay_amb_mo   += fin_w_18_54;  pay_stim_mo   += 0;  pay_fap_mo   += 0;  pay_maint_mo   += 0;  pay_all_mo   += fin_w_18_54;
                            count_head += cnt_w_18_54;  pay_amb_head += fin_w_18_54;  pay_stim_head += 0;  pay_fap_head += 0;  pay_maint_head += 0;  pay_all_head += fin_w_18_54;
                        }
                        // от 18 до 54 лет, закрытие - сумматоры
                        s_new_line = s_age_end;
                        s_new_line = s_new_line
                                .replace("#AGE#",       "от 18 до 54 лет")
                                .replace("#COUNT2#",    QString::number(count_age))
                                .replace("#PAY_AMB2#",  show_rubles(pay_amb_age))
                                .replace("#PAY_STIM2#", show_rubles(pay_stim_age))
                                .replace("#PAY_FAP2#",  show_rubles(pay_fap_age))
                                .replace("#PAY_MAINT2#",show_rubles(pay_maint_age))
                                .replace("#PAY_ALL2#",  show_rubles(pay_all_age));
                        s_content_new += s_new_line;
                    }

                    // --------------- //
                    // 60 лет и старше //
                    if ( type_ms!=1 && type_ms!=4      // не педиатор и не ВОП для детей
                         && (cnt_m_60_>0) ) {          // есть дети 60 лет и старше, М

                        s_new_line = s_age_start;
                        s_new_line = s_new_line
                                .replace("#AGE#", "60 лет и старше");
                        s_content_new += s_new_line;

                        // сбросим сумматоры
                        count_age = 0;  pay_amb_age = 0;  pay_stim_age = 0;  pay_fap_age = 0;  pay_maint_age = 0;  pay_all_age = 0;

                        if (cnt_m_60_>0 ) {
                            // 60 лет и старше - муж
                            s_new_line = s_sex;
                            s_new_line = s_new_line
                                    .replace("#SEX#",      "муж")
                                    .replace("#COUNT#",    QString::number(cnt_m_60_))
                                    .replace("#PAY_AMB#",  show_rubles(fin_m_60_))
                                    .replace("#PAY_STIM#", "0,00")
                                    .replace("#PAY_FAP#",  "0,00")
                                    .replace("#PAY_MAINT#","0,00")
                                    .replace("#PAY_ALL#",  show_rubles(fin_m_60_));
                            s_content_new += s_new_line;

                            // суумматоры
                            count_age  += cnt_m_60_;  pay_amb_age  += fin_m_60_;  pay_stim_age  += 0;  pay_fap_age  += 0;  pay_maint_age  += 0;  pay_all_age  += fin_m_60_;
                            count_ms   += cnt_m_60_;  pay_amb_ms   += fin_m_60_;  pay_stim_ms   += 0;  pay_fap_ms   += 0;  pay_maint_ms   += 0;  pay_all_ms   += fin_m_60_;
                            count_mo   += cnt_m_60_;  pay_amb_mo   += fin_m_60_;  pay_stim_mo   += 0;  pay_fap_mo   += 0;  pay_maint_mo   += 0;  pay_all_mo   += fin_m_60_;
                            count_head += cnt_m_60_;  pay_amb_head += fin_m_60_;  pay_stim_head += 0;  pay_fap_head += 0;  pay_maint_head += 0;  pay_all_head += fin_m_60_;
                        }
                        // 60 лет и старше, закрытие - сумматоры
                        s_new_line = s_age_end;
                        s_new_line = s_new_line
                                .replace("#AGE#",       "60 лет и старше")
                                .replace("#COUNT2#",    QString::number(count_age))
                                .replace("#PAY_AMB2#",  show_rubles(pay_amb_age))
                                .replace("#PAY_STIM2#", show_rubles(pay_stim_age))
                                .replace("#PAY_FAP2#",  show_rubles(pay_fap_age))
                                .replace("#PAY_MAINT2#",show_rubles(pay_maint_age))
                                .replace("#PAY_ALL2#",  show_rubles(pay_all_age));
                        s_content_new += s_new_line;
                    }

                    // --------------- //
                    // 55 лет и старше //
                    if ( type_ms!=1 && type_ms!=4      // не педиатор и не ВОП для детей
                         && (cnt_w_55_>0) ) {          // есть дети 55 лет и старше, М или Ж

                        s_new_line = s_age_start;
                        s_new_line = s_new_line
                                .replace("#AGE#", "55 лет и старше");
                        s_content_new += s_new_line;

                        // сбросим сумматоры
                        count_age = 0;  pay_amb_age = 0;  pay_stim_age = 0;  pay_fap_age = 0;  pay_maint_age = 0;  pay_all_age = 0;

                        if (cnt_w_55_>0 ) {
                            // 55 лет и старше - жен
                            s_new_line = s_sex;
                            s_new_line = s_new_line
                                    .replace("#SEX#",      "жен")
                                    .replace("#COUNT#",    QString::number(cnt_w_55_))
                                    .replace("#PAY_AMB#",  show_rubles(fin_w_55_))
                                    .replace("#PAY_STIM#", "0,00")
                                    .replace("#PAY_FAP#",  "0,00")
                                    .replace("#PAY_MAINT#","0,00")
                                    .replace("#PAY_ALL#",  show_rubles(fin_w_55_));
                            s_content_new += s_new_line;

                            // суумматоры
                            count_age  += cnt_w_55_;  pay_amb_age  += fin_w_55_;  pay_stim_age  += 0;  pay_fap_age  += 0;  pay_maint_age  += 0;  pay_all_age  += fin_w_55_;
                            count_ms   += cnt_w_55_;  pay_amb_ms   += fin_w_55_;  pay_stim_ms   += 0;  pay_fap_ms   += 0;  pay_maint_ms   += 0;  pay_all_ms   += fin_w_55_;
                            count_mo   += cnt_w_55_;  pay_amb_mo   += fin_w_55_;  pay_stim_mo   += 0;  pay_fap_mo   += 0;  pay_maint_mo   += 0;  pay_all_mo   += fin_w_55_;
                            count_head += cnt_w_55_;  pay_amb_head += fin_w_55_;  pay_stim_head += 0;  pay_fap_head += 0;  pay_maint_head += 0;  pay_all_head += fin_w_55_;
                        }
                        // 55 лет и старше, закрытие - сумматоры
                        s_new_line = s_age_end;
                        s_new_line = s_new_line
                                .replace("#AGE#",       "55 лет и старше")
                                .replace("#COUNT2#",    QString::number(count_age))
                                .replace("#PAY_AMB2#",  show_rubles(pay_amb_age))
                                .replace("#PAY_STIM2#", show_rubles(pay_stim_age))
                                .replace("#PAY_FAP2#",  show_rubles(pay_fap_age))
                                .replace("#PAY_MAINT2#",show_rubles(pay_maint_age))
                                .replace("#PAY_ALL2#",  show_rubles(pay_all_age));
                        s_content_new += s_new_line;
                    }
                }

                // --------------------------------- //
                // запомним последние значения кодов //
                code_head_old = code_head;
                code_mo_old   = code_mo;
                code_mp_old   = code_mp;
                code_ms_old   = code_ms;
            }

            // ------------------------- //
            // закроем последний участок //
            s_new_line = s_ms_end;
            s_new_line = s_new_line
                  .replace("#CODE_MP#",   code_mp_old)
                  .replace("#CODE_MS#",   code_ms_old)
                  .replace("#COUNT3#",    QString::number(count_ms))
                  .replace("#PAY_AMB3#",  show_rubles(pay_amb_ms))
                  .replace("#PAY_STIM3#", show_rubles(pay_stim_ms))
                  .replace("#PAY_FAP3#",  show_rubles(pay_fap_ms))
                  .replace("#PAY_MAINT3#",show_rubles(pay_maint_ms))
                  .replace("#PAY_ALL3#",  show_rubles(pay_all_ms));
            s_content_new += s_new_line;

            // -------------------- //
            // закроем последнее МО //
            s_new_line = s_mo_end;
            s_new_line = s_new_line
                    .replace("#CODE_MO#",   code_mo)
                    .replace("#NAME_MO#",   name_mo)
                    .replace("#COUNT4#",    QString::number(count_mo))
                    .replace("#PAY_AMB4#",  show_rubles(pay_amb_mo))
                    .replace("#PAY_STIM4#", show_rubles(pay_stim_mo))
                    .replace("#PAY_FAP4#",  show_rubles(pay_fap_mo))
                    .replace("#PAY_MAINT4#",show_rubles(pay_maint_mo))
                    .replace("#PAY_ALL4#",  show_rubles(pay_all_mo));
            s_content_new += s_new_line;

            // ------------------------ //
            // закроем последнее юрдицо //
            s_new_line = s_head_end;
            s_new_line = s_new_line
                  .replace("#COUNT5#",    QString::number(count_head))
                  .replace("#PAY_AMB5#",  show_rubles(pay_amb_head))
                  .replace("#PAY_STIM5#", show_rubles(pay_stim_head))
                  .replace("#PAY_FAP5#",  show_rubles(pay_fap_head))
                  .replace("#PAY_MAINT5#",show_rubles(pay_maint_head))
                  .replace("#PAY_ALL5#",  show_rubles(pay_all_head));
            s_content_new += s_new_line;

            // --------------------- //
            // закроем файл контента //
            s_content_new += s_end;


            delete query_data;

            // сохранение контента шаблона
            // QFile f_content_save(tmp_folder + "/content.xml");
            f_content_save.open(QIODevice::WriteOnly);
            f_content_save.write(s_content_new.toUtf8());
            f_content_save.close();


            // ===========================
            // архивация шаблона
            // ===========================
            QString fname_res = data_app.path_out + "_FIN_REPS_/" + QDate::currentDate().toString("yyyy-MM-dd") + "  -  " + data_app.smo_short + " - " + data_app.filial_name + "  -  " + code_head + " - Отчёт по финансированию МО в разрезе участков и категорий пациентов.odt";

            QFile::remove(fname_res);
            while (QFile::exists(fname_res)) {
                if (QMessageBox::warning(this,
                                         "Ошибка сохранения акта",
                                         "Не удалось сохранить сформированный отчёт: \n" + rep_template +
                                         "\n\nво временную папку\n" + fname_res +
                                         "\n\nПроверьте, не открыт ли целевой файл в сторонней программе и повторите операцию.",
                                         QMessageBox::Retry|QMessageBox::Abort,
                                         QMessageBox::Retry)==QMessageBox::Abort) {
                    this->setCursor(Qt::ArrowCursor);
                    ui->pan_finance_DN->setEnabled(true);
                    ui->pan_wait->setVisible(false);
                    return;
                }
                QFile::remove(fname_res);
            }
            if (!QFile::copy(rep_template, fname_res) || !QFile::exists(fname_res)) {
                QMessageBox::warning(this, "Ошибка копирования шаблона",
                                     "Не удалось скопировать сформированный отчёт: \n" + rep_template +
                                     "\n\nво временную папку\n" + fname_res +
                                     "\n\nОперация прервана.");
                this->setCursor(Qt::ArrowCursor);
                ui->pan_finance_DN->setEnabled(true);
                ui->pan_wait->setVisible(false);
                return;
            }
            com = "\"" + data_app.path_arch + "7z.exe\" a \"" + fname_res + "\" \"" + tmp_folder + "/content.xml\"";
            if (myProcess.execute(com)!=0) {
                QMessageBox::warning(this,
                                     "Ошибка обновления контента",
                                     "При добавлении нового контента в шаблон печатной формы заявления произошла непредвиденная ошибка\n\n"
                                     "Опреация прервана.");
            }
        }
    }

    // ===========================
    // собственно открытие шаблона
    // ===========================
    // открытие полученного ODT
    QString s = data_app.path_out + "_FIN_REPS_/";
    long result = (long long)ShellExecute(0, 0, reinterpret_cast<const WCHAR*>(s.utf16()), 0, 0, SW_NORMAL);
    this->setCursor(Qt::ArrowCursor);
    if (result<32) {
        QMessageBox::warning(this,
                             "Ошибка при открытии папки с отчётами",
                             "Папка со сформированными отчётами не найдена:\n\n" + s +
                             "\nКод ошибки: " + QString::number(result) +
                             "\n\nПопробуйте открыть эту папку вручную.\n\n"
                             "Если папка не создана или ошибка будет повторяться - обратитесь к разработчику.");
    }

    this->setCursor(Qt::ArrowCursor);
    ui->pan_finance_DN->setEnabled(true);
    ui->pan_wait->setVisible(false);
}

void finance_medorgs_wnd::on_bn_now_clicked() {
    ui->date_calc->setDate(QDate::currentDate());
}
