#ifndef REPORT_LAUNCHER_H
#define REPORT_LAUNCHER_H

#include <QDialog>

#include "pages/report_launcher_page_report_case_info.h"
#include "pages/report_launcher_page_report_type_selection.h"
#include "launcher/plugin_selector/plugin_selector.h"

#include "header_global_variables_1.h"
                                   
#include "header_report.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"
#include <QMessageBox>

namespace Ui {
class report_launcher;
}

class report_launcher : public QDialog
{
    Q_OBJECT

public:
    explicit report_launcher(QWidget *parent = 0);
    ~report_launcher();

    void set_narad_muni_for_report(narad_muni*);
    void pub_initialise_launcher();

signals:
    void signal_all_report_selection_completed();
    void signal_set_window_title(QString);

private slots:
    void on_pushButton_next_clicked();

    void on_pushButton_back_clicked();
    void slot_plugins_selected(bool status);
    void slot_report_type_selected(bool check);
    void slot_report_scope_selected(QString scope);

    void on_checkBox_show_password_toggled(bool checked);

    void on_checkBox_encrypt_report_toggled(bool checked);

private:
    Ui::report_launcher *ui;

    enum report_launcher_page{
        enum_report_launcher_page_case_info,
        enum_report_launcher_page_report_type_selection,
        enum_report_launcher_report_plugin_selection
    };

    page_report_case_info *page_report_case_info_object;
    page_report_type_selection *page_report_type_selection_object;
    plugin_selector *page_report_plugins_selection_object;

    narad_muni *narad_muni_for_report_obj;
    bool bool_plugins_selected;
    bool bool_report_type_selected;


    void get_info_from_pages_for_generate_report();
    QString report_scope_name;
    void get_taged_selected_plugins_info();
    QStringList get_plugin_and_tab_list(QStringList selected_plugins_with_and_tabs_list, QString os_name);

    recon_helper_standard *recon_helper_standard_obj;

};

#endif // REPORT_LAUNCHER_H
