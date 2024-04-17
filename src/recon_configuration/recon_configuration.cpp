#include "recon_configuration.h"
#include "ui_recon_configuration.h"

recon_configuration::recon_configuration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    ui->treeWidget_page_list->setAttribute(Qt::WA_MacShowFocusRect,false);

    recon_static_functions::app_debug(" rcconfiguration.11.aa", Q_FUNC_INFO);

    page_examiner_details_obj = new recon_configuration_page_examiner_details(this);
    recon_static_functions::app_debug(" rcconfiguration.11.bb", Q_FUNC_INFO);
    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_examiner_info,page_examiner_details_obj);
    recon_static_functions::app_debug(" rcconfiguration.11.cc", Q_FUNC_INFO);

    //-5.
    plugin_selector_obj = new plugin_selector(this);
    recon_static_functions::app_debug(" rcconfiguration.11.dd", Q_FUNC_INFO);
    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_plugin_selection,plugin_selector_obj);
    recon_static_functions::app_debug(" rcconfiguration.11.ee", Q_FUNC_INFO);

    file_extension_page_obj = new recon_configuration_page_file_extension(this);
    recon_static_functions::app_debug(" rcconfiguration.11.ff", Q_FUNC_INFO);
    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_file_extension,file_extension_page_obj);
    recon_static_functions::app_debug(" rcconfiguration.11.gg", Q_FUNC_INFO);

    file_signature_widget_obj = new file_signature_widget(this);
    recon_static_functions::app_debug(" rcconfiguration.11.hh", Q_FUNC_INFO);
    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_file_signature,file_signature_widget_obj);
    recon_static_functions::app_debug(" rcconfiguration.11.ii", Q_FUNC_INFO);

    // recon_configuration_page_point_hashset_obj =  new recon_configuration_page_point_hashset(this);
    //    recon_static_functions::app_debug(" rcconfiguration.11.jj", Q_FUNC_INFO);
    //    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_hash_set, recon_configuration_page_point_hashset_obj);
    //    recon_static_functions::app_debug(" rcconfiguration.11.kk", Q_FUNC_INFO);

    keyword_list_page_obj = new recon_configuration_page_keyword_list(this);
    recon_static_functions::app_debug(" rcconfiguration.11.ll", Q_FUNC_INFO);
    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_content_search,keyword_list_page_obj);
    recon_static_functions::app_debug(" rcconfiguration.11.mm", Q_FUNC_INFO);

    index_files_page_obj = new recon_configuration_page_index_files(this);
    recon_static_functions::app_debug(" rcconfiguration.11.nn", Q_FUNC_INFO);
    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_index_files,index_files_page_obj);
    recon_static_functions::app_debug(" rcconfiguration.11.oo", Q_FUNC_INFO);


    apple_metadata_page_obj = new recon_configuration_page_apple_metadata(this);
    recon_static_functions::app_debug(" rcconfiguration.11.pp", Q_FUNC_INFO);
    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_apple_metadata,apple_metadata_page_obj);
    recon_static_functions::app_debug(" rcconfiguration.11.qq", Q_FUNC_INFO);

    exif_metadata_page_obj = new recon_configuration_page_exif_metadata(this);
    recon_static_functions::app_debug(" rcconfiguration.11.rr", Q_FUNC_INFO);
    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_exif_metadata,exif_metadata_page_obj);
    recon_static_functions::app_debug(" rcconfiguration.11.ss", Q_FUNC_INFO);

    volatility_page_obj = new recon_configuration_page_volatility(this);
    recon_static_functions::app_debug(" rcconfiguration.11.tt", Q_FUNC_INFO);
    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_volatility_path,volatility_page_obj);
    recon_static_functions::app_debug(" rcconfiguration.11.uu", Q_FUNC_INFO);

    system_pwd_page_obj = new recon_configuration_page_system_password(this);
    recon_static_functions::app_debug(" rcconfiguration.11.vv", Q_FUNC_INFO);
    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_system_password,system_pwd_page_obj);
    recon_static_functions::app_debug(" rcconfiguration.11.ww", Q_FUNC_INFO);

    external_applications_page_obj = new recon_configuration_page_external_applications(this);
    recon_static_functions::app_debug(" rcconfiguration.11.zz", Q_FUNC_INFO);
    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_external_applications, external_applications_page_obj);
    recon_static_functions::app_debug(" rcconfiguration.22..aa", Q_FUNC_INFO);

    recon_configuration_page_global_timezone_obj = new recon_configuration_page_global_timezone(this);
    recon_static_functions::app_debug(" rcconfiguration.22..bb", Q_FUNC_INFO);
    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_global_time_zone, recon_configuration_page_global_timezone_obj);
    recon_static_functions::app_debug(" rcconfiguration.22..cc", Q_FUNC_INFO);

    filename_list_page_obj = new recon_configuration_page_filename_list(this);
    recon_static_functions::app_debug(" rcconfiguration.22.dd", Q_FUNC_INFO);
    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_filename_search,filename_list_page_obj);
    recon_static_functions::app_debug(" rcconfiguration.22.ee", Q_FUNC_INFO);

    recon_configuration_page_preferences_obj = new recon_configuration_page_preferences(this);
    ui->stackedWidget_load_config_page->insertWidget(enum_recon_config_page_preferences, recon_configuration_page_preferences_obj);

    app_configuration_page_preferences_obj = new app_configuration_page_preferences(this);
    ui->stackedWidget_load_config_page->insertWidget(enum_app_config_page_preferences, app_configuration_page_preferences_obj);


    list_stack_pages << MACRO_RECON_CONFIGURATION_EXAMINER_DETAILS ;
    list_stack_pages << MACRO_RECON_CONFIGURATION_EXAMINER_PLUGINS;
    list_stack_pages << MACRO_RECON_CONFIGURATION_USER_DEFINED_EXTENSIONS;
    list_stack_pages << MACRO_RECON_CONFIGURATION_USER_DEFINED_FILE_SIGNATURES;
    //    list_stack_pages << MACRO_RECON_CONFIGURATION_HASHSET;
    list_stack_pages << MACRO_RECON_CONFIGURATION_KEYWORD_LIST;
    list_stack_pages << MACRO_RECON_CONFIGURATION_FILENAME_LIST;

    list_stack_pages << MACRO_RECON_CONFIGURATION_INDEX_FILES;
    list_stack_pages << MACRO_RECON_CONFIGURATION_APPLE_METADATA;
    list_stack_pages << MACRO_RECON_CONFIGURATION_EXIF_METADATA;
    list_stack_pages << MACRO_RECON_CONFIGURATION_VOLATILITY;
    list_stack_pages << MACRO_RECON_CONFIGURATION_SYSTEM_PASSWORD;
    list_stack_pages << MACRO_RECON_CONFIGURATION_EXTERNAL_APPLICATIONS;
    list_stack_pages << MACRO_RECON_CONFIGURATION_GLOBAL_TIMEZONE;
    list_stack_pages << MACRO_RECON_CONFIGURATION_PREFERENCES;
    list_stack_pages << MACRO_APP_CONFIGURATION_PREFERENCES;


    recon_static_functions::app_debug(" rcconfiguration.22..dd", Q_FUNC_INFO);

    set_config_tree_widget_display();
    recon_static_functions::app_debug(" rcconfiguration.22..ee", Q_FUNC_INFO);

    message_dialog_object = new message_dialog(this);
    recon_static_functions::app_debug(" rcconfiguration.22..ff", Q_FUNC_INFO);

}

recon_configuration::~recon_configuration()
{
    delete ui;
}


void recon_configuration::set_config_tree_widget_display()
{
    for(int i = 0 ; i< list_stack_pages.size(); i++)
    {
        QString category_name = list_stack_pages.at(i);
        QTreeWidgetItem *items_toplevel = new QTreeWidgetItem(ui->treeWidget_page_list);
        items_toplevel->setText(0,category_name);
        items_toplevel->setIcon(0,QIcon("../icons/Launcher/" + category_name.toLower().replace(" ","_") +".png"));

        ui->treeWidget_page_list->insertTopLevelItem(i,items_toplevel);

        if(i == 0)
            ui->treeWidget_page_list->topLevelItem(0)->setSelected(true);
    }
}

void recon_configuration::on_treeWidget_page_list_itemClicked(QTreeWidgetItem *item, int column)
{
    recon_static_functions::app_debug(" - Start",Q_FUNC_INFO);

    if(item->text(0) == MACRO_RECON_CONFIGURATION_EXAMINER_DETAILS)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_examiner_info);
    else if(item->text(0) == MACRO_RECON_CONFIGURATION_EXAMINER_PLUGINS)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_plugin_selection);
    //    else if(item->text(0) == MACRO_RECON_CONFIGURATION_HASHSET)
    //        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_hash_set);
    else if(item->text(0) == MACRO_RECON_CONFIGURATION_USER_DEFINED_FILE_SIGNATURES)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_file_signature);
    else if(item->text(0) == MACRO_RECON_CONFIGURATION_KEYWORD_LIST)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_content_search);
    else if(item->text(0) == MACRO_RECON_CONFIGURATION_FILENAME_LIST)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_filename_search);
    else if(item->text(0) == MACRO_RECON_CONFIGURATION_USER_DEFINED_EXTENSIONS)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_file_extension);
    else if(item->text(0) == MACRO_RECON_CONFIGURATION_APPLE_METADATA)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_apple_metadata);
    else if(item->text(0) == MACRO_RECON_CONFIGURATION_EXIF_METADATA)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_exif_metadata);
    else if(item->text(0) == MACRO_RECON_CONFIGURATION_INDEX_FILES)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_index_files);
    else if(item->text(0) == MACRO_RECON_CONFIGURATION_VOLATILITY)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_volatility_path);
    else if(item->text(0) == MACRO_RECON_CONFIGURATION_SYSTEM_PASSWORD)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_system_password);
    else if(item->text(0) == MACRO_RECON_CONFIGURATION_EXTERNAL_APPLICATIONS)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_external_applications);
    else if(item->text(0) == MACRO_RECON_CONFIGURATION_GLOBAL_TIMEZONE)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_global_time_zone);
    else if(item->text(0) == MACRO_RECON_CONFIGURATION_PREFERENCES)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_preferences);
    else if(item->text(0) == MACRO_APP_CONFIGURATION_PREFERENCES)
        ui->stackedWidget_load_config_page->setCurrentIndex(enum_app_config_page_preferences);

    recon_static_functions::app_debug(" - End",Q_FUNC_INFO);
}

void recon_configuration::pub_set_essentials()
{
    volatility_page_obj->pub_set_essentials();

}

void recon_configuration::pub_set_caller_type(QString type)
{
    caller_type = type;

    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG)
        ui->pushButton_apply->hide();
    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER)
    {
        for(int i = 0; i < ui->treeWidget_page_list->topLevelItemCount(); i++)
        {
            if(ui->treeWidget_page_list->topLevelItem(i)->text(0) == MACRO_RECON_CONFIGURATION_EXAMINER_DETAILS)
                delete ui->treeWidget_page_list->topLevelItem(i);
        }

        for(int i = 0; i < ui->treeWidget_page_list->topLevelItemCount(); i++)
        {
            if(ui->treeWidget_page_list->topLevelItem(i)->text(0) == MACRO_RECON_CONFIGURATION_EXAMINER_PLUGINS)
                delete ui->treeWidget_page_list->topLevelItem(i);
        }


        ui->pushButton_back->hide();

        ui->stackedWidget_load_config_page->setCurrentIndex(enum_recon_config_page_file_extension);

        ui->treeWidget_page_list->setCurrentItem(ui->treeWidget_page_list->topLevelItem(0));
    }

    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG)
    {
        for(int i = 0; i < ui->treeWidget_page_list->topLevelItemCount(); i++)
        {
            if(ui->treeWidget_page_list->topLevelItem(i)->text(0) == MACRO_RECON_CONFIGURATION_GLOBAL_TIMEZONE)
                delete ui->treeWidget_page_list->topLevelItem(i);
        }
    }


    apple_metadata_page_obj->pub_set_caller_type(caller_type);
    exif_metadata_page_obj->pub_set_caller_type(caller_type);
    file_extension_page_obj->pub_set_caller_type(caller_type);
    file_signature_widget_obj->pub_set_caller_type(caller_type);
    index_files_page_obj->pub_set_caller_type(caller_type);
    keyword_list_page_obj->pub_set_caller_type(caller_type);
    filename_list_page_obj->pub_set_caller_type(caller_type);
    system_pwd_page_obj->pub_set_caller_type(caller_type);
    external_applications_page_obj->pub_set_caller_type(caller_type);
    // recon_configuration_page_point_hashset_obj->pub_set_caller_type(caller_type);
    recon_configuration_page_global_timezone_obj->pub_set_caller_type(caller_type);
    recon_configuration_page_preferences_obj->pub_set_caller_type(caller_type);

    app_configuration_page_preferences_obj->pub_set_caller_type(caller_type);

}

void recon_configuration::pub_initialise_configuration()
{
    recon_static_functions::app_debug(" - Start",Q_FUNC_INFO);

    page_examiner_details_obj->pub_initialise_examiner_details();


    plugin_selector_obj->pub_set_launcher_type(MACRO_Launcher_Plugin_Selection_RECON_Configuration);
    plugin_selector_obj->set_narad_muni_for_report(global_narad_muni_class_obj);
    plugin_selector_obj->pub_initialise_multi_os_plugins_from_recon_configuration();


    recon_static_functions::app_debug(" - End",Q_FUNC_INFO);
}

void recon_configuration::on_pushButton_back_clicked()
{
    emit signal_back_button_clicked();
}


void recon_configuration::on_pushButton_apply_clicked()
{
    emit signal_recon_config_apply_button_clicked();

    hide();
}
