#ifndef PAGE_FILESYSYEM_N_MODULES_H
#define PAGE_FILESYSYEM_N_MODULES_H

#include <QWidget>
#include <QtSql>
#include<QCheckBox>
#include <QVBoxLayout>
#include "recon_generic_functions/recon_static_functions.h"

#include "ui_custom_classes/m_tablewidget.h"
                                   
#include "hashset/hashset_storage.h"

#include "message_box/message_dialog.h"

#include "recon_configuration/pages_recon_configuration/file_signature/file_signature_widget.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_index_files.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_apple_metadata.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_exif_metadata.h"
#include "recon_configuration/pages_recon_configuration/import_hashset/recon_configuration_page_point_hashset.h"


namespace Ui {
class page_filesystem_n_modules;
}

class page_filesystem_n_modules : public QWidget
{
    Q_OBJECT

public:
    explicit page_filesystem_n_modules(QWidget *parent = 0);
    ~page_filesystem_n_modules();

    void extract_launcher_values();
    void pub_set_essentials_for_case_launcher();
    void pub_create_ui();



public slots:
    void slot_checkbox_analyse_hashes_clicked(bool checked);


private slots:


    void on_pushButton_hashes_clicked(bool checked);

    void on_pushButton_exif_metadata_clicked(bool checked);
    void slot_checkbox_exif_metadata_clicked(bool checked);
    void slot_exif_data_tablewidget_cellClicked(int row, int column);

    void slot_checkbox_apple_metadata_clicked(bool checked);
    void on_pushButton_apple_metadata_clicked(bool checked);

    void on_pushButton_signature_analysis_clicked(bool checked);

    void slot_checkbox_analyse_signature_clicked(bool checked);

    void on_pushButton_face_analysis_clicked(bool checked);
    void slot_checkbox_extract_face_clicked(bool checked);
    void slot_checkbox_optical_character_recognition_clicked(bool checked);
    void on_pushButton_optical_character_recognition_clicked(bool checked);
    void on_pushButton_skin_tone_detection_clicked(bool checked);
    void slot_checkbox_skin_tone_detection_clicked(bool checked);
    void on_pushButton_weapons_clicked(bool checked);
    void slot_checkbox_weapons_clicked(bool checked);

    void on_pushButton_fire_analysis_clicked(bool checked);
    void slot_checkbox_fire_analysis_clicked(bool checked);

    void on_pushButton_unified_logs_clicked(bool checked);
    void slot_checkbox_unified_logs_clicked(bool checked);


signals:

private:
    Ui::page_filesystem_n_modules *ui;

    enum enum_fs_options_page
    {
        enum_stack_page_apple_metatdata = 0,
        enum_stack_page_signature_analysis,
        enum_stack_page_exif_metadata,
        enum_stack_page_hashset,
        enum_stack_page_unified_logs,
        enum_stack_page_face_analysis,
        enum_stack_page_optical_character_recognition,
        enum_stack_page_skin_tone_detection,
        enum_stack_page_weapons,
        enum_stack_page_fire_analysis
    };

    enum enum_for_gui
    {
        enum_column_bookmark = 0,
        enum_column_category
    };

    QCheckBox *checkbox_analyse_hashes;

    QWidget *hashset_widget;

    void create_hashset_ui();


    void create_exif_metadata_ui();
    QWidget *exif_metadata_widget;
    QCheckBox *checkbox_exif_metadata_obj;
    m_tablewidget *m_tablewidget_exif_data_obj;
    QLabel *label_exif_data;

    QCheckBox *checkbox_apple_metadata_obj;
    QCheckBox *checkbox_mime_type_obj;
    QWidget *apple_metadata_widget;
    QWidget *mime_type_widget;

    QWidget   *signature_analysis_widget;
    QCheckBox *checkbox_signature_analysis_obj;

    file_signature_widget *signature_widget_obj;

    void create_apple_metadata_ui();
    void create_signature_analysis_ui();

    recon_configuration_page_apple_metadata *recon_configuration_page_apple_metadata_obj;
   // recon_configuration_page_point_hashset *recon_configuration_page_point_hashset_obj;
    recon_configuration_page_exif_metadata *recon_configuration_page_exif_metadata_obj;

    QWidget *face_analysis_widget;
    QCheckBox *checkbox_extract_face_obj;
    void create_face_analysis_ui();

    QCheckBox *checkbox_optical_character_recognition_obj;
    QWidget *optical_character_recognition_widget;
    void create_optical_character_recognition_ui();

    QWidget *skin_tone_detection_widget;
    QCheckBox *checkbox_skin_tone_detection_obj;
    void create_skin_tone_detection_ui();
    QCheckBox *checkbox_weapons_obj;
    QWidget *weapons_widget;
    void create_weapons_ui();

    QCheckBox *checkbox_fire_analysis_obj;
    QWidget *fire_analysis_widget;
    void create_fire_analysis_ui();

    QWidget *unified_logs_widget;
    QCheckBox *checkbox_unified_logs_obj;
    void create_unified_logs_ui();

};

#endif //
