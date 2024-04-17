#ifndef OFFLINE_MAPS_H
#define OFFLINE_MAPS_H

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QQuickView>
#include <QTextEdit>
#include <QTextBrowser>

#include <QStackedWidget>
#include <QWebEngineView>
#include "previewer/blank.h"
#include <QCheckBox>
#include <QPushButton>

                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "exif_data/exif_data_images/exif_data_image_files.h"
#include "header_global_variables_1.h"
#include "header_global_variables_2.h"
#include "maps/online_maps/online_maps_dialog.h"
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"      
#include "message_box/message_dialog.h"

namespace Ui {
class offline_maps;
}

class offline_maps : public QFrame
{
    Q_OBJECT

public:
    explicit offline_maps(QWidget *parent = 0);
    ~offline_maps();

    void pub_change_location(QString latitude, QString longitude, QString fil_path_2, QString source_count_name);

    void pub_set_recon_file_info(struct_global_recon_file_info obj);
signals:
    void signal_add_saved_maps_in_case_tree();
    void signal_display_saved_maps(QString tab_text);

private slots:
    void slot_checkBox_use_online_maps_clicked(bool status);
    void slot_pushbutton_save_maps_clicked(bool status);
    void slot_pushbutton_use_online_maps_clicked(bool status);
    void slot_online_maps_closed();
    void slot_message_box_yes_no_clicked(bool yes_no_clicked);

private:
    Ui::offline_maps *ui;
    void create_ui();
    QHBoxLayout *hbox_map;
    QVBoxLayout *vbox_main_layout;

    QObject *quickview_object;

    QTextBrowser *text_browser_obj;


    QStackedWidget *stacked_widget_obj;
    blank *blank_obj;

    //QWebEngineView *web_view;
    QString current_url_str;

    QCheckBox *checkBox_use_online_maps;

    QPushButton *pushbutton_use_online_maps;

    QString current_source_count_name;
    QString current_filepath_str;
    QString current_latitude_str;
    QString current_longitude_str;

    QPushButton *pushbutton_save_maps;
    QQuickView  *view;

    exif_data_image_files *exif_data_image_files_obj;

    bool bool_is_location_available = false;
    recon_helper_standard *recon_helper_standard_obj;

    online_maps_dialog *online_maps_dialog_obj;
    bool bool_online_maps_running = false;

    struct_global_recon_file_info st_globl_recon_file_info_obj;
    message_dialog *message_dialog_obj;
    bool bool_message_box_yes_button_clicked;
    bool bool_message_box_clicked;

    QComboBox *combobox_online_maps;
    QString get_online_map_url();

};

#endif // OFFLINE_MAPS_H
