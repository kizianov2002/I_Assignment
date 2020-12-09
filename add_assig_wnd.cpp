#include "add_assig_wnd.h"
#include <QSqlError>
#include <QMessageBox>
#include <QInputDialog>
#include "ui_add_assig_wnd.h"

add_assig_wnd::add_assig_wnd(QSqlDatabase &db, QSqlDatabase &db_FIAS, s_data_app &data_app, s_data_assig &data_assig, QSettings &settings, QTextEdit *te_log, QWidget *parent)
    : db(db), db_FIAS(db_FIAS), data_app(data_app), data_assig(data_assig), settings(settings), te_log(te_log), QDialog(parent), ui(new Ui::add_assig_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

    update_data();

    log = "";

    if (data_assig.id>0) {
        ui->bn_add->setEnabled(true);
        ui->bn_add->setVisible(true);
        ui->bn_ok->setText("Изменить");
    } else {
        ui->bn_add->setEnabled(false);
        ui->bn_add->setVisible(false);
        ui->bn_ok->setText("Добавить");
    }

    ui->ch_from_start->setChecked(data_assig.from_start);
    ui->ch_to_end->setChecked(data_assig.to_end);
}

add_assig_wnd::~add_assig_wnd() {
    delete ui;
}

void add_assig_wnd::update_data() {
    if (data_assig.id<0) {
        this->setWindowTitle("Новое правило терр-прикрепления");
        ui->rb_one_house->setEnabled(true);
        ui->rb_line_houses->setEnabled(true);
        ui->rb_all_street->setEnabled(true);
        ui->rb_open_line->setEnabled(true);
        ui->rb_numlist->setEnabled(true);
    } else {
        this->setWindowTitle("Существующее правило терр-прикрепления");
        ui->rb_one_house->setEnabled(true);
        ui->rb_line_houses->setEnabled(false);
        ui->rb_all_street->setEnabled(false);
        ui->rb_open_line->setEnabled(true);
        ui->rb_numlist->setEnabled(false);
    }
    refresh_subj_terr();
    ui->combo_subj_terrs->setCurrentIndex(ui->combo_subj_terrs->findData(data_assig.subj));

    refresh_dstr_terr();
    ui->combo_dstr_terrs->setCurrentIndex(ui->combo_dstr_terrs->findData(data_assig.dstr));

    refresh_city_terr();
    ui->combo_city_terrs->setCurrentIndex(ui->combo_city_terrs->findData(data_assig.city));

    refresh_nasp_terr();
    ui->combo_nasp_terrs->setCurrentIndex(ui->combo_nasp_terrs->findData(data_assig.nasp));

    refresh_strt_terr();
    ui->combo_strt_terrs->setCurrentIndex(ui->combo_strt_terrs->findData(data_assig.strt));

    switch (data_assig.right_type) {
    case 1:
        ui->rb_one_house->setChecked(true);
        break;
    case 2:
        ui->rb_line_houses->setChecked(true);
        break;
    case 3:
        ui->rb_all_street->setChecked(true);
        break;
    case 4:
        ui->rb_open_line->setChecked(true);
        ui->ch_from_start->setChecked(data_assig.from_start);
        ui->ch_to_end->setChecked(data_assig.to_end);
        break;
    case 5:
        ui->rb_numlist->setChecked(true);
        break;
    }
    update_right_type();

    //QMessageBox::warning(this, "+","h="+QString::number(data_assig.house)+" c="+data_assig.corp+" h2="+QString::number(data_assig.house2)+" c2="+data_assig.corp2);

    ui->spin_house->setValue(data_assig.house);
    ui->spin_house2->setValue(data_assig.house2);
    ui->line_corp->setText(data_assig.corp.trimmed().simplified().toUpper());
    ui->line_corp2->setText(data_assig.corp2.trimmed().simplified().toUpper());
    ui->combo_side->setCurrentIndex(0);

    ui->combo_sex->setCurrentIndex(data_assig.sex);
    ui->combo_age->setCurrentIndex(data_assig.age);

    refresh_mo();
    ui->combo_mo->setCurrentIndex(ui->combo_mo->findData(data_assig.code_mo));

    //on_combo_mo_currentIndexChanged(ui->combo_mo->findData(data_assig.code_mo));
    /*refresh_mp();
    ui->combo_mp->setCurrentIndex(ui->combo_mp->findData(data_assig.code_mp));
    refresh_ms();
    ui->combo_ms->setCurrentIndex(ui->combo_ms->findData(data_assig.code_ms));
    refresh_mt();
    ui->combo_mt->setCurrentIndex(ui->combo_mt->findData(data_assig.snils_mt));*/

    ui->line_comment->setText(data_assig.comment);
}

void add_assig_wnd::update_right_type() {
    if (ui->rb_one_house->isChecked()) {
        ui->pan_first_house->setVisible(true);
        ui->pan_last_house->setVisible(false);
        ui->pan_open_line->setVisible(false);
        ui->combo_side->setVisible(false);
        ui->line_numlist->setVisible(false);

        ui->ch_from_start->setChecked(false);
        ui->ch_to_end->setChecked(false);

        ui->pan_first_house->setTitle("Номер дома");
    }

    if (ui->rb_line_houses->isChecked()) {
        ui->pan_first_house->setVisible(true);
        ui->pan_last_house->setVisible(true);
        ui->pan_open_line->setVisible(false);
        ui->combo_side->setVisible(true);
        ui->line_numlist->setVisible(false);

        ui->ch_from_start->setChecked(false);
        ui->ch_to_end->setChecked(false);

        ui->pan_first_house->setTitle("Начало диапазона");
    }

    if (ui->rb_all_street->isChecked()) {
        ui->pan_first_house->setVisible(false);
        ui->pan_last_house->setVisible(false);
        ui->pan_open_line->setVisible(false);
        ui->combo_side->setVisible(false);
        ui->line_numlist->setVisible(false);

        ui->ch_from_start->setChecked(false);
        ui->ch_to_end->setChecked(false);

        ui->pan_first_house->setTitle("Начало диапазона");
    }

    if (ui->rb_open_line->isChecked()) {
        ui->pan_first_house->setVisible(true);
        ui->pan_last_house->setVisible(false);
        ui->pan_open_line->setVisible(true);
        ui->combo_side->setVisible(false);
        ui->line_numlist->setVisible(false);

        ui->ch_from_start->setChecked(false);
        ui->ch_to_end->setChecked(false);

        ui->pan_first_house->setTitle("Крайний дом");
    }

    if (ui->rb_numlist->isChecked()) {
        ui->pan_first_house->setVisible(false);
        ui->pan_last_house->setVisible(false);
        ui->pan_open_line->setVisible(false);
        ui->combo_side->setVisible(false);
        ui->line_numlist->setVisible(true);

        ui->ch_from_start->setChecked(false);
        ui->ch_to_end->setChecked(false);

        ui->pan_first_house->setTitle("Крайний дом");
    }
}

void add_assig_wnd::on_rb_one_house_clicked() {
    update_right_type();
}

void add_assig_wnd::on_rb_line_houses_clicked() {
    update_right_type();
}

void add_assig_wnd::on_rb_all_street_clicked() {
    update_right_type();
}

void add_assig_wnd::on_rb_open_line_clicked() {
    update_right_type();
}

void add_assig_wnd::on_rb_numlist_clicked() {
    update_right_type();
}

void add_assig_wnd::refresh_subj_terr() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка регионов
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select subj, offname||' '||shortname "
                  "  from fias.subj "
                  " where aolevel=1 "
                  "   and actstatus=1 "
                  " order by offname||' '||shortname ; ";
    query->exec(sql);

    ui->combo_subj_terrs->clear();
    ui->combo_subj_terrs->addItem(" - все регионы - ", 0);
    while (query->next()) {
        ui->combo_subj_terrs->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    delete query;

    if (data_app.KLADR_subj==0) {
        ui->combo_subj_terrs->setCurrentIndex(0);
    } else {
        ui->combo_subj_terrs->setCurrentIndex(ui->combo_subj_terrs->findData(data_app.KLADR_subj, Qt::UserRole));
    }

    this->setCursor(Qt::ArrowCursor);
}

void add_assig_wnd::refresh_dstr_terr() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка районов выбранного региона
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select dstr, offname||' '||shortname "
                  "  from fias.dstr "
                  " where subj=" + QString::number(ui->combo_subj_terrs->currentData().toInt()) + " "
                  "   and aolevel=3 "
                  "   and actstatus=1 "
                  " order by offname||' '||shortname ";
    query->exec(sql);

    ui->combo_dstr_terrs->clear();
    ui->combo_dstr_terrs->addItem(" - все районы - ", 0);
    while (query->next()) {
        ui->combo_dstr_terrs->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    ui->combo_dstr_terrs->setCurrentIndex(ui->combo_dstr_terrs->findData(data_app.KLADR_dstr, Qt::UserRole));
    delete query;

    if (data_app.KLADR_dstr==0) {
        ui->combo_dstr_terrs->setCurrentIndex(0);
    } else {
        ui->combo_dstr_terrs->setCurrentIndex(ui->combo_dstr_terrs->findData(data_app.KLADR_dstr, Qt::UserRole));
    }

    this->setCursor(Qt::ArrowCursor);
}

void add_assig_wnd::refresh_city_terr() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка городов
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select city, offname||' '||shortname "
                  "  from fias.city "
                  " where subj=" + QString::number(ui->combo_subj_terrs->currentData().toInt()) + " "
                  "   and dstr=" + QString::number(ui->combo_dstr_terrs->currentData().toInt()) + " "
                  "   and aolevel=4 "
                  "   and actstatus=1 "
                  " order by offname||' '||shortname ";
    query->exec(sql);

    ui->combo_city_terrs->clear();
    ui->combo_city_terrs->addItem(" - все города - ", 0);
    while (query->next()) {
        ui->combo_city_terrs->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    ui->combo_city_terrs->setCurrentIndex(ui->combo_city_terrs->findData(data_app.KLADR_city, Qt::UserRole));
    delete query;

    if (data_app.KLADR_city==0) {
        ui->combo_city_terrs->setCurrentIndex(0);
    } else {
        ui->combo_city_terrs->setCurrentIndex(ui->combo_city_terrs->findData(data_app.KLADR_city, Qt::UserRole));
    }

    this->setCursor(Qt::ArrowCursor);
}

void add_assig_wnd::refresh_nasp_terr() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка населенных пунктов
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select nasp, offname||' '||shortname "
                  "  from fias.nasp "
                  " where subj=" + QString::number(ui->combo_subj_terrs->currentData().toInt()) + " "
                  "   and dstr=" + QString::number(ui->combo_dstr_terrs->currentData().toInt()) + " "
                  "   and city=" + QString::number(ui->combo_city_terrs->currentData().toInt()) + " "
                  "   and aolevel=6 "
                  "   and actstatus=1 "
                  " order by offname||' '||shortname ";
    query->exec(sql);

    ui->combo_nasp_terrs->clear();
    ui->combo_nasp_terrs->addItem(" - все нас.пункты - ", 0);
    while (query->next()) {
        ui->combo_nasp_terrs->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    ui->combo_nasp_terrs->setCurrentIndex(ui->combo_nasp_terrs->findData(data_app.KLADR_nasp, Qt::UserRole));
    delete query;

    if (data_app.KLADR_nasp==0) {
        ui->combo_nasp_terrs->setCurrentIndex(0);
    } else {
        ui->combo_nasp_terrs->setCurrentIndex(ui->combo_nasp_terrs->findData(data_app.KLADR_nasp, Qt::UserRole));
    }

    this->setCursor(Qt::ArrowCursor);
}

void add_assig_wnd::refresh_strt_terr() {
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка улиц
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select strt, offname||' '||shortname "
                  "  from fias.strt "
                  " where subj=" + QString::number(ui->combo_subj_terrs->currentData().toInt()) + " "
                  "   and dstr=" + QString::number(ui->combo_dstr_terrs->currentData().toInt()) + " "
                  "   and city=" + QString::number(ui->combo_city_terrs->currentData().toInt()) + " "
                  "   and nasp=" + QString::number(ui->combo_nasp_terrs->currentData().toInt()) + " "
                  "   and aolevel=7 "
                  "   and actstatus=1 "
                  " order by offname||' '||shortname ";
    query->exec(sql);

    ui->combo_strt_terrs->clear();
    ui->combo_strt_terrs->addItem(" - все улицы - ", 0);
    while (query->next()) {
        ui->combo_strt_terrs->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    ui->combo_strt_terrs->setCurrentIndex(ui->combo_strt_terrs->findData(data_app.KLADR_strt, Qt::UserRole));
    delete query;

    if (data_app.KLADR_strt==0) {
        ui->combo_strt_terrs->setCurrentIndex(0);
    } else {
        ui->combo_strt_terrs->setCurrentIndex(ui->combo_strt_terrs->findData(data_app.KLADR_strt, Qt::UserRole));
    }

    this->setCursor(Qt::ArrowCursor);
}





void add_assig_wnd::refresh_mo() {
    this->setCursor(Qt::WaitCursor);

    // обновление выпадающего списка медорганизаций
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mo, '(' || code_mo || ') ' || name_mo "
                  "  from spr_medorgs s "
                  " where is_head=1 "
                  " order by code_mo ; ";
    //QInputDialog::getText(this, "refresh_mo", "refresh_mo", QLineEdit::Normal, sql);

    mySQL.exec(this, sql, QString("Справочник медорганизаций для окна правки правила прикрепления"), *query, true, db, data_app);
    ui->combo_mo->clear();
    while (query->next()) {
        ui->combo_mo->addItem(query->value(1).toString(), query->value(0).toString());
    }
    //ui->combo_mo->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void add_assig_wnd::refresh_mp() {
    this->setCursor(Qt::WaitCursor);

    // обновление выпадающего списка подразделений
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_mp, '(' || code_mp || ') ' || name_mp "
                  "  from spr_medparts u "
                  " where u.code_mo='" + ui->combo_mo->currentData().toString() + "' "
                  " order by code_mp ; ";
    //QInputDialog::getText(this, "refresh_mp", "refresh_mp", QLineEdit::Normal, sql);

    mySQL.exec(this, sql, QString("Справочник подразделений для окна правки правила прикрепления"), *query, true, db, data_app);
    ui->combo_mp->clear();
    while (query->next()) {
        ui->combo_mp->addItem(query->value(1).toString(), query->value(0).toString());
    }
    //ui->combo_mp->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void add_assig_wnd::refresh_ms() {
    this->setCursor(Qt::WaitCursor);

    // обновление выпадающего списка подразделений
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select code_ms, '(' || code_ms || ') ' || name_ms "
                  "  from spr_medsites u "
                  " where u.code_mo='" + ui->combo_mo->currentData().toString() + "' ";
    if (ui->combo_mp->currentIndex()>0)
        sql +=    "       and u.code_mp='" + ui->combo_mp->currentData().toString() + "' "
                  " order by code_ms ; ";
    //QInputDialog::getText(this, "refresh_ms", "refresh_ms", QLineEdit::Normal, sql);

    mySQL.exec(this, sql, QString("Справочник участков для окна правки правила прикрепления"), *query, true, db, data_app);
    ui->combo_ms->clear();
    while (query->next()) {
        ui->combo_ms->addItem(query->value(1).toString(), query->value(0).toString());
    }
    //ui->combo_ms->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

void add_assig_wnd::refresh_mt() {
    this->setCursor(Qt::WaitCursor);

    // обновление выпадающего списка врачей
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select snils_mt, '('||coalesce(snils_mt,' - ')||')  '||coalesce(fio_mt) "
                  "  from spr_medters u "
                  " where u.code_mo='" + ui->combo_mo->currentData().toString() + "' "
                  " order by fio_mt ; ";

    mySQL.exec(this, sql, QString("Справочник врачей для окна правки правила прикрепления"), *query, true, db, data_app);
    ui->combo_mt->clear();
    ui->combo_mt->addItem(" - нет - ", "");
    while (query->next()) {
        ui->combo_mt->addItem(query->value(1).toString(), query->value(0).toString());
    }
    //ui->combo_mt->setCurrentIndex(-1);
    delete query;
    this->setCursor(Qt::ArrowCursor);
}

/*void add_assig_wnd::on_combo_mo_activated(int index) {
    refresh_mp();
}
void add_assig_wnd::on_combo_mp_activated(int index) {
    refresh_ms();
    refresh_mt();
}
void add_assig_wnd::on_combo_ms_activated(int index) {
    refresh_mt();
}
void add_assig_wnd::on_combo_mt_activated(int index) {
    // ???
}*/

void add_assig_wnd::on_combo_subj_terrs_activated(int index) {
    refresh_dstr_terr();
    refresh_city_terr();
    refresh_nasp_terr();
    refresh_strt_terr();
}
void add_assig_wnd::on_combo_dstr_terrs_activated(int index) {
    refresh_city_terr();
    refresh_nasp_terr();
    refresh_strt_terr();
}
void add_assig_wnd::on_combo_city_terrs_activated(int index) {
    refresh_nasp_terr();
    refresh_strt_terr();
}
void add_assig_wnd::on_combo_nasp_terrs_activated(int index) {
    refresh_strt_terr();
}
void add_assig_wnd::on_combo_strt_terrs_activated(int index) {
    // ???
}

void add_assig_wnd::on_buttonBox_accepted() {
}

bool add_assig_wnd::try_house (QString ocato, int subj, int dstr, int city, int nasp, int strt, int house, QString corp, int sex, int age, QString code_mo, QString code_mp, QString code_ms, QString snils_mt, bool from_start, bool to_end) {
    // проверим, занят ли дом
    QString sql_test =  " ( select id, code_mo, code_mp, code_ms, snils_mt, sex, age, subj, dstr, city, nasp, strt, house, corp, ocato, from_start, to_end "
                        "    from assig_by_terr a "
                        "    where ( (subj=" + QString::number(subj>0 ? subj : 0)  + ") or (subj  is NULL and " + QString::number(subj) + "<=0) ) and "
                        "          ( (dstr=" + QString::number(dstr>0 ? dstr : 0)  + ") or (dstr  is NULL and " + QString::number(dstr) + "<=0) ) and "
                        "          ( (city=" + QString::number(city>0 ? city : 0)  + ") or (city  is NULL and " + QString::number(city) + "<=0) ) and "
                        "          ( (nasp=" + QString::number(nasp>0 ? nasp : 0)  + ") or (nasp  is NULL and " + QString::number(nasp) + "<=0) ) and "
                        "          ( (strt=" + QString::number(strt>0 ? strt : 0)  + ") or (strt  is NULL and " + QString::number(strt) + "<=0) ) and "
                        "          ( (house="+ QString::number(house>0? house: 0)  + ") or (house is NULL and " + QString::number(house)+ "<=0) ) and "
                        "          ( (corp"  + ( corp.isEmpty() ? " is NULL" : ("='"+corp+"'") ) + ") ) and "
                        "          ( (sex="  + QString::number(sex>0  ? sex  : 0)  + ") or (sex  is NULL  and " + QString::number(sex)  + "<=0) ) and "
                        "          ( (age="  + QString::number(age>0  ? age  : 0)  + ") or (age  is NULL  and " + QString::number(age)  + "<=0) ) ) "
                        " UNION "
                        " ( select id, code_mo, code_mp, code_ms, snils_mt, sex, age, subj, dstr, city, nasp, strt, house, corp, ocato, from_start, to_end "
                        "    from assig_by_terr a "
                        "    where ( (subj=" + QString::number(subj>0 ? subj : 0)  + ") or (subj is NULL and " + QString::number(subj) + "<=0) ) and "
                        "          ( (dstr=" + QString::number(dstr>0 ? dstr : 0)  + ") or (dstr is NULL and " + QString::number(dstr) + "<=0) ) and "
                        "          ( (city=" + QString::number(city>0 ? city : 0)  + ") or (city is NULL and " + QString::number(city) + "<=0) ) and "
                        "          ( (nasp=" + QString::number(nasp>0 ? nasp : 0)  + ") or (nasp is NULL and " + QString::number(nasp) + "<=0) ) and "
                        "          ( (strt=" + QString::number(strt>0 ? strt : 0)  + ") or (strt is NULL and " + QString::number(strt) + "<=0) ) and "
                        "          ( (sex="  + QString::number(sex>0  ? sex  : 0)  + ") or (sex  is NULL and " + QString::number(sex)  + "<=0) ) and "
                        "          ( (age="  + QString::number(age>0  ? age  : 0)  + ") or (age  is NULL and " + QString::number(age)  + "<=0) ) and "
                        "          ( (from_start=1 and " + (from_start ? "1" : "0") + "=1 and house<=" + QString::number(house>0? house: 0) + " ) or "
                        "            (to_end    =1 and " + (to_end     ? "1" : "0") + "=1 and house>=" + QString::number(house>0? house: 0) + " ) ) ) ; ";

    QSqlQuery *query_test = new QSqlQuery(db);
    if (!mySQL.exec(this, sql_test, "Проверка на пересечение с существующими правилами", *query_test, true, db, data_app)) {
        QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке проверить новую запись на пересечение с существующими правилами произошла неожиданная ошибка.\n\nОперация отменена.");
        delete query_test;

        db.rollback();
        return false;
    } else {
    }

    if (query_test->size()>0) {

        while (query_test->next()) {

            int id2 = query_test->value(0).toInt();            
            del_house(id2);

            /*QString code_mo2 = query_test->value(1).toString();
            QString code_mp2 = query_test->value(2).toString();
            QString code_ms2 = query_test->value(3).toString();
            QString snils_mt2= query_test->value(4).toString();
            int sex2  = query_test->value(5).toInt();
            int age2  = query_test->value(6).toInt();
            int subj2 = query_test->value(7).toInt();
            int dstr2 = query_test->value(8).toInt();
            int city2 = query_test->value(9).toInt();
            int nasp2 = query_test->value(10).toInt();
            int strt2 = query_test->value(11).toInt();
            int house2= query_test->value(12).toInt();
            QString corp2 = query_test->value(13).toString();
            QString ocato2 = query_test->value(14).toString();
            bool from_start2 = (query_test->value(15).toInt()>0);
            bool to_end2     = (query_test->value(16).toInt()>0);*/

            /*if (sex==0) {
                // проверка по age
                if (age==0) {
                    // новый диапазон полностью перекрывает старый - удалим старый
                    del_house(id2);
                    return add_house(ocato, subj, dstr, city, nasp, strt, house, corp, sex, age, code_mo, code_mp, code_ms, snils_mt, from_start, to_end);
                } else if (age==age2) {
                    // новый диапазон полностью перекрывает старый - удалим старый
                    del_house(id2);
                    return add_house(ocato, subj, dstr, city, nasp, strt, house, corp, sex, age, code_mo, code_mp, code_ms, snils_mt, from_start, to_end);
                } else if (age!=age2) {
                    // разные возрастные категории - не пересекаются
                    // проверка по age
                    if (age2==0) {
                        // старый диапазон шире нового - расщепим старый
                        del_house(id2);
                        add_house(ocato2, subj2, dstr2, city2, nasp2, strt2, house2, corp2, sex2, (age==1?2:1), code_mo2, code_mp2, code_ms2, snils_mt2, from_start2, to_end2);
                    } else if (age==age2) {
                        // разные возрастные категории - не пересекаются, ничего не делаем
                    } else if (age!=age2) {
                        // разные возрастные категории - не пересекаются, ничего не делаем
                    }
                    return add_house(ocato, subj, dstr, city, nasp, strt, house, corp, sex, age, code_mo, code_mp, code_ms, snils_mt, from_start, to_end);
                }
            } else if (sex==sex2) {
                // проверка по age
                if (age==0) {
                    // новый диапазон полностью перекрывает старый - удалим старый
                    del_house(id2);
                    return add_house(ocato, subj, dstr, city, nasp, strt, house, corp, sex, age, code_mo, code_mp, code_ms, snils_mt, from_start, to_end);
                } else if (age==age2) {
                    // новый диапазон полностью перекрывает старый - удалим старый
                    del_house(id2);
                    return add_house(ocato, subj, dstr, city, nasp, strt, house, corp, sex, age, code_mo, code_mp, code_ms, snils_mt, from_start, to_end);
                } else if (age!=age2) {
                    // разные возрастные категории - не пересекаются
                    // проверка по age
                    if (age2==0) {
                        // старый диапазон шире нового - расщепим старый
                        del_house(id2);
                        add_house(ocato2, subj2, dstr2, city2, nasp2, strt2, house2, corp2, sex2, (age==1?2:1), code_mo2, code_mp2, code_ms2, snils_mt2, from_start2, to_end2);
                    } else if (age==age2) {
                        // разные возрастные категории - не пересекаются, ничего не делаем
                    } else if (age!=age2) {
                        // разные возрастные категории - не пересекаются, ничего не делаем
                    }
                    return add_house(ocato, subj, dstr, city, nasp, strt, house, corp, sex, age, code_mo, code_mp, code_ms, snils_mt, from_start, to_end);
                }
            } else if (sex!=sex2) {
                if (sex2==0) {
                    // старый диапазон шире
                    // проверка по age
                    if (age2==0) {
                        // старый диапазон шире нового - расщепим старый
                        del_house(id2);
                        add_house(ocato2, subj2, dstr2, city2, nasp2, strt2, house2, corp2, (sex==1?2:1), age2, code_mo2, code_mp2, code_ms2, snils_mt2, from_start2, to_end2);
                        return add_house(ocato, subj, dstr, city, nasp, strt, house, corp, sex, age, code_mo, code_mp, code_ms, snils_mt, from_start, to_end);
                    } else if (age==age2) {
                        // старый диапазон шире нового - расщепим старый
                        del_house(id2);
                        add_house(ocato2, subj2, dstr2, city2, nasp2, strt2, house2, corp2, (sex==1?2:1), age2, code_mo2, code_mp2, code_ms2, snils_mt2, from_start2, to_end2);
                        return add_house(ocato, subj, dstr, city, nasp, strt, house, corp, sex, age, code_mo, code_mp, code_ms, snils_mt, from_start, to_end);
                    } else if (age!=age2) {
                        // разные возрастные категории - не пересекаются, ничего не делаем
                    }
                    return add_house(ocato, subj, dstr, city, nasp, strt, house, corp, sex, age, code_mo, code_mp, code_ms, snils_mt, from_start, to_end);
                } else {
                    // разные возрастные категории - не пересекаются, ничего не делаем
                }
            }*/



            /*  if (age==0 || age==age2) {
                    // новый диапазон полностью перекрывает старый - удалим старый
                    del_house(id2);
                }

            } else if ( (sex!=0 && sex2==0) || (age2==0 && age!=0) ) {
                // новый диапазон частично перекрывает существующий - сузим старый
                // ... удалим старый
                del_house(id2);

                // ... добавим для взрослых
                if ((age2==0 || age2==2) && age==1) {
                    add_house(ocato2, subj2, dstr2, city2, nasp2, strt2, house2, corp2, sex2, 2, code_mo2, code_mp2, code_ms2, snils_mt2);
                }
                // ... добавим для детей
                if ((age2==0 || age2==1) && age==2) {
                    add_house(ocato2, subj2, dstr2, city2, nasp2, strt2, house2, corp2, sex2, 1, code_mo2, code_mp2, code_ms2, snils_mt2);
                }

                te_log->append(" - разбивка старой записи ");
                QApplication::processEvents();

            } else if ( (sex!=sex2) && (age==0 || age==age2) ) {
                // новый диапазон полностью перекрывает старый - удалим старый
                del_house(id2);

            }*/


        }
    }
    delete query_test;

    // просто добавим запись
    return add_house(ocato, subj, dstr, city, nasp, strt, house, corp, sex, age, code_mo, code_mp, code_ms, snils_mt, from_start, to_end);
}


bool add_assig_wnd::add_house (QString ocato, int subj, int dstr, int city, int nasp, int strt, int house, QString corp, int sex, int age, QString code_mo, QString code_mp, QString code_ms, QString snils_mt, bool from_start, bool to_end) {
    // добавим запись дома
    QString sql_add = "insert into assig_by_terr (ocato, subj, dstr, city, nasp, strt, house, corp, sex, age, code_mo, code_mp, code_ms, snils_mt, from_start, to_end, comment, id_point, id_operator, dt_ins) "
                      " values('" + ocato + "', "
                              " " + QString::number(subj>0 ? subj : 0) + ", "
                              " " + QString::number(dstr>0 ? dstr : 0) + ", "
                              " " + QString::number(city>0 ? city : 0) + ", "
                              " " + QString::number(nasp>0 ? nasp : 0) + ", "
                              " " + QString::number(strt>0 ? strt : 0) + ", "
                              " " + (house>0 ? QString::number(house) : "NULL") + ", "
                              " " + (corp.isEmpty() ? "NULL" : ("'"+corp+"'")) + ", "
                              " " + QString::number(sex>0 ? sex : 0) + ", "
                              " " + QString::number(age>0 ? age : 0) + ", "
                              " " + (code_mo.isEmpty() ? "NULL" : ("'"+code_mo+"'")) + ", "
                              " " + (code_mp.isEmpty() ? "NULL" : ("'"+code_mp+"'")) + ", "
                              " " + (code_ms.isEmpty() ? "NULL" : ("'"+code_ms+"'")) + ", "
                              " " + (snils_mt.isEmpty()? "NULL" : ("'"+snils_mt+"'"))+ ", "
                              " " + (from_start ? "1" : "0") + ", "
                              " " + (to_end     ? "1" : "0") + ", "
                              "'" + ui->line_comment->text() + "', "
                              " " + QString::number(data_app.id_point) + ", "
                              " " + QString::number(data_app.id_operator) + ", "
                              " " + "CURRENT_TIMESTAMP) ; ";

    QSqlQuery *query_add = new QSqlQuery(db);
    if (!mySQL.exec(this, sql_add, "Добавление нового правила терр-прикрепления", *query_add, true, db, data_app)) {
        QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новое правило терр-прикрепления произошла неожиданная ошибка.\n\nОперация отменена.");
        delete query_add;

        db.rollback();
        return false;
    } else {
        delete query_add;
    }
    te_log->append(" + " + QString::number(house) + (corp.isEmpty() ? "" : "/") + corp + ";  " + QString(sex==0 ? "М/Ж" : (sex==1 ? "М" : "Ж")) + ";  " + QString(age==0 ? "все" : (age==1 ? "дет." : "взр.")) );
    QApplication::processEvents();
}


bool add_assig_wnd::del_house (int id) {
    QString sql_del = "delete from assig_by_terr where id=" + QString::number(id) + " ; ";

    QSqlQuery *query_del = new QSqlQuery(db);
    if (!mySQL.exec(this, sql_del, "Удаление старого правила терр-прикрепления", *query_del, true, db, data_app)) {
        QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке удалить старое правило терр-прикрепления произошла неожиданная ошибка.\n\nОперация отменена.");
        delete query_del;

        te_log->append(" - удаление старой записи ");
        QApplication::processEvents();

        //db.rollback();
        return false;
    } else {
        delete query_del;
    }
    te_log->append(" - ID " + QString::number(id) + " ");
    QApplication::processEvents();

    return true;
}

bool add_assig_wnd::del_house (QString ocato, int subj, int dstr, int city, int nasp, int strt, int house, QString corp, int sex, int age, QString code_mo, QString code_mp, QString code_ms, QString snils_mt, bool from_start, bool to_end) {
    db.transaction();

    QString sql_del = "delete from assig_by_terr "
                      " where subj=" + QString::number(subj>0 ? subj : 0)  + " and "
                      "       dstr=" + QString::number(dstr>0 ? dstr : 0)  + " and "
                      "       city=" + QString::number(city>0 ? city : 0)  + " and "
                      "       nasp=" + QString::number(nasp>0 ? nasp : 0)  + " and "
                      "       strt=" + QString::number(strt>0 ? strt : 0)  + " and "
                      "       house="+ QString::number(house>0? house: 0)  + " and "
                      "       corp"  + ( corp.isEmpty() ? " is NULL" : ("='"+corp+"'") ) + " and "
                      "       sex="+ QString::number(sex>0? sex: 0)  + " and "
                      "       age="+ QString::number(age>0? age: 0)  + " and "
                      "       code_mo"  + ( code_mo.isEmpty() ? " is NULL" : ("='"+code_mo+"'") ) + " and "
                      "       code_mp"  + ( code_mp.isEmpty() ? " is NULL" : ("='"+code_mp+"'") ) + " and "
                      "       code_ms"  + ( code_ms.isEmpty() ? " is NULL" : ("='"+code_ms+"'") ) + " and "
                      "       snils_mt" + ( snils_mt.isEmpty()? " is NULL" : ("='"+snils_mt+"'") )+ " and "
                      "       " + (from_start ? "from_start=1" : " (from_start is NULL or from_start=0) ") + " and "
                      "       " + (to_end     ? "to_end=1"     : " (to_end is NULL or to_end=0) "        ) + "; ";

    //QInputDialog::getText(this, "sql_del", "del_house(1,2,3,4...)", QLineEdit::Normal, sql_del);

    QSqlQuery *query_del = new QSqlQuery(db);
    if (!mySQL.exec(this, sql_del, "Удаление старого правила терр-прикрепления", *query_del, true, db, data_app)) {
        QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке удалить старое правило терр-прикрепления произошла неожиданная ошибка.\n\nОперация отменена.");
        delete query_del;

        db.rollback();
        return false;
    } else {
        delete query_del;
    }
    db.commit();

    te_log->append(" - " + QString::number(house) + " " + corp + " ");
    QApplication::processEvents();
    return true;
}




bool add_assig_wnd::try_all_street(QString ocato, int subj, int dstr, int city, int nasp, int strt, int sex, int age, QString code_mo, QString code_mp, QString code_ms, QString snils_mt) {
    // проверим, занята ли улица
    QString sql_test =  "select id, code_mo, code_mp, code_ms, snils_mt, sex, age, subj, dstr, city, nasp, strt, house, corp, ocato, from_start, to_end "
                        "  from assig_by_terr a "
                        " where ( (subj=" + QString::number(subj>0 ? subj : 0)  + ") or (subj  is NULL and " + QString::number(subj) + "<=0) ) and "
                        "       ( (dstr=" + QString::number(dstr>0 ? dstr : 0)  + ") or (dstr  is NULL and " + QString::number(dstr) + "<=0) ) and "
                        "       ( (city=" + QString::number(city>0 ? city : 0)  + ") or (city  is NULL and " + QString::number(city) + "<=0) ) and "
                        "       ( (nasp=" + QString::number(nasp>0 ? nasp : 0)  + ") or (nasp  is NULL and " + QString::number(nasp) + "<=0) ) and "
                        "       ( (strt=" + QString::number(strt>0 ? strt : 0)  + ") or (strt  is NULL and " + QString::number(strt) + "<=0) ) and "
                        "       ( (house is NULL or house=0) ) and "
                        "       ( (sex="  + QString::number(sex>0  ? sex  : 0)  + ") or (sex  is NULL  and " + QString::number(sex)  + "<=0) ) and "
                        "       ( (age="  + QString::number(age>0  ? age  : 0)  + ") or (age  is NULL  and " + QString::number(age)  + "<=0) ) ; ";

    QSqlQuery *query_test = new QSqlQuery(db);
    if (!mySQL.exec(this, sql_test, "Проверка на пересечение с существующими правилами", *query_test, true, db, data_app)) {
        QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке проверить новую запись на пересечение с существующими правилами произошла неожиданная ошибка.\n\nОперация отменена.");
        delete query_test;

        db.rollback();
        return false;
    } else { }

    if (query_test->size()>0) {
        while (query_test->next()) {
            int id2 = query_test->value(0).toInt();
            del_house(id2);

            /*QString code_mo2 = query_test->value(1).toString();
            QString code_mp2 = query_test->value(2).toString();
            QString code_ms2 = query_test->value(3).toString();
            QString snils_mt2= query_test->value(4).toString();
            int sex2  = query_test->value(5).toInt();
            int age2  = query_test->value(6).toInt();
            int subj2 = query_test->value(7).toInt();
            int dstr2 = query_test->value(8).toInt();
            int city2 = query_test->value(9).toInt();
            int nasp2 = query_test->value(10).toInt();
            int strt2 = query_test->value(11).toInt();
            int house2= query_test->value(12).toInt();
            QString corp2 = query_test->value(13).toString();
            QString ocato2 = query_test->value(14).toString();
            bool from_start2 = (query_test->value(15).toInt()>0);
            bool to_end2     = (query_test->value(16).toInt()>0);

            if ( (sex==0 || sex==sex2) && (age==0 || age==age2) ) {
                // новый диапазон полностью перекрвывает старый - удалим старый
                del_house(id2);

            } else if ( (sex2==0 && sex!=0) || (age2==0 && age!=0) ) {
                // новый диапазон частично перекрывает существующий - сузим старый
                // ... удалим старый
                del_house(id2);

                // ... добавим для взрослых
                if ((age2==0 || age2==2) && age==1) {
                    add_house(ocato2, subj2, dstr2, city2, nasp2, strt2, house2, corp2, sex2, 2, code_mo2, code_mp2, code_ms2, snils_mt2, from_start2, to_end2);
                }
                // ... добавим для детей
                if ((age2==0 || age2==1) && age==2) {
                    add_house(ocato2, subj2, dstr2, city2, nasp2, strt2, house2, corp2, sex2, 1, code_mo2, code_mp2, code_ms2, snils_mt2, from_start2, to_end2);
                }

                te_log->append(" - разбивка старой записи ");
                QApplication::processEvents();
            }*/
        }
    }
    delete query_test;

    // просто добавим запись
    return add_all_street(ocato, subj, dstr, city, nasp, strt, sex, age, code_mo, code_mp, code_ms, snils_mt);
}


bool add_assig_wnd::add_all_street (QString ocato, int subj, int dstr, int city, int nasp, int strt, int sex, int age, QString code_mo, QString code_mp, QString code_ms, QString snils_mt) {
    // добавим запись дома
    QString sql_add = "insert into assig_by_terr (ocato, subj, dstr, city, nasp, strt, house, corp, sex, age, code_mo, code_mp, code_ms, snils_mt, from_start, to_end, comment, id_point, id_operator, dt_ins) "
                      " values('" + ocato + "', "
                              " " + QString::number(subj>0 ? subj : 0) + ", "
                              " " + QString::number(dstr>0 ? dstr : 0) + ", "
                              " " + QString::number(city>0 ? city : 0) + ", "
                              " " + QString::number(nasp>0 ? nasp : 0) + ", "
                              " " + QString::number(strt>0 ? strt : 0) + ", "
                              " NULL, "
                              " NULL, "
                              " " + QString::number(sex>0 ? sex : 0) + ", "
                              " " + QString::number(age>0 ? age : 0) + ", "
                              " " + (code_mo.isEmpty() ? "NULL" : ("'"+code_mo+"'")) + ", "
                              " " + (code_mp.isEmpty() ? "NULL" : ("'"+code_mp+"'")) + ", "
                              " " + (code_ms.isEmpty() ? "NULL" : ("'"+code_ms+"'")) + ", "
                              " " + (snils_mt.isEmpty()? "NULL" : ("'"+snils_mt+"'"))+ ", "
                              " " + "0, "
                              " " + "0, "
                              "'" + ui->line_comment->text() + "', "
                              " " + QString::number(data_app.id_point) + ", "
                              " " + QString::number(data_app.id_operator) + ", "
                              " " + "CURRENT_TIMESTAMP) ; ";

    QSqlQuery *query_add = new QSqlQuery(db);
    if (!mySQL.exec(this, sql_add, "Добавление нового правила терр-прикрепления", *query_add, true, db, data_app)) {
        QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новое правило терр-прикрепления произошла неожиданная ошибка.\n\nОперация отменена.");
        delete query_add;

        db.rollback();
        return false;
    } else {
        delete query_add;
    }
    te_log->append(" + вся улица;  " + QString(sex==0 ? "М/Ж" : (sex==1 ? "М" : "Ж")) + ";  " + QString(age==0 ? "все" : (age==1 ? "дет." : "взр.")) );
}

void add_assig_wnd::on_line_corp_textEdited(const QString &arg1) {
    ui->line_corp->setText(ui->line_corp->text().trimmed().simplified().toUpper());
    ui->line_corp->setCursorPosition(ui->line_corp->text().size());
}

void add_assig_wnd::on_line_corp2_textEdited(const QString &arg1) {
    ui->line_corp2->setText(ui->line_corp2->text().trimmed().simplified().toUpper());
    ui->line_corp2->setCursorPosition(ui->line_corp2->text().size());
}

void add_assig_wnd::on_bn_ok_clicked() {
    // проверка данных
    if (ui->combo_mo->currentIndex()<0) {
        QMessageBox::warning(this, "Не хватает данных", "Выберите медорганизацию !");
        return;
    }
    if (ui->rb_one_house->isChecked()) {
        if (ui->spin_house->value()<1 || ui->spin_house->value()>1500) {
            QMessageBox::warning(this, "Неверные данные", "Номер дома должен попадать в интервал от 1 до 1500 !");
            return;
        }
    }
    if (ui->rb_open_line->isChecked()) {
        if (ui->spin_house->value()<1 || ui->spin_house->value()>1500) {
            QMessageBox::warning(this, "Неверные данные", "Номер крайнего дома в открытом диапазоне должен попадать в интервал от 1 до 1500 !");
            return;
        }
        if ( ( ui->ch_from_start->isChecked() &&  ui->ch_to_end->isChecked()) ||
             (!ui->ch_from_start->isChecked() && !ui->ch_to_end->isChecked()) ) {
            QMessageBox::warning(this, "Неверные данные", "Выберите одну (!) открытую сторону диапазона !");
            return;
        }
    }
    if (ui->rb_line_houses->isChecked()) {
        if (ui->spin_house->value()<1 || ui->spin_house->value()>1500) {
            QMessageBox::warning(this, "Неверные данные", "Номер дома начала диапазона должен попадать в интервал от 1 до 1500 !");
            return;
        }
        if (ui->spin_house2->value()<1 || ui->spin_house2->value()>1500) {
            QMessageBox::warning(this, "Неверные данные", "Номер дома конца диапазона должен попадать в интервал от 1 до 1500 !");
            return;
        }
        if (ui->combo_side->currentIndex()==0) {
            if (ui->spin_house->value()%2==0 && ui->spin_house2->value()%2==0) {
                int res = QMessageBox::question(this, "Чётная сторона?", "Обе границы интервала чёные.\nВыбрать только чётную сторону улицы?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes);
                if (res==QMessageBox::Yes) {
                    ui->combo_side->setCurrentIndex(2);
                } else if (res==QMessageBox::Cancel) {
                    return;
                }
            }
            if (ui->spin_house->value()%2==1 && ui->spin_house2->value()%2==1) {
                int res = QMessageBox::question(this, "Нечётная сторона?", "Обе границы интервала нечёные.\nВыбрать только нечётную сторону улицы?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes);
                if (res==QMessageBox::Yes) {
                    ui->combo_side->setCurrentIndex(1);
                } else if (res==QMessageBox::Cancel) {
                    return;
                }
            }
        }
        if (ui->combo_side->currentIndex()>0) {
            if (ui->spin_house->value()%2!=ui->spin_house2->value()%2) {
                if (QMessageBox::question(this, "Разные стороны?", "Границы интервала приходятся на разные стороны улицы.\nВыбрать обе стороны улицы?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes)==QMessageBox::Yes) {
                    ui->combo_side->setCurrentIndex(0);
                }
            }
        }
    }
    if (ui->rb_numlist->isChecked()) {
        // почистим список номеров от мусора
        ui->line_numlist->setText(ui->line_numlist->text().simplified().trimmed().replace(" ","").replace(";",",").replace("\"","").replace(".",",").replace("\'","").toUpper());
        if (ui->line_numlist->text().isEmpty()) {
            QMessageBox::warning(this, "Недостаточно данных", "Не задан список номеров домов для разбора !");
            return;
        }
    }

    db.transaction();

    if (data_assig.id<0) {

        if (ui->rb_one_house->isChecked()) {
            te_log->append("Добавление дома ");
            QApplication::processEvents();

            if (try_house(data_assig.ocato,
                          ui->combo_subj_terrs->currentData().toInt(),
                          ui->combo_dstr_terrs->currentData().toInt(),
                          ui->combo_city_terrs->currentData().toInt(),
                          ui->combo_nasp_terrs->currentData().toInt(),
                          ui->combo_strt_terrs->currentData().toInt(),
                          ui->spin_house->value(),
                          ui->line_corp->text(),
                          ui->combo_sex->currentIndex(),
                          ui->combo_age->currentIndex(),
                          ui->combo_mo->currentData().toString(),
                          ui->combo_mp->currentData().toString(),
                          ui->combo_ms->currentData().toString(),
                          ui->combo_mt->currentData().toString(),
                          false,
                          false) ) {
                // всё нормально

            } else {
                QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (один дом) произошла неожиданная ошибка.\n\nОперация отменена.");

                db.rollback();
                return;
            }

        } else if (ui->rb_all_street->isChecked()) {
            te_log->append("Добавление улицы ");
            QApplication::processEvents();

            if (try_all_street(data_assig.ocato,
                               ui->combo_subj_terrs->currentData().toInt(),
                               ui->combo_dstr_terrs->currentData().toInt(),
                               ui->combo_city_terrs->currentData().toInt(),
                               ui->combo_nasp_terrs->currentData().toInt(),
                               ui->combo_strt_terrs->currentData().toInt(),
                               ui->combo_sex->currentIndex(),
                               ui->combo_age->currentIndex(),
                               ui->combo_mo->currentData().toString(),
                               ui->combo_mp->currentData().toString(),
                               ui->combo_ms->currentData().toString(),
                               ui->combo_mt->currentData().toString()) ) {
                // всё нормально
            } else {
                QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (улица целиком) произошла неожиданная ошибка.\n\nОперация отменена.");

                db.rollback();
                return;
            }

        } else if (ui->rb_line_houses->isChecked()) {
            int house = ui->spin_house->value();
            QString corp = ui->line_corp->text().trimmed().simplified().toUpper();
            int house2 = ui->spin_house2->value();
            QString corp2 = ui->line_corp2->text().trimmed().simplified().toUpper();
            if (house>house2) {
                int a = house;
                QString c = corp;
                house = house2;
                corp = corp2;
                house2 = a;
                corp2 = c;
            }
            int n = house2-house+1;
            if (ui->combo_side->currentIndex()>0) {
                n = n/2;
            }

            int side = ui->combo_side->currentIndex();
            te_log->append("Добавление диапазона домов ");
            QApplication::processEvents();

            // первый дом
            if (try_house(data_assig.ocato,
                          ui->combo_subj_terrs->currentData().toInt(),
                          ui->combo_dstr_terrs->currentData().toInt(),
                          ui->combo_city_terrs->currentData().toInt(),
                          ui->combo_nasp_terrs->currentData().toInt(),
                          ui->combo_strt_terrs->currentData().toInt(),
                          house,
                          corp,
                          ui->combo_sex->currentIndex(),
                          ui->combo_age->currentIndex(),
                          ui->combo_mo->currentData().toString(),
                          ui->combo_mp->currentData().toString(),
                          ui->combo_ms->currentData().toString(),
                          ui->combo_mt->currentData().toString(),
                          false,
                          false) ) {
                // всё нормально
            } else {
                QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (начало диапазона домов) произошла неожиданная ошибка.\n\nОперация отменена.");

                db.rollback();
                return;
            }

            house++;
            if (corp.isEmpty()) {
                house--;
            }
            for (int i=house; i++; i<house2) {
                if ( (side==1 && i%2==0) ||
                     (side==2 && i%2==1) ) {
                    continue;
                }
                if (i>house2) {
                    break;
                }
                //QMessageBox::information(this,  "...", QString::number(house) + " -> " + QString::number(i) + " -> " + QString::number(house2));

                if (try_house(data_assig.ocato,
                              ui->combo_subj_terrs->currentData().toInt(),
                              ui->combo_dstr_terrs->currentData().toInt(),
                              ui->combo_city_terrs->currentData().toInt(),
                              ui->combo_nasp_terrs->currentData().toInt(),
                              ui->combo_strt_terrs->currentData().toInt(),
                              i,
                              "",
                              ui->combo_sex->currentIndex(),
                              ui->combo_age->currentIndex(),
                              ui->combo_mo->currentData().toString(),
                              ui->combo_mp->currentData().toString(),
                              ui->combo_ms->currentData().toString(),
                              ui->combo_mt->currentData().toString(),
                              false,
                              false) ) {
                    // всё нормально
                } else {
                    QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (диапазон домов) произошла неожиданная ошибка.\n\nОперация отменена.");

                    db.rollback();
                    return;
                }
            }

            // последний дом
            if (try_house(data_assig.ocato,
                          ui->combo_subj_terrs->currentData().toInt(),
                          ui->combo_dstr_terrs->currentData().toInt(),
                          ui->combo_city_terrs->currentData().toInt(),
                          ui->combo_nasp_terrs->currentData().toInt(),
                          ui->combo_strt_terrs->currentData().toInt(),
                          house2,
                          corp2,
                          ui->combo_sex->currentIndex(),
                          ui->combo_age->currentIndex(),
                          ui->combo_mo->currentData().toString(),
                          ui->combo_mp->currentData().toString(),
                          ui->combo_ms->currentData().toString(),
                          ui->combo_mt->currentData().toString(),
                          false,
                          false) ) {
                // всё нормально
            } else {
                QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (конец диапазона домов) произошла неожиданная ошибка.\n\nОперация отменена.");

                db.rollback();
                return;
            }

        } else if (ui->rb_open_line->isChecked()) {
            te_log->append("Добавление открытого диапазона ");
            QApplication::processEvents();

            if (try_house(data_assig.ocato,
                          ui->combo_subj_terrs->currentData().toInt(),
                          ui->combo_dstr_terrs->currentData().toInt(),
                          ui->combo_city_terrs->currentData().toInt(),
                          ui->combo_nasp_terrs->currentData().toInt(),
                          ui->combo_strt_terrs->currentData().toInt(),
                          ui->spin_house->value(),
                          ui->line_corp->text(),
                          ui->combo_sex->currentIndex(),
                          ui->combo_age->currentIndex(),
                          ui->combo_mo->currentData().toString(),
                          ui->combo_mp->currentData().toString(),
                          ui->combo_ms->currentData().toString(),
                          ui->combo_mt->currentData().toString(),
                          ui->ch_from_start->isChecked(),
                          ui->ch_to_end->isChecked()) ) {
                // всё нормально

            } else {
                QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (один дом) произошла неожиданная ошибка.\n\nОперация отменена.");

                db.rollback();
                return;
            }

        } else if (ui->rb_numlist->isChecked()) {
            te_log->append("Добавление произволного списка домов ");
            QApplication::processEvents();

            QString s = "0123456789,-/";
            QString s_numlist = ui->line_numlist->text();
            s_numlist = s_numlist.replace(";", ",").trimmed().replace(" ","").replace("\t","").replace("\n","");

            // разберём список по запятым
            int pos_pt = 0;
            int pos_max = s_numlist.length();

            while (pos_pt<pos_max) {
                int pos_pt2 = s_numlist.indexOf(",", pos_pt);
                if (pos_pt2<0)  pos_pt2 = pos_max;
                if (pos_pt==pos_pt2) {
                    pos_pt = pos_pt2 +1;
                    continue;
                }
                QString s_numpart = s_numlist.mid(pos_pt, pos_pt2 - pos_pt);
                pos_pt = pos_pt2+1;

                // разберём выбранный фрагмент списка
                {
                    int pos = 0;
                    int len = s_numpart.size();
                    bool f_end = false;

                    QString house_s;
                    int house = 0;
                    QString corp;
                    QString house2_s;
                    int house2 = 0;
                    QString corp2;

                    while (pos<len) {
                        //QMessageBox::information(this,  "1", "len=" + QString::number(len) + " pos=" + QString::number(pos));

                        house_s = "";
                        house = 0;
                        corp = "";
                        house2_s = "";
                        house2 = 0;
                        corp2 = "";

                        //поиск первой цифры
                        while ( s_numpart.at(pos)<s.at(0) || s_numpart.at(pos)>s.at(9) ) {
                            //QMessageBox::information(this,  "2", "len=" + QString::number(len) + " pos=" + QString::number(pos));
                            pos++;
                            if (pos>=len) {
                                f_end = true;
                                break;
                            }
                        }
                        //QMessageBox::information(this,  "3", "len=" + QString::number(len) + " pos=" + QString::number(pos));
                        if (f_end) break;

                        // выбор первого числа
                        house_s = "";
                        house = 0;
                        while ( s_numpart.at(pos)>=s.at(0) && s_numpart.at(pos)<=s.at(9) ) {
                            //QMessageBox::information(this,  "4", "len=" + QString::number(len) + " pos=" + QString::number(pos) + " house_s=" + house_s);
                            house_s += s_numpart.at(pos);
                            pos++;
                            if (pos>=len) {
                                f_end = true;
                                break;
                            }
                        }
                        //QMessageBox::information(this,  "5", "len=" + QString::number(len) + " pos=" + QString::number(pos) + " house_s=" + house_s);
                        if (!house_s.isEmpty()) {
                            // номер найден
                            house = house_s.toInt();
                        }
                        if (f_end) break;

                        // выбор литеры корпуса
                        corp = "";
                        while ( s_numpart.at(pos)!=s.at(10) && s_numpart.at(pos)!=s.at(11) ) {
                            //QMessageBox::information(this,  "6", "len=" + QString::number(len) + " pos=" + QString::number(pos) + " house=" + QString::number(house) + " corp=" + corp);
                            if (s_numpart.at(pos)!=s.at(12)) {
                                corp += s_numpart.at(pos);
                            }
                            pos++;
                            if (pos>=len) {
                                f_end = true;
                                break;
                            }
                        }
                        //QMessageBox::information(this,  "7", "len=" + QString::number(len) + " pos=" + QString::number(pos) + " house=" + QString::number(house) + " corp=" + corp);
                        if (f_end) break;

                        if (s_numpart.at(pos)==s.at(11) && (pos<len-1) && ( s_numpart.at(pos+1)>=s.at(0) && s_numpart.at(pos+1)<=s.at(9) )) {
                            // найдём второе число

                            //поиск первой цифры
                            while ( s_numpart.at(pos)<s.at(0) || s_numpart.at(pos)>s.at(9) ) {
                                //QMessageBox::information(this,  "8", "len=" + QString::number(len) + " pos=" + QString::number(pos) + " house=" + QString::number(house) + " corp=" + corp);
                                pos++;
                                if (pos>=len) {
                                    f_end = true;
                                    break;
                                }
                            }
                            //QMessageBox::information(this,  "9", "len=" + QString::number(len) + " pos=" + QString::number(pos) + " house=" + QString::number(house) + " corp=" + corp);
                            if (f_end) break;

                            // выбор второго числа
                            house2_s = "";
                            house2 = 0;
                            while ( s_numpart.at(pos)>=s.at(0) && s_numpart.at(pos)<=s.at(9) ) {
                                //QMessageBox::information(this,  "10", "len=" + QString::number(len) + " pos=" + QString::number(pos) + " house=" + QString::number(house) + " corp=" + corp + " house2_s=" + house2_s);
                                house2_s += s_numpart.at(pos);
                                pos++;
                                if (pos>=len) {
                                    f_end = true;
                                    break;
                                }
                            }
                            //QMessageBox::information(this,  "11", "len=" + QString::number(len) + " pos=" + QString::number(pos) + " house=" + QString::number(house) + " corp=" + corp + " house2_s=" + house2_s);
                            if (!house2_s.isEmpty()) {
                                // номер найден
                                house2 = house2_s.toInt();
                            }
                            if (f_end) break;

                            // выбор литеры корпуса
                            corp2 = "";
                            while ( s_numpart.at(pos)!=s.at(10) && s_numpart.at(pos)!=s.at(11) ) {
                                //QMessageBox::information(this,  "12", "len=" + QString::number(len) + " pos=" + QString::number(pos) + " house=" + QString::number(house) + " corp=" + corp + " house2_s=" + QString::number(house2) + " corp2=" + corp2);
                                if (s_numpart.at(pos)!=s.at(12)) {
                                    corp2 += s_numpart.at(pos);
                                }
                                pos++;
                                if (pos>=len) {
                                    f_end = true;
                                    break;
                                }
                            }
                            //QMessageBox::information(this,  "13", "len=" + QString::number(len) + " pos=" + QString::number(pos) + " house=" + QString::number(house) + " corp=" + corp + " house2_s=" + QString::number(house2) + " corp2=" + corp2);
                            if (f_end) break;

                        }

                        if (house>0 && house2>0) {

                            // --------------------------------------------
                            // обработаем диапазон
                            // --------------------------------------------

                            if (house>house2) {
                                int a = house;
                                QString c = corp;
                                house = house2;
                                corp = corp2;
                                house2 = a;
                                corp2 = c;
                            }
                            int n = house2-house+1;
                            if (ui->combo_side->currentIndex()>0) {
                                n = n/2;
                            }

                            int side = 0;
                            if (house%2==1 && house2%2==1) {
                                side = 1;
                            }
                            if (house%2==0 && house2%2==0) {
                                side = 2;
                            }
                            te_log->append(" -> ");
                            QApplication::processEvents();

                            // первый дом
                            if (try_house(data_assig.ocato,
                                          ui->combo_subj_terrs->currentData().toInt(),
                                          ui->combo_dstr_terrs->currentData().toInt(),
                                          ui->combo_city_terrs->currentData().toInt(),
                                          ui->combo_nasp_terrs->currentData().toInt(),
                                          ui->combo_strt_terrs->currentData().toInt(),
                                          house,
                                          corp,
                                          ui->combo_sex->currentIndex(),
                                          ui->combo_age->currentIndex(),
                                          ui->combo_mo->currentData().toString(),
                                          ui->combo_mp->currentData().toString(),
                                          ui->combo_ms->currentData().toString(),
                                          ui->combo_mt->currentData().toString(),
                                          false,
                                          false) ) {
                                // всё нормально
                            } else {
                                QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (начало диапазона домов) произошла неожиданная ошибка.\n\nОперация отменена.");

                                db.rollback();
                                return;
                            }

                            house++;
                            if (corp.isEmpty()) {
                                house--;
                            }
                            for (int i=house; i++; i<house2) {
                                if ( (side==1 && i%2==0) ||
                                     (side==2 && i%2==1) ) {
                                    continue;
                                }
                                if (i>house2) {
                                    break;
                                }
                                //QMessageBox::information(this,  "...", QString::number(house) + " -> " + QString::number(i) + " -> " + QString::number(house2));

                                if (try_house(data_assig.ocato,
                                              ui->combo_subj_terrs->currentData().toInt(),
                                              ui->combo_dstr_terrs->currentData().toInt(),
                                              ui->combo_city_terrs->currentData().toInt(),
                                              ui->combo_nasp_terrs->currentData().toInt(),
                                              ui->combo_strt_terrs->currentData().toInt(),
                                              i,
                                              "",
                                              ui->combo_sex->currentIndex(),
                                              ui->combo_age->currentIndex(),
                                              ui->combo_mo->currentData().toString(),
                                              ui->combo_mp->currentData().toString(),
                                              ui->combo_ms->currentData().toString(),
                                              ui->combo_mt->currentData().toString(),
                                              false,
                                              false) ) {
                                    // всё нормально
                                } else {
                                    QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (диапазон домов) произошла неожиданная ошибка.\n\nОперация отменена.");

                                    db.rollback();
                                    return;
                                }
                            }

                            // последний дом
                            if (try_house(data_assig.ocato,
                                          ui->combo_subj_terrs->currentData().toInt(),
                                          ui->combo_dstr_terrs->currentData().toInt(),
                                          ui->combo_city_terrs->currentData().toInt(),
                                          ui->combo_nasp_terrs->currentData().toInt(),
                                          ui->combo_strt_terrs->currentData().toInt(),
                                          house2,
                                          corp2,
                                          ui->combo_sex->currentIndex(),
                                          ui->combo_age->currentIndex(),
                                          ui->combo_mo->currentData().toString(),
                                          ui->combo_mp->currentData().toString(),
                                          ui->combo_ms->currentData().toString(),
                                          ui->combo_mt->currentData().toString(),
                                          false,
                                          false) ) {
                                // всё нормально
                            } else {
                                QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (конец диапазона домов) произошла неожиданная ошибка.\n\nОперация отменена.");

                                db.rollback();
                                return;
                            }

                        } else if (house>0) {

                            // --------------------------------------------
                            // попробуем обработать один дом
                            // --------------------------------------------

                            te_log->append(" -> ");
                            QApplication::processEvents();

                            if (try_house(data_assig.ocato,
                                          ui->combo_subj_terrs->currentData().toInt(),
                                          ui->combo_dstr_terrs->currentData().toInt(),
                                          ui->combo_city_terrs->currentData().toInt(),
                                          ui->combo_nasp_terrs->currentData().toInt(),
                                          ui->combo_strt_terrs->currentData().toInt(),
                                          house,
                                          corp,
                                          ui->combo_sex->currentIndex(),
                                          ui->combo_age->currentIndex(),
                                          ui->combo_mo->currentData().toString(),
                                          ui->combo_mp->currentData().toString(),
                                          ui->combo_ms->currentData().toString(),
                                          ui->combo_mt->currentData().toString(),
                                          false,
                                          false) ) {
                                // всё нормально

                            } else {
                                QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (один дом) произошла неожиданная ошибка.\n\nОперация отменена.");

                                db.rollback();
                                return;
                            }

                        }
                    }

                    // --------------------------------------------
                    // обработаем последний дом или диапазон домов
                    // --------------------------------------------

                    if (house>0 && house2>0) {

                        // --------------------------------------------
                        // обработаем диапазон
                        // --------------------------------------------

                        if (house>house2) {
                            int a = house;
                            QString c = corp;
                            house = house2;
                            corp = corp2;
                            house2 = a;
                            corp2 = c;
                        }
                        int n = house2-house+1;
                        if (ui->combo_side->currentIndex()>0) {
                            n = n/2;
                        }

                        int side = 0;
                        if (house%2==1 && house2%2==1) {
                            side = 1;
                        }
                        if (house%2==0 && house2%2==0) {
                            side = 2;
                        }
                        te_log->append(" -> ");
                        QApplication::processEvents();

                        // первый дом
                        if (try_house(data_assig.ocato,
                                      ui->combo_subj_terrs->currentData().toInt(),
                                      ui->combo_dstr_terrs->currentData().toInt(),
                                      ui->combo_city_terrs->currentData().toInt(),
                                      ui->combo_nasp_terrs->currentData().toInt(),
                                      ui->combo_strt_terrs->currentData().toInt(),
                                      house,
                                      corp,
                                      ui->combo_sex->currentIndex(),
                                      ui->combo_age->currentIndex(),
                                      ui->combo_mo->currentData().toString(),
                                      ui->combo_mp->currentData().toString(),
                                      ui->combo_ms->currentData().toString(),
                                      ui->combo_mt->currentData().toString(),
                                      false,
                                      false) ) {
                            // всё нормально
                        } else {
                            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (начало диапазона домов) произошла неожиданная ошибка.\n\nОперация отменена.");

                            db.rollback();
                            return;
                        }

                        house++;
                        if (corp.isEmpty()) {
                            house--;
                        }
                        for (int i=house; i++; i<house2) {
                            if ( (side==1 && i%2==0) ||
                                 (side==2 && i%2==1) ) {
                                continue;
                            }
                            if (i>house2) {
                                break;
                            }
                            //QMessageBox::information(this,  "...", QString::number(house) + " -> " + QString::number(i) + " -> " + QString::number(house2));

                            if (try_house(data_assig.ocato,
                                          ui->combo_subj_terrs->currentData().toInt(),
                                          ui->combo_dstr_terrs->currentData().toInt(),
                                          ui->combo_city_terrs->currentData().toInt(),
                                          ui->combo_nasp_terrs->currentData().toInt(),
                                          ui->combo_strt_terrs->currentData().toInt(),
                                          i,
                                          "",
                                          ui->combo_sex->currentIndex(),
                                          ui->combo_age->currentIndex(),
                                          ui->combo_mo->currentData().toString(),
                                          ui->combo_mp->currentData().toString(),
                                          ui->combo_ms->currentData().toString(),
                                          ui->combo_mt->currentData().toString(),
                                          false,
                                          false) ) {
                                // всё нормально
                            } else {
                                QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (диапазон домов) произошла неожиданная ошибка.\n\nОперация отменена.");

                                db.rollback();
                                return;
                            }
                        }

                        // последний дом
                        if (try_house(data_assig.ocato,
                                      ui->combo_subj_terrs->currentData().toInt(),
                                      ui->combo_dstr_terrs->currentData().toInt(),
                                      ui->combo_city_terrs->currentData().toInt(),
                                      ui->combo_nasp_terrs->currentData().toInt(),
                                      ui->combo_strt_terrs->currentData().toInt(),
                                      house2,
                                      corp2,
                                      ui->combo_sex->currentIndex(),
                                      ui->combo_age->currentIndex(),
                                      ui->combo_mo->currentData().toString(),
                                      ui->combo_mp->currentData().toString(),
                                      ui->combo_ms->currentData().toString(),
                                      ui->combo_mt->currentData().toString(),
                                      false,
                                      false) ) {
                            // всё нормально
                        } else {
                            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (конец диапазона домов) произошла неожиданная ошибка.\n\nОперация отменена.");

                            db.rollback();
                            return;
                        }

                    } else if (house>0) {

                        // --------------------------------------------
                        // попробуем обработать один дом
                        // --------------------------------------------

                        te_log->append(" -> ");
                        QApplication::processEvents();

                        if (try_house(data_assig.ocato,
                                      ui->combo_subj_terrs->currentData().toInt(),
                                      ui->combo_dstr_terrs->currentData().toInt(),
                                      ui->combo_city_terrs->currentData().toInt(),
                                      ui->combo_nasp_terrs->currentData().toInt(),
                                      ui->combo_strt_terrs->currentData().toInt(),
                                      house,
                                      corp,
                                      ui->combo_sex->currentIndex(),
                                      ui->combo_age->currentIndex(),
                                      ui->combo_mo->currentData().toString(),
                                      ui->combo_mp->currentData().toString(),
                                      ui->combo_ms->currentData().toString(),
                                      ui->combo_mt->currentData().toString(),
                                      false,
                                      false) ) {
                            // всё нормально

                        } else {
                            QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (один дом) произошла неожиданная ошибка.\n\nОперация отменена.");

                            db.rollback();
                            return;
                        }
                    }
                }




            }















        }


    } else {

        if (QMessageBox::question(this,
                                  "Нужно подтверждение",
                                  "Вы действительно хотите изменить выбранный адресный элемент или переместить его в состав территории обслуживания другой МО?",
                                  QMessageBox::Yes|QMessageBox::No,
                                  QMessageBox::No)==QMessageBox::Yes) {
            // удалим старую версию записи
            del_house(data_assig.id);

            // добавим новую версию записи
            if (try_house(data_assig.ocato,
                          ui->combo_subj_terrs->currentData().toInt(),
                          ui->combo_dstr_terrs->currentData().toInt(),
                          ui->combo_city_terrs->currentData().toInt(),
                          ui->combo_nasp_terrs->currentData().toInt(),
                          ui->combo_strt_terrs->currentData().toInt(),
                          ui->spin_house->value(),
                          ui->line_corp->text().trimmed().simplified().toUpper(),
                          ui->combo_sex->currentIndex(),
                          ui->combo_age->currentIndex(),
                          ui->combo_mo->currentData().toString(),
                          ui->combo_mp->currentData().toString(),
                          ui->combo_ms->currentData().toString(),
                          ui->combo_mt->currentData().toString(),
                          (ui->rb_open_line->isChecked() && ui->ch_from_start->isChecked()),
                          (ui->rb_open_line->isChecked() && ui->ch_to_end->isChecked())) ) {
                // всё нормально
            } else {
                QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке изменить существующую запись произошла неожиданная ошибка.\n\nОперация отменена.");
                return;
            }
        } else {
            return;
        }
    }

    db.commit();

    data_assig.subj = ui->combo_subj_terrs->currentData().toInt();
    data_assig.dstr = ui->combo_dstr_terrs->currentData().toInt();
    data_assig.city = ui->combo_city_terrs->currentData().toInt();
    data_assig.nasp = ui->combo_nasp_terrs->currentData().toInt();
    data_assig.strt = ui->combo_strt_terrs->currentData().toInt();

    if (ui->rb_one_house->isChecked())
        data_assig.right_type;
    else if (ui->rb_line_houses->isChecked())
        data_assig.right_type = 2;
    else if (ui->rb_all_street->isChecked())
        data_assig.right_type = 3;
    else if (ui->rb_open_line->isChecked())
        data_assig.right_type = 4;

    data_assig.house = ui->spin_house->value();
    data_assig.corp = ui->line_corp->text();
    data_assig.house2 = ui->spin_house2->value();
    data_assig.corp2 = ui->line_corp2->text();

    data_assig.from_start = ui->ch_from_start->isChecked();
    data_assig.to_end = ui->ch_to_end->isChecked();

    data_assig.code_mo = ui->combo_mo->currentData().toString();
    data_assig.code_mp = ui->combo_mp->currentData().toString();
    data_assig.code_ms = ui->combo_ms->currentData().toString();
    data_assig.snils_mt = ui->combo_mt->currentData().toString();

    data_assig.comment = ui->line_comment->text();

    close();
}

void add_assig_wnd::on_bn_cansel_clicked() {
    close();
}

void add_assig_wnd::on_bn_add_clicked() {
    if (QMessageBox::question(this, "Нужно подтверждение", "Вы действительно хотите создать независимую копию выбранной записи?", QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Yes)==QMessageBox::Yes) {

        // проверка данных
        if (ui->combo_mo->currentIndex()<0) {
            QMessageBox::warning(this, "Не хватает данных", "Выберите медорганизацию !");
            return;
        }
        if (ui->rb_one_house->isChecked()) {
            if (ui->spin_house->value()<0 || ui->spin_house->value()>1500) {
                QMessageBox::warning(this, "Неверные данные", "Номер дома должен попадать в интервал от 1 до 1500 !");
                return;
            }
        }
        if (ui->rb_line_houses->isChecked()) {
            if (ui->spin_house->value()<0 || ui->spin_house->value()>1500) {
                QMessageBox::warning(this, "Неверные данные", "Номер дома начала диапазона должен попадать в интервал от 1 до 1500 !");
                return;
            }
            if (ui->spin_house2->value()<0 || ui->spin_house2->value()>1500) {
                QMessageBox::warning(this, "Неверные данные", "Номер дома конца диапазона должен попадать в интервал от 1 до 1500 !");
                return;
            }
            if (ui->combo_side->currentIndex()==0) {
                if (ui->spin_house->value()%2==0 && ui->spin_house2->value()%2==0) {
                    int res = QMessageBox::question(this, "Чётная сторона?", "Обе границы интервала чёные.\nВыбрать только чётную сторону улицы?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes);
                    if (res==QMessageBox::Yes) {
                        ui->combo_side->setCurrentIndex(2);
                    } else if (res==QMessageBox::Cancel) {
                        return;
                    }
                }
                if (ui->spin_house->value()%2==1 && ui->spin_house2->value()%2==1) {
                    int res = QMessageBox::question(this, "Нечётная сторона?", "Обе границы интервала нечёные.\nВыбрать только нечётную сторону улицы?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes);
                    if (res==QMessageBox::Yes) {
                        ui->combo_side->setCurrentIndex(1);
                    } else if (res==QMessageBox::Cancel) {
                        return;
                    }
                }
            }
            if (ui->combo_side->currentIndex()>0) {
                if (ui->spin_house->value()%2!=ui->spin_house2->value()%2) {
                    if (QMessageBox::question(this, "Разные стороны?", "Границы интервала приходятся на разные стороны улицы.\nВыбрать обе стороны улицы?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes)==QMessageBox::Yes) {
                        ui->combo_side->setCurrentIndex(0);
                    }
                }
            }
        }

        db.transaction();

        data_assig.id = -1;

        {
            if (ui->rb_one_house->isChecked()) {
                te_log->append("Добавление дома ");
                QApplication::processEvents();

                if (try_house(data_assig.ocato,
                              ui->combo_subj_terrs->currentData().toInt(),
                              ui->combo_dstr_terrs->currentData().toInt(),
                              ui->combo_city_terrs->currentData().toInt(),
                              ui->combo_nasp_terrs->currentData().toInt(),
                              ui->combo_strt_terrs->currentData().toInt(),
                              ui->spin_house->value(),
                              ui->line_corp->text(),
                              ui->combo_sex->currentIndex(),
                              ui->combo_age->currentIndex(),
                              ui->combo_mo->currentData().toString(),
                              ui->combo_mp->currentData().toString(),
                              ui->combo_ms->currentData().toString(),
                              ui->combo_mt->currentData().toString(),
                              false,
                              false) ) {
                    // всё нормально

                } else {
                    QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (один дом) произошла неожиданная ошибка.\n\nОперация отменена.");

                    db.rollback();
                    return;
                }

            } else if (ui->rb_all_street->isChecked()) {
                te_log->append("Добавление улицы ");
                QApplication::processEvents();

                if (try_all_street(data_assig.ocato,
                                   ui->combo_subj_terrs->currentData().toInt(),
                                   ui->combo_dstr_terrs->currentData().toInt(),
                                   ui->combo_city_terrs->currentData().toInt(),
                                   ui->combo_nasp_terrs->currentData().toInt(),
                                   ui->combo_strt_terrs->currentData().toInt(),
                                   ui->combo_sex->currentIndex(),
                                   ui->combo_age->currentIndex(),
                                   ui->combo_mo->currentData().toString(),
                                   ui->combo_mp->currentData().toString(),
                                   ui->combo_ms->currentData().toString(),
                                   ui->combo_mt->currentData().toString()) ) {
                    // всё нормально
                } else {
                    QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (улица целиком) произошла неожиданная ошибка.\n\nОперация отменена.");

                    db.rollback();
                    return;
                }

            } else if (ui->rb_line_houses->isChecked()) {
                int house = ui->spin_house->value();
                QString corp = ui->line_corp->text().trimmed().simplified().toUpper();
                int house2 = ui->spin_house2->value();
                QString corp2 = ui->line_corp2->text().trimmed().simplified().toUpper();
                if (house>house2) {
                    int a = house;
                    QString c = corp;
                    house = house2;
                    corp = corp2;
                    house2 = a;
                    corp2 = c;
                }
                int n = house2-house+1;
                if (ui->combo_side->currentIndex()>0) {
                    n = n/2;
                }

                int side = ui->combo_side->currentIndex();
                te_log->append("Добавление диапазона домов ");
                QApplication::processEvents();

                // первый дом
                if (try_house(data_assig.ocato,
                              ui->combo_subj_terrs->currentData().toInt(),
                              ui->combo_dstr_terrs->currentData().toInt(),
                              ui->combo_city_terrs->currentData().toInt(),
                              ui->combo_nasp_terrs->currentData().toInt(),
                              ui->combo_strt_terrs->currentData().toInt(),
                              house,
                              corp,
                              ui->combo_sex->currentIndex(),
                              ui->combo_age->currentIndex(),
                              ui->combo_mo->currentData().toString(),
                              ui->combo_mp->currentData().toString(),
                              ui->combo_ms->currentData().toString(),
                              ui->combo_mt->currentData().toString(),
                              false,
                              false) ) {
                    // всё нормально
                } else {
                    QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (начало диапазона домов) произошла неожиданная ошибка.\n\nОперация отменена.");

                    db.rollback();
                    return;
                }

                house++;
                if (corp.isEmpty()) {
                    house--;
                }
                for (int i=house; i++; i<house2) {
                    if ( (side==1 && i%2==0) ||
                         (side==2 && i%2==1) ) {
                        continue;
                    }
                    if (i>house2) {
                        break;
                    }
                    //QMessageBox::information(this,  "...", QString::number(house) + " -> " + QString::number(i) + " -> " + QString::number(house2));

                    if (try_house(data_assig.ocato,
                                  ui->combo_subj_terrs->currentData().toInt(),
                                  ui->combo_dstr_terrs->currentData().toInt(),
                                  ui->combo_city_terrs->currentData().toInt(),
                                  ui->combo_nasp_terrs->currentData().toInt(),
                                  ui->combo_strt_terrs->currentData().toInt(),
                                  i,
                                  "",
                                  ui->combo_sex->currentIndex(),
                                  ui->combo_age->currentIndex(),
                                  ui->combo_mo->currentData().toString(),
                                  ui->combo_mp->currentData().toString(),
                                  ui->combo_ms->currentData().toString(),
                                  ui->combo_mt->currentData().toString(),
                                  false,
                                  false) ) {
                        // всё нормально
                    } else {
                        QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (диапазон домов) произошла неожиданная ошибка.\n\nОперация отменена.");

                        db.rollback();
                        return;
                    }
                }

                // последний дом
                if (try_house(data_assig.ocato,
                              ui->combo_subj_terrs->currentData().toInt(),
                              ui->combo_dstr_terrs->currentData().toInt(),
                              ui->combo_city_terrs->currentData().toInt(),
                              ui->combo_nasp_terrs->currentData().toInt(),
                              ui->combo_strt_terrs->currentData().toInt(),
                              house2,
                              corp2,
                              ui->combo_sex->currentIndex(),
                              ui->combo_age->currentIndex(),
                              ui->combo_mo->currentData().toString(),
                              ui->combo_mp->currentData().toString(),
                              ui->combo_ms->currentData().toString(),
                              ui->combo_mt->currentData().toString(),
                              false,
                              false) ) {
                    // всё нормально
                } else {
                    QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (конец диапазона домов) произошла неожиданная ошибка.\n\nОперация отменена.");

                    db.rollback();
                    return;
                }
            } else if (ui->rb_open_line->isChecked()) {
                te_log->append("Добавление открытого диапазона ");
                QApplication::processEvents();

                if (try_house(data_assig.ocato,
                              ui->combo_subj_terrs->currentData().toInt(),
                              ui->combo_dstr_terrs->currentData().toInt(),
                              ui->combo_city_terrs->currentData().toInt(),
                              ui->combo_nasp_terrs->currentData().toInt(),
                              ui->combo_strt_terrs->currentData().toInt(),
                              ui->spin_house->value(),
                              ui->line_corp->text(),
                              ui->combo_sex->currentIndex(),
                              ui->combo_age->currentIndex(),
                              ui->combo_mo->currentData().toString(),
                              ui->combo_mp->currentData().toString(),
                              ui->combo_ms->currentData().toString(),
                              ui->combo_mt->currentData().toString(),
                              ui->ch_from_start->isChecked(),
                              ui->ch_to_end->isChecked()) ) {
                    // всё нормально

                } else {
                    QMessageBox::critical(this, "Непредвиденная ошибка", "При попытке добавить новую запись (один дом) произошла неожиданная ошибка.\n\nОперация отменена.");

                    db.rollback();
                    return;
                }
            }
        }

        db.commit();

        data_assig.subj = ui->combo_subj_terrs->currentData().toInt();
        data_assig.dstr = ui->combo_dstr_terrs->currentData().toInt();
        data_assig.city = ui->combo_city_terrs->currentData().toInt();
        data_assig.nasp = ui->combo_nasp_terrs->currentData().toInt();
        data_assig.strt = ui->combo_strt_terrs->currentData().toInt();

        if (ui->rb_one_house->isChecked())
            data_assig.right_type;
        else if (ui->rb_line_houses->isChecked())
            data_assig.right_type = 2;
        else if (ui->rb_all_street->isChecked())
            data_assig.right_type = 3;
        else if (ui->rb_open_line->isChecked())
            data_assig.right_type = 4;

        data_assig.house = ui->spin_house->value();
        data_assig.corp = ui->line_corp->text();
        data_assig.house2 = ui->spin_house2->value();
        data_assig.corp2 = ui->line_corp2->text();

        data_assig.from_start = ui->ch_from_start->isChecked();
        data_assig.to_end = ui->ch_to_end->isChecked();

        data_assig.code_mo = ui->combo_mo->currentData().toString();
        data_assig.code_mp = ui->combo_mp->currentData().toString();
        data_assig.code_ms = ui->combo_ms->currentData().toString();
        data_assig.snils_mt = ui->combo_mt->currentData().toString();

        data_assig.comment = ui->line_comment->text();

        close();
    }
}

void add_assig_wnd::on_combo_mo_currentIndexChanged(int index) {

    refresh_mp();
    ui->combo_mp->setCurrentIndex(ui->combo_mp->findData(data_assig.code_mp));

    refresh_ms();
    ui->combo_ms->setCurrentIndex(ui->combo_ms->findData(data_assig.code_ms));

    refresh_mt();
    ui->combo_mt->setCurrentIndex(ui->combo_mt->findData(data_assig.snils_mt));

    /*refresh_mp();
    refresh_ms();
    refresh_mt();*/
}
void add_assig_wnd::on_combo_mp_currentIndexChanged(int index) {

    refresh_ms();
    ui->combo_ms->setCurrentIndex(ui->combo_ms->findData(data_assig.code_ms));

    refresh_mt();
    ui->combo_mt->setCurrentIndex(ui->combo_mt->findData(data_assig.snils_mt));
}
void add_assig_wnd::on_combo_ms_currentIndexChanged(int index) {

    refresh_mt();
    ui->combo_mt->setCurrentIndex(ui->combo_mt->findData(data_assig.snils_mt));
    /*refresh_mt();*/
}

void add_assig_wnd::on_ch_from_start_clicked(bool checked) {
    if (checked)
        ui->ch_to_end->setChecked(false);
}
void add_assig_wnd::on_ch_to_end_clicked(bool checked) {
    if (checked)
        ui->ch_from_start->setChecked(false);
}

void add_assig_wnd::on_line_numlist_editingFinished() {
    QString sss = "0123456789--,";
    //QMessageBox::warning(this, "line_numlist_editingFinished - 1", ui->line_numlist->text());
    QString s = ui->line_numlist->text().simplified().trimmed().replace(" ","").replace(";",",").replace("\"","").replace(".",",").replace("\'","").toUpper();
    /*QString s2 = "";
    if (s.size()>0)    s2 += s.at(0);
    for (int i=1; i<s.size()-1; i++) {
        if (s.at(i)==sss.at(10)) {
            if (s.at(i-1)>=sss.at(0) && s.at(i-1)<=sss.at(12) && s.at(i+1)>=sss.at(0) && s.at(i+1)<=sss.at(12)) {
                // скопируем симол
                s2 += s.at(i);
            } else {
                // пропустим
            }
        } else {
            // просто скопируем символ
            s2 += s.at(i);
        }
    }
    if (s.size()>0)    s2 += s.at(s.size()-1);*/
    ui->line_numlist->setText(s);
    //QMessageBox::warning(this, "line_numlist_editingFinished - 2", ui->line_numlist->text());
}
