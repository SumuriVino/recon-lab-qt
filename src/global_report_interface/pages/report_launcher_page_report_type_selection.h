#ifndef PAGE_REPORT_TYPE_SELECTION_H
#define PAGE_REPORT_TYPE_SELECTION_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QFileDialog>

                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "header_report.h"
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class page_report_type_selection;
}

class page_report_type_selection : public QDialog
{
    Q_OBJECT

public:
    explicit page_report_type_selection(QWidget *parent = 0);
    ~page_report_type_selection();
    void set_narad_muni_for_report(narad_muni*);
    void pub_initialise_report_type_page();

    void pub_set_report_type_narad_muni();
signals:

    void signal_report_type_selected(bool);
    void signal_report_scope_selected(QString scope);

private slots:
    void slot_tags_horizontal_header_clicked(int index);
    void on_pushButton_report_save_path_clicked();
    void on_radioButton_full_plugin_toggled(bool checked);

    void on_radioButton_tags_toggled(bool checked);

    void on_tableWidget_tags_cellClicked(int row, int column);

    void on_checkBox_advance_html_toggled(bool checked);

    void on_checkBox_standard_html_toggled(bool checked);

    void on_checkBox_pdf_toggled(bool checked);

    void on_checkBox_csv_toggled(bool checked);

    void on_checkBox_xml_toggled(bool checked);

    void on_radioButton_tags_clicked();

    void on_radioButton_full_plugin_clicked();

    void on_checkBox_tag_export_toggled(bool checked);


private:
    Ui::page_report_type_selection *ui;

    narad_muni *narad_muni_for_report_obj;

    void display_tags_table();

    bool select_all_tags;
    QTableWidgetItem *horizontal_header_select_tags;
    QStringList tags_list_from_database;


    void set_report_default_name();
    QString report_dir;

    bool bool_report_type_is_selected;
    bool bool_is_tags_selected;
    QFileDialog file_dialog_obj;
    recon_helper_standard *recon_helper_standard_obj;

};

#endif // PAGE_REPORT_TYPE_SELECTION_H
