#include "file_system.h"
#include "ui_file_system.h"

void file_system::slot_gallery_bookmark_value_changed(struct_global_gallery_view_file_info f1)
{

    if(f1.INT.isEmpty())
    {
        m_file_system_tablewidget_obj->clearSelection();
        emit signal_clear_all_metadata();
        return;
    }

    /// control goes to table view tab
    /// now all operations can be done on basis of m_file_system_tablewidget_obj

    int current_row_selection_int = f1.table_view_row_no;
    for(int pp = 0 ; pp < m_file_system_tablewidget_obj->rowCount() ; pp++)
    {

        if((f1.INT == m_file_system_tablewidget_obj->item(pp,enum_File_System_display_INT)->text())
                && (f1.source_count_name == m_file_system_tablewidget_obj->item(pp,enum_File_System_display_SOURCE_COUNT_NAME)->text()))
        {
            current_row_selection_int = pp;
            break;
        }
    }


    m_file_system_tablewidget_obj->selectRow(current_row_selection_int);


    bool bool_chkstate = false;

    if(f1.bookmark == MACRO_TRUE_VALUE_NUMERIC_QString)
        bool_chkstate = true;

    if(bool_chkstate)
    {
        m_file_system_tablewidget_obj->item(m_file_system_tablewidget_obj->currentRow(),m_file_system_tablewidget_obj->currentColumn())->setCheckState(Qt::Checked);
    }
    else
    {
        m_file_system_tablewidget_obj->item(m_file_system_tablewidget_obj->currentRow(),m_file_system_tablewidget_obj->currentColumn())->setCheckState(Qt::Unchecked);
    }

    update_bookmarks_status_on_checkbox_click(current_row_selection_int,enum_File_System_display_BOOKMARK);


}



void file_system::slot_gallery_spacebar_quicklook_pressed(struct_global_gallery_view_file_info f1)
{
    slot_spacebar_quicklook_key_pressed_on_tablewidget();
}

void file_system::slot_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info f1)
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    file_export_dialog_obj->show();

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);

}



void file_system::slot_gallery_view_item_clicked(struct_global_gallery_view_file_info f1)
{
    QString record =   f1.INT;

    if(record.isEmpty())
    {
        bool_cell_changed_connection_buildup_for_tablewidget = false;
        m_file_system_tablewidget_obj->clearSelection();
        emit signal_clear_all_metadata();
        return;
    }

    /// control goes to table view tab
    /// now all operations can be done on basis of tablewidget_obj

    int current_row_selection_int = f1.table_view_row_no;
    for(int pp = 0 ; pp < m_file_system_tablewidget_obj->rowCount() ; pp++)
    {
        if((f1.INT == m_file_system_tablewidget_obj->item(pp,enum_File_System_display_INT)->text())
                && (f1.source_count_name == m_file_system_tablewidget_obj->item(pp,enum_File_System_display_SOURCE_COUNT_NAME)->text()))
        {
            current_row_selection_int = pp;
            break;
        }
    }


    m_file_system_tablewidget_obj->selectRow(current_row_selection_int);

    if(!bool_cell_changed_connection_buildup_for_tablewidget)
        m_file_system_tablewidget_obj->cellClicked(current_row_selection_int,enum_File_System_display_BOOKMARK);


    // context menu emits tablewidget click
}

void file_system::slot_gallery_view_item_double_clicked(struct_global_gallery_view_file_info f1)
{
    QString record_no =   f1.INT;
    if(record_no.isEmpty())
        return;

    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(f1.source_count_name);
    QString file_full_path;

    if(struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
            || struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
    {
        QString display_file_path = f1.file_path;
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + struct_info.source_count_name + "/file_system.sqlite";

        QString cache_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_iOS_Preview_Cache_In_Result_QString).toString() + f1.source_count_name;
        QString unique_filename_str = f1.source_count_name + "_" + record_no + "_" + QFileInfo(display_file_path).fileName();

        QString idpath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("select id_path from files where filepath LIKE ?" , QStringList(display_file_path), 0, fs_db_path, Q_FUNC_INFO);
        if(!idpath.isEmpty())
        {
            file_full_path = cache_dir_path + "/" + unique_filename_str;
        }
    }
    else
    {
        file_full_path = f1.full_filepath;
    }

    QStringList argss;
    argss << file_full_path;

    recon_helper_process_obj->run_command_with_arguments("open",argss,Q_FUNC_INFO);

}

void file_system::slot_gallery_view_invalid_tem_clicked()
{
    m_file_system_tablewidget_obj->clearSelection();
    emit signal_clear_all_metadata();
}


void file_system::slot_table_widget_header_item_clicked(int column_index)
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    if(column_index < 0)
        return;


    if(st_gallery_view_file_info_list.size() <= 0)
        return;


    QList<struct_global_gallery_view_file_info>  sorted_gallery_info_qlist;

    QMap<int ,struct_global_gallery_view_file_info> index_row_qmap;
    index_row_qmap.clear();
    QList<int> row_no_QList;
    row_no_QList.clear();

    for(int pp = 0 ; pp < st_gallery_view_file_info_list.size() ; pp++)
    {
        struct_global_gallery_view_file_info f1 = st_gallery_view_file_info_list.at(pp);

        for(int qq = 0 ; qq < m_file_system_tablewidget_obj->rowCount() ; qq++)
        {

            if((f1.INT == m_file_system_tablewidget_obj->item(qq,enum_File_System_display_INT)->text())
                    && (f1.source_count_name == m_file_system_tablewidget_obj->item(qq,enum_File_System_display_SOURCE_COUNT_NAME)->text()))
            {

                int row_no = qq;
                row_no_QList.append(row_no);

                index_row_qmap.insert(row_no , f1);

            }
        }

    }

    std::sort(row_no_QList.begin(),row_no_QList.end());

    for(int ii = 0 ; ii < row_no_QList.size() ; ii++)
    {
        sorted_gallery_info_qlist.append(index_row_qmap.value(row_no_QList.at(ii)));
    }


    st_gallery_view_file_info_list.clear();

    st_gallery_view_file_info_list = sorted_gallery_info_qlist;

    bool_did_tableview_display_changed = true;

    recon_static_functions::app_debug("End " , Q_FUNC_INFO);

}
