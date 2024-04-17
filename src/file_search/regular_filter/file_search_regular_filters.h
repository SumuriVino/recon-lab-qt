#ifndef FILE_SEARCH_REGULAR_FILTERS_H
#define FILE_SEARCH_REGULAR_FILTERS_H

#include <QDialog>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_1.h"
#include <QMessageBox>
#include "header_global_variables_2.h"
#include "source_interface/select_source_interface/select_source_interface.h"
#include "recon_generic_functions/recon_helper_standard.h"

namespace Ui {
class file_search_regular_filters;
}

class file_search_regular_filters : public QDialog
{
    Q_OBJECT

public:
    explicit file_search_regular_filters(QWidget *parent = 0);
    ~file_search_regular_filters();

    void pub_set_sources_in_ui();

    void pub_load_defaults();
    void closeEvent(QCloseEvent *);

signals:
    void signal_file_search_regular_filter_work_done(QString command, QStringList sources, QString search_label);

private slots:
    void on_pushButton_search_clicked();

    void on_checkBox_date_change_clicked(bool checked);

    void on_comboBox_date_change_options_currentTextChanged(const QString &arg1);

    void on_checkBox_date_modified_clicked(bool checked);

    void on_comboBox_date_modified_options_currentTextChanged(const QString &arg1);

    void on_checkBox_date_accessed_clicked(bool checked);

    void on_comboBox_date_accessed_options_currentTextChanged(const QString &arg1);

    void on_checkBox_date_created_clicked(bool checked);

    void on_comboBox_date_created_options_currentTextChanged(const QString &arg1);

    void on_checkBox_date_added_clicked(bool checked);

    void on_comboBox_date_added_options_currentTextChanged(const QString &arg1);

    void on_checkBox_content_creation_clicked(bool checked);

    void on_comboBox_content_creation_options_currentTextChanged(const QString &arg1);


    void on_comboBox_content_modification_options_currentTextChanged(const QString &arg1);

    void on_checkBox_last_used_clicked(bool checked);

    void on_comboBox_last_used_options_currentTextChanged(const QString &arg1);

    void on_checkBox_content_modification_clicked(bool checked);

    void on_comboBox_file_size_options_currentTextChanged(const QString &arg1);

    void on_checkBox_file_size_clicked(bool checked);

    void on_radioButton_all_source_toggled(bool checked);
    void on_radioButton_select_source_toggled(bool checked);
    void on_pushButton_select_source_list_clicked();
    void slot_selected_source_list(QStringList selected_src_list);

    void on_checkBox_ocr_text_clicked(bool checked);

    void on_checkBox_file_name_clicked(bool checked);

    void slot_action_select_filename_category_triggered(bool checked);

private:
    Ui::file_search_regular_filters *ui;

    QString file_name_sql_predicate();
    QString file_size_sql_predicate();
    QString date_change_sql_predicate();
    QString date_modified_sql_predicate();
    QString date_accessed_sql_predicate();
    QString date_created_sql_predicate();
    QString date_content_creation_sql_predicate();
    QString date_content_modification_sql_predicate();
    QString date_added_sql_predicate();
    QString date_last_used_sql_predicate();
    QString ocr_text_sql_predicate();

    int current_machine_offset ;
    int current_case_offset ;

    qint64 round_off_offset;

    QList<QCheckBox *> source_checkbox_list;

    select_source_interface *select_source_interface_obj;
    QStringList selected_source_cnt_nm_list;
    QStringList prevoius_selected_source_cnt_nm_list;

    recon_helper_standard *recon_helper_standard_obj;
    QMenu       *menu_select_category;
    QString existing_text_in_line_edit;
    void add_category_in_pushbutton_file_name_select_category();
};

#endif // FILE_SEARCH_REGULAR_FILTERS_H
