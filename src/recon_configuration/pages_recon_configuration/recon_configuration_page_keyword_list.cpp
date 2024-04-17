#include "recon_configuration_page_keyword_list.h"
#include "ui_recon_configuration_page_keyword_list.h"

recon_configuration_page_keyword_list::recon_configuration_page_keyword_list(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration_page_keyword_list)
{
    ui->setupUi(this);

    font_s.setPixelSize(12);

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->pushButton_category_add->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_category_remove->setIcon(QIcon("../icons/signs/remove.png"));
    ui->pushButton_edit_category->setIcon(QIcon("../icons/signs/edit.png"));


    ui->pushButton_keyword_add->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_keyword_remove->setIcon(QIcon("../icons/signs/remove.png"));
    ui->pushButton_keyword_clipboard->setIcon(QIcon("../icons/signs/clipboard.png"));
    ui->pushButton_edit_keyword->setIcon(QIcon("../icons/signs/edit.png"));


    ui->tableWidget_category->setColumnWidth(0, 25);
    ui->tableWidget_keywords->hideColumn(0);
    ui->tableWidget_keywords->setColumnWidth(1, 400);

}

recon_configuration_page_keyword_list::~recon_configuration_page_keyword_list()
{
    delete ui;
}

void recon_configuration_page_keyword_list::pub_set_caller_type(QString caller_type)
{
    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG)
    {
        destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");
   }
    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    {
        destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + QString("case_configuration/case_configuration.sqlite");
    }
    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER)
        destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString("case_configuration.sqlite");

    prepare_category_display();
    prepare_keyword_display();
    prepare_add_category_dialog();
    prepare_add_keyword_dialog();


    prepare_edit_category_dialog();
    prepare_edit_keyword_dialog();

}

void recon_configuration_page_keyword_list::prepare_category_display()
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString recon_config_db_path = destination_db_path;
    QString command_str = "select category, category_status from keyword_category";

    QStringList keyword_category_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str ,0 ,recon_config_db_path ,Q_FUNC_INFO);
    QStringList keyword_category_status_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str ,1 ,recon_config_db_path ,Q_FUNC_INFO);

    ui->tableWidget_category->setRowCount(0);

    for(int row = 0; row < keyword_category_list.size(); row++)
    {
        ui->tableWidget_category->insertRow(row);
        ui->tableWidget_category->setRowHeight(row, 23);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(keyword_category_list.at(row));

        QTableWidgetItem *item_status = new QTableWidgetItem;
        item_status->setCheckState(Qt::Unchecked);

        if(keyword_category_status_list.at(row) == "1")
            item_status->setCheckState(Qt::Checked);

        ui->tableWidget_category->setItem(row , 0 , item_status);
        ui->tableWidget_category->setItem(row , 1 , item);
    }

    if(ui->tableWidget_category->rowCount() > 0)
        ui->tableWidget_category->selectRow(0);

    //prepare_keyword_display();

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}


void recon_configuration_page_keyword_list::prepare_keyword_display()
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    int current_row = ui->tableWidget_category->currentRow();

    if(current_row < 0)
        return;

    if(ui->tableWidget_category->item(current_row, 1) == NULL)
        return;

    QString category = ui->tableWidget_category->item(current_row, 1)->text().trimmed();

    if(category.isEmpty())
        return;

    QString recon_config_db_path = destination_db_path;
    QString command_str = "select keyword, keyword_status, is_regex from keyword_list Where category = ?";
    QStringList value_list;
    value_list << category;

    ui->tableWidget_keywords->setRowCount(0);

    QStringList keyword_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command_str,value_list ,0 ,recon_config_db_path ,Q_FUNC_INFO);
    QStringList keyword_list_status = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command_str,value_list ,1 ,recon_config_db_path ,Q_FUNC_INFO);
    QStringList regex_list_status  = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command_str,value_list ,2 ,recon_config_db_path ,Q_FUNC_INFO);


    for(int row = 0; row < keyword_list.size(); row++)
    {
        ui->tableWidget_keywords->insertRow(row);
        ui->tableWidget_keywords->setRowHeight(row, 23);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(keyword_list.at(row));

        QTableWidgetItem *item_status = new QTableWidgetItem;
        item_status->setCheckState(Qt::Unchecked);

        if(keyword_list_status.at(row) == "1")
            item_status->setCheckState(Qt::Checked);

        QTableWidgetItem *item_regex = new QTableWidgetItem;
        item_regex->setCheckState(Qt::Unchecked);
        if(regex_list_status.at(row) == "1")
            item_regex->setCheckState(Qt::Checked);

        ui->tableWidget_keywords->setItem(row , 0 , item_status);
        ui->tableWidget_keywords->setItem(row , 1 , item);
        ui->tableWidget_keywords->setItem(row , 2 , item_regex);
    }

    if(ui->tableWidget_keywords->rowCount() > 0)
        ui->tableWidget_keywords->selectRow(0);

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void recon_configuration_page_keyword_list::on_tableWidget_category_cellClicked(int row, int column)
{
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

        QString cmd = "update keyword_category set category_status = ? where category = ?";

        QStringList args;
        args << category_status << category;

        QString destination_db_file = destination_db_path;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd, args, destination_db_file, Q_FUNC_INFO);

    }

    prepare_keyword_display();
}


void recon_configuration_page_keyword_list::prepare_add_category_dialog()
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

void recon_configuration_page_keyword_list::prepare_add_keyword_dialog()
{
    recon_static_functions::debug_intensive(" - Starts " ,Q_FUNC_INFO);

    //-Create Dialog for adding Catagories.
    dialog_add_keyword = new QDialog(this);
    dialog_add_keyword->setWindowModality(Qt::WindowModal);


    QVBoxLayout *vboxlayout_add_keyword = new QVBoxLayout(dialog_add_keyword);

    lineedit_add_keyword = new QLineEdit(this);
    lineedit_add_keyword->setPlaceholderText(QString("Type Keyword / Regular Expression..."));

    connect(lineedit_add_keyword,SIGNAL(textChanged(QString)),SLOT(slot_lineedit_add_keyword_text_changed(QString)));

    pushbutton_add_keyword = new QPushButton(QString("Add"),this);
    pushbutton_add_keyword->setFixedWidth(80);
    pushbutton_add_keyword->setFont(font_s);
    connect(pushbutton_add_keyword,SIGNAL(clicked()),this,SLOT(slot_pushbutton_dialog_add_keyword_clicked()));
    pushbutton_add_keyword->setEnabled(false);

    lineedit_add_keyword->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_add_keyword->setAttribute(Qt::WA_MacShowFocusRect,false);

    QHBoxLayout *hbox_add_button_keyword_layout = new QHBoxLayout;
    label_message_keyword = new QLabel(this);
    label_message_keyword->setMinimumWidth(140);
    label_message_keyword->setStyleSheet("QLabel{color : red;}");

    QPushButton *cancel_button = new QPushButton("Cancel",this);
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(slot_keyword_cancel_button_clicked()));
    cancel_button->setFixedWidth(80);

    hbox_add_button_keyword_layout->addWidget(label_message_keyword);
    hbox_add_button_keyword_layout->addWidget(cancel_button);
    hbox_add_button_keyword_layout->addWidget(pushbutton_add_keyword);

    vboxlayout_add_keyword->addWidget(lineedit_add_keyword);
    vboxlayout_add_keyword->addLayout(hbox_add_button_keyword_layout);
    vboxlayout_add_keyword->setContentsMargins(9,9,9,6);

    dialog_add_keyword->setLayout(vboxlayout_add_keyword);
    dialog_add_keyword->setWindowTitle(QString("Add keyword"));
    dialog_add_keyword->setMinimumWidth(200);
    dialog_add_keyword->setFont(font_s);

    dialog_add_keyword->setFixedSize(QSize(350, 100));

    recon_static_functions::debug_intensive(" - Ends " ,Q_FUNC_INFO);

}

void recon_configuration_page_keyword_list::on_pushButton_category_add_clicked()
{
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

    ui->tableWidget_keywords->setRowCount(0);

}

void recon_configuration_page_keyword_list::on_pushButton_category_remove_clicked()
{

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

    QString command_str_extension = QString("DELETE FROM keyword_list where category=?");

    QString command_str_category = QString("DELETE FROM keyword_category where category=?");



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
        ui->tableWidget_keywords->setRowCount(0);

}


void recon_configuration_page_keyword_list::on_pushButton_keyword_add_clicked()
{
    int current_category_row_number = ui->tableWidget_category->currentRow();

    if(current_category_row_number < 0)
        return;

    if(ui->tableWidget_category->item(current_category_row_number, 1) == NULL)
        return;

    //    if(ui->tableWidget_category->item(current_category_row_number, 1)->text() == QString(""))
    //        return;

    int row_count = ui->tableWidget_keywords->rowCount();

    if(ui->tableWidget_keywords->item(row_count - 1, 1) == NULL && row_count > 0)
        return;




    if(ui->tableWidget_category->currentRow() < 0)
        return;

    ui->tableWidget_keywords->insertRow(row_count);
    ui->tableWidget_keywords->setRowHeight(row_count, 23);

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setCheckState(Qt::Checked);
    ui->tableWidget_keywords->setItem(row_count, 0, item);

    QTableWidgetItem *item_regex = new QTableWidgetItem;
    item_regex->setCheckState(Qt::Unchecked);
    ui->tableWidget_keywords->setItem(row_count, 2, item_regex);


    new_keyword_LineEdit = new QLineEdit(this);
    new_keyword_LineEdit->setAttribute(Qt::WA_MacShowFocusRect,false);
    connect(new_keyword_LineEdit, SIGNAL(returnPressed()), this, SLOT(slot_line_edit_keyword_return_pressed()));

    ui->tableWidget_keywords->setCellWidget(row_count, 1, new_keyword_LineEdit);
    ui->tableWidget_keywords->setCurrentCell(row_count, 1);

}

void recon_configuration_page_keyword_list::on_pushButton_keyword_remove_clicked()
{

    int current_row = ui->tableWidget_keywords->currentRow();

    if(current_row < 0)
        return;

    if(ui->tableWidget_keywords->item(current_row, 1) == NULL)
    {
        ui->tableWidget_keywords->removeRow(current_row);
        ui->tableWidget_keywords->setCurrentCell( -1, 1);
        return;
    }

    QString destination_db_file = destination_db_path;

    QString keyword_name = ui->tableWidget_keywords->item(current_row, 1)->text().trimmed();

    if(keyword_name.isEmpty())
        return;

    if(ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1) == NULL)
        return;

    QString category_name = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1)->text().trimmed();

    if(category_name.isEmpty())
        return;


    QString command_str = QString("DELETE FROM keyword_list where keyword=? AND category=?");

    QStringList value_list;
    value_list << keyword_name << category_name;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    ui->tableWidget_keywords->removeRow(current_row);

    if(ui->tableWidget_keywords->rowCount() - 1  < current_row)
        ui->tableWidget_keywords->setCurrentCell(ui->tableWidget_keywords->rowCount() - 1, 1);
    else
        ui->tableWidget_keywords->setCurrentCell(current_row, 1);
}


void recon_configuration_page_keyword_list::slot_lineedit_add_category_text_changed(QString text)
{
    recon_static_functions::app_debug("- Starts",Q_FUNC_INFO);

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

    recon_static_functions::app_debug("- Ends",Q_FUNC_INFO);

}

void recon_configuration_page_keyword_list::slot_pushbutton_dialog_add_category_clicked()
{
    recon_static_functions::app_debug("- Starts",Q_FUNC_INFO);

    dialog_add_category->hide();

    QString category_str = lineedit_add_category->text().trimmed();
    if(category_str.isEmpty())
        return;

    /// check for category already exist(to avoid same entry)
    QString destination_db_file = destination_db_path;

    QString command = QString("Select category from keyword_category");
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
        command = QString("insert into keyword_category(category_status,category) values(?,?)");
        QStringList list;
        list << QString("1") << category_str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,destination_db_file,Q_FUNC_INFO);

        prepare_category_display();
        prepare_keyword_display();
    }
    else
    {
        /// set item selected if already exists
        for(int row = 0; row < ui->tableWidget_category->rowCount(); row++)
        {
            if(ui->tableWidget_category->item(row , 1)->text().trimmed().toLower() == category_str.trimmed().toLower())
            {
                ui->tableWidget_category->selectRow(row);
                break;
            }
        }
    }


    recon_static_functions::app_debug("- Ends",Q_FUNC_INFO);
}

void recon_configuration_page_keyword_list::slot_category_cancel_button_clicked()
{
    dialog_add_category->hide();
}

void recon_configuration_page_keyword_list::slot_lineedit_add_keyword_text_changed(QString text)
{
    recon_static_functions::app_debug("- Starts",Q_FUNC_INFO);

    text = text.trimmed();

    if(!text.isEmpty())
    {
        pushbutton_add_keyword->setEnabled(true);
        lineedit_add_keyword->setStyleSheet(styleSheet());
    }
    else
    {
        pushbutton_add_keyword->setEnabled(false);
    }

    recon_static_functions::app_debug("- Ends",Q_FUNC_INFO);

}

void recon_configuration_page_keyword_list::slot_pushbutton_dialog_add_keyword_clicked()
{
    recon_static_functions::app_debug("- Starts",Q_FUNC_INFO);

    dialog_add_keyword->hide();

    QString keyword_str = lineedit_add_keyword->text().trimmed();
    if(keyword_str.isEmpty())
        return;

    if(ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1) == NULL)
        return;

    QString category_str = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1)->text();
    if(category_str.isEmpty())
        return;


    /// check for keyword already exist(to avoid same entry)
    QString destination_db_file = destination_db_path;

    QString command = QString("Select keyword from keyword_list Where category = ?");
    QStringList value_list;
    value_list << category_str;
    QStringList existing_keyword_in_category_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, value_list, 0, destination_db_file, Q_FUNC_INFO);

    bool bool_keyword_already_exist = false;

    for(int count = 0; count < existing_keyword_in_category_list.size(); count++)
    {
        if(keyword_str.trimmed().toLower() == existing_keyword_in_category_list.at(count).trimmed().toLower())
        {
            bool_keyword_already_exist =  true;
            break;
        }
    }

    if(!bool_keyword_already_exist)
    {
        command = QString("insert into keyword_list(keyword_status,category,keyword) values(?,?,?)");
        QStringList list;
        list << QString("1") << category_str << keyword_str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,destination_db_file,Q_FUNC_INFO);
        prepare_category_display();
        prepare_keyword_display();
    }
    else
    {
        /// set item selected if already exists
        for(int row = 0; row < ui->tableWidget_keywords->rowCount(); row++)
        {
            if(ui->tableWidget_keywords->item(row , 1)->text().trimmed().toLower() == keyword_str.trimmed().toLower())
            {
                ui->tableWidget_keywords->selectRow(row);
                break;
            }
        }
    }

    recon_static_functions::app_debug("- Ends",Q_FUNC_INFO);
}

void recon_configuration_page_keyword_list::slot_keyword_cancel_button_clicked()
{
    dialog_add_keyword->hide();
}

void recon_configuration_page_keyword_list::slot_line_edit_keyword_return_pressed()
{
    QString new_keyword = new_keyword_LineEdit->text().trimmed();

    if(new_keyword.trimmed().isEmpty())
        return;


    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(new_keyword);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);

    if(ui->tableWidget_category->currentRow() < 0)
        return;


    if(ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1) == NULL)
        return;

    QString destination_db_file = destination_db_path;

    QString category_name = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1)->text().trimmed();
    if(category_name.isEmpty())
        return;

    QString command_str = QString("INSERT INTO keyword_list (keyword_status, category, keyword) values (?,?,?)");

    QStringList value_list;
    value_list << QString::number(1) << category_name << new_keyword;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    ui->tableWidget_keywords->removeCellWidget(ui->tableWidget_keywords->currentRow(), 1);
    ui->tableWidget_keywords->setItem(ui->tableWidget_keywords->currentRow(), 1, item);

}

void recon_configuration_page_keyword_list::slot_new_category_line_edit_return_pressed()
{
    QString new_category = new_category_LineEdit->text().trimmed();
    if(new_category.isEmpty())
        return;


    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(new_category);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);


    if(ui->tableWidget_category->currentRow() < 0)
        return;

    QString destination_db_file = destination_db_path;

    QString command_str = QString("INSERT INTO keyword_category (category_status, category) values (?,?)");

    QStringList value_list;
    value_list << "1" << new_category ;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    ui->tableWidget_category->removeCellWidget(ui->tableWidget_category->currentRow(), 1);
    ui->tableWidget_category->setItem(ui->tableWidget_category->currentRow(), 1, item);

}


void recon_configuration_page_keyword_list::on_pushButton_keyword_clipboard_clicked()
{
    if(ui->tableWidget_category->currentRow() < 0)
        return;

    if(ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1) == NULL)
        return;

    QString category = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1)->text().trimmed();

    if(category.isEmpty())
        return;

    QString clipboard_data =  QApplication::clipboard()->text();

    QStringList data_list = clipboard_data.split("\n", Qt::SkipEmptyParts);

    if(data_list.size() > 0)
    {
        QString destination_db_file = destination_db_path;

        for(int i = 0 ; i < data_list.size(); i++)
        {
            QString command_str = QString("INSERT INTO keyword_list (keyword_status, category, keyword, is_regex) values (?,?,?,0)");

            QStringList value_list;
            value_list << QString::number(1) << category << data_list.at(i);

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);
        }
    }

    prepare_keyword_display();

}

void recon_configuration_page_keyword_list::on_tableWidget_keywords_cellClicked(int row, int column)
{
    if(row < 0)
        return;

    if(column != 2)
        return;

    QTableWidgetItem *item = ui->tableWidget_keywords->item(row, column);

    if(item == NULL)
        return;

    if(ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1) == NULL)
        return;

    QString category_name = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1)->text().trimmed();

    if(ui->tableWidget_keywords->item(row, 1) == NULL)
        return;

    QString keyword_name = ui->tableWidget_keywords->item(row, 1)->text().trimmed();

    if(category_name.isEmpty() || keyword_name.isEmpty())
        return;

    QString regex_value = QString::number(0);

    if(item->checkState() == Qt::Checked)
        regex_value = QString::number(1);

    QString command_str = QString("update keyword_list set is_regex = ?  where category = ? AND  keyword = ? ");

    QStringList value_list;
    value_list << regex_value << category_name << keyword_name;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_path ,Q_FUNC_INFO);

}

void recon_configuration_page_keyword_list::on_pushButton_edit_category_clicked()
{
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
}

void recon_configuration_page_keyword_list::prepare_edit_category_dialog()
{
    recon_static_functions::debug_intensive(" - Starts " ,Q_FUNC_INFO);

    //-Create Dialog for editing Catagories.
    dialog_edit_category = new QDialog(this);
    dialog_edit_category->setWindowModality(Qt::WindowModal);


    QVBoxLayout *vboxlayout_edit_category = new QVBoxLayout(dialog_edit_category);

    lineedit_edit_category = new QLineEdit(this);
    lineedit_edit_category->setPlaceholderText(QString("Type Category..."));

    connect(lineedit_edit_category,SIGNAL(textChanged(QString)),SLOT(slot_lineedit_edit_category_text_changed(QString)));

    pushbutton_edit_category = new QPushButton(QString("Add"),this);
    pushbutton_edit_category->setFixedWidth(80);
    pushbutton_edit_category->setFont(font_s);
    connect(pushbutton_edit_category,SIGNAL(clicked()),this,SLOT(slot_pushbutton_dialog_edit_add_category_clicked()));
    pushbutton_edit_category->setEnabled(false);

    lineedit_edit_category->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_edit_category->setAttribute(Qt::WA_MacShowFocusRect,false);

    QHBoxLayout *hbox_add_button_category_layout = new QHBoxLayout;
    label_message_edit_category = new QLabel(this);
    label_message_edit_category->setMinimumWidth(140);
    label_message_edit_category->setStyleSheet("QLabel{color : red;}");

    QPushButton *cancel_button = new QPushButton("Cancel",this);
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(slot_edit_category_cancel_button_clicked()));
    cancel_button->setFixedWidth(80);

    hbox_add_button_category_layout->addWidget(label_message_edit_category);
    hbox_add_button_category_layout->addWidget(cancel_button);
    hbox_add_button_category_layout->addWidget(pushbutton_edit_category);

    vboxlayout_edit_category->addWidget(lineedit_edit_category);
    vboxlayout_edit_category->addLayout(hbox_add_button_category_layout);
    vboxlayout_edit_category->setContentsMargins(9,9,9,6);

    dialog_edit_category->setLayout(vboxlayout_edit_category);
    dialog_edit_category->setWindowTitle(QString("Edit category"));
    dialog_edit_category->setMinimumWidth(200);
    dialog_edit_category->setFont(font_s);

    dialog_edit_category->setFixedSize(QSize(350, 100));

    recon_static_functions::debug_intensive(" - Ends " ,Q_FUNC_INFO);

}

void recon_configuration_page_keyword_list::slot_pushbutton_dialog_edit_add_category_clicked()
{
    recon_static_functions::app_debug("- Starts",Q_FUNC_INFO);

    dialog_edit_category->hide();

    QString category_str = lineedit_edit_category->text().trimmed();
    if(category_str.isEmpty() || category_to_update_str.trimmed().isEmpty())
        return;

    QString destination_db_file = destination_db_path;

    QString command = QString("Update keyword_category SET category = ? Where category = ?");
    QStringList list;
    list <<  category_str << category_to_update_str;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,destination_db_file,Q_FUNC_INFO);

    command = QString("Update keyword_list SET category = ? Where category = ?");
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,destination_db_file,Q_FUNC_INFO);

    int current_row = ui->tableWidget_category->currentRow();

    prepare_category_display();

    ui->tableWidget_category->selectRow(current_row);

    recon_static_functions::app_debug("- Ends",Q_FUNC_INFO);
}

void recon_configuration_page_keyword_list::slot_lineedit_edit_category_text_changed(QString text)
{
    recon_static_functions::app_debug("- Starts",Q_FUNC_INFO);

    text = text.trimmed();

    if(!text.isEmpty())
    {
        pushbutton_edit_category->setEnabled(true);
        lineedit_edit_category->setStyleSheet(styleSheet());
    }
    else
    {
        pushbutton_edit_category->setEnabled(false);
    }

    recon_static_functions::app_debug("- Ends",Q_FUNC_INFO);

}

void recon_configuration_page_keyword_list::slot_edit_category_cancel_button_clicked()
{
    dialog_edit_category->hide();
}

void recon_configuration_page_keyword_list::on_pushButton_edit_keyword_clicked()
{
    int current_category_row_number = ui->tableWidget_category->currentRow();
    if(current_category_row_number < 0)
        return;

    if(ui->tableWidget_category->item(current_category_row_number, 1) == NULL)
        return;


    int row_count = ui->tableWidget_keywords->rowCount();
    if(ui->tableWidget_keywords->item(row_count - 1, 1) == NULL && row_count > 0)
        return;

    lineedit_edit_keyword->clear();

    int current_row = ui->tableWidget_keywords->currentRow();
    int current_col = ui->tableWidget_keywords->currentColumn();

    if(current_row < 0 || current_col < 0)
        return;

    keyword_to_update_str = ui->tableWidget_keywords->item(current_row, current_col)->text().trimmed();

    if(keyword_to_update_str.isEmpty())
        return;

    lineedit_edit_keyword->setText(keyword_to_update_str);

    dialog_edit_keyword->show();
}

void recon_configuration_page_keyword_list::prepare_edit_keyword_dialog()
{
    recon_static_functions::debug_intensive(" - Starts " ,Q_FUNC_INFO);

    //-Create Dialog for edit keyword.
    dialog_edit_keyword = new QDialog(this);
    dialog_edit_keyword->setWindowModality(Qt::WindowModal);


    QVBoxLayout *vboxlayout_edit_keyword = new QVBoxLayout(dialog_edit_keyword);

    lineedit_edit_keyword = new QLineEdit(this);
    lineedit_edit_keyword->setPlaceholderText(QString("Keyword..."));

    connect(lineedit_edit_keyword,SIGNAL(textChanged(QString)),SLOT(slot_lineedit_edit_keyword_text_changed(QString)));

    pushbutton_edit_keyword = new QPushButton(QString("Add"),this);
    pushbutton_edit_keyword->setFixedWidth(80);
    pushbutton_edit_keyword->setFont(font_s);
    connect(pushbutton_edit_keyword,SIGNAL(clicked()),this,SLOT(slot_pushbutton_dialog_edit_keyword_add_clicked()));
    pushbutton_edit_keyword->setEnabled(false);

    lineedit_edit_keyword->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_edit_keyword->setAttribute(Qt::WA_MacShowFocusRect,false);

    QHBoxLayout *hbox_add_button_keyword_layout = new QHBoxLayout;
    label_message_edit_keyword = new QLabel(this);
    label_message_edit_keyword->setMinimumWidth(140);
    label_message_edit_keyword->setStyleSheet("QLabel{color : red;}");

    QPushButton *cancel_button = new QPushButton("Cancel",this);
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(slot_edit_keyword_cancel_button_clicked()));
    cancel_button->setFixedWidth(80);

    hbox_add_button_keyword_layout->addWidget(label_message_edit_keyword);
    hbox_add_button_keyword_layout->addWidget(cancel_button);
    hbox_add_button_keyword_layout->addWidget(pushbutton_edit_keyword);

    vboxlayout_edit_keyword->addWidget(lineedit_edit_keyword);
    vboxlayout_edit_keyword->addLayout(hbox_add_button_keyword_layout);
    vboxlayout_edit_keyword->setContentsMargins(9,9,9,6);

    dialog_edit_keyword->setLayout(vboxlayout_edit_keyword);
    dialog_edit_keyword->setWindowTitle(QString("Edit Keyword"));
    dialog_edit_keyword->setMinimumWidth(200);
    dialog_edit_keyword->setFont(font_s);

    dialog_edit_keyword->setFixedSize(QSize(350, 100));

    recon_static_functions::debug_intensive(" - Ends " ,Q_FUNC_INFO);

}

void recon_configuration_page_keyword_list::slot_lineedit_edit_keyword_text_changed(QString text)
{
    recon_static_functions::app_debug("- Starts",Q_FUNC_INFO);

    text = text.trimmed();

    if(!text.isEmpty())
    {
        pushbutton_edit_keyword->setEnabled(true);
        lineedit_edit_keyword->setStyleSheet(styleSheet());
    }
    else
    {
        pushbutton_edit_keyword->setEnabled(false);
    }

    recon_static_functions::app_debug("- Ends",Q_FUNC_INFO);

}

void recon_configuration_page_keyword_list::slot_pushbutton_dialog_edit_keyword_add_clicked()
{
    recon_static_functions::app_debug("- Starts",Q_FUNC_INFO);

    dialog_edit_keyword->hide();

    QString keyword_str = lineedit_edit_keyword->text().trimmed();
    if(keyword_str.isEmpty() || keyword_to_update_str.trimmed().isEmpty())
        return;

    if(ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1) == NULL)
        return;

    QString category_str = ui->tableWidget_category->item(ui->tableWidget_category->currentRow(), 1)->text();
    if(category_str.isEmpty())
        return;

    QString destination_db_file = destination_db_path;

    QString command = QString("Update keyword_list SET keyword = ? Where category = ? AND keyword = ?");
    QStringList list;
    list << keyword_str <<  category_str << keyword_to_update_str;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,destination_db_file,Q_FUNC_INFO);

    int current_row_category = ui->tableWidget_category->currentRow();
    int current_row_keyword  = ui->tableWidget_keywords->currentRow();

    prepare_keyword_display();

    ui->tableWidget_category->selectRow(current_row_category);
    ui->tableWidget_keywords->selectRow(current_row_keyword);

    recon_static_functions::app_debug("- Ends",Q_FUNC_INFO);
}

void recon_configuration_page_keyword_list::slot_edit_keyword_cancel_button_clicked()
{
    dialog_edit_keyword->hide();
}

