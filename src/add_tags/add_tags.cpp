#include "add_tags.h"
#include "ui_add_tags.h"

add_tags::add_tags(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_tags)
{
    ui->setupUi(this);
    ui->lineEdit->setAttribute(Qt::WA_MacShowFocusRect,0);

    qRegisterMetaType<QVector<int>>("QVector<int>");

    recon_helper_standard_obj = new recon_helper_standard(this);


    QRegularExpression rx("^[a-zA-Z0-9_]*$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);

    ui->lineEdit->setValidator(validator);

    setWindowTitle(QObject::tr("Create Tags"));

    message_dialog_object = new message_dialog(this);

}

add_tags::~add_tags()
{
    delete ui;
}

void add_tags::pub_set_essentials()
{
    tags_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    fill_colours_to_select();
}

void add_tags::on_pushButton_save_tag_clicked()
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    QString new_tag = ui->lineEdit->text().trimmed();

    if(new_tag == QString(MACRO_Tag_Name_Bookmarks))
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Tag already exists");
        message_dialog_object->show();
        ui->lineEdit->clear();
        ui->comboBox->setCurrentIndex(0);
        return;
    }

    if(new_tag == QString(MACRO_Tag_Name_Saved_graphs)
            || new_tag == QString(MACRO_Tag_Name_Saved_Maps)
            || new_tag == QString(MACRO_Tag_Name_Screenshots)
            || new_tag == QString(MACRO_Tag_Name_Notes)
            || new_tag == QString(MACRO_Tag_Name_Untagged_Items)
            || new_tag == QString(MACRO_Tag_Name_Examiner_Comments))
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Tag reserved for " + recon_static_functions::get_app_name() + ", Please choose another name");
        message_dialog_object->show();
        ui->lineEdit->clear();
        ui->comboBox->setCurrentIndex(0);
        return;
    }

    if(new_tag == QString(""))
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Please Insert/Select a Tag.");
        message_dialog_object->show();
        ui->lineEdit->clear();
        ui->comboBox->setCurrentIndex(0);
        return;
    }

    ///add new tags
    QString colour = ui->comboBox->currentText();
    if(colour == QString("<Select Color>"))
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Please Select a Tag Color");
        message_dialog_object->show();
        return;
    }

    ///check if tag already exists in tags db

    QString command_str = "SELECT tag_name FROM tags WHERE tag_name = ? COLLATE NOCASE";
    QStringList list1;
    list1 << new_tag;
    QString status_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,list1,0 ,tags_db_path,Q_FUNC_INFO);

    bool value_not_exist = false;
    if(status_str == QString(""))
        value_not_exist = true;

    if(value_not_exist)
        emit tag_status(value_not_exist,new_tag,colour);
    else
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Tag already exists");
        message_dialog_object->show();
        return;
    }


    add_new_tag_to_tags_database(new_tag , colour);

    emit signal_tags(new_tag,colour);

    ui->lineEdit->clear();
    ui->comboBox->setCurrentIndex(0);
    this->hide();

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);

}

QString add_tags::get_color_hex_from_colour_name(QString colour ,  QString destination_db_path , QString caller_func)
{
    QString command = "select color_hex from 'colors' where color_name = '" + colour + "'";
    QString color_hex = recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,destination_db_path,caller_func);

    return color_hex;
}

void add_tags::add_new_tag_to_tags_database(QString new_tag , QString colour)
{

    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    if(new_tag == QString(""))
        return;


    QString colour_hex = get_color_hex_from_colour_name(colour , tags_db_path, Q_FUNC_INFO);


    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tags_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open ---FAILED--- " , Q_FUNC_INFO);
        return;
    }



    QSqlQuery query_insert_tag(destination_db);

    query_insert_tag.prepare("INSERT into tags (tag_name , selected_colour ,color_hex ,is_default_tag) values(?,?,?,?)");

    query_insert_tag.addBindValue(new_tag);
    query_insert_tag.addBindValue(colour);
    query_insert_tag.addBindValue(colour_hex);
    query_insert_tag.addBindValue("0");

    if(!query_insert_tag.exec())
    {
        recon_static_functions::app_debug(" -insert query ---FAILED--- " , Q_FUNC_INFO);

        destination_db.close();
        return;
    }

    destination_db.close();

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);

}

///fill colours in the combobox
void add_tags::fill_colours_to_select()
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tags_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open ---FAILED--- " + destination_db.databaseName(), Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_colors(destination_db);
    query_colors.prepare("select color_name,color_hex from colors");
    if(!query_colors.exec())
    {
        recon_static_functions::app_debug(" -insert query ---FAILED--- " , Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    ui->comboBox->clear();
    ui->comboBox->addItem("<Select Color>");
    int count = 1;
    while(query_colors.next())
    {
        ui->comboBox->addItem(query_colors.value(0).toString());
        ui->comboBox->setItemData(count,QColor(query_colors.value(1).toString()), Qt::SolidPattern);

        count++;
    }

    destination_db.close();
    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);

}


QStringList add_tags::get_tags_and_colours_list_from_db(QString command,QString destination_db_file)
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    QStringList list_gen;

    QFileInfo info(destination_db_file);
    if(!info.exists())
    {
        recon_static_functions::app_debug("  Db NOT Exists ---FAILED--- " + destination_db_file ,Q_FUNC_INFO);
        return list_gen;
    }

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return list_gen;
    }

    QSqlQuery query(destination_db);
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return list_gen;
    }

    while(query.next())
    {
        QString value_str = query.value(0).toString().trimmed();

        value_str.append("::");

        value_str.append(query.value(1).toString().trimmed());

        list_gen << value_str;
    }

    destination_db.close();
    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);

    return list_gen;
}
