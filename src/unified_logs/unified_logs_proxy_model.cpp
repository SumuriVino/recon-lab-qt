#include "unified_logs_proxy_model.h"

unified_logs_proxy_model::unified_logs_proxy_model()
{

}

bool unified_logs_proxy_model::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    //Search
    QModelIndex timestamp_index = sourceModel()->index(source_row, enum_unified_logs_timestamp, source_parent);
    QModelIndex process_index = sourceModel()->index(source_row, enum_unified_logs_process, source_parent);
    QModelIndex event_type_index = sourceModel()->index(source_row, enum_unified_logs_event_type, source_parent);
    QModelIndex event_message_index = sourceModel()->index(source_row, enum_unified_logs_event_message, source_parent);

    bool bool_timestamp_index =  sourceModel()->data(timestamp_index).toString().contains(search_keyword, Qt::CaseInsensitive);
    bool bool_process_index =  sourceModel()->data(process_index).toString().contains(search_keyword, Qt::CaseInsensitive);
    bool bool_event_type_index =  sourceModel()->data(event_type_index).toString().contains(search_keyword, Qt::CaseInsensitive);
    bool bool_event_message_index =  sourceModel()->data(event_message_index).toString().contains(search_keyword, Qt::CaseInsensitive);

    return (bool_timestamp_index | bool_process_index | bool_event_type_index | bool_event_message_index);

}
