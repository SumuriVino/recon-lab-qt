#ifndef ARTIFACTS_KEYWORD_SEARCH_INTERFACE_H
#define ARTIFACTS_KEYWORD_SEARCH_INTERFACE_H

#include <QWidget>
#include "filters/keyword_search_filters/keywords_list_selector.h"
                                   
#include "launcher/plugin_selector/plugin_selector.h"
#include "header_global_variables_1.h"
#include "message_box/message_dialog.h"
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"

namespace Ui {
class artifacts_keyword_search_interface;
}

class artifacts_keyword_search_interface : public QWidget
{
    Q_OBJECT

public:
    explicit artifacts_keyword_search_interface(QWidget *parent = 0);
    ~artifacts_keyword_search_interface();

    void set_initialisation();

signals:
    void signal_keyword_search_plugin_selection_workdone(QString search_name,QStringList keyword_list, QStringList selected_plugin_list, bool bool_plugin_selected, bool files_selected);

private slots:
    void slot_label_keyword_list_clicked(QString);

    void on_pushButton_search_clicked();

private:
    Ui::artifacts_keyword_search_interface *ui;
    //    plugin_selector *plugin_selector_obj;
    plugin_selector *plugin_selector_obj;
    keywords_list_selector *keyword_list_selector_obj;
    QVBoxLayout *layout;

    void set_keywordsearch_default_name();
    QString search_name;
    recon_helper_standard *recon_helper_standard_obj;


    message_dialog *message_dialog_object;

};

#endif //
