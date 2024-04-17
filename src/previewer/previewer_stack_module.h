#ifndef PREVIEWER_STACK_module_H
#define PREVIEWER_STACK_module_H

#include <QStackedWidget>
#include "image_preview.h"
#include <QtCore>
#include "av_preview.h"
#include "blank.h"

namespace Ui {
class previewer_stack_module;
}

class previewer_stack_module : public QStackedWidget
{
    Q_OBJECT
    
public:
    explicit previewer_stack_module(QWidget *parent = 0);
    ~previewer_stack_module();
    image_preview *img_previewer;
    av_preview *audio_video_player;
    blank *blank_previewer;
    void set_image(QString file_path);
    void load_media_file(QString file_path);
    
private:
    Ui::previewer_stack_module *ui;

};

#endif // PREVIEWER_STACK_H
