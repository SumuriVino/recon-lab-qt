#include "thread_zip_file_export.h"

thread_zip_file_export::thread_zip_file_export()
{

}

void thread_zip_file_export::run()
{ // run thread for exporting as a zip file


    QuaZip zip(zip_file_path);
    QDir().mkpath(QFileInfo(zip_file_path).absolutePath());
    if(!zip.open(QuaZip::mdCreate)) {
        QFile::remove(zip_file_path);
        return ;
    }


    for(int file_count = 0; file_count < st_file_export_list_obj.size(); file_count++)
    {
        if(bool_cancel_export)
            break ;
        compressFile( &zip,   st_file_export_list_obj.at(file_count).source_path,st_file_export_list_obj.at(file_count).destination_path);
    }


    zip.close();
    if(zip.getZipError()!=0) {
        QFile::remove(zip_file_path);
        return ;
    }

}

void thread_zip_file_export::pub_set_essentials(QString zip_fpath, QList<struct_global_file_for_zip_export> list, bool bool_enc_zip, QString passwd_str)
{ // set essentials database paths
    zip_file_path = zip_fpath;

    st_file_export_list_obj = list;

    bool_cancel_export = false;

    bool_is_enc_zip  = bool_enc_zip;
    password_enc_zip = passwd_str;

}

void thread_zip_file_export::pub_cancel_export()
{
    bool_cancel_export = true;

}


bool thread_zip_file_export::compressFile(QuaZip* zip, QString fileName, QString fileDest)
{ // compress file or source in zip format
    if (!zip) return false;
    if (zip->getMode()!=QuaZip::mdCreate &&
            zip->getMode()!=QuaZip::mdAppend &&
            zip->getMode()!=QuaZip::mdAdd) return false;

    // Apro il file originale
    QFile inFile;
    inFile.setFileName(fileName);
    if(!inFile.open(QIODevice::ReadOnly)) return false;


    QuaZipNewInfo ff =  QuaZipNewInfo(fileDest,  inFile.fileName());


    QuaZipFile outFile(zip);
   // if(!outFile.open(QIODevice::WriteOnly, ff)) return false;
    bool open_status = false;
    if(bool_is_enc_zip)
        open_status = outFile.open(QIODevice::WriteOnly, ff, password_enc_zip.toLocal8Bit().data());
    else
        open_status = outFile.open(QIODevice::WriteOnly, ff);

    if(!open_status)
        return false;


    if (!copyData(inFile, outFile) || outFile.getZipError()!=UNZ_OK) {
        return false;
    }

    outFile.close();
    if (outFile.getZipError()!=UNZ_OK) return false;
    inFile.close();

    return true;
}


bool thread_zip_file_export::copyData(QIODevice &inFile, QIODevice &outFile)
{ // copy data on destination folder
    qint64 counter = 0;
    while (!inFile.atEnd()) {
        char buf[4096];
        qint64 readLen = inFile.read(buf, 4096);
        if (readLen <= 0)
            return false;
        if (outFile.write(buf, readLen) != readLen)
            return false;

        counter++;
        if(counter % 100 == 0)
        {
            counter = 0;
            QApplication::processEvents();
        }

        if(bool_cancel_export)
            break ;

    }
    return true;
}
