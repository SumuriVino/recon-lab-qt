#ifndef UNIFIED_LOGS_MODEL_H
#define UNIFIED_LOGS_MODEL_H

#include <QAbstractTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtCore>
#include "recon_generic_functions/recon_helper_standard.h"
#include "m_tableview.h"
#include "header_global_enum.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"

class unified_logs_model : public QAbstractTableModel
{
    Q_OBJECT

public:

    unified_logs_model();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    void pub_set_table_view_obj(m_tableview *obj);
    void pub_set_source_count_name(QString src_count_name);
    void pub_get_log_data_list_from_unified_logs_db();
    void pub_update_tag_record(qint64 row, QString tag);

private:

    QStringList int_list;
    QStringList timestamp_list;
    QStringList process_list;
    QStringList event_type_list;
    QStringList event_message_list;
    QStringList tags_list;
    qint64 _row_count;

    QString source_count_name;
    QStringList column_list;

    recon_helper_standard *recon_helper_standard_obj;
    m_tableview *m_tableview_obj;
};

#endif // UNIFIED_LOGS_MODEL_H
