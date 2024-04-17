#ifndef REDEFINED_RESULT_FILTERS_PLUGINS_TAB_SELECT_H
#define REDEFINED_RESULT_FILTERS_PLUGINS_TAB_SELECT_H

#include <QFrame>
#include "recon_generic_functions/recon_static_functions.h"
                                   
#include "message_box/message_dialog.h"
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class redefined_result_filters_plugins_tab_select;
}

class redefined_result_filters_plugins_tab_select : public QFrame
{
    Q_OBJECT

public:
    explicit redefined_result_filters_plugins_tab_select(QWidget *parent = 0);
    ~redefined_result_filters_plugins_tab_select();

    void pub_clear_plugins_tab_tablewidget();
    QString func_to_set_stylesheet();


    void pub_display_plugins_information();

signals:
    void signal_selected_plugin_and_tab(QString, QString);

private slots:
    void slot_category_tablewidget_single_clicked(int row, int col);
    void slot_plugin_name_tablewidget_single_clicked(int row, int col);

    void on_pushButton_add_plugin_tab_clicked();

private:
    Ui::redefined_result_filters_plugins_tab_select *ui;


    struct strct_plugin_info{
        QString parent_name;
        QString plugin_name;
        QStringList  plugin_tab_name_list;
    };


    QList<strct_plugin_info> list_all_plugin_info;
    QList<strct_plugin_info> list_plugins_in_category;

    QStringList plugin_categories_list;
    QStringList selected_plugin_tabs_list;

    message_dialog *message_dialog_obj;
    recon_helper_standard *recon_helper_standard_obj;


};

#endif // REDEFINED_RESULT_FILTERS_PLUGINS_TAB_SELECT_H
