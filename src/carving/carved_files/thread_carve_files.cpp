#include "thread_carve_files.h"

thread_carve_files::thread_carve_files(QWidget *parent)
{

}

thread_carve_files::~thread_carve_files()
{

}

void thread_carve_files::pub_set_current_source_info(struct_GLOBAL_witness_info_source obj)
{ // public functions to set object
    target_info_struct_obj = obj;
}

void thread_carve_files::pub_set_separate_process_obj(QProcess *obj)
{
    process_carved_files = obj;
}

void thread_carve_files::pub_set_essential_db_path(QString str)
{
    essential_db_path = str;
}

void thread_carve_files::run()
{ // run/start file carver feature from here. we use carver lib for that
    QStringList args_list;
    args_list << essential_db_path;

    process_carved_files->start("./carver.app/Contents/MacOS/carver", args_list);
    if (!process_carved_files->waitForStarted())
    {
        recon_static_functions::app_debug("----FAILED---- Process Start",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + process_carved_files->errorString() ,Q_FUNC_INFO);
        return;
    }

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    ///forcing thread to wait and read when data available from process.
    int count = 0;
    while(1)
    {
        count++;
        if(count % 100 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        if(process_carved_files->state() == QProcess::NotRunning)
            break;

        process_carved_files->waitForReadyRead();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    ///===============Note============
    /// do not use process variable after this point,
    /// because it is deleted in process finish slot.

}
