#include "detach_manager.h"

detach_window_manager::detach_window_manager(QWidget *parent)
{
    list_detach_window_list.clear();

    exif_interface_obj = new exif_data_interface(this);
    recon_helper_standard_obj = new recon_helper_standard(this);


}
detach_window_manager::~detach_window_manager()
{

}


void detach_window_manager::pub_set_hex_viewer_block_db_path(QString path)
{
    hex_viewer_block_db_path = path;
}

void detach_window_manager::pub_set_essentials()
{
    result_dir_val = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
}


void detach_window_manager::pub_set_plugin_name_table_record_no(QString plugin, QString table, QString record_no)
{
    plugin_name = plugin;
    table_name = table;
    record_number = record_no;

}

void detach_window_manager::pub_open_in_detach_clicked(QString metadata_value, QString hex_file_path, QString preview_path, int preview_index,QString searched_keyword, QString source_count_name)
{ // most of detach function not in use
    recon_static_functions::app_debug(" :START",Q_FUNC_INFO);

    struct_detach_window_object_info struct_detach_info_obj;
    struct_detach_info_obj.detach_window_obj = new detach_detailed_information(this);

    struct_detach_info_obj.splitter_metadata_tabs_detach = new QSplitter(Qt::Horizontal);
    struct_detach_info_obj.splitter_metadata_tabs_detach->setHandleWidth(0);


    struct_detach_info_obj.text_edit_metadata_detach = new m_textedit(this);
    struct_detach_info_obj.text_edit_metadata_detach->setText(metadata_value);

    //    struct_detach_info_obj.hex_viewer_obj_detach = new hex_viewer(this);
    //    struct_detach_info_obj.hex_viewer_obj_detach->pub_display_hex_viewer_metadata();

    //    struct_detach_info_obj.hex_viewer_obj_detach->set_file_path_and_db_path(hex_file_path, hex_viewer_block_db_path);
    //    struct_detach_info_obj.hex_viewer_obj_detach->pub_set_essentials();
    //    struct_detach_info_obj.hex_viewer_obj_detach->pub_set_source_count_name(source_count_name);
    //    struct_detach_info_obj.hex_viewer_obj_detach->pub_display_hex_viewer_full();
    //    struct_detach_info_obj.hex_viewer_obj_detach->pub_set_comboBox_block_list();

    //    struct_detach_info_obj.hex_viewer_obj_detach->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    //    QFileInfo info_file(hex_file_path);

    //    if(hex_file_path.isEmpty() || (!info_file.exists()))
    //    {
    //        struct_detach_info_obj.hex_viewer_obj_detach->setdata("");
    //    }
    //    else
    //    {
    //        struct_detach_info_obj.hex_viewer_obj_detach->set_file_data(hex_file_path);
    //    }

    struct_detach_info_obj.tree_exif_metadata = new m_treewidget(this);
    struct_detach_info_obj.tree_exif_metadata->setColumnCount(2);

    QString initial_path = global_recon_helper_singular_class_obj->get_file_system_initial_path_by_giving_source_count_name(source_count_name , Q_FUNC_INFO);
    QString file_path_initial_path_removed = preview_path;
    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    if(struct_source_info.os_scheme_internal != QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
            && struct_source_info.os_scheme_internal != QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
    {
        file_path_initial_path_removed = preview_path.mid(initial_path.size() , preview_path.size());
    }

    set_exif_data_in_detach_window(struct_detach_info_obj.tree_exif_metadata , source_count_name , file_path_initial_path_removed, plugin_name, table_name);

    struct_detach_info_obj.stackwidget_previewer_obj_detach = new m_stackedwidget(this);
    struct_detach_info_obj.blank_obj_detach = new blank(this);
    struct_detach_info_obj.av_preview_obj_detach = new av_preview(this);
    struct_detach_info_obj.image_preview_obj_detach = new image_preview(this);

    struct_detach_info_obj.stackwidget_previewer_obj_detach->addWidget(struct_detach_info_obj.blank_obj_detach);
    struct_detach_info_obj.stackwidget_previewer_obj_detach->addWidget(struct_detach_info_obj.image_preview_obj_detach);
    struct_detach_info_obj.stackwidget_previewer_obj_detach->addWidget(struct_detach_info_obj.av_preview_obj_detach);

    struct_detach_info_obj.stackwidget_previewer_obj_detach->setCurrentIndex(0);

    struct_detach_info_obj.av_preview_obj_detach->stop_media();
    struct_detach_info_obj.stackwidget_previewer_obj_detach->setCurrentIndex(0);

    if(preview_index == 0)
    {
        struct_detach_info_obj.stackwidget_previewer_obj_detach->setCurrentIndex(preview_index);
    }
    else if(preview_index == 1)
    {
        struct_detach_info_obj.image_preview_obj_detach->set_image_by_path(preview_path);
        struct_detach_info_obj.stackwidget_previewer_obj_detach->setCurrentIndex(preview_index);
    }
    else if( preview_index == 2)
    {
        struct_detach_info_obj.av_preview_obj_detach->load_source(preview_path);
        struct_detach_info_obj.stackwidget_previewer_obj_detach->setCurrentIndex(preview_index);
    }

    QScrollArea *scroll_area = new QScrollArea;
    //    scroll_area->setWidget(struct_detach_info_obj.hex_viewer_obj_detach);
    scroll_area->setWidgetResizable(true);

    struct_detach_info_obj.tabwidget_metadata_viewer_detach = new R_Tabwidget(this);
    struct_detach_info_obj.tabwidget_metadata_viewer_detach->setStyleSheet("QTabBar::tab { max-height: 8px; min-height: 8px; color:#EAEAEA; font:  11pt; color:#404040; }"
                                                                           " QTabBar::tab:hover { color:#000000; } QTabBar::tab:selected { color:#000000;  }" );
    struct_detach_info_obj.tabwidget_metadata_viewer_detach->setTabPosition(QTabWidget::TabPosition::South);
    struct_detach_info_obj.tabwidget_metadata_viewer_detach->setElideMode(Qt::ElideNone);
    struct_detach_info_obj.tabwidget_metadata_viewer_detach->setDocumentMode(true);
    struct_detach_info_obj.tabwidget_metadata_viewer_detach->addTab(struct_detach_info_obj.text_edit_metadata_detach,"Detailed Information");
    struct_detach_info_obj.tabwidget_metadata_viewer_detach->addTab(scroll_area, "Hex View");
    struct_detach_info_obj.tabwidget_metadata_viewer_detach->addTab(struct_detach_info_obj.tree_exif_metadata, QString(MACRO_Metadata_TabName_Exif_Meatadta));

    struct_detach_info_obj.tree_apple_metadata = new m_treewidget(this);
    struct_detach_info_obj.tabwidget_metadata_viewer_detach->addTab(struct_detach_info_obj.tree_apple_metadata, "Apple Metadata");

    struct_detach_info_obj.tree_apple_metadata->setColumnCount(6);

    QTreeWidgetItem* header_tree = struct_detach_info_obj.tree_apple_metadata->headerItem();
    header_tree->setText(enum_detach_window_manager_apple_metadata_KEY, "Attribute");
    header_tree->setText(enum_detach_window_manager_apple_metadata_Value, "Value");
    header_tree->setText(enum_detach_window_manager_apple_metadata_Plugin_Table_Name, "Plugin Table Name");
    header_tree->setText(enum_detach_window_manager_apple_metadata_Plugin_Db_Path, "Plugin Db Path");
    header_tree->setText(enum_detach_window_manager_apple_metadata_Plugin_Record_no, "Record no");
    header_tree->setText(enum_detach_window_manager_apple_metadata_File_Path, "File Path");

    struct_detach_info_obj.tree_apple_metadata->header()->resizeSection(0, 220);

    connect(struct_detach_info_obj.tree_apple_metadata, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(slot_tree_apple_metadata_item_clicked(QTreeWidgetItem*,int)));

    QStringList path_list;
    path_list << file_path_initial_path_removed;

    change_apple_attributes_content_on_row_click(path_list, source_count_name, record_number, table_name, plugin_name, struct_detach_info_obj.tree_apple_metadata);




    struct_detach_info_obj.tabwidget_metadata_viewer_detach->setTabsClosable(false);

    struct_detach_info_obj.tabwidget_av_and_images_previewer_detach = new R_Tabwidget(this);
    struct_detach_info_obj.tabwidget_av_and_images_previewer_detach->setStyleSheet("QTabBar::tab { max-height: 8px; min-height: 8px; color:#EAEAEA; font:  11pt; color:#404040; }"
                                                                                   " QTabBar::tab:hover { color:#000000; } QTabBar::tab:selected { color:#000000;  }" );
    struct_detach_info_obj.tabwidget_av_and_images_previewer_detach->setTabPosition(QTabWidget::TabPosition::South);
    struct_detach_info_obj.tabwidget_av_and_images_previewer_detach->setElideMode(Qt::ElideNone);
    struct_detach_info_obj.tabwidget_av_and_images_previewer_detach->setDocumentMode(true);
    struct_detach_info_obj.tabwidget_av_and_images_previewer_detach->addTab(struct_detach_info_obj.stackwidget_previewer_obj_detach,"Preview");
    struct_detach_info_obj.tabwidget_av_and_images_previewer_detach->setMinimumWidth(350);
    struct_detach_info_obj.tabwidget_av_and_images_previewer_detach->setTabsClosable(false);


    struct_detach_info_obj.splitter_metadata_tabs_detach->addWidget(struct_detach_info_obj.tabwidget_metadata_viewer_detach);
    struct_detach_info_obj.splitter_metadata_tabs_detach->addWidget(struct_detach_info_obj.tabwidget_av_and_images_previewer_detach);
    struct_detach_info_obj.splitter_metadata_tabs_detach->setStretchFactor(0,1);
    struct_detach_info_obj.splitter_metadata_tabs_detach->setStretchFactor(1,0);

    struct_detach_info_obj.splitter_metadata_tabs_detach->setCollapsible(0,false);
    struct_detach_info_obj.splitter_metadata_tabs_detach->setCollapsible(1,false);



    struct_detach_info_obj.hlayout_detach = new QHBoxLayout;
    struct_detach_info_obj.hlayout_detach->addWidget(struct_detach_info_obj.splitter_metadata_tabs_detach);
    struct_detach_info_obj.hlayout_detach->setContentsMargins(6,6,6,6);

    struct_detach_info_obj.detach_window_obj->setLayout(struct_detach_info_obj.hlayout_detach);
    struct_detach_info_obj.detach_window_obj->show();
    struct_detach_info_obj.detach_window_obj->setWindowTitle(recon_static_functions::get_app_name() + " - Detached Viewer");


    struct_detach_info_obj.text_edit_metadata_detach->moveCursor(QTextCursor::Start);

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


        QTextCursor cursor(struct_detach_info_obj.text_edit_metadata_detach->document());

        QString text = struct_detach_info_obj.text_edit_metadata_detach->toPlainText().toLower();

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

    connect(struct_detach_info_obj.detach_window_obj, SIGNAL(close_detach_window(detach_detailed_information*)), this, SLOT(slot_close_windows_index(detach_detailed_information*)));

    list_detach_window_list.append(struct_detach_info_obj);

    recon_static_functions::app_debug(" :END",Q_FUNC_INFO);

}

void detach_window_manager::pub_detach_content_change(QString metadata_value, QString hex_file_path, QString preview_path, int preview_index,QString searched_keyword,QString source_count_name)
{ // not in use
    for(int i = 0; i < list_detach_window_list.size(); i++)
    {
        struct_detach_window_object_info struct_detach_info_obj;
        struct_detach_info_obj = list_detach_window_list.at(i);

        struct_detach_info_obj.text_edit_metadata_detach->setText(metadata_value);

        QFileInfo info_file(hex_file_path);

        //        struct_detach_info_obj.hex_viewer_obj_detach->set_file_path_and_db_path(hex_file_path, hex_viewer_block_db_path);
        //        struct_detach_info_obj.hex_viewer_obj_detach->pub_set_essentials();
        //        struct_detach_info_obj.hex_viewer_obj_detach->pub_set_source_count_name(source_count_name);
        //        struct_detach_info_obj.hex_viewer_obj_detach->pub_display_hex_viewer_full();
        //        struct_detach_info_obj.hex_viewer_obj_detach->pub_set_comboBox_block_list();

        //        if(hex_file_path.isEmpty() || (!info_file.exists()))
        //        {
        //            struct_detach_info_obj.hex_viewer_obj_detach->setdata("");
        //        }
        //        else
        //        {
        //            struct_detach_info_obj.hex_viewer_obj_detach->set_file_data(hex_file_path);
        //        }

        QStringList path_list;
        path_list << preview_path;
        change_apple_attributes_content_on_row_click(path_list, source_count_name, record_number, table_name, plugin_name, struct_detach_info_obj.tree_apple_metadata);

        set_exif_data_in_detach_window(struct_detach_info_obj.tree_exif_metadata , source_count_name , preview_path, plugin_name, table_name);

        struct_detach_info_obj.stackwidget_previewer_obj_detach->addWidget(struct_detach_info_obj.blank_obj_detach);
        struct_detach_info_obj.stackwidget_previewer_obj_detach->addWidget(struct_detach_info_obj.image_preview_obj_detach);
        struct_detach_info_obj.stackwidget_previewer_obj_detach->addWidget(struct_detach_info_obj.av_preview_obj_detach);

        struct_detach_info_obj.stackwidget_previewer_obj_detach->setCurrentIndex(0);

        struct_detach_info_obj.av_preview_obj_detach->stop_media();
        struct_detach_info_obj.stackwidget_previewer_obj_detach->setCurrentIndex(0);

        if(preview_index == 0)
        {
            struct_detach_info_obj.stackwidget_previewer_obj_detach->setCurrentIndex(preview_index);
        }
        else if(preview_index == 1)
        {
            struct_detach_info_obj.image_preview_obj_detach->set_image_by_path(preview_path);
            struct_detach_info_obj.stackwidget_previewer_obj_detach->setCurrentIndex(preview_index);
        }
        else if( preview_index == 2)
        {
            struct_detach_info_obj.av_preview_obj_detach->load_source(preview_path);
            struct_detach_info_obj.stackwidget_previewer_obj_detach->setCurrentIndex(preview_index);
        }


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


            QTextCursor cursor(struct_detach_info_obj.text_edit_metadata_detach->document());

            QString text = struct_detach_info_obj.text_edit_metadata_detach->toPlainText().toLower();

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
}

void detach_window_manager::slot_close_windows_index(detach_detailed_information *closed_window_addr)
{
    int closed_window_index = 0;

    for(int i = 0; i < list_detach_window_list.size(); i++)
    {
        if(list_detach_window_list.at(i).detach_window_obj == closed_window_addr)
        {
            closed_window_index = i;
        }
    }

    delete list_detach_window_list.at(closed_window_index).detach_window_obj;
    list_detach_window_list.removeAt(closed_window_index);
}

void detach_window_manager::slot_tree_apple_metadata_item_clicked(QTreeWidgetItem *item, int column)
{
    QString tablename = item->text(enum_detach_window_manager_apple_metadata_Plugin_Table_Name);
    QString db_path = item->text(enum_detach_window_manager_apple_metadata_Plugin_Db_Path);
    QString rec_no = item->text(enum_detach_window_manager_apple_metadata_Plugin_Record_no);
    QString key = item->text(enum_detach_window_manager_apple_metadata_KEY);
    QString value = item->text(enum_detach_window_manager_apple_metadata_Value);

    QString command_select = "select  bookmarked_extended_attributes from '" + tablename + "' where INT = ?";
    QStringList Int_value;
    Int_value << rec_no;
    QString stored_ext_attr = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select, Int_value, 0, db_path, Q_FUNC_INFO);

    if(!stored_ext_attr.endsWith("\n"))
        stored_ext_attr.append("\n");

    QString apl_meta_str = QString(key + " : " + value + "\n");

    if(!item->checkState(enum_detach_window_manager_apple_metadata_KEY))
    {
        if(stored_ext_attr.contains(apl_meta_str))
        {
            stored_ext_attr.remove(apl_meta_str);

            QString command = "update '" + tablename + "' set bookmarked_extended_attributes = ? where INT = ?";
            QStringList list;

            list << stored_ext_attr << rec_no;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,db_path, Q_FUNC_INFO);

        }
    }
    else
    {
        if(!stored_ext_attr.contains(apl_meta_str))
        {
            stored_ext_attr +=  QString(key + " : " + value + "\n");

            qint64 child_count = item->childCount();
            for(int mm = 0; mm < child_count; mm++)
            {
                stored_ext_attr += item->child(mm)->text( enum_detach_window_manager_apple_metadata_Value) + "\n";
            }

            QString command = "update '" + tablename + "' set bookmarked_extended_attributes = ? where INT = ?";
            QStringList list;
            list << stored_ext_attr << rec_no;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,db_path, Q_FUNC_INFO);

        }
    }

}

void detach_window_manager::set_exif_data_in_detach_window(m_treewidget *tree_exif_metadata_display, QString source_count_name, QString filepath, QString plugin_name, QString table_name)
{
    if(plugin_name == QString(MACRO_Plugin_Name_Screenshots))
        return;

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    struct_GLOBAL_witness_info_source source_info_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);


    QString fs_filepath = filepath.mid(source_info_obj.virtual_source_path.size() , filepath.size());

    if(!fs_filepath.startsWith(QString("/")))
        fs_filepath.prepend(QString("/"));

    /// get record number from filepath from fs db
    if(!fs_filepath.isEmpty())
    {
        QString record_no_str = QString("");
        struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        if(struct_source_info.os_scheme_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
                || struct_source_info.os_scheme_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
        {
            QString value_str = QFileInfo(filepath).fileName().remove(QFileInfo(filepath).suffix());

            if(value_str.endsWith("."))
                value_str.chop(1);

            QString command_str = QString("SELECT INT FROM files Where fileID LIKE ?");

            QStringList value_list;
            value_list << value_str;
            record_no_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str ,value_list, 0, fs_db_path, Q_FUNC_INFO);

        }
        else
        {
            QString command_str = QString("Select INT from files Where filepath = ?");
            QStringList value_list;
            value_list << fs_filepath;

            record_no_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str ,value_list, 0, fs_db_path, Q_FUNC_INFO);

        }


        if(!record_no_str.isEmpty())
        {
            exif_interface_obj->pub_set_result_dir(result_dir_val);
            // exif_interface_obj->pub_set_exif_data_in_case_tree_display(filepath,record_no_str , fs_db_path , tree_exif_metadata_display, plugin_name, table_name, source_count_name);
        }

    }
}

void detach_window_manager::change_apple_attributes_content_on_row_click(QStringList file_path_list, QString source_count_name, QString record_no, QString table_name, QString plugin_name, m_treewidget *tree_metadata_apple_metadata)
{
    recon_static_functions::app_debug("---Starts----",Q_FUNC_INFO);

    if(file_path_list.isEmpty() || plugin_name.trimmed().isEmpty())
    {
        tree_metadata_apple_metadata->clear();
        return;
    }

    tree_metadata_apple_metadata->clear();
    QString tmp_plugin_name = plugin_name;

    QString destination_db_path = QString(result_dir_val + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plugin_name.replace(" ", "_") + "/" + tmp_plugin_name + ".sqlite");

    if(plugin_name == QString(MACRO_Plugin_Name_File_System))
        destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";


    for(int file_count = 0; file_count < file_path_list.size(); file_count++)
    {
        QString file_path = file_path_list.at(file_count);

        struct_GLOBAL_witness_info_source source_info_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
        file_path = file_path.remove(0,source_info_obj.virtual_source_path.size());

        if(!file_path.startsWith(QString("/")))
            file_path.prepend(QString("/"));

        QString command = "select bookmarked_extended_attributes from '" + table_name + "' where INT = ?";
        QStringList list;
        list << record_no;
        QString stored_apl_attributes = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command , list, 0, destination_db_path, Q_FUNC_INFO);

        if(!stored_apl_attributes.endsWith("\n") && !stored_apl_attributes.isEmpty())
            stored_apl_attributes.append("\n");

        QStringList ext_attribute_list = get_apple_metadata_key_value_list_from_fs_db(file_path, source_count_name);
        if(ext_attribute_list.isEmpty())
            continue;

        QFileInfo file(file_path);

        QTreeWidgetItem *root_item = new QTreeWidgetItem();
        root_item->setText(enum_detach_window_manager_apple_metadata_KEY, file.fileName());
        root_item->setText(enum_detach_window_manager_apple_metadata_Value, "");
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


            QString check_already_selected = QString(key + " : " + value + "\n");


            QTreeWidgetItem *tree_child = new QTreeWidgetItem();
            if(stored_apl_attributes.contains(check_already_selected))
                tree_child->setCheckState(enum_detach_window_manager_apple_metadata_KEY,Qt::Checked);
            else
                tree_child->setCheckState(enum_detach_window_manager_apple_metadata_KEY,Qt::Unchecked);

            tree_child->setText(enum_detach_window_manager_apple_metadata_KEY,key);

            if(value.contains("\n"))
            {
                QStringList sub_values_list = value.split("\n", Qt::SkipEmptyParts);

                int count = 0;
                while(count < sub_values_list.size())
                {
                    QString sub_value = sub_values_list.at(count);

                    if(count == 0)
                    {
                        tree_child->setText(enum_detach_window_manager_apple_metadata_Value, sub_value);
                        tree_child->setText(enum_detach_window_manager_apple_metadata_Plugin_Table_Name, table_name);
                        tree_child->setText(enum_detach_window_manager_apple_metadata_Plugin_Db_Path, destination_db_path);
                        tree_child->setText(enum_detach_window_manager_apple_metadata_Plugin_Record_no, record_no);
                        tree_child->setText(enum_detach_window_manager_apple_metadata_File_Path, file_path);


                        root_item->addChild(tree_child);
                    }
                    else
                    {
                        QTreeWidgetItem *tree_sub_child = new QTreeWidgetItem;
                        tree_sub_child->setText( enum_detach_window_manager_apple_metadata_Value, sub_value);

                        tree_child->addChild(tree_sub_child);

                    }
                    count++;
                }
            }
            else
            {

                tree_child->setText(enum_detach_window_manager_apple_metadata_Value, value);
                tree_child->setText(enum_detach_window_manager_apple_metadata_Plugin_Table_Name, table_name);
                tree_child->setText(enum_detach_window_manager_apple_metadata_Plugin_Db_Path, destination_db_path);
                tree_child->setText(enum_detach_window_manager_apple_metadata_Plugin_Record_no, record_no);
                tree_child->setText(enum_detach_window_manager_apple_metadata_File_Path, file_path);

                root_item->addChild(tree_child);
            }

        }


        break;///so that we need only first file to get extended attributes in case of multiple files attach with single record
    }

    tree_metadata_apple_metadata->expandAll();

    tree_metadata_apple_metadata->setColumnWidth(enum_detach_window_manager_apple_metadata_KEY,300);
    tree_metadata_apple_metadata->setColumnWidth(enum_detach_window_manager_apple_metadata_Value,400);
    tree_metadata_apple_metadata->setColumnHidden(enum_detach_window_manager_apple_metadata_Plugin_Table_Name,true);
    tree_metadata_apple_metadata->setColumnHidden(enum_detach_window_manager_apple_metadata_Plugin_Db_Path,true);
    tree_metadata_apple_metadata->setColumnHidden(enum_detach_window_manager_apple_metadata_Plugin_Record_no,true);
    tree_metadata_apple_metadata->setColumnHidden(enum_detach_window_manager_apple_metadata_File_Path,true);


    recon_static_functions::app_debug("---Ends----",Q_FUNC_INFO);

}

QStringList detach_window_manager::get_apple_metadata_key_value_list_from_fs_db(QString file_path, QString source_count_name)
{
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
void detach_detailed_information::highlight_search_text(QString pattern)
{
    //    textedit_metadata->moveCursor(QTextCursor::Start);

    //    pattern = pattern.trimmed();

    //    if(pattern.size() < 1)
    //        return;

    //    QStringList patternlist;

    //    if(pattern.contains(","))
    //    {
    //        QStringList keyword_list;
    //        keyword_list = pattern.split(",");

    //        for(int i = 0; i < keyword_list.size(); i++)
    //        {
    //            QString temp_string = keyword_list.at(i);
    //            temp_string = temp_string.trimmed();
    //            if(temp_string == "")
    //            {
    //                continue;
    //            }

    //            patternlist.append(temp_string);
    //        }
    //    }
    //    else
    //        patternlist.append(pattern);

    //    QTextCharFormat fmt;
    //    fmt.setBackground(Qt::yellow);


    //    QTextCursor cursor(textedit_metadata->document());

    //    QString text = textedit_metadata->toPlainText().toLower();

    //    for(int i = 0; i < patternlist.size(); i++)
    //    {
    //        QString pattern = patternlist.at(i);
    //        cursor.setCharFormat(fmt);
    //        int length = pattern.size();

    //        int index = 0;
    //        while (index >= 0)
    //        {

    //            index = text.indexOf(pattern.toLower(), index);
    //            if(index < 0)
    //            {
    //                break;
    //            }

    //            cursor.setPosition(index, QTextCursor::MoveAnchor);
    //            cursor.setPosition(index + length, QTextCursor::KeepAnchor);

    //            index = index + length;

    //            cursor.setCharFormat(fmt);
    //        }
    //    }

}
