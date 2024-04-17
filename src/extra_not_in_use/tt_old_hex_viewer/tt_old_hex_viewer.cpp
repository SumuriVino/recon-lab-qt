#include "tt_old_hex_viewer.h"
#include "ui_tt_old_hex_viewer.h"

hex_viewer::hex_viewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hex_viewer)
{
    //    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    bool_is_called_from_detailed_info = false;
    setWindowFlag(Qt::Window);
    setAcceptDrops(true);

    hex_edit_obj = new QHexEdit(this);

    current_page_count = 0;
    tmp_no_of_pages = 0;

    selection_button_checked = true;
    offset_button_checked = true;

    // Tag Add and Export
    export_tag_module_obj   = new export_tag_module(this);
    export_selected_bytes_obj = new hex_viewer_export_selected_bytes(this);

    bool_cancel_extraction = false;

    message_dialog_obj = new message_dialog(this);

    bool_at_page_end_for_search_highlight = true;
    current_cursor_pos_for_prev_clicked   = -1;

    bool_show_informative_dialogs = true;

    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false;

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);

    tsk_default_block_size = 512;


}

hex_viewer::~hex_viewer()
{
    delete ui;
}

void hex_viewer::slot_hide_loading_display_dialog_box(bool cancel_status)
{
    bool_cancel_loading = true;
}

void hex_viewer::loading_display_dialog_based_on_records(int total_records, int processed_count)
{
    QString label_text = QString("Loading...   ") + QString::number(processed_count) + "/" + QString::number(total_records);
    display_loading_progress_bar_obj->pub_set_label_messsge(label_text);

    int temp_count = processed_count;
    float record_percent = ((temp_count*100)/total_records);
    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(int(record_percent));

}


void hex_viewer::viewer_connections()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    connect(pushButton_next_goto_page, SIGNAL(clicked()), this, SLOT(on_pushButton_next_clicked()));
    connect(pushButton_prev_goto_page, SIGNAL(clicked()), this, SLOT(on_pushButton_prev_clicked()));
    connect(pushButton_goto_offset, SIGNAL(clicked()), this, SLOT(on_pushButton_goto_offset_clicked()));
    connect(pushButton_search, SIGNAL(clicked()), this, SLOT(on_pushButton_search_clicked()));
    connect(pushButton_search_next, SIGNAL(clicked()), this, SLOT(on_pushButton_find_next_clicked()));
    connect(pushButton_search_previous, SIGNAL(clicked()), this, SLOT(on_pushButton_find_previous_clicked()));
    connect(pushButton_selection_info_offset, SIGNAL(clicked(bool)), this, SLOT(on_pushButton_offset_clicked(bool)));
    connect(pushButton_selection_info_byte_count, SIGNAL(clicked(bool)), this, SLOT(on_pushButton_selection_clicked(bool)));
    connect(checkBox_unsigned, SIGNAL(clicked(bool)), this, SLOT(on_checkBox_unsigned_clicked(bool)));
    connect(checkBox_big_endian, SIGNAL(clicked(bool)), this, SLOT(on_checkBox_big_endian_clicked(bool)));
    connect(lineEdit_go_to_offset, SIGNAL(returnPressed()), this, SLOT(on_pushButton_goto_offset_clicked()));
    connect(lineEdit_search, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_search_returnPressed()));
    connect(lineEdit_search, SIGNAL(textChanged(QString)), this, SLOT(on_lineEdit_search_text_change(QString)));

    connect(spinBox_page_size, SIGNAL(valueChanged(QString)), this, SLOT(on_spinBox_page_size_valueChanged(QString)));
    connect(pushbutton_go_to_page, SIGNAL(clicked()), this, SLOT(on_pushButton_go_to_page_clicked()));
    connect(lineedit_goto_page,SIGNAL(returnPressed()) , this, SLOT(on_pushButton_go_to_page_clicked()));
    connect(pushbutton_open_in_other_recon_viewers,SIGNAL(clicked()),this,SLOT(slot_open_in_other_recon_viewers_clicked()));

    // Hex Search
    connect(this,SIGNAL(search_hex(QByteArray, uint64_t,bool,bool)),hex_edit_obj,SLOT(slot_search_hex(QByteArray, uint64_t,bool,bool)));

    // ASCII Search
    connect(this,SIGNAL(search_ascii(QString, uint64_t, bool,bool)),hex_edit_obj,SLOT(slot_search_ascii(QString, uint64_t, bool,bool)));

    connect(this,SIGNAL(signal_set_informative_status(bool)),hex_edit_obj,SLOT(slot_set_informative_dialog_status(bool)));

    connect(hex_edit_obj,SIGNAL(hex_value_of_cur(QByteArray)),this,SLOT(slot_cur_hex(QByteArray)));
    connect(this,SIGNAL(go_to_offset(uint64_t, uint64_t)),hex_edit_obj,SLOT(slot_go_to_offset(uint64_t, uint64_t)));
    connect(this,SIGNAL(search_next(int,int)),hex_edit_obj,SLOT(slot_search_next(int,int)));
    connect(this,SIGNAL(search_previous(int,int)),hex_edit_obj,SLOT(slot_search_previous(int,int)));
    connect(hex_edit_obj,SIGNAL(size_index_list(int)),this,SLOT(slot_index_list_size(int)));
    connect(this,SIGNAL(set_searched_data_clear(int)),hex_edit_obj,SLOT(slot_set_page_search_clear(int)));
    connect(this,SIGNAL(signal_highlight_ascii_searched_text(QString, uint64_t)),hex_edit_obj,SLOT(slot_highlight_ascii_text_on_search(QString, uint64_t)));
    connect(this,SIGNAL(signal_highlight_hex_searched_data(QByteArray,uint64_t)),hex_edit_obj,SLOT(slot_highlight_hex_data_on_search(QByteArray, uint64_t)));

    connect(hex_edit_obj,SIGNAL(current_cur_offset(int)),this,SLOT(current_offset_cursor(int)));
    connect(hex_edit_obj,SIGNAL(bytes_offset_selected(int, int,bool)), this, SLOT(selct_bytes_offsets(int, int, bool)));

    connect(export_tag_module_obj, SIGNAL(export_clicked(QString)), this, SLOT(slot_export_clicked(QString)));
    connect(hex_edit_obj, SIGNAL(signal_hex_edit_tag_start_offset(int)), this, SLOT(slot_hex_viewer_tag_start_offset_clicked(int)));

    // Change offset
    connect(this, SIGNAL(signal_change_address_area_value(uint64_t)),hex_edit_obj,SLOT(slot_change_offset_value(uint64_t)));

    //Combobox Export block index changed
    connect(comboBox_blocks_list,SIGNAL(currentIndexChanged(QString)),this,SLOT(slot_combobox_export_block_index_change(QString)));

    //clear block index list
    connect(this,SIGNAL(clear_block_index_list()),hex_edit_obj,SLOT(slot_clear_block_list()));

    // Current Page details
    connect(this,SIGNAL(signal_current_page_details(int,uint64_t,uint64_t)),hex_edit_obj,SLOT(slot_current_page_details(int,uint64_t,uint64_t)));
    connect(hex_edit_obj,SIGNAL(signal_export_selected_bytes(QByteArray)),this,SLOT(slot_export_selected_bytes(QByteArray)));
    connect(export_selected_bytes_obj,SIGNAL(signal_save_exported_file_click(QString,QString)),this,SLOT(slot_export_bytes_save_clicked(QString,QString)));
    connect(this,SIGNAL(signal_set_blocks_list_for_this_file(QList<struct_global_hex_viewer_start_and_end_index>)),hex_edit_obj,SLOT(slot_set_already_set_blocks(QList<struct_global_hex_viewer_start_and_end_index>)));

    connect(pushbutton_export_block,SIGNAL(clicked()),this,SLOT(slot_pushbutton_export_clicked()));
    connect(pushButton_remove_block,SIGNAL(clicked()),this,SLOT(slot_pushbutton_remove_block_clicked()));

    connect(lineedit_goto_page,SIGNAL(textEdited(QString)),this,SLOT(slot_line_edit_go_to_page_text_changed(QString)));

    connect(this,SIGNAL(signal_set_context_menu(bool)),hex_edit_obj,SLOT(slot_set_context_menu(bool)));

    connect(hex_edit_obj, SIGNAL(signal_insert_selected_bytes_info_in_db(int,int,QString,QString)),this, SLOT(slot_insert_tag_selected_bytes_info_in_db(int,int,QString,QString)));

    connect(hex_edit_obj, SIGNAL(signal_hex_edit_tag_end_offset_clicked(QString, QString, uint64_t, uint64_t)), this, SLOT(slot_hex_viewer_tag_end_offset_clicked(QString, QString, uint64_t, uint64_t)));

    connect(this,SIGNAL(signal_add_selected_bytes_to_block_clicked(QString,QString)),hex_edit_obj,SLOT(slot_add_selected_bytes_to_block_clicked(QString,QString)));
    connect(this, SIGNAL(signal_tag_start_index_clicked(bool)), hex_edit_obj, SLOT(slot_tag_start_index_clicked(bool)));
    connect(this, SIGNAL(signal_set_tag_start_offset(uint64_t)), hex_edit_obj, SLOT(slot_hex_edit_set_tag_start_offset(uint64_t)));

    connect(hex_edit_obj, SIGNAL(signal_page_end_reached_for_search_highlight(bool)), this, SLOT(slot_page_end_reached_for_search_highlight(bool)));
    connect(hex_edit_obj, SIGNAL(signal_cursor_current_pos_for_previous_click(qint64)), this, SLOT(slot_cursor_current_pos_for_previous_click(qint64)));



    recon_static_functions::app_debug("ends ",Q_FUNC_INFO);
}

void hex_viewer::setdata(const QByteArray &data)
{
    if(data.isEmpty())
        reset_all_the_widgets_data();

    hex_edit_obj->setData(data);
}

void hex_viewer::pub_set_outsqlite_path(QString name)
{
    destination_hexview_index_db_path = name;
}

void hex_viewer::pub_set_source_count_name(QString name)
{
    source_count_name = name;
}

void hex_viewer::pub_set_called_from_detailed_info()
{
    bool_is_called_from_detailed_info = true;
}

void hex_viewer::pub_set_called_from_metadata_display()
{
    bool_is_called_from_metadata_display = true;
}

void hex_viewer::pub_set_global_connection_manager(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
}

void hex_viewer::pub_highlight_keyword(QString keyword_str)
{
    keyword_str = keyword_str.trimmed();

    if(keyword_str.isEmpty())
        return;


    bool_show_informative_dialogs = false;

    emit signal_set_informative_status(bool_show_informative_dialogs);

    lineEdit_search->setText(keyword_str);
    comboBox_search_type->setCurrentIndex(1);
    on_pushButton_search_clicked();
}


void hex_viewer::pub_set_hex_viewer_name(QString name)
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    bool_cancel_extraction = false;

    QStringList list_value = name.split(MACRO_RECON_Splitter_1_multiple, QString::SkipEmptyParts);

    if(list_value.size() > 0)
        hex_file_name = list_value.at(0);

    if(list_value.size() > 1)
        hex_plugin_name = list_value.at(1);

    if(list_value.size() > 2)
        hex_plugin_category_name = list_value.at(2);

    if(list_value.size() > 3)
        hex_plugin_record_no = list_value.at(3);

    QString file_name;
    if(hex_plugin_name == QString(MACRO_Plugin_Name_Disk_Hex_Viewer))
        file_name = hex_plugin_name ;
    else
        file_name = hex_file_name + " [" + hex_plugin_name +" "+ hex_plugin_record_no + "]";

    file_name = file_name.trimmed();

    //- Display name..
    hex_viewer_display_name = file_name;

    if(hex_plugin_name == QString(MACRO_Plugin_Name_Disk_Hex_Viewer))
        file_name = hex_file_name + " " + hex_plugin_name;
    else
        file_name = hex_file_name + " " + hex_plugin_name +" "+ hex_plugin_record_no;

    file_name = file_name.trimmed();


    //db_table_name.
    file_name = file_name.replace(" ","_");
    file_name = file_name.replace("-","_");
    file_name = file_name.replace(".","_");
    hex_view_db_table_name = file_name;

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::pub_set_essentials()
{

    recon_result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    hex_edit_obj->pub_set_essentials();
}

void hex_viewer::set_viewer_diaplay_name(QString display_nm)
{
    hex_viewer_display_name = display_nm;
}

QString hex_viewer::set_file_path_from_db(QString db_path)
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    QString command = "select recon_file_infopath,INT,source_count_name , file_path , recon_filefrom from 'hex_viewer_index' where viewer_display_name = ?";
    QStringList list;
    list << hex_viewer_display_name;

    QString file_path_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,list,0,db_path,Q_FUNC_INFO);
    QString INT_str   = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,list,1,db_path,Q_FUNC_INFO);
    if(!bool_is_from_disk_viewer)
        source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,list,2,db_path,Q_FUNC_INFO);

    QString display_filepath = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,list,3,db_path,Q_FUNC_INFO);
    QString recon_file_from = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,list,4,db_path,Q_FUNC_INFO);
    st_globl_recon_file_info_obj.display_file_path = display_filepath;
    st_globl_recon_file_info_obj.recon_file_from = recon_file_from;
    st_globl_recon_file_info_obj.recon_file_info_path = file_path_str;

    file_path_str = recon_helper_standard_obj->get_file_infopath_by_dbpath(INT_str,db_path, "hex_viewer_index", source_count_name, Q_FUNC_INFO);

    file_path = file_path_str;

    block_db_path = db_path;

    return file_path;

}

void hex_viewer::set_file_path_and_db_path(QString file_path_str,QString db_path)
{
    file_path = file_path_str;

    block_db_path = db_path;
}

void hex_viewer::slot_set_block_db_path(QString db_path)
{
    block_db_path = db_path;
}

void hex_viewer::pub_set_hex_path(QString path)
{
    destination_hex_path = path;
}

void hex_viewer::pub_set_comboBox_block_list()
{
    while(comboBox_blocks_list->count() != 0)
    {
        comboBox_blocks_list->removeItem(0);
    }

    comboBox_blocks_list->addItems(combobox_display_list);
}

void hex_viewer::pub_display_hex_viewer_full()
{
    extract_block_name_list();
}

void hex_viewer::pub_start_hex_extraction()
{
    entry_in_index_table();
}

void hex_viewer::pub_set_recon_file_info(struct_global_recon_file_info obj)
{
    st_globl_recon_file_info_obj = obj;
}


void hex_viewer::entry_in_index_table()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_hexview_index_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug( destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QString disp_file_path_str = st_globl_recon_file_info_obj.display_file_path;
    QString recon_file_from =  st_globl_recon_file_info_obj.recon_file_from;
    QString recon_file_info_path = st_globl_recon_file_info_obj.recon_file_info_path;

    QSqlQuery query_index(destination_db);
    query_index.prepare("insert into 'hex_viewer_index' (viewer_display_name,db_table_name,file_name,file_path,plugin_name,category_name,record_no,creation_timestamp,source_count_name,recon_filefrom,recon_file_infopath) values (?,?,?,?,?, ?,?,?,?,?,?)");

    query_index.addBindValue(hex_viewer_display_name);
    query_index.addBindValue(hex_view_db_table_name);
    query_index.addBindValue(hex_file_name);
    query_index.addBindValue(disp_file_path_str);
    query_index.addBindValue(plugin_name_str);
    query_index.addBindValue(tab_name_str);
    query_index.addBindValue(record_no);

    query_index.addBindValue(QString::number(QDateTime::currentDateTimeUtc().toSecsSinceEpoch()));
    query_index.addBindValue(source_count_name);
    query_index.addBindValue(recon_file_from);
    query_index.addBindValue(recon_file_info_path);

    if(!query_index.exec())
    {
        recon_static_functions::app_debug(" query_index execution-----FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug(query_index.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(query_index.executedQuery() ,Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    destination_db.close();

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void hex_viewer::dropEvent(QDropEvent *event)
{
    //    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    file_path = event->mimeData()->text();

    if(file_path.contains("file://"))
        file_path.replace("file://","");

    QFileInfo info(file_path);

    if(!info.exists())
        return;

    if(info.isDir())
        return;

    if(recon_static_functions::check_is_it_plist_file_by_signature(file_path, Q_FUNC_INFO))
    {
        pushbutton_open_in_other_recon_viewers->setText(QString("Open in ") +  QString(MACRO_Plugin_Name_Plist_Viewer));
    }
    else if(recon_static_functions::check_is_it_sqlite_file_by_signature(file_path, Q_FUNC_INFO))
    {
        pushbutton_open_in_other_recon_viewers->setText(QString("Open in ") + QString(MACRO_Plugin_Name_Sqlite_Viewer));
    }
    else
    {
        groupBox_open_in_other_recon_viewers->hide();
    }

    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString tmp_file_path = info.filePath();

    if(!tmp_file_path.trimmed().isEmpty() && !tmp_file_path.startsWith("/"))
        tmp_file_path.prepend("/");

    if(tmp_file_path.startsWith(struct_source_info.virtual_source_path))
    {
        tmp_file_path.remove(0, struct_source_info.virtual_source_path.size());
        if(!tmp_file_path.trimmed().isEmpty() && !tmp_file_path.startsWith("/"))
            tmp_file_path.prepend("/");
    }

    label_file_name->setText("<b>File Name : </b>" + info.fileName());
    label_file_path->setText("<b>File Path : </b>" + tmp_file_path);
    label_file_size->setText("<b>File Size : </b>" + recon_static_functions::human_readable_size(info.size(), Q_FUNC_INFO));

    QFile myfile(file_path);
    if(!myfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + myfile.errorString(),Q_FUNC_INFO);
        return;
    }

    qint64 page_size = spinBox_page_size->text().toLongLong();

    if(page_size <= 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert a valid page size");
        message_dialog_obj->show();
        myfile.close();
        return;
    }

    if(page_size > myfile.size())
        page_size = myfile.size();

    data_arr = myfile.read(page_size);

    total_file_size = myfile.size();

    if(data_arr.isEmpty())
    {
        pushButton_selection_info_byte_count->setText("");
        pushButton_selection_info_offset->setText("");
        //        lineEdit_8_bit->clear();
        //        lineEdit_16_bit->clear();
        //        lineEdit_32_bit->clear();
        //        lineEdit_binary->clear();
        lineEdit_search->clear();
        //        lineEdit_mac_timestamp->clear();
        //        lineEdit_unix_timestamp->clear();
    }

    myfile.close();

    current_offset = page_size;
    previous_entered_size = page_size;

    current_page_count = 1;

    selected_data_size = data_arr.size();

    setdata(data_arr);

    tmp_no_of_pages = myfile.size() / page_size;

    if((myfile.size() % page_size) == 0)
    {
        total_no_of_pages = tmp_no_of_pages;
        label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(total_no_of_pages));
    }
    else
    {
        total_no_of_pages = tmp_no_of_pages + 1;
        label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(total_no_of_pages));
    }

    emit clear_block_index_list();
    emit signal_current_page_details(current_page_count,0,current_offset-1);

    //    if(is_full_or_meta)
    fill_the_existing_blocks_details_in_list();

    pushButton_next_goto_page->setEnabled(true);
    pushButton_prev_goto_page->setEnabled(false);

    if(current_page_count >= total_no_of_pages)
    {
        pushButton_next_goto_page->setEnabled(false);
    }

    //    recon_static_functions::app_debug(" ends",Q_FUNC_INFO);
}

void hex_viewer::set_file_data(QString path)
{
    //    recon_static_functions::app_debug(" start",Q_FUNC_INFO);


    file_path = path;

    QFileInfo info(file_path);

    if(!info.exists() || info.isDir())
        return;


    if(!bool_is_called_from_detailed_info)
    {
        if(recon_static_functions::check_is_it_plist_file_by_signature(file_path, Q_FUNC_INFO))
        {
            pushbutton_open_in_other_recon_viewers->setText(QString("Open in ") +  QString(MACRO_Plugin_Name_Plist_Viewer));

        }
        else if(recon_static_functions::check_is_it_sqlite_file_by_signature(file_path, Q_FUNC_INFO))
        {
            pushbutton_open_in_other_recon_viewers->setText(QString("Open in ") + QString(MACRO_Plugin_Name_Sqlite_Viewer));
        }
        else
        {
            groupBox_open_in_other_recon_viewers->hide();
        }
    }

    QString tmp_file_path = st_globl_recon_file_info_obj.display_file_path;


    label_file_name->setText("<b>File Name : </b>" + QFileInfo(tmp_file_path).fileName());
    label_file_path->setText("<b>File Path : </b>" + tmp_file_path);
    label_file_size->setText("<b>File Size : </b>" + recon_static_functions::human_readable_size(info.size(), Q_FUNC_INFO));

    QFile myfile(file_path);
    if(!myfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + myfile.errorString(),Q_FUNC_INFO);

        setdata("");
        return;
    }

    qint64 page_size = spinBox_page_size->text().toLongLong();

    if(page_size <= 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert a valid page size");
        message_dialog_obj->show();
        myfile.close();
        return;
    }

    data_arr = myfile.read(page_size);

    total_file_size = myfile.size();

    if(data_arr.isEmpty())
    {
        pushButton_selection_info_byte_count->setText("");
        pushButton_selection_info_offset->setText("");
        //        lineEdit_8_bit->clear();
        //        lineEdit_16_bit->clear();
        //        lineEdit_32_bit->clear();
        //        lineEdit_binary->clear();
        lineEdit_search->clear();
        //        lineEdit_mac_timestamp->clear();
        //        lineEdit_unix_timestamp->clear();
    }

    myfile.close();

    current_offset = page_size;
    previous_entered_size = page_size;

    current_page_count = 1;

    selected_data_size = data_arr.size();

    setdata(data_arr);

    tmp_no_of_pages = myfile.size() / page_size;

    if((myfile.size() % page_size) == 0)
    {
        label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages));
        total_no_of_pages = tmp_no_of_pages;
    }
    else
    {
        label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages + 1));
        total_no_of_pages = tmp_no_of_pages + 1;
    }

    emit signal_current_page_details(current_page_count,0,current_offset-1);

    emit clear_block_index_list();

    if(!bool_is_called_from_detailed_info)
        fill_the_existing_blocks_details_in_list();

    pushButton_next_goto_page->setEnabled(true);
    pushButton_prev_goto_page->setEnabled(false);

    if(current_page_count >= total_no_of_pages)
    {
        pushButton_next_goto_page->setEnabled(false);
    }

    //    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}


void hex_viewer::on_pushButton_next_clicked()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    set_palette_lineedit_goto_page(0);

    label_search_results->setText("");
    lineEdit_search->setText("");

    qint64 entered_page_size = spinBox_page_size->text().toLongLong();

    if(entered_page_size <= 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert a valid page size");
        message_dialog_obj->show();
        return;
    }

    if(bool_is_from_disk_viewer)
    {
        if(list_disk_hex_view_data.size() > 0)
        {
            pushButton_next_goto_page->setEnabled(false);

            if(bool_disk_viewer_unallocated_disk)
            {
                get_next_block_for_unallocated_disk_hex_view();
            }
            else
            {
                struct_disk_hex_view_data disk_hex_view_data_obj = list_disk_hex_view_data.at(0);
                get_next_block_for_complete_disk_hex_view(disk_hex_view_data_obj.fs_info_ptr);
            }
        }
    }
    else
    {
        index_list_size = 0;

        previous_entered_size = entered_page_size;

        if(total_file_size - current_offset < previous_entered_size)
            previous_entered_size = total_file_size - current_offset;

        lineedit_goto_page->clear();

        if(current_offset < total_file_size)
        {
            QFile myfile(file_path);
            if(!myfile.open(QIODevice::ReadOnly))
            {
                recon_static_functions::app_debug("file open ---FAILED--- " + file_path,Q_FUNC_INFO);
                recon_static_functions::app_debug("error " + myfile.errorString(),Q_FUNC_INFO);
                return;
            }

            myfile.seek(current_offset);

            current_page_count++;

            QByteArray selected_page_size_bytes = myfile.read(previous_entered_size);

            setdata(selected_page_size_bytes);

            selected_data_size = selected_page_size_bytes.size();

            QString text_display = QString::number(0) + "/" + QString::number(selected_page_size_bytes.size() - 1);
            pushButton_selection_info_offset->setText(text_display);

            if((myfile.size() % entered_page_size) == 0)
            {
                label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages));
                total_no_of_pages = tmp_no_of_pages;
            }
            else
            {
                label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages + 1));
                total_no_of_pages = tmp_no_of_pages + 1;
            }

            emit signal_change_address_area_value(current_offset-1);

            emit signal_current_page_details(current_page_count,current_offset,(current_offset + previous_entered_size - 1));

            current_offset += previous_entered_size;

            pushButton_prev_goto_page->setEnabled(true);

            myfile.close();
        }
    }

    fill_the_existing_blocks_details_in_list();

    if(current_page_count >= total_no_of_pages)
    {
        pushButton_next_goto_page->setEnabled(false);
    }

    emit set_searched_data_clear(0);

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::on_pushButton_prev_clicked()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    qint64 entered_page_size = spinBox_page_size->text().toLongLong();
    set_palette_lineedit_goto_page(0);

    label_search_results->setText("");
    lineEdit_search->setText("");

    if(entered_page_size <= 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert a valid page size");
        message_dialog_obj->show();
        return;
    }

    index_list_size = 0;

    lineedit_goto_page->clear();

    qint64 curr_off = current_offset - (previous_entered_size + entered_page_size);
    selected_data_size = entered_page_size;
    current_page_count--;
    current_offset = curr_off;

    if(bool_is_from_disk_viewer)
    {
        if(list_disk_hex_view_data.size() > 0)
        {
            pushButton_next_goto_page->setEnabled(false);

            if(bool_disk_viewer_unallocated_disk)
            {
                get_previous_block_for_unallocated_disk_hex_view();
            }
            else
            {
                struct_disk_hex_view_data disk_hex_view_data_obj = list_disk_hex_view_data.at(0);
                get_previous_block_for_complete_disk_hex_view(disk_hex_view_data_obj.fs_info_ptr);
            }


        }
    }
    else
    {
        if(curr_off >= 0)
        {

            QFile myfile(file_path);
            if(!myfile.open(QIODevice::ReadOnly))
            {
                recon_static_functions::app_debug("file open ---FAILED--- " + file_path,Q_FUNC_INFO);
                recon_static_functions::app_debug("error " + myfile.errorString(),Q_FUNC_INFO);
                return;
            }

            myfile.seek(current_offset);

            QByteArray selected_page_size_bytes = myfile.read(entered_page_size);

            setdata(selected_page_size_bytes);

            if(current_offset != 0)
            {
                emit signal_change_address_area_value(current_offset-1);
            }
            else
            {
                emit signal_change_address_area_value(current_offset);
            }

            emit signal_current_page_details(current_page_count,(current_offset),(current_offset + entered_page_size -1));

            current_offset += entered_page_size;

            QString text_display = QString::number(0) + "/" + QString::number(entered_page_size - 1);
            pushButton_selection_info_offset->setText(text_display);

            if((myfile.size() % entered_page_size) == 0)
            {
                label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages));
                total_no_of_pages = tmp_no_of_pages;
            }
            else
            {
                label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages + 1));
                total_no_of_pages = tmp_no_of_pages + 1;
            }

            previous_entered_size = entered_page_size;
            pushButton_next_goto_page->setEnabled(true);

            myfile.close();

        }
        else
        {
            qint64 remaining_bytes = entered_page_size + curr_off;

            if(remaining_bytes > 0)
            {
                selected_data_size = remaining_bytes;

                current_page_count--;

                QFile myfile(file_path);
                if(!myfile.open(QIODevice::ReadOnly))
                {
                    recon_static_functions::app_debug("file open ---FAILED--- " + file_path,Q_FUNC_INFO);
                    recon_static_functions::app_debug("error " + myfile.errorString(),Q_FUNC_INFO);
                    return;
                }

                myfile.seek(0);

                QByteArray selected_page_size_bytes = myfile.read(remaining_bytes);

                setdata(selected_page_size_bytes);

                QString text_display = QString::number(0) + "/" + QString::number(remaining_bytes - 1);
                pushButton_selection_info_offset->setText(text_display);

                if((myfile.size() % entered_page_size) == 0)
                {
                    label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages));
                    total_no_of_pages = tmp_no_of_pages;
                }
                else
                {
                    label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages + 1));
                    total_no_of_pages = tmp_no_of_pages + 1;
                }

                previous_entered_size = remaining_bytes;

                if(current_offset != 0)
                {
                    emit signal_change_address_area_value(current_offset-1);
                }
                else
                {
                    emit signal_change_address_area_value(current_offset);
                }

                emit signal_current_page_details(current_page_count,(current_offset),(remaining_bytes-1));

                current_offset = remaining_bytes;
                pushButton_next_goto_page->setEnabled(true);

                myfile.close();

            }
        }
    }


    fill_the_existing_blocks_details_in_list();

    if(current_page_count <= 1)
    {
        pushButton_prev_goto_page->setEnabled(false);
    }

    emit set_searched_data_clear(0);

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::common_go_to_page(qint64 enter_page_index)
{

    qint64 data_size_entered = spinBox_page_size->text().trimmed().toLongLong();

    QFile myfile(file_path);
    if(!myfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ----  FAILED ----",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + myfile.errorString(),Q_FUNC_INFO);
        return;
    }

    int total_no_of_pages = myfile.size() / data_size_entered;

    if((myfile.size() % data_size_entered) != 0)
    {
        total_no_of_pages += 1;
    }

    if(enter_page_index <= 0 || enter_page_index > total_no_of_pages)
    {
        set_palette_lineedit_goto_page(1);
        myfile.close();
        return;
    }

    qint64 page_end_offset      = enter_page_index * data_size_entered;
    qint64 page_start_offset    = page_end_offset - data_size_entered;

    myfile.seek(page_start_offset);

    QByteArray required_bytes = myfile.read(data_size_entered);

    current_offset = (page_start_offset + required_bytes.size());
    previous_entered_size = required_bytes.size();

    selected_data_size = required_bytes.size();

    setdata(required_bytes);

    myfile.close();

    uint64_t page_start = current_offset - previous_entered_size;

    if(page_start != 0)
    {
        emit signal_change_address_area_value(page_start-1);
    }
    else
    {
        emit signal_change_address_area_value(page_start);
    }

    current_page_count = enter_page_index;

    emit signal_current_page_details(current_page_count,(page_start),current_offset-1);

    if(current_page_count >= total_no_of_pages)
    {
        pushButton_next_goto_page->setEnabled(false);
        pushButton_prev_goto_page->setEnabled(true);
    }
    else if(current_page_count <= 1)
    {
        pushButton_prev_goto_page->setEnabled(false);
        pushButton_next_goto_page->setEnabled(true);
    }
    else
    {
        pushButton_prev_goto_page->setEnabled(true);
        pushButton_next_goto_page->setEnabled(true);
    }

    label_no_of_pages->setText(QString::number(enter_page_index) + " of " + QString::number(total_no_of_pages));

}

void hex_viewer::on_pushButton_go_to_page_clicked()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    set_palette_lineedit_goto_page(0);
    if(lineedit_goto_page->text().trimmed().isEmpty())
        return;

    qint64 enter_page_index = lineedit_goto_page->text().trimmed().toLongLong();

    if(bool_is_from_disk_viewer)
    {
        display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please wait...");
        display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
        display_loading_progress_bar_non_cancelable_obj->show();

        common_go_to_page_for_disk_hex_viewer(enter_page_index);


        display_loading_progress_bar_non_cancelable_obj->hide();
    }
    else
    {
        common_go_to_page(enter_page_index);
    }

    fill_the_existing_blocks_details_in_list();

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::on_pushButton_goto_offset_clicked()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    label_search_results->clear();
    lineEdit_search->clear();

    QString offset_str = lineEdit_go_to_offset->text();

    if(offset_str.isEmpty())
        return;

    if(offset_str.contains(" "))
        offset_str.replace(" ","");

    QString current_type = comboBox_go_to_offset_type->currentText();

    qint64 offset = 0;
    if(current_type == "DECIMAL")
    {
        offset_str = offset_str.toLower();

        bool is_decimal = is_this_decimal(offset_str);

        if(!is_decimal)
        {
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert valid decimal offset");
            message_dialog_obj->show();
            return;
        }

        offset = offset_str.toLongLong();
    }
    else if(current_type == "HEX")
    {
        offset_str = offset_str.toLower();

        bool is_this_hex = is_this_val_hex(offset_str);

        if(!is_this_hex)
        {
            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert valid hex offset");
            message_dialog_obj->show();
            return;
        }

        offset = offset_str.toLongLong(0,16);
    }

    if(offset < 0 || offset >= total_file_size)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert valid offset");
        message_dialog_obj->show();
        return;
    }

    if(bool_is_from_disk_viewer)
    {
        display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please wait...");
        display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
        display_loading_progress_bar_non_cancelable_obj->show();

        go_to_offset_for_disk_hex_viewer(offset);

        display_loading_progress_bar_non_cancelable_obj->hide();
    }
    //    else if(bool_disk_viewer_unallocated_disk)
    //    {

    //    }
    else
    {
        go_to_offset_for_file(offset);
    }

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::go_to_offset_for_file(qint64 offset)
{
    qint64 page_size = spinBox_page_size->text().toLongLong();

    int page_no = offset / page_size;
    uint64_t real_offset = offset % page_size;

    uint64_t start_offset = 0;
    if(page_no > 0)
        start_offset = page_size * page_no;
    else
        start_offset = 0;


    QFile myfile(file_path);
    if(!myfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + myfile.errorString(),Q_FUNC_INFO);
        return;
    }
    myfile.seek(start_offset);


    current_page_count = page_no + 1;

    uint64_t end_offset = total_file_size - start_offset;

    QByteArray selected_page_size_bytes;
    if(end_offset < page_size)
    {
        selected_page_size_bytes = myfile.read(end_offset);
        setdata(selected_page_size_bytes);
        current_offset = start_offset + end_offset;
        previous_entered_size = end_offset;
    }
    else
    {
        selected_page_size_bytes = myfile.read(page_size);
        setdata(selected_page_size_bytes);
        current_offset = start_offset + page_size;
        previous_entered_size = page_size;
    }

    if((myfile.size() % page_size) == 0)
    {
        label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages));
        total_no_of_pages = tmp_no_of_pages;
    }
    else
    {
        label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages + 1));
        total_no_of_pages = tmp_no_of_pages + 1;
    }

    if(current_page_count >= total_no_of_pages)
    {
        pushButton_next_goto_page->setEnabled(false);
        pushButton_prev_goto_page->setEnabled(true);
    }
    else if(current_page_count <= 1)
    {
        pushButton_prev_goto_page->setEnabled(false);
        pushButton_next_goto_page->setEnabled(true);
    }
    else
    {
        pushButton_prev_goto_page->setEnabled(true);
        pushButton_next_goto_page->setEnabled(true);
    }

    if(start_offset != 0)
        emit signal_change_address_area_value(start_offset-1);
    else
        emit signal_change_address_area_value(start_offset);

    emit go_to_offset(real_offset, selected_page_size_bytes.size());

    emit signal_current_page_details(current_page_count,start_offset,current_offset-1);

    myfile.close();

}


void hex_viewer::selct_bytes_offsets(int select_start, int select_end, bool selection)
{
    if(lineEdit_search->text().isEmpty())
    {
        label_search_results->setText("");
        index_list_size = 0;
    }

    if(total_file_size == 0)
        return;

    if(select_end >= total_file_size)
        select_end = total_file_size;

    uint selected_bytes = select_end - select_start;

    if(select_start > (selected_data_size - 1) || select_end > (selected_data_size -1))
    {
        return;
    }

    if(selection && selection_button_checked)
    {
        QString start_hex;
        start_hex.setNum(select_start,16);
        start_hex.prepend("0x");

        QString end_hex;
        end_hex.setNum(select_end,16);
        end_hex.prepend("0x");

        QString select_hex;
        select_hex.setNum(selected_bytes + 1,16);
        select_hex.prepend("0x");

        QString selected_bytes_str = start_hex + " to " + end_hex + " : " + select_hex + " Byte(s)";

        pushButton_selection_info_byte_count->setText(selected_bytes_str);
    }
    else if(selection && !selection_button_checked)
    {
        QString selected_decimal = QString::number(select_start) + " to " + QString::number(select_end) + " : "
                + QString::number(selected_bytes + 1) + " Byte(s)";

        pushButton_selection_info_byte_count->setText(selected_decimal);
    }
    else
    {
        pushButton_selection_info_byte_count->setText("");
    }
}

void hex_viewer::current_offset_cursor(int offset)
{
    uint64_t offset_uint = (current_offset - previous_entered_size) + offset;

    if(total_file_size == 0)
        return;

    qint64 entered_page_size = selected_data_size;

    if(offset_button_checked)
    {
        if(entered_page_size < total_file_size)
        {
            QString offset_hex;
            offset_hex.setNum(offset_uint,16);
            offset_hex.prepend("0x");

            QString page_size_hex;
            page_size_hex.setNum(total_file_size - 1,16);
            page_size_hex.prepend("0x");

            QString text_display = offset_hex + " / " + page_size_hex;
            pushButton_selection_info_offset->setText(text_display);
        }
        else
        {
            QString offset_hex;
            offset_hex.setNum(offset_uint,16);
            offset_hex.prepend("0x");

            QString page_size_hex;
            page_size_hex.setNum(total_file_size - 1,16);
            page_size_hex.prepend("0x");

            QString text_display = offset_hex + " / " + page_size_hex;
            pushButton_selection_info_offset->setText(text_display);
        }
    }
    else
    {
        if(entered_page_size < total_file_size)
        {
            QString text_display = QString::number(offset_uint) + " / " + QString::number(total_file_size - 1);
            pushButton_selection_info_offset->setText(text_display);
        }
        else
        {
            QString text_display = QString::number(offset_uint) + " / " + QString::number(total_file_size - 1);
            pushButton_selection_info_offset->setText(text_display);
        }
    }
}


void hex_viewer::on_pushButton_selection_clicked(bool checked)
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    selection_button_checked = !checked;

    QString pushbutton_string = pushButton_selection_info_byte_count->text();

    QStringList tmp_list = pushbutton_string.split(" ");

    if(tmp_list.size() > 5)
    {
        QString start_index     = tmp_list.at(0);
        QString end_index       = tmp_list.at(2);
        QString selected_bytes  = tmp_list.at(4);

        if(start_index.startsWith("0x"))
        {
            start_index = QString::number(start_index.toUInt(0,16));
            end_index   = QString::number(end_index.toUInt(0,16));
            selected_bytes = QString::number(selected_bytes.toUInt(0,16));

            QString selected_decimal = start_index + " to " + end_index + " : " + selected_bytes + " Byte(s)";

            pushButton_selection_info_byte_count->setText(selected_decimal);
        }
        else
        {
            QString start_hex;
            start_hex.setNum(start_index.toUInt(),16);
            start_hex.prepend("0x");

            QString end_hex;
            end_hex.setNum(end_index.toUInt(),16);
            end_hex.prepend("0x");

            QString select_hex;
            select_hex.setNum(selected_bytes.toUInt(),16);
            select_hex.prepend("0x");

            QString selected_bytes_str = start_hex + " to " + end_hex + " : " + select_hex + " Byte(s)";

            pushButton_selection_info_byte_count->setText(selected_bytes_str);
        }
    }

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::on_pushButton_offset_clicked(bool checked)
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    offset_button_checked = !checked;

    QString pushbutton_string = pushButton_selection_info_offset->text();

    QStringList tmp_list = pushbutton_string.split(" ");

    if(tmp_list.size() > 2)
    {
        QString offset          = tmp_list.at(0);
        QString total_bytes     = tmp_list.at(2);

        if(offset.startsWith("0x"))
        {
            offset = QString::number(offset.toULongLong(0,16));
            total_bytes = QString::number(total_bytes.toULongLong(0,16));

            QString text_display = offset + " / " + total_bytes;
            pushButton_selection_info_offset->setText(text_display);
        }
        else
        {
            QString offset_hex;
            offset_hex.setNum(offset.toULongLong(),16);
            offset_hex.prepend("0x");

            QString page_size_hex;
            page_size_hex.setNum(total_bytes.toULongLong(),16);
            page_size_hex.prepend("0x");

            QString text_display = offset_hex + " / " + page_size_hex;
            pushButton_selection_info_offset->setText(text_display);
        }
    }

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::on_spinBox_page_size_valueChanged(const QString &arg1)
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    qint64 page_size = arg1.toLongLong();

    QFileInfo info_file(file_path);
    if(info_file.exists())
    {
        tmp_no_of_pages = info_file.size() / page_size;

        if((info_file.size() % page_size) == 0)
        {
            label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages));
            total_no_of_pages = tmp_no_of_pages;
        }
        else
        {
            label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages + 1));
            total_no_of_pages = tmp_no_of_pages + 1;
        }
    }

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);

}
void hex_viewer::set_palette_lineedit_goto_page(int pp)
{
    QPalette palette;

    if(pp == 0)
        palette.setColor(QPalette::Text,Qt::black);
    else
        palette.setColor(QPalette::Text,Qt::red);

    lineedit_goto_page->setPalette(palette);

}

void hex_viewer::slot_line_edit_go_to_page_text_changed(QString text)
{
    set_palette_lineedit_goto_page(0);
    return;
}

void hex_viewer::slot_open_in_other_recon_viewers_clicked()
{
    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(block_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug( destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);
    QString command = "SELECT plugin_name FROM 'hex_viewer_index' WHERE viewer_display_name=?";
    QStringList arg_list;
    arg_list << hex_viewer_display_name;
    QString tmp_plugin_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,arg_list,0,destination_db,Q_FUNC_INFO);

    command = "SELECT category_name FROM 'hex_viewer_index' WHERE viewer_display_name=?";
    QString tmp_category_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,arg_list,0,destination_db,Q_FUNC_INFO);

    command = "SELECT record_no FROM 'hex_viewer_index' WHERE viewer_display_name=?";
    QString tmp_record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,arg_list,0,destination_db,Q_FUNC_INFO);

    destination_db.close();

    if(recon_static_functions::check_is_it_plist_file_by_signature(file_path, Q_FUNC_INFO))
    {
        QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Plist_viewer_In_Result_QString).toString();

        QStringList list_plist_display_name = recon_helper_standard_obj->get_plist_displayname_list_from_result_db(sqlite_path, Q_FUNC_INFO);
        if(list_plist_display_name.contains(hex_viewer_display_name))
        {
            emit signal_open_file_in_other_recon_viewers_from_hex_viewer(hex_viewer_display_name,MACRO_CASE_TREE_PLIST_VIEWER,source_count_name);
            destination_db.close();
            return;
        }

        plist_viewer *plist_viewer_obj = new plist_viewer(this);
        QString plist_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + tmp_plugin_name + MACRO_RECON_Splitter_1_multiple + tmp_category_name + MACRO_RECON_Splitter_1_multiple + tmp_record_no;

        plist_viewer_obj->pub_set_resultsqlite_path(sqlite_path);
        plist_viewer_obj->pub_set_plist_viewer_name(plist_view_table_name);
        plist_viewer_obj->pub_set_plist_path(file_path);
        plist_viewer_obj->pub_set_source_count_name(source_count_name);
        plist_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
        plist_viewer_obj->pub_set_essentials();

        plist_viewer_obj->pub_start_plist_extraction();

        emit signal_open_file_in_other_recon_viewers_from_hex_viewer(hex_viewer_display_name,MACRO_CASE_TREE_PLIST_VIEWER,source_count_name);
    }
    else if(recon_static_functions::check_is_it_sqlite_file_by_signature(file_path, Q_FUNC_INFO))
    {
        QString sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

        QStringList list_sqlite_display_name = recon_helper_standard_obj->get_sqlite_namelist_from_db(sqlite_path, Q_FUNC_INFO);
        if(list_sqlite_display_name.contains(hex_viewer_display_name))
        {
            emit signal_open_file_in_other_recon_viewers_from_hex_viewer(hex_viewer_display_name,MACRO_CASE_TREE_SQLITE_VIEWER,source_count_name);
            destination_db.close();
            return;
        }

        QString sqlite_view_table_name = display_file_name + MACRO_RECON_Splitter_1_multiple + tmp_plugin_name + MACRO_RECON_Splitter_1_multiple + tmp_category_name + MACRO_RECON_Splitter_1_multiple + tmp_record_no;

        sqlite_viewer_starter *sqlite_viewer_starter_obj = new sqlite_viewer_starter(this);
        sqlite_viewer_starter_obj->pub_set_sqlite_viewer_name(sqlite_view_table_name);
        sqlite_viewer_starter_obj->pub_set_outsqlite_path(sqlite_path);
        sqlite_viewer_starter_obj->pub_set_recon_result_dir_path(recon_result_dir_path);
        sqlite_viewer_starter_obj->pub_set_sqlite_path(file_path);
        sqlite_viewer_starter_obj->pub_set_source_count_name(source_count_name);
        sqlite_viewer_starter_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
        sqlite_viewer_starter_obj->pub_set_essentials();

        sqlite_viewer_starter_obj->pub_start_sqlite_extraction(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

        emit signal_open_file_in_other_recon_viewers_from_hex_viewer(hex_viewer_display_name,MACRO_CASE_TREE_SQLITE_VIEWER,source_count_name);
    }
}

bool hex_viewer::is_this_decimal(QString value_str)
{
    for(int i = 0; i < value_str.size(); i++)
    {
        QChar ch = value_str.at(i);

        int ascii = ch.toLatin1();

        if(ascii < 48 || ascii > 57)
        {
            return false;
        }
    }

    return true;
}

bool hex_viewer::is_this_val_hex(QString value_str)
{
    for(int i = 0; i < value_str.size(); i++)
    {
        QChar ch = value_str.at(i);

        int ascii = ch.toLatin1();

        if((ascii < 48 || ascii > 57) && (ascii < 97 || ascii > 102))
        {
            return false;
        }
    }

    return true;
}

void hex_viewer::pub_set_plugin_name(QString plugn_nm)
{
    plugin_name_str  = plugn_nm;
}

void hex_viewer::pub_set_tab_name(QString tab_nm)
{
    tab_name_str  = tab_nm;
}

void hex_viewer::pub_set_record_no(QString recrd_no)
{
    record_no = recrd_no;
}
