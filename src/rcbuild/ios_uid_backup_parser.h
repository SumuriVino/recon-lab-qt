
#ifndef IOS_UID_BACKUP_PARSER_H
#define IOS_UID_BACKUP_PARSER_H

#include <QObject>


class ios_uid_backup_parser : public QObject
{
    Q_OBJECT
public:
    explicit ios_uid_backup_parser(QObject *parent = 0);



    virtual QStringList pub_get_artifact_source_file_from_directory_scan(QString domain_name);
    virtual QString pub_get_artifact_source_file_uid_path(QString file_path_from_domain);






private:


};

#endif // IOS_UID_BACKUP_PARSER_H









