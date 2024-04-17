#ifndef ICHAT_PARSER_H
#define ICHAT_PARSER_H

#include <QtCore>
#include <QObject>
#include <QWidget>
#include <QDateTimeEdit>
#include <QElapsedTimer>
#include <QTextBrowser>
#include <QFileDialog>
#include <QWebEngineView>

#include "recon_generic_functions/recon_static_functions.h"
#include "file_viewer/ichat_viewer/ichat_viewer/ichat_viewer.h"

#include "recon_generic_functions/recon_helper_standard.h"
#include "header_gui_settings.h"
#include "global_recon_file_info/global_recon_file_info.h"

#include "header_global_structures.h"
#include "file_viewer/ichat_viewer/ichat_templtaes/chat_template_class.h"
#include "export/export_except_reports/export_rsync/export_rsync.h"
#include "export_dialog_box/export_dialog_box.h"
#include "message_box/message_dialog.h"
#include "report/report_essentials/report_status.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"


namespace Ui {
class ichat_parser;
}
class ichat_parser : public QWidget
{

    Q_OBJECT

public:
    explicit ichat_parser(QWidget *parent = nullptr);
    ~ichat_parser();

    void pub_clear_data();
    void pub_set_global_recon_file_info_structure_obj(struct_global_recon_file_info obj);
    void pub_set_essentials();
    void pub_parse_ichat();

signals:

private slots:
    void on_textBrowser_ichat_anchorClicked(const QUrl &arg1);
    void slot_right_click_main_menu_triggered(QAction *current_clicked_action);
    void onCustomContextMenuRequested(const QPoint &pos);
    void slot_export_ichat_ok_clicked(QString given_name, QString selected_dir_path);
private:
    Ui::ichat_parser *ui;

    //void mousePressEvent(QMouseEvent *event);



    QMenu *mainMenu;
    QAction *action_export;

    QString exported_files_dir;
    QString source_count_name_str;
    QString source_file_path_ichat;


    QStringList file_name_list;

    void extract_xcode_archieved_chat(QString source_file);
    void export_ichat_file_in_html_format(QFile &m_file);
    void create_right_clicks();
    void act_export_ichat();


    //    void act_select_all_ichat();
    //    void act_copy_selected_text_ichat();



    recon_helper_standard *recon_helper_standard_obj;
    struct_global_recon_file_info struct_global_recon_file_info_obj;


    export_rsync *export_rsync_obj;
    export_dialog_box * export_dialog_box_obj;

    // display_loading_progress_bar *

};

#endif // ICHAT_PARSER_H
