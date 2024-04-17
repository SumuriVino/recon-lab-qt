#ifndef CHAT_VIEWER_H
#define CHAT_VIEWER_H

#include "ui_custom_classes/m_tablewidget.h"
#include "ui_custom_classes/m_treewidget.h"
#include "ui_custom_classes/m_stackedwidget.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"
#include "recon_generic_functions/recon_helper_standard.h"      

#include <QHeaderView>
#include <QWebEngineView>
#include <QHBoxLayout>

class chat_viewer : public QWidget
{
    Q_OBJECT

public:
    chat_viewer(QWidget *parent = 0);
    ~chat_viewer();

    m_treewidget *treewidget_configured_account;


    m_stackedwidget *stackedwidget_bubble_message_obj;

    void pub_set_display_chat_view_in_plugins_tab();
    void pub_set_plugin_name(QString plugin);

    void create_ui();

    void pub_set_tab_name(QString tab_name_str);
private slots:
    void slot_treewidget_configured_account_single_clicked(QTreeWidgetItem*item, int column);

private:

    QString plugin_name;

    QString get_chat_buddy_name(QString info_file_path);
    QString get_html_file(QString html_dir_path);
    QString get_configured_account(QString account_dir_path);
    QString destination_db_file;

    QString tab_name;

    recon_helper_standard *recon_helper_standard_obj;

};

#endif // BUBBLE_CHAT_H
