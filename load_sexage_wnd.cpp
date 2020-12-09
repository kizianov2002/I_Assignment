#include "load_sexage_wnd.h"
#include "ui_load_sexage_wnd.h"
#include "ki_funcs.h"

load_sexage_wnd::load_sexage_wnd(int &id_sexage, QSqlDatabase &db, s_data_app &data_app, QSettings &settings, QWidget *parent) :
    id_sexage(id_sexage),
    db(db),
    data_app(data_app),
    settings(settings),
    QDialog(parent),
    ui(new Ui::load_sexage_wnd)
{
    ui->setupUi(this);

    QFont font = this->font();
    font.setPointSize(data_app.font_size);
    this->setFont(font);

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

    if (QMessageBox::warning(this, "Нужно подтверждение",
                             "Вы действительно хотите заменить табличные данные половозрастной  " + ui->combo_sexage->currentText() + "  данными от ТФОМС?\n\n"
                             "При этом табличные данные перестанут согласовываться с сохранёнными списками застрахованных для ячеек таблицы.\n\n"
                             "Это необратимая операция, но в случае ошибочной загрузки данных таблицу можно заново сгенерировать с нуля.",
                             QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
        // отмена выбора
        ui->combo_sexage->setCurrentIndex(ui->combo_sexage->findData(-1));
        id_sexage = -1;
    }
}

load_sexage_wnd::~load_sexage_wnd() {
    delete ui;
}

void load_sexage_wnd::on_bn_cansel_clicked() {
    reject();
}

void load_sexage_wnd::refresh_combo_sexage() {
    f_refresh = true;
    this->setCursor(Qt::WaitCursor);
    // обновление выпадающего списка медорганизаций
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "select s.id, (l.layer_name ||'  -  '|| case s.assig_type when 1 then 'по терр.' when 2 then 'по заявл.' else '- все -' end) as sexage_name, "
                  "       l.layer_date, l.layer_type, s.assig_type "
                  "  from assig.sexages s "
                  "  left join assig.layers l on(l.id=s.id_layer)"
                  " where l.layer_type=2 "
                  "   and coalesce(s.assig_type,0)=0 "
                  " order by l.layer_date DESC, l.layer_type, s.assig_type ; ";
    mySQL.exec(this, sql, "Список снимков данных прикреления", *query, true, db, data_app);
    ui->combo_sexage->clear();
    while (query->next()) {
        ui->combo_sexage->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    delete query;
    this->setCursor(Qt::ArrowCursor);
    f_refresh = false;
}

void load_sexage_wnd::on_combo_sexage_currentIndexChanged(int index) {
    if ( !f_refresh
         && ui->combo_sexage->currentIndex()>=0 ) {
        if (QMessageBox::warning(this, "Нужно подтверждение",
                                 "Вы действительно хотите заменить табличные данные половозрастной  " + ui->combo_sexage->currentText() + "  данными от ТФОМС?\n\n"
                                 "При этом табличные данные перестанут согласовываться с сохранёнными списками застрахованных для ячеек таблицы.\n\n"
                                 "Это необратимая операция, но в случае ошибочной загрузки данных таблицу можно заново сгенерировать с нуля.",
                                 QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Cancel)==QMessageBox::Cancel) {
            // отмена выбора
            ui->combo_sexage->setCurrentIndex(ui->combo_sexage->findData(-1));
        } else {
            // выбор половозрастной
            id_sexage = ui->combo_sexage->currentData().toInt();
        }
    }
}

void load_sexage_wnd::on_bn_process_clicked() {
    if (id_sexage<0) {
        QMessageBox::warning(this, "Ничего не выбрано",
                             "Для загрузки половозрастной таблицы от ТФОМС сначала выберите таблицу в базе данных, которую хотите подменить фондовскими данными.");
        return;
    }
    QString text = ui->te_data->toPlainText().trimmed();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Поле данных не заполнено",
                             "Для загрузки половозрастной таблицы от ТФОМС сначала скопируйте в поле данных информацию из таблицы, полученной от ТФОМС.");
        return;
    }

    ui->lab_wait->setText("  ...  ПОДГРУЗКА ПОЛОВОЗРАСТНОЙ ТФОМС  ...  ");
    ui->pan_wait->setVisible(true);
    QApplication::processEvents();

    db.transaction();

    // сбросим старые данные прикрепления
    QString sql_clear = "update assig.sexages_lines sl "
                        "   set m_0_0   = 0, "
                        "       m_1_4   = 0, "
                        "       m_5_17  = 0, "
                        "       m_18_59 = 0, "
                        "       m_60_   = 0, "
                        "       m_sum   = 0, "
                        "       w_0_0   = 0, "
                        "       w_1_4   = 0, "
                        "       w_5_17  = 0, "
                        "       w_18_54 = 0, "
                        "       w_55_   = 0, "
                        "       w_sum   = 0, "
                        "       sum     = 0, "
                        "       snils_mt= 0  "
                        " where id_sexage= " + QString::number(id_sexage) + " ; ";
    QSqlQuery *query_clear = new QSqlQuery(db);
    bool res_clear = mySQL.exec(this, sql_clear, QString("сохраним строку половозрастной"), *query_clear, true, db, data_app);
    if (!res_clear) {
        delete query_clear;
        this->setCursor(Qt::ArrowCursor);
        db.rollback();
        QMessageBox::warning(this, "Ошибка при попытке сохранить строку половозрастной",
                             "При попытке сохранить строку половозрастной произошла неожиданная ошибка");
        ui->pan_wait->setVisible(false);
        QApplication::processEvents();
        return;
    }
    query_clear->next();
    int cnt = query_clear->value(0).toInt();
    delete query_clear;


    // разбор CSV
    QStringList col_names;
    QVector<QStringList> data_lists;
    int data_lists_cnt = 0;

    QString text_s = ui->te_data->toPlainText();
    if (read_csvData(text_s, data_lists, data_lists_cnt, this)) {
        // собственно загрузка данных
        int     N;
        QString code_mo;
        QString name_mo;
        int     sum, m_0_0, m_1_4, m_5_17, m_18_59, m_60_, m_sum, w_0_0, w_1_4, w_5_17, w_18_54, w_55_, w_sum, snils_mt;

        int n = 0;
        for (int i=0; i<data_lists[0].size(); i++) {
            // прочитаем значения колонок
            N        = data_lists[ 0].at(i).toInt();
            name_mo  = data_lists[ 1].at(i);
            sum      = data_lists[ 2].at(i).toInt();

            m_0_0    = data_lists[ 3].at(i).toInt();
            m_1_4    = data_lists[ 4].at(i).toInt();
            m_5_17   = data_lists[ 5].at(i).toInt();
            m_18_59  = data_lists[ 6].at(i).toInt();
            m_60_    = data_lists[ 7].at(i).toInt();
            m_sum    = m_0_0 + m_1_4 + m_5_17 + m_18_59 + m_60_;

            w_0_0    = data_lists[ 8].at(i).toInt();
            w_1_4    = data_lists[ 9].at(i).toInt();
            w_5_17   = data_lists[10].at(i).toInt();
            w_18_54  = data_lists[11].at(i).toInt();
            w_55_    = data_lists[12].at(i).toInt();
            w_sum    = w_0_0 + w_1_4 + w_5_17 + w_18_54 + w_55_;

            snils_mt = data_lists[13].at(i).toInt();

            name_mo  = name_mo.trimmed().simplified().toUpper();

            // определим реестовый номер медорганизации по названию
            code_mo = "";
            if (name_mo.indexOf("ЭРТИЛЬСКАЯ РБ")>=0)                               code_mo = "360356";
            else if (name_mo.indexOf("ХОХОЛЬСКАЯ РБ")>=0)                          code_mo = "360354";
            else if (name_mo.indexOf("ТЕРНОВСКАЯ РБ")>=0)                          code_mo = "360353";
            else if (name_mo.indexOf("ТАЛОВСКАЯ РБ")>=0)                           code_mo = "360350";
            else if (name_mo.indexOf("СЕМИЛУКСКАЯ РБ")>=0)                         code_mo = "360346";
            else if (name_mo.indexOf("РОССОШАНСКАЯ РБ")>=0)                        code_mo = "360344";
            else if (name_mo.indexOf("РЕПЬЕВСКАЯ РБ")>=0)                          code_mo = "360342";
            else if (name_mo.indexOf("РАМОНСКАЯ РБ")>=0)                           code_mo = "360341";
            else if (name_mo.indexOf("ПОДГОРЕНСКАЯ РБ")>=0)                        code_mo = "360340";
            else if (name_mo.indexOf("ПОВОРИНСКАЯ РБ")>=0)                         code_mo = "360339";
            else if (name_mo.indexOf("ПЕТРОПАВЛОВСКАЯ РБ")>=0)                     code_mo = "360338";
            else if (name_mo.indexOf("ПАНИНСКАЯ РБ")>=0)                           code_mo = "360336";
            else if (name_mo.indexOf("ПАВЛОВСКАЯ РБ")>=0)                          code_mo = "360333";
            else if (name_mo.indexOf("ОТДЕЛЕНЧЕСКАЯ БОЛЬНИЦА НА СТ. ЛИСКИ")>=0)    code_mo = "360124";
            else if (name_mo.indexOf("ОСТРОГОЖСКАЯ РБ")>=0)                        code_mo = "360331";
            else if (name_mo.indexOf("ОЛЬХОВАТСКАЯ РБ")>=0)                        code_mo = "360330";
            else if (name_mo.indexOf("НОВОХОПЕРСКАЯ РБ")>=0)                       code_mo = "360328";
            else if (name_mo.indexOf("НОВОУСМАНСКАЯ РБ")>=0)                       code_mo = "360326";
            else if (name_mo.indexOf("НИЖНЕДЕВИЦКАЯ РБ")>=0)                       code_mo = "360325";
            else if (name_mo.indexOf("МСЧ № 97 ФМБА РОССИИ")>=0)                   code_mo = "360022";
            else if (name_mo.indexOf("МСЧ № 33 ФМБА РОССИИ")>=0)                   code_mo = "360021";
            else if (name_mo.indexOf("МСЧ МВД РОССИИ ПО ВОРОНЕЖСКОЙ ОБЛАСТИ")>=0)  code_mo = "360160";
            else if (name_mo.indexOf("ЛИСКИНСКАЯ РБ")>=0)                          code_mo = "360322";
            else if (name_mo.indexOf("КАШИРСКАЯ РБ")>=0)                           code_mo = "360321";
            else if (name_mo.indexOf("КАНТЕМИРОВСКАЯ РБ")>=0)                      code_mo = "360319";
            else if (name_mo.indexOf("КАМЕНСКАЯ РБ")>=0)                           code_mo = "360318";
            else if (name_mo.indexOf("КАЛАЧЕЕВСКАЯ РБ")>=0)                        code_mo = "360316";
            else if (name_mo.indexOf("КЛИНИЧЕСКАЯ БОЛЬНИЦА НА СТ.ВОРОНЕЖ-1")>=0)   code_mo = "360090";
            else if (name_mo.indexOf("ГРИБАНОВСКАЯ РБ")>=0)                        code_mo = "360315";
            else if (name_mo.indexOf("ВОРОБЬЕВСКАЯ РБ")>=0)                        code_mo = "360313";
            else if (name_mo.indexOf("ВОКБ №2")>=0)                                code_mo = "360013";
            else if (name_mo.indexOf("ВЕРХНЕХАВСКАЯ РБ")>=0)                       code_mo = "360312";
            else if (name_mo.indexOf("ВЕРХНЕМАМОНСКАЯ РБ")>=0)                     code_mo = "360311";
            else if (name_mo.indexOf("ВГП № 8")>=0)                                code_mo = "360040";
            else if (name_mo.indexOf("ВГП № 7")>=0)                                code_mo = "360043";
            else if (name_mo.indexOf("ВГП № 3")>=0)                                code_mo = "360025";
            else if (name_mo.indexOf("ВГП № 22")>=0)                               code_mo = "360521";
            else if (name_mo.indexOf("ВГП № 18")>=0)                               code_mo = "360032";
            else if (name_mo.indexOf("ВГП № 11")>=0)                               code_mo = "360101";
            else if (name_mo.indexOf("ВГП № 10")>=0)                               code_mo = "360037";
            else if (name_mo.indexOf("ВГП № 1")>=0 || name_mo.indexOf("ВГП №1")>=0)code_mo = "360046";
            else if (name_mo.indexOf("ВГКП № 4")>=0)                               code_mo = "360050";
            else if (name_mo.indexOf("ВГКБ № 20")>=0)                              code_mo = "360083";
            else if (name_mo.indexOf("ВГКБ № 11")>=0)                              code_mo = "360086";
            else if (name_mo.indexOf("ВГБ №16")>=0)                                code_mo = "360110";
            else if (name_mo.indexOf("ВГБ № 5")>=0)                                code_mo = "360029";
            else if (name_mo.indexOf("ВГБ № 4")>=0)                                code_mo = "360117";
            else if (name_mo.indexOf("ВГБ № 14")>=0)                               code_mo = "360104";
            else if (name_mo.indexOf("БУТУРЛИНОВСКАЯ РБ")>=0)                      code_mo = "360305";
            else if (name_mo.indexOf("БОРИСОГЛЕБСКАЯ РБ")>=0)                      code_mo = "360310";
            else if (name_mo.indexOf("БОГУЧАРСКАЯ РБ")>=0)                         code_mo = "360309";
            else if (name_mo.indexOf("БОБРОВСКАЯ РБ")>=0)                          code_mo = "360303";
            else if (name_mo.indexOf("АННИНСКАЯ РБ")>=0)                           code_mo = "360300";

            if (!code_mo.isEmpty()) {
                // сохраним строку половозрастной
                QString sql_save = "update assig.sexages_lines sl "
                                   "   set m_0_0="     + QString::number(m_0_0)     + ", "
                                   "       m_1_4="     + QString::number(m_1_4)     + ", "
                                   "       m_5_17="    + QString::number(m_5_17)    + ", "
                                   "       m_18_59="   + QString::number(m_18_59)   + ", "
                                   "       m_60_="     + QString::number(m_60_)     + ", "
                                   "       m_sum="     + QString::number(m_sum)     + ", "
                                   "       w_0_0="     + QString::number(w_0_0)     + ", "
                                   "       w_1_4="     + QString::number(w_1_4)     + ", "
                                   "       w_5_17="    + QString::number(w_5_17)    + ", "
                                   "       w_18_54="   + QString::number(w_18_54)   + ", "
                                   "       w_55_="     + QString::number(w_55_)     + ", "
                                   "       w_sum="     + QString::number(w_sum)     + ", "
                                   "       sum="       + QString::number(sum)       + ", "
                                   "       snils_mt="  + QString::number(snils_mt)  + "  "
                                   " where id_sexage=" + QString::number(id_sexage) + "  "
                                   "   and code_head='"  + code_mo                    + "' ; ";
                QSqlQuery *query_save = new QSqlQuery(db);
                bool res_save = mySQL.exec(this, sql_save, QString("сохраним строку половозрастной"), *query_save, true, db, data_app);
                if (!res_save) {
                    delete query_save;
                    this->setCursor(Qt::ArrowCursor);
                    db.rollback();
                    QMessageBox::warning(this, "Ошибка при попытке сохранить строку половозрастной",
                                         "При попытке сохранить строку половозрастной произошла неожиданная ошибка");

                    ui->pan_wait->setVisible(false);
                    QApplication::processEvents();
                    return;
                }
                query_save->next();
                int cnt = query_save->value(0).toInt();
                delete query_save;
            }
        }

        // отметим источник половозрастной - ТФОМС
        QString sql_sour = "update assig.sexages s "
                           "   set source='ТФОМС'  "
                           " where id=" + QString::number(id_sexage) + " ; ";
        QSqlQuery *query_sour = new QSqlQuery(db);
        bool res_sour = mySQL.exec(this, sql_sour, QString("Отметка о перезагрузке половозрастной"), *query_sour, true, db, data_app);
        if (!res_sour) {
            delete query_sour;
            this->setCursor(Qt::ArrowCursor);
            db.rollback();
            QMessageBox::warning(this, "Ошибка при попытке поставить отметку о перезагрузке половозрастной",
                                 "При попытке поставить отметку о перезагрузке половозрастной произошла неожиданная ошибка");

            ui->pan_wait->setVisible(false);
            QApplication::processEvents();
            return;
        }
        query_sour->next();
        int cnt = query_sour->value(0).toInt();
        delete query_sour;


        ui->pan_wait->setVisible(false);
        QApplication::processEvents();

        // все данные загружены
        db.commit();
        QMessageBox::warning(this, "Готово", "Готово.");
    }
    accept();
}
