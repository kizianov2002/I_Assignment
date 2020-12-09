#ifndef S_DATA_APP
#define S_DATA_APP

typedef struct {
    QString ini_path;
    QString version;

    bool f_readonly;

    // данные подключения к БД
    QString drvr, host, base, user, pass;
    int     port;

    int font_size;

    int terminal_id;
    QString terminal_name;
    int terminal_status;
    int terminal_id_point;

    int id_session;

    QString db_host;
    QString db_name;
    QString db_vers;

    int     id_point;
    QString point_regnum;
    QString point_name;
    int     id_operator;
    QString operator_fio;

    QString ocato_full;
    int     KLADR_subj;
    int     KLADR_dstr;
    int     KLADR_city;
    int     KLADR_nasp;
    int     KLADR_strt;
    int     KLADR_house;
    QString KLADR_corp;

    QString path_install;
    QString path_arch;
    QString path_reports;
    QString path_temp;
    QString path_foto;
    QString path_sign;
    QString path_in;
    QString path_out;
    QString path_in_backup;
    QString path_out_backup;
    QString path_dbf;

    QString terr_name;
    QString terr_name_rp;
    QString ocato;
    QString ocato_text;

    QString smo_ogrn;
    QString smo_regnum;
    QString smo_logo1;
    QString smo_logo2;
    QString smo_logo3;
    QString smo_short;
    QString smo_name;
    QString smo_name_full;
    QString smo_name_full_rp;
    QString smo_director;
    QString smo_oms_chif;
    QString smo_address;
    QString smo_contact_info;
    QString smo_master_state;
    QString smo_master_FamIO;

    QString tfoms_director;
    QString tfoms_director_2;
    QString tfoms_director_fio;
    QString tfoms_chif;

    QString filial_name;
    QString filial_name_rp;
    QString filial_city;
    QString filial_by_city;
    QString filial_by_city_tp;
    QString filial_director;
    QString filial_director_2;
    QString filial_director_fio;
    QString filial_chif;
    QString filial_phone;
    QString filial_phone_2;
    QString filial_email;
    QString filial_email_2;

    QString letter_doer1_FamIO;
    QString letter_doer1_phone;
    QString letter_doer2_FamIO;
    QString letter_doer2_phone;


    /*QString smo_ogrn;
    QString smo_regnum;
    QString smo_name;
    QString smo_name_full;
    QString smo_name_full_rp;
    QString smo_director;
    QString smo_oms_chif;
    QString smo_address;
    QString smo_contact_info;
    QString smo_logo1;
    QString smo_logo2;
    QString smo_logo3;

    QString terr_name;
    QString terr_name_rp;
    QString tfoms_director;
    QString tfoms_director_2;
    QString tfoms_director_fio;
    QString filial_director;
    QString filial_director_2;
    QString filial_director_fio;

    bool csv_headers;       // 0 - в CSV-файле прикреплений не добавлять заголовки

    QString xml_vers;
    QString ocato;
    QString ocato_text;
    int split_by_ot;        // 0 - все данные одним файлом, 1 - люди без отчества отдельным файлом
    int snils_format;       // 0 - только цифры ХХХХХХХХХХХ, 1 - с разделителями ХХХ-ХХХ-ХХХ ХХ
    int send_p021_in_p060;  // 0 - при П060 не отсылать П021 на ВС, 1 - при закрытии ВС всегда шлём П021
    int pol_start_date_by_vs;  // 0 - при дата актиации полиса единого образца - дате выдачи ВС, 1 - у каждого полиса своя дата выдачи
    bool f_zip_packs;

    int font_size;

    int vs_blank_width_mm;
    int vs_blank_height_mm;
    int vs_field_left_mm;
    int vs_field_right_mm;
    int vs_field_top_mm;
    int vs_field_bottom_mm;*/

    // филиалы
    QString filials_list;
    QString kursk_drvr, kursk_host, kursk_base, kursk_user, kursk_pass;
    int kursk_port;
    QString belg_drvr, belg_host, belg_base, belg_user, belg_pass;
    int belg_port;
    QString voro_drvr, voro_host, voro_base, voro_user, voro_pass;
    int voro_port;
    QString work_drvr, work_host, work_base, work_user, work_pass;
    int work_port;

    bool is_tech;
    bool is_head;
    bool is_locl;
    bool is_call;

    // подключение к ФИАС
    bool FIAS_flag;     // признак, что в выгрузки для ТФОМС надо использовать ФИАС
    QString FIAS_db_drvr;
    QString FIAS_db_host;
    QString FIAS_db_name;
    QString FIAS_db_port;
    QString FIAS_db_user;
    QString FIAS_db_pass;
    QString FIAS_db_admn;
    QString FIAS_db_admp;
    QString FIAS_db_connstring;

    // подгрузка данных из старой базы полисов
    QString old_db_drvr;
    QString old_db_host;
    QString old_db_name;
    QString old_db_port;
    QString old_db_user;
    QString old_db_pass;

    // подключение к базе для отдела счетов
    QString acc_db_drvr;
    QString acc_db_host;
    QString acc_db_name;
    QString acc_db_port;
    QString acc_db_user;
    QString acc_db_pass;

} s_data_app;

#endif // S_DATA_APP

