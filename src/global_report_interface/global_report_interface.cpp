#include "global_report_interface.h"
#include "ui_global_report_interface.h"

global_report_interface::global_report_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::global_report_interface)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    bool_export_available = false;

    bool_pushbutton_logo_enable = false;

    //--REPORT LAUNCHER.
    report_launcher_object = new report_launcher;
    connect(report_launcher_object,SIGNAL(signal_all_report_selection_completed()),this,SLOT(slot_all_report_selection_completed()));
    connect(report_launcher_object,SIGNAL(signal_set_window_title(QString)),this,SLOT(slot_set_window_title(QString)));

    QVBoxLayout *layout_view = new QVBoxLayout;
    layout_view->addWidget(report_launcher_object);
    layout_view->setContentsMargins(4,4,4,4);
    layout_view->setSizeConstraint(QLayout::SetFixedSize);
    this->setLayout(layout_view);

    setWindowTitle("Global Report - Case informaion");

    setWindowModality(Qt::ApplicationModal);

}

global_report_interface::~global_report_interface()
{
    delete ui;
}
void global_report_interface::set_initialisation()
{
    report_launcher_object->set_narad_muni_for_report(&narad_muni_for_report_obj);
    report_launcher_object->pub_initialise_launcher();

}

void global_report_interface::set_narad_muni_for_report(narad_muni *obj)
{
    narad_muni_for_report_obj = *obj;
}

void global_report_interface::slot_all_report_selection_completed()
{

    report_name = narad_muni_for_report_obj.get_field(MACRO_NARAD_Global_Report_Name_QString).toString();
    this->hide();

    report_generate_clicked();
}

void global_report_interface::slot_set_window_title(QString title)
{
    setWindowTitle(title);
}

void global_report_interface::report_generate_clicked()
{

    report_type_list.clear();

    if(narad_muni_for_report_obj.get_field(MACRO_NARAD_Global_Report_Advance_Html_QString).toString() == "1")
        report_type_list << (MACRO_REPORT_TYPE_HTML_ADVANCE_QString);

    if(narad_muni_for_report_obj.get_field(MACRO_NARAD_Global_Report_Standard_Html_QString).toString() == "1")
        report_type_list << (MACRO_REPORT_TYPE_HTML_STANDARD_QString);

    if(narad_muni_for_report_obj.get_field(MACRO_NARAD_Global_Report_CSV_QString).toString() == "1")
        report_type_list << (MACRO_REPORT_TYPE_CSV_QString);

    if(narad_muni_for_report_obj.get_field(MACRO_NARAD_Global_Report_PDF_QString).toString() == "1")
        report_type_list << (MACRO_REPORT_TYPE_PDF_QString);

    if(narad_muni_for_report_obj.get_field(MACRO_NARAD_Global_Report_XML_QString).toString() == "1")
        report_type_list << (MACRO_REPORT_TYPE_XML_QString);


    if(narad_muni_for_report_obj.get_field(MACRO_NARAD_Global_Report_Scope_Full_QString).toString() == "Full")
        repot_scope = (MACRO_REPORT_SCOPE_FULL_QString);
    else if(narad_muni_for_report_obj.get_field(MACRO_NARAD_Global_Report_Scope_Tags_QString).toString() == "Tags")
        repot_scope = (MACRO_REPORT_SCOPE_TAGS_QString);


    selected_tags_list = narad_muni_for_report_obj.get_field(MACRO_NARAD_Global_Report_Selected_Tags_QStringList).toStringList();

    report_encryption_password = narad_muni_for_report_obj.get_field(MACRO_NARAD_Global_Report_Password_QString).toString();



    create_global_report_directory_and_path();



    QStringList report_plugin_info_list = narad_muni_for_report_obj.get_field(MACRO_NARAD_Global_Report_Selected_Plugins_With_Tabs_AND_Export_Status_QStringList).toStringList();



    //======================================== Start =========================================//
    recon_static_functions::app_debug(" updating narad muni and passing to report class START", Q_FUNC_INFO);
    narad_muni_for_report_obj.set_field(MACRO_NARAD_Report_Tabwidget_CurrentIndex_int,-1);
    narad_muni_for_report_obj.set_field(MACRO_NARAD_Report_Checkbox_Export_bool,false);

    QList<QVariant> talwidget_list;
    narad_muni_for_report_obj.set_field(MACRO_NARAD_Report_Screen_Items_m_tablewidget_QList,talwidget_list);
    recon_static_functions::app_debug(" updating narad muni and passing to report class END", Q_FUNC_INFO);
    //======================================== End =========================================//


    recon_static_functions::app_debug(" Global Report report_name " + report_name, Q_FUNC_INFO);
    recon_static_functions::app_debug(" Global Report repot_scope " + repot_scope, Q_FUNC_INFO);
    recon_static_functions::app_debug(" Global Report report_type_list " + report_type_list.join("----"), Q_FUNC_INFO);
    recon_static_functions::app_debug(" Global Report selected_tags_list " + selected_tags_list.join("----"), Q_FUNC_INFO);
    recon_static_functions::app_debug(" Global Report report_file_path_list " + report_file_path_list.join("----"), Q_FUNC_INFO);
    recon_static_functions::app_debug(" Global Report report_plugin_info_list " + report_plugin_info_list.join("----"), Q_FUNC_INFO);

    emit signal_global_report_plugin_selection_workdone(&narad_muni_for_report_obj,report_plugin_info_list,report_name,report_file_path_list, report_type_list, repot_scope, selected_tags_list);
}


void global_report_interface::create_global_report_directory_and_path()
{
    report_agency_logo_path.clear();

    report_file_path_list.clear();

    QDir dir;
    QString agency_logo_dir = narad_muni_for_report_obj.get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + "icons/report_agency_logo";
    dir.mkpath(agency_logo_dir);
    report_agency_logo_path = agency_logo_dir +"/" + report_name + ".png";

    QString report_dir = narad_muni_for_report_obj.get_field(MACRO_NARAD_Report_Location_Global_Report_In_Result_QString).toString();
    QString file_name = narad_muni_for_report_obj.get_field(MACRO_NARAD_Global_Report_Name_QString).toString();

    for(int i = 0; i < report_type_list.size(); i++)
    {
        QString report_type_value = report_type_list.at(i);

        QString mm_type = report_type_value;
        QString mm_scope = repot_scope;
        QString report_currently_dir_name = "RECON_" + mm_type.replace(" ","_") +"_" + mm_scope.replace(" ","_") + "_" + QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss");
        QString reports_dir_path = report_dir + report_currently_dir_name + "/";
        dir.mkpath(reports_dir_path);
        create_resource_and_files_dir(reports_dir_path, report_type_value);


        if(report_type_value == MACRO_REPORT_TYPE_HTML_ADVANCE_QString)
        {
            QDir dir;
            QString plugins_dir = reports_dir_path + "Plugins";
            dir.mkpath(plugins_dir);

            report_file_path_list << reports_dir_path + "index.html";
        }
        else if(report_type_value == MACRO_REPORT_TYPE_HTML_STANDARD_QString)
        {
            report_file_path_list << QString(reports_dir_path)  + file_name + ".html";
        }
        else if(report_type_value == MACRO_REPORT_TYPE_PDF_QString)
        {
            report_file_path_list << QString(reports_dir_path)  + file_name + ".pdf";
        }
        else if(report_type_value == MACRO_REPORT_TYPE_CSV_QString)
        {
            report_file_path_list << QString(reports_dir_path)  + file_name + ".csv";
        }
        if(report_type_value == MACRO_REPORT_TYPE_XML_QString)
        {
            report_file_path_list << QString(reports_dir_path)  + file_name + ".xml";
        }
        if(report_type_value == MACRO_REPORT_TYPE_KML_QString)
        {
            report_file_path_list << QString(reports_dir_path)  + file_name + ".kml";
        }
    }

}

void global_report_interface::create_resource_and_files_dir(QString parent_dir_path, QString report_type)
{
    QDir dir;
    //    QString exported_files_path = parent_dir_path + "/files";
    //    dir.mkdir(exported_files_path);


    if(report_type == MACRO_REPORT_TYPE_HTML_STANDARD_QString || report_type == MACRO_REPORT_TYPE_HTML_ADVANCE_QString || report_type == MACRO_REPORT_TYPE_PDF_QString)
    {
        QString new_resource_path = parent_dir_path + "/resources/";
        dir.mkpath(new_resource_path);

        QString create_file_dir = parent_dir_path + "/Files/";
        dir.mkpath(create_file_dir);

        QFile file;
        QFile::copy(recon_static_functions::get_appilcation_relative_path() + "/Resources/graphics/html_reports/css/chat_template.css" , new_resource_path + "/chat_template.css");

        file.copy("../Resources/graphics/html_reports/style.css", new_resource_path + "style.css");
        file.copy("../icons/open_file_link.png", new_resource_path + "open_file_link.png");
        file.copy("../icons/person1.png", new_resource_path + "person1.png");
        file.copy("../icons/webicon.png", new_resource_path + "webicon.png");
        file.copy("../icons/appicon.png", new_resource_path + "appicon.png");
        recon_static_functions::copy_directory("../icons/Features/",new_resource_path + "plugins/", Q_FUNC_INFO);
        recon_static_functions::copy_directory("../icons/plugins/",new_resource_path + "plugins/", Q_FUNC_INFO);

        file.copy("../icons/signs/home_green.png", new_resource_path + "home_green.png");
        file.copy("../icons/signs/first_green.png", new_resource_path + "first_green.png");
        file.copy("../icons/signs/last_green.png", new_resource_path + "last_green.png");
        file.copy("../icons/signs/next_green.png", new_resource_path + "next_green.png");
        file.copy("../icons/signs/previous_green.png", new_resource_path + "previous_green.png");
        file.copy("../icons/signs/stop_next.png", new_resource_path + "stop_next.png");
        file.copy("../icons/signs/stop_previous.png", new_resource_path + "stop_previous.png");
        file.copy("../icons/Report_Plugin.png", new_resource_path + "Report_Plugin.png");

        recon_static_functions::copy_directory("../Resources/graphics/html_reports", new_resource_path, Q_FUNC_INFO);

        QFile file1(new_resource_path + "/agency_logo.png");
        file1.remove();
        file1.copy(narad_muni_for_report_obj.get_field(MACRO_NARAD_Case_Agency_Logo_QString).toString(),new_resource_path +"agency_logo.png");
        file1.copy(narad_muni_for_report_obj.get_field(MACRO_NARAD_Case_Agency_Logo_QString).toString(),report_agency_logo_path);

        narad_muni_for_report_obj.set_field(MACRO_NARAD_Case_Agency_Logo_QString,new_resource_path + "/agency_logo.png");

    }
}


