#include "quick_look.h"


quick_look::quick_look(QWidget *parent)
{
    setAttribute(Qt::WA_MacShowFocusRect,0);

    setMaximumHeight(1);
    setMaximumWidth(1);

    setGeometry(1,1,1,1);

//    setMinimumHeight(500);
//    setMinimumWidth(500);

    QStringList list;
    list  << QString("");

    list_model = new QStringListModel(list);
    list_model->setStringList(list);

    setModel(list_model);

    global_variable_quick_look_preview_close_status_bool = false;
}

quick_look::~quick_look()
{

}

void quick_look::pub_set_preview_filepath(QString path)
{
    global_variable_quick_look_preview_filepath_QString = path;

    showPreview();
}


bool quick_look::pub_get_preview_closed_status()
{
    return global_variable_quick_look_preview_close_status_bool;
}

void quick_look::pub_set_preview_closed_status(bool status)
{
    global_variable_quick_look_preview_close_status_bool = status;
}

