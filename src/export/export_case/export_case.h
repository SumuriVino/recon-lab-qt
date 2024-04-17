#ifndef EXPORT_CASE_H
#define EXPORT_CASE_H

#include <QWidget>
#include <QCoreApplication>
#include <QtCore>
#include "narad_muni/narad_muni.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "QFileDialog"
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQuery>
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "csv_reader/csv_reader.h"
#include "header_global_structures.h"
#include <QCheckBox>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QMessageBox>
#include "export/export_except_reports/export_rsync/export_rsync.h"

#define MACRO_LOCAL_EXPORT_CASE_LABEL_CATEGORY          "Category"
#define MACRO_LOCAL_EXPORT_CASE_LABEL_TAGS              "Tags"
#define MACRO_LOCAL_EXPORT_CASE_LABEL_LIST              "List"
#define MACRO_LOCAL_EXPORT_CASE_DIR_FILE_SYSTEM         "File_System"
#define MACRO_LOCAL_EXPORT_CASE_DIR_CASE_DATA           "Case_Data"
#define MACRO_LOCAL_EXPORT_CASE_DIR_TAG_SEARCH        "Tag_Search"
#define MACRO_LOCAL_EXPORT_CASE_DIR_BOOKMARKS_SEARCH         "Bookmarks_Search"


namespace Ui {
class export_case;
}

class export_case : public QDialog
{
    Q_OBJECT

public:
    explicit export_case(QWidget *parent = nullptr);
    ~export_case();

    void pub_set_display_loading_progress_bar(display_loading_progress_bar *obj);
    void pub_prepare_display();

signals:
    void signal_checkbox_blur_status(bool status);

private slots:

    void on_pushButton_select_output_clicked();
    void on_pushButton_export_clicked();

    void slot_proc_copy_finished(int);

    void on_tableWidget_quick_mode_category_cellClicked(int row, int column);
    void on_tableWidget_quick_mode_category_cellChanged(int row, int column);
    void on_tableWidget_quick_mode_list_cellClicked(int row, int column);

    void on_tableWidget_expert_mode_tag_list_cellChanged(int row, int column);

    void on_radioButton_quick_mode_toggled(bool checked);
    void on_checkBox_blur_status_clicked(bool checked);
    void slot_process_blur_image_finished(int status);

private:
    Ui::export_case *ui;

    void prepare_quick_mode_display();

    void prepare_internet_history_display_list();
    void prepare_messages_display_list();
    void prepare_emails_display_list();
    void prepare_images_display_list();
    void prepare_multimedia_display_list();
    void prepare_documents_display_list();
    void prepare_other_plugins_display_list();
    void prepare_expert_mode_display();
    void prepare_tags_display(QStringList tag_list ,QStringList check_state_list);

    void update_export_status_db_based_upon_quick_mode_category(QString str, int value);
    void update_export_status_db_based_upon_expert_mode_category_and_tag_str(QString category_str, QString tag_str, int value);

    struct struct_export_case_artifacts_info
    {
        QString record_no;
        QString plugin_name;
        QString table_name;
        QString source_count_name;
    };

    QList<struct_export_case_artifacts_info> struct_export_case_artifacts_info_QList;

    QList<struct_global_export_file_system_record> struct_file_system_export_tagged_files_info_QList;

    recon_helper_standard *recon_helper_standard_obj;

    display_loading_progress_bar *display_loading_progress_bar_obj;

    bool bool_is_copy_finished = false;

    export_rsync *export_rsync_obj;

    enum enum_tablewidget_quick_mode_category_items_column_index
    {
        enum_tablewidget_quick_mode_category_items_column_zero_category = 0,
    };

    enum enum_tablewidget_quick_mode_list_items_column_index
    {
        enum_tablewidget_quick_mode_list_items_column_zero_list = 0,
    };

    bool bool_checkbox_blur_status = false;
    void export_case_with_blur_effect_on_image(QString filepath, QString dest_filepath, QString display_f_path);
    bool bool_proc_blur_img_finished = false;
};

#endif // EXPORT_CASE_H
