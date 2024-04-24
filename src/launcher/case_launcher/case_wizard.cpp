#include "case_wizard.h"
#include "ui_case_wizard.h"

case_wizard::case_wizard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::case_wizard)
{

    recon_static_functions::app_debug(" start", Q_FUNC_INFO);

    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    file_dialog_obj.setOption(QFileDialog::DontUseNativeDialog);
    QList<QUrl> drives;
    drives << QUrl::fromLocalFile(QDir("/Volumes/").absolutePath());
    drives << QUrl::fromLocalFile(QDir("/").absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Downloads")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Desktop")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Documents")).absolutePath());
    file_dialog_obj.setSidebarUrls(drives);
    file_dialog_obj.setDirectory(getenv("HOME"));
    file_dialog_obj.setFilter(QDir::AllEntries | QDir::Dirs );

    setWindowTitle(recon_static_functions::get_app_name());

    setWindowFlags(Qt::Window);

    main_stack_widget_functionality();
    ui->pushButton_new_case->setAttribute(Qt::WA_TranslucentBackground);
    ui->pushButton_acquire_device->setAttribute(Qt::WA_MacShowFocusRect,0);

    page_case_info_object = new page_case_info(this);
    ui->stackedWidget_case_launcher->insertWidget(enum_common_case_wizard_page_case_info_selction,page_case_info_object);
    connect(page_case_info_object,SIGNAL(signal_case_info_essentials_filled(QString)),this,SLOT(slot_case_info_essentials_filled(QString)));


    case_directory_object = new page_case_directory(this);
    ui->stackedWidget_case_launcher->insertWidget(enum_common_case_wizard_page_case_directory_selection,case_directory_object);
    connect(case_directory_object,SIGNAL(signal_result_dir_path_selected(QString)),this,SLOT(slot_result_dir_path_selected(QString)));

    source_selector_obj = new source_selector(this);
    ui->stackedWidget_case_launcher->insertWidget(enum_comman_case_wizard_page_multi_source_selection,source_selector_obj);

    connect(source_selector_obj,SIGNAL(signal_source_selected(bool)),this, SLOT(slot_source_selected(bool)));

    //connect(source_selector_obj,SIGNAL(signal_added_roots_details_list(QList<struct_GLOBAL_witness_info_root>)),this, SLOT(slot_added_root_details_list(QList<struct_GLOBAL_witness_info_root>)));
    // connect(source_selector_obj,SIGNAL(signal_added_sources_details_list(QList<struct_GLOBAL_derived_source_info>)),this, SLOT(slot_added_source_details_list(QList<struct_GLOBAL_derived_source_info>)));


    connect(source_selector_obj,SIGNAL(signal_hide_source_selector_parent_window()),this,SLOT(slot_hide_source_selector_parent_window()));
    connect(source_selector_obj,SIGNAL(signal_qml_button_initial_back_clicked()),this,SLOT(slot_qml_button_initial_back_clicked()));

    page_timezone_selection_obj = new case_launcher_page_timezone_selection(this);
    connect(page_timezone_selection_obj,SIGNAL(signal_enable_disable_next_button_in_launcher_page(bool)),this, SLOT(slot_enable_disable_next_button_from_timezone_selection_page(bool)));
    ui->stackedWidget_case_launcher->insertWidget(enum_comman_case_wizard_page_timezone_selection,page_timezone_selection_obj);

    page_filesystem_n_modules_obj = new page_filesystem_n_modules(this);
    ui->stackedWidget_case_launcher->insertWidget(enum_comman_case_wizard_page_file_system_selection,page_filesystem_n_modules_obj);


    plugin_selector_obj = new plugin_selector(this);
    ui->stackedWidget_case_launcher->insertWidget(enum_comman_case_wizard_page_plugin_selection,plugin_selector_obj);
    connect(plugin_selector_obj,SIGNAL(signal_plugin_selected(bool)),this,SLOT(slot_total_plugins_selected(bool)));


    ui->pushButton_next->setText("Next");
    ui->pushButton_previous->setText("Back");

    ui->pushButton_previous->hide();
    ui->pushButton_previous->setEnabled(false);
    QTreeWidgetItem *item_case_selection = new QTreeWidgetItem(ui->treeWidget);
    item_case_selection->setText(0,global_lang_tr_obj.get_translated_string(MACRO_Case_Launcher_Case_Info));
    item_case_selection->setIcon(0,QIcon("../icons/case_info.png"));
    ui->treeWidget->addTopLevelItem(item_case_selection);
    ui->treeWidget->topLevelItem(0)->setSelected(true);

    //-Insertion in list for first default_item
    list_wizard_page_opened << MACRO_Case_Launcher_Case_Info;

    ui->treeWidget->setAttribute(Qt::WA_MacShowFocusRect,0);
    //-To disable unless the pushbuuton is not clicked.

    ui->stackedWidget_case_launcher->setCurrentIndex(enum_common_case_wizard_page_case_info_selction);

    QPalette palette = ui->label_end_user->palette();
    palette.setColor(ui->label_end_user->foregroundRole(),QColor("#D59B3F"));
    ui->label_end_user->setPalette(palette);
    ui->label_end_user->setText("If the END User or expiration information is incorrect, please contact us at hello@sumuri.com.");


    display_treewidget_item();


    tree_selected_item = 0;
    ui->treeWidget->topLevelItem(tree_selected_item)->setSelected(true);


    result_dir_path = "";
    bool_device_selected = false;
    bool_source_selected = false;
    bool_case_info_filled = false;
    bool_case_dir_selected= false;
    bool_plugins_selected = false;
    bool_backup_source_selected = false;

    case_info_details_obj = new case_info_details(this);

    bool_treewidget_navigated = false;

    recon_configuration_obj = new recon_configuration(this);

    ui->pushButton_next->setEnabled(false);

    message_dialog_obj = new message_dialog(this);

    MVAR_LOAD_RESULT_QString_Other_Case = "Other Case";
    MVAR_ACQUIRE_DEVICES_QString_iOS_Device = "iOS Device";
    MVAR_ACQUIRE_DEVICES_QString_androidOS_Device = "androidOS Device";


    qmenu_acquire_devices  = new QMenu(this);

    //ui->pushButton_acquire_device->setMenu(qmenu_acquire_devices);
    connect(qmenu_acquire_devices, SIGNAL(triggered(QAction*)), this, SLOT(slot_menu_triggered_acquire_devices(QAction*)));
    connect(qmenu_acquire_devices, SIGNAL(aboutToShow()), this, SLOT(slot_abouttoshow_acquire_devices()));


    acquire_iOS_devices_obj = new acquire_iOS_devices(this);
    ui->stackedWidget_RECON_wallpaper_and_case_launcher->insertWidget(enum_case_wizard_stack_acquie_ios_device, acquire_iOS_devices_obj);
    connect(acquire_iOS_devices_obj, SIGNAL(signal_bring_recon_lab_banner_page_in_front()), this, SLOT(slot_bring_recon_lab_banner_page_in_front()));


    acquire_android_devices_obj = new acquire_android_devices(this);
    ui->stackedWidget_RECON_wallpaper_and_case_launcher->insertWidget(enum_case_wizard_stack_acquie_android_device, acquire_android_devices_obj);
    connect(acquire_android_devices_obj, SIGNAL(signal_bring_recon_lab_banner_page_in_front()), this, SLOT(slot_bring_recon_lab_banner_page_in_front()));


    qmenu_load_case_recent_result  = new QMenu(this);

    ui->pushButton_load_case->setMenu(qmenu_load_case_recent_result);
    connect(qmenu_load_case_recent_result, SIGNAL(triggered(QAction*)), this, SLOT(slot_menu_triggered_load_case(QAction*)));
    connect(qmenu_load_case_recent_result, SIGNAL(aboutToShow()), this, SLOT(slot_abouttoshow_recent_result_list()));


    ui->label_version_key->hide();
    ui->label_version_value->hide();
    ui->label_release_date_key->hide();
    ui->label_release_date_value->hide();

    ui->pushButton_about_recon->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_about_recon->text()));
    ui->pushButton_recon_configuration->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_recon_configuration->text()));
    ui->pushButton_acquire_device->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_acquire_device->text()));
    ui->pushButton_new_case->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_new_case->text()));
    ui->pushButton_load_case->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_load_case->text()));
    ui->pushButton_back_from_about_recon->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_back_from_about_recon->text()));
    ui->pushButton_check_for_updates->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_check_for_updates->text()));
    ui->pushButton_next->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_next->text()));
    ui->pushButton_online_manual->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_online_manual->text()));
    ui->pushButton_previous->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_previous->text()));
    ui->pushButton_support->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_support->text()));

    ui->pushButton_online_manual->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->pushButton_support->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->pushButton_check_for_updates->setAttribute(Qt::WA_MacShowFocusRect, false);

    list_root_info.clear();

    ui->tabWidget->setCurrentIndex(0); // 'About RECON LAB'

    disk_manager_obj = new disk_manager();

    recon_static_functions::app_debug(" end", Q_FUNC_INFO);
}

case_wizard::~case_wizard()
{
    if(disk_manager_obj != nullptr)
        delete  disk_manager_obj;

    delete ui;
}

void case_wizard::closeEvent(QCloseEvent *event) // when we close recon lab, it delete all the mount points, fuse directories from backend
{


    QString result_config_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString();
    recon_static_functions::do_directory_blank_completely(result_config_path, Q_FUNC_INFO);


    recon_static_functions::do_directory_blank_completely(global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Volume_Dir_QString).toString() ,Q_FUNC_INFO);
    recon_static_functions::do_directory_blank_completely(global_narad_muni_class_obj->get_field(MACRO_NARAD_Mount_Point_Fuse_Dir_QString).toString() ,Q_FUNC_INFO);
    recon_static_functions::do_directory_blank_completely(global_variable_private_var_tmp_Dir_Path_Case_QString ,Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(global_variable_private_var_tmp_Dir_Path_Case_QString ,Q_FUNC_INFO);

    global_variable_license_validation_value_int = 44;
    exit(0);
}

void case_wizard::pub_set_window_title(QString title) // Here we set window title
{
    setWindowTitle(title);
}


void case_wizard::set_initialise_pages_information() // Launch other functionality like select timezones, result directory, pre launcher features, plugins etc
{
    recon_static_functions::app_debug("- Start",Q_FUNC_INFO);
    create_copy_for_case_configuration_dir();

    page_case_info_object->pub_set_initialise_case_info();

    page_timezone_selection_obj->pub_set_page_caller(MACRO_TIMEZONE_SELECTION_CASE_LAUNCHER);
    page_timezone_selection_obj->pub_initialise_timezone_details();

    //page_filesystem_n_modules_obj->pub_set_essentials_for_case_launcher();
    page_filesystem_n_modules_obj->pub_create_ui();

    plugin_selector_obj->pub_set_launcher_type(MACRO_Launcher_Plugin_Selection_New_Case);
    plugin_selector_obj->set_narad_muni_for_report(global_narad_muni_class_obj);
    plugin_selector_obj->pub_initialise_plugin_info_for_main_new_case_launcher();

    recon_static_functions::app_debug("- END",Q_FUNC_INFO);
}


void case_wizard::create_copy_for_case_configuration_dir() // copy recon config database in library folder and rename it. replace if already exists
{
    recon_static_functions::app_debug("- START",Q_FUNC_INFO);

    QString result_config_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString();

    //-1. Remove Case Configuration Directory if exist
    QString case_config_dir_path = result_config_dir_path + "case_configuration/";

    recon_static_functions::do_directory_blank_completely(case_config_dir_path, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(case_config_dir_path, Q_FUNC_INFO);

    QDir dir(case_config_dir_path);

    //-2. Copy Directory of Recon Config into Case Config.
    dir.mkpath(case_config_dir_path);

    QString recon_config_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString();
    recon_static_functions::copy_directory(recon_config_dir_path,case_config_dir_path, Q_FUNC_INFO);

    //-3. Rename Recon configuration db
    QString file_old_name = case_config_dir_path + "recon_configuration.sqlite";
    QString file_new_name = case_config_dir_path + "case_configuration.sqlite";
    QFile::rename(file_old_name,file_new_name);



    recon_static_functions::app_debug("- END",Q_FUNC_INFO);
}


void case_wizard::on_pushButton_next_clicked() // set source selection, directory selection window on widget when we click next click, set according to index(enum)
{
    int index = ui->stackedWidget_case_launcher->currentIndex();

    //    bool_treewidget_navigated = true;
    bool bool_plugin_selector_page = false;

    ui->pushButton_back->hide();

    ui->pushButton_previous->show();
    if(index == enum_common_case_wizard_page_case_info_selction)
    {
        if(!list_wizard_page_opened.contains((MACRO_Case_Launcher_Source)))
        {
            bool_treewidget_navigated = false;
            list_wizard_page_opened << (MACRO_Case_Launcher_Source);
        }

        if(bool_source_selected)
            ui->pushButton_next->setEnabled(true);
        else
            ui->pushButton_next->setEnabled(false);

        ui->stackedWidget_case_launcher->setCurrentIndex(enum_comman_case_wizard_page_multi_source_selection);

    }
    else if(index == enum_comman_case_wizard_page_multi_source_selection)
    {
        if(!list_wizard_page_opened.contains((MACRO_Case_Launcher_Case_Directory)))
        {
            bool_treewidget_navigated = false;
            list_wizard_page_opened << (MACRO_Case_Launcher_Case_Directory);
        }

        case_directory_object->pub_set_stored_output_path();

        bool_case_dir_selected =  case_directory_object->pub_check_output_path_exist();

        if(bool_case_dir_selected)
            ui->pushButton_next->setEnabled(true);

        ui->stackedWidget_case_launcher->setCurrentIndex(enum_common_case_wizard_page_case_directory_selection);
    }
    else if(index == enum_common_case_wizard_page_case_directory_selection)
    {
        if(!list_wizard_page_opened.contains(MACRO_Case_Launcher_DateTime))
        {
            bool_treewidget_navigated = false;
            list_wizard_page_opened << MACRO_Case_Launcher_DateTime;
        }

        ui->stackedWidget_case_launcher->setCurrentIndex(enum_comman_case_wizard_page_timezone_selection);
    }
    else if(index == enum_comman_case_wizard_page_timezone_selection)
    {
        if(!list_wizard_page_opened.contains(MACRO_Case_Launcher_File_System))
        {
            bool_treewidget_navigated = false;
            list_wizard_page_opened << MACRO_Case_Launcher_File_System;
        }

        ui->stackedWidget_case_launcher->setCurrentIndex(enum_comman_case_wizard_page_file_system_selection);
        ui->pushButton_previous->show();
        ui->pushButton_next->show();

    }
    else if(index == enum_comman_case_wizard_page_file_system_selection)
    {

        if(!list_wizard_page_opened.contains(MACRO_Case_Launcher_Artifacts))
        {
            bool_treewidget_navigated = false;
            list_wizard_page_opened << MACRO_Case_Launcher_Artifacts;
        }

        ui->stackedWidget_case_launcher->setCurrentIndex(enum_comman_case_wizard_page_plugin_selection);
        ui->pushButton_previous->setEnabled(true);
        ui->pushButton_next->setText("Start");

    }
    else if(index == enum_comman_case_wizard_page_plugin_selection)
    {
        bool_plugin_selector_page = true;
    }

    //For Comman Plugin Selection page.

    if(bool_plugin_selector_page)
    {
        bool_treewidget_navigated = false;

        collect_page_details_from_all_pages();

        //emit signal_case_launched(launched_case_name,list_source_info);
        emit signal_case_launched(launched_case_name,list_root_info);
    }

    //--Insertion in the tree widget for the entered items on next click..
    if(!bool_treewidget_navigated)
    {
        ui->treeWidget->clear();
        for(int i = 0; i < list_wizard_page_opened.size();i++)
        {
            QString item_text = list_wizard_page_opened.at(i);
            QTreeWidgetItem *item_tree_page_opend = new QTreeWidgetItem(ui->treeWidget);
            item_tree_page_opend->setText(0,global_lang_tr_obj.get_translated_string(item_text));
            item_tree_page_opend->setIcon(0,QIcon("../icons/Launcher/" + item_text.toLower().replace(" ","_") +".png"));
            ui->treeWidget->addTopLevelItem(item_tree_page_opend);

            if(i == list_wizard_page_opened.size() - 1)
            {
                ui->treeWidget->topLevelItem(i)->setSelected(true);
            }
        }
    }
    else
    {
        ui->treeWidget->topLevelItem(index)->setSelected(false);
        ui->treeWidget->topLevelItem(index + 1)->setSelected(true);
    }

}

void case_wizard::on_pushButton_previous_clicked() // when we click back button, show previous window according to index
{
    bool_treewidget_navigated = true;

    int index = ui->stackedWidget_case_launcher->currentIndex();

    if(index < enum_common_case_wizard_page_case_info_selction)
        return;

    ui->pushButton_next->setText("Next");

    ui->stackedWidget_case_launcher->setCurrentIndex(index - 1);

    index = index - 1;

    if(index == enum_common_case_wizard_page_case_info_selction)
    {
        ui->pushButton_previous->hide();

        ui->pushButton_back->show();
    }
    else {
        ui->pushButton_previous->show();
    }

    //-Get previous item selected and  current item diselected in treewidget.
    QList<QTreeWidgetItem*> items_list = ui->treeWidget->selectedItems();

    int item_index = 0;
    if(items_list.size() > 0)
        item_index =  ui->treeWidget->indexOfTopLevelItem(items_list.at(0));

    ui->treeWidget->topLevelItem(item_index)->setSelected(false);
    ui->treeWidget->topLevelItem(item_index - 1)->setSelected(true);
}

void case_wizard::on_pushButton_new_case_clicked(bool checked) //when we click new case button
{
    ui->stackedWidget_RECON_wallpaper_and_case_launcher->setCurrentIndex(enum_case_wizard_stack_main_new_case_selections_stack);

    set_initialise_pages_information();

    if(!checked)
    {
        ui->pushButton_new_case->setChecked(true);
        ui->pushButton_next->setEnabled(false);
    }


    if(checked)
    {
        ui->pushButton_next->setEnabled(true);
        ui->pushButton_load_case->setChecked(false);

        //--Insertion in the tree widget for the entered items..
        ui->treeWidget->clear();
        for(int i = 0; i < list_wizard_page_opened.size();i++)
        {
            QString item_text = list_wizard_page_opened.at(i);
            QTreeWidgetItem *item_tree_page_opend = new QTreeWidgetItem(ui->treeWidget);
            item_tree_page_opend->setText(0,item_text);
            item_tree_page_opend->setIcon(0,QIcon("../icons/launcher/"+item_text.replace(" ","_") +".png"));
            ui->treeWidget->addTopLevelItem(item_tree_page_opend);
        }

        ui->treeWidget->topLevelItem(0)->setSelected(true);
    }

}

void case_wizard::on_pushButton_load_case_clicked(bool checked) // function work when we click on load case button, opens result directory to load case. Result dir path takes from db
{
    QString lib_config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + "/recon_configuration.sqlite";

    QString command = QString("select result_path from case_result_info");

    QString output_dir_path = recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,lib_config_db_path,Q_FUNC_INFO);

    if(output_dir_path.isEmpty())
    {
        output_dir_path = QDir::homePath();
    }

    QString path = QFileDialog::getExistingDirectory(this, "Case", output_dir_path, QFileDialog::ShowDirsOnly);


    if(path.trimmed().isEmpty())
        return;

    QFile file(path + "/Result.recon/caseresult");
    if(!file.exists())
    {
        recon_static_functions::app_debug(" : ---FAILED--- file not exist " + path + "/Result.recon/caseresult",Q_FUNC_INFO);
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Selected directory does not contain RECON case result.");
        message_dialog_obj->show();
        return;
    }

    result_dir_path = path;

    emit signal_load_case_clicked(result_dir_path);
    return;

}

void case_wizard::display_treewidget_item() // Treewidget fonts (left side after clicking on new case)
{
    QFont font;
    font.setPixelSize(13);
    ui->treeWidget->setFont(font);
}

void case_wizard::on_stackedWidget_case_launcher_currentChanged(int arg1) // change index of window(source selection, dir selection, timezone selection etc) according to clicking next or back button
{
    ui->pushButton_previous->setEnabled(true);
    if(arg1 == enum_common_case_wizard_page_case_info_selction)
    {
        ui->pushButton_previous->setEnabled(true);

        if(bool_case_info_filled)
            ui->pushButton_next->setEnabled(true);
        else
            ui->pushButton_next->setEnabled(false);
    }
    else if(arg1 == enum_comman_case_wizard_page_multi_source_selection)
    {
        ui->pushButton_previous->setEnabled(true);

        if(bool_source_selected)
            ui->pushButton_next->setEnabled(true);
        else
            ui->pushButton_next->setEnabled(false);
    }
    else if(arg1 == enum_common_case_wizard_page_case_directory_selection)
    {
        ui->pushButton_previous->setEnabled(true);

        if(bool_case_dir_selected)
            ui->pushButton_next->setEnabled(true);
        else
            ui->pushButton_next->setEnabled(false);
    }
    else if(arg1 == enum_comman_case_wizard_page_timezone_selection)
    {
        ui->pushButton_previous->setEnabled(true);

    }
    else if(arg1 == enum_comman_case_wizard_page_file_system_selection)
    {
        ui->pushButton_previous->setEnabled(true);
        ui->pushButton_next->setEnabled(true);
    }

}

void case_wizard::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column) //Jump on that window which user clicks in treewidget(left side)
{
    bool_treewidget_navigated = true;

    int page_count = ui->stackedWidget_case_launcher->count();

    for(int ii = 0; ii < ui->treeWidget->topLevelItemCount();ii++)
    {
        if(item == ui->treeWidget->topLevelItem(ii))
        {
            if(ii == 0)
            {
                ui->pushButton_previous->hide();

                ui->pushButton_back->show();
            }
            else
            {
                ui->pushButton_previous->show();

                ui->pushButton_back->hide();
            }

            if(ii == (page_count - 1))
                ui->pushButton_next->setText("Start");
            else
                ui->pushButton_next->setText("Next");

            ui->stackedWidget_case_launcher->setCurrentIndex(ii);
            break;
        }
    }
}

void case_wizard::slot_result_dir_path_selected(QString path) //check weather result dir path selected or empty
{
    result_dir_path = path;

    if(result_dir_path != "")
    {
        bool_case_dir_selected = true;
        ui->pushButton_next->setEnabled(true);
    }
    else
        bool_case_dir_selected = false;
}

void case_wizard::slot_case_info_essentials_filled(QString str) // case info/examiner info filled or not
{
    if(str == "")
    {
        bool_case_info_filled = false;
        ui->pushButton_next->setEnabled(false);
    }
    else
    {
        launched_case_name = str;
        bool_case_info_filled = true;
        ui->pushButton_next->setEnabled(true);
    }
}


void case_wizard::slot_total_plugins_selected(bool status) // plugins selected or not
{
    if(!status)
    {
        bool_plugins_selected = false;
        //        ui->pushButton_next->setEnabled(false);
    }
    else
    {
        bool_plugins_selected = true;
        ui->pushButton_next->setEnabled(true);
    }
}

void case_wizard::slot_added_root_details_list(QList<struct_GLOBAL_witness_info_root> target_list) //root sources list which we added using add source button
{
    list_root_info = target_list;

}

void case_wizard::slot_source_selected(bool is_source_selected) //source selected or not after clicing on add source
{
    this->show();

    if(is_source_selected)
    {
        bool_source_selected = true;
        ui->pushButton_next->setEnabled(true);
    }
    else
    {
        bool_source_selected = false;
        ui->pushButton_next->setEnabled(false);

    }

}

void case_wizard::slot_hide_source_selector_parent_window() // after selection, hide previous window
{
    this->hide();
}

void case_wizard::slot_qml_button_initial_back_clicked() // when click on back button, show previous window
{
    this->show();
}

void case_wizard::comman_case_info_essentials_filled(QString str) // case info filled function
{
    slot_case_info_essentials_filled(str);
}

void case_wizard::on_pushButton_about_recon_clicked() // when we click on about button, about window index set on stackwidget and display the same
{
    ui->stackedWidget_RECON_wallpaper_and_case_launcher->setCurrentIndex(enum_case_wizard_stack_main_about_recon);
}

void case_wizard::on_pushButton_back_from_about_recon_clicked() // when we click back from about button, splash window index set on stackwidget and display the same
{
    ui->stackedWidget_RECON_wallpaper_and_case_launcher->setCurrentIndex(enum_case_wizard_stack_qml_splash_screen);
}

void case_wizard::on_pushButton_recon_configuration_clicked() //when click on configuration button
{
    delete recon_configuration_obj;

    recon_configuration_obj = new recon_configuration(this);
    connect(recon_configuration_obj,SIGNAL(signal_back_button_clicked()),this,SLOT(on_pushButton_back_from_about_recon_clicked()));

    recon_configuration_obj->pub_set_essentials();
    recon_configuration_obj->pub_set_caller_type(MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG);
    recon_configuration_obj->pub_initialise_configuration();
    ui->stackedWidget_RECON_wallpaper_and_case_launcher->insertWidget(enum_case_wizard_stack_recon_configuration,recon_configuration_obj);
    ui->stackedWidget_RECON_wallpaper_and_case_launcher->setCurrentIndex(enum_case_wizard_stack_recon_configuration);
}

void case_wizard::slot_backup_locater_source_selected(bool status) // function not in use
{
    bool_backup_source_selected = status;
    ui->pushButton_next->setEnabled(status);
}

void case_wizard::main_stack_widget_functionality() //set RECON LAB logo on splash window
{
    ui->stackedWidget_RECON_wallpaper_and_case_launcher->setCurrentIndex(0);
    QImage img("../icons/sumuri_logo/recon_lab_splash.png");

    ui->label_recon_wallpaper->setStyleSheet(QString("QLabel {background-image: url(\"../icons/sumuri_logo/recon_lab_splash.png\"); color:#DCEA08;}"));

    ui->label_recon_wallpaper->setAlignment(Qt::AlignRight | Qt::AlignBottom);
}


void case_wizard::collect_page_details_from_all_pages() // Collect all the page details like what tiemzone selected, what result dir selected, plugins etc
{
    // source_selector_obj->pub_get_essential_value_from_all_source();
    source_selector_obj->pub_get_essential_value_from_all_roots();

    list_root_info = source_selector_obj->pub_get_root_info_list_from_gui();

    //-a.Get the Output path from CASE Directory page.
    case_directory_object->pub_set_output_dir_path();

    //-b.Get Case info from CASE info page.
    page_case_info_object->pub_set_case_info();

    //-c. Get Timezone Details
    page_timezone_selection_obj->pub_update_timezone_info();
    page_timezone_selection_obj->pub_get_timezone_details();

    //-d. Page Hash set
    page_filesystem_n_modules_obj->extract_launcher_values();


    //-f. Plugins Selector page.
    plugin_selector_obj->pub_get_selected_plugins_from_main_case_launcher();
}

void case_wizard::pub_set_message_in_label_for_unmounting_and_enable_disable_ui(QString message_str, bool value) //During unmounting, enable or dispable other ui accordingly
{
    ui->label_umounting_message->setText(message_str);
    ui->pushButton_new_case->setEnabled(value);
    ui->pushButton_load_case->setEnabled(value);
    ui->pushButton_about_recon->setEnabled(value);
    ui->pushButton_recon_configuration->setEnabled(value);
    ui->stackedWidget_RECON_wallpaper_and_case_launcher->setEnabled(value);
}

void case_wizard::slot_enable_disable_next_button_from_timezone_selection_page(bool value) // enable/disable ui while selecting timezone
{
    if(value)
        ui->pushButton_next->setEnabled(true);
    else
        ui->pushButton_next->setEnabled(false);
}


void case_wizard::on_pushButton_acquire_device_clicked() // set acquire window index when we click in acquire button
{
    ui->stackedWidget_RECON_wallpaper_and_case_launcher->setCurrentIndex(enum_case_wizard_stack_acquie_ios_device);
}

void case_wizard::slot_bring_recon_lab_banner_page_in_front() //show recon lab splash window on front when this slot calls
{
    ui->stackedWidget_RECON_wallpaper_and_case_launcher->setCurrentIndex(enum_case_wizard_stack_qml_splash_screen);
}

void case_wizard::slot_set_license_label_text(QString name, qint64 pd, qint64 ed) //Set license info on main splash window
{
    QDateTime dt1 = QDateTime::fromSecsSinceEpoch(pd);
    QDateTime dt2 = QDateTime::fromSecsSinceEpoch(ed);


    name.replace("_", " ");
    QString msg;
    msg.append("<b>Licensed to : ");
    msg.append(name);
    msg.append(" <br>");
    msg.append("Purchase Date : " + dt1.date().toString());
    msg.append("<br>");
    msg.append("Expiration Date : " + dt2.date().toString());
    msg.append("</b>");

    ui->label_recon_wallpaper->setContentsMargins(10,420,0,0);
    ui->label_recon_wallpaper->setAlignment(Qt::AlignLeft);

    ui->label_recon_wallpaper->setText(msg);
    // ui->label_recon_wallpaper->setIndent(5);
}


void case_wizard::onRequestCompleted() { //When we want to update, click on update button at that time this function calls

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    QByteArray data;

    while(reply->bytesAvailable())
    {
        data += reply->readLine();
    }
    download_link.clear();

    if(data.trimmed().size() < 20)
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Network connectivity issue.");
        ui->pushButton_check_for_updates->setEnabled(true);
        return;
    }

    QString all_data = QString::fromLocal8Bit(data);

    QStringList all_data_strings = all_data.split("<br>");

    if(all_data_strings.size() > 2)
    {
        ui->label_version_value->setText(all_data_strings.at(0));
        QString epch = all_data_strings.at(1);

        // 1541030400 GMT: Thursday, November 1, 2018 12:00:00 AM
        int release_epch = 1541030400;

        if(release_epch >= epch.toInt())
        {
            QDateTime dt = QDateTime::fromSecsSinceEpoch(epch.toInt());
            ui->label_release_date_value->setText(dt.date().toString("MMM-dd-yyyy"));
            QMessageBox::information(this, recon_static_functions::get_app_name(), "Your copy of " + recon_static_functions::get_app_name() + " is up to date.");
            ui->pushButton_check_for_updates->setEnabled(true);
            return;
        }


        QDateTime dt = QDateTime::fromSecsSinceEpoch(epch.toInt());
        ui->label_release_date_value->setText(dt.date().toString("MMM-dd-yyyy"));

        download_link = all_data_strings.at(2);
        ui->label_download_link->setText(download_link);

        ui->label_download_link->setText("<a href="+ QUrl::toPercentEncoding(download_link) + " > " + QString("Download") + " </a>");
        ui->label_download_link->setTextInteractionFlags(Qt::TextBrowserInteraction);
        connect(ui->label_download_link, SIGNAL(linkActivated(QString)), this, SLOT(slot_label_download_link_clicked(QString)));

    }
    ui->pushButton_check_for_updates->setEnabled(true);
    return;
}

void case_wizard::slot_abouttoshow_acquire_devices() //Show acquire or attached iphones
{
    MVAR_ACQUIRE_DEVICES_QString_iOS_Device = "iOS Device";
    MVAR_ACQUIRE_DEVICES_QString_androidOS_Device = "androidOS Device";

    qmenu_acquire_devices->clear();

    QAction *action_filters1   =  new QAction(QString(MVAR_ACQUIRE_DEVICES_QString_iOS_Device), qmenu_acquire_devices);
    qmenu_acquire_devices->addAction(action_filters1);

    QAction *action_filters2   =  new QAction(QString(MVAR_ACQUIRE_DEVICES_QString_androidOS_Device), qmenu_acquire_devices);
    qmenu_acquire_devices->addAction(action_filters2);

}

void case_wizard::slot_abouttoshow_recent_result_list() // function not in use
{
    qmenu_load_case_recent_result->clear();
    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "loaded_results_info.sqlite";
    QString command = "Select case_name , result_dir_path from tbl_loaded_result_info ORDER BY INT DESC";
    QStringList case_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,1,dest_db_path , Q_FUNC_INFO);
    for(int count = 0; count < case_path_list.size(); count++)
    {
        QString t_rslt_dir_path = case_path_list.at(count);
        QFileInfo file_info(t_rslt_dir_path);
        if(!file_info.exists())
        {
            QString delete_cmd = "Delete from  tbl_loaded_result_info where result_dir_path = ?";
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(delete_cmd,QStringList(t_rslt_dir_path),dest_db_path , Q_FUNC_INFO);
            continue ;
        }

        QAction *action_filters   =  new QAction(t_rslt_dir_path, this);
        qmenu_load_case_recent_result->addAction(action_filters);
    }

    QAction *action_filters_others   =  new QAction(MVAR_LOAD_RESULT_QString_Other_Case, this);
    if(case_path_list.size() != 0)
        qmenu_load_case_recent_result->addSeparator();

    qmenu_load_case_recent_result->addAction(action_filters_others);
}

void case_wizard::slot_menu_triggered_acquire_devices(QAction *current_clicked_action) //Only display iOS acquire window as android one is disabled
{
    if(current_clicked_action->text() == MVAR_ACQUIRE_DEVICES_QString_iOS_Device)
    {
        ui->stackedWidget_RECON_wallpaper_and_case_launcher->setCurrentIndex(enum_case_wizard_stack_acquie_ios_device);
    }
    else if(current_clicked_action->text() == MVAR_ACQUIRE_DEVICES_QString_androidOS_Device)
    {
        ui->stackedWidget_RECON_wallpaper_and_case_launcher->setCurrentIndex(enum_case_wizard_stack_acquie_android_device);
    }

}

void case_wizard::slot_menu_triggered_load_case(QAction *current_clicked_action) //Function not in use
{
    if(current_clicked_action->text() == MVAR_LOAD_RESULT_QString_Other_Case)
    {
        action_load_other_case_clicked();
    }
    else
    {
        action_load_already_added_case_clicked(current_clicked_action->text());
    }

}

void case_wizard::network_error(QNetworkReply::NetworkError)
{

    return;
}

void case_wizard::slot_label_download_link_clicked(QString str) // Download link up of lab
{
    QString link = QString::fromLocal8Bit(QByteArray::fromPercentEncoding(str.toLocal8Bit()));
    QDesktopServices::openUrl(QUrl(link));
}

void case_wizard::action_load_other_case_clicked() //not in use
{

    QString path;

    file_dialog_obj.setFileMode(QFileDialog::Directory);
    // file_dialog_obj.setOptions(QFileDialog::ShowDirsOnly);

    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            path = selected_files.at(0);
        }
    }

    set_loaded_result_path(path);
}

void case_wizard::action_load_already_added_case_clicked(QString load_case_str) //function not in use
{
    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "loaded_results_info.sqlite";

    QString command = "Select result_dir_path from tbl_loaded_result_info where result_dir_path = ?";
    QString loaded_result_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, QStringList(load_case_str) , 0 ,db_path ,Q_FUNC_INFO);
    if(!loaded_result_path.endsWith("/"))
        loaded_result_path.append("/");

    set_loaded_result_path(loaded_result_path);
}

void case_wizard::set_loaded_result_path(QString loaded_result_path) // set result directory path, when we click on load case, it opens result dir.
{
    if(loaded_result_path.trimmed().isEmpty())
        return;

    QFile file_info(loaded_result_path + "/Result.recon/caseresult");
    if(!file_info.exists())
    {
        recon_static_functions::app_debug(" : ---FAILED--- file not exist " + loaded_result_path + "/Result.recon/caseresult",Q_FUNC_INFO);
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Selected directory does not contain RECON case result.");
        message_dialog_obj->show();
        return;
    }

    result_dir_path = loaded_result_path;
    emit signal_load_case_clicked(result_dir_path);
    return;

}

void case_wizard::on_pushButton_check_for_updates_clicked() //check for updates button performs
{
    //QString link = QString::fromLocal8Bit(QByteArray::fromPercentEncoding("https://goo.gl/wWm2qi"));
    QString link = QString::fromLocal8Bit(QByteArray::fromPercentEncoding("https://sumuri.com/updates/"));
    QDesktopServices::openUrl(QUrl(link));

    //    ui->label_download_link->clear();
    //    ui->label_release->clear();
    //    ui->label_version->clear();

    //    QNetworkRequest request;
    //    request.setUrl(QUrl("http://forensodigital.com/reconlab.php"));

    //    m_networkManager = new QNetworkAccessManager(this);
    //    QNetworkReply *reply = m_networkManager->get(request);

    //    connect(reply, SIGNAL(finished()),this, SLOT(onRequestCompleted()));

    //    ui->pushButton_check_for_updates->setEnabled(false);

}

void case_wizard::on_pushButton_support_clicked() //support button clicked
{
    //QString link = QString::fromLocal8Bit(QByteArray::fromPercentEncoding("https://sumuri.freshdesk.com/support/home"));
    QString link = QString::fromLocal8Bit(QByteArray::fromPercentEncoding("https://helpdesk.sumuri.com/"));
    QDesktopServices::openUrl(QUrl(link));

}

void case_wizard::on_pushButton_online_manual_clicked() // want to see manual, click online manual button
{
    //QString link = QString::fromLocal8Bit(QByteArray::fromPercentEncoding("https://sumuri.com/software/software-manuals/"));
    QString link = QString::fromLocal8Bit(QByteArray::fromPercentEncoding("https://sumuri.com/manuals/"));
    QDesktopServices::openUrl(QUrl(link));

}


void case_wizard::on_pushButton_disk_manager_clicked() // disk manager button which display the attached disks on table widget
{

    if(disk_manager_obj->pub_get_disk_arbitration_started_status())
    {
        disk_manager_obj->pub_set_disk_arbitration_label_status(QString("Turn ON"));
    }
    else
    {
        disk_manager_obj->pub_set_disk_arbitration_label_status(QString("Turn OFF"));
    }

    disk_manager_obj->pub_display_data_with_progress_message();

    disk_manager_obj->show();
}

disk_manager * case_wizard::pub_get_disk_manager_object() // disk manager class object return  to use
{
    return  disk_manager_obj;
}

void case_wizard::pub_set_qml_on_first_main_page() // Set RECON LAB main splash window on first screen created by QML
{
    QUrl launcher_page_url = QUrl("qrc:/splash_window.qml");

    QQuickWidget *quick_widget = new QQuickWidget();

    QQmlContext *context = quick_widget->rootContext();

    context->setContextProperty("case_wizard_obj",this);

    quick_widget->setSource(launcher_page_url);

    quick_widget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    ui->stackedWidget_RECON_wallpaper_and_case_launcher->insertWidget(enum_case_wizard_stack_qml_splash_screen,quick_widget);
    ui->stackedWidget_RECON_wallpaper_and_case_launcher->setCurrentIndex(enum_case_wizard_stack_qml_splash_screen);

}

void case_wizard::pub_set_lic_info(QString username, qint64 license_expiry_date_numeric_qint64) // set license info in private variable for further use
{
    QDateTime dt2 = QDateTime::fromSecsSinceEpoch(license_expiry_date_numeric_qint64);

    QString expiry_date = dt2.date().toString();

    license_customer_name = username;
    license_expiry_date = expiry_date;
}

void case_wizard::pbin_button_on_splash_window_clicked(QString m_value) //the button we click from qml, will perform his functionality
{
    if(m_value == "new_case")
        on_pushButton_new_case_clicked(false);
    else if(m_value == "load_case")
        on_pushButton_load_case_clicked(false);
    else if(m_value == "acquire_ios_device")
        on_pushButton_acquire_device_clicked();
    else if(m_value == "settings")
        on_pushButton_recon_configuration_clicked();
    else if(m_value == "about_recon")
        on_pushButton_about_recon_clicked();
    else if(m_value == "disk_manager")
        on_pushButton_disk_manager_clicked();
}

QString case_wizard::pbin_get_label(QString m_value) // Show these details on main screen
{
    if(m_value == "customer_name")
        return license_customer_name;
    else if(m_value == "expiry_date")
        return license_expiry_date;
    else if(m_value == "app_name")
        return recon_static_functions::get_app_name();
}

QString case_wizard::pbin_get_icons_path(QString icon_name ) //Set icons on main screen using qml
{
    QString icon_path_1 = QApplication::applicationDirPath() + "/../icons/";
    QString icon_path_2 = QApplication::applicationDirPath() + "/../icons/sumuri_logo/";
    QString icon_path_3;

    if(icon_name  == "lab_splash_image")
        icon_path_3 = icon_path_2 + "recon_lab_splash.png";
    else if(icon_name  == "new_case")
        icon_path_3 = icon_path_1 + "new_case.png";
    else if(icon_name  == "load_case")
        icon_path_3 = icon_path_1 + "load_case.png";
    else if(icon_name  == "acquire_ios_device")
        icon_path_3 = icon_path_1 + "acquire_ios_device.png";
    else if(icon_name  == "settings")
        icon_path_3 = icon_path_1 + "settings.png";
    else if(icon_name  == "about")
        icon_path_3 = icon_path_1 + "about_recon.png";
    else if(icon_name  == "disk_manager")
        icon_path_3 = icon_path_1 + "disk_manager.png";
    else if(icon_name  == "calendar")
        icon_path_3 = icon_path_1 + "calendar.png";

    return icon_path_3;
}

void case_wizard::on_pushButton_back_clicked() // Change index to qml splash window when we click on back button
{
    ui->stackedWidget_RECON_wallpaper_and_case_launcher->setCurrentIndex(enum_case_wizard_stack_qml_splash_screen);
}
