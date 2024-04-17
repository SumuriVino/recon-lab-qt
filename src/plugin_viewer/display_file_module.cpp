#include "plugin_viewer.h"
#include "ui_plugin_viewer.h"


/*
 Calling Function:   pub_display_data  File:library_interface.cpp
 Arguments:          Nil
 Return:             void
 Function:           Intialize array of QTableWidget pointer with Ui table-widgets adresses.
 */


void plugin_viewer::initialize_tablewidget_array()
{
    list_tablewidgets.clear();

    display_tablewidget_table_name_list.clear();

    for(int pp = 0; pp < tab_name_list_tablewidget.size(); pp++)
    {
        m_tablewidget *widget = new m_tablewidget(this);
        widget->horizontalHeader()->setSectionsMovable(true);
        widget->setSelectionMode(QAbstractItemView::ExtendedSelection);

        list_tablewidgets << widget;

        widget->pub_set_tab_name(tab_name_list_tablewidget.at(pp));

        display_tablewidget_table_name_list << table_name_list_tablewidget.at(pp);
        //ui->tabWidget->insertTab(ui->tabWidget->count(),widget,tab_name_list_tablewidget.at(pp));
        QString tab_name = table_name_list_tablewidget.at(pp);
        int record_count = get_total_record_count_in_plugin_tab(tab_name);

        ui->tabWidget->insertTab(ui->tabWidget->count(),widget,tab_name_list_tablewidget.at(pp) + QString(MACRO_Plugin_tab_record_braces_open) + QString::number(record_count) + QString(MACRO_Plugin_tab_record_braces_close));
        ui->tabWidget->setContentsMargins(0,2,0,0);
        //tabWidget->tabBar()->setTabTextColor(1,Qt::white);

        connect(widget,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tableWidget_cellClicked(int,int)));
        connect(widget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_tableWidget_doubleClicked(int,int)));
        connect(widget, SIGNAL(signal_spacebar_quicklook_pressed()), this, SLOT(slot_spacebar_quicklook_pressed()));

//        connect(widget, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));
    }


    return;
}


void plugin_viewer::initialize_textedit_array()
{
    list_texteditwidgets.clear();
    list_checkbox_textedit_widgets.clear();

    for(int pp = 0; pp < tab_name_list_textedit.size(); pp++)
    {

        m_textedit *widget_text = new m_textedit(this);
        m_checkbox *widget_check = new m_checkbox(this);

        list_texteditwidgets << widget_text;
        list_checkbox_textedit_widgets << widget_check;

        QVBoxLayout *lay = new QVBoxLayout;
        lay->addWidget(widget_check);
        lay->addWidget(widget_text);
        lay->setContentsMargins(4,0,4,2);
        lay->setSpacing(4);

        QWidget *widget = new QWidget;
        widget->setLayout(lay);


        //  ui->tabWidget->addTab(widget,struct_obj.table_tab_name);
        //        ui->tabWidget->insertTab(ui->tabWidget->count(),widget,tab_name_list_textedit.at(pp));
        QString tab_name = table_name_list_textedit.at(pp);
        int record_count = get_total_record_count_in_plugin_tab(tab_name);

        ui->tabWidget->insertTab(ui->tabWidget->count(),widget,tab_name_list_textedit.at(pp) + QString(MACRO_Plugin_tab_record_braces_open) + QString::number(record_count) + QString(MACRO_Plugin_tab_record_braces_close));
        ui->tabWidget->setContentsMargins(0,2,0,0);


        //ui->tabWidget->setContentsMargins(4,0,4,2);
        connect(widget_check,SIGNAL(toggled(bool)),this,SLOT(slot_m_checkbox_toggled_textedit(bool)));

    }


    return;

}

void plugin_viewer::initialize_stack_array()
{
    list_stackedwidgetwidgets.clear();
    list_checkbox_stackedwidget_widgets.clear();

    for(int pp = 0; pp < tab_name_list_stack.size(); pp++)
    {

        m_stackedwidget *widget_stack = new m_stackedwidget(this);
        m_checkbox *widget_check = new m_checkbox(this);

        list_stackedwidgetwidgets << widget_stack;
        list_checkbox_stackedwidget_widgets << widget_check;

        QVBoxLayout *lay = new QVBoxLayout;
        lay->addWidget(widget_check);
        lay->addWidget(widget_stack);
        lay->setContentsMargins(4,0,4,2);
        lay->setSpacing(4);

        QWidget *widget = new QWidget;
        widget->setLayout(lay);

        // ui->tabWidget->addTab(widget,struct_obj.table_tab_name);
        //        ui->tabWidget->insertTab(ui->tabWidget->count(),widget,tab_name_list_stack.at(pp));
        QString tab_name = table_name_list_stack.at(pp);
        int record_count = get_total_record_count_in_plugin_tab(tab_name);

        ui->tabWidget->insertTab(ui->tabWidget->count(),widget,tab_name_list_stack.at(pp) + QString(MACRO_Plugin_tab_record_braces_open) + QString::number(record_count) + QString(MACRO_Plugin_tab_record_braces_close));
        ui->tabWidget->setContentsMargins(0,2,0,0);

        connect(widget_check,SIGNAL(toggled(bool)),this,SLOT(slot_m_checkbox_toggled_stack(bool)));

    }


    return;
}

void plugin_viewer::initialize_tree_array()
{

    list_treewidgets.clear();
    for(int pp = 0; pp < tab_name_list_tree.size(); pp++)
    {

        m_treewidget *widget = new m_treewidget(this);
        list_treewidgets << widget;
        // ui->tabWidget->addTab(widget,struct_obj.table_tab_name);
        //        ui->tabWidget->insertTab(ui->tabWidget->count(),widget,tab_name_list_tree.at(pp));
        QString tab_name = table_name_list_tree.at(pp);
        int record_count = get_total_record_count_in_plugin_tab(tab_name);

        ui->tabWidget->insertTab(ui->tabWidget->count(),widget,tab_name_list_tree.at(pp) + QString(MACRO_Plugin_tab_record_braces_open) + QString::number(record_count) + QString(MACRO_Plugin_tab_record_braces_close));
        ui->tabWidget->setContentsMargins(0,2,0,0);

        connect(widget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(slot_m_treewidget_itemclicked(QTreeWidgetItem*,int)));

    }

    return;
}

void plugin_viewer::initialize_thumbnail_viewer_array()
{
    list_thumbnail_viewer_widgets.clear();

    st_gallery_view_file_info_QList_QList.clear();

    for(int pp = 0; pp < tab_name_list_thumbnail.size(); pp++)
    {
        if(!display_tablewidget_table_name_list.contains(table_name_list_thumbnail.at(pp)))
            continue;

        gallery_viewer *widget = new gallery_viewer(this);
        widget->pub_create_ui();

        list_thumbnail_viewer_widgets << widget;


        QList<struct_global_gallery_view_file_info> *t_list = new QList<struct_global_gallery_view_file_info>;
        t_list->clear();
        st_gallery_view_file_info_QList_QList.append(t_list);

        QString tab_name = table_name_list_thumbnail.at(pp);
        int record_count = get_total_record_count_in_plugin_tab(tab_name);

        ui->tabWidget->insertTab(ui->tabWidget->count(),widget,tab_name_list_thumbnail.at(pp) + QString(MACRO_Plugin_tab_record_braces_open) + QString::number(record_count) + QString(MACRO_Plugin_tab_record_braces_close));
        ui->tabWidget->setContentsMargins(0,2,0,0);



        connect(widget, SIGNAL(signal_gallery_bookmark_value_changed(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_bookmark_value_changed(struct_global_gallery_view_file_info)));
        connect(widget, SIGNAL(signal_gallery_viewer_item_clicked(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_view_item_clicked(struct_global_gallery_view_file_info)));
        connect(widget, SIGNAL(signal_gallery_view_spacebar_pressed(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_spacebar_quicklook_pressed(struct_global_gallery_view_file_info)));
        connect(widget, SIGNAL(signal_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info)), this, SLOT(slot_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info)));
        connect(widget, SIGNAL(signal_gallery_view_invalid_item_clicked()), this, SLOT(slot_gallery_view_invalid_tem_clicked()));

        connect(this, SIGNAL(signal_set_bookmark_status_to_gallery_view(bool)), widget, SLOT(slot_set_bookmark_status_to_gallery_view(bool)));

    }


    return;

}

void plugin_viewer::initialise_chat_viewer_array()
{
    list_chat_viewer_widgets.clear();
    for(int pp = 0; pp < tab_name_list_chat_viewer.size(); pp++)
    {
        if(!display_tablewidget_table_name_list.contains(table_name_list_chat_viewer.at(pp)))
            continue;

        chat_viewer *chat_viewer_obj = new chat_viewer(this);
        chat_viewer_obj->pub_set_plugin_name(get_plugin_name());
        chat_viewer_obj->pub_set_tab_name(tab_name_list_chat_viewer.at(pp));
        chat_viewer_obj->create_ui();

        list_chat_viewer_widgets << chat_viewer_obj;
        //        ui->tabWidget->insertTab(ui->tabWidget->count(),chat_viewer_obj,tab_name_list_chat_viewer.at(pp));
        QString tab_name = table_name_list_chat_viewer.at(pp);
        int record_count = get_total_record_count_in_plugin_tab(tab_name);

        ui->tabWidget->insertTab(ui->tabWidget->count(),chat_viewer_obj,tab_name_list_chat_viewer.at(pp) + QString(MACRO_Plugin_tab_record_braces_open) + QString::number(record_count) + QString(MACRO_Plugin_tab_record_braces_close));
        ui->tabWidget->setContentsMargins(0,2,0,0);


    }
}


void plugin_viewer::draw_thumbnail(QString table_name, int m_index)
{
    recon_static_functions::app_debug(" start table name - " + table_name,Q_FUNC_INFO);

    if(list_thumbnail_viewer_widgets.size() <= m_index)
    {
        recon_static_functions::app_debug(" Mismatch thumbnail count ---FAILED--- " + table_name + " , index " + QString::number(m_index),Q_FUNC_INFO);
        return;
    }


    QString tab_nm =  global_csv_reader_class_selected_obj->get_tab_name_associated_with_widget(get_plugin_name(), table_name, MACRO_CSV_TEMPLATE_TAB_TYPE_TABLEWIDGET);

    int tabindx_tblwdgt = 0;
    bool bool_tblwdget_indx_found = false;
    for(int zz = 0; zz < list_tablewidgets.size(); zz++)
    {
        if(list_tablewidgets.at(zz)->pub_get_tab_name() == tab_nm)
        {
            tabindx_tblwdgt = zz;
            bool_tblwdget_indx_found = true;
            break;
        }

    }

    if(!bool_tblwdget_indx_found)
        return;

    QList<struct_global_gallery_view_file_info> *st_gallery_view_file_info_list = st_gallery_view_file_info_QList_QList.at(m_index);

    st_gallery_view_file_info_list->clear();

    m_tablewidget *my_tablewidget =  list_tablewidgets.at(tabindx_tblwdgt);

    if(tw_col_order_list_tablewidget.size() != tw_data_type_list_tablewidget.size())
        return;


    int record_order = -1;
    int bkmk_order = -1;
    int file_name_order = -1;
    int file_path_order = -1;
    int timestmp_order = -1;


    QStringList dt_type_list = tw_data_type_list_tablewidget.at(tabindx_tblwdgt);
    QStringList ordr_list = tw_col_order_list_tablewidget.at(tabindx_tblwdgt);

    int indx = dt_type_list.indexOf(QString(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString));
    if(indx != -1)
    {
        bkmk_order = QString(ordr_list.value(indx)).toInt();
    }
    indx = dt_type_list.indexOf(QString(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString));
    if(indx != -1)
    {
        record_order = QString(ordr_list.value(indx)).toInt();
    }
    indx = dt_type_list.indexOf(QString(MACRO_CSV_TEMPLATE_DATATYPE_THUMBNAIL_FILE_NAME_QString));
    if(indx != -1)
    {
        file_name_order = QString(ordr_list.value(indx)).toInt();
    }
    indx = dt_type_list.indexOf(QString(MACRO_CSV_TEMPLATE_DATATYPE_THUMBNAIL_FILE_PATH_QString));
    if(indx != -1)
    {
        file_path_order = QString(ordr_list.value(indx)).toInt();
    }
    indx = dt_type_list.indexOf(QString(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString));
    if(indx != -1)
    {
        timestmp_order = QString(ordr_list.value(indx)).toInt();
    }




    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("-------------------------FAILED----------------------- to open DB" + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:-" + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    for(int mm = 0; mm < my_tablewidget->rowCount(); mm++)
    {
        if(my_tablewidget->isRowHidden(mm))
            continue;

        QString record_val;
        QString bkmk_val;
        QString file_name_val;
        QString file_path_val;
        QString timestmp_val;


        if(bkmk_order != -1)
        {
            if(my_tablewidget->item(mm, bkmk_order)->checkState() == Qt::Checked)
                bkmk_val = "1" ;
            else
                bkmk_val = "0" ;

        }
        if(record_order != -1)
        {
            record_val = my_tablewidget->item(mm, record_order)->text();
        }
        if(file_name_order != -1)
        {
            file_name_val = my_tablewidget->item(mm, file_name_order)->text();
        }


        if(file_path_order != -1)
        {
            file_path_val = my_tablewidget->item(mm, file_path_order)->text();
        }

        if(timestmp_order != -1)
        {
            timestmp_val =  my_tablewidget->item(mm, timestmp_order)->text();
        }



        struct_global_gallery_view_file_info f1;
        f1.file_name = file_name_val;

        f1.file_path = file_path_val;
        struct_global_recon_file_info st_recon_f_info_obj  = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbreference(get_plugin_name() , tab_nm, record_val,destination_db);
        f1.thumbnail_file_path = st_recon_f_info_obj.complete_file_path;
        f1.full_filepath = st_recon_f_info_obj.complete_file_path;
        f1.source_count_name = st_recon_f_info_obj.source_count_name;
        f1.INT = record_val;
        f1.table_view_row_no = mm;
        f1.bookmark = bkmk_val;
        f1.latitude = "";
        f1.longitude = "";
        f1.last_access_timestamp = timestmp_val;
        f1.index = st_gallery_view_file_info_list->size();


        st_gallery_view_file_info_list->append(f1);

    }

    destination_db.close();

    gallery_viewer *widget = list_thumbnail_viewer_widgets.at(m_index);
    widget->pub_set_file_list(st_gallery_view_file_info_list);
    QCoreApplication::processEvents();


    recon_static_functions::app_debug(" end table name - " + table_name,Q_FUNC_INFO);

}


void plugin_viewer::draw_tree(int index, QString table_name, QString parent_column_name, QString parent_datatype,QStringList col_name_list, QStringList col_type_list, QStringList col_width_list)
{
    recon_static_functions::app_debug(" start table name - " + table_name,Q_FUNC_INFO);

    if(list_treewidgets.size() <= index)
        return;

    m_treewidget *my_treewidget = list_treewidgets.at(index);

    my_treewidget->clear();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("-------------------------FAILED----------------------- to open DB" + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:-" + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_parent(destination_db);

    //    QString command_parent = "select " + parent_column_name + " , bookmark from " + table_name + " where " + parent_column_name + " IS NOT NULL  group by " + parent_column_name;
    QString command_parent = "select " + parent_column_name + " , bookmark from " + table_name + " group by " + parent_column_name;

    if(!query_parent.exec(command_parent))
    {
        recon_static_functions::app_debug(" -------------------------FAILED----------------------- to execute query_parent " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command - " + command_parent,Q_FUNC_INFO);
        recon_static_functions::app_debug(" last error -" + query_parent.lastError().text(),Q_FUNC_INFO);

        destination_db.close();
        return;
    }

    QStringList parent_list;
    QStringList bookmarked_record_list;
    while(query_parent.next())
    {
        parent_list <<  query_parent.value(0).toString();
        bookmarked_record_list <<  query_parent.value(1).toString();
    }


    if(bookmarked_record_list.size() != parent_list.size())
    {
        destination_db.close();
        recon_static_functions::app_debug(" end table name - " + table_name,Q_FUNC_INFO);
        return;
    }

    for(int i = 0 ; i < parent_list.size(); i++)
    {

        int base_value = 10; //decimal
        int req_digit_count = 4;

        QString num =  QString("%1").arg(i+1,req_digit_count,base_value, QChar('0'));

        QString parent_value = parent_list.at(i);
        QString bookmark_value = bookmarked_record_list.at(i);
        if(parent_datatype == MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString)
        {
            parent_value = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(parent_list.at(i), Q_FUNC_INFO);
        }
        else if(parent_datatype == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
        {
            parent_value = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(parent_list.at(i), Q_FUNC_INFO);
        }
        else if(parent_datatype == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
        {
            parent_value = recon_static_functions::convert_bytes_to_kilobytes_for_display(parent_list.at(i), Q_FUNC_INFO);
        }
        else if(parent_datatype == MACRO_CSV_TEMPLATE_DATATYPE_COLUMN_SEPERATOR_QString)
        {
            parent_value = (parent_value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple,"\n"));

            if(parent_value.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
                parent_value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"\n");

        }

        m_treewidgetitem *parent_item = new m_treewidgetitem(this);
        parent_item->set_state(0);
        parent_item->setText(0,num );

        //parent_item->setText(1, parent_value);
        if(parent_value == "" || parent_value.isEmpty())
        {
            parent_item->setText(1, "Unknown");
        }
        else
        {
            parent_item->setText(1, parent_value);
        }



        int bookmark_status = bookmark_value.toInt();
        if(bookmark_status == 0)
        {
            parent_item->setCheckState(0,Qt::Unchecked);
        }
        else if(bookmark_status == 1)
        {
            parent_item->setCheckState(0,Qt::Checked);
        }




        my_treewidget->addTopLevelItem(parent_item);

        QString command = col_name_list.join(",");

        //        QString command_child  = "select " + command + " from " + table_name + " where " + parent_column_name + " = '" +  parent_list.at(i) + "'";

        QSqlQuery query_child(destination_db);

        if(parent_list.at(i).isNull())
        {
            query_child.prepare("select " + command + " from " + table_name + " where " + parent_column_name + " IS NULL");
        }
        else
        {
            query_child.prepare("select " + command + " from " + table_name + " where " + parent_column_name + " = ?");
            query_child.addBindValue(parent_list.at(i));
        }

        //        query_child.prepare("select ? from ? where ? = ?");
        //        query_child.addBindValue(command);
        //        query_child.addBindValue(table_name);
        //        query_child.addBindValue(parent_column_name);
        //        query_child.addBindValue(parent_list.at(i));

        if(!query_child.exec())
        {
            recon_static_functions::app_debug(" -------------------------FAILED----------------------- to execute query query_child " + destination_db_file,Q_FUNC_INFO);
            recon_static_functions::app_debug(" command - " + query_child.executedQuery(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" last error -" + query_child.lastError().text(),Q_FUNC_INFO);
            continue;
        }

        while(query_child.next())
        {

            QTreeWidgetItem *child_item = new QTreeWidgetItem;

            for(int c_values = 2; c_values < col_name_list.size(); c_values++) // 0 is bookmark, 1 is parent_name
            {
                QString child_value = query_child.value(c_values).toString();

                if(col_type_list.at(c_values) == MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString)
                {
                    child_value = (global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(child_value, Q_FUNC_INFO));
                }
                else if(col_type_list.at(c_values) == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
                {
                    child_value = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(child_value, Q_FUNC_INFO);
                }
                else if(col_type_list.at(c_values) == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
                {
                    child_value = (recon_static_functions::convert_bytes_to_kilobytes_for_display(child_value, Q_FUNC_INFO));
                }
                else if(col_type_list.at(c_values) == MACRO_CSV_TEMPLATE_DATATYPE_COLUMN_SEPERATOR_QString)
                {
                    child_value = (child_value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple,"\n"));

                    if(child_value.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
                        child_value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"\n");

                }

                child_item->setText(c_values,child_value );
            }

            parent_item->addChild(child_item);

        }


    }   // for parent list

    for(int w = 0 ; w < col_width_list.size() ; w++)
    {
        QString width = col_width_list.at(w);

        if(width == QString(""))
            continue;

        my_treewidget->setColumnWidth(w, width.toInt());

    }


    destination_db.close();
    recon_static_functions::app_debug(" end table name - " + table_name,Q_FUNC_INFO);
}


QString plugin_viewer::mining_parent_column_for_tree(QStringList parent_childlist, QStringList col_namelist)
{
    if(parent_childlist.contains("1"))
    {
        return col_namelist.at(parent_childlist.indexOf("1"));
    }

    return "";
}

QString plugin_viewer::mining_parent_data_type_for_tree(QStringList parent_childlist, QStringList col_data_type_list)
{
    if(parent_childlist.contains("1"))
    {
        return col_data_type_list.at(parent_childlist.indexOf("1"));
    }

    return "";
}

void plugin_viewer::draw_chat_viewer(int index, QString command, QStringList cols_data_type_list)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    if(destination_db_file.trimmed() == QString(""))
    {
        recon_static_functions::app_debug("--- destination_db_file ----------------------FAILED-----------------------  DB path" + destination_db_file,Q_FUNC_INFO);
        return;
    }

    if(list_chat_viewer_widgets.size() <= index)
        return;

    chat_viewer *chat_viewer_obj =  list_chat_viewer_widgets.at(index);
    chat_viewer_obj->pub_set_display_chat_view_in_plugins_tab();


    recon_static_functions::app_debug(" end",Q_FUNC_INFO);


}

void plugin_viewer::draw_textedit(int index, QString table_name,QStringList col_name_list,QStringList col_datatype_list)
{
    recon_static_functions::app_debug(" start, table name - " + table_name,Q_FUNC_INFO);

    int textedt_pos = -1,bk_pos = -1;
    for(int j = 0; j < col_name_list.size(); j++)
    {
        if(col_datatype_list.at(j) == MACRO_CSV_TEMPLATE_DATATYPE_TEXTEDIT_QString){
            textedt_pos = j;
        }
        if(col_datatype_list.at(j) == MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString){
            bk_pos = j;
        }
    }
    if(textedt_pos == -1 || bk_pos == -1)
    {
        recon_static_functions::app_debug(" this table does not contain TEXTEDIT OR Bookmarkcolumn type --------FAILED---------- plugi name - " + get_plugin_name() + ",    table name - " + table_name,Q_FUNC_INFO);
        return;
    }
    if(list_texteditwidgets.size() <= index)
        return;

    if(list_checkbox_textedit_widgets.size() <= index)
        return;

    m_textedit *my_textedit = list_texteditwidgets.at(index);
    m_checkbox *my_checkbox = list_checkbox_textedit_widgets.at(index);

    my_textedit->clear();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("-------------------------FAILED----------------------- to open DB" + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:-" + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QSqlQuery query_select(destination_db);

    QString command_parent = "select " + col_name_list.join(",") + " from " + table_name;

    if(!query_select.exec(command_parent))
    {
        recon_static_functions::app_debug(" -------------------------FAILED----------------------- to execute query " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command - " + command_parent,Q_FUNC_INFO);
        recon_static_functions::app_debug(" last error -" + query_select.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString bk_value;
    while(query_select.next())
    {

        bk_value =  query_select.value(bk_pos).toString();
        my_textedit->append(query_select.value(textedt_pos).toString());
    }

    if(bk_value == "1")
    {
        my_checkbox->setChecked(Qt::Checked);
    }
    else
    {
        my_checkbox->setChecked(Qt::Unchecked);
    }

    my_textedit->moveCursor(QTextCursor::Start);

    destination_db.close();

    recon_static_functions::app_debug(" end, table name - " + table_name,Q_FUNC_INFO);

}

void plugin_viewer::draw_stack(int index, QString table_name, QStringList col_name_list, QStringList col_datatype_list)
{
    recon_static_functions::app_debug(" start, table name - " + table_name,Q_FUNC_INFO);

    int stack_pos = -1,bk_pos = -1;
    for(int j = 0; j < col_name_list.size(); j++)
    {
        if(col_datatype_list.at(j) == MACRO_CSV_TEMPLATE_DATATYPE_STACK_QString){
            stack_pos = j;
        }
        if(col_datatype_list.at(j) == MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString){
            bk_pos = j;
        }
    }
    if(stack_pos == -1 || bk_pos == -1)
    {
        recon_static_functions::app_debug(" this table does not contain STACK OR Bookmark column type --------FAILED---------- plugin name - " + get_plugin_name() + ",    table name - " + table_name,Q_FUNC_INFO);
        return;
    }


    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("-------------------------FAILED----------------------- to open DB" + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("error" + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QSqlQuery query_select(destination_db);

    QString command_parent = "select " + col_name_list.join(",") + " from " + table_name;

    if(!query_select.exec(command_parent))
    {
        recon_static_functions::app_debug(" -------------------------FAILED----------------------- to execute query " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command - " + command_parent,Q_FUNC_INFO);
        recon_static_functions::app_debug(" last error -" + query_select.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString stack_path;
    QString bk_value;
    while(query_select.next())
    {

        bk_value =  query_select.value(bk_pos).toString();
        // stack_path = (query_select.value(stack_pos).toString());
    }

    stack_path = destination_file_location + "/screenshot.png";
    QFileInfo info(stack_path);
    if(!info.exists())
        stack_path = destination_file_location + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images + "/screenshot.png";


    image_preview *image_previewer;
    image_previewer = new image_preview(this);
    image_previewer->setMaximumWidth(9999);
    image_previewer->setMaximumHeight(9999);

    if(list_stackedwidgetwidgets.size() <= index)
        return;
    if(list_checkbox_stackedwidget_widgets.size() <= index)
        return;

    m_stackedwidget *widget_stack = list_stackedwidgetwidgets.at(index);
    m_checkbox *widget_check = list_checkbox_stackedwidget_widgets.at(index);

    widget_stack->addWidget(image_previewer);
    widget_stack->setCurrentIndex(2);
    image_previewer->set_image_by_path(stack_path);




    if(bk_value == "1")
    {
        widget_check->setChecked(Qt::Checked);
    }
    else
    {
        widget_check->setChecked(Qt::Unchecked);
    }



    destination_db.close();

    recon_static_functions::app_debug(" end, table name - " + table_name,Q_FUNC_INFO);

}

/*
 Calling Function:   pub_display_data  File:library_interface.cpp
 Arguments:          Nil
 Return:             void
 Function:           Set number of columns for each table, set horizontal header lables,
                     connect arrow and space key signals with table-widget and initialize
                     array of boolean(bookmark sorting bool) to true;
 */

void plugin_viewer::draw_headers_tablewidget()
{
    m_tablewidget *my_tablewidget;

    tab_custom_header_widgets_QMap.clear();

    for(int i = 0 ; i < list_tablewidgets.size() ; i++)
    {
        QCoreApplication::processEvents();

        QStringList header_list;
        QStringList temp_header_list = tw_header_list_tablewidget.at(i);
        for(int i = 0; i < temp_header_list.size(); i++)
        {
            QString tmp_str = temp_header_list.value(i);

            header_list << tmp_str;
        }


        if(!header_list.isEmpty())
            header_list  << QString("");

        my_tablewidget = list_tablewidgets.at(i);

        QList<custom_table_header_widget *> custom_header_widgets_QList;

        QStringList empty_header_stringlist;

        my_tablewidget->setColumnCount(header_list.size());

        for(int column_count = 0; column_count < header_list.size(); column_count++)
        {
            QString column_name = header_list.at(column_count);

            custom_table_header_widget_obj = new custom_table_header_widget;

            custom_table_header_widget_obj->pub_set_column_name(column_name);
            custom_table_header_widget_obj->pub_set_column_seq_number(column_count);

            my_tablewidget->pub_set_horizontal_header_item_widget(column_count, custom_table_header_widget_obj);

            if(column_count ==  enum_Plugin_display_Bookmarks && column_name.isEmpty())
            {
                QPixmap bookmark("../icons/signs/bookmark.png");
                custom_table_header_widget_obj->pub_set_icon_on_bookmark_or_seen(bookmark);
                custom_table_header_widget_obj->pushbutton_sort->show();
            }
            else if(column_count ==  enum_Plugin_display_Notes_Icon && column_name.isEmpty())
            {
                custom_table_header_widget_obj->pushbutton_sort->show();
            }

            empty_header_stringlist << "";

            connect(custom_table_header_widget_obj, SIGNAL(search_field_return_pressed()), this, SLOT(slot_custom_table_header_widget_search_field_return_pressed()));
            connect(custom_table_header_widget_obj, SIGNAL(search_field_text_changed()), this, SLOT(slot_custom_table_header_widget_search_field_text_changed()));
            connect(custom_table_header_widget_obj, SIGNAL(signal_sorting_clicked(int,int)), this, SLOT(slot_custom_table_header_widget_sorting_clicked(int,int)));

            custom_header_widgets_QList.append(custom_table_header_widget_obj);
        }
        tab_custom_header_widgets_QMap.insert(i,custom_header_widgets_QList);

        my_tablewidget->setHorizontalHeaderLabels(empty_header_stringlist);

        bool_sort_bookmarked[i] = true;
        my_tablewidget->installEventFilter(this);
        my_tablewidget->verticalHeader()->setFixedWidth(50);
        my_tablewidget->verticalHeader()->setHighlightSections(false);
        my_tablewidget->horizontalHeader()->setHighlightSections(false);

        QObject::connect((QObject*)my_tablewidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_bookmarked(int)));
        QObject::connect((QObject*)my_tablewidget->horizontalHeader(),SIGNAL(sectionPressed(int)),this,SLOT(slot_fixed_last_blank_column(int)));
        my_tablewidget->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(my_tablewidget->horizontalHeader(),SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(slot_table_header_customContextMenuRequested(const QPoint&)));
        connect(my_tablewidget, SIGNAL(signal_arrow_key_pressed()), this, SLOT(slot_arrow_key_pressed()));
        connect(my_tablewidget, SIGNAL(signal_bookmarked_key_pressed()), this, SLOT(slot_bookmark_key_pressed_tablewidget()));

        my_tablewidget->horizontalHeader()->hide();
        my_tablewidget->horizontalHeader()->show();

        QString ext_atr =  table_apple_metadata_available_list_tablewidget.at(i);
        ext_atr = ext_atr.trimmed();

        if(ext_atr == QString("1"))
        {
            //            connect(my_tablewidget, SIGNAL(signal_control_m_key_pressed()), this, SLOT(slot_control_m_key_pressed()));
            //            connect(my_tablewidget, SIGNAL(signal_control_n_key_pressed()), this, SLOT(slot_control_n_key_pressed()));


        }


        QString exp_avail =  table_export_available_list_tablewidget.at(i);
        exp_avail = exp_avail.trimmed();
        if(exp_avail == QString("1") || exp_avail == QString("1::2"))
        {
            connect(my_tablewidget, SIGNAL(signal_control_e_key_pressed()), this, SLOT(slot_control_e_key_pressed()));

        }



        int tab_index = i;
        QStringList cols_nam_list =  md_column_name_list_tablewidget.at(tab_index);
        QStringList preview_col_list = md_preview_list_tablewidget.at(tab_index);

        int preview_index = -1;
        if(preview_col_list.size() > 0)
        {
            preview_index = cols_nam_list.indexOf(preview_col_list.at(0),0);
        }



    }


    return;

}

void plugin_viewer::draw_headers_tree()
{
    m_treewidget *my_treewidget;

    for(int i = 0 ; i < tab_name_list_tree.size() ; i++)
    {
        QCoreApplication::processEvents();

        QStringList header_list =  header_list_tree.at(i);

        my_treewidget = list_treewidgets.at(i);

        my_treewidget->setColumnCount(header_list.size());

        my_treewidget->setHeaderLabels(header_list);
        my_treewidget->installEventFilter(this);

    }

    return;

}


/*
 Calling Function:   pub_display_data  File:library_interface.cpp
 Arguments:          Nil
 Return:             void
 Function:           Prepare command and call draw_table function for all visible tabs.
 */

void plugin_viewer::populate_data_in_tablewidgets()
{
    recon_static_functions::app_debug(" [populate_data_in_tablewidgets] start ",Q_FUNC_INFO);

    ///--Progressbar work---START.

    bool_sort_with_search = false;
    total_covered_count = 0;
    total_record_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, destination_db_file, QStringList(), Q_FUNC_INFO);
    //    progressbar_increment_step = total_record_count / 5;

    //    if(progressbar_increment_step <= 0)
    //        progressbar_increment_step = 1;

    //    emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,0);
    ///--Progressbar work---END.

    for(int  i = 0 ; i < tab_name_list_tablewidget.size() ; i++)
    {
        QCoreApplication::processEvents();

        QString table_name = table_name_list_tablewidget.at(i);
        QStringList cols_nam_list =  tw_column_name_list_tablewidget.at(i);
        QStringList cols_type_list = tw_data_type_list_tablewidget.at(i);

        // adding notes because of notes visibily for display is not set, and we want to show icon of notes on_pushbutton_showall_click
        cols_nam_list.insert(cols_nam_list.size(), "notes");
        cols_type_list.insert(cols_type_list.size(), MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString);

        QString command = cols_nam_list.join(",");
        command = "select " + command + " from " + table_name;

        int timeline_col_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
        int notes_col_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString,0);

        int os_scheme_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);


        QStringList cols_width_list;
        QStringList tmp_cols_width_list = tw_col_width_list_tablewidget.at(i);

        for(int jj = 0; jj < tmp_cols_width_list.size(); jj++)
        {
            if(cols_type_list.at(jj) == MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString
                    || cols_type_list.at(jj) == MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString
                    || cols_type_list.at(jj) == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString)
            {
                cols_width_list << tmp_cols_width_list.at(jj);
            }
            else
            {
                int cols_width = tmp_cols_width_list.at(jj).toInt();
                cols_width = cols_width + 25;
                cols_width_list << QString::number(cols_width);
            }
        }

        QStringList abc,xyz; //becoz they need at search
        abc.clear();
        xyz.clear();

        draw_table(command , i, cols_nam_list.size(),cols_type_list, timeline_col_index,  notes_col_index, os_scheme_index, cols_width_list,abc,xyz);
    }


    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" [populate_data_in_tablewidgets] end ",Q_FUNC_INFO);

}
void plugin_viewer::populate_data_in_textedit()
{

    for(int  i = 0 ; i < tab_name_list_textedit.size() ; i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QString table_name = table_name_list_textedit.at(i);
        QStringList cols_nam_list =  column_name_list_textedit.at(i);
        QStringList cols_data_type_list =  data_type_list_textedit.at(i);


        draw_textedit(i,table_name,cols_nam_list,cols_data_type_list);
    }

}
void plugin_viewer::populate_data_in_stack()
{
    for(int  i = 0 ; i < tab_name_list_stack.size() ; i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QString table_name = table_name_list_stack.at(i);
        QStringList cols_nam_list =  column_name_list_stack.at(i);
        QStringList cols_data_type_list =  data_type_list_stack.at(i);


        draw_stack(i,table_name,cols_nam_list,cols_data_type_list);
    }

}

void plugin_viewer::populate_data_in_tree()
{

    for(int  i = 0 ; i < tab_name_list_tree.size() ; i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QString table_name = table_name_list_tree.at(i);
        QStringList cols_nam_list =  column_name_list_tree.at(i);
        QStringList cols_type_list = data_type_list_tree.at(i);
        QStringList cols_width_list = col_width_list_tree.at(i);


        QString parent_column_name = mining_parent_column_for_tree(parent_child_list_tree.at(i),column_name_list_tree.at(i));

        QString parent_datatype = mining_parent_data_type_for_tree(parent_child_list_tree.at(i),data_type_list_tree.at(i));

        if(parent_column_name == "" || parent_datatype == "")
        {
            recon_static_functions::app_debug(" [populate_data_in_tree] ---FAILED---- parent column does not exist in table " + table_name + " plugin -->>" + get_plugin_name(),Q_FUNC_INFO);
            continue;
        }


        draw_tree(i,table_name,parent_column_name,parent_datatype,cols_nam_list,cols_type_list,cols_width_list);


    }

}
void plugin_viewer::populate_data_in_thumbnail_viewer()
{
    recon_static_functions::app_debug("  start",Q_FUNC_INFO);


    for(int  i = 0 ; i < tab_name_list_thumbnail.size() ; i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QString table_name = table_name_list_thumbnail.at(i);

        draw_thumbnail(table_name,i);

    }


    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

}

void plugin_viewer::populate_data_in_chat_viewer()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    for(int  i = 0 ; i < tab_name_list_chat_viewer.size() ; i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QString table_name = table_name_list_chat_viewer.at(i);
        QStringList cols_nam_list =  display_column_name_list_chat_viewer.at(i);
        QStringList cols_type_list = display_data_type_list_chat_viewer.at(i);

        QString command = cols_nam_list.join(",");
        command = "select " + command + " from '" + table_name + "'";

        draw_chat_viewer(i,command,cols_type_list);
    }


    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}


void plugin_viewer::draw_table(QString command, int index, int cols_count, QStringList col_type_list, int tw_timeline_col_index, int tw_notes_index,int tw_os_scheme_index, QStringList col_width_list, QStringList search_cols_list, QStringList search_string_list)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);



    if(destination_db_file.trimmed() == QString(""))
    {
        recon_static_functions::app_debug("--- destination_db_file ----------------------FAILED-----------------------  DB path" + destination_db_file,Q_FUNC_INFO);
        return;
    }
    if(list_tablewidgets.size() <= index)
        return;


    m_tablewidget *my_tablewidget =  list_tablewidgets.at(index);
    my_tablewidget->horizontalHeader()->setStretchLastSection(true);


    QHeaderView *plugin_viewer_header_view_obj =  my_tablewidget->horizontalHeader();
    connect(plugin_viewer_header_view_obj , SIGNAL(sectionClicked(int)), this , SLOT(slot_table_widget_header_item_clicked(int)));


    for(int w = 0 ; w < col_width_list.size() ; w++)
    {
        QString width = col_width_list.at(w);

        if(width == QString(""))
            continue;

        my_tablewidget->setColumnWidth(w, width.toInt());
    }

    my_tablewidget->setRowCount(0);

    //=================Check for columns hidden by manage columns - Start ===========///
    QList<struct_manage_column_info> column_no_and_name_list;
    column_no_and_name_list = get_hidden_column_from_manage_column_db();
    for(int i= 0; i < column_no_and_name_list.size(); i++)
    {
        struct_manage_column_info var = column_no_and_name_list.at(i);
        my_tablewidget->hideColumn(var.hid_col_no);
    }
    //=================Check for columns hidden by manage columns - End ===========///

    int os_scheme_index1 = col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);
    my_tablewidget->hideColumn(os_scheme_index1);


    QTableWidgetItem *item_tablewidget[cols_count];

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("-------------------------FAILED----------------------- to open DB" + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("error" + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_select(destination_db);

    destination_db.transaction();


    query_select.prepare(command);

    if(!bool_sort_with_search)
    {
        for(int k = 0 ; k < search_cols_list.size() ; k++)
        {
            for(int i = 0; i < search_string_list.size(); i++)
            {
                query_select.addBindValue(search_string_list.at(i));

            }
        }
    }
    if(!query_select.exec())
    {
        recon_static_functions::app_debug(" -------------------------FAILED----------------------- to execute query",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:-" + query_select.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query:--" + query_select.lastQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug("destination_db_file" + destination_db_file,Q_FUNC_INFO);
        return;
    }

    QString tab_name_str = tab_name_list_tablewidget.at(index);

    refresh_horizontal_header();

    int count = 0;
    while(query_select.next())
    {
        if(count < 100)
        {
            if(count % 8 == 0)
            {
                QCoreApplication::processEvents();
            }
        }
        else
        {
            if(count % 500 == 0)
            {
                QCoreApplication::processEvents();
            }
        }

        total_covered_count++;
        //        if(total_record_count > 0)
        //        {
        //            if(total_covered_count % progressbar_increment_step == 0)
        //            {

        //                float record_percent = ((total_covered_count*100)/total_record_count);
        //                emit signal_progress_count_for_statusbar_progress_bar(total_record_count,total_covered_count,record_percent);
        //            }
        //        }

        loading_display_dialog_based_on_records(total_record_count , total_covered_count , get_plugin_name());
        if(bool_cancel_loading)
        {
            bool_cancel_loading = false;
            break ;
        }


        if(ui->checkBox_timeline->isChecked())
        {
            if(tw_timeline_col_index != -1)
            {
                bool ok;
                qint64 result_date =  query_select.value(tw_timeline_col_index).toULongLong(&ok);
                if(result_date <= timeline_start_time || result_date >= timeline_end_time)
                    continue;
            }
        }


        for(int i = 0 ; i < cols_count ; i++)
        {
            item_tablewidget[i] = new QTableWidgetItem;

            if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString)
            {
                if(query_select.value(0).toString() == "0")
                    item_tablewidget[i]->setCheckState(Qt::Unchecked);
                else
                {
                    int record_no_index  =  QString(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString).toInt();
                    QString record_str   = query_select.value(record_no_index).toString();
                    recon_helper_standard_obj->pub_add_bookmarked_record_to_map_for_detailed_info_bookmark_control(map_bookmarked_records_from_table_display, tab_name_str, record_str);

                    item_tablewidget[i]->setCheckState(Qt::Checked);
                }

                if(!query_select.value(tw_notes_index).toString().isEmpty())
                {
                    QIcon icon("../icons/notes.png");
                    item_tablewidget[i]->setIcon(icon);
                }

            }
            else if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
            {
                item_tablewidget[i]->setData(Qt::EditRole , query_select.value(i).toLongLong());
                item_tablewidget[i]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            }
            else if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_PREPEND_COUNT_QString)
            {
                item_tablewidget[i]->setData(Qt::EditRole,query_select.value(i).toLongLong());
                item_tablewidget[i]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

                if(query_select.value(i).toString().trimmed().isEmpty())
                {
                    item_tablewidget[i]->setData(Qt::EditRole,query_select.value(i).toString());
                }
                else
                {
                    item_tablewidget[i]->setData(Qt::EditRole,query_select.value(i).toLongLong());
                    item_tablewidget[i]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                }
            }
            else if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_SYSTEM_ACCOUNT_QString)
            {
                item_tablewidget[i]->setText(query_select.value(i).toString());
            }
            else if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString)
            {
                item_tablewidget[i]->setText(global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query_select.value(i).toString(), Q_FUNC_INFO));
            }
            else if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
            {
                item_tablewidget[i]->setData(Qt::EditRole,recon_static_functions::convert_bytes_to_kilobytes_for_display(query_select.value(i).toString(), Q_FUNC_INFO).toLongLong());
                item_tablewidget[i]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            }
            else if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
            {
                item_tablewidget[i]->setData(Qt::EditRole,recon_static_functions::convert_bytes_to_kilobytes_for_display(query_select.value(i).toString(),Q_FUNC_INFO).toLongLong());
                item_tablewidget[i]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            }
            else if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_TEXT_QString)
            {
                item_tablewidget[i]->setText(query_select.value(i).toString());

            }
            else if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_THUMBNAIL_FILE_NAME_QString)
            {
                item_tablewidget[i]->setText(query_select.value(i).toString());

            }
            else if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_THUMBNAIL_FILE_PATH_QString)
            {
                item_tablewidget[i]->setText(query_select.value(i).toString());

            }
            else if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_FILE_EXISTENCE_QString)
            {
                item_tablewidget[i]->setText(query_select.value(i).toString());
            }
            else if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_COLUMN_SEPERATOR_QString)
            {
                QString value = query_select.value(i).toString();

                if(value.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                    value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple,"\n");

                if(value.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
                    value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"\n");

                item_tablewidget[i]->setText(value);
            }
            else if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString)
            {
                if(!query_select.value(i).toString().isEmpty())
                {
                    QString tag_data = query_select.value(i).toString();

                    QString icon_path = "";
                    if(tag_data.contains(","))
                        icon_path = get_icon_path_tags("",tag_data);
                    else
                    {
                        QString color = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tag_data , global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                        icon_path = get_icon_path_tags(color,"");
                    }

                    item_tablewidget[i]->setIcon(QIcon(icon_path));
                }

            }
            else if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString)
            {
                ///continue, becoz this column adjusted in bookmark column
                /// --and notes text was visible in blank column due to 'else' statement
                /// --data type inserted manually from where this function has been called
                continue;
            }
            else if(col_type_list.at(i) == MACRO_CSV_TEMPLATE_DATATYPE_FILE_IN_RESULT_DIR_QString)
            {
                QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                QString str_val = query_select.value(i).toString();

                str_val = recon_static_functions::get_complete_file_path(result_dir_path , str_val, Q_FUNC_INFO);
                item_tablewidget[i]->setText(str_val);
            }
            else
            {
                item_tablewidget[i]->setText(query_select.value(i).toString());
            }
        }

        my_tablewidget->insertRow(count);
        my_tablewidget->setRowHeight(count, 22);

        for(int j=0; j < cols_count ; j++)
        {

            my_tablewidget->setItem(count, j, item_tablewidget[j]);

            if(col_type_list.at(j) == MACRO_CSV_TEMPLATE_DATATYPE_FILE_EXISTENCE_QString)
            {
                if(my_tablewidget->item(count,j)->text() == "NO")
                    my_tablewidget->item(count,j)->setForeground(Qt::red);
            }
        }

        count++;

    }


    // int system_account_index = col_type_list.indexOf(MACRO_GLOBAL_CSV_TEMPLATE_DATATYPE_SYSTEM_ACCOUNT,0);
    //    if(system_account_name_list.size() <= 1)
    //        my_tablewidget->hideColumn(system_account_index);

    int os_scheme_index = col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);
    my_tablewidget->hideColumn(os_scheme_index);

    for(int w = 0 ; w < col_width_list.size() ; w++)
    {

        QString width = col_width_list.at(w);

        if(width == QString(""))
            continue;

        my_tablewidget->setColumnWidth(w, width.toInt());

    }

    my_tablewidget->resizeColumnToContents(my_tablewidget->columnCount() - 2);


    my_tablewidget->horizontalHeader()->stretchLastSection();

    if(my_tablewidget->rowCount() > 0)
        my_tablewidget->scrollToItem(my_tablewidget->item(0,0));

    destination_db.commit();
    destination_db.close();



    //=================Check for columns hidden by manage columns - Start ===========///
    QList<struct_manage_column_info> column_no_and_name_list1;
    column_no_and_name_list1 = get_hidden_column_from_manage_column_db();
    for(int i= 0; i < column_no_and_name_list1.size(); i++)
    {
        struct_manage_column_info var = column_no_and_name_list1.at(i);
        my_tablewidget->hideColumn(var.hid_col_no);
    }
    //=================Check for columns hidden by manage columns - End ===========///

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

}

void plugin_viewer::prepare_record_no_str_and_hash_md5_str_for_search_files_with_same_hash(QString fs_db_path, QString file_path)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    record_no_for_search_files_with_same_hash.clear();
    hash_md5_for_search_files_with_same_hash.clear();

    QString connection_naam_1 = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam_1);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_1);
    destination_db.setDatabaseName(fs_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ----FAILED---- ",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:-" + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }
    QSqlQuery query(destination_db);

    query.prepare("select INT, hash_md5 from files where filepath = ?");
    query.addBindValue(file_path);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query insert ----FAILED---- " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    if(query.next())
    {
        record_no_for_search_files_with_same_hash = query.value("INT").toString();
        hash_md5_for_search_files_with_same_hash  = query.value("hash_md5").toString();
    }

    destination_db.close();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}


int plugin_viewer::get_total_record_count_in_plugin_tab(QString tab_name)
{
    recon_static_functions::app_debug("Start" , Q_FUNC_INFO);
    int record_count;


    QString command = "select COUNT(*) from " + tab_name;
    QString count = recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,destination_db_file,Q_FUNC_INFO);
    record_count = count.toInt();

    recon_static_functions::app_debug("End" , Q_FUNC_INFO);

    return  record_count;
}
