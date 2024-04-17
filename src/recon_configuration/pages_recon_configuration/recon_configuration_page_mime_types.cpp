#include "recon_configuration_page_mime_types.h"
#include "ui_recon_configuration_page_mime_types.h"

recon_configuration_page_mime_types::recon_configuration_page_mime_types(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration_page_mime_types)
{
    ui->setupUi(this);

    ui->pushButton_add_category->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_remove_category->setIcon(QIcon("../icons/signs/remove.png"));

    ui->pushButton_add_mime_type->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_remove_mime_type->setIcon(QIcon("../icons/signs/remove.png"));

    font_s.setPixelSize(12);

    recon_helper_standard_obj = new recon_helper_standard(this);

    prepare_category_display_dialog();
    prepare_extension_display_dialog();
}

recon_configuration_page_mime_types::~recon_configuration_page_mime_types()
{
    delete ui;
}

void recon_configuration_page_mime_types::on_pushButton_add_category_clicked()
{
    int row_count = ui->tableWidget_category->rowCount();

    if(ui->tableWidget_category->item(row_count - 1, 0) == NULL && row_count > 0)
    {
        return;
    }


    ui->tableWidget_category->insertRow(row_count);

    new_category_LineEdit = new QLineEdit(this);
    connect(new_category_LineEdit, SIGNAL(returnPressed()), this, SLOT(slot_new_category_line_edit_return_pressed()));

    ui->tableWidget_category->setCellWidget(row_count, 0, new_category_LineEdit);

    ui->tableWidget_category->setCurrentCell(row_count, 0);

    while(ui->tableWidget_mime_type->rowCount())
        ui->tableWidget_mime_type->removeRow(0);

    return;
    lineedit_add_category->clear();
    dialog_add_category->show();
}

void recon_configuration_page_mime_types::on_pushButton_remove_category_clicked()
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


    QString command_str_extension = QString("DELETE FROM tbl_mime_type where category_name=?");

    QString command_str_category = QString("DELETE FROM mime_type_category where category_name=?");



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
        while(ui->tableWidget_mime_type->rowCount())
            ui->tableWidget_mime_type->removeRow(0);
    }
}

void recon_configuration_page_mime_types::on_pushButton_add_mime_type_clicked()
{
    int row_count = ui->tableWidget_mime_type->rowCount();

    if(ui->tableWidget_mime_type->item(row_count - 1, 0) == NULL && row_count > 0)
    {
        return;
    }

    if(ui->tableWidget_category->currentRow() < 0)
        return;

    ui->tableWidget_mime_type->insertRow(row_count);

    new_mime_type_LineEdit = new QLineEdit(this);
    connect(new_mime_type_LineEdit, SIGNAL(returnPressed()), this, SLOT(slot_new_mime_type_LineEdit_pressed()));

    ui->tableWidget_mime_type->setCellWidget(row_count, 0, new_mime_type_LineEdit);

    ui->tableWidget_mime_type->setCurrentCell(row_count, 0);



    return;

    lineedit_add_mime_type->clear();
    dialog_add_mime_type->show();
}

void recon_configuration_page_mime_types::on_pushButton_remove_mime_type_clicked()
{
    int current_row = ui->tableWidget_mime_type->currentRow();

    if(current_row < 0)
        return;

    if(ui->tableWidget_mime_type->item(current_row, 0) == NULL)
    {
        ui->tableWidget_mime_type->removeRow(current_row);
        ui->tableWidget_mime_type->setCurrentCell( -1, 0);
        return;
    }

    QString mime_type = ui->tableWidget_mime_type->item(current_row, 0)->text();

    QString category_name = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 0)->text();


    QString command_str = QString("DELETE FROM tbl_mime_type where mime_type=? AND category_name=?");



    QStringList value_list;
    value_list << mime_type << category_name;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    ui->tableWidget_mime_type->removeRow(current_row);

    int row_count = ui->tableWidget_mime_type->rowCount();

    if(ui->tableWidget_mime_type->rowCount() - 1  < current_row)
        ui->tableWidget_mime_type->setCurrentCell(ui->tableWidget_mime_type->rowCount() - 1, 0);
    else
        ui->tableWidget_mime_type->setCurrentCell(current_row, 0);


    //    int extension_current_row = ui->tableWidget_mime_type->currentRow();
    //    int category_current_row  = ui->tableWidget_category->currentRow();

    //    if(extension_current_row < 0 || category_current_row < 0)
    //        return;

    //    QString selected_extn_str = ui->tableWidget_mime_type->currentItem()->text();

    //    QStringList value_list;
    //    QString command_str = QString("Delete From tbl_mime_type Where mime_type = ? AND category_name = ?");
    //    value_list << selected_extn_str << ui->tableWidget_category->currentItem()->text();
    //    recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command_str , value_list , destination_db_file , Q_FUNC_INFO);

    //    prepare_mime_category_display();
}

void recon_configuration_page_mime_types::on_tableWidget_category_cellClicked(int row, int column)
{
    if(row < 0 || column < 0)
        return;

    prepare_mime_type_display();
}

void recon_configuration_page_mime_types::pub_set_essentials()
{
    destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");

    prepare_mime_category_display();
}

void recon_configuration_page_mime_types::prepare_mime_category_display()
{
    QString command_str = QString("Select category_name From mime_type_category");
    QStringList category_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str , 0 , destination_db_file , Q_FUNC_INFO);

    ui->tableWidget_category->setRowCount(0);

    for(int row = 0; row < category_list.size(); row++)
    {
        QString category_str = category_list.at(row);

        ui->tableWidget_category->insertRow(row);
        QTableWidgetItem *item_category = new QTableWidgetItem;
        item_category->setText(category_str);

        ui->tableWidget_category->setItem(row , 0 , item_category);
    }

    if(ui->tableWidget_category->rowCount() > 0)
        ui->tableWidget_category->selectRow(0);

    prepare_mime_type_display();

}

void recon_configuration_page_mime_types::prepare_mime_type_display()
{
    int current_row_category = ui->tableWidget_category->currentRow();
    if(current_row_category < 0 )
        return;

    QString category_str = ui->tableWidget_category->currentItem()->text();

    QString command_str = QString("Select mime_type From tbl_mime_type Where category_name = ?");
    QStringList value_list;
    value_list << category_str;
    QStringList extension_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command_str , value_list , 0 , destination_db_file , Q_FUNC_INFO);

    ui->tableWidget_mime_type->setRowCount(0);

    for(int row = 0; row < extension_list.size(); row++)
    {
        QString extension_str = extension_list.at(row);

        ui->tableWidget_mime_type->insertRow(row);
        QTableWidgetItem *item_extension = new QTableWidgetItem;
        item_extension->setText(extension_str);
        ui->tableWidget_mime_type->setItem(row , 0 , item_extension);
    }

    if(ui->tableWidget_mime_type->rowCount() > 0)
        ui->tableWidget_mime_type->selectRow(0);

}

void recon_configuration_page_mime_types::prepare_category_display_dialog()
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

void recon_configuration_page_mime_types::slot_pushbutton_dialog_add_category_clicked()
{
    recon_static_functions::app_debug(" - Starts",Q_FUNC_INFO);

    QString category_str = lineedit_add_category->text().trimmed();

    if(category_str.isEmpty())
    {
        return;
    }

    dialog_add_category->hide();

    /// check for category already exist(to avoid same entry)
    QString command = QString("Select category_name from mime_type_category");

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
        command = QString("insert into mime_type_category(bookmark,category_name) values(?,?)");
        QStringList list;
        list << QString("1") << category_str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,destination_db_file,Q_FUNC_INFO);
        prepare_mime_category_display();

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

        prepare_mime_category_display();
    }


    recon_static_functions::app_debug(" - Ends",Q_FUNC_INFO);
}

void recon_configuration_page_mime_types::slot_lineedit_add_category_text_changed(QString text)
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

void recon_configuration_page_mime_types::slot_category_cancel_button_clicked()
{
    dialog_add_category->hide();
}

void recon_configuration_page_mime_types::prepare_extension_display_dialog()
{
    recon_static_functions::debug_intensive(" - Starts " ,Q_FUNC_INFO);

    //-Create Dialog for adding Extension.
    dialog_add_mime_type = new QDialog(this);
    dialog_add_mime_type->setWindowModality(Qt::WindowModal);


    QVBoxLayout *vboxlayout_add_extension = new QVBoxLayout(dialog_add_mime_type);

    lineedit_add_mime_type = new QLineEdit(this);
    lineedit_add_mime_type->setPlaceholderText(QString("MIME Type..."));
    connect(lineedit_add_mime_type,SIGNAL(textChanged(QString)),SLOT(slot_lineedit_add_extension_text_changed(QString)));

    pushbutton_add_extension = new QPushButton(QString("Add"),this);

    pushbutton_add_extension->setFixedWidth(80);

    pushbutton_add_extension->setFont(font_s);
    connect(pushbutton_add_extension,SIGNAL(clicked()),this,SLOT(slot_pushbutton_dialog_add_mime_type_clicked()));

    QPushButton *cancel_button = new QPushButton("Cancel",this);
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(slot_extension_cancel_button_clicked()));
    cancel_button->setFixedWidth(80);


    lineedit_add_mime_type->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_add_extension->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_add_extension->setEnabled(false);

    QHBoxLayout *hbox_add_button_ext_layout = new QHBoxLayout;
    label_message_extension = new QLabel(this);
    label_message_extension->setMinimumWidth(140);
    label_message_extension->setStyleSheet("QLabel{color : red;}");

    hbox_add_button_ext_layout->addWidget(label_message_extension);
    hbox_add_button_ext_layout->addWidget(cancel_button);
    hbox_add_button_ext_layout->addWidget(pushbutton_add_extension);

    vboxlayout_add_extension->addWidget(lineedit_add_mime_type);
    vboxlayout_add_extension->addLayout(hbox_add_button_ext_layout);
    vboxlayout_add_extension->setContentsMargins(9,9,9,6);
    dialog_add_mime_type->setLayout(vboxlayout_add_extension);
    dialog_add_mime_type->setWindowTitle(QString("Add MIME Type"));
    dialog_add_mime_type->setMinimumWidth(200);
    dialog_add_mime_type->setFont(font_s);

    dialog_add_mime_type->setFixedSize(QSize(350, 100));

    recon_static_functions::debug_intensive(" - Ends " ,Q_FUNC_INFO);
}

void recon_configuration_page_mime_types::slot_lineedit_add_extension_text_changed(QString text)
{
    recon_static_functions::debug_intensive(" - Starts",Q_FUNC_INFO);

    text = text.trimmed();

    if(!text.isEmpty())
    {
        pushbutton_add_extension->setEnabled(true);
        lineedit_add_mime_type->setStyleSheet(styleSheet());
        label_message_extension->setStyleSheet(styleSheet());
        label_message_extension->clear();
    }
    else
    {
        pushbutton_add_extension->setEnabled(false);
    }

    recon_static_functions::debug_intensive(" - Ends",Q_FUNC_INFO);
}

void recon_configuration_page_mime_types::slot_pushbutton_dialog_add_mime_type_clicked()
{
    recon_static_functions::app_debug(" - Starts",Q_FUNC_INFO);

    /// extension to be added will be lower case (Manoj Sir)
    QString exten_str = lineedit_add_mime_type->text().trimmed();

    if(exten_str.isEmpty())
    {
        /// warning here for Empty extension
        dialog_add_mime_type->hide();
        return;
    }

    if(ui->tableWidget_category->currentRow() < 0|| ui->tableWidget_category->currentColumn() < 0)
    {
        dialog_add_mime_type->hide();
        return;
    }

    QString category_str = ui->tableWidget_category->currentItem()->text().trimmed();

    /// get existing extension
    QString command_str = QString("Select mime_type From tbl_mime_type Where category_name = ?");

    QStringList values;
    values << category_str;

    QStringList existing_extension = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command_str, values, 0, destination_db_file, Q_FUNC_INFO);

    /// case sensitivity not taken here because MIME type can be both upper case
    /// and lower case. So we support both (abc and ABC both allowed).
    if(!existing_extension.contains(exten_str))
    {
        command_str = QString("insert into tbl_mime_type (bookmark, mime_type , category_name) VALUES(?,?,?)");

        QStringList value_list;
        value_list << QString("0") << exten_str << category_str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);
    }

    if(ui->tableWidget_mime_type->rowCount() > 0)
        ui->tableWidget_mime_type->selectRow(0);

    dialog_add_mime_type->hide();

    prepare_mime_category_display();

    recon_static_functions::app_debug(" - Ends",Q_FUNC_INFO);
}

void recon_configuration_page_mime_types::slot_extension_cancel_button_clicked()
{
    dialog_add_mime_type->hide();
}

void recon_configuration_page_mime_types::slot_new_mime_type_LineEdit_pressed()
{
    QString new_mime_type = new_mime_type_LineEdit->text();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(new_mime_type);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);




    if(new_mime_type.trimmed().isEmpty())
        return;

    if(ui->tableWidget_category->currentRow() < 0)
        return;

    QString category_name = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 0)->text();


    QString command_str = QString("INSERT INTO tbl_mime_type (bookmark,category_name, mime_type) values (?,?,?)");



    QStringList value_list;
    value_list << "0" << category_name << new_mime_type;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    ui->tableWidget_mime_type->removeCellWidget(ui->tableWidget_mime_type->currentRow(), 0);
    ui->tableWidget_mime_type->setItem(ui->tableWidget_mime_type->currentRow(), 0, item);

}

void recon_configuration_page_mime_types::slot_new_category_line_edit_return_pressed()
{
    QString new_category = new_category_LineEdit->text();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(new_category);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);


    if(new_category.trimmed().isEmpty())
        return;

    if(ui->tableWidget_category->currentRow() < 0)
        return;


    QString command_str = QString("INSERT INTO mime_type_category (bookmark,category_name) values (?,?)");



    QStringList value_list;
    value_list << "1" << new_category;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    ui->tableWidget_category->removeCellWidget(ui->tableWidget_category->currentRow(), 0);
    ui->tableWidget_category->setItem(ui->tableWidget_category->currentRow(), 0, item);

}

