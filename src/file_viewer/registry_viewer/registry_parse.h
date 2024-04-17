#ifndef REGISTRY_H
#define REGISTRY_H

#include <QObject>
#include <QtCore>
#include "hivex.h"

class registry_parse
{
public:
    registry_parse();
    virtual ~registry_parse();

    void values_of_current_node(hive_h *hive_opened, hive_node_h p_node);

private:
    void find_attached_node_data(hive_h *hive_opened, hive_value_h p_keys, QString key_name_str);

protected:

    QStringList keys_name_list, values_list , hex_value_list ,hive_type_list ;
};

#endif // REGISTRY_H
