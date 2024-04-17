#ifndef CROPPED_IMAGE_DISPLAY_H
#define CROPPED_IMAGE_DISPLAY_H

#include <QWidget>
#include <QtCore>
#include <QLabel>
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"

#include "witness_info/witness_info_manager/witness_info_manager.h"

#include "display_loading_progress_bar/display_loading_progress_bar.h"

#include <QMessageBox>

namespace Ui {
class cropped_image_display;
}

class cropped_image_display : public QWidget
{
    Q_OBJECT

public:
    explicit cropped_image_display(QWidget *parent = nullptr);
    ~cropped_image_display();

    void prepare_image_display(QString path);

    void pub_clear_fields();

    void add_dummy_face_img();

signals:
void signal_start_search_clicked(QString val, QString label_name);

private slots:
    void on_pushButton_prev_clicked();

    void on_pushButton_next_clicked();

    void on_pushButton_start_clicked();

private:
    Ui::cropped_image_display *ui;

    QMap<int,QString> map_found_face_info;
    void set_linedit_search_label_placeholder_text();
};

#endif // CROPPED_IMAGE_DISPLAY_H
