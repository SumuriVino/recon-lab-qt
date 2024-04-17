#ifndef THREAD_FILE_SEARCH_REGULAR_FILTERS_H
#define THREAD_FILE_SEARCH_REGULAR_FILTERS_H


#include <QThread>
#include <QObject>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_1.h"
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"


class thread_file_search_regular_filters : public QThread
{
public:
    thread_file_search_regular_filters(QWidget *parent = 0);


    void run();

    void pub_set_search_command_sources_search_label(QString command, QStringList src_list, QString srch_label);
    void pub_cancel_extraction();

private:

    QString search_command;
    QString search_label;
    QStringList sources_list;
    QString search_db_name;
    QString destination_db_path;

    bool bool_cancel_extraction = false;
    int total_records = 0;

    void insert_file_search_config_into_index_db();
    QString create_file_search_destination_db();
    void update_total_records_extracted_in_file_search_index_db();

    recon_helper_standard *recon_helper_standard_obj;

};

#endif // THREAD_FILE_SEARCH_REGULAR_FILTERS_H
