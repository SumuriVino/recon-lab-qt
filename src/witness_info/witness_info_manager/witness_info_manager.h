#ifndef WITNESS_INFO_MANAGER_H
#define WITNESS_INFO_MANAGER_H

#include "recon_generic_functions/recon_static_functions.h"
#include <QtCore>
#include <QObject>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "header_global_structures.h"
#include "header_identical_1.h"
#include "header_global_variables_2.h"

class witness_info_manager : public QObject
{
    Q_OBJECT

public:
    witness_info_manager(QWidget *parent = 0);
    ~witness_info_manager();

    void pub_fill_witness_info_QList_from_store();

    QList<struct_GLOBAL_witness_info_root> pub_get_root_structure_QList();
    struct_GLOBAL_witness_info_root pub_get_root_structure_according_root_count_name(QString root_count_name);
    struct_GLOBAL_witness_info_root pub_get_root_structure_according_source_count_name(QString source_count_name);
    void pub_set_updated_root_structure_QList(QList<struct_GLOBAL_witness_info_root> list);


    QList<struct_GLOBAL_witness_info_source> pub_get_source_structure_QList();
    struct_GLOBAL_witness_info_source pub_get_source_structure_according_source_count_name(QString source_count_name);
    void pub_set_updated_source_structure_QList(QList<struct_GLOBAL_witness_info_source> list);

private:

    QList<struct_GLOBAL_witness_info_root> list_witness_info_root;
    QList<struct_GLOBAL_witness_info_source> list_witness_info_source;

    void fill_root_structure_list_from_db();
    void fill_root_structure_list_from_text_file();


    void fill_source_structure_list_from_db();
    void fill_source_structure_list_from_text_file();

    QString get_complete_source_name_from_source(struct_GLOBAL_witness_info_source target_sources_information);

    QList<struct_GLOBAL_snapshots_info> get_apfs_snapshots_QList_for_source(QString source_count_name, QSqlDatabase &sources_db);
    QList<struct_GLOBAL_snapshots_info> get_apfs_snapshots_QList_for_source_from_snapshots_plist(QString source_count_name);

    QList <struct_global_system_account_info> get_system_account_qlist_for_all_users(QString system_user_str);


    QString get_string_with_addbindvalues_by_dbreference(QString command, QStringList values, int pos, QSqlDatabase &destination_db, QString caller_func);


    void update_source_structure_for_root_info(struct_GLOBAL_witness_info_source &struct_src_info);


};

#endif // WITNESS_INFO_MANAGER_H
