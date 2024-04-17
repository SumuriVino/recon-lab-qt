#ifndef DETACH_WINDOW_MANAGER_H
#define DETACH_WINDOW_MANAGER_H

#include "detach_detailed_info.h"
#include "detach_detailed_info.h"
#include "ui_custom_classes/m_textedit.h"
#include "tsk/libtsk.h"
#include "previewer/av_preview.h"
#include "previewer/blank.h"
#include "previewer/image_preview.h"
#include "ui_custom_classes/m_stackedwidget.h"
#include "ui_custom_classes/r_tabwidget.h"
#include "header_global_variables_1.h"
#include "ui_custom_classes/m_treewidget.h"
#include "exif_data/exif_data_interface.h"
#include "header_global_variables_2.h"

#include "recon_generic_functions/recon_helper_standard.h"      

class detach_window_manager : public QWidget
{
    Q_OBJECT

public:
    detach_window_manager(QWidget *parent = 0);
    ~detach_window_manager();

    void pub_set_essentials();
    void pub_set_hex_viewer_block_db_path(QString path);
    void pub_open_in_detach_clicked(QString metadata_value, QString hex_file_path, QString preview_path, int preview_index, QString searched_keyword, QString source_count_name);
    void pub_detach_content_change(QString metadata_value, QString hex_file_path, QString preview_path, int preview_index, QString searched_keyword, QString source_count_name);

   void pub_set_plugin_name_table_record_no(QString plugin, QString table, QString record_no);


private slots:
    void slot_close_windows_index(detach_detailed_information *closed_window_addr);
    void slot_tree_apple_metadata_item_clicked(QTreeWidgetItem *item, int column);


private:

    enum enum_detach_window_manager_apple_metadata{
        enum_detach_window_manager_apple_metadata_KEY = 0,
        enum_detach_window_manager_apple_metadata_Value,
        enum_detach_window_manager_apple_metadata_Plugin_Table_Name,
        enum_detach_window_manager_apple_metadata_Plugin_Db_Path,
        enum_detach_window_manager_apple_metadata_Plugin_Record_no,
        enum_detach_window_manager_apple_metadata_File_Path
    };


    struct struct_detach_window_object_info
    {
        detach_detailed_information *detach_window_obj;

        QSplitter *splitter_metadata_tabs_detach;
        m_textedit *text_edit_metadata_detach;
        image_preview *image_preview_obj_detach;
        blank *blank_obj_detach;
        av_preview *av_preview_obj_detach;
        m_stackedwidget *stackwidget_previewer_obj_detach;
        R_Tabwidget *tabwidget_metadata_viewer_detach;
        R_Tabwidget *tabwidget_av_and_images_previewer_detach;
        QHBoxLayout *hlayout_detach;
        m_treewidget *tree_exif_metadata;
        m_treewidget *tree_apple_metadata;

    };

    QList<struct_detach_window_object_info> list_detach_window_list;

    QString result_dir_val;
    QString hex_viewer_block_db_path;

    exif_data_interface *exif_interface_obj;
    void set_exif_data_in_detach_window(m_treewidget *tree_exif_metadata_display, QString source_count_name, QString filepath, QString plugin_name, QString table_name);


    void change_apple_attributes_content_on_row_click(QStringList file_path_list, QString source_count_name, QString record_no, QString table_name, QString plugin_name, m_treewidget *tree_metadata_apple_metadata);
    QStringList get_apple_metadata_key_value_list_from_fs_db(QString file_path, QString source_count_name);
    QString plugin_name,  table_name,  record_number;

    recon_helper_standard *recon_helper_standard_obj;

};

#endif
