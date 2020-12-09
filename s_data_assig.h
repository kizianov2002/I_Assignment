#ifndef S_DATA_ASSIG
#define S_DATA_ASSIG

typedef struct {
    int id;
    QString ocato;
    int subj, dstr, city, nasp, strt;
    int right_type; // 1 - дом, 2 - диапазон номеров, 3 - вся улица, 4 - открытый диапазон номеров, 5 - произвольный список номеров домов
    int house;
    QString corp;
    int house2;
    QString corp2;
    int sex;
    int age;
    QString code_mo, code_mp, code_ms, snils_mt;
    bool from_start, to_end;
    QString comment;
} s_data_assig;

#endif // S_DATA_ASSIG

