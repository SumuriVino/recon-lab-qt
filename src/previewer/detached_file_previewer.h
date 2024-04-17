#ifndef DETACHED_FILE_PREVIEWER_H
#define DETACHED_FILE_PREVIEWER_H

#include <QMainWindow>
#include "previewer/image_preview.h"
#include "previewer/av_preview.h"
#include "previewer/blank.h"
#include <QtGui>
#include <QtCore>

namespace Ui {
class detached_file_previewer;
}

class detached_file_previewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit detached_file_previewer(QWidget *parent = 0);
    ~detached_file_previewer();
    void set_current_index(int);
    void set_image(QString path);
    void load_source(QString path);

    void closeEvent(QCloseEvent *);

    void stop_media();

    void set_window_title(QString);

private:
    Ui::detached_file_previewer *ui;

    image_preview *image_previe_stack_widget;
    blank *blank_stack_widget;
    av_preview * av_preview_stack_widget;

};

#endif // DETACHED_FILE_PREVIEWER_H
