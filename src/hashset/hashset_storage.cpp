#include "hashset_storage.h"
#include "ui_hashset_storage.h"

hashset_storage::hashset_storage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hashset_storage)
{
    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    setWindowTitle("Create Hashset");

    vbox_add_hash_lineedit_layout = new QVBoxLayout(this);
    QFont font;
    font.setPixelSize(12);

    //---START
    QHBoxLayout *hbox_name = new QHBoxLayout;
    label_name = new QLabel("Name",this);
    label_name->setFixedWidth(50);
    lineEdit_name = new QLineEdit(this);
    connect(lineEdit_name,SIGNAL(textChanged(QString)),this,SLOT(slot_lineedit_name_textChanged(QString)));
    lineEdit_name->setAttribute(Qt::WA_MacShowFocusRect,false);

    hbox_name->addWidget(label_name);
    hbox_name->addWidget(lineEdit_name);
    hbox_name->setContentsMargins(0,0,3,0);
    hbox_name->setSpacing(7);
    //--End----

    //---Start
    QHBoxLayout *hbox_path = new QHBoxLayout;
    label_path = new QLabel("Path",this);
    label_path->setFixedWidth(50);

    lineEdit_path = new QLineEdit(this);
    lineEdit_path->setReadOnly(true);

    pushButton_open = new QPushButton(this);
    pushButton_open->setText("...");

    pushButton_open->setFocusPolicy(Qt::NoFocus);
    pushButton_open->setFont(font);
    pushButton_open->setFixedWidth(50);
    connect(pushButton_open,SIGNAL(clicked()),this,SLOT(slot_pushButton_open_clicked()));

    hbox_path->addWidget(label_path);
    hbox_path->addWidget(lineEdit_path);
    hbox_path->addWidget(pushButton_open);
    hbox_path->setContentsMargins(2,0,2,0);
    hbox_path->setSpacing(5);
    //-End.

    QHBoxLayout *hbox_create_path = new QHBoxLayout;
    label_existence = new QLabel(this);
    QSpacerItem *spacer = new QSpacerItem(15,10,QSizePolicy::Fixed,QSizePolicy::Minimum);
    pushButton_create_hashset = new QPushButton("Create",this);

    pushButton_create_hashset->setFocusPolicy(Qt::NoFocus);
    pushButton_create_hashset->setFont(font);
    pushButton_create_hashset->setFixedWidth(80);
    connect(pushButton_create_hashset,SIGNAL(clicked()),this,SLOT(slot_pushButton_ok_clicked()));

    QPushButton *cancel_button = new QPushButton("Cancel",this);
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(slot_cancel_button_clicked()));
    cancel_button->setFixedWidth(80);

    ///hashset
    hbox_create_path->addWidget(label_existence);
    hbox_create_path->addSpacerItem(spacer);
    hbox_create_path->addWidget(cancel_button);
    hbox_create_path->addWidget(pushButton_create_hashset);
    hbox_create_path->setContentsMargins(0,0,0,0);

    vbox_add_hash_lineedit_layout->addLayout(hbox_name);
    vbox_add_hash_lineedit_layout->addLayout(hbox_path);
    vbox_add_hash_lineedit_layout->addLayout(hbox_create_path);
    vbox_add_hash_lineedit_layout->setContentsMargins(12,12,12,12);
    vbox_add_hash_lineedit_layout->setSpacing(4);
    setLayout(vbox_add_hash_lineedit_layout);


    message_dialog_object = new message_dialog(this);
}

hashset_storage::~hashset_storage()
{
    delete ui;
}


void hashset_storage::pub_set_essentials()
{
    list_dbs_to_add.clear();
    str_dateformat = global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString).toString();

    QString text = QString("Hashset_")  +  QDateTime::currentDateTime().toString(str_dateformat);
    lineEdit_name->setText(text);

    if(page_caller == MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG)
    {
        hashset_default_db_initial_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Parent_QString).toString();

        hashset_master_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + "recon_configuration.sqlite";
        hashset_default_dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + "hashset";
    }

    if(page_caller == MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    {
        hashset_default_db_initial_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Parent_QString).toString();

        hashset_master_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + "case_configuration/case_configuration.sqlite";
        hashset_default_dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + "case_configuration/hashset";
    }


    lineEdit_path->setText(hashset_default_dbpath);
}

void hashset_storage::slot_pushButton_open_clicked()
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    QString get_db_path = QFileDialog::getExistingDirectory(this, "Open", getenv("HOME"));
    if(!get_db_path.trimmed().isEmpty())
        lineEdit_path->setText(get_db_path);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void hashset_storage::insert_existing_hashset_in_master_database(QStringList list_dbs)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(hashset_master_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + hashset_master_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    for(int i = 0; i < list_dbs.size() ; i++)
    {
        QStringList list = list_dbs.at(i).split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);
        if(list.size() != 2)
            continue;

        QString db_name = list.at(0);
        QString db_path = list.at(1);

        QString command_check = "select path from hashset_databases";
        if(recon_helper_standard_obj->check_value_existence_in_db_by_dbreference(command_check,0,db_path,destination_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("value already exist in db continue from here" , Q_FUNC_INFO);
            continue;
        }

        QString masterdb_command = "INSERT INTO hashset_databases(bookmarks,is_default,name,path) VALUES(?,?,?,?)";
        QStringList listofvalues;
        listofvalues << QString("1") << QString("0") <<db_name << db_path;

        if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(masterdb_command,listofvalues, destination_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("-----FAILED----- to insert destination db information in hashset db" , Q_FUNC_INFO);
            continue;
        }
    }

    destination_db.close();
    recon_static_functions::app_debug("Ends" , Q_FUNC_INFO);
}

void hashset_storage::insert_file_details_into_hashset_database(QString file_path, QString clicked_db_path, QString source_count_name)
{
    recon_static_functions::app_debug("Starts" , Q_FUNC_INFO);

    QFile file(clicked_db_path);
    if(!file.exists())
    {
        message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,"Selected database does not exist.");
        message_dialog_object->show();
        return;
    }



    struct_GLOBAL_witness_info_source struct_info;
    struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    file.setFileName(file_path);
    if(!file.exists())
    {
        message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,"Selected file does not exist.");
        message_dialog_object->show();
        return;
    }

    QString md5_hash = recon_static_functions::create_md5(file_path, Q_FUNC_INFO);
    md5_hash = md5_hash.trimmed();

    if(md5_hash.isEmpty())
        return;


    QString command  = QString("Insert into saved_hashsets (bookmark,md5_hash) values(?,?)");
    QStringList value_list;
    value_list << QString::number(1) << md5_hash;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, clicked_db_path, Q_FUNC_INFO);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return;

}

void hashset_storage::pub_set_page_caller(QString caller_name)
{
    page_caller = caller_name;
}

void hashset_storage::slot_lineedit_name_textChanged(QString arg)
{
    if(arg != "")
    {
        lineEdit_name->setStyleSheet(styleSheet());
        label_existence->setStyleSheet(styleSheet());
        label_existence->clear();
        return;
    }
}

void hashset_storage::slot_pushButton_ok_clicked()
{
    QString db_name_by_user = lineEdit_name->text().trimmed();
    QString db_path_by_user = lineEdit_path->text().trimmed();

    if(db_name_by_user.isEmpty())
    {
        lineEdit_name->setStyleSheet("QLineEdit{color : red; border: 1px solid red;}");
        label_existence->setText("Insert hashset name.");
        return;
    }

    if(db_path_by_user.isEmpty())
    {
        lineEdit_path->setStyleSheet("QLineEdit{color : red; border: 1px solid red;}");
        label_existence->setText("Insert destination path .");
        return;
    }

    db_path_by_user = db_path_by_user + "/" + db_name_by_user + ".sqlite";
    QFileInfo info_file(db_path_by_user);
    if(info_file.exists())
    {
        lineEdit_name->setStyleSheet("QLineEdit{color : red; border: 1px solid red;}");
        label_existence->setText("Hashset already exist.");

        return;
    }

    QString db_name = lineEdit_name->text() + ".sqlite";
    QString db_path = lineEdit_path->text() + "/" + db_name;

    list_dbs_to_add.clear();
    list_dbs_to_add << lineEdit_name->text() + MACRO_RECON_Splitter_1_multiple + db_path;


    if(page_caller ==  MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG)
    {
        emit signal_hashset_storage_recon_conf_workdone_clicked(db_path);
        this->hide();
    }
    else if( page_caller ==  MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    {
        emit signal_hashset_storage_recon_conf_workdone_clicked(db_path);
        this->hide();
    }
    else
    {
        emit signal_hashset_storage_workdone_clicked(list_dbs_to_add, MACRO_Hashset_New_Hash_Created);
        list_dbs_to_add.clear();
        lineEdit_name->setText(QString("Hashset_")  +  QDateTime::currentDateTime().toString(str_dateformat));
    }
}

void hashset_storage::pub_display_create_hash()
{
    setWindowTitle("Create Hashset");
    lineEdit_name->setText(QString("Hashset_")  +  QDateTime::currentDateTime().toString(str_dateformat));
    label_existence->clear();
}

void hashset_storage::pub_display_add_hash(QString get_db_path)
{
    if(get_db_path.isEmpty())
        return;

    QFile file(get_db_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File open ------FAILED------" + get_db_path, Q_FUNC_INFO);
        recon_static_functions::app_debug("----error ------" + file.errorString(), Q_FUNC_INFO);
        return;
    }

    QByteArray dataread = file.readAll();
    QByteArray header_str;
    header_str.append(0x53);
    header_str.append(0x51);
    header_str.append(0x4C);
    header_str.append(0x69);
    header_str.append(0x74);
    header_str.append(0x65);
    header_str.append(0x20);
    header_str.append(0x66);
    header_str.append(0x6F);
    header_str.append(0x72);
    header_str.append(0x6D);
    header_str.append(0x61);
    header_str.append(0x74);


    if(dataread.indexOf(header_str) == -1)
    {
        message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,"This is not sqlite file");
        message_dialog_object->show();
        file.close();
        return;
    }

    if(page_caller ==  MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG || page_caller ==  MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    {
        QFileInfo file(get_db_path);

        //        QString copy_dest_path = hashset_default_dbpath + "/" + file.fileName();
        //        if(!QFile::copy(get_db_path,copy_dest_path))
        //            recon_static_functions::app_debug("hashset db copy  --FAILED-- " + copy_dest_path,Q_FUNC_INFO);

        //        if(copy_dest_path.startsWith(hashset_default_db_initial_path))
        //            copy_dest_path.remove(0,hashset_default_db_initial_path.size());

        //        if(!copy_dest_path.startsWith("/"))
        //            copy_dest_path.prepend("/");

        list_dbs_to_add << file.fileName().remove(".sqlite") + MACRO_RECON_Splitter_1_multiple + get_db_path;
        insert_existing_hashset_in_master_database(list_dbs_to_add);
        emit signal_hashset_storage_recon_conf_workdone_clicked(get_db_path);
    }

    file.close();
}

void hashset_storage::slot_cancel_button_clicked()
{
    this->hide();
}
