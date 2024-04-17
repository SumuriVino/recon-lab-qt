#ifndef EXPORT_AS_KML_H
#define EXPORT_AS_KML_H

#include <QDialog>
#include <QtCore>
#include "message_box/file_open_cell_widget.h"


namespace Ui {
class export_as_KML;
}

class export_as_KML : public QDialog
{
    Q_OBJECT

public:
    explicit export_as_KML(QWidget *parent = 0);
    ~export_as_KML();


    void pub_set_lable(QStringList filepath_list);

signals:

    void signal_cancel_export_as_kml();
    void signal_continue_export_as_kml();

private slots:
    void on_pushButton_continue_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::export_as_KML *ui;
};

#endif // EXPORT_AS_KML_H
