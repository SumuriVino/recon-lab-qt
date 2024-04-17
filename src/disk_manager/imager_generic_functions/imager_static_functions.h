#ifndef RECON_STATIC_FUNCTIONS_H
#define RECON_STATIC_FUNCTIONS_H

#include <QtCore>
#include <QtWidgets>
#include "disk_manager/imager_global_headers.h"
#include "openssl/md5.h"
#include "openssl/sha.h"
#include <QtSql>

class imager_static_functions : public QWidget
{

public:
    explicit imager_static_functions(QWidget *parent = 0);

    static QString convert_cfstring_to_string_xcode_mm(CFStringRef str, QString caller_func);
    static QString get_nsstring_values_from_nsdictionary_xcode_mm(void *ptr_nsstring, void *ptr_nsdictionary, QString caller_func);

    static  qint64 get_nsnumber_values_qint64(void *ptr_nsstring, void *ptr_nsdictionary, QString caller_func);
    static QString human_readable_size(qint64 size_in_qint64, QString caller_func);
    static QString get_available_filename(QString file_name, QString dir_path, QString caller_func);
    static QString get_available_or_existing_dirname(QString dir_name, QString dir_path, QString caller_func);

    static bool is_file_exist_in_dir(QString dir_path, QString filename, QString caller_func);

    static bool execute_db_command_by_dbpath(QString command, QString destination_db_file, QString caller_func);
    static bool execute_db_command_with_addbindvalues_by_dbpath(QString command, QStringList values, QString destination_db_file, QString caller_func);
};
#endif
