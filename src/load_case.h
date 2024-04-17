#ifndef LOAD_CASE_H
#define LOAD_CASE_H

#include <QDialog>
#include <QtCore>
#include <QFileDialog>
#include <QtSql>
#include "recon_generic_functions/recon_static_functions.h"

#include "message_box/message_dialog.h"


namespace Ui {
class load_case;
}

class load_case : public QDialog
{
    Q_OBJECT

public:
    explicit load_case(QWidget *parent = 0);
    ~load_case();

signals:
    void signal_loadcase_launched(QString path);

private slots:
    void slot_load_case_button_clicked();
    void slot_load_case_from_launcher_clicked(QString path);
private:
    Ui::load_case *ui;

    message_dialog *message_dialog_object;
};

#endif // LOAD_CASE_H
