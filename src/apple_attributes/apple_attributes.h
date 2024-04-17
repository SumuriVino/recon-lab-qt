#ifndef APPLE_ATTRIBUTES_H
#define APPLE_ATTRIBUTES_H

#include <QFrame>
#include<QStandardItemModel>
#include<QtCore>
#include<QtSql>
#include"recon_generic_functions/recon_static_functions.h"
                                   
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class apple_attributes;
}

class apple_attributes : public QFrame
{
    Q_OBJECT

public:
    explicit apple_attributes(QWidget *parent = 0);
    ~apple_attributes();

    void set_essentials();

    void pub_set_page_caller(QString caller_name);
signals:

    void signal_workdone_complete(bool something_changed);

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::apple_attributes *ui;

    enum enum_apple_attributes{
        enum_apple_attributes_Bookmark = 0,
        enum_apple_attributes_INT,
        enum_apple_attributes_Name,
        enum_apple_attributes_Description,
        enum_apple_attributes_Command,
        enum_apple_attributes_Custom_Column_Name,
        enum_apple_attributes_Blank_Column

    };

    QString destination_db_Path;
    void display_attributes();
    QStandardItemModel *item_model;

    QString page_caller;

    recon_helper_standard *recon_helper_standard_obj;

};

#endif //
