#include "r_treewidget.h"

R_Treewidget::R_Treewidget(QWidget *parent)
{
    this->setAttribute(Qt::WA_MacShowFocusRect,0);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(const QPoint&)),SLOT(onCustomContextMenuRequested(const QPoint&)));

    this->setAttribute(Qt::WA_MacShowFocusRect,0);

    this->setIndentation(15);
    setAnimated(true);

    //    this->setAlternatingRowColors(true);
    this->setStyleSheet("QTreeWidget {"
                        "selection-background-color: #CDCDCD;"
                        "selection-color: black;"
                        "}"
                        "QTreeWidget::item {"
                        "height: 23px;"

                        "}"
                        "QTreeWidget::item:selected:!active {"
                        "color:black;"
                        " background-color:  #CDCDCD;"

                        "}"
                        );

    action_delete_item = new QAction(QObject::tr("Remove"), this);
    connect(action_delete_item, SIGNAL(triggered()), this, SLOT(slot_action_remove_item()));

    menu_delete_from_tree = new QMenu(this);
    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    menu_delete_from_tree->setFont(m_font);
    menu_delete_from_tree->addAction(action_delete_item);

    menu_case_tree_sources = new QMenu(this);
    action_add_to_indexing  = new QAction(MACRO_Generic_Right_Click_Add_To_Text_Indexing_Queue, this);
    connect(action_add_to_indexing, SIGNAL(triggered()), this, SLOT(slot_action_add_to_text_indexing()));

    action_carve_unallocated_space  = new QAction(MACRO_Generic_Right_Click_Carve_Unallocated_Space, this);
    action_carve_unallocated_space->setEnabled(false);
    connect(action_carve_unallocated_space, SIGNAL(triggered()), this, SLOT(slot_action_carve_unallocated_space()));

    action_snapshots_apfs  = new QAction(MACRO_CASE_TREE_Snapshots, this);
    action_snapshots_apfs->setEnabled(false);
    connect(action_snapshots_apfs, SIGNAL(triggered()), this, SLOT(slot_action_snapshots_apfs_clicked()));


    action_disk_hex_viewer  = new QAction(MACRO_Generic_Right_Click_Disk_Hex_Viewer_Complete, this);
    connect(action_disk_hex_viewer, SIGNAL(triggered()), this, SLOT(slot_action_disk_hex_viewer_triggered()));

    action_view_forensic_image_in_hex_viewer  = new QAction(MACRO_Generic_Right_Click_View_In_Hex_Viewer, this);
    action_view_forensic_image_in_hex_viewer->setEnabled(false);
    connect(action_view_forensic_image_in_hex_viewer, SIGNAL(triggered()), this, SLOT(slot_action_view_forensic_image_in_hex_viewer()));

    //    action_disk_hex_viewer_unallocated  = new QAction(MACRO_Generic_Right_Click_Disk_Hex_Viewer_Unallocated, this);
    //    connect(action_disk_hex_viewer_unallocated, SIGNAL(triggered()), this, SLOT(slot_action_disk_hex_viewer_triggered()));

    menu_disk_hex_viewer = new QMenu(MACRO_Generic_Right_Click_Disk_Hex_Viewer, this);
    menu_disk_hex_viewer->addAction(action_disk_hex_viewer);
    //    menu_disk_hex_viewer->addSeparator();
    //    menu_disk_hex_viewer->addAction(action_disk_hex_viewer_unallocated);

    menu_case_tree_sources->setFont(m_font);
    //  menu_case_tree_sources->addMenu(menu_disk_hex_viewer);
    menu_case_tree_sources->addSeparator();
    menu_case_tree_sources->addAction(action_add_to_indexing);
    menu_case_tree_sources->addSeparator();
    menu_case_tree_sources->addAction(action_carve_unallocated_space);
    menu_case_tree_sources->addSeparator();
    menu_case_tree_sources->addAction(action_snapshots_apfs);
    menu_case_tree_sources->addSeparator();
    menu_case_tree_sources->addAction(action_view_forensic_image_in_hex_viewer);

    menu_story_board = new QMenu(this);
    action_remove_story  = new QAction("Remove Story", this);
    connect(action_remove_story, SIGNAL(triggered()), this, SLOT(slot_action_remove_story()));

    menu_story_board->setFont(m_font);
    menu_story_board->addAction(action_remove_story);


}

R_Treewidget::~R_Treewidget()
{

}


void R_Treewidget::slot_action_run_plugins()
{
    common_action_run_plugins();
}

void R_Treewidget::slot_action_keyword_search()
{
    common_action_keyword_search();
}

void R_Treewidget::slot_action_artifacts_timeline_full()
{
    common_action_artifacts_timeline_full();
}

void R_Treewidget::slot_action_global_report()
{
    common_action_global_report();
}

void R_Treewidget::slot_action_close_case()
{
    common_action_close_case();
}

void R_Treewidget::slot_action_add_source_case()
{
    comman_action_add_source();
}

void R_Treewidget::slot_action_k_search_bookmark_all()
{
    common_action_k_search_bookmark_all();
}

void R_Treewidget::slot_action_k_search_remove_bookmark_all()
{
    common_action_k_search_remove_bookmark_all();
}

void R_Treewidget::slot_action_k_search_goto_record()
{
    common_action_k_search_goto_record();
}

void R_Treewidget::common_action_run_plugins()
{
    emit signal_tree_run_plugins();
}

void R_Treewidget::common_action_keyword_search()
{
    emit signal_tree_keyword_search();

}

void R_Treewidget::common_action_artifacts_timeline_full()
{
    emit signal_artifacts_timeline_full();
}

void R_Treewidget::common_action_global_report()
{
    emit signal_tree_global_report();
}

void R_Treewidget::common_action_close_case()
{
    emit signal_tree_close_case();
}

void R_Treewidget::comman_action_add_source()
{
    emit signal_tree_add_source();
}

void R_Treewidget::slot_submenu_extract_added_sources_triggered(QAction *clicked_action)
{
    QString source_name = clicked_action->text();
    QString source_count_name = clicked_action->toolTip();

}

void R_Treewidget::common_action_k_search_bookmark_all()
{
    emit signal_action_k_search_bookmark_all();
}

void R_Treewidget::slot_submenu_extract_apple_metadata_triggered()
{

}

void R_Treewidget::common_action_k_search_remove_bookmark_all()
{
    emit signal_action_k_search_remove_bookmark_all();
}

void R_Treewidget::common_action_k_search_goto_record()
{
    emit signal_action_k_search_goto_record();
}

void R_Treewidget::slot_submenu_extract_signature_triggered()
{
}

void R_Treewidget::slot_submenu_extract_exif_data_triggered()
{
}

void R_Treewidget::slot_submenu_extract_file_hashes_triggered()
{
}

void R_Treewidget::slot_action_disk_hex_viewer_triggered()
{
    QAction *clicked_action = (QAction*)sender();

    QTreeWidgetItem* item = currentItem();

    if(clicked_action == NULL || item == NULL)
        return;

    if(bool_complete_source_for_hex_view)
        emit signal_right_click_disk_hex_view(clicked_action->text().trimmed(), source_count_name_for_root_node, bool_complete_source_for_hex_view);
    else
        emit signal_right_click_disk_hex_view(clicked_action->text().trimmed(), item->text(1).trimmed(), bool_complete_source_for_hex_view);
}

void R_Treewidget::slot_submenu_extract_fs_all_triggered()
{
}

void R_Treewidget::slot_action_add_to_text_indexing()
{
    QTreeWidgetItem* item = currentItem();

    emit signal_right_click_add_to_text_indexing(item->text(3).trimmed(), item->text(1).trimmed());

}

void R_Treewidget::slot_action_remove_item()
{
    emit signal_right_click_remove_selected_item();
}

void R_Treewidget::slot_action_remove_story()
{
    QTreeWidgetItem* item = currentItem();

    emit signal_right_click_remove_story(item->text(0).trimmed());

}

void R_Treewidget::slot_action_carve_unallocated_space()
{
    QTreeWidgetItem* item = currentItem();
    emit signal_right_click_carve_unallocated_space(item->text(1).trimmed(), item->text(3).trimmed(), true);
}

void R_Treewidget::onCustomContextMenuRequested(const QPoint& pos)
{
    menu_disk_hex_viewer->setDisabled(true);
    action_add_to_indexing->setDisabled(true);
    action_carve_unallocated_space->setDisabled(true);
    action_snapshots_apfs->setDisabled(true);

    action_snapshots_apfs->setVisible(false);
    action_view_forensic_image_in_hex_viewer->setDisabled(true);

    QTreeWidgetItem* item = itemAt(pos);

    if(item)
    {
        QTreeWidgetItem* itm_prnt = item->parent();


        //        if(tree_type == MACRO_ACTION_MENU_TREE_CASE_int)
        if(itm_prnt)
        {
            if(item->text(enum_global_case_tree_display_column_ZERO).trimmed().startsWith(MACRO_CASE_TREE_Case))
            {
                menu->exec(QCursor::pos());
            }
            else if(item->parent()->text(enum_global_case_tree_display_column_ZERO).trimmed().startsWith(MACRO_CASE_TREE_Plist_Viewer))
            {
                menu_delete_from_tree->exec(QCursor::pos());
            }
            else if(item->parent()->text(enum_global_case_tree_display_column_ZERO).trimmed().startsWith(MACRO_CASE_TREE_Hex_Viewer))
            {
                menu_delete_from_tree->exec(QCursor::pos());
            }
            else if(item->parent()->text(enum_global_case_tree_display_column_ZERO).trimmed().startsWith(MACRO_CASE_TREE_Sqlite_Viewer))
            {
                menu_delete_from_tree->exec(QCursor::pos());
            }
            else if(item->parent()->text(enum_global_case_tree_display_column_ZERO).trimmed().startsWith(MACRO_CASE_TREE_Log_Viewer))
            {
                menu_delete_from_tree->exec(QCursor::pos());
            }
            else if(item->parent()->text(enum_global_case_tree_display_column_ZERO).trimmed().startsWith(MACRO_CASE_TREE_Artifacts_Keyword_Search))
            {
                menu_delete_from_tree->exec(QCursor::pos());
            }
            else if(item->parent()->text(enum_global_case_tree_display_column_ZERO).trimmed().startsWith(MACRO_CASE_TREE_Artifacts_Timeline))
            {
                menu_delete_from_tree->exec(QCursor::pos());
            }
            else if(item->parent()->text(enum_global_case_tree_display_column_FIRST).trimmed().startsWith(MACRO_CASE_TREE_Redefined_Result_Saved_Parent))
            {
                menu_delete_from_tree->exec(QCursor::pos());
            }
            else if(item->parent()->text(enum_global_case_tree_display_column_ZERO).trimmed().startsWith(MACRO_CASE_TREE_Content_Search))
            {
                menu_delete_from_tree->exec(QCursor::pos());
            }
            else if(item->parent()->text(enum_global_case_tree_display_column_ZERO).trimmed().startsWith(MACRO_CASE_TREE_File_Search))
            {
                menu_delete_from_tree->exec(QCursor::pos());
            }
            else if(item->parent()->text(enum_global_case_tree_display_column_ZERO).trimmed().startsWith(MACRO_CASE_TREE_Exif_Metadata_Search))
            {
                menu_delete_from_tree->exec(QCursor::pos());
            }
            else if(item->text(enum_global_case_tree_display_column_FIRST).trimmed() == QString(MACRO_CASE_TREE_Parent_Source))
            {
                bool_complete_source_for_hex_view = false;
                source_count_name_for_root_node.clear();

                QTreeWidgetItem *child_item = item->child(enum_global_case_tree_display_column_ZERO);
                if(child_item != NULL)
                {
                    QString source_count_name = child_item->text(enum_global_case_tree_display_column_FIRST).trimmed();
                    source_count_name_for_root_node = source_count_name;
                    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_for_root_node);

                    bool_complete_source_for_hex_view = true;
                    QString src_type_intrnl = struct_source_info.source_type_internal;
                    QString fs_type = struct_source_info.file_system_type;

                    if((src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
                        || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal)
                        || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal)
                        || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal))
                            && (fs_type.toLower() != QString(MACRO_FILE_SYSTEM_TYPE_APFS)))
                    {
                        menu_disk_hex_viewer->setEnabled(true);
                        action_disk_hex_viewer->setEnabled(true);
                    }

                    if((src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
                        || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal)
                        || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal)
                        || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal)))
                    {
                        action_view_forensic_image_in_hex_viewer->setEnabled(true);
                    }

                    menu_case_tree_sources->exec(QCursor::pos());
                }
            }
            else if(item->text(enum_global_case_tree_display_column_FIRST).trimmed().startsWith(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag)))
            {
                // for folder or file as source
                if(item->parent()->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Source) || item->parent()->text(enum_global_case_tree_display_column_FIRST) == QString(MACRO_CASE_TREE_Parent_Source))
                {
                    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(item->text(enum_global_case_tree_display_column_FIRST).trimmed());
                    QString fs_type = struct_source_info.file_system_type.trimmed();
                    if((struct_source_info.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
                        || struct_source_info.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal)
                        || struct_source_info.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal)
                        || struct_source_info.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal))
                            && (check_fs_type_supported_for_carve_unallocated(fs_type)))
                    {
                        action_carve_unallocated_space->setEnabled(true);
                    }
                    else
                    {
                        action_carve_unallocated_space->setEnabled(false);
                    }

                    QString src_type_intrnl = struct_source_info.source_type_internal;
                    if((src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
                        || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal)
                        || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal)
                        || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal)
                        || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal))
                            && (fs_type == QString(MACRO_FILE_SYSTEM_TYPE_APFS)))
                    {
                        /// check snapshot available for source
                        if(struct_source_info.snapshots_QList.size() > 0)
                        {
                            action_snapshots_apfs->setVisible(true);
                            action_snapshots_apfs->setEnabled(true);
                        }
                        else
                            action_snapshots_apfs->setEnabled(false);
                    }
                    else
                        action_snapshots_apfs->setEnabled(false);

                    bool_complete_source_for_hex_view = false;
                    source_count_name_for_root_node.clear();

                    if((src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
                        || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal)
                        || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal)
                        || src_type_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal))
                            && (fs_type.toLower() != QString(MACRO_FILE_SYSTEM_TYPE_APFS)))
                    {
                        menu_disk_hex_viewer->setEnabled(true);
                        action_disk_hex_viewer->setEnabled(true);
                        action_carve_unallocated_space->setEnabled(true);
                    }

                    action_add_to_indexing->setEnabled(true);

                    menu_case_tree_sources->exec(QCursor::pos());
                }
                else if(item->text(enum_global_case_tree_display_column_SECOND).trimmed() == QString(MACRO_CASE_TREE_Parent_Source))
                {

                    /// works for iOS source directories only
                    //struct_GLOBAL_derived_source_info struct_source_info = global_target_sources_info_class_obj->pub_get_target_info_structure_according_source_count_name(item->text(1).trimmed());
                    // QString fs_type = struct_source_info.file_system_type;

                    action_add_to_indexing->setEnabled(true);

                    menu_case_tree_sources->exec(QCursor::pos());
                }
            }
            //            else if(item->text(2).trimmed() == QString(MACRO_CASE_TREE_PARENT_SOURCE) && !item->text(1).trimmed().isEmpty())
            //            {
            //                struct_GLOBAL_derived_source_info struct_source_info = global_target_m_sources_info_obj->pub_get_target_info_structure_according_source_count_name(item->text(1).trimmed());
            //                QString fs_type = struct_source_info.file_system_type;

            //                action_carve_unallocated_space->setEnabled(false);
            //                bool_complete_source_for_hex_view = false;
            //                source_count_name_for_root_node.clear();
            //                action_disk_hex_viewer->setEnabled(false);

            //                if(fs_type.trimmed() == QString(MACRO_FILE_SYSTEM_TYPE_APFS))
            //                    action_snapshots_apfs->setEnabled(true);
            //                else
            //                    action_snapshots_apfs->setEnabled(false);


            //                menu_fs_add_to_text_indexing->exec(QCursor::pos());
            //            }
            else if(item->parent()->text(enum_global_case_tree_display_column_ZERO).trimmed().startsWith(MACRO_CASE_TREE_Carved_Files))
            {
                menu_delete_from_tree->exec(QCursor::pos());
            }
            else if(item->text(enum_global_case_tree_display_column_FIRST).trimmed() == QString(MACRO_CASE_TREE_Story_Board) && item->text(enum_global_case_tree_display_column_SECOND).trimmed() != QString(MACRO_CASE_TREE_Story_Board_Parent))
            {
                menu_story_board->exec(QCursor::pos());
            }
            else if(item->parent()->text(enum_global_case_tree_display_column_ZERO).trimmed().startsWith(MACRO_CASE_TREE_Registry_Viewer))
            {
                menu_delete_from_tree->exec(QCursor::pos());
            }

        }
        //        else if(tree_type == MACRO_ACTION_MENU_TREE_KEYWORD_SEARCH_int)
        //        {
        //            menu->exec(QCursor::pos());
        //        }
    }

}


void R_Treewidget::slot_action_snapshots_apfs_clicked()
{
    QTreeWidgetItem* item = currentItem();
    emit signal_right_click_snapshot_clicked(item->text(1).trimmed());

}


void R_Treewidget::keyPressEvent( QKeyEvent *event )
{
    if(event->key() == Qt::Key_Up )
    {
        selectionModel()->setCurrentIndex( indexAbove(currentIndex()), QItemSelectionModel::NoUpdate );
    }
    else if(event->key() == Qt::Key_Down )
    {
        selectionModel()->setCurrentIndex( indexBelow(currentIndex()), QItemSelectionModel::NoUpdate );
    }
    else
    {
        QTreeWidget::keyPressEvent(event);
    }
}

bool R_Treewidget::check_fs_type_supported_for_carve_unallocated(QString fs_type)
{
    bool is_supported = false;
    fs_type = fs_type.trimmed();

    //ntfs can be ud_ntfs or something else so contains check used

    if(fs_type == QString(MACRO_FILE_SYSTEM_TYPE_MSDOS) ||
            fs_type == QString(MACRO_FILE_SYSTEM_TYPE_EXFAT) ||
            fs_type == QString(MACRO_FILE_SYSTEM_TYPE_LIFS) ||
            fs_type.contains(QString(MACRO_FILE_SYSTEM_TYPE_NTFS), Qt::CaseInsensitive) ||
            fs_type == QString(MACRO_FILE_SYSTEM_TYPE_HFS))
    {
        is_supported = true;
    }

    return is_supported;
}

void R_Treewidget::slot_action_view_forensic_image_in_hex_viewer()
{

    if(bool_complete_source_for_hex_view)
    {
        emit signal_right_click_view_forensic_image_in_hex_viewer(source_count_name_for_root_node, bool_complete_source_for_hex_view);
    }
}
