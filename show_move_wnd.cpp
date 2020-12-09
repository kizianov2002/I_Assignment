#include "show_move_wnd.h"
#include "ui_show_move_wnd.h"
#include "ki_funcs.h"

show_move_wnd::show_move_wnd(int id_move, QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    id_move(id_move),
    assig_type(assig_type),
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::show_move_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    show_tab_w = NULL;

    ui->pan_wait->setVisible(false);

    refresh_combo_move();
    ui->combo_move->setCurrentIndex(ui->combo_move->findData(id_move));
    refresh_move_tab();
}

show_move_wnd::~show_move_wnd() {
    delete ui;
}

void show_move_wnd::on_bn_close_clicked() {
    close();
}

void show_move_wnd::refresh_combo_move() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка медорганизаций
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select m.id, (lo.layer_name||' - '||ln.layer_name||'  -  '||case m.assig_type when 1 then 'по терр.' when 2 then 'по заявл.' else '- все -' end) as move_name, "
                  "       ln.layer_date, ln.layer_type, m.assig_type "
                  "  from assig.moves m "
                  "  left join assig.layers lo on(lo.id=m.id_layer_old) "
                  "  left join assig.layers ln on(ln.id=m.id_layer_new) "
                  " order by ln.layer_date DESC, ln.layer_type, m.assig_type ; ";
    mySQL.exec(this, sql, "Список снимков данных прикреления", *query, true, db, data_app);
    ui->combo_move->clear();
    while (query->next()) {
        ui->combo_move->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void show_move_wnd::on_combo_move_activated(int index) {
    id_move = ui->combo_move->currentData().toInt();
    refresh_move_tab();
}

void show_move_wnd::on_bn_refresh_move_clicked() {
    refresh_move_tab();
}


void show_move_wnd::refresh_move_tab() {
    this->setCursor(Qt::WaitCursor);
    if (!db.isOpen()) {
        QMessageBox::warning(this, ("Нет доступа к базе данных"), ("Нет открытого соединения к базе данных?\n Это ненормальная ситуация и вы не должны были увидеть этого сообщения..(\n Перезапустите программу. Если это сообщение будет повторяться - сообщите разработчику."));
        return;
    }
    ui->lab_wait->setText("...  ПОДОЖДИТЕ, ИДЁТ ЧТЕНИЕ ДАННЫХ  ...");
    ui->pan_wait->setVisible(true);
    ui->tab_move->setEnabled(false);
    QApplication::processEvents();

    // получим данные выбранной таблицы
    QString title = "Движение застрахованных в разрезе МО:  " + ui->combo_move->currentText();
    this->setWindowTitle(title);

    sql_move_tab = "select ml.id, ml.code_head, mo.name_mo, "
                   "       ml.cnt_old, ml.cnt_dec, ml.cnt_add, ml.cnt_new "
                   "  from assig.moves_lines ml "
                   "  left join spr_medorgs mo on(mo.code_head=ml.code_head and mo.is_head=1) "
                   " where ml.id_move=" + QString::number(id_move) + " "
                   " order by code_mo ; ";

    model_move_tab.setQuery(sql_move_tab,db);
    QString err2 = model_move_tab.lastError().driverText();

    // подключаем модель из БД
    ui->tab_move->setModel(&model_move_tab);

    // обновляем таблицу
    ui->tab_move->reset();

    // задаём ширину колонок
    ui->tab_move->setColumnWidth(  0,  2);   // m.id,
    ui->tab_move->setColumnWidth(  1, 70);   // m.code_mo,
    ui->tab_move->setColumnWidth(  2,300);   // mo.name_mo,
    ui->tab_move->setColumnWidth(  3,100);   // m.cnt_old,
    ui->tab_move->setColumnWidth(  4,100);   // m.cnt_dec,
    ui->tab_move->setColumnWidth(  5,100);   // m.cnt_add,
    ui->tab_move->setColumnWidth(  6,100);   // m.cnt_new

    // правим заголовки
    model_move_tab.setHeaderData(  0, Qt::Horizontal, ("ID"));
    model_move_tab.setHeaderData(  1, Qt::Horizontal, ("код МО"));
    model_move_tab.setHeaderData(  2, Qt::Horizontal, ("Медицинская организация"));
    model_move_tab.setHeaderData(  3, Qt::Horizontal, ("было"));
    model_move_tab.setHeaderData(  4, Qt::Horizontal, ("откреплено"));
    model_move_tab.setHeaderData(  5, Qt::Horizontal, ("прикреплено"));
    model_move_tab.setHeaderData(  6, Qt::Horizontal, ("стало"));

    ui->tab_move->repaint();

    ui->pan_wait->setVisible(false);
    ui->tab_move->setEnabled(true);
    QApplication::processEvents();
    this->setCursor(Qt::ArrowCursor);
}

void show_move_wnd::on_bn_toCSV_clicked() {
    bool res_CSV = table2csv(db, sql_move_tab, model_move_tab, ui->tab_move, true,
                             data_app.path_out + "_ASSIG_DATA_/" + QDate::currentDate().toString("yyyy-MM-dd") + "  -  " + data_app.smo_short + " - " + data_app.filial_name + "  -  " +
                             "Движение прикреплённых застрахованных по снимку данны прикреления - " +
                             ui->combo_move->currentText() + " ",
                             this);
}

void show_move_wnd::on_tab_move_clicked(const QModelIndex &index) {
    int n_col = index.column();
    int n_row = index.row();
    QString capt = model_move_tab.data(model_move_tab.index(n_row,1), Qt::EditRole).toString();

    if ( n_col<3
         || capt.isEmpty() ) {
        ui->bn_show_list->setEnabled(false);
    } else {
        ui->bn_show_list->setEnabled(true);
    }
}

void show_move_wnd::on_bn_show_list_clicked() {
    ui->lab_wait->setText("...  ПОДОЖДИТЕ, ИДЁТ СОСТАВЛЕНИЕ СПИСКА  ...");
    ui->pan_wait->setVisible(true);
    ui->tab_move->setEnabled(false);
    QApplication::processEvents();

    // проверим выделенную строку
    QModelIndexList indexes = ui->tab_move->selectionModel()->selection().indexes();

    if (indexes.size()>0) {
        this->setCursor(Qt::WaitCursor);
        QModelIndex index = indexes.front();

        QString assig_type_s = " ";
        bool f_assig_type = false;
        switch (assig_type) {
        case 1:
            assig_type_s = " and d.assig_type=1 ";
            f_assig_type = true;
            break;
        case 2:
            assig_type_s = " and d.assig_type=2 ";
            f_assig_type = true;
            break;
        default:
            assig_type_s = " ";
            f_assig_type = false;
            break;
        }

        QString assig_type_move_s = " ";
        bool f_assig_type_move = false;
        switch (assig_type) {
        case 1:
            assig_type_move_s = " and d.assig_type_new=1 ";
            f_assig_type_move = true;
            break;
        case 2:
            assig_type_move_s = " and d.assig_type_new=2 ";
            f_assig_type_move = true;
            break;
        default:
            assig_type_move_s = " ";
            f_assig_type_move = false;
            break;
        }

        int n_col = index.column();
        int n_row = index.row();
        QString capt = model_move_tab.data(model_move_tab.index(n_row,1), Qt::EditRole).toString();

        if ( n_col<3
             || capt.isEmpty() ) {
            ui->pan_wait->setVisible(false);
            ui->tab_move->setEnabled(true);
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
            cell_name = "cnt_old";
            break;
        case 4:
            cell_name = "cnt_dec";
            break;
        case 5:
            cell_name = "cnt_add";
            break;
        case 6:
            cell_name = "cnt_new";
            break;
        default:
            cell_name = "?-?-?";
            break;
        }
        int id_layer_move = model_move_tab.data(model_move_tab.index(n_row,0), Qt::EditRole).toInt();
        QString code_mo = model_move_tab.data(model_move_tab.index(n_row,1), Qt::EditRole).toString();
        QString name_mo = model_move_tab.data(model_move_tab.index(n_row,2), Qt::EditRole).toString();

        QString sql;
        if (n_col==3) {
            sql = "select d.id_person, d.fam, d.im, d.ot, d.date_birth, d.sex, d.snils, d.pol_v, d.vs_num, d.enp, d.pol_ser, d.pol_num, d.assig_type, d.assig_date, d.code_mo, d.code_mp, d.code_ms, d.snils_mt, d.subj_name, d.dstr_name, d.city_name, d.nasp_name, d.strt_name, d.house, d.corp "
                  "  from assig.moves_lines m "
                  "  left join assig.sexages_data d on(d.id_layer=m.id_layer_old) "
                  " where m.id=" + QString::number(id_layer_move) + " "
                  "   and d.code_head='" + code_mo + "' "
                  "   and ( (m.assig_type in(1,2) and d.assig_type=m.assig_type) "
                  "          or (m.assig_type is NULL) "
                  "          or (m.assig_type not in(1,2)) ) " + assig_type_s +
                  " order by d.fam, d.im, d.ot, d.date_birth, d.sex, d.snils ; ";
        } else if (n_col==6) {
            sql = "select d.id_person, d.fam, d.im, d.ot, d.date_birth, d.sex, d.snils, d.pol_v, d.vs_num, d.enp, d.pol_ser, d.pol_num, d.assig_type, d.assig_date, d.code_mo, d.code_mp, d.code_ms, d.snils_mt, d.subj_name, d.dstr_name, d.city_name, d.nasp_name, d.strt_name, d.house, d.corp "
                  "  from assig.moves_lines m "
                  "  left join assig.sexages_data d on(d.id_layer=m.id_layer_new) "
                  " where m.id=" + QString::number(id_layer_move) + " "
                  "   and d.code_head='" + code_mo + "' "
                  "   and ( (m.assig_type in(1,2) and d.assig_type=m.assig_type) "
                  "          or (m.assig_type is NULL) "
                  "          or (m.assig_type not in(1,2)) ) " + assig_type_s +
                  " order by d.fam, d.im, d.ot, d.date_birth, d.sex, d.snils ; ";
        } else {
            sql = "select id_person, fam, im, ot, date_birth, sex, snils, pol_v, vs_num, enp, pol_ser, pol_num, "
                  "       assig_type_old, assig_date_old, code_mo_old, code_mp_old, code_ms_old, snils_mt_old, "
                  "       assig_type_new, assig_date_new, code_mo_new, code_mp_new, code_ms_new, snils_mt_new, "
                  "       subj_name, dstr_name, city_name, nasp_name, strt_name, house, corp, quart "
                  "  from assig.moves_data d "
                  " where d.id_move_line=" + QString::number(id_layer_move) + " "
                  "   and cell_name='" + cell_name + "' " + assig_type_move_s +
                  " order by d.fam, d.im, d.ot, d.date_birth, d.sex, d.snils ; ";
        }

        delete show_tab_w;
        show_tab_w = new show_tab_wnd(QString("Список персон для ячейки " + cell_name + " по медорганизации (" + code_mo + ") " + name_mo),
                                      sql, db, data_app, this);
        this->setCursor(Qt::ArrowCursor);
        show_tab_w->exec();

    } else {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Выберите ячейку, список персон для которой хотите просмотреть.");
    }
    ui->pan_wait->setVisible(false);
    ui->tab_move->setEnabled(true);
    QApplication::processEvents();
}
