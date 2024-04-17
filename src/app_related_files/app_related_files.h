#ifndef APP_RELATED_FILES_H
#define APP_RELATED_FILES_H

#include <QFrame>
#include <QtCore>
#include <QSqlDatabase>
#include <QSql>
#include <QSqlQuery>
#include <recon_generic_functions/recon_static_functions.h>
#include <recon_generic_functions/recon_helper_standard.h>

namespace Ui {
class app_related_files;
}

class app_related_files : public QFrame
{
    Q_OBJECT

public:
    explicit app_related_files(QWidget *parent = nullptr);
    ~app_related_files();
    bool pub_set_data_in_table(QString db_path, QString searched_app_identifier, QString app_name);

    bool pub_set_source_count_name(QString src_cnt_name);
private slots:
    void slot_action_goto_record_triggered(bool status);
    void slot_action_goto_record_double_clicked(QTableWidgetItem*item);
signals:
//    void signal_related_files_tablewidget_goto_record(QString plugin_name, QString parent_name, QString tab_name ,QString record_no, QString db_path, QString os_name_str);
    void signal_related_files_tablewidget_goto_record(QString tab_name ,QString record_no, QString src_name);

private:

    Ui::app_related_files *ui;

    enum enum_table_column{
        enum_table_column_INT = 0,
        enum_table_column_record_no,
        enum_table_column_filepath
    };


    void contextMenuEvent(QContextMenuEvent *event);

    QMenu *m_menu;
    QAction *action_goto_record;
    QString source_cnt_name;
    QString fs_db_path;
};

#endif // APP_RELATED_FILES_H
