#include "recon_case.h"

void RECON_case::slot_right_click_global_report_clicked()
{
    recon_static_functions::app_debug("slot_right_click_global_report_clicked ",Q_FUNC_INFO);

    current_report_name.clear();


    global_report_interface_obj->set_narad_muni_for_report(global_narad_muni_class_obj);

    global_report_interface_obj->set_initialisation();
    global_report_interface_obj->show();
}


void RECON_case::slot_global_report_plugin_selection_workdone(narad_muni*mobj, QStringList report_plugin_info_list, QString rpt_name, QStringList rpt_file_path_list, QStringList rpt_type_list, QString rpt_scope, QStringList selctd_tags_list)
{
    qint64 date_time = QDateTime::currentMSecsSinceEpoch();
    emit signal_activity_logs_update_status(MACRO_JobType_G_Report,MACRO_ACTIVITY_LOGS_TASK_QUEUED,rpt_name,date_time,"Reports Initialised...");

    common_global_report_plugin_selection_workdone(mobj, report_plugin_info_list,rpt_name, rpt_file_path_list, rpt_type_list, rpt_scope, selctd_tags_list);
}

void RECON_case::common_global_report_plugin_selection_workdone(narad_muni*mobj, QStringList report_plugin_info_list, QString rpt_name, QStringList rpt_file_path_list, QStringList rpt_type_list, QString rpt_scope, QStringList selctd_tags_list)
{

    r_case_struct_queue_2 obj;
    obj.g_report_obj.report_maker = MACRO_REPORT_MAKER_GLOBAL_REPORT_int;
    obj.g_report_obj.report_name = rpt_name;
    obj.g_report_obj.report_type_list = rpt_type_list;
    obj.g_report_obj.report_scope = rpt_scope;

    obj.g_report_obj.narad_muni_report_obj = mobj;
    obj.g_report_obj.report_path_list = rpt_file_path_list;
    obj.g_report_obj.selected_tags_list = selctd_tags_list;
    obj.g_report_obj.report_plugin_info_list = report_plugin_info_list;

    obj.operation_type = enum_recon_case_queue_g_report;

    queued_operation_for_g_report(obj);

}

void RECON_case::queued_operation_for_g_report(r_case_struct_queue_2 obj)
{
    qint64 date_time = QDateTime::currentMSecsSinceEpoch();
    emit signal_activity_logs_update_status(MACRO_JobType_G_Report,MACRO_ACTIVITY_LOGS_TASK_START,obj.g_report_obj.report_name,date_time,"Reports Generation Started...");

    current_report_name =  obj.g_report_obj.report_name;

    thread_report_builder_obj->pub_set_essentials(obj.g_report_obj);

    bool_cancel_operation = false;
    display_loading_progress_bar_obj->pub_set_progress_bar_title("REPORTS");
    display_loading_progress_bar_obj->pub_set_label_messsge("Building Reports......");
    display_loading_progress_bar_obj->show();
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();


    thread_report_builder_obj->start();

}

void RECON_case::slot_hide_loading_display_dialog_box(bool cancel_status)
{
    bool_cancel_operation = true;

    if(thread_file_search_obj != NULL && thread_file_search_obj->isRunning())
    {
        thread_file_search_obj->cancel_extraction();
    }

    thread_report_builder_obj->pub_cancel_report();
    display_loading_progress_bar_obj->hide();
}

void RECON_case::slot_report_encrypt_prcoess(int)
{
    bool_report_encrypt_process = true;
}

void RECON_case::slot_thread_global_report_finished_or_cancelled()
{
    recon_static_functions::app_debug(" report name" + current_report_name,Q_FUNC_INFO);

    if(bool_cancel_operation)
        return;

    thread_report_builder *m_obj = dynamic_cast<thread_report_builder*> (sender());

    QStringList filepaths = m_obj->pub_get_report_filepath_list();


    QString report_password = m_obj->pub_get_report_password();

    QStringList filespaths_for_enc_reports;

    for(int i = 0; i < filepaths.size(); i++)
    {
        QFileInfo info(filepaths.at(i));

        if(info.exists())
        {
            QString report_dir_path = info.absolutePath();

            QProcess proc;

            QStringList args;
            args << "-R" << "0777" << report_dir_path;

            proc.execute("chmod", args);


            if(!report_password.isEmpty())
            {

                QString report_dir_parent_path = QFileInfo(report_dir_path).absolutePath();

                bool_report_encrypt_process = false;

                QStringList args_enc;
                args_enc << "-r" << "--encrypt" << "--password" << report_password << report_dir_path + ".zip" << QFileInfo(report_dir_path).fileName();

                filespaths_for_enc_reports.append(report_dir_path + ".zip");

                QString app_dir_path = QDir::currentPath();
                QDir mmdir;
                mmdir.setCurrent(report_dir_parent_path);

                report_encrypt_prcoess->start("zip", args_enc);

                while(1)
                {
                    if(bool_report_encrypt_process)
                        break;
                    else
                        QCoreApplication::processEvents();
                }

                if(report_dir_path != QString("/") && report_dir_path != QString(getenv("HOME")))
                {
                    QDir r_dir(report_dir_path);
                    r_dir.removeRecursively();
                }

                mmdir.setCurrent(app_dir_path);
            }
            QCoreApplication::processEvents();
        }
    }

    if(filespaths_for_enc_reports.size() > 0)
        filepaths = filespaths_for_enc_reports;

    qint64 date_time = QDateTime::currentMSecsSinceEpoch();
    emit signal_activity_logs_update_status(MACRO_JobType_G_Report,MACRO_ACTIVITY_LOGS_TASK_COMPLETED,thread_report_builder_obj->pub_get_report_name(),date_time,"Reports Generation Completed");

    emit signal_global_report_finished_or_cancelled(filepaths);
    display_loading_progress_bar_obj->hide();

}
