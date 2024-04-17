#ifndef IOS_UID_BACKUP_EXTRACTOR_H
#define IOS_UID_BACKUP_EXTRACTOR_H

#include <QMainWindow>
#include <QtCore>
#include <QCryptographicHash>
#include <QtSql/QtSql>

#include "recon_generic_functions/recon_static_functions.h"

class ios_uid_backup_extractor
{
public:
    ios_uid_backup_extractor();
    ~ios_uid_backup_extractor();

    void pub_extract_backup(QString source, QString destination_path);
    void pub_set_full_os_version(QString version);

private:
    struct struct_mbdb_data_extractor
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

    void create_database_table_7_8_9();
    void prepare_structure_and_database_7_8_9();

    void create_database_table_10_11();
    void prepare_structure_and_database_10_11();

    int offset;

    QByteArray barray_entry_end_index;
    int int_checker_for_mbdb_insertion;

    QByteArray file_data;
    QString source_backup_path;

    void fill_value_in_mbdb_structure(struct struct_mbdb_data_extractor *ptr);
    void backup_spawn_func();
    void file_copier(QString file_to_copied, QString source_id);

    QString extraction_destination_path;

    QString os_version_stripped;
};

#endif // IOS_UID_BACKUP_EXTRACTOR_H
