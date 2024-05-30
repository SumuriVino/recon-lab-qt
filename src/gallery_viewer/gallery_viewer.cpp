#include "gallery_viewer.h"

gallery_viewer::gallery_viewer(QWidget *parent)
    : QWidget(parent)
{

}

void gallery_viewer::pub_create_ui()
{ // create ui of gallery view
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setMinimum(MACRO_LOCAL_GALLERY_VIEWER_MIN_THUMBNAIL_SIZE);
    m_slider->setMaximum(MACRO_LOCAL_GALLERY_VIEWER_MAX_THUMBNAIL_SIZE);
    m_slider->setValue(MACRO_LOCAL_GALLERY_VIEWER_MIN_THUMBNAIL_SIZE);
    m_slider->setMaximumWidth(300);
    connect(m_slider, &QSlider::valueChanged, this, &gallery_viewer::scroll_value_changed);

    m_model = new ImageListModel(this);
    connect(m_model, SIGNAL(signal_bookmark_value_changed(struct_global_gallery_view_file_info)), this, SLOT(slot_bookmark_value_changed(struct_global_gallery_view_file_info)));
    connect(this, SIGNAL(signal_imageList_model_bookmark_value_changed(bool ,qint64 )), m_model, SLOT(slot_imageList_model_bookmark_value_changed(bool , qint64)));

    m_list = new ImageListView(this);
    m_list->setModel(m_model);
    connect(m_list, SIGNAL(signal_item_clicked(qint64)), this, SLOT(slot_item_clicked(qint64)));
    connect(m_list, SIGNAL(signal_spacebar_pressed(qint64)), this, SLOT(slot_spacebar_pressed(qint64)));
    connect(m_list, SIGNAL(signal_control_e_key_pressed(qint64)), this, SLOT(slot_control_e_key_pressed(qint64)));
    connect(m_list, SIGNAL(signal_list_view_invalid_item_clicked()), this, SIGNAL(signal_gallery_view_invalid_item_clicked()));
    connect(m_list, SIGNAL(signal_item_double_clicked(qint64)), this, SLOT(slot_item_double_clicked(qint64)));

    auto *mainlt = new QVBoxLayout(this);
    auto *sliderlt = new QHBoxLayout;

    sliderlt->addStretch();
    sliderlt->addWidget(new QLabel(MACRO_LOCAL_GALLERY_VIEWER_LABEL_THUMBNAIL_SIZE, this));
    sliderlt->addWidget(m_slider);
    mainlt->addLayout(sliderlt);

    if(file_type_parent == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
    {
        QHBoxLayout *vvbox = new QHBoxLayout;
        list_view_faces = new QListView(this);
        connect(list_view_faces, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_list_view_item_clicked(QModelIndex)));
        list_view_faces->setStyleSheet("QListView::item:selected { background-color: lightgray;}");
        list_view_faces->setFixedHeight(150);

        list_view_faces->setEditTriggers(QAbstractItemView::NoEditTriggers);

        vvbox->addWidget(list_view_faces);

        mainlt->addLayout(vvbox);

        QLabel *label = new QLabel;
        label->setStyleSheet("QLabel { background-color : #cdcdcd}");

        mainlt->addWidget(label);


    }

    mainlt->addWidget(m_list);

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void gallery_viewer::pub_set_file_type_parent_type(QString type)
{
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    file_type_parent = type;

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void gallery_viewer::prepare_list_view_display()
{ // prepare list view to display
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);
    QString rslt_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString cropped_face_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Faces_Group_In_Result_QString).toString() + "face_info.sqlite";

    QString select_cmd = "SELECT file_path from distinct_faces";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);

    QSqlDatabase db_distinct_faces = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    db_distinct_faces.setDatabaseName(cropped_face_db_path);

    if(!db_distinct_faces.open())
        return;

    distinct_face_file_path_list.clear();

    distinct_face_file_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_cmd, 0, db_distinct_faces, Q_FUNC_INFO);

    db_distinct_faces.close();

    QStandardItemModel *model = new QStandardItemModel;

    if(distinct_face_file_path_list.isEmpty())
    {
        list_view_faces->reset();
        list_view_faces->setModel(model);
        list_view_faces->setFlow(QListView::Flow::LeftToRight);
        list_view_faces->setContextMenuPolicy(Qt::CustomContextMenu);
        return;
    }


    QStandardItem *item = new QStandardItem;
    item->setData(get_pixmap("../icons/all_items.png"), Qt::DecorationRole);

    model->appendRow(item);

    for(int ii = 0; ii < distinct_face_file_path_list.size(); ii++)
    {
        QString distinct_face_final_path = rslt_dir_path + distinct_face_file_path_list.at(ii);

        QStandardItem *item = new QStandardItem;
        item->setData(get_pixmap(distinct_face_final_path), Qt::DecorationRole);

        model->appendRow(item);
    }

    list_view_faces->reset();
    list_view_faces->setModel(model);
    list_view_faces->setFlow(QListView::Flow::LeftToRight);
    list_view_faces->setContextMenuPolicy(Qt::CustomContextMenu);

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);

}

void gallery_viewer::slot_list_view_item_clicked(QModelIndex index)
{ // prepare list view item clicked to get the metadata info of the record
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    int row = index.row();

    if(row < 0)
        return;

    if(row == 0)
    {

        face_tmp_QList.clear();

        face_tmp_QList = *file_QList;

        m_model->pub_set_imagelist(file_QList);

        m_list->reset();

        return;
    }


    QString cropped_face_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Faces_Group_In_Result_QString).toString() + "face_info.sqlite";

    QString source_file_path = distinct_face_file_path_list.at(row - 1);

    QString cmd = "SELECT face_id from distinct_faces where file_path LIKE ?";

    QString face_id = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd, QStringList() << source_file_path, 0, cropped_face_db_path, Q_FUNC_INFO);

    QString select_cmd = "SELECT fs_record, source_count_name from all_faces where face_id = ?";

    QStringList rec_number_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, QStringList() << face_id, 0, cropped_face_db_path, Q_FUNC_INFO);

    QStringList sou_cnt_name_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, QStringList() << face_id, 1, cropped_face_db_path, Q_FUNC_INFO);

    face_tmp_QList.clear();

    face_tmp_QList = *file_QList;

    for(int ii = face_filtered_QList->size() - 1; ii >= 0; ii--)
    {
        struct_global_gallery_view_file_info obj = face_filtered_QList->at(ii);

        bool bool_is_need_to_continue = false;

        for(int jj = 0; jj < rec_number_list.size(); jj++)
        {
            if(rec_number_list.at(jj) == obj.INT && sou_cnt_name_list.at(jj) == obj.source_count_name)
            {
                bool_is_need_to_continue = true;
                break;
            }
        }

        if(bool_is_need_to_continue)
            continue;

        face_filtered_QList->removeAt(ii);

    }

    m_model->pub_set_imagelist(face_filtered_QList);

    m_list->reset();

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

QPixmap gallery_viewer::get_pixmap(QString crop_image_path)
{ // get pix map of the record to make it visible on gallery viewer
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    QPixmap target = QPixmap(140,140);
    target.fill(Qt::transparent);

    QPixmap pixmap;
    pixmap.load(crop_image_path);
    pixmap.scaled(140, 140, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPainter painter (&target);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPainterPath path = QPainterPath();
    path.addRoundedRect(0, 0, 140, 140, 100, 100);

    painter.setClipPath(path);
    painter.drawPixmap(0, 0, pixmap);

    return target;

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);

}

void gallery_viewer::pub_set_file_list(QList<struct_global_gallery_view_file_info> *llist)
{ // set file list on gallery view
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    file_QList = llist;


    if(file_type_parent == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
    {
        if(index_file_bookmark >= 0 && face_filtered_QList->size() > 0)
        {
            if(face_filtered_QList->size() > index_file_bookmark)
                face_filtered_QList->removeAt(index_file_bookmark);

            m_model->pub_set_imagelist(face_filtered_QList);
            m_list->reset();
        }
        else
        {

            face_tmp_QList = *file_QList;

            face_filtered_QList = &face_tmp_QList;

            m_model->pub_set_imagelist(file_QList);

            m_list->reset();

        }

        prepare_list_view_display();
    }
    else
    {
        m_model->pub_set_imagelist(file_QList);
        m_list->reset();
    }

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void gallery_viewer::scroll_value_changed(int value)
{ // update thumbnails on scrollview
    m_list->setThumbnailSize(value);
}

void gallery_viewer::slot_item_clicked(qint64 index)
{ // any item clicked randomly on click
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    index_file_bookmark = index;

    if(file_type_parent == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
        emit signal_gallery_viewer_item_clicked(face_filtered_QList->at(index));
    else
        emit signal_gallery_viewer_item_clicked(file_QList->at(index));

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void gallery_viewer::slot_item_double_clicked(qint64 index)
{ // double clicked on any item which can open the record using system other application
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    index_file_bookmark = index;

    if(file_type_parent == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
        emit signal_gallery_viewer_item_double_clicked(face_filtered_QList->at(index));
    else
        emit signal_gallery_viewer_item_double_clicked(file_QList->at(index));

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void gallery_viewer::slot_bookmark_value_changed(struct_global_gallery_view_file_info file_info)
{ // update bookmark value
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    if(file_type_parent == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
    {
        for(int ii = 0; ii < file_QList->size(); ii++)
        {
            struct_global_gallery_view_file_info obj = file_QList->at(ii);
            if(obj.INT == file_info.INT && obj.source_count_name == file_info.source_count_name)
            {
                obj.bookmark = file_info.bookmark;

                file_QList->removeAt(ii);
                file_QList->insert(ii, obj);
            }
        }
    }

    emit signal_gallery_bookmark_value_changed(file_info);

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);

}

void gallery_viewer::slot_spacebar_pressed(qint64 index)
{ // quicklook using space bar button
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    emit signal_gallery_view_spacebar_pressed(file_QList->at(index));

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void gallery_viewer::slot_control_e_key_pressed(qint64 index)
{ // export the file usng ctrl + e button clicked
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    emit signal_gallery_view_control_e_key_pressed(file_QList->at(index));

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

void gallery_viewer::slot_set_bookmark_status_to_gallery_view(bool checked)
{ // set bookmark
    recon_static_functions::app_debug("Starts ",Q_FUNC_INFO);

    emit  signal_imageList_model_bookmark_value_changed(checked , index_file_bookmark);

    recon_static_functions::app_debug("Ends ",Q_FUNC_INFO);
}

