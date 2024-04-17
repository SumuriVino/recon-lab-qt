#include "detached_file_previewer_module.h"
#include "ui_detached_file_previewer_module.h"


detached_file_previewer_module::detached_file_previewer_module(const QString &title, QWidget *parent) :
    QMainWindow(parent), w_title(title),
    ui(new Ui::detached_file_previewer_module)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window|Qt::WindowTitleHint|Qt::WindowSystemMenuHint|Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);


    image_previe_stack_widget = new image_preview(this);
    image_previe_stack_widget->setMaximumHeight(9999);
    image_previe_stack_widget->setMaximumWidth(9999);

    blank_stack_widget = new blank(this);
    av_preview_stack_widget = new av_preview(this);

    ui->stackedWidget->addWidget(blank_stack_widget);
    ui->stackedWidget->addWidget(image_previe_stack_widget);
    ui->stackedWidget->addWidget(av_preview_stack_widget);


    ui->stackedWidget->setCurrentIndex(2);

}

detached_file_previewer_module::~detached_file_previewer_module()
{
    delete ui;
}

void detached_file_previewer_module::set_current_index(int index)
{
    QString set_title = module_name + " - Detached Previewer";
    setWindowTitle(QObject::tr(set_title.toLocal8Bit().data()));
    ui->stackedWidget->setCurrentIndex(index);
}

void detached_file_previewer_module::set_image(QString preview_path, QString real_file_path)
{

    image_previe_stack_widget->set_image_for_detach_window(preview_path);
    QFileInfo info(real_file_path);

    QString set_title;
    if(info.fileName().trimmed() != "")
    {
        set_title = module_name + " - Detached Previewer" + " [ " + info.fileName() + " ]";
    }
    else
        set_title = module_name + " - Detached Previewer";

    setWindowTitle(QObject::tr(set_title.toLocal8Bit().data()));

}


void detached_file_previewer_module::load_source(QString preview_path, QString real_file_path)
{
    av_preview_stack_widget->load_source(preview_path);
    QFileInfo info(real_file_path);

    QString set_title;
    if(info.fileName().trimmed() != "")
    {
        set_title = module_name + " - Detached Previewer" + " [ " + info.fileName() + " ]";
    }
    else
        set_title = module_name + " - Detached Previewer";

    setWindowTitle(QObject::tr(set_title.toLocal8Bit().data()));


}

void detached_file_previewer_module::stop_media()
{
    av_preview_stack_widget->stop_media();
}


void detached_file_previewer_module::closeEvent(QCloseEvent *)
{
    av_preview_stack_widget->stop_media();

    ui->stackedWidget->setCurrentIndex(2);

    QString set_title = module_name + " - Detached Previewer";
    setWindowTitle(QObject::tr(set_title.toLocal8Bit().data()));

}

void detached_file_previewer_module::set_window_title(QString md_name)
{
    QString set_title = md_name + " - Detach Previewer";
    setWindowTitle(QObject::tr(set_title.toLocal8Bit().data()));

    module_name = md_name;

}
