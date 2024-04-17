#ifndef SQLITE_VIEWER_PLUGIN_SAVESTATE_H
#define SQLITE_VIEWER_PLUGIN_SAVESTATE_H

#include <QFrame>
#include<QTableWidget>
#include<QTableWidgetItem>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"

namespace Ui {
class sqlite_viewer_plugin_savestate;
}

class sqlite_viewer_plugin_savestate : public QFrame
{
    Q_OBJECT

public:
    explicit sqlite_viewer_plugin_savestate(QWidget *parent = 0);
    ~sqlite_viewer_plugin_savestate();
    void get_dbtable_list(QStringList tableList);


signals:
    void signal_export_as_plugin(struct_global_sqlite_viewer_plugin_info);

private slots:
    void on_pushButton_save_clicked();

private:
    Ui::sqlite_viewer_plugin_savestate *ui;
    QStringList list_Dbtable;
    QStringList listSelectedTable;

    struct_global_sqlite_viewer_plugin_info insertSelectedTableList_AND_CustomPath();
    void displayTableData();
};

#endif // SQLITE_VIEWER_PLUGIN_SAVESTATE_H
