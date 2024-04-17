#include "detached_file_previewer.h"
#include "ui_detached_file_previewer.h"


detached_file_previewer::detached_file_previewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::detached_file_previewer)
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

detached_file_previewer::~detached_file_previewer()
{
    delete ui;
}

void detached_file_previewer::set_current_index(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void detached_file_previewer::set_image(QString path)
{
        image_previe_stack_widget->set_image_for_detach_window(path);
}



void detached_file_previewer::load_source(QString path)
{
    av_preview_stack_widget->load_source(path);
//    av_preview_stack_widget->play_media();

    QFileInfo info(path);
   // setWindowTitle("Safari - Detached Previewer [ " + info.fileName() + " ]");

}


void detached_file_previewer::closeEvent(QCloseEvent *)
{
    av_preview_stack_widget->stop_media();

    ui->stackedWidget->setCurrentIndex(2);

}

void detached_file_previewer::stop_media()
{
    av_preview_stack_widget->stop_media();
}

void detached_file_previewer::set_window_title(QString title)
{
    setWindowTitle(title);
}
