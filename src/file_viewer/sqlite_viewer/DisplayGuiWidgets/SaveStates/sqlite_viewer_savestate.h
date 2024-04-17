#ifndef SQLITE_VIEWER_SAVESTATE_H
#define SQLITE_VIEWER_SAVESTATE_H


#include <QFrame>
#include<QTableWidget>
#include<QTableWidgetItem>
#include <QMessageBox>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"
#include <QFileDialog>

namespace Ui {
class sqlite_viewer_savestate;
}

class sqlite_viewer_savestate : public QFrame
{
    Q_OBJECT

public:
    explicit sqlite_viewer_savestate(QWidget *parent = 0);
    ~sqlite_viewer_savestate();

    void get_dbtable_list(QStringList tableList);
    QString destination_dir_path;

signals:
    void signal_sqlite_savestate_clicked(struct_global_sqlite_viewer_plugin_info);

private slots:
    void on_pushButton_save_clicked();
    void on_checkBox_customPath_toggled(bool checked);
    void on_pushButton_custom_path_open_clicked();

private:
    Ui::sqlite_viewer_savestate *ui;
    QStringList list_Dbtable;
    QStringList listSelectedTable;

    void display_table_data();
    struct_global_sqlite_viewer_plugin_info insert_selected_table_list_and_custom_path(void);

public slots:
    void slot_tableWidget_horizontal_header_clicked(int column);
};

#endif // SQLITE_VIEWER_SAVESTATE_H
