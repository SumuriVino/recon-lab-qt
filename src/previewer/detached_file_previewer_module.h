#ifndef DETACHED_FILE_PREVIEWER_module_H
#define DETACHED_FILE_PREVIEWER_module_H

#include <QMainWindow>
#include "previewer/image_preview.h"
#include "previewer/av_preview.h"
#include "previewer/blank.h"

#include <QtGui>
#include <QtCore>

namespace Ui {
class detached_file_previewer_module;
}

class detached_file_previewer_module : public QMainWindow
{
    Q_OBJECT

public:
    explicit detached_file_previewer_module(const QString &title, QWidget *parent = 0);
    ~detached_file_previewer_module();
    void set_current_index(int);
    void set_image(QString preview_path, QString real_file_path);
    void load_source(QString preview_path, QString real_file_path);
    void stop_media();
    void closeEvent(QCloseEvent *);


    void set_window_title(QString);


private:
    Ui::detached_file_previewer_module *ui;

    image_preview *image_previe_stack_widget;
    blank *blank_stack_widget;
    av_preview* av_preview_stack_widget;
    QString w_title;

    QString module_name;
};

#endif // DETACHED_FILE_PREVIEWER_H
