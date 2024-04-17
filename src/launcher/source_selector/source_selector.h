#ifndef SOURCE_SELECTOR_H
#define SOURCE_SELECTOR_H

#include <QWidget>
#include <QtCore>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QQuickWidget>
#include <QQmlContext>
#include <QMessageBox>
#include "launcher/source_selector/source_type/mounted_volumes_class.h"
#include "launcher/source_selector/source_type/file_vault_image_class.h"
#include "launcher/source_selector/source_type/fusion_drive_class.h"
#include "launcher/source_selector/source_type/home_directory_class.h"

#include "source_widget/source_widgets.h"
#include "header_global_structures.h"
#include "header_identical_1.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"


namespace Ui {
class source_selector;
}

class source_selector : public QWidget
{
    Q_OBJECT

public:
    explicit source_selector(QWidget *parent = 0);
    ~source_selector();

    void pub_hide_widgets_for_load_case();
    void pub_set_root_info_in_page(QStringList root_path_with_type_list);

    void pub_get_essential_value_from_all_roots();

    QList<struct_GLOBAL_witness_info_root> pub_get_root_info_list_from_gui();

    Q_INVOKABLE void pbin_button_back_clicked();
    Q_INVOKABLE void pbin_on_close_clicked();
    Q_INVOKABLE QString pbin_get_label(QString m_value);
    Q_INVOKABLE QString pbin_button_source_path_selection_clicked(QString rt_value, QString src_internal_value);
    Q_INVOKABLE void pbin_button_add_selected_sources(QString evdnc_type,QString rt_type,QString src_display_type,QString src_internal_type,QString src_path,QString password,QString fusion_path,QString username);
    Q_INVOKABLE void pbin_set_messagebox_for_unsupported_image(QString rt_value, QString src_internal_value);
    Q_INVOKABLE bool pbin_is_image_supported(QString rt_value, QString src_internal_value, QString image_path);
    Q_INVOKABLE QString pbin_get_icons_path(QString icon_name);

signals:
    void signal_source_selected(bool);
    void signal_added_roots_details_list(QList<struct_GLOBAL_witness_info_root>);

    void signal_hide_source_selector_parent_window();
    void signal_qml_button_initial_back_clicked();

private slots:
    void slot_mounted_volumes_selected(QStringList media_list);
    void slot_toolButton_add_source_clicked();
    void slot_toolButton_add_source_clicked_with_menu(QAction* current_clicked_action);
    void slot_remove_button_clicked(int id, QString name);
    void slot_file_vault_password_inserted(QString passwd);
    void slot_home_directory_user_name_inserted(QString username);
    void slot_fusion_drives_selected(QStringList fusion_drives);
    void slot_source_submenu_action_triggered(QAction *current_clicked_action);
    void on_pushButton_add_source_qml_clicked();

private:
    Ui::source_selector *ui;

    recon_helper_standard *recon_helper_standard_obj;

    QMenu *menu_main;

    QMenu *menu_evidence_physical;
    QMenu *menu_evidence_logical;
    QMenu *menu_evidence_mobile;
    QMenu *menu_evidence_cloud;
    QMenu *menu_evidence_network;

    QAction *action_mounted_volumes;
    QAction *action_forensics_image;
    QAction *action_file_vault_image;
    QAction *action_fusion_drive_image;
    QAction *action_home_directory;
    QAction *action_optical_disk_image;
    QAction *action_folder;
    QAction *action_files;
    QAction *action_time_machine;
    QAction *action_ios_backup_itunes;


    QAction *action_RAM_Image;
    QAction *action_google_takeout;
    QMenu *menu_recon_logical_image;
    QMenu *menu_recon_mac_sharing_mode;
    QMenu *menu_encase_logical_image;
    QMenu *menu_android_backup;
    QMenu *menu_recon_fs_block_image;
    QMenu *menu_ios_backup_cellebrite;

    //QMenu *menu_ios_backup;


    int src_widget_id;
    int prnt_widget_id;
    int root_count_index;

    void function_mounted_volumes_triggered();
    void function_forensics_image_triggered();
    void function_file_vault_triggered();
    void function_fusion_drive_triggered();
    void function_recon_fs_block_image_triggered();
    void function_home_directory_triggered();
    void function_folder_triggered();
    void function_file_triggered();
    void function_time_machine_triggered();
    void function_ios_uid_backup_triggered();
    void function_RAM_image_triggered();
    void function_google_takeout_triggered();
    void function_rcn_logical_image_sparseimage_triggered();
    void function_rcn_logical_image_dmg_triggered();
    void function_rcn_logical_image_folder_triggered();
    void function_encase_logical_image_triggered();
    void function_recon_mac_sharing_mode_sparseimage_triggered();
    void function_recon_mac_sharing_mode_dmg_triggered();
    void function_recon_mac_sharing_mode_folder_triggered();
    void function_optical_disk_image_triggered();
    void function_adb_android_backup_folder_triggered();
    void function_adb_android_backup_ab_file_triggered();
    void function_cellebrite_ios_backup_ufdr_file_triggered();
    void function_cellebrite_ios_backup_tar_file_triggered();

    QString file_vault_path;
    QString home_directory_path;

    QList<struct_GLOBAL_witness_info_root> list_struct_root_info;

    QFileDialog file_dialog_obj;

    void create_submenu_actions(QMenu *mainMenu, QStringList submenuList);

    void set_info_for_sources_widgets(QString evidence_type, QString root_type, QString source_type_display, QString source_type_internal, QString source_path, QString password, QString username);
    void update_root_count_index();

    QString M_str_QML_image_path;
    QString M_str_QML_source_path;
    QString M_str_QML_angular_brace;
    QString M_str_QML_left_big_brace;
    QString M_str_QML_right_big_brace;
    QString M_str_QML_browse_text;
    QString M_str_QML_add;
    QString M_str_QML_select_folder;
    QString M_str_QML_select_dmg_image;
    QString M_str_QML_select_sparseimage;
    QString M_str_QML_select_forensic_image;
    QString M_str_QML_is_filevaulted;
    QString M_str_QML_is_fusion;
    QString M_str_QML_type_password;
    QString M_str_QML_select_fusion_image;
    QString M_str_QML_select_evidence_type;
    QString M_str_QML_select_image_type;
    QString M_str_QML_select_backup_type;
    QString M_str_QML_select_operating_system;
    QString M_str_QML_select_ram_image;
    QString M_str_QML_select_optical_disc_image;
    QString M_str_QML_select_mounted_volumes;
    QString M_str_QML_select_time_machine_backup_folder;
    QString M_str_QML_select_time_machine_backup_image;
    QString M_str_QML_select_macos_home_directory;
    QString M_str_QML_select_l01_image;
    QString M_str_QML_select_file;
    QString M_str_QML_username;
    QString M_str_QML_type_username;
    QString M_str_QML_select_ios_backup;
    QString M_str_QML_select_ab_backup;
    QString M_str_QML_select_google_takeout;
    QString M_str_QML_select_android_backup_folder;

};

#endif // source_selector
