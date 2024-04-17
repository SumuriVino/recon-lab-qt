#include "task_progress_bar.h"
#include "ui_task_progress_bar.h"

task_progress_bar::task_progress_bar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::task_progress_bar)
{
    ui->setupUi(this);

    bool_request_for_cancellation = false;

    ui->label_skip->hide();
}

task_progress_bar::~task_progress_bar()
{
    deleteLater();

    //  delete ui;
}

void task_progress_bar::set_title(QString title)
{
    ui->label_feature_name->setText(title);
}

void task_progress_bar::set_job_type_and_title(struct_global_task_type_and_title t1, QString icon_path)
{
    //    type_and_title = t1;
    //    ui->label_feature_name->setText(t1.title + QString(" - Queued"));
    //    ui->label_feature_category->setText(t1.type);

    //    QPixmap pixmap(icon_path); // load pixmap
    //    int w = ui->label_icon->width() - 5;
    //    int h = ui->label_icon->height() - 5;
    //    ui->label_icon->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));


    type_and_title = t1;

    if(t1.type == MACRO_JobType_File_Search || t1.type == MACRO_JobType_Text_Indexing || t1.type == MACRO_JobType_Content_Search)
    {
        ui->label_feature_name->setText(t1.title);
    }
    else if(t1.type == MACRO_JobType_Verifying)
    {
        ui->label_feature_name->setText("");
    }
    else if(t1.type == MACRO_JobType_Artifact_Loading)
    {
        ui->label_skip->hide();
        ui->pushButton_close->hide();
        set_infinite_progress_bar();
    }
    else
    {
        ui->label_feature_name->setText(t1.title + QString(" - Queued"));
    }

    if(t1.type == MACRO_JobType_Verifying)
    {
        QString label_with_source_name =  t1.type + QString(" ")  + t1.title;
        ui->label_feature_category->setText(label_with_source_name);
    }
    else if(t1.type == MACRO_JobType_Mime_Type)
    {
        ui->label_feature_category->setText("Analyzing File Types...");
    }
    else
        ui->label_feature_category->setText(t1.type);


    QPixmap pixmap(icon_path); // load pixmap

    int w = ui->label_icon->width() - 5;

    int h = ui->label_icon->height() - 5;

    QSize msize(w, h);

    QImage img(icon_path);

    ui->label_icon->setPixmap(QPixmap::fromImage(img).scaled(msize,Qt::KeepAspectRatio, Qt::SmoothTransformation));


}

struct_global_task_type_and_title task_progress_bar::get_job_type_and_title()
{
    return type_and_title;
}

void task_progress_bar::pub_progressbar_set_value(int value)
{
    if(value < 0)
        return;

    ui->progressBar->setValue(value);
}

void task_progress_bar::set_next_task_with_same_parent_in_status_bar_for_fs(struct_global_task_type_and_title t1)
{
    type_and_title = t1;
    ui->label_feature_name->setText(t1.title + QString(" - Queued"));
    ui->label_feature_category->setText(t1.type);
    ui->pushButton_close->show();
}

void task_progress_bar::pub_progressbar_set_value_for_plugins(int value)
{
    ui->progressBar->setValue(value);
}

void task_progress_bar::set_infinite_progress_bar()
{
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);

    ui->progressBar->setTextVisible(false);
}


void task_progress_bar::pub_set_infinite_progress_bar()
{
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);

    ui->progressBar->setTextVisible(false);
}

void task_progress_bar::pub_show_skip_label(bool status)
{
    if(status)
        ui->label_skip->show();
    else
        ui->label_skip->hide();
}

void task_progress_bar::pub_set_progress_message(QString msg)
{
    ui->label_feature_name->setText(msg);
}

void task_progress_bar::pub_set_icon_for_plugin(QString plugin_name)
{
    QString icon_path = "../icons/plugins/" + plugin_name.replace(" ", "_") + ".png";

    int w = ui->label_icon->width() - 5;

    int h = ui->label_icon->height() - 5;

    QSize msize(w, h);

    QImage img(icon_path);

    ui->label_icon->setPixmap(QPixmap::fromImage(img).scaled(msize,Qt::KeepAspectRatio, Qt::SmoothTransformation));

}

void task_progress_bar::pub_hide_cancel_button()
{
    ui->pushButton_close->hide();
}
void task_progress_bar::on_pushButton_close_clicked()
{
    bool_request_for_cancellation = true;
    ui->pushButton_close->setEnabled(false);
    emit signal_task_cancelled_requested(type_and_title);
}

void task_progress_bar::common_set_extracted_records_in_progress(qint64 records, struct_global_task_type_and_title type_and_title_obj, bool bool_show_close_button, int progress, bool bool_infinite_process)
{
    if(bool_request_for_cancellation || (ui == NULL))
    {
        return;
    }

    if(records == MACRO_Generic_Progress_No_Records_int)
    {
        ///show progress bar without records
        ui->label_feature_name->setText(type_and_title_obj.title);
    }
    else
    {
        ///show progress bar with records
        ui->label_feature_name->setText(type_and_title_obj.title + " (" + QString::number(records) + ")");
    }

    if(type_and_title_obj.type == MACRO_JobType_Verifying)
    {
        QString lble_with_source_name = get_job_type_and_title().type + QString(" ") + get_job_type_and_title().title;
        ui->label_feature_category->setText(lble_with_source_name);
    }
    else if(type_and_title_obj.type == MACRO_JobType_Mime_Type)
    {
        ui->label_feature_category->setText("Analyzing File Types...");
    }
    else
        ui->label_feature_category->setText(type_and_title_obj.type);

    if(bool_infinite_process)
        set_infinite_progress_bar();
    else
        ui->progressBar->setValue(progress);


    if(bool_show_close_button)
        ui->pushButton_close->show();
    else
        ui->pushButton_close->hide();
}

void task_progress_bar::common_set_extracted_estimated_time(QString estimated_time)
{
    ui->label_estimated_time->setText("Estimated Time - " + estimated_time);
}

void task_progress_bar::change_display_messge_in_progress_display(QString msg, bool bool_show_close_button, int max_size_of_progress)
{
    if((ui == NULL))
    {
        return;
    }


    if(bool_show_close_button)
        ui->pushButton_close->show();
    else
        ui->pushButton_close->hide();

    ui->label_feature_name->setText(msg);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(max_size_of_progress);
    ui->progressBar->reset();
}

void task_progress_bar::set_progress_maximum_bar_value(qint64 max_val)
{
    ui->progressBar->reset();
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(max_val);
}

QString task_progress_bar::pub_get_current_feature_name_and_status()
{
    return ui->label_feature_name->text();
}

void task_progress_bar::on_label_skip_linkActivated(const QString &link)
{
    emit signal_skip_triggered();
}
