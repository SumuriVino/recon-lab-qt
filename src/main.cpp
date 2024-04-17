#include "z_mainwindow/mainwindow.h"
#include <QApplication>
#include <QFileDialog>

int global_variable_license_validation_value_int = 0;

bool bool_Global_hide_seen_files;
QString global_variable_quick_look_preview_filepath_QString;
bool global_variable_quick_look_preview_close_status_bool;

bool global_variable_debug_mode_status_bool = false;
bool global_variable_debug_encryption_status_bool = false;
bool global_variable_debug_print_on_terminal_bool = false;
bool global_variable_debug_print_in_file_bool = false;

bool global_variable_debug_type_general_bool = false;
bool global_variable_debug_type_conditional_bool = false;
bool global_variable_debug_type_intensive_bool = false;

bool global_variable_macOS_appearance_light_mode_bool = false;
bool global_variable_desktop_silicon_bool = false;

witness_info_manager *global_witness_info_manager_class_obj = new witness_info_manager;
csv_reader *global_csv_reader_class_selected_obj = new csv_reader;
csv_reader *global_csv_reader_class_full_obj = new csv_reader;
narad_muni *global_narad_muni_class_obj = new narad_muni;
recon_helper_singular *global_recon_helper_singular_class_obj = new recon_helper_singular;
language_translator global_lang_tr_obj;
desktop_info *global_desktop_info_class_obj = new desktop_info;


const QString global_variable_private_var_tmp_Dir_Path_Parent_QString = QString("/private/var/tmp/RECON_LAB_") + getenv("USER") + ("/");
const QString global_variable_private_var_tmp_Dir_Path_Case_QString = global_variable_private_var_tmp_Dir_Path_Parent_QString + QString::number(QDateTime::currentSecsSinceEpoch()) + "/";

const QString global_varibale_python_path = "/usr/local/bin/python3";

bool get_debug_mode_status();


int main(int argc, char *argv[])
{
    QFont::insertSubstitution(".Lucida Grande UI", "Lucida Grande");


    global_variable_debug_mode_status_bool = false;
    global_variable_debug_print_on_terminal_bool = false;
    global_variable_debug_print_in_file_bool = false;
    global_variable_debug_type_general_bool = false;
    global_variable_debug_type_conditional_bool = false;
    global_variable_debug_type_intensive_bool = false;


    global_variable_debug_encryption_status_bool = true;


    bool bool_debug_mode_status = get_debug_mode_status();
    if(bool_debug_mode_status)
    {
        global_variable_debug_mode_status_bool = true;
        global_variable_debug_print_in_file_bool = true;
        global_variable_debug_type_general_bool = true;
    }

    //=======================For Forenso Use====================//
    //    global_variable_debug_mode_status_bool = true;
    //    global_variable_debug_encryption_status_bool = false;
    //    global_variable_debug_print_on_terminal_bool = true;
    //    global_variable_debug_type_general_bool = true;

    //    global_variable_debug_print_in_file_bool = false;
    //    global_variable_debug_type_conditional_bool = false;
    //    global_variable_debug_type_intensive_bool = false;

    //=======================For Forenso Use====================//

    recon_static_functions::create_debug_file();


    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    QFont font = a.font();
    font.setFamily("Lucida Grande");
    font.setPointSize(12);
    a.setFont(font);

    QDir dir;


    dir.setCurrent(QApplication::applicationDirPath());

    global_variable_license_validation_value_int = 0;


    MainWindow w;

    if(global_variable_license_validation_value_int == 32)
        return -1;

    if(global_variable_license_validation_value_int == 33)
    {
        QProcess::startDetached("./RECON_LAB");
        return -1;
    }


    return a.exec();
}


bool get_debug_mode_status()
{
    QString usr_lib_path = QString(getenv("HOME")) + QString("/Library/") + recon_static_functions::get_app_name().replace(" ", "_");
    QString recon_lib_vrsn = recon_static_functions::get_recon_library_version().replace("." ,"_");
    QString app_config_db_path = usr_lib_path + "/" + recon_lib_vrsn + QString("/app_configuration/app_configuration.sqlite");

    if(!QFile(app_config_db_path).exists())
        return false;

    QString enable_debug_mode = QString(MACRO_FALSE_VALUE_READABLE_QString);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(app_config_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" app config db opening  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return false;
    }

    QString cmd = QString("SELECT value from tbl_preferences where key = ?");

    QSqlQuery query(destination_db);
    query.prepare(cmd);
    query.addBindValue(MACRO_App_Preferences_Key_Enable_Debug_Mode);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + cmd,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return false;
    }

    if(query.next())
    {
        enable_debug_mode = query.value("value").toString();
    }

    destination_db.close();

    if(enable_debug_mode == MACRO_TRUE_VALUE_READABLE_QString)
    {
        return true;
    }


    return false;

}
