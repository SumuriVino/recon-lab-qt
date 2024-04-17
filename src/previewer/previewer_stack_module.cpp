#include "previewer_stack_module.h"
#include "ui_previewer_stack_module.h"


previewer_stack_module::previewer_stack_module(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::previewer_stack_module)
{
    ui->setupUi(this);

    img_previewer = new image_preview(this);
    addWidget(img_previewer);

    audio_video_player = new av_preview(this);
    addWidget(audio_video_player);

    blank_previewer = new blank(this);
    addWidget(blank_previewer);

}

previewer_stack_module::~previewer_stack_module()
{
    delete img_previewer;
    delete audio_video_player;
    delete ui;
}

void previewer_stack_module::set_image(QString file_path)
{
    QFileInfo info(file_path);
    if(!info.exists())
    {
        setCurrentIndex(4);
        return;
    }

    img_previewer->set_image_by_path(file_path);
}

void previewer_stack_module::load_media_file(QString file_path)
{
    QFileInfo info(file_path);
    if(!info.exists())
    {
        setCurrentIndex(4);
        return;
    }

    audio_video_player->load_source(file_path);

}
