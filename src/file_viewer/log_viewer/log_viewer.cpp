#include "log_viewer.h"

log_viewer::log_viewer(QWidget *parent) : QObject(parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

}

log_viewer::~log_viewer()
{

}

void log_viewer::pub_set_log_file_path(QString path)
{
    destination_log_file_path = path;
}

void log_viewer::pub_set_result_log_viewer_sqlite_path(QString name)
{
    destination_log_result_db_path = name;
}

void log_viewer::pub_set_source_count_name(QString name)
{
    source_count_name = name;
}

void log_viewer::pub_set_log_viewer_name(QString log_viewer_tbl_name)
{
    QStringList list_value = log_viewer_tbl_name.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

    if(list_value.size() > 0)
        log_file_name = list_value.at(0);

    if(list_value.size() > 1)
        log_plugin_name = list_value.at(1);

    if(list_value.size() > 2)
        log_plugin_category_name = list_value.at(2);

    if(list_value.size() > 3)
        log_plugin_record_no = list_value.at(3);

}

void log_viewer::pub_set_essentials()
{
    QString file_name = log_file_name + " [" + log_plugin_name +" "+ log_plugin_record_no + "]" + " [" + source_count_name + "]";

    //- Display name..
    log_viewer_display_name = file_name;

    file_name = log_file_name + " " + log_plugin_name +" "+ log_plugin_record_no + " " + source_count_name;

    //db_table_name.
    file_name = file_name.replace(" ","_");
    file_name = file_name.replace("-","_");
    file_name = file_name.replace(".","_");

    log_view_db_table_name = file_name;
}


void log_viewer::pub_start_log_extraction()
{
    set_source_file_path();
    entry_in_log_viewer_db_index_table();
    create_log_viewer_table();
}

void log_viewer::pub_set_recon_file_info(struct_global_recon_file_info obj)
{
    st_globl_recon_file_info_obj = obj;
}

int log_viewer::create_log_viewer_table()
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    QString command = "CREATE TABLE '" + log_view_db_table_name + "'"+
            "(INT INTEGER PRIMARY KEY,"
            "record_no INTEGER,"
            "tagged_data BLOB,"
            "cursor_start_index VARCHAR(300),"
            "cursor_end_index VARCHAR(300),"
            "filepath VARCHAR(300),"
            "recon_tag_value VARCHAR(300),"
            "colour_value VARCHAR(300))";

    recon_helper_standard_obj->execute_db_command_by_dbpath(command,destination_log_result_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
    return 1;
}

void log_viewer::entry_in_log_viewer_db_index_table()
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    QFile myFile(destination_log_file_path);
    if(!myFile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File Open -----FAILED---- " + myFile.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + myFile.errorString(), Q_FUNC_INFO);
        return;
    }

    QByteArray data_arr = myFile.readAll();
    myFile.close();

    std::string std_str(data_arr.constData(), data_arr.length());
    QString log_data_str = QString::fromStdString(std_str);


    QString command = " INSERT INTO 'log_viewer_index'(INT, viewer_display_name, db_table_name, file_name, file_path,"
                      " log_file_data, plugin_name, category_name, record_no, creation_timestamp, source_count_name,"
                      " recon_filefrom, recon_file_infopath) VALUES (?,?,?,?,?,?, ?,?,?,?,?,?,?)";

    QStringList list;


    list << log_plugin_record_no << log_viewer_display_name << log_view_db_table_name << log_file_name << source_file_path << log_data_str << log_plugin_name << log_plugin_category_name;
    list << log_plugin_record_no << QString::number(QDateTime::currentDateTimeUtc().toSecsSinceEpoch()) << source_count_name << recon_file_from << source_info_relative_path;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,destination_log_result_db_path, Q_FUNC_INFO);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void log_viewer::set_source_file_path()
{
    source_file_path          = st_globl_recon_file_info_obj.display_file_path;
    recon_file_from           = st_globl_recon_file_info_obj.recon_file_from;
    source_info_relative_path = st_globl_recon_file_info_obj.recon_file_info_path;
}
