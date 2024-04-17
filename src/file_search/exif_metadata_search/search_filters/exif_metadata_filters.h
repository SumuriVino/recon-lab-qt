#ifndef EXIF_METADATA_FILTERS_H
#define EXIF_METADATA_FILTERS_H

#include <QDialog>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_1.h"
#include <QMessageBox>
#include "header_global_variables_2.h"
#include "source_interface/select_source_interface/select_source_interface.h"


namespace Ui {
class exif_metadata_filters;
}

class exif_metadata_filters : public QDialog
{
    Q_OBJECT

public:
    explicit exif_metadata_filters(QWidget *parent = 0);
    ~exif_metadata_filters();


    void pub_load_defaults();

    void pub_set_sources_in_ui();


signals:
    void signal_exif_metadata_filter_work_done(QString command,QString where_statement, QStringList sources_list, QString search_label);

private slots:

    void on_checkBox_latitude_clicked(bool checked);

    void on_checkBox_longitude_clicked(bool checked);

    void on_pushButton_search_clicked();


    void on_radioButton_all_source_toggled(bool checked);

    void on_radioButton_select_source_toggled(bool checked);

    void on_pushButton_select_source_list_clicked();

    void slot_selected_source_list(QStringList selected_src_list);


private:

    Ui::exif_metadata_filters *ui;

    QString latitude_sql_predicate();
    QString longitude_sql_predicate();

    QString author_sql_predicate();
    QString make_sql_predicate();
    QString model_sql_predicate();



    QString contains_str;
    QString matches_str;
    QString starts_with_str;
    QString ends_with_str;

    QList<QCheckBox *> source_checkbox_list;

    select_source_interface *select_source_interface_obj;
    QStringList selected_source_cnt_nm_list;
    QStringList prevoius_selected_source_cnt_nm_list;

};

#endif // EXIF_METADATA_FILTERS_H
