#ifndef RECON_CONFIGURATION_H
#define RECON_CONFIGURATION_H

#include <QWidget>
#include <QtCore>
#include <QTreeWidgetItem>

#include "pages_recon_configuration/recon_configuration_page_examiner_details.h"
#include "pages_recon_configuration/file_signature/file_signature_widget.h"

#include  "launcher/case_launcher/pages/page_filesystem_n_modules.h"
#include "recon_generic_functions/recon_static_functions.h"

#include "launcher/plugin_selector/plugin_selector.h"

#include "message_box/message_dialog.h"
#include "header_identical_2.h"

#include "recon_configuration/pages_recon_configuration/recon_configuration_page_keyword_list.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_file_extension.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_mime_types.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_file_size.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_apple_metadata.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_index_files.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_volatility.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_system_password.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_external_applications.h"
#include "recon_configuration/pages_recon_configuration/import_hashset/recon_configuration_page_point_hashset.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_exif_metadata.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_global_timezone.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_preferences.h"
#include "recon_configuration/pages_app_configuration/app_configuration_page_preferences.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_filename_list.h"

namespace Ui {
class recon_configuration;
}

class recon_configuration : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration(QWidget *parent = 0);
    ~recon_configuration();

    void pub_set_essentials();
    void pub_initialise_configuration();

    void pub_set_caller_type(QString type);

private slots:
    void on_treeWidget_page_list_itemClicked(QTreeWidgetItem *item, int column);
    void on_pushButton_back_clicked();

    void on_pushButton_apply_clicked();

signals:
    void signal_back_button_clicked();

    void signal_recon_config_apply_button_clicked();
private:
    Ui::recon_configuration *ui;

    enum enum_page_order{
        enum_recon_config_page_examiner_info = 0,
        enum_recon_config_page_plugin_selection,
        enum_recon_config_page_file_extension,
        enum_recon_config_page_file_signature,
        // enum_recon_config_page_hash_set,
        enum_recon_config_page_content_search,
        enum_recon_config_page_filename_search,
        enum_recon_config_page_index_files,
        enum_recon_config_page_apple_metadata,
        enum_recon_config_page_exif_metadata,
        enum_recon_config_page_volatility_path,
        enum_recon_config_page_system_password,
        enum_recon_config_page_external_applications,
        enum_recon_config_page_global_time_zone,
        enum_recon_config_page_preferences,
        enum_app_config_page_preferences
    };


    QStringList list_stack_pages;

    void set_config_tree_widget_display();
    message_dialog *message_dialog_object;

    plugin_selector *plugin_selector_obj;
    file_signature_widget *file_signature_widget_obj;
    page_filesystem_n_modules *page_hashset_obj;
    recon_configuration_page_examiner_details *page_examiner_details_obj;
    recon_configuration_page_keyword_list *keyword_list_page_obj;
    recon_configuration_page_file_extension *file_extension_page_obj;
    recon_configuration_page_apple_metadata *apple_metadata_page_obj;
    recon_configuration_page_index_files *index_files_page_obj;
    recon_configuration_page_volatility *volatility_page_obj;
    recon_configuration_page_system_password *system_pwd_page_obj;
    recon_configuration_page_external_applications *external_applications_page_obj;
    //recon_configuration_page_point_hashset *recon_configuration_page_point_hashset_obj;
    recon_configuration_page_exif_metadata *exif_metadata_page_obj;
    recon_configuration_page_global_timezone *recon_configuration_page_global_timezone_obj;
    recon_configuration_page_filename_list *filename_list_page_obj;
    QString caller_type;
    recon_configuration_page_preferences *recon_configuration_page_preferences_obj;

    app_configuration_page_preferences *app_configuration_page_preferences_obj;

};

#endif // RECON_CONFIGURATION_H
