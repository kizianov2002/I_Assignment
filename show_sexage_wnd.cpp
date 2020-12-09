#include "show_sexage_wnd.h"
#include "ui_show_sexage_wnd.h"
#include "ki_funcs.h"

show_sexage_wnd::show_sexage_wnd(int id_sexage, QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    id_sexage(id_sexage),
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::show_sexage_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    show_tab_w = NULL;

    ui->pan_wait->setVisible(false);

    // получим данные таблицы
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select id, id_layer, assig_type "
                  "  from assig.sexages s "
                  " where id=" + QString::number(id_sexage) + " ; ";
    mySQL.exec(this, sql, "получим данные таблицы", *query, true, db, data_app);
    if (query->next()) {
        id_layer = query->value(1).toInt();
        assig_type = query->value(2).toInt();
    }
    delete query;

    refresh_combo_sexage();
    ui->combo_sexage->setCurrentIndex(ui->combo_sexage->findData(id_sexage));
    refresh_sexage_tab();
}

show_sexage_wnd::~show_sexage_wnd() {
    delete ui;
}

void show_sexage_wnd::on_bn_close_clicked() {
    close();
}

void show_sexage_wnd::refresh_combo_sexage() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка медорганизаций
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select s.id, (l.layer_name ||'  -  '|| case s.assig_type when 1 then 'по терр.' when 2 then 'по заявл.' else '- все -' end) as sexage_name, "
                  "       l.layer_date, l.layer_type, s.assig_type "
                  "  from assig.sexages s "
                  "  left join assig.layers l on(l.id=s.id_layer) "
                  " order by l.layer_date DESC, l.layer_type, s.assig_type ; ";
    mySQL.exec(this, sql, "Список снимков данных прикреления", *query, true, db, data_app);
    ui->combo_sexage->clear();
    while (query->next()) {
        ui->combo_sexage->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void show_sexage_wnd::on_combo_sexage_activated(int index) {
    id_sexage = ui->combo_sexage->currentData().toInt();
    refresh_sexage_tab();
}

void show_sexage_wnd::on_bn_refresh_sexage_clicked() {
    refresh_sexage_tab();
}


void show_sexage_wnd::refresh_sexage_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    ui->lab_wait->setText("...  ПОДОЖДИТЕ, ИДЁТ ЧТЕНИЕ ДАННЫХ  ...");
    ui->pan_wait->setVisible(true);
    ui->tab_sexage->setEnabled(false);
    QApplication::processEvents();

    // получим данные выбранной таблицы
    QString title = "Половозрастное распределение в разрезе МО:  " + ui->combo_sexage->currentText();
    this->setWindowTitle(title);

    sql_sexage_tab = "select * "
                     "  from "
                     "  ( ( select s.id, s.code_head, mo.name_mo, "
                     "             s.m_0_0, s.m_1_4, s.m_5_17, s.m_18_59, s.m_60_, s.m_sum, (s.m_0_0+s.m_1_4+s.m_5_17+s.m_18_59+s.m_60_) as m_all, "
                     "             s.w_0_0, s.w_1_4, s.w_5_17, s.w_18_54, s.w_55_, s.w_sum, (s.w_0_0+s.w_1_4+s.w_5_17+s.w_18_54+s.w_55_) as w_all, "
                     "             sum, (s.m_sum+s.w_sum) as all, snils_mt "
                     "        from assig.sexages_lines s "
                     "        left join spr_medorgs mo on(mo.code_mo=s.code_head) "
                     "       where s.id_sexage=" + QString::number(id_sexage) + " "
                     "       order by code_head ) "
                     "   union "
                     "    ( select NULL, 'ИТОГО:', NULL, "
                     "             sum(s.m_0_0), sum(s.m_1_4), sum(s.m_5_17), sum(s.m_18_59), sum(s.m_60_), sum(s.m_sum), sum(s.m_0_0+s.m_1_4+s.m_5_17+s.m_18_59+s.m_60_) as m_all, "
                     "             sum(s.w_0_0), sum(s.w_1_4), sum(s.w_5_17), sum(s.w_18_54), sum(s.w_55_), sum(s.w_sum), sum(s.w_0_0+s.w_1_4+s.w_5_17+s.w_18_54+s.w_55_) as w_all, "
                     "             sum(s.sum), sum(s.m_sum+s.w_sum) as all, sum(snils_mt) "
                     "        from assig.sexages_lines s "
                     "       where s.id_sexage=" + QString::number(id_sexage) + " "
                     "         and code_head is not NULL ) "
                     "  ) q "
                     " order by q.code_head ; ";

    model_sexage_tab.setQuery(sql_sexage_tab,db);
    QString err2 = model_sexage_tab.lastError().driverText();

    // подключаем модель из БД
    ui->tab_sexage->setModel(&model_sexage_tab);

    // обновляем таблицу
    ui->tab_sexage->reset();

    // задаём ширину колонок
    ui->tab_sexage->setColumnWidth(  0,  2);   // s.id,
    ui->tab_sexage->setColumnWidth(  1, 50);   // s.code_mo,
    ui->tab_sexage->setColumnWidth(  2,210);   // mo.name_mo,
    ui->tab_sexage->setColumnWidth(  3, 45);   // s.m_0_0,
    ui->tab_sexage->setColumnWidth(  4, 45);   // s.m_1_4,
    ui->tab_sexage->setColumnWidth(  5, 55);   // s.m_5_17,
    ui->tab_sexage->setColumnWidth(  6, 55);   // s.m_18_59,
    ui->tab_sexage->setColumnWidth(  7, 60);   // s.m_60_,
    ui->tab_sexage->setColumnWidth(  8, 55);   // s.m_sum,
    ui->tab_sexage->setColumnWidth(  9,  2);   // (s.m_0_0+s.m_1_4+s.m_5_17+s.m_18_59+s.m_60_) as m_all,
    ui->tab_sexage->setColumnWidth( 10, 45);   // s.w_0_0,
    ui->tab_sexage->setColumnWidth( 11, 45);   // s.w_1_4,
    ui->tab_sexage->setColumnWidth( 12, 55);   // s.w_5_17,
    ui->tab_sexage->setColumnWidth( 13, 55);   // s.w_18_54,
    ui->tab_sexage->setColumnWidth( 14, 60);   // s.w_55_,
    ui->tab_sexage->setColumnWidth( 15, 55);   // s_w_sum,
    ui->tab_sexage->setColumnWidth( 16,  2);   // (s.w_0_0+s.w_1_4+s.w_5_17+s.w_18_54+s.w_55_) as w_all,
    ui->tab_sexage->setColumnWidth( 17, 60);   // sum,
    ui->tab_sexage->setColumnWidth( 18,  2);   // (s.m_sum+s_w_sum) as all,
    ui->tab_sexage->setColumnWidth( 19, 60);   // snils_mt

    // правим заголовки
    model_sexage_tab.setHeaderData(  0, Qt::Horizontal, ("ID"));
    model_sexage_tab.setHeaderData(  1, Qt::Horizontal, ("код МО"));
    model_sexage_tab.setHeaderData(  2, Qt::Horizontal, ("Медицинская организация"));
    model_sexage_tab.setHeaderData(  3, Qt::Horizontal, ("М \nдо 1г."));
    model_sexage_tab.setHeaderData(  4, Qt::Horizontal, ("М \n1-4г."));
    model_sexage_tab.setHeaderData(  5, Qt::Horizontal, ("М \n5-17л."));
    model_sexage_tab.setHeaderData(  6, Qt::Horizontal, ("М \n18-59л."));
    model_sexage_tab.setHeaderData(  7, Qt::Horizontal, ("М \nстар.60л."));
    model_sexage_tab.setHeaderData(  8, Qt::Horizontal, ("М \nвсего"));
    model_sexage_tab.setHeaderData(  9, Qt::Horizontal, ("М \nсумма"));
    model_sexage_tab.setHeaderData( 10, Qt::Horizontal, ("Ж \nдо 1г."));
    model_sexage_tab.setHeaderData( 11, Qt::Horizontal, ("Ж \n1-4г."));
    model_sexage_tab.setHeaderData( 12, Qt::Horizontal, ("Ж \n5-17л."));
    model_sexage_tab.setHeaderData( 13, Qt::Horizontal, ("Ж \n18-54л."));
    model_sexage_tab.setHeaderData( 14, Qt::Horizontal, ("Ж \nстар.55л."));
    model_sexage_tab.setHeaderData( 15, Qt::Horizontal, ("Ж \nвсего"));
    model_sexage_tab.setHeaderData( 16, Qt::Horizontal, ("Ж \nсумма"));
    model_sexage_tab.setHeaderData( 17, Qt::Horizontal, ("всего"));
    model_sexage_tab.setHeaderData( 18, Qt::Horizontal, ("сумма"));
    model_sexage_tab.setHeaderData( 19, Qt::Horizontal, ("со СНИЛС \nврача"));

    ui->tab_sexage->repaint();

    ui->pan_wait->setVisible(false);
    ui->tab_sexage->setEnabled(true);
    QApplication::processEvents();
    this->setCursor(Qt::ArrowCursor);
}

void show_sexage_wnd::on_bn_toCSV_clicked() {
    bool res_CSV = table2csv(db, sql_sexage_tab, model_sexage_tab, ui->tab_sexage, true,
                             data_app.path_out + "_ASSIG_DATA_/" + QDate::currentDate().toString("yyyy-MM-dd") + "  -  " + data_app.smo_short + " - " + data_app.filial_name + "  -  " +
                             "Половозрастное распределение застрахованных по снимку данных прикреления - " +
                             ui->combo_sexage->currentText() + " ",
                             this);
}

void show_sexage_wnd::on_tab_sexage_clicked(const QModelIndex &index) {
    int n_col = index.column();
    int n_row = index.row();
    QString capt = model_sexage_tab.data(model_sexage_tab.index(n_row,1), Qt::EditRole).toString();

    if ( n_col<3
         || n_col==8
         || n_col==9
         || n_col==15
         || n_col==16
         || n_col==17
         || n_col==18
         || capt=="ИТОГО:"
         || capt.isEmpty() ) {
        ui->bn_show_list->setEnabled(false);
    } else {
        ui->bn_show_list->setEnabled(true);
    }
}

void show_sexage_wnd::on_bn_show_list_clicked() {
    ui->lab_wait->setText("...  ПОДОЖДИТЕ, ИДЁТ СОСТАВЛЕНИЕ СПИСКА  ...");
    ui->pan_wait->setVisible(true);
    ui->tab_sexage->setEnabled(false);
    QApplication::processEvents();

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_sexage->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        this->setCursor(Qt::WaitCursor);
        QModelIndex index = indexes.front();

        QString assig_type_s = " ";
        bool f_assig_type = false;
        switch (assig_type) {
        case 1:
            assig_type_s = " and assig_type=1 ";
            f_assig_type = true;
            break;
        case 2:
            assig_type_s = " and assig_type=2 ";
            f_assig_type = true;
            break;
        default:
            assig_type_s = " ";
            f_assig_type = false;
            break;
        }

        int n_col = index.column();
        int n_row = index.row();
        int id_sexage_line = model_sexage_tab.data(model_sexage_tab.index(n_row,0), Qt::EditRole).toInt();
        QString code_mo = model_sexage_tab.data(model_sexage_tab.index(n_row,1), Qt::EditRole).toString();
        QString name_mo = model_sexage_tab.data(model_sexage_tab.index(n_row,2), Qt::EditRole).toString();

        if ( n_col<3
             || n_col==8
             || n_col==9
             || n_col==15
             || n_col==16
             || n_col==17
             || n_col==18
             || code_mo=="ИТОГО:"
             || code_mo.isEmpty() ) {
            ui->pan_wait->setVisible(false);
            ui->tab_sexage->setEnabled(true);
            QApplication::processEvents();
            QMessageBox::warning(this, "Данные не доступны",
                                 "Невозможно показать список рперсон для этой ячеки таблицы");
            return;
        }

        QString cell_name = "-???-";
        switch (n_col) {
        case 0:
            cell_name = "id";
            break;
        case 1:
            cell_name = "code_mo";
            break;
        case 2:
            cell_name = "name_mo";
            break;
        case 3:
            cell_name = "m_0_0";
            break;
        case 4:
            cell_name = "m_1_4";
            break;
        case 5:
            cell_name = "m_5_17";
            break;
        case 6:
            cell_name = "m_18_59";
            break;
        case 7:
            cell_name = "m_60_";
            break;
        case 8:
            cell_name = "m_sum";
            break;
        case 9:
            cell_name = "m_all";
            break;
        case 10:
            cell_name = "w_0_0";
            break;
        case 11:
            cell_name = "w_1_4";
            break;
        case 12:
            cell_name = "w_5_17";
            break;
        case 13:
            cell_name = "w_18_54";
            break;
        case 14:
            cell_name = "w_55_";
            break;
        case 15:
            cell_name = "w_sum";
            break;
        case 16:
            cell_name = "w_all";
            break;
        case 17:
            cell_name = "sum";
            break;
        case 18:
            cell_name = "all";
            break;
        case 19:
            cell_name = "snils_mt";
            break;
        default:
            cell_name = "?-?-?";
            break;
        }

        delete show_tab_w;
        QString sql = "select id_person, fam, im, ot, date_birth, sex, snils, pol_v, vs_num, enp, pol_ser, pol_num, assig_type, assig_date, code_mo, code_mp, code_ms, snils_mt, subj_name, dstr_name, city_name, nasp_name, strt_name, house, corp "
                      "  from assig.sexages_data d "
                      " where id_layer=" + QString::number(id_layer) + " "
                      "   and code_head='" + code_mo   + "' "
                      "   and cell_name='" + cell_name + "' " + assig_type_s +
                      " order by d.fam, d.im, d.ot, d.date_birth, d.sex, d.snils ; ";

        show_tab_w = new show_tab_wnd(QString("Список персон для ячейки " + cell_name + " по медорганизации (" + code_mo + ") " + name_mo),
                                      sql, db, data_app, this);
        show_tab_w->exec();
        this->setCursor(Qt::ArrowCursor);

    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Выберите ячейку, список персон для которой хотите просмотреть.");
    }
    ui->pan_wait->setVisible(false);
    ui->tab_sexage->setEnabled(true);
    QApplication::processEvents();
}
