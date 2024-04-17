#ifndef THREAD_ARTIFACTS_KEYWORD_SEARCH_H
#define THREAD_ARTIFACTS_KEYWORD_SEARCH_H

#include <QObject>
#include <QtCore>
#include <QtSql/QtSql>
#include "header_global_variables_1.h"
                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"

class thread_artifacts_keyword_search : public QThread
{
    Q_OBJECT

public:
    thread_artifacts_keyword_search(QWidget *parent = 0);
    ~thread_artifacts_keyword_search();

    void run();
    void pub_set_plugin_list_for_search(QStringList list);
    void pub_set_keywords_list_for_search(QStringList list);
    void pub_set_essentials();
    void pub_set_search_name(QString name);
    QString pub_get_search_name();
    void pub_cancel_keyword_search_extraction();
    void pub_set_bool_search_on_plugin(bool val);
    void pub_set_bool_search_on_files(bool val);

signals:
    void signal_increase_progressbar_of_keyword_search(int plugin_count , QString job_title);

private slots:

private:

    QString search_name;
    QString current_k_search_db_path;
    QString result_path,plugin_name,debug_file_path;
    QStringList list_plugins_for_search;
    QStringList list_search_keyword;

    QStringList  list_table , list_tab;

    QString destination_file_location;
    QString destination_search_db_file;
    QString source_plugins_db_file;

    QList<QStringList>list_search_on_column;
    QList<QStringList>list_display_columns;

    void create_paths_for_plugins(void);
    void create_search_table(void);
    void save_current_search(void);
    void save_current_search_with_the_keyword_hit();
    QString get_partial_command_forsearch(QStringList search_cols_list, QStringList search_string_list);

    void insert_index_table_entry();
    void create_index_table();
    void create_search_result_db();
    bool bool_cancel_search;
    void search_in_plugins();


    QElapsedTimer *timer_records_k_search ;

    int total_records;
    bool bool_search_on_plugin;
    bool bool_search_on_files;

    recon_helper_standard *recon_helper_standard_obj;

};

#endif //
