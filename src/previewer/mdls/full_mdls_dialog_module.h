#ifndef FULL_MDLS_DIALOG_module_H
#define FULL_MDLS_DIALOG_module_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <QTreeWidget>
#include <QToolTip>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"

namespace Ui {
class full_mdls_dialog_module;
}

class full_mdls_dialog_module : public QDialog
{
    Q_OBJECT

public:
    explicit full_mdls_dialog_module(const QString &title, QWidget *parent = 0);
    ~full_mdls_dialog_module();

    void set_db_file_loaction(QString file_location);
    void set_INT_value(QString int_value);
    void set_category_name(QString category);
    void set_module_name(QString module_naam);
    void set_table_name(QString my_table);
    void display_full_metadata(QString received_path);

private slots:

    void on_treeWidget_full_apple_metadata_itemClicked(QTreeWidgetItem *item, int column);
private:
    Ui::full_mdls_dialog_module *ui;


    enum enum_full_metadata_table{
        enum_full_mdls_dialog_module_META_BOOKMARK,
        enum_full_mdls_dialog_module_META_FIELD,
        enum_full_mdls_dialog_module_META_VALUE,
        enum_full_mdls_dialog_module_META_DESC
    };

    QString db_file_location_recieved;
    QString INT_value_recieved;
    QString category_name_recieved;
    QString module_name_recieved;
    QString table_name_recieved;

    QString w_title;

};

#endif // FULL_MDLS_DIALOG_H
