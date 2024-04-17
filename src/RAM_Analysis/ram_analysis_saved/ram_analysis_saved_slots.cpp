#include "ram_analysis_saved_loader.h"

void ram_analysis_saved_loader::slot_add_notes_pushbutton_clicked(QString text)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    add_notes_to_singal_record_obj->hide();

    text = text.trimmed();


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;


        if(m_tablewidget_obj->isRowHidden(selected_row_no))
                    continue;

        QString plugin_name_str = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text().trimmed();

        if(plugin_name_str.isEmpty())
            continue;

        int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

        QString is_bookmarked = "0";
        if(bookmark_index != -1)
        {
            if(m_tablewidget_obj->item(selected_row_no, bookmark_index)->checkState() != 0)
            {
                is_bookmarked = "1";
            }
        }
        update_notes_value(text,selected_row_no);

    }
    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}


