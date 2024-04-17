#include "ios_xml_backup_parser.h"

ios_xml_backup_parser::ios_xml_backup_parser(QObject *parent) : QObject(parent)
{
    ios_xml_report_path.clear();
}

void ios_xml_backup_parser::pub_set_ios_xml_file_path(QString path)
{
    ios_xml_report_path = path;
}

void ios_xml_backup_parser::pub_parse_ios_xml_device_info()
{
    recon_static_functions::app_debug("Starts", Q_FUNC_INFO);

    QString file_path =  ios_xml_report_path;
    QFile file(file_path);

    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open -----FAILED-----" + file_path , Q_FUNC_INFO);
        recon_static_functions::app_debug("file open -----ERROR-----" + file.errorString() , Q_FUNC_INFO);
        return;
    }

    // Create a document to read XML
    QDomDocument document;

    if(!document.setContent(&file))
    {
        recon_static_functions::app_debug("Failed to load the file for reading. -----FAILED-----" + file_path , Q_FUNC_INFO);
        file.close();
        return ;
    }

    // Getting root element
    QDomElement root = document.firstChildElement();
    if(root.tagName() == Macro_iOS_XML_Root_Element_project)
    {
        parse_ios_xml_device_info(root);
    }

    file.close();

    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}

void ios_xml_backup_parser::pub_parse_ios_xml_files_info()
{
    recon_static_functions::app_debug("Starts", Q_FUNC_INFO);

    QString file_path = ios_xml_report_path;
    QFile file(file_path);

    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open -----FAILED-----" + file_path , Q_FUNC_INFO);
        recon_static_functions::app_debug("file open -----ERROR-----" + file.errorString() , Q_FUNC_INFO);
        return;
    }

    // Create a document to read XML
    QDomDocument document;

    if(!document.setContent(&file))
    {
        recon_static_functions::app_debug("Failed to load the file for reading. -----FAILED-----" + file_path , Q_FUNC_INFO);
        file.close();
        return ;
    }

    // Getting root element
    QDomElement root = document.firstChildElement();
    if(root.tagName() == Macro_iOS_XML_Root_Element_project)
    {
        parse_ios_xml_files_info(root);
    }

    file.close();

    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}

void ios_xml_backup_parser::parse_ios_xml_device_info(QDomElement root)
{
    recon_static_functions::app_debug("Starts", Q_FUNC_INFO);

    clear_struct_device_info_variable();

    QDomNodeList metadata_node_list = root.elementsByTagName(Macro_iOS_XML_start_element_metadata);

    for (int ii = 0; ii < metadata_node_list.count() ; ii++ )
    {
        QDomNode metadata_node = metadata_node_list.at(ii);
        if(metadata_node.isElement())
        {
            QDomElement metadata_root_element = metadata_node.toElement();

            QString metadata_section_attribute_name = metadata_root_element.attribute("section");

            if(metadata_section_attribute_name == "Device Info")
            {
                QDomNodeList item_node_list = metadata_root_element.elementsByTagName(Macro_iOS_XML_start_element_metadata_child_item);

                for (int jj = 0; jj < item_node_list.count() ; jj++ )
                {
                    QDomNode item_node = item_node_list.at(jj);
                    if(item_node.isElement())
                    {
                        QDomElement item_node_element = item_node.toElement();

                        QString name = item_node_element.attribute("name");
                        QString group = item_node_element.attribute("group");
                        QString value = item_node_element.text();

                        if(name == Macro_iOS_XML_Item_ICCID)
                        {
                            struct_device_info_obj.sim_no_iccid_QStringList << value;
                            struct_device_info_obj.sim_no_iccid_QStringList.removeDuplicates();
                            struct_device_info_obj.sim_no_iccid_QStringList.removeAll("");
                        }
                        else if(name == Macro_iOS_XML_Item_DeviceInfoDetectedPhoneModel)
                        {
                            if(group.trimmed().isEmpty())
                            {
                                if(struct_device_info_obj.product_name.trimmed().isEmpty())
                                    struct_device_info_obj.product_name = value;
                            }
                            else if(group.contains("phone" , Qt::CaseInsensitive))
                            {
                                struct_device_info_obj.product_model = value;
                            }

                        }
                        else if(name == Macro_iOS_XML_Item_MSISDN)
                        {
                            struct_device_info_obj.phone_no_msisdn_QStringList << value;
                            struct_device_info_obj.phone_no_msisdn_QStringList.removeDuplicates();
                            struct_device_info_obj.phone_no_msisdn_QStringList.removeAll("");
                        }
                        else if(name == Macro_iOS_XML_Item_DeviceInfoOwnerName)
                        {
                            struct_device_info_obj.device_name = value;
                            struct_device_info_obj.display_name = value;
                        }
                        else if(name == Macro_iOS_XML_Item_DeviceInfoSerial)
                        {
                            struct_device_info_obj.serial_number = value;
                        }
                        else if(name == Macro_iOS_XML_Item_DeviceInfoBluetoothDeviceAddress)
                        {
                            struct_device_info_obj.bluetooth_device_address = value;
                        }
                        else if(name == Macro_iOS_XML_Item_DeviceInfoWiFiAddress)
                        {
                            struct_device_info_obj.wifi_address = value;
                        }
                        else if(name == Macro_iOS_XML_Item_DeviceInfoDetectedPhoneModelIdentifier)
                        {
                            struct_device_info_obj.product_type = value;
                        }
                        else if(name == Macro_iOS_XML_Item_DeviceInfoOSVersion)
                        {
                            struct_device_info_obj.product_version = value;
                        }
                        else if(name == Macro_iOS_XML_Item_IMEI)
                        {
                            struct_device_info_obj.imei_QStringList << value;
                            struct_device_info_obj.imei_QStringList.removeDuplicates();
                            struct_device_info_obj.imei_QStringList.removeAll("");

                        }
                        else if(name == Macro_iOS_XML_Item_DeviceInfoUniqueID)
                        {
                            struct_device_info_obj.unique_identifier = value;
                        }
                        else if(name == Macro_iOS_XML_Item_DeviceInfoAppleID)
                        {
                            struct_device_info_obj.apple_id = value;
                        }
                        else if(name == Macro_iOS_XML_Item_DeviceInfoOSType)
                        {
                            struct_device_info_obj.os_type = value;
                        }
                        else if(name == Macro_iOS_XML_Item_DeviceInfoTimeZone)
                        {
                            struct_device_info_obj.timezone = value;
                        }
                        else if(name == Macro_iOS_XML_Item_DeviceInfoLocaleLanguage)
                        {
                            struct_device_info_obj.locale_language = value;
                        }
                    }
                }
            }
        }
    }

    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}

void ios_xml_backup_parser::clear_struct_device_info_variable()
{

    recon_static_functions::app_debug("Starts", Q_FUNC_INFO);


    struct_device_info_obj.sim_no_iccid_QStringList.clear();
    struct_device_info_obj.imei_QStringList.clear();
    struct_device_info_obj.phone_no_msisdn_QStringList.clear();

    struct_device_info_obj.display_name.clear();
    struct_device_info_obj.product_name.clear();
    struct_device_info_obj.device_name.clear();

    struct_device_info_obj.product_type.clear();
    struct_device_info_obj.product_version.clear();
    struct_device_info_obj.serial_number.clear();

    struct_device_info_obj.unique_identifier.clear();
    struct_device_info_obj.apple_id.clear();
    struct_device_info_obj.product_model.clear();

    struct_device_info_obj.bluetooth_device_address.clear();
    struct_device_info_obj.wifi_address.clear();
    struct_device_info_obj.timezone.clear();
    struct_device_info_obj.locale_language.clear();
    struct_device_info_obj.os_type.clear();


    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);

}


void ios_xml_backup_parser::parse_ios_xml_files_info(QDomElement root)
{
    recon_static_functions::app_debug("Starts", Q_FUNC_INFO);

    QDomNodeList tagged_files_node_list = root.elementsByTagName(Macro_iOS_XML_start_element_tagged_files);

    for (int ii = 0; ii < tagged_files_node_list.count() ; ii++ )
    {
        QDomNode tagged_files_node = tagged_files_node_list.at(ii);

        if(tagged_files_node.isElement())
        {
            QDomElement tagged_files_root_element = tagged_files_node.toElement();

            QDomNodeList tagged_files_child_file_nodes_list = tagged_files_root_element.elementsByTagName(Macro_iOS_XML_start_element_tagged_files_child_file);

            for (int jj = 0; jj < tagged_files_child_file_nodes_list.count() ; jj++ )
            {
                QDomNode child_file_node = tagged_files_child_file_nodes_list.at(jj);

                if(child_file_node.isElement())
                {
                    QDomElement child_file_node_element = child_file_node.toElement();

                    //-------------------- files info---------------------------//
                    QString fs = child_file_node_element.attribute("fs");
                    QString fsid = child_file_node_element.attribute("fsid");
                    QString path = child_file_node_element.attribute("path");
                    QString size = child_file_node_element.attribute("size");
                    QString id = child_file_node_element.attribute("id");

                    QString extractionId = child_file_node_element.attribute("extractionId");
                    QString deleted = child_file_node_element.attribute("deleted");
                    QString embedded = child_file_node_element.attribute("embedded");
                    QString isrelated = child_file_node_element.attribute("isrelated");
                    QString source_index = child_file_node_element.attribute("source_index");

                    struct_ios_xml_files_info struct_files_info_obj;
                    struct_files_info_obj.fs = fs;
                    struct_files_info_obj.fsid = fsid;
                    struct_files_info_obj.domain_path = path;
                    struct_files_info_obj.size_byte = size.toLongLong();
                    struct_files_info_obj.file_id = id;

                    struct_files_info_obj.extraction_id = extractionId;
                    struct_files_info_obj.deleted = deleted;

                    if(embedded.contains(Macro_iOS_XML_Flag_False,Qt::CaseInsensitive))
                    {
                        struct_files_info_obj.embedded = false;
                    }
                    else
                    {
                        struct_files_info_obj.embedded = true;
                    }

                    if(isrelated.contains(Macro_iOS_XML_Flag_False,Qt::CaseInsensitive))
                    {
                        struct_files_info_obj.isrelated = false;
                    }
                    else
                    {
                        struct_files_info_obj.isrelated = true;
                    }

                    struct_files_info_obj.source_index = source_index;
                    //-------------------- files info---------------------------//

                    //--------------------accessInfo----------------------------//
                    QDomNodeList accessInfo_nodes_list = child_file_node_element.elementsByTagName(Macro_iOS_XML_start_element_tagged_files_child_file_child_accessInfo);
                    for (int kk = 0; kk < accessInfo_nodes_list.count() ; kk++ )
                    {
                        QDomNode accessInfo_node = accessInfo_nodes_list.at(kk);
                        if(accessInfo_node.isElement())
                        {
                            QDomElement accessInfo_node_element = accessInfo_node.toElement();
                            QDomNodeList timestamp_nodes_list = accessInfo_node_element.elementsByTagName(Macro_iOS_XML_start_element_tagged_files_child_file_child_accessInfo_child_timestamp);

                            struct_ios_xml_tagged_files_timestamp_info timestamp_info_obj;

                            timestamp_info_obj.access_timestamp_numeric_qint64 = 0;
                            timestamp_info_obj.modify_timestamp_numeric_qint64 = 0;
                            timestamp_info_obj.change_timestamp_numeric_qint64 = 0;
                            timestamp_info_obj.create_timestamp_numeric_qint64 = 0;

                            for (int ll = 0; ll < timestamp_nodes_list.count() ; ll++ )
                            {
                                QDomNode timestamp_node = timestamp_nodes_list.at(ll);

                                if(timestamp_node.isElement())
                                {
                                    QDomElement timestamp_node_element = timestamp_node.toElement();

                                    QString name = timestamp_node_element.attribute("name");
                                    QString value = timestamp_node_element.text();

                                    qint64 epch_timestamp = global_recon_helper_singular_class_obj->convert_timestamp_readable_to_numeric_3(value,Q_FUNC_INFO);

                                    if(name == Macro_iOS_XML_Item_ChangeTime)
                                    {
                                        timestamp_info_obj.change_timestamp_numeric_qint64 = epch_timestamp;
                                    }
                                    else if(name == Macro_iOS_XML_Item_ModifyTime)
                                    {
                                        timestamp_info_obj.modify_timestamp_numeric_qint64 = epch_timestamp;
                                    }
                                    else if(name == Macro_iOS_XML_Item_AccessTime)
                                    {
                                        timestamp_info_obj.access_timestamp_numeric_qint64 = epch_timestamp;
                                    }
                                    else if(name == Macro_iOS_XML_Item_CreationTime)
                                    {
                                        timestamp_info_obj.create_timestamp_numeric_qint64 = epch_timestamp;
                                    }
                                }
                            }

                            struct_files_info_obj.struct_tagged_files_timestamp_info_obj = timestamp_info_obj;
                        }
                    }
                    //--------------------accessInfo---------------------------//

                    //--------------------metadata sections----------------------------//

                    QDomNodeList metadata_section_nodes_list = child_file_node_element.elementsByTagName(Macro_iOS_XML_start_element_tagged_files_child_file_child_metadata);
                    for (int mm = 0; mm < metadata_section_nodes_list.count() ; mm++ )
                    {
                        QDomNode metadata_section_node = metadata_section_nodes_list.at(mm);
                        if(metadata_section_node.isElement())
                        {
                            QDomElement metadata_section_node_element = metadata_section_node.toElement();

                            QString metadata_section_node_attrburte_name = metadata_section_node_element.attribute("section");

                            if(metadata_section_node_attrburte_name == "File")
                            {
                                QDomNodeList item_nodes_list = metadata_section_node_element.elementsByTagName(Macro_iOS_XML_start_element_tagged_files_child_file_child_metadata_child_item);

                                struct_ios_xml_tagged_files_metadata_section_file_info metadata_section_file_info_obj;

                                metadata_section_file_info_obj.hash_md5.clear();
                                metadata_section_file_info_obj.hash_sha256.clear();
                                metadata_section_file_info_obj.preview_file_path.clear();
                                metadata_section_file_info_obj.tags.clear();

                                for (int nn = 0; nn < item_nodes_list.count() ; nn++ )
                                {
                                    QDomNode item_node = item_nodes_list.at(nn);

                                    if(item_node.isElement())
                                    {
                                        QDomElement item_node_element = item_node.toElement();

                                        QString name = item_node_element.attribute("name");
                                        QString value = item_node_element.text();

                                        if(name == Macro_iOS_XML_Item_Local_Path)
                                        {
                                            QString preview_path = value;

                                            preview_path.replace("\\" , "/");

                                            if(!preview_path.startsWith("/"))
                                                preview_path.prepend("/");

                                            if(preview_path.endsWith("/"))
                                                preview_path.chop(1);

                                            metadata_section_file_info_obj.preview_file_path = preview_path;
                                            continue;
                                        }

                                        if(name == Macro_iOS_XML_Item_SHA256)
                                        {
                                            metadata_section_file_info_obj.hash_sha256 = value;
                                            continue;
                                        }

                                        if(name == Macro_iOS_XML_Item_MD5)
                                        {
                                            metadata_section_file_info_obj.hash_md5 = value;
                                            continue;
                                        }

                                        if(name == Macro_iOS_XML_Item_Tags)
                                        {
                                            metadata_section_file_info_obj.tags = value;
                                            continue;
                                        }

                                    }
                                }

                                struct_files_info_obj.struct_tagged_files_metadata_section_file_info_obj = metadata_section_file_info_obj;

                            }

                        }
                    }

                    struct_ios_xml_files_info_qlist.append(struct_files_info_obj);
                }
            }
        }
    }

    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}


struct_ios_xml_device_info ios_xml_backup_parser::pub_get_struct_device_info_obj()
{
    return struct_device_info_obj;
}

QList<struct_ios_xml_files_info> ios_xml_backup_parser::pub_get_struct_files_info_qlist()
{
    return struct_ios_xml_files_info_qlist;
}
