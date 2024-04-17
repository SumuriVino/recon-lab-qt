#ifndef HASHSET_STORAGE_H
#define HASHSET_STORAGE_H

#include <QDialog>
#include <QtCore>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QMenu>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QLineEdit>


#include "recon_generic_functions/recon_static_functions.h"
                                   
#include "header_identical_1.h"
#include "message_box/message_dialog.h"

#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"      


namespace Ui {
class hashset_storage;
}

class hashset_storage : public QDialog
{
    Q_OBJECT

public:
    explicit hashset_storage(QWidget *parent = 0);
    ~hashset_storage();

    void pub_set_essentials();
    void insert_file_details_into_hashset_database(QString file_path, QString selected_db_path, QString source_count_name);
    void insert_existing_hashset_in_master_database(QStringList list_dbs);


    void pub_set_page_caller(QString caller_name);
    void pub_display_create_hash();
    void pub_display_add_hash(QString get_db_path);


private slots:

    void slot_pushButton_open_clicked();
    void slot_lineedit_name_textChanged(QString arg);

    void slot_pushButton_ok_clicked();
    void slot_cancel_button_clicked();

signals:
    void signal_hashset_storage_workdone_clicked(QStringList, QString);
    void signal_hashset_storage_recon_conf_workdone_clicked(QString);


private:
    Ui::hashset_storage *ui;

    QString hashset_master_db_path;

    QStringList list_dbs_to_add;
    QString str_dateformat;

    void create_display_for_plugin_viewer_plugin_viewer();

    QLineEdit *lineEdit_path;
    QLineEdit *lineEdit_name;

    QPushButton *pushButton_open;
    QPushButton *pushButton_create_hashset;

    QLabel *label_name;
    QLabel *label_path;

    QVBoxLayout *vbox_add_hash_lineedit_layout;
    QString page_caller;

    //QVBoxLayout *vbox_layout_add_existing;

    ///hashset
    QLabel *label_existence;

    QWidget *widget_create_launcher_button;
    QWidget *widget_add_launcher_button;
    message_dialog *message_dialog_object;


    QString hashset_default_dbpath;
    QString hashset_default_db_initial_path;
    recon_helper_standard *recon_helper_standard_obj;


};


#endif // HASHSET_STORAGE_H
