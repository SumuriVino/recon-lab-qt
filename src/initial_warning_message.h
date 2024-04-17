#ifndef INITIAL_WARNING_MESSAGE_H
#define INITIAL_WARNING_MESSAGE_H

#include <QDialog>
#include <QtCore>
#include "recon_generic_functions/recon_static_functions.h"

namespace Ui {
class initial_warning_message;
}

class initial_warning_message : public QDialog
{
    Q_OBJECT


public:
    explicit initial_warning_message(QWidget *parent = 0);
    ~initial_warning_message();
    void pub_show();

private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::initial_warning_message *ui;
};

#endif // INITIAL_WARNING_MESSAGE_H
