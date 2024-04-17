#include "fs_viewer_tsk.h"

fs_viewer_tsk::fs_viewer_tsk(QWidget *parent) : QWidget(parent)
{

    create_ui();

    installEventFilter(this);

    tableWidget->setColumnWidth(enum_table_data_filename, 200);
    tableWidget->setColumnWidth(enum_table_data_filepath, 250);

}

void fs_viewer_tsk::pub_set_destination_database(QSqlDatabase *db)
{
    destination_database = db;

    query = new QSqlQuery(*destination_database);
}

void fs_viewer_tsk::pub_set_fs_type(QString fs)
{
    fs_type = fs;

    if(fs_type.contains(MACRO_FILE_SYSTEM_TYPE_FAT, Qt::CaseInsensitive))
    {
        action_goto_directory_entry =  right_click_menu.addAction("Goto Directory entry");
        connect(action_goto_directory_entry, SIGNAL(triggered(bool)), this, SLOT(slot_action_goto_directory_entry(bool)));

        action_show_fat_template =  right_click_menu.addAction("Show FAT data");
        connect(action_show_fat_template, SIGNAL(triggered(bool)), this, SLOT(slot_action_show_fat_template_triggered(bool)));

        tableWidget->hideColumn(enum_table_data_inode);
        tableWidget->hideColumn(enum_table_data_is_dir);
    }
    else if(fs_type.contains(MACRO_FILE_SYSTEM_TYPE_NTFS, Qt::CaseInsensitive))
    {
        action_goto_directory_entry =  right_click_menu.addAction("Goto $MTF entry");
        connect(action_goto_directory_entry, SIGNAL(triggered(bool)), this, SLOT(slot_action_goto_directory_entry(bool)));

//        action_show_fat_template =  right_click_menu.addAction("Show FAT data");
//        connect(action_show_fat_template, SIGNAL(triggered(bool)), this, SLOT(slot_action_show_fat_template_triggered(bool)));

        tableWidget->hideColumn(enum_table_data_inode);
        tableWidget->hideColumn(enum_table_data_is_dir);
    }

}

void fs_viewer_tsk::pub_set_root_directory_address(qint64 adr)
{
    root_directory_address = adr;
}

void fs_viewer_tsk::pub_set_mft_address(qint64 adr)
{
    mft_address = adr;
}

bool fs_viewer_tsk::eventFilter(QObject *obj, QEvent *event)
{

    if(fs_type.contains(MACRO_FILE_SYSTEM_TYPE_FAT, Qt::CaseInsensitive))
    {
        action_show_fat_template->setDisabled(true);
    }
    action_goto_directory_entry->setDisabled(false);

    if(tableWidget->currentRow() < 0)
        return false;

    QTableWidgetItem *item_filename = tableWidget->item(tableWidget->currentRow(), enum_table_data_filename);

    if(item_filename == NULL)
        return false;

    QString file_name = item_filename->text();

    if(fs_type.contains(MACRO_FILE_SYSTEM_TYPE_FAT, Qt::CaseInsensitive) && (file_name.startsWith("$MBR") || file_name.startsWith(ROOT_DIRECTORY)))
        return false;

    if(fs_type.contains(MACRO_FILE_SYSTEM_TYPE_FAT, Qt::CaseInsensitive) && file_name.startsWith("$FAT"))
    {
        action_show_fat_template->setDisabled(false);
        action_goto_directory_entry->setDisabled(true);
    }



    if(event->type() == QEvent::ContextMenu)
    {
        if(tableWidget->currentRow() >= 0 && tableWidget->currentRow() < tableWidget->rowCount())
        {
            right_click_menu.exec(QCursor::pos());
            return true;

        }
    }
    return false;

}

void fs_viewer_tsk::pub_create_display(QString dir)
{

    tableWidget->setRowCount(0);
    int row = 0;

    current_dir = dir;
    if(current_dir == "/" && fs_type.contains(MACRO_FILE_SYSTEM_TYPE_FAT, Qt::CaseInsensitive))
    {
        tableWidget->insertRow(row);
        tableWidget->setItem(row, enum_table_data_INT, new QTableWidgetItem(""));

        QTableWidgetItem *item_filename = new QTableWidgetItem(ROOT_DIRECTORY);

        item_filename->setIcon(icon_provider.icon(QFileIconProvider::File));

        tableWidget->setItem(row, enum_table_data_filename, item_filename);
        tableWidget->setItem(row, enum_table_data_filepath, new QTableWidgetItem("/"));
        tableWidget->setItem(row, enum_table_data_size, new QTableWidgetItem(""));
        tableWidget->setItem(row, enum_table_data_inode, new QTableWidgetItem(""));
        tableWidget->setItem(row, enum_table_data_is_dir, new QTableWidgetItem(""));
        tableWidget->setItem(row, enum_table_data_blank, new QTableWidgetItem(""));

        row++;
    }


    QString command = "select INT,filename, inode_no, file_parent_dir, size, m_access_time, m_modified_time,m_change_time, is_dir from files where file_parent_dir='"
            + dir + "'";

    if(!query->exec(command))
    {
        recon_static_functions::app_debug("Select query execute -----FAILED----- " + query->executedQuery(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error: " + query->lastError().text(), Q_FUNC_INFO);
        return;
    }


    while(query->next())
    {
        tableWidget->insertRow(row);
        tableWidget->setItem(row, enum_table_data_INT, new QTableWidgetItem(query->value("INT").toString()));
        QTableWidgetItem *item_filename = new QTableWidgetItem(query->value("filename").toString());
        if(query->value("is_dir").toString() == "1")
            item_filename->setIcon(icon_provider.icon(QFileIconProvider::Folder));
        else
            item_filename->setIcon(icon_provider.icon(QFileIconProvider::File));

        tableWidget->setItem(row, enum_table_data_filename, item_filename);
        tableWidget->setItem(row, enum_table_data_filepath, new QTableWidgetItem(query->value("file_parent_dir").toString()));
        tableWidget->setItem(row, enum_table_data_size, new QTableWidgetItem(query->value("size").toString()));
        tableWidget->setItem(row, enum_table_data_inode, new QTableWidgetItem(query->value("inode_no").toString()));
        tableWidget->setItem(row, enum_table_data_is_dir, new QTableWidgetItem(query->value("is_dir").toString()));
        tableWidget->setItem(row, enum_table_data_blank, new QTableWidgetItem());

        row++;
    }
}

void fs_viewer_tsk::pub_set_partition_number(int num)
{
    partition_number = num;
}



void fs_viewer_tsk::slot_tablewidget_cell_double_clicked(int row, int col)
{
    if(row < 0 )
        return;


    QString is_dir = tableWidget->item(row, enum_table_data_is_dir)->text();
    QString dir_name = tableWidget->item(row, enum_table_data_filename)->text();

    if(fs_type.contains("fat32", Qt::CaseInsensitive) && is_dir != "1" )
    {
        if(dir_name.startsWith("$"))
        {
            emit signal_goto_fat32_dollar_files_content(partition_number, dir_name);
            return;
        }

        qint64 inode_num = tableWidget->item(row, enum_table_data_inode)->text().toULongLong();

        emit signal_goto_fat32_file_content(partition_number, inode_num);
        return;
    }

    if((fs_type.contains("fat16", Qt::CaseInsensitive) || fs_type.contains("fat12", Qt::CaseInsensitive))&& is_dir != "1" )
    {
        if(dir_name.startsWith("$"))
        {
            emit signal_goto_fat_dollar_files_content(partition_number, dir_name);
            return;
        }

        qint64 inode_num = tableWidget->item(row, enum_table_data_inode)->text().toULongLong();

        emit signal_goto_fat_file_content(partition_number, inode_num);
        return;
    }


    if(fs_type.contains(MACRO_FILE_SYSTEM_TYPE_NTFS, Qt::CaseInsensitive) && is_dir != "1")
    {
        qint64 inode_num = tableWidget->item(row, enum_table_data_inode)->text().toULongLong();

        emit signal_goto_ntfs_file_content(partition_number, inode_num);
        return;
    }


    if(is_dir != "1")
        return;

    QString path = tableWidget->item(row, enum_table_data_filepath)->text();

    if(dir_name == ".")
        return;

    if(dir_name == "..")
    {
        QStringList a_list = current_dir.split("/", Qt::SkipEmptyParts);

        a_list.removeLast();

        QString new_path = a_list.join("/");

        if(!new_path.startsWith("/"))
            new_path.prepend("/");

        if(!new_path.endsWith("/"))
            new_path.append("/");

        pub_create_display(new_path);
        return;
    }


    // current_dir = path + dir_name + "/";
    pub_create_display(path + dir_name + "/");

}

void fs_viewer_tsk::slot_action_goto_directory_entry(bool)
{
    int row = tableWidget->currentRow();

    if(row < 0)
        return;

    qint64 inode = tableWidget->item(row, enum_table_data_inode)->text().toInt();

    if(fs_type.contains(MACRO_FILE_SYSTEM_TYPE_FAT, Qt::CaseInsensitive))
        emit signal_goto_offset(root_directory_address + (inode - 3)*32);

    if(fs_type.contains(MACRO_FILE_SYSTEM_TYPE_NTFS, Qt::CaseInsensitive))
    {
        emit signal_goto_offset(mft_address + (inode )*1024);
    }

}

void fs_viewer_tsk::slot_action_show_fat_template_triggered(bool)
{
    QString file_name = tableWidget->item(tableWidget->currentRow(), enum_table_data_filename)->text();
    emit signal_show_fat_template(partition_number, file_name);
}

void fs_viewer_tsk::create_ui()
{
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(enum_table_data_column_count);
    tableWidget->setWordWrap(false);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(slot_tablewidget_cell_double_clicked(int,int)));

    tableWidget->setHorizontalHeaderItem(enum_table_data_INT, new QTableWidgetItem("Record No."));
    tableWidget->setHorizontalHeaderItem(enum_table_data_filename, new QTableWidgetItem("File Name"));
    tableWidget->setHorizontalHeaderItem(enum_table_data_filepath, new QTableWidgetItem("File Path"));
    tableWidget->setHorizontalHeaderItem(enum_table_data_size, new QTableWidgetItem("File Size"));
    tableWidget->setHorizontalHeaderItem(enum_table_data_inode, new QTableWidgetItem("Inode"));
    tableWidget->setHorizontalHeaderItem(enum_table_data_is_dir, new QTableWidgetItem(""));
    tableWidget->setHorizontalHeaderItem(enum_table_data_blank, new QTableWidgetItem(""));

    vbox_main = new QVBoxLayout(this);

    vbox_main->addWidget(tableWidget);
}
