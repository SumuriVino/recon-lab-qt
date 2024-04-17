#ifndef THREAD_CARVE_FILES_H
#define THREAD_CARVE_FILES_H

#include <QObject>
#include "recon_generic_functions/recon_static_functions.h"
                                   

class thread_carve_files : public QThread
{
    Q_OBJECT

public:
    thread_carve_files(QWidget *parent = 0);
    ~thread_carve_files();

    void run();
    void pub_set_current_source_info(struct_GLOBAL_witness_info_source obj);
    void pub_set_separate_process_obj(QProcess *obj);
    void pub_set_essential_db_path(QString str);

signals:

private:
    struct_GLOBAL_witness_info_source target_info_struct_obj;
    QProcess *process_carved_files;
    QString essential_db_path;

};

#endif // THREAD_CARVE_FILES_H
