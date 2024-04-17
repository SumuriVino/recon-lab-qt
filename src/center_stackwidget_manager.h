#ifndef CENTER_STACKWIDGET_MANAGER_H
#define CENTER_STACKWIDGET_MANAGER_H

#include <QObject>
#include <QtCore>
#include "container_qwidget.h"
#include <QWidget>
#include "header_identical_3.h"

class center_stackwidget_manager: public QWidget
{
    Q_OBJECT
public:
    explicit center_stackwidget_manager(QWidget *parent = 0);
    ~center_stackwidget_manager();

    void pub_set_container_widget_ptr_in_list(Container_QWidget *widget);

    QStringList pub_get_feature_name_list(QString category_name);
    QStringList pub_get_category_name_list(QString case_name);
    QStringList pub_get_case_name_list();
    Container_QWidget *pub_get_container_widget_ptr(QString unique_name);

    void pub_remove_entry_feature_form_list(QString unique_name);
    void pub_remove_category_entry_from_list(QString unique_category_pattern);
    void pub_remove_case_entry_from_list(QString case_name);

signals:

public slots:

private:

    QList<Container_QWidget*> list_container_qwidget;

};

#endif // CENTER_STACKWIDGET_MANAGER_H
