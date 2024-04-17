#ifndef LICENSE_LOCATOR_H
#define LICENSE_LOCATOR_H



#include <QDialog>
#include <QtCore>
#include <QFileDialog>
#include <QMessageBox>
#include "header_global_variables_1.h"
#include "header_identical_3.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"

namespace Ui {
class license_locator;
}

class license_locator : public QDialog
{
    Q_OBJECT

public:
    explicit license_locator(QWidget *parent = 0);
    ~license_locator();

    void closeEvent(QCloseEvent *);

    void set_executer_type(QString m_type);

    void set_gui_messages(QString);

    void pub_hide_browse_button();

    void pub_set_cancel_button_text(QString);

signals:
    void signal_waapis_jao();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_browse_clicked();

private:
    Ui::license_locator *ui;

    QString executer_type;

    QFileDialog file_dialog_obj;

    recon_helper_standard *recon_helper_standard_obj;

};

#endif // LICENSE_LOCATOR_H
