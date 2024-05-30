#include "full_detailed_info.h"
#include "ui_full_detailed_info.h"

full_detail_information::full_detail_information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::full_detail_information)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    exif_interface_obj = new exif_data_interface(this);

    strings_viewer_obj =  new strings_viewer(this);
    text_viewer_aascii_unicode_obj = new text_viewer_aascii_unicode(this);

    maps_metadata_obj = new offline_maps(this);
    connect(maps_metadata_obj, SIGNAL(signal_add_saved_maps_in_case_tree()), this, SIGNAL(signal_add_saved_maps_in_case_tree()));
    maps_metadata_obj->setMinimumHeight(40);
    maps_metadata_obj->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);

    recon_helper_standard_obj = new recon_helper_standard(this);


}

full_detail_information::~full_detail_information()
{
    delete ui;
}

void full_detail_information::closeEvent(QCloseEvent *)
{
    // delete this;
}

void full_detail_information::pub_set_recon_file_info(struct_global_recon_file_info obj)
{
    st_globl_recon_file_info_obj = obj;
}

void full_detail_information::pub_set_essentials()
{
    result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
}

void full_detail_information::pub_set_global_connaction_manager_obj(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
}

void full_detail_information::pub_set_metadata_values_obj(struct_global_set_metadata_info obj)
{
    st_metadata_values_obj = obj;
}

void full_detail_information::pub_set_hex_viewer_block_db_path(QString db_path)
{
    hex_viewer_block_db_path = db_path;
}

void full_detail_information::pub_show_full_detail_info(QString metadata_value, QString preview_path55, int preview_index,QString searched_keyword, QString source_count_name, QString record_number, QString plugin_name, QString table_name55)
{ // show full details info metadata of any records
    QString meta_file_path = st_globl_recon_file_info_obj.complete_file_path;
    QString preview_path = st_globl_recon_file_info_obj.complete_file_path;
    QString table_name = st_globl_recon_file_info_obj.db_table_name;

    if(global_recon_helper_singular_class_obj->is_it_recon_custom_picture_plugin(plugin_name))
    {
        meta_file_path.clear();
    }

    if(st_globl_recon_file_info_obj.bool_file_from_result && plugin_name != MACRO_Plugin_Name_Carved_Files && plugin_name != MACRO_Plugin_Name_Hex_Viewer)
    {
        meta_file_path.clear();
    }

    source_count_name =  st_globl_recon_file_info_obj.source_count_name.trimmed();

    QSplitter *splitter_metadata_tabs = new QSplitter(Qt::Horizontal);
    splitter_metadata_tabs->setHandleWidth(0);

    m_textedit *text_edit_metadata_detach;
    m_stackedwidget *stackwidget_previewer_obj;
    image_preview *image_preview_obj;
    av_preview *av_preview_obj;
    hex_viewer_complete *hex_displyr_obj;

    text_edit_optical_character_recognition_metadata_obj = new m_textedit(this);

    text_edit_metadata_detach = new m_textedit(this);
    QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);

    QString hex_display_nm = display_file_name + " [" + st_metadata_values_obj.plugin_name + " " + st_metadata_values_obj.record_no + "]";

    struct_global_hex_viewer_open_file_data hex_viewer_open_file_data_obj;
    recon_static_functions::clear_variables_hex_viewer_open_file_data(hex_viewer_open_file_data_obj);

    hex_viewer_open_file_data_obj.bool_is_file_in_bucket  = false;
    hex_viewer_open_file_data_obj.hex_viewer_display_name = hex_display_nm;

    hex_viewer_open_file_data_obj.complete_file_path  = st_globl_recon_file_info_obj.complete_file_path;
    hex_viewer_open_file_data_obj.display_file_path   = st_globl_recon_file_info_obj.display_file_path;
    hex_viewer_open_file_data_obj.source_count_name   = st_globl_recon_file_info_obj.source_count_name;
    hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_file_from;
    hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path;

    hex_viewer_open_file_data_obj.plugin_name_str     = st_metadata_values_obj.plugin_name;
    hex_viewer_open_file_data_obj.plugin_tab_name_str = st_metadata_values_obj.tab_name;
    hex_viewer_open_file_data_obj.record_no_str       = st_metadata_values_obj.record_no;

    hex_viewer_open_file_data_obj.hex_feature_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

    hex_displyr_obj = new hex_viewer_complete(this);
    connect(hex_displyr_obj,SIGNAL(signal_save_openwith_changes_to_bucket(QString)),this,SIGNAL(signal_save_openwith_changes_to_bucket_full_detail_info(QString)));

    hex_displyr_obj->pub_downsize_hexviewer_for_metadata_display();
    hex_displyr_obj->pub_set_global_connection_manager(global_connection_manager_obj);
    hex_displyr_obj->pub_set_hex_viewer_open_file_data_obj(hex_viewer_open_file_data_obj);
    hex_displyr_obj->pub_set_source_file_size(QFile(meta_file_path).size());
    hex_displyr_obj->pub_set_source_block_size(512);
    hex_displyr_obj->pub_set_file_path(meta_file_path, hex_viewer_open_file_data_obj.display_file_path);


    blank *blank_obj;

    stackwidget_previewer_obj = new m_stackedwidget(this);
    blank_obj = new blank(this);
    av_preview_obj = new av_preview(this);
    image_preview_obj = new image_preview(this);

    stackwidget_previewer_obj->addWidget(blank_obj);
    stackwidget_previewer_obj->addWidget(image_preview_obj);
    stackwidget_previewer_obj->addWidget(av_preview_obj);

    stackwidget_previewer_obj->setCurrentIndex(0);

    av_preview_obj->stop_media();
    stackwidget_previewer_obj->setCurrentIndex(0);


    tree_exif_metadata_display = new m_treewidget(this);
    tree_exif_metadata_display->setColumnCount(2);
    connect(tree_exif_metadata_display, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(slot_tree_exif_metadata_item_clicked(QTreeWidgetItem*,int)));


    set_exif_data_in_full_window(tree_exif_metadata_display , source_count_name , meta_file_path, plugin_name, table_name, record_number,st_globl_recon_file_info_obj.display_file_path);


    tabwidget_metadata_viewer_full = new R_Tabwidget(this);

    tabwidget_metadata_viewer_full->setTabPosition(QTabWidget::TabPosition::South);
    tabwidget_metadata_viewer_full->setElideMode(Qt::ElideNone);
    tabwidget_metadata_viewer_full->setDocumentMode(true);

    tabwidget_metadata_viewer_full->addTab(text_edit_metadata_detach,QIcon("../icons/Features/detailed_info.png"), MACRO_Metadata_TabName_Detailed_Information);

    tabwidget_metadata_viewer_full->addTab(text_edit_optical_character_recognition_metadata_obj,QIcon("../icons/Features/optical_character_recognition.png"), MACRO_Metadata_TabName_Optical_Character_Recognition);
    connect(tabwidget_metadata_viewer_full, SIGNAL(currentChanged(int)), this, SLOT(slot_tabwidget_metadata_viewer_full_current_Changed(int)));

    tabwidget_metadata_viewer_full->addTab(hex_displyr_obj, QIcon("../icons/Features/hex_viewer.png"), MACRO_Metadata_TabName_Hex_View);


    ///==========Text Viewer==========///
    tabwidget_metadata_viewer_full->addTab(text_viewer_aascii_unicode_obj,QIcon("../icons/Features/text_viewer.png"),  MACRO_Metadata_TabName_Text_View);

    text_viewer_aascii_unicode_obj->pub_clear_textview();
    text_viewer_aascii_unicode_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);

    text_viewer_aascii_unicode_obj->pub_set_source_count_name(source_count_name);
    text_viewer_aascii_unicode_obj->pub_set_essentials();
    text_viewer_aascii_unicode_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    text_viewer_aascii_unicode_obj->pub_hide_search_navigate_buttons(false);

    text_viewer_aascii_unicode_obj->pub_load_file_in_text_view(meta_file_path, record_number);



    ///==========Strings Viewer==========///
    tabwidget_metadata_viewer_full->addTab(strings_viewer_obj,QIcon("../icons/Features/strings.png"),  MACRO_Metadata_TabName_Strings);

    strings_viewer_obj->pub_clear_data();
    strings_viewer_obj->pub_set_source_count_name(source_count_name);
    strings_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    strings_viewer_obj->pub_set_essentials();
    strings_viewer_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    strings_viewer_obj->pub_hide_search_navigate_buttons(false);

    strings_viewer_obj->pub_show_file_data(meta_file_path, record_number);


    tabwidget_metadata_viewer_full->addTab(tree_exif_metadata_display, QIcon("../icons/Features/exif_data.png"), QString(MACRO_Metadata_TabName_Exif_Meatadta));
    tabwidget_metadata_viewer_full->setTabsClosable(false);



    tree_apple_metadata = new m_treewidget(this);

    tabwidget_metadata_viewer_full->addTab(tree_apple_metadata, QIcon("../icons/Features/apple_metadata.png"), MACRO_Metadata_TabName_Apple_Meatadta);

    tree_apple_metadata->setColumnCount(6);

    QTreeWidgetItem* header_tree = tree_apple_metadata->headerItem();
    header_tree->setText(enum_full_detail_information_apple_metadata_KEY, "Attribute");
    header_tree->setText(enum_full_detail_information_apple_metadata_Value, "Value");
    header_tree->setText(enum_full_detail_information_apple_metadata_Plugin_Table_Name, "Plugin Table Name");
    header_tree->setText(enum_full_detail_information_apple_metadata_Plugin_Db_Path, "Plugin Db Path");
    header_tree->setText(enum_full_detail_information_apple_metadata_Plugin_Record_no, "Record no");
    header_tree->setText(enum_full_detail_information_apple_metadata_File_Path, "File Path");

    tree_apple_metadata->header()->resizeSection(0, 220);

    connect(tree_apple_metadata, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(slot_tree_apple_metadata_item_clicked(QTreeWidgetItem*,int)));

    QStringList path_list;
    path_list  << meta_file_path;

    change_apple_metadata_content_on_row_click(path_list, source_count_name, record_number, table_name, plugin_name, tree_apple_metadata);

    ///==========Offline Maps==========///
    tabwidget_metadata_viewer_full->addTab(maps_metadata_obj, QIcon("../icons/Features/maps.png"), QString(MACRO_Metadata_TabName_Maps));
    maps_metadata_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    maps_metadata_obj->pub_change_location(QString("") , QString(""),QString(""),QString(""));

    maps_metadata_obj->pub_change_location("" , "", meta_file_path, source_count_name);

    R_Tabwidget *tabwidget_av_and_images_previewer_full = new R_Tabwidget(this);
    tabwidget_av_and_images_previewer_full->setTabPosition(QTabWidget::TabPosition::South);
    tabwidget_av_and_images_previewer_full->setElideMode(Qt::ElideNone);
    tabwidget_av_and_images_previewer_full->setDocumentMode(true);
    tabwidget_av_and_images_previewer_full->addTab(stackwidget_previewer_obj,QIcon("../icons/Features/preview.png"),"Preview");
    tabwidget_av_and_images_previewer_full->setMinimumWidth(350);
    tabwidget_av_and_images_previewer_full->setTabsClosable(false);

    splitter_metadata_tabs->addWidget(tabwidget_metadata_viewer_full);
    splitter_metadata_tabs->addWidget(tabwidget_av_and_images_previewer_full);
    splitter_metadata_tabs->setContentsMargins(0,0,0,0);
    splitter_metadata_tabs->setStretchFactor(0,1);
    splitter_metadata_tabs->setStretchFactor(1,0);

    splitter_metadata_tabs->setCollapsible(0,false);
    splitter_metadata_tabs->setCollapsible(1,false);




    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addWidget(splitter_metadata_tabs);
    hlayout->setContentsMargins(6,6,6,6);

    setLayout(hlayout);
    setWindowTitle(recon_static_functions::get_app_name() + "- Full Viewer");

    text_edit_metadata_detach->setText(metadata_value);
    //    hex_viewer_obj->pub_display_hex_viewer_metadata();
    //    hex_viewer_obj->set_file_data(hex_file_path);

    QMimeDatabase mime_database_obj;
    QString file_mime_type = mime_database_obj.mimeTypeForFile(preview_path).name();
    QStringList mime_extensions_list = mime_database_obj.mimeTypeForFile(preview_path).suffixes();
    if(preview_index == 0)
    {
        stackwidget_previewer_obj->setCurrentIndex(preview_index);
    }
    else if(preview_index == 1)
    {
        QString suffix = recon_helper_standard_obj->mining_suffix(preview_path, Q_FUNC_INFO);
        suffix = suffix.trimmed();
        if(suffix.isEmpty())
        {
            if(mime_extensions_list.size() > 0)
            {
                suffix = mime_extensions_list.at(0);
                suffix.prepend(".");
            }
            preview_path = create_link_path_for_preview(suffix,preview_path);
        }

        image_preview_obj->set_image_by_path(preview_path);
        stackwidget_previewer_obj->setCurrentIndex(preview_index);
    }
    else if( preview_index == 2)
    {
        stackwidget_previewer_obj->setCurrentIndex(preview_index);
        //av_preview_obj->load_source(preview_path);
        if(file_mime_type.startsWith("video/"))
        {
            QString suffix = recon_helper_standard_obj->mining_suffix(preview_path, Q_FUNC_INFO);
            suffix = suffix.trimmed();
            if(suffix.isEmpty())
            {
                if(mime_extensions_list.size() > 0)
                {
                    suffix = mime_extensions_list.at(0);
                    suffix.prepend(".");
                }
                preview_path = create_link_path_for_preview(suffix,preview_path);
            }

            av_preview_obj->load_source(preview_path);
        }
        else if(file_mime_type.startsWith("audio/"))
        {
            QString suffix = recon_helper_standard_obj->mining_suffix(preview_path, Q_FUNC_INFO);
            suffix = suffix.trimmed();
            if(suffix.isEmpty())
            {
                if(mime_extensions_list.size() > 0)
                {
                    suffix = mime_extensions_list.at(0);
                    suffix.prepend(".");
                }
                preview_path = create_link_path_for_preview(suffix,preview_path);
            }

            av_preview_obj->load_source(preview_path);
        }
    }

    searched_keyword = searched_keyword.trimmed();

    if(!searched_keyword.isEmpty())
    {
        QStringList patternlist;

        if(searched_keyword.contains(","))
        {
            QStringList keyword_list;
            keyword_list = searched_keyword.split(",", Qt::SkipEmptyParts);

            for(int i = 0; i < keyword_list.size(); i++)
            {
                QString temp_string = keyword_list.at(i);
                temp_string = temp_string.trimmed();
                if(temp_string == "")
                {
                    continue;
                }

                patternlist.append(temp_string);
            }
        }
        else
            patternlist.append(searched_keyword);

        QTextCharFormat fmt;
        fmt.setBackground(QColor("#D59B3F"));


        QTextCursor cursor(text_edit_metadata_detach->document());

        QString text = text_edit_metadata_detach->toPlainText().toLower();

        for(int i = 0; i < patternlist.size(); i++)
        {
            QString pattern = patternlist.at(i);
            cursor.setCharFormat(fmt);
            int length = pattern.size();

            int index = 0;
            while (index >= 0)
            {

                index = text.indexOf(pattern.toLower(), index);
                if(index < 0)
                {
                    break;
                }

                cursor.setPosition(index, QTextCursor::MoveAnchor);
                cursor.setPosition(index + length, QTextCursor::KeepAnchor);

                index = index + length;

                cursor.setCharFormat(fmt);
            }
        }
    }
}

QString full_detail_information::create_link_path_for_preview(QString extension_str, QString file_path)
{ // create link path for preview to view any vieweable file from metadata
    QString link_file_path;
    QString link_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Preview_Links_In_Result_QString).toString();

    bool dir_exist = false;
    QDir link_dir(link_dir_path);
    if(!link_dir.exists())
    {
        dir_exist = link_dir.mkpath(link_dir_path);
    }
    else
        dir_exist = true;

    if(dir_exist)
    {
        link_file_path = link_dir_path + QFileInfo(file_path).fileName() + extension_str;

        if(QFileInfo(link_file_path).exists())
            QFile::remove(link_file_path);

        QFile::link(file_path, link_file_path);
    }
    else
        link_file_path = file_path;

    return link_file_path;
}

void full_detail_information::set_exif_data_in_full_window(m_treewidget *tree_exif_metadata_display, QString source_count_name, QString filepath, QString plugin_name, QString table_name , QString record_no_str,QString display_file_path)
{ // set exif data in full window
    if(filepath.trimmed().isEmpty())
        return ;

    if(global_recon_helper_singular_class_obj->is_it_recon_custom_picture_plugin(plugin_name))
        return;


    QString db_path = st_globl_recon_file_info_obj.db_path;

    exif_interface_obj->pub_set_result_dir(result_dir);
    exif_interface_obj->pub_set_exif_data_in_case_tree_display(filepath,record_no_str , db_path , tree_exif_metadata_display, plugin_name, table_name, source_count_name,display_file_path);

    return;
}


void full_detail_information::slot_tree_apple_metadata_item_clicked(QTreeWidgetItem *item, int column)
{ // tree of apple metadata item clicked from metadata
    QString tablename = item->text(enum_full_detail_information_apple_metadata_Plugin_Table_Name);
    QString db_path = item->text(enum_full_detail_information_apple_metadata_Plugin_Db_Path);
    QString rec_no = item->text(enum_full_detail_information_apple_metadata_Plugin_Record_no);
    QString key = item->text(enum_full_detail_information_apple_metadata_KEY);
    QString value = item->text(enum_full_detail_information_apple_metadata_Value);

    if(!QFileInfo(db_path).exists())
        return ;


    QString command_select = "select  bookmarked_extended_attributes from '" + tablename + "' where INT = ?";
    QStringList Int_value;
    Int_value << rec_no;
    QString stored_ext_attr = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select, Int_value, 0, db_path, Q_FUNC_INFO);

    if(!stored_ext_attr.endsWith("\n"))
        stored_ext_attr.append("\n");

    QString apl_metadata_str = QString(key + " : " + value + "\n");

    if(!item->checkState(enum_full_detail_information_apple_metadata_KEY))
    {
        if(stored_ext_attr.contains(apl_metadata_str))
        {
            stored_ext_attr.remove(apl_metadata_str);

            QString command = "update '" + tablename + "' set bookmarked_extended_attributes = ? where INT = ?";
            QStringList list;

            list << stored_ext_attr << rec_no;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,db_path, Q_FUNC_INFO);

        }
    }else
    {
        if(!stored_ext_attr.contains(apl_metadata_str))
        {
            stored_ext_attr +=  QString(key + " : " + value + "\n");

            qint64 child_count = item->childCount();
            for(int mm = 0; mm < child_count; mm++)
            {
                stored_ext_attr += item->child(mm)->text( enum_full_detail_information_apple_metadata_Value) + "\n";
            }

            QString command = "update '" + tablename + "' set bookmarked_extended_attributes = ? where INT = ?";
            QStringList list;
            list << stored_ext_attr << rec_no;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,db_path, Q_FUNC_INFO);

        }
    }

    QString bookmark_status = QString(MACRO_FALSE_VALUE_NUMERIC_QString);

    bool check_status = check_any_exif_or_apple_metadata_item_selected();
    if(check_status)
        bookmark_status = QString(MACRO_TRUE_VALUE_NUMERIC_QString);

    emit signal_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked(rec_no, bookmark_status);

}

void full_detail_information::slot_tabwidget_metadata_viewer_full_current_Changed(int index)
{ // metadata viewer like OCR data
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    if(tabwidget_metadata_viewer_full->tabText(index) == MACRO_Metadata_TabName_Optical_Character_Recognition)
    {
        QString source_count_name  = st_metadata_values_obj.source_count_name;
        QString record_no = st_metadata_values_obj.record_no;
        QString tab_name = st_metadata_values_obj.tab_name;

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

        QString metadata_str;

        QString cmd_ocr = "SELECT ocr_text from files WHERE INT = ?";
        QStringList bindValue;
        bindValue << record_no;

        QString ocr_text = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_ocr, bindValue, 0, fs_db_path, Q_FUNC_INFO);
        if(!ocr_text.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("OCR Text") + ":</b>" +QString("\t") + ocr_text + "\n<br>";
            metadata_str += "<br>";
        }

        text_edit_optical_character_recognition_metadata_obj->clear();
        text_edit_optical_character_recognition_metadata_obj->append(metadata_str);
        text_edit_optical_character_recognition_metadata_obj->moveCursor(QTextCursor::Start);

    }
    recon_static_functions::app_debug(" Ends" , Q_FUNC_INFO);
}

void full_detail_information::change_apple_metadata_content_on_row_click(QStringList file_path_list, QString source_count_name,QString record_no, QString table_name, QString plugin_name, m_treewidget *tree_metadata_apple_metadata)
{ // change metadata tab on cclicked and get the index of it
    recon_static_functions::app_debug("---Starts----",Q_FUNC_INFO);


    if(global_recon_helper_singular_class_obj->is_it_recon_custom_picture_plugin(plugin_name))
    {
        tree_metadata_apple_metadata->clear();
        return ;
    }
    else if(plugin_name == MACRO_Plugin_Name_Carved_Files)
    {
        tree_metadata_apple_metadata->clear();
        return ;
    }
    else if(st_globl_recon_file_info_obj.recon_file_from.toInt() == enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result)
    {
        tree_metadata_apple_metadata->clear();
        return;
    }

    QString src_cnt_name = st_globl_recon_file_info_obj.source_count_name;
    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(src_cnt_name);

    if(struct_source_info.file_system_type != MACRO_FILE_SYSTEM_TYPE_HFS
            && struct_source_info.file_system_type != MACRO_FILE_SYSTEM_TYPE_APFS)
    {
        if(struct_source_info.source_type_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal)
        {
            tree_metadata_apple_metadata->clear();
            return;
        }
    }

    file_path_list.removeAll(QString(""));

    if(file_path_list.isEmpty() || plugin_name.trimmed().isEmpty())
    {
        tree_metadata_apple_metadata->clear();
        return;
    }

    tree_metadata_apple_metadata->clear();

    QString  destination_db_path = st_globl_recon_file_info_obj.db_path;


    QString command = "select bookmarked_extended_attributes from '" + table_name + "' where INT = ?";
    QStringList list;
    list << record_no;
    QString stored_apl_attributes;
    if(QFileInfo(destination_db_path).exists())
        stored_apl_attributes = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command , list, 0, destination_db_path, Q_FUNC_INFO);


    QString file_system_apple_metadata = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Metadata_QString).toString();
    QString tmp_apple_timestamps_db_path = file_system_apple_metadata + struct_source_info.source_count_name + QString("/apple_metadata.sqlite");
    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    for(int file_count = 0; file_count < file_path_list.size(); file_count++)
    {
        QString file_path = file_path_list.at(file_count);

        QStringList ext_attribute_list;
        struct_global_desktop_info pwd_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

        QString disp_filepath = st_globl_recon_file_info_obj.display_file_path;
        ext_attribute_list = recon_helper_standard_obj->get_apple_metadata_from_result_apple_metadata_db(disp_filepath,struct_source_info.source_count_name);

        if(ext_attribute_list.isEmpty())
            continue;

        file_path = st_globl_recon_file_info_obj.display_file_path;

        QString m_file_name = QFileInfo(st_globl_recon_file_info_obj.display_file_path).fileName();

        QTreeWidgetItem *root_item = new QTreeWidgetItem();
        root_item->setText(enum_full_detail_information_apple_metadata_KEY, m_file_name);
        root_item->setText(enum_full_detail_information_apple_metadata_Value, "");
        tree_metadata_apple_metadata->addTopLevelItem(root_item);

        for(int i = 0 ; i < ext_attribute_list.size() ; i++)
        {
            QStringList keyvalue_list = ext_attribute_list.at(i).split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

            QString key,value;
            if(keyvalue_list.size() > 1)
            {
                key = keyvalue_list.at(0);
                value = keyvalue_list.at(1);
            }

            if(value.trimmed().isEmpty())
                continue;


            QString check_already_selected = QString(key + " :");


            QTreeWidgetItem *tree_child = new QTreeWidgetItem();
            if(stored_apl_attributes.contains(check_already_selected))
                tree_child->setCheckState(enum_full_detail_information_apple_metadata_KEY,Qt::Checked);
            else
                tree_child->setCheckState(enum_full_detail_information_apple_metadata_KEY,Qt::Unchecked);

            tree_child->setText(enum_full_detail_information_apple_metadata_KEY,key);

            if(value.contains("\n"))
            {
                QStringList sub_values_list = value.split("\n", Qt::SkipEmptyParts);

                int count = 0;
                while(count < sub_values_list.size())
                {
                    QString sub_value = sub_values_list.at(count);

                    if(count == 0)
                    {
                        tree_child->setText(enum_full_detail_information_apple_metadata_Value, sub_value);
                        tree_child->setText(enum_full_detail_information_apple_metadata_Plugin_Table_Name, table_name);
                        tree_child->setText(enum_full_detail_information_apple_metadata_Plugin_Db_Path, destination_db_path);
                        tree_child->setText(enum_full_detail_information_apple_metadata_Plugin_Record_no, record_no);
                        tree_child->setText(enum_full_detail_information_apple_metadata_File_Path, file_path);


                        root_item->addChild(tree_child);
                    }
                    else
                    {
                        QTreeWidgetItem *tree_sub_child = new QTreeWidgetItem;
                        tree_sub_child->setText( enum_full_detail_information_apple_metadata_Value, sub_value);

                        tree_child->addChild(tree_sub_child);

                    }
                    count++;
                }
            }
            else
            {

                tree_child->setText(enum_full_detail_information_apple_metadata_Value, value);
                tree_child->setText(enum_full_detail_information_apple_metadata_Plugin_Table_Name, table_name);
                tree_child->setText(enum_full_detail_information_apple_metadata_Plugin_Db_Path, destination_db_path);
                tree_child->setText(enum_full_detail_information_apple_metadata_Plugin_Record_no, record_no);
                tree_child->setText(enum_full_detail_information_apple_metadata_File_Path, file_path);

                root_item->addChild(tree_child);
            }

        }


        break;///so that we need only first file to get extended attributes in case of multiple files attach with single record
    }

    tree_metadata_apple_metadata->expandAll();

    tree_metadata_apple_metadata->setColumnWidth(enum_full_detail_information_apple_metadata_KEY,300);
    tree_metadata_apple_metadata->setColumnWidth(enum_full_detail_information_apple_metadata_Value,400);
    tree_metadata_apple_metadata->setColumnHidden(enum_full_detail_information_apple_metadata_Plugin_Table_Name,true);
    tree_metadata_apple_metadata->setColumnHidden(enum_full_detail_information_apple_metadata_Plugin_Db_Path,true);
    tree_metadata_apple_metadata->setColumnHidden(enum_full_detail_information_apple_metadata_Plugin_Record_no,true);
    tree_metadata_apple_metadata->setColumnHidden(enum_full_detail_information_apple_metadata_File_Path,true);


    recon_static_functions::app_debug("---Ends----",Q_FUNC_INFO);

}


bool full_detail_information::check_any_exif_or_apple_metadata_item_selected()
{ // exif or apple metadata clicked to get the proper index of or position of it for further use
    bool check_status = false;

    if(tree_exif_metadata_display != nullptr)
    {
        for(int count = 0; count < tree_exif_metadata_display->topLevelItemCount(); count++)
        {
            QTreeWidgetItem *child_item = tree_exif_metadata_display->topLevelItem(count);
            if(child_item != nullptr)
            {
                if(child_item->checkState(0) == Qt::Checked)
                {
                    check_status = true;
                    break;
                }
            }
        }
    }


    if(check_status)
        return check_status;

    if(tree_apple_metadata != nullptr)
    {
        for(int pp = 0; pp < tree_apple_metadata->topLevelItemCount(); pp++)
        {
            QTreeWidgetItem *root_item = tree_apple_metadata->topLevelItem(pp);
            if(root_item != nullptr)
            {
                for(int cc = 0; cc < root_item->childCount(); cc++)
                {
                    QTreeWidgetItem *child_item = root_item->child(cc);
                    if(child_item != nullptr)
                    {
                        if(child_item->checkState(0) == Qt::Checked)
                        {
                            check_status = true;
                            break;
                        }
                    }
                }
            }
        }
    }


    return check_status;
}

QStringList full_detail_information::get_apple_metadata_key_value_list_from_fs_db(QString file_path, QString source_count_name)
{ // apple metadata attributes list fron main file system database to set them on metadata viewer
    QStringList ext_attribute_list;

    QString case_config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString command = "select custom_column_name from tbl_apple_attributes";

    QStringList list_ext_attributes_column =  recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, case_config_db_path, Q_FUNC_INFO);

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(fs_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" db open---FAILED----" + fs_db_path, Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(), Q_FUNC_INFO);
        return ext_attribute_list;
    }


    QStringList values_list;
    QSqlQuery query(destination_db);


    query.prepare("select " + list_ext_attributes_column.join(",") + " from files where filepath = ?");
    query.addBindValue(file_path);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query execution ----FAILED-----" + fs_db_path, Q_FUNC_INFO);
        recon_static_functions::app_debug("---Error-----" + query.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Command " + query.executedQuery(), Q_FUNC_INFO);
        return ext_attribute_list;
    }

    while(query.next())
    {
        for(int i = 0 ; i < list_ext_attributes_column.size() ; i++)
        {
            values_list << query.value(i).toString();
        }

    }

    destination_db.close();

    if(list_ext_attributes_column.size() != values_list.size())
        return ext_attribute_list;


    for(int i = 0 ; i < list_ext_attributes_column.size() ; i++)
    {
        QString key = list_ext_attributes_column.at(i);
        QString value = values_list.at(i);

        key.remove(key.indexOf("_"),key.size());

        if(value.trimmed().isEmpty())
            continue;

        ext_attribute_list << QString(key + MACRO_RECON_Splitter_1_multiple + value);
    }


    return ext_attribute_list;
}

void full_detail_information::slot_tree_exif_metadata_item_clicked(QTreeWidgetItem *item, int m_column)
{ // exif metadata item clicked and set there exif necessary things

    Q_UNUSED(m_column);

    if(item->text(enum_global_exif_data_interface_exif_data_KEY).isEmpty())
        return;

    QString source_count_name    = item->text(enum_global_exif_data_interface_exif_data_Plugin_Source_count_name).trimmed();

    QString tablename = item->text(enum_global_exif_data_interface_exif_data_Plugin_Table_Name);
    QString db_path   = item->text(enum_global_exif_data_interface_exif_data_Plugin_Db_Path);
    QString rec_no    = item->text(enum_global_exif_data_interface_exif_data_Plugin_Record_no);
    QString key   = item->text(enum_global_exif_data_interface_exif_data_KEY);
    QString value = item->text(enum_global_exif_data_interface_exif_data_Value);


    QString command_select = "select  bookmarked_exif_data from '" + tablename + "' where INT = ?";
    QStringList Int_value;
    Int_value << rec_no;
    QString stored_exif_data = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select, Int_value, 0, db_path, Q_FUNC_INFO);

    if(!stored_exif_data.endsWith("\n"))
        stored_exif_data.append("\n");

    QString exif_metadata_str = QString(key + " : " + value + "\n");

    if(!item->checkState(enum_global_exif_data_interface_exif_data_KEY))
    {
        if(stored_exif_data.contains(exif_metadata_str))
        {
            stored_exif_data.remove(exif_metadata_str);

            QString command = "update '" + tablename + "' set bookmarked_exif_data = ? where INT = ?";
            QStringList list;

            list << stored_exif_data << rec_no;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,db_path, Q_FUNC_INFO);
        }
    }
    else
    {

        if(!stored_exif_data.contains(exif_metadata_str))
        {
            stored_exif_data +=  QString(key + " : " + value + "\n");

            qint64 child_count = item->childCount();
            for(int mm = 0; mm < child_count; mm++)
            {
                stored_exif_data += item->child(mm)->text(enum_global_exif_data_interface_exif_data_Value).trimmed() + "\n";
            }

            QString command = "update '" + tablename + "' set bookmarked_exif_data = ? where INT = ?";
            QStringList list;
            list << stored_exif_data << rec_no;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,db_path, Q_FUNC_INFO);
        }
    }

    QString bookmark_status = QString(MACRO_FALSE_VALUE_NUMERIC_QString);

    bool check_status = check_any_exif_or_apple_metadata_item_selected();

    if(check_status)
        bookmark_status = QString(MACRO_TRUE_VALUE_NUMERIC_QString);

    emit signal_handle_bookmark_on_exif_or_apple_metdata_tree_item_clicked(rec_no, bookmark_status);

}

void full_detail_information::extract_apple_metadata_from_apple_metadata_reader(QString file_paths_str, QString source_cnt_name,QString virtual_source_path)
{ //  extract apple metadata from metadata reader which we aren't using any more
    QString config_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString all_apple_attributes_command_str = QString("Select metadata_attribute_name From tbl_apple_attributes");
    QStringList attributes_name_list_all = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(all_apple_attributes_command_str, 0, config_db_path, Q_FUNC_INFO);

    QString all_apple_attributes_columnname_command_str = QString("Select custom_column_name From tbl_apple_attributes");
    QStringList column_name_list_all = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(all_apple_attributes_columnname_command_str, 0, config_db_path, Q_FUNC_INFO);

    emit signal_detail_information_triggered();
    recon_case_obj->pub_rcase_extract_apple_metadata_from_apple_metadata_reader(file_paths_str, source_cnt_name, virtual_source_path,attributes_name_list_all,column_name_list_all);

}

void full_detail_information::pub_set_recon_case_obj(RECON_case  *obj)
{
    recon_case_obj = obj;
}
