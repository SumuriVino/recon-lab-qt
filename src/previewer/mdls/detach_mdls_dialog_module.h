#ifndef DETACH_MDLS_DIALOG_module_H
#define DETACH_MDLS_DIALOG_module_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <QTreeWidget>
#include <QToolTip>
#include "recon_generic_functions/recon_static_functions.h"



namespace Ui {
class detach_mdls_dialog_module;
}

class detach_mdls_dialog_module : public QDialog
{
    Q_OBJECT

public:
    explicit detach_mdls_dialog_module(const QString &title,QWidget *parent = 0);
    ~detach_mdls_dialog_module();

    void set_db_file_loaction(QString file_location);
    void set_INT_value(QString int_value);
    void set_category_name(QString category);
    void set_window_title(QString module_naam);
    void set_table_name(QString my_table);
    void display_detach_apple_metadata(QString received_path);

    virtual void closeEvent(QCloseEvent *);

private slots:
    void on_treeWidget_detach_apple_metadata_itemClicked(QTreeWidgetItem *item, int column);

signals:
    void signal_detach_mdls_dialog_window_closed();

private:
    Ui::detach_mdls_dialog_module *ui;

    enum enum_detach_mdls_dialog_module{
        enum_detach_mdls_dialog_module_META_BOOKMARK,
         enum_detach_mdls_dialog_module_META_FIELD,
         enum_detach_mdls_dialog_module_META_VALUE,
         enum_detach_mdls_dialog_module_META_DESC
    };

    QString db_file_location_recieved;
    QString INT_value_recieved;
    QString category_name_recieved;
    QString module_name_recieved;
    QString table_name_recieved;

     QString w_title;
};


#endif // DETACH_MDLS_DIALOG_H
