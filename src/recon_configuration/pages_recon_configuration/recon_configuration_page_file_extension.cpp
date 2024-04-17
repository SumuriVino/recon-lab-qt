#include "recon_configuration_page_file_extension.h"
#include "ui_recon_configuration_page_file_extension.h"

recon_configuration_page_file_extension::recon_configuration_page_file_extension(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration_page_file_extension)
{
    ui->setupUi(this);

    font_s.setPixelSize(12);

    ui->pushButton_add_category->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_remove_category->setIcon(QIcon("../icons/signs/remove.png"));

    ui->pushButton_add_extension->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_remove_extension->setIcon(QIcon("../icons/signs/remove.png"));
    ui->pushButton_keyword_clipboard->setIcon(QIcon("../icons/signs/clipboard.png"));

    recon_helper_standard_obj = new recon_helper_standard(this);


}


recon_configuration_page_file_extension::~recon_configuration_page_file_extension()
{
    delete ui;
}


void recon_configuration_page_file_extension::pub_set_caller_type(QString caller_type)
{

    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG)
    {
        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");
    }
    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    {
        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + QString("case_configuration/case_configuration.sqlite");
    }
    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER)
        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString("case_configuration.sqlite");

    prepare_extension_category_display();

    if(ui->tableWidget_category->rowCount() > 0)
        ui->tableWidget_category->setCurrentCell(0,0);

    prepare_extension_display();
}


void recon_configuration_page_file_extension::prepare_extension_category_display()
{
    QString command_str = QString("Select category_name From extension_category");
    QStringList category_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str , 0 , destination_db_file , Q_FUNC_INFO);

    //int current_category_row = ui->tableWidget_category->currentRow();

    if(category_list.size() <= 0)
        return;

    ui->tableWidget_category->setRowCount(0);

    for(int row = 0; row < category_list.size(); row++)
    {
        QString category_str = category_list.at(row);

        ui->tableWidget_category->insertRow(row);
        QTableWidgetItem *item_category = new QTableWidgetItem;
        item_category->setText(category_str);

        ui->tableWidget_category->setItem(row , 0 , item_category);
    }

    // prepare_extension_display();

}

void recon_configuration_page_file_extension::prepare_extension_display()
{
    int current_row_category = ui->tableWidget_category->currentRow();
    if(current_row_category < 0 )
        return;

    QString category_str = ui->tableWidget_category->currentItem()->text();

    QString command_str = QString("Select extension_name From extension_list Where category_name = ?");
    QStringList value_list;
    value_list << category_str;
    QStringList extension_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command_str , value_list , 0 , destination_db_file , Q_FUNC_INFO);

    ui->tableWidget_extension->setRowCount(0);

    for(int row = 0; row < extension_list.size(); row++)
    {
        QString extension_str = extension_list.at(row);

        int row_count = ui->tableWidget_extension->rowCount();

        if(extension_str.trimmed() == QString(""))
            continue;

        ui->tableWidget_extension->insertRow(row_count);
        ui->tableWidget_extension->setRowHeight(row_count, 23);
        QTableWidgetItem *item_extension = new QTableWidgetItem;
        item_extension->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);

        item_extension->setText(extension_str);
        ui->tableWidget_extension->setItem(row_count , 0 , item_extension);



        //        if(extension_str.contains(","))
        //        {
        //            QStringList extn_split_list = extension_str.split(",", Qt::SkipEmptyParts);
        //            for(int count = 0; count < extn_split_list.size(); count++)
        //            {
        //                extension_str = extn_split_list.at(count);
        //                ui->tableWidget_extension->insertRow(count);
        //                QTableWidgetItem *item_extension = new QTableWidgetItem;
        //                item_extension->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);

        //                item_extension->setText(extension_str);
        //                ui->tableWidget_extension->setItem(count , 0 , item_extension);
        //            }
        //        }
        //        else
        //        {
        //            ui->tableWidget_extension->insertRow(row);
        //            QTableWidgetItem *item_extension = new QTableWidgetItem;
        //            item_extension->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        //            item_extension->setText(extension_str);
        //            ui->tableWidget_extension->setItem(row , 0 , item_extension);
        //        }
    }

    if(ui->tableWidget_extension->rowCount() > 0)
        ui->tableWidget_extension->selectRow(0);

}

void recon_configuration_page_file_extension::on_tableWidget_category_cellClicked(int row, int column)
{
    if(row < 0 || column < 0)
        return;

    prepare_extension_display();
}

void recon_configuration_page_file_extension::on_pushButton_add_category_clicked()
{
    int row_count = ui->tableWidget_category->rowCount();

    if(ui->tableWidget_category->item(row_count - 1, 0) == NULL && row_count > 0)
    {
        return;
    }


    ui->tableWidget_category->insertRow(row_count);
    ui->tableWidget_category->setRowHeight(row_count, 23);

    new_category_LineEdit = new QLineEdit(this);
    new_category_LineEdit->setAttribute(Qt::WA_MacShowFocusRect,false);
    connect(new_category_LineEdit, SIGNAL(returnPressed()), this, SLOT(slot_new_category_line_edit_return_pressed()));

    ui->tableWidget_category->setCellWidget(row_count, 0, new_category_LineEdit);

    ui->tableWidget_category->setCurrentCell(row_count, 0);

    while(ui->tableWidget_extension->rowCount())
        ui->tableWidget_extension->removeRow(0);

}

void recon_configuration_page_file_extension::on_pushButton_remove_category_clicked()
{

    int current_row = ui->tableWidget_category->currentRow();

    if(current_row < 0)
        return;

    if(ui->tableWidget_category->item(current_row, 0) == NULL)
    {
        ui->tableWidget_category->removeRow(current_row);
        ui->tableWidget_category->setCurrentCell( -1, 0);

        return;
    }

    QString category_name = ui->tableWidget_category->item(current_row, 0)->text();


    QString command_str_extension = QString("DELETE FROM extension_list where category_name=?");

    QString command_str_category = QString("DELETE FROM extension_category where category_name=?");



    QStringList value_list;
    value_list << category_name;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str_extension , value_list, destination_db_file ,Q_FUNC_INFO);

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str_category , value_list, destination_db_file ,Q_FUNC_INFO);

    ui->tableWidget_category->removeRow(current_row);

    int row_count = ui->tableWidget_category->rowCount();

    if(ui->tableWidget_category->rowCount() - 1  < current_row)
        ui->tableWidget_category->setCurrentCell(ui->tableWidget_category->rowCount() - 1, 0);
    else
        ui->tableWidget_category->setCurrentCell(current_row, 0);

    on_tableWidget_category_cellClicked(current_row, 0);


    if(ui->tableWidget_category->rowCount() == 0)
    {
        while(ui->tableWidget_extension->rowCount())
            ui->tableWidget_extension->removeRow(0);
    }


    return;

    //    int current_row_category = ui->tableWidget_category->currentRow();
    //    if(current_row_category < 0)
    //    {
    //        ui->tableWidget_extension->setRowCount(0);
    //        return;
    //    }

    //    QString category_str = ui->tableWidget_category->currentItem()->text();
    //    category_str = category_str.trimmed();

    //    QString command_str = QString("Delete From extension_catagory Where catagory_name = ?");
    //    QStringList value_list;
    //    value_list << category_str;
    //    recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command_str , value_list, destination_db_file , Q_FUNC_INFO);

    //    command_str = QString("Delete From file_types Where catagory_name = ?");
    //    value_list.clear();
    //    value_list << category_str;
    //    recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command_str , value_list, destination_db_file , Q_FUNC_INFO);

    //    prepare_extension_category_display();
}

void recon_configuration_page_file_extension::on_pushButton_add_extension_clicked()
{
    int current_category_row_number = ui->tableWidget_category->currentRow();

    if(current_category_row_number < 0)
        return;

    if(ui->tableWidget_category->item(current_category_row_number, 0) == NULL)
        return;

    int row_count = ui->tableWidget_extension->rowCount();

    if(ui->tableWidget_extension->item(row_count - 1, 0) == NULL && row_count > 0)
    {
        return;
    }

    if(ui->tableWidget_category->currentRow() < 0)
        return;


    ui->tableWidget_extension->insertRow(row_count);
    ui->tableWidget_extension->setRowHeight(row_count, 23);

    new_extension_LineEdit = new QLineEdit(this);
    new_extension_LineEdit->setAttribute(Qt::WA_MacShowFocusRect,false);
    connect(new_extension_LineEdit, SIGNAL(returnPressed()), this, SLOT(slot_line_edit_return_pressed()));

    ui->tableWidget_extension->setCellWidget(row_count, 0, new_extension_LineEdit);

    ui->tableWidget_extension->setCurrentCell(row_count, 0);



    return;

    //    QTableWidgetItem *item = new QTableWidgetItem;
    //    //item->setFlags(item->flags() | Qt::ItemIsEditable);


    //    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);

    //    int row_count = ui->tableWidget_extension->rowCount();
    //    ui->tableWidget_extension->insertRow(row_count);

    //    ui->tableWidget_extension->setItem(row_count, 0, item);

    //    ui->tableWidget_extension->setCurrentItem(item);


    return;
    lineedit_add_extension->clear();
    dialog_add_extension->show();
}

void recon_configuration_page_file_extension::on_pushButton_remove_extension_clicked()
{
    int current_row = ui->tableWidget_extension->currentRow();

    if(current_row < 0)
        return;

    if(ui->tableWidget_extension->item(current_row, 0) == NULL)
    {
        ui->tableWidget_extension->removeRow(current_row);
        ui->tableWidget_extension->setCurrentCell( -1, 0);
        return;
    }

    QString extension_name = ui->tableWidget_extension->item(current_row, 0)->text();

    QString category_name = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 0)->text();


    QString command_str = QString("DELETE FROM extension_list where extension_name=? AND category_name=?");



    QStringList value_list;
    value_list << extension_name << category_name;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    ui->tableWidget_extension->removeRow(current_row);

    int row_count = ui->tableWidget_extension->rowCount();

    if(ui->tableWidget_extension->rowCount() - 1  < current_row)
        ui->tableWidget_extension->setCurrentCell(ui->tableWidget_extension->rowCount() - 1, 0);
    else
        ui->tableWidget_extension->setCurrentCell(current_row, 0);



    //    int extension_current_row = ui->tableWidget_extension->currentRow();
    //    int category_current_row  = ui->tableWidget_category->currentRow();

    //    if(extension_current_row < 0 || category_current_row < 0)
    //        return;

    //    QString selected_extn_str = ui->tableWidget_extension->currentItem()->text();

    //    QString command_str  = QString("Select extension_name From file_types Where catagory_name = ?");
    //    QStringList value_list;
    //    value_list << ui->tableWidget_category->currentItem()->text();
    //    QString db_exten_str = recon_static_functions::get_string_from_db_with_addbindvalue_by_dbpath(command_str , value_list, 0, destination_db_file, Q_FUNC_INFO);

    //    QString new_extn_str;
    //    if(db_exten_str.contains(","))
    //    {
    //        QStringList extn_split_list = db_exten_str.split(",", Qt::SkipEmptyParts);
    //        for(int count = 0; count < extn_split_list.size(); count++)
    //        {
    //            if(extn_split_list.at(count) == selected_extn_str)
    //                continue;


    //            new_extn_str.append(extn_split_list.at(count));
    //            new_extn_str.append(",");
    //        }
    //        if(new_extn_str.endsWith(","))
    //            new_extn_str.chop(1);

    //        new_extn_str = new_extn_str.trimmed();

    //    }

    //    command_str = QString("Update file_types set extension_name = ? Where catagory_name = ?");
    //    value_list.clear();
    //    value_list << new_extn_str << ui->tableWidget_category->currentItem()->text();
    //    recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command_str , value_list , destination_db_file , Q_FUNC_INFO);

    //    prepare_extension_category_display();

}

void recon_configuration_page_file_extension::prepare_category_display_dialog()
{
    recon_static_functions::debug_intensive(" - Starts " ,Q_FUNC_INFO);

    //-Create Dialog for adding Catagories.
    dialog_add_category = new QDialog(this);
    dialog_add_category->setWindowModality(Qt::WindowModal);

    QVBoxLayout *vboxlayout_add_category = new QVBoxLayout(dialog_add_category);

    lineedit_add_category = new QLineEdit(this);

    lineedit_add_category->setPlaceholderText(QString("Type Category..."));

    connect(lineedit_add_category,SIGNAL(textChanged(QString)),SLOT(slot_lineedit_add_category_text_changed(QString)));

    pushbutton_add_category = new QPushButton(QString("Add"),this);
    pushbutton_add_category->setFixedWidth(80);
    pushbutton_add_category->setFont(font_s);
    connect(pushbutton_add_category,SIGNAL(clicked()),this,SLOT(slot_pushbutton_dialog_add_category_clicked()));
    pushbutton_add_category->setEnabled(false);

    lineedit_add_category->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_add_category->setAttribute(Qt::WA_MacShowFocusRect,false);

    QHBoxLayout *hbox_add_button_category_layout = new QHBoxLayout;
    label_message_category = new QLabel(this);
    label_message_category->setMinimumWidth(140);
    label_message_category->setStyleSheet("QLabel{color : red;}");

    QPushButton *cancel_button = new QPushButton("Cancel",this);
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(slot_category_cancel_button_clicked()));
    cancel_button->setFixedWidth(80);



    hbox_add_button_category_layout->addWidget(label_message_category);
    hbox_add_button_category_layout->addWidget(cancel_button);
    hbox_add_button_category_layout->addWidget(pushbutton_add_category);


    vboxlayout_add_category->addWidget(lineedit_add_category);
    vboxlayout_add_category->addLayout(hbox_add_button_category_layout);
    vboxlayout_add_category->setContentsMargins(9,9,9,6);

    dialog_add_category->setLayout(vboxlayout_add_category);
    dialog_add_category->setWindowTitle(QString("Add category"));
    dialog_add_category->setMinimumWidth(200);
    dialog_add_category->setFont(font_s);

    dialog_add_category->setFixedSize(QSize(350, 100));

    recon_static_functions::debug_intensive(" - Ends " ,Q_FUNC_INFO);

}

void recon_configuration_page_file_extension::slot_pushbutton_dialog_add_category_clicked()
{
    recon_static_functions::app_debug(" - Starts",Q_FUNC_INFO);


    QString category_str = lineedit_add_category->text().trimmed();

    if(category_str.isEmpty())
    {
        return;
    }

    dialog_add_category->hide();



    /// check for category already exist(to avoid same entry)

    QString command = QString("Select category_name from extension_category");

    QStringList existing_category_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, destination_db_file, Q_FUNC_INFO);

    bool bool_category_already_exist = false;

    for(int count = 0; count < existing_category_list.size(); count++)
    {
        if(category_str.trimmed().toLower() == existing_category_list.at(count).trimmed().toLower())
        {
            bool_category_already_exist =  true;
            break;
        }
    }


    if(!bool_category_already_exist)
    {
        command = QString("insert into extension_category(bookmark,category_name) values(?,?)");
        QStringList list;
        list << QString("1") << category_str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,destination_db_file,Q_FUNC_INFO);

        prepare_extension_category_display();
    }
    else
    {
        /// set item selected if already exists
        for(int row = 0; row < ui->tableWidget_category->rowCount(); row++)
        {
            if(ui->tableWidget_category->item(row , 0)->text().trimmed().toLower() == category_str.trimmed().toLower())
            {
                ui->tableWidget_category->selectRow(row);
                break;
            }
        }

        prepare_extension_category_display();
    }


    recon_static_functions::app_debug(" - Ends",Q_FUNC_INFO);
}

void recon_configuration_page_file_extension::slot_lineedit_add_category_text_changed(QString text)
{
    text = text.trimmed();

    if(!text.isEmpty())
    {
        pushbutton_add_category->setEnabled(true);
        lineedit_add_category->setStyleSheet(styleSheet());
    }
    else
    {
        pushbutton_add_category->setEnabled(false);
    }
}

void recon_configuration_page_file_extension::slot_category_cancel_button_clicked()
{
    dialog_add_category->hide();
}

void recon_configuration_page_file_extension::prepare_extension_display_dialog()
{
    recon_static_functions::debug_intensive(" - Starts " ,Q_FUNC_INFO);

    //-Create Dialog for adding Extension.
    dialog_add_extension = new QDialog(this);
    dialog_add_extension->setWindowModality(Qt::WindowModal);


    QVBoxLayout *vboxlayout_add_extension = new QVBoxLayout(dialog_add_extension);

    lineedit_add_extension = new QLineEdit(this);
    lineedit_add_extension->setPlaceholderText(QString("File Extension..."));
    connect(lineedit_add_extension,SIGNAL(textChanged(QString)),SLOT(slot_lineedit_add_extension_text_changed(QString)));

    pushbutton_add_extension = new QPushButton(QString("Add"),this);

    pushbutton_add_extension->setFixedWidth(80);

    pushbutton_add_extension->setFont(font_s);
    connect(pushbutton_add_extension,SIGNAL(clicked()),this,SLOT(slot_pushbutton_dialog_add_extension_clicked()));

    QPushButton *cancel_button = new QPushButton("Cancel",this);
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(slot_extension_cancel_button_clicked()));
    cancel_button->setFixedWidth(80);


    lineedit_add_extension->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_add_extension->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_add_extension->setEnabled(false);

    QHBoxLayout *hbox_add_button_ext_layout = new QHBoxLayout;
    label_message_extension = new QLabel(this);
    label_message_extension->setMinimumWidth(140);
    label_message_extension->setStyleSheet("QLabel{color : red;}");

    hbox_add_button_ext_layout->addWidget(label_message_extension);
    hbox_add_button_ext_layout->addWidget(cancel_button);
    hbox_add_button_ext_layout->addWidget(pushbutton_add_extension);

    vboxlayout_add_extension->addWidget(lineedit_add_extension);
    vboxlayout_add_extension->addLayout(hbox_add_button_ext_layout);
    vboxlayout_add_extension->setContentsMargins(9,9,9,6);
    dialog_add_extension->setLayout(vboxlayout_add_extension);
    dialog_add_extension->setWindowTitle(QString("Add Extension"));
    dialog_add_extension->setMinimumWidth(200);
    dialog_add_extension->setFont(font_s);

    dialog_add_extension->setFixedSize(QSize(350, 100));

    recon_static_functions::debug_intensive(" - Ends " ,Q_FUNC_INFO);
}

void recon_configuration_page_file_extension::slot_lineedit_add_extension_text_changed(QString text)
{
    recon_static_functions::debug_intensive(" - Starts",Q_FUNC_INFO);

    text = text.trimmed();

    if(!text.isEmpty())
    {
        pushbutton_add_extension->setEnabled(true);
        lineedit_add_extension->setStyleSheet(styleSheet());
        label_message_extension->setStyleSheet(styleSheet());
        label_message_extension->clear();
    }
    else
    {
        pushbutton_add_extension->setEnabled(false);
    }

    recon_static_functions::debug_intensive(" - Ends",Q_FUNC_INFO);
}

void recon_configuration_page_file_extension::slot_pushbutton_dialog_add_extension_clicked()
{
    recon_static_functions::app_debug(" - Starts",Q_FUNC_INFO);

    /// extension to be added will be lower case (Manoj Sir)
    QString exten_str = lineedit_add_extension->text().trimmed().toLower();

    if(exten_str.isEmpty())
    {
        /// warning here for Empty extension
        dialog_add_extension->hide();
        return;
    }

    if(!exten_str.startsWith("."))
        exten_str.prepend(".");

    if(ui->tableWidget_category->currentRow() < 0|| ui->tableWidget_category->currentColumn() < 0)
    {
        dialog_add_extension->hide();
        return;
    }

    QString category_str = ui->tableWidget_category->currentItem()->text().trimmed();

    /// get existing extension
    QString command_str = QString("Select extension_name From file_types Where category_name = ?");

    QStringList values;
    values << category_str;

    QString existing_extension = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str, values, 0, destination_db_file, Q_FUNC_INFO);

    if(!existing_extension.isEmpty())
    {
        if(existing_extension.contains(QString(",")))
        {
            QStringList tmp_extn_list = existing_extension.split(QString(","), Qt::SkipEmptyParts);

            bool bool_extn_already_exist = false;
            for(int count = 0; count < tmp_extn_list.size(); count++)
            {
                if(tmp_extn_list.at(count) == exten_str)
                {
                    bool_extn_already_exist = true;
                    break;
                }
            }
            if(!bool_extn_already_exist)
            {
                existing_extension.append(QString(","));
                existing_extension.append(exten_str);

                command_str = QString("UPDATE file_types SET extension_name = ? WHERE category_name = ?");

                QStringList value_list;
                value_list << existing_extension << category_str;

                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);
            }

        }
        else
        {
            if(existing_extension != exten_str)
            {
                existing_extension.append(QString(","));
                existing_extension.append(exten_str);

                command_str = QString("UPDATE file_types SET extension_name = ? WHERE category_name = ?");

                QStringList value_list;
                value_list << existing_extension << category_str;

                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);
            }
        }

    }
    else
    {
        existing_extension = exten_str;
        command_str = QString("insert into file_types (bookmark, extension_name , category_name , extension_type) VALUES(?,?,?,?)");

        QStringList value_list;
        value_list << QString("1") << existing_extension << category_str << category_str.toUpper();

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);
    }

    if(ui->tableWidget_extension->rowCount() > 0)
        ui->tableWidget_extension->selectRow(0);

    dialog_add_extension->hide();

    int current_selection = ui->tableWidget_category->currentRow();
    prepare_extension_category_display();
    ui->tableWidget_category->setCurrentCell(current_selection, 0);
    //  on_tableWidget_category_cellClicked(current_selection, 0);


    recon_static_functions::app_debug(" - Ends",Q_FUNC_INFO);
}

void recon_configuration_page_file_extension::slot_extension_cancel_button_clicked()
{
    dialog_add_extension->hide();
}

void recon_configuration_page_file_extension::on_tableWidget_extension_itemPressed(QTableWidgetItem *item)
{
    //    QString new_extension = item->text();

    //    if(new_extension.trimmed().isEmpty())
    //        return;

    //    if(ui->tableWidget_category->currentRow() < 0)
    //        return;

    //    QString category_name = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 0)->text();


    //    QString command_str = QString("INSERT INTO extension_list (bookmark,category_name, extension_name) values (?,?,?)");



    //    QStringList value_list;
    //    value_list << "0" << category_name << new_extension;

    //    recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    //    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    //    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);

}

void recon_configuration_page_file_extension::on_tableWidget_extension_itemChanged(QTableWidgetItem *item)
{
    //disconnect(ui->tableWidget_extension, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(on_tableWidget_extension_itemChanged(QTableWidgetItem*)));

    //    if(display_loaded == false)
    //        return;

    //    QString new_extension = item->text();

    //    if(new_extension.trimmed().isEmpty())
    //        return;

    //    if(ui->tableWidget_category->currentRow() < 0)
    //        return;

    //    QString category_name = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 0)->text();


    //    QString command_str = QString("INSERT INTO extension_list (bookmark,category_name, extension_name) values (?,?,?)");



    //    QStringList value_list;
    //    value_list << "0" << category_name << new_extension;

    //    recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    //    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    //    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);

    //connect(ui->tableWidget_extension, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(on_tableWidget_extension_itemChanged(QTableWidgetItem*)));

}

void recon_configuration_page_file_extension::slot_line_edit_return_pressed()
{

    QString new_extension = new_extension_LineEdit->text();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(new_extension);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);




    if(new_extension.trimmed().isEmpty())
        return;

    if(ui->tableWidget_category->currentRow() < 0)
        return;

    QString category_name = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 0)->text();


    QString command_str = QString("INSERT INTO extension_list (bookmark,category_name, extension_name) values (?,?,?)");



    QStringList value_list;
    value_list << "1" << category_name << new_extension;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    ui->tableWidget_extension->removeCellWidget(ui->tableWidget_extension->currentRow(), 0);
    ui->tableWidget_extension->setItem(ui->tableWidget_extension->currentRow(), 0, item);

}

void recon_configuration_page_file_extension::slot_new_category_line_edit_return_pressed()
{
    QString new_category = new_category_LineEdit->text();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(new_category);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);


    if(new_category.trimmed().isEmpty())
        return;

    if(ui->tableWidget_category->currentRow() < 0)
        return;


    QString command_str = QString("INSERT INTO extension_category (bookmark,category_name) values (?,?)");



    QStringList value_list;
    value_list << "1" << new_category;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    ui->tableWidget_category->removeCellWidget(ui->tableWidget_category->currentRow(), 0);
    ui->tableWidget_category->setItem(ui->tableWidget_category->currentRow(), 0, item);

}

void recon_configuration_page_file_extension::on_pushButton_keyword_clipboard_clicked()
{
    if(ui->tableWidget_category->currentRow() < 0)
        return;

    QString category = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 0)->text();

    QString clipboard_data =  QApplication::clipboard()->text();

    QStringList data_list = clipboard_data.split("\n", Qt::SkipEmptyParts);

    if(data_list.size() > 0)
    {
        for(int i = 0 ; i < data_list.size(); i++)
        {
            QString command_str = QString("INSERT INTO extension_list (bookmark, category_name, extension_name) values (?,?,?)");

            QStringList value_list;
            value_list << "1" << category << data_list.at(i);

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

        }
    }

    prepare_extension_display();
}
