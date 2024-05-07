#include "exif_data_interface.h"

exif_data_interface::exif_data_interface(QWidget *parent) : QWidget(parent)
{
    exif_data_image_obj       = new exif_data_image_files();
    exif_data_audio_video_obj = new exif_data_audio_video();
    exif_data_documents_obj   = new exif_data_document_files();

    recon_helper_standard_obj = new recon_helper_standard(this);

}

exif_data_interface::~exif_data_interface()
{

}

void exif_data_interface::pub_set_result_dir(QString result_dir)
{
    result_dir_val = result_dir;

}

QString exif_data_interface::pub_extract_and_save_exif_data(QString mime_type, QString file_path, qint64 fs_record_no , QSqlDatabase &exif_data_db, struct_global_exif_data_parsed_data &exif_data_parsed_obj,QString display_file_path)
{ // extract exif data and save the info in database
    recon_static_functions::debug_intensive(" -Starts " , Q_FUNC_INFO);

    QString exif_data_str;

    if(recon_helper_standard_obj->check_is_it_image_file(file_path,display_file_path, Q_FUNC_INFO))
    {
        exif_data_str = exif_data_image_obj->pub_get_exif_data_images(file_path , exif_data_parsed_obj);
    }
    else if(mime_type.startsWith(QString("audio/")) || mime_type.startsWith(QString("video/")))
    {
        exif_data_str = exif_data_audio_video_obj->pub_get_exif_data_audio_video(file_path);
    }
    else if(mime_type == QString("application/pdf"))
    {
        /*
         *  this check if for pdf type only. It needs to be updated
         *  for another document types.
         */


        exif_data_str = exif_data_documents_obj->pub_get_exif_data_documents(file_path , exif_data_parsed_obj);

    }


    if(!exif_data_str.isEmpty())
    {

        QString command_del = QString("Delete from files Where filesystem_record = ?");
        //        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_del,QStringList(QString::number(fs_record_no)),exif_data_db,Q_FUNC_INFO);

        QString command = QString("insert into files(filesystem_record,exif_data ,make,model,author,latitude,longitude) values(?,?,?,?,?,?,?)");
        QStringList list;
        list <<  QString::number(fs_record_no)
              << exif_data_str << exif_data_parsed_obj.exif_make << exif_data_parsed_obj.exif_model << exif_data_parsed_obj.exif_author << exif_data_parsed_obj.exif_latitude << exif_data_parsed_obj.exif_longitude;

        //        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,exif_data_db,Q_FUNC_INFO);


        ///delete existing record
        QSqlQuery query_exif_data(exif_data_db);
        query_exif_data.prepare(command_del);
        query_exif_data.addBindValue(QString::number(fs_record_no));

        if(!query_exif_data.exec())
        {
            recon_static_functions::app_debug(" query delete  --------FAILED------ db file - " + exif_data_db.databaseName() ,Q_FUNC_INFO);
            recon_static_functions::app_debug(" command -  " + query_exif_data.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query_exif_data.lastError().text(),Q_FUNC_INFO);
        }

        ///insert record
        query_exif_data.clear();
        query_exif_data.prepare(command);

        for(int count = 0; count < list.size(); count++)
            query_exif_data.addBindValue(list.at(count));

        if(!query_exif_data.exec())
        {
            recon_static_functions::app_debug(" query insert  --------FAILED------ db file - " + exif_data_db.databaseName() ,Q_FUNC_INFO);
            recon_static_functions::app_debug(" command -  " + query_exif_data.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query_exif_data.lastError().text(),Q_FUNC_INFO);
        }
    }

    recon_static_functions::debug_intensive(" -Ends " , Q_FUNC_INFO);


    return exif_data_str;

}

void exif_data_interface::pub_set_exif_data_in_case_tree_display(QString file_path, QString record_no, QString db_path, m_treewidget *tree_exif_metadata_display, QString plugin_name, QString table_name, QString source_count_name,QString display_file_path)
{ // set exif data in case tree for display
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);

    QTreeWidgetItem* header = tree_exif_metadata_display->headerItem();
    header->setText(enum_global_exif_data_interface_exif_data_KEY, "Key");
    header->setText(enum_global_exif_data_interface_exif_data_Value, "Value");
    header->setText(enum_global_exif_data_interface_exif_data_Plugin_Table_Name, "Table Name");
    header->setText(enum_global_exif_data_interface_exif_data_Plugin_Name, "Plugin Name");
    header->setText(enum_global_exif_data_interface_exif_data_Plugin_Record_no, "Record No");
    header->setText(enum_global_exif_data_interface_exif_data_Plugin_Db_Path, "DB Path");
    header->setText(enum_global_exif_data_interface_exif_data_Plugin_Source_count_name, "Source Count Name");

    QString destination_db_path = db_path;


    QMimeType var;
    QMimeDatabase obj;

    var = obj.mimeTypeForFile(file_path);
    QString mime_type_str = var.name();


    QStringList exif_data_list;
    if(recon_helper_standard_obj->check_is_it_image_file(file_path,display_file_path, Q_FUNC_INFO))
    {
        exif_data_list = exif_data_image_obj->pub_get_all_available_exif_data_images(file_path);
    }
    else if(mime_type_str.startsWith(QString("audio/")) || mime_type_str.startsWith(QString("video/")))
    {
        exif_data_list = exif_data_audio_video_obj->pub_get_all_available_exif_data_audio_video(file_path);
    }
    else if(mime_type_str == QString("application/pdf"))
    {
        /*
             *  this check if for pdf type only. It needs to be updated
             *  for another document types.
             */

        exif_data_list = exif_data_documents_obj->pub_get_all_available_exif_data_documents(file_path);

    }


    QString command = QString("select bookmarked_exif_data from " + table_name + " where INT = ") + record_no;
    QString bookmarked_items = recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,destination_db_path,Q_FUNC_INFO);


    for(int count = 0; count < exif_data_list.size(); count++)
    {
        QStringList keyvalue_list = exif_data_list.at(count).split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

        if(keyvalue_list.size() <= 1)
            continue;

        QTreeWidgetItem *root_item = new QTreeWidgetItem();

        if(bookmarked_items.contains(keyvalue_list.at(0) + " :"))
            root_item->setCheckState(enum_global_exif_data_interface_exif_data_KEY,Qt::Checked);
        else
            root_item->setCheckState(enum_global_exif_data_interface_exif_data_KEY,Qt::Unchecked);

        root_item->setText(enum_global_exif_data_interface_exif_data_KEY  , keyvalue_list.at(0));
        root_item->setText(enum_global_exif_data_interface_exif_data_Value, keyvalue_list.at(1));
        root_item->setText(enum_global_exif_data_interface_exif_data_Plugin_Name, plugin_name);
        root_item->setText(enum_global_exif_data_interface_exif_data_Plugin_Record_no, record_no);
        root_item->setText(enum_global_exif_data_interface_exif_data_Plugin_Source_count_name, source_count_name);

        root_item->setText(enum_global_exif_data_interface_exif_data_Plugin_Table_Name, table_name);
        root_item->setText(enum_global_exif_data_interface_exif_data_Plugin_Db_Path, destination_db_path);

        tree_exif_metadata_display->addTopLevelItem(root_item);
    }

    tree_exif_metadata_display->expandAll();
    tree_exif_metadata_display->setColumnWidth(enum_global_exif_data_interface_exif_data_KEY,300);
    tree_exif_metadata_display->setColumnWidth(enum_global_exif_data_interface_exif_data_Value,400);
    tree_exif_metadata_display->setColumnHidden(enum_global_exif_data_interface_exif_data_Plugin_Name,true);
    tree_exif_metadata_display->setColumnHidden(enum_global_exif_data_interface_exif_data_Plugin_Table_Name,true);
    tree_exif_metadata_display->setColumnHidden(enum_global_exif_data_interface_exif_data_Plugin_Db_Path,true);
    tree_exif_metadata_display->setColumnHidden(enum_global_exif_data_interface_exif_data_Plugin_Record_no,true);
    tree_exif_metadata_display->setColumnHidden(enum_global_exif_data_interface_exif_data_Plugin_Source_count_name,true);

    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);

    return;
}


void exif_data_interface::slot_tree_metadata_exif_data_item_clicked(QTreeWidgetItem *item, int m_column)
{ // exif data cliked in metadata
    QString tablename = item->text(enum_global_exif_data_interface_exif_data_Plugin_Table_Name);
    QString db_path = item->text(enum_global_exif_data_interface_exif_data_Plugin_Db_Path);
    QString rec_no = item->text(enum_global_exif_data_interface_exif_data_Plugin_Record_no);
    QString key = item->text(enum_global_exif_data_interface_exif_data_KEY);
    QString value = item->text(enum_global_exif_data_interface_exif_data_Value);

    QString command_select = "select  bookmarked_exif_data from '" + tablename + "' where INT = ?";
    QStringList Int_value;
    Int_value << rec_no;
    QString stored_exif_data = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select, Int_value, 0, db_path, Q_FUNC_INFO);

    if(!stored_exif_data.endsWith("\n"))
        stored_exif_data.append("\n");

    QString exif_data_str = QString(key + " : " + value + "\n");

    if(!item->checkState(enum_global_exif_data_interface_exif_data_KEY))
    {
        if(stored_exif_data.contains(exif_data_str))
        {
            stored_exif_data.remove(exif_data_str);

            QString command = "update '" + tablename + "' set bookmarked_exif_data = ? where INT = ?";
            QStringList list;

            list << stored_exif_data << rec_no;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,db_path, Q_FUNC_INFO);

        }
    }else
    {

        if(!stored_exif_data.contains(exif_data_str))
        {
            stored_exif_data +=  QString(key + " : " + value + "\n");

            qint64 child_count = item->childCount();
            for(int mm = 0; mm < child_count; mm++)
            {
                stored_exif_data += item->child(mm)->text(1) + "\n";
            }

            QString command = "update '" + tablename + "' set bookmarked_exif_data = ? where INT = ?";
            QStringList list;
            list << stored_exif_data << rec_no;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,db_path, Q_FUNC_INFO);

        }
    }
}


bool exif_data_interface::check_exif_parent_already_exists(QString category_parent, QTreeWidget *tree_widget_obj)
{ // check aprent of exif exist or not
    QList<QTreeWidgetItem*> clist = tree_widget_obj->findItems(category_parent, Qt::MatchContains|Qt::MatchRecursive, 0);

    if(clist.isEmpty())
        return false;
    else
        return true;
}

QStringList exif_data_interface::get_exif_metadata_key_value_for_file_system(QString record_no, QString source_count_name)
{ // get exif metadata values for file system to store in db and display in metadata
    recon_static_functions::app_debug("start" , Q_FUNC_INFO);
    QStringList ext_attribute_list;

    QString exif_metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/exif_metadata.sqlite";

    QStringList list_attributes_column = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exif_Attributes_For_Metadata_Custom_Column_Name_QStringList).toStringList();
    QStringList list_key_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exif_Attributes_For_Metadata_Key_QStringList).toStringList();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(exif_metadata_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(), Q_FUNC_INFO);
        return ext_attribute_list;
    }


    QStringList values_list;
    QSqlQuery query(destination_db);


    query.prepare("select " + list_attributes_column.join(",") + " from files where filesystem_record = ?");
    query.addBindValue(record_no);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query execution ----FAILED-----" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("---Error-----" + query.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Command " + query.executedQuery(), Q_FUNC_INFO);
        return ext_attribute_list;
    }

    while(query.next())
    {
        for(int i = 0 ; i < list_attributes_column.size() ; i++)
        {
            values_list << query.value(i).toString();
        }

    }

    destination_db.close();

    if(list_attributes_column.size() != values_list.size())
        return ext_attribute_list;


    for(int i = 0 ; i < list_attributes_column.size() ; i++)
    {
        QString key = list_key_name.at(i);
        QString value = values_list.at(i);


        if(value.trimmed().isEmpty())
            continue;

        ext_attribute_list << QString(key + MACRO_RECON_Splitter_1_multiple + value);
    }


    return ext_attribute_list;
}

