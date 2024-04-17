#ifndef GET_ADDRESS_DIALOG_H
#define GET_ADDRESS_DIALOG_H

#include <QDialog>
#include <QtCore>
#include "common_header.h"

#include <QMessageBox>

namespace Ui {
class get_address_dialog;
}

class get_address_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit get_address_dialog(QWidget *parent = 0);
    ~get_address_dialog();

signals:
    void signal_jump_to_offset(qint64 offset, int);
private slots:
    void on_pushButton_clicked();

private:
    Ui::get_address_dialog *ui;
};

#endif // GET_ADDRESS_DIALOG_H
