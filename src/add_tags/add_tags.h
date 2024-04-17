#ifndef ADD_TAGS_H
#define ADD_TAGS_H

#include <QDialog>

#include "header_csv_template.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "message_box/message_dialog.h"
                                   
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class add_tags;
}

class add_tags : public QDialog
{
    Q_OBJECT

public:
    explicit add_tags(QWidget *parent = 0);
    ~add_tags();

     QString tags_db_path;
     void pub_set_essentials();

     QStringList get_tags_and_colours_list_from_db(QString command, QString destination_db_file);

private slots:
    void on_pushButton_save_tag_clicked();


signals:
    void signal_tags(QString,QString);
    void tag_status(bool,QString,QString);

private:
    Ui::add_tags *ui;

    void add_new_tag_to_tags_database(QString new_tag, QString colour);
    void fill_colours_to_select();


    message_dialog *message_dialog_object;
    recon_helper_standard *recon_helper_standard_obj;

    QString get_color_hex_from_colour_name(QString colour, QString destination_db_path, QString caller_func);
};

#endif // ADD_TAGS_H
