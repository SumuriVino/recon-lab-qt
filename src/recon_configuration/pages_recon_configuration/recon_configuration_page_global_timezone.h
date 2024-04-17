#ifndef RECON_CONFIGURATION_PAGE_GLOBAL_TIMEZONE_H
#define RECON_CONFIGURATION_PAGE_GLOBAL_TIMEZONE_H

#include <QWidget>
#include <QtCore>
#include "recon_generic_functions/recon_static_functions.h"

#include "recon_configuration/pages_recon_configuration/add_timzones/set_global_timezone.h"

#include "message_box/message_dialog.h"
#include "header_global_variables_3.h"
#include "recon_generic_functions/recon_helper_standard.h"      


namespace Ui {
class recon_configuration_page_global_timezone;
}

class recon_configuration_page_global_timezone : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration_page_global_timezone(QWidget *parent = 0);
    ~recon_configuration_page_global_timezone();

    void pub_set_caller_type(QString caller_type);

private slots:

    void on_pushButton_add_timezone_clicked();
    void slot_set_time_zone(qint64 start_date_epch , qint64 end_date_epch, QString timezone, QString start_date_str, QString end_date_str);
    void slot_on_pushbutton_cancel_clicked();
    void on_pushButton_remove_timezone_clicked();
    void on_pushButton_edit_timezone_clicked();
    void slot_edit_time_zone(qint64 start_date_epch , qint64 end_date_epch, QString timezone , QString start_date_str, QString end_date_str);
    void on_comboBox_timezones_currentTextChanged(const QString &arg1);
    void on_comboBox_date_format_currentTextChanged(const QString &arg1);

private:

    Ui::recon_configuration_page_global_timezone *ui;

    enum enum_timezone_table_widget
    {
        enum_INT_column = 0,
        enum_start_date_readable_column,
        enum_end_date_readable_column,
        enum_start_date_numeric_column,
        enum_end_date_numeric_column,
        enum_time_zone_str_column
    };


    QString caller_type_str;
    QString destination_db_file;

    qint64 current_timezone_offset;
    QString display_time;

    QList <QString> timezone_qlist;
    QList <QString> offset_qlist;
    QList <QString> display_time_name_qlist;


    message_dialog *message_dialog_obj;
    set_global_timezone * set_global_timezone_obj;
    QDialog *qdialog_set_global_timezone;

    void display_data();
    void set_default_time_zone(bool bool_set_label_value);
    void set_essential_variables_for_default_timezones();
    void set_essential_data_lists(bool bool_add_items_combobox);
    void set_essential_variables_for_added_timezones(QString tm_zone);

    bool is_timezone_already_added(qint64 start_date_epch , qint64 end_date_epch );
    bool is_timezone_already_added_for_edit(qint64 start_date_epch , qint64 end_date_epch , QString INT_str);
    recon_helper_standard *recon_helper_standard_obj;

    void set_combobox_items(QStringList combobox_list_items, bool bool_add_items_combobox);
};

#endif // RECON_CONFIGURATION_GLOBAL_TIMEZONE_H
