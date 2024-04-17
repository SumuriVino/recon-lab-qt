#ifndef NEW_FILE_SIGNATURE_H
#define NEW_FILE_SIGNATURE_H

#include <QDialog>
#include <QtSql>
#include <QtGui>
#include <QtCore>

#include "header_identical_1.h"
#include "message_box/message_dialog.h"
#include "header_global_variables_3.h"


namespace Ui {
class new_file_signature;
}

class new_file_signature : public QDialog
{
    Q_OBJECT
    
public:
    explicit new_file_signature(QWidget *parent = 0);
    ~new_file_signature();
    void set_current_category(QString cat);
    void set_list_signature(QStringList list);


    void pub_set_destination_db_file(QString dp_path);
signals:
    void signal_signature_added();
private slots:
    void on_pushButton_add_file_signature_clicked();

private:
    Ui::new_file_signature *ui;
    QString current_cat;
    QStringList received_list_signature;

    message_dialog *message_dialog_object;

    QString destination_db_file;
};

#endif // NEW_FILE_SIGNATURE_H
