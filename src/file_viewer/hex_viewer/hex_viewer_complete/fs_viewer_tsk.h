#ifndef FS_VIEWER_TSK_H
#define FS_VIEWER_TSK_H

#include <QObject>
#include <QWidget>
#include <QSqlDatabase>
#include <QtCore>
#include <QtGui>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QSql>
#include <QSqlQuery>
#include <QFileIconProvider>
#include <QAction>
#include <QMenu>
#include "recon_generic_functions/recon_static_functions.h"

#define ROOT_DIRECTORY "$ROOT DIRECTORY"

class fs_viewer_tsk : public QWidget
{
    Q_OBJECT
public:
    explicit fs_viewer_tsk(QWidget *parent = nullptr);

    void pub_set_destination_database(QSqlDatabase *db);
    void pub_set_fs_type(QString fs);
    void pub_set_root_directory_address(qint64 adr);
    void pub_set_mft_address(qint64 adr);

    void pub_create_display(QString dir);

    void pub_set_partition_number(int num);

signals:
    void signal_goto_offset(qint64 offset);

    void signal_goto_fat32_file_content(int part_num, qint64 inode_number);
    void signal_goto_fat32_dollar_files_content(int part_num, QString file_name);

    void signal_goto_fat_file_content(int part_num, qint64 inode_number);
    void signal_goto_fat_dollar_files_content(int part_num, QString file_name);

    void signal_goto_ntfs_file_content(int part_num, qint64 inode_number);

    void signal_show_fat_template(int part_num, QString fat_name);

public slots:

private slots:
    void slot_tablewidget_cell_double_clicked(int row, int col);

    void slot_action_goto_directory_entry(bool);

    void slot_action_show_fat_template_triggered(bool);

private:

    enum enum_table_data_columns {
        enum_table_data_INT,
        enum_table_data_filename,
        enum_table_data_filepath,
        enum_table_data_size,
        enum_table_data_inode,
        enum_table_data_is_dir,
        enum_table_data_blank,
        enum_table_data_column_count
    };

    QTableWidget *tableWidget;

    void create_ui();

    QVBoxLayout *vbox_main;

    QSqlDatabase *destination_database;
    QFileIconProvider icon_provider;

    QSqlQuery *query;

    QString current_dir;

    QMenu right_click_menu;
    bool eventFilter(QObject *obj, QEvent *event);

    QAction *action_goto_directory_entry;

    QAction *action_show_fat_template;

    QString fs_type;
    qint64 root_directory_address = -1;
    qint64 mft_address = -1;

    int partition_number;


};

#endif // FS_VIEWER_TSK_H
