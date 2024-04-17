#ifndef MESSAGE_DIALOG_H
#define MESSAGE_DIALOG_H

#include <QDialog>
#include <QtCore>
#include "recon_generic_functions/recon_static_functions.h"
#include "message_box/file_open_cell_widget.h"

namespace Ui {
class message_dialog;
}

class message_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit message_dialog(QWidget *parent = 0);
    ~message_dialog();

    void pub_set_message(QString msg_type,QString msg);
    void pub_set_message(QString windo_title, QString msg_type,QString msg);
    void pub_set_minimum_window_size(int w, int h);
    void pub_set_file_path_message(QString msg_type, QStringList path_list, QString export_dir_path);
    void pub_set_file_path_message_for_global_report(QString msg_type, QStringList path_list);
    void pub_set_multiple_message_list(QString msg_type, QStringList message_list);
    void pub_set_window_frameless(bool status);

signals:
    void signal_open_report(bool value);
    void signal_yes_no_check(bool value);

private slots:
    void on_pushButton_yes_clicked();

    void on_pushButton_no_clicked();

private:
    Ui::message_dialog *ui;
    QString message;
    QString message_type;

    void set_message(QString msg_type,QString msg);

};

#endif // MESSAGE_DIALOG_H
