#ifndef HASHSET_DISPLAY_H
#define HASHSET_DISPLAY_H

#include <QDialog>
#include <QtCore>
#include <QtSql>
#include "ui_custom_classes/m_tablewidget.h"
#include <QVBoxLayout>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"                         

                                   
#include "header_identical_1.h"

 #include "message_box/message_dialog.h"
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class hashset_display;
}

class hashset_display : public QDialog
{
    Q_OBJECT

public:
    explicit hashset_display(QWidget *parent = 0);
    ~hashset_display();

    void pub_set_essentials();
    void pub_search_file_in_hash_db(QString file_path, QString source_initial_path, QString selected_db_path, QString record_number, QString plugin_name, QString tab_name, QString parent_plugin_name);
    void insert_hashset_in_result(QString filepath, QString source_initial_path, QString md5_hash, QString hash_db_path, QString record_number, QString plugin_name, QString tab_name, QString parent_plugin_name, bool is_from_other_hash_db);
    bool display_data_in_hashset_tablewidget();

    void pub_set_is_from_other_hash_database(bool value);
private:
    Ui::hashset_display *ui;


    enum enum_hashset_display_Columns{
        enum_hashset_display_INT = 0,
        enum_hashset_display_DB_Name,
        enum_hashset_display_Filename,
        enum_hashset_display_Md5,
//        enum_hashset_display_Size,
//        enum_hashset_display_Datemodified,
        enum_hashset_display_DB_Path,
        enum_hashset_display_Path
    };

    QString hashset_master_db_path;
    QString recon_result_hashset_path;
    QString recon_result_ext_hashset_path;

    bool bool_file_exist_with_same_hash;
    m_tablewidget *m_tablewidget_obj;

    message_dialog *message_dialog_object;
    bool bool_is_from_hashset_db;
    recon_helper_standard *recon_helper_standard_obj;

};

#endif // HASHSET_DISPLAY_H

