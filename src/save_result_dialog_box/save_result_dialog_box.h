#ifndef SAVE_RESULT_DIALOG_BOX_H
#define SAVE_RESULT_DIALOG_BOX_H

#include <QDialog>
#include <QDebug>
namespace Ui {
class save_result_dialog_box;
}

class save_result_dialog_box : public QDialog
{
    Q_OBJECT

public:
    explicit save_result_dialog_box(QWidget *parent = 0);
    ~save_result_dialog_box();


    void pub_set_name(QString set_name);

    void closeEvent(QCloseEvent *);
signals:
    void signal_save_result_name_value(QString);
    void signal_cancel_save_results();


private slots:
    void on_pushButton_save_clicked();

private:
    Ui::save_result_dialog_box *ui;
    QString set_save_text;

};

#endif // SAVE_RESULT_DIALOG_BOX_H
