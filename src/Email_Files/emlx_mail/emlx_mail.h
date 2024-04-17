#ifndef EMLX_MAIL_H
#define EMLX_MAIL_H

#include <QFrame>
#include <QFileDialog>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>


#include "recon_generic_functions/recon_static_functions.h"
#include "header_gui_settings.h"
#include "add_tags/add_tags.h"
#include "header_global_variables_1.h"
#include "previewer/av_preview.h"
#include "previewer/image_preview.h"
#include "previewer/blank.h"
#include "header_global_variables_3.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "header_global_variables_2.h"
#include "Email_Files/email_search/email_search.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "global_connection_manager.h"

namespace Ui {
class emlx_mail;
}

class emlx_mail : public QFrame
{
    Q_OBJECT

public:
    explicit emlx_mail(QWidget *parent = 0);
    ~emlx_mail();

    void pub_set_essentials();
    void pub_display_data_in_table();

    void pub_set_global_connection_manager_obj(global_connection_manager *obj);
private slots:

    void on_pushButton_search_clicked();
    void on_pushButton_show_all_clicked();
    void on_treeWidget_accounts_itemClicked(QTreeWidgetItem *item, int column);
    void on_tableWidget_emails_cellPressed(int row, int column);
    void slot_file_system_menu_right_clicked(QAction *current_clicked_action);
    void action_remove_bookmarks_triggered();
    void action_add_note_triggered();
    void action_add_note_to_bookmark_triggered();
    void action_remove_note_triggered();
    void action_remove_note_bookmark_triggered();
    void action_remove_tag_triggered();
    void slot_tags_submenu_clicked(QAction *current_clicked_action);
    void slot_act_create_tags();
    void slot_add_tags(QString tag_data, QString colour);

    void slot_quicklook_link_clicked(QString link_text);
    void slot_open_in_finder_link_clicked(QString link_text);
    void slot_tablewidget_goto_record(QString record);

    void on_lineEdit_message_search_returnPressed();

    void on_pushButton_search_message_clicked();

    void on_tableWidget_emails_itemSelectionChanged();

    void on_tableWidget_emails_itemClicked(QTableWidgetItem *item);

    void on_lineEdit_message_search_textChanged(const QString &arg1);
    void on_lineEdit_raw_data_search_textChanged(const QString &arg1);

    void slot_update_tags_submenu(bool status, QString new_tag, QString tag_colour);
    void on_lineEdit_raw_data_search_returnPressed();

    void on_pushButton_raw_data_search_clicked();

    void on_tableWidget_attachments_itemSelectionChanged();

    void slot_sort_tablewidget_display(int column_index);
    void slot_abouttoshow_tags_in_submenu();

    void slot_cancel_loading_search_results(bool status);
    void slot_email_search_work_done(QString command);
    void slot_highlight_searched_text(QString emlx_searched_text);

    void on_pushButton_advanced_search_clicked();
    void slot_textBrowser_raw_data_customContextMenuRequested(const QPoint &pos);
    void slot_right_click_menu_raw_data_triggered(QAction *current_clicked_action);
    void slot_sub_menu_raw_data_tags_clicked(QAction *action);

signals:
    void signal_common_add_tag_name_into_case_tree(QString tag_name);
    void signal_add_tags(QString tag_name,QString colour_name);

    void signal_quick_look_preview_filepath_from_email_files(QString filepath);
    void signal_act_quick_look_triggered();
    void signal_go_to_source_in_file_system_from_emlx(QString filepath, QString fs_db_path, QString source_count_name);

private:
    Ui::emlx_mail *ui;

    enum enum_emlx
    {
        enum_emlx_sr_no_column = 0,
        enum_emlx_bookmark_column,
        enum_emlx_tag_column,
        enum_emlx_to_column,
        enum_emlx_from_column,
        enum_emlx_Cc_column,
        enum_emlx_subject_column,
        enum_emlx_datetime_column,
        enum_emlx_attachments_column,
        enum_emlx_account_id_column,
        enum_emlx_mbox_column,
        enum_emlx_file_name_column,
        enum_emlx_source_file_column,
        enum_emlx_source_count_name_column
    };

    enum enum_av_preview
    {
        enum_no_preview_index = 2,
        enum_audio_video_preview_index,
        enum_image_preview_index
    };

    QString source_db_path ;
    QString attachment_dir_path;
    QString current_record_sr_str;

    QMenu   *menu_search_filters;

    QTreeWidgetItem *parent_item;      ///for parent account id
    QTreeWidgetItem *parent_mbox_item; ///for parent mbox


    QMenu *mainMenu;

    QStringList display_header_name_list;
    QMenu *sub_menu_tags;

    av_preview *av_preview_obj;
    image_preview *image_preview_obj;
    blank *blank_obj;
    QMimeDatabase mime_database_obj;

    void emlx_show_data_table_widget();
    void emlx_show_data_text_edit();
    bool check_item_already_exist_in_complete_tree(QString item_text, bool parent_object);
    bool check_item_already_exist_in_specified_tree(QTreeWidgetItem *item_tree, QString item_text, bool parent_object);
    QStringList get_index_of_mbox_from_path(QStringList filepath_list);
    qint64 get_header_column_index(QString header);
    void emlx_show_data_in_raw_data_text_edit();
    void create_right_click_actions(QMenu *mainMenu, QStringList submenuList);
    bool eventFilter(QObject *obj, QEvent *event);
    void action_bookmark_triggered();

    void create_tags_submenu();
    void action_submenu_tag_triggered(QString tag_name);
    QAction *act_create_tag;
    add_tags *add_tags_module_obj;
    QAction *act_remove_tag;
    QAction *act_already_saved_tags;
    QMenu *create_right_click_submenu(QString submenuStr, QMenu *mainMenu, QMenu *subMenu);
    void update_tags_in_tag_serach(QStringList record_no_list, QString tag_data);
    void update_bookmarks_in_bookmark_serach(QStringList record_no_list, QStringList tag_name_list);
    QString case_confg_db_path;
    void delete_tags_from_tag_serach_other_than_current_tag_and_for_current_record(QString current_tag, QString record_no, QString plugin_name, QString tab_name);

    void emlx_preview_attachment_file();

    recon_helper_standard *recon_helper_standard_obj;

    bool bool_sort_ascending;
    QList<QTableWidgetItem *> takeRow(int row);
    void setRow(int row, const QList<QTableWidgetItem *> &rowItems);
    email_search *email_search_obj;
    display_loading_progress_bar *display_loading_progress_bar_obj;
    bool bool_cancel_loading_search_results;

    QString highlight_searched_text_in_result;add_tags *add_raw_data_tag_module_obj;

    QMenu   *menu_raw_data;
    QMenu   *sub_menu_raw_data_tags;
    QAction *act_raw_data_select_all;
    QAction *act_raw_data_create_tag;

    global_connection_manager *global_connection_manager_obj;

    void create_raw_data_tags_submenu();
    void action_submenu_raw_data_tag_triggered(QString tag_name);
    void update_raw_data_tags_value(QString tag_data, QString colour);
    void create_table_export_content_for_raw_data_tag();

    QMenu *submenu_bookmark;

    struct struct_mail_pst_and_mbox_file_info {
        QString record_int;
        QString source_count_name;
        QString folder_name;
        QByteArray file_data_bytes;
    };

    QList <struct_mail_pst_and_mbox_file_info> QList_struct_mail_pst_and_mbox_file_info_details;
};

#endif // EMLX_MAIL_H
