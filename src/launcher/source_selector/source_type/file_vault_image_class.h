#ifndef FILE_VAULT_IMAGE_CLASS_H
#define FILE_VAULT_IMAGE_CLASS_H

#include <QFrame>
#include <QtCore>

namespace Ui {
class File_Vault_image_Class;
}

class File_Vault_image_Class : public QFrame
{
    Q_OBJECT

public:
    explicit File_Vault_image_Class(QWidget *parent = 0);
    ~File_Vault_image_Class();

    void pub_set_id_and_name(int id,QString name);
    int pub_get_id();
    int pub_get_window_height();
signals:
    void signal_file_vault_password_inserted(QString);

private slots:

    void on_pushButton_add_password_clicked();

    void on_pushButton_cancel_clicked();


    void on_pushButton_show_hide_password_clicked(bool checked);

private:
    Ui::File_Vault_image_Class *ui;
    int received_id;
    QString received_name;

};

#endif // FILE_VAULT_IMAGE_CLASS_H
