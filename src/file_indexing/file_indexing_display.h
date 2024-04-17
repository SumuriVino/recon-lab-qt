#ifndef FILE_INDEXING_DISPLAY_H
#define FILE_INDEXING_DISPLAY_H

#include <QWidget>
                                   
#include "header_global_variables_1.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"

namespace Ui {
class file_indexing_display;
}

class file_indexing_display : public QWidget
{
    Q_OBJECT

public:
    explicit file_indexing_display(QWidget *parent = 0);
    ~file_indexing_display();

    void pub_set_essentials();

    void pub_display_selected_files();
signals:

    void signal_done_clicked();


private slots:
    void on_pushButton_remove_clicked();
    void on_pushButton_done_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_checkBox_show_processed_files_clicked(bool checked);

private:
    Ui::file_indexing_display *ui;

    enum enum_colum_indexes
    {
        enum_bookmark_column_index = 0,
        enum_filepath_column_index,
        enum_status_column_index,
        enum_source_name_index,
        enum_fs_record_column_index,
        enum_source_count_column_index,
        enum_INT_index
    };


    QString indexing_db_path;
    recon_helper_standard *recon_helper_standard_obj;

    void prepare_selected_files_display();
};

#endif // FILE_INDEXING_DISPLAY_H
