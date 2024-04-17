#include "center_next_previous_navigation.h"

center_next_previous_navigation::center_next_previous_navigation(QWidget *parent)
{

}

center_next_previous_navigation::~center_next_previous_navigation()
{

}

QString center_next_previous_navigation::pub_get_next_stackwidget_index_from_list(QString current_unique_index)
{
    recon_static_functions::debug_intensive(" -Start",Q_FUNC_INFO);

    QString next_unique_name = "No Page";
    for(int i = 0; i < list_pages_unique_name.size();i++)
    {
        if(list_pages_unique_name.at(i) == current_unique_index)
        {
            if(i < (list_pages_unique_name.size() -1))
            {
                next_unique_name = list_pages_unique_name.at(i + 1);
                emit signal_set_case_tree_item_bold_on_navigation_button_click(next_unique_name);

                if((i + 1) == (list_pages_unique_name.size() -1))
                    emit signal_page_is_last_record(true);
                else
                    emit signal_page_is_last_record(false);

                break;
            }
        }
    }

    recon_static_functions::debug_intensive(" -End",Q_FUNC_INFO);

    return next_unique_name;
}

QString center_next_previous_navigation::pub_get_previous_stackwidget_index_from_list(QString current_unique_index)
{
    recon_static_functions::debug_intensive(" -Start",Q_FUNC_INFO);

    QString previous_unique_name = "No Page";
    for(int i = 0; i < list_pages_unique_name.size();i++)
    {
        if(list_pages_unique_name.at(i) == current_unique_index)
        {
            if(i > 0)
            {
                previous_unique_name = list_pages_unique_name.at(i - 1);
                emit signal_set_case_tree_item_bold_on_navigation_button_click(previous_unique_name);

                if((i - 1) == 0)
                {
                    emit signal_page_is_first_record(true);
                }
                else
                {
                    emit signal_page_is_first_record(false);
                }

                break;
            }
        }
    }

    recon_static_functions::debug_intensive(" -End",Q_FUNC_INFO);

    return previous_unique_name;
}


void center_next_previous_navigation::slot_update_navigation_list_on_combobox_change( QString page_name)
{
    update_navigation_list_on_combobox_change(page_name);
}

void center_next_previous_navigation::slot_remove_navigation_list_entry_on_combobox_pusbutton_close( QString page_name)
{
    remove_navigation_list_entry_on_combobox_pusbutton_close(page_name);
}

void center_next_previous_navigation::slot_insert_new_entry_in_navigation_list( QString current_page_unique_name)
{
    insert_new_entry_in_navigation_list(current_page_unique_name);
}


void center_next_previous_navigation::insert_new_entry_in_navigation_list(QString current_page_unique_name)
{
    recon_static_functions::debug_intensive(" -Start",Q_FUNC_INFO);

    if(current_page_unique_name.contains(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CASE)
            || current_page_unique_name.contains(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CATEGORY)
            || current_page_unique_name.contains(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE))
    {
        return;
    }


    //-To Check weather list contains any item or mnot.
    bool bool_is_list_contains_item = false;

    if(list_pages_unique_name.contains(current_page_unique_name))
        bool_is_list_contains_item = true;

    if(bool_is_list_contains_item)
        return;

    list_pages_unique_name << current_page_unique_name;

    emit signal_set_case_tree_item_bold_on_navigation_button_click(current_page_unique_name);

    recon_static_functions::debug_intensive(" -END",Q_FUNC_INFO);
}

void center_next_previous_navigation::update_navigation_list_on_combobox_change( QString page_name)
{
    recon_static_functions::debug_intensive("  -Start",Q_FUNC_INFO);

    int index = -1;
    bool bool_page_name_matched = false;
    for(int i = 0; i < list_pages_unique_name.size();i++)
    {

        QString tmp_name_str = list_pages_unique_name.at(i);

        if(tmp_name_str == page_name)
        {
            index = i;
            bool_page_name_matched = true;
            break;
        }

    }

    if(!bool_page_name_matched)
    {
        recon_static_functions::app_debug(" ----------FAILED----------",Q_FUNC_INFO);
        recon_static_functions::app_debug(" -Unique name not matched->>> " + page_name ,Q_FUNC_INFO);
    }

    if(index != -1)
    {
        QString unique_name = list_pages_unique_name.at(index);
//        list_pages_unique_name.removeAt(index);
//        list_pages_unique_name << unique_name;
        emit signal_set_case_tree_item_bold_on_navigation_button_click(unique_name);
    }

    recon_static_functions::debug_intensive(" -End",Q_FUNC_INFO);
}

void center_next_previous_navigation::remove_navigation_list_entry_on_combobox_pusbutton_close(QString page_name)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    int index = -1;
    bool bool_debug_page_name_matched = false;
    for(int i = 0; i < list_pages_unique_name.size();i++)
    {
        if(list_pages_unique_name.at(i).startsWith(page_name))
        {
            index = i;
            bool_debug_page_name_matched = true;
            break;
        }
    }

    if(!bool_debug_page_name_matched)
    {
        recon_static_functions::app_debug(" ----------FAILED----------",Q_FUNC_INFO);
        recon_static_functions::app_debug(" -Unique name not matched->>> " + page_name ,Q_FUNC_INFO);
        return;
    }

    if(index != -1)
    {
        list_pages_unique_name.removeAt(index);
    }


    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void center_next_previous_navigation::slot_set_center_next_previous_navigation_list_page_unique_name(QString previous_str, QString current_feature)
{
    QStringList list = list_pages_unique_name;

    list_pages_unique_name.clear();

    for(int i = 0; i < list.size();i++)
    {

        if(list.at(i).contains(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CASE)
                || list.at(i).contains(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CATEGORY)
                || list.at(i).contains(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE))
        {
            continue;
        }


        QString str = list.at(i);

        str = recon_static_functions::remove_record_count_from_feature_name(str);

        if(previous_str.startsWith(str))
        {
            previous_str = list.at(i);
            continue;
        }

        if(current_feature.startsWith(str))
        {
            current_feature = list.at(i);
            continue;
        }

        if(!list_pages_unique_name.contains(list.at(i)))
        list_pages_unique_name << list.at(i);

    }

    if(!previous_str.contains(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CASE)
            && !previous_str.contains(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CATEGORY)
            && !previous_str.contains(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE))
    {
        if(!list_pages_unique_name.contains(previous_str))
            list_pages_unique_name << previous_str;
    }



    if(!current_feature.contains(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CASE)
            && !current_feature.contains(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CATEGORY)
            && !current_feature.contains(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE))
    {
        if(!list_pages_unique_name.contains(current_feature))
            list_pages_unique_name << current_feature;
    }

}
