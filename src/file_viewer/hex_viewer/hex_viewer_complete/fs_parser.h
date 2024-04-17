#ifndef FS_PARSER_H
#define FS_PARSER_H

#include <QObject>
#include <tsk/libtsk.h>
#include <QtCore>
#include <QtSql>
#include <QCoreApplication>
#include "file_viewer/hex_viewer/hex_viewer_complete/common_header.h"
#include "recon_generic_functions/recon_static_functions.h"


TSK_WALK_RET_ENUM fs_function_walk(TskFsFile * a_fs_file, const char *a_path, void *a_ptr);

class fs_parser : public QObject
{
    Q_OBJECT
public:
    explicit fs_parser(QObject *parent = nullptr);

    void pub_set_image_path(QString);

    void pub_set_output_directory(QString);

    void pub_read_image();

    int pub_get_file_data(QString, int size, unsigned char *buf);

    bool pub_is_image_has_MBR();

    QList<partition_detail> pub_get_partition_details();

signals:
    void signal_work_finished();

public slots:

    void slot_start_processing();
private:
    QString image_path;

    QString output_dir;

    TskImgInfo *tsk_img_info;

    TskVsInfo *tsk_vs_info;

    TskFsInfo *tsk_fs_info;

    QList<partition_detail> list_tsk_part_info;

    int open_image();

    void open_vs();

    void open_all_fs();

    bool bool_no_vs;

    QString dest_db_file_path;

    void create_file_list(TskFsInfo *fs_info, QSqlDatabase &db);

    QList<file_detail> all_file_details;

    QList<struct_hex_viewer_markers> markers_list;

    int progress_counter = 0;

    int partition_counter = 0;

};

#endif // FS_PARSER_H
