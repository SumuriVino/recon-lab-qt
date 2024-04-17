#include "hashset_display_toolbar.h"
#include "ui_hashset_display_toolbar.h"

hashset_display_toolbar::hashset_display_toolbar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hashset_display_toolbar)
{
    ui->setupUi(this);

    ui->tableWidget_hashset->setColumnWidth(enum_hashset_INT,         100);
    ui->tableWidget_hashset->setColumnWidth(enum_hashset_name,        150);
    ui->tableWidget_hashset->setColumnWidth(enum_hashset_db_name,     150);
    ui->tableWidget_hashset->setColumnWidth(enum_hashset_show_files,  200);
    ui->tableWidget_hashset->setColumnWidth(enum_hashset_hide_files,  200);
    ui->tableWidget_hashset->setColumnWidth(enum_hashset_md5_column,  150);
    ui->tableWidget_hashset->setColumnWidth(enum_hashset_sha1_column, 150);

    ui->tableWidget_hashset->hideColumn(enum_hashset_INT);
    ui->tableWidget_hashset->hideColumn(enum_hashset_db_name);
    ui->tableWidget_hashset->hideColumn(enum_hashset_md5_column);
    ui->tableWidget_hashset->hideColumn(enum_hashset_sha1_column);
    ui->tableWidget_hashset->hideColumn(enum_hashset_show_files);

    ui->pushButton_remove->setEnabled(false);

    recon_helper_standard_obj = new recon_helper_standard(this);

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);

    create_dialog_for_create_new_sqlite_hashset();

    create_dialog_for_import_vic_hashset();
    create_dialog_for_import_nsrl_hashset();
    create_dialog_for_import_csv_hashset();

    QMenu *menu_add_new = new QMenu(this);

    QAction *action_add_db   = new QAction("SQLite", this);
    QAction *action_add_VIC  = new QAction("Project VIC", this);
    QAction *action_add_NSRL = new QAction("NSRL", this);
    QAction *action_add_CSV  = new QAction("CSV", this);


    connect(action_add_db ,  SIGNAL(triggered(bool)), this,SLOT(slot_action_add_db_clicked(bool)));
    connect(action_add_VIC,  SIGNAL(triggered(bool)), this,SLOT(slot_action_add_VIC_clicked(bool)));
    connect(action_add_NSRL, SIGNAL(triggered(bool)), this,SLOT(slot_action_add_NSRL_clicked(bool)));
    connect(action_add_CSV,  SIGNAL(triggered(bool)), this,SLOT(slot_action_add_CSV_clicked(bool)));

    menu_add_new->addAction(action_add_db);
    menu_add_new->addSeparator();

    menu_add_new->addAction(action_add_VIC);
    menu_add_new->addSeparator();

    menu_add_new->addAction(action_add_NSRL);
    menu_add_new->addSeparator();
    menu_add_new->addAction(action_add_CSV);

    ui->pushButton_import->setMenu(menu_add_new);

    existing_hash_column_selection_obj = new existing_hash_column_selection(this);
    connect(existing_hash_column_selection_obj , SIGNAL(signal_pushbutton_save_clicked(QString)), this, SLOT(slot_pushbutton_existing_sqlite_hash_save_clicked(QString)));
    connect(existing_hash_column_selection_obj , SIGNAL(signal_pushbutton_cancel_clicked()), this, SLOT(slot_pushbutton_existing_sqlite_hash_cancel_clicked()));
    connect(existing_hash_column_selection_obj , SIGNAL(signal_selected_hash_database_file(QString)), this, SLOT(slot_existing_hash_selected_hash_database_file(QString)));
    connect(existing_hash_column_selection_obj , SIGNAL(signal_selected_vic_supported_file(QString)), this, SLOT(slot_existing_hash_selected_vic_supported_hashset_file(QString)));


    file_dialog_obj.setOption(QFileDialog::DontUseNativeDialog);
    QList<QUrl> drives;
    drives << QUrl::fromLocalFile(QDir("/Volumes/").absolutePath());
    drives << QUrl::fromLocalFile(QDir("/").absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Downloads")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Desktop")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Documents")).absolutePath());
    file_dialog_obj.setSidebarUrls(drives);
    file_dialog_obj.setDirectory(getenv("HOME"));



}

hashset_display_toolbar::~hashset_display_toolbar()
{
    delete dialog_add_nsrl_supported_hashset;
    delete dialog_add_vic_supported_hashset;
    delete dialog_add_csv_supported_hashset;

    delete ui;
}

void hashset_display_toolbar::pub_set_hashset_toolbar_essentials()
{
    hashset_dir_path     = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Hashset_In_Result_QString).toString();
    hashes_index_db_path = hashset_dir_path + "hashset_index.sqlite";

    prepare_table_display_for_saved_hashsets();
}

void hashset_display_toolbar::on_pushButton_remove_clicked()
{
    int current_row = ui->tableWidget_hashset->currentRow();
    if(current_row < 0)
        return;

    QString int_str = ui->tableWidget_hashset->item(current_row, enum_hashset_INT)->text().trimmed();
    if(int_str.isEmpty())
        return;

    QString cmd_delete = QString("Delete from hashset_indexes Where INT= ?");

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_delete, QStringList(int_str), hashes_index_db_path, Q_FUNC_INFO);

    prepare_table_display_for_saved_hashsets();

}

void hashset_display_toolbar::on_pushButton_create_clicked()
{
    dialog_create_new_hashset->show();
}

void hashset_display_toolbar::on_pushButton_apply_clicked()
{
    this->hide();

    //    QString msg_hashes = global_lang_tr_obj.get_translated_string(QString("Hashes will run automatically in case wasn't run earlier."));
    //    QString msg_str = global_lang_tr_obj.get_translated_string(QString("Files which have already been added in other features can not be hidden from case."));

    //    QString final_msg_str = msg_hashes + QString("\n\n") + msg_str;

    //    QMessageBox::information(this, recon_static_functions::get_app_name(), final_msg_str);


    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(hashes_index_db_path);

    if (!destination_db.open())
    {
        recon_static_functions::app_debug(" db open ------FAILED-------" + destination_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db.lastError().text() ,Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        return ;
    }

    QString command_index_db = QString("Select saved_db_name,recon_file_infopath,recon_filefrom,saved_table_name,md5_hash_column,saved_db_path From hashset_indexes Where show_files = ?");

    QSqlQuery query_index(destination_db);

    query_index.prepare(command_index_db);
    query_index.addBindValue(QString(MACRO_TRUE_VALUE_NUMERIC_QString));

    if(!query_index.exec())
    {
        recon_static_functions::app_debug("query execute ------FAILED-------" + query_index.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_index.lastError().text() ,Q_FUNC_INFO);
        destination_db.close();
        QSqlDatabase::removeDatabase(connection_naam);
        return ;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QStringList available_hashset_list;
    QString result_conf_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Parent_In_Result_QString).toString();
    QString result_dir_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    ///add only visible hashsets, so delete all previous
    QString conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString command_delete_conf_hashes = QString("Delete from tbl_hashset");
    recon_helper_standard_obj->execute_db_command_by_dbpath(command_delete_conf_hashes,conf_db_path, Q_FUNC_INFO);


    QString command_insert_conf_hashes = QString("Insert into tbl_hashset(table_name,column_name,database_path,is_created_by_user,recon_filefrom,recon_file_infopath,bookmark) Values(?,?,?,?,?,?,?)");

    while(query_index.next())
    {

        QString saved_db_name        = query_index.value("saved_db_name").toString().trimmed();
        QString recon_file_infopath  = query_index.value("recon_file_infopath").toString().trimmed();
        QString recon_filefrom       = query_index.value("recon_filefrom").toString().trimmed();
        QString saved_table_name     = query_index.value("saved_table_name").toString().trimmed();
        QString md5_hash_column      = query_index.value("md5_hash_column").toString().trimmed();

        QString hashset_name = saved_db_name;
        if(hashset_name.endsWith(".sqlite"))
            hashset_name.chop(QString(".sqlite").size());

        hashset_name = hashset_name.trimmed();

        QString dbpath;
        QString is_created_by_user;
        if(recon_filefrom == QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result))
        {
            if(recon_file_infopath.startsWith(QString(MACRO_CASE_TREE_Hashset)))
                dbpath = result_conf_dir_path + recon_file_infopath;
            else
                dbpath = result_dir_path + recon_file_infopath;

            is_created_by_user = QString(MACRO_TRUE_VALUE_NUMERIC_QString);
        }
        else
        {
            dbpath = recon_file_infopath;
            is_created_by_user = QString(MACRO_FALSE_VALUE_NUMERIC_QString);
        }

        available_hashset_list << hashset_name + MACRO_RECON_Splitter_1_multiple + dbpath;

        QStringList value_list_insert;
        value_list_insert << saved_table_name << md5_hash_column << dbpath << is_created_by_user << recon_filefrom << recon_file_infopath << QString::number(1);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert_conf_hashes,value_list_insert, conf_db_path, Q_FUNC_INFO);
    }


    ///delete hidden hashsets from configuration
    command_index_db = QString("Select saved_db_name,recon_file_infopath,recon_filefrom,saved_table_name,md5_hash_column,saved_db_path From hashset_indexes Where hide_files = ?");

    query_index.prepare(command_index_db);
    query_index.addBindValue(QString(MACRO_TRUE_VALUE_NUMERIC_QString));

    if(!query_index.exec())
    {
        recon_static_functions::app_debug("query execute ------FAILED-------" + query_index.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_index.lastError().text() ,Q_FUNC_INFO);
        destination_db.close();
        QSqlDatabase::removeDatabase(connection_naam);
        display_loading_progress_bar_non_cancelable_obj->hide();
        return ;
    }

    while(query_index.next())
    {
        QString saved_db_name        = query_index.value("saved_db_name").toString().trimmed();
        QString recon_file_infopath  = query_index.value("recon_file_infopath").toString().trimmed();
        QString recon_filefrom       = query_index.value("recon_filefrom").toString().trimmed();
        QString saved_table_name     = query_index.value("saved_table_name").toString().trimmed();
        QString md5_hash_column      = query_index.value("md5_hash_column").toString().trimmed();

        QString hashset_name = saved_db_name;
        if(hashset_name.endsWith(".sqlite"))
            hashset_name.chop(QString(".sqlite").size());

        hashset_name = hashset_name.trimmed();

        QString dbpath;
        if(recon_filefrom == QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result))
        {
            dbpath = result_conf_dir_path + recon_file_infopath;
        }
        else
        {
            dbpath = recon_file_infopath;
        }

        QStringList value_list_delete;
        value_list_delete << saved_table_name << md5_hash_column << dbpath;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_delete_conf_hashes,value_list_delete, conf_db_path, Q_FUNC_INFO);
    }

    destination_db.close();

    display_loading_progress_bar_non_cancelable_obj->hide();

    emit signal_hashset_toolbar_apply_clicked(available_hashset_list);

    if(available_hashset_list.size() <= 0)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), global_lang_tr_obj.get_translated_string("No Hashset available."));
        return;
    }

}

void hashset_display_toolbar::on_pushButton_cancel_clicked()
{
    this->hide();
}

void hashset_display_toolbar::on_tableWidget_hashset_cellChanged(int row, int column)
{
    ui->pushButton_remove->setEnabled(false);

    if(row < 0 || column < 0)
        return;

    ui->pushButton_remove->setEnabled(true);

}

void hashset_display_toolbar::slot_action_add_db_clicked(bool status)
{
    existing_hash_column_selection_obj->pub_set_hashset_toolbar_essentials(true, false);
    existing_hash_column_selection_obj->pub_select_file_for_hashset_toolbar();
}

void hashset_display_toolbar::slot_action_add_VIC_clicked(bool status)
{
    existing_hash_column_selection_obj->pub_set_hashset_toolbar_essentials(false, true);
    existing_hash_column_selection_obj->pub_select_file_for_hashset_toolbar();
}

void hashset_display_toolbar::create_dialog_for_create_new_sqlite_hashset()
{
    dialog_create_new_hashset = new QDialog(this);
    dialog_create_new_hashset->setWindowModality(Qt::ApplicationModal);

    QFont font_s;
    font_s.setPixelSize(12);

    QVBoxLayout *vboxlayout_main = new QVBoxLayout(dialog_create_new_hashset);

    lineedit_hashset_label = new QLineEdit(this);
    lineedit_hashset_label->setPlaceholderText(QString("Hashset Name"));

    connect(lineedit_hashset_label,SIGNAL(textChanged(QString)),SLOT(slot_lineedit_hashset_label_text_changed(QString)));

    pushbutton_create_hashset = new QPushButton(QString("Create"),this);
    pushbutton_create_hashset->setFixedWidth(80);
    pushbutton_create_hashset->setFont(font_s);
    connect(pushbutton_create_hashset,SIGNAL(clicked()),this,SLOT(slot_hashset_dialog_create_sqlite_clicked()));
    pushbutton_create_hashset->setEnabled(false);

    lineedit_hashset_label->setAttribute(Qt::WA_MacShowFocusRect,false);

    pushbutton_create_hashset->setAttribute(Qt::WA_MacShowFocusRect,false);

    QPushButton *cancel_button = new QPushButton("Cancel",this);
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(slot_hashset_dialog_cancel_sqlite_clicked()));
    cancel_button->setFixedWidth(80);


    QHBoxLayout *hbox_pushbutton_layout = new QHBoxLayout;

    hbox_pushbutton_layout->addStretch(1);
    hbox_pushbutton_layout->addWidget(cancel_button);
    hbox_pushbutton_layout->addWidget(pushbutton_create_hashset);

    vboxlayout_main->addWidget(lineedit_hashset_label);

    vboxlayout_main->addLayout(hbox_pushbutton_layout);
    vboxlayout_main->setContentsMargins(9,9,9,6);

    dialog_create_new_hashset->setLayout(vboxlayout_main);
    dialog_create_new_hashset->setWindowTitle(QString(MACRO_RECON_CONFIGURATION_HASHSET));
    dialog_create_new_hashset->setMinimumWidth(200);
    dialog_create_new_hashset->setFont(font_s);

    dialog_create_new_hashset->setFixedSize(QSize(350, 120));
}

void hashset_display_toolbar::slot_lineedit_hashset_label_text_changed(QString text_str)
{
    pushbutton_create_hashset->setEnabled(false);

    QString current_text = text_str.trimmed();
    if(current_text.isEmpty())
        return;

    pushbutton_create_hashset->setEnabled(true);

}

void hashset_display_toolbar::slot_hashset_dialog_create_sqlite_clicked()
{

    //dialog_create_new_hashset->hide();

    QString filename_str = lineedit_hashset_label->text().trimmed();

    if(filename_str.isEmpty())
        return;

    dialog_create_new_hashset->hide();


    filename_str.replace(" ", "_");
    filename_str.append(".sqlite");

    filename_str = recon_static_functions::get_available_filename(filename_str, hashset_dir_path, Q_FUNC_INFO);

    QString db_file_path = hashset_dir_path + filename_str;
    QString command  = QString("CREATE TABLE saved_hashsets (bookmark INTEGER ,INT INTEGER PRIMARY KEY,md5_hash varchar(100),sha1_hash varchar(100))");
    recon_helper_standard_obj->execute_db_command_by_dbpath(command,db_file_path, Q_FUNC_INFO);

    command  = QString("CREATE TABLE software (name varchar(100))");
    recon_helper_standard_obj->execute_db_command_by_dbpath(command,db_file_path, Q_FUNC_INFO);

    command  = QString("Insert into software (name) values(?)");
    QStringList value_list;
    value_list << "RECON";
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, db_file_path, Q_FUNC_INFO);


    ///save in indexes database
    QString recon_file_infopath = db_file_path;

    QString result_dir_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    if(recon_file_infopath.startsWith(result_dir_path))
        recon_file_infopath.remove(0, result_dir_path.size());


    QString recon_file_from_result = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);

    QString cmd_hashset  = QString("Insert into hashset_indexes (saved_db_name,show_files,hide_files,md5_hash_column,sha1_hash_column,was_imported_external,"
                                   "saved_db_path,recon_filefrom,recon_file_infopath,saved_table_name) values(?,?,?,?,? ,?,?,?,?,?)");
    QStringList values_list;
    values_list << filename_str << QString("1") << QString("0") << QString("md5_hash") << QString("sha1_hash")
                << QString(MACRO_FALSE_VALUE_NUMERIC_QString) << db_file_path << recon_file_from_result << recon_file_infopath << QString("saved_hashsets");

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_hashset,values_list, hashes_index_db_path, Q_FUNC_INFO);

    prepare_table_display_for_saved_hashsets();

}

void hashset_display_toolbar::slot_hashset_dialog_cancel_sqlite_clicked()
{
    dialog_create_new_hashset->hide();
}

void hashset_display_toolbar::create_dialog_for_import_vic_hashset()
{

    dialog_add_vic_supported_hashset = new QDialog();
    dialog_add_vic_supported_hashset->setWindowModality(Qt::ApplicationModal);

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
    vbox_selected_hashset->setSpacing(0);
    vboxlayout_main->addLayout(vbox_selected_hashset);

    QLabel *label_hashset_name = new QLabel("Hashset Name:");
    lineedit_vic_hashest_name = new QLineEdit(this);
    lineedit_vic_hashest_name->setAttribute(Qt::WA_MacShowFocusRect,false);

    QRegularExpression rx("^[a-zA-Z0-9_ -]*$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    lineedit_vic_hashest_name->setValidator(validator);


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

    QVBoxLayout *vbox_hashset_name = new QVBoxLayout();
    vbox_hashset_name->addWidget(label_hashset_name);
    vbox_hashset_name->addWidget(lineedit_vic_hashest_name);
    vbox_selected_hashset->setSpacing(0);

    vboxlayout_main->addLayout(vbox_hashset_name);
    vboxlayout_main->addLayout(hbox_layout);

    vboxlayout_main->setSpacing(5);
    vboxlayout_main->setContentsMargins(5,0,5,0);

    dialog_add_vic_supported_hashset->setLayout(vboxlayout_main);
    dialog_add_vic_supported_hashset->setWindowTitle(QString("VIC - ") + QString(MACRO_RECON_CONFIGURATION_HASHSET));
    dialog_add_vic_supported_hashset->setFont(font_s);
    dialog_add_vic_supported_hashset->setFixedSize(QSize(600, 130));

    dialog_add_vic_supported_hashset->setContentsMargins(0,0,0,0);

}

void hashset_display_toolbar::slot_create_vic_hashset_clicked()
{
    dialog_add_vic_supported_hashset->hide();
    extract_and_save_vic_supported_hashset_file(lineedit_selected_vic_hashest->text().trimmed());
}

void hashset_display_toolbar::slot_cancel_vic_hashset_clicked()
{
    dialog_add_vic_supported_hashset->hide();
}

void hashset_display_toolbar::extract_and_save_vic_supported_hashset_file(QString filepath)
{
    if(filepath.trimmed().isEmpty())
        return;

    QString destination_db_name = lineedit_vic_hashest_name->text().trimmed();
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

    QJsonDocument json_doc_obj = QJsonDocument::fromJson(json_bytes);
    QJsonObject  json_root_obj = json_doc_obj.object();



    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

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
                    QString griffeye_category  = obj_media["Tags"].toString().trimmed();

                    struct_vic_parsed_hashes obj_VIC_parse;
                    obj_VIC_parse.hash_md5  = hash_md5.toLower();
                    obj_VIC_parse.hash_sha1 = hash_sha1.toLower();
                    obj_VIC_parse.media_id = media_id;
                    obj_VIC_parse.media_size = media_size;
                    obj_VIC_parse.name = name;
                    obj_VIC_parse.odata_id = odata_id;
                    obj_VIC_parse.vic_version = vic_version;

                    if(griffeye_category.startsWith("#"))
                        griffeye_category.remove("#");

                    obj_VIC_parse.griffeye_tags = griffeye_category;

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

    filename_str = recon_static_functions::get_available_filename(filename_str, hashset_dir_path, Q_FUNC_INFO);

    QString db_file_path = hashset_dir_path + filename_str;
    QString command = QString("CREATE TABLE saved_hashsets (bookmark INTEGER, INT INTEGER PRIMARY KEY, md5_hash varchar(100), sha1_hash varchar(100), griffeye_category varchar(100))");

    recon_helper_standard_obj->execute_db_command_by_dbpath(command,db_file_path, Q_FUNC_INFO);

    //    command  = QString("CREATE TABLE software (name varchar(100),imported_as varchar(100), imported_filename varchar(100),imported_filepath varchar(100))");
    //    recon_helper_standard_obj->execute_db_command_by_dbpath(command,db_file_path, Q_FUNC_INFO);

    //    command  = QString("Insert into software (name,imported_as,imported_filename,imported_filepath) values(?,?,?,?)");
    //    QStringList value_list;
    //    value_list << QString("RECON") << QString("VIC") << QFileInfo(filepath).fileName() << filepath;
    //    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, db_file_path, Q_FUNC_INFO);

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

        query.prepare("Insert into saved_hashsets(bookmark, md5_hash, sha1_hash, griffeye_category) values(?,?,?,?)");

        query.addBindValue("0");
        query.addBindValue(vic_parsed_hashes_obj.hash_md5);
        query.addBindValue(vic_parsed_hashes_obj.hash_sha1);

        query.addBindValue(vic_parsed_hashes_obj.griffeye_tags);
        if(!query.exec())
        {
            recon_static_functions::app_debug(" query  --------FAILED------ db file - " + db_file_path,Q_FUNC_INFO);
            recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        }
    }

    destination_db.commit();


    QString index_cmd = QString("CREATE INDEX hashset_index ON saved_hashsets(md5_hash)");
    recon_helper_standard_obj->execute_db_command_by_dbreference(index_cmd, destination_db, Q_FUNC_INFO);


    destination_db.close();


    ///save in indexes database
    QString recon_file_from_by_user = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);

    QString recon_file_infopath = db_file_path;
    QString result_dir_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    if(recon_file_infopath.startsWith(result_dir_path))
        recon_file_infopath.remove(0, result_dir_path.size() - 1);


    QString cmd_hashset  = QString("Insert into hashset_indexes (saved_db_name,show_files,hide_files,md5_hash_column,sha1_hash_column,was_imported_external,"
                                   "saved_db_path,recon_filefrom,recon_file_infopath,saved_table_name) values(?,?,?,?,? ,?,?,?,?,?)");
    QStringList values_list;
    values_list << filename_str << QString("1") << QString("0") << QString("md5_hash") << QString("sha1_hash")
                << QString(MACRO_TRUE_VALUE_NUMERIC_QString) << db_file_path << recon_file_from_by_user << recon_file_infopath << QString("saved_hashsets");


    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_hashset,values_list, hashes_index_db_path, Q_FUNC_INFO);


    prepare_table_display_for_saved_hashsets();

    display_loading_progress_bar_non_cancelable_obj->hide();

}

void hashset_display_toolbar::prepare_table_display_for_saved_hashsets()
{
    ui->tableWidget_hashset->setRowCount(0);

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(hashes_index_db_path);

    if (!destination_db.open())
    {
        recon_static_functions::app_debug(" db open ------FAILED-------" + destination_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db.lastError().text() ,Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        return ;
    }

    QString command_index_db = QString("Select saved_db_name,show_files,hide_files,md5_hash_column,sha1_hash_column,INT From hashset_indexes");

    QSqlQuery query_index(destination_db);

    query_index.prepare(command_index_db);

    if(!query_index.exec())
    {
        recon_static_functions::app_debug("query execute ------FAILED-------" + query_index.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_index.lastError().text() ,Q_FUNC_INFO);
        destination_db.close();
        QSqlDatabase::removeDatabase(connection_naam);
        return ;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge(global_lang_tr_obj.get_translated_string("Please Wait..."));
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    int row_count = 0;
    while(query_index.next())
    {

        QString saved_db_name    = query_index.value("saved_db_name").toString().trimmed();
        QString show_files       = query_index.value("show_files").toString().trimmed();
        QString hide_files       = query_index.value("hide_files").toString().trimmed();
        QString md5_hash_column  = query_index.value("md5_hash_column").toString().trimmed();
        QString sha1_hash_column = query_index.value("sha1_hash_column").toString().trimmed();
        QString INT_str          = query_index.value("INT").toString().trimmed();

        QString hashset_name = saved_db_name;
        if(hashset_name.endsWith(".sqlite"))
            hashset_name.chop(QString(".sqlite").size());

        hashset_name = hashset_name.trimmed();

        ui->tableWidget_hashset->insertRow(row_count);
        ui->tableWidget_hashset->setRowHeight(row_count, 23);

        QTableWidgetItem *item_saved_name       = new QTableWidgetItem;
        QTableWidgetItem *item_saved_db_name    = new QTableWidgetItem;
        QTableWidgetItem *item_md5_hash_column  = new QTableWidgetItem;
        QTableWidgetItem *item_sha1_hash_column = new QTableWidgetItem;
        QTableWidgetItem *item_INT_column       = new QTableWidgetItem;


        item_saved_name->setText(hashset_name);
        item_saved_db_name->setText(saved_db_name);

        QCheckBox *checkbox_show_files = new QCheckBox(this);
        if(show_files == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
            checkbox_show_files->setCheckState(Qt::Checked);
        else
            checkbox_show_files->setCheckState(Qt::Unchecked);

        ///sets row count as variant value in QObject
        checkbox_show_files->setProperty("row", row_count);

        QCheckBox *checkbox_hide_files = new QCheckBox(this);
        if(hide_files == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
            checkbox_hide_files->setCheckState(Qt::Checked);
        else
            checkbox_hide_files->setCheckState(Qt::Unchecked);

        ///sets row count as variant value in QObject
        checkbox_hide_files->setProperty("row", row_count);

        connect(checkbox_show_files, SIGNAL(toggled(bool)), this, SLOT(slot_checkbox_show_files_clicked(bool)));
        connect(checkbox_hide_files, SIGNAL(toggled(bool)), this, SLOT(slot_checkbox_hide_files_clicked(bool)));


        item_md5_hash_column->setText(md5_hash_column);
        item_sha1_hash_column->setText(sha1_hash_column);
        item_INT_column->setText(INT_str);


        ui->tableWidget_hashset->setItem(row_count, enum_hashset_INT,     item_INT_column);
        ui->tableWidget_hashset->setItem(row_count, enum_hashset_name,    item_saved_name);
        ui->tableWidget_hashset->setItem(row_count, enum_hashset_db_name, item_saved_db_name);

        ui->tableWidget_hashset->setCellWidget(row_count, enum_hashset_show_files,  checkbox_show_files);
        ui->tableWidget_hashset->setCellWidget(row_count, enum_hashset_hide_files,  checkbox_hide_files);

        ui->tableWidget_hashset->setItem(row_count, enum_hashset_md5_column,  item_md5_hash_column);
        ui->tableWidget_hashset->setItem(row_count, enum_hashset_sha1_column, item_sha1_hash_column);

        row_count++;
    }

    destination_db.close();
    display_loading_progress_bar_non_cancelable_obj->hide();

}

void hashset_display_toolbar::slot_checkbox_show_files_clicked(bool status)
{
    int current_row = -1;

    if(bool_checked_from_table)
        current_row = ui->tableWidget_hashset->currentRow();
    else
    {
        QObject *cb = sender();
        if(cb)
            current_row = cb->property("row").toInt();
        else
            current_row = ui->tableWidget_hashset->currentRow();
    }

    if(current_row < 0)
        return;

    QString int_str = ui->tableWidget_hashset->item(current_row, enum_hashset_INT)->text().trimmed();
    if(int_str.isEmpty())
        return;

    QString update_cmd = QString("Update hashset_indexes Set show_files = ? , hide_files = ? Where INT= ?");

    QString status_show_files_str = QString(MACRO_FALSE_VALUE_NUMERIC_QString);
    QString status_hide_files_str = QString(MACRO_TRUE_VALUE_NUMERIC_QString);
    if(status)
    {
        status_show_files_str = QString(MACRO_TRUE_VALUE_NUMERIC_QString);
        status_hide_files_str = QString(MACRO_FALSE_VALUE_NUMERIC_QString);
    }

    QStringList value_list;
    value_list << status_show_files_str << status_hide_files_str << int_str;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd, value_list, hashes_index_db_path, Q_FUNC_INFO);

    prepare_table_display_for_saved_hashsets();

}

void hashset_display_toolbar::slot_checkbox_hide_files_clicked(bool status)
{
    int current_row = -1;

    if(bool_checked_from_table)
        current_row = ui->tableWidget_hashset->currentRow();
    else
    {
        QObject *cb = sender();
        if(cb)
            current_row = cb->property("row").toInt();
        else
            current_row = ui->tableWidget_hashset->currentRow();
    }

    if(current_row < 0)
        return;


    QString int_str = ui->tableWidget_hashset->item(current_row, enum_hashset_INT)->text().trimmed();
    if(int_str.isEmpty())
        return;


    QString update_cmd = QString("Update hashset_indexes Set hide_files = ? , show_files = ? Where INT= ?");

    QString status_hide_files_str = QString(MACRO_FALSE_VALUE_NUMERIC_QString);
    QString status_show_files_str = QString(MACRO_TRUE_VALUE_NUMERIC_QString);
    if(status)
    {
        status_hide_files_str = QString(MACRO_TRUE_VALUE_NUMERIC_QString);
        status_show_files_str = QString(MACRO_FALSE_VALUE_NUMERIC_QString);
    }

    QStringList value_list;
    value_list << status_hide_files_str << status_show_files_str << int_str;


    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd, value_list, hashes_index_db_path, Q_FUNC_INFO);

    prepare_table_display_for_saved_hashsets();

}

void hashset_display_toolbar::slot_pushbutton_existing_sqlite_hash_save_clicked(QString saved_data_str)
{
    //saved_data_str is like this
    //table_name + QString(MACRO_RECON_Splitter) + column_name + QString(MACRO_RECON_Splitter) + sqlite_file_path;

    existing_hash_column_selection_obj->hide();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge(global_lang_tr_obj.get_translated_string("Please Wait..."));
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QStringList value_split_list;
    value_split_list = saved_data_str.split(QString(MACRO_RECON_Splitter_1_multiple));

    if(value_split_list.size() > 2)
    {
        ///save in indexes database
        QString table_name  = value_split_list.at(0);
        QString column_name = value_split_list.at(1);
        QString db_path     = value_split_list.at(2);


        QString dest_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Hashset_In_Result_QString).toString();
        QString db_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(db_path);

        db_name = recon_static_functions::get_available_filename(db_name, dest_dir_path, Q_FUNC_INFO);

        QString file_copy_path = dest_dir_path + db_name;

        QProcess process_copy;
        QStringList values_list;
        values_list << "-rf" << db_path << file_copy_path;

        process_copy.start("cp", values_list);
        if(!process_copy.waitForStarted(3000))
        {
            recon_static_functions::app_debug("Copy process -----FAILED-----", Q_FUNC_INFO);
            return;
        }

        recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
        int count = 0;
        while(1)
        {
            count++;
            if(count == 50)
            {
                count = 0;
                QCoreApplication::processEvents();

            }
            if(process_copy.state() == QProcess::NotRunning)
                break;
        }
        recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);


        ///save in indexes database
        QString recon_file_infopath = file_copy_path;
        QString result_dir_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

        if(recon_file_infopath.startsWith(result_dir_path))
            recon_file_infopath.remove(0, result_dir_path.size() -1);

        QString recon_file_from_result = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);


        QString cmd_hashset  = QString("Insert into hashset_indexes (saved_db_name,show_files,hide_files,md5_hash_column,sha1_hash_column,was_imported_external,"
                                       "saved_db_path,recon_filefrom,recon_file_infopath,saved_table_name) values(?,?,?,?,? ,?,?,?,?,?)");
        values_list.clear();
        values_list << db_name << QString("1") << QString("0") << column_name << QString("")
                    << QString(MACRO_TRUE_VALUE_NUMERIC_QString) << file_copy_path << recon_file_from_result << recon_file_infopath << table_name;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_hashset,values_list, hashes_index_db_path, Q_FUNC_INFO);

        QString index_cmd = QString("CREATE INDEX hashset_index ON ") + table_name + QString("(") + column_name + QString(")");
        recon_helper_standard_obj->execute_db_command_by_dbpath(index_cmd, file_copy_path, Q_FUNC_INFO);

    }

    prepare_table_display_for_saved_hashsets();

    display_loading_progress_bar_non_cancelable_obj->hide();


}

void hashset_display_toolbar::slot_pushbutton_existing_sqlite_hash_cancel_clicked()
{
    existing_hash_column_selection_obj->hide();
}

void hashset_display_toolbar::slot_existing_hash_selected_vic_supported_hashset_file(QString filepath)
{
    lineedit_selected_vic_hashest->setText(filepath);
    lineedit_selected_vic_hashest->setToolTip(filepath);

    QFileInfo info(filepath);
    QString hashset_default_name = info.fileName();
    hashset_default_name.remove(info.suffix());

    if(hashset_default_name.endsWith("."))
        hashset_default_name.chop(1);

    lineedit_vic_hashest_name->setText(hashset_default_name);

    dialog_add_vic_supported_hashset->show();
}

void hashset_display_toolbar::slot_existing_hash_selected_hash_database_file(QString filepath)
{
    existing_hash_column_selection_obj->show();
}

void hashset_display_toolbar::create_dialog_for_import_nsrl_hashset()
{
    dialog_add_nsrl_supported_hashset = new QDialog();
    dialog_add_nsrl_supported_hashset->setWindowModality(Qt::ApplicationModal);

    QFont font_s;
    font_s.setPixelSize(12);

    QVBoxLayout *vboxlayout_main = new QVBoxLayout(dialog_add_nsrl_supported_hashset);

    QLabel *label_selected_hashset = new QLabel("Selected Hashset:");
    lineedit_selected_nsrl_hashest = new QLineEdit(this);
    lineedit_selected_nsrl_hashest->setReadOnly(true);
    lineedit_selected_nsrl_hashest->setAttribute(Qt::WA_MacShowFocusRect,false);

    QVBoxLayout *vbox_selected_hashset = new QVBoxLayout();
    vbox_selected_hashset->addWidget(label_selected_hashset);
    vbox_selected_hashset->addWidget(lineedit_selected_nsrl_hashest);
    vbox_selected_hashset->setSpacing(0);
    vboxlayout_main->addLayout(vbox_selected_hashset);

    QLabel *label_hashset_name = new QLabel("Hashset Name:");
    lineedit_nsrl_hashest_name = new QLineEdit(this);
    lineedit_nsrl_hashest_name->setAttribute(Qt::WA_MacShowFocusRect,false);

    QRegularExpression rx("^[a-zA-Z0-9_ -]*$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    lineedit_nsrl_hashest_name->setValidator(validator);


    pushbutton_create_nsrl_hashset = new QPushButton(QString("Create"),this);
    pushbutton_cancel_nsrl_hashset = new QPushButton(QString("Cancel"),this);

    connect(pushbutton_create_nsrl_hashset,SIGNAL(clicked()),this,SLOT(slot_create_nsrl_hashset_clicked()));
    connect(pushbutton_cancel_nsrl_hashset,SIGNAL(clicked()),this,SLOT(slot_cancel_nsrl_hashset_clicked()));

    pushbutton_create_nsrl_hashset->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_cancel_nsrl_hashset->setAttribute(Qt::WA_MacShowFocusRect,false);

    QHBoxLayout *hbox_layout = new QHBoxLayout;
    QSpacerItem *h_spacer = new QSpacerItem(30,10,QSizePolicy::Expanding,QSizePolicy::Minimum);
    hbox_layout->addSpacerItem(h_spacer);
    hbox_layout->addWidget(pushbutton_cancel_nsrl_hashset);
    hbox_layout->addWidget(pushbutton_create_nsrl_hashset);

    QVBoxLayout *vbox_hashset_name = new QVBoxLayout();
    vbox_hashset_name->addWidget(label_hashset_name);
    vbox_hashset_name->addWidget(lineedit_nsrl_hashest_name);
    vbox_selected_hashset->setSpacing(0);

    vboxlayout_main->addLayout(vbox_hashset_name);
    vboxlayout_main->addLayout(hbox_layout);

    vboxlayout_main->setSpacing(5);
    vboxlayout_main->setContentsMargins(5,0,5,0);

    dialog_add_nsrl_supported_hashset->setLayout(vboxlayout_main);
    dialog_add_nsrl_supported_hashset->setWindowTitle(QString("NSRL - ") + QString(MACRO_RECON_CONFIGURATION_HASHSET));
    dialog_add_nsrl_supported_hashset->setFont(font_s);
    dialog_add_nsrl_supported_hashset->setFixedSize(QSize(600, 130));

    dialog_add_nsrl_supported_hashset->setContentsMargins(0,0,0,0);



}


void hashset_display_toolbar::create_dialog_for_import_csv_hashset()
{
    dialog_add_csv_supported_hashset = new QDialog();
    dialog_add_csv_supported_hashset->setWindowModality(Qt::ApplicationModal);

    QFont font_s;
    font_s.setPixelSize(12);

    QVBoxLayout *vboxlayout_main = new QVBoxLayout(dialog_add_csv_supported_hashset);

    QLabel *label_selected_hashset = new QLabel("Selected Hashset:");
    lineedit_selected_csv_hashest = new QLineEdit(this);
    lineedit_selected_csv_hashest->setReadOnly(true);
    lineedit_selected_csv_hashest->setAttribute(Qt::WA_MacShowFocusRect,false);

    QVBoxLayout *vbox_selected_hashset = new QVBoxLayout();
    vbox_selected_hashset->addWidget(label_selected_hashset);
    vbox_selected_hashset->addWidget(lineedit_selected_csv_hashest);
    vbox_selected_hashset->setSpacing(0);
    vboxlayout_main->addLayout(vbox_selected_hashset);

    QLabel *label_hashset_name = new QLabel("Hashset Name:");
    lineedit_csv_hashest_name = new QLineEdit(this);
    lineedit_csv_hashest_name->setAttribute(Qt::WA_MacShowFocusRect,false);

    QRegularExpression rx("^[a-zA-Z0-9_ -]*$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    lineedit_csv_hashest_name->setValidator(validator);


    pushbutton_create_csv_hashset = new QPushButton(QString("Create"),this);
    pushbutton_cancel_csv_hashset = new QPushButton(QString("Cancel"),this);

    connect(pushbutton_create_csv_hashset,SIGNAL(clicked()),this,SLOT(slot_create_csv_hashset_clicked()));
    connect(pushbutton_cancel_csv_hashset,SIGNAL(clicked()),this,SLOT(slot_cancel_csv_hashset_clicked()));

    pushbutton_create_csv_hashset->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_cancel_csv_hashset->setAttribute(Qt::WA_MacShowFocusRect,false);

    QHBoxLayout *hbox_layout = new QHBoxLayout;
    QSpacerItem *h_spacer = new QSpacerItem(30,10,QSizePolicy::Expanding,QSizePolicy::Minimum);
    hbox_layout->addSpacerItem(h_spacer);
    hbox_layout->addWidget(pushbutton_cancel_csv_hashset);
    hbox_layout->addWidget(pushbutton_create_csv_hashset);

    QVBoxLayout *vbox_hashset_name = new QVBoxLayout();
    vbox_hashset_name->addWidget(label_hashset_name);
    vbox_hashset_name->addWidget(lineedit_csv_hashest_name);
    vbox_selected_hashset->setSpacing(0);

    vboxlayout_main->addLayout(vbox_hashset_name);
    vboxlayout_main->addLayout(hbox_layout);

    vboxlayout_main->setSpacing(5);
    vboxlayout_main->setContentsMargins(5,0,5,0);

    dialog_add_csv_supported_hashset->setLayout(vboxlayout_main);
    dialog_add_csv_supported_hashset->setWindowTitle(QString("CSV - ") + QString(MACRO_RECON_CONFIGURATION_HASHSET));
    dialog_add_csv_supported_hashset->setFont(font_s);
    dialog_add_csv_supported_hashset->setFixedSize(QSize(600, 130));

    dialog_add_csv_supported_hashset->setContentsMargins(0,0,0,0);



}

void hashset_display_toolbar::slot_action_add_NSRL_clicked(bool status)
{
    QString nsrl_text_file_path;
    file_dialog_obj.setNameFilter(("*txt"));

    file_dialog_obj.setFileMode(QFileDialog::AnyFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            nsrl_text_file_path = selected_files.at(0);
        }
    }
    nsrl_text_file_path = nsrl_text_file_path.trimmed();

    if(nsrl_text_file_path.isEmpty())
        return;

    QFileInfo info(nsrl_text_file_path);

    QString filename_no_suffix = info.fileName();
    filename_no_suffix.remove(info.suffix());

    if(filename_no_suffix.endsWith("."))
        filename_no_suffix.chop(1);

    lineedit_selected_nsrl_hashest->setText(nsrl_text_file_path);
    lineedit_nsrl_hashest_name->setText(filename_no_suffix);

    dialog_add_nsrl_supported_hashset->show();
}

void hashset_display_toolbar::slot_create_nsrl_hashset_clicked()
{
    dialog_add_nsrl_supported_hashset->hide();

    QString nsrl_text_file_path = lineedit_selected_nsrl_hashest->text().trimmed();

    if(nsrl_text_file_path.isEmpty() || !QFileInfo(nsrl_text_file_path).exists())
        return;

    QString nsrl_dest_db_path = create_nsrl_destination_db();
    if(nsrl_dest_db_path.isEmpty())
        return;

    thread_parse_nsrl_hashes_obj = new thread_parse_nsrl_hashes(this);

    connect(thread_parse_nsrl_hashes_obj, SIGNAL(finished()),this, SLOT(slot_thread_parse_nsrl_finished()));
    connect(thread_parse_nsrl_hashes_obj, SIGNAL(signal_nsrl_parse_status(qint64, int)),this, SLOT(slot_nsrl_parse_status(qint64, int)));

    nsrl_parse_prog_obj = new display_loading_progress_bar(this);
    connect(nsrl_parse_prog_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));

    thread_parse_nsrl_hashes_obj->pub_set_essentials(nsrl_text_file_path,nsrl_dest_db_path);

    nsrl_parse_prog_obj->pub_set_window_modal(Qt::WindowModal);
    nsrl_parse_prog_obj->pub_set_label_messsge(global_lang_tr_obj.get_translated_string("Please Wait..."));
    nsrl_parse_prog_obj->show();

    thread_parse_nsrl_hashes_obj->start();

}

void hashset_display_toolbar::slot_thread_parse_nsrl_finished()
{
    if(thread_parse_nsrl_hashes_obj->isRunning())
        thread_parse_nsrl_hashes_obj->quit();

    if(thread_parse_nsrl_hashes_obj)
        delete thread_parse_nsrl_hashes_obj;

    nsrl_parse_prog_obj->hide();

    prepare_table_display_for_saved_hashsets();
}

void hashset_display_toolbar::slot_hide_loading_display_dialog_box(bool status)
{
    thread_parse_nsrl_hashes_obj->pub_cancel_extraction();
}

void hashset_display_toolbar::slot_nsrl_parse_status(qint64 records, int prog_status)
{
    nsrl_parse_prog_obj->pub_set_progress_bar_definite_value(prog_status);
}


QString hashset_display_toolbar::create_nsrl_destination_db()
{
    QString filename_str = lineedit_nsrl_hashest_name->text().trimmed();

    if(filename_str.isEmpty())
        return filename_str.trimmed();

    filename_str.replace(" ", "_");
    filename_str.append(".sqlite");

    filename_str = recon_static_functions::get_available_filename(filename_str, hashset_dir_path, Q_FUNC_INFO);

    QString db_file_path = hashset_dir_path + filename_str;
    QString command  = QString("CREATE TABLE saved_hashsets (bookmark INTEGER ,INT INTEGER PRIMARY KEY,md5_hash varchar(100),sha1_hash varchar(100))");
    bool bool_cmd_status = recon_helper_standard_obj->execute_db_command_by_dbpath(command,db_file_path, Q_FUNC_INFO);
    if(!bool_cmd_status)
        return QString("").trimmed();

    //    bool_cmd_status = false;
    //    command  = QString("CREATE TABLE software (name varchar(100))");
    //    bool_cmd_status = recon_helper_standard_obj->execute_db_command_by_dbpath(command,db_file_path, Q_FUNC_INFO);
    //    if(!bool_cmd_status)
    //        return QString("").trimmed();

    //    bool_cmd_status = false;
    //    command  = QString("Insert into software (name) values(?)");
    //    QStringList value_list;
    //    value_list << "RECON";
    //    bool_cmd_status = recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, db_file_path, Q_FUNC_INFO);
    //    if(!bool_cmd_status)
    //        return QString("").trimmed();

    ///save in indexes database
    QString recon_file_infopath = db_file_path;
    QString result_dir_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    if(recon_file_infopath.startsWith(result_dir_path))
        recon_file_infopath.remove(0, result_dir_path.size() - 1);

    QString recon_file_from_result = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);

    QString cmd_hashset  = QString("Insert into hashset_indexes (saved_db_name,show_files,hide_files,md5_hash_column,sha1_hash_column,was_imported_external,"
                                   "saved_db_path,recon_filefrom,recon_file_infopath,saved_table_name) values(?,?,?,?,? ,?,?,?,?,?)");
    QStringList values_list;
    values_list << filename_str << QString("1") << QString("0") << QString("md5_hash") << QString("sha1_hash")
                << QString(MACRO_FALSE_VALUE_NUMERIC_QString) << db_file_path << recon_file_from_result << recon_file_infopath << QString("saved_hashsets");

    bool_cmd_status = false;
    bool_cmd_status = recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_hashset,values_list, hashes_index_db_path, Q_FUNC_INFO);

    if(!bool_cmd_status)
        return QString("").trimmed();

    return db_file_path;
}

void hashset_display_toolbar::slot_cancel_nsrl_hashset_clicked()
{
    dialog_add_nsrl_supported_hashset->hide();
}

void hashset_display_toolbar::on_tableWidget_hashset_cellClicked(int row, int column)
{
    if(row < 0 || column < 0)
        return;

    if(column == enum_hashset_show_files)
    {
        QCheckBox *checkbox = (QCheckBox*)ui->tableWidget_hashset->cellWidget(row, column);

        bool bool_check_state = false;
        if(checkbox->checkState() == Qt::Checked)
            bool_check_state = true;

        bool_checked_from_table = true;
        slot_checkbox_show_files_clicked(!bool_check_state);
        bool_checked_from_table = false;

    }
    else if(column == enum_hashset_hide_files)
    {
        QCheckBox *checkbox = (QCheckBox*)ui->tableWidget_hashset->cellWidget(row, column);

        bool bool_check_state = false;
        if(checkbox->checkState() == Qt::Checked)
            bool_check_state = true;

        bool_checked_from_table = true;
        slot_checkbox_hide_files_clicked(!bool_check_state);
        bool_checked_from_table = false;

    }
}

void hashset_display_toolbar::slot_action_add_CSV_clicked(bool status)
{
    QString csv_file_path;
    file_dialog_obj.setNameFilter(("*csv"));

    file_dialog_obj.setFileMode(QFileDialog::AnyFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            csv_file_path = selected_files.at(0);
        }
    }
    csv_file_path = csv_file_path.trimmed();

    if(csv_file_path.isEmpty())
        return;

    QFileInfo info(csv_file_path);

    QString filename_no_suffix = info.fileName();
    filename_no_suffix.remove(info.suffix());

    if(filename_no_suffix.endsWith("."))
        filename_no_suffix.chop(1);

    lineedit_selected_csv_hashest->setText(csv_file_path);
    lineedit_csv_hashest_name->setText(filename_no_suffix);

    dialog_add_csv_supported_hashset->show();

}

void hashset_display_toolbar::slot_create_csv_hashset_clicked()
{
    dialog_add_csv_supported_hashset->hide();

    QString csv_file_path = lineedit_selected_csv_hashest->text().trimmed();

    if(csv_file_path.isEmpty() || !QFileInfo(csv_file_path).exists())
        return;

    QString csv_dest_db_path = create_csv_destination_db();
    if(csv_dest_db_path.isEmpty())
        return;

    QFile file_csv(csv_file_path);

    if(!file_csv.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open -----FAILED-----" + file_csv.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error : " + file_csv.errorString(), Q_FUNC_INFO);
        return;
    }

    QSqlDatabase hashes_database =QSqlDatabase::addDatabase("QSQLITE");
    hashes_database.setDatabaseName(csv_dest_db_path);
    if(!hashes_database.open())
    {
        recon_static_functions::app_debug("DB open ----FAILED----" + hashes_database.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + hashes_database.lastError().text(), Q_FUNC_INFO);
        return;
    }

    hashes_database.transaction();

    QSqlQuery insert_into_hashes(hashes_database) ;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge(global_lang_tr_obj.get_translated_string("Please Wait..."));
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QByteArray line_arr;
    QString line_str;
    QStringList line_split_list;
    QString md5_hash_str;

    while(!file_csv.atEnd())
    {
        line_arr = file_csv.readLine();
        line_str = QString::fromLocal8Bit(line_arr, line_arr.size()).trimmed();

        line_split_list = line_str.split(",");

        for(int count = 0; count < line_split_list.size(); count++)
        {
            md5_hash_str = line_split_list.at(count);

            if(md5_hash_str.size() == 32)
            {
                insert_into_hashes.prepare("insert into saved_hashsets(md5_hash) values (?)");
                insert_into_hashes.addBindValue(md5_hash_str);

                if(!insert_into_hashes.exec())
                {
                    recon_static_functions::app_debug("query insert ----FAILED----" + insert_into_hashes.executedQuery(), Q_FUNC_INFO);
                    recon_static_functions::app_debug("Error:" + insert_into_hashes.lastError().text(), Q_FUNC_INFO);
                    continue;
                }
            }
        }
    }
    file_csv.close();

    hashes_database.commit();

    QString index_cmd = QString("CREATE INDEX hashset_index ON saved_hashsets(md5_hash)");
    recon_helper_standard_obj->execute_db_command_by_dbreference(index_cmd, hashes_database, Q_FUNC_INFO);

    hashes_database.close();

    display_loading_progress_bar_non_cancelable_obj->hide();

    prepare_table_display_for_saved_hashsets();

}

void hashset_display_toolbar::slot_cancel_csv_hashset_clicked()
{
    dialog_add_csv_supported_hashset->hide();
}

QString hashset_display_toolbar::create_csv_destination_db()
{
    QString filename_str = lineedit_csv_hashest_name->text().trimmed();

    if(filename_str.isEmpty())
        return filename_str.trimmed();

    filename_str.replace(" ", "_");
    filename_str.append(".sqlite");

    filename_str = recon_static_functions::get_available_filename(filename_str, hashset_dir_path, Q_FUNC_INFO);

    QString db_file_path = hashset_dir_path + filename_str;
    QString command  = QString("CREATE TABLE saved_hashsets (bookmark INTEGER ,INT INTEGER PRIMARY KEY,md5_hash varchar(100),sha1_hash varchar(100))");
    bool bool_cmd_status = recon_helper_standard_obj->execute_db_command_by_dbpath(command,db_file_path, Q_FUNC_INFO);
    if(!bool_cmd_status)
        return QString("").trimmed();

    //    bool_cmd_status = false;
    //    command  = QString("CREATE TABLE software (name varchar(100))");
    //    bool_cmd_status = recon_helper_standard_obj->execute_db_command_by_dbpath(command,db_file_path, Q_FUNC_INFO);
    //    if(!bool_cmd_status)
    //        return QString("").trimmed();

    //    bool_cmd_status = false;
    //    command  = QString("Insert into software (name) values(?)");
    //    QStringList value_list;
    //    value_list << "RECON";
    //    bool_cmd_status = recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, db_file_path, Q_FUNC_INFO);
    //    if(!bool_cmd_status)
    //        return QString("").trimmed();

    ///save in indexes database
    QString recon_file_infopath = db_file_path;
    QString result_dir_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    if(recon_file_infopath.startsWith(result_dir_path))
        recon_file_infopath.remove(0, result_dir_path.size() - 1);


    QString recon_file_from_result = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);

    QString cmd_hashset  = QString("Insert into hashset_indexes (saved_db_name,show_files,hide_files,md5_hash_column,sha1_hash_column,was_imported_external,"
                                   "saved_db_path,recon_filefrom,recon_file_infopath,saved_table_name) values(?,?,?,?,? ,?,?,?,?,?)");
    QStringList values_list;
    values_list << filename_str << QString("1") << QString("0") << QString("md5_hash") << QString("sha1_hash")
                << QString(MACRO_FALSE_VALUE_NUMERIC_QString) << db_file_path << recon_file_from_result << recon_file_infopath << QString("saved_hashsets");

    bool_cmd_status = false;
    bool_cmd_status = recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_hashset,values_list, hashes_index_db_path, Q_FUNC_INFO);

    if(!bool_cmd_status)
        return QString("").trimmed();

    return db_file_path;
}

void hashset_display_toolbar::parse_unknown_vic_format_file(QString filepath)
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

QString hashset_display_toolbar::get_sha1_hash_for_unknown_vic_format(QByteArray &file_data_arr, int md5_end_index)
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
