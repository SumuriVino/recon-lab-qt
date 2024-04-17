#ifndef DECOMPRESS_FILE_SYSTEM_H
#define DECOMPRESS_FILE_SYSTEM_H

#include <QObject>
#include <QWidget>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"                         
                                   
#include "header_global_structures.h"
#include "header_global_variables_1.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "message_box/message_dialog.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "header_global_variables_2.h"

#include "ui_custom_classes/r_treewidget.h"
#include "recon_generic_functions/recon_helper_process.h"    


class decompress_file_system : public QWidget
{
    Q_OBJECT
public:
    explicit decompress_file_system(QWidget *parent = nullptr);
    ~decompress_file_system();

    void pub_set_db_essentials(QString db_path, QSqlDatabase destination_db);
    void pub_set_source_count_name(QString src_cnt_name);
    void pub_set_record_no_for_decompression(QString record_no);

    QString start_run();
    void pub_set_treewidget_case_display_obj(R_Treewidget *obj);
    void pub_set_decompress_file_path(QString file_path);
signals:

    void signal_close_loading_display_dialog(bool status);

public slots:

private slots:
    void slot_hide_loading_display_dialog_box(bool status);

private:


    QString received_source_count_name, received_record_no;

    QSqlDatabase destination_db_obj;

    bool bool_cancel_loading;
    display_loading_progress_bar *display_loading_progress_bar_obj;
    display_loading_progress_bar *display_loading_progress_bar_non_cancelable_obj;

    message_dialog  *message_dialog_obj;
    recon_helper_standard *recon_helper_standard_obj;
    recon_helper_process *recon_helper_process_obj;

    void check_for_other_sources(QString display_path, QString info_path);

    R_Treewidget *treewidget_case_display_obj;
    void update_record_count_for_file_system_in_case_tree();
    void update_record_count_for_mobile_backup_in_case_tree();
    void update_record_count_for_disk_images_in_case_tree();

    QString decompress_file_path;
};

#endif // DECOMPRESS_FILE_SYSTEM_H
