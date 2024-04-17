#include "recon_configuration_page_file_size.h"
#include "ui_recon_configuration_page_file_size.h"

recon_configuration_page_file_size::recon_configuration_page_file_size(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration_page_file_size)
{
    ui->setupUi(this);

    ui->pushButton_add_label->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_remove_label->setIcon(QIcon("../icons/signs/remove.png"));

    recon_helper_standard_obj = new recon_helper_standard(this);


    font_s.setPixelSize(12);
    prepare_add_filters_display_dialog();
}

recon_configuration_page_file_size::~recon_configuration_page_file_size()
{
    delete ui;
}

void recon_configuration_page_file_size::pub_set_essentials()
{
    destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");

    prepare_file_size_filters_display();
}

void recon_configuration_page_file_size::prepare_file_size_filters_display()
{
    QString command_str = QString("Select filter_name From file_size_filters");
    QStringList category_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str , 0 , destination_db_file , Q_FUNC_INFO);

    ui->tableWidget_filter_label->setRowCount(0);

    for(int row = 0; row < category_list.size(); row++)
    {
        QString category_str = category_list.at(row);

        ui->tableWidget_filter_label->insertRow(row);
        QTableWidgetItem *item_category = new QTableWidgetItem;
        item_category->setText(category_str);

        ui->tableWidget_filter_label->setItem(row , 0 , item_category);
    }

    if(ui->tableWidget_filter_label->rowCount() > 0)
        ui->tableWidget_filter_label->selectRow(0);

    prepare_file_size_filters_details_display();

}


void recon_configuration_page_file_size::prepare_file_size_filters_details_display()
{
    int current_row =  ui->tableWidget_filter_label->currentRow();

    if(current_row < 0)
        return;

    QString filter_name_str = ui->tableWidget_filter_label->currentItem()->text();

    QString command_str = QString("Select greater_than_size, greater_than_size_unit, less_than_size, less_than_size_unit, start_size, start_size_unit, end_size, end_size_unit From file_size_filters Where filter_name = ?");

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    query.prepare(command_str);
    query.addBindValue(filter_name_str);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------db file - " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("command -  " + command_str,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    ui->tableWidget_filter_details->setRowCount(0);

    int row = 0;
    while(query.next())
    {

        QString greater_than_size_str  = query.value(0).toString();
        QString greater_than_size_unit_str  = query.value(1).toString();

        QString less_than_size_str = query.value(2).toString();
        QString less_than_size_unit_str = query.value(3).toString();

        QString start_size_str = query.value(4).toString();
        QString start_size_unit_str = query.value(5).toString();

        QString end_size_str = query.value(6).toString();
        QString end_size_unit_str = query.value(7).toString();

        if(!greater_than_size_str.isEmpty())
        {
            ui->tableWidget_filter_details->insertRow(row);

            QTableWidgetItem *item_file_size_start_title = new QTableWidgetItem;
            item_file_size_start_title->setText(QString("Greater than"));
            ui->tableWidget_filter_details->setItem(row , 0 , item_file_size_start_title);

            QTableWidgetItem *item_file_size_start_value = new QTableWidgetItem;
            item_file_size_start_value->setText(greater_than_size_str);
            ui->tableWidget_filter_details->setItem(row , 1 , item_file_size_start_value);


            QTableWidgetItem *item_file_size_start_unit = new QTableWidgetItem;
            item_file_size_start_unit->setText(greater_than_size_unit_str);
            ui->tableWidget_filter_details->setItem(row , 2 , item_file_size_start_unit);

            row++;

        }

        if(!less_than_size_str.isEmpty())
        {
            ui->tableWidget_filter_details->insertRow(row);

            QTableWidgetItem *item_file_size_end_title = new QTableWidgetItem;
            item_file_size_end_title->setText(QString("Less than"));
            ui->tableWidget_filter_details->setItem(row , 0 , item_file_size_end_title);

            QTableWidgetItem *item_file_size_end_value = new QTableWidgetItem;
            item_file_size_end_value->setText(less_than_size_str);
            ui->tableWidget_filter_details->setItem(row , 1 , item_file_size_end_value);


            QTableWidgetItem *item_file_size_end_unit = new QTableWidgetItem;
            item_file_size_end_unit->setText(less_than_size_unit_str);
            ui->tableWidget_filter_details->setItem(row , 2 , item_file_size_end_unit);
            row++;

        }

        if(!start_size_str.isEmpty() && !end_size_str.isEmpty())
        {
            ui->tableWidget_filter_details->insertRow(row);

            QTableWidgetItem *item_file_size_equal_to_title = new QTableWidgetItem;
            item_file_size_equal_to_title->setText(QString("Between"));
            ui->tableWidget_filter_details->setItem(row , 0 , item_file_size_equal_to_title);

            QTableWidgetItem *item_file_size_equal_to_value = new QTableWidgetItem;
            item_file_size_equal_to_value->setText(start_size_str + QString(",") + end_size_str);
            ui->tableWidget_filter_details->setItem(row , 1 , item_file_size_equal_to_value);


            QTableWidgetItem *item_file_size_equal_to_unit = new QTableWidgetItem;
            item_file_size_equal_to_unit->setText(start_size_unit_str + QString(",") + end_size_unit_str);
            ui->tableWidget_filter_details->setItem(row , 2 , item_file_size_equal_to_unit);
            row++;
        }
    }

    destination_db.close();

    if(ui->tableWidget_filter_details->rowCount() > 0)
        ui->tableWidget_filter_details->selectRow(0);

}

void recon_configuration_page_file_size::on_tableWidget_filter_label_cellClicked(int row, int column)
{
    if(row < 0 || column < 0)
        return;


    prepare_file_size_filters_details_display();
}



void recon_configuration_page_file_size::on_pushButton_add_label_clicked()
{
    lineedit_add_filters->clear();

    lineedit_greater_than_size->clear();
    lineedit_less_than_size->clear();
    lineedit_beteen_start_size->clear();
    lineedit_beteen_end_size->clear();

    combobox_start_size_unit->setCurrentIndex(0);
    combobox_end_size_unit->setCurrentIndex(0);
    combobox_between_start_size_unit->setCurrentIndex(0);
    combobox_between_end_size_unit->setCurrentIndex(0);


    radiobutton_greater_than->setAutoExclusive(false);
    radiobutton_greater_than->setChecked(false);
    radiobutton_greater_than->setAutoExclusive(true);

    radiobutton_less_than->setAutoExclusive(false);
    radiobutton_less_than->setChecked(false);
    radiobutton_less_than->setAutoExclusive(true);

    radiobutton_between->setAutoExclusive(false);
    radiobutton_between->setChecked(false);
    radiobutton_between->setAutoExclusive(true);

    dialog_add_filter->show();
}

void recon_configuration_page_file_size::on_pushButton_remove_label_clicked()
{
    int category_current_row = ui->tableWidget_filter_label->currentRow();

    if(category_current_row < 0)
    {
        ui->tableWidget_filter_details->setRowCount(0);
        return;
    }

    QString selected_extn_str = ui->tableWidget_filter_label->currentItem()->text();

    QStringList value_list;
    QString command_str = QString("Delete From file_size_filters Where filter_name = ?");
    value_list << selected_extn_str;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list , destination_db_file , Q_FUNC_INFO);

    prepare_file_size_filters_display();
}

void recon_configuration_page_file_size::prepare_add_filters_display_dialog()
{
    recon_static_functions::debug_intensive(" - Starts " ,Q_FUNC_INFO);

    //-Create Dialog for adding Catagories.
    dialog_add_filter = new QDialog(this);
    dialog_add_filter->setWindowModality(Qt::WindowModal);

    QVBoxLayout *vboxlayout_add_category = new QVBoxLayout(dialog_add_filter);

    lineedit_add_filters = new QLineEdit(this);
    lineedit_add_filters->setPlaceholderText(QString("Filter Title..."));



    radiobutton_greater_than = new QRadioButton("Greater than", this);
    radiobutton_less_than    = new QRadioButton("Less than", this);
    radiobutton_between      = new QRadioButton("Between", this);

    lineedit_greater_than_size = new QLineEdit(this);
    lineedit_less_than_size   = new QLineEdit(this);
    lineedit_beteen_start_size = new QLineEdit(this);
    lineedit_beteen_end_size = new QLineEdit(this);

    lineedit_greater_than_size->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_less_than_size->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_beteen_start_size->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_beteen_end_size->setAttribute(Qt::WA_MacShowFocusRect,false);

    radiobutton_greater_than->setAttribute(Qt::WA_MacShowFocusRect,false);
    radiobutton_less_than->setAttribute(Qt::WA_MacShowFocusRect,false);
    radiobutton_between->setAttribute(Qt::WA_MacShowFocusRect,false);


    combobox_start_size_unit = new QComboBox();
    combobox_end_size_unit   = new QComboBox();
    combobox_between_start_size_unit = new QComboBox();
    combobox_between_end_size_unit   = new QComboBox();

    QStringList list_units;
    list_units << QString(MACRO_Generic_Filter_Condition_Bytes)
               << QString(MACRO_Generic_Filter_Condition_KB)
               << QString(MACRO_Generic_Filter_Condition_MB)
               << QString(MACRO_Generic_Filter_Condition_GB)
               << QString(MACRO_Generic_Filter_Condition_TB);

    combobox_start_size_unit->addItems(list_units);
    combobox_end_size_unit->addItems(list_units);
    combobox_between_start_size_unit->addItems(list_units);
    combobox_between_end_size_unit->addItems(list_units);

    QSpacerItem *spacer_less_than = new QSpacerItem(17, QSizePolicy::Fixed, QSizePolicy::Fixed);
    QHBoxLayout *hbox_start_size = new QHBoxLayout;
    hbox_start_size->addWidget(radiobutton_greater_than);
    hbox_start_size->addWidget(lineedit_greater_than_size);
    hbox_start_size->addWidget(combobox_start_size_unit);

    QHBoxLayout *hbox_end_size = new QHBoxLayout;
    hbox_end_size->addWidget(radiobutton_less_than);
    hbox_end_size->addSpacerItem(spacer_less_than);
    hbox_end_size->addWidget(lineedit_less_than_size);
    hbox_end_size->addWidget(combobox_end_size_unit);

    QHBoxLayout *hbox_between_size = new QHBoxLayout;
    hbox_between_size->addWidget(radiobutton_between);
    hbox_between_size->addWidget(lineedit_beteen_start_size);
    hbox_between_size->addWidget(combobox_between_start_size_unit);
    hbox_between_size->addWidget(lineedit_beteen_end_size);
    hbox_between_size->addWidget(combobox_between_end_size_unit);

    connect(lineedit_add_filters,SIGNAL(textChanged(QString)),SLOT(slot_lineedit_add_category_text_changed(QString)));

    pushbutton_add_filters = new QPushButton(QString("Add"),this);
    pushbutton_add_filters->setFixedWidth(80);
    pushbutton_add_filters->setFont(font_s);
    connect(pushbutton_add_filters,SIGNAL(clicked()),this,SLOT(slot_pushbutton_dialog_add_category_clicked()));
    pushbutton_add_filters->setEnabled(false);

    lineedit_add_filters->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_add_filters->setAttribute(Qt::WA_MacShowFocusRect,false);

    QHBoxLayout *hbox_add_button_category_layout = new QHBoxLayout;
    label_message_filters = new QLabel(this);
    label_message_filters->setMinimumWidth(140);
    label_message_filters->setStyleSheet("QLabel{color : red;}");

    QPushButton *cancel_button = new QPushButton("Cancel",this);
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(slot_category_cancel_button_clicked()));
    cancel_button->setFixedWidth(80);

    hbox_add_button_category_layout->addWidget(label_message_filters);
    hbox_add_button_category_layout->addWidget(cancel_button);
    hbox_add_button_category_layout->addWidget(pushbutton_add_filters);
    vboxlayout_add_category->addWidget(lineedit_add_filters);
    vboxlayout_add_category->addLayout(hbox_start_size);
    vboxlayout_add_category->addLayout(hbox_end_size);
    vboxlayout_add_category->addLayout(hbox_between_size);
    vboxlayout_add_category->addLayout(hbox_add_button_category_layout);
    vboxlayout_add_category->setContentsMargins(9,9,9,6);

    dialog_add_filter->setLayout(vboxlayout_add_category);
    dialog_add_filter->setWindowTitle(QString("Add category"));
    dialog_add_filter->setMinimumWidth(200);
    dialog_add_filter->setFont(font_s);

    dialog_add_filter->setFixedSize(QSize(500, 200));

    recon_static_functions::debug_intensive(" - Ends " ,Q_FUNC_INFO);

}

void recon_configuration_page_file_size::slot_pushbutton_dialog_add_category_clicked()
{
    recon_static_functions::app_debug(" - Starts",Q_FUNC_INFO);

    QString filter_str = lineedit_add_filters->text().trimmed();

    if(filter_str.isEmpty())
    {
        return;
    }

    dialog_add_filter->hide();

    /// check for category already exist(to avoid same entry)
    QString command = QString("Select filter_name from file_size_filters");

    QStringList existing_filters_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, destination_db_file, Q_FUNC_INFO);

    bool bool_category_already_exist = false;

    for(int count = 0; count < existing_filters_list.size(); count++)
    {
        if(filter_str.trimmed().toLower() == existing_filters_list.at(count).trimmed().toLower())
        {
            bool_category_already_exist =  true;
            break;
        }
    }

    QString greater_than_size_str ,less_than_size_str,between_start_size_str;

    if(!bool_category_already_exist)
    {

        QString greater_than_size_unit_str ,less_than_size_unit_str,between_start_size_unit_str;
        QString between_end_size_str,between_end_size_unit_str;

        greater_than_size_str  = lineedit_greater_than_size->text().trimmed();
        less_than_size_str     = lineedit_less_than_size->text().trimmed();
        between_start_size_str = lineedit_beteen_start_size->text().trimmed();
        between_end_size_str   = lineedit_beteen_end_size->text().trimmed();

        if(!greater_than_size_str.isEmpty())
            greater_than_size_unit_str = combobox_start_size_unit->currentText();

        if(!less_than_size_str.isEmpty())
            less_than_size_unit_str = combobox_end_size_unit->currentText();

        if(!between_start_size_str.isEmpty())
            between_start_size_unit_str = combobox_between_start_size_unit->currentText();

        if(!between_end_size_str.isEmpty())
            between_end_size_unit_str = combobox_between_end_size_unit->currentText();

        command = QString("insert into file_size_filters(bookmark,filter_name,greater_than_size, greater_than_size_unit, less_than_size, less_than_size_unit, start_size, start_size_unit, end_size, end_size_unit) values(?,?,?,?,?,?,?,?,?,?)");

        QStringList list;
        list << QString("1")
             << filter_str
             << greater_than_size_str
             << greater_than_size_unit_str
             << less_than_size_str
             << less_than_size_unit_str
             << between_start_size_str
             << between_start_size_unit_str
             << between_end_size_str
             << between_end_size_unit_str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,destination_db_file,Q_FUNC_INFO);


        prepare_file_size_filters_display();

    }
    else
    {
        /// set item selected if already exists
        for(int row = 0; row < ui->tableWidget_filter_label->rowCount(); row++)
        {
            if(ui->tableWidget_filter_label->item(row , 0)->text().trimmed().toLower() == filter_str.trimmed().toLower())
            {
                ui->tableWidget_filter_label->selectRow(row);
                break;
            }
        }

        prepare_file_size_filters_display();
    }


    recon_static_functions::app_debug(" - Ends",Q_FUNC_INFO);
}

void recon_configuration_page_file_size::slot_lineedit_add_category_text_changed(QString text)
{
    text = text.trimmed();

    if(!text.isEmpty())
    {
        pushbutton_add_filters->setEnabled(true);
        lineedit_add_filters->setStyleSheet(styleSheet());
    }
    else
    {
        pushbutton_add_filters->setEnabled(false);
    }
}

void recon_configuration_page_file_size::slot_category_cancel_button_clicked()
{
    dialog_add_filter->hide();
}

