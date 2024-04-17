#include "task_status_window.h"
#include "ui_task_status_window.h"
#include <QLineEdit>

task_status_window::task_status_window(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::task_status_window)
{
    ui->setupUi(this);

    semaphore_sub_process_status_window = new QSemaphore(1);

    qRegisterMetaType<struct_global_task_type_and_title>("struct_global_task_type_and_title");

    bool_status_button_checked = true;
}

task_status_window::~task_status_window()
{
    deleteLater();
    // delete ui;
}

void task_status_window::set_fix_size(int w, int h)
{
    this->setFixedSize(w,h);
}

void task_status_window::pub_set_source_count_name(QString naming)
{
    source_count_name = naming;
}


void task_status_window::set_task_in_taskbar(QString name, QString type)
{
    recon_static_functions::app_debug(" start", Q_FUNC_INFO);

    task_progress_bar *sub_window = new task_progress_bar(this);

    connect(sub_window, SIGNAL(signal_task_cancelled_requested(struct_global_task_type_and_title)), this, SLOT(slot_job_cancelled_requested_artifacts_plugin(struct_global_task_type_and_title)),Qt::QueuedConnection);
    connect(sub_window, SIGNAL(signal_skip_triggered()), this, SLOT(slot_skip_triggered()));
    struct struct_global_task_type_and_title t1;
    t1.type = type;
    t1.title = name;

    t1.source_count_name = source_count_name;

    QString icon_path;

    if(type == MACRO_JobType_Artifacts)
    {
        QString icon_name = t1.title;
        icon_name = icon_name.replace(" ","_") + ".png";

        icon_path = "../icons/plugins/" + icon_name;
    }
    else if(type == MACRO_JobType_Adding_Source_To_Case)
    {
        icon_path = "../icons/Features/source.png";
    }
    else if(type == MACRO_JobType_Artifacts_Timeline)
    {
        QString tmp_type = MACRO_JobType_Artifacts_Timeline;
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;

    }
    else if(type == MACRO_JobType_Artifacts_Keyword_Search)
    {
        QString tmp_type = MACRO_JobType_Artifacts_Keyword_Search;
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_Content_Search))
    {
        QString tmp_type = QString(MACRO_JobType_Content_Search);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;
        sub_window->pub_set_infinite_progress_bar();
    }
    else if(type == QString(MACRO_JobType_Text_Indexing))
    {
        QString tmp_type = QString(MACRO_JobType_Text_Indexing);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;
        sub_window->pub_set_infinite_progress_bar();

    }
    else if(type == QString(MACRO_JobType_Apple_Metadata))
    {
        QString tmp_type = QString(MACRO_JobType_Apple_Metadata);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_Signature_Analysis))
    {
        QString tmp_type = QString(MACRO_JobType_Signature_Analysis);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_Exif_Metadata))
    {
        QString tmp_type = QString(MACRO_JobType_Exif_Metadata);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_Hashes))
    {
        QString tmp_type = QString(MACRO_JobType_Hashes);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_Mime_Type))
    {
        QString tmp_type = QString(MACRO_JobType_Mime_Type);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_Face_Analysis))
    {
        QString tmp_type = QString(MACRO_JobType_Face_Analysis);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_Face_Search))
    {
        QString tmp_type = QString(MACRO_JobType_Face_Search);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_File_Search))
    {
        QString tmp_type = QString(MACRO_JobType_File_Search);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;

        sub_window->pub_set_infinite_progress_bar();
    }
    else if(type == QString(MACRO_JobType_Optical_Character_Recognition))
    {
        QString tmp_type = QString(MACRO_JobType_Optical_Character_Recognition);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_Skin_Tone_Detection))
    {
        QString tmp_type = QString(MACRO_JobType_Skin_Tone_Detection);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";
        icon_path = "../icons/Features/" + tmp_type;

    }
    else if(type == QString(MACRO_JobType_Weapons))
    {
        QString tmp_type = QString(MACRO_JobType_Weapons);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";
        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_Fire_Analysis))
    {
        QString tmp_type = QString(MACRO_JobType_Fire_Analysis);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";
        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_Artifact_Extraction))
    {
        QString tmp_type = QString(MACRO_JobType_File_Search);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;

        sub_window->pub_set_infinite_progress_bar();
        sub_window->pub_show_skip_label(true);
    }
    else if(type == QString(MACRO_JobType_Verifying))
    {
        QString tmp_type = QString(MACRO_JobType_Verifying);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_Email_Parser_Pst_Mail))
    {
        QString tmp_type = QString(MACRO_JobType_Email_Parser_Pst_Mail);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";

        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_Email_Parser_Emlx_Mail))
    {
        QString tmp_type = QString(MACRO_JobType_Email_Parser_Emlx_Mail);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";
        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_Email_Parser_Mbox_Mail))
    {
        QString tmp_type = QString(MACRO_JobType_Email_Parser_Mbox_Mail);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";
        icon_path = "../icons/Features/" + tmp_type;
    }
    else if(type == QString(MACRO_JobType_Unified_Logs))
    {
        QString tmp_type = QString(MACRO_JobType_Unified_Logs);
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = tmp_type + ".png";
        icon_path = "../icons/Features/" + tmp_type;
    }
    else
    {
        QString tmp_type = type;
        tmp_type = tmp_type.toLower();
        tmp_type = tmp_type.replace(" ","_");
        tmp_type = "sub_" + tmp_type + ".png";
        icon_path = "../icons/signs/" + tmp_type ;
    }

    sub_window->set_job_type_and_title(t1,icon_path);
    add_progress_widget(sub_window);

    //-Add info to the structure with row count.
    struct_task_progress_bar_info info;
    info.task_progress_bar_obj = sub_window;
    info.row_position = ui->tableWidget_status_window->rowCount();
    status_task_progress_bar_list << info;

    if(ui->tableWidget_status_window->rowCount() > 0)
    {
        if(!bool_status_button_checked)
        {
            sub_mainwindow_statusbar_update();
        }
    }
    else
    {
        emit signal_current_item_for_statusbar("");
        bool_status_button_checked = true;
    }


}


void task_status_window::show_keyword_search(QString search_name)
{

    //    status_sub_process_window *sub_window = new status_sub_process_window(this);
    //    connect(sub_window, SIGNAL(signal_task_cancelled_requested(job_type_and_title)), this, SLOT(slot_job_cancelled_requested(job_type_and_title)),Qt::QueuedConnection);
    //    struct job_type_and_title t1;
    //    t1.type = JobType_K_Search;
    //    t1.title = search_name;

    //    sub_window->set_job_type_and_title(t1,icon_path);
    //    add_progress_widget(sub_window);


    //    //-Add ifo to the structure with row count.
    //    struct_sub_process_windo_info info;
    //    info.status_sub_process_window_obj = sub_window;
    //    info.row_position = ui->tableWidget_status_window->rowCount();
    //    status_sub_process_window_list << info;

}

void task_status_window::add_progress_widget(task_progress_bar *widget)
{
    widget->setParent(this);

    int i = ui->tableWidget_status_window->rowCount();
    ui->tableWidget_status_window->insertRow(i);
    ui->tableWidget_status_window->setRowHeight(i,53);
    ui->tableWidget_status_window->setCellWidget(i,0,widget);

    show();
}

void task_status_window::slot_set_progress_bar_max_value(struct_global_task_type_and_title type_and_title, int value)
{
    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);

    while(1) {
        if(semaphore_sub_process_status_window->tryAcquire(1))
            break;

        QCoreApplication::processEvents();
    }

    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    currently_running_plugin_title_in_status_window = type_and_title.title;
    currently_running_plugin_category_in_status_window = type_and_title.type;

    for(int i = 0 ; i < status_task_progress_bar_list.size(); i++)
    {
        task_progress_bar *item = status_task_progress_bar_list.at(i).task_progress_bar_obj;

        if(item->get_job_type_and_title().title == type_and_title.title && item->get_job_type_and_title().type == type_and_title.type)
        {
            for(int jj = 0; jj < ui->tableWidget_status_window->rowCount(); jj++)
            {
                task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(jj,0));
                if(m_item == item)
                {
                    if(!bool_status_button_checked)
                    {
                        emit signal_current_item_for_statusbar(m_item->pub_get_current_feature_name_and_status());
                    }

                    m_item->set_progress_maximum_bar_value(value);

                    semaphore_sub_process_status_window->release(1);
                    return;
                }
            }
        }
    }

    semaphore_sub_process_status_window->release(1);
}

void task_status_window::common_remove_sub_process_status_window_object(struct_global_task_type_and_title type_and_title)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1) {
        if(semaphore_sub_process_status_window->tryAcquire(1))
            break;

        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    for(int i = 0 ; i < status_task_progress_bar_list.size(); i++)
    {
        task_progress_bar *item = status_task_progress_bar_list.at(i).task_progress_bar_obj;

        if(item->get_job_type_and_title().title == type_and_title.title && item->get_job_type_and_title().type == type_and_title.type)
        {

            for(int jj = 0; jj < ui->tableWidget_status_window->rowCount(); jj++)
            {
                task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(jj,0));
                if(m_item == item)
                {
                    status_task_progress_bar_list.removeAt(i);
                    ui->tableWidget_status_window->removeRow(jj);
                    semaphore_sub_process_status_window->release(1);
                    return;
                }
            }
        }
    }

    semaphore_sub_process_status_window->release(1);
    if(status_task_progress_bar_list.size() < 1)
        hide();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void task_status_window::slot_remove_sub_process_status_window_object(struct_global_task_type_and_title type_and_title)
{
    common_remove_sub_process_status_window_object(type_and_title);

    if(ui->tableWidget_status_window->rowCount() > 0)
    {
        task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(0,0));

        if(!bool_status_button_checked)
            emit signal_current_item_for_statusbar(m_item->pub_get_current_feature_name_and_status());

    }
    else
    {
        emit signal_current_item_for_statusbar("");
        bool_status_button_checked = true;
        hide();
    }
}


void task_status_window::slot_remove_hashset_status_window_object(struct_global_task_type_and_title type_and_title)
{
    remove_hashset_status_window_obj(type_and_title);

    if(ui->tableWidget_status_window->rowCount() > 0)
    {
        task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(0,0));
        if(!bool_status_button_checked)
            emit signal_current_item_for_statusbar(m_item->pub_get_current_feature_name_and_status());
    }
    else
    {
        emit signal_current_item_for_statusbar("");
        bool_status_button_checked = true;
    }
}

void task_status_window::remove_hashset_status_window_obj(struct_global_task_type_and_title type_and_title)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    for(int i = 0 ; i < status_task_progress_bar_list.size(); i++)
    {
        task_progress_bar *item = status_task_progress_bar_list.at(i).task_progress_bar_obj;

        if(item->get_job_type_and_title().type == type_and_title.type && item->get_job_type_and_title().source_count_name == type_and_title.source_count_name)
        {
            for(int jj = 0; jj < ui->tableWidget_status_window->rowCount(); jj++)
            {
                task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(jj,0));
                if(m_item == item)
                {
                    status_task_progress_bar_list.removeAt(i);
                    ui->tableWidget_status_window->removeRow(jj);
                    return;
                }
            }
        }
    }

    recon_static_functions::app_debug(" start",Q_FUNC_INFO);
}

void task_status_window::slot_PBAR_file_system_started(QString job_type)
{
    task_progress_bar_file_system = new task_progress_bar(this);

    connect(task_progress_bar_file_system, SIGNAL(signal_task_cancelled_requested(struct_global_task_type_and_title)), this, SLOT(slot_PBAR_file_system_job_cancelled_requested(struct_global_task_type_and_title)),Qt::QueuedConnection);

    connect(task_progress_bar_file_system, SIGNAL(signal_skip_triggered()), this, SLOT(slot_skip_triggered()));

    if(job_type == MACRO_JobType_Adding_Source_To_Case || job_type == MACRO_JobType_Analysing_Snapshot || job_type == MACRO_JobType_Processing_Unallocated_Data)
    {
        show();
    }

    struct struct_global_task_type_and_title t1;
    t1.type = job_type;
    t1.title = "";

    t1.source_count_name = source_count_name;

    QString icon_path;

    QString tmp_type = job_type;
    tmp_type = tmp_type.toLower();
    tmp_type = tmp_type.replace(" ","_");
    tmp_type = tmp_type + ".png";
    icon_path = "../icons/Features/" + tmp_type;

    if(job_type == MACRO_JobType_Artifact_Loading)
    {
        icon_path = "../icons/Features/" + QString(MACRO_JobType_Artifacts);
    }

    task_progress_bar_file_system->set_job_type_and_title(t1,icon_path);
    add_progress_widget(task_progress_bar_file_system);

    struct_task_progress_bar_info info;
    info.task_progress_bar_obj = task_progress_bar_file_system;
    info.row_position = ui->tableWidget_status_window->rowCount();
    status_task_progress_bar_list << info;

    if(ui->tableWidget_status_window->rowCount() > 0)
    {
        if(!bool_status_button_checked)
        {
            sub_mainwindow_statusbar_update();
        }
    }
    else
    {
        emit signal_current_item_for_statusbar("");
        bool_status_button_checked = true;
    }
}

void task_status_window::slot_PBAR_file_system_finished(QString job_type)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1) {
        if(semaphore_sub_process_status_window->tryAcquire(1))
            break;

        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    for(int i = 0 ; i < status_task_progress_bar_list.size(); i++)
    {
        task_progress_bar *item = status_task_progress_bar_list.at(i).task_progress_bar_obj;

        if(item->get_job_type_and_title().type == job_type)
        {

            for(int jj = 0; jj < ui->tableWidget_status_window->rowCount(); jj++)
            {
                task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(jj,0));
                if(m_item == item)
                {
                    status_task_progress_bar_list.removeAt(i);
                    ui->tableWidget_status_window->removeRow(jj);
                    semaphore_sub_process_status_window->release(1);

                    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

                    return;
                }
            }
        }
    }

    semaphore_sub_process_status_window->release(1);

    refresh_task_bar();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

}

void task_status_window::slot_PBAR_file_system_required_time(QString job_type, QString estimated_time)
{
    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        if(semaphore_sub_process_status_window->tryAcquire(1))
            break;

        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    if(job_type == QString(MACRO_JobType_Fire_Analysis)
            || job_type == QString(MACRO_JobType_Apple_Metadata)
            || job_type == QString(MACRO_JobType_Face_Analysis)
            || job_type == QString(MACRO_JobType_Optical_Character_Recognition)
            || job_type == QString(MACRO_JobType_Skin_Tone_Detection)
            || job_type == QString(MACRO_JobType_Weapons)
            || job_type == QString(MACRO_JobType_Exif_Metadata)
            || job_type == QString(MACRO_JobType_Signature_Analysis)
            || job_type == QString(MACRO_JobType_Mime_Type)
            || job_type == QString(MACRO_JobType_Hashes)
            || job_type == QString(MACRO_JobType_Extract_Apple_Timestamps)
            || job_type == QString(MACRO_JobType_Email_Parser_Pst_Mail)
            || job_type == QString(MACRO_JobType_Email_Parser_Emlx_Mail))
    {
        for(int i = 0 ; i < status_task_progress_bar_list.size(); i++)
        {
            task_progress_bar *item = status_task_progress_bar_list.at(i).task_progress_bar_obj;

            if(item->get_job_type_and_title().type == job_type)
            {
                for(int jj = 0; jj < ui->tableWidget_status_window->rowCount(); jj++)
                {
                    task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(jj,0));
                    if(m_item == item)
                    {
                        m_item->common_set_extracted_estimated_time(estimated_time);
                        semaphore_sub_process_status_window->release(1);
                        return;
                    }
                }
            }
        }
    }

    semaphore_sub_process_status_window->release(1);
    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

}

void task_status_window::slot_PBAR_file_system_value(QString job_type, QString message, bool bool_show_close_button, qint64 records, int progress, bool bool_infinite_process)
{
    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        if(semaphore_sub_process_status_window->tryAcquire(1))
            break;

        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    if(job_type == QString(MACRO_JobType_Adding_Source_To_Case)
            || job_type == QString(MACRO_JobType_Apple_Metadata)
            || job_type == QString(MACRO_JobType_Extract_Apple_Timestamps)
            || job_type == QString(MACRO_JobType_Exif_Metadata)
            || job_type == QString(MACRO_JobType_Signature_Analysis)
            || job_type == QString(MACRO_JobType_Mime_Type)
            || job_type == QString(MACRO_JobType_Hashes)
            || job_type == QString(MACRO_JobType_Artifact_Extraction)
            || job_type == QString(MACRO_JobType_Text_Indexing)
            || job_type == QString(MACRO_JobType_Carved_Files)
            || job_type == QString(MACRO_JobType_Carved_Data)
            || job_type == QString(MACRO_JobType_Analysing_Snapshot)
            || job_type == QString(MACRO_JobType_Processing_Unallocated_Data)
            || job_type == QString(MACRO_JobType_Verifying)
            || job_type == QString(MACRO_JobType_Content_Search)
            || job_type == QString(MACRO_JobType_Exif_Metadata_Search)
            || job_type == QString(MACRO_JobType_Artifact_Loading)
            || job_type == QString(MACRO_JobType_Face_Analysis)
            || job_type == QString(MACRO_JobType_Face_Search)
            || job_type == QString(MACRO_JobType_Optical_Character_Recognition)
            || job_type == QString(MACRO_JobType_Skin_Tone_Detection)
            || job_type == QString(MACRO_JobType_Weapons)
            || job_type == QString(MACRO_JobType_Fire_Analysis)
            || job_type == QString(MACRO_JobType_Email_Parser_Pst_Mail)
            || job_type == QString(MACRO_JobType_Email_Parser_Emlx_Mail)
            || job_type == QString(MACRO_JobType_Email_Parser_Mbox_Mail)
            || job_type == QString(MACRO_JobType_Unified_Logs)
            )
    {
        for(int i = 0 ; i < status_task_progress_bar_list.size(); i++)
        {
            task_progress_bar *item = status_task_progress_bar_list.at(i).task_progress_bar_obj;

            if(item->get_job_type_and_title().type == job_type)
            {

                for(int jj = 0; jj < ui->tableWidget_status_window->rowCount(); jj++)
                {
                    task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(jj,0));
                    if(m_item == item)
                    {
                        struct_global_task_type_and_title type_and_title;

                        type_and_title.title = message;
                        type_and_title.type  = job_type;

                        m_item->common_set_extracted_records_in_progress(records, type_and_title, bool_show_close_button, progress,bool_infinite_process);

                        if(!bool_status_button_checked)
                        {
                            emit signal_current_item_for_statusbar(m_item->pub_get_current_feature_name_and_status());
                        }

                        semaphore_sub_process_status_window->release(1);
                        return;
                    }
                }
            }
        }

    }

    semaphore_sub_process_status_window->release(1);

}

void task_status_window::slot_PBAR_plugin_extraction_status(QString plugin_name, qint64 record_count, QString current_count_and_total_plugins)
{
    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        if(semaphore_sub_process_status_window->tryAcquire(1))
            break;

        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    for(int i = 0 ; i < status_task_progress_bar_list.size(); i++)
    {
        task_progress_bar *item = status_task_progress_bar_list.at(i).task_progress_bar_obj;

        if(item->get_job_type_and_title().type == MACRO_JobType_Artifact_Extraction)
        {

            for(int jj = 0; jj < ui->tableWidget_status_window->rowCount(); jj++)
            {
                task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(jj,0));
                if(m_item == item)
                {

                    QString str = current_count_and_total_plugins + " " + plugin_name + " - " + QString::number(record_count);

                    m_item->pub_set_progress_message(str);
                    m_item->pub_set_icon_for_plugin(plugin_name);
                    m_item->pub_hide_cancel_button();
                    m_item->pub_set_infinite_progress_bar();
                    m_item->pub_show_skip_label(true);

                    semaphore_sub_process_status_window->release(1);
                    return;
                }
            }
        }
    }

    semaphore_sub_process_status_window->release(1);
}

void task_status_window::slot_PBAR_plugin_extraction_status_message(QString plugin_name,  QString message)
{
    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        if(semaphore_sub_process_status_window->tryAcquire(1))
            break;

        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    for(int i = 0 ; i < status_task_progress_bar_list.size(); i++)
    {
        task_progress_bar *item = status_task_progress_bar_list.at(i).task_progress_bar_obj;

        if(item->get_job_type_and_title().type == MACRO_JobType_Artifact_Extraction)
        {

            for(int jj = 0; jj < ui->tableWidget_status_window->rowCount(); jj++)
            {
                task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(jj,0));
                if(m_item == item)
                {

                    QString str =  plugin_name + " - " + message;

                    m_item->pub_set_progress_message(str);
                    m_item->pub_set_icon_for_plugin(plugin_name);
                    m_item->pub_hide_cancel_button();
                    m_item->pub_set_infinite_progress_bar();
                    m_item->pub_show_skip_label(false);

                    semaphore_sub_process_status_window->release(1);
                    return;
                }
            }
        }
    }

    semaphore_sub_process_status_window->release(1);
}

void task_status_window::set_next_task_in_same_taskbar_for_fs(struct_global_task_type_and_title finished_task, struct_global_task_type_and_title next_task)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        if(semaphore_sub_process_status_window->tryAcquire(1))
            break;

        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    for(int i = 0 ; i < status_task_progress_bar_list.size(); i++)
    {
        task_progress_bar *item = status_task_progress_bar_list.at(i).task_progress_bar_obj;

        if(item->get_job_type_and_title().type == finished_task.type && item->get_job_type_and_title().source_count_name == finished_task.source_count_name)
        {
            for(int jj = 0; jj < ui->tableWidget_status_window->rowCount(); jj++)
            {
                task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(jj,0));
                if(m_item == item)
                {
                    m_item->set_next_task_with_same_parent_in_status_bar_for_fs(next_task);

                    if(!bool_status_button_checked)
                    {
                        emit signal_current_item_for_statusbar(m_item->pub_get_current_feature_name_and_status());
                    }

                    semaphore_sub_process_status_window->release(1);
                    return;
                }
            }
        }
    }

    semaphore_sub_process_status_window->release(1);

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);
}

void task_status_window::slot_mainwindow_statusbar_button_clicked(bool checked)
{
    bool_status_button_checked = checked;

    if(!bool_status_button_checked)
    {
        sub_mainwindow_statusbar_update();
    }
    else
        emit signal_current_item_for_statusbar("");

}

void task_status_window::sub_mainwindow_statusbar_update()
{
    for(int i = 0 ; i < status_task_progress_bar_list.size(); i++)
    {
        task_progress_bar *item = status_task_progress_bar_list.at(i).task_progress_bar_obj;

        if(item->get_job_type_and_title().title == currently_running_plugin_title_in_status_window && item->get_job_type_and_title().type == currently_running_plugin_category_in_status_window)
        {
            for(int jj = 0; jj < ui->tableWidget_status_window->rowCount(); jj++)
            {
                task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(jj,0));
                if(m_item == item)
                {
                    emit signal_current_item_for_statusbar(m_item->pub_get_current_feature_name_and_status());
                    return;
                }
            }
        }
    }

}

void task_status_window::refresh_task_bar()
{
    if(ui->tableWidget_status_window->rowCount() > 0)
    {
        task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(0,0));

        if(!bool_status_button_checked)
            emit signal_current_item_for_statusbar(m_item->pub_get_current_feature_name_and_status());

    }
    else
    {
        emit signal_current_item_for_statusbar("");
        bool_status_button_checked = true;
        hide();
    }

}


void task_status_window::slot_job_cancelled_requested_artifacts_plugin(struct_global_task_type_and_title type_and_title)
{
    recon_static_functions::app_debug("status_window::slot_job_cancelled_requested start",Q_FUNC_INFO);


    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1) {
        if(semaphore_sub_process_status_window->tryAcquire(1))
            break;
        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    for(int i = 0 ; i < status_task_progress_bar_list.size(); i++)
    {
        task_progress_bar *item = status_task_progress_bar_list.at(i).task_progress_bar_obj;

        if(item->get_job_type_and_title().title == type_and_title.title && item->get_job_type_and_title().type == type_and_title.type)
        {
            semaphore_sub_process_status_window->release(1);


            if(!currently_running_plugin_title_in_status_window.isEmpty() &&
                    currently_running_plugin_title_in_status_window != type_and_title.title)
            {
                common_remove_sub_process_status_window_object(type_and_title);
            }


            recon_static_functions::debug_conditional(" : running for loop", Q_FUNC_INFO);
            while(1) {
                if(semaphore_sub_process_status_window->tryAcquire(1))
                    break;
                QCoreApplication::processEvents();
            }
            recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

            if(type_and_title.type == MACRO_JobType_Artifacts)
            {
                emit signal_cancel_plugin_extraction(type_and_title);
            }
            else if(type_and_title.type == MACRO_JobType_Artifacts_Keyword_Search)
            {
                emit signal_cancel_keyword_search(type_and_title);
            }
            else if(type_and_title.type == MACRO_JobType_Content_Search)
            {
                emit signal_cancel_content_search(type_and_title);
            }
            else if(type_and_title.type == MACRO_JobType_Verifying)
            {
                emit signal_cancel_source_verification(type_and_title);
            }

            semaphore_sub_process_status_window->release(1);
            return;
        }
    }

    semaphore_sub_process_status_window->release(1);

    recon_static_functions::app_debug("status_window::slot_job_cancelled_requested end",Q_FUNC_INFO);

}

void task_status_window::slot_skip_triggered()
{
    emit signal_skip_triggered();
}

void task_status_window::slot_PBAR_file_system_job_cancelled_requested(struct_global_task_type_and_title type_and_title)
{
    recon_static_functions::app_debug("start",Q_FUNC_INFO);

    if(type_and_title.type == MACRO_JobType_Carved_Files)
    {
        emit signal_cancel_carved_files_extraction(type_and_title.type);
    }
    else if(type_and_title.type == MACRO_JobType_Carved_Data)
    {
        emit signal_cancel_carved_data_extraction(type_and_title.type);
    }
    else if(type_and_title.type == MACRO_JobType_Processing_Unallocated_Data)
    {
        emit signal_cancel_disk_hex_view_unallocated_extraction(type_and_title.type);
    }
    else if(type_and_title.type == MACRO_JobType_Face_Search)
    {
        emit signal_cancel_face_search(type_and_title.type);
    }
    else
    {
        // these jobs pointer is in Recon Case
        emit signal_PBAR_file_system_job_cancelled_requested(type_and_title.type);
    }

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void task_status_window::set_extracted_records_in_progress_for_artifacts_plugin(qint64 records)
{

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1) {
        if(semaphore_sub_process_status_window->tryAcquire(1))
            break;
        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    struct_global_task_type_and_title type_and_title;

    for(int row = 0; row < ui->tableWidget_status_window->rowCount(); row++)
    {
        QPointer<task_progress_bar> m_item = new task_progress_bar(this);
        m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(row,0));

        if(m_item == NULL)
            continue;


        type_and_title = m_item->get_job_type_and_title();

        if(m_item->get_job_type_and_title().title ==  currently_running_plugin_title_in_status_window)
        {
            m_item->common_set_extracted_records_in_progress(records, type_and_title, true, -1, false);

            if(!bool_status_button_checked)
            {
                emit signal_current_item_for_statusbar(m_item->pub_get_current_feature_name_and_status());
            }

            break;
        }

    }

    semaphore_sub_process_status_window->release(1);

}

int task_status_window::get_running_tasks_count()
{
    return ui->tableWidget_status_window->rowCount();
}

void task_status_window::set_progressbar_value(int tabs , QString job_title)
{

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1) {
        if(semaphore_sub_process_status_window->tryAcquire(1))
            break;
        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);


    for(int row = 0; row < ui->tableWidget_status_window->rowCount(); row++)
    {

        QPointer<task_progress_bar> m_item = new task_progress_bar(this);
        m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(row,0));

        if(m_item == NULL)
            continue;


        if(m_item->get_job_type_and_title().title ==  job_title || m_item->get_job_type_and_title().type ==  job_title)
        {


            m_item->pub_progressbar_set_value(tabs);
            if(!bool_status_button_checked)
            {
                emit signal_current_item_for_statusbar(m_item->pub_get_current_feature_name_and_status());
            }

            break;
        }
    }

    semaphore_sub_process_status_window->release(1);

}

void task_status_window::pub_change_display_message_in_task_progress_bar(QString task_type, QString plugin_name, QString message, bool bool_show_close_button, int max_size_of_progress)
{
    if(max_size_of_progress > 100)
        return;

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        if(semaphore_sub_process_status_window->tryAcquire(1))
            break;
        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    if(task_type == QString(MACRO_JobType_Adding_Source_To_Case))
    {
        for(int i = 0 ; i < status_task_progress_bar_list.size(); i++)
        {
            task_progress_bar *item = status_task_progress_bar_list.at(i).task_progress_bar_obj;

            if(item->get_job_type_and_title().type == task_type && item->get_job_type_and_title().source_count_name == plugin_name)
            {
                for(int jj = 0; jj < ui->tableWidget_status_window->rowCount(); jj++)
                {
                    task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(jj,0));
                    if(m_item == item)
                    {


                        m_item->change_display_messge_in_progress_display(message, bool_show_close_button, max_size_of_progress);

                        if(!bool_status_button_checked)
                        {
                            emit signal_current_item_for_statusbar(m_item->pub_get_current_feature_name_and_status());
                        }

                        semaphore_sub_process_status_window->release(1);
                        return;
                    }
                }

                break;
            }
        }
    }
    else
    {
        struct_global_task_type_and_title type_and_title;

        for(int row = 0; row < ui->tableWidget_status_window->rowCount(); row++)
        {
            QPointer<task_progress_bar> m_item = new task_progress_bar(this);
            m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(row,0));

            if(m_item == NULL)
                continue;

            type_and_title = m_item->get_job_type_and_title();

            if(type_and_title.title ==  plugin_name)
            {
                m_item->change_display_messge_in_progress_display(message,bool_show_close_button, max_size_of_progress);

                if(!bool_status_button_checked)
                {
                    emit signal_current_item_for_statusbar(m_item->pub_get_current_feature_name_and_status());
                }

                break;
            }
        }
    }

    semaphore_sub_process_status_window->release(1);
}

void task_status_window::set_maximum_value_of_task_progress_bar_in_fs(QString source_count_name, int max_value)
{
    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        if(semaphore_sub_process_status_window->tryAcquire(1))
            break;

        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    for(int i = 0 ; i < status_task_progress_bar_list.size(); i++)
    {
        task_progress_bar *item = status_task_progress_bar_list.at(i).task_progress_bar_obj;

        if(item->get_job_type_and_title().type == QString(MACRO_JobType_Adding_Source_To_Case) && item->get_job_type_and_title().source_count_name == source_count_name)
        {
            for(int jj = 0; jj < ui->tableWidget_status_window->rowCount(); jj++)
            {
                task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(jj,0));
                if(m_item == item)
                {

                    m_item->set_progress_maximum_bar_value(max_value);

                    if(!bool_status_button_checked)
                    {
                        emit signal_current_item_for_statusbar(m_item->pub_get_current_feature_name_and_status());
                    }


                    semaphore_sub_process_status_window->release(1);
                    return;

                }
            }
        }
    }

    semaphore_sub_process_status_window->release(1);
}

void task_status_window::pub_task_progressbar_set_value(int total_size_processed, QString source_count_name)
{
    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        if(semaphore_sub_process_status_window->tryAcquire(1))
            break;

        QCoreApplication::processEvents();
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    for(int i = 0 ; i < status_task_progress_bar_list.size(); i++)
    {
        task_progress_bar *item = status_task_progress_bar_list.at(i).task_progress_bar_obj;

        if(item->get_job_type_and_title().type == QString(MACRO_JobType_Adding_Source_To_Case) && item->get_job_type_and_title().source_count_name == source_count_name)
        {
            for(int jj = 0; jj < ui->tableWidget_status_window->rowCount(); jj++)
            {
                task_progress_bar *m_item = dynamic_cast<task_progress_bar *>(ui->tableWidget_status_window->cellWidget(jj,0));
                if(m_item == item)
                {
                    m_item->pub_progressbar_set_value(total_size_processed);

                    if(!bool_status_button_checked)
                    {
                        emit signal_current_item_for_statusbar(m_item->pub_get_current_feature_name_and_status());
                    }

                    semaphore_sub_process_status_window->release(1);
                    return;
                }
            }
        }
    }

    semaphore_sub_process_status_window->release(1);
}

