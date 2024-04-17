#include "recon_case.h"

RECON_case::RECON_case(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<narad_muni>("narad_muni");

    recon_helper_standard_obj = new recon_helper_standard(this);

    semaphore_plugin_list = new QSemaphore(1);

    report_encrypt_prcoess = new QProcess(this);
    connect(report_encrypt_prcoess, SIGNAL(finished(int)), this, SLOT(slot_report_encrypt_prcoess(int)));


    keyword_search_obj = new artifacts_keyword_search_interface(this);
    connect(keyword_search_obj,SIGNAL(signal_keyword_search_plugin_selection_workdone(QString,QStringList,QStringList, bool,bool)),this,SLOT(slot_keyword_search_plugin_selection_workdone(QString,QStringList,QStringList, bool,bool)),Qt::QueuedConnection);

    thread_keyword_search_obj = new thread_artifacts_keyword_search(this);
    connect(thread_keyword_search_obj,SIGNAL(finished()),this,SLOT(slot_thread_keyword_search_finished_or_cancelled()),Qt::QueuedConnection);
    connect(thread_keyword_search_obj,SIGNAL(signal_increase_progressbar_of_keyword_search(int,QString)),this,SIGNAL(signal_set_progressbar_value(int,QString)),Qt::QueuedConnection);

    global_report_interface_obj = new global_report_interface(this);
    connect(global_report_interface_obj,SIGNAL(signal_global_report_plugin_selection_workdone(narad_muni*,QStringList, QString , QStringList ,QStringList , QString ,QStringList)),this,SLOT(slot_global_report_plugin_selection_workdone(narad_muni*,QStringList, QString , QStringList ,QStringList , QString ,QStringList)),Qt::QueuedConnection);


    thread_report_builder_obj = new thread_report_builder(this);
    connect(thread_report_builder_obj,SIGNAL(finished()),this,SLOT(slot_thread_global_report_finished_or_cancelled()),Qt::QueuedConnection);

    // plugin_selector_obj = new plugin_selector;
    run_plugin_obj = new run_plugin(this);

    bool_cancel_operation = false;

    bool_thread_hashset_cancel_finish = false;


    bool_run_plugin_triggered = false;

    bool_plugin_extraction_qobject_running = false;


    finished_thread_hashset_source_count_name_list.clear();


    thread_file_search_obj = new thread_file_search(this);
    connect(thread_file_search_obj,SIGNAL(finished()),this,SLOT(slot_thread_global_search_extraction_finished_or_cancelled()));
    connect(thread_file_search_obj, SIGNAL(signal_run_separate_process_for_mdfind_outside_thread(QString,QString)), this, SLOT(slot_run_mdfind_in_separate_function(QString, QString)));

    bool_extraction_running_queue_1 = false;
    bool_file_system_run = false;


    activity_log_storage_obj = new activity_logs_storage(this);
    connect(this,SIGNAL(signal_activity_logs_update_status(QString,QString,QString,qint64,QString)),activity_log_storage_obj,SLOT(slot_update_log_status(QString,QString,QString,qint64,QString)),Qt::AutoConnection);

    thread_content_search_obj = new thread_content_search(this);
    connect(thread_content_search_obj,SIGNAL(finished()),this,SLOT(slot_thread_indexed_k_search_extraction_finished_or_cancelled()));
    connect(thread_content_search_obj, SIGNAL(signal_set_progress_bar_max_value(struct_global_task_type_and_title, int)), this, SIGNAL(signal_set_progress_bar_max_value(struct_global_task_type_and_title, int)));
    connect(thread_content_search_obj, SIGNAL(signal_set_progressbar_value(int , QString)), this, SIGNAL(signal_set_progressbar_value(int , QString)));
    connect(thread_content_search_obj, SIGNAL(signal_set_search_label_with_records_in_progress_window(QString,QString,bool,qint64,int,bool)), this, SIGNAL(signal_set_search_label_with_records_in_progress_window_content_search(QString,QString,bool,qint64,int,bool)));

    bool_extraction_running_queue_3 = false;

    bool_extract_already_added_sources = false;
    bool_mdfind_process_is_finished = false;

    mutex_exclusive_for_fs_main_db = new QMutex;

    mutex_exclusive_for_email_db = new QMutex;

    display_loading_progress_bar_obj = new display_loading_progress_bar();
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));

    bool_thread_file_system_running = false;
    bool_thread_file_search_regular_filter_or_apple_metadata_running = false;

    display_loading_non_cancelable_progress_bar_obj = new display_loading_progress_bar();
    display_loading_non_cancelable_progress_bar_obj->pub_set_hidden_cancel_button(true);

    thread_apple_metadata_file_click_obj = new thread_apple_metadata;
    process_apple_metadata_file_click = new QProcess(this);
    connect(process_apple_metadata_file_click, SIGNAL(finished(int)), this, SLOT(slot_prc_apple_metadata_prc_left_click_finished(int)));
    thread_apple_metadata_file_click_obj->pub_set_apple_metadata_reader_process_for_left_click(process_apple_metadata_file_click);

    bool_thread_face_analysis_running = false;

}

RECON_case::~RECON_case()
{
    if(semaphore_plugin_list != NULL)
        delete semaphore_plugin_list;

    if(thread_apple_metadata_file_click_obj != NULL)
        delete thread_apple_metadata_file_click_obj;

    //    if(thread_qobject_run_multiple_plugins != NULL && thread_qobject_run_multiple_plugins->isRunning())
    //    {
    //        qobject_run_multiple_plugins_obj->pub_cancel_everything();

    //        while(1)
    //        {
    //            if(!thread_qobject_run_multiple_plugins->isRunning())
    //                break;
    //        }
    //    }

    //    if(thread_file_system_obj != NULL && thread_file_system_obj->isRunning())
    //    {
    //        thread_file_system_obj->pub_cancel_extraction();
    //        while(1)
    //        {
    //            if(!thread_file_system_obj->isRunning())
    //                break;
    //        }

    //    }

}


void RECON_case::pub_set_file_system_selected_job_source_count_name(struct_global_file_system_post_launcher_job_info obj)
{
    st_file_system_post_launcher_job_info_obj = obj;
}

void RECON_case::pub_start_file_system_post_launcher_jobs()
{

    if(thread_file_system_obj->isRunning())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Task 'Adding source to case' is going on. Can not start new jobs until source has been added to the case.");
        return;
    }


    recon_case_runner_type = MACRO_RECON_CASE_RUNNER_TYPE_POST_LAUNCH;

    if(st_file_system_post_launcher_job_info_obj.source_list_apple_metadata.size() > 0)
        start_apple_metadata_extraction();

    if(st_file_system_post_launcher_job_info_obj.source_list_signature_analysis.size() > 0)
        start_signature_extraction();

    if(st_file_system_post_launcher_job_info_obj.source_list_hashes.size() > 0)
        pub_start_hashes_data_extraction();

    if(st_file_system_post_launcher_job_info_obj.source_list_unified_logs.size() > 0)
        start_unified_logs_extraction();

    if(st_file_system_post_launcher_job_info_obj.source_list_exif_metadata.size() > 0)
    {
        if(bool_thread_mime_type_running)
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Task 'Mime Type' is going on. Can not start new Dependent jobs until Mime Type has been Completed.");
            return;
        }

        start_exif_data_extraction();
    }

    if(st_file_system_post_launcher_job_info_obj.source_list_face_analysis.size() > 0)
    {
        if(bool_thread_mime_type_running)
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Task 'Mime Type' is going on. Can not start new Dependent jobs until Mime Type has been Completed.");
            return;
        }

        start_face_analysis_extraction();
    }

    if(st_file_system_post_launcher_job_info_obj.source_list_optical_character_recognition.size() > 0)
    {
        if(bool_thread_mime_type_running)
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Task 'Mime Type' is going on. Can not start new Dependent jobs until Mime Type has been Completed.");
            return;
        }

        start_optical_character_recognition_extraction();
    }

    if(st_file_system_post_launcher_job_info_obj.source_list_skin_tone_detection.size() > 0)
    {
        if(bool_thread_mime_type_running)
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Task 'Mime Type' is going on. Can not start new Dependent jobs until Mime Type has been Completed.");
            return;
        }

        start_skin_tone_detection_extraction();
    }

    if(st_file_system_post_launcher_job_info_obj.source_list_weapons.size() > 0)
    {
        if(bool_thread_mime_type_running)
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Task 'Mime Type' is going on. Can not start new Dependent jobs until Mime Type has been Completed.");
            return;
        }

        start_weapons_extraction();
    }

    if(st_file_system_post_launcher_job_info_obj.source_list_fire_analysis.size() > 0)
    {
        if(bool_thread_mime_type_running)
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Task 'Mime Type' is going on. Can not start new Dependent jobs until Mime Type has been Completed.");
            return;
        }

        start_fire_analysis_extraction();
    }

}




void RECON_case::pub_set_recon_case_runner_type(QString type)
{
    recon_case_runner_type = type;
}
void RECON_case::set_progress_bar_object(progress_bar_fixed_in_statusbar *obj)
{
    progress_bar_obj = obj;
}

void RECON_case::pub_set_task_status_window_object(task_status_window *obj)
{
    task_status_window_obj = obj;

    connect(task_status_window_obj, SIGNAL(signal_PBAR_file_system_job_cancelled_requested(QString)), this, SLOT(slot_PBAR_file_system_job_cancelled_requested(QString)));
}

void RECON_case::pub_set_face_finder_obj(facefinder *obj)
{
    facefinder_obj = obj;

    connect(facefinder_obj, SIGNAL(signal_add_matched_faces_to_case_tree(QString)), this, SLOT(slot_add_matched_faces_to_case_tree(QString)));
}

void RECON_case::pub_set_recognize_face_obj(recognize_face *obj)
{
    recognize_face_obj = obj;

    connect(recognize_face_obj, SIGNAL(signal_add_search_result_in_case_tree(QString)), this, SLOT(slot_add_matched_faces_to_case_tree(QString)));

}

void RECON_case::set_case_treewidget_obj(R_Treewidget *obj)
{
    treewidget_case_display_obj = obj;
}

bool RECON_case::is_plugin_selected_for_exraction(QString plugin_name)
{
    // Cancel extraction of plugins, also removed these plugins in from received_plugin_list_for_current_extraction

    for(int i = 0; i < received_plugin_list_for_current_extraction.size(); i++)
    {
        if(received_plugin_list_for_current_extraction.at(i).trimmed() == plugin_name.trimmed())
        {
            return true;
        }
    }

    return false;
}

void RECON_case::set_essentials()
{
    list_struct_queue_2.clear();
    bool_extraction_running_queue_2 = false;
    bool_thread_hashset_cancel_finish = false;
    bool_run_plugin_triggered = false;

    activity_log_storage_obj->pub_set_recon_result_directory_path(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString());
}


void RECON_case::start_extraction()
{
    make_struct_plugin_extraction_list_for_first_and_add_source();

    file_system_extraction_initiator();
}

void RECON_case::pub_set_FS_source_selected_source_count_name_list(QStringList m_list)
{
    source_selected_source_count_name_list = m_list;
}


void RECON_case::finish_operation_queue_3()
{
    semaphore_plugin_list->release(1);

    if(list_struct_queue_3.size() < 1)
    {
        bool_extraction_running_queue_3 = false;
        is_all_task_and_queue_finish();
        return;
    }

    recon_static_functions::app_debug("finish ",Q_FUNC_INFO);

    list_struct_queue_3.dequeue();

    start_extraction_queue_3();
}

void RECON_case::set_selected_pluginname_list_for_extraction(QStringList list)
{
    received_plugin_list_for_current_extraction = list;
}

bool RECON_case::is_all_task_and_queue_finish()
{
    if(!bool_extraction_running_queue_2 && !bool_extraction_running_queue_1 && !bool_extraction_running_queue_3)
    {
        emit signal_show_progressbar(false);
        return true;
    }

    return false;
}


void RECON_case::start_extraction_queue_2()
{

    if(list_struct_queue_2.size() < 1)
    {
        bool_extraction_running_queue_2 = false;
        is_all_task_and_queue_finish();

        return;
    }

    recon_static_functions::app_debug("start_queued_operation start ",Q_FUNC_INFO);

    emit signal_show_progressbar(true);
    r_case_struct_queue_2 obj = list_struct_queue_2.at(0);


    if(obj.operation_type == enum_recon_case_queue_run_plugin)
    {
        // not taking semaphore here bcoz semaphore is taken at extraction level(extrct_plugins)
        bool_extraction_running_queue_2 = true;
        //queued_operation_for_run_plugin(obj);
    }
    else if(obj.operation_type == enum_recon_case_queue_k_search)
    {

        bool_extraction_running_queue_2 = true;

        recon_static_functions::debug_conditional(" : running " + QString::number(enum_recon_case_queue_k_search), Q_FUNC_INFO);
        while(1)
        {
            if(semaphore_plugin_list->tryAcquire(1))
                break;
            QCoreApplication::processEvents();
        }
        recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

        queued_operation_for_k_search(obj);
    }
    else if(obj.operation_type == enum_recon_case_queue_file_search)
    {

        bool_extraction_running_queue_2 = true;

        recon_static_functions::debug_conditional(" : running " + QString::number(enum_recon_case_queue_file_search), Q_FUNC_INFO);
        while(1)
        {
            if(semaphore_plugin_list->tryAcquire(1))
                break;

            QCoreApplication::processEvents();
        }
        recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

        queued_operation_for_file_search(obj);
    }
    else if(obj.operation_type == enum_recon_case_queue_content_search)
    {

        bool_extraction_running_queue_2 = true;

        recon_static_functions::debug_conditional(" : running " + QString::number(enum_recon_case_queue_content_search), Q_FUNC_INFO);
        while(1)
        {
            if(semaphore_plugin_list->tryAcquire(1))
                break;

            QCoreApplication::processEvents();
        }
        recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

        queued_operation_for_content_search(obj);
    }




    recon_static_functions::app_debug("start_queued_operation end ",Q_FUNC_INFO);

}

void RECON_case::finish_operation_queue_2()
{
    semaphore_plugin_list->release(1);

    if(list_struct_queue_2.size() < 1)
        return;

    recon_static_functions::app_debug(" ",Q_FUNC_INFO);

    list_struct_queue_2.dequeue();

    start_extraction_queue_2();
}

void RECON_case::interrupt_queued_operation(struct_global_task_type_and_title t1)
{
    if(list_struct_queue_2.size() < 2) // 1 is running
        return;

    for(int i = 1; i < list_struct_queue_2.size(); i++)
    {
        r_case_struct_queue_2 obj = list_struct_queue_2.at(i);

        if(t1.type == MACRO_JobType_Artifacts)
        {

            int plg_index = obj.run_plugin_obj.plugin_list.indexOf(t1.title);
            if(plg_index != -1)
            {
                obj.run_plugin_obj.plugin_list.removeAt(plg_index);


                list_struct_queue_2.removeAt(i);
                list_struct_queue_2.insert(i,obj);


                // if it was last plugin in the queue
                if(obj.run_plugin_obj.plugin_list.size() <= 0)
                    list_struct_queue_2.removeAt(i);
                break;
            }
            continue;
        }

        if(t1.type == MACRO_JobType_Artifacts_Keyword_Search)
        {
            if(obj.k_search_obj.search_name.trimmed() == t1.title.trimmed())
            {
                list_struct_queue_2.removeAt(i);
                break;
            }
            continue;
        }
        if(t1.type == MACRO_JobType_File_Search)
        {
            if(obj.struct_file_search_obj.file_search_label_name.trimmed() == t1.title.trimmed())
            {
                list_struct_queue_2.removeAt(i);
                break;
            }
            continue;
        }

        if(t1.type == MACRO_JobType_Artifacts_Timeline)
        {
            if(obj.g_timeline_obj.timeline_name.trimmed() == t1.title.trimmed())
            {
                list_struct_queue_2.removeAt(i);
                break;
            }
            continue;
        }
        if(t1.type == MACRO_JobType_G_Report)
        {
            if(obj.g_report_obj.report_name.trimmed() == t1.title.trimmed())
            {
                list_struct_queue_2.removeAt(i);
                break;
            }
            continue;
        }

    }

}

QStringList RECON_case::extract_already_exist_plugins_in_queued_list(QStringList list)
{
    QStringList plugin_found;

    for(int i = 0; i < list_struct_queue_2.size(); i++)
    {
        r_case_struct_queue_2 obj = list_struct_queue_2.at(i);
        if(obj.operation_type == enum_recon_case_queue_run_plugin)
        {
            for(int jj = 0; jj < list.size(); jj++)
            {
                if(obj.run_plugin_obj.plugin_list.contains(list.at(jj)))
                {
                    plugin_found  << list.at(jj);
                }
            }
        }
    }

    return plugin_found;
}

void RECON_case::set_ios_parser_map(QMap<QString, ios_uid_backup_parser*> *obj)
{
    map_ios_uid_backup_parser_obj = obj;
}


void RECON_case::start_extraction_queue_3()
{
    if(list_struct_queue_3.size() < 1)
    {
        bool_extraction_running_queue_3 = false;
        is_all_task_and_queue_finish();
        return;
    }

    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    emit signal_show_progressbar(true);
}

void RECON_case::pub_set_dt_search_obj(dt_search_interface *obj)
{
    dt_search_interface_obj = obj;
}

void RECON_case::slot_right_clicked_artifacts_timeline_full()
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    emit signal_artifacts_timeline_full_clicked(MACRO_JobType_Artifacts_Timeline);

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);

}

bool RECON_case::pub_get_apple_metadata_run_status()
{
    return bool_thread_apple_metadata_running;
}

bool RECON_case::pub_get_exif_metadata_run_status()
{
    return bool_thread_exif_data_running;
}

bool RECON_case::pub_get_signature_analysis_run_status()
{
    return bool_thread_signature_running;
}

bool RECON_case::pub_get_mime_type_analysis_run_status()
{
    return bool_thread_mime_type_running;
}

bool RECON_case::pub_get_hashset_run_status()
{
    return bool_thread_hashes_running;
}

bool RECON_case::pub_get_face_analysis_run_status()
{
    return bool_thread_face_analysis_running;
}

bool RECON_case::pub_get_optical_character_recognition_run_status()
{
    return bool_thread_optical_character_recognition_running;
}

bool RECON_case::pub_get_weapons_run_status()
{
    return bool_thread_weapons_running;
}

bool RECON_case::pub_get_fire_analysis_run_status()
{
    return bool_thread_fire_analysis_running;
}

bool RECON_case::pub_get_skin_tone_detection_run_status()
{
    return bool_thread_skin_tone_detection_running;
}

void RECON_case::pub_start_run_fs_module_apple_metadata(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    if(bool_thread_apple_metadata_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Apple Metadata already running.");
        return;
    }

    thread_apple_metadata_obj = new thread_apple_metadata;

    process_apple_metadata_right_click = new QProcess(this);
    process_apple_metadata_right_click->setProcessChannelMode(QProcess::MergedChannels);
    connect(process_apple_metadata_right_click, SIGNAL(readyRead()), thread_apple_metadata_obj, SLOT(slot_prc_apple_metadata_reader_for_right_click_readyread()));

    thread_apple_metadata_obj->pub_set_essentials();
    thread_apple_metadata_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_apple_metadata_obj->set_recon_case_runner_type(QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE));

    thread_apple_metadata_obj->pub_set_fs_run_module_file_info_list(run_fs_modules_details_list);
    thread_apple_metadata_obj->pub_set_apple_metadata_all_attributes_name_list(apple_all_attribute_name_list);
    thread_apple_metadata_obj->pub_set_apple_metadata_reader_process_for_right_click(process_apple_metadata_right_click);

    thread_extended_attr_extract = new QThread;
    thread_apple_metadata_obj->moveToThread(thread_extended_attr_extract);

    connect(thread_extended_attr_extract, SIGNAL(started()), thread_apple_metadata_obj, SLOT(slot_extract_apple_metadata()));
    connect(thread_apple_metadata_obj, SIGNAL(finished()), thread_extended_attr_extract, SLOT(quit()));
    connect(thread_apple_metadata_obj, SIGNAL(finished()), this, SLOT(slot_apple_metadata_thread_finish()));
    connect(thread_apple_metadata_obj, SIGNAL(signal_add_apple_metadata_to_case(QStringList)), this, SLOT(slot_add_apple_metadata_to_case(QStringList)));
    connect(thread_apple_metadata_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    connect(thread_apple_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_apple_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_apple_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(thread_apple_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);

    bool_thread_apple_metadata_running = true;
    thread_extended_attr_extract->start();
}

void RECON_case::pub_start_run_fs_module_exif_metadata(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    if(bool_thread_exif_data_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Exif metadata already running.");
        return;
    }

    thread_exif_metadata_obj = new thread_exif_metadata;

    thread_exif_metadata_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_exif_metadata_obj->set_recon_case_runner_type(QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE));

    thread_exif_metadata_obj->pub_set_fs_run_module_file_info_list(run_fs_modules_details_list);

    thread_exif_data_extract = new QThread;
    thread_exif_metadata_obj->moveToThread(thread_exif_data_extract);

    connect(thread_exif_data_extract, SIGNAL(started()), thread_exif_metadata_obj, SLOT(slot_extract_exif_data()));
    connect(thread_exif_metadata_obj, SIGNAL(finished()), thread_exif_data_extract, SLOT(quit()));
    connect(thread_exif_metadata_obj, SIGNAL(finished()), this, SLOT(slot_exif_metadata_thread_finish()));
    connect(thread_exif_metadata_obj, SIGNAL(signal_add_exif_to_case_tree(QStringList)), this, SLOT(slot_add_exif_data_to_case_tree(QStringList)));
    connect(thread_exif_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_exif_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_exif_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(thread_exif_metadata_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);
    connect(thread_exif_metadata_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    bool_thread_exif_data_running = true;
    thread_exif_data_extract->start();
}

void RECON_case::pub_start_run_fs_module_signature_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    if(bool_thread_signature_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Signature Analysis already running.");
        return;
    }

    thread_signature_obj = new thread_signature;

    thread_signature_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_signature_obj->set_recon_case_runner_type(QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE));

    thread_signature_obj->pub_set_fs_run_module_file_info_list(run_fs_modules_details_list);


    thread_signature_extract = new QThread;
    thread_signature_obj->moveToThread(thread_signature_extract);

    connect(thread_signature_extract, SIGNAL(started()), thread_signature_obj, SLOT(slot_extract_signature()));
    connect(thread_signature_obj, SIGNAL(finished()), thread_signature_extract, SLOT(quit()));
    connect(thread_signature_obj, SIGNAL(finished()), this, SLOT(slot_signature_thread_finish()));
    connect(thread_signature_obj, SIGNAL(signal_add_all_signatures_to_case_tree(QStringList)), this, SLOT(slot_add_all_signatures_to_case_tree(QStringList)));
    connect(thread_signature_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_signature_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_signature_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)));
    connect(thread_signature_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);
    connect(thread_signature_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    bool_thread_signature_running = true;
    thread_signature_extract->start();
}

void RECON_case::pub_start_run_fs_module_mime_type_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    if(bool_thread_mime_type_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Mime Type already running.");
        return;
    }

    thread_mime_type_obj = new thread_mime_type;

    thread_mime_type_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_mime_type_obj->set_recon_case_runner_type(QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE));

    thread_mime_type_obj->pub_set_fs_run_module_file_info_list(run_fs_modules_details_list);


    thread_mime_extract = new QThread;
    thread_mime_type_obj->moveToThread(thread_mime_extract);

    connect(thread_mime_extract, SIGNAL(started()), thread_mime_type_obj, SLOT(slot_extract_mime_type()));
    connect(thread_mime_type_obj, SIGNAL(finished()), thread_mime_extract, SLOT(quit()));
    connect(thread_mime_type_obj, SIGNAL(finished()), this, SLOT(slot_mime_type_thread_finish()));
    connect(thread_mime_type_obj, SIGNAL(signal_add_mime_types_to_case_tree(QStringList)), this, SLOT(slot_add_mime_type_to_case_tree(QStringList)));
    connect(thread_mime_type_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    connect(thread_mime_type_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_mime_type_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_mime_type_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);

    bool_thread_mime_type_running = true;
    thread_mime_extract->start();

}

void RECON_case::pub_start_run_fs_module_hashset(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    if(bool_thread_hashes_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Hashset already running.");
        return;
    }

    thread_hashes_obj = new thread_hashes;

    thread_hashes_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_hashes_obj->set_recon_case_runner_type(QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE));

    thread_hashes_obj->pub_set_fs_run_module_file_info_list(run_fs_modules_details_list);

    thread_hashes_data_extract = new QThread;
    thread_hashes_obj->moveToThread(thread_hashes_data_extract);

    connect(thread_hashes_data_extract, SIGNAL(started()), thread_hashes_obj, SLOT(slot_hashset_extraction()));
    connect(thread_hashes_obj, SIGNAL(finished()), thread_hashes_data_extract, SLOT(quit()));
    connect(thread_hashes_obj, SIGNAL(finished()), this, SLOT(slot_hashes_thread_finish()));
    connect(thread_hashes_obj, SIGNAL(signal_add_matched_hashes_for_fs_module_to_case_tree(QStringList)), this, SLOT(slot_add_matched_hashes_for_fs_module_to_case_tree(QStringList)));
    connect(thread_hashes_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_hashes_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_hashes_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)));
    connect(thread_hashes_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);
    connect(thread_hashes_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    bool_thread_hashes_running = true;

    thread_hashes_data_extract->start();
}

void RECON_case::pub_start_run_fs_module_face_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    if(bool_thread_face_analysis_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Face Analysis already running.");
        return;
    }

    thread_face_analysis_obj = new thread_face_analysis;

    thread_face_analysis_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_face_analysis_obj->set_recon_case_runner_type(QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE));

    thread_face_analysis_obj->pub_set_fs_run_module_file_info_list(run_fs_modules_details_list);


    thread_face_extract = new QThread;
    thread_face_analysis_obj->moveToThread(thread_face_extract);

    connect(thread_face_extract, SIGNAL(started()), thread_face_analysis_obj, SLOT(slot_extract_faces()));
    connect(thread_face_analysis_obj, SIGNAL(finished()), thread_face_extract, SLOT(quit()));
    connect(thread_face_analysis_obj, SIGNAL(finished()), this, SLOT(slot_face_analysis_thread_finished()));
    connect(thread_face_analysis_obj, SIGNAL(signal_add_faces_to_case_tree(QStringList)), this, SLOT(slot_add_faces_to_case_tree(QStringList)));
    connect(thread_face_analysis_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    connect(thread_face_analysis_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_face_analysis_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_face_analysis_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(thread_face_analysis_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);

    bool_thread_face_analysis_running = true;
    thread_face_extract->start();

}

void RECON_case::pub_start_run_fs_module_optical_character_recognition(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    if(bool_thread_optical_character_recognition_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Optical Character Recognition already running.");
        return;
    }

    thread_optical_character_recognition_obj = new thread_optical_character_recognition;

    thread_optical_character_recognition_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_optical_character_recognition_obj->set_recon_case_runner_type(QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE));

    thread_optical_character_recognition_obj->pub_set_fs_run_module_file_info_list(run_fs_modules_details_list);

    thread_optical_character_recognition_extract = new QThread;
    thread_optical_character_recognition_obj->moveToThread(thread_optical_character_recognition_extract);

    connect(thread_optical_character_recognition_extract, SIGNAL(started()), thread_optical_character_recognition_obj, SLOT(slot_extract_optical_character_recognition()));
    connect(thread_optical_character_recognition_obj, SIGNAL(finished()), thread_optical_character_recognition_extract, SLOT(quit()));
    connect(thread_optical_character_recognition_obj, SIGNAL(finished()), this, SLOT(slot_optical_character_recognition_thread_finish()));
    connect(thread_optical_character_recognition_obj, SIGNAL(signal_add_optical_character_recognition_to_case_tree(QStringList)), this, SLOT(slot_add_optical_character_recognition_to_case_tree(QStringList)));
    connect(thread_optical_character_recognition_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    connect(thread_optical_character_recognition_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_optical_character_recognition_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_optical_character_recognition_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(thread_optical_character_recognition_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);

    bool_thread_optical_character_recognition_running = true;
    thread_optical_character_recognition_extract->start();

}

void RECON_case::pub_start_run_fs_module_weapons(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    if(bool_thread_weapons_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Weapons already running.");
        return;
    }

    thread_weapons_obj = new thread_weapons;

    thread_weapons_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_weapons_obj->set_recon_case_runner_type(QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE));

    thread_weapons_obj->pub_set_fs_run_module_file_info_list(run_fs_modules_details_list);

    thread_weapons_extraction = new QThread;
    thread_weapons_obj->moveToThread(thread_weapons_extraction);

    connect(thread_weapons_extraction, SIGNAL(started()), thread_weapons_obj, SLOT(slot_weapons_extraction()));
    connect(thread_weapons_obj, SIGNAL(finished()), thread_weapons_extraction, SLOT(quit()));
    connect(thread_weapons_obj, SIGNAL(finished()), this, SLOT(slot_weapons_thread_finished()));
    connect(thread_weapons_obj, SIGNAL(signal_add_weapons_to_case_tree(QStringList)), this, SLOT(slot_add_weapons_to_case_tree(QStringList)));
    connect(thread_weapons_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    connect(thread_weapons_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_weapons_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_weapons_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(thread_weapons_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);

    bool_thread_weapons_running = true;
    thread_weapons_extraction->start();

}

void RECON_case::pub_start_run_fs_module_fire_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    if(bool_thread_fire_analysis_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Fire Analysis already running.");
        return;
    }

    thread_fire_analysis_obj = new thread_fire_analysis;

    thread_fire_analysis_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_fire_analysis_obj->set_recon_case_runner_type(QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE));

    thread_fire_analysis_obj->pub_set_fs_run_module_file_info_list(run_fs_modules_details_list);

    thread_fire_analysis_extraction = new QThread;
    thread_fire_analysis_obj->moveToThread(thread_fire_analysis_extraction);

    connect(thread_fire_analysis_extraction, SIGNAL(started()), thread_fire_analysis_obj, SLOT(slot_fire_analysis_extraction()));
    connect(thread_fire_analysis_obj, SIGNAL(finished()), thread_fire_analysis_extraction, SLOT(quit()));
    connect(thread_fire_analysis_obj, SIGNAL(finished()), this, SLOT(slot_fire_analysis_thread_finished()));
    connect(thread_fire_analysis_obj, SIGNAL(signal_add_fire_analysis_to_case_tree(QStringList)), this, SLOT(slot_add_fire_analysis_to_case_tree(QStringList)));
    connect(thread_fire_analysis_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    connect(thread_fire_analysis_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_fire_analysis_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_fire_analysis_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(thread_fire_analysis_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);

    bool_thread_fire_analysis_running = true;
    thread_fire_analysis_extraction->start();

}

void RECON_case::pub_start_run_fs_module_skin_tone_detection(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    if(bool_thread_skin_tone_detection_running)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Skin Tone Detection already running.");
        return;
    }

    thread_skin_tone_detection_obj = new thread_skin_tone_detection;

    thread_skin_tone_detection_obj->pub_set_mutex_for_fs_main_db(mutex_exclusive_for_fs_main_db);
    thread_skin_tone_detection_obj->set_recon_case_runner_type(QString(MACRO_RECON_CASE_RUNNER_TYPE_FS_MODULE));

    thread_skin_tone_detection_obj->pub_set_fs_run_module_file_info_list(run_fs_modules_details_list);

    thread_skin_tone_detection_extraction = new QThread;
    thread_skin_tone_detection_obj->moveToThread(thread_skin_tone_detection_extraction);

    connect(thread_skin_tone_detection_extraction, SIGNAL(started()), thread_skin_tone_detection_obj, SLOT(slot_extract_skin_tone_detection()));
    connect(thread_skin_tone_detection_obj, SIGNAL(finished()), thread_skin_tone_detection_extraction, SLOT(quit()));
    connect(thread_skin_tone_detection_obj, SIGNAL(finished()), this, SLOT(slot_skin_tone_detection_thread_finished()));

    connect(thread_skin_tone_detection_obj, SIGNAL(signal_add_skin_tone_detection_to_case_tree(QStringList)), this, SLOT(slot_add_skin_tone_detection_to_case_tree(QStringList)));
    connect(thread_skin_tone_detection_obj, SIGNAL(signal_completion_status(QString,QString,QString)), this, SLOT(slot_job_type_completion_status(QString,QString,QString)), Qt::QueuedConnection);

    connect(thread_skin_tone_detection_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_skin_tone_detection_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_skin_tone_detection_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);
    connect(thread_skin_tone_detection_obj, SIGNAL(signal_PBAR_thread_file_system_required_time(QString,QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_required_time(QString,QString)), Qt::QueuedConnection);

    bool_thread_skin_tone_detection_running = true;
    thread_skin_tone_detection_extraction->start();

}

QMutex* RECON_case::pub_get_fs_db_exclusive_mutex()
{
    return mutex_exclusive_for_fs_main_db;
}

void RECON_case::slot_prc_apple_metadata_prc_left_click_finished(int status)
{
    emit signal_prc_apple_metadata_prc_left_click_finished();
}
