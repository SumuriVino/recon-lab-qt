#ifndef RECOGNIZE_FACE_H
#define RECOGNIZE_FACE_H

#include <QWidget>
#include <QtCore>
#include <QFileDialog>
#include "cropped_image_display.h"
#include <QMessageBox>
#include "face_extractor/facefinder.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "task_status/task_status_window.h"

namespace Ui {
class recognize_face;
}

class recognize_face : public QWidget
{
    Q_OBJECT

public:
    explicit recognize_face(QWidget *parent = nullptr);
    ~recognize_face();

    void pub_clear_fields();

    void pub_set_task_status_window(task_status_window *obj);
signals:
    void signal_add_search_result_in_case_tree(QString);

private slots:
    void on_pushButton_import_clicked();

    void on_pushButton_crop_faces_clicked();

    void slot_start_search_clicked(QString s_path, QString lbl_name);
private:
    Ui::recognize_face *ui;

    display_loading_progress_bar *display_loading_progress_bar_obj;
    cropped_image_display *cropped_image_display_obj;

    facefinder *face_finder_obj;
    recon_helper_standard *recon_helper_standard_obj;

    task_status_window *task_status_window_obj;

    bool bool_status_face_found;
};

#endif // RECOGNIZE_FACE_H
