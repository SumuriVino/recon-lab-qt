#ifndef TAG_CREATE_POPUP_H
#define TAG_CREATE_POPUP_H

#include <QDialog>
#include <QtCore>
#include <QColorDialog>
#include <QMessageBox>
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_2.h"

namespace Ui {
class tag_create_popup;
}

class tag_create_popup : public QDialog
{
    Q_OBJECT

public:
    explicit tag_create_popup(QWidget *parent = 0);
    ~tag_create_popup();

    void pub_set_start_address(qint64);
    qint64 pub_get_start_address();

    void pub_set_end_address(qint64);
    qint64 pub_get_end_address();

    void pub_set_tags_data();
signals:
    void signal_save_tag_details(qint64 start, qint64 end, QString tag_name, QString color_hex);

private slots:
    void on_pushButton_save_clicked();

    void on_checkBox_create_new_clicked(bool checked);


    void on_pushButton_cancel_clicked();

private:
    Ui::tag_create_popup *ui;

    qint64 start_address;
    qint64 end_address;

    QString highlight_color_hex;

    QMap <QString,QString> tag_color_hex_map;
    QMap <QString,QString> tag_all_colors_map;

    void prepare_colors_list();
    void save_tag_details_in_tags_color_db(QString tag_name_str);
    QString get_color_hex_from_saved_colors(QString color_name_str);
    void add_existing_tags_to_combobox();
    QString get_color_hex_from_all_available_colors(QString color_name_str);
    bool save_new_tag_details();

};

#endif // TAG_CREATE_POPUP_H
