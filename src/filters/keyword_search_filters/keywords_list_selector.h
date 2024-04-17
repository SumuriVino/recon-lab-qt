#ifndef KEYWORDS_LIST_SELECTOR_H
#define KEYWORDS_LIST_SELECTOR_H

#include <QWidget>
#include <QFrame>
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtCore>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_identical_2.h"
#include "message_box/message_dialog.h"
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class keywords_list_selector;
}

class keywords_list_selector : public QDialog
{
    Q_OBJECT

public:
    explicit keywords_list_selector(QWidget *parent = 0);
    ~keywords_list_selector();

    void pub_display_keyword_list();


signals:
    void signal_pushbotton_done_clicked();

private slots:
    void on_pushButton_done_clicked();

    void on_lineEdit_insert_list_returnPressed();

    void on_lineEdit_insert_keywords_returnPressed();

    void on_pushButton_list_add_clicked();

    void on_pushButton_keyword_add_clicked();

    void on_tableWidget_keyword_list_cellClicked(int row, int column);

    void on_pushButton_list_remove_clicked();

    void on_pushButton_list_edit_clicked();

    void on_pushButton_keyword_remove_clicked();

    void on_pushButton_keyword_edit_clicked();

    void on_tableWidget_keyword_cellClicked(int row, int column);

    void on_pushButton_cancel_clicked();

private:
    Ui::keywords_list_selector *ui;


    void list_database_manager_function(int operation_type);
    void keywords_database_manager_function(int operation_type);
    void display_keyword_list_table();

    bool bool_delete_all_keywords_of_list_item;

    bool bool_update_keyword_list_item;
    bool bool_keyword_updated;

    int selected_keywords_row;
    int selected_list_row;

    message_dialog *message_dialog_object;
    recon_helper_standard *recon_helper_standard_obj;

};

#endif // KEYWORDS_LIST_SELECTOR_H
