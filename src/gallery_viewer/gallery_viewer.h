#ifndef GALLERY_VIEWER_H
#define GALLERY_VIEWER_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLineEdit>
#include <QtCore>
#include <QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFileDialog>
#include "imagelistview.h"
#include "imagelistmodel.h"
#include "header_global_structures.h"
#include "recon_generic_functions/recon_static_functions.h"

#include "recon_generic_functions/recon_helper_standard.h"

#define MACRO_LOCAL_GALLERY_VIEWER_MIN_THUMBNAIL_SIZE       150
#define MACRO_LOCAL_GALLERY_VIEWER_MAX_THUMBNAIL_SIZE       400
#define MACRO_LOCAL_GALLERY_VIEWER_LABEL_THUMBNAIL_SIZE    "Thumbnail size:"

class gallery_viewer : public QWidget
{
    Q_OBJECT
public:
    explicit gallery_viewer(QWidget *parent = nullptr);

    void pub_set_file_list(QList<struct_global_gallery_view_file_info> *llist);
    void pub_set_file_type_parent_type(QString type);
    void pub_create_ui();

private slots:
    void scroll_value_changed(int value);
    void slot_item_clicked(qint64 index);

    void slot_bookmark_value_changed(struct_global_gallery_view_file_info file_info);
    void slot_spacebar_pressed(qint64 index);
    void slot_control_e_key_pressed(qint64 index);
    void slot_set_bookmark_status_to_gallery_view(bool checked);

    void slot_list_view_item_clicked(QModelIndex index);
    void slot_item_double_clicked(qint64 index);

signals:

    void signal_gallery_viewer_item_clicked(struct_global_gallery_view_file_info);
    void signal_gallery_bookmark_value_changed(struct_global_gallery_view_file_info);
    void signal_gallery_view_spacebar_pressed(struct_global_gallery_view_file_info);
    void signal_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info);
    void signal_gallery_view_invalid_item_clicked();
    void signal_imageList_model_bookmark_value_changed(bool , qint64) ;
    void signal_gallery_viewer_item_double_clicked(struct_global_gallery_view_file_info);

private:

    QSlider *m_slider;
    ImageListView *m_list;
    ImageListModel *m_model;

    QList<struct_global_gallery_view_file_info> *file_QList;
    qint64 index_file_bookmark = -1;

    QList<struct_global_gallery_view_file_info> face_tmp_QList;
    QList<struct_global_gallery_view_file_info> *face_filtered_QList;
    QListView *list_view_faces;
    void prepare_list_view_display();

    QString file_type_parent;
    QStringList distinct_face_file_path_list;
    recon_helper_standard *recon_helper_standard_obj;

    QPixmap get_pixmap(QString crop_image_path);

};

#endif // GALLERY_VIEWER_H
