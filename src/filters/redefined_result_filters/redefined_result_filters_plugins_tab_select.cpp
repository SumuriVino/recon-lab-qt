#include "redefined_result_filters_plugins_tab_select.h"
#include "ui_redefined_result_filters_plugins_tab_select.h"

redefined_result_filters_plugins_tab_select::redefined_result_filters_plugins_tab_select(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::redefined_result_filters_plugins_tab_select)
{
    ui->setupUi(this);

    setWindowTitle("Plugin Tab Selection");
    setWindowFlags(Qt::Window);
    list_all_plugin_info.clear();

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->tableWidget_categories->setAlternatingRowColors(true);
    ui->tableWidget_categories->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_categories->setGridStyle(Qt::NoPen);
    ui->tableWidget_categories->setShowGrid(false);
    ui->tableWidget_categories->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_categories->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_categories->setAutoScroll(true);
    QFont m_font = font();
    m_font.setPointSize(12);
    ui->tableWidget_categories->setFont(m_font);
    QString stylesheet = func_to_set_stylesheet();
    ui->tableWidget_categories->setStyleSheet(stylesheet);


    ui->tableWidget_plugins->setAlternatingRowColors(true);
    ui->tableWidget_plugins->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_plugins->setGridStyle(Qt::NoPen);
    ui->tableWidget_plugins->setShowGrid(false);
    ui->tableWidget_plugins->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_plugins->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_plugins->setAutoScroll(true);
    ui->tableWidget_plugins->setFont(m_font);
    ui->tableWidget_plugins->setStyleSheet(stylesheet);


    ui->tableWidget_tabs->setAlternatingRowColors(true);
    ui->tableWidget_tabs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_tabs->setGridStyle(Qt::NoPen);
    ui->tableWidget_tabs->setShowGrid(false);
    ui->tableWidget_tabs->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_tabs->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_tabs->setAutoScroll(true);
    ui->tableWidget_tabs->setFont(m_font);
    ui->tableWidget_tabs->setStyleSheet(stylesheet);


    connect(ui->tableWidget_categories,SIGNAL(cellClicked(int,int)),this,SLOT(slot_category_tablewidget_single_clicked(int,int)));
    connect(ui->tableWidget_plugins,SIGNAL(cellClicked(int,int)),this,SLOT(slot_plugin_name_tablewidget_single_clicked(int,int)));

    message_dialog_obj = new message_dialog(this);
    message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Tab Selection Missing");

}

redefined_result_filters_plugins_tab_select::~redefined_result_filters_plugins_tab_select()
{
    delete ui;
}

QString redefined_result_filters_plugins_tab_select::func_to_set_stylesheet()
{
    QString stylesheet = QString(
                "QTableView {"
                "alternate-background-color: white;"
                "background-color:  #f1f1f1;"
                " }"

                "QTableView::item {"
                "border-top: 1px solid white;"
                " }"

                "QTableView::item:selected:!active {"
                "color:white;"
                "background-color:  #a1c9f7;"
                "}"

                "QTableView::indicator:checked {background-color:black;border: 1px solid #d6d6d6;}"
                "QTableView::indicator:unchecked {background-color:#f1f1f1;border: 1px solid #777777;}"

                "QTableView::indicator {"
                "width: 13px;"
                "height: 13px;}"
                );

    return stylesheet;
}

void redefined_result_filters_plugins_tab_select::pub_display_plugins_information()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

   QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return ;
    }

    QString command = "select name from plugins";
    QStringList plugin_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,destination_db_file,Q_FUNC_INFO);

    plugin_categories_list.clear();
    QString tmp_str;

    for(int i = 0; i < plugin_list.size(); i++)
    {
        strct_plugin_info obj;
        obj.plugin_name = plugin_list.at(i);
        obj.parent_name = recon_helper_standard_obj->get_category_of_plugin_by_dbreference(destination_db,plugin_list.at(i),Q_FUNC_INFO);

        tmp_str.clear();
        tmp_str = recon_helper_standard_obj->get_category_of_plugin_by_dbreference(destination_db,plugin_list.at(i),Q_FUNC_INFO);
        plugin_categories_list << tmp_str;

        QStringList tabs_list;
        tabs_list = recon_helper_standard_obj->get_tablist_of_plugin_from_case_configuration_by_dbreference(destination_db, plugin_list.at(i),Q_FUNC_INFO);

        obj.plugin_tab_name_list = tabs_list;
        list_all_plugin_info << obj;
    }

    plugin_categories_list.removeDuplicates();
    for(int i = 0; i < plugin_categories_list.size(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(plugin_categories_list.at(i));

        ui->tableWidget_categories->insertRow(i);
        ui->tableWidget_categories->setItem(i,0,item);
        ui->tableWidget_categories->setRowHeight(i,24);
    }

    destination_db.close();
    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void redefined_result_filters_plugins_tab_select::pub_clear_plugins_tab_tablewidget()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    while(ui->tableWidget_tabs->rowCount())
        ui->tableWidget_tabs->removeRow(0);

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void redefined_result_filters_plugins_tab_select::slot_category_tablewidget_single_clicked(int row,int col)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    list_plugins_in_category.clear();
    ui->tableWidget_tabs->setRowCount(0);        //It's to clean last tablewidget.

    QString category_str = ui->tableWidget_categories->item(row,col)->text();;

    for(int iter=0; iter < list_all_plugin_info.size(); iter++){
       strct_plugin_info tmp_obj = list_all_plugin_info.at(iter);
        if(tmp_obj.parent_name != category_str){
            continue;
        }

        list_plugins_in_category << tmp_obj;
    }

    ui->tableWidget_plugins->setRowCount(0);

    for(int i = 0; i < list_plugins_in_category.size(); i++)
    {
       strct_plugin_info obj_var = list_plugins_in_category.at(i);

        QString plugin_name = obj_var.plugin_name;

        QTableWidgetItem *item = new QTableWidgetItem;
        QString icon_path = "../icons/plugins/" + plugin_name.replace(" ","_") + ".png";

        item->setIcon(QIcon(icon_path));
        item->setText(obj_var.plugin_name);

        ui->tableWidget_plugins->insertRow(i);
        ui->tableWidget_plugins->setItem(i,0,item);
        ui->tableWidget_plugins->setRowHeight(i,24);
    }

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void redefined_result_filters_plugins_tab_select::slot_plugin_name_tablewidget_single_clicked(int row, int col)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    QString plugin_name_str = ui->tableWidget_plugins->item(row,col)->text();;

    ui->tableWidget_tabs->setRowCount(0);

    for(int i = 0; i < list_plugins_in_category.size(); i++)
    {
        strct_plugin_info obj_var;
        obj_var = list_plugins_in_category.at(i);

        if(obj_var.plugin_name != plugin_name_str){
            continue;
        }

        selected_plugin_tabs_list.clear();
        selected_plugin_tabs_list = obj_var.plugin_tab_name_list;

        for(int itr=0; itr < selected_plugin_tabs_list.size(); itr++){

            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(selected_plugin_tabs_list.at(itr));

            ui->tableWidget_tabs->insertRow(itr);
            ui->tableWidget_tabs->setItem(itr,0,item);
            ui->tableWidget_tabs->setRowHeight(itr,24);
        }
    }

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);

}

void redefined_result_filters_plugins_tab_select::on_pushButton_add_plugin_tab_clicked()
{
    if(ui->tableWidget_tabs->currentRow() < 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_WARNING, "Please select a tab.");
        message_dialog_obj->show();
    }

    QString plugin_name = ui->tableWidget_plugins->item(ui->tableWidget_plugins->currentRow(), 0)->text();
    QString tab_name = ui->tableWidget_tabs->item(ui->tableWidget_tabs->currentRow(), 0)->text();

    emit signal_selected_plugin_and_tab(plugin_name, tab_name);

}
