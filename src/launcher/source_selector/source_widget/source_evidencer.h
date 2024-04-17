#ifndef SOURCE_EVIDENCER_H
#define SOURCE_EVIDENCER_H

#include <QDialog>
#include "message_box/message_dialog.h"
#include "header_global_variables_3.h"

namespace Ui {
class source_evidencer;
}

class source_evidencer : public QDialog
{
    Q_OBJECT

public:
    explicit source_evidencer(QWidget *parent = 0);
    ~source_evidencer();

    void set_source_message(QString message);

    struct_global_source_evidencer get_evicencer_info();

    void closeEvent(QCloseEvent *);

    bool pub_check_evidence_opertion_status();
    void pub_hide_cancel_button(bool status);
    void pub_set_evidence_number(QString source_file_path);
    void pub_disable_evidence_no_and_description_widgets(bool status);
    void pub_set_evidence_no_and_description(QString evidence_no, QString description);

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_src_password_show_hide_clicked(bool checked);

private:
    Ui::source_evidencer *ui;

    message_dialog *message_dialog_obj;

    bool bool_evidence_operation_ok = false;

    void reject();
};

#endif // SOURCE_EVIDENCER_H
