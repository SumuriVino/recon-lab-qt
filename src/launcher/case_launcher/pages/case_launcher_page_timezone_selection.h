#ifndef PAGE_TIMEZONE_SELECTION_H
#define PAGE_TIMEZONE_SELECTION_H

#include <QWidget>
                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_4.h"

namespace Ui {
class case_launcher_page_timezone_selection;
}

class case_launcher_page_timezone_selection : public QWidget
{
    Q_OBJECT

public:
    explicit case_launcher_page_timezone_selection(QWidget *parent = 0);
    ~case_launcher_page_timezone_selection();

    void pub_initialise_timezone_details();
    void pub_update_timezone_info();
    void pub_set_page_caller(QString caller_name);
    void pub_get_timezone_details();

signals:
    void signal_enable_disable_next_button_in_launcher_page(bool bool_val);

private slots:
    void on_radioButton_utc_timezone_toggled(bool checked);

    void on_radioButton_system_timezone_toggled(bool checked);

    void on_radioButton_select_timezone_toggled(bool checked);

    void on_comboBox_user_time_zone_currentIndexChanged(int index);

private:
    Ui::case_launcher_page_timezone_selection *ui;
    void display_timezone_from_db();

    void create_time_zone_list_combobox();

    QString destination_db_file;
    recon_helper_standard *recon_helper_standard_obj;

    QString page_caller_string;
};

#endif // PAGE_TIMEZONE_SELECTION_H
