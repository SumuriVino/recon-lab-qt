#include "recon_configuration_page_preferences.h"
#include "ui_recon_configuration_page_preferences.h"

recon_configuration_page_preferences::recon_configuration_page_preferences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration_page_preferences)
{
    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    QRegularExpression rx("^[0-9]*$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);

    ui->lineEdit_minimum_file_size_skin_tone_detection->setValidator(validator);
    ui->lineEdit_minimum_file_size_skin_tone_detection->setAttribute(Qt::WA_MacShowFocusRect,false);

    ui->lineEdit_minimum_file_size_face_analysis->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_minimum_file_size_face_analysis->setValidator(validator);

    ui->lineEdit_minimum_file_size_optical_character_recognition->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_minimum_file_size_optical_character_recognition->setValidator(validator);

    ui->lineEdit_maximum_size_text_view->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_maximum_size_text_view->setValidator(validator);



}

recon_configuration_page_preferences::~recon_configuration_page_preferences()
{
    delete ui;
}

void recon_configuration_page_preferences::pub_set_caller_type(QString caller_type)
{
    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG)
    {
        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");
   }
    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    {
        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + QString("case_configuration/case_configuration.sqlite");
    }
    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER)
        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString("case_configuration.sqlite");

    display_data();
}

void recon_configuration_page_preferences::update_databases(QString m_key , QString m_value)
{

    QString select_cmd = QString("SELECT key from tbl_preferences");
    QStringList key_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(select_cmd,0,destination_db_file,Q_FUNC_INFO);

    if(key_list.contains(m_key))
    {
        QString update_command = "UPDATE tbl_preferences SET value = ? WHERE key = ?";
        QStringList args_list;
        args_list << m_value << m_key;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_command ,args_list ,destination_db_file ,Q_FUNC_INFO);
    }
    else
    {
        QString insert_command = "INSERT into tbl_preferences (key,value) values(?,?)";
        QStringList args_list;
        args_list << m_key << m_value;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_command ,args_list ,destination_db_file ,Q_FUNC_INFO);

    }

    update_narad_variables();
}


void recon_configuration_page_preferences::display_data()
{
    QString command = "select value from tbl_preferences Where key = ?";

    QString m_user_opened_file_str =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,QStringList(MACRO_Result_Preferences_Key_Highlight_User_Opened_Files),0,destination_db_file, Q_FUNC_INFO);
    if(m_user_opened_file_str == QString(MACRO_TRUE_VALUE_READABLE_QString))
    {
        ui->checkBox_highligh_user_opened_files->setCheckState(Qt::Checked);
    }
    else
    {
        ui->checkBox_highligh_user_opened_files->setCheckState(Qt::Unchecked);
    }

    QString m_min_file_size_ndty_detcn =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,QStringList(MACRO_Result_Preferences_Key_Minimum_File_Size_Skin_Tone_Detection_in_KB),0,destination_db_file, Q_FUNC_INFO).trimmed();
    ui->lineEdit_minimum_file_size_skin_tone_detection->setPlaceholderText(m_min_file_size_ndty_detcn);

    QString m_min_file_size_face_analysis =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,QStringList(MACRO_Result_Preferences_Key_Minimum_File_Size_Face_Analysis_in_KB),0,destination_db_file, Q_FUNC_INFO).trimmed();
    ui->lineEdit_minimum_file_size_face_analysis->setPlaceholderText(m_min_file_size_face_analysis);

    QString m_min_file_size_ocr =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,QStringList(MACRO_Result_Preferences_Key_Minimum_File_Size_Optical_Character_Recognition_in_KB),0,destination_db_file, Q_FUNC_INFO).trimmed();
    ui->lineEdit_minimum_file_size_optical_character_recognition->setPlaceholderText(m_min_file_size_ocr);

    QString m_min_file_size_weapons =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,QStringList(MACRO_Result_Preferences_Key_Minimum_File_Size_Weapons_in_KB),0,destination_db_file, Q_FUNC_INFO).trimmed();
    ui->lineEdit_minimum_file_size_weapons->setPlaceholderText(m_min_file_size_weapons);

    QString m_min_file_size_fire_analysis =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,QStringList(MACRO_Result_Preferences_Key_Minimum_File_Size_Fire_Analysis_in_KB),0,destination_db_file, Q_FUNC_INFO).trimmed();
    ui->lineEdit_minimum_file_size_fire_analysis->setPlaceholderText(m_min_file_size_fire_analysis);

    QString m_max_file_size_text_view =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,QStringList(MACRO_Result_Preferences_Key_Maximum_File_Size_Text_View_in_MB),0,destination_db_file, Q_FUNC_INFO).trimmed();
    ui->lineEdit_maximum_size_text_view->setPlaceholderText(m_max_file_size_text_view);

    update_narad_variables();

}



void recon_configuration_page_preferences::update_narad_variables()
{

    bool bool_user_opened_files_checked = false;
    if(ui->checkBox_highligh_user_opened_files->isChecked())
        bool_user_opened_files_checked = true;

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Preferences_User_Opened_Files_bool, bool_user_opened_files_checked);

    QString m_size_nd = ui->lineEdit_minimum_file_size_skin_tone_detection->text();
    if(m_size_nd.isEmpty())
        m_size_nd = ui->lineEdit_minimum_file_size_skin_tone_detection->placeholderText();

    qint64 size_kb_nd = m_size_nd.toLongLong();
    qint64 size_bytes_nd = recon_static_functions::convert_human_readable_size_to_bytes(size_kb_nd, MACRO_Generic_Filter_Condition_KB, Q_FUNC_INFO).toLongLong();
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Prefrences_Skin_Tone_Detection_Minimum_File_Size_Bytes_qint64, size_bytes_nd);


    QString m_size_face = ui->lineEdit_minimum_file_size_face_analysis->text();
    if(m_size_face.isEmpty())
        m_size_face = ui->lineEdit_minimum_file_size_face_analysis->placeholderText();

    qint64 size_kb_face = m_size_face.toLongLong();
    qint64 size_bytes_face = recon_static_functions::convert_human_readable_size_to_bytes(size_kb_face, MACRO_Generic_Filter_Condition_KB, Q_FUNC_INFO).toLongLong();
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Prefrences_Face_Analysis_Minimum_File_Size_Bytes_qint64, size_bytes_face);


    QString m_size_ocr = ui->lineEdit_minimum_file_size_optical_character_recognition->text();
    if(m_size_ocr.isEmpty())
        m_size_ocr = ui->lineEdit_minimum_file_size_optical_character_recognition->placeholderText();

    qint64 size_kb_ocr = m_size_ocr.toLongLong();
    qint64 size_bytes_ocr = recon_static_functions::convert_human_readable_size_to_bytes(size_kb_ocr, MACRO_Generic_Filter_Condition_KB, Q_FUNC_INFO).toLongLong();
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Prefrences_Optical_Character_Recognition_Minimum_File_Size_Bytes_qint64, size_bytes_ocr);

    QString m_size_weapons = ui->lineEdit_minimum_file_size_weapons->text();
    if(m_size_weapons.isEmpty())
        m_size_weapons = ui->lineEdit_minimum_file_size_weapons->placeholderText();

    qint64 size_kb_weapons = m_size_weapons.toLongLong();
    qint64 size_bytes_weapons = recon_static_functions::convert_human_readable_size_to_bytes(size_kb_weapons, MACRO_Generic_Filter_Condition_KB, Q_FUNC_INFO).toLongLong();
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Prefrences_Weapons_Minimum_File_Size_Bytes_qint64, size_bytes_weapons);

    QString m_size_fire = ui->lineEdit_minimum_file_size_fire_analysis->text();
    if(m_size_fire.isEmpty())
        m_size_fire = ui->lineEdit_minimum_file_size_fire_analysis->placeholderText();

    qint64 size_kb_fire = m_size_fire.toLongLong();
    qint64 size_bytes_fire = recon_static_functions::convert_human_readable_size_to_bytes(size_kb_fire, MACRO_Generic_Filter_Condition_KB, Q_FUNC_INFO).toLongLong();
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Prefrences_Fire_Analysis_Minimum_File_Size_Bytes_qint64, size_bytes_fire);

    QString m_size_text_view = ui->lineEdit_maximum_size_text_view->text();
    if(m_size_text_view.isEmpty())
        m_size_text_view = ui->lineEdit_maximum_size_text_view->placeholderText();

    qint64 size_mb_text_view = m_size_text_view.toLongLong();
    qint64 size_bytes_text_view = recon_static_functions::convert_human_readable_size_to_bytes(size_mb_text_view, MACRO_Generic_Filter_Condition_MB, Q_FUNC_INFO).toLongLong();
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Prefrences_Text_View_Maximum_File_Size_Bytes_qint64, size_bytes_text_view);

}

void recon_configuration_page_preferences::on_checkBox_highligh_user_opened_files_clicked(bool checked)
{
    if(checked)
    {
        update_databases(MACRO_Result_Preferences_Key_Highlight_User_Opened_Files , QString(MACRO_TRUE_VALUE_READABLE_QString));
    }
    else
    {
        update_databases(MACRO_Result_Preferences_Key_Highlight_User_Opened_Files , QString(MACRO_FALSE_VALUE_READABLE_QString));
    }

}

void recon_configuration_page_preferences::on_lineEdit_minimum_file_size_skin_tone_detection_returnPressed()
{
    QString m_size = ui->lineEdit_minimum_file_size_skin_tone_detection->text();

    update_databases(MACRO_Result_Preferences_Key_Minimum_File_Size_Skin_Tone_Detection_in_KB ,m_size);

    ui->lineEdit_minimum_file_size_skin_tone_detection->clear();
    ui->lineEdit_minimum_file_size_skin_tone_detection->setPlaceholderText(m_size);

}

void recon_configuration_page_preferences::on_lineEdit_minimum_file_size_face_analysis_returnPressed()
{
    QString m_size = ui->lineEdit_minimum_file_size_face_analysis->text();

    update_databases(MACRO_Result_Preferences_Key_Minimum_File_Size_Face_Analysis_in_KB ,m_size);

    ui->lineEdit_minimum_file_size_face_analysis->clear();
    ui->lineEdit_minimum_file_size_face_analysis->setPlaceholderText(m_size);

}

void recon_configuration_page_preferences::on_lineEdit_minimum_file_size_optical_character_recognition_returnPressed()
{
    QString m_size = ui->lineEdit_minimum_file_size_optical_character_recognition->text();

    update_databases(MACRO_Result_Preferences_Key_Minimum_File_Size_Optical_Character_Recognition_in_KB ,m_size);

    ui->lineEdit_minimum_file_size_optical_character_recognition->clear();
    ui->lineEdit_minimum_file_size_optical_character_recognition->setPlaceholderText(m_size);

}

void recon_configuration_page_preferences::on_lineEdit_maximum_size_text_view_returnPressed()
{
    QString m_size = ui->lineEdit_maximum_size_text_view->text();

    update_databases(MACRO_Result_Preferences_Key_Maximum_File_Size_Text_View_in_MB ,m_size);

    ui->lineEdit_maximum_size_text_view->clear();
    ui->lineEdit_maximum_size_text_view->setPlaceholderText(m_size);

}

void recon_configuration_page_preferences::on_lineEdit_minimum_file_size_weapons_returnPressed()
{
    QString m_size = ui->lineEdit_minimum_file_size_weapons->text();

    update_databases(MACRO_Result_Preferences_Key_Minimum_File_Size_Weapons_in_KB ,m_size);

    ui->lineEdit_minimum_file_size_weapons->clear();
    ui->lineEdit_minimum_file_size_weapons->setPlaceholderText(m_size);

}

void recon_configuration_page_preferences::on_lineEdit_minimum_file_size_fire_analysis_returnPressed()
{
    QString m_size = ui->lineEdit_minimum_file_size_fire_analysis->text();

    update_databases(MACRO_Result_Preferences_Key_Minimum_File_Size_Fire_Analysis_in_KB ,m_size);

    ui->lineEdit_minimum_file_size_fire_analysis->clear();
    ui->lineEdit_minimum_file_size_fire_analysis->setPlaceholderText(m_size);

}
