#ifndef ADD_NOTES_H
#define ADD_NOTES_H

#include <QDialog>
#include "message_box/message_dialog.h"

namespace Ui {
class add_notes;
}

class add_notes : public QDialog
{
    Q_OBJECT
    
public:
    explicit add_notes(QWidget *parent = 0);
    ~add_notes();
    void set_notes(QString data);
signals:
    void signal_notes(QString);

    
private slots:
    void on_pushButton_add_clicked();


private:
    Ui::add_notes *ui;

    message_dialog *message_dialog_object;

};

#endif // ADD_NOTES_DIALOG_H
