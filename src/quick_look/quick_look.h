#ifndef QUICK_LOOK_H
#define QUICK_LOOK_H

#include <QtCore>
#include <QListView>
#include <QListWidget>
#include <QObject>

#include "header_global_variables_1.h"
#include "header_global_variables_2.h"


class quick_look : public QListView
{
    Q_OBJECT
public:
    explicit quick_look(QWidget *parent = 0);
    ~quick_look();
    void pub_set_preview_filepath(QString path);

    void pub_close_view();

    bool pub_get_preview_closed_status();

    void pub_set_preview_closed_status(bool status);

private:


    QStringListModel *list_model;

    void setPreviewIndex(const QModelIndex &index);
    void showPreview();
    void hidePreview();




};



#endif
