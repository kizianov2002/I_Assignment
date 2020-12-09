#-------------------------------------------------
#
# Project created by QtCreator 2015-06-17T09:04:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = _INKO_ASSIGNMENT
TEMPLATE = app
QT += sql
QT += opengl
LIBS += D:/_INKO_ASSIGNMENT_src/_INKO_ASSIGNMENT/lgl.dll

CONFIG -= embeded_manifest_dll

RC_FILE = inko_assig.rc

SOURCES += main.cpp\
        connection_window.cpp \
    assignment_wnd.cpp \
    add_assig_wnd.cpp \
    autoassig_wnd.cpp \
    ki_sql_query_model.cpp \
    sessions_wnd.cpp \
    add_medsite_ter_wnd.cpp \
    snils_list_wnd.cpp \
    show_text_wnd.cpp \
    show_tab_wnd.cpp \
    matchtfoms_wnd.cpp \
    geodata_wnd.cpp \
    myglwidget.cpp \
    old_db_wnd.cpp \
    accounts_base_wnd.cpp \
    ki_funcs.cpp \
    find_FIAS_wnd.cpp \
    reindex_wnd.cpp \
    assiglays_wnd.cpp \
    assiglay_data_wnd.cpp \
    get_date_wnd.cpp \
    assiglay_tfoms_master_wnd.cpp \
    add_sexage_wnd.cpp \
    add_move_wnd.cpp \
    show_sexage_wnd.cpp \
    show_move_wnd.cpp \
    add_matrix_wnd.cpp \
    load_sexage_wnd.cpp \
    letters2mo_wnd.cpp \
    medsites_reg_wnd.cpp \
    finance_medorgs_wnd.cpp \
    finance_mo_coef_wnd.cpp \
    add_person_wnd.cpp \
    add_assig_rec_wnd.cpp \
    import4csv_wnd.cpp \
    reports_assig2mo_wnd.cpp \
    print_letters_wnd.cpp \
    print_letters_data_wnd.cpp

HEADERS  += connection_window.h \
    s_data_app.h \
    assignment_wnd.h \
    add_assig_wnd.h \
    s_data_assig.h \
    autoassig_wnd.h \
    ki_sql_query_model.h \
    sessions_wnd.h \
    add_medsite_ter_wnd.h \
    snils_list_wnd.h \
    show_text_wnd.h \
    show_tab_wnd.h \
    ki_exec_query.h \
    matchtfoms_wnd.h \
    geodata_wnd.h \
    myglwidget.h \
    old_db_wnd.h \
    accounts_base_wnd.h \
    ki_funcs.h \
    find_FIAS_wnd.h \
    reindex_wnd.h \
    assiglays_wnd.h \
    assiglay_data_wnd.h \
    get_date_wnd.h \
    s_data_pers.h \
    assiglay_tfoms_master_wnd.h \
    add_sexage_wnd.h \
    add_move_wnd.h \
    show_sexage_wnd.h \
    show_move_wnd.h \
    add_matrix_wnd.h \
    load_sexage_wnd.h \
    letters2mo_wnd.h \
    medsites_reg_wnd.h \
    finance_medorgs_wnd.h \
    finance_mo_coef_wnd.h \
    add_person_wnd.h \
    add_assig_rec_wnd.h \
    import4csv_wnd.h \
    reports_assig2mo_wnd.h \
    print_letters_wnd.h \
    print_letters_data_wnd.h

FORMS    += connection_window.ui \
    assignment_wnd.ui \
    add_assig_wnd.ui \
    autoassig_wnd.ui \
    sessions_wnd.ui \
    add_medsite_ter_wnd.ui \
    add_medsite_ter_wnd.ui \
    snils_list_wnd.ui \
    show_text_wnd.ui \
    show_tab_wnd.ui \
    matchtfoms_wnd.ui \
    geodata_wnd.ui \
    old_db_wnd.ui \
    accounts_base_wnd.ui \
    find_FIAS_wnd.ui \
    reindex_wnd.ui \
    assiglays_wnd.ui \
    assiglay_data_wnd.ui \
    get_date_wnd.ui \
    assiglay_tfoms_master_wnd.ui \
    add_sexage_wnd.ui \
    add_move_wnd.ui \
    show_sexage_wnd.ui \
    show_move_wnd.ui \
    add_matrix_wnd.ui \
    load_sexage_wnd.ui \
    letters2mo_wnd.ui \
    medsites_reg_wnd.ui \
    finance_medorgs_wnd.ui \
    finance_mo_coef_wnd.ui \
    add_person_wnd.ui \
    add_assig_rec_wnd.ui \
    import4csv_wnd.ui \
    reports_assig2mo_wnd.ui \
    print_letters_wnd.ui \
    print_letters_data_wnd.ui

win32:LIBS             += -lsetupapi -ladvapi32 -luser32

DISTFILES += \
    _inko_assig.rc \
    inko_asg.ico

RESOURCES += \
    _inko_assig_res.qrc
