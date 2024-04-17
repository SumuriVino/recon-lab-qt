#ifndef SOURCE_WIDGETS_H
#define SOURCE_WIDGETS_H

#include <QFrame>
#include <QBitmap>
#include <QFileDialog>
#include "header_identical_1.h"
#include "source_evidencer.h"
#include "header_global_variables_3.h"
#include <QMessageBox>


namespace Ui {
class source_widgets;
}

class source_widgets : public QFrame
{
    Q_OBJECT

public:
    explicit source_widgets(QWidget *parent = 0);
    ~source_widgets();

    void pub_set_widget_id(int id);
    void pub_set_root_type(QString name);
    QString pub_get_root_type(void);
    void pub_set_source_type_internal(QString name);
    QString pub_get_source_type_internal(void);

    void pub_set_source_type_display(QString name);
    QString pub_get_source_type_display();


    void pub_set_media_path(QString path);
    void pub_ask_for_eveidence_no(QString source_path);

    int pub_get_id();
    int pub_get_window_height();

    void pub_set_password_string(QString passwd);
    QString pub_get_password_string();

    void pub_set_username_string(QString);
    QString pub_get_user_name_string();

    QString pub_get_path_1_string();
    QString pub_get_path_2_string();

    struct_global_source_evidencer get_source_evidence_info();

    void pub_set_is_load_case(bool status);
    void pub_set_root_count_name(QString source_count_name);
    QString pub_get_root_count_name();

    void pub_set_evidence_type(QString evidence_type);
    QString pub_get_evidence_type();

signals:
    void signal_remove_button_clicked(int,QString);

private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_repoint_clicked();
    void on_pushButton_repoint_2_clicked();

private:
    Ui::source_widgets *ui;
    int received_id;
    QString received_root_type;
    QString received_source_type_internal;
    QString received_source_type_display;

    QString password_string;
    QString username_string;

    bool bool_is_load_case;

    source_evidencer *source_evidencer_obj;
    QString received_root_count_name;

    QString recevied_evidence_type_string;

    QFileDialog file_dialog_obj;

};

#endif // SOURCE_WIDGETS_H
