#include "sqlite_viewer.h"
#include "ui_sqlite_viewer.h"

void sqlite_viewer::on_comboBox_dbtables_selection_currentIndexChanged(int index)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    ///-----The following code is to clear the tablewidget display after exec query table changing -----//
    ui->tableWidget_query_execution->clearContents();
    ui->tableWidget_query_execution->setColumnCount(0);
    ui->tableWidget_query_execution->setRowCount(0);

    if(!bool_combobox_clear)
        return;

    ///---Following code is to acomodate proper check/uncheck even if right click manage columns haven't happend----//
    for(int i = 0; i < list_struct_tablewidget_data_sqlite_viewer.size(); i++)
    {
        struct_global_sqlite_viewer_tablewidget_main obj_main = list_struct_tablewidget_data_sqlite_viewer.at(i);

        if(obj_main.table_name == ui->comboBox_dbtables_selection->currentText())
        {
            ret_tableWidget_from_cols_sel = NULL;
            ret_tableWidget_from_cols_sel = obj_main.tableWidget_columns_selection_ptr;

            disconnect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
            connect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));

            disconnect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));
            connect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

            table_name_of_cols_sel = obj_main.table_name;
            break;
        }
    }

    //-Change Text on current tab change
    if(bool_combotext_change_on_tab_change)
    {
        ui->comboBox_dbtables_selection->setCurrentText(current_tab_name);
        bool_combotext_change_on_tab_change = false;
        return;
    }

    bool_index_change_from_comboBox = true;

    ///-0. Reset Widget or clear them
    int index_to_clear_tab = 0;

    reset_gui_widgets();

    if( ui->tabWidget_FilterViewSearch->currentIndex() != -1)
        ui->tabWidget_FilterViewSearch->removeTab(index_to_clear_tab);


    ///-2 Display Data Table
    for(int i = 0; i < list_struct_tablewidget_data_sqlite_viewer.size(); i++)
    {
        struct_global_sqlite_viewer_tablewidget_main obj_main = list_struct_tablewidget_data_sqlite_viewer.at(i);

        if(obj_main.table_name == ui->comboBox_dbtables_selection->currentText())
        {
            ///-A. Display FilterViewSearchTable
            int index_FilterView =  ui->tabWidget_FilterViewSearch->addTab(obj_main.tableWidget_FilterViewSearch_ptr,obj_main.table_name);
            ui->tabWidget_FilterViewSearch->setCurrentIndex(index_FilterView);
            index_to_clear_tab =  ui->tabWidget_FilterViewSearch->currentIndex();

            ///-B. Display MainDataDisplayTable
            int index_MainData = ui->tabWidget_main_data_panel->addTab(obj_main.tableWidget_MainDataDisplay_ptr,obj_main.table_name);
            ui->tabWidget_main_data_panel->setCurrentIndex(index_MainData);

            //-Check To disable Closable cross in last or very first tab.
            if(ui->tabWidget_main_data_panel->count() == 1)
            {
                ui->tabWidget_main_data_panel->setTabsClosable(false);
            }
            else
            {
                ui->tabWidget_main_data_panel->setTabsClosable(true);
            }

            active_spacebar_bookmark_and_metadat_show(obj_main);
            break;
        }
    }

    connect_table_filter_view_search_for_cell_clicked(index);

    ///------------------------
    for(int i = 0; i < list_struct_tablewidget_data_sqlite_viewer.size(); i++)
    {
        struct_global_sqlite_viewer_tablewidget_main obj_main = list_struct_tablewidget_data_sqlite_viewer.at(i);

        if(obj_main.table_name == current_tab_name)
        {

            QTableWidget *m_tablewidget_ptr = obj_main.tableWidget_FilterViewSearch_ptr;

            QString str;
            str += "select ";
            for(int itr = 2; itr < (m_tablewidget_ptr->rowCount() - 2) ; itr++)
            {
                QString column_name = m_tablewidget_ptr->item(itr,0)->text().trimmed();

                if(column_name == "RECON INT" || column_name == "RECON Examiner Tags")
                    continue;

                str += column_name;
                str+= QString(",");
            }

            if(str.endsWith(",")){
                str.remove(str.size() -1,1);
            }

            str += QString(" from ") +  current_tab_name;

            ui->textEdit_query_insert->setText(str);
        }
    }

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::slot_common_tablewidget_col_sel_cell_clicked(int row, int column)
{
    recon_static_functions::app_debug(" - START",Q_FUNC_INFO);

    int index= -1;
    for(int itr=0; itr < list_struct_tablewidget_data_sqlite_viewer.size(); itr++)
    {
        struct_global_sqlite_viewer_tablewidget_main obj_main;
        obj_main = list_struct_tablewidget_data_sqlite_viewer.at(itr);

        if(obj_main.table_name == table_name_of_cols_sel)
        {
            index = itr;
            break;
        }
    }

    if(index == -1)
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    struct_global_sqlite_viewer_tablewidget_main obj_main;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > index)
        obj_main = list_struct_tablewidget_data_sqlite_viewer.at(index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }
    QTableWidget *tableWidget_filter_view_search = obj_main.tableWidget_FilterViewSearch_ptr;

    m_tablewidget *tableWidget_main_data_display = obj_main.tableWidget_MainDataDisplay_ptr;

    struct_global_sqlite_viewer_single_dbtable_info obj_db_struct = obj_main.obj_struct_dbtable_detail;

    QStringList searchList = obj_db_struct.search_list;
    QStringList viewList = obj_db_struct.view_list;

    tableWidget_main_data_display->showColumn(0);
    tableWidget_main_data_display->showColumn(1);
    tableWidget_main_data_display->showColumn(2);
    tableWidget_main_data_display->showColumn(tableWidget_main_data_display->columnCount() - 1);
    tableWidget_main_data_display->setColumnWidth(0,70);

    tableWidget_main_data_display->setColumnWidth(1,65);
    tableWidget_main_data_display->setColumnWidth(2,100);

    emit signal_sqlite_viewer_metadata("","","","");
    emit signal_save_openwith_changes_to_bucket(QString(MACRO_CASE_TREE_Sqlite_Viewer));

    if(column == 0)
    {
        if(sender() == ret_tableWidget_from_cols_sel)
        {
            disconnect(tableWidget_filter_view_search,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
            disconnect(tableWidget_filter_view_search->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

            disconnect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
            disconnect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

            tableWidget_filter_view_search->selectRow(row);
            ret_tableWidget_from_cols_sel->selectRow(row);

            if(ret_tableWidget_from_cols_sel->item(row,column)->checkState())
            {
                ///- View coloum in data table
                tableWidget_main_data_display->showColumn(row );
                tableWidget_filter_view_search->item(row,1)->setCheckState(Qt::Checked);
                tableWidget_filter_view_search->item(row,0)->setCheckState(Qt::Checked);
                ret_tableWidget_from_cols_sel->item(row,1)->setCheckState(Qt::Checked);

                if(check_vertical_hdr_with_col_count == 2){
                    tableWidget_main_data_display->verticalHeader()->setVisible(true);
                }

                ///- Update structure list search by default if coloum enable to show.
                searchList.removeAt(row);
                searchList.insert(row,"1");

                viewList.removeAt(row);
                viewList.insert(row,"1");
            }
            else
            {
                ///- Hide Coloum
                tableWidget_main_data_display->hideColumn(row);
                tableWidget_filter_view_search->item(row,1)->setCheckState(Qt::Unchecked);
                tableWidget_filter_view_search->item(row,0)->setCheckState(Qt::Unchecked);
                ret_tableWidget_from_cols_sel->item(row,1)->setCheckState(Qt::Unchecked);

                if(check_vertical_hdr_with_col_count == 1)
                {
                    tableWidget_main_data_display->verticalHeader()->setVisible(false);
                }

                ///- remove search by default if coloum disable to show.
                searchList.removeAt(row);
                searchList.insert(row,"0");

                viewList.removeAt(row);
                viewList.insert(row,"0");
            }
        }
        else
        {
            disconnect(tableWidget_filter_view_search,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
            disconnect(tableWidget_filter_view_search->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

            disconnect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
            disconnect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

            tableWidget_filter_view_search->selectRow(row);
            ret_tableWidget_from_cols_sel->selectRow(row);

            if(tableWidget_filter_view_search->item(row,column)->checkState())
            {
                ///- View coloum in data table
                tableWidget_main_data_display->showColumn(row );
                tableWidget_filter_view_search->item(row,1)->setCheckState(Qt::Checked);
                ret_tableWidget_from_cols_sel->item(row,1)->setCheckState(Qt::Checked);
                ret_tableWidget_from_cols_sel->item(row,0)->setCheckState(Qt::Checked);

                if(check_vertical_hdr_with_col_count == 2)
                {
                    tableWidget_main_data_display->verticalHeader()->setVisible(true);
                }

                ///- Update structure list search by default if coloum enable to show.
                searchList.removeAt(row);
                searchList.insert(row,"1");

                viewList.removeAt(row);
                viewList.insert(row,"1");
            }
            else
            {
                ///- Hide Coloum
                tableWidget_main_data_display->hideColumn(row);
                tableWidget_filter_view_search->item(row,1)->setCheckState(Qt::Unchecked);
                ret_tableWidget_from_cols_sel->item(row,1)->setCheckState(Qt::Unchecked);
                ret_tableWidget_from_cols_sel->item(row,0)->setCheckState(Qt::Unchecked);

                if(check_vertical_hdr_with_col_count == 1){
                    tableWidget_main_data_display->verticalHeader()->setVisible(false);
                }

                ///- remove search by default if coloum disable to show.
                searchList.removeAt(row);
                searchList.insert(row,"0");

                viewList.removeAt(row);
                viewList.insert(row,"0");
            }
        }
    }
    if(column == 1)
    {
        if(sender() == ret_tableWidget_from_cols_sel)
        {
            disconnect(tableWidget_filter_view_search,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
            disconnect(tableWidget_filter_view_search->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

            disconnect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
            disconnect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

            tableWidget_filter_view_search->selectRow(row);
            ret_tableWidget_from_cols_sel->selectRow(row);

            if(ret_tableWidget_from_cols_sel->item(row,column)->checkState())
            {
                tableWidget_filter_view_search->item(row,1)->setCheckState(Qt::Checked);

                searchList.removeAt(row);
                searchList.insert(row,"1");

                viewList.removeAt(row);
                viewList.insert(row,"1");
            }
            else
            {
                tableWidget_filter_view_search->item(row,1)->setCheckState(Qt::Unchecked);

                searchList.removeAt(row);
                searchList.insert(row,"0");

                viewList.removeAt(row);
                viewList.insert(row,"0");
            }
        }
        else
        {
            disconnect(tableWidget_filter_view_search,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
            disconnect(tableWidget_filter_view_search->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

            disconnect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
            disconnect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

            tableWidget_filter_view_search->selectRow(row);
            ret_tableWidget_from_cols_sel->selectRow(row);

            if(tableWidget_filter_view_search->item(row,column)->checkState())
            {
                ret_tableWidget_from_cols_sel->item(row,1)->setCheckState(Qt::Checked);

                searchList.removeAt(row);
                searchList.insert(row,"1");

                viewList.removeAt(row);
                viewList.insert(row,"1");
            }
            else
            {
                ret_tableWidget_from_cols_sel->item(row,1)->setCheckState(Qt::Unchecked);

                searchList.removeAt(row);
                searchList.insert(row,"0");

                viewList.removeAt(row);
                viewList.insert(row,"0");
            }
        }
    }

    connect(tableWidget_filter_view_search,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
    connect(tableWidget_filter_view_search->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

    connect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
    connect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

    //-updated in sub structure
    obj_db_struct.search_list = searchList;

    obj_db_struct.view_list = viewList;

    //updat in main structure
    obj_main.obj_struct_dbtable_detail = obj_db_struct;

    obj_main.tableWidget_MainDataDisplay_ptr->horizontalHeader()->hide();
    obj_main.tableWidget_MainDataDisplay_ptr->horizontalHeader()->show();

    if(list_struct_tablewidget_data_sqlite_viewer.size() > index)
    {
        list_struct_tablewidget_data_sqlite_viewer.removeAt(index);
        list_struct_tablewidget_data_sqlite_viewer.insert(index,obj_main);
    }

    int ret_value = check_savedstate_and_update_filterview_search();
    if(ret_value == 1)
    {
        recon_static_functions::app_debug("END",Q_FUNC_INFO);
        return;
    }

    create_or_update_current_save_state_of_sqlite_viewer();

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer:: slot_savestate_on_combobox_conversion_change()
{
    int ret_value = check_savedstate_and_update_filterview_search();
    if(ret_value == 1)
    {
        return;
    }

    create_or_update_current_save_state_of_sqlite_viewer();
}

void sqlite_viewer::slot_onload_savestate_on_combobox_conversion_change()
{
    int ret_value = check_savedstate_and_update_filterview_search();
    if(ret_value == 1)
    {
        return;
    }

    create_or_update_current_save_state_of_sqlite_viewer();
}

void sqlite_viewer::slot_update_sqlite_viewer_export_checkbox_and_count(bool chckbox,int count)
{
    bool_global_export_checkbox_enable_sqlite_viewer = chckbox;
    global_export_checkbox_disable_sqlite_viewer = count;
}

void sqlite_viewer:: slot_on_tableWidget_FilterViewSearch_cellClicked(int row, int column)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    for(int itr=0; itr < list_struct_tablewidget_data_sqlite_viewer.size(); itr++)
    {
        struct_global_sqlite_viewer_tablewidget_main obj_main;
        obj_main = list_struct_tablewidget_data_sqlite_viewer.at(itr);

        if(obj_main.table_name == table_name_of_cols_sel)
        {
            break;
        }
    }

    ///-1. Get the pointer of the tables from the list
    int index = 0;
    if(bool_index_change_from_comboBox)
    {
        index = ui->comboBox_dbtables_selection->currentIndex();
    }
    else
    {
        index = current_tab_index;
    }

    bool_index_change_from_comboBox = false;

    struct_global_sqlite_viewer_tablewidget_main obj_main;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > index)
        obj_main = list_struct_tablewidget_data_sqlite_viewer.at(index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }
    QTableWidget *tableWidget_filter_view_search = obj_main.tableWidget_FilterViewSearch_ptr;

    m_tablewidget *tableWidget_main_data_display = obj_main.tableWidget_MainDataDisplay_ptr;

    emit signal_save_openwith_changes_to_bucket(QString(MACRO_CASE_TREE_Sqlite_Viewer));

    struct_global_sqlite_viewer_single_dbtable_info obj_db_struct = obj_main.obj_struct_dbtable_detail;
    QStringList searchList = obj_db_struct.search_list;

    if(column == 0)
    {
        tableWidget_filter_view_search->selectRow(row);

        if(tableWidget_filter_view_search->item(row,column)->checkState())
        {
            ///- View coloum in data table
            tableWidget_main_data_display->showColumn(row);
            tableWidget_filter_view_search->item(row,1)->setCheckState(Qt::Checked);

            if(check_vertical_hdr_with_col_count == 2)
            {
                tableWidget_main_data_display->verticalHeader()->setVisible(true);
            }

            ///- Update structure list search by default if coloum enable to show.
            searchList.removeAt(row);
            searchList.insert(row,"1");
        }
        else
        {
            ///- Hide Coloum
            tableWidget_main_data_display->hideColumn(row);
            tableWidget_filter_view_search->item(row,1)->setCheckState(Qt::Unchecked);

            if(check_vertical_hdr_with_col_count == 1)
            {
                tableWidget_main_data_display->verticalHeader()->setVisible(false);
            }

            ///- remove search by default if coloum disable to show.
            searchList.removeAt(row);
            searchList.insert(row,"0");
        }
    }

    if(column == 1)
    {
        tableWidget_filter_view_search->selectRow(row);

        if(tableWidget_filter_view_search->item(row,column)->checkState())
        {
            searchList.removeAt(row);
            searchList.insert(row,"1");
        }
        else
        {
            searchList.removeAt(row);
            searchList.insert(row,"0");
        }
    }

    //-updated in sub structure
    obj_db_struct.search_list = searchList;

    //updat in main structure
    obj_main.obj_struct_dbtable_detail = obj_db_struct;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > index)
    {
        list_struct_tablewidget_data_sqlite_viewer.removeAt(index);
        list_struct_tablewidget_data_sqlite_viewer.insert(index,obj_main);
    }

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::slot_common_tablewidget_col_sel_horizontal_header_clicked(int column)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    if(sender()->parent() == ret_tableWidget_from_cols_sel)
    {
        recon_static_functions::app_debug("by manage columns",Q_FUNC_INFO);
    }

    recon_static_functions::app_debug("END",Q_FUNC_INFO);

    struct_global_sqlite_viewer_tablewidget_main obj_struct;
    int index = -1;
    for(int itr=0; itr < list_struct_tablewidget_data_sqlite_viewer.size(); itr++)
    {
        obj_struct = list_struct_tablewidget_data_sqlite_viewer.at(itr);
        if(obj_struct.table_name == table_name_of_cols_sel)
        {
            index = itr;
            break;
        }
    }

    if(index == -1)
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    check_vertical_hdr_with_col_count = 0;

    QTableWidget *filter_view_tableWidget = obj_struct.tableWidget_FilterViewSearch_ptr;
    ret_tableWidget_from_cols_sel->clearSelection();
    obj_struct.tableWidget_FilterViewSearch_ptr->clearSelection();

    m_tablewidget *main_display_tableWidget = obj_struct.tableWidget_MainDataDisplay_ptr;

    struct_global_sqlite_viewer_single_dbtable_info obj_db_struct = obj_struct.obj_struct_dbtable_detail;
    QStringList searchList = obj_db_struct.search_list;
    QStringList viewList = obj_db_struct.view_list;

    if(column == 0)
    {
        if(sender()->parent() == ret_tableWidget_from_cols_sel)
        {
            if(ret_tableWidget_from_cols_sel->horizontalHeaderItem(0)->checkState())
            {
                disconnect(filter_view_tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(filter_view_tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

                disconnect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

                ret_tableWidget_from_cols_sel->horizontalHeaderItem(0)->setIcon(QIcon("../icons/unchecked.png"));
                ret_tableWidget_from_cols_sel->horizontalHeaderItem(0)->setCheckState(Qt::Unchecked);

                filter_view_tableWidget->horizontalHeaderItem(0)->setIcon(QIcon("../icons/unchecked.png"));
                filter_view_tableWidget->horizontalHeaderItem(0)->setCheckState(Qt::Unchecked);

                obj_struct.tableWidget_MainDataDisplay_ptr->clearSelection();

                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setIcon(QIcon("../icons/unchecked.png"));
                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setCheckState(Qt::Unchecked);

                filter_view_tableWidget->horizontalHeaderItem(1)->setIcon(QIcon("../icons/unchecked.png"));
                filter_view_tableWidget->horizontalHeaderItem(1)->setCheckState(Qt::Unchecked);

                for(int row = 0 ; row < ret_tableWidget_from_cols_sel->rowCount(); row++)
                {
                    filter_view_tableWidget->item(row,0)->setCheckState(Qt::Unchecked);
                    ret_tableWidget_from_cols_sel->item(row,0)->setCheckState(Qt::Unchecked);

                    filter_view_tableWidget->item(row,1)->setCheckState(Qt::Unchecked);
                    ret_tableWidget_from_cols_sel->item(row,1)->setCheckState(Qt::Unchecked);

                    main_display_tableWidget->hideColumn(row);

                    if(row == (filter_view_tableWidget->rowCount() - 1))
                    {
                        check_vertical_hdr_with_col_count = 1;
                        main_display_tableWidget->verticalHeader()->setVisible(false);
                    }

                    viewList.removeAt(row);
                    viewList.insert(row,"0");
                }
            }
            else
            {
                disconnect(filter_view_tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(filter_view_tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

                disconnect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));


                ret_tableWidget_from_cols_sel->horizontalHeaderItem(0)->setIcon(QIcon("../icons/checked.png"));
                ret_tableWidget_from_cols_sel->horizontalHeaderItem(0)->setCheckState(Qt::Checked);

                filter_view_tableWidget->horizontalHeaderItem(0)->setIcon(QIcon("../icons/checked.png"));
                filter_view_tableWidget->horizontalHeaderItem(0)->setCheckState(Qt::Checked);

                obj_struct.tableWidget_MainDataDisplay_ptr->clearSelection();

                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setIcon(QIcon("../icons/checked.png"));
                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setCheckState(Qt::Checked);

                filter_view_tableWidget->horizontalHeaderItem(1)->setIcon(QIcon("../icons/checked.png"));
                filter_view_tableWidget->horizontalHeaderItem(1)->setCheckState(Qt::Checked);

                //- Check all the row items
                for(int row = 0 ; row < ret_tableWidget_from_cols_sel->rowCount(); row++)
                {
                    filter_view_tableWidget->item(row,0)->setCheckState(Qt::Checked);
                    ret_tableWidget_from_cols_sel->item(row,0)->setCheckState(Qt::Checked);

                    filter_view_tableWidget->item(row,1)->setCheckState(Qt::Checked);
                    ret_tableWidget_from_cols_sel->item(row,1)->setCheckState(Qt::Checked);


                    main_display_tableWidget->showColumn(row);

                    if(row == (filter_view_tableWidget->rowCount() - 1))
                    {
                        check_vertical_hdr_with_col_count = 2;
                        main_display_tableWidget->verticalHeader()->setVisible(true);
                    }

                    viewList.removeAt(row);
                    viewList.insert(row,"1");
                }
            }
        }
        else
        {
            if(filter_view_tableWidget->horizontalHeaderItem(0)->checkState())
            {
                disconnect(filter_view_tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(filter_view_tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

                disconnect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));


                ret_tableWidget_from_cols_sel->horizontalHeaderItem(0)->setIcon(QIcon("../icons/unchecked.png"));
                ret_tableWidget_from_cols_sel->horizontalHeaderItem(0)->setCheckState(Qt::Unchecked);

                filter_view_tableWidget->horizontalHeaderItem(0)->setIcon(QIcon("../icons/unchecked.png"));
                filter_view_tableWidget->horizontalHeaderItem(0)->setCheckState(Qt::Unchecked);

                obj_struct.tableWidget_MainDataDisplay_ptr->clearSelection();

                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setIcon(QIcon("../icons/unchecked.png"));
                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setCheckState(Qt::Unchecked);

                filter_view_tableWidget->horizontalHeaderItem(1)->setIcon(QIcon("../icons/unchecked.png"));
                filter_view_tableWidget->horizontalHeaderItem(1)->setCheckState(Qt::Unchecked);

                for(int row = 0 ; row < filter_view_tableWidget->rowCount(); row++)
                {
                    ret_tableWidget_from_cols_sel->item(row,0)->setCheckState(Qt::Unchecked);
                    filter_view_tableWidget->item(row,0)->setCheckState(Qt::Unchecked);

                    ret_tableWidget_from_cols_sel->item(row,1)->setCheckState(Qt::Unchecked);
                    filter_view_tableWidget->item(row,1)->setCheckState(Qt::Unchecked);

                    main_display_tableWidget->hideColumn(row);

                    if(row == (filter_view_tableWidget->rowCount() - 1))
                    {
                        check_vertical_hdr_with_col_count = 1;
                        main_display_tableWidget->verticalHeader()->setVisible(false);
                    }

                    viewList.removeAt(row);
                    viewList.insert(row,"0");
                }
            }
            else
            {
                disconnect(filter_view_tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(filter_view_tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

                disconnect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));


                ret_tableWidget_from_cols_sel->horizontalHeaderItem(0)->setIcon(QIcon("../icons/checked.png"));
                ret_tableWidget_from_cols_sel->horizontalHeaderItem(0)->setCheckState(Qt::Checked);

                filter_view_tableWidget->horizontalHeaderItem(0)->setIcon(QIcon("../icons/checked.png"));
                filter_view_tableWidget->horizontalHeaderItem(0)->setCheckState(Qt::Checked);

                obj_struct.tableWidget_MainDataDisplay_ptr->clearSelection();

                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setIcon(QIcon("../icons/checked.png"));
                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setCheckState(Qt::Checked);

                filter_view_tableWidget->horizontalHeaderItem(1)->setIcon(QIcon("../icons/checked.png"));
                filter_view_tableWidget->horizontalHeaderItem(1)->setCheckState(Qt::Checked);


                //- Check all the row items
                for(int row = 0 ; row < filter_view_tableWidget->rowCount(); row++)
                {
                    if(row == (filter_view_tableWidget->rowCount() - 2) || row == (filter_view_tableWidget->rowCount() - 3) )
                        continue;

                    ret_tableWidget_from_cols_sel->item(row,0)->setCheckState(Qt::Checked);
                    filter_view_tableWidget->item(row,0)->setCheckState(Qt::Checked);

                    ret_tableWidget_from_cols_sel->item(row,1)->setCheckState(Qt::Checked);
                    filter_view_tableWidget->item(row,1)->setCheckState(Qt::Checked);

                    main_display_tableWidget->showColumn(row);

                    if(row == (filter_view_tableWidget->rowCount() - 1))
                    {
                        check_vertical_hdr_with_col_count = 2;
                        main_display_tableWidget->verticalHeader()->setVisible(true);
                    }

                    viewList.removeAt(row);
                    viewList.insert(row,"1");
                }
            }
        }
    }

    if(column == 1)
    {

        if(sender()->parent() == ret_tableWidget_from_cols_sel)
        {
            if(ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->checkState())
            {
                disconnect(filter_view_tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(filter_view_tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

                disconnect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setIcon(QIcon("../icons/unchecked.png"));
                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setCheckState(Qt::Unchecked);

                filter_view_tableWidget->horizontalHeaderItem(1)->setIcon(QIcon("../icons/unchecked.png"));
                filter_view_tableWidget->horizontalHeaderItem(1)->setCheckState(Qt::Unchecked);

                for(int row = 0 ; row < ret_tableWidget_from_cols_sel->rowCount(); row++)
                {
                    ret_tableWidget_from_cols_sel->item(row,1)->setCheckState(Qt::Unchecked);
                    filter_view_tableWidget->item(row,1)->setCheckState(Qt::Unchecked);
                    searchList.removeAt(row);
                    searchList.insert(row,"0");
                }
            }
            else
            {
                disconnect(filter_view_tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(filter_view_tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

                disconnect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setIcon(QIcon("../icons/checked.png"));
                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setCheckState(Qt::Checked);

                filter_view_tableWidget->horizontalHeaderItem(1)->setIcon(QIcon("../icons/checked.png"));
                filter_view_tableWidget->horizontalHeaderItem(1)->setCheckState(Qt::Checked);

                //- Check all the row items
                for(int row = 0 ; row < ret_tableWidget_from_cols_sel->rowCount(); row++)
                {
                    ret_tableWidget_from_cols_sel->item(row,1)->setCheckState(Qt::Checked);
                    filter_view_tableWidget->item(row,1)->setCheckState(Qt::Checked);
                    searchList.removeAt(row);
                    searchList.insert(row,"1");
                }
            }
        }
        else
        {
            if(filter_view_tableWidget->horizontalHeaderItem(1)->checkState())
            {
                disconnect(filter_view_tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(filter_view_tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

                disconnect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setIcon(QIcon("../icons/unchecked.png"));
                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setCheckState(Qt::Unchecked);

                filter_view_tableWidget->horizontalHeaderItem(1)->setIcon(QIcon("../icons/unchecked.png"));
                filter_view_tableWidget->horizontalHeaderItem(1)->setCheckState(Qt::Unchecked);

                for(int row = 0 ; row < filter_view_tableWidget->rowCount(); row++)
                {
                    ret_tableWidget_from_cols_sel->item(row,1)->setCheckState(Qt::Unchecked);
                    filter_view_tableWidget->item(row,1)->setCheckState(Qt::Unchecked);

                    searchList.removeAt(row);
                    searchList.insert(row,"0");
                }
            }
            else
            {

                disconnect(filter_view_tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(filter_view_tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

                disconnect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
                disconnect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setIcon(QIcon("../icons/checked.png"));
                ret_tableWidget_from_cols_sel->horizontalHeaderItem(1)->setCheckState(Qt::Checked);

                filter_view_tableWidget->horizontalHeaderItem(1)->setIcon(QIcon("../icons/checked.png"));
                filter_view_tableWidget->horizontalHeaderItem(1)->setCheckState(Qt::Checked);

                //- Check all the row items
                for(int row = 0 ; row < filter_view_tableWidget->rowCount(); row++)
                {
                    ret_tableWidget_from_cols_sel->item(row,1)->setCheckState(Qt::Checked);
                    filter_view_tableWidget->item(row,1)->setCheckState(Qt::Checked);

                    searchList.removeAt(row);
                    searchList.insert(row,"1");
                }
            }
        }
    }


    connect(filter_view_tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
    connect(filter_view_tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

    connect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
    connect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

    //-updated in sub structure
    obj_db_struct.search_list = searchList;
    obj_db_struct.view_list = viewList;

    //updat in main structure
    obj_struct.obj_struct_dbtable_detail = obj_db_struct;

    obj_struct.tableWidget_MainDataDisplay_ptr->horizontalHeader()->hide();
    obj_struct.tableWidget_MainDataDisplay_ptr->horizontalHeader()->show();

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    {
        list_struct_tablewidget_data_sqlite_viewer.removeAt(current_tab_index);

        m_tablewidget *tableWidget = obj_struct.tableWidget_MainDataDisplay_ptr;
        tableWidget->setColumnWidth(1,40);
        tableWidget->setColumnWidth(2,100);

        list_struct_tablewidget_data_sqlite_viewer.insert(current_tab_index,obj_struct);
    }


    int ret_value = check_savedstate_and_update_filterview_search();
    if(ret_value == 1)
    {
        recon_static_functions::app_debug("End",Q_FUNC_INFO);
        return;
    }

    create_or_update_current_save_state_of_sqlite_viewer();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void sqlite_viewer::slot_on_tableWidget_FilterViewSearch_horizontal_header_clicked(int column)
{
    check_vertical_hdr_with_col_count = 0;

    struct_global_sqlite_viewer_tablewidget_main obj_struct;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        obj_struct = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    QTableWidget *m_tableWidget_obj = obj_struct.tableWidget_FilterViewSearch_ptr;

    struct_global_sqlite_viewer_single_dbtable_info obj_db_struct = obj_struct.obj_struct_dbtable_detail;
    QStringList searchList = obj_db_struct.search_list;

    QStringList viewList = obj_db_struct.view_list;

    if(column == 0)
    {
        if(m_tableWidget_obj->horizontalHeaderItem(0)->checkState())
        {
            m_tableWidget_obj->horizontalHeaderItem(0)->setIcon(QIcon("../icons/unchecked.png"));
            m_tableWidget_obj->horizontalHeaderItem(0)->setCheckState(Qt::Unchecked);

            obj_struct.tableWidget_MainDataDisplay_ptr->clearSelection();

            m_tableWidget_obj->horizontalHeaderItem(1)->setIcon(QIcon("../icons/unchecked.png"));
            m_tableWidget_obj->horizontalHeaderItem(1)->setCheckState(Qt::Unchecked);
            int count = 0;

            for(int row = 0 ; row < m_tableWidget_obj->rowCount(); row++)
            {
                count++;
                if(count % 300 == 0)
                {
                    count = 0;
                    QCoreApplication::processEvents();
                }

                m_tableWidget_obj->item(row,0)->setCheckState(Qt::Unchecked);

                if(row == (m_tableWidget_obj->rowCount() - 1))
                {
                    check_vertical_hdr_with_col_count = 1;
                }

                slot_on_tableWidget_FilterViewSearch_cellClicked(row,0);

                viewList.removeAt(row);
                viewList.insert(row,"0");
            }
        }
        else
        {
            m_tableWidget_obj->horizontalHeaderItem(0)->setIcon(QIcon("../icons/checked.png"));
            m_tableWidget_obj->horizontalHeaderItem(0)->setCheckState(Qt::Checked);

            obj_struct.tableWidget_MainDataDisplay_ptr->clearSelection();

            m_tableWidget_obj->horizontalHeaderItem(1)->setIcon(QIcon("../icons/checked.png"));
            m_tableWidget_obj->horizontalHeaderItem(1)->setCheckState(Qt::Checked);

            //- Check all the row items
            int count = 0;
            for(int row = 0 ; row < m_tableWidget_obj->rowCount(); row++)
            {
                count++;
                if(count % 300 == 0)
                {
                    count = 0;
                    QCoreApplication::processEvents();
                }

                m_tableWidget_obj->item(row,0)->setCheckState(Qt::Checked);

                if(row == (m_tableWidget_obj->rowCount() - 1))
                {
                    check_vertical_hdr_with_col_count = 2;
                }

                slot_on_tableWidget_FilterViewSearch_cellClicked(row,0);

                viewList.removeAt(row);
                viewList.insert(row,"1");
            }
        }
    }

    if(column == 1)
    {
        if(m_tableWidget_obj->horizontalHeaderItem(1)->checkState())
        {
            m_tableWidget_obj->horizontalHeaderItem(1)->setIcon(QIcon("../icons/unchecked.png"));
            m_tableWidget_obj->horizontalHeaderItem(1)->setCheckState(Qt::Unchecked);

            int count = 0;
            for(int row = 0 ; row < m_tableWidget_obj->rowCount(); row++)
            {
                count++;
                if(count % 300 == 0)
                {
                    count = 0;
                    QCoreApplication::processEvents();
                }

                m_tableWidget_obj->item(row,1)->setCheckState(Qt::Unchecked);
                searchList.removeAt(row);
                searchList.insert(row,"0");
            }
        }
        else
        {
            m_tableWidget_obj->horizontalHeaderItem(1)->setIcon(QIcon("../icons/checked.png"));
            m_tableWidget_obj->horizontalHeaderItem(1)->setCheckState(Qt::Checked);

            //- Check all the row items
            int count = 0;
            for(int row = 0 ; row < m_tableWidget_obj->rowCount(); row++)
            {
                count++;
                if(count % 300 == 0)
                {
                    count = 0;
                    QCoreApplication::processEvents();
                }

                m_tableWidget_obj->item(row,1)->setCheckState(Qt::Checked);
                searchList.removeAt(row);
                searchList.insert(row,"1");
            }
        }
    }

    //-updated in sub structure
    obj_db_struct.search_list = searchList;

    obj_db_struct.view_list = viewList;

    //updat in main structure
    obj_struct.obj_struct_dbtable_detail = obj_db_struct;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    {
        list_struct_tablewidget_data_sqlite_viewer.removeAt(current_tab_index);

        m_tablewidget *m_tableWidget_obj = obj_struct.tableWidget_MainDataDisplay_ptr;
        m_tableWidget_obj->setColumnWidth(1,100);

        list_struct_tablewidget_data_sqlite_viewer.insert(current_tab_index,obj_struct);
    }
}

void sqlite_viewer::slot_act_remove_all_bookmark()
{
    //    recon_static_functions::app_debug(" - START",Q_FUNC_INFO);


    //    struct_global_sqlite_viewer_tablewidget_main structMainObject;

    //    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    //        structMainObject = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    //    else{
    //        recon_static_functions::app_debug(" index out of range ------FAILED----",Q_FUNC_INFO);
    //        return;
    //    }

    //    if(structMainObject.tableWidget_MainDataDisplay_ptr->currentRow() < 0)
    //        return;

    //    setWidgetsDisable(true);

    //    for(int i = 0; i < structMainObject.tableWidget_MainDataDisplay_ptr->rowCount(); i++)
    //    {

    //        if(i % 500 == 0)
    //            QCoreApplication::processEvents();

    //        float percent = ((i*100)/structMainObject.tableWidget_MainDataDisplay_ptr->rowCount());

    //        if(i % 10 == 0)
    //            emit signal_progress_count_for_statusbar_progress_bar(structMainObject.tableWidget_MainDataDisplay_ptr->rowCount(), i, percent);

    //        if(structMainObject.tableWidget_MainDataDisplay_ptr->isRowHidden(i))
    //            continue;

    //        structMainObject.tableWidget_MainDataDisplay_ptr->item(i,0)->setCheckState(Qt::Unchecked);
    //    }
    //    setWidgetsDisable(false);




    //    //--- To save state on remove all ----//


    //    int ret_value = check_savedstate_and_update_MainData_display();
    //    if(ret_value == 1){
    //        recon_static_functions::app_debug(" - END",Q_FUNC_INFO);
    //    }else
    //    {

    //        create_or_update_current_save_state_of_sqlite_viewer();
    //    }



}

void sqlite_viewer::slot_act_add_notes()
{
    struct_global_sqlite_viewer_tablewidget_main struct_obj;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    {
        struct_obj = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    }
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    if(struct_obj.tableWidget_MainDataDisplay_ptr->currentRow() < 0)
        return;

    add_notes_dialog_popup->show();

    struct_global_sqlite_viewer_tablewidget_main obj_main;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        obj_main = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }


    m_tablewidget *m_tableWidget_obj = obj_main.tableWidget_MainDataDisplay_ptr;

    int row = m_tableWidget_obj->currentRow();
    int column_count = m_tableWidget_obj->columnCount();

    if(m_tableWidget_obj->item(row,(column_count -2))->text() != "")
    {
        add_notes_dialog_popup->set_notes(m_tableWidget_obj->item(row,(column_count -1))->text());
    }
}

void sqlite_viewer::slot_act_add_notes_to_bookmarks()
{
    struct_global_sqlite_viewer_tablewidget_main struct_obj;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        struct_obj = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    if(struct_obj.tableWidget_MainDataDisplay_ptr->currentRow() < 0)
        return;

    add_notes_to_bookmarks_dialog_popup->show();
}

void sqlite_viewer::slot_act_remove_all_notes()
{
    //    recon_static_functions::app_debug(" - START",Q_FUNC_INFO);

    //    struct_global_sqlite_viewer_tablewidget_main obj_main;


    //    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    //        obj_main = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    //    else{
    //        recon_static_functions::app_debug(" index out of range ------FAILED----",Q_FUNC_INFO);
    //        return;
    //    }

    //    if(obj_main.tableWidget_MainDataDisplay_ptr->currentRow() < 0)
    //        return;


    //    m_tablewidget*tableWidget = obj_main.tableWidget_MainDataDisplay_ptr;

    //    int rows = tableWidget->rowCount();
    //    int columns_count = tableWidget->columnCount();

    //    for(int itr=0; itr < rows; itr++){


    //        float percent = ((itr*100)/rows);

    //        if(itr % 10 == 0)
    //            emit signal_progress_count_for_statusbar_progress_bar(rows, itr, percent);

    //        add_notes_dialog_popup->set_notes(QString(""));
    //        tableWidget->item(itr,(columns_count-2))->setText("");

    //        QIcon icon(QString(""));
    //        tableWidget->item(itr,0)->setIcon(icon);

    //    }



    //    int ret_value = notes_check_savedstate_and_update_MainData_display(3);
    //    if(ret_value == 1){
    //        recon_static_functions::app_debug("MainDisplaydb exists there, updating it in case of notes",Q_FUNC_INFO);
    //    }
    //    else{
    //        create_or_update_current_save_state_of_sqlite_viewer();
    //    }


    //    recon_static_functions::app_debug(" - END",Q_FUNC_INFO);
}

void sqlite_viewer::slot_full_detail_info_triggered()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);


    full_detail_information *full_detail_info_obj = new full_detail_information(this);
    connect(full_detail_info_obj, SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)), this, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)));

    full_detail_info_obj->pub_set_essentials();
    QString block_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
    full_detail_info_obj->pub_set_hex_viewer_block_db_path(block_db_path);
    full_detail_info_obj->pub_set_global_connaction_manager_obj(global_connection_manager_obj);
    full_detail_info_obj->pub_set_recon_case_obj(recon_case_obj);
    full_detail_info_obj->pub_show_full_detail_info(sqlite_viewer_metadata_text,sqlite_viewer_current_media_file,index_sqlite_viewer_media_type_av_blank_image,ui->lineEdit_search->text().trimmed(),"","","","");

    full_detail_info_obj->show();


    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::slot_open_in_detach_triggered()
{
    emit signal_sqlite_viewer_open_in_detach_clicked_table_result(sqlite_viewer_metadata_text,sqlite_viewer_current_media_file,sqlite_viewer_current_media_file,index_sqlite_viewer_media_type_av_blank_image,ui->lineEdit_search->text().trimmed(),"");
}

void sqlite_viewer::slot_act_remove_notes_from_bookmarks()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    struct_global_sqlite_viewer_tablewidget_main obj_main;


    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        obj_main = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    m_tablewidget *tableWidget = obj_main.tableWidget_MainDataDisplay_ptr;

    int column_count = tableWidget->columnCount();

    for(int i = 0; i < tableWidget->rowCount(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();



        if(tableWidget->item(i,enum_coloumn_Sqlite_viewer_Bookmark)->checkState() == Qt::Checked)
        {
            if(tableWidget->item(i,(column_count -2))->text() =="")
                continue;

            tableWidget->item(i,(column_count -2))->setText("");
            tableWidget->item(i,enum_coloumn_Sqlite_viewer_Bookmark)->setIcon(QIcon(""));

            //----to remove notes from bookmarks-----//
        }
    }


    int ret_value = notes_check_savedstate_and_update_main_data_display(1);
    if(ret_value == 1)
    {
        recon_static_functions::app_debug("MainDisplaydb exists there, updating it in case of notes",Q_FUNC_INFO);
    }
    else
    {
        create_or_update_current_save_state_of_sqlite_viewer();
    }

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::slot_act_remove_notes()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    struct_global_sqlite_viewer_tablewidget_main obj_main;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        obj_main = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }
    if(obj_main.tableWidget_MainDataDisplay_ptr->currentRow() < 0)
        return;


    QString selected_sql_tab_name;
    if(focusWidget()->objectName() == "tableWidget_query_execution")
        selected_sql_tab_name = QString(MACRO_SQLITE_VIEWER_EXECUTE_SQL_TAB);
    else
        selected_sql_tab_name = QString(MACRO_SQLITE_VIEWER_BROWSE_DATA_TAB) + "-" + current_tab_name;


    current_sql_tab = selected_sql_tab_name;

    QString cmd = "SELECT file_path FROM 'sv_index' WHERE viewer_display_name = ?";
    QString source_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd ,QStringList(sqlite_viewer_display_name_in_tree_and_tab),0,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite",Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait.....");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();



    m_tablewidget *tableWidget = obj_main.tableWidget_MainDataDisplay_ptr;
    QModelIndexList selection_model_list = tableWidget->selectionModel()->selectedRows();
    QStringList INT_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= tableWidget->rowCount())
            continue;

        if(tableWidget->isRowHidden(selected_row_no))
            continue;

        int column_count = tableWidget->columnCount();

        add_notes_dialog_popup->set_notes(QString(""));

        tableWidget->item(selected_row_no,(column_count-2))->setText("");
        QIcon icon(QString(""));
        tableWidget->item(selected_row_no,0)->setIcon(icon);

        QString record_number =  tableWidget->item(selected_row_no , 2)->text();
        INT_list << record_number;
    }

    global_connection_manager_obj->pub_remove_notes_in_sqlite_viewer_db(INT_list , sqlite_viewer_display_name_in_tree_and_tab ,current_tab_name , selected_sql_tab_name);
    global_connection_manager_obj->pub_delete_notes_for_sqlite_viewer_db(INT_list, sqlite_viewer_display_name_in_tree_and_tab , current_tab_name , selected_sql_tab_name);
    global_connection_manager_obj->pub_delete_or_update_record_export_content_sqlite_viewer_db(INT_list ,current_tab_name ,source_file_path ,selected_sql_tab_name ,"");

    display_loading_progress_bar_non_cancelable_obj->hide();


    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::slot_rignt_click_menu_trigerred_sqlite_viewer(QAction *current_clicked_action)
{

    if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_Bookmark))
    {
        action_bookmark_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Bookmarks))
    {
        action_remove_bookmark_triggered();
    }

}

void sqlite_viewer::slot_act_export()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QString dest_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_SQLite_Viewer_QString).toString();


    bool bool_file_copied  =false;
    QString export_file_path = dest_dir_path;

    QFileInfo info(current_media_file);
    if(!info.exists())
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"No File(s) Exists");
        message_dialog_object->show();

        return;
    }

    export_file_path = export_file_path + info.fileName();
    if(info.isDir())
    {
        recon_static_functions::copy_directory(current_media_file,export_file_path, Q_FUNC_INFO);
        bool_file_copied = true;
    }

    if(info.isFile())
    {
        QFile::copy(current_media_file,export_file_path);
        bool_file_copied = true;
    }


    QStringList list_paths_to_message_box;
    list_paths_to_message_box << export_file_path;

    if(bool_file_copied)
    {
        message_dialog_object->pub_set_file_path_message(MACRO_MSG_INFORMATION,list_paths_to_message_box, export_file_path);
        message_dialog_object->show();
    }

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}


void sqlite_viewer::slot_hide_loading_display_dialog_box(bool cancel_status)
{
    bool_cancel_loading = cancel_status;
    display_loading_progress_bar_obj->hide();
}

void sqlite_viewer::slot_act_copy_to_clipboard()
{
    recon_static_functions::app_debug("--Starts---", Q_FUNC_INFO);

    if(focusWidget()->objectName() == "tableWidget_query_execution")
    {
        m_tablewidget *tableWidget = (m_tablewidget*)ui->tableWidget_query_execution;
        recon_static_functions::copy_tablewidget_data_to_clipboard(tableWidget, Q_FUNC_INFO);
    }
    else
    {
        struct_global_sqlite_viewer_tablewidget_main obj_main;
        if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        {
            obj_main= list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);

            m_tablewidget  *tableWidget_main_data_display;
            tableWidget_main_data_display = obj_main.tableWidget_MainDataDisplay_ptr;

            recon_static_functions::copy_tablewidget_data_to_clipboard(tableWidget_main_data_display, Q_FUNC_INFO);
        }
    }

    recon_static_functions::app_debug("--Ends---", Q_FUNC_INFO);
}

void sqlite_viewer::slot_add_notes(QString notes_text)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    add_notes_dialog_popup->hide();
    struct_global_sqlite_viewer_tablewidget_main obj_main;
    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        obj_main = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }


    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait.......");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    emit signal_save_openwith_changes_to_bucket(QString(MACRO_CASE_TREE_Sqlite_Viewer));

    m_tablewidget *m_tableWidget_obj = obj_main.tableWidget_MainDataDisplay_ptr;
    QModelIndexList selection_model_list = m_tableWidget_obj->selectionModel()->selectedRows();

    QString selectecd_sql_tab_name;

    if(focusWidget()->objectName() == "tableWidget_query_execution")
        selectecd_sql_tab_name = QString(MACRO_SQLITE_VIEWER_EXECUTE_SQL_TAB);
    else
        selectecd_sql_tab_name = QString(MACRO_SQLITE_VIEWER_BROWSE_DATA_TAB) + "-" + current_tab_name;;

    current_sql_tab = selectecd_sql_tab_name;

    QString cmd = "SELECT file_path FROM 'sv_index' WHERE viewer_display_name = ?";
    QString source_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd ,QStringList(sqlite_viewer_display_name_in_tree_and_tab),0,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite",Q_FUNC_INFO);
    QFileInfo info_temp(source_file_path);
    QString file_path_str = source_file_path;


    QStringList RECON_INT_list;
    QList<struct_global_tagging_sqlite_viewer> sqlite_viewer_record_list;
    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);
        int selected_row_no = model_index.row();
        if(selected_row_no < 0 && selected_row_no >= m_tableWidget_obj->rowCount())
            continue;


        if(m_tableWidget_obj->isRowHidden(selected_row_no))
            continue;

        ///start--------------make list of column's values visible on display
        QStringList col_list = get_data_for_current_record(m_tableWidget_obj);
        ///end----------------make list of column's values visible on display

        QString record_no_str = m_tableWidget_obj->item(selected_row_no,2)->text();
        RECON_INT_list << record_no_str;

        int column_count = m_tableWidget_obj->columnCount();
        m_tableWidget_obj->item(selected_row_no,(column_count -2))->setText(notes_text);
        m_tableWidget_obj->item(selected_row_no,enum_coloumn_Sqlite_viewer_Bookmark)->setIcon(QIcon("../icons/notes.png"));


        ///--export content db
        QString tmp_note_txt = notes_text;
        tmp_note_txt.append(MACRO_RECON_Splitter_1_multiple).append(MACRO_CASE_TREE_Notes_Category_All_Notes);
        create_and_insert_exported_content_info_in_db(selected_row_no, record_no_str ,tmp_note_txt, sqlite_viewer_display_name_in_tree_and_tab, col_list, selectecd_sql_tab_name ,"0");
        ///--export content db


        struct_global_tagging_sqlite_viewer s1;
        s1.INT_record = record_no_str;
        s1.plugin_name = QString(MACRO_Plugin_Name_Sqlite_Viewer);
        s1.tab_name = selectecd_sql_tab_name;
        s1.source_count_name = source_count_name;
        s1.item0 = info_temp.fileName();
        s1.item1 = file_path_str;
        s1.item2 = current_tab_name;
        s1.item3 = sqlite_viewer_display_name_in_tree_and_tab;
        s1.item4 = col_list.join("\n");
        s1.item10 = current_tab_name;
        s1.item11 = sqlite_viewer_display_name_in_tree_and_tab;
        s1.notes_text = notes_text;

        sqlite_viewer_record_list << s1;
    }

    global_connection_manager_obj->pub_set_notes_in_sqlite_viewer_db(RECON_INT_list , sqlite_viewer_display_name_in_tree_and_tab, current_tab_name, notes_text);
    global_connection_manager_obj->pub_add_notes_for_sqlite_viewer_db(sqlite_viewer_record_list ,notes_text);

    global_connection_manager_obj->pub_set_notes_in_tags_search_for_sqlite_viewer(RECON_INT_list,selectecd_sql_tab_name,current_tab_name,sqlite_viewer_display_name_in_tree_and_tab,notes_text);
    global_connection_manager_obj->pub_set_notes_in_bookmark_search_for_sqlite_viewer(RECON_INT_list,selectecd_sql_tab_name,current_tab_name,sqlite_viewer_display_name_in_tree_and_tab,notes_text);

    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::slot_add_notes_to_bookmarks(QString data)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    add_notes_to_bookmarks_dialog_popup->hide();


    struct_global_sqlite_viewer_tablewidget_main obj_main;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        obj_main = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    m_tablewidget *tableWidget = obj_main.tableWidget_MainDataDisplay_ptr;

    int column_count = tableWidget->columnCount();

    for(int i = 0; i < tableWidget->rowCount(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();


        if(tableWidget->item(i,0)->checkState())
        {
            if(tableWidget->item(i,(column_count -2))->text() !="")
                continue;

            tableWidget->item(i,(column_count -2))->setText(data);
            tableWidget->item(i,enum_coloumn_Sqlite_viewer_Bookmark)->setIcon(QIcon("../icons/notes.png"));

            //----to save state ofnotes on add notes to bookmarks --//
        }
    }


    int ret_value = notes_check_savedstate_and_update_main_data_display(1);
    if(ret_value == 1)
    {
        recon_static_functions::app_debug("MainDisplaydb exists there, updating it in case of notes",Q_FUNC_INFO);
    }
    else
    {
        create_or_update_current_save_state_of_sqlite_viewer();
    }

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::slot_arrow_key_pressed_on_table()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    struct_global_sqlite_viewer_tablewidget_main structMainObject;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        structMainObject = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    m_tablewidget *mytable = structMainObject.tableWidget_MainDataDisplay_ptr;

    slot_on_tableWidget_MainDataDisplay_cellClicked(mytable->currentRow(),0);

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::slot_bookmark_key_pressed_tablewidget()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    check_bookmark_status = true;

    struct_global_sqlite_viewer_tablewidget_main structMainObject;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        structMainObject = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    m_tablewidget *mytable = structMainObject.tableWidget_MainDataDisplay_ptr;

    if(mytable->item(mytable->currentRow(), enum_coloumn_Sqlite_viewer_Bookmark)->checkState() == Qt::Checked)
    {
        mytable->item(mytable->currentRow(), enum_coloumn_Sqlite_viewer_Bookmark)->setCheckState(Qt::Unchecked);
    }
    else
    {
        mytable->item(mytable->currentRow(), enum_coloumn_Sqlite_viewer_Bookmark)->setCheckState(Qt::Checked);
    }

    slot_on_tableWidget_MainDataDisplay_cellClicked(mytable->currentRow(),0);

    if(mytable->currentRow() == mytable->rowCount() - 1)
    {
        check_bookmark_status = false;
    }
    //    else
    //    {
    //        mytable->setCurrentCell(mytable->currentRow() + 1, 0);
    //    }

    emit signal_save_openwith_changes_to_bucket(QString(MACRO_CASE_TREE_Sqlite_Viewer));

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::slot_act_columns_selection()
{
    recon_static_functions::app_debug("START ",Q_FUNC_INFO);

    for(int i = 0; i < list_struct_tablewidget_data_sqlite_viewer.size(); i++)
    {
        struct_global_sqlite_viewer_tablewidget_main obj_main = list_struct_tablewidget_data_sqlite_viewer.at(i);

        if(obj_main.table_name == ui->comboBox_dbtables_selection->currentText())
        {
            sqlite_viewer_column_selection_obj = new sqlite_viewer_manage_colums_type(this);

            ret_tableWidget_from_cols_sel = NULL;
            ret_tableWidget_from_cols_sel = obj_main.tableWidget_columns_selection_ptr;

            disconnect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));
            connect(ret_tableWidget_from_cols_sel,SIGNAL(cellChanged(int,int)),this,SLOT(slot_common_tablewidget_col_sel_cell_clicked(int,int)));

            disconnect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));
            connect(ret_tableWidget_from_cols_sel->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_common_tablewidget_col_sel_horizontal_header_clicked(int)));

            table_name_of_cols_sel = obj_main.table_name;
            break;
        }
    }

    sqlite_viewer_column_selection_obj->pub_set_widget_and_tab_name(ret_tableWidget_from_cols_sel,table_name_of_cols_sel);
    sqlite_viewer_column_selection_obj->show();

    recon_static_functions::app_debug("ENDS ",Q_FUNC_INFO);

}

void sqlite_viewer::on_lineEdit_search_returnPressed()
{
    on_pushButton_search_clicked();
}

void sqlite_viewer::slot_show_all(int old_pos,int new_pos)
{
    //    if((old_pos == -1) && (new_pos == 0))
    //    {
    //        return;
    //    }

    //    if((new_pos == 0) && ui->lineEdit_search->text().isEmpty())
    //    {
    //        on_pushButton_show_all_clicked();
    //    }
}

void sqlite_viewer::on_tabWidget_main_data_panel_tabCloseRequested(int index)
{
    ui->tabWidget_main_data_panel->removeTab(index);

    //-Check To disable Closable cross in last tab.
    if(ui->tabWidget_main_data_panel->count() == 1)
    {
        ui->tabWidget_main_data_panel->setTabsClosable(false);
    }
    else
        ui->tabWidget_main_data_panel->setTabsClosable(true);
}

void sqlite_viewer::slot_on_tableWidget_MainDataDisplay_cellClicked(int row, int column)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return;

    current_media_file.clear();

    struct_global_sqlite_viewer_tablewidget_main obj_main;
    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    {
        obj_main= list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    }
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    bool_set_enable_right_click_export = false;

    m_tablewidget *tablewidget_main_data_display;
    tablewidget_main_data_display = obj_main.tableWidget_MainDataDisplay_ptr;


    ///Enable right click after clipboard on dobule click
    if(!tablewidget_main_data_display->selectionModel()->selectedRows(column).isEmpty())
    {
        QList <QAction *> act_list = menu->actions();

        for(int i = 0 ; i < act_list.size() ; i++)
        {
            act_list.at(i)->setEnabled(true);
        }

    }
    else
    {
        if(bool_check_double_click)
        {
            bool_check_double_click = false;
            return;
        }
    }

    bool_check_double_click = false;

    QStringList text_data;
    QString tableName = QString("<b>Table:</b>" +QString("\t")+ obj_main.table_name +"<br>" );

    QTableWidget *tableWidget_columns_selection = obj_main.tableWidget_columns_selection_ptr;

    if(column == 0)
    {
        tablewidget_main_data_display->selectRow(row);
        //---- To Save state when bookmark state changes clicked ----//

        int ret_value = check_savedstate_and_update_main_data_display_single_record();
        if(ret_value == 1)
        {
            recon_static_functions::app_debug("Save State Updated",Q_FUNC_INFO);
        }
        else
        {
            create_or_update_current_save_state_of_sqlite_viewer();
        }
    }

    int bookmark_index = 0;
    if(column == bookmark_index)
    {
        if(tablewidget_main_data_display->item(row, bookmark_index)->checkState() == Qt::Checked)
            update_bookmark_value(QString("1"),row,bookmark_index);
        else if(tablewidget_main_data_display->item(row, bookmark_index)->checkState() == Qt::Unchecked)
            update_bookmark_value(QString("0"),row,bookmark_index);
    }

    for(int i = 1; i < tablewidget_main_data_display->columnCount(); i++)
    {

        QString header;
        header = tablewidget_main_data_display->horizontalHeaderItem(i)->text();

        QString data;
        data = tablewidget_main_data_display->item(row,i)->text();

        if(!(tablewidget_main_data_display->columnCount() -1) || !(tablewidget_main_data_display->columnCount() -2) || !(tablewidget_main_data_display->columnCount() -3))
        {

            if(tablewidget_main_data_display->isColumnHidden(i))
                continue;
        }

        if(i == (tablewidget_main_data_display->columnCount() - 1) || i ==  1){
            continue;
        }

        if((i != (tablewidget_main_data_display->columnCount() -1)) && (i != (tablewidget_main_data_display->columnCount() -2))&&
                (i != (tablewidget_main_data_display->columnCount() -3)))
        {
            if(tablewidget_main_data_display->isColumnHidden(i))
                continue;
        }


        QString convertedData;
        QString recivedComboboxText;

        recivedComboboxText =  (dynamic_cast<QComboBox *>(tableWidget_columns_selection->cellWidget((i) ,2))->currentText());

        if(recivedComboboxText != "Select Conversion")
        {
            convertedData =  convert_data(data,recivedComboboxText);

            if(convertedData.contains("<b>File Exist: </b>	<font  color=\"green\"> <b>YES </b></font><br>"))
            {
                bool_set_enable_right_click_export = true;
                current_media_file = data;
            }

            if(recivedComboboxText != "File")
                text_data << QString("<b>" + header + " (Original Value)" + ":</b>" +QString("\t")+ data) + "<br>" + QString("<b>" + header + " (Converted Value)" + ":</b>" +QString("\t")+ convertedData +"\n");
            else
                text_data << QString("<b>" + header + ":</b>" +QString("\t")+ convertedData +"\n");
        }
        else
        {
            if( i == (tablewidget_main_data_display->columnCount() -2))
            {
                text_data << QString("<b>" + header + ":</b>" +QString("\t")+ "<font  color=\"blue\">" +data + "</font>""\n");
            }
            else if(i == (tablewidget_main_data_display->columnCount() -3))
            {
                QString tag_text = "";
                QStringList list_tags = data.split(",", Qt::SkipEmptyParts);
                for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
                {
                    QString colour = recon_helper_standard_obj->get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
                    tag_text += "<font color = \"" + recon_helper_standard_obj->get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
                }

                if(tag_text.endsWith(","))
                    tag_text.chop(1);

                text_data << QString("<b>" + header + ":</b>" +QString("\t")+ tag_text + "\n");
            }
            else
            {
                text_data << QString("<b>" + header + ":</b>" +QString("\t")+ data +"\n");
            }
        }
    }

    QString tmp_text;
    tmp_text = tableName + text_data.join("<br><br>");

    QString temp_search_string;
    temp_search_string = sqlite_keyword_search.trimmed();

    emit signal_sqlite_viewer_metadata(temp_search_string,tmp_text,current_media_file,"");

    if(tablewidget_main_data_display->item(row, enum_coloumn_Sqlite_viewer_Bookmark)->checkState())
    {
        emit signal_save_openwith_changes_to_bucket(QString(MACRO_CASE_TREE_Sqlite_Viewer));
    }

    //----to assist in detach & Full ---//

    sqlite_viewer_metadata_text.clear();
    sqlite_viewer_current_media_file.clear();
    index_sqlite_viewer_media_type_av_blank_image = 0;

    sqlite_viewer_metadata_text = tmp_text;
    sqlite_viewer_current_media_file = current_media_file;

    if(recon_helper_standard_obj->check_is_it_image_file(sqlite_viewer_current_media_file,"", Q_FUNC_INFO))
    {
        index_sqlite_viewer_media_type_av_blank_image = 1;
    }
    if(recon_helper_standard_obj->check_is_video_by_extn(sqlite_viewer_current_media_file, Q_FUNC_INFO))
    {
        index_sqlite_viewer_media_type_av_blank_image = 2;
    }
    if(recon_helper_standard_obj->check_is_audio_by_extn(sqlite_viewer_current_media_file, Q_FUNC_INFO))
    {
        index_sqlite_viewer_media_type_av_blank_image = 2;
    }

    emit signal_sqlite_viewer_change_content_of_detach_window(sqlite_viewer_metadata_text,sqlite_viewer_current_media_file,sqlite_viewer_current_media_file,index_sqlite_viewer_media_type_av_blank_image,ui->lineEdit_search->text().trimmed(),"");

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::slot_on_tableWidget_MainDataDisplay_celldoubleClicked(int row, int column)
{
    //    m_tablewidget *m_tablewidget_obj = (m_tablewidget*) sender();

    //    m_tablewidget_obj->clearSelection();

    //    QList <QAction *> act_list = menu->actions();

    //    for(int i = 0 ; i < act_list.size() ; i++)
    //    {
    //        if(act_list.at(i)->text().trimmed() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard))
    //            continue;
    //        else
    //            act_list.at(i)->setDisabled(true);
    //    }

    //    emit signal_sqlite_viewer_metadata("","","","");

    //    bool_check_double_click = true;

    //    m_tablewidget_obj->item(row,column)->setSelected(true);
}

void sqlite_viewer::on_tabWidget_main_data_panel_currentChanged(int index)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    bool_index_change_from_comboBox = false;
    bool_set_enable_right_click_export = false;
    bool_combotext_change_on_tab_change = true;

    //--Work To change the Filter View Table On Change Tab
    ///-0. Reset Widget or clear them
    int index_to_clear_tab = 0;

    reset_gui_widgets();

    if(ui->tabWidget_FilterViewSearch->currentIndex() != -1)
        ui->tabWidget_FilterViewSearch->removeTab(index_to_clear_tab);

    current_tab_name = ui->tabWidget_main_data_panel->tabText(ui->tabWidget_main_data_panel->currentIndex());

    for(int i = 0; i < list_struct_tablewidget_data_sqlite_viewer.size(); i++)
    {
        struct_global_sqlite_viewer_tablewidget_main obj_main = list_struct_tablewidget_data_sqlite_viewer.at(i);

        if(obj_main.table_name == current_tab_name)
        {
            current_tab_index = i;

            obj_main.tableWidget_MainDataDisplay_ptr->clearSelection();

            ui->textEdit_query_insert->clear();

            QTableWidget *ptr = obj_main.tableWidget_FilterViewSearch_ptr;

            QString str;
            str += "select ";
            for(int itr = 2; itr < (ptr->rowCount() - 2) ; itr++){

                QString column_name = ptr->item(itr,0)->text().trimmed();

                if(column_name == "RECON INT" || column_name == "RECON Examiner Tags")
                    continue;

                str += column_name;
                str+= QString(",");
            }

            if(str.endsWith(","))
            {
                str.remove(str.size() -1,1);
            }

            str += QString(" from ") +  current_tab_name;

            ui->textEdit_query_insert->setText(str);

            int index_FilterView =  ui->tabWidget_FilterViewSearch->addTab(obj_main.tableWidget_FilterViewSearch_ptr,obj_main.table_name);
            ui->tabWidget_FilterViewSearch->setCurrentIndex(index_FilterView);
            index_to_clear_tab =  ui->tabWidget_FilterViewSearch->currentIndex();

            ///- Active Signals and Slots of Main Tables
            m_tablewidget *tableWidget;
            tableWidget = obj_main.tableWidget_MainDataDisplay_ptr;

            ///-0. Disconnect  for the safty if it is Active before  for metadata.
            disconnect(tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(slot_on_tableWidget_MainDataDisplay_cellClicked(int,int)));

            ///-1. Connection for the single Click tableWidget for metadata.
            connect(tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(slot_on_tableWidget_MainDataDisplay_cellClicked(int,int)));
            connect(tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_on_tableWidget_MainDataDisplay_celldoubleClicked(int,int)));

            bool_sort_bookmarked = true;
            QObject::connect((QObject*)tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_bookmarked(int)));
            load_saved_state_class_obj->pub_set_current_tab_detail(current_tab_name, tableWidget);
            base_core_class_obj->pub_set_current_tab_detail(current_tab_name, tableWidget);
            break;
        }
    }

    ///-3. Establish connection for the TableViewSearch Clicked
    connect_table_filter_view_search_for_cell_clicked(current_tab_index);

    ///-Change Combobiox text
    on_comboBox_dbtables_selection_currentIndexChanged(index);

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::slot_sort_bookmarked(int index)
{
    ///--Work Pending to do.

    return;

    struct_global_sqlite_viewer_tablewidget_main obj_main;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        obj_main = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    m_tablewidget *tableWidget_main_data_display;
    tableWidget_main_data_display = obj_main.tableWidget_MainDataDisplay_ptr;

    if(index == 0)
    {
        set_widgets_disable(true);

        while(tableWidget_main_data_display->rowCount())
            tableWidget_main_data_display->removeRow(0);

        if(bool_sort_bookmarked)
        {
            for(int i = 0; i< tableWidget_main_data_display->rowCount(); i++)
            {
                if(tableWidget_main_data_display->item(i,enum_coloumn_Sqlite_viewer_Bookmark)->checkState())
                {

                }
            }

            bool_sort_bookmarked = false;
        }
        else
        {
            bool_sort_bookmarked = true;
        }

        set_widgets_disable(false);
    }
}

void sqlite_viewer::slot_tab_bar_clicked(int index)
{
    bool status = false;

    if(index == 0)
        status = true;
    else
        status = false;

    emit signal_show_metadata(status);
}

void sqlite_viewer::slot_sub_menu_tags_clicked_for_sqlite_viewer(QAction* current_clicked_action)
{
    action_submenu_tag_triggered_for_sqlite_viewer(current_clicked_action->text());
}

void sqlite_viewer::slot_show_tag()
{
    add_tags_module_obj->show();
}

void sqlite_viewer::slot_remove_tags_from_sqlite_viewer()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);


    struct_global_sqlite_viewer_tablewidget_main obj_main;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        obj_main = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }



    m_tablewidget *m_tableWidget_obj ;
    int column_count;
    QString sql_tab_name;
    int record_number_index;
    int tag_index;
    QModelIndexList selection_model_list;
    QString table_name =  obj_main.table_name;
    if(focusWidget()->objectName() == "tableWidget_query_execution")
    {
        m_tableWidget_obj = (m_tablewidget*)ui->tableWidget_query_execution;
        sql_tab_name = QString(MACRO_SQLITE_VIEWER_EXECUTE_SQL_TAB) + "-" + table_name;
        column_count = m_tableWidget_obj->columnCount();
        selection_model_list = m_tableWidget_obj->selectionModel()->selectedRows();
        record_number_index = 1;
        tag_index = 0;
    }
    else
    {
        m_tableWidget_obj  = obj_main.tableWidget_MainDataDisplay_ptr;
        column_count = m_tableWidget_obj->columnCount();
        selection_model_list = m_tableWidget_obj->selectionModel()->selectedRows();
        sql_tab_name = QString(MACRO_SQLITE_VIEWER_BROWSE_DATA_TAB) + "-" + table_name;
        record_number_index = 2;
        tag_index = 1;

    }

    current_sql_tab = sql_tab_name;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();
        if(selected_row_no < 0 && selected_row_no >= m_tableWidget_obj->rowCount())
            continue;

        if(m_tableWidget_obj->isRowHidden(selected_row_no))
            continue;

        QString record_no;
        if(sql_tab_name.contains(MACRO_SQLITE_VIEWER_EXECUTE_SQL_TAB))
        {
            record_no = QString::number(selected_row_no+1);
        }
        else
            record_no = m_tableWidget_obj->item(selected_row_no,record_number_index)->text();



        QString record_no_prepend_zero = record_no;
        QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_SQLite_Viewer_QString).toString() + "sqlite_viewer_content_info.sqlite";

        QString command = "Select recon_tag_value  from sv_content_info WHERE record_no_in_sqliteviewer = ? AND sql_tab_name = ?";
        QStringList arg_list;
        arg_list << record_no_prepend_zero << sql_tab_name;
        QString tag_to_be_removed = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,exported_content_info_db,Q_FUNC_INFO);


        QIcon icon(QString(""));
        m_tableWidget_obj->item(selected_row_no,enum_coloumn_Sqlite_viewer_Tag_icon)->setIcon(icon);
        int column_count = m_tableWidget_obj->columnCount();
        m_tableWidget_obj->item(selected_row_no,(column_count - 3))->setText("");

        QStringList data_list = get_data_for_current_record(m_tableWidget_obj);
        create_and_insert_exported_content_info_in_db(selected_row_no , record_no_prepend_zero ,"", sqlite_viewer_display_name_in_tree_and_tab, data_list, sql_tab_name , "");


        QString tag_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_to_be_removed + ".sqlite";
        command = QString("Delete from tags Where record_no=? AND plugin_name=? AND tab_name=?");
        arg_list.clear();
        arg_list << record_no_prepend_zero << QString(MACRO_Plugin_Name_Sqlite_Viewer) << sql_tab_name;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,tag_db_path,Q_FUNC_INFO);



        if(sql_tab_name.contains(QString(MACRO_SQLITE_VIEWER_BROWSE_DATA_TAB)))
        {
            QStringList arg_list;
            QString saved_sqlite_path = saved_state_dir_path + "/" + main_datadisplay_table_sqlite_name();
            QString command = QString("UPDATE " + current_tab_name + " SET RECON_Examiner_tags = ? where RECON_INT = ?");
            arg_list << "" << record_no_prepend_zero;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command , arg_list,saved_sqlite_path , Q_FUNC_INFO);
        }


        if(!tag_to_be_removed.trimmed().isEmpty())
            emit signal_common_add_tag_name_into_case_tree(tag_to_be_removed);
    }


    return ;
    //---- To Save state when bookmark all clicked ----//
    int ret_value = check_savedstate_and_update_main_data_display();
    if(ret_value == 1)
    {
        recon_static_functions::app_debug("Save State Updated",Q_FUNC_INFO);
    }
    else
    {
        create_or_update_current_save_state_of_sqlite_viewer();
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void sqlite_viewer::slot_add_tags_to_sqlite_viewer(QString tag_name , QString colour)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    emit signal_save_openwith_changes_to_bucket(QString(MACRO_CASE_TREE_Sqlite_Viewer));

    if(focusWidget()->objectName() == "tableWidget_query_execution")
        add_tags_to_executed_query(tag_name,colour);
    else
        add_tags_to_sqlite_viewer(tag_name, colour);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void sqlite_viewer::slot_update_tags_in_actions_menu(bool status, QString new_tag, QString tag_colour)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        act_already_saved_tags = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags->setIcon(icon);
        sub_menu_tags->addAction(act_already_saved_tags);

        emit signal_update_tag_submenu_of_plugins(status,new_tag,tag_colour);
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void sqlite_viewer::slot_removed_tags_list_sqlite_viewer(QStringList list_removed_tags, QString table_name, QString record_no)
{
    QString record_no_prepend_zero = record_no;

    emit signal_remove_tag_entry_from_tag_search_db_and_display(QString(MACRO_Plugin_Name_Sqlite_Viewer), QString(QString(MACRO_SQLITE_VIEWER_BROWSE_DATA_TAB) + "-" + table_name), record_no_prepend_zero, list_removed_tags, QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Display));
}

void sqlite_viewer::slot_tag_list_after_remove_multiple_tag_sqlite_viewer(QStringList list_tags_remained, QString table_name, QString record)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    struct_global_sqlite_viewer_tablewidget_main obj_main;

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
        obj_main = list_struct_tablewidget_data_sqlite_viewer.at(current_tab_index);
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    m_tablewidget *tableWidget_obj = obj_main.tableWidget_MainDataDisplay_ptr;

    int current_row = tableWidget_obj->currentRow();
    int column_count = tableWidget_obj->columnCount();

    if(current_row < 0)
        return;


    QString record_no = tableWidget_obj->item(tableWidget_obj->currentRow(),2)->text();

    QString tags_str = list_tags_remained.join(",");

    tableWidget_obj->item(current_row,(column_count -3))->setText(tags_str);

    if(tags_str == "")
    {
        QIcon icon(QString(""));
        tableWidget_obj->item(current_row,enum_coloumn_Sqlite_viewer_Tag_icon)->setIcon(icon);
    }
    else if(tags_str.contains(","))
    {
        QString icon_path = "../icons/tag_icons/multiple.png";
        QIcon icon(icon_path);
        tableWidget_obj->item(current_row,enum_coloumn_Sqlite_viewer_Tag_icon)->setIcon(icon);
    }
    else if((tags_str != "") && (!tags_str.contains(",")))
    {
        QString color = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tags_str,QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
        color = color.toLower();
        color = color.replace(" ","_");
        QString icon_path = recon_static_functions::get_icon_path_from_colour_name(color, Q_FUNC_INFO);
        QIcon icon(icon_path);
        tableWidget_obj->item(current_row,enum_coloumn_Sqlite_viewer_Tag_icon)->setIcon(icon);
    }


    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_SQLite_Viewer_QString).toString() + "sqlite_viewer_content_info.sqlite";

    QString command = "UPDATE sv_content_info SET recon_tag_value = ? WHERE record_no_in_sqliteviewer=? AND sql_tab_name = ?";

    QStringList arg_list;
    arg_list << tags_str << record_no << QString(QString(MACRO_SQLITE_VIEWER_BROWSE_DATA_TAB) + "-" + table_name);
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,exported_content_info_db,Q_FUNC_INFO);

    //---- To Save state when bookmark all clicked ----//
    int ret_value = check_savedstate_and_update_main_data_display();
    if(ret_value == 1)
    {
        recon_static_functions::app_debug("Save State Updated",Q_FUNC_INFO);
    }
    else
    {
        create_or_update_current_save_state_of_sqlite_viewer();
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void sqlite_viewer::on_pushButton_search_clicked()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);


    emit signal_sqlite_viewer_metadata("","","","");


    display_loading_progress_bar_obj->pub_set_label_messsge("Searching .....");
    //display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(0);
    display_loading_progress_bar_obj->pub_set_hidden_cancel_button(false);
    bool_cancel_loading = false;
    display_loading_progress_bar_obj->show();

    //int tab_county = ui->tabWidget_main_data_panel->count();

    //    for(int iter=0; iter < tab_county; iter++)
    //    {
    struct_global_sqlite_viewer_tablewidget_main obj_struct_tablewidget_main;

    for(int iterator = 0; iterator < list_struct_tablewidget_data_sqlite_viewer.size(); iterator++)
    {
        if(bool_cancel_loading)
            break;

        obj_struct_tablewidget_main = list_struct_tablewidget_data_sqlite_viewer.at(iterator);
        //if(obj_struct_tablewidget_main.table_name == ui->tabWidget_main_data_panel->tabText(itr))

        if(obj_struct_tablewidget_main.table_name == ui->tabWidget_main_data_panel->tabText(ui->tabWidget_main_data_panel->currentIndex()))
        {
            break;
        }
    }

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    {
        recon_static_functions::app_debug("list_struct_tablewidget_data.size is greater then current_tab_index",Q_FUNC_INFO);
    }
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        display_loading_progress_bar_obj->hide();
        return;
    }

    struct_global_sqlite_viewer_single_dbtable_info struct_obj_table_dbinfo = obj_struct_tablewidget_main.obj_struct_dbtable_detail;

    ///-2.
    QStringList search_list = struct_obj_table_dbinfo.search_list;
    int row_count = obj_struct_tablewidget_main.tableWidget_MainDataDisplay_ptr->rowCount();

    int index_search = search_list.indexOf("1",0);
    if(index_search == -1)
    {
        ui->lineEdit_search->clear();
        for(int v = 0 ; v < row_count; v++)
        {
            obj_struct_tablewidget_main.tableWidget_MainDataDisplay_ptr->showRow(v);
        }

        display_loading_progress_bar_obj->hide();
        return;
    }

    sqlite_keyword_search.clear();
    sqlite_keyword_search = ui->lineEdit_search->text().trimmed();

    QString search_string = ui->lineEdit_search->text().trimmed();
    search_string = search_string.trimmed();
    if(search_string == QString(""))
    {
        display_loading_progress_bar_obj->hide();
        return;
    }

    QStringList list_multiple_keyword_search;
    if(ui->lineEdit_search->text().contains(","))
    {
        QStringList temp_list;
        temp_list = ui->lineEdit_search->text().split(",");

        for(int i = 0; i < temp_list.size(); i++)
        {
            if(temp_list.at(i).trimmed() == "")
                continue;

            list_multiple_keyword_search << temp_list.at(i);
        }
    }
    else
    {
        list_multiple_keyword_search << search_string;
    }

    int mycount = 1;

    obj_struct_tablewidget_main.tableWidget_MainDataDisplay_ptr->clearSelection();

    int search_count = 0;
    for(int i = 0 ; i < row_count ; i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        if(bool_cancel_loading)
            break;


        int flag = 0;
        for(int k = 0 ; k < search_list.size() ;k++)
        {
            if(bool_cancel_loading)
                break;

            if(search_list.at(k) == "0")
                continue;


            QString text = obj_struct_tablewidget_main.tableWidget_MainDataDisplay_ptr->item(i,k)->text().trimmed();

            for(int li = 0; li < list_multiple_keyword_search.size(); li++)
            {

                if(bool_cancel_loading)
                    break;

                if(flag == 1)
                    break;

                QString search_keyword = list_multiple_keyword_search.at(li);

                if(text.contains(search_keyword,Qt::CaseInsensitive))
                {
                    obj_struct_tablewidget_main.tableWidget_MainDataDisplay_ptr->showRow(i);
                    obj_struct_tablewidget_main.tableWidget_MainDataDisplay_ptr->setVerticalHeaderItem(i,new QTableWidgetItem(QString::number(mycount++)));

                    //                    search_count++;
                    //                    if(search_count % 10 == 0)
                    //                    {
                    //                        int progress_value = (search_count * 100) / row_count;

                    //                        if(progress_value > 99)
                    //                            progress_value = 99;

                    //                        display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(progress_value);
                    //                    }

                    flag = 1;
                    break;
                }
            }
        }

        search_count++;
        if(search_count % 10 == 0)
        {
            int progress_value = (search_count * 100) / row_count;

            if(progress_value > 99)
                progress_value = 99;

            display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(progress_value);
        }

        if(flag == 0)
        {
            obj_struct_tablewidget_main.tableWidget_MainDataDisplay_ptr->hideRow(i);
        }
    }
    // }


    display_loading_progress_bar_obj->hide();


    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer::on_pushButton_show_all_clicked()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    sqlite_keyword_search.clear();

    ///-0. Reset All Display widgets

    ui->lineEdit_search->clear();   // we did so becoz linedit should clear only in showall.

    reset_gui_widgets();

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    {
        recon_static_functions::app_debug("list_struct_tablewidget_data.size is greater then current_tab_index",Q_FUNC_INFO);
    }
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        return;
    }

    //=============================================================================//
    display_loading_progress_bar_obj->pub_set_label_messsge("Refreshing Data.....");
    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(0);
    display_loading_progress_bar_obj->pub_set_hidden_cancel_button(true);
    display_loading_progress_bar_obj->show();

    //    int tab_county = ui->tabWidget_main_data_panel->count();
    //    for(int iter=0; iter < tab_county; iter++)
    //    {
    struct_global_sqlite_viewer_tablewidget_main obj_struct_tablewidget_main;

    for(int iterator = 0; iterator < list_struct_tablewidget_data_sqlite_viewer.size(); iterator++)
    {
        obj_struct_tablewidget_main = list_struct_tablewidget_data_sqlite_viewer.at(iterator);
        if(obj_struct_tablewidget_main.table_name == ui->tabWidget_main_data_panel->tabText(ui->tabWidget_main_data_panel->currentIndex()))
        {
            break;
        }
    }

    if(list_struct_tablewidget_data_sqlite_viewer.size() > current_tab_index)
    {
        recon_static_functions::app_debug("list_struct_tablewidget_data.size is greater then current_tab_index",Q_FUNC_INFO);
    }
    else
    {
        recon_static_functions::app_debug("index out of range ------FAILED----",Q_FUNC_INFO);
        display_loading_progress_bar_obj->hide();
        return;
    }

    struct_global_sqlite_viewer_single_dbtable_info struct_obj_table_dbinfo = obj_struct_tablewidget_main.obj_struct_dbtable_detail;

    ///-2.
    QStringList search_list = struct_obj_table_dbinfo.search_list;
    int row_count = obj_struct_tablewidget_main.tableWidget_MainDataDisplay_ptr->rowCount();

    int index_search = search_list.indexOf("1",0);
    if(index_search == -1)
    {
        ui->lineEdit_search->clear();
        for(int v = 0 ; v < row_count; v++)
        {
            obj_struct_tablewidget_main.tableWidget_MainDataDisplay_ptr->showRow(v);
        }

        display_loading_progress_bar_obj->hide();
        return;
    }


    int mycount = 0;

    obj_struct_tablewidget_main.tableWidget_MainDataDisplay_ptr->clearSelection();

    int processed_count = 0;
    for(int i = 0 ; i < row_count ; i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        mycount++;
        processed_count++;

        for(int k = 0 ; k < search_list.size() ;k++)
        {
            if(search_list.at(k) == "0")
                continue;

            obj_struct_tablewidget_main.tableWidget_MainDataDisplay_ptr->showRow(i);
            obj_struct_tablewidget_main.tableWidget_MainDataDisplay_ptr->setVerticalHeaderItem(i,new QTableWidgetItem(QString::number(mycount)));
            if(processed_count % 10 == 0)
            {
                int progress_value = (processed_count * 100) / row_count;

                if(progress_value > 99)
                    progress_value = 99;

                display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(progress_value);
            }
        }
    }


    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("END",Q_FUNC_INFO);

    return;
    //=================================================================================//

    //    display_loading_progress_bar_obj->pub_set_label_messsge("Refreshing Data.....");
    //    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    //    display_loading_progress_bar_obj->pub_set_hidden_cancel_button(true);
    //    display_loading_progress_bar_obj->show();

    //    int tab_county = ui->tabWidget_main_data_panel->count();
    //    for(int itr =0 ; itr < tab_county; itr++)
    //    {
    //        struct_global_sqlite_viewer_tablewidget_main obj_struct_tablewidget_main;

    //        for(int iter=0; iter < list_struct_tablewidget_data_sqlite_viewer.size(); iter++)
    //        {
    //            obj_struct_tablewidget_main = list_struct_tablewidget_data_sqlite_viewer.at(iter);

    //            if(ui->tabWidget_main_data_panel->tabText(itr) == obj_struct_tablewidget_main.table_name)
    //            {
    //                break;
    //            }
    //        }

    //        QString table_name = obj_struct_tablewidget_main.table_name;



    //        refresh_table_widget_data(obj_struct_tablewidget_main.tableWidget_MainDataDisplay_ptr , table_name);
    //    }

    //    display_loading_progress_bar_obj->hide();

}

void sqlite_viewer::on_pushButton_generate_report_clicked()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);
    display_loading_progress_bar_obj->pub_set_label_messsge("Building Report Please Wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_hidden_cancel_button(false);
    display_loading_progress_bar_obj->show();

    QString current_report_destination;
    current_report_destination.clear();

    current_report_destination = global_narad_muni_class_obj->get_field(MACRO_NARAD_Report_Location_SQLite_Viewer_In_Result_QString).toString();

    sqlite_viewer_export_db_path.clear();
    sqlite_viewer_export_db_path = current_report_destination;

    QTime time;
    time = QTime::currentTime();
    QString time_string = time.toString("HH-mm-ss");

    QDate m_date = QDate::currentDate();
    QString date_string = m_date.toString("MM-dd-yyyy");


    QString report_scope;
    QString report_type;


    if(ui->comboBox_report_content_selection->currentText() == QString("Full"))
        report_scope = MACRO_REPORT_SCOPE_FULL_QString;
    else if(ui->comboBox_report_content_selection->currentText() == QString(MACRO_Tag_Name_Bookmarks))
        report_scope = MACRO_REPORT_SCOPE_TAGS_QString;
    else if(ui->comboBox_report_content_selection->currentText() == QString("Screen Items"))
        report_scope = MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString;

    if(ui->comboBox_report_type_selection->currentText() == QString("HTML"))
        report_type = MACRO_REPORT_TYPE_HTML_STANDARD_QString;
    else if(ui->comboBox_report_type_selection->currentText() == QString("PDF"))
        report_type = MACRO_REPORT_TYPE_PDF_QString;
    else if(ui->comboBox_report_type_selection->currentText() == QString("XML"))
        report_type = MACRO_REPORT_TYPE_XML_QString;
    else if(ui->comboBox_report_type_selection->currentText() == QString("CSV"))
        report_type = MACRO_REPORT_TYPE_CSV_QString;
    else if(ui->comboBox_report_type_selection->currentText() == QString("KML"))
        report_type = MACRO_REPORT_TYPE_KML_QString;


    QString sqlite_plugin_name =  QString(MACRO_Plugin_Name_Sqlite_Viewer);
    QString report_scope_full_str =  QString(MACRO_REPORT_SCOPE_FULL_QString);
    QString report_scope_tag_str =  QString(MACRO_REPORT_SCOPE_TAGS_QString);
    QString report_scope_screenitem_str =  QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString);

    if(report_scope == QString(MACRO_REPORT_SCOPE_FULL_QString))
    {
        reports_dir_final_path = current_report_destination + "/" +  sqlite_plugin_name.replace(" ","_") + "_" + report_scope_full_str.replace(" ","_") + "_" + date_string + "T" + time_string;
    }

    if(report_scope == QString(MACRO_REPORT_SCOPE_TAGS_QString))
    {
        reports_dir_final_path = current_report_destination + "/" + sqlite_plugin_name.replace(" ","_") + "_" + report_scope_tag_str.replace(" ","_") + "_" + date_string + "T" + time_string;
    }

    if(report_scope.contains(QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString)))
    {
        reports_dir_final_path = current_report_destination + "/" + sqlite_plugin_name.replace(" ","_") + "_" + report_scope_screenitem_str.replace(" ","_") + "_" + date_string + "T" + time_string;
    }

    if(bool_cancel_loading)
        return;

    QDir dir;
    if(!dir.mkpath(reports_dir_final_path))
    {
        recon_static_functions::app_debug("dir make path FAILED------>>>  " + reports_dir_final_path,Q_FUNC_INFO);
    }

    if(report_type == MACRO_REPORT_TYPE_HTML_STANDARD_QString || report_type == MACRO_REPORT_TYPE_PDF_QString)
    {
        QString resource_dir_path =   "../Resources/graphics/html_reports";
        QString new_resource_path = reports_dir_final_path + "/resources";

        dir.mkpath(new_resource_path);

        recon_static_functions::copy_directory(resource_dir_path,new_resource_path, Q_FUNC_INFO);

        //  QFile::copy("../icons/agency_logo.png",new_resource_path + "/agency_logo.png");
        QString agency_logo_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "agency_logo.png";
        QFile::copy(agency_logo_path,new_resource_path + "/agency_logo.png");


        QFile::copy("../icons/Features/sqLite_viewer.png",new_resource_path + "/sqlite_viewer.png");
        QFile::copy("../icons/open_file_link.png", new_resource_path + "/open_file_link.png");

    }

    QString reoort_final_path;

    int is_pdf = 0;

    if(report_type == MACRO_REPORT_TYPE_HTML_STANDARD_QString && report_scope == QString(MACRO_REPORT_SCOPE_FULL_QString))
    {
        reoort_final_path.clear();
        reoort_final_path =  (reports_dir_final_path + "/index.html");

        sqlite_viewer_create_html_pdf_report(0,reoort_final_path,MACRO_SQLITE_VIEWER_FULL_REPORT_int);
    }
    if(report_type == MACRO_REPORT_TYPE_HTML_STANDARD_QString && report_scope == QString(MACRO_REPORT_SCOPE_TAGS_QString))
    {
        reoort_final_path.clear();
        reoort_final_path =  (reports_dir_final_path + "/index.html");

        sqlite_viewer_create_html_pdf_report(0,reoort_final_path,MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int);
    }
    if(report_type == MACRO_REPORT_TYPE_HTML_STANDARD_QString && report_scope == QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString))
    {
        reoort_final_path.clear();
        reoort_final_path =  (reports_dir_final_path + "/index.html");

        sqlite_viewer_create_html_pdf_report(0,reoort_final_path,MACRO_SQLITE_VIEWER_SCREEN_REPORT_int);
    }

    if(report_type == MACRO_REPORT_TYPE_PDF_QString && report_scope == QString(MACRO_REPORT_SCOPE_FULL_QString) )
    {
        reoort_final_path.clear();
        reoort_final_path =  (reports_dir_final_path + "/index.html");

        sqlite_viewer_create_html_pdf_report(1, reoort_final_path,MACRO_SQLITE_VIEWER_FULL_REPORT_int);

        is_pdf = 1;
    }
    if(report_type == MACRO_REPORT_TYPE_PDF_QString && report_scope == QString(MACRO_REPORT_SCOPE_TAGS_QString) )
    {
        reoort_final_path.clear();
        reoort_final_path =  (reports_dir_final_path + "/index.html");

        sqlite_viewer_create_html_pdf_report(1, reoort_final_path,MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int);

        is_pdf = 1;
    }
    if(report_type == MACRO_REPORT_TYPE_PDF_QString && report_scope == QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString))
    {
        reoort_final_path.clear();
        reoort_final_path =  (reports_dir_final_path + "/index.html");

        sqlite_viewer_create_html_pdf_report(1, reoort_final_path,MACRO_SQLITE_VIEWER_SCREEN_REPORT_int);

        is_pdf = 1;
    }



    if(report_type == MACRO_REPORT_TYPE_CSV_QString && report_scope == QString(MACRO_REPORT_SCOPE_FULL_QString)  )
    {
        reoort_final_path.clear();
        reoort_final_path=  (reports_dir_final_path + "/index.csv");

        sqlite_viewer_create_csv_report(reoort_final_path,MACRO_SQLITE_VIEWER_FULL_REPORT_int);
    }
    if(report_type == MACRO_REPORT_TYPE_CSV_QString && report_scope == QString(MACRO_REPORT_SCOPE_TAGS_QString))
    {
        reoort_final_path.clear();
        reoort_final_path =  (reports_dir_final_path + "/index.csv");

        sqlite_viewer_create_csv_report(reoort_final_path,MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int);
    }
    if(report_type == MACRO_REPORT_TYPE_CSV_QString && report_scope == QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString))
    {
        reoort_final_path.clear();
        reoort_final_path =  (reports_dir_final_path + "/index.csv");

        sqlite_viewer_create_csv_report(reoort_final_path,MACRO_SQLITE_VIEWER_SCREEN_REPORT_int);
    }



    if(report_type == MACRO_REPORT_TYPE_XML_QString && report_scope == QString(MACRO_REPORT_SCOPE_FULL_QString))
    {
        reoort_final_path.clear();
        reoort_final_path =  (reports_dir_final_path + "/index.xml");

        sqlite_viewer_create_xml_report(reoort_final_path,MACRO_SQLITE_VIEWER_FULL_REPORT_int);
    }
    if(report_type == MACRO_REPORT_TYPE_XML_QString && report_scope == QString(MACRO_REPORT_SCOPE_TAGS_QString))
    {
        reoort_final_path.clear();
        reoort_final_path =  (reports_dir_final_path + "/index.xml");

        sqlite_viewer_create_xml_report(reoort_final_path,MACRO_SQLITE_VIEWER_BOOKMARK_REPORT_int);
    }
    if(report_type == MACRO_REPORT_TYPE_XML_QString && report_scope == QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString))
    {
        reoort_final_path.clear();
        reoort_final_path =  (reports_dir_final_path + "/index.xml");
        sqlite_viewer_create_xml_report(reoort_final_path,MACRO_SQLITE_VIEWER_SCREEN_REPORT_int);
    }

    display_loading_progress_bar_obj->hide();

    if(is_pdf == 1){

        recon_static_functions::app_debug("pdf file",Q_FUNC_INFO);
    }
    else
    {
        if(bool_cancel_loading)
            return;
        set_sqlite_viewer_report_generation_status(reoort_final_path);
    }

    if(ui->checkBox_export->isChecked()){
        QFile::remove(export_db_destination_path);
    }

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void sqlite_viewer::on_pushButton_generate_report_execute_query_clicked()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    display_loading_progress_bar_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_hidden_cancel_button(false);
    display_loading_progress_bar_obj->show();
    
    QTime time;
    time = QTime::currentTime();
    QString time_string = time.toString("HH-mm-ss");

    QDate m_date = QDate::currentDate();
    QString date_string = m_date.toString("MM-dd-yyyy");

    QString current_report_destination;
    current_report_destination.clear();

    //current_report_destination = sqlite_viewer_reports_destination_path;
    current_report_destination = global_narad_muni_class_obj->get_field(MACRO_NARAD_Report_Location_SQLite_Viewer_In_Result_QString).toString();

    QString report_type;
    if(ui->comboBox_report_type_selection_execute_query->currentText() == QString("HTML"))
        report_type = MACRO_REPORT_TYPE_HTML_STANDARD_QString;
    else if(ui->comboBox_report_type_selection_execute_query->currentText() == QString("PDF"))
        report_type = MACRO_REPORT_TYPE_PDF_QString;
    else if(ui->comboBox_report_type_selection_execute_query->currentText() == QString("XML"))
        report_type = MACRO_REPORT_TYPE_XML_QString;
    else if(ui->comboBox_report_type_selection_execute_query->currentText() == QString("CSV"))
        report_type = MACRO_REPORT_TYPE_CSV_QString;
    else if(ui->comboBox_report_type_selection_execute_query->currentText() == QString("KML"))
        report_type = MACRO_REPORT_TYPE_KML_QString;



    QString sqlite_plugin_name =  QString(MACRO_Plugin_Name_Sqlite_Viewer);
    if(report_type.contains(MACRO_REPORT_TYPE_HTML_STANDARD_QString) || report_type.contains(MACRO_REPORT_TYPE_PDF_QString) || report_type.contains(MACRO_REPORT_TYPE_CSV_QString) || report_type.contains(MACRO_REPORT_TYPE_XML_QString))
    {
        reports_dir_final_path = current_report_destination + "/" + sqlite_plugin_name.replace(" ","_") + "_" + "Query_Execution" + "_" + date_string + "T" + time_string;
    }

    QDir dir;

    if(!dir.mkpath(reports_dir_final_path))
    {
        recon_static_functions::app_debug("dir make path Fail  " + reports_dir_final_path,Q_FUNC_INFO);
    }

    QDir dir_2;
    if(report_type == MACRO_REPORT_TYPE_HTML_STANDARD_QString || report_type == MACRO_REPORT_TYPE_PDF_QString)
    {
        QString resource_dir_path =   "../Resources/graphics/html_reports";

        QString new_resource_path = reports_dir_final_path + "/resources";

        dir_2.mkpath(new_resource_path);

        recon_static_functions::copy_directory(resource_dir_path,new_resource_path, Q_FUNC_INFO);

        // QFile::copy("../icons/agency_logo.png",new_resource_path + "/agency_logo.png");
        QString agency_logo_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "agency_logo.png";
        QFile::copy(agency_logo_path,new_resource_path + "/agency_logo.png");

        QFile::copy("../icons/Features/sqLite_viewer.png",new_resource_path + "/sqlite_viewer.png");
        QFile::copy("../icons/open_file_link.png", new_resource_path + "/open_file_link.png");

    }

    if(bool_cancel_loading)
        return;

    QString report_final_path;
    int is_pdf = 0;

    if(report_type == MACRO_REPORT_TYPE_HTML_STANDARD_QString)
    {
        report_final_path =  (reports_dir_final_path + "/" + sqlite_plugin_name.replace(" ","_")  + "_" + "Execute_SQL" + "_" + date_string + "T" + time_string + ".html");
        create_screen_html_pdf_report_for_query_execution(0,report_final_path);
    }

    if(report_type == MACRO_REPORT_TYPE_PDF_QString)
    {
        report_final_path =  (reports_dir_final_path + "/" + sqlite_plugin_name.replace(" ","_")  + "_" + "Execute_SQL" + "_" + date_string + "T" + time_string + ".html");
        create_screen_html_pdf_report_for_query_execution(1,report_final_path);

        is_pdf = 1;
    }

    if(report_type == MACRO_REPORT_TYPE_CSV_QString)
    {
        report_final_path =  (reports_dir_final_path + "/" + sqlite_plugin_name.replace(" ","_")  + "_" + "Execute_SQL" + "_" + date_string + "T" + time_string + ".csv");
        create_Screen_csv_report_for_query_execution(report_final_path);
    }

    if(report_type == MACRO_REPORT_TYPE_XML_QString)
    {
        report_final_path =  (reports_dir_final_path + "/" + sqlite_plugin_name.replace(" ","_")  + "_" + "Execute_SQL" + "_" + date_string + "T" + time_string + ".xml");
        create_Screen_xml_report_for_query_execution(report_final_path);
    }

    display_loading_progress_bar_obj->hide();

    if(is_pdf == 1)
    {
        QString src_fl_path = report_final_path;
        report_file_path.clear();
        report_final_path.chop(QString("html").size()); //html
        report_final_path.append("pdf");
        report_file_path = report_final_path;

        emit signal_create_pdf_report(src_fl_path, report_final_path);
        recon_static_functions::app_debug("pdf file",Q_FUNC_INFO);
    }
    else
    {
        if(bool_cancel_loading)
            return;
        set_sqlite_viewer_report_generation_status(report_final_path);
    }

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

