#include "hex_viewer_unallocated.h"
#include "ui_hex_viewer_unallocated.h"

hex_viewer_unallocated::hex_viewer_unallocated(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hex_viewer_unallocated)
{
    ui->setupUi(this);

    bool_is_complete_source = false;

    widget_main    = new QWidget(this);
    //    hex_viewer_obj = new hex_viewer(this);

    //    connect(hex_viewer_obj,SIGNAL(signal_remove_tag_entry_from_tag_search_db_and_display(QString,QString,QString,QStringList,QString)),this,SIGNAL(signal_remove_tag_entry_from_tag_search_db_and_display(QString,QString,QString,QStringList,QString)));
    //    connect(hex_viewer_obj,SIGNAL(signal_remove_tag_entry_from_notes_db_and_display(QString,QString,QString,QString,QString)),this,SIGNAL(signal_remove_tag_entry_from_notes_db_and_display(QString,QString,QString,QString,QString)));


    //    hex_viewer_obj->create_ui_full();
    //    hex_viewer_obj->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    //    QScrollArea *scroll_area_tmp = new QScrollArea(this);
    //    //    scroll_area_tmp->setWidget(hex_viewer_obj);
    //    scroll_area_tmp->setWidgetResizable(true);

    //    QVBoxLayout *v_main_layout = new QVBoxLayout(this);
    //    v_main_layout->addWidget(scroll_area_tmp);

    //    v_main_layout->setContentsMargins(0,0,0,0);

    //    setLayout(v_main_layout);
}

hex_viewer_unallocated::~hex_viewer_unallocated()
{
    delete ui;
}

void hex_viewer_unallocated::pub_set_source_count_name(QString source_str)
{
    source_count_name = source_str;
}

void hex_viewer_unallocated::pub_set_is_complete_source(bool status)
{
    bool_is_complete_source = status;
}

void hex_viewer_unallocated::pub_set_is_unalloctaed_disk_view(bool status)
{
    bool_is_unallocated_disk_view = status;
}

void hex_viewer_unallocated::pub_set_unallocated_db_table_name(QString tbl_str)
{
    unalloc_db_tbl_name = tbl_str;
}


QString hex_viewer_unallocated::pub_get_hex_device_node()
{
    return hex_device_node;
}

void hex_viewer_unallocated::pub_set_global_connection_manager_obj(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
}

void hex_viewer_unallocated::pub_set_essentials()
{

    struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    hex_device_node.clear();

    QString tmp_idntf = struct_source_info.source_disk_identifier;

    if(bool_is_complete_source)
    {
        int tmp_index = tmp_idntf.lastIndexOf("s");
        if(tmp_index > 4) // (input : disk4s1 and output : disk4)
            tmp_idntf = tmp_idntf.mid(0, tmp_index);
    }

    hex_device_node = "/dev/r" + tmp_idntf;

}

void hex_viewer_unallocated::pub_show_disk_in_disk_viewer()
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();

    QString tmp_disk_node = hex_device_node;
    if(tmp_disk_node.startsWith("/dev/r"))
        tmp_disk_node.remove(0, QString("/dev/r").size());

    QString hex_view_table_name = tmp_disk_node + MACRO_RECON_Splitter_1_multiple + QString(MACRO_Plugin_Name_Disk_Hex_Viewer) + MACRO_RECON_Splitter_1_multiple + source_count_name;

    QString display_node_str = QString("/dev/" + tmp_disk_node);

    struct_global_recon_file_info st_globl_recon_file_info_obj;
    st_globl_recon_file_info_obj.display_file_path    = display_node_str; // "/dev/disk*"
    st_globl_recon_file_info_obj.recon_file_info_path = hex_device_node;  // "/dev/rdisk*"
    st_globl_recon_file_info_obj.recon_file_from      = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Examiner_Selected_Path);

    //    hex_viewer_obj->pub_set_disk_viewer_essentials(true, bool_is_complete_source, bool_is_unallocated_disk_view, unalloc_db_tbl_name);


    //    hex_viewer_obj->pub_set_plugin_name(QString(MACRO_Plugin_Name_Disk_Hex_Viewer));

    //    if(bool_is_unallocated_disk_view)
    //        hex_viewer_obj->pub_set_tab_name(QString(MACRO_Plugin_Disk_Hex_Viewer_TAB_Name_Unallocated));
    //    else
    //        hex_viewer_obj->pub_set_tab_name(QString(MACRO_Plugin_Disk_Hex_Viewer_TAB_Name_Complete));


    //    hex_viewer_obj->pub_set_record_no(QString(""));

    //    hex_viewer_obj->pub_set_essentials();
    //    hex_viewer_obj->pub_set_global_connection_manager(global_connection_manager_obj);
    //    hex_viewer_obj->pub_set_source_count_name(source_count_name);
    //    hex_viewer_obj->pub_set_hex_viewer_name(hex_view_table_name);
    //    hex_viewer_obj->pub_set_outsqlite_path(sqlite_path);

    //    hex_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    //    hex_viewer_obj->pub_start_hex_extraction();

    //    hex_viewer_obj->set_file_path_from_db(sqlite_path);

    //    hex_viewer_obj->pub_display_hex_viewer_full();
    //    hex_viewer_obj->pub_set_comboBox_block_list();

    //    hex_viewer_obj->pub_set_file_data_for_complete_disk_viewer(hex_device_node);

    recon_static_functions::app_debug(" End", Q_FUNC_INFO);
}
