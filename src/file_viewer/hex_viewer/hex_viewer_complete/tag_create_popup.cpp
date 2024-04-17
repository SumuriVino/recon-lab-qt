#include "tag_create_popup.h"
#include "tag_create_popup.h"
#include "ui_tag_create_popup.h"

tag_create_popup::tag_create_popup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tag_create_popup)
{
    ui->setupUi(this);

    ui->lineEdit_start_address->setReadOnly(true);
    ui->lineEdit_end_address->setReadOnly(true);
    setWindowModality(Qt::ApplicationModal);

    ui->comboBox_new_tag->setEnabled(false);
    ui->lineEdit_new_tag->setEnabled(false);

    QRegularExpression rx("^[a-zA-Z0-9_]*$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);

    ui->lineEdit_new_tag->setValidator(validator);


}

tag_create_popup::~tag_create_popup()
{
    delete ui;
}

void tag_create_popup::pub_set_start_address(qint64 start)
{
    ui->lineEdit_start_address->setText(QString::number(start));
}

qint64 tag_create_popup::pub_get_start_address()
{
    QString address_str = ui->lineEdit_start_address->text().trimmed();
    return address_str.toLongLong();
}

void tag_create_popup::pub_set_end_address(qint64 end)
{
    ui->lineEdit_end_address->setText(QString::number(end));
}

qint64 tag_create_popup::pub_get_end_address()
{
    QString address_str = ui->lineEdit_end_address->text().trimmed();
    return address_str.toLongLong();
}


void tag_create_popup::pub_set_tags_data()
{
    add_existing_tags_to_combobox();
    prepare_colors_list();
}

void tag_create_popup::on_pushButton_save_clicked()
{
    if(ui->checkBox_create_new->checkState() == Qt::Checked)
    {
        if(!save_new_tag_details())
            return;
        else
        {
            QMessageBox::information(this, recon_static_functions::get_app_name(), "New tag save successfully.");
            return;
        }
    }

    if(ui->comboBox_saved_tags->currentText().trimmed() == QString("<Select Tag>"))
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "No Tag Selected.");
        return;
    }

    QString tag_name_str = ui->comboBox_saved_tags->currentText().trimmed();
    highlight_color_hex = get_color_hex_from_saved_colors(tag_name_str);
    highlight_color_hex = highlight_color_hex.trimmed();
    if(highlight_color_hex.isEmpty())
        return;

    emit signal_save_tag_details(pub_get_start_address(), pub_get_end_address(), tag_name_str, highlight_color_hex);
    hide();
}

void tag_create_popup::add_existing_tags_to_combobox()
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    QString tags_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tags_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open ---FAILED--- " + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error: " + destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_colors(destination_db);
    query_colors.prepare("select tag_name,color_hex from tags");
    if(!query_colors.exec())
    {
        recon_static_functions::app_debug(" -select query ---FAILED--- "+ query_colors.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error: " + query_colors.lastError().text(), Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    tag_color_hex_map.clear();
    ui->comboBox_saved_tags->clear();
    ui->comboBox_saved_tags->addItem("<Select Tag>");
    int count = 1;
    while(query_colors.next())
    {
        QString tag_name   = query_colors.value("tag_name").toString().trimmed();
        QString color_hex  = query_colors.value("color_hex").toString().trimmed();

        ui->comboBox_saved_tags->addItem(tag_name);
        ui->comboBox_saved_tags->setItemData(count,QColor(color_hex), Qt::SolidPattern);

        count++;
        tag_color_hex_map.insert(tag_name, color_hex);
    }
    destination_db.close();
    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}

void tag_create_popup::prepare_colors_list()
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    QString tags_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tags_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open ---FAILED--- " + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error: " + destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_colors(destination_db);
    query_colors.prepare("select color_name,color_hex from colors");
    if(!query_colors.exec())
    {
        recon_static_functions::app_debug(" -select query ---FAILED--- " + query_colors.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error: " + query_colors.lastError().text(), Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    tag_all_colors_map.clear();
    ui->comboBox_new_tag->clear();
    ui->comboBox_new_tag->addItem("<Select Color>");
    int count = 1;
    while(query_colors.next())
    {
        QString color_name = query_colors.value("color_name").toString().trimmed();
        QString color_hex  = query_colors.value("color_hex").toString().trimmed();

        ui->comboBox_new_tag->addItem(color_name);
        ui->comboBox_new_tag->setItemData(count,QColor(color_hex), Qt::SolidPattern);

        count++;
        tag_all_colors_map.insert(color_name, color_hex);
    }
    destination_db.close();
    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}

QString tag_create_popup::get_color_hex_from_saved_colors(QString color_name_str)
{
    QString color_hex;
    QMapIterator<QString, QString> iter(tag_color_hex_map);
    while (iter.hasNext()) {
        iter.next();
        if(iter.key() == color_name_str)
        {
            color_hex = iter.value();
            break;
        }
    }
    return color_hex;
}

QString tag_create_popup::get_color_hex_from_all_available_colors(QString color_name_str)
{
    QString color_hex;
    QMapIterator<QString, QString> iter(tag_all_colors_map);
    while (iter.hasNext()) {
        iter.next();
        if(iter.key() == color_name_str)
        {
            color_hex = iter.value();
            break;
        }
    }
    return color_hex;
}

void tag_create_popup::on_checkBox_create_new_clicked(bool checked)
{
    ui->comboBox_saved_tags->setCurrentIndex(0);

    if(checked)
    {
        ui->comboBox_new_tag->setEnabled(true);
        ui->lineEdit_new_tag->setEnabled(true);
        ui->comboBox_saved_tags->setEnabled(false);
    }
    else
    {
        ui->comboBox_new_tag->setEnabled(false);
        ui->lineEdit_new_tag->setEnabled(false);
        ui->comboBox_saved_tags->setEnabled(true);
    }

}

bool tag_create_popup::save_new_tag_details()
{
    QString tag_name_str = ui->lineEdit_new_tag->text().trimmed();


    if(tag_name_str.isEmpty())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Tag 'Name' Empty.");
        return false;
    }
    QString tag_color_name_str = ui->comboBox_new_tag->currentText().trimmed();
    if(tag_color_name_str.isEmpty() || tag_color_name_str == QString("<Select Color>"))
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Tag 'Color' not selected.");
        return false;
    }

    QString tag_color_hex_str = get_color_hex_from_all_available_colors(tag_color_name_str);

    QString tags_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(tags_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open ---FAILED--- " + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + destination_db.lastError().text(), Q_FUNC_INFO);
        return false;
    }

    QSqlQuery query_colors(destination_db);
    query_colors.prepare("select tag_name from tags Where tag_name = ?");
    query_colors.addBindValue(tag_name_str);
    if(!query_colors.exec())
    {
        recon_static_functions::app_debug(" -select query ---FAILED--- " + query_colors.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + query_colors.lastError().text(), Q_FUNC_INFO);
        destination_db.close();
        return false;
    }

    query_colors.clear();

    if(query_colors.next())
    {
        query_colors.prepare("Update tags Set color_hex = ?,selected_colour = ?  Where tag_name = ?");
        query_colors.addBindValue(tag_color_hex_str);
        query_colors.addBindValue(tag_color_name_str);
        query_colors.addBindValue(tag_name_str);
    }
    else
    {
        query_colors.prepare("Insert into tags(bookmark,color_hex,selected_colour,tag_name) Values(?,?,?,?)");
        query_colors.addBindValue(QString::number(1));
        query_colors.addBindValue(tag_color_hex_str);
        query_colors.addBindValue(tag_color_name_str);
        query_colors.addBindValue(tag_name_str);
    }

    if(!query_colors.exec())
    {
        recon_static_functions::app_debug(" -insert/update query ---FAILED--- " , Q_FUNC_INFO);
        destination_db.close();
        return false;
    }

    destination_db.close();

    add_existing_tags_to_combobox();

    ui->comboBox_saved_tags->setCurrentText(tag_name_str);

    on_checkBox_create_new_clicked(false);
    ui->checkBox_create_new->setCheckState(Qt::Unchecked);

    ui->lineEdit_new_tag->clear();

    return true;
}

void tag_create_popup::on_pushButton_cancel_clicked()
{
    hide();

}
