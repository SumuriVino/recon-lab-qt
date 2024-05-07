#include "file_search_interface.h"
#include "ui_file_search_interface.h"

file_search_interface::file_search_interface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::file_search_interface)
{
    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    pub_create_ui();

    setFixedWidth(780);

    bool_run_in_full_mode = true;
    parent_dir_path_for_search.clear();

    message_box_obj = new message_dialog(this);
}

file_search_interface::~file_search_interface()
{
    delete ui;
}

void file_search_interface::pub_set_essentials()
{
    regular_filters_obj->reset_all_fields_of_ui();

    spotlight_filters_obj->pub_set_essentials();
    apple_metadata_filters_obj->pub_set_essentials();

    set_linedit_search_label_placeholder_text();
}

void file_search_interface::pub_set_bool_run_full_mode(bool bool_mode)
{
    bool_run_in_full_mode = bool_mode;
}

void file_search_interface::pub_set_parent_dir_path(QString parent_dir_path)
{
    parent_dir_path_for_search = parent_dir_path;
}

void file_search_interface::pub_set_file_system_source_count_name(QString source_count_name)
{
    file_system_source_count_name = source_count_name;
}


void file_search_interface::pub_create_ui()
{ // set search interface where we set multiple searches widgets on this interface like regular search, spotlight search, metadata etc.
    stack_widget_filters = new QStackedWidget(this);

    QVBoxLayout *vlayout_main = new QVBoxLayout(this);

    hlayout_search_and_cancel = new QHBoxLayout;

    // Set UI for File Search
    set_ui_for_regular_file_search();

    // Set UI for Spotlight Search
    set_ui_for_spotlight_search();

    set_ui_for_apple_meatdata_search();

    setup_ui_for_any_all_filters();
    set_ui_for_search_and_cancel();

    stack_widget_filters->addWidget(groupbox_regular_search);
    stack_widget_filters->addWidget(groupbox_apple_metadata_search);
    stack_widget_filters->setCurrentIndex(enum_global_stack_widget_file_search_regular);
    // vlayout_main->addWidget(groupbox_regular_search);
    // vlayout_main->addWidget(groupbox_apple_metadata_search);
    vlayout_main->addWidget(stack_widget_filters);
    vlayout_main->addWidget(group_box_any_all_filter_buttons);

    vlayout_main->addLayout(hlayout_search_and_cancel);

    vlayout_main->setSpacing(0);
    //  vlayout_main->setSpacing(25);
    vlayout_main->setContentsMargins(4,4,4,4);

    set_all_connections();

    setLayout(vlayout_main);
}
// below we have set ui for their searches section
void file_search_interface::set_ui_for_regular_file_search()
{
    regular_filters_obj = new regular_filters(this);
    regular_filters_obj->create_ui_for_global_filters_search();
    regular_filters_obj->set_parent_dir_path_for_extraction("/");        // Because Initially its would work on whole file system entries


    groupbox_regular_search = new QGroupBox(this);
    QHBoxLayout *hlayout_file_search = new QHBoxLayout(groupbox_regular_search);


    hlayout_file_search->addWidget(regular_filters_obj);
    hlayout_file_search->setContentsMargins(4,4,4,4);
    hlayout_file_search->setSpacing(0);

    groupbox_regular_search->setLayout(hlayout_file_search);
    groupbox_regular_search->setFixedWidth(770);
    groupbox_regular_search->setFixedHeight(200);
    groupbox_regular_search->setContentsMargins(4,4,4,4);
    groupbox_regular_search->setTitle("Regular Filters");


}

void file_search_interface::set_ui_for_spotlight_search()
{
    groupbox_spotlight_search = new QGroupBox();
    QVBoxLayout *vlayout = new QVBoxLayout(groupbox_spotlight_search);

    QSpacerItem *spacer_vertical = new QSpacerItem(10,60,QSizePolicy::Minimum,QSizePolicy::Expanding);

    spotlight_filters_obj = new spotlight_filters(this);

    QHBoxLayout *hlayout_spotlight_search = new QHBoxLayout(vlayout->widget());
    hlayout_spotlight_search->addWidget(spotlight_filters_obj);
    hlayout_spotlight_search->setContentsMargins(0,0,0,0);
    hlayout_spotlight_search->setSpacing(0);

    vlayout->addLayout(hlayout_spotlight_search);
    vlayout->addSpacerItem(spacer_vertical);
    vlayout->setSpacing(4);
    vlayout->setContentsMargins(0,0,0,0);

    groupbox_spotlight_search->setAlignment(Qt::AlignTop);
    groupbox_spotlight_search->setFixedWidth(770);
    groupbox_spotlight_search->setFixedHeight(220);
    groupbox_spotlight_search->setLayout(vlayout);
    groupbox_spotlight_search->setContentsMargins(4,4,4,4);
    groupbox_spotlight_search->setTitle("Spotlight Filters");

}

void file_search_interface::set_ui_for_apple_meatdata_search()
{
    groupbox_apple_metadata_search = new QGroupBox(this);
    QVBoxLayout *vlayout = new QVBoxLayout(groupbox_apple_metadata_search);

    QSpacerItem *spacer_vertical = new QSpacerItem(10,60,QSizePolicy::Minimum,QSizePolicy::Expanding);

    apple_metadata_filters_obj = new apple_metadata_filters(this);

    QHBoxLayout *hlayout_spotlight_search = new QHBoxLayout(vlayout->widget());
    hlayout_spotlight_search->addWidget(apple_metadata_filters_obj);
    hlayout_spotlight_search->setContentsMargins(0,0,0,0);
    hlayout_spotlight_search->setSpacing(0);

    vlayout->addLayout(hlayout_spotlight_search);
    vlayout->addSpacerItem(spacer_vertical);
    vlayout->setSpacing(4);
    vlayout->setContentsMargins(0,0,0,0);

    groupbox_apple_metadata_search->setAlignment(Qt::AlignTop);
    groupbox_apple_metadata_search->setFixedWidth(770);
    groupbox_apple_metadata_search->setFixedHeight(220);
    groupbox_apple_metadata_search->setLayout(vlayout);
    groupbox_apple_metadata_search->setContentsMargins(4,4,4,4);
    groupbox_apple_metadata_search->setTitle("Apple Metadata");

}

void file_search_interface::setup_ui_for_any_all_filters()
{   
    group_box_any_all_filter_buttons = new QGroupBox;

    radioButton_any_filter = new QRadioButton(this);
    radioButton_any_filter->setText("Any Filter");
    radioButton_any_filter->setFixedWidth(200);
    radioButton_any_filter->setFixedHeight(25);

    radioButton_all_filters = new QRadioButton(this);
    radioButton_all_filters->setText("All Filters");
    radioButton_all_filters->setChecked(true);

    hbox_any_all_filters_buttons = new QHBoxLayout;
    hbox_any_all_filters_buttons->setContentsMargins(0, 0, 0, 0);
    hbox_any_all_filters_buttons->addWidget(radioButton_all_filters);
    hbox_any_all_filters_buttons->addWidget(radioButton_any_filter);
    hbox_any_all_filters_buttons->addStretch();

    group_box_any_all_filter_buttons->setLayout(hbox_any_all_filters_buttons);
    group_box_any_all_filter_buttons->setContentsMargins(15,0,0,0);
    group_box_any_all_filter_buttons->setMaximumHeight(30);
}

void file_search_interface::set_ui_for_search_and_cancel()
{
    QFont myfont;
    myfont.setPointSize(MACRO_QMenu_Font_Size_int);

    pushbutton_search = new QPushButton(this);
    pushbutton_search->setText("Search");
    pushbutton_search->setFocusPolicy(Qt::NoFocus);
    pushbutton_search->setFont(myfont);

    pushbutton_cancel = new QPushButton(this);
    pushbutton_cancel->setText("Cancel");
    pushbutton_cancel->setFocusPolicy(Qt::NoFocus);
    pushbutton_cancel->setFont(myfont);

    label_file_search_name = new QLabel(this);
    label_file_search_name->setText("Search Label");
    label_file_search_name->setFont(myfont);

    line_edit_search_label = new QLineEdit(this);
    line_edit_search_label->setAttribute(Qt::WA_MacShowFocusRect,0);
    line_edit_search_label->setFont(myfont);

    //    QSpacerItem *spacer_horrizontal = new QSpacerItem(60,10,QSizePolicy::Expanding);

    //    hlayout_search_and_cancel->addSpacerItem(spacer_horrizontal);
    hlayout_search_and_cancel->addWidget(label_file_search_name);
    hlayout_search_and_cancel->addWidget(line_edit_search_label);
    hlayout_search_and_cancel->addWidget(pushbutton_cancel);
    hlayout_search_and_cancel->addWidget(pushbutton_search);

    hlayout_search_and_cancel->setSpacing(4);
    hlayout_search_and_cancel->setContentsMargins(4,4,4,4);
}

void file_search_interface::set_all_connections()
{
    connect(pushbutton_search, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_search_clicked(bool)));
    connect(pushbutton_cancel, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_cancel_clicked(bool)));
}

void file_search_interface::slot_pushbutton_search_clicked(bool checked)
{


    Q_UNUSED(checked);

    struct_global_selected_regular_filters_info struct_file_search_selected_fields_info;
    if(file_search_filter_type == enum_global_stack_widget_file_search_regular)
    {
        struct_file_search_selected_fields_info = regular_filters_obj->pub_collect_all_selected_fields_info();
        struct_file_search_selected_fields_info.bool_is_combined_by_or = radioButton_all_filters->isChecked();

        if(struct_file_search_selected_fields_info.bool_is_structure_is_empty)
        {
            message_box_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please Select atleast one filter to search");
            message_box_obj->show();
            return;
        }

    }

    QString mdfind_condition;
    if(file_search_filter_type == enum_global_stack_widget_file_search_apple_metadata)
    {
        apple_metadata_filters_obj->pub_set_all_filter_status(radioButton_all_filters->isChecked());
        mdfind_condition  = apple_metadata_filters_obj->pub_collect_all_spotlight_search_filters_information();

        if(mdfind_condition.trimmed().isEmpty())
        {
            message_box_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please Select atleast one filter to search");
            message_box_obj->show();
            return;
        }

    }


    this->hide();

    QString search_label_name = line_edit_search_label->text().trimmed();

    if(search_label_name.isEmpty())
        search_label_name = line_edit_search_label->placeholderText().trimmed();

    emit signal_start_file_search_thread(struct_file_search_selected_fields_info, mdfind_condition, search_label_name, bool_run_in_full_mode, parent_dir_path_for_search, file_system_source_count_name);
}

void file_search_interface::slot_pushbutton_cancel_clicked(bool checked)
{
    this->hide();
}

void file_search_interface::set_linedit_search_label_placeholder_text()
{
    QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString() + "index_file_search.sqlite";

    QString command = "SELECT COUNT(*) FROM file_search_index";
    QString value = recon_helper_standard_obj->get_string_from_db_by_dbpath(command, 0, index_db_path, Q_FUNC_INFO);

    line_edit_search_label->clear();
    line_edit_search_label->setPlaceholderText("Search Label " + QString::number(value.toInt() + 1));
}

void file_search_interface::pub_hide_apple_filters_for_non_mac_source()
{
    groupbox_spotlight_search->hide();
    groupbox_apple_metadata_search->hide();

}

void file_search_interface::pub_set_file_search_stack_widget_index(enum_stack_widget_file_search_filters index)
{
    stack_widget_filters->setCurrentIndex(index);

    file_search_filter_type = index;

    if(index == enum_global_stack_widget_file_search_regular)
    {
        setWindowTitle("File Search - Regular Filters");
        setFixedHeight(270);
    }
    else {
        setWindowTitle("File Search - Apple Metadata");
        setFixedHeight(290);
    }

}

