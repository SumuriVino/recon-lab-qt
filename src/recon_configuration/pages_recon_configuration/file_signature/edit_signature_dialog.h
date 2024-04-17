#ifndef EDIT_SIGNATURE_DIALOG_H
#define EDIT_SIGNATURE_DIALOG_H

#include <QDialog>
#include "header_global_variables_3.h"


namespace Ui {
class edit_signature_dialog;
}

class edit_signature_dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit edit_signature_dialog(QWidget *parent = 0);
    ~edit_signature_dialog();
    void set_laber_header(QString header, QString footer, QString offset);

signals:
    void signal_signature_edit(QString label ,QString signature , QString offset);
private slots:


    void on_pushButton_add_edit_signature_clicked();

private:
    Ui::edit_signature_dialog *ui;
};

#endif // EDIT_SIGNATURE_DIALOG_H
