#ifndef RAM_ANALYSIS_H
#define RAM_ANALYSIS_H

#include <QFrame>
                                   
#include "header_global_variables_1.h"
#include <QMessageBox>
#include "message_box/message_dialog.h"
#include "save_result_dialog_box/save_result_dialog_box.h"
#include "export_dialog_box/export_dialog_box_1/export_dialog_box_1.h"
#include "report/report_essentials/report_status.h"

#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"


namespace Ui {
class ram_analysis;
}

class ram_analysis : public QFrame
{
    Q_OBJECT

public:
    explicit ram_analysis(QWidget *parent = 0);
    ~ram_analysis();

    void pub_set_essentials();
    void pub_set_fill_sources();
    void pub_set_version_international();
private slots:
    void on_pushButton_execute_command_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_export_clicked();

    void on_comboBox_operating_system_currentTextChanged(const QString &arg1);

    void slot_process_ram_analyzer_finished(int);
    void on_pushButton_password_analyser_clicked();
    void slot_ram_analysis_saved_result_in_db_by_saved_name(QString saved_name);
    void slot_ram_analysis_saved_result_in_file_by_saved_name(QString given_name, QString selected_dir_path, QString export_type_str);

    void slot_ram_analysis_cancel_export_results();
    void slot_ram_analysis_cancel_save_results();
    void on_pushButton_refresh_source_clicked();

    void slot_hide_loading_display_dialog_box(bool status);
signals:

    void signal_ram_analysis_saved_add_in_case_tree(QString plugin_name);
    void signal_ram_analysis_carved_password_add_in_case_tree(QString tab_name);

private:
    Ui::ram_analysis *ui;

    struct struct_password_info
    {
        QString m_password;
        QString m_server;
        QString m_account;
        QString m_description;
        QString m_source_count_name;
        QString m_os_scheme_display;
    };



    QStringList mac_profiles_list;
    QStringList windows_profiles_list;
    // QStringList mac_command_list;

    QProcess *process_ram_analyzer;


    QString selected_ram_image_path;


    void fill_sources_in_source_combobox();
    void enable_disable_ui_on_execute_click(bool status);

    message_dialog *message_dialog_object;
    void saved_ram_analaysis_in_db_on_save_button_clicked(QString saved_name);
    void saved_ram_analaysis_in_file_on_export_button_clicked(QString saved_name);
    save_result_dialog_box *save_result_dialog_box_obj;
    export_dialog_box_1 *export_dialog_box_1_obj;

    QString destination_db_file;
    QStringList get_profile_list(QString os_type);
    QMap <QString, QString> get_plugin_map(QString os_type);
    QMap<QString , QString > map_mac_plugin;
    void create_carved_password_dbs(QString source_count_name_str, QString source_name_str);
    QString carved_password_db_path;
    QString carved_password_result_name;


    void password_first_method(QString file_path, struct_GLOBAL_witness_info_source src_obj);

    void password_third_method(QString file_path, struct_GLOBAL_witness_info_source src_obj);

    void password_second_method(QString file_path, struct_GLOBAL_witness_info_source src_obj);

    QByteArray sub_array(QByteArray ar, int length, int offset);

    bool check_for_sub_array(QByteArray source, QByteArray to_check, int offset);

    bool check_if_array_contains_non_readable_characters(QByteArray);

    void insert_password_info_in_db(struct_password_info m_obj);

    int password_count;

    display_loading_progress_bar *display_loading_progress_bar_obj;
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;
    bool bool_cancel_loading ;

    recon_helper_standard *recon_helper_standard_obj;

    void open_exported_file(QString file_path);
};

#endif // RAM_ANALYSIS_H
