#include "page_filesystem_n_modules.h"
#include "ui_page_filesystem_n_modules.h"

page_filesystem_n_modules::page_filesystem_n_modules(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_filesystem_n_modules)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);


    ui->pushButton_hashes->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->pushButton_exif_metadata->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->pushButton_apple_metadata->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->pushButton_signature_analysis->setAttribute(Qt::WA_MacShowFocusRect,false);

    ui->pushButton_face_analysis->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->pushButton_face_analysis->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_face_analysis->text()));

    ui->pushButton_skin_tone_detection->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->pushButton_skin_tone_detection->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_skin_tone_detection->text()));

    ui->pushButton_optical_character_recognition->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->pushButton_optical_character_recognition->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_optical_character_recognition->text()));

    ui->pushButton_weapons->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->pushButton_weapons->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_weapons->text()));

    ui->pushButton_fire_analysis->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->pushButton_fire_analysis->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_fire_analysis->text()));

    ui->label_filesystem_modules->setText(global_lang_tr_obj.get_translated_string(ui->label_filesystem_modules->text()));
    ui->pushButton_apple_metadata->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_apple_metadata->text()));
    ui->pushButton_mime_type->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_mime_type->text()));
    ui->pushButton_signature_analysis->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_signature_analysis->text()));
    ui->pushButton_exif_metadata->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_exif_metadata->text()));
    ui->pushButton_hashes->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_hashes->text()));

    ui->pushButton_unified_logs->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->pushButton_unified_logs->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_unified_logs->text()));

}

page_filesystem_n_modules::~page_filesystem_n_modules()
{
    delete ui;
}

void page_filesystem_n_modules::extract_launcher_values() //set hashset check status
{
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Hashset_Check_Status_bool,checkbox_analyse_hashes->isChecked());

}

void page_filesystem_n_modules::pub_set_essentials_for_case_launcher() //function not in use
{
    QString lib_recon_conf_dir_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString();
    QString lib_result_conf_dir_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString();

    if(!recon_static_functions::copy_directory(lib_recon_conf_dir_path + "hashset",lib_result_conf_dir_path + "case_configuration/hashset/", Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" dir copy ---FAILED----" + lib_result_conf_dir_path + "case_configuration/", Q_FUNC_INFO);
    }
}

void page_filesystem_n_modules::pub_create_ui() //Here we create ui of checkboxes and names display on pre launcher page like Apple metadata, hashes, exif etc
{

    create_apple_metadata_ui();
    ui->stackedWidget_fs_options->insertWidget(enum_stack_page_apple_metatdata,apple_metadata_widget);

    ui->pushButton_mime_type->setHidden(true);

    create_signature_analysis_ui();
    ui->stackedWidget_fs_options->insertWidget(enum_stack_page_signature_analysis,signature_analysis_widget);

    create_exif_metadata_ui();
    ui->stackedWidget_fs_options->insertWidget(enum_stack_page_exif_metadata,exif_metadata_widget);

    create_hashset_ui();
    ui->stackedWidget_fs_options->insertWidget(enum_stack_page_hashset,hashset_widget);

    create_unified_logs_ui();
    ui->stackedWidget_fs_options->insertWidget(enum_stack_page_unified_logs,unified_logs_widget);

//    create_face_analysis_ui();
//    ui->stackedWidget_fs_options->insertWidget(enum_stack_page_face_analysis,face_analysis_widget);
      ui->pushButton_face_analysis->setHidden(true);

//    create_optical_character_recognition_ui();
//    ui->stackedWidget_fs_options->insertWidget(enum_stack_page_optical_character_recognition,optical_character_recognition_widget);
      ui->pushButton_optical_character_recognition->setHidden(true);

//    create_skin_tone_detection_ui();
//    ui->stackedWidget_fs_options->insertWidget(enum_stack_page_skin_tone_detection,skin_tone_detection_widget);
      ui->pushButton_skin_tone_detection->setHidden(true);

//    create_weapons_ui();
//    ui->stackedWidget_fs_options->insertWidget(enum_stack_page_weapons,weapons_widget);
      ui->pushButton_weapons->setHidden(true);

//    create_fire_analysis_ui();
//    ui->stackedWidget_fs_options->insertWidget(enum_stack_page_fire_analysis,fire_analysis_widget);
      ui->pushButton_fire_analysis->setHidden(true);


    ui->stackedWidget_fs_options->setCurrentIndex(enum_stack_page_apple_metatdata);
}

void page_filesystem_n_modules::create_hashset_ui() // create hashset ui and name
{
    ///----Hashset --Start.
    hashset_widget = new QWidget(this);

    //    recon_configuration_page_point_hashset_obj = new recon_configuration_page_point_hashset(this);
    //    recon_configuration_page_point_hashset_obj->pub_set_caller_type(MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER);
    //    recon_configuration_page_point_hashset_obj->hide();

    QVBoxLayout *vbox_layout_main = new QVBoxLayout(hashset_widget);

    checkbox_analyse_hashes = new QCheckBox(this);
    checkbox_analyse_hashes->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(checkbox_analyse_hashes, SIGNAL(clicked(bool)), this, SLOT(slot_checkbox_analyse_hashes_clicked(bool)));
    checkbox_analyse_hashes->setText(global_lang_tr_obj.get_translated_string(" Analyse Hashes"));
    checkbox_analyse_hashes->setStyleSheet("QCheckBox::indicator:checked {background-color:#777777;border: 1px solid #777777;}"
                                           "QCheckBox::indicator:unchecked {background-color:#FFFFFF;border: 1px solid #D0D0D0;}"

                                           "QCheckBox::indicator {"
                                           "width: 12px;"
                                           "height: 12px;"
                                           "}"
                                           );
    checkbox_analyse_hashes->setMinimumHeight(25);

    vbox_layout_main->addWidget(checkbox_analyse_hashes);
    // vbox_layout_main->addWidget(recon_configuration_page_point_hashset_obj);

    vbox_layout_main->setSizeConstraint(QLayout::SetMinAndMaxSize);
    vbox_layout_main->setSpacing(5);
    vbox_layout_main->setContentsMargins(0,1,0,0);
    hashset_widget->setLayout(vbox_layout_main);

    checkbox_analyse_hashes->setChecked(false);

}

void page_filesystem_n_modules::create_exif_metadata_ui() // create exif metadata ui and name
{

    recon_configuration_page_exif_metadata_obj = new recon_configuration_page_exif_metadata(this);
    recon_configuration_page_exif_metadata_obj->pub_set_caller_type(MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER);
    recon_configuration_page_exif_metadata_obj->hide();

    exif_metadata_widget = new QWidget(this);
    QVBoxLayout *vbox_layout_main_exif_metadata = new QVBoxLayout(exif_metadata_widget);
    vbox_layout_main_exif_metadata->setContentsMargins(0,0,0,0);

    checkbox_exif_metadata_obj = new QCheckBox(this);
    checkbox_exif_metadata_obj->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(checkbox_exif_metadata_obj, SIGNAL(clicked(bool)), this, SLOT(slot_checkbox_exif_metadata_clicked(bool)));
    checkbox_exif_metadata_obj->setText(global_lang_tr_obj.get_translated_string(" Extract Exif Metadata"));
    checkbox_exif_metadata_obj->setStyleSheet("QCheckBox::indicator:checked {background-color:#777777;border: 1px solid #777777;}"
                                              "QCheckBox::indicator:unchecked {background-color:#FFFFFF;border: 1px solid #D0D0D0;}"

                                              "QCheckBox::indicator {"
                                              "width: 12px;"
                                              "height: 12px;"
                                              "}"
                                              );
    checkbox_exif_metadata_obj->setChecked(false);
    checkbox_exif_metadata_obj->setMinimumHeight(25);

    vbox_layout_main_exif_metadata->addWidget(checkbox_exif_metadata_obj);
    vbox_layout_main_exif_metadata->addWidget(recon_configuration_page_exif_metadata_obj);

    vbox_layout_main_exif_metadata->setSizeConstraint(QLayout::SetMaximumSize);

    exif_metadata_widget->setLayout(vbox_layout_main_exif_metadata);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Metadata_Check_Status_bool, false);




    return;

}


void page_filesystem_n_modules::create_apple_metadata_ui() // create apple metadata ui and name
{

    recon_configuration_page_apple_metadata_obj = new recon_configuration_page_apple_metadata(this);
    recon_configuration_page_apple_metadata_obj->pub_set_caller_type(MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER);
    recon_configuration_page_apple_metadata_obj->hide();

    apple_metadata_widget = new QWidget(this);
    QVBoxLayout *vbox_layout_main_apple_metadata = new QVBoxLayout(apple_metadata_widget);
    vbox_layout_main_apple_metadata->setContentsMargins(0,0,0,0);

    checkbox_apple_metadata_obj = new QCheckBox(this);
    checkbox_apple_metadata_obj->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(checkbox_apple_metadata_obj, SIGNAL(clicked(bool)), this, SLOT(slot_checkbox_apple_metadata_clicked(bool)));
    checkbox_apple_metadata_obj->setText(global_lang_tr_obj.get_translated_string(" Extract Apple Metadata"));
    checkbox_apple_metadata_obj->setStyleSheet("QCheckBox::indicator:checked {background-color:#777777;border: 1px solid #777777;}"
                                               "QCheckBox::indicator:unchecked {background-color:#FFFFFF;border: 1px solid #D0D0D0;}"

                                               "QCheckBox::indicator {"
                                               "width: 12px;"
                                               "height: 12px;"
                                               "}"
                                               );
    checkbox_apple_metadata_obj->setChecked(false);
    checkbox_apple_metadata_obj->setMinimumHeight(25);

    vbox_layout_main_apple_metadata->addWidget(checkbox_apple_metadata_obj);
    vbox_layout_main_apple_metadata->addWidget(recon_configuration_page_apple_metadata_obj);

    vbox_layout_main_apple_metadata->setSizeConstraint(QLayout::SetMaximumSize);

    apple_metadata_widget->setLayout(vbox_layout_main_apple_metadata);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Apple_Metadata_Check_Status_bool, false);

    return;

}

void page_filesystem_n_modules::create_signature_analysis_ui() // create signature analysis ui and name
{

    signature_widget_obj = new file_signature_widget(this);
    signature_widget_obj->pub_set_caller_type(MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER);
    signature_widget_obj->pub_initialise_signature_details();

    signature_analysis_widget = new QWidget(this);
    QVBoxLayout *vbox_layout_main = new QVBoxLayout(signature_analysis_widget);


    checkbox_signature_analysis_obj = new QCheckBox(this);
    checkbox_signature_analysis_obj->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(checkbox_signature_analysis_obj, SIGNAL(clicked(bool)), this, SLOT(slot_checkbox_analyse_signature_clicked(bool)));
    checkbox_signature_analysis_obj->setText(global_lang_tr_obj.get_translated_string("  Analyse User Defined File Signatures"));
    checkbox_signature_analysis_obj->setStyleSheet("QCheckBox::indicator:checked {background-color:#777777;border: 1px solid #777777;}"
                                                   "QCheckBox::indicator:unchecked {background-color:#FFFFFF;border: 1px solid #D0D0D0;}"

                                                   "QCheckBox::indicator {"
                                                   "width: 12px;"
                                                   "height: 12px;"
                                                   "}"
                                                   );

    checkbox_signature_analysis_obj->setChecked(false);
    checkbox_signature_analysis_obj->setContentsMargins(0,0,0,10);


    QVBoxLayout *vbox_checbox = new QVBoxLayout;
    vbox_checbox->addWidget(checkbox_signature_analysis_obj);
    vbox_checbox->setSpacing(20);

    vbox_layout_main->addLayout(vbox_checbox);
    vbox_layout_main->addWidget(signature_widget_obj);

    vbox_layout_main->setSizeConstraint(QLayout::SetMinAndMaxSize);
    vbox_layout_main->setSpacing(15);
    vbox_layout_main->setContentsMargins(0,1,0,0);
    signature_analysis_widget->setLayout(vbox_layout_main);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Signature_Check_Status_bool, false);



    signature_widget_obj->pub_set_ui_awake_status(true);
    signature_widget_obj->hide();

}

void page_filesystem_n_modules::create_unified_logs_ui() // create unified logs ui and name
{
    unified_logs_widget = new QWidget(this);
    QVBoxLayout *vbox_layout_main_unified_logs = new QVBoxLayout(unified_logs_widget);
    vbox_layout_main_unified_logs->setContentsMargins(0,0,0,0);

    checkbox_unified_logs_obj = new QCheckBox(this);
    checkbox_unified_logs_obj->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(checkbox_unified_logs_obj, SIGNAL(clicked(bool)), this, SLOT(slot_checkbox_unified_logs_clicked(bool)));
    checkbox_unified_logs_obj->setText(global_lang_tr_obj.get_translated_string(" Extract Unified Logs"));
    checkbox_unified_logs_obj->setStyleSheet("QCheckBox::indicator:checked {background-color:#777777;border: 1px solid #777777;}"
                                                              "QCheckBox::indicator:unchecked {background-color:#FFFFFF;border: 1px solid #D0D0D0;}"

                                                              "QCheckBox::indicator {"
                                                              "width: 12px;"
                                                              "height: 12px;"
                                                              "}"
                                                              );

    checkbox_unified_logs_obj->setChecked(false);
    checkbox_unified_logs_obj->setMinimumHeight(25);

    vbox_layout_main_unified_logs->addWidget(checkbox_unified_logs_obj);

    vbox_layout_main_unified_logs->setSizeConstraint(QLayout::SetMaximumSize);

    unified_logs_widget->setLayout(vbox_layout_main_unified_logs);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Unified_Logs_Check_Status_bool, false);

}

void page_filesystem_n_modules::slot_checkbox_unified_logs_clicked(bool checked) //unified logs checkbox click or not check status set in global class
{
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Unified_Logs_Check_Status_bool, checked);
}

void page_filesystem_n_modules::slot_checkbox_analyse_hashes_clicked(bool checked) //function not in use
{
    //    if(checked)
    //    {
    //        recon_configuration_page_point_hashset_obj->show();
    //    }
    //    else
    //    {
    //        recon_configuration_page_point_hashset_obj->hide();
    //    }

}

void page_filesystem_n_modules::slot_checkbox_analyse_signature_clicked(bool checked) //signature analysis checkbox click or not check status set in global class or show window accordingly
{

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Signature_Check_Status_bool, checked);

    if(checked)
        signature_widget_obj->show();
    else
        signature_widget_obj->hide();

}


void page_filesystem_n_modules::slot_checkbox_exif_metadata_clicked(bool checked) //Exif metadata checkbox click or not check status set in global class or show window accordingly
{
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Metadata_Check_Status_bool, checked);

    if(checked)
        recon_configuration_page_exif_metadata_obj->show();
    else
        recon_configuration_page_exif_metadata_obj->hide();

}

void page_filesystem_n_modules::slot_checkbox_apple_metadata_clicked(bool checked) //apple metadata checkbox click or not check status set in global class or hide window
{                                                                               // because previously we give examiner to select apple metadata list which he wants to process
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Apple_Metadata_Check_Status_bool, checked); // but later on we select all metadata by default

    recon_configuration_page_apple_metadata_obj->hide();
}


void page_filesystem_n_modules::slot_exif_data_tablewidget_cellClicked(int row, int column) // Function not in use
{
    recon_static_functions::app_debug(" Starts" , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return;



    recon_static_functions::app_debug(" Ends" , Q_FUNC_INFO);

}


void page_filesystem_n_modules::on_pushButton_apple_metadata_clicked(bool checked) //hide other button index/widget when we click on apple metadata button here
{
    ui->stackedWidget_fs_options->setCurrentIndex(enum_stack_page_apple_metatdata);

    ui->pushButton_apple_metadata->setChecked(checked);

    ui->pushButton_exif_metadata->setChecked(!checked);
    ui->pushButton_hashes->setChecked(!checked);
    ui->pushButton_signature_analysis->setChecked(!checked);
    ui->pushButton_mime_type->setChecked(!checked);
    ui->pushButton_face_analysis->setChecked(!checked);
    ui->pushButton_optical_character_recognition->setChecked(!checked);
    ui->pushButton_skin_tone_detection->setChecked(!checked);
    ui->pushButton_weapons->setChecked(!checked);
    ui->pushButton_fire_analysis->setChecked(!checked);
    ui->pushButton_unified_logs->setChecked(!checked);

}

void page_filesystem_n_modules::on_pushButton_signature_analysis_clicked(bool checked) //hide other button index/widget when we click on Signature analysis button here
{
    ui->stackedWidget_fs_options->setCurrentIndex(enum_stack_page_signature_analysis);

    ui->pushButton_signature_analysis->setChecked(checked);

    ui->pushButton_exif_metadata->setChecked(!checked);
    ui->pushButton_hashes->setChecked(!checked);
    ui->pushButton_apple_metadata->setChecked(!checked);
    ui->pushButton_mime_type->setChecked(!checked);
    ui->pushButton_face_analysis->setChecked(!checked);
    ui->pushButton_optical_character_recognition->setChecked(!checked);
    ui->pushButton_skin_tone_detection->setChecked(!checked);
    ui->pushButton_weapons->setChecked(!checked);
    ui->pushButton_fire_analysis->setChecked(!checked);
    ui->pushButton_unified_logs->setChecked(!checked);

}

void page_filesystem_n_modules::on_pushButton_exif_metadata_clicked(bool checked) //hide other button index/widget when we click on exif metadata button here
{
    ui->stackedWidget_fs_options->setCurrentIndex(enum_stack_page_exif_metadata);
    if(checked == false)
    {
        ui->pushButton_exif_metadata->setChecked(true);
        return;
    }
    ui->pushButton_hashes->setChecked(!checked);
    ui->pushButton_apple_metadata->setChecked(!checked);
    ui->pushButton_signature_analysis->setChecked(!checked);
    ui->pushButton_mime_type->setChecked(!checked);
    ui->pushButton_face_analysis->setChecked(!checked);
    ui->pushButton_optical_character_recognition->setChecked(!checked);
    ui->pushButton_skin_tone_detection->setChecked(!checked);
    ui->pushButton_weapons->setChecked(!checked);
    ui->pushButton_fire_analysis->setChecked(!checked);
    checkbox_exif_metadata_obj->setEnabled(checked);
    ui->pushButton_unified_logs->setChecked(!checked);
}

void page_filesystem_n_modules::on_pushButton_hashes_clicked(bool checked) //hide other button index/widget when we click on hashes button here
{
    ui->stackedWidget_fs_options->setCurrentIndex(enum_stack_page_hashset);
    if(checked == false)
    {
        ui->pushButton_hashes->setChecked(true);
        return;
    }

    ui->pushButton_exif_metadata->setChecked(!checked);
    ui->pushButton_apple_metadata->setChecked(!checked);
    ui->pushButton_signature_analysis->setChecked(!checked);
    ui->pushButton_mime_type->setChecked(!checked);
    ui->pushButton_face_analysis->setChecked(!checked);
    ui->pushButton_optical_character_recognition->setChecked(!checked);
    ui->pushButton_skin_tone_detection->setChecked(!checked);
    ui->pushButton_weapons->setChecked(!checked);
    ui->pushButton_fire_analysis->setChecked(!checked);
    checkbox_analyse_hashes->setEnabled(checked);
    ui->pushButton_unified_logs->setChecked(!checked);
}

void page_filesystem_n_modules::on_pushButton_face_analysis_clicked(bool checked) //hide other button index/widget when we click on face analysis button here
{
    ui->stackedWidget_fs_options->setCurrentIndex(enum_stack_page_face_analysis);

    ui->pushButton_face_analysis->setChecked(checked);

    ui->pushButton_mime_type->setChecked(!checked);
    ui->pushButton_exif_metadata->setChecked(!checked);
    ui->pushButton_hashes->setChecked(!checked);
    ui->pushButton_apple_metadata->setChecked(!checked);
    ui->pushButton_signature_analysis->setChecked(!checked);
    ui->pushButton_optical_character_recognition->setChecked(!checked);
    ui->pushButton_skin_tone_detection->setChecked(!checked);
    ui->pushButton_weapons->setChecked(!checked);
    ui->pushButton_fire_analysis->setChecked(!checked);
    ui->pushButton_unified_logs->setChecked(!checked);
}

void page_filesystem_n_modules::create_face_analysis_ui() //create face analysis button and name ui
{
    face_analysis_widget = new QWidget(this);
    QVBoxLayout *vbox_layout_main_face_analysis = new QVBoxLayout(face_analysis_widget);
    vbox_layout_main_face_analysis->setContentsMargins(0,0,0,0);

    checkbox_extract_face_obj = new QCheckBox(this);
    checkbox_extract_face_obj->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(checkbox_extract_face_obj, SIGNAL(clicked(bool)), this, SLOT(slot_checkbox_extract_face_clicked(bool)));
    checkbox_extract_face_obj->setText(global_lang_tr_obj.get_translated_string(" Extract Faces"));
    checkbox_extract_face_obj->setStyleSheet("QCheckBox::indicator:checked {background-color:#777777;border: 1px solid #777777;}"
                                             "QCheckBox::indicator:unchecked {background-color:#FFFFFF;border: 1px solid #D0D0D0;}"

                                             "QCheckBox::indicator {"
                                             "width: 12px;"
                                             "height: 12px;"
                                             "}"
                                             );
    checkbox_extract_face_obj->setChecked(false);
    checkbox_extract_face_obj->setMinimumHeight(25);

    vbox_layout_main_face_analysis->addWidget(checkbox_extract_face_obj);

    vbox_layout_main_face_analysis->setSizeConstraint(QLayout::SetMaximumSize);

    face_analysis_widget->setLayout(vbox_layout_main_face_analysis);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Face_Analysis_Check_Status_bool, false);

}

void page_filesystem_n_modules::slot_checkbox_extract_face_clicked(bool checked) //set checkbox of face clicked or not status
{
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Face_Analysis_Check_Status_bool, checked);
}

void page_filesystem_n_modules::create_optical_character_recognition_ui() //create OCR button and name ui
{
    optical_character_recognition_widget = new QWidget(this);
    QVBoxLayout *vbox_layout_main_optical_character_recognition = new QVBoxLayout(optical_character_recognition_widget);
    vbox_layout_main_optical_character_recognition->setContentsMargins(0,0,0,0);

    checkbox_optical_character_recognition_obj = new QCheckBox(this);
    checkbox_optical_character_recognition_obj->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(checkbox_optical_character_recognition_obj, SIGNAL(clicked(bool)), this, SLOT(slot_checkbox_optical_character_recognition_clicked(bool)));
    checkbox_optical_character_recognition_obj->setText(global_lang_tr_obj.get_translated_string(" Extract Optical Character"));
    checkbox_optical_character_recognition_obj->setStyleSheet("QCheckBox::indicator:checked {background-color:#777777;border: 1px solid #777777;}"
                                                              "QCheckBox::indicator:unchecked {background-color:#FFFFFF;border: 1px solid #D0D0D0;}"

                                                              "QCheckBox::indicator {"
                                                              "width: 12px;"
                                                              "height: 12px;"
                                                              "}"
                                                               );

    checkbox_optical_character_recognition_obj->setChecked(false);
    checkbox_optical_character_recognition_obj->setMinimumHeight(25);

    vbox_layout_main_optical_character_recognition->addWidget(checkbox_optical_character_recognition_obj);

    vbox_layout_main_optical_character_recognition->setSizeConstraint(QLayout::SetMaximumSize);

    optical_character_recognition_widget->setLayout(vbox_layout_main_optical_character_recognition);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Optical_Character_Recognition_Check_Status_bool, false);

}

void page_filesystem_n_modules::slot_checkbox_optical_character_recognition_clicked(bool checked) //OCR checkbox click or not check status set in global class
{
    recon_static_functions::app_debug(" Starts" , Q_FUNC_INFO);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Optical_Character_Recognition_Check_Status_bool, checked);

    recon_static_functions::app_debug(" Ends" , Q_FUNC_INFO);
}

void page_filesystem_n_modules::on_pushButton_optical_character_recognition_clicked(bool checked) //hide other button index/widget when we click on ocr button here
{
    ui->stackedWidget_fs_options->setCurrentIndex(enum_stack_page_optical_character_recognition);

    ui->pushButton_optical_character_recognition->setChecked(checked);

    ui->pushButton_mime_type->setChecked(!checked);
    ui->pushButton_exif_metadata->setChecked(!checked);
    ui->pushButton_hashes->setChecked(!checked);
    ui->pushButton_apple_metadata->setChecked(!checked);
    ui->pushButton_signature_analysis->setChecked(!checked);
    ui->pushButton_face_analysis->setChecked(!checked);
    ui->pushButton_skin_tone_detection->setChecked(!checked);
    ui->pushButton_weapons->setChecked(!checked);
    ui->pushButton_fire_analysis->setChecked(!checked);
    ui->pushButton_unified_logs->setChecked(!checked);
}

void page_filesystem_n_modules::on_pushButton_skin_tone_detection_clicked(bool checked) //hide other button index/widget when we click on skin tone button here
{
    ui->stackedWidget_fs_options->setCurrentIndex(enum_stack_page_skin_tone_detection);

    ui->pushButton_skin_tone_detection->setChecked(checked);

    ui->pushButton_face_analysis->setChecked(!checked);
    ui->pushButton_mime_type->setChecked(!checked);
    ui->pushButton_exif_metadata->setChecked(!checked);
    ui->pushButton_hashes->setChecked(!checked);
    ui->pushButton_apple_metadata->setChecked(!checked);
    ui->pushButton_signature_analysis->setChecked(!checked);
    ui->pushButton_optical_character_recognition->setChecked(!checked);
    ui->pushButton_weapons->setChecked(!checked);
    ui->pushButton_fire_analysis->setChecked(!checked);
    ui->pushButton_unified_logs->setChecked(!checked);
}

void page_filesystem_n_modules::create_skin_tone_detection_ui() //create skin tone button and name ui
{
    skin_tone_detection_widget = new QWidget(this);
    QVBoxLayout *vbox_layout_main_skin_tone_detection = new QVBoxLayout(skin_tone_detection_widget);
    vbox_layout_main_skin_tone_detection->setContentsMargins(0,0,0,0);

    checkbox_skin_tone_detection_obj = new QCheckBox(this);
    checkbox_skin_tone_detection_obj->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(checkbox_skin_tone_detection_obj, SIGNAL(clicked(bool)), this, SLOT(slot_checkbox_skin_tone_detection_clicked(bool)));
    checkbox_skin_tone_detection_obj->setText(global_lang_tr_obj.get_translated_string(" Start Skin Tone Detection"));
    checkbox_skin_tone_detection_obj->setStyleSheet("QCheckBox::indicator:checked {background-color:#777777;border: 1px solid #777777;}"
                                          "QCheckBox::indicator:unchecked {background-color:#FFFFFF;border: 1px solid #D0D0D0;}"

                                          "QCheckBox::indicator {"
                                          "width: 12px;"
                                          "height: 12px;"
                                          "}"
                                          );
    checkbox_skin_tone_detection_obj->setChecked(false);
    checkbox_skin_tone_detection_obj->setMinimumHeight(25);

    vbox_layout_main_skin_tone_detection->addWidget(checkbox_skin_tone_detection_obj);

    vbox_layout_main_skin_tone_detection->setSizeConstraint(QLayout::SetMaximumSize);

    skin_tone_detection_widget->setLayout(vbox_layout_main_skin_tone_detection);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Skin_Tone_Detection_Check_Status_bool, false);

}

void page_filesystem_n_modules::slot_checkbox_skin_tone_detection_clicked(bool checked) //skin tone checkbox click or not check status set in global class
{

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Skin_Tone_Detection_Check_Status_bool, checked);
}

void page_filesystem_n_modules::create_weapons_ui() //create Weapon analysis button and name ui
{
    weapons_widget = new QWidget(this);
    QVBoxLayout *vbox_layout_main_weapons = new QVBoxLayout(weapons_widget);
    vbox_layout_main_weapons->setContentsMargins(0,0,0,0);

    checkbox_weapons_obj = new QCheckBox(this);
    checkbox_weapons_obj->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(checkbox_weapons_obj, SIGNAL(clicked(bool)), this, SLOT(slot_checkbox_weapons_clicked(bool)));
    checkbox_weapons_obj->setText(global_lang_tr_obj.get_translated_string(" Extract Weapon Items "));
    checkbox_weapons_obj->setStyleSheet("QCheckBox::indicator:checked {background-color:#777777;border: 1px solid #777777;}"
                                          "QCheckBox::indicator:unchecked {background-color:#FFFFFF;border: 1px solid #D0D0D0;}"

                                          "QCheckBox::indicator {"
                                          "width: 12px;"
                                          "height: 12px;"
                                          "}"
                                          );
    checkbox_weapons_obj->setChecked(false);
    checkbox_weapons_obj->setMinimumHeight(25);

    vbox_layout_main_weapons->addWidget(checkbox_weapons_obj);

    vbox_layout_main_weapons->setSizeConstraint(QLayout::SetMaximumSize);

    weapons_widget->setLayout(vbox_layout_main_weapons);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Weapons_Check_Status_bool, false);
}

void page_filesystem_n_modules::slot_checkbox_weapons_clicked(bool checked) //Weapon analysis checkbox click or not check status set in global class
{

   global_narad_muni_class_obj->set_field(MACRO_NARAD_Weapons_Check_Status_bool, checked);

}

void page_filesystem_n_modules::on_pushButton_weapons_clicked(bool checked) //hide other button index/widget when we click on Weapons button here
{

    ui->stackedWidget_fs_options->setCurrentIndex(enum_stack_page_weapons);

    ui->pushButton_weapons->setChecked(checked);

    ui->pushButton_mime_type->setChecked(!checked);
    ui->pushButton_exif_metadata->setChecked(!checked);
    ui->pushButton_hashes->setChecked(!checked);
    ui->pushButton_apple_metadata->setChecked(!checked);
    ui->pushButton_signature_analysis->setChecked(!checked);
    ui->pushButton_face_analysis->setChecked(!checked);
    ui->pushButton_optical_character_recognition->setChecked(!checked);
    ui->pushButton_skin_tone_detection->setChecked(!checked);
    ui->pushButton_fire_analysis->setChecked(!checked);
    ui->pushButton_unified_logs->setChecked(!checked);

}

void page_filesystem_n_modules::create_fire_analysis_ui() //create fire analysis button and name ui
{
    fire_analysis_widget = new QWidget(this);
    QVBoxLayout *vbox_layout_main_fire_analysis = new QVBoxLayout(fire_analysis_widget);
    vbox_layout_main_fire_analysis->setContentsMargins(0,0,0,0);

    checkbox_fire_analysis_obj = new QCheckBox(this);
    checkbox_fire_analysis_obj->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(checkbox_fire_analysis_obj, SIGNAL(clicked(bool)), this, SLOT(slot_checkbox_fire_analysis_clicked(bool)));
    checkbox_fire_analysis_obj->setText(global_lang_tr_obj.get_translated_string(" Extract Fires "));
    checkbox_fire_analysis_obj->setStyleSheet("QCheckBox::indicator:checked {background-color:#777777;border: 1px solid #777777;}"
                                          "QCheckBox::indicator:unchecked {background-color:#FFFFFF;border: 1px solid #D0D0D0;}"
                                          "QCheckBox::indicator {"
                                          "width: 12px;"
                                          "height: 12px;"
                                          "}"
                                          );
    checkbox_fire_analysis_obj->setChecked(false);
    checkbox_fire_analysis_obj->setMinimumHeight(25);

    vbox_layout_main_fire_analysis->addWidget(checkbox_fire_analysis_obj);

    vbox_layout_main_fire_analysis->setSizeConstraint(QLayout::SetMaximumSize);

    fire_analysis_widget->setLayout(vbox_layout_main_fire_analysis);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Fire_Analysis_Check_Status_bool, false);
}

void page_filesystem_n_modules::slot_checkbox_fire_analysis_clicked(bool checked) //fire analysis checkbox click or not check status set in global class
{

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Fire_Analysis_Check_Status_bool, checked);
}

void page_filesystem_n_modules::on_pushButton_fire_analysis_clicked(bool checked) //hide other button index/widget when we click on fire analysis button here
{

    ui->stackedWidget_fs_options->setCurrentIndex(enum_stack_page_fire_analysis);

    ui->pushButton_fire_analysis->setChecked(checked);
    ui->pushButton_mime_type->setChecked(!checked);
    ui->pushButton_exif_metadata->setChecked(!checked);
    ui->pushButton_hashes->setChecked(!checked);
    ui->pushButton_apple_metadata->setChecked(!checked);
    ui->pushButton_signature_analysis->setChecked(!checked);
    ui->pushButton_face_analysis->setChecked(!checked);
    ui->pushButton_optical_character_recognition->setChecked(!checked);
    ui->pushButton_weapons->setChecked(!checked);
    ui->pushButton_skin_tone_detection->setChecked(!checked);
    ui->pushButton_unified_logs->setChecked(!checked);

}

void page_filesystem_n_modules::on_pushButton_unified_logs_clicked(bool checked) //hide other button index/widget when we click on unified logs button here
{
    ui->stackedWidget_fs_options->setCurrentIndex(enum_stack_page_unified_logs);
    ui->pushButton_unified_logs->setChecked(checked);
    ui->pushButton_fire_analysis->setChecked(!checked);
    ui->pushButton_mime_type->setChecked(!checked);
    ui->pushButton_exif_metadata->setChecked(!checked);
    ui->pushButton_hashes->setChecked(!checked);
    ui->pushButton_apple_metadata->setChecked(!checked);
    ui->pushButton_signature_analysis->setChecked(!checked);
    ui->pushButton_face_analysis->setChecked(!checked);
    ui->pushButton_optical_character_recognition->setChecked(!checked);
    ui->pushButton_weapons->setChecked(!checked);
    ui->pushButton_skin_tone_detection->setChecked(!checked);

}

