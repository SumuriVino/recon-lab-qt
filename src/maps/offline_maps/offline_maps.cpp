#include "offline_maps.h"
#include "ui_offline_maps.h"
#include <QtQuick>
#include <QGeoCoordinate>

offline_maps::offline_maps(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::offline_maps)
{
    recon_static_functions::app_debug("offline maps constructor aaa", Q_FUNC_INFO);

    recon_helper_standard_obj = new recon_helper_standard(this);

    create_ui();

    recon_static_functions::app_debug("offline maps constructor bbb", Q_FUNC_INFO);

    exif_data_image_files_obj = new exif_data_image_files(this);

    recon_static_functions::app_debug("offline maps constructor ccc", Q_FUNC_INFO);

    exif_data_image_files_obj->hide();

    recon_static_functions::app_debug("offline maps constructor ddd", Q_FUNC_INFO);


    bool_online_maps_running = false;
    online_maps_dialog_obj = new online_maps_dialog(this);
    connect(online_maps_dialog_obj, SIGNAL(signal_online_maps_closed()), this, SLOT(slot_online_maps_closed()));

    bool_message_box_yes_button_clicked = false;
    bool_message_box_clicked = false;
    message_dialog_obj = new message_dialog(this);
    connect(message_dialog_obj,SIGNAL(signal_yes_no_check(bool)),this,SLOT(slot_message_box_yes_no_clicked(bool)));

}

offline_maps::~offline_maps()
{
    delete ui;
}


void offline_maps::pub_set_recon_file_info(struct_global_recon_file_info obj)
{
    st_globl_recon_file_info_obj = obj;

}


// https://www.google.com/maps/dir//50.219269,-66.2155669/
void offline_maps::pub_change_location(QString latitude , QString longitude, QString fil_path_2, QString source_count_name)
{
    QString filepath = st_globl_recon_file_info_obj.complete_file_path;

    if(!filepath.trimmed().isEmpty())
    {
        if(latitude.trimmed().isEmpty() || longitude.trimmed().isEmpty())
        {
            QStringList my_list;
            my_list << "Latitude";
            my_list << "Longitude";

            QMap<QString, QString> map_obj =  exif_data_image_files_obj->pub_extract_specific_attributes(filepath, my_list);
            QMap <QString, QString>::iterator mapiter;
            for(mapiter = map_obj.begin(); mapiter != map_obj.end() ; mapiter++)
            {
                if(mapiter.key() == "Latitude")
                {
                    latitude = mapiter.value();
                }
                else if(mapiter.key() == "Longitude")
                {
                    longitude = mapiter.value();
                }
            }
        }

        //        if(filepath.startsWith(struct_source_info.virtual_source_path))
        //        {
        //            filepath.remove(0,struct_source_info.virtual_source_path.size());
        //        }
        //        else if(filepath.startsWith(result_dir_path))
        //        {
        //            filepath.remove(0,result_dir_path.size());
        //        }

        //        if(struct_source_info.os_naming_scheme == MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_OS_NAMING_SCHEME_iOS)
        //        {
        //            filepath = global_recon_555_helper_singular_class_obj->pub_get_fs_display_path_according_recon_file_infopath(filepath,source_count_name);
        //        }

        //        if(!filepath.startsWith("/") && !filepath.trimmed().isEmpty())
        //            filepath.prepend("/");

    }

    current_filepath_str = st_globl_recon_file_info_obj.display_file_path;
    current_source_count_name = source_count_name;
    current_latitude_str = latitude;
    current_longitude_str = longitude;

    //current_url_str = QString("https://www.google.com/maps/dir//") + latitude + "," + longitude;
    current_url_str = get_online_map_url();

    if(bool_online_maps_running)
    {
        if(combobox_online_maps->currentText() == QString(MACRO_Online_Maps_Apple_Maps))
        {
            online_maps_dialog_obj->pub_show_apple_maps(current_url_str);
        }
        else
        {
            online_maps_dialog_obj->pub_show_map(current_url_str);
        }
    }

    QString text_edit_data;
    text_edit_data.append("© OpenStreetMap contributors<br></br><br></br>");
    text_edit_data.append("<b>Latitude</b> : "  + latitude  + "<br></br>");
    text_edit_data.append("<b>Longitude</b> : " + longitude + "<br></br>");
    text_edit_data.append("<b>Google Maps</b> : <a href=\"https://www.google.com/maps/dir//" + latitude + "," + longitude + "\">Open with Google</a>");

    text_browser_obj->setHtml(text_edit_data);

    bool_is_location_available = true;

    if(latitude.trimmed().isEmpty() && longitude.trimmed().isEmpty())
    {
        pushbutton_use_online_maps->setDisabled(true);
        stacked_widget_obj->setCurrentIndex(0);
        bool_is_location_available = false;
        return;
    }
    else
        pushbutton_use_online_maps->setEnabled(true);


    QGeoCoordinate geo(latitude.toDouble(), longitude.toDouble());
    QQmlProperty(quickview_object, "location").write(QVariant::fromValue(geo));
    stacked_widget_obj->setCurrentIndex(1);

    return;
}

void offline_maps::create_ui()
{
    recon_static_functions::app_debug("offline maps create ui 1", Q_FUNC_INFO);

    hbox_map = new QHBoxLayout;
    hbox_map->setSpacing(0);
    hbox_map->setContentsMargins(0,0,0,0);
    hbox_map->setContentsMargins(0,0,0,0);
    vbox_main_layout = new QVBoxLayout;
    vbox_main_layout->setContentsMargins(0,0,0,0);

    //    checkBox_use_online_maps = new QCheckBox(this);
    //    checkBox_use_online_maps->setCheckState(Qt::Unchecked);
    //    checkBox_use_online_maps->setText("Use online Maps");
    //    connect(checkBox_use_online_maps, SIGNAL(clicked(bool)), this, SLOT(slot_checkBox_use_online_maps_clicked(bool)));

    combobox_online_maps = new QComboBox(this);
    combobox_online_maps->setMaximumWidth(150);

    //==========(Temp)-Start
    combobox_online_maps->hide();
    //==========(Temp)-End

    QStringList items_list;
    items_list << QString(MACRO_Online_Maps_Google_Maps);
//    items_list << QString(MACRO_Online_Maps_Apple_Maps);
//    items_list << QString(MACRO_Online_Maps_Bing_Maps);

    combobox_online_maps->addItems(items_list);


    pushbutton_use_online_maps = new QPushButton("Use online Maps", this);
    pushbutton_use_online_maps->setMaximumWidth(150);
    connect(pushbutton_use_online_maps, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_use_online_maps_clicked(bool)));

    if(global_variable_desktop_silicon_bool)
        pushbutton_use_online_maps->setHidden(true);
    else
        pushbutton_use_online_maps->setHidden(false);


    QVBoxLayout *vbox_left_side = new QVBoxLayout;
    vbox_left_side->setSpacing(10);

    text_browser_obj = new QTextBrowser(this);
    text_browser_obj->setFixedWidth(300);
    text_browser_obj->setReadOnly(true);
    text_browser_obj->setOpenExternalLinks(true);

    pushbutton_save_maps = new QPushButton("Save", this);
    pushbutton_save_maps->setAttribute(Qt::WA_MacShowFocusRect, false);

    pushbutton_save_maps->setFixedWidth(80);

    connect(pushbutton_save_maps, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_save_maps_clicked(bool)));

    QHBoxLayout *hbox_save = new QHBoxLayout;
    hbox_save->addWidget(pushbutton_save_maps);
    hbox_save->setContentsMargins(0,0,0,0);

    QHBoxLayout *hbox_online_maps = new QHBoxLayout;
    hbox_online_maps->setSpacing(4);

    hbox_online_maps->addWidget(combobox_online_maps);
    hbox_online_maps->addWidget(pushbutton_use_online_maps);

    //    vbox_left_side->addWidget(checkBox_use_online_maps);
    //vbox_left_side->addWidget(pushbutton_use_online_maps);
    vbox_left_side->addLayout(hbox_online_maps);
    vbox_left_side->addWidget(text_browser_obj);

    vbox_left_side->addLayout(hbox_save);
    vbox_left_side->setContentsMargins(0,0,0,0);

    //text_edit_obj->setTextInteractionFlags(Qt::LinksAccessibleByMouse);


    view  = new QQuickView();
    QWidget *container = QWidget::createWindowContainer(view, this);
    container->setFocusPolicy(Qt::TabFocus);
    view->setSource(QUrl("qrc:/offline_maps.qml"));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    container->setMinimumWidth(300);
    quickview_object = view->rootObject();

    stacked_widget_obj = new QStackedWidget(this);
    blank_obj = new blank(this);
    blank_obj->pub_set_label_text(QString("No Map Location Available"));

    //    web_view = new QWebEngineView();

    stacked_widget_obj->addWidget(blank_obj);
    //stacked_widget_obj->addWidget(web_view);
    stacked_widget_obj->addWidget(container);

    hbox_map->addItem(vbox_left_side);

    hbox_map->addWidget(stacked_widget_obj);
    vbox_main_layout->addLayout(hbox_map);

    this->setLayout(vbox_main_layout);
    recon_static_functions::app_debug("offline maps create ui 47", Q_FUNC_INFO);
}


void offline_maps::slot_checkBox_use_online_maps_clicked(bool status)
{
    if(!bool_is_location_available)
    {
        stacked_widget_obj->setCurrentIndex(0);
        return;
    }

    if(status)
        stacked_widget_obj->setCurrentIndex(1);
    else
        stacked_widget_obj->setCurrentIndex(2);
}

void offline_maps::slot_pushbutton_use_online_maps_clicked(bool status)
{
    Q_UNUSED(status);

    current_url_str = get_online_map_url();

    bool_online_maps_running = true;
    online_maps_dialog_obj->show();
    online_maps_dialog_obj->pub_show_map(current_url_str);

    online_maps_dialog_obj->show();
    if(combobox_online_maps->currentText() == QString(MACRO_Online_Maps_Apple_Maps))
    {
        online_maps_dialog_obj->pub_show_apple_maps(current_url_str);
    }
    else
    {
        online_maps_dialog_obj->pub_show_map(current_url_str);
    }
}

void offline_maps::slot_online_maps_closed()
{
    bool_online_maps_running = false;
}

void offline_maps::slot_message_box_yes_no_clicked(bool yes_no_clicked)
{
    bool_message_box_yes_button_clicked = yes_no_clicked;
    bool_message_box_clicked = true;

}

void offline_maps::slot_pushbutton_save_maps_clicked(bool status)
{
    Q_UNUSED(status);

    if(current_latitude_str.trimmed().isEmpty() && current_longitude_str.trimmed().isEmpty())
        return ;

    QString export_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "Maps/";

    QString current_filename = QFileInfo(current_filepath_str).fileName();
    current_filename.remove(QFileInfo(current_filepath_str).suffix());
    if(current_filename.endsWith("."))
        current_filename.chop(1);

    current_filename = current_filename.trimmed();

    if(current_filename.isEmpty())
        return;

    current_filename.append(".png");
    current_filename.replace(" ","_");

    current_filename = recon_static_functions::get_available_filename(current_filename, export_path, Q_FUNC_INFO);

    QString saved_filepath = export_path + current_filename;


    QImage image = view->grabWindow();
    image.save(saved_filepath , "PNG");

    QString saved_maps_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";

    QString complete_file_path = saved_filepath;

    if(saved_filepath.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString()))
        saved_filepath.remove(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString());

    QString recon_filefrom = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);
    QString recon_file_info_path = complete_file_path;
    if(recon_file_info_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString()))
        recon_file_info_path.remove(0,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString().size());


    if(!recon_file_info_path.startsWith("/"))
        recon_file_info_path.prepend("/");

    QString command = QString("Insert into tags(bookmark,record_no,category,plugin_name,tab_name,item0,item1,item2,item3,item4,recon_tag_value,source_count_name,recon_filefrom,recon_file_infopath) values(?,?,?,?,?, ?,?,?,?,?,?,?,?,?)");
    QStringList value_list;

    value_list << QString::number(0) << "" << QString(MACRO_Plugin_Name_Saved_Maps) << QString(MACRO_Plugin_Saved_Maps_TAB_Name_Saved_Maps)
               << QString(MACRO_Plugin_Saved_Maps_TAB_Name_Saved_Maps) << QFileInfo(current_filepath_str).fileName() << current_filepath_str
               << current_latitude_str << current_longitude_str << saved_filepath << QString(MACRO_Tag_Name_Saved_Maps) << current_source_count_name
               << recon_filefrom <<  recon_file_info_path;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command , value_list , saved_maps_db_path, Q_FUNC_INFO);


    QString update_cmd = "UPDATE tags set record_no = INT";
    recon_helper_standard_obj->execute_db_command_by_dbpath(update_cmd,saved_maps_db_path,Q_FUNC_INFO);

    emit signal_add_saved_maps_in_case_tree();

    bool_message_box_yes_button_clicked = false;
    bool_message_box_clicked = false;
    message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO, global_lang_tr_obj.get_translated_string(QString("Map has been saved. Do you want to open it?")));
    message_dialog_obj->show();

    recon_static_functions::debug_conditional(" : infinite loop start", Q_FUNC_INFO);
    int proc_counter = 0;
    while(1)
    {
        proc_counter++;
        if(proc_counter % 100 == 0)
        {
            proc_counter = 0;
            QCoreApplication::processEvents();
        }

        if(bool_message_box_clicked)
            break ;
    }
    recon_static_functions::debug_conditional(" : infinite loop end", Q_FUNC_INFO);

    if(bool_message_box_yes_button_clicked)
    {
        emit signal_display_saved_maps(MACRO_CASE_TREE_Saved_Maps);
    }

}

QString offline_maps::get_online_map_url()
{
    QString online_map_type_str = combobox_online_maps->currentText();

    if(online_map_type_str.isEmpty())
        online_map_type_str = QString(MACRO_Online_Maps_Google_Maps);


    QString current_url_str;
    if(online_map_type_str == QString(MACRO_Online_Maps_Google_Maps))
    {
        current_url_str = QString("https://www.google.com/maps/dir//") + current_latitude_str + "," + current_longitude_str;
    }
    else if(online_map_type_str == QString(MACRO_Online_Maps_Apple_Maps))
    {
        current_url_str = QString("https://maps.apple.com/?q=") + current_latitude_str + "," + current_longitude_str;
    }
    else if(online_map_type_str == QString(MACRO_Online_Maps_Bing_Maps))
    {
        current_url_str = QString("http://www.bing.com/maps/?v=2&cp=")  + current_latitude_str + "~" + current_longitude_str +
                "&sp=point." + current_latitude_str + "_" + current_longitude_str + "_You%20Are%20Here";
    }

    return  current_url_str;
}

