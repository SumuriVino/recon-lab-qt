#ifndef CENTER_NEXT_PREVIOUS_NAVIGATION_H
#define CENTER_NEXT_PREVIOUS_NAVIGATION_H

#include <QString>
#include <QtCore>
#include <QWidget>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_identical_2.h"
#include "header_global_variables_1.h"
#include "header_global_variables_2.h"


class center_next_previous_navigation : public QWidget
{
    Q_OBJECT

public:
    center_next_previous_navigation(QWidget *parent = 0);
    ~center_next_previous_navigation();

    QString pub_get_next_stackwidget_index_from_list(QString current_unique_index);
    QString pub_get_previous_stackwidget_index_from_list(QString current_unique_index);


signals:
    void signal_page_is_first_record(bool value);
    void signal_page_is_last_record(bool value);
    void signal_set_case_tree_item_bold_on_navigation_button_click(QString text);


private slots:

    void slot_update_navigation_list_on_combobox_change(QString page_name);
    void slot_remove_navigation_list_entry_on_combobox_pusbutton_close(QString page_name);
    void slot_insert_new_entry_in_navigation_list(QString current_page_unique_name);


    void slot_set_center_next_previous_navigation_list_page_unique_name(QString previous_str, QString current_feature);
private:
    QStringList list_pages_unique_name;

    void insert_new_entry_in_navigation_list( QString current_page_unique_name);
    void update_navigation_list_on_combobox_change(QString page_name);
    void remove_navigation_list_entry_on_combobox_pusbutton_close(QString page_name);


};

#endif // CENTER_NEXT_PREVIOUS_NAVIGATION_H
