#ifndef IOS_UID_BACKUP_PARSER_H
#define IOS_UID_BACKUP_PARSER_H

#include <QObject>
#include <QtCore>
#include <QList>
#include <QtSql/QtSql>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"                         

class ios_uid_backup_parser : public QObject
{
    Q_OBJECT
public:
    explicit ios_uid_backup_parser(QObject *parent = 0);

    void pub_set_backup_path(QString path);
    void pub_set_os_version(QString version);


    void pub_parse_backup();


    QString ios_uid_backup_path;
    QString os_version_stripped;
    QList<struct_global_main_node_entry_for_ios_7_8_9> list_node_entry_for_ios_7_8_9;
    QList<struct_global_main_node_entry_for_ios_10_11> list_node_entry_for_ios_10_11;


    virtual QStringList pub_get_artifact_source_file_from_directory_scan(QString domain_name);
    virtual QString pub_get_artifact_source_file_uid_path(QString file_path_from_domain);


private:

    struct struct_mbdb_data
    {
        QByteArray domain;
        QByteArray filename;
        QByteArray fullpath;
        QByteArray fileID;
        QByteArray linktarget;
        QByteArray datahash;
        QByteArray unknown1;
        QByteArray mode;
        QByteArray unknown2;
        QByteArray unknown3;
        QByteArray userid;
        QByteArray groupid;
        QByteArray mtime;
        QByteArray atime;
        QByteArray ctime;
        QByteArray filelen;
        QByteArray flags;
        QByteArray numprops;
        QByteArray propname;
        QByteArray propval;
    };

    void start_extraction_7_8_9();
    void start_extraction_10_11();

    QByteArray file_data;
    QByteArray barray_entry_end_index;

    int offset;
    int int_checker_for_mbdb_insertion;
    void fill_value_in_mbdb_structure(struct struct_mbdb_data *ptr);
    void insert_into_root_node(struct struct_mbdb_data data_contnr);

    struct_global_main_node_entry_for_ios_10_11 node_entry_for_ios_10_11;


};

#endif // IOS_UID_BACKUP_PARSER_H
