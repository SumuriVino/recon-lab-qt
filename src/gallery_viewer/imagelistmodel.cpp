#include "imagelistmodel.h"

ImageListModel::ImageListModel(QObject* parent)
    : QAbstractTableModel(parent)
{

}

void ImageListModel::pub_set_imagelist(QList<struct_global_gallery_view_file_info> *list)
{
    file_list = list;
}

int ImageListModel::rowCount(const QModelIndex& parent) const
{
    if(file_list == NULL)
        return 0;
    else
        return parent.isValid() ? 0 : file_list->size();
}

int ImageListModel::columnCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : 1;
}

QVariant ImageListModel::data(const QModelIndex& index, int role) const
{ // set dislay role checkable or uncheckable

    if (index.isValid())
    {
        if (role == Qt::DisplayRole)
        {
            return file_list->at(index.row()).thumbnail_file_path;
        }
        else if (role == Qt::CheckStateRole)
        {

            if(file_list->at(index.row()).bookmark == "1")
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }
    }
    return QVariant();
}

bool ImageListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{ // create checkable boookmark for adding/removing bookmark
    if (index.isValid())
    {
        if (role == Qt::CheckStateRole)
        {
            struct_global_gallery_view_file_info file_info = file_list->at(index.row());

            if (file_info.bookmark == "1")
                file_info.bookmark = "0";
            else
                file_info.bookmark = "1";

            file_list->replace(index.row(), file_info);
            emit signal_bookmark_value_changed(file_info);
            return true;
        }
    }
    return false;
}

void ImageListModel::slot_imageList_model_bookmark_value_changed(bool bookmark_status , qint64 index_file_bookmark)
{ // bookmark value changed
    QModelIndex model_index = index(index_file_bookmark , 0);

    QVariant state = data(model_index,Qt::CheckStateRole);

    bool check_status = false;
    if(state.toInt() == 0)
    {
        check_status = false;
    }
    else if(state.toInt() == 2)
    {
        check_status = true;
    }

    if(bookmark_status && !check_status)
    {
        setData(model_index,Qt::Checked, Qt::CheckStateRole);
    }
    else if(!bookmark_status && check_status)
    {
        setData(model_index,Qt::Unchecked, Qt::CheckStateRole);
    }
}



