#ifndef SYSTEM_PASSWORD_ASKER_H
#define SYSTEM_PASSWORD_ASKER_H

#include <QDialog>
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "recon_generic_functions/recon_helper_process.h"    
#include <qnamespace.h>

namespace Ui {
class system_password_asker;
}

class system_password_asker : public QDialog
{
    Q_OBJECT

public:
    explicit system_password_asker(QWidget *parent = 0);
    ~system_password_asker();

    void closeEvent(QCloseEvent *);

    void pub_set_essentials();

    void pub_set_window_modality();

    void pub_set_label_msg(QString text);

signals:
    void signal_system_password_asker_workdone(bool pwd_status, QString typed_pwd);

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_show_hide_password_clicked(bool checked);

    void on_lineEdit_pwd_textChanged(const QString &arg1);

    void on_lineEdit_pwd_returnPressed();

private:
    Ui::system_password_asker *ui;

    recon_helper_standard *recon_helper_standard_obj;
    recon_helper_process *recon_helper_process_obj;

};

#endif // SYSTEM_PASSWORD_ASKER_H
