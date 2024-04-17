#ifndef CASE_WIZARD_H
#define CASE_WIZARD_H


#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "QTreeWidgetItem"
#include "pages/case_launcher_page_case_info.h"
#include "pages/case_launcher_page_case_directory.h"

#include "case_info/case_info_details.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "launcher/plugin_selector/plugin_selector.h"

#include "pages/page_filesystem_n_modules.h"

#include "recon_configuration/recon_configuration.h"
#include "header_identical_2.h"
#include "launcher/case_launcher/pages/case_launcher_page_timezone_selection.h"
#include "header_global_structures.h"

#include "launcher/source_selector/source_selector.h"

#include "acquire_devices/ios_devices/acquire_iOS_devices.h"
#include "acquire_devices/android_devices/acquire_android_devices.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "disk_manager/Disk_Manager/disk_manager.h"


namespace Ui {
class case_wizard;
}

class case_wizard : public QWidget
{
    Q_OBJECT

public:
    explicit case_wizard(QWidget *parent = 0);
    ~case_wizard();


    void closeEvent(QCloseEvent *event);

    void pub_set_message_in_label_for_unmounting_and_enable_disable_ui(QString message_str, bool value);

    void pub_set_window_title(QString);


    disk_manager *pub_get_disk_manager_object();
    void pub_set_qml_on_first_main_page();
    void pub_set_lic_info(QString username, qint64 license_expiry_date_numeric_qint64);
    Q_INVOKABLE void pbin_button_on_splash_window_clicked(QString m_value);
    Q_INVOKABLE QString pbin_get_label(QString m_value);
    Q_INVOKABLE QString pbin_get_icons_path(QString icon_name);

private slots:
    void on_pushButton_previous_clicked();

    void on_pushButton_next_clicked();

    void on_pushButton_new_case_clicked(bool checked);

    void on_pushButton_load_case_clicked(bool checked);

    void on_stackedWidget_case_launcher_currentChanged(int arg1);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void slot_result_dir_path_selected(QString path);

    void slot_case_info_essentials_filled(QString info);

    void slot_total_plugins_selected(bool status);

    void slot_source_selected(bool is_source_selected);


    void on_pushButton_about_recon_clicked();

    void on_pushButton_back_from_about_recon_clicked();

    void on_pushButton_recon_configuration_clicked();

    void slot_backup_locater_source_selected(bool ststus);

    void slot_enable_disable_next_button_from_timezone_selection_page(bool value);

    void on_pushButton_acquire_device_clicked();

    void slot_bring_recon_lab_banner_page_in_front();

    void on_pushButton_check_for_updates_clicked();

    void slot_label_download_link_clicked(QString str);
    void network_error(QNetworkReply::NetworkError);
    void onRequestCompleted();
    void slot_menu_triggered_load_case(QAction* current_clicked_action);


    void slot_abouttoshow_recent_result_list();
    void on_pushButton_support_clicked();
    void on_pushButton_online_manual_clicked();


    void slot_menu_triggered_acquire_devices(QAction *current_clicked_action);
    void slot_abouttoshow_acquire_devices();
    void on_pushButton_disk_manager_clicked();

    void slot_added_root_details_list(QList<struct_GLOBAL_witness_info_root> target_list);
    void slot_hide_source_selector_parent_window();
    void slot_qml_button_initial_back_clicked();
    void on_pushButton_back_clicked();

public slots:
    void slot_set_license_label_text(QString name, qint64 pd, qint64 ed);

signals:
    //void signal_case_launched(QString case_name, QList<struct_GLOBAL_derived_source_info>);
    void signal_case_launched(QString case_name, QList<struct_GLOBAL_witness_info_root>);
    void signal_load_case_clicked(QString path);


private:
    Ui::case_wizard *ui;



    enum main_stack_page{
        enum_case_wizard_stack_main_first_page = 0,
        enum_case_wizard_stack_qml_splash_screen,
        enum_case_wizard_stack_main_new_case_selections_stack,
        enum_case_wizard_stack_main_about_recon,
        enum_case_wizard_stack_acquie_ios_device,
        enum_case_wizard_stack_acquie_android_device,
        enum_case_wizard_stack_recon_configuration

    };

    enum wizard_page{

        enum_common_case_wizard_page_case_info_selction = 0,
        enum_comman_case_wizard_page_multi_source_selection,
        enum_common_case_wizard_page_case_directory_selection,
        enum_comman_case_wizard_page_timezone_selection,
        enum_comman_case_wizard_page_file_system_selection,
        enum_comman_case_wizard_page_plugin_selection
    };


    QString MVAR_LOAD_RESULT_QString_Other_Case;
    QString MVAR_ACQUIRE_DEVICES_QString_iOS_Device;
    QString MVAR_ACQUIRE_DEVICES_QString_androidOS_Device;

    source_selector *source_selector_obj;

    recon_configuration *recon_configuration_obj;

    case_info_details *case_info_details_obj;
    plugin_selector *plugin_selector_obj;
    page_filesystem_n_modules *page_filesystem_n_modules_obj;
    case_launcher_page_timezone_selection *page_timezone_selection_obj;
    page_case_info *page_case_info_object;
    page_case_directory *case_directory_object;


    void display_treewidget_item();
    int tree_selected_item;

    QStringList list_wizard_page_opened;
    QString result_dir_path;

    bool bool_device_selected;
    bool bool_source_selected;
    bool bool_case_info_filled;
    bool bool_case_dir_selected;
    bool bool_plugins_selected;
    bool bool_backup_source_selected;

    void main_stack_widget_functionality();

    void comman_case_info_essentials_filled(QString);

    bool bool_treewidget_navigated;

    void set_initialise_pages_information();

    void create_copy_for_case_configuration_dir();

    message_dialog *message_dialog_obj;

    void collect_page_details_from_all_pages();

    QList<struct_GLOBAL_witness_info_root> list_root_info;

    QString launched_case_name;
    acquire_iOS_devices *acquire_iOS_devices_obj;
    acquire_android_devices *acquire_android_devices_obj;

    QNetworkAccessManager *m_networkManager;
    QString download_link;

    QMenu *qmenu_load_case_recent_result;
    QMenu *qmenu_acquire_devices;
    void action_load_other_case_clicked();
    void action_load_already_added_case_clicked(QString load_case_str);
    void set_loaded_result_path(QString loaded_result_path);
    QFileDialog file_dialog_obj;
    recon_helper_standard *recon_helper_standard_obj;

    disk_manager *disk_manager_obj;
    QString license_customer_name;
    QString license_expiry_date;

};

#endif // CASE_WIZARD_H
