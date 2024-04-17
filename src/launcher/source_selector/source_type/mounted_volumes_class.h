#ifndef MOUNTED_VOLUMES_CLASS_H
#define MOUNTED_VOLUMES_CLASS_H

#include <QFrame>
#include <QtCore>
#include "header_global_variables_3.h"                         


namespace Ui {
class Mounted_Volumes_Class;
}

class Mounted_Volumes_Class : public QFrame
{
    Q_OBJECT

public:
    explicit Mounted_Volumes_Class(QWidget *parent = 0);
    ~Mounted_Volumes_Class();

    void pub_set_id_and_name(int id, QString name);
    int pub_get_id();
    int pub_get_window_height();
    void pub_set_essentials();


signals:
    void signal_mounted_volumes_selected(QStringList volumes_list);

private slots:
    void on_pushButton_Add_clicked();

    void on_pushButton_Cancel_clicked();

private:
    Ui::Mounted_Volumes_Class *ui;
    int received_id;
    QString received_name;
    void prepare_mounted_volumes_list();

};

#endif // MOUNTED_VOLUMES_CLASS_H
