#include "recon_static_functions.h"
#include <QApplication>
#include <QtCore>
#include <QtGui>


QString recon_static_functions::recon_result_path = QString();
QString recon_static_functions::debug_file_path_regular = QString();
QString recon_static_functions::debug_file_path_examiner = QString();
QString recon_static_functions::debug_file_path_source = QString();
QString recon_static_functions::debug_file_path_root = QString();
QString recon_static_functions::debug_file_path_sumuri = QString();

qint64 recon_static_functions::recon_jaari_smay = 0;
QFile recon_static_functions::debug_file_regular_ptr;


recon_static_functions::recon_static_functions(QWidget *parent)
{
    Q_UNUSED(parent)
}

QString recon_static_functions::get_recon_library_version()
{
    /* Always give main version in 3 numeric digit like 1.1.1
     * Always give main version in ascending order (1.1.2 OR 2.0.0) comparatively last given version(1.1.1)
     * Mean 112 > 111  OR 200 > 111
    */

    return QString("1.6.2");

}

QString recon_static_functions::get_app_version()
{
    /* Always give main version in 3 numeric digit like 1.1.1
     * Always give main version in ascending order (1.1.2 OR 2.0.0) comparatively last given version(1.1.1)
     * Mean 112 > 111  OR 200 > 111
    */

    return QString("1.6.2");

}

QString recon_static_functions::get_app_build_modified()
{
    //QString str = " (" + get_app_build_regular() + ") - TestDebugBuild";
    QString str = " (" + get_app_build_regular() + ") - Silicon";

    //    QString str = " (" + get_app_build_regular() + ") Silicon - Test Build";

    //     QString str = " (" + get_app_build_regular() + ")";
    return str;

}

QString recon_static_functions::get_app_build_regular()
{
    // [A1 to A9]
    // [Z1 to Z9]

    // D - For Department [D1,D2....]

    return "A1";
}


QString recon_static_functions::get_code_timestamp()
{

#ifdef MACRO_RECON_STATIC_APP_RUN_BY_FORENSO
    return QString(" - [Feb.06.2023 - 12:30]");
#endif

    return "";
}

void recon_static_functions::set_recon_jaari_smay()
{
    // 1672145706  GMT: Tuesday, 27 December 2022 12:55:06


    QString t_1 = QString::number(1);
    QString t_2 = QString::number(6);
    QString t_3 = QString::number(7);

    QString t_4 = QString::number(2);
    QString t_5 = QString::number(1);
    QString t_6 = QString::number(4);

    QString t_7 = QString::number(5);
    QString t_8 = QString::number(7);

    QString t_9 = QString::number(0);
    QString t_10 = QString::number(6);

    QString str;
    str.append(t_1).append(t_2).append(t_3).append(t_4).append(t_5).append(t_6).append(t_7).append(t_8).append(t_9).append(t_10);

    recon_jaari_smay =  str.toLong();

}

QString recon_static_functions::get_app_name()
{
    return QString("RECON LAB");
}

QString recon_static_functions::get_appilcation_relative_path()
{
    QString app_pth = QApplication::applicationDirPath();

    app_pth = app_pth.trimmed();
    app_pth = app_pth.remove(app_pth.size() - 6, 10); // /Macos
    app_pth = app_pth.trimmed();

    return app_pth;
}

QString recon_static_functions::get_debug_filepath_regular()
{
    return debug_file_path_regular;
}

QString recon_static_functions::get_debug_filepath_examiner()
{
    return debug_file_path_examiner;
}


QString recon_static_functions::get_debug_filepath_root()
{
    return debug_file_path_root;
}

QString recon_static_functions::get_debug_filepath_source()
{
    return debug_file_path_source;
}

qint64 recon_static_functions::get_recon_jaari_smay()
{
    return recon_jaari_smay;
}

bool recon_static_functions::copy_directory(QString received_source_path,QString received_dest_path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    received_source_path = received_source_path.trimmed();
    received_dest_path   = received_dest_path.trimmed();

    if(received_source_path.isEmpty() || received_dest_path.isEmpty() || received_source_path == QString("/"))
        return false;

    QDir source_dir(received_source_path);


    if(!source_dir.exists())
        return false;


    QDir dest_dir(received_dest_path);

    if(!dest_dir.exists())
    {
        dest_dir.mkpath(received_dest_path);
    }

    QStringList files = source_dir.entryList(QDir::Files | QDir::Hidden | QDir::System);

    for(int i = 0; i< files.count(); i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QFileInfo info;
        QString src_name = received_source_path + "/" + files[i];
        QString dest_name = received_dest_path + "/" + files[i];
        info.setFile(src_name);

        QFile source_file(src_name);
        QFile dest_file(dest_name);
        if(source_file.exists())
        {
            if(!source_file.open(QIODevice::ReadOnly))
            {
                recon_static_functions::app_debug(prepare_callerfun(caller_func) + " -- source_file.open  ----FAILED---" + src_name,Q_FUNC_INFO);
                recon_static_functions::app_debug(source_file.errorString(),Q_FUNC_INFO);
                continue;
            }
            if(!dest_file.open(QIODevice::WriteOnly))
            {
                recon_static_functions::app_debug(prepare_callerfun(caller_func) + " -- dest_file.open  ----FAILED---" + dest_name,Q_FUNC_INFO);
                recon_static_functions::app_debug(dest_file.errorString(),Q_FUNC_INFO);
                source_file.close();
                continue;

            }


            while(1)
            {
                // QCoreApplication::processEvents();
                QCoreApplication::processEvents();

                QByteArray dar = source_file.read(102400);
                if(dar.size() <= 0)
                {
                    break;
                }

                dest_file.write(dar);

            }

            source_file.close();
            dest_file.close();

        }
    }

    files.clear();
    files = source_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

    for(int i = 0; i< files.count(); i++)
    {
        QCoreApplication::processEvents();

        QString src_name = received_source_path + "/" + files[i];
        QString dest_name = received_dest_path + "/" + files[i];
        copy_directory(src_name, dest_name, caller_func);
    }

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return true;
}

bool recon_static_functions::is_file_exist_in_dir(QString dir_path, QString rcvd_filename, QString caller_func)
{
    Q_UNUSED(caller_func)
    rcvd_filename = rcvd_filename.trimmed();
    rcvd_filename = rcvd_filename.toLower();

    QDirIterator it(dir_path); // Never ever use any diriterator flags here.
    while(it.hasNext())
    {
        QFileInfo info(it.next());
        QString crnt_filename = info.fileName().trimmed();
        crnt_filename = crnt_filename.toLower();

        if(crnt_filename == rcvd_filename)
        {
            return true;
        }
    }

    return false;
}

void recon_static_functions::set_result_path(QString path)
{
    recon_result_path = path;
}

void recon_static_functions::create_debug_file()
{

    if(!global_variable_debug_mode_status_bool)
        return;

    debug_file_path_regular.clear();
    debug_file_path_examiner.clear();
    debug_file_path_root.clear();
    debug_file_path_source.clear();
    debug_file_path_sumuri.clear();


    if(global_variable_debug_print_in_file_bool)
    {
        QString debug_dir_path =  QString(getenv("HOME")) + "/Desktop/RECON_LOGS/LAB_Logs/" +  get_app_name().replace(" ", "_") + "-" + get_app_version() + "_" + get_app_build_regular() + "-" + QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss") + "/";

        QDir dir;
        dir.mkpath(debug_dir_path);

        debug_file_path_regular = debug_dir_path + "z_water_flow.log";
        //        debug_file_path_examiner = debug_dir_path + "z_examiner_info.log";
        //        debug_file_path_examiner = debug_dir_path + "z_parikshak_yantar.log";
        debug_file_path_examiner = debug_dir_path + "z_prime_master.log";
        debug_file_path_root     = debug_dir_path + "z_roots_info.log";
        debug_file_path_source   = debug_dir_path + "z_sources_info.log";
        debug_file_path_sumuri = debug_dir_path + "z_sumuri_info.log";

        debug_file_regular_ptr.setFileName(debug_file_path_regular);
        if(!debug_file_regular_ptr.open(QIODevice::Append))
        {
            exit(32);
        }

    }

}

void recon_static_functions::close_debug_file()
{
    if(debug_file_regular_ptr.isOpen())
    {
        debug_file_regular_ptr.flush();
        debug_file_regular_ptr.close();
    }
}


bool recon_static_functions::check_is_it_registry_file_by_signature(QString file_path, QString caller_func)
{
    QFile file(file_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " file open---FAILED----" +file.fileName()  , Q_FUNC_INFO);
        recon_static_functions::app_debug(file.errorString(),  Q_FUNC_INFO);
        return false;
    }

    QByteArray dataread = file.read(4096);
    file.close();

    QByteArray resistry_header_fst;
    resistry_header_fst.append(0x72);
    resistry_header_fst.append(0x65);
    resistry_header_fst.append(0x67);
    resistry_header_fst.append(0x66);

    if(dataread.startsWith(resistry_header_fst))
        return true ;

    if(file_path.endsWith(".reg"))
        return true;

    return false;
}

bool recon_static_functions::check_is_it_sqlite_file_by_signature(QString file_path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    QFileInfo info(file_path);
    if(!(info.exists()))
        return false;

    QFile file(file_path);
    if(!file.open(QIODevice::ReadOnly))
        return false;

    QString str = file.readLine().trimmed();
    file.close();

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    //  if(str.startsWith("SQLite format") || var.name() == QString("application/x-sqlite3"))
    if(str.startsWith("SQLite format"))
    {
        return true;
    }


    QString dependend_db_files_path = file_path;
    if(dependend_db_files_path.endsWith("-shm"))
    {
        QFile file_shm(file_path);
        if(!file_shm.open(QIODevice::ReadOnly))
            return false;

        QByteArray dataread = file_shm.read(64);
        file_shm.close();
        QByteArray shm_header_ar;
        shm_header_ar.append(0x18);
        shm_header_ar.append(0xe2);
        shm_header_ar.append(0x2d);
        if(dataread.startsWith(shm_header_ar))
            return true ;
    }
    else if(dependend_db_files_path.endsWith("-wal"))
    {
        /// Wal file signature 0x377f0682 or 0x377f0683

        QFile file_wal(file_path);
        if(!file_wal.open(QIODevice::ReadOnly))
            return false;

        QByteArray dataread = file_wal.read(64);
        file_wal.close();
        QByteArray wal_header_ar_1;
        wal_header_ar_1.append(0x37);
        wal_header_ar_1.append(0x7f);
        wal_header_ar_1.append(0x06);
        wal_header_ar_1.append(0x82);

        if(dataread.startsWith(wal_header_ar_1))
            return true;
        else
        {
            QByteArray wal_header_ar_2;
            wal_header_ar_2.append(0x37);
            wal_header_ar_2.append(0x7f);
            wal_header_ar_2.append(0x06);
            wal_header_ar_2.append(0x83);
            if(dataread.startsWith(wal_header_ar_2))
                return true;

        }

    }

    return false;
}

bool recon_static_functions::is_it_plist_file(QString file_path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    bool is_plist_file = false;
    QFile file(file_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        (prepare_callerfun(caller_func) + " file open---FAILED----",Q_FUNC_INFO);
        (file.errorString(),Q_FUNC_INFO);
        return false;
    }

    QByteArray dataread = file.readAll();
    file.close();

    QByteArray plist_header;
    plist_header.append(0x62);
    plist_header.append(0x70);
    plist_header.append(0x6C);
    plist_header.append(0x69);
    plist_header.append(0x73);
    plist_header.append(0x74);


    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    if(dataread.startsWith(plist_header))
        is_plist_file  = true;;

    if(dataread.startsWith("<?xml version") && dataread.contains("<!DOCTYPE plist"))
        is_plist_file  = true;;

    return is_plist_file;
}


QString recon_static_functions::mining_size(QString path, QString caller_func)
{
    Q_UNUSED(caller_func)

    QFileInfo minfo(path);
    if(!minfo.exists())
        return QString("");

    if(!minfo.isDir())
    {
        return QString::number(minfo.size());  //return file size
    }

    qint64 total_size = 0;

    QDirIterator iter(path, QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
    while(iter.hasNext())
    {
        QFileInfo info(iter.next());

        if(info.isDir() || info.isSymLink())
            continue;

        total_size = total_size + info.size();
    }

    return QString::number(total_size); // return dir size
}

QString recon_static_functions::human_readable_size(qint64 size_in_qint64, QString caller_func)
{
    Q_UNUSED(caller_func)

    qint64 size_in_int;

    size_in_int = size_in_qint64;

    if(size_in_int <= MACRO_Generic_Value_KB_in_byte_int - 1)
    {
        return QString::number(size_in_int) + " B";
    }

    if(size_in_int >= MACRO_Generic_Value_KB_in_byte_int && size_in_int <= ((MACRO_Generic_Value_MB_in_byte_int ) - 1))
    {
        double size_double = size_in_int / (1024.0);

        QString size_str = QString::number(size_double, 'f', 2);
        return size_str + " KB";
    }

    if(size_in_int >= (MACRO_Generic_Value_MB_in_byte_int) && size_in_int <= ((MACRO_Generic_Value_GB_in_byte_int ) - 1))
    {
        double size_double = size_in_int / (1024.0 * 1024.0);
        QString size_str = QString::number(size_double, 'f', 2);
        return size_str + " MB";

    }

    if(size_in_int >= (MACRO_Generic_Value_GB_in_byte_int))
    {
        double size_double = size_in_int / (1024.0 * 1024.0 * 1024.0);
        QString size_str = QString::number(size_double, 'f', 2);
        return size_str + " GB";
    }


    return QString("0 KB");
}

QString recon_static_functions::get_forensic_image_type(QString file_path)
{
    QString file_type = MACRO_Target_Disk_ImageType_Unsupported;

    QString path_small = file_path;
    path_small = path_small.trimmed();

    if(path_small.endsWith(".dmg" ,Qt::CaseInsensitive) || path_small.endsWith(".sparseimage" , Qt::CaseInsensitive) || path_small.endsWith(".sparsebundle",Qt::CaseInsensitive))
        file_type = MACRO_Target_Disk_ImageType_Apple;
    else if(path_small.endsWith(".raw", Qt::CaseInsensitive) || path_small.endsWith(".dd",Qt::CaseInsensitive) || path_small.endsWith(".00001",Qt::CaseInsensitive) || path_small.endsWith(".001",Qt::CaseInsensitive))
        file_type = MACRO_Target_Disk_ImageType_RAW_FULL;
    else if(path_small.endsWith(".e01",Qt::CaseInsensitive) || path_small.endsWith(".ex01",Qt::CaseInsensitive) || path_small.endsWith(".s01",Qt::CaseInsensitive) )
        file_type = MACRO_Target_Disk_ImageType_ENCASE_PHYSICAL;
    else if(path_small.endsWith(".l01",Qt::CaseInsensitive))
        file_type = MACRO_Target_Disk_ImageType_ENCASE_LOGICAL;
    else if(path_small.endsWith(".000",Qt::CaseInsensitive))
        file_type = MACRO_Target_Disk_ImageType_RAW_SPLIT;
    else if(path_small.endsWith(".vmdk",Qt::CaseInsensitive))
        file_type = MACRO_Target_Disk_ImageType_VMDK;
    else if(path_small.endsWith(".vhd",Qt::CaseInsensitive))
        file_type = MACRO_Target_Disk_ImageType_VHD;
    else if(path_small.endsWith(".iso",Qt::CaseInsensitive) || path_small.endsWith(".cdr",Qt::CaseInsensitive))
        file_type = MACRO_Target_Disk_ImageType_Optical_Disc;
    else if(path_small.endsWith(".aff4", Qt::CaseInsensitive))
        file_type = MACRO_Target_Disk_ImageType_AFF4;


    return file_type;
}

QString recon_static_functions::human_readable_size(QString size_in_bytes, QString caller_func)
{

    Q_UNUSED(caller_func)

    size_in_bytes = size_in_bytes.trimmed();
    if(size_in_bytes == QString(""))
        return "";

    qint64 size_in_int;
    bool ok;

    size_in_int = size_in_bytes.toULongLong(&ok);

    if(size_in_int <= MACRO_Generic_Value_KB_in_byte_int - 1)
    {
        return QString::number(size_in_int) + " B";
    }

    if(size_in_int >= MACRO_Generic_Value_KB_in_byte_int && size_in_int <= ((MACRO_Generic_Value_MB_in_byte_int ) - 1))
    {
        double size_double = size_in_int / (1024.0);

        QString size_str = QString::number(size_double, 'f', 2);
        return size_str + " KB";
    }

    if(size_in_int >= (MACRO_Generic_Value_MB_in_byte_int) && size_in_int <= ((MACRO_Generic_Value_GB_in_byte_int ) - 1))
    {
        double size_double = size_in_int / (1024.0 * 1024.0);
        QString size_str = QString::number(size_double, 'f', 2);
        return size_str + " MB";

    }

    if(size_in_int >= (MACRO_Generic_Value_GB_in_byte_int))
    {
        double size_double = size_in_int / (1024.0 * 1024.0 * 1024.0);
        QString size_str = QString::number(size_double, 'f', 2);
        return size_str + " GB";
    }

    return QString("0 KB");
}

QString recon_static_functions::get_available_filename(QString file_name, QString path, QString caller_func)
{
    QDir dir;
    dir.mkpath(path);

    int count = 1;
    QString file_tmp = file_name;

    recon_static_functions::debug_conditional(prepare_callerfun(caller_func) + " : running", Q_FUNC_INFO);
    while(1)
    {
        if(is_file_exist_in_dir(path,file_tmp, Q_FUNC_INFO))
        {
            file_tmp = QString::number(count) + "_" + file_name;
            count++;
            continue;
        }
        break;
    }
    recon_static_functions::debug_conditional(prepare_callerfun(caller_func) + " : infinite loop cross", Q_FUNC_INFO);

    return file_tmp;
}

QString recon_static_functions::get_available_stringname(QString string_name, QStringList stringlist_values, QString caller_func)
{

    int count = 1;
    QString new_name = string_name;
    recon_static_functions::debug_conditional(prepare_callerfun(caller_func) + " : running", Q_FUNC_INFO);
    while(1)
    {
        if(stringlist_values.contains(new_name))
        {
            new_name = QString::number(count) + "_" + string_name;
            count++;
            continue;
        }
        break;
    }
    recon_static_functions::debug_conditional(prepare_callerfun(caller_func) + " : infinite loop cross", Q_FUNC_INFO);

    return new_name;
}


QString recon_static_functions::prepare_callerfun(QString data)
{

#ifndef MACRO_RECON_STATIC_APP_RUN_BY_FORENSO
    return data;
#endif

    QString tmp_func_name = data;
    data = data.trimmed();
    data.remove(data.indexOf("(",0),data.size());
    data = data.trimmed();
    data.remove(0,data.lastIndexOf(" "));
    data = data.trimmed();
    data.remove("*");
    data = data.trimmed();

    if(data.contains("(") || data.contains(" "))
        data = tmp_func_name;

    return "{{CallerFunc = " + data + "}}";
}

void recon_static_functions::encrypt_file_1(QString file_path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    QFile file(file_path);


    if(!file.exists())
        return;


    QFileInfo info(file_path);


    QDir dir;
    // QString current_path = dir.currentPath();

    QString new_lib_path = QApplication::applicationDirPath() + "/." + info.fileName();

    QFile file3234(new_lib_path);
    file3234.remove();

    QCoreApplication::processEvents();

    file.copy(new_lib_path);
    QCoreApplication::processEvents();


    QString str1 = QString::number(QFont::Black);

    QDateTimeEdit mindatetime;

    QString str2 = QString::number((mindatetime.minimumDateTime().toMSecsSinceEpoch())/500);


    QString str3 = QString::number(Qt::Monday);


    QString str4 = QString::number(QFont::Decorative);

    QByteArray ar1 = QCryptographicHash::hash(str1.toUtf8(),QCryptographicHash::Sha1);
    QByteArray ar2 = QCryptographicHash::hash(str2.toUtf8(),QCryptographicHash::Md5);
    QByteArray ar3 = QCryptographicHash::hash(str3.toUtf8(),QCryptographicHash::Md5);
    QByteArray ar4 = QCryptographicHash::hash(str4.toUtf8(),QCryptographicHash::Sha1);


    QByteArray total = ar1 + ar3 + ar4 + ar2;

    //  QString magic_key = QCryptographicHash::hash(total,QCryptographicHash::Sha1);

    QString magic_key = str1 + str2 + str3 + str4;

    char mdata[100];
    memset(mdata,'\0',100);

    memcpy(mdata, magic_key.toLocal8Bit().data(), 8);
    // my_main_encrypt(1, (char *)"1234567", new_lib_path.toAscii().data());
    QString nstr = "#$34mnL$";
    my_main_encrypt(0, (char *)"#$34mnL$", file_path.toLocal8Bit().data());
    // my_main_encrypt(1, (char *)mdata, new_lib_path.toAscii().data());

    file3234.remove();

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

}

QString recon_static_functions::decrypt_file_1(QString file_path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    QFile file(file_path);

    if(!file.exists())
        return file_path;

    QFileInfo info(file_path);


    QDir dir;
    // QString new_lib_path = QApplication::applicationDirPath() + "/." + info.fileName();

    QString current_path = QString(global_variable_private_var_tmp_Dir_Path_Case_QString);
    QString new_lib_path = current_path + info.fileName();

    QFile file3234(new_lib_path);
    file3234.remove();

    // QCoreApplication::processEvents();

    file.copy(new_lib_path);
    // QCoreApplication::processEvents();


    QString str1 = QString::number(QFont::Black);

    QDateTimeEdit mindatetime;

    QString str2 = QString::number((mindatetime.minimumDateTime().toMSecsSinceEpoch())/500);


    QString str3 = QString::number(Qt::Monday);


    QString str4 = QString::number(QFont::Decorative);

    QByteArray ar1 = QCryptographicHash::hash(str1.toUtf8(),QCryptographicHash::Sha1);
    QByteArray ar2 = QCryptographicHash::hash(str2.toUtf8(),QCryptographicHash::Md5);
    QByteArray ar3 = QCryptographicHash::hash(str3.toUtf8(),QCryptographicHash::Md5);
    QByteArray ar4 = QCryptographicHash::hash(str4.toUtf8(),QCryptographicHash::Sha1);


    QByteArray total = ar1 + ar3 + ar4 + ar2;

    //  QString magic_key = QCryptographicHash::hash(total,QCryptographicHash::Sha1);

    QString magic_key = str1 + str2 + str3 + str4;

    char mdata[100];
    memset(mdata,'\0',100);

    memcpy(mdata, magic_key.toLocal8Bit().data(), 8);
    // my_main_encrypt(1, (char *)"1234567", new_lib_path.toAscii().data());
    QString nstr = "#$34mnL$";
    my_main_encrypt(1, (char *)"#$34mnL$", new_lib_path.toLocal8Bit().data());
    // my_main_encrypt(1, (char *)mdata, new_lib_path.toAscii().data());



    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return new_lib_path;

}

bool recon_static_functions::encrypt_file_2(QString source_file, QString dest_file, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);


    encryptor_file_2 m_obj;
    bool val = m_obj.pub_encrypt_file(source_file , dest_file);


    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    return val;
}

bool recon_static_functions::decrypt_file_2(QString source_file, QString dest_file, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);


    encryptor_file_2 m_obj;
    bool val = m_obj.pub_decrypt_file(source_file , dest_file);


    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    return val;

}

QString recon_static_functions::encrypt_string(QString plain_str , QString caller_func)
{
    Q_UNUSED(caller_func)

    //================Do not use generic debug funcion in this function ==========//
    //================It will cause LAB to crash =================================//

    QString encrypted_str;

    if(plain_str.isEmpty())
    {
        return encrypted_str;
    }

    encryptor_string m_obj;
    encrypted_str = m_obj.encryptToString(plain_str);


    return encrypted_str;
}

QString recon_static_functions::decrypt_string(QString encrypted_str, QString caller_func)
{
    Q_UNUSED(caller_func)

    //================Do not use generic debug funcion in this function ==========//
    //================It will cause LAB to crash =================================//

    QString decrypted_str;

    if(encrypted_str.isEmpty())
    {
        return decrypted_str;
    }

    encryptor_string m_obj;
    decrypted_str = m_obj.decryptToString(encrypted_str);


    return decrypted_str;

}


QString recon_static_functions::prepend_zero(QString rec, QString upto, QString caller_func)
{
    Q_UNUSED(caller_func)

    bool ok;
    qint64 rec_int;

    rec_int = rec.toULongLong(&ok);

    QString str;

    int base_value = 10; //decimal
    str =  QString("%1").arg(rec_int, upto.toInt(),base_value, QChar('0'));

    return str;
}

bool recon_static_functions::remove_safely_blank_dir_OR_file(QString path, QString caller_func)
{

    if(!check_path_removable(path, Q_FUNC_INFO))
        return false;


    QFileInfo info(path);
    if(!info.exists())
    {
        (prepare_callerfun(caller_func) + " do not take tension return FALSE file does not exist : " + info.filePath(),Q_FUNC_INFO);
        return false;
    }

    if(info.isDir())
    {
        QDir dir(info.filePath());
        if(dir.count()<3)
        {
            // The directory must be empty for rmdir() to succeed
            return dir.rmdir(info.filePath());
        }
    }
    else
    {
        return QFile::remove(info.filePath());
    }

    return false;
}

bool recon_static_functions::check_path_removable(QString dir_path, QString caller_func)
{
    /*
     *  This check ensures that root or users directory will not be deleted.
     */


    if(dir_path.trimmed().isEmpty() || dir_path.trimmed() == QString("/"))
        return false;


    if(dir_path.startsWith(global_variable_private_var_tmp_Dir_Path_Case_QString))
    {
        return true;
    }
    else if(dir_path.startsWith(recon_result_path))
    {
        return true;
    }
    else if(dir_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString()))
    {
        return true;
    }

    return false;
}

void recon_static_functions::do_directory_blank_completely(QString dir_path, QString caller_func)
{
    Q_UNUSED(caller_func)

    if(!check_path_removable(dir_path, Q_FUNC_INFO))
        return;


    QDir dir(dir_path);
    if (dir.exists())
    {
        // Must use QDir::NoDotAndDotDot otherwise removeRecursively() will remove input dir also
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Hidden  | QDir::AllDirs | QDir::Files))
        {
            if(info.isDir())
            {

                recon_static_functions::do_directory_blank_completely(info.filePath(), Q_FUNC_INFO);
                recon_static_functions::remove_safely_blank_dir_OR_file(info.filePath(), Q_FUNC_INFO);
            }
            else
            {
                QFile::remove(info.filePath());
            }
        }
    }
}

void recon_static_functions::do_directory_blank_partially(QString dir_path ,QStringList files_to_skip_list , QString caller_func)
{

    Q_UNUSED(caller_func)

    if(!check_path_removable(dir_path, Q_FUNC_INFO))
        return;

    QDir dir(dir_path);
    if (dir.exists())
    {
        // Must use QDir::NoDotAndDotDot otherwise removeRecursively() will remove input dir also
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Hidden  | QDir::AllDirs | QDir::Files))
        {

            if(files_to_skip_list.contains(info.fileName()))
                continue;

            if(info.isDir())
            {
                recon_static_functions::do_directory_blank_completely(info.filePath(), Q_FUNC_INFO);
                recon_static_functions::remove_safely_blank_dir_OR_file(info.filePath(), Q_FUNC_INFO);
            }
            else
            {
                QFile::remove(info.filePath());
            }
        }
    }
}


QString recon_static_functions::size_string(long double size, QString caller_func)
{
    Q_UNUSED(caller_func)

    QString size_string;
    //float f1;
    if (size > 1024)
    {

        size = (size/1024);
        if (size > 1024)
        {
            size = (size/1024);
            if (size > 1024)
            {
                size = (size/1024);
                if (size > 1024)
                {
                    size = (size/1024);
                    size_string = QString::number(size, 'f', 2) + " T";
                }
                else //G
                {
                    size_string = QString::number(size, 'f', 2) + " G";
                }
            }
            else //M
            {
                size_string = QString::number(size, 'f', 2) + " M";
            }
        }
        else //k
        {
            size_string = QString::number(size, 'f', 2) + " K";

        }

    }
    else //B less than= 1024
    {
        size_string = QString::number(size, 'f', 2) + " B";
    }

    return size_string;
}

QStringList recon_static_functions::get_search_string_list(QString keyword, QString caller_func)
{
    Q_UNUSED(caller_func)

    QStringList search_string_list;

    if(keyword.contains(","))
    {
        QStringList keyword_list;
        keyword_list = keyword.split(",", Qt::SkipEmptyParts);

        for(int i = 0; i < keyword_list.size(); i++)
        {
            QString temp_string = keyword_list.at(i);
            temp_string = temp_string.trimmed();

            if(temp_string == "")
            {
                continue;
            }
            temp_string = "%" + temp_string + "%";
            search_string_list.append(temp_string);
        }
    }
    else
        search_string_list.append("%" + keyword + "%");

    return search_string_list;
}

qint64 recon_static_functions::overwrite_data_into_file(QString file_path, QString our_data, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    QFile file;
    file.setFileName(file_path);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " file.open ------FAILED------- at path " + file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + file.errorString(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" CALLER FUNCTION  " + caller_func,Q_FUNC_INFO);
        return -1;
    }

    qint64 bt_wrtn = file.write(our_data.toLocal8Bit());

    file.flush();
    file.close();

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return bt_wrtn;
}

qint64 recon_static_functions::append_data_into_file(QString file_path, QString our_data, int append_status, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    QFile file;
    file.setFileName(file_path);

    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " file.open ------FAILED------- at path " + file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + file.errorString(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" CALLER FUNCTION  " + caller_func,Q_FUNC_INFO);
        return -1;
    }
    qint64 bt_wrtn = 0;

    if(append_status == enum_global_append_data_in_file_as_it_is)
    {
        bt_wrtn = file.write(our_data.toLocal8Bit());
    }
    else if(append_status == enum_global_append_data_in_file_with_prepend_one_next_line)
    {
        bt_wrtn = file.write("\n" + our_data.toLocal8Bit());
    }
    else if(append_status == enum_global_append_data_in_file_with_append_one_next_line)
    {
        bt_wrtn = file.write(our_data.toLocal8Bit() + "\n");
    }

    file.flush();
    file.close();

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return bt_wrtn;
}

bool recon_static_functions::remove_data_contain_line_from_file(QString file_path , QString data, QString caller_func)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QFile read_file(file_path);
    if(!read_file.open(QIODevice::ReadWrite))
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " file.open ------FAILED------- at path " + file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + read_file.errorString(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" CALLER FUNCTION  " + caller_func,Q_FUNC_INFO);
        return false;
    }

    QTextStream stream(&read_file);
    QString line = stream.readLine();
    qint64 rpos,wpos=0;
    bool m_shift = false;

    while (!line.isNull())
    {
        rpos = stream.pos();

        if(!line.contains(data))
        {
            if(m_shift)
            {
                stream.seek(wpos);
                stream<<line<<Qt::endl;
                wpos = stream.pos();
                stream.seek(rpos);
            }
            else
            {
                wpos = rpos;
            }
        }
        else
        {
            m_shift = true;
        }

        stream.seek(rpos);
        line = stream.readLine();
    }

    read_file.resize(wpos);
    read_file.flush();
    read_file.close();

    recon_static_functions::app_debug("End", Q_FUNC_INFO);
    return  m_shift;

}


bool recon_static_functions::is_having_macos_user(QString path, QString caller_func)
{
    Q_UNUSED(caller_func)

    path = path.trimmed();
    QFile file(path + "/Library/Preferences/com.apple.finder.plist");
    if(file.exists())
        return true;
    else
        return false;
}

QString recon_static_functions::create_md5(QString file_path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    unsigned char c[MD5_DIGEST_LENGTH];
    MD5_CTX md_context;
    int i, bytes;
    unsigned char data[1024];

    QFileInfo info(file_path);
    if(info.isDir())
        return QString("");



    QFile target_file(file_path);
    if(!target_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " target_file.open---FAILED---" + file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(target_file.errorString(),Q_FUNC_INFO);
        return QString("");

    }

    MD5_Init(&md_context);
    while ((bytes = target_file.read((char *)data,1024)) > 0)
    {
        MD5_Update(&md_context, data, bytes);
    }


    QByteArray ar;
    MD5_Final(c, &md_context);
    for(i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        ar.append(c[i]);
    }


    target_file.close();

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return QString(ar.toHex());
}

QString recon_static_functions::create_sha1(QString file_path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    unsigned char c[SHA_DIGEST_LENGTH];
    SHA_CTX sha_context;
    int i, bytes;
    unsigned char data[1024];
    QString utf8_file_name = QString::fromUtf8(file_path.toLocal8Bit().data());

    QFileInfo info(file_path);
    if(info.isDir())
        return QString("");

    QFile target_file(file_path);

    if(!target_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " target_file.open---FAILED---" + file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(target_file.errorString(),Q_FUNC_INFO);
        return QString("");

    }
    SHA1_Init(&sha_context);

    while ((bytes = target_file.read((char *)data,1024)) > 0)
        SHA1_Update(&sha_context, data, bytes);


    QByteArray ar;
    SHA1_Final(c, &sha_context);
    for(i = 0; i < SHA_DIGEST_LENGTH; i++)
        ar.append(c[i]);


    target_file.close();


    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return QString(ar.toHex());
}

QString recon_static_functions::get_available_filename_as_last_braces(QString file_name, QString path, QString caller_func)
{
    // input: image
    // return :image(1)


    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);
    int count = 1;
    QString file_tmp = file_name;
    QString tmp_name;
    recon_static_functions::debug_conditional(prepare_callerfun(caller_func) + " : running", Q_FUNC_INFO);
    while(1)
    {
        if(is_file_exist_in_dir(path,file_tmp, Q_FUNC_INFO))
        {
            if(file_tmp.contains("."))
            {

                if(file_tmp.startsWith("."))
                {
                    //file_name: .DS_store
                    file_tmp = file_name + "(" + QString::number(count) + ")";
                    count++;
                    continue;
                }
                else
                {
                    //file_name: abc.txt
                    QStringList list = file_name.split(".");
                    tmp_name = list.at(0);
                    tmp_name = tmp_name + "(" + QString::number(count) + ")";
                    file_tmp = tmp_name + "." + list.at(1);
                    count++;
                    continue;
                }
            }
            else
            {
                //file_name: caseresult
                file_tmp = file_name + "(" + QString::number(count) + ")";
                count++;
                continue;
            }
            //            QStringList list = file_name.split(".");
            //            tmp_name = list.at(0);
            //            tmp_name = tmp_name + "(" + QString::number(count) + ")";
            //            file_tmp = tmp_name + "." + list.at(1);
            //            count++;

            //            continue;
        }

        break;
    }

    recon_static_functions::debug_conditional(prepare_callerfun(caller_func) + " : infinite loop cross", Q_FUNC_INFO);



    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);
    return file_tmp;
}

QString recon_static_functions::get_human_readable_time(QString str, QString caller_func)
{

    Q_UNUSED(caller_func)

    if(str.contains("."))
        str = str.remove(str.indexOf("."), str.size());


    bool ok;
    int rcvd = str.toInt(&ok,10);


    if(rcvd < 0 || rcvd > 86399) // 24 hrs
        return "";

    int hr = rcvd/3600;
    int hr_p = rcvd % 3600;

    int min = hr_p / 60;
    int min_p = hr_p % 60;

    int base_value = 10; //decimal
    int req_digit_count = 2;

    QString hour =  QString("%1").arg(hr,req_digit_count,base_value, QChar('0'));

    QString minute =  QString("%1").arg(min,req_digit_count,base_value, QChar('0'));

    QString second =  QString("%1").arg(min_p,req_digit_count,base_value, QChar('0'));

    if(hour.trimmed() == "00")
        return (minute + ":" +second + " Min");
    else
        return (hour + ":" + minute + ":" +second + " Hrs");

}

bool recon_static_functions::check_file_signature(QString filepath , QString signature_str , QString offset_str, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " -Start " , Q_FUNC_INFO);

    if(filepath.isEmpty() || signature_str.isEmpty())
        return false;

    qint64 bytes_to_read = signature_str.size();

    bool bool_is_signature_hex = false;

    if(signature_str.startsWith(QString("0x")))
    {
        bool_is_signature_hex = true;
        signature_str = signature_str.mid(2, signature_str.size() - 2);
        bytes_to_read = signature_str.size() / 2;
    }



    if(bytes_to_read <= 0)
        return false;


    QFile myfile(filepath);
    if(!myfile.exists())
        return false;

    if (!myfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " -File Open ------FAILED----- : "  + filepath, Q_FUNC_INFO);
        recon_static_functions::app_debug("---Error---- " + myfile.errorString() , Q_FUNC_INFO);

        return false;
    }

    offset_str = offset_str.trimmed();

    if(!offset_str.isEmpty())
    {
        bool ok;
        myfile.seek(offset_str.toInt(&ok, 10));

    }


    ///
    ///  Do Not use trimm here...(because there could be null bytes)
    ///


    QByteArray data_read = myfile.read(bytes_to_read);
    myfile.close();

    bool m_status = false;
    if(bool_is_signature_hex)
    {
        if(signature_str.toLower() == QString::fromLocal8Bit(data_read.toHex()).toLower())
        {
            m_status = true;
        }
    }
    else
    {
        if(signature_str.toLower() == QString::fromLocal8Bit(data_read).toLower())
        {
            m_status = true;
        }
    }

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);

    return m_status;
}

QString recon_static_functions::convert_human_readable_size_to_bytes(qint64 size_in_qint64, QString size_unit , QString caller_func)
{
    Q_UNUSED(caller_func)

    QString converted_size_in_bytes;

    if(size_unit.contains(MACRO_Generic_Filter_Condition_KB))
    {
        qint64 value = (size_in_qint64) * (1024);
        converted_size_in_bytes = QString::number(value);
    }
    else if(size_unit.contains(MACRO_Generic_Filter_Condition_MB))
    {
        qint64 value = (size_in_qint64) * (1024 * 1024);
        converted_size_in_bytes = QString::number(value);
    }
    else if(size_unit.contains(MACRO_Generic_Filter_Condition_GB))
    {
        qint64 value = (size_in_qint64) * (1024.0 * (qint64)1024 *1024);
        converted_size_in_bytes = QString::number(value);
    }
    else if(size_unit.contains(MACRO_Generic_Filter_Condition_TB))
    {
        double value = (size_in_qint64) * (1024.0 * 1024 * (qint64)1024 * 1024);
        converted_size_in_bytes = QString::number(value);
    }
    else
    {
        converted_size_in_bytes = QString::number(size_in_qint64);
    }

    return converted_size_in_bytes;
}

qint64 recon_static_functions::get_round_up_value_near_hundred_or_thousand(qint64 value, QString caller_func)
{
    recon_static_functions::app_debug(prepare_callerfun(caller_func) + " Starts ", Q_FUNC_INFO);

    qint64 round_up_value = 0;

    if(value < 25)
        return 30;

    if(value < 50)
        return 50;

    qint64 value_near_to = 0;
    if(value < 1000)
        value_near_to = 100;
    else
        value_near_to = 1000;

    int remainder = value % value_near_to;

    round_up_value = (value-remainder) + value_near_to;

    recon_static_functions::app_debug(prepare_callerfun(caller_func) + " End " ,Q_FUNC_INFO);

    return round_up_value;
}


QString recon_static_functions::convert_string_to_unicode(QString file_data, QString caller_func)
{
    Q_UNUSED(caller_func)

    int ind = -1;

    QStringList mlist;


    //recon_static_functions::debug_conditional(prepare_callerfun(caller_func) + " : running", Q_FUNC_INFO);
    while(1)
    {
        ind = file_data.indexOf("\\U", ind + 1);
        if(ind < 0)
            break;

        QString mk;
        for(int k =2; k < 6; k++)
        {
            if(file_data.size() < ind + k)
                break;

            mk.append( file_data.at(ind + k));
        }

        mlist.append(mk);
    }
    //recon_static_functions::debug_conditional(prepare_callerfun(caller_func) + " : infinite loop cross", Q_FUNC_INFO);

    QList<QChar> qchar_list;

    for(int i = 0 ; i < mlist.size(); i++)
    {
        bool ok;
        uint ep =  mlist.at(i).toULongLong(&ok, 16);
        QChar ch(ep);

        qchar_list.append(ch);
    }

    if(mlist.size() != qchar_list.size())
    {
        return file_data;
    }

    for(int i = 0 ; i < mlist.size(); i++)
    {
        file_data.replace("\\U" +  mlist.at(i), qchar_list.at(i));
    }

    return file_data;
}


void recon_static_functions::clear_variables_detailed_information(struct_global_set_metadata_info &st_meta_d_obj)
{
    st_meta_d_obj.bool_file_from_result = false;

    st_meta_d_obj.db_file_path.clear();
    st_meta_d_obj.detailed_info.clear();
    st_meta_d_obj.source_count_name.clear();
    st_meta_d_obj.final_file_path.clear();
    st_meta_d_obj.record_no.clear();
    st_meta_d_obj.display_searched_text.clear();
    st_meta_d_obj.content_searched_text.clear();
    st_meta_d_obj.plugin_name.clear();
    st_meta_d_obj.tab_name.clear();
}
void recon_static_functions::clear_variables_desktop_info_all(struct_global_desktop_info &mobj)
{
    clear_variables_desktop_password(mobj);
    clear_variables_desktop_general(mobj);
}

void recon_static_functions::clear_variables_desktop_password(struct_global_desktop_info &mobj)
{
    mobj.bool_desktop_pwd_accurate = false;
    mobj.desktop_password_modified_general.clear();
    mobj.desktop_password_original.clear();
    mobj.desktop_password_modified_shell_script.clear();
    mobj.desktop_password_modified_symbol_hex_value.clear();
}

void recon_static_functions::clear_variables_desktop_general(struct_global_desktop_info &mobj)
{
    mobj.os_version_full.clear();
    mobj.os_version_extended.clear();
    mobj.hardware_overview.clear();
    mobj.software_overview.clear();
    mobj.csrutil_status_info.clear();

    mobj.label_name.clear();
    mobj.volume_name.clear();
    mobj.file_system_type.clear();
    mobj.device_disk_node.clear();

    mobj.model_name_info.clear();
    mobj.processor_brand_info.clear();
    mobj.ram_size_info.clear();
    mobj.os_version_info.clear();
    mobj.model_year_info.clear();
    mobj.power_settings_info.clear();
    mobj.file_vault_status_info.clear();
    mobj.security_controller_info.clear();

    mobj.osxfuse_info.clear();
    mobj.java_info.clear();
    mobj.xcode_info.clear();
    mobj.python_info.clear();
    mobj.lz4_info.clear();
    mobj.enum34_info.clear();
    mobj.nudenet_info.clear();
    mobj.filetype_info.clear();
    mobj.mime_info.clear();
    mobj.mimelib_info.clear();
    mobj.glob2_info.clear();
    mobj.moviepy_info.clear();
    mobj.ffmpeg_info.clear();
    mobj.opencv_python_info.clear();
    mobj.pillow_info.clear();
    mobj.full_disk_acces_info.clear();
    mobj.timezone_name_complete_QString.clear();
    mobj.timezone_name_QString.clear();
    mobj.timezone_offset_readable_QString.clear();

    mobj.bool_consistency_requirements_satisfied = false;
    mobj.bool_processor_chip_silicon = false;

    mobj.block_size_numeric_int = 0;
    mobj.available_space_bytes_numeric_qint64 = 0;
    mobj.total_space_bytes_numeric_qint64 = 0;
    mobj.timezone_offset_numeric_qint64 = 0;

    mobj.consistency_requirements_failure_items_QStringList.clear();
}


void recon_static_functions::clear_variables_destination_drive(struct_global_destination_drive_info &mobj)
{
    mobj.label_name.clear();
    mobj.volume_name.clear();
    mobj.device_disk_node.clear();
    mobj.drive_path.clear();

    mobj.block_size_numeric_int = 0;
    mobj.bool_ready_to_work = false;
    mobj.bool_system_root_volume = false;
    mobj.bool_valid_drive = false;

    mobj.file_system_type.clear();
    mobj.bool_read_only = false;
    mobj.available_size_byte_at_case_uptime_numeric_qint64 = 0;
    mobj.total_size_byte_numeric_qint64 = 0;
}


void recon_static_functions::clear_variables_recon_and_artifact_common_system_account_info(struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_system_account &mobj)
{
    mobj.system_user.clear();
    mobj.domain_user.clear();
}


void recon_static_functions::clear_variables_recon_and_artifact_common_source_info(struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_source_info &mobj)
{
    mobj.regular_source_path.clear();
    mobj.source_type_internal.clear();
    mobj.macos_system_source_path.clear();

    mobj.gtakeoutos_google_takeout_account_id.clear();
    mobj.macos_logs_unified_log_custom_text_file_path1.clear();
    mobj.ios_logs_unified_log_custom_text_file_path1.clear();
    mobj.macos_logs_unified_log_actual_path1_QStringList.clear();


    mobj.os_scheme_display.clear();
    mobj.os_scheme_internal.clear();

    mobj.os_version_full.clear();

    mobj.system_account_QList.clear();

    mobj.source_password_original.clear();
    mobj.source_password_modified_symbol_hex_value.clear();
}

void recon_static_functions::clear_variables_recon_and_artifact_desktop_info(struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_desktop_info &mobj)
{
    mobj.os_version_full.clear();
    mobj.python_executable_path.clear();

    mobj.bool_desktop_password_accurate = false;
    mobj.desktop_password_original.clear();
    mobj.desktop_password_modified_symbol_hex_value.clear();
}

void recon_static_functions::clear_variables_recon_and_artifact_general_info(struct_GLOBAL_COMMON_RECON_AND_ARTIFACT_general_info &mobj)
{
    mobj.examiner_selected_spotlight_status_bool = false;
    mobj.debug_mode_status_bool = false;
    mobj.debug_print_in_file_bool = false;
    mobj.debug_print_on_terminal_bool = false;
    mobj.debug_encryption_status_bool = false;

    mobj.debug_file_path.clear();
}


void recon_static_functions::clear_variables_hex_viewer_open_file_data(struct_global_hex_viewer_open_file_data &struct_hex_file_info)
{
    struct_hex_file_info.bool_is_file_in_bucket = false;

    struct_hex_file_info.hex_viewer_display_name.clear();
    struct_hex_file_info.complete_file_path.clear();
    struct_hex_file_info.display_file_path.clear();
    struct_hex_file_info.record_no_str.clear();
    struct_hex_file_info.source_count_name.clear();
    struct_hex_file_info.plugin_name_str.clear();
    struct_hex_file_info.plugin_tab_name_str.clear();
    struct_hex_file_info.hex_feature_db_path.clear();
    struct_hex_file_info.recon_filefrom.clear();
    struct_hex_file_info.recon_file_infopath.clear();
}

QString recon_static_functions::get_modify_password(QString m_password, QString caller_func)
{
    Q_UNUSED(caller_func)

    m_password = m_password.replace("\\","\\\\");
    m_password = m_password.replace("!","\\!");
    m_password = m_password.replace("(","\\(");
    m_password = m_password.replace(")","\\)");
    m_password = m_password.replace("&","\\&");
    m_password = m_password.replace("$","\\$");
    m_password = m_password.replace("|","\\|");
    m_password = m_password.replace("\"","\\\"");
    m_password = m_password.replace("<","\\<");
    m_password = m_password.replace(">","\\>");
    m_password = m_password.replace("'","\\'");
    m_password = m_password.replace(";","\\;");
    m_password = m_password.replace("`","\\`");

    return m_password;
}

QString recon_static_functions::get_modified_password_replace_symbol_with_hex(QString passwd, QString caller_func)
{
    // Replace special symbol with hex value
    // input ==>>> recon@111
    // output ==>>> recon%40111

    Q_UNUSED(caller_func)

    QString formatted_pass;

    QString passwd_char_str;

    QRegularExpression re("[^A-Za-z0-9]");

    for(int num = 0; num < passwd.size(); num++)
    {
        passwd_char_str = passwd.at(num);

        QRegularExpressionMatchIterator exp_itr = re.globalMatch(passwd_char_str);
        if (!exp_itr.hasNext())
        {
            formatted_pass.append(passwd_char_str);
        }
        else
        {
            QString special_sym_hex = passwd_char_str.toLatin1().toHex();
            special_sym_hex.prepend("%");

            formatted_pass.append(special_sym_hex);
        }
    }

    return formatted_pass;
}


bool recon_static_functions::is_source_supported_for_tsk(struct_GLOBAL_witness_info_source src_obj)
{
    if((src_obj.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal)
        || src_obj.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
        || src_obj.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal)
        || src_obj.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
        || src_obj.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal)
        || src_obj.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal)
        || src_obj.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal)
        || src_obj.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal))
            && (src_obj.file_system_type == QString(MACRO_FILE_SYSTEM_TYPE_HFS)
                || src_obj.file_system_type.contains(QString(MACRO_FILE_SYSTEM_TYPE_NTFS), Qt::CaseInsensitive) //ntfs can be ud_ntfs or something else so contains check used
                || src_obj.file_system_type == QString(MACRO_FILE_SYSTEM_TYPE_MSDOS)
                || src_obj.file_system_type == QString(MACRO_FILE_SYSTEM_TYPE_LIFS)
                || src_obj.file_system_type == QString(MACRO_FILE_SYSTEM_TYPE_EXFAT)))
    {
        return true;
    }

    return false;
}


QString recon_static_functions::pub_get_simplified_password_for_shell_script(QString passwd, QString caller_func)
{
    /*
    =====Working Fine======

    space           -
    single quote    - '
    percentage      - %
    hash            - #
    exclaimation    - !
    empersand       - &

    brace start     - (
    brace end       - )
    multiply        - *
    Add             - +
    Minus           - -
    Dot             - .

    Forward Slash   - /
    Colon           - :
    Semi Colon      - ;
    Less Than       - <
    Greater Than    - >
    Equal To        - =

    Question Mark   - ?
    At the rate     - @
    Big Brace Start - [
    Big Brace End   - ]
    Caret           - ^

    Underscore      - _
    Curl Brace Start - {
    Curl Brace End   - }
    Or Operator      - |
    tilde            - ~
    Comma           - ,



    =====Converted=====
    Backward Slash  - \
    Double Quote    - "
    Dollar          - $
    Grave accent (backtick) - `

    */



    //QString special_chars_str = QString(" \\!\"#$%&'()*+,-./:;<=>?@[]^_`{|}~");

    QString special_chars_str = QString("\\\"$`");

    QString formatted_pass;

    QString passwd_char_str;
    QString spcl_char_str;

    for(int num = 0; num < passwd.size(); num++)
    {
        passwd_char_str = passwd.at(num);

        //replace special characters
        if(special_chars_str.contains(passwd_char_str))
        {
            for(int count = 0; count < special_chars_str.size(); count++)
            {
                spcl_char_str = special_chars_str.at(count);

                if(spcl_char_str == passwd_char_str)
                {
                    passwd_char_str.prepend(QString("\\"));
                    break;
                }
            }
        }

        formatted_pass.append(passwd_char_str);
    }
    return formatted_pass;
}


QString recon_static_functions::get_plain_data_from_encrypt_file_1(QString file_path, QString caller_func)
{
    recon_static_functions::debug_intensive("start ", recon_static_functions::prepare_callerfun(caller_func));
    QString mdata;

    QFile m_file5(file_path);
    if(!m_file5.exists())
        return mdata;

    QString new_path = recon_static_functions::decrypt_file_1(file_path, Q_FUNC_INFO);

    QFile m_file(new_path);
    if(!m_file.exists())
        return mdata;


    if(!m_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File Open ---FAILED---: "  + m_file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + m_file.errorString() , Q_FUNC_INFO);
        m_file.remove();
        return mdata;
    }

    mdata = QString::fromLocal8Bit(m_file.readAll());

    m_file.remove();


    recon_static_functions::debug_intensive("end ", recon_static_functions::prepare_callerfun(caller_func));

    return mdata;

}



void recon_static_functions::clear_variables_system_account_info(struct_global_system_account_info &m_obj)
{
    m_obj.system_user.clear();
    m_obj.domain_user.clear();

}


QStringList recon_static_functions::get_system_user_list_from_system_account_QList(QList <struct_global_system_account_info> system_account_QList , QString caller_func)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QStringList users_list;

    for(int qq = 0; qq < system_account_QList.size(); qq++)
    {
        struct_global_system_account_info mobj = system_account_QList.at(qq);
        users_list << mobj.system_user;
    }


    recon_static_functions::app_debug("end", Q_FUNC_INFO);
    return users_list;

}


QString recon_static_functions::get_system_users_and_domain_users_from_system_account_QList(QList <struct_global_system_account_info> list_sys_account_info , QString caller_func)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QStringList all_user_lists;

    for(int qq = 0; qq < list_sys_account_info.size() ; qq++)
    {
        struct_global_system_account_info mobj = list_sys_account_info.at(qq);

        QStringList users_list ;
        users_list << QString(MACRO_System_Account_List_Prefix_SYSTEM_USER) + "'" + mobj.system_user + "'" << QString( MACRO_System_Account_List_Prefix_DOMAIN_USER) + "'" + mobj.domain_user + "'";

        QString users_str = users_list.join(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon);
        all_user_lists << users_str;

    }

    QString system_accounts_str = all_user_lists.join(MACRO_RECON_Splitter_5_resolution_scope);


    recon_static_functions::app_debug("End", Q_FUNC_INFO);
    return system_accounts_str;

}
