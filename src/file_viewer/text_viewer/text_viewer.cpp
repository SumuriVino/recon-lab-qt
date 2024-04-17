#include "text_viewer.h"
#include "ui_text_viewer.h"

text_viewer::text_viewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::text_viewer)
{
    ui->setupUi(this);

    hexEdit = new QHexEdit_textviewer(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    connect(hexEdit , SIGNAL(signal_add_tag_to_tag_search(qint64, qint64, QString, QString, QString, QByteArray)), this, SLOT(slot_add_tag_to_tag_search(qint64, qint64, QString, QString, QString,QByteArray)));

    QHBoxLayout *h_layout = new QHBoxLayout(this);

    h_layout->setContentsMargins(0,0,0,0);

    h_layout->addWidget(hexEdit);

    this->setLayout(h_layout);


}

text_viewer::~text_viewer()
{
    delete ui;
}

void text_viewer::pub_clear_textview()
{
    hexEdit->clear_text_viewer();
}


void text_viewer::pub_set_essentials()
{

    hexEdit->pub_set_essentials();
}

void text_viewer::pub_load_file_in_text_view(QString filepath)
{
    qint64 pref_min_file_size =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Prefrences_Text_View_Maximum_File_Size_Bytes_qint64).toLongLong();

    if(QFileInfo(filepath).size() > pref_min_file_size)
    {
        pub_clear_textview();
        return;
    }

    loadFile(filepath);
}


void text_viewer::loadFile(const QString &fileName)
{
    current_file_path = fileName;

    file.setFileName(fileName);
    if (!hexEdit->setData_text_viewer(file))
    {
        recon_static_functions::app_debug("-----Failed----- file open: " + file.fileName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("--Error--:" + file.errorString() , Q_FUNC_INFO);
        return;
    }
}


void text_viewer::slot_add_tag_to_tag_search(qint64 start_offset , qint64 end_offset,QString tag_name, QString tag_color, QString color_hex, QByteArray tagged_content)
{
    insert_exported_content_info_in_db(tag_name,color_hex,start_offset,end_offset,tagged_content);
}

void text_viewer::pub_set_source_count_name(QString str)
{
    source_count_name = str;
}


void text_viewer::insert_exported_content_info_in_db(QString tag_name, QString color_hex, qint64 start_index, qint64 end_index, QByteArray file_contents_arr)
{


    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString).toString() + "text_viewer_content_info.sqlite";

    QFileInfo info(exported_content_info_db);
    if(!info.exists())
    {
        QString command = "CREATE TABLE 'text_content_info'(INT INTEGER PRIMARY KEY,bookmark,file_name VARCHAR(1000), file_path VARCHAR(1000),"
                          "file_size VARCHAR(1000), start_index VARCHAR(1000), end_index VARCHAR(1000), recon_tag_value VARCHAR(1000),"
                          "color_hex VARCHAR(1000), content_file_path VARCHAR(1000), source_count_name VARCHAR(1000), notes VARCHAR(1000), hex_data BLOB,"
                          "LINKFILENAME VARCHAR(1000), source_file VARCHAR(1000))";

        if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command,exported_content_info_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("CREATE table text_content_info in text viewer ---FAILED---" + exported_content_info_db,Q_FUNC_INFO);
        }


        QString export_file_cmd = recon_static_functions::get_exportfilelist_command();
        recon_helper_standard_obj->execute_db_command_by_dbpath(export_file_cmd , exported_content_info_db, Q_FUNC_INFO);

    }

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(exported_content_info_db);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" + exported_content_info_db , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(), Q_FUNC_INFO);
        return ;
    }


    QFileInfo info_temp(current_file_path);

    QString tmp_filename = info_temp.fileName();
    tmp_filename.remove(info_temp.suffix());

    if(tmp_filename.endsWith("."))
        tmp_filename.chop(1);

    QString export_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString).toString();
    tmp_filename = recon_static_functions::get_available_filename(tmp_filename, export_file_path, Q_FUNC_INFO);
    export_file_path = export_file_path + tmp_filename;



    QFile myfile(export_file_path);
    if(!myfile.open(QIODevice::ReadWrite))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + export_file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + myfile.errorString(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    myfile.write(file_contents_arr.data());
    myfile.flush();
    myfile.close();


    QString file_path_str = current_file_path;
    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString virtual_source_path = struct_info.virtual_source_path;

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    if(file_path_str.startsWith(result_dir_path))
    {
        file_path_str.remove(0,result_dir_path.size());
    }
    else
    {
        if(file_path_str.startsWith(virtual_source_path))
            file_path_str.remove(0,virtual_source_path.size());
    }
    if(!file_path_str.startsWith("/") && !file_path_str.trimmed().isEmpty())
        file_path_str.prepend("/");

    QSqlQuery query_insert(destination_db);
    query_insert.prepare("INSERT INTO 'text_content_info'(bookmark,file_name, file_path, file_size, start_index, end_index, recon_tag_value,"
                         "color_hex, content_file_path, source_count_name,hex_data,source_file) VALUES(?,?,?,?,? ,?,?,?,?,?, ?,?)");

    query_insert.addBindValue("0");
    query_insert.addBindValue(info_temp.fileName());
    query_insert.addBindValue(info_temp.filePath());
    query_insert.addBindValue(info_temp.size());
    query_insert.addBindValue(QString::number(start_index));
    query_insert.addBindValue(QString::number(end_index));
    query_insert.addBindValue(tag_name);
    query_insert.addBindValue(color_hex);
    query_insert.addBindValue(export_file_path);
    query_insert.addBindValue(source_count_name);
    query_insert.addBindValue(file_contents_arr.toHex());
    query_insert.addBindValue(info_temp.filePath());

    if(!query_insert.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query_insert.executedQuery(),Q_FUNC_INFO);
    }

    destination_db.close();

    QString command = "SELECT INT FROM text_content_info ORDER BY INT DESC LIMIT 1";
    QString record_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,exported_content_info_db,Q_FUNC_INFO);


    emit signal_insert_tag_detail_into_tag_search_db(current_file_path,tag_name, start_index, end_index, record_count, QString(MACRO_Plugin_Name_Text_Viewer), source_count_name);

}
