#include "registry_parse.h"

registry_parse::registry_parse()
{
}

registry_parse::~registry_parse()
{
}




void registry_parse::values_of_current_node(hive_h *hive_opened, hive_node_h p_node)
{ // get the values of current node of registry record
    keys_name_list.clear();
    values_list.clear();
    hex_value_list.clear();
    hive_type_list.clear();

    // Get child keys
    hive_value_h *p_keys = hivex_node_values(hive_opened, p_node);
    if(p_keys==NULL)
        return;


    int i = 0;
    char *p_name;
    while(p_keys[i])
    {

        p_name = hivex_value_key(hive_opened, p_keys[i]);
        if(p_name == NULL)
            return;


        QString p_name_qstr(p_name);
        find_attached_node_data(hive_opened,p_keys[i],p_name_qstr);
        free(p_name);
        i++;

    }

}

void registry_parse::find_attached_node_data(hive_h *hive_opened,hive_value_h p_keys ,QString key_name_str)
{ // find attached node data

    hive_set_value *val;
    val = new hive_set_value;

    hivex_value_type(hive_opened, p_keys, &val->t, &val->len);

    val->value = hivex_value_value(hive_opened, p_keys, &val->t, &val->len);

    QByteArray value_to_hex(val->value, val->len);
    QByteArray hex_value_arr = value_to_hex;

    value_to_hex.replace('\0', "");
    QString t_val_str = QString::fromLocal8Bit(value_to_hex);


    if(val->t == hive_t_REG_NONE)
    {
        hive_type_list << "REG_NONE";

        t_val_str = hex_value_arr.toHex().toUpper();

        if(hex_value_arr.toHex().trimmed().isEmpty())
        {
            t_val_str = QString("(zero-length binary value)");
        }
    }
    else if(val->t == hive_t_REG_SZ)
    {
        hive_type_list << "REG_SZ";

        t_val_str = QString::fromUtf16((ushort *)hex_value_arr.data(), hex_value_arr.size()/2 - 1);
    }
    else if(val->t == hive_t_REG_EXPAND_SZ)
    {
        hive_type_list << "REG_EXPAND_SZ";
    }
    else if(val->t == hive_t_REG_BINARY)
    {
        hive_type_list << "REG_BINARY";
        t_val_str = hex_value_arr.toHex().toUpper();

    }
    else if(val->t == hive_t_REG_DWORD)
    {
        hive_type_list << "REG_DWORD";

        quint32 val_dword = hex_value_arr.toHex().toUpper().toULong(0,16);
        t_val_str = QString("0x") + hex_value_arr.toHex().toUpper() + QString(" (") + QString::number(val_dword)  + QString(")") ;

    }
    else if(val->t == hive_t_REG_DWORD_BIG_ENDIAN)
    {
        hive_type_list  << "REG_DWORD_BIG_ENDIAN";

        quint32 val_dword_big_endian = hex_value_arr.toHex().toUpper().toULong(0,16);
        t_val_str = QString("0x") + hex_value_arr.toHex().toUpper() + QString(" (") + QString::number(val_dword_big_endian)  + QString(")") ;

    }
    else if(val->t == hive_t_REG_LINK)
    {
        hive_type_list << "REG_LINK";
        t_val_str = hex_value_arr.toHex().toUpper();

    }
    else if(val->t == hive_t_REG_MULTI_SZ)
    {
        hive_type_list << "REG_MULTI_SZ";
    }
    else if(val->t == hive_t_REG_RESOURCE_LIST)
    {
        hive_type_list << "REG_RESOURCE_LIST";
        t_val_str = hex_value_arr.toHex().toUpper();

    }
    else if(val->t == hive_t_REG_FULL_RESOURCE_DESCRIPTOR)
    {
        //hive_type_list << "REG_FULL_RESOURCE_DESC";
        hive_type_list << "REG_FULL_RESOURCE_DESCRIPTOR";
        t_val_str = hex_value_arr.toHex().toUpper();
    }
    else if(val->t == hive_t_REG_RESOURCE_REQUIREMENTS_LIST)
    {
        //hive_type_list << "REG_RESOURCE_REQ_LIST";
        hive_type_list << "REG_RESOURCE_REQUIREMENTS_LIST";
        t_val_str = hex_value_arr.toHex().toUpper();
    }
    else if(val->t == hive_t_REG_QWORD)
    {
        hive_type_list << "REG_QWORD";

        quint64 val_qword = hex_value_arr.toHex().toUpper().toULongLong(0,16);
        t_val_str = QString("0x") + hex_value_arr.toHex().toUpper() + QString(" (") + QString::number(val_qword)  + QString(")") ;
    }


//    QByteArray value_to_hex(val->value, val->len);
//    QByteArray hex_value_arr = value_to_hex;

//    value_to_hex.replace('\0', "");
//    QString t_val_str = QString::fromLocal8Bit(value_to_hex);

    if(hex_value_arr.toHex().trimmed().isEmpty() && val->t != hive_t_REG_NONE)
    {
        t_val_str = QString("(value not set)");
    }

    keys_name_list << key_name_str;
    values_list << t_val_str;
    //hex_value_list << hex_value_arr.toHex();
    hex_value_list << hex_value_arr.toHex().toUpper();

}



