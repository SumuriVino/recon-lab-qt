#ifndef ADD_NEW_SOURCE_H
#define ADD_NEW_SOURCE_H

#include <QWidget>

#include "launcher/source_selector/source_selector.h"
#include "recon_generic_functions/recon_static_functions.h"

                                   
#include "header_identical_2.h"

namespace Ui {
class add_new_source;
}

class add_new_source : public QWidget
{
    Q_OBJECT

public:
    explicit add_new_source(QWidget *parent = 0);
    ~add_new_source();


signals:

    void signal_update_recon_case(QList<struct_GLOBAL_witness_info_root>);

private slots:

    void on_pushButton_next_clicked();

    void slot_source_selected(bool is_source_selected);

    void slot_added_root_details_list(QList<struct_GLOBAL_witness_info_root> target_list);

    void slot_hide_source_selector_parent_window();
    void slot_qml_button_initial_back_clicked();

private:
    Ui::add_new_source *ui;

    enum multi_source_wizard_common_page{
        enum_comman_multi_source_page_multi_source_selection = 0
    };
    bool bool_source_selected;


    source_selector *source_selector_obj;

    void get_data_from_the_multisource_launcher_pages();
    QList<struct_GLOBAL_witness_info_root> list_root_info;

};

#endif // ADD_NEW_SOURCE_H
