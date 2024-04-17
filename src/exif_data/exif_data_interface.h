#ifndef EXIF_DATA_INTERFACE_H
#define EXIF_DATA_INTERFACE_H

#include <QWidget>

#include "exif_data/exif_data_images/exif_data_image_files.h"
#include "exif_data/exif_data_audio_video/exif_data_audio_video.h"
#include "exif_data/exif_data_document/exif_data_document_files.h"

#include "recon_generic_functions/recon_static_functions.h"
#include "ui_custom_classes/m_treewidget.h"
#include "recon_generic_functions/recon_helper_standard.h"      

class exif_data_interface : public QWidget
{
    Q_OBJECT
public:
    explicit exif_data_interface(QWidget *parent = 0);
    ~exif_data_interface();

    QString pub_extract_and_save_exif_data(QString mime_type, QString file_path, qint64 fs_record_no, QSqlDatabase &exif_data_db, struct_global_exif_data_parsed_data &exif_data_parsed_obj, QString display_file_path);
    void pub_set_exif_data_in_case_tree_display(QString filepath, QString record_num, QString fs_db_path , m_treewidget *tree_exif_metadata_display, QString plugin_name, QString table_name, QString source_count_name,QString display_file_path);

    void pub_set_result_dir(QString result_dir);

signals:

public slots:

private slots:
    void slot_tree_metadata_exif_data_item_clicked(QTreeWidgetItem *item, int m_column);

private:



    QString result_dir_val;

    exif_data_image_files    *exif_data_image_obj;
    exif_data_audio_video    *exif_data_audio_video_obj;
    exif_data_document_files *exif_data_documents_obj;
    bool check_exif_parent_already_exists(QString category_parent, QTreeWidget *tree_widget_obj);

    recon_helper_standard *recon_helper_standard_obj;

    QStringList get_exif_metadata_key_value_for_file_system(QString record_no, QString source_count_name);


};

#endif // EXIF_DATA_INTERFACE_H
