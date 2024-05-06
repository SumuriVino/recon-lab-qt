#include "case_info_display.h"
#include "ui_case_info_display.h"

case_info_display::case_info_display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::case_info_display)
{
    ui->setupUi(this);
    ui->treeWidget_multiple_source->hideColumn(1);
    ui->treeWidget_multiple_source->setAttribute(Qt::WA_MacShowFocusRect,false);
}

case_info_display::~case_info_display()
{
    delete ui;
}


void case_info_display::pub_initialise_case_info_display()
{
    display_sources_in_tree();
}


void case_info_display::slot_toplevel_source_button_clicked(bool expand ,int id,QString src_name,QString os_scheme)
{ // source button clicked to expand to see source information
    ui->textEdit_source_info_display->clear();
    QTreeWidgetItem *item = ui->treeWidget_multiple_source->topLevelItem(id);

    if(expand)
        ui->treeWidget_multiple_source->expandItem(item);
    else
        ui->treeWidget_multiple_source->collapseItem(item);
}

void case_info_display::slot_link_label_sub_source_list(QString link_txt)
{ // set source info for display in line edit
    ui->textEdit_source_info_display->clear();
    QString source_count_name = link_txt.trimmed();
    if(source_count_name.isEmpty())
        return;

    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString textedit_data = get_device_detail_filled_in_source_textEdit(struct_source_info);
    textedit_data.replace("\n" , "<br>");
    ui->textEdit_source_info_display->setHtml(textedit_data);


}

void case_info_display::display_sources_in_tree()
{ // display below examiner info
    QString str;
    str += QString("<b>Case Details </b>") + "<br>";
    str += QString("<b>Case No.  :  </b>") + global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_ID_QString).toString() + "<br>";
    str += QString("<b>Case Name  :  </b>") + global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString() + "<br>";
    str += QString("<b>Location  :  </b>") + global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Location_QString).toString() + "<br>";
    str += QString("<b>Case Notes  :  </b>") + global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Notes_QString).toString() + "<br>";

    QString machine_timezone_str = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Created_Timezone_QString).toString();
    QString numeric_timezone_value;
    if(machine_timezone_str.contains("GMT"))
    {
        QStringList tmp_timezone_list = machine_timezone_str.split("GMT");
        numeric_timezone_value = tmp_timezone_list.at(1);
    }

    qint64 case_time = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Created_Timestamp_UTC_qint64).toLongLong();
    if(case_time > 0)
    {
        str += QString("<b>Case Created Time  :  </b>") + QDateTime::fromSecsSinceEpoch(case_time, Qt::UTC).toString("MMM-dd-yyyy hh:mm:ss ") + QString("+00:00") + "<br>";
    }

    if(!machine_timezone_str.isEmpty())
    {
        str += QString("<b>Case Created Machine Timezone  :  </b>") + machine_timezone_str  + "<br>";
    }

    str += QString("<b>Case Created Version   :  </b>") + global_narad_muni_class_obj->get_field(MACRO_NARAD_Result_App_Version_QString).toString() + " (" + recon_static_functions::get_app_name() + ")" + "<br>";

    str += QString("<b>Case Path   :  </b>") + global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + "<br>";

    str += QString("<br><b>Examiner Details </b>") + "<br>";
    str += QString("<b>Examiner :  </b>") + global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Name_QString).toString() + "<br>";
    str += QString("<b>Examiner Phone   :  </b>") + global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Phone_QString).toString() + "<br>";
    str += QString("<b>Examiner Email   :  </b>") + global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Email_QString).toString() + "<br>";
    str += QString("<b>Agency Name   :  </b>") + global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString() + "<br>";
    str += QString("<b>Agency Address   :  </b>") + global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Address_QString).toString() + "<br>";
    str += QString("<b>Examiner Selected Time Zone  :  </b>") + global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_QString).toString() + "<br>";

    str += "<br>";
    str += QString("<b>Sources</b>");

    ui->textEdit_case_info->setHtml(str);

    QStringList list_rt_name;
    QStringList list_rt_cnt_name;
    QStringList list_to_check_main_source_list;
    QStringList list_to_check_root_count_list;

    QList<struct_GLOBAL_witness_info_root> list_general_rt_info = global_witness_info_manager_class_obj->pub_get_root_structure_QList();

    for(int pp = 0 ; pp < list_general_rt_info.size(); pp++)
    {
        struct_GLOBAL_witness_info_root struct_root_info = list_general_rt_info.at(pp);

        m_custom_source_button *pushbutton_source  = new m_custom_source_button(this);


        pushbutton_source->setStyleSheet("QPushButton {Text-align:left; margin: 1px;border-color: #D0D0D0; border-style: solid; border-radius: 1px; border-width: 1px;}"
                                         "QPushButton:pressed { border-color: #000000; }"
                                         " QPushButton:checked {border-color: #484848; }"
                                         "QPushButton:disabled {"
                                         " }");

        disconnect(pushbutton_source,SIGNAL(signal_source_button_clicked(bool,int,QString,QString)),this,SLOT(slot_toplevel_source_button_clicked(bool,int,QString,QString)));
        connect(pushbutton_source,SIGNAL(signal_source_button_clicked(bool,int,QString,QString)),this,SLOT(slot_toplevel_source_button_clicked(bool,int,QString,QString)));

        pushbutton_source->setAttribute(Qt::WA_MacShowFocusRect,false);


        QIcon m_icon = global_recon_helper_singular_class_obj->get_qicon_of_root(struct_root_info);
        pushbutton_source->setIcon(m_icon);

        QString rt_name = struct_root_info.root_name;

        pushbutton_source->setText(rt_name);
        pushbutton_source->pub_set_source_id(pp);
        pushbutton_source->pub_set_source_name(rt_name);
        pushbutton_source->pub_set_os_scheme("");
        pushbutton_source->setFixedSize(400, 30);
        pushbutton_source->setContentsMargins(0,6,0,6);

        QTreeWidgetItem *item_toplevel_source = new QTreeWidgetItem;
        item_toplevel_source->setText(1,QString::number(pp));
        ui->treeWidget_multiple_source->addTopLevelItem(item_toplevel_source);


        for(int sl = 0; sl < struct_root_info.source_count_name_all_QStringList.size(); sl++)
        {

            struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(struct_root_info.source_count_name_all_QStringList.at(sl));

            //============= if mounting gets failed, source count name and source name will be empty ============//
            if(struct_source_info.source_count_name.isEmpty() || struct_source_info.source_name.isEmpty())
                continue;


            QTreeWidgetItem *item_child_hyperlinks = new QTreeWidgetItem;
            item_child_hyperlinks->setIcon(0, global_recon_helper_singular_class_obj->get_qicon_of_source(struct_source_info));
            item_toplevel_source->addChild(item_child_hyperlinks);

            QLabel *label_source_link = new QLabel(this);

            label_source_link->setText("<a href = "+ struct_source_info.source_count_name + " > " + QString(struct_source_info.source_name) + " </a>");

            QObject::connect(label_source_link, SIGNAL(linkActivated(QString)), this, SLOT(slot_link_label_sub_source_list(QString)));

            ui->treeWidget_multiple_source->setItemWidget(item_child_hyperlinks, 0, label_source_link);
        }

        ui->treeWidget_multiple_source->setItemWidget(item_toplevel_source, 0, pushbutton_source);

    }

}


QString case_info_display::get_device_detail_filled_in_source_textEdit(struct_GLOBAL_witness_info_source target_source_info)
{ //  set and display below source info

    QString str = QString("<br><b>Source Details </b>") + "<br>";

    if(!target_source_info.complete_source_name.isEmpty())
        str += QString("<b>Source Name  :  </b>") + target_source_info.complete_source_name + QString("<br>");

    if(!target_source_info.evidence_number.isEmpty())
        str += QString("<b>Evidence No.  :  </b>") + target_source_info.evidence_number + QString("<br>");

    if(!target_source_info.root_type.isEmpty())
        str += QString("<b>Source Type  :  </b>") + target_source_info.root_type + QString("<br>");

    if(!target_source_info.os_scheme_display.isEmpty())
    {
        if((target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
                || target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                || target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_Uniform
                || target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML
                || target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal
                || target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Internal)
        {
            if(target_source_info.source_type_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
            {
                str += QString("<b>OS Type  :  </b>") + target_source_info.os_scheme_display + QString("<br>");
            }
        }
    }

    if(!target_source_info.file_system_type.isEmpty())
    {
        if(target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal
                || target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal
                || target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_otherOS_Internal)
        {
            str += QString("<b>File System  :  </b>") + target_source_info.file_system_type + QString("<br>");
        }
    }

    if(!target_source_info.system_user_QStringList.isEmpty())
    {
        if((target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Internal)
                || target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Internal)
        {
            if(target_source_info.source_type_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
            {
                str += QString("<b>User(s)  :  </b>") + target_source_info.system_user_QStringList.join(", ") + QString("<br>");
            }
        }
    }

    if(!target_source_info.configured_ids_QStringList.isEmpty())
        str += QString("<b>Configured ID    :  </b>") + target_source_info.configured_ids_QStringList.join(MACRO_RECON_Splitter_6_comma) + QString("<br>");

    if(!target_source_info.display_name.isEmpty())
        str += QString("<b>Display Name  :  </b>") + target_source_info.display_name + QString("<br>");

    if(!target_source_info.device_name.isEmpty())
        str += QString("<b>Device Name  :  </b>") + target_source_info.device_name + QString("<br>");

    if(!target_source_info.imei_no_QStringList.isEmpty())
        str += QString("<b>Device IMEI  :  </b>") + target_source_info.imei_no_QStringList.join(MACRO_RECON_Splitter_6_comma) + QString("<br>");

    if(!target_source_info.serial_no.isEmpty())
        str += QString("<b>Device Serial No  :  </b>") + target_source_info.serial_no + QString("<br>");

    if(!target_source_info.sim_no_iccid_QStringList.isEmpty())
        str += QString("<b>Device ICCID  :  </b>") + target_source_info.sim_no_iccid_QStringList.join(MACRO_RECON_Splitter_6_comma) + QString("<br>");

    if(!target_source_info.device_id.isEmpty())
        str += QString("<b>Device ID  :  </b>") + target_source_info.device_id + QString("<br>");

    if(!target_source_info.product_name.isEmpty())
        str += QString("<b>Product Name  :  </b>") + target_source_info.product_name + QString("<br>");

    if(!target_source_info.product_type.isEmpty())
        str += QString("<b>Product Type  :  </b>") + target_source_info.product_type + QString("<br>");

    if(!target_source_info.product_model.isEmpty())
        str += QString("<b>Product Model  :  </b>") + target_source_info.product_model + QString("<br>");

    if(!target_source_info.os_version_extended.isEmpty())
        str += QString("<b>OS Version  :  </b>") + target_source_info.os_version_extended + QString("<br>");

    if(!target_source_info.phone_no_msisdn_QStringList.isEmpty())
        str += QString("<b>Phone No.  :  </b>") + target_source_info.phone_no_msisdn_QStringList.join(MACRO_RECON_Splitter_6_comma) + QString("<br>");

    if(!target_source_info.target_type.isEmpty())
        str += QString("<b>Target Type  :  </b>") + target_source_info.target_type + QString("<br>");

    if(!target_source_info.itune_version.isEmpty())
        str += QString("<b>iTune Version  :  </b>") + target_source_info.itune_version + QString("<br>");

    if(!target_source_info.latest_backup_date_readable_QString.isEmpty())
        str += QString("<b>Backup Date  :  </b>") + target_source_info.latest_backup_date_readable_QString + QString("<br>");

    if(!target_source_info.build_version.isEmpty())
        str += QString("<b>Build Version  :  </b>") + target_source_info.build_version + QString("<br>");

    if(!target_source_info.country.isEmpty())
        str += QString("<b>Country  :  </b>") + target_source_info.country + QString("<br>");

    if(!target_source_info.city.isEmpty())
        str += QString("<b>City  :  </b>") + target_source_info.city + QString("<br>");

    if(!target_source_info.latitude.isEmpty())
        str += QString("<b>Latitude  :  </b>") + target_source_info.latitude + QString("<br>");

    if(!target_source_info.longitude.isEmpty())
        str += QString("<b>Longitude  :  </b>") + target_source_info.longitude + QString("<br>");

    if(!target_source_info.bluetooth_device_address.isEmpty())
        str += QString("<b>Bluetooth Device Address   :  </b>") + target_source_info.bluetooth_device_address + QString("<br>");

    if(!target_source_info.wifi_address.isEmpty())
        str += QString("<b>Wifi Address   :  </b>") + target_source_info.wifi_address + QString("<br>");

    if(!target_source_info.locale_language.isEmpty())
        str += QString("<b>Locale Language  :  </b>") + target_source_info.locale_language + QString("<br>");

    if(!target_source_info.timezone_readable_QString.isEmpty())
        str += QString("<b>System Time Zone  :  </b>") + target_source_info.timezone_readable_QString + QString("<br>");

    if(target_source_info.os_installer_timestamp_utc_numeric_qint64 != 0)
    {
        QDateTime date_timee;
        date_timee.setTimeSpec(Qt::UTC);
        date_timee = date_timee.fromSecsSinceEpoch(target_source_info.os_installer_timestamp_utc_numeric_qint64);
        QString  os_installer_timestamps = date_timee.toString("MMM-dd-yyyy hh:mm:ss");
        str += QString("<b>Installer Date :  </b>") + os_installer_timestamps + " +00:00" + QString("<br>");
    }

    if(!target_source_info.description.isEmpty())
        str += QString("<b>Description  :  </b>") + target_source_info.description + QString("<br>");


    return str;

}


