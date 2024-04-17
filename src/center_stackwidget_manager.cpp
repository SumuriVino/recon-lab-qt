#include "center_stackwidget_manager.h"

center_stackwidget_manager::center_stackwidget_manager(QWidget *parent)
    :QWidget(parent)

{
    list_container_qwidget.clear();
}

center_stackwidget_manager::~center_stackwidget_manager()
{

}

void center_stackwidget_manager::pub_set_container_widget_ptr_in_list(Container_QWidget *widget)
{
    list_container_qwidget << widget;
}

Container_QWidget *center_stackwidget_manager::pub_get_container_widget_ptr(QString unique_name)
{
    Container_QWidget *containor_wgtp = new Container_QWidget(this);

    for(int l = 0; l < list_container_qwidget.size(); l++)
    {
        QString temp_str = QString(list_container_qwidget.at(l)->pub_get_widget_unique_name());

        if(temp_str != unique_name)
            continue;

        containor_wgtp = list_container_qwidget.at(l);
        return containor_wgtp;
    }

    return containor_wgtp;
}

QStringList center_stackwidget_manager::pub_get_case_name_list()
{
    QStringList case_list;
    for(int i = 0; i < list_container_qwidget.size(); i++)
    {
        Container_QWidget *widget =  list_container_qwidget.at(i);

        case_list << widget->pub_get_case_name();
    }

    case_list.removeDuplicates();
    return case_list;
}

QStringList center_stackwidget_manager::pub_get_category_name_list(QString case_name)
{
    QStringList category_list;
    for(int i = 0; i < list_container_qwidget.size(); i++)
    {
        Container_QWidget *widget =  list_container_qwidget.at(i);
        if(widget->pub_get_case_name() != case_name)
            continue;

        category_list << widget->pub_get_category_name();
    }

    category_list.removeDuplicates();

    return category_list;
}

QStringList center_stackwidget_manager::pub_get_feature_name_list(QString category_name)
{
    QStringList feature_list;
    for(int i = 0; i < list_container_qwidget.size(); i++)
    {
        Container_QWidget *widget =  list_container_qwidget.at(i);
        if(widget->pub_get_category_name() != category_name)
            continue;

        feature_list << widget->pub_get_feature_widget_name();
    }

    feature_list.removeDuplicates();
    return feature_list;
}

void center_stackwidget_manager::pub_remove_entry_feature_form_list(QString unique_name)
{
    for(int fea = (list_container_qwidget.size() -1) ; fea >= 0; fea--)
    {
        QString temp_str = QString(list_container_qwidget.at(fea)->pub_get_widget_unique_name());
        if(temp_str != unique_name)
            continue;

        if(temp_str == unique_name)
        {
            Container_QWidget *widget_ptr = dynamic_cast<Container_QWidget *>(list_container_qwidget.at(fea));
            list_container_qwidget.removeAt(fea);
            if(widget_ptr->pub_get_category_name() != MACRO_CASE_TREE_Source)
            {
                delete widget_ptr;
            }
            break;
        }
    }

}

void center_stackwidget_manager::pub_remove_category_entry_from_list(QString unique_category_pattern)
{
    for(int cat = (list_container_qwidget.size() -1) ; cat >= 0; cat--)
    {
        QString temp_str = QString(list_container_qwidget.at(cat)->pub_get_widget_unique_name());
        if(!temp_str.contains(unique_category_pattern))
            continue;

        if(temp_str.contains(unique_category_pattern))
        {
            list_container_qwidget.removeAt(cat);

        }
    }

}

void center_stackwidget_manager::pub_remove_case_entry_from_list(QString rcvd_case_name)
{
    for(int i = (list_container_qwidget.size() -1) ; i >= 0; i--)
    {
        QString temp_case_str = QString(list_container_qwidget.at(i)->pub_get_case_name());
        if(temp_case_str != rcvd_case_name)
            continue;

        if(temp_case_str == rcvd_case_name)
        {
            list_container_qwidget.removeAt(i);
        }
    }

}

