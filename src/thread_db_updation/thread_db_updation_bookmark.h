#ifndef THREAD_DB_UPDATION_BOOKAMARK_H
#define THREAD_DB_UPDATION_BOOKAMARK_H

#include <QObject>
#include <QtCore>
#include "header_global_structures.h"
#include "header_global_variables_1.h"
#include "header_global_variables_3.h"
#include "header_global_variables_2.h"

class thread_db_updation_bookmark : public QObject
{
    Q_OBJECT

public:
    explicit thread_db_updation_bookmark(QObject *parent = nullptr);


    void set_record_value_list(QList<struct_global_tagging_tag_notes_db_updation> list_obj);

    void pub_set_updation_status(bool status);

signals:
    void finished();

public slots:
    void slot_bookmark_updation_start();

private:
    QList<struct_global_tagging_tag_notes_db_updation> list_st_global_bookmark_db_updation;

     bool bool_updation_status;
};

#endif //
