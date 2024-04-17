#include "recon_configuration_page_filename_list.h"
#include "ui_recon_configuration_page_filename_list.h"

recon_configuration_page_filename_list::recon_configuration_page_filename_list(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration_page_filename_list)
{
    ui->setupUi(this);

    font_s.setPixelSize(12);

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->pushButton_category_add->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_category_remove->setIcon(QIcon("../icons/signs/remove.png"));
    ui->pushButton_edit_category->setIcon(QIcon("../icons/signs/edit.png"));


    ui->pushButton_filename_add->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_filename_remove->setIcon(QIcon("../icons/signs/remove.png"));
    ui->pushButton_filename_clipboard->setIcon(QIcon("../icons/signs/clipboard.png"));
    ui->pushButton_edit_filename->setIcon(QIcon("../icons/signs/edit.png"));


    ui->tableWidget_category->setColumnWidth(0, 25);
    ui->tableWidget_filename->hideColumn(0);
    ui->tableWidget_filename->setColumnWidth(1, 400);
}

recon_configuration_page_filename_list::~recon_configuration_page_filename_list()
{
    delete ui;
}

void recon_configuration_page_filename_list::pub_set_caller_type(QString caller_type)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG)
    {
        destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");
    }
    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    {
        destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + QString("case_configuration/case_configuration.sqlite");
    }
    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER)
    {
        destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString("case_configuration.sqlite");
    }

    prepare_category_display();
    prepare_filename_display();

    prepare_edit_category_dialog();
    prepare_edit_filename_dialog();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::prepare_category_display()
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    QString recon_config_db_path = destination_db_path;
    QString command_str = "select category, category_status from filename_category";

    QStringList filename_category_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str ,0 ,recon_config_db_path ,Q_FUNC_INFO);
    QStringList filename_category_status_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str ,1 ,recon_config_db_path ,Q_FUNC_INFO);

    ui->tableWidget_category->setRowCount(0);

    for(int row = 0; row < filename_category_list.size(); row++)
    {
        ui->tableWidget_category->insertRow(row);
        ui->tableWidget_category->setRowHeight(row, 23);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(filename_category_list.at(row));

        QTableWidgetItem *item_status = new QTableWidgetItem;
        item_status->setCheckState(Qt::Unchecked);

        if(filename_category_status_list.at(row) == "1")
            item_status->setCheckState(Qt::Checked);

        ui->tableWidget_category->setItem(row , 0 , item_status);
        ui->tableWidget_category->setItem(row , 1 , item);
    }

    if(ui->tableWidget_category->rowCount() > 0)
        ui->tableWidget_category->selectRow(0);

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::prepare_filename_display()
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    int current_row = ui->tableWidget_category->currentRow();

    if(current_row < 0)
        return;

    if(ui->tableWidget_category->item(current_row, 1) == NULL)
        return;

    QString category = ui->tableWidget_category->item(current_row, 1)->text().trimmed();

    if(category.isEmpty())
        return;

    QString recon_config_db_path = destination_db_path;
    QString command_str = "select filename, filename_status from filename_list Where category = ?";
    QStringList value_list;
    value_list << category;

    ui->tableWidget_filename->setRowCount(0);

    QStringList filename_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command_str,value_list ,0 ,recon_config_db_path ,Q_FUNC_INFO);
    QStringList filename_list_status = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command_str,value_list ,1 ,recon_config_db_path ,Q_FUNC_INFO);

    for(int row = 0; row < filename_list.size(); row++)
    {
        ui->tableWidget_filename->insertRow(row);
        ui->tableWidget_filename->setRowHeight(row, 23);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(filename_list.at(row));

        QTableWidgetItem *item_status = new QTableWidgetItem;
        item_status->setCheckState(Qt::Unchecked);

        if(filename_list_status.at(row) == "1")
            item_status->setCheckState(Qt::Checked);

        ui->tableWidget_filename->setItem(row , 0 , item_status);
        ui->tableWidget_filename->setItem(row , 1 , item);
    }

    if(ui->tableWidget_filename->rowCount() > 0)
        ui->tableWidget_filename->selectRow(0);

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::on_tableWidget_category_cellClicked(int row, int column)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return;

    if(column == 0)
    {
        if(ui->tableWidget_category->item(row, 1) == NULL)
            return;

        QString category_status = "0";
        QString category = ui->tableWidget_category->item(row, 1)->text();

        if(ui->tableWidget_category->item(row, 0)->checkState() == Qt::Checked)
            category_status = "1";

        QString cmd = "update filename_category set category_status = ? where category = ?";

        QStringList args;
        args << category_status << category;

        QString destination_db_file = destination_db_path;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd, args, destination_db_file, Q_FUNC_INFO);
    }

    prepare_filename_display();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::on_pushButton_category_add_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    int row_count = ui->tableWidget_category->rowCount();

    if(ui->tableWidget_category->item(row_count - 1, 1) == NULL && row_count > 0)
        return;

    ui->tableWidget_category->insertRow(row_count);
    ui->tableWidget_category->setRowHeight(row_count, 23);

    new_category_LineEdit = new QLineEdit(this);
    new_category_LineEdit->setAttribute(Qt::WA_MacShowFocusRect,false);
    connect(new_category_LineEdit, SIGNAL(returnPressed()), this, SLOT(slot_new_category_line_edit_return_pressed()));

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setCheckState(Qt::Checked);
    ui->tableWidget_category->setItem(row_count, 0, item);

    ui->tableWidget_category->setCellWidget(row_count, 1, new_category_LineEdit);
    ui->tableWidget_category->setCurrentCell(row_count, 1);

    ui->tableWidget_filename->setRowCount(0);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::on_pushButton_category_remove_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    int current_row = ui->tableWidget_category->currentRow();

    if(current_row < 0)
        return;

    if(ui->tableWidget_category->item(current_row, 1) == NULL)
    {
        ui->tableWidget_category->removeRow(current_row);
        ui->tableWidget_category->setCurrentCell( -1, 1);
        return;
    }


    QString destination_db_file = destination_db_path;

    QString category_name = ui->tableWidget_category->item(current_row, 1)->text();

    QString command_str_extension = QString("DELETE FROM filename_list where category=?");

    QString command_str_category = QString("DELETE FROM filename_category where category=?");



    QStringList value_list;
    value_list << category_name;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str_extension , value_list, destination_db_file ,Q_FUNC_INFO);

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str_category , value_list, destination_db_file ,Q_FUNC_INFO);

    ui->tableWidget_category->removeRow(current_row);

    if(ui->tableWidget_category->rowCount() - 1  < current_row)
        ui->tableWidget_category->setCurrentCell(ui->tableWidget_category->rowCount() - 1, 1);
    else
        ui->tableWidget_category->setCurrentCell(current_row, 1);

    on_tableWidget_category_cellClicked(current_row, 1);


    if(ui->tableWidget_category->rowCount() == 0)
        ui->tableWidget_filename->setRowCount(0);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::on_pushButton_filename_add_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    int current_category_row_number = ui->tableWidget_category->currentRow();

    if(current_category_row_number < 0)
        return;

    if(ui->tableWidget_category->item(current_category_row_number, 1) == NULL)
        return;


    int row_count = ui->tableWidget_filename->rowCount();

    if(ui->tableWidget_filename->item(row_count - 1, 1) == NULL && row_count > 0)
        return;

    if(ui->tableWidget_category->currentRow() < 0)
        return;

    ui->tableWidget_filename->insertRow(row_count);
    ui->tableWidget_filename->setRowHeight(row_count, 23);

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setCheckState(Qt::Checked);
    ui->tableWidget_filename->setItem(row_count, 0, item);

    QTableWidgetItem *item_regex = new QTableWidgetItem;
    item_regex->setCheckState(Qt::Unchecked);
    ui->tableWidget_filename->setItem(row_count, 2, item_regex);


    lineedit_add_filename = new QLineEdit(this);
    lineedit_add_filename->setAttribute(Qt::WA_MacShowFocusRect,false);
    connect(lineedit_add_filename, SIGNAL(returnPressed()), this, SLOT(slot_filename_line_add_return_pressed()));
    ui->tableWidget_filename->setCellWidget(row_count, 1, lineedit_add_filename);
    ui->tableWidget_filename->setCurrentCell(row_count, 1);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::on_pushButton_filename_remove_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    int current_row = ui->tableWidget_filename->currentRow();

    if(current_row < 0)
        return;

    if(ui->tableWidget_filename->item(current_row, 1) == NULL)
    {
        ui->tableWidget_filename->removeRow(current_row);
        ui->tableWidget_filename->setCurrentCell( -1, 1);
        return;
    }

    QString destination_db_file = destination_db_path;

    QString filename_name = ui->tableWidget_filename->item(current_row, 1)->text().trimmed();

    if(filename_name.isEmpty())
        return;

    if(ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1) == NULL)
        return;

    QString category_name = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1)->text().trimmed();

    if(category_name.isEmpty())
        return;

    QString command_str = QString("DELETE FROM filename_list where filename=? AND category=?");

    QStringList value_list;
    value_list << filename_name << category_name;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    ui->tableWidget_filename->removeRow(current_row);

    if(ui->tableWidget_filename->rowCount() - 1  < current_row)
        ui->tableWidget_filename->setCurrentCell(ui->tableWidget_filename->rowCount() - 1, 1);
    else
        ui->tableWidget_filename->setCurrentCell(current_row, 1);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::slot_filename_line_add_return_pressed()
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    QString filename_str = lineedit_add_filename->text().trimmed();

    int current_row = ui->tableWidget_filename->currentRow();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(filename_str);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);

    if(filename_str.trimmed().isEmpty())
        return;

    if(ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1) == NULL)
        return;

    QString category_str = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1)->text();
    if(category_str.isEmpty())
        return;

    QString destination_db_file = destination_db_path;

    QString command = QString("Select filename from filename_list Where category = ?");
    QStringList value_list;
    value_list << category_str;
    QStringList existing_filename_in_category_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, destination_db_file, Q_FUNC_INFO);

    bool bool_filename_already_exist = false;

    for(int count = 0; count < existing_filename_in_category_list.size(); count++)
    {
        if(filename_str.trimmed() == existing_filename_in_category_list.at(count).trimmed())
        {
            bool_filename_already_exist =  true;
            break;
        }
    }

    if(!bool_filename_already_exist)
    {
        command = QString("insert into filename_list(filename_status,category,filename) values(?,?,?)");
        QStringList list;
        list << QString("1") << category_str << filename_str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,destination_db_file,Q_FUNC_INFO);
        ui->tableWidget_filename->removeCellWidget(ui->tableWidget_filename->currentRow(), 1);
        ui->tableWidget_filename->setItem(ui->tableWidget_filename->currentRow(), 1, item);
    }
    else
    {
        for(int row = 0; row < ui->tableWidget_filename->rowCount(); row++)
        {
            if(ui->tableWidget_filename->item(row , 1)->text().trimmed().toLower() == filename_str.trimmed().toLower())
            {
                ui->tableWidget_filename->removeRow(current_row);
                break;
            }
        }
    }

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::slot_new_category_line_edit_return_pressed()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString new_category = new_category_LineEdit->text().trimmed();
    int current_row = ui->tableWidget_category->currentRow();

    if(new_category.isEmpty())
        return;

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(new_category);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);

    if(ui->tableWidget_category->currentRow() < 0)
        return;

    QString destination_db_file = destination_db_path;

    QString command = QString("Select category from filename_category");
    QStringList filename_category_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command ,0 ,destination_db_file ,Q_FUNC_INFO);

    bool bool_category_already_exist = false;

    for(int count = 0; count < filename_category_list.size(); count++)
    {
        if(new_category.trimmed() == filename_category_list.at(count).trimmed())
        {
            bool_category_already_exist =  true;
            break;
        }
    }

    if(!bool_category_already_exist)
    {
        QString command_str = QString("INSERT INTO filename_category (category_status, category) values (?,?)");

        QStringList value_list;
        value_list << "1" << new_category;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

        ui->tableWidget_category->removeCellWidget(ui->tableWidget_category->currentRow(), 1);
        ui->tableWidget_category->setItem(ui->tableWidget_category->currentRow(), 1, item);
    }
    else
    {
        for(int row = 0; row < ui->tableWidget_category->rowCount(); row++)
        {
            if(ui->tableWidget_category->item(row , 1)->text().trimmed().toLower() == new_category.trimmed().toLower())
            {
                ui->tableWidget_category->removeRow(current_row);
                break;
            }
        }
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::on_pushButton_filename_clipboard_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    if(ui->tableWidget_category->currentRow() < 0)
        return;

    if(ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1) == NULL)
        return;

    QString category = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1)->text().trimmed();

    if(category.isEmpty())
        return;

    QString destination_db_file = destination_db_path;

    QString clipboard_data =  QApplication::clipboard()->text().trimmed();

    QStringList clipboard_filename_list = clipboard_data.split("\n", Qt::SkipEmptyParts);
    clipboard_filename_list.removeDuplicates();

    QString command = QString("Select filename from filename_list Where category = ?");
    QStringList value_list;
    value_list << category;
    QStringList existing_filename_in_category_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, destination_db_file, Q_FUNC_INFO);

    for(int count = 0; count < clipboard_filename_list.size(); count++)
    {
        QString clipboard_filename = clipboard_filename_list.at(count).trimmed();

        if(!existing_filename_in_category_list.contains(clipboard_filename))
        {
            QString command_str = QString("INSERT INTO filename_list (filename_status, category, filename) values (?,?,?)");
            QStringList value_list;
            value_list << QString::number(1) << category << clipboard_filename.trimmed();

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);
        }
    }
    prepare_filename_display();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::on_pushButton_edit_category_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    int row_count = ui->tableWidget_category->rowCount();

    if(ui->tableWidget_category->item(row_count - 1, 1) == NULL && row_count > 0)
        return;

    lineedit_edit_category->clear();
    int current_row = ui->tableWidget_category->currentRow();
    int current_col = ui->tableWidget_category->currentColumn();

    if(current_row < 0 || current_col < 0)
        return;

    category_to_update_str = ui->tableWidget_category->item(current_row, current_col)->text().trimmed();

    if(category_to_update_str.isEmpty())
        return;

    lineedit_edit_category->setText(category_to_update_str);

    dialog_edit_category->show();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::prepare_edit_category_dialog()
{
    recon_static_functions::debug_intensive("Starts " ,Q_FUNC_INFO);

    //-Create Dialog for editing Catagories.
    dialog_edit_category = new QDialog(this);
    dialog_edit_category->setWindowModality(Qt::WindowModal);


    QVBoxLayout *vboxlayout_edit_category = new QVBoxLayout(dialog_edit_category);

    lineedit_edit_category = new QLineEdit(this);
    lineedit_edit_category->setPlaceholderText(QString("Type Category..."));

    connect(lineedit_edit_category,SIGNAL(textChanged(QString)),SLOT(slot_lineedit_edit_category_text_changed(QString)));

    pushbutton_add_edited_category = new QPushButton(QString("Add"),this);
    pushbutton_add_edited_category->setFixedWidth(80);
    pushbutton_add_edited_category->setFont(font_s);
    connect(pushbutton_add_edited_category,SIGNAL(clicked()),this,SLOT(slot_pushbutton_add_edited_category_clicked()));
    pushbutton_add_edited_category->setEnabled(false);

    lineedit_edit_category->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_add_edited_category->setAttribute(Qt::WA_MacShowFocusRect,false);

    QHBoxLayout *hbox_add_button_category_layout = new QHBoxLayout;
    label_message_edit_category = new QLabel(this);
    label_message_edit_category->setMinimumWidth(140);
    label_message_edit_category->setStyleSheet("QLabel{color : red;}");

    QPushButton *edit_category_cancel_button = new QPushButton("Cancel",this);
    connect(edit_category_cancel_button,SIGNAL(clicked()),this,SLOT(slot_edit_category_cancel_button_clicked()));
    edit_category_cancel_button->setFixedWidth(80);

    hbox_add_button_category_layout->addWidget(label_message_edit_category);
    hbox_add_button_category_layout->addWidget(edit_category_cancel_button);
    hbox_add_button_category_layout->addWidget(pushbutton_add_edited_category);

    vboxlayout_edit_category->addWidget(lineedit_edit_category);
    vboxlayout_edit_category->addLayout(hbox_add_button_category_layout);
    vboxlayout_edit_category->setContentsMargins(9,9,9,6);

    dialog_edit_category->setLayout(vboxlayout_edit_category);
    dialog_edit_category->setWindowTitle(QString("Edit category"));
    dialog_edit_category->setMinimumWidth(200);
    dialog_edit_category->setFont(font_s);

    dialog_edit_category->setFixedSize(QSize(350, 100));

    recon_static_functions::debug_intensive("Ends " ,Q_FUNC_INFO);

}

void recon_configuration_page_filename_list::slot_pushbutton_add_edited_category_clicked()
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    dialog_edit_category->hide();

    QString category_str = lineedit_edit_category->text().trimmed();
    if(category_str.isEmpty() || category_to_update_str.trimmed().isEmpty())
        return;

    QString destination_db_file = destination_db_path;

    QString command = QString("Select category from filename_category");
    QStringList existing_category_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, destination_db_file, Q_FUNC_INFO);

    bool bool_category_already_exist = false;

    for(int count = 0; count < existing_category_list.size(); count++)
    {
        if(category_str.trimmed() == existing_category_list.at(count).trimmed())
        {
            bool_category_already_exist =  true;
            break;
        }
    }

    int current_row = ui->tableWidget_category->currentRow();

    if(!bool_category_already_exist)
    {
        QString command = QString("Update filename_category SET category = ? Where category = ?");
        QStringList list;
        list <<  category_str << category_to_update_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,destination_db_file,Q_FUNC_INFO);

        command = QString("Update filename_list SET category = ? Where category = ?");
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,destination_db_file,Q_FUNC_INFO);

        prepare_category_display();

        ui->tableWidget_category->selectRow(current_row);
    }


    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::slot_lineedit_edit_category_text_changed(QString text)
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    text = text.trimmed();

    if(!text.isEmpty())
    {
        pushbutton_add_edited_category->setEnabled(true);
        lineedit_edit_category->setStyleSheet(styleSheet());
    }
    else
    {
        pushbutton_add_edited_category->setEnabled(false);
    }

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::slot_edit_category_cancel_button_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    dialog_edit_category->hide();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::on_pushButton_edit_filename_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    int current_category_row_number = ui->tableWidget_category->currentRow();
    if(current_category_row_number < 0)
        return;

    if(ui->tableWidget_category->item(current_category_row_number, 1) == NULL)
        return;


    int row_count = ui->tableWidget_filename->rowCount();
    if(ui->tableWidget_filename->item(row_count - 1, 1) == NULL && row_count > 0)
        return;

    lineedit_edit_filename->clear();

    int current_row = ui->tableWidget_filename->currentRow();
    int current_col = ui->tableWidget_filename->currentColumn();

    if(current_row < 0 || current_col < 0)
        return;

    filename_to_update_str = ui->tableWidget_filename->item(current_row, current_col)->text().trimmed();

    if(filename_to_update_str.isEmpty())
        return;

    lineedit_edit_filename->setText(filename_to_update_str);

    dialog_edit_filename->show();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::prepare_edit_filename_dialog()
{
    recon_static_functions::debug_intensive("Starts " ,Q_FUNC_INFO);

    //-Create Dialog for edit filename.
    dialog_edit_filename = new QDialog(this);
    dialog_edit_filename->setWindowModality(Qt::WindowModal);


    QVBoxLayout *vboxlayout_edit_filename = new QVBoxLayout(dialog_edit_filename);

    lineedit_edit_filename = new QLineEdit(this);
    lineedit_edit_filename->setPlaceholderText(QString("filename..."));

    connect(lineedit_edit_filename,SIGNAL(textChanged(QString)),SLOT(slot_lineedit_edit_filename_text_changed(QString)));

    pushbutton_add_edited_filename = new QPushButton(QString("Add"),this);
    pushbutton_add_edited_filename->setFixedWidth(80);
    pushbutton_add_edited_filename->setFont(font_s);
    connect(pushbutton_add_edited_filename,SIGNAL(clicked()),this,SLOT(slot_pushbutton_add_edited_filename_clicked()));
    pushbutton_add_edited_filename->setEnabled(false);

    lineedit_edit_filename->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_add_edited_filename->setAttribute(Qt::WA_MacShowFocusRect,false);

    QHBoxLayout *hbox_add_button_filename_layout = new QHBoxLayout;
    label_message_edit_filename = new QLabel(this);
    label_message_edit_filename->setMinimumWidth(140);
    label_message_edit_filename->setStyleSheet("QLabel{color : red;}");

    QPushButton *edit_filename_cancel_button = new QPushButton("Cancel",this);
    connect(edit_filename_cancel_button,SIGNAL(clicked()),this,SLOT(slot_edit_filename_cancel_button_clicked()));
    edit_filename_cancel_button->setFixedWidth(80);

    hbox_add_button_filename_layout->addWidget(label_message_edit_filename);
    hbox_add_button_filename_layout->addWidget(edit_filename_cancel_button);
    hbox_add_button_filename_layout->addWidget(pushbutton_add_edited_filename);

    vboxlayout_edit_filename->addWidget(lineedit_edit_filename);
    vboxlayout_edit_filename->addLayout(hbox_add_button_filename_layout);
    vboxlayout_edit_filename->setContentsMargins(9,9,9,6);

    dialog_edit_filename->setLayout(vboxlayout_edit_filename);
    dialog_edit_filename->setWindowTitle(QString("Edit Filename"));
    dialog_edit_filename->setMinimumWidth(200);
    dialog_edit_filename->setFont(font_s);

    dialog_edit_filename->setFixedSize(QSize(350, 100));

    recon_static_functions::debug_intensive("Ends " ,Q_FUNC_INFO);

}

void recon_configuration_page_filename_list::slot_lineedit_edit_filename_text_changed(QString text)
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    text = text.trimmed();

    if(!text.isEmpty())
    {
        pushbutton_add_edited_filename->setEnabled(true);
        lineedit_edit_filename->setStyleSheet(styleSheet());
    }
    else
    {
        pushbutton_add_edited_filename->setEnabled(false);
    }

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);

}

void recon_configuration_page_filename_list::slot_pushbutton_add_edited_filename_clicked()
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    dialog_edit_filename->hide();

    QString filename_str = lineedit_edit_filename->text().trimmed();
    if(filename_str.isEmpty() || filename_to_update_str.trimmed().isEmpty())
        return;

    if(ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1) == NULL)
        return;

    QString category_str = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1)->text();
    if(category_str.isEmpty())
        return;

    QString destination_db_file = destination_db_path;

    QString command = QString("Select filename from filename_list Where category = ?");
    QStringList value_list;
    value_list << category_str;
    QStringList existing_filename_in_category_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, destination_db_file, Q_FUNC_INFO);

    bool bool_filename_already_exist = false;

    for(int count = 0; count < existing_filename_in_category_list.size(); count++)
    {
        if(filename_str.trimmed() == existing_filename_in_category_list.at(count).trimmed().toLower())
        {
            bool_filename_already_exist =  true;
            break;
        }
    }

    if(!bool_filename_already_exist)
    {
        QString command_str = QString("Update filename_list SET filename = ? Where category = ? AND filename = ?");
        QStringList list;
        list << filename_str <<  category_str << filename_to_update_str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str,list,destination_db_file,Q_FUNC_INFO);

        int current_row_category = ui->tableWidget_category->currentRow();
        int current_row_filename  = ui->tableWidget_filename->currentRow();

        prepare_filename_display();

        ui->tableWidget_category->selectRow(current_row_category);
        ui->tableWidget_filename->selectRow(current_row_filename);
    }

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void recon_configuration_page_filename_list::slot_edit_filename_cancel_button_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    dialog_edit_filename->hide();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}


