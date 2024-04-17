#ifndef EXISTING_HASH_COLUMN_SELECTION_H
#define EXISTING_HASH_COLUMN_SELECTION_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QSqlDatabase>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QRadioButton>
#include <QPushButton>
#include <QSplitter>
#include <QtSql>

#include "message_box/message_dialog.h"
#include "recon_generic_functions/recon_static_functions.h"


namespace Ui {
class existing_hash_column_selection;
}

class existing_hash_column_selection : public QDialog
{
    Q_OBJECT

public:
    explicit existing_hash_column_selection(QWidget *parent = 0);
    ~existing_hash_column_selection();


    void pub_select_file();
    void pub_select_file_for_hashset_toolbar();
    void pub_set_hashset_toolbar_essentials(bool is_sqlite, bool is_prohect_vic);
signals:
    void signal_pushbutton_save_clicked(QString saved_data_str);
    void signal_pushbutton_cancel_clicked();
    void signal_selected_hash_database_file(QString filepath);
    void signal_selected_vic_supported_file(QString filepath);

private slots:

    void slot_table_item_clicked(QTableWidgetItem *item);
    void slot_pushbutton_save_clicked(bool status);
    void slot_pushbutton_cancel_clicked(bool status);

private:
    Ui::existing_hash_column_selection *ui;

    QPushButton *pushbutton_save;
    QPushButton *pushbutton_cancel;

    QTableWidget *table_widget_Tables;
    QTableWidget *table_widget_sqlite_data;

    message_dialog *message_dialog_obj;

    QRadioButton *radio_button_key_col;
    void prepare_sqlite_file_viewer(QString clicked_table_name);
    QString sqlite_file_path;


    void create_gui();
    void create_table_display();

    QFileDialog file_dialog_obj;

    bool bool_hashset_toolbar_is_sqlite = false;
    bool bool_hashset_toolbar_is_project_vic = false;

};

#endif //
