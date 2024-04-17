#include "recon_configuration_page_point_hashset.h"
#include "ui_recon_configuration_page_point_hashset.h"

recon_configuration_page_point_hashset::recon_configuration_page_point_hashset(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration_page_point_hashset)
{
    ui->setupUi(this);

    recon_static_functions::app_debug(" start", Q_FUNC_INFO);

    recon_helper_standard_obj = new recon_helper_standard(this);
    recon_static_functions::app_debug(" page_point_hashset 22", Q_FUNC_INFO);

    ui->pushButton_add->setIcon(QIcon("../icons/signs/browse.png"));
    ui->pushButton_add->setHidden(true);

    ui->pushButton_remove->setIcon(QIcon("../icons/signs/remove.png"));
    ui->pushButton_create->setIcon(QIcon("../icons/signs/add.png"));

    recon_static_functions::app_debug(" page_point_hashset 33", Q_FUNC_INFO);

    ui->tableWidget_hashset->setColumnWidth(0,160);
    ui->tableWidget_hashset->setColumnWidth(1,160);

    existing_hash_column_selection_obj = new existing_hash_column_selection(this);
    connect(existing_hash_column_selection_obj , SIGNAL(signal_pushbutton_save_clicked(QString)), this, SLOT(slot_pushbutton_save_clicked(QString)));
    connect(existing_hash_column_selection_obj , SIGNAL(signal_pushbutton_cancel_clicked()), this, SLOT(slot_pushbutton_cancel_clicked()));
    connect(existing_hash_column_selection_obj , SIGNAL(signal_selected_vic_supported_file(QString)), this, SLOT(slot_selected_vic_supported_hashset_file(QString)));
    connect(existing_hash_column_selection_obj , SIGNAL(signal_selected_hash_database_file(QString)), this, SLOT(slot_selected_hash_database_file(QString)));
    recon_static_functions::app_debug(" page_point_hashset 44", Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(parent);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);
    recon_static_functions::app_debug(" page_point_hashset 55", Q_FUNC_INFO);

    create_dialog_for_add_hashset();
    recon_static_functions::app_debug(" page_point_hashset 66", Q_FUNC_INFO);
    create_dialog_for_create_new_hashset();
    recon_static_functions::app_debug(" page_point_hashset 77", Q_FUNC_INFO);
    create_dialog_for_import_vic_hashset();

    recon_helper_standard_obj->translate_tablewidget_horizontal_header_item_all(ui->tableWidget_hashset, Q_FUNC_INFO);
    recon_static_functions::app_debug(" end", Q_FUNC_INFO);

}

recon_configuration_page_point_hashset::~recon_configuration_page_point_hashset()
{
    if(dialog_add_hashset != NULL)
        delete dialog_add_hashset;

    delete ui;
}

void recon_configuration_page_point_hashset::pub_set_caller_type(QString caller_type)
{
    caller_type_str = caller_type;

    if(caller_type_str == MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG)
    {
        config_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");
        config_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("hashset/");
    }
    else if(caller_type_str == MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    {
        config_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + QString("case_configuration/case_configuration.sqlite");
        config_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + QString("case_configuration/hashset/");
    }
    else if(caller_type_str == MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER)
    {
        config_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString("case_configuration.sqlite");
        config_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString("Hashset/");
    }

    prepare_table_display();

}

void recon_configuration_page_point_hashset::slot_create_vic_hashset_clicked()
{
    dialog_add_vic_supported_hashset->hide();
    extract_and_save_vic_supported_hashset_file(lineedit_selected_vic_hashest->text().trimmed());
}

void recon_configuration_page_point_hashset::slot_cancel_vic_hashset_clicked()
{
    dialog_add_vic_supported_hashset->hide();
}

void recon_configuration_page_point_hashset::slot_selected_vic_supported_hashset_file(QString filepath)
{
    lineedit_selected_vic_hashest->setText(filepath);
    lineedit_selected_vic_hashest->setToolTip(filepath);

    QFileInfo info(filepath);
    QString hashset_default_name = info.fileName();
    hashset_default_name.remove(info.suffix());

    if(hashset_default_name.endsWith("."))
        hashset_default_name.chop(1);

    lineedit_hashest_name->setText(hashset_default_name);

    dialog_add_vic_supported_hashset->show();
}

void recon_configuration_page_point_hashset::extract_and_save_vic_supported_hashset_file(QString filepath)
{

    if(filepath.trimmed().isEmpty())
        return;

    QString destination_db_name = lineedit_hashest_name->text().trimmed();
    if(destination_db_name.isEmpty())
        return;



    QFile file_obj(filepath);
    if(!file_obj.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File open -----FAILED----:" + file_obj.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:"+ file_obj.errorString(), Q_FUNC_INFO);
        return;
    }


    QByteArray json_bytes = file_obj.readAll();
    file_obj.close();

    hashes_struct_vic_parsed_list.clear();

    QString vic_data_models_link = QString(MACRO_VIC_Data_Model_Link);
    QString vic_extn = QString(MACRO_VIC_Data_Model_Extension);
    QString vic_data_models_str = QString(MACRO_VIC_Data_Model_String);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QJsonDocument json_doc_obj = QJsonDocument::fromJson(json_bytes);
    QJsonObject  json_root_obj = json_doc_obj.object();

    QString vic_version = json_root_obj["odata.metadata"].toString();

    if(!vic_version.isEmpty())
    {
        if(vic_version.startsWith(vic_data_models_link))
            vic_version.remove(0, vic_data_models_link.size());

        if(vic_version.endsWith(vic_extn))
            vic_version.chop(vic_extn.size());

        QJsonArray json_obj_values_arr = json_root_obj["value"].toArray();

        foreach (const QJsonValue &obj_values_key, json_obj_values_arr)
        {
            QJsonObject obj_arr = obj_values_key.toObject();

            ///media details
            QJsonArray json_obj_media_arr = obj_arr["Media"].toArray();

            int rec_count = 0;

            foreach (const QJsonValue &obj_media_key, json_obj_media_arr)
            {
                rec_count++;
                if(rec_count % 50 == 0)
                {
                    QCoreApplication::processEvents();
                    rec_count = 0;
                }


                QJsonObject obj_media = obj_media_key.toObject();

                QString hash_md5 = obj_media["MD5"].toString().trimmed();

                if(hash_md5.isEmpty())
                    continue;

                QString media_id   = obj_media["MediaID"].toString().trimmed();
                QString media_size = obj_media["MediaSize"].toString().trimmed();
                QString name       = obj_media["Name"].toString().trimmed();
                QString odata_id   = obj_media["odata.id"].toString().trimmed();
                QString hash_sha1  = obj_media["SHA1"].toString().trimmed();

                struct_vic_parsed_hashes obj_VIC_parse;
                obj_VIC_parse.hash_md5  = hash_md5.toLower();
                obj_VIC_parse.hash_sha1 = hash_sha1.toLower();
                obj_VIC_parse.media_id = media_id;
                obj_VIC_parse.media_size = media_size;
                obj_VIC_parse.name = name;
                obj_VIC_parse.odata_id = odata_id;
                obj_VIC_parse.vic_version = vic_version;
                hashes_struct_vic_parsed_list.append(obj_VIC_parse);
            }
        }

    }
    else
    {
        //http://github.com/VICSDATAMODEL/ProjectVic/DataModels/2.0.xml/CUSTOM/$metadata#Media
        vic_version = json_root_obj["@odata.context"].toString();
        if(vic_version.contains(vic_data_models_str))
        {
            int tmp_start_index = vic_version.indexOf(vic_data_models_str, 0);
            tmp_start_index += vic_data_models_str.size();

            int tmp_end_index = vic_version.indexOf("/", tmp_start_index);

            QString tmp_str = vic_version.mid(tmp_start_index, (tmp_end_index - tmp_start_index)); // 2.0.xml
            if(tmp_str.endsWith(".xml"))
                tmp_str.chop(4);

            vic_version = tmp_str;
        }

        if(vic_version == QString("2.0"))
        {
            QJsonArray json_obj_values_arr = json_root_obj["value"].toArray();

            foreach (const QJsonValue &obj_values_key, json_obj_values_arr)
            {
                QJsonObject obj_arr = obj_values_key.toObject();

                ///media details
                QJsonArray json_obj_media_arr = obj_arr["Media"].toArray();

                int rec_count = 0;

                foreach (const QJsonValue &obj_media_key, json_obj_media_arr)
                {
                    rec_count++;
                    if(rec_count % 50 == 0)
                    {
                        QCoreApplication::processEvents();
                        rec_count = 0;
                    }

                    QJsonObject obj_media = obj_media_key.toObject();

                    QString hash_md5 = obj_media["MD5"].toString().trimmed();

                    if(hash_md5.isEmpty())
                        continue;

                    QString media_id   = obj_media["MediaID"].toString().trimmed();
                    QString media_size = obj_media["MediaSize"].toString().trimmed();
                    QString name       = obj_media["Name"].toString().trimmed();
                    QString odata_id   = obj_media["odata.id"].toString().trimmed();
                    QString hash_sha1  = obj_media["SHA1"].toString().trimmed();

                    struct_vic_parsed_hashes obj_VIC_parse;
                    obj_VIC_parse.hash_md5  = hash_md5.toLower();
                    obj_VIC_parse.hash_sha1 = hash_sha1.toLower();
                    obj_VIC_parse.media_id = media_id;
                    obj_VIC_parse.media_size = media_size;
                    obj_VIC_parse.name = name;
                    obj_VIC_parse.odata_id = odata_id;
                    obj_VIC_parse.vic_version = vic_version;

                    hashes_struct_vic_parsed_list.append(obj_VIC_parse);
                }
            }
        }
    }

    if(hashes_struct_vic_parsed_list.isEmpty())
    {
        parse_unknown_vic_format_file(filepath);
    }


    if(hashes_struct_vic_parsed_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    ///save in hashset database
    QString filename_str = destination_db_name;
    filename_str.replace(" ", "_");
    filename_str.append(".sqlite");

    filename_str = recon_static_functions::get_available_filename(filename_str, config_dir_path, Q_FUNC_INFO);

    QString db_file_path = config_dir_path + filename_str;
    QString command  = QString("CREATE TABLE saved_hashsets (bookmark INTEGER ,INT INTEGER PRIMARY KEY,md5_hash varchar(100),sha1_hash varchar(100))");
    recon_helper_standard_obj->execute_db_command_by_dbpath(command,db_file_path, Q_FUNC_INFO);

    command  = QString("CREATE TABLE software (name varchar(100),imported_as varchar(100), imported_filename varchar(100),imported_filepath varchar(100))");
    recon_helper_standard_obj->execute_db_command_by_dbpath(command,db_file_path, Q_FUNC_INFO);

    command  = QString("Insert into software (name,imported_as,imported_filename,imported_filepath) values(?,?,?,?)");
    QStringList value_list;
    value_list << QString("RECON") << QString("VIC") << QFileInfo(filepath).fileName() << filepath;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, db_file_path, Q_FUNC_INFO);

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    destination_db.setDatabaseName(db_file_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + db_file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return;
    }

    destination_db.transaction();

    QSqlQuery query(destination_db);

    int rec_count = 0;
    for(qint64 count = 0; count < hashes_struct_vic_parsed_list.size(); count++)
    {
        rec_count++;
        if(rec_count % 50 == 0)
        {
            QCoreApplication::processEvents();
            rec_count = 0;
        }

        struct_vic_parsed_hashes vic_parsed_hashes_obj = hashes_struct_vic_parsed_list.at(count);

        query.prepare("Insert into saved_hashsets(bookmark, md5_hash, sha1_hash) values(?,?,?)");

        query.addBindValue("0");
        query.addBindValue(vic_parsed_hashes_obj.hash_md5);
        query.addBindValue(vic_parsed_hashes_obj.hash_sha1);

        if(!query.exec())
        {
            recon_static_functions::app_debug(" query  --------FAILED------ db file - " + db_file_path,Q_FUNC_INFO);
            recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        }
    }

    destination_db.commit();
    destination_db.close();


    ///save in configuration database
    QString cmd_hashset  = QString("Insert into tbl_hashset (bookmark,table_name,column_name,database_path,is_created_by_user,recon_filefrom,recon_file_infopath) values(?,?,?,?,?,?,?)");
    QStringList values_list;
    values_list << QString("1") << QString("saved_hashsets") << QString("md5_hash") << db_file_path << QString::number(1);
    values_list << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Examiner_Selected_Path) << db_file_path;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_hashset,values_list,config_db_file, Q_FUNC_INFO);



    prepare_table_display();

    display_loading_progress_bar_non_cancelable_obj->hide();

}

void recon_configuration_page_point_hashset::slot_selected_hash_database_file(QString filepath)
{
    existing_hash_column_selection_obj->show();
}

void recon_configuration_page_point_hashset::create_dialog_for_import_vic_hashset()
{

    //-Create Dialog for create Storyboard file.
    dialog_add_vic_supported_hashset = new QDialog();
    dialog_add_vic_supported_hashset->setWindowModality(Qt::WindowModal);

    QFont font_s;
    font_s.setPixelSize(12);

    QVBoxLayout *vboxlayout_main = new QVBoxLayout(dialog_add_vic_supported_hashset);

    QLabel *label_selected_hashset = new QLabel("Selected Hashset:");
    lineedit_selected_vic_hashest = new QLineEdit(this);
    lineedit_selected_vic_hashest->setReadOnly(true);
    lineedit_selected_vic_hashest->setAttribute(Qt::WA_MacShowFocusRect,false);

    QVBoxLayout *vbox_selected_hashset = new QVBoxLayout();
    vbox_selected_hashset->addWidget(label_selected_hashset);
    vbox_selected_hashset->addWidget(lineedit_selected_vic_hashest);

    QLabel *label_hashset_name = new QLabel("Hashset Name:");
    lineedit_hashest_name = new QLineEdit(this);
    lineedit_hashest_name->setAttribute(Qt::WA_MacShowFocusRect,false);

    pushbutton_create_vic_hashset = new QPushButton(QString("Create"),this);
    pushbutton_cancel_vic_hashset = new QPushButton(QString("Cancel"),this);

    connect(pushbutton_create_vic_hashset,SIGNAL(clicked()),this,SLOT(slot_create_vic_hashset_clicked()));
    connect(pushbutton_cancel_vic_hashset,SIGNAL(clicked()),this,SLOT(slot_cancel_vic_hashset_clicked()));

    pushbutton_create_vic_hashset->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_cancel_vic_hashset->setAttribute(Qt::WA_MacShowFocusRect,false);

    QHBoxLayout *hbox_layout = new QHBoxLayout;
    QSpacerItem *h_spacer = new QSpacerItem(30,10,QSizePolicy::Expanding,QSizePolicy::Minimum);
    hbox_layout->addSpacerItem(h_spacer);
    hbox_layout->addWidget(pushbutton_cancel_vic_hashset);
    hbox_layout->addWidget(pushbutton_create_vic_hashset);
    hbox_layout->setSpacing(5);

    QVBoxLayout *vbox_hashset_name = new QVBoxLayout();
    vbox_hashset_name->addWidget(label_hashset_name);
    vbox_hashset_name->addWidget(lineedit_hashest_name);
    vbox_hashset_name->addLayout(hbox_layout);

    vboxlayout_main->addLayout(vbox_selected_hashset);
    vboxlayout_main->addLayout(vbox_hashset_name);

    vboxlayout_main->setContentsMargins(5,0,5,2);

    dialog_add_vic_supported_hashset->setLayout(vboxlayout_main);
    dialog_add_vic_supported_hashset->setWindowTitle(QString("VIC - ") + QString(MACRO_RECON_CONFIGURATION_HASHSET));
    dialog_add_vic_supported_hashset->setFont(font_s);
    dialog_add_vic_supported_hashset->setFixedSize(QSize(600, 180));

}

void recon_configuration_page_point_hashset::on_pushButton_add_clicked()
{   
    existing_hash_column_selection_obj->pub_select_file();

    ui->pushButton_add->setEnabled(false);
    ui->pushButton_remove->setEnabled(false);

}

void recon_configuration_page_point_hashset::on_pushButton_remove_clicked()
{
    int current_row = ui->tableWidget_hashset->currentRow();

    if(current_row < 0)
        return;


    QString table_str   = ui->tableWidget_hashset->item(current_row, 1)->text().trimmed();
    QString column_str  = ui->tableWidget_hashset->item(current_row, 2)->text().trimmed();
    QString db_path_str = ui->tableWidget_hashset->item(current_row, 3)->text().trimmed();

    QString cmd_hashset  = QString("Delete from tbl_hashset Where table_name = ? AND column_name = ? AND database_path= ?");
    QStringList values_list;
    values_list << table_str << column_str << db_path_str;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_hashset,values_list,config_db_file, Q_FUNC_INFO);

    prepare_table_display();
}

void recon_configuration_page_point_hashset::slot_pushbutton_save_clicked(QString saved_data_str)
{

    //saved_data_str is like this
    //table_name + QString(MACRO_RECON_Splitter) + column_name + QString(MACRO_RECON_Splitter) + sqlite_file_path;

    QStringList value_split_list;
    value_split_list = saved_data_str.split(QString(MACRO_RECON_Splitter_1_multiple));

    if(value_split_list.size() > 2)
    {
        ///save in configuration database
        QString cmd_hashset  = QString("Insert into tbl_hashset (bookmark,table_name,column_name,database_path,recon_filefrom,recon_file_infopath) values(?,?,?, ?,?,?)");
        QStringList values_list;
        values_list << QString("1") << value_split_list.at(0) << value_split_list.at(1) << value_split_list.at(2);
        values_list << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Examiner_Selected_Path)  << value_split_list.at(2);

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_hashset,values_list,config_db_file, Q_FUNC_INFO);
    }

    prepare_table_display();


    ui->pushButton_add->setEnabled(true);
    ui->pushButton_remove->setEnabled(true);

    existing_hash_column_selection_obj->hide();

}
void recon_configuration_page_point_hashset::slot_pushbutton_cancel_clicked()
{
    ui->pushButton_add->setEnabled(true);
    ui->pushButton_remove->setEnabled(true);

    existing_hash_column_selection_obj->hide();
}

void recon_configuration_page_point_hashset::prepare_table_display()
{
    if(caller_type_str != QString(MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER))
        ui->tableWidget_hashset->hideColumn(0);


    ui->tableWidget_hashset->setRowCount(0);
    ui->tableWidget_hashset->setColumnWidth(0,30);

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(config_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString command = QString("select bookmark,table_name,column_name,database_path from tbl_hashset");

    QSqlQuery query(destination_db);
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }


    int row = 0;
    while(query.next())
    {
        ui->tableWidget_hashset->insertRow(row);
        ui->tableWidget_hashset->setRowHeight(row, 23);

        QTableWidgetItem *item_bookmark  = new QTableWidgetItem;
        QTableWidgetItem *item_table  = new QTableWidgetItem;
        QTableWidgetItem *item_column = new QTableWidgetItem;
        QTableWidgetItem *item_database_path = new QTableWidgetItem;

        if(query.value(0).toString().trimmed() == QString::number(1))
            item_bookmark->setCheckState(Qt::Checked);
        else
            item_bookmark->setCheckState(Qt::Unchecked);


        item_table->setText(query.value(1).toString().trimmed());
        item_column->setText(query.value(2).toString().trimmed());
        item_database_path->setText(query.value(3).toString().trimmed());

        ui->tableWidget_hashset->setItem(row , 0 , item_bookmark);
        ui->tableWidget_hashset->setItem(row , 1 , item_table);
        ui->tableWidget_hashset->setItem(row , 2 , item_column);
        ui->tableWidget_hashset->setItem(row , 3 , item_database_path);

        row++;
    }

    destination_db.close();

}

void recon_configuration_page_point_hashset::on_tableWidget_hashset_cellClicked(int row, int column)
{
    if(row < 0)
        return;

    if(ui->tableWidget_hashset->rowCount() > 0)
        ui->pushButton_remove->setEnabled(true);

    if(column == 0)
    {
        QTableWidgetItem *item = ui->tableWidget_hashset->item(row, column);

        QString table_str   = ui->tableWidget_hashset->item(row, 1)->text().trimmed();
        QString column_str  = ui->tableWidget_hashset->item(row, 2)->text().trimmed();
        QString db_path_str = ui->tableWidget_hashset->item(row, 3)->text().trimmed();

        QStringList values_list;


        QString command = QString("Update tbl_hashset SET bookmark = ? Where table_name = ? AND column_name = ? AND database_path= ?");
        QString bookmark_value;
        if(item->checkState() == Qt::Checked)
        {
            bookmark_value = QString::number(1);
        }
        else
        {
            bookmark_value = QString::number(0);
        }

        values_list << bookmark_value << table_str << column_str << db_path_str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,values_list,config_db_file, Q_FUNC_INFO);

    }
}

void recon_configuration_page_point_hashset::on_pushButton_create_clicked()
{

    dialog_add_hashset->show();;

}


void recon_configuration_page_point_hashset::create_dialog_for_create_new_hashset()
{
    //-Create Dialog for create Storyboard file.
    dialog_create_new_hashset = new QDialog(this);
    dialog_create_new_hashset->setWindowModality(Qt::WindowModal);

    QFont font_s;
    font_s.setPixelSize(12);

    QVBoxLayout *vboxlayout_storyboard = new QVBoxLayout(dialog_create_new_hashset);

    lineedit_hashset_label = new QLineEdit(this);
    lineedit_hashset_label->setPlaceholderText(global_lang_tr_obj.get_translated_string("Hashset Name"));

    connect(lineedit_hashset_label,SIGNAL(textChanged(QString)),SLOT(slot_lineedit_hashset_label_text_changed(QString)));

    pushbutton_create_hashset = new QPushButton(global_lang_tr_obj.get_translated_string("Create"),this);
    pushbutton_create_hashset->setFixedWidth(80);
    pushbutton_create_hashset->setFont(font_s);
    connect(pushbutton_create_hashset,SIGNAL(clicked()),this,SLOT(slot_hashset_dialog_save_clicked()));
    pushbutton_create_hashset->setEnabled(false);

    //    lineedit_hashset_md5_hash->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_hashset_label->setAttribute(Qt::WA_MacShowFocusRect,false);

    pushbutton_create_hashset->setAttribute(Qt::WA_MacShowFocusRect,false);

    QPushButton *cancel_button = new QPushButton(global_lang_tr_obj.get_translated_string("Cancel"),this);
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(slot_hashset_dialog_cancel_clicked()));
    cancel_button->setFixedWidth(80);


    QHBoxLayout *hbox_pushbutton_layout = new QHBoxLayout;

    hbox_pushbutton_layout->addStretch(1);
    hbox_pushbutton_layout->addWidget(cancel_button);
    hbox_pushbutton_layout->addWidget(pushbutton_create_hashset);

    //    vboxlayout_storyboard->addWidget(lineedit_hashset_md5_hash);
    vboxlayout_storyboard->addWidget(lineedit_hashset_label);

    vboxlayout_storyboard->addLayout(hbox_pushbutton_layout);
    vboxlayout_storyboard->setContentsMargins(9,9,9,6);

    dialog_create_new_hashset->setLayout(vboxlayout_storyboard);
    dialog_create_new_hashset->setWindowTitle(global_lang_tr_obj.get_translated_string(MACRO_RECON_CONFIGURATION_HASHSET));
    dialog_create_new_hashset->setMinimumWidth(200);
    dialog_create_new_hashset->setFont(font_s);

    dialog_create_new_hashset->setFixedSize(QSize(350, 120));
}

void recon_configuration_page_point_hashset::create_dialog_for_add_hashset()
{
    //-Create Dialog for create Storyboard file.
    dialog_add_hashset = new QDialog();
    dialog_add_hashset->setWindowModality(Qt::WindowModal);

    QFont font_s;
    font_s.setPixelSize(12);

    QVBoxLayout *vboxlayout_hashset = new QVBoxLayout(dialog_add_hashset);

    pushbutton_create_new_hashset = new QPushButton(global_lang_tr_obj.get_translated_string("Create New"),this);;
    pushbutton_existing_hashset   = new QPushButton(global_lang_tr_obj.get_translated_string("Existing"),this);;

    pushbutton_create_new_hashset->setFixedSize(QSize(95, 27));
    pushbutton_existing_hashset->setFixedSize(QSize(95, 27));

    connect(pushbutton_create_new_hashset,SIGNAL(clicked()),this,SLOT(slot_hashset_dialog_create_new_clicked()));
    connect(pushbutton_existing_hashset,SIGNAL(clicked()),this,SLOT(slot_hashset_dialog_add_existing_clicked()));

    pushbutton_create_new_hashset->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_existing_hashset->setAttribute(Qt::WA_MacShowFocusRect,false);

    QHBoxLayout *hbox_layout = new QHBoxLayout;
    hbox_layout->addWidget(pushbutton_create_new_hashset);
    hbox_layout->addWidget(pushbutton_existing_hashset);

    vboxlayout_hashset->addLayout(hbox_layout);

    vboxlayout_hashset->setContentsMargins(9,9,9,6);

    dialog_add_hashset->setLayout(vboxlayout_hashset);
    dialog_add_hashset->setWindowTitle(global_lang_tr_obj.get_translated_string(MACRO_RECON_CONFIGURATION_HASHSET));
    dialog_add_hashset->setMinimumWidth(200);
    dialog_add_hashset->setFont(font_s);
    dialog_add_hashset->setFixedSize(QSize(250, 80));
}


void recon_configuration_page_point_hashset::slot_hashset_dialog_create_new_clicked()
{
    dialog_add_hashset->hide();
    dialog_create_new_hashset->show();
}

void recon_configuration_page_point_hashset::slot_hashset_dialog_add_existing_clicked()
{
    dialog_add_hashset->hide();
    on_pushButton_add_clicked();
}

void recon_configuration_page_point_hashset::slot_hashset_dialog_save_clicked()
{
    dialog_create_new_hashset->hide();

    //    QString current_hash_str = lineedit_hashset_md5_hash->text().trimmed();

    //    if(current_hash_str.isEmpty())
    //        return;

    QString filename_str = lineedit_hashset_label->text().trimmed();
    filename_str.replace(" ", "_");
    filename_str.append(".sqlite");

    filename_str = recon_static_functions::get_available_filename(filename_str, config_dir_path, Q_FUNC_INFO);

    QString db_file_path = config_dir_path + filename_str;
    QString command  = QString("CREATE TABLE saved_hashsets (bookmark INTEGER ,INT INTEGER PRIMARY KEY,md5_hash varchar(100),sha1_hash varchar(100))");
    recon_helper_standard_obj->execute_db_command_by_dbpath(command,db_file_path, Q_FUNC_INFO);

    command  = QString("CREATE TABLE software (name varchar(100))");
    recon_helper_standard_obj->execute_db_command_by_dbpath(command,db_file_path, Q_FUNC_INFO);

    command  = QString("Insert into software (name) values(?)");
    QStringList value_list;
    value_list << "RECON";
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, db_file_path, Q_FUNC_INFO);


    ///save in configuration database
    QString cmd_hashset  = QString("Insert into tbl_hashset (bookmark,table_name,column_name,database_path,is_created_by_user,recon_filefrom,recon_file_infopath) values(?,?,?, ?,?,?, ?)");
    QStringList values_list;
    values_list << QString("1") << QString("saved_hashsets") << QString("md5_hash") << db_file_path << QString::number(1);
    values_list << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Examiner_Selected_Path) << db_file_path;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_hashset,values_list,config_db_file, Q_FUNC_INFO);

    prepare_table_display();

}

void recon_configuration_page_point_hashset::slot_hashset_dialog_cancel_clicked()
{
    dialog_create_new_hashset->hide();
}

void recon_configuration_page_point_hashset::slot_lineedit_hashset_md5_text_changed(QString text_str)
{
    pushbutton_create_hashset->setEnabled(false);

    QString current_text = text_str.trimmed();

    if(current_text.isEmpty())
        return;

    if(lineedit_hashset_label->text().trimmed().isEmpty())
        return;

    pushbutton_create_hashset->setEnabled(true);

}

void recon_configuration_page_point_hashset::slot_lineedit_hashset_label_text_changed(QString text_str)
{
    pushbutton_create_hashset->setEnabled(false);

    QString current_text = text_str.trimmed();

    if(current_text.isEmpty())
        return;

    //    if(lineedit_hashset_md5_hash->text().trimmed().isEmpty())
    //        return;

    pushbutton_create_hashset->setEnabled(true);

}

void recon_configuration_page_point_hashset::parse_unknown_vic_format_file(QString filepath)
{
    QFile file(filepath);

    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File Open -----FAILED-----" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + file.errorString(), Q_FUNC_INFO);
        return;
    }

    int count         = 0;
    int move_index_at = 0;
    QString line_str;

    QString md5_data_str;
    QString md5_start_upper_str = QString("\"MD5\":");
    QString md5_start_lower_str = QString("\"md5\":");
    int md5_start_index = 0;
    int md5_end_index   = 0;

    QString sha1_data_str;

    QByteArray file_data_arr;

    while(!file.atEnd())
    {
        count++;
        if(count % 20 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        file_data_arr = file.read(1024);

        //MD5 and SHA1 Parse
        if(file_data_arr.contains("MD5") || file_data_arr.contains("md5"))
        {
            int tmp_count = 0;
            do
            {
                tmp_count++;
                if(tmp_count % 100 == 0)
                {
                    tmp_count = 0;
                    QCoreApplication::processEvents();
                }

                md5_start_index = file_data_arr.indexOf(md5_start_upper_str.toStdString(), md5_end_index);
                move_index_at = md5_start_upper_str.size();

                if(md5_start_index < 0)
                {
                    md5_start_index = file_data_arr.indexOf(md5_start_lower_str.toStdString(), md5_end_index);
                    move_index_at = md5_start_lower_str.size();
                }

                if(md5_start_index >= 0)
                {
                    md5_start_index += move_index_at;
                    md5_end_index = file_data_arr.indexOf(",", md5_start_index);

                    if(md5_end_index > md5_start_index)
                    {
                        md5_data_str = file_data_arr.mid(md5_start_index, (md5_end_index - md5_start_index));
                        md5_data_str.replace("\"", "");
                        md5_data_str = md5_data_str.trimmed();

                        //SHA1 Parse
                        sha1_data_str = get_sha1_hash_for_unknown_vic_format(file_data_arr, md5_end_index);

                        struct_vic_parsed_hashes obj_VIC_parse;
                        obj_VIC_parse.hash_md5  = md5_data_str.toLower();
                        obj_VIC_parse.hash_sha1 = sha1_data_str.toLower();
                        obj_VIC_parse.media_id = QString("");
                        obj_VIC_parse.media_size = QString("");
                        obj_VIC_parse.name = QString("");
                        obj_VIC_parse.odata_id = QString("");
                        obj_VIC_parse.vic_version = QString("");

                        hashes_struct_vic_parsed_list.append(obj_VIC_parse);

                    }
                }

            }while(md5_start_index != -1);

            md5_start_index = 0;
            md5_end_index   = 0;
        }
    }

    file.close();
}

QString recon_configuration_page_point_hashset::get_sha1_hash_for_unknown_vic_format(QByteArray &file_data_arr, int md5_end_index)
{
    QString sha1_data_str;
    QString sha1_start_upper_str = QString("\"SHA1\":");
    QString sha1_start_lower_str = QString("\"sha1\":");
    int sha1_start_index = 0;
    int sha1_end_index   = 0;
    int move_index_at    = 0;


    sha1_start_index = file_data_arr.indexOf(sha1_start_upper_str.toStdString(), md5_end_index);
    move_index_at = sha1_start_upper_str.size();

    if(sha1_start_index < 0)
    {
        sha1_start_index = file_data_arr.indexOf(sha1_start_lower_str.toStdString(), sha1_end_index);
        move_index_at = sha1_start_lower_str.size();
    }

    if(sha1_start_index >= 0)
    {
        sha1_start_index += move_index_at;
        sha1_end_index = file_data_arr.indexOf(",", sha1_start_index);

        if(sha1_end_index > sha1_start_index)
        {
            sha1_data_str = file_data_arr.mid(sha1_start_index, (sha1_end_index - sha1_start_index));
            sha1_data_str.replace("\"", "");
            sha1_data_str = sha1_data_str.trimmed();
        }
    }

    return sha1_data_str;
}
