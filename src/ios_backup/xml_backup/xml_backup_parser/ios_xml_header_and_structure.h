#ifndef IOS_XML_HEADER_AND_STRUCTURE_H
#define IOS_XML_HEADER_AND_STRUCTURE_H

#include <QtCore>

#define Macro_iOS_XML_Root_Element_project                                                          "project"
#define Macro_iOS_XML_Root_Element_index_zero_int                                                    0

#define Macro_iOS_XML_start_element_metadata                                                        "metadata"
#define Macro_iOS_XML_start_element_metadata_child_item                                             "item"

#define Macro_iOS_XML_start_element_tagged_files                                                    "taggedFiles"
#define Macro_iOS_XML_start_element_tagged_files_child_file                                         "file"
#define Macro_iOS_XML_start_element_tagged_files_child_file_child_accessInfo                        "accessInfo"
#define Macro_iOS_XML_start_element_tagged_files_child_file_child_accessInfo_child_timestamp        "timestamp"

#define Macro_iOS_XML_start_element_tagged_files_child_file_child_metadata                          "metadata"
#define Macro_iOS_XML_start_element_tagged_files_child_file_child_metadata_child_item               "item"


#define Macro_iOS_XML_Item_Local_Path                                                               "Local Path"
#define Macro_iOS_XML_Item_SHA256                                                                   "SHA256"
#define Macro_iOS_XML_Item_MD5                                                                      "MD5"
#define Macro_iOS_XML_Item_Tags                                                                     "Tags"

#define Macro_iOS_XML_Item_ChangeTime                                                               "ChangeTime"
#define Macro_iOS_XML_Item_CreationTime                                                             "CreationTime"
#define Macro_iOS_XML_Item_ModifyTime                                                               "ModifyTime"
#define Macro_iOS_XML_Item_AccessTime                                                               "AccessTime"

#define Macro_iOS_XML_Item_DeviceInfoOSVersion                                                      "DeviceInfoOSVersion"
#define Macro_iOS_XML_Item_DeviceInfoBluetoothDeviceAddress                                         "DeviceInfoBluetoothDeviceAddress"
#define Macro_iOS_XML_Item_DeviceInfoWiFiAddress                                                    "DeviceInfoWiFiAddress"
#define Macro_iOS_XML_Item_DeviceInfoTimeZone                                                       "DeviceInfoTimeZone"
#define Macro_iOS_XML_Item_DeviceInfoOSType                                                         "DeviceInfoOSType"
#define Macro_iOS_XML_Item_DeviceInfoLocaleLanguage                                                 "DeviceInfoLocaleLanguage"

#define Macro_iOS_XML_Item_DeviceInfoDetectedPhoneModel                                             "DeviceInfoDetectedPhoneModel"
#define Macro_iOS_XML_Item_DeviceInfoOwnerName                                                      "DeviceInfoOwnerName"
#define Macro_iOS_XML_Item_DeviceInfoSerial                                                         "DeviceInfoSerial"
#define Macro_iOS_XML_Item_DeviceInfoDetectedPhoneModelIdentifier                                   "DeviceInfoDetectedPhoneModelIdentifier"
#define Macro_iOS_XML_Item_DeviceInfoUniqueID                                                       "DeviceInfoUniqueID"
#define Macro_iOS_XML_Item_DeviceInfoAppleID                                                        "DeviceInfoAppleID"
#define Macro_iOS_XML_Item_IMEI                                                                     "IMEI"
#define Macro_iOS_XML_Item_IMSI                                                                     "IMSI"
#define Macro_iOS_XML_Item_ICCID                                                                    "ICCID"
#define Macro_iOS_XML_Item_MSISDN                                                                   "MSISDN"

#define Macro_iOS_XML_Flag_True                                                                     "true"
#define Macro_iOS_XML_Flag_False                                                                    "false"


struct struct_ios_xml_device_info
{
    QStringList imei_QStringList;
    QStringList sim_no_iccid_QStringList;
    QStringList phone_no_msisdn_QStringList;

    QString display_name;
    QString product_name;
    QString device_name;
    QString product_type;
    QString product_version;
    QString serial_number;
    QString unique_identifier;
    QString apple_id;
    QString timezone;
    QString product_model;
    QString bluetooth_device_address;
    QString wifi_address;
    QString locale_language;
    QString os_type;

};

struct struct_ios_xml_tagged_files_timestamp_info
{
    qint64 change_timestamp_numeric_qint64;
    qint64 modify_timestamp_numeric_qint64;
    qint64 access_timestamp_numeric_qint64;
    qint64 create_timestamp_numeric_qint64;
};

struct struct_ios_xml_tagged_files_metadata_section_file_info
{
    QString preview_file_path;
    QString hash_sha256;
    QString hash_md5;
    QString tags;
};

struct struct_ios_xml_files_info
{
    QString fs;
    QString fsid;
    QString domain_path;
    qint64  size_byte;
    QString file_id;
    QString extraction_id;
    QString deleted;
    bool    embedded;
    bool    isrelated;
    QString source_index;

    struct_ios_xml_tagged_files_timestamp_info struct_tagged_files_timestamp_info_obj;
    struct_ios_xml_tagged_files_metadata_section_file_info struct_tagged_files_metadata_section_file_info_obj;
};
//***********************************************************************************************

#endif // IOS_XML_HEADER_AND_STRUCTURE_H
