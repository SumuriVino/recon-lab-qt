#ifndef STORY_BOARD_H
#define STORY_BOARD_H

#include <QFrame>
#include <QtCore>
#include "story_board_textedit.h"
                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "story_board_loader.h"
#include "full_detailed_information/full_detailed_info.h"
#include "report/report_essentials/report_template_html_standard.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "export/export_except_reports/export_rsync/export_rsync.h"


namespace Ui {
class story_board;
}

class story_board : public QFrame
{
    Q_OBJECT

public:
    explicit story_board(QWidget *parent = 0);
    ~story_board();

    void pub_set_initialisation();
    void pub_set_story_board_file_name_and_path(QString file_name, QString file_path);

    void pub_set_previous_saved_data_in_textedit(QString filepath);
    void pub_update_combobox_and_tablewidget_display();
    void pub_add_and_save_case_details_in_textedit(QString filepath);


signals :
    void signal_tags_combobox_index_changed();
    void signal_storyboard_tablewidget_goto_record(QString plugin_name, QString parent_name, QString tab_name ,QString record_no, QString db_path, QString os_name_str);
    void signal_act_quick_look_triggered();
    void signal_quick_look_preview_filepath_from_table_result_loader(QString path);
    void signal_checkbox_blurr_status(bool status);
    void signal_tablewidget_goto_record_snapshots(QString ,QString , QString , QString);

    void signal_checkBox_censor_skintone_tag_images_clicked(bool status);


private slots:
    void on_pushButton_show_hide_tag_table_clicked(bool checked);

    void on_comboBox_tags_currentTextChanged(const QString &arg1);

    void slot_add_record_to_story_board(QString metadata,QString media_path);//

    void on_lineEdit_search_returnPressed();

    void on_pushButton_search_clicked();

    void on_pushButton_show_all_clicked();

    void slot_add_file_to_story_board(QString actual_filename, QString filepath , bool blurr_status, QString label_file_path);//
    void slot_show_all_clicked();

    void slot_save_data();

    void slot_refresh_storyboard_display();

    void on_checkBox_blurr_image_clicked(bool checked);

    void slot_pushbutton_open_clicked();
    void slot_pushbutton_close_clicked();

    void slot_on_cancel_from_progress_bar();
    void on_checkBox_censor_skintone_tag_images_clicked(bool checked);

private:
    Ui::story_board *ui;
    story_board_textedit *story_board_textedit_obj;

    void display_tags_combobox();
    void display_tags_table_data();

    bool bool_combobox_filling;

    story_board_loader *story_board_loader_obj;
    full_detail_information *full_detail_information_obj;

    QVBoxLayout *vbox_layout_metadata_preview;
    QPushButton *pushbutton_show_hide_meatadata;

    QString story_file_name;
    QString story_file_path;

    QDialog *dialog_export;
    QString current_exported_file_path;
    void prepare_export_dialog();
    recon_helper_standard *recon_helper_standard_obj;

    display_loading_progress_bar * progress_loading_obj;
    QString remove_ios_dir_adhoc_path;
    export_rsync *export_rsync_obj;


};

#endif // STORY_BOARD_H
