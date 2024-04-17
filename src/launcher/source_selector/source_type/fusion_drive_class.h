#ifndef FUSION_DRIVE_CLASS_H
#define FUSION_DRIVE_CLASS_H

#include <QFrame>
#include <QFileDialog>
#include "header_identical_1.h"


namespace Ui {
class Fusion_Drive_Class;
}

class Fusion_Drive_Class : public QFrame
{
    Q_OBJECT

public:
    explicit Fusion_Drive_Class(QWidget *parent = 0);
    ~Fusion_Drive_Class();

    void pub_set_id_and_name(int id,QString name);
    int pub_get_id();
    int pub_get_window_height();
signals:

    void signal_fusion_drives_selected(QStringList);
private slots:


    void on_pushButton_add_fusion_drives_clicked();

    void on_pushButton_ssd_clicked();

    void on_pushButton_platter_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::Fusion_Drive_Class *ui;
    int received_id;
    QString received_name;

    QFileDialog file_dialog_obj;

};

#endif // FUSION_DRIVE_CLASS_H
