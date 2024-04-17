#ifndef HEX_VIEWER_UNALLOCATED_H
#define HEX_VIEWER_UNALLOCATED_H

#include <QWidget>
#include <QObject>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"
                                   
#include "header_global_variables_1.h"
#include "tsk/libtsk.h"
#include "global_connection_manager.h"
#include "header_global_variables_2.h"

namespace Ui {
class hex_viewer_unallocated;
}

class hex_viewer_unallocated : public QWidget
{
    Q_OBJECT

public:
    explicit hex_viewer_unallocated(QWidget *parent = 0);
    ~hex_viewer_unallocated();

    void pub_set_source_count_name(QString source_str);
    void pub_show_disk_in_disk_viewer();

    void pub_set_is_complete_source(bool status);
    QString pub_get_hex_device_node();
    void pub_set_essentials();
    void pub_set_global_connection_manager_obj(global_connection_manager *obj);
    void pub_set_is_unalloctaed_disk_view(bool status);
    void pub_set_unallocated_db_table_name(QString tbl_str);

signals:
    void signal_remove_tag_entry_from_tag_search_db_and_display(QString plugin_name, QString tab_name, QString record_no, QStringList removed_tags_list, QString os_scheme);
    void signal_remove_tag_entry_from_notes_db_and_display(QString plugin_name, QString tab_name, QString record_no, QString notes_val, QString os_scheme);

private:
    Ui::hex_viewer_unallocated *ui;

    global_connection_manager *global_connection_manager_obj;

    QWidget *widget_main;

    //hex_viewer *hex_viewer_obj;
    QString source_count_name;

    struct_GLOBAL_witness_info_source struct_source_info;

    QString hex_device_node;
    bool bool_is_complete_source;

    bool bool_is_unallocated_disk_view = false;
    QString unalloc_db_tbl_name;

};

#endif // HEX_VIEWER_UNALLOCATED_H
