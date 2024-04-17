#include "story_board.h"
#include "ui_story_board.h"

story_board::story_board(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::story_board)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    setWindowTitle("RECON Story Board");

    recon_helper_standard_obj = new recon_helper_standard(this);

    story_board_textedit_obj = new story_board_textedit(this);

    export_rsync_obj = new export_rsync(this);

    ui->verticalLayout_story_board->addWidget(story_board_textedit_obj);

    ui->lineEdit_search->setMinimumWidth(200);
    ui->lineEdit_search->setMaximumWidth(220);
    ui->lineEdit_search->setAttribute(Qt::WA_MacShowFocusRect,false);

    QFont m_font_lineedit_search = ui->lineEdit_search->font();
    m_font_lineedit_search.setPointSize(12);
    ui->lineEdit_search->setFont(m_font_lineedit_search);

    QFont m_font_pushbutton_search = ui->pushButton_search->font();
    m_font_pushbutton_search.setPointSize(12);
    ui->pushButton_search->setFont(m_font_pushbutton_search);

    QFont m_font_pushbutton_show_all = ui->pushButton_show_all->font();
    m_font_pushbutton_show_all.setPointSize(12);
    ui->pushButton_show_all->setFont(m_font_pushbutton_show_all);
    ui->lineEdit_search->hide();
    ui->pushButton_search->hide();
    ui->pushButton_show_all->hide();

    story_board_loader_obj = new story_board_loader(this);
    story_board_loader_obj->pub_set_storyboard_textedit_class_obj(story_board_textedit_obj);
    connect(story_board_loader_obj,SIGNAL(signal_add_record_to_story_board_by_loader(QString,QString)),this,SLOT(slot_add_record_to_story_board(QString,QString)),Qt::QueuedConnection);
    connect(story_board_loader_obj,SIGNAL(signal_add_file_to_story_board_by_loader(QString,QString,bool,QString)),this,SLOT(slot_add_file_to_story_board(QString, QString,bool,QString)),Qt::QueuedConnection);
    connect(story_board_loader_obj,SIGNAL(signal_show_all_clicked()),this,SLOT(slot_show_all_clicked()),Qt::QueuedConnection);
    connect(story_board_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString,QString)),this,SIGNAL(signal_storyboard_tablewidget_goto_record(QString,QString,QString,QString,QString,QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(signal_tags_combobox_index_changed()), story_board_loader_obj, SLOT(slot_tags_combobox_index_changed()));
    connect(story_board_loader_obj,SIGNAL(signal_on_cancel_pushbutton()),this,SLOT(slot_on_cancel_from_progress_bar()),Qt::QueuedConnection);

    connect(story_board_loader_obj,SIGNAL(signal_act_quick_look_triggered()),this,SIGNAL(signal_act_quick_look_triggered()),Qt::QueuedConnection);
    connect(story_board_loader_obj,SIGNAL(signal_quick_look_preview_filepath_from_table_result_loader(QString)),this,SIGNAL(signal_quick_look_preview_filepath_from_table_result_loader(QString)),Qt::QueuedConnection);

    connect(story_board_loader_obj,SIGNAL(signal_save_data()),this,SLOT(slot_save_data()),Qt::QueuedConnection);

    connect(this, SIGNAL(signal_checkbox_blurr_status(bool)), story_board_loader_obj, SLOT(slot_checkbox_blurr_status(bool)));
    connect(this, SIGNAL(signal_checkBox_censor_skintone_tag_images_clicked(bool)), story_board_loader_obj, SLOT(slot_checkBox_censor_skintone_tag_images_clicked(bool)));

    connect(story_board_loader_obj,SIGNAL(signal_tablewidget_goto_record_snapshots(QString,QString,QString,QString)),this,SIGNAL(signal_tablewidget_goto_record_snapshots(QString,QString,QString,QString)),Qt::QueuedConnection);


    bool_combobox_filling = false;

    ui->pushButton_show_hide_tag_table->setIcon(QIcon("../icons/Features/show_full_display.png"));

    progress_loading_obj = new display_loading_progress_bar(this);
    progress_loading_obj->pub_set_hidden_cancel_button(true);
    progress_loading_obj->pub_set_progress_bar_indefinite_value();
    progress_loading_obj->pub_set_label_messsge("Please wait...");

}

story_board::~story_board()
{
    delete ui;
}

void story_board::pub_set_story_board_file_name_and_path(QString file_name, QString file_path)
{
    story_file_name = file_name;
    story_file_path = file_path;
}


void story_board::pub_set_initialisation()
{
    story_board_loader_obj->pub_set_essentials();
    story_board_loader_obj->pub_set_feature_name(MACRO_Plugin_Name_Story_Board);
    story_board_loader_obj->pub_set_plugin_name(MACRO_Plugin_Name_Story_Board);

    story_board_loader_obj->pub_set_story_file_name_and_path(story_file_name,story_file_path);

    QStringList tab_list;
    tab_list << QString(MACRO_Plugin_Name_Story_Board);

    story_board_loader_obj->pub_set_display_tab_name_list(tab_list);
    story_board_loader_obj->pub_create_ui();

    //    QString csv_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Feature_CSV_In_Result_QString).toString() + "tag_search_csv/";
    //    story_board_loader_obj->pub_set_csv_dir_path(csv_path);

    display_tags_combobox();
    story_board_loader_obj->pub_set_table_display_essentials();
    display_tags_table_data();

    story_board_textedit_obj->pub_set_story_file_name_and_path(story_file_name,story_file_path);
    story_board_textedit_obj->pub_set_story_default_export_path(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString());

}

void story_board::pub_set_previous_saved_data_in_textedit(QString filepath)
{
    story_board_textedit_obj->load_file(filepath);
}

void story_board::pub_add_and_save_case_details_in_textedit(QString filepath)
{
    /// Only add case details and save them if file is blank.

    QString file_size = recon_static_functions::mining_size(filepath , Q_FUNC_INFO);
    if(file_size == QString::number(0))
    {
        ///set agency logo
        QString agency      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString();
        QString logo_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Logo_QString).toString();

        QString icon_data_str = "data:image/png;base64,";
        QFile file(logo_path);
        if(file.open(QIODevice::ReadOnly))
        {
            icon_data_str.append(QString::fromLocal8Bit(file.readAll().toBase64()));
        }

        QString agency_details = report_template_html_standard::html_report_main_title(icon_data_str,agency);

        QString case_details = agency_details;
        case_details += global_recon_helper_singular_class_obj->write_html_case_info_and_source_info(QString(""), global_narad_muni_class_obj);

        story_board_textedit_obj->pub_set_case_details_to_story_board(case_details);
        story_board_textedit_obj->pub_save_storyboard();
    }
}

void story_board::pub_update_combobox_and_tablewidget_display()
{
    display_tags_combobox();
    display_tags_table_data();
}

void story_board::on_pushButton_show_hide_tag_table_clicked(bool checked)
{
    story_board_loader_obj->setHidden(checked);
    ui->comboBox_tags->setDisabled(checked);

    if(checked)
    {
        QList<int> list_size;
        list_size << 0 << 250;
        ui->splitter->setSizes(list_size);
        ui->splitter->setCollapsible(0,true);
        ui->splitter->setCollapsible(1,false);
    }
    else
    {
        QList<int> list_size;
        list_size  << 250 << 250;
        ui->splitter->setSizes(list_size);
        ui->splitter->setCollapsible(0,false);
        ui->splitter->setCollapsible(1,false);
    }
}

void story_board::display_tags_combobox()
{
    bool_combobox_filling = true;

    int current_index = ui->comboBox_tags->currentIndex();

    ui->comboBox_tags->clear();

    QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";
    QString command = QString("select tag_name from tag_search_index");
    QStringList list_tags_name;
    list_tags_name << recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,destination_db_path,Q_FUNC_INFO);

    QIcon icon_all_tags(QString("../icons/tag_icons/") + QString(MACRO_Tags_All_Tags_Title).replace(" " , "_").toLower() + QString(".png"));
    ui->comboBox_tags->addItem(icon_all_tags , QString(" ") +  QString(MACRO_Tags_All_Tags_Title));


    QIcon icon_bookmarks(QString("../icons/tag_icons/") + QString(MACRO_Tag_Name_Bookmarks).replace(" " , "_").toLower() + QString(".png"));
    ui->comboBox_tags->addItem(icon_bookmarks , QString(" ") +  QString(MACRO_Tag_Name_Bookmarks));

    QString configuration_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";


    for(int i = 0; i< list_tags_name.size(); i++)
    {
        QString tag_name_str = list_tags_name.at(i);
        tag_name_str = tag_name_str.trimmed();

        if(tag_name_str.isEmpty())
            continue;

        QString command_str = QString("SELECT selected_colour FROM tags where tag_name = '") + tag_name_str + "'";
        QString selected_color_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(command_str , 0 , configuration_db_path , Q_FUNC_INFO);

        QString icon_path = recon_static_functions::get_icon_path_from_colour_name(selected_color_str , Q_FUNC_INFO);

        QIcon icon(icon_path);
        ui->comboBox_tags->addItem(icon," " + tag_name_str);
    }

    QIcon icon_notes(QString("../icons/Features/notes.png"));
    ui->comboBox_tags->addItem(icon_notes , QString(" ") +  QString("Notes"));


    QIcon icon_timeline_saved(QString("../icons/Features/timeline.png"));
    ui->comboBox_tags->addItem(icon_timeline_saved , QString(" ") +  QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph));

    QIcon icon_maps_saved(QString("../icons/Features/locations.png"));
    ui->comboBox_tags->addItem(icon_maps_saved , QString(" ") +  QString(MACRO_Plugin_Name_Saved_Maps));


    QIcon icon_messenger_Graphs(QString("../icons/Features/graphs.png"));
    ui->comboBox_tags->addItem(icon_messenger_Graphs , QString(" ") +  QString(MACRO_Plugin_Name_Saved_Graphs_Messenger));

    QIcon icon_browswers_Graphs(QString("../icons/Features/graphs.png"));
    ui->comboBox_tags->addItem(icon_browswers_Graphs , QString(" ") +  QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History));

    QIcon icon_screenshot(QString("../icons/Features/") + QString(MACRO_Plugin_Name_Screenshots).replace(" " , "_").toLower() + QString(".png"));
    ui->comboBox_tags->addItem(icon_screenshot , QString(" ") +  QString(MACRO_Plugin_Name_Screenshots));

    QIcon icon_examiner_notes(QString("../icons/Features/") + QString(MACRO_Plugin_Name_Examiner_Space).replace(" " , "_").toLower() + QString(".png"));
    ui->comboBox_tags->addItem(icon_examiner_notes , QString(" ") +  QString(MACRO_Plugin_Name_Examiner_Space));

    if((ui->comboBox_tags->count() > current_index) && (current_index != -1))
        ui->comboBox_tags->setCurrentIndex(current_index);

    bool_combobox_filling = false;
}

void story_board::display_tags_table_data()
{

    story_board_loader_obj->pub_set_current_tag_name(ui->comboBox_tags->currentText().trimmed());
    story_board_loader_obj->pub_set_story_board_loader_essential();

    /// All Tags
    if(ui->comboBox_tags->currentText().trimmed() == QString(MACRO_Tags_All_Tags_Title))
    {
        int check_clear = 0;

        for(int count = 0; count < ui->comboBox_tags->count(); count++)
        {
            QString tab_db_name = ui->comboBox_tags->itemText(count).trimmed();

            if(tab_db_name == QString(MACRO_Tags_All_Tags_Title) || tab_db_name.isEmpty() )
                continue;



            QString file_path;

            QString db_path;
            if(tab_db_name == QString(MACRO_Tag_Name_Bookmarks))
            {
                story_board_loader_obj->pub_set_tag_icon_path(QString(""));

                story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/tag_icons/") + QString(MACRO_Tag_Name_Bookmarks).replace(" " , "_").toLower() + QString(".png"));
                tab_db_name.replace(" ","_");
                //                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + tab_db_name + ".sqlite";
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
            }
            else if(tab_db_name == QString("Notes"))
            {
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
                story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/Features/notes.png"));
            }
            else if(tab_db_name == QString(MACRO_Plugin_Name_Screenshots))
            {
                story_board_loader_obj->pub_set_tag_icon_path(QString(""));

                story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/Features/") + QString(MACRO_Plugin_Name_Screenshots).replace(" " , "_").toLower() + QString(".png"));
                tab_db_name.replace(" ","_");
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + tab_db_name + ".sqlite";
            }
            else if(tab_db_name == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
            {
                story_board_loader_obj->pub_set_tag_icon_path(QString(""));

                story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/Features/timeline.png"));
                tab_db_name.replace(" ","_");
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";
            }
            else if(tab_db_name == QString(MACRO_Plugin_Name_Saved_Maps))
            {
                story_board_loader_obj->pub_set_tag_icon_path(QString(""));

                story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/Features/locations.png"));
                tab_db_name.replace(" ","_");
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";
            }
            else if(tab_db_name == MACRO_Plugin_Name_Saved_Graphs_Messenger)
            {
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";
                story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/Features/graphs.png"));
            }
            else if(tab_db_name == MACRO_Plugin_Name_Saved_Graphs_Browser_History)
            {
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite";
                story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/Features/graphs.png"));
            }
            else if(tab_db_name == MACRO_Plugin_Name_Examiner_Space)
            {
                story_board_loader_obj->pub_set_tag_icon_path(QString(""));

                story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/Features/examiner_space.png"));
                tab_db_name.replace(" ","_");
                file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Examiner_Space_In_Result_QString).toString() + "Examiner_Notes.html";
                if(QFileInfo(file_path).exists())
                    story_board_loader_obj->pub_set_examiner_notes_file_path(file_path);
            }
            else
            {
                story_board_loader_obj->pub_set_feature_icon_path(QString(""));

                QString icon_str = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tab_db_name, global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                icon_str = icon_str.toLower();
                icon_str = icon_str.replace(" ","_");
                icon_str = "../icons/tag_icons/"+ icon_str +".png";

                story_board_loader_obj->pub_set_tag_icon_path(icon_str);

                tab_db_name.replace(" ","_");
                db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tab_db_name + ".sqlite";
            }

            story_board_loader_obj->pub_tag_search_set_display_db_path(db_path);
            story_board_loader_obj->pub_set_clicked_item_tab_text(ui->comboBox_tags->currentText().trimmed());

            if(check_clear == 0)
                story_board_loader_obj->pub_set_clear_table_display(true);
            else
                story_board_loader_obj->pub_set_clear_table_display(false);

            story_board_loader_obj->populate_data_in_table();

            story_board_loader_obj->pub_set_bool_for_right_click_actions(false);

            check_clear++;
        }
    }
    else
    {
        /// Selected Tag
        QString tab_db_name = ui->comboBox_tags->currentText().trimmed();
        if(tab_db_name.isEmpty())
            return;


        QString file_path;

        QString db_path;
        if(tab_db_name == QString(MACRO_Tag_Name_Bookmarks))
        {
            story_board_loader_obj->pub_set_tag_icon_path(QString(""));

            story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/tag_icons/") + QString(MACRO_Tag_Name_Bookmarks).replace(" " , "_").toLower() + QString(".png"));
            tab_db_name.replace(" ","_");
            //            db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + tab_db_name + ".sqlite";
            db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";

        }
        else if(tab_db_name == QString(MACRO_Plugin_Name_Screenshots))
        {
            story_board_loader_obj->pub_set_tag_icon_path(QString(""));

            story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/Features/") + QString(MACRO_Plugin_Name_Screenshots).replace(" " , "_").toLower() + QString(".png"));
            tab_db_name.replace(" ","_");
            db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + tab_db_name + ".sqlite";
        }
        else if(tab_db_name == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
        {
            story_board_loader_obj->pub_set_tag_icon_path(QString(""));

            story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/Features/timeline.png"));
            tab_db_name.replace(" ","_");
            db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";
        }
        else if(tab_db_name == QString(MACRO_Plugin_Name_Saved_Maps))
        {
            story_board_loader_obj->pub_set_tag_icon_path(QString(""));

            story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/Features/locations.png"));
            tab_db_name.replace(" ","_");
            db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";
        }
        else if(tab_db_name == QString("Notes"))
        {
            db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
            story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/Features/notes.png"));
        }
        else if(tab_db_name == MACRO_Plugin_Name_Saved_Graphs_Messenger)
        {
            db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";
            story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/Features/graphs.png"));
        }
        else if(tab_db_name == MACRO_Plugin_Name_Saved_Graphs_Browser_History)
        {
            db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite";
            story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/Features/graphs.png"));
        }
        else if(tab_db_name == MACRO_Plugin_Name_Examiner_Space)
        {
            story_board_loader_obj->pub_set_tag_icon_path(QString(""));

            story_board_loader_obj->pub_set_feature_icon_path(QString("../icons/Features/examiner_space.png"));
            tab_db_name.replace(" ","_");
            file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Examiner_Space_In_Result_QString).toString() + "Examiner_Notes.html";
            if(QFileInfo(file_path).exists())
                story_board_loader_obj->pub_set_examiner_notes_file_path(file_path);
        }
        else
        {
            story_board_loader_obj->pub_set_feature_icon_path(QString(""));

            QString icon_str = recon_helper_standard_obj->get_colour_name_of_tag_from_db(tab_db_name, global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
            icon_str = icon_str.toLower();
            icon_str = icon_str.replace(" ","_");
            icon_str = "../icons/tag_icons/"+ icon_str +".png";

            story_board_loader_obj->pub_set_tag_icon_path(icon_str);

            tab_db_name.replace(" ","_");

            db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tab_db_name + ".sqlite";
        }

        story_board_loader_obj->pub_tag_search_set_display_db_path(db_path);
        story_board_loader_obj->pub_set_clicked_item_tab_text(ui->comboBox_tags->currentText().trimmed());
        story_board_loader_obj->pub_set_clear_table_display(true);
        story_board_loader_obj->populate_data_in_table();
        story_board_loader_obj->pub_set_bool_for_right_click_actions(false);

    }

    ui->verticalLayout_story_board_loader_table->addWidget(story_board_loader_obj);

    story_board_loader_obj->populate_data_in_timeline_display();

}

void story_board::on_comboBox_tags_currentTextChanged(const QString &arg1)
{
    ui->lineEdit_search->clear();

    if(bool_combobox_filling)
        return;



    emit signal_tags_combobox_index_changed();
    story_board_loader_obj->pub_set_current_tag_name(arg1);

    story_board_loader_obj->pub_clear_all_custom_header_widget_entries();
    display_tags_table_data();

}

void story_board::slot_add_record_to_story_board(QString metadata, QString media_path)
{
    story_board_textedit_obj->pub_add_data_to_story_board(metadata,media_path);
}

void story_board::slot_add_file_to_story_board(QString actual_filename, QString filepath, bool blurr_status, QString label_file_path)
{

    filepath = filepath.trimmed();
    if(filepath.isEmpty())
        return;


    if(recon_helper_standard_obj->check_is_it_image_file(filepath,"" , Q_FUNC_INFO))
    {
        story_board_textedit_obj->pub_add_image_file_to_story(filepath, blurr_status);
    }
    else
        story_board_textedit_obj->pub_add_other_file_to_story(actual_filename, filepath);

    if(!label_file_path.trimmed().isEmpty() && !label_file_path.startsWith("/"))
        label_file_path.prepend("/");

    story_board_textedit_obj->pub_add_data_to_story_board(label_file_path, QString(""));
    story_board_textedit_obj->pub_add_data_to_story_board(QString("<br>"), QString(""));
    story_board_textedit_obj->pub_save_storyboard();

}

void story_board::slot_on_cancel_from_progress_bar()
{
    story_board_textedit_obj->pub_cancel_copying_data();
}

void story_board::slot_save_data()
{
    story_board_textedit_obj->pub_save_storyboard();
}

void story_board::slot_refresh_storyboard_display()
{
    display_tags_combobox();
    on_pushButton_show_all_clicked();
}

void story_board::slot_show_all_clicked()
{
    progress_loading_obj->show();

    display_tags_combobox();
    display_tags_table_data();

    progress_loading_obj->hide();
}

void story_board::on_lineEdit_search_returnPressed()
{
    QString current_text = ui->lineEdit_search->text();
    story_board_loader_obj->pub_story_board_search_clicked(current_text);

}

void story_board::on_pushButton_search_clicked()
{
    QString current_text = ui->lineEdit_search->text();
    story_board_loader_obj->pub_story_board_search_clicked(current_text);
}

void story_board::on_pushButton_show_all_clicked()
{
    ui->lineEdit_search->setText(QString(""));
    story_board_loader_obj->pub_story_board_show_all_clicked();
}



void story_board::on_checkBox_blurr_image_clicked(bool checked)
{
    emit signal_checkbox_blurr_status(checked);
}


void story_board::prepare_export_dialog()
{
    recon_static_functions::debug_intensive(" - Starts " ,Q_FUNC_INFO);

    QFont font_s;
    font_s.setPixelSize(12);

    //-Create Dialog for adding Catagories.
    dialog_export = new QDialog(this);
    dialog_export->setWindowModality(Qt::WindowModal);

    QVBoxLayout *vbox_main = new QVBoxLayout;


    QLabel *label = new QLabel("Report Exported.");

    QHBoxLayout *hbox_pushbutton = new QHBoxLayout;


    QPushButton * pushbutton_open  = new QPushButton("Open", this);
    QPushButton * pushbutton_close = new QPushButton("Close", this);

    pushbutton_close->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_open->setAttribute(Qt::WA_MacShowFocusRect,false);

    connect(pushbutton_open,SIGNAL(clicked()),this,SLOT(slot_pushbutton_open_clicked()));
    connect(pushbutton_close,SIGNAL(clicked()),this,SLOT(slot_pushbutton_close_clicked()));


    hbox_pushbutton->addWidget(pushbutton_open);
    hbox_pushbutton->addWidget(pushbutton_close);


    vbox_main->addWidget(label);
    vbox_main->addLayout(hbox_pushbutton);

    dialog_export->setLayout(vbox_main);
    dialog_export->setWindowTitle(QString("Export"));
    dialog_export->setMinimumWidth(200);
    dialog_export->setFont(font_s);

    dialog_export->setFixedSize(QSize(500, 200));

    recon_static_functions::debug_intensive(" - Ends " ,Q_FUNC_INFO);

}

void story_board::slot_pushbutton_open_clicked()
{
    dialog_export->hide();

}

void story_board::slot_pushbutton_close_clicked()
{
    dialog_export->hide();

}

void story_board::on_checkBox_censor_skintone_tag_images_clicked(bool checked)
{
    emit signal_checkBox_censor_skintone_tag_images_clicked(checked);

}
