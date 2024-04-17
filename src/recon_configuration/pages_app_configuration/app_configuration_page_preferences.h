#ifndef APP_CONFIGURATION_PAGE_PREFERENCES_H
#define APP_CONFIGURATION_PAGE_PREFERENCES_H

#include <QWidget>

#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "header_global_variables_4.h"

namespace Ui {
class app_configuration_page_preferences;
}

class app_configuration_page_preferences : public QWidget
{
    Q_OBJECT

public:
    explicit app_configuration_page_preferences(QWidget *parent = nullptr);
    ~app_configuration_page_preferences();

    void pub_set_caller_type(QString caller_type);

private slots:

    void on_checkBox_enable_debug_mode_clicked(bool checked);

private:

    Ui::app_configuration_page_preferences *ui;

    recon_helper_standard *recon_helper_standard_obj;
    void update_databases(QString m_key, QString m_value);

    QString destination_db_file;
    QString caller_type_str;
    void display_data();
};

#endif // APP_CONFIGURATION_PAGE_PREFERENCES_H
