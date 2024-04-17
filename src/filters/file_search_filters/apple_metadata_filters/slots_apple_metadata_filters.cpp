#include "apple_metadata_filters.h"

void apple_metadata_filters::slot_on_combobox_search_type_current_index_changed(int index)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(db_path_apple_attributes);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination_dbOpen -------FAILED------ " + db_path_apple_attributes, Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(db_path_apple_attributes , Q_FUNC_INFO);
        return;
    }

    QComboBox *combobx_txt_wid = new QComboBox(this);
    combobx_txt_wid = (QComboBox *)sender();
    QString item_match_in_csv = combobx_txt_wid->itemText(index).trimmed();
    item_match_in_csv = item_match_in_csv.trimmed();

    QSqlQuery query(destination_db);
    query.prepare("select widget_type from tbl_apple_attributes where attribute_name = ?");
    query.addBindValue(item_match_in_csv);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query Open -------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(query.executedQuery() , Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    int type = enum_File_System_Apple_Attribute_Widget_Type_MAX_Value_int;//temporary
    if(query.next())
    {
        type = query.value(0).toInt();
    }

    int indexoditem = enum_File_System_MAX_Index_Of_Widget_In_Layout_int;
    for(int i = 0 ; i < vlayout_filters->count(); i++)
    {
        QLayoutItem *item = vlayout_filters->itemAt(i);

        for(int j = 0; j < item->layout()->count() ; j++)
        {
            QLayoutItem *item2 = item->layout()->itemAt(j);
            if(item2->widget() == sender())
            {
                indexoditem = i;
                break;
            }
        }
    }

    if(indexoditem == enum_File_System_MAX_Index_Of_Widget_In_Layout_int)
    {
        destination_db.close();
        return;
    }

    if(type == enum_File_System_Widget_Type_FILESYSTEM_NAME)
    {
        set_names_interface(indexoditem, combobx_txt_wid->currentText());
    }

    if(type == enum_File_System_Widget_Type_FILESYSTEM_OPTIONS)
    {
        set_options_interface(indexoditem,combobx_txt_wid->currentText());
    }

    if(type == enum_File_System_Widget_Type_FILESYSTEM_RANGE)
    {
        set_range_interface(indexoditem,combobx_txt_wid->currentText());
    }

    if(type == enum_File_System_Widget_Type_FILESYSTEM_REAL_VALUE)
    {
        set_range_interface(indexoditem,combobx_txt_wid->currentText());
    }

    if(type == enum_File_System_Widget_Type_FILESYSTEM_Duration)
    {
        set_duration_interface(indexoditem,combobx_txt_wid->currentText());
    }

    if(type == enum_File_System_Widget_Type_FILESYSTEM_SIZE)
    {
        set_size_interface(indexoditem,combobx_txt_wid->currentText());
    }

    if(type == enum_File_System_Widget_Type_FILESYSTEM_SIMPLE_LINEEDIT)
    {
        set_lineedit_interface(indexoditem,combobx_txt_wid->currentText());
    }

    if(type == enum_File_System_Widget_Type_FILESYSTEM_KIND)
    {
        set_kind_interface(indexoditem,combobx_txt_wid->currentText());
    }

    if(type == enum_File_System_Widget_Type_FILESYSTEM_CONTENT)
    {
        set_contents_interface(indexoditem,combobx_txt_wid->currentText());
    }

    if(type == enum_File_System_Widget_Type_FILESYSTEM_DATE)
    {
        set_date_interface(indexoditem,combobx_txt_wid->currentText());
    }

    if(type == enum_File_System_Widget_Type_FILESYSTEM_SIMPLE_LABEL_N_LINEEDIT)
    {
        set_label_with_lineedit_interface(indexoditem,combobx_txt_wid->currentText());
    }

    if(type == enum_File_System_Widget_Type_FILESYSTEM_SINGLE_SYSTEM_COMBOBX)
    {
        set_single_combobox_interface(indexoditem,combobx_txt_wid->currentText());
    }

    if(type == enum_File_System_Widget_Type_FILESYSTEM_SINGLE_VISIBLE_COMBOBX)
    {
        set_single_visible_combobox_interface(indexoditem,combobx_txt_wid->currentText());
    }

    if(type == enum_File_System_Widget_Type_FILESYSTEM_FILE_LABEL)
    {
        set_file_labels_interface(indexoditem,combobx_txt_wid->currentText());
    }

    last_save_index = index;

    destination_db.close();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::slot_on_pushbutton_delete_clicked()
{
    recon_static_functions::app_debug(" Starts" , Q_FUNC_INFO);

    if(search_count == 1)
    {
        return;
    }

    --search_count;

    int indexoditem = 0;
    for(int i = 0 ; i < vlayout_filters->count(); i++)
    {
        QLayoutItem *item = vlayout_filters->itemAt(i);

        for(int j = 0; j < item->layout()->count() ; j++)
        {
            QLayoutItem *item2 = item->layout()->itemAt(j);

            if(item2->spacerItem())
            {
                continue;
            }

            if(item2->widget() == sender())
            {
                indexoditem = i;
            }
        }
    }


    for(int i = 0 ; i < vlayout_filters->count(); i++)
    {
        QLayoutItem *item = vlayout_filters->itemAt(i);

        for(int j = 0; j < item->layout()->count() ; j++)
        {
            QLayoutItem *item2 = item->layout()->itemAt(j);

            if(item2->spacerItem())
            {
                continue;
            }

            if(i == indexoditem)
            {
                vlayout_filters->removeWidget(item2->widget());
            }
        }
    }


    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexoditem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexoditem)->layout()->takeAt(0);
        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
        }
    }

    vlayout_filters->removeItem(vlayout_filters->itemAt(indexoditem));

//    on_pushButton_display_clicked();

    recon_static_functions::app_debug(" Ends" , Q_FUNC_INFO);
}

void apple_metadata_filters::slot_on_pushbutton_add_clicked()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(db_path_apple_attributes);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Open -------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(db_path_apple_attributes , Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    if(search_count > enum_File_System_MAX_Filter_Count_int)
        return;

    int indexoditem = 0;
    for(int i = 0 ; i < vlayout_filters->count(); i++)
    {
        QLayoutItem *item = vlayout_filters->itemAt(i);
        for(int j = 0; j < item->layout()->count() ; j++)
        {
            QLayoutItem *item2 = item->layout()->itemAt(j);

            if(item2->spacerItem())
            {
                continue;
            }

            if(item2->widget() == sender())
            {
                indexoditem = i;
                break;
            }
        }
    }

    QLayoutItem *item_layout = vlayout_filters->itemAt(indexoditem);

    for(int k = 0 ; k < item_layout->layout()->count() ; k++)
    {
        QLayoutItem *item_layout22 = item_layout->layout()->itemAt(k);

        if(item_layout22->spacerItem())
        {
            continue;
        }

        QString str_classname = item_layout22->widget()->metaObject()->className();

        if(str_classname == "QComboBox" )
        {
            QComboBox *box = new QComboBox(this);
            box = (QComboBox*) item_layout22->widget();

            QString namee = box->currentText();

            query.prepare("select widget_type from tbl_apple_attributes where attribute_name = ?");
            query.addBindValue(namee);
            if(!query.exec())
            {
                recon_static_functions::app_debug(" Open -------FAILED------ " , Q_FUNC_INFO);
                recon_static_functions::app_debug(query.lastError().text() , Q_FUNC_INFO);
                recon_static_functions::app_debug(query.executedQuery() , Q_FUNC_INFO);
                continue;
            }

            int type = enum_File_System_Apple_Attribute_Widget_Type_MAX_Value_int;//temporary
            if(query.next())
            {
                type = query.value(0).toInt();
            }

            if(type == enum_File_System_Apple_Attribute_Widget_Type_MAX_Value_int)
            {
                destination_db.close();
                return;
            }


            if(type == enum_File_System_Widget_Type_FILESYSTEM_NAME)
            {
                set_names_interface_add_filter(indexoditem + 1, namee);
            }

            if(type == enum_File_System_Widget_Type_FILESYSTEM_KIND)
            {
                set_kind_interface_add_filter(indexoditem + 1, namee);
            }

            if(type == enum_File_System_Widget_Type_FILESYSTEM_CONTENT)
            {
                set_contents_interface_add_filter(indexoditem + 1, namee);
            }

            if(type == enum_File_System_Widget_Type_FILESYSTEM_DATE)
            {
                set_date_interface_add_filter(indexoditem + 1, namee);
            }

            if(type == enum_File_System_Widget_Type_FILESYSTEM_OPTIONS)
            {
                set_options_interface_add_filter(indexoditem + 1, namee);
            }

            if(type == enum_File_System_Widget_Type_FILESYSTEM_RANGE)
            {
                set_range_interface_add_filter(indexoditem + 1, namee);
            }

            if(type == enum_File_System_Widget_Type_FILESYSTEM_Duration)
            {
                set_duration_interface_add_filter(indexoditem + 1, namee);
            }

            if(type == enum_File_System_Widget_Type_FILESYSTEM_SIZE)
            {
                set_size_interface_add_filter(indexoditem + 1,namee);
            }

            if(type == enum_File_System_Widget_Type_FILESYSTEM_SIMPLE_LINEEDIT)
            {
                set_lineedit_interface_add_filter(indexoditem + 1,namee);
            }

            if(type == enum_File_System_Widget_Type_FILESYSTEM_SIMPLE_LABEL_N_LINEEDIT)
            {
                set_label_with_lineedit_interface_add_filter(indexoditem + 1,namee);
            }

            if(type == enum_File_System_Widget_Type_FILESYSTEM_SINGLE_SYSTEM_COMBOBX)
            {
                set_single_combobox_interface_add_filter(indexoditem + 1,namee);
            }

            if(type == enum_File_System_Widget_Type_FILESYSTEM_SINGLE_VISIBLE_COMBOBX)
            {
                set_single_visible_combobox_interface_add_filter(indexoditem + 1,namee);
            }

            if(type == enum_File_System_Widget_Type_FILESYSTEM_FILE_LABEL)
            {
                set_file_labels_interface_add_filter(indexoditem + 1,namee);
            }
            if(type == enum_File_System_Widget_Type_FILESYSTEM_REAL_VALUE)
            {
                set_range_interface(indexoditem + 1,namee);
            }

            break;
        }
    }

    search_count++;
    destination_db.close();
    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::slot_apple_attributes_work_done(bool bool_ok_or_cancel)
{
    apple_attributes_obj->hide();

    if(bool_ok_or_cancel)
        on_apl_attribute_ok_button_click();
    else
        on_apl_attribute_cancel_button_click();
}

void apple_metadata_filters::slot_on_combobox_ext_attr_kind_current_index_changed(QString current_str)
{
    int index_of_item = enum_File_System_MAX_Index_Of_Widget_In_Layout_int;
    for(int i = 0 ; i < vlayout_filters->count(); i++)
    {
        QLayoutItem *item = vlayout_filters->itemAt(i);

        for(int j = 0; j < item->layout()->count() ; j++)
        {
            QLayoutItem *item2 = item->layout()->itemAt(j);
            if(item2->widget() == sender())
            {
                index_of_item = i;
                break;
            }
        }
    }

    if(index_of_item == enum_File_System_MAX_Index_Of_Widget_In_Layout_int)
        return;

    if(current_str == MACRO_Generic_Filter_Condition_Image)
    {
        set_kind_images_filter(index_of_item,current_str,list_search_type_kind_images_search_items);
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Music)
    {
        set_kind_images_filter(index_of_item,current_str,list_search_type_kind_music_search_items);
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Text)
    {
        set_kind_images_filter(index_of_item,current_str,list_search_type_kind_textfiles_search_items);
    }
    else if(current_str == MACRO_Generic_Filter_Condition_Other)
    {
        set_kind_others_filter(index_of_item,current_str);
    }
    else
    {
        set_kind_interface_all_filter(index_of_item,current_str);
    }
}

void apple_metadata_filters::slot_on_combobox_ext_attr_dates_current_index_changed(QString current_str)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    int indexoditem = enum_File_System_MAX_Index_Of_Widget_In_Layout_int;
    for(int i = 0 ; i < vlayout_filters->count(); i++)
    {
        QLayoutItem *item = vlayout_filters->itemAt(i);

        for(int j = 0; j < item->layout()->count() ; j++)
        {
            QLayoutItem *item2 = item->layout()->itemAt(j);
            if(item2->widget() == sender())
            {
                indexoditem = i;
                break;
            }
        }
    }

    if(indexoditem == enum_File_System_MAX_Index_Of_Widget_In_Layout_int)
        return;

    if(current_str == MACRO_Generic_Filter_Condition_Exactly || current_str == MACRO_Generic_Filter_Condition_Before || current_str == MACRO_Generic_Filter_Condition_After)
    {
        set_dates_interface_with_display_format(indexoditem,current_str);
    }
//    else
//    {
//        if(current_str == MACRO_Generic_Filter_Condition_Today || current_str == MACRO_Generic_Filter_Condition_Yesterday || current_str == MACRO_Generic_Filter_Condition_This_Week || current_str == MACRO_Generic_Filter_Condition_This_Month || current_str == MACRO_Generic_Filter_Condition_This_Year)
//        {
//            set_dates_interface_others_filter(indexoditem,current_str);
//        }
//        else
//        {
//            set_date_interface_within_filter(indexoditem, current_str);
//        }
//    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

