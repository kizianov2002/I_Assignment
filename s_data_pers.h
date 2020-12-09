#ifndef S_DATA_PERS
#define S_DATA_PERS

typedef struct {
    int id;
    int pol_v;
    QString fam, im, ot, snils, enp;
    int sex;
    QDate date_birth, date_death, date_reg;
    int status;
    bool bomj;
    bool has_date_insure_begin, has_date_insure_end;
    QDate date_insure_begin, date_insure_end;
    int id_udl, id_drp, id_pol, id_asg;
} s_data_pers;

#endif // S_DATA_PERS

