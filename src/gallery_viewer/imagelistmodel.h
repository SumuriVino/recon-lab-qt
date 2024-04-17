#ifndef IMAGELISTMODEL_H
#define IMAGELISTMODEL_H

#include <QAbstractTableModel>
#include <QFileInfoList>
#include <QList>
#include <QDebug>
#include <QDir>
#include <QtCore/QDirIterator>
#include"header_global_structures.h"

class ImageListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ImageListModel(QObject* parent = Q_NULLPTR);
    void pub_set_imagelist(QList<struct_global_gallery_view_file_info> *list);

public:
    bool loadDirectoryImageList(const QString& fullPath);
public:
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
signals:
    void signal_bookmark_value_changed(struct_global_gallery_view_file_info);

private slots:
    void slot_imageList_model_bookmark_value_changed(bool bookmark_status, qint64 index_file_bookmark);
private:

    QList<struct_global_gallery_view_file_info> *file_list = NULL;

};

#endif // IMAGELISTMODEL_H
