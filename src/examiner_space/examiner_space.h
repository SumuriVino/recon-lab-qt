#ifndef EXAMINER_SPACE_H
#define EXAMINER_SPACE_H

#include <QtGui>
#include <QColorDialog>
#include "QtCore"
#include "QPushButton"
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>

#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"

#include <QPrinter>
#include <QFileDialog>
#include "message_box/message_dialog.h"
#include "report/pdf_report_module.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "header_global_variables_4.h"

namespace Ui {
class examiner_space;
}

class examiner_space : public QWidget
{
    Q_OBJECT

public:
    explicit examiner_space(QWidget *parent = nullptr);
    ~examiner_space();

    void closeEvent(QCloseEvent *);

    void pub_set_essential();
    void pub_prepare_display();


signals:
    void signal_yes_no_check(bool value);
    void signal_create_pdf_wkhtmltopdf(QString source_html_file, QString dest_pdf_file);

private slots:

    void slot_textedit_examiner_notes_textchanged();
    void slot_pushbutton_examiner_notes_bold_clicked();
    void slot_pushbutton_examiner_notes_italic_clicked();
    void slot_pushbutton_examiner_notes_underline_clicked();
    void slot_pushbutton_examiner_notes_color_clicked();

    void slot_action_report_html_triggered();
    void slot_action_report_pdf_triggered();
    void slot_message_box_yes_no_clicked(bool yes_no_clicked);
    void slot_html_to_pdf_finished(QString pdf_file_path, bool stauts);

    void on_tableWidget_to_do_list_itemClicked(QTableWidgetItem *item);
    void on_pushButton_to_do_list_add_task_clicked();
    void slot_line_edit_task_add_returnpressed();
    void on_pushButton_to_do_list_remove_task_clicked();
    void on_pushButton_to_do_list_edit_task_clicked();
    void slot_line_edit_task_edit_returnpressed();
    void on_tabWidget_examiner_space_currentChanged(int index);
    void on_pushButton_add_timestamp_clicked();
    void slot_action_report_odt_triggered();
    void slot_hide_loading_display_dialog_box(bool cancel_status);
private:
    Ui::examiner_space *ui;

    enum enum_to_do_list_table_column
    {
        enum_to_do_list_table_column_record_no = 0,
        enum_to_do_list_table_column_task
    };


    QFile examiner_notes_html_QFile;
    QPushButton *pushbutton_examiner_notes_color;

    QString to_do_list_database_path;
    recon_helper_standard *recon_helper_standard_obj;
    QLineEdit *line_edit_task_add;
    QLineEdit *line_edit_task_edit;
    QString record_no;

    QGroupBox *groupbox_text_formatting_tool;

    void display_examiner_notes();
    void create_examiner_notes_custom_ui();
    void mergeFormatOnWordOrSelection_examiner_notes(const QTextCharFormat &format);
    void colorChanged_examiner_notes(const QColor &color);

    void create_table_to_do_list();
    void display_to_do_list();

    QFileDialog file_dialog_obj;
    report_template_html_advance *report_html_template_plugin_advance_obj;

    bool bool_message_box_yes_button_clicked;
    message_dialog *message_dialog_obj;

    bool bool_message_box_clicked;

    htmltopdf_wkhtmltopdf *htmltopdf_wkhtmltopdf_obj;

    QString create_html_report_for_examiner_notes();
    QString write_report_initials();
    QString html_header_after_title();
    QString create_examiner_notes_dest_and_copy_essentials(QString dest_path);
    QString write_odt_report_initials();

    display_loading_progress_bar *display_loading_progress_bar_obj;

    bool bool_cancel_operation;

};

#endif // EXAMINER_SPACE_H
