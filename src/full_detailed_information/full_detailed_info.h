#ifndef Full_Detail_Information_H
#define Full_Detail_Information_H

#include <QDialog>
#include "ui_custom_classes/m_textedit.h"
#include "ui_custom_classes/m_stackedwidget.h"
#include "ui_custom_classes/r_tabwidget.h"

#include "previewer/image_preview.h"
#include "previewer/blank.h"
#include "previewer/av_preview.h"
#include "tsk/libtsk.h"

#include <QSplitter>
#include "header_global_variables_1.h"
#include "ui_custom_classes/m_treewidget.h"
#include "exif_data/exif_data_interface.h"
                                   
#include "header_global_variables_2.h"

#include "file_viewer/strings_viewer/strings_viewer.h"
#include "file_viewer/text_viewer/text_viewer_aascii_unicode.h"
#include "maps/offline_maps/offline_maps.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "file_viewer/hex_viewer/hex_viewer_complete/hex_viewer_complete.h"
#include "recon_case/recon_case.h"
#include "header_global_variables_4.h"


namespace Ui {
class full_detail_information;
}

class full_detail_information : public QWidget
{
    Q_OBJECT

public:
    explicit full_detail_information(QWidget *parent = 0);
    ~full_detail_information();

    void pub_set_essentials();
    void pub_set_hex_viewer_block_db_path(QString db_path);
    void pub_show_full_detail_info(QString metadata_value, QString preview_path, int preview_index, QString searched_keyword, QString source_count_name, QString record_number, QString plugin_name, QString table_name);

    void pub_set_global_connaction_manager_obj(global_connection_manager *obj);
    void pub_set_recon_file_info(struct_global_recon_file_info obj);
    void pub_set_metadata_values_obj(struct_global_set_metadata_info obj);
    void pub_set_recon_case_obj(RECON_case *obj);
signals:
    void signal_add_saved_maps_in_case_tree();
    void signal_save_openwith_changes_to_bucket_full_detail_info(QString viewer_type);
    void signal_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked(QString record_no_str, QString bookmark_status);
    void signal_detail_information_triggered();



private slots:
    void slot_tree_apple_metadata_item_clicked(QTreeWidgetItem *item, int column);

    void slot_tree_exif_metadata_item_clicked(QTreeWidgetItem *item, int m_column);

    void slot_tabwidget_metadata_viewer_full_current_Changed(int);

private:
    Ui::full_detail_information *ui;
    void closeEvent(QCloseEvent *);


    enum enum_full_detail_information_apple_metadata{
        enum_full_detail_information_apple_metadata_KEY = 0,
        enum_full_detail_information_apple_metadata_Value,
        enum_full_detail_information_apple_metadata_Plugin_Table_Name,
        enum_full_detail_information_apple_metadata_Plugin_Db_Path,
        enum_full_detail_information_apple_metadata_Plugin_Record_no,
        enum_full_detail_information_apple_metadata_File_Path
    };

    QString result_dir;
    QString hex_viewer_block_db_path;

    m_treewidget *tree_exif_metadata_display;
    exif_data_interface *exif_interface_obj;

    void set_exif_data_in_full_window(m_treewidget *tree_exif_metadata_display, QString source_count_name, QString filepath, QString plugin_name, QString table_name, QString record_no_str, QString display_file_path);
    void change_apple_metadata_content_on_row_click(QStringList file_path_list, QString source_count_name, QString record_no, QString table_name, QString plugin_name, m_treewidget *tree_metadata_apple_metadata);
    QStringList get_apple_metadata_key_value_list_from_fs_db(QString file_path, QString source_count_name);

    strings_viewer *strings_viewer_obj;
    text_viewer_aascii_unicode *text_viewer_aascii_unicode_obj;
    global_connection_manager *global_connection_manager_obj;
    offline_maps *maps_metadata_obj;
    QString create_link_path_for_preview(QString extension_str, QString file_path);

    struct_global_recon_file_info st_globl_recon_file_info_obj;
    recon_helper_standard *recon_helper_standard_obj;
    struct_global_set_metadata_info st_metadata_values_obj;

    m_treewidget *tree_apple_metadata;


    bool check_any_exif_or_apple_metadata_item_selected();
    RECON_case *recon_case_obj;
    void extract_apple_metadata_from_apple_metadata_reader(QString file_paths_str, QString source_cnt_name, QString virtual_source_path);

    m_textedit *text_edit_optical_character_recognition_metadata_obj;
    R_Tabwidget *tabwidget_metadata_viewer_full;
};

#endif // fulldetail_H
