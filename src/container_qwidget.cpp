#include "container_qwidget.h"

Container_QWidget::Container_QWidget(QWidget *parent) :
    QWidget(parent)
{

}

Container_QWidget::~Container_QWidget()
{

}

void Container_QWidget::pub_set_feature_widget_name(QString wd_name)
{
    widget_name = wd_name;
}

void Container_QWidget::pub_set_category_name(QString cat_nm)
{
    category_name = cat_nm;
}

void Container_QWidget::pub_set_case_name(QString case_nm)
{
    case_name = case_nm;
}

void Container_QWidget::pub_set_widget_unique_name(QString u_nm)
{
    unique_name = u_nm;
}

QString Container_QWidget::pub_get_feature_widget_name()
{
    return widget_name;
}

QString Container_QWidget::pub_get_category_name()
{
    return category_name;
}

QString Container_QWidget::pub_get_case_name()
{
    return case_name;
}

QString Container_QWidget::pub_get_widget_unique_name()
{
    return unique_name;
}

