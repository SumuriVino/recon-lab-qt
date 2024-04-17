#ifndef THREAD_ZIP_FILE_EXPORT_H
#define THREAD_ZIP_FILE_EXPORT_H

#include <QObject>
#include <QThread>
#include <QtCore>
#include "recon_generic_functions/recon_static_functions.h"
#include "zipper/JlCompress.h"

class thread_zip_file_export : public QThread
{
public:
    thread_zip_file_export();

    void run();

    void pub_set_essentials(QString zip_fpath, QList<struct_global_file_for_zip_export> list, bool bool_enc_zip, QString passwd_str);

    void pub_cancel_export();

private:
    bool bool_cancel_export;

    QList<struct_global_file_for_zip_export> st_file_export_list_obj;

    QString zip_file_path;
    bool compressFile(QuaZip *zip, QString fileName, QString fileDest);
    bool copyData(QIODevice &inFile, QIODevice &outFile);

    bool bool_is_enc_zip = false;
    QString password_enc_zip;

};

#endif // THREAD_ZIP_FILE_EXPORT_H
