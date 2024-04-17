#ifndef LOAD_CASE_SOURCE_DISPLAYER_H
#define LOAD_CASE_SOURCE_DISPLAYER_H

#include <QDialog>
#include <QWidget>
#include <QtCore>
#include "launcher/source_selector/source_selector.h"
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class load_case_source_displayer;
}

class load_case_source_displayer : public QDialog
{
    Q_OBJECT

public:
    explicit load_case_source_displayer(QWidget *parent = 0);
    ~load_case_source_displayer();

    void pub_set_list_of_source_path_and_type_in_page(QStringList source_path_with_type_list);


signals:
    void signal_load_case_set_sources_path(QList<struct_GLOBAL_witness_info_source> list_target_info, bool is_ok_clicked);

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::load_case_source_displayer *ui;

    source_selector *source_selector_obj;

    recon_helper_standard *recon_helper_standard_obj;


    void prepare_all_sources_list_from_root_source();
    QStringList get_source_count_name_list_from_root_source(QString root_count_name);
};

#endif // LOAD_CASE_SOURCE_DISPLAYER_H
