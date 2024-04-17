#ifndef IOS_XML_BACKUP_PARSER_H
#define IOS_XML_BACKUP_PARSER_H

#include <QObject>
#include <QtCore>
#include <QtXml>
#include "recon_generic_functions/recon_static_functions.h"
#include "ios_xml_header_and_structure.h"
#include "header_global_variables_3.h"

class ios_xml_backup_parser : public QObject
{
    Q_OBJECT
public:
    explicit ios_xml_backup_parser(QObject *parent = nullptr);

    void pub_set_ios_xml_file_path(QString path);

    void pub_parse_ios_xml_device_info();
    struct_ios_xml_device_info pub_get_struct_device_info_obj();

    void pub_parse_ios_xml_files_info();
    QList<struct_ios_xml_files_info> pub_get_struct_files_info_qlist();


signals:

private:

    QString ios_xml_report_path;

    void parse_ios_xml_device_info(QDomElement root);
    struct_ios_xml_device_info struct_device_info_obj;
    void clear_struct_device_info_variable();

    void parse_ios_xml_files_info(QDomElement root);
    QList<struct_ios_xml_files_info> struct_ios_xml_files_info_qlist;


};

#endif // IOS_XML_BACKUP_PARSER_H
