#include "ram_analysis.h"
#include "ui_ram_analysis.h"

ram_analysis::ram_analysis(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ram_analysis)
{
    ui->setupUi(this);

    message_dialog_object = new message_dialog(this);

    ui->pushButton_refresh_source->setIcon(QIcon("../icons/signs/refresh.png"));

    recon_helper_standard_obj = new recon_helper_standard(this);


    ui->comboBox_operating_system->addItem(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display);
    ui->comboBox_operating_system->addItem(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display);
    ui->comboBox_operating_system->setCurrentIndex(0);

    process_ram_analyzer = new QProcess(this);
    connect(process_ram_analyzer, SIGNAL(finished(int)), this, SLOT(slot_process_ram_analyzer_finished(int)));



    map_mac_plugin =  get_plugin_map(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display);

    QMap <QString, QString>::iterator mapiter;
    for(mapiter = map_mac_plugin.begin(); mapiter != map_mac_plugin.end() ; mapiter++)
    {
        ui->comboBox_volatility_commands->addItem(mapiter.value());
    }


    mac_profiles_list      = get_profile_list(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display);
    windows_profiles_list  =  get_profile_list(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display);


    ui->comboBox_build_version->clear();
    ui->comboBox_build_version->addItems(mac_profiles_list);
    ui->comboBox_build_version->setCurrentIndex(0);


    save_result_dialog_box_obj = new save_result_dialog_box(this);
    connect(save_result_dialog_box_obj, SIGNAL(signal_save_result_name_value(QString)), this, SLOT(slot_ram_analysis_saved_result_in_db_by_saved_name(QString)));
    connect(save_result_dialog_box_obj, SIGNAL(signal_cancel_save_results()), this, SLOT(slot_ram_analysis_cancel_save_results()));

    ui->pushButton_export->setEnabled(true);
    ui->pushButton_save->setEnabled(true);

    export_dialog_box_1_obj = new export_dialog_box_1(this);
    connect(export_dialog_box_1_obj, SIGNAL(signal_save_results_by_export_window(QString , QString, QString)), this, SLOT(slot_ram_analysis_saved_result_in_file_by_saved_name(QString , QString,QString)));
    connect(export_dialog_box_1_obj, SIGNAL(signal_cancel_export_results()), this, SLOT(slot_ram_analysis_cancel_export_results()));



    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));
    bool_cancel_loading = false;

    display_loading_progress_bar_non_cancelable_obj = new display_loading_progress_bar(this);
    display_loading_progress_bar_non_cancelable_obj->pub_set_hidden_cancel_button(true);


}

ram_analysis::~ram_analysis()
{
    delete ui;
}

void ram_analysis::pub_set_essentials()
{
    destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");

}

void ram_analysis::pub_set_fill_sources()
{
    fill_sources_in_source_combobox();
}

void ram_analysis::pub_set_version_international()
{
    ui->pushButton_password_analyser->hide();
}

void ram_analysis::fill_sources_in_source_combobox()
{

    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);
        if(obj.source_type_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal)
            continue;

        QString ram_image_path = recon_static_functions::get_complete_file_path(obj.virtual_source_path, obj.source_name, Q_FUNC_INFO);

        QFileInfo minfo(ram_image_path);
        if(minfo.exists())
        {
            ui->comboBox_source->addItem(minfo.fileName());
        }
    }

    if(ui->comboBox_source->currentText().trimmed().isEmpty())
    {
        ui->pushButton_execute_command->setDisabled(true);
        ui->pushButton_export->setDisabled(true);
        ui->pushButton_save->setDisabled(true);
    }

}


void ram_analysis::on_pushButton_execute_command_clicked()
{
    ui->textEdit_command_output->clear();


    QString command_vol_dir = "select volatility_dir from tbl_volatility";
    QString volatility_dir = recon_helper_standard_obj->get_string_from_db_by_dbpath(command_vol_dir, 0, destination_db_file, Q_FUNC_INFO);

    QString volatility_script_path = volatility_dir + "/vol.py";

    QFileInfo info(volatility_script_path);

    if(!info.exists())
    {
        QMessageBox::warning(this,recon_static_functions::get_app_name(), "Volatility not found.");
        return;
    }

    QString source_name = ui->comboBox_source->currentText();
    QString source_path;


    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);
        if(obj.source_type_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal)
            continue;

        QString ram_image_path = recon_static_functions::get_complete_file_path(obj.virtual_source_path, obj.source_name, Q_FUNC_INFO);

        QFileInfo minfo(ram_image_path);
        if(minfo.fileName() == source_name)
        {
            source_path = ram_image_path;
            break;
        }
    }

    QString command_vol;
    QMap <QString, QString>::iterator mapiter;
    for(mapiter = map_mac_plugin.begin(); mapiter != map_mac_plugin.end() ; mapiter++)
    {
        if(ui->comboBox_volatility_commands->currentText() == mapiter.value())
            command_vol  = mapiter.key();
    }

    // ./vol.py --info

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please wait while executing command...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString command = global_varibale_python_path;

    QStringList args_list;
    args_list << volatility_script_path  << "--profile=" + ui->comboBox_build_version->currentText()
              << command_vol.trimmed() << "-f" << source_path;

    process_ram_analyzer->start(command, args_list);

    enable_disable_ui_on_execute_click(false);

}

void ram_analysis::enable_disable_ui_on_execute_click(bool status)
{
    ui->comboBox_source->setEnabled(status);
    ui->comboBox_build_version->setEnabled(status);
    ui->comboBox_operating_system->setEnabled(status);
    ui->comboBox_volatility_commands->setEnabled(status);

    ui->pushButton_execute_command->setEnabled(status);
    ui->pushButton_export->setEnabled(status);
    ui->pushButton_save->setEnabled(status);
    ui->pushButton_password_analyser->setEnabled(status);
    ui->pushButton_refresh_source->setEnabled(status);

}


void ram_analysis::on_pushButton_save_clicked()
{
    recon_static_functions::app_debug("---starts---",Q_FUNC_INFO);

    QString selected_source = ui->comboBox_source->currentText();
    QString selected_artifacts = ui->comboBox_volatility_commands->currentText();
    QString set_line_edit_text = selected_source + "-" + selected_artifacts;

    save_result_dialog_box_obj->show();
    save_result_dialog_box_obj->pub_set_name(set_line_edit_text);
    ui->pushButton_save->setDisabled(true);
    ui->pushButton_export->setDisabled(true);

    recon_static_functions::app_debug("---end---",Q_FUNC_INFO);

}

void ram_analysis::on_pushButton_export_clicked()
{
    recon_static_functions::app_debug("start",Q_FUNC_INFO);

    QString selected_source = ui->comboBox_source->currentText();
    QString selected_artifacts = ui->comboBox_volatility_commands->currentText();

    QString set_line_edit_text = selected_source + "-" + selected_artifacts;

    QString export_dir_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_RAM_Analysis_QString).toString();
    export_dialog_box_1_obj->show();
    ui->pushButton_export->setDisabled(true);
    ui->pushButton_save->setDisabled(true);
    export_dialog_box_1_obj->pub_set_name_and_dir_path(set_line_edit_text,export_dir_path);

    recon_static_functions::app_debug("end",Q_FUNC_INFO);
}

void ram_analysis::on_comboBox_operating_system_currentTextChanged(const QString &arg1)
{
    ui->comboBox_build_version->clear();

    if(arg1 == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display))
    {
        ui->comboBox_build_version->addItems(mac_profiles_list);
    }
    else if(arg1 == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display))
    {
        ui->comboBox_build_version->addItems(windows_profiles_list);
    }
    else
    {

    }

    ui->comboBox_build_version->setCurrentIndex(0);

}

void ram_analysis::slot_process_ram_analyzer_finished(int status)
{
    ui->textEdit_command_output->clear();

    display_loading_progress_bar_non_cancelable_obj->hide();

    while(!process_ram_analyzer->atEnd())
    {
        QString process_output_str = process_ram_analyzer->readLine();

        if(process_output_str.startsWith("*** Failed to import volatility.plugins."))
            continue;

        ui->textEdit_command_output->append(process_output_str);

    }

    ui->textEdit_command_output->moveCursor(QTextCursor::Start);

    enable_disable_ui_on_execute_click(true);
}



void ram_analysis::on_pushButton_password_analyser_clicked()
{

    password_count = 0;


    QString source_name = ui->comboBox_source->currentText();

    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();


    bool bool_source_found = false;
    struct_GLOBAL_witness_info_source src_obj;
    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        src_obj = list_target_source_info.at(count);
        if(src_obj.source_type_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal)
            continue;

        QString ram_image_path = recon_static_functions::get_complete_file_path(src_obj.virtual_source_path, src_obj.source_name, Q_FUNC_INFO);

        QFileInfo minfo(ram_image_path);
        if(minfo.fileName() == source_name)
        {
            bool_source_found = true;
            break;
        }
    }

    if(!bool_source_found)
    {
        return;
    }


    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->show();



    QString source_count_name, os_name;
    source_count_name = src_obj.source_count_name;
    //os_name = src_obj.os_scheme_internal;

    create_carved_password_dbs(src_obj.source_count_name , src_obj.source_name);

    QString file_path = recon_static_functions::get_complete_file_path(src_obj.virtual_source_path, src_obj.source_name, Q_FUNC_INFO);

    //QString file_path = src_obj.source_path_1;

    //    QFile m_file(file_path);
    //    if(!m_file.open(QIODevice::ReadOnly))
    //    {
    //        recon_static_functions::app_debug("File Open ---FAILED---: "  + m_file.fileName(), Q_FUNC_INFO);
    //        recon_static_functions::app_debug("Error " + m_file.errorString() , Q_FUNC_INFO);
    //        ui->label_message->hide();
    //        ui->progressBar_volatility->hide();

    //        return;
    //    }

    enable_disable_ui_on_execute_click(false);


    password_first_method(file_path, src_obj);
    password_second_method(file_path, src_obj);
    password_third_method(file_path, src_obj);


    display_loading_progress_bar_obj->hide();

    //    QStringList pwd_list;
    //    int count = 0;
    //    while(1)
    //    {
    //        count++;
    //        if(count % 500 == 0)
    //        {
    //            count = 0;
    //            QCoreApplication::processEvents();
    //        }

    //        QByteArray dar = m_file.read(102400);
    //        if(dar.size() <= 0)
    //        {
    //            break;
    //        }

    //        if(dar.contains("XpasswordZrecordtypeXfuncnameZrecordname]"))
    //        {
    //            int ind =  dar.indexOf("XpasswordZrecordtypeXfuncnameZrecordname]");

    //            int ind_start = ind;
    //            QString line_data = QString::fromLocal8Bit(dar.mid(ind_start, 200));
    //            line_data.remove(line_data.indexOf("_dsRecTypeStandard:Users_ODRecordVerifyPasswordV"), line_data.size());
    //            QStringList m_list = line_data.split("passwordZrecordtypeXfuncnameZrecordname]");
    //            if(m_list.size() > 1)
    //            {
    //                QString pwd = m_list.at(1);
    //                pwd_list << pwd;
    //            }

    //        }
    //    }


    //    m_file.close();


    //    pwd_list.removeDuplicates();

    //    QString output = pwd_list.join("\n");
    //    if(output.trimmed().isEmpty())
    //        output = "No password found";


    //    message_dialog_object->pub_set_message(MACRO_MSG_Nothing, "<b>Extracted Password: </b><br>" + output);
    //    message_dialog_object->show();

    enable_disable_ui_on_execute_click(true);



    //    QString command_2 = QString("INSERT INTO carved_password"
    //                                "(bookmark ,plugin_name, tab_name ,m_password ,m_server,source_count_name,os_scheme_display)"
    //                                "VALUES(?,?,?,?,?,?,?)");
    //    QStringList arg_value_list;
    //    arg_value_list << "0" << MACRO_Plugin_Name_Carved_Password << MACRO_Plugin_Carved_Passwords_TAB_Name_Passwords << "m_password" << "m_server" << source_count_name << os_name;
    //    recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command_2,arg_value_list,carved_password_db_path,Q_FUNC_INFO);

    emit signal_ram_analysis_carved_password_add_in_case_tree(carved_password_result_name);

}

void ram_analysis::saved_ram_analaysis_in_db_on_save_button_clicked(QString saved_name)
{
    QString saved_rm_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Analysis_Saved_In_Result_QString).toString() + "ram_analysis_saved.sqlite";
    QString temp_saved_name = saved_name;

    QString selected_source        = ui->comboBox_source->currentText();
    QString selected_os            = ui->comboBox_operating_system->currentText();
    QString selected_build_version = ui->comboBox_build_version->currentText();
    QString selected_artifacts     = ui->comboBox_volatility_commands->currentText();
    QString command_output         = ui->textEdit_command_output->toPlainText();


    QString command = QString("insert into tbl_saved_result(source_name,operating_system,build_version,artifacts,command_output,result_name,plugin_name,tab_name,os_scheme_display) values (?,?,?,?,?,?,?,?,?)");
    QStringList values_list;
    values_list << selected_source << selected_os << selected_build_version
                << selected_artifacts << command_output << temp_saved_name
                << QString(MACRO_Plugin_Name_Ram_Analysis_Saved)
                << QString(MACRO_Plugin_RAM_Analysis_Saved_TAB_Name_Saved_Result)
                << "";

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, values_list,saved_rm_db_path,Q_FUNC_INFO);
    emit signal_ram_analysis_saved_add_in_case_tree(QString(MACRO_Plugin_Name_Ram_Analysis_Saved));

}

void ram_analysis::saved_ram_analaysis_in_file_on_export_button_clicked(QString saved_name)
{
    QString label_source_text         = ui->label_source->text();
    QString label_build_version_text  = ui->label_build_version->text();
    QString label_opr_system_text     = ui->label_operating_system->text();
    QString label_volatility_cmd_text = ui->label_volatility_commands->text();


    QString selected_source        = ui->comboBox_source->currentText();
    QString selected_os            = ui->comboBox_operating_system->currentText();
    QString selected_build_version = ui->comboBox_build_version->currentText();
    QString selected_artifacts     = ui->comboBox_volatility_commands->currentText();
    QString command_output         = ui->textEdit_command_output->toPlainText();


    QString saved_export_file_path = saved_name;


    ///1.end------saved name--------
    ///2.start--------data------
    QString text_to_be_written_str;
    QString colon_sep = QString(": ");
    ///-----------source info

    text_to_be_written_str.append("\n");
    text_to_be_written_str.append(label_source_text + colon_sep);
    text_to_be_written_str.append(selected_source);
    text_to_be_written_str.append("\n");

    ///-----------operating system info
    text_to_be_written_str.append(label_opr_system_text + colon_sep);
    text_to_be_written_str.append(selected_os) ;
    text_to_be_written_str.append("\n");

    ///-----------build version info
    text_to_be_written_str.append(label_build_version_text + colon_sep);
    text_to_be_written_str.append(selected_build_version);
    text_to_be_written_str.append("\n");


    ///-----------artifcats
    text_to_be_written_str.append(label_volatility_cmd_text + colon_sep);
    text_to_be_written_str.append(selected_artifacts);
    text_to_be_written_str.append("\n").append("\n").append("\n");

    ///-----------output command
    text_to_be_written_str.append(QString("Command Output") + colon_sep);
    text_to_be_written_str.append("\n");
    text_to_be_written_str.append(command_output);
    ///2.end------data--------




    QFile dest_file;
    dest_file.setFileName(saved_export_file_path);
    if(!dest_file.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug(" File Opened -----  Failed ----" + dest_file.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error -" + dest_file.errorString() ,Q_FUNC_INFO);
        return ;
    }

    dest_file.write(text_to_be_written_str.toLocal8Bit());
    dest_file.flush();
    dest_file.close();

    open_exported_file(saved_export_file_path);

    return ;

}

QStringList ram_analysis::get_profile_list(QString os_schm_dsply)
{
    QStringList profiles_list ;

    if(os_schm_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display)
    {

        profiles_list  << "MacMojave_10_14_18A391x64"

                       << "MacHighSierra_10_13_17A264cx64"
                       << "MacHighSierra_10_13_17A291jx64"
                       << "MacHighSierra_10_13_17A306fx64"
                       << "MacHighSierra_10_13_17A315ix64"
                       << "MacHighSierra_10_13_17A315ix64"
                       << "MacHighSierra_10_13_17A344bx64"
                       << "MacHighSierra_10_13_17A352ax64"
                       << "MacHighSierra_10_13_17A358ax64"
                       << "MacHighSierra_10_13_17A360a_17A362ax64"
                       << "MacHighSierra_10_13_1_17B25cx64"
                       << "MacHighSierra_10_13_1_17B35ax64"
                       << "MacHighSierra_10_13_1_17B46a_17B42a_17B45ax64"
                       << "MacHighSierra_10_13_1_17B48x64"
                       << "MacHighSierra_10_13_2_17C88x64"
                       << "MacHighSierra_10_13_2_Seed_17C60cx64"
                       << "MacHighSierra_10_13_3_17D47x64"
                       << "MacHighSierra_10_13_4_17E150gx64"
                       << "MacHighSierra_10_13_4_17E160ex64"
                       << "MacHighSierra_10_13_4_17E170cx64"
                       << "MacHighSierra_10_13_4_17E182ax64"
                       << "MacHighSierra_10_13_4_17E202x64"
                       << "MacHighSierra_10_13_5_17F45cx64"
                       << "MacHighSierra_10_13_5_17F59bx64"
                       << "MacHighSierra_10_13_5_17F66ax64"
                       << "MacHighSierra_10_13_6_17G65x64"
                       << "MacHighSierra_10_13_6_17G2208x64"


                       << "MacSierra_10_12_16A323x64"
                       << "MacSierra_10_12_1_16B2327ex64"
                       << "MacSierra_10_12_1_16B2338cx64"
                       << "MacSierra_10_12_1_16B2657x64"
                       << "MacSierra_10_12_2_16C48bx64"
                       << "MacSierra_10_12_2_16C63ax64"
                       << "MacSierra_10_12_2_16C67x64"
                       << "MacSierra_10_12_3_16D30a_16D32x64"
                       << "MacSierra_10_12_4_16E144fx64"
                       << "MacSierra_10_12_4_16E163fx64"
                       << "MacSierra_10_12_4_16E183bx64"
                       << "MacSierra_10_12_4_16E195x64"
                       << "MacSierra_10_12_5_16F73x64"
                       << "MacSierra_10_12_6_16G1036x64"
                       << "MacSierra_10_12_6_16G1114x64"
                       << "MacSierra_10_12_6_16G1212x64"
                       << "MacSierra_10_12_6_16G1314x64"
                       << "MacSierra_10_12_6_16G18ax64"
                       << "MacSierra_10_12_6_16G23ax64"
                       << "MacSierra_10_12_6_16G29x64"

                       << "MacElCapitan_10_11_15A284x64"
                       << "MacElCapitan_10_11_1_15B42x64"
                       << "MacElCapitan_10_11_2_15C50x64"
                       << "MacElCapitan_10_11_3_15D21_15D13bx64"
                       << "MacElCapitan_10_11_4_15E27ex64"
                       << "MacElCapitan_10_11_4_15E39dx64"
                       << "MacElCapitan_10_11_4_15E49ax64"
                       << "MacElCapitan_10_11_4_15E65x64"
                       << "MacElCapitan_10_11_5_15F18b_15F24bx64"
                       << "MacElCapitan_10_11_5_15F34x64"
                       << "MacElCapitan_10_11_6_15G1004_15G1108x64"
                       << "MacElCapitan_10_11_6_15G1212x64"
                       << "MacElCapitan_10_11_6_15G1217x64"
                       << "MacElCapitan_10_11_6_15G12ax64"
                       << "MacElCapitan_10_11_6_15G1421x64"
                       << "MacElCapitan_10_11_6_15G1510x64"
                       << "MacElCapitan_10_11_6_15G1611x64"
                       << "MacElCapitan_10_11_6_15G17023x64"
                       << "MacElCapitan_10_11_6_15G18013x64"
                       << "MacElCapitan_10_11_6_15G19009x64"
                       << "MacElCapitan_10_11_6_15G19ax64"
                       << "MacElCapitan_10_11_6_15G20015x64"
                       << "MacElCapitan_10_11_6_15G24b_15G31x64"
                       << "MacElCapitan_10_11_6_15G7ax64"

                       << "MacYosemite_10_10_1_14b25_14a389x64"
                       << "MacYosemite_10_10_2_14C1514x64"
                       << "MacYosemite_10_10_3_14D131_14D136x64"
                       << "MacYosemite_10_10_4_14E46x64"
                       << "MacYosemite_10_10_5_14F1021_14f1509x64"
                       << "MacYosemite_10_10_5_14F1912x64"
                       << "MacYosemite_10_10_5_14F2009_14F2109x64"
                       << "MacYosemite_10_10_5_14F2315x64"
                       << "MacYosemite_10_10_5_14F2411x64"
                       << "MacYosemite_10_10_5_14F2511x64"
                       << "MacYosemite_10_10_5_14F27x64"


                       << "MacMavericks_10_9_1_13b42_13a603x64"
                       << "MacMavericks_10_9_2_13C1021_AMDx64"
                       << "MacMavericks_10_9_5_13F1911_AMDx64"


                       << "MacMountainLion_10_8_1_AMDx64"
                       << "MacMountainLion_10_8_2_12c54_12c60x64"
                       << "MacMountainLion_10_8_3_AMDx64"
                       << "MacMountainLion_10_8_4_12e55_AMDx64"
                       << "MacMountainLion_10_8_5_12F2518_AMDx64"
                       << "MacMountainLion_10_8_5_12f37_AMDx64"
                       << "MacMountainLion_10_8_5_12f45_AMDx64"

                       << "MacLion_10_7_1_AMDx64"
                       << "MacLion_10_7_1_Intelx86"
                       << "MacLion_10_7_2_AMDx64"
                       << "MacLion_10_7_2_Intelx86"
                       << "MacLion_10_7_3_AMDx64"
                       << "MacLion_10_7_3_Intelx86"
                       << "MacLion_10_7_4_AMDx64"
                       << "MacLion_10_7_4_Intelx86"
                       << "MacLion_10_7_5_AMDx64"
                       << "MacLion_10_7_5_Intelx86"
                       << "MacLion_10_7_AMDx64"
                       << "MacLion_10_7_Intelx86"

                       << "MacSnowLeopard_10_6_10_6_1_AMDx64"
                       << "MacSnowLeopard_10_6_10_6_1_Intelx86"
                       << "MacSnowLeopard_10_6_2_AMDx64"
                       << "MacSnowLeopard_10_6_2_Intelx86"
                       << "MacSnowLeopard_10_6_3_Intelx86"
                       << "MacSnowLeopard_10_6_4_AMDx64"
                       << "MacSnowLeopard_10_6_4_Intelx86"
                       << "MacSnowLeopard_10_6_5_AMDx64"
                       << "MacSnowLeopard_10_6_5_Intelx86"
                       << "MacSnowLeopard_10_6_6_AMDx64"
                       << "MacSnowLeopard_10_6_6_Intelx86"
                       << "MacSnowLeopard_10_6_7_AMDx64"
                       << "MacSnowLeopard_10_6_7_Intelx86"
                       << "MacSnowLeopard_10_6_8_AMDx64"
                       << "MacSnowLeopard_10_6_8_Intelx86"

                       << "MacLeopard_10_5_3_Intelx86"
                       << "MacLeopard_10_5_4_Intelx86"
                       << "MacLeopard_10_5_5_Intelx86"
                       << "MacLeopard_10_5_6_Intelx86"
                       << "MacLeopard_10_5_7_Intelx86"
                       << "MacLeopard_10_5_8_Intelx86"
                       << "MacLeopard_10_5_Intelx86";
    }
    else if(os_schm_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display)
    {
        profiles_list        << "Win10x64"
                             << "Win10x64_10586"
                             << "Win10x64_14393"
                             << "Win10x86"
                             << "Win10x86_10586"
                             << "Win10x86_14393"
                             << "VistaSP0x64"

                             << "Win81U1x64"
                             << "Win81U1x86"
                             << "Win8SP0x64"
                             << "Win8SP0x86"
                             << "Win8SP1x64"
                             << "Win8SP1x64_18340"
                             << "Win8SP1x86"

                             << "Win7SP0x64"
                             << "Win7SP0x86"
                             << "Win7SP1x64"
                             << "Win7SP1x64_23418"
                             << "Win7SP1x86"
                             << "Win7SP1x86_23418"

                             << "VistaSP0x86"
                             << "VistaSP1x64"
                             << "VistaSP1x86"
                             << "VistaSP2x64"
                             << "VistaSP2x86"

                             << "Win2003SP0x86"
                             << "Win2003SP1x64"
                             << "Win2003SP1x86"
                             << "Win2003SP2x64"
                             << "Win2003SP2x86"
                             << "Win2008R2SP0x64"
                             << "Win2008R2SP1x64"
                             << "Win2008R2SP1x64_23418"
                             << "Win2008SP1x64"
                             << "Win2008SP1x86"
                             << "Win2008SP2x64"
                             << "Win2008SP2x86"
                             << "Win2012R2x64"
                             << "Win2012R2x64_18340"
                             << "Win2012x64"
                             << "Win2016x64_14393"


                             << "WinXPSP1x64"
                             << "WinXPSP2x64"
                             << "WinXPSP2x86"
                             << "WinXPSP3x86";

    }

    return profiles_list ;
}

QMap<QString, QString> ram_analysis::get_plugin_map(QString os_schm_dsply)
{
    QMap<QString , QString > m_map;

    if(os_schm_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display)
    {
        m_map.insert("mac_adium" ,  "Adium messages");
        m_map.insert("mac_arp  " ,  "ARP table");
        m_map.insert("mac_bash " ,  "Bash history");
        m_map.insert("mac_bash_env" ,  "Bash's environment variables");
        m_map.insert("mac_bash_hash" ,  "Bash hash table");
        m_map.insert("mac_calendar" ,  "Calendar events");
        m_map.insert("mac_check_fop" ,  "File Operation Pointers");
        m_map.insert("mac_check_mig_table " ,  "Kernel's MIG table");
        m_map.insert("mac_check_syscalls" ,  "System call hooks");
        m_map.insert("mac_check_sysctl  " ,  "Unknown sysctl handlers");
        m_map.insert("mac_check_trap_table" ,  "MACH trap table entries");
        m_map.insert("mac_compressed_swap " ,  "Mac OS X VM pages");
        m_map.insert("mac_contacts" ,  "Contact names");
        m_map.insert("mac_dead_procs    " ,  "Terminated/de-allocated processes");
        m_map.insert("mac_dead_sockets  " ,  "Terminated/de-allocated network sockets");
        m_map.insert("mac_dead_vnodes   " ,  "Freed vnode");
        m_map.insert("mac_devfs" ,  "File cache");
        m_map.insert("mac_dmesg" ,  "Kernel debug messages");
        m_map.insert("mac_get_profile   " ,  "Mac profiles");
        m_map.insert("mac_ifconfig" ,  "Network interface information");
        m_map.insert("mac_interest_handlers" ,  "IOKit Interest Handlers");
        m_map.insert("mac_ip_filters    " ,  "Hooked IP filters");
        m_map.insert("mac_kernel_classes" ,  "Loaded c++ classes in the kernel");
        m_map.insert("mac_kevents " ,  "Show parent/child relationship of processes");
        m_map.insert("mac_keychaindump  " ,  "Recovers possbile keychain keys");
        m_map.insert("mac_list_files    " ,  "Lists files in the file cache");
        m_map.insert("mac_list_kauth_listeners" ,  "Lists Kauth Scope listeners");
        m_map.insert("mac_list_kauth_scopes" ,  "Lists Kauth Scopes and their status");
        m_map.insert("mac_list_raw" ,  "List applications with promiscuous sockets");
        m_map.insert("mac_list_sessions " ,  "Enumerates sessions");
        m_map.insert("mac_list_zones    " ,  "Prints active zones");
        m_map.insert("mac_lsmod" ,  "Lists loaded kernel modules");
        m_map.insert("mac_lsmod_iokit   " ,  "Lists loaded kernel modules through IOkit");
        m_map.insert("mac_lsmod_kext_map" ,  "Lists loaded kernel modules");
        m_map.insert("mac_lsof " ,  "Lists per-process opened files");
        m_map.insert("mac_machine_info  " ,  "Prints machine information about the sample");
        m_map.insert("mac_malfind " ,  "Looks for suspicious process mappings");
        m_map.insert("mac_memdump " ,  "Dump addressable memory pages to a file");
        m_map.insert("mac_moddump " ,  "Writes the specified kernel extension to disk");
        m_map.insert("mac_mount" ,  "Prints mounted device information");
        m_map.insert("mac_netstat " ,  "Lists active per-process network connections");
        m_map.insert("mac_network_conns " ,  "Lists network connections from kernel network structures");
        m_map.insert("mac_notesapp" ,  "Finds contents of Notes messages");
        m_map.insert("mac_notifiers" ,  "Detects rootkits that add hooks into I/O Kit (e.g. LogKext)");
        m_map.insert("mac_orphan_threads" ,  "Lists threads that don't map back to known modules/processes");
        m_map.insert("mac_pgrp_hash_table " ,  "Walks the process group hash table");
        m_map.insert("mac_pid_hash_table" ,  "Walks the pid hash table");
        m_map.insert("mac_print_boot_cmdline  " ,  "Prints kernel boot arguments");
        m_map.insert("mac_proc_maps" ,  "Gets memory maps of processes");
        m_map.insert("mac_procdump" ,  "Dumps the executable of a process");
        m_map.insert("mac_psaux" ,  "Prints processes with arguments in user land (**argv)");
        m_map.insert("mac_psenv" ,  "Prints processes with environment in user land (**envp)");
        m_map.insert("mac_pslist  " ,  "List Running Processes");
        m_map.insert("mac_pstree  " ,  "Show parent/child relationship of processes");
        m_map.insert("mac_psxview " ,  "Find hidden processes with various process listings");
        m_map.insert("mac_recover_filesystem  " ,  "Recover the cached filesystem");
        m_map.insert("mac_route" ,  "Prints the routing table");
        m_map.insert("mac_socket_filters" ,  "Reports socket filters");
        m_map.insert("mac_tasks" ,  "List Active Tasks");
        m_map.insert("mac_threads " ,  "List Process Threads");
        m_map.insert("mac_threads_simple" ,  "Lists threads along with their start time and priority");
        m_map.insert("mac_version " ,  "Prints the Mac version");
        m_map.insert("mac_vfsevents" ,  "Lists processes filtering file system events");
    }

    return m_map;
}

void ram_analysis::on_pushButton_refresh_source_clicked()
{

    while(ui->comboBox_source->count() > 0)
        ui->comboBox_source->removeItem(0);


    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);
        if(obj.source_type_internal != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal)
            continue;

        QString source_path = recon_static_functions::get_complete_file_path(obj.virtual_source_path, obj.source_name, Q_FUNC_INFO);

        QFileInfo minfo(source_path);
        if(minfo.exists())
            ui->comboBox_source->addItem(minfo.fileName());
    }


    if(ui->comboBox_source->currentText().trimmed().isEmpty())
    {
        ui->pushButton_execute_command->setDisabled(true);
        ui->pushButton_export->setDisabled(true);
        ui->pushButton_save->setDisabled(true);
    }
    else
    {
        ui->pushButton_execute_command->setDisabled(false);
        ui->pushButton_export->setDisabled(false);
        ui->pushButton_save->setDisabled(false);
    }

}

void ram_analysis::create_carved_password_dbs(QString source_count_name_str , QString source_name_str)
{
    source_name_str.replace(".", "_");
    carved_password_result_name = source_name_str.replace(" ","_") + "_" + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss"));
    QString db_path_dir = source_count_name_str + "/" + carved_password_result_name;

    QString carved_password_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString).toString();
    QString carved_password_path = carved_password_dir_path + db_path_dir + "/";
    QDir dir;
    dir.mkpath(carved_password_path);


    QString db_name = "carved_password.sqlite";
    carved_password_db_path = carved_password_path + db_name;
    QString command = "create table carved_password"
                      "(INT INTEGER PRIMARY KEY,"
                      "bookmark INTEGER,"
                      "plugin_name varchar(30),"
                      "tab_name varchar(30),"
                      "m_password varchar(30),"
                      "m_server varchar(30),"
                      "m_account varchar(30),"
                      "m_description INTEGER,"
                      "item1 varchar(30),"
                      "item2 varchar(30),"
                      "item3 varchar(300),"
                      "item4 varchar(300),"
                      "item5 varchar(300),"
                      "item6 varchar(300),"
                      "item7 varchar(300),"
                      "item8 varchar(300),"
                      "item9 varchar(300),"
                      "item10 varchar(300),"
                      "os_scheme_display varchar(300),"
                      "recon_tag_value varchar(30),"
                      "notes varchar(100),"
                      "notes_icon varchar(30),"
                      "blank_column varchar(100),"
                      "source_file varchar(100),"
                      "source_count_name varchar(100))";
    recon_helper_standard_obj->execute_db_command_by_dbpath(command,carved_password_db_path,Q_FUNC_INFO);




    ////make entry in index db
    QString index_db_path = carved_password_dir_path + "carved_password_index.sqlite";
    QString relative_db_path = "/" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Lab_Features) + "Carved_Password/" + db_path_dir + "/" + db_name;
    QString command_2 = QString("INSERT INTO tbl_carved_password_index"
                                "(label_name , relative_db_path ,source_count_name)"
                                "VALUES(?,?,?)");
    QStringList arg_value_list;
    arg_value_list << carved_password_result_name << relative_db_path << source_count_name_str;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_2,arg_value_list,index_db_path,Q_FUNC_INFO);


}

void ram_analysis::slot_hide_loading_display_dialog_box(bool status)
{
    bool_cancel_loading = true;
}


void ram_analysis::open_exported_file(QString file_path)
{
    report_status obj;
    if(obj.exec())
    {
        QStringList arg;
        arg << file_path;

        QProcess *myProcess = new QProcess(this);
        myProcess->startDetached("open",arg);
    }

}
