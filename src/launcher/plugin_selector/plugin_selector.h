#ifndef PAGE_REPORT_PLUGINS_SELECTION_H
#define PAGE_REPORT_PLUGINS_SELECTION_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QFocusEvent>


#include "recon_generic_functions/recon_static_functions.h"
#include "header_identical_1.h"
#include "header_global_variables_1.h"
#include "message_box/message_dialog.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"

namespace Ui {
class plugin_selector;
}

class plugin_selector : public QFrame
{
    Q_OBJECT

public:
    explicit plugin_selector(QWidget *parent = 0);
    ~plugin_selector();

    void set_narad_muni_for_report(narad_muni*);
    void pub_set_launcher_type(QString launcher);

    //- Right Click Launcher.
    void pub_initialise_plugin_info_for_right_click_run_plugin();
    bool pub_get_selected_plugins_from_launcher_for_add_source();

    //---Multi OS support.
    //-Recon Configuration.
    void pub_initialise_multi_os_plugins_from_recon_configuration();
    void pub_update_default_multi_OS_plugins_db_for_recon_configuration();

    //-Main New Case Launcher Work.
    void pub_initialise_plugin_info_for_main_new_case_launcher();
    void pub_get_selected_plugins_from_main_case_launcher();

    //-Global Report.
    void pub_initialise_plugin_info_for_global_report();
    void pub_put_plugin_info_for_global_report();

    QStringList pub_get_seleted_tabs_and_plugins_for_Keyword_Search();

    //-Keyword Search Configuration Window.
    void pub_initialise_plugin_info_for_keyword_search();

    bool pub_get_selected_plugins_from_launcher_for_run_plugin();

    void pub_set_widget_content_margin(int left, int top, int right, int bottom);


    bool pub_bool_run_plugin_having_record();
signals:
    void signal_plugin_selected(bool);
    void signal_plugin_and_tab_list_from_plugin_selector(QStringList);

private slots:
    void on_lineEdit_plugin_search_textChanged(const QString &arg1);

    void on_comboBox_template_plugins_selection_currentTextChanged(const QString &arg1);

    void on_checkBox_save_template_clicked(bool checked);

    void on_lineEdit_save_template_returnPressed();

    void on_pushButton_remove_template_clicked();

    void on_lineEdit_save_template_textChanged(const QString &arg1);

    void on_treeWidget_plugins_itemClicked(QTreeWidgetItem *item, int column);

    void on_tableWidget_parent_list_cellClicked(int row, int column);

    void on_treeWidget_plugins_itemChanged(QTreeWidgetItem *item, int column);

    void on_pushButton_add_template_clicked();

private:
    Ui::plugin_selector *ui;

    enum enum_plugins_tree_column{
        enum_plugin_tree_column_plugin_translated_name = 0,
        enum_plugin_tree_column_plugin_original_name,
        enum_plugin_tree_column_runned_plugin,
        enum_plugin_tree_column_export_available,
        enum_plugin_tree_column_macOS,
        enum_plugin_tree_column_iOS,
        enum_plugin_tree_column_winOS,
        enum_plugin_tree_column_gTakeoutOS,
        enum_plugin_tree_column_androidOS,
        enum_plugin_tree_column_blank
    };
    enum enum_parent_tablewidget_column{
        enum_parent_tablewidget_column_translated_name = 0,
        enum_parent_tablewidget_column_original_name

    };

    struct struct_report_plugin_info{
        QString parent_name;
        QString plugin_name;
        QStringList  plugin_tab_name_list;
        bool is_export_selected;
        bool is_export_available;
        bool is_selected;

        QStringList supported_os_scheme_display_QStringList;

        QStringList selected_plugins_tabs_list;
        QStringList export_available_tabs_list;
        QStringList export_enabled_tabs_list;
    };

    QString M_VAR_PLUGIN_TEMPLATE_Plugin_selection_QString;
    QString M_VAR_PLUGIN_TEMPLATE_Select_All_QString;
    QString M_VAR_PLUGIN_TEMPLATE_Remove_All_QString;
    QString M_VAR_PLUGIN_TEMPLATE_Select_All_Without_Export_QString;
    QString M_VAR_PLUGIN_TEMPLATE_Select_All_With_Export_QString;
    QString M_VAR_PLUGIN_Parent_All_Plugins_QString;
    QString M_VAR_FUNCTIONALITY_Not_Available_QString;


    QList<struct_report_plugin_info> list_plugins_info;

    narad_muni *narad_muni_for_report_obj;
    QStringList list_plugin_parents;
    int total_plugin_selected;

    ///- Selected plugins Extraction work.
    struct struct_display_plugins_info{
        QString plugin_name;
        QStringList tabs_list;
    };
    QList<struct_display_plugins_info> list_plugin_and_tab_selected_at_extraction;

    QString launcher_type;
    int select_or_remove;


    bool bool_plugin_is_checked;
    QString destination_db_file;

    void fill_template_combobox();
    void set_hide_widgets(bool value);

    bool bool_combobox_updating;

    //---Multi OS support.
    void initialise_plugins_for_recon_config_and_new_case_launcher(QString destination_db_file);
    void display_plugins_tree_for_multiple_os(QList<struct_report_plugin_info> &list_plugins_info);
    void display_parent_category_list_for_multiple_os(QStringList &list_plugin_parents);
    void set_destination_db_file_path_for_multi_os();
    void initialise_plugins_for_global_report_comman_for_multi_os(QString destination_db_file);
    void initialise_plugin_info_for_keyword_search_comman_for_multi_os( QString destination_db_file);
    void initialise_plugin_info_for_right_click_run_plugin_comman_for_multi_os(QString destination_db_file);

    //--Comman For all work..
    int total_plugin_selected_common;
    QStringList list_plugin_parents_common;
    QList<struct_report_plugin_info> list_plugins_info_common;
    void on_treeWidget_plugins_itemClicked_new_case_common(QTreeWidgetItem *item, int column);
    void on_treeWidget_plugins_itemClicked_run_plugin_common(QTreeWidgetItem *item, int column);
    void select_all_with_export_clicked_common();
    void select_all_without_export();
    void remove_all_clicked_common();
    void select_template_plugins_for_common(QString template_name);
    void lineEdit_plugin_search_textChanged_common(const QString &arg1);
    void get_selected_plugins_from_main_case_launcher_common();
    QStringList get_plugin_info_for_global_report_common();
    QStringList get_seleted_tabs_and_plugins_for_Keyword_Search_common();
    bool get_selected_plugins_from_run_plugin_common();

    message_dialog *message_dialog_object;

    bool bool_plugins_tree_updated;
    QString supported_parent_macOS;
    QString supported_parent_iOS;
    QString supported_parent_winOS;
    QString supported_parent_gtakeoutOS;
    QString supported_parent_androidOS;

    recon_helper_standard *recon_helper_standard_obj;

};

#endif // PAGE_REPORT_PLUGINS_SELECTION_H
