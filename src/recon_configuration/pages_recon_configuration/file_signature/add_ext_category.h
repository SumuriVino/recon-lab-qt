#ifndef ADD_EXT_CATEGORY_H
#define ADD_EXT_CATEGORY_H

#include <QDialog>
#include <QtSql>
#include <QtGui>
#include <QtCore>
#include "message_box/message_dialog.h"
                                   



namespace Ui {
class add_ext_category;
}

class add_ext_category : public QDialog
{
    Q_OBJECT
    
public:
    explicit add_ext_category(QWidget *parent = 0);
    ~add_ext_category();
    void set_list_signature_category(QStringList list);

    void clear_lineedit();

    void pub_set_destination_db_file(QString dp_path);
signals:
    void signal_category_added();

private slots:
    void on_pushButton_add_category_clicked();

private:
    Ui::add_ext_category *ui;
    QStringList received_list_signature_category;
    message_dialog *message_dialog_object;

    QString destination_db_file;
};

#endif // ADD_EXT_CATEGORY_H
