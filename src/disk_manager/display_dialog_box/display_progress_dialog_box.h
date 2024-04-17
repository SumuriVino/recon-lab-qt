#ifndef DISPLAY_DIALOG_BOX_H
#define DISPLAY_DIALOG_BOX_H

#include <QDialog>
#include <QDebug>
#include <QtCore>
#include <QtGui>

namespace Ui {
class display_progress_dialog_box;
}

class display_progress_dialog_box : public QDialog
{
    Q_OBJECT

public:
    explicit display_progress_dialog_box(QWidget *parent = 0);
    ~display_progress_dialog_box();

    void pub_hide_cancel_button();
    void pub_show_cancel_button();
    void pub_disable_cancel_button(bool status);

    void pub_set_label_message(QString str);

    void pub_initialize_progress_bar();
    void pub_set_progres_value(int percent);

signals:
    void signal_cancel_button_clicked();
private slots:
    void on_pushButton_cancel_clicked();

private:
    Ui::display_progress_dialog_box *ui;


};

#endif // DISPLAY_DIALOG_BOX_H
