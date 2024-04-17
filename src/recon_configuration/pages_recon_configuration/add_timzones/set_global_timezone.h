#ifndef SET_GLOBAL_TIMEZONE_H
#define SET_GLOBAL_TIMEZONE_H

#include <QWidget>
#include <QtCore>
#include "recon_generic_functions/recon_static_functions.h"
#include "message_box/message_dialog.h"

namespace Ui {
class set_global_timezone;
}

class set_global_timezone : public QWidget
{
    Q_OBJECT

public:
    explicit set_global_timezone(QWidget *parent = 0);
    ~set_global_timezone();

    void pub_set_timezone(QList <QString> qlist_obj);
    void pub_edit_timezone(QList <QString> qlist_obj, QString last_tmzn_str, QString last_start_str, QString last_end_str);

signals:
   void signal_set_time_zone(qint64 start_date_epch , qint64 end_date_epch, QString timezone , QString start_date_str, QString end_date_str);
   void signal_on_pushbutton_cancel_clicked();
   void signal_edit_time_zone(qint64 start_date_epch , qint64 end_date_epch, QString timezone , QString start_date_str, QString end_date_str);

private slots:

    void on_pushButton_cancel_clicked();
    void on_pushButton_OK_clicked();

private:
    Ui::set_global_timezone *ui;
    message_dialog *message_dialog_obj;

    bool bool_edit_button_clicked;
};

#endif // SET_GLOBAL_TIMEZONE_H


