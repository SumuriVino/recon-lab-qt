#ifndef UNIFIED_LOGS_PROXY_MODEL_H
#define UNIFIED_LOGS_PROXY_MODEL_H
#include <QtCore>
#include <QSortFilterProxyModel>
#include "header_global_enum.h"

class unified_logs_proxy_model : public QSortFilterProxyModel
{

public:
    unified_logs_proxy_model();

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    QString search_keyword;
    QStringList search_col_name_list;

};

#endif // UNIFIED_LOGS_PROXY_MODEL_H
