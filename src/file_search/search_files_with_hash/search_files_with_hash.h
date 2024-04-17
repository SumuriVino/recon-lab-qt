#ifndef SEARCH_FILE_WITH_HASH_H
#define SEARCH_FILE_WITH_HASH_H

#include <QWidget>
#include <QUrl>
#include <QtCore>
#include <QLabel>
#include <QCoreApplication>
#include <QMessageBox>
#include <QDialog>
#include "narad_muni/narad_muni.h"
#include "header_identical_3.h"
#include "global_connection_manager.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"

namespace Ui {
class search_files_with_hash;
}

class search_files_with_hash : public QWidget
{
    Q_OBJECT

public:
    explicit search_files_with_hash(QWidget *parent = nullptr);
    ~search_files_with_hash();

    void pub_set_global_connection_manager_obj(global_connection_manager *obj);
    void pub_set_display_loading_progress_bar_obj(display_loading_progress_bar *object);
    void pub_refresh_tableWidget();
    void pub_search_files_having_same_hash(QString hash_md5, QString record, QString source_count_name, QString com_file_path);

private slots:
    void slot_open_in_source_link_clicked(QString curr_row);


private:
    Ui::search_files_with_hash *ui;

    enum enum_search_hashes_display
    {
        enum_Search_Hashes_display_Record = 0,
        enum_Search_Hashes_display_File_Name,
        enum_Search_Hashes_display_File_Path,
        enum_Search_Hashes_display_Source_Name,
        enum_Search_Hashes_display_Hash_MD5,
        enum_Search_Hashes_display_Source_Count_Name,
        enum_Search_Hashes_display_OS_Naming_Scheme,
        enum_Search_Hashes_display_Open_in_Source_Link
    };

    global_connection_manager *global_connection_manager_obj;
    recon_helper_standard     *recon_helper_standard_obj;
    display_loading_progress_bar *display_loading_progress_bar_obj;

    void display_list_of_files_having_same_hash(QString record, QString file_name , QString file_path , QString source_name , QString hash_md5 , QString source_c_name , QString os_naming_scheme);
};

#endif // SEARCH_FILE_WITH_HASH_H
