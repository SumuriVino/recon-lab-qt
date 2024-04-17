#ifndef RECON_CONFIGURATION_PAGE_PREFERENCES_H
#define RECON_CONFIGURATION_PAGE_PREFERENCES_H

#include <QWidget>

#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"      


namespace Ui {
class recon_configuration_page_preferences;
}

class recon_configuration_page_preferences : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration_page_preferences(QWidget *parent = 0);
    ~recon_configuration_page_preferences();

    void pub_set_caller_type(QString caller_type);

private slots:

    void on_checkBox_highligh_user_opened_files_clicked(bool checked);
    void on_lineEdit_minimum_file_size_skin_tone_detection_returnPressed();
    void on_lineEdit_minimum_file_size_face_analysis_returnPressed();

    void on_lineEdit_minimum_file_size_optical_character_recognition_returnPressed();

    void on_lineEdit_maximum_size_text_view_returnPressed();

    void on_lineEdit_minimum_file_size_weapons_returnPressed();

    void on_lineEdit_minimum_file_size_fire_analysis_returnPressed();

private:
    Ui::recon_configuration_page_preferences *ui;

    QString destination_db_file;

    void update_databases(QString m_key, QString m_value);

    void display_data(void);
    void update_narad_variables(void);
    recon_helper_standard *recon_helper_standard_obj;

};


#endif // RECON_CONFIGURATION_PAGE_PREFERENCES_H
