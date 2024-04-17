#include "regular_filters.h"

void regular_filters::create_ui_full()
{
    vbox_layout_main = new QVBoxLayout(this);

    vbox_layout_file_name_and_size = new QVBoxLayout;
    vbox_layout_file_name_and_size->setContentsMargins(0,0,0,0);
    vbox_layout_file_name_and_size->setSpacing(4);

    vboxlayout_all_dates = new QVBoxLayout;
    vboxlayout_all_dates->setContentsMargins(0,0,0,0);
    vboxlayout_all_dates->setSpacing(0);

    vbox_mode_selection_radio_button = new QVBoxLayout;
    vbox_mode_selection_radio_button->setContentsMargins(0,0,0,0);
    vbox_mode_selection_radio_button->setSpacing(0);

    vboxLayout_file_search_option = new QVBoxLayout;
    vboxLayout_file_search_option->setContentsMargins(0,0,0,0);
    vboxLayout_file_search_option->setSpacing(0);

    vboxLayout_search_and_cancel = new QVBoxLayout;
    vboxLayout_search_and_cancel->setContentsMargins(0,0,0,0);
    vboxLayout_search_and_cancel->setSpacing(0);

    set_ui_for_file_name();
    set_ui_for_file_size();

    set_ui_for_all_dates();

    set_ui_mode_selection_radio_button();

    set_ui_for_search_option();

    set_ui_for_search_and_cancel();

    vbox_layout_main->addLayout(vbox_layout_file_name_and_size);
    vbox_layout_main->addLayout(vboxlayout_all_dates);
    vbox_layout_main->addLayout(vbox_mode_selection_radio_button);
    vbox_layout_main->addLayout(vboxLayout_file_search_option);
    vbox_layout_main->addLayout(vboxLayout_search_and_cancel);

    // vbox_layout_main->setSpacing(10);
    vbox_layout_main->setSpacing(0);
    //    vbox_layout_main->setContentsMargins(12,12,12,12);
    vbox_layout_main->setContentsMargins(0,0,0,0);

    setLayout(vbox_layout_main);

    all_ui_connections();

    setFixedWidth(530);
    setFixedHeight(150);
}

void regular_filters::create_ui_for_global_filters_search()
{
    vbox_layout_main = new QVBoxLayout(this);

    vbox_layout_file_name_and_size = new QVBoxLayout;
    vbox_layout_file_name_and_size->setContentsMargins(0,0,0,0);
    vbox_layout_file_name_and_size->setSpacing(4);

    vboxlayout_all_dates = new QVBoxLayout;
    vboxlayout_all_dates->setContentsMargins(0,0,0,0);
    vboxlayout_all_dates->setSpacing(0);

    vbox_mode_selection_radio_button = new QVBoxLayout;
    vbox_mode_selection_radio_button->setContentsMargins(0,0,0,0);
    vbox_mode_selection_radio_button->setSpacing(0);

    vboxLayout_file_search_option = new QVBoxLayout;
    vboxLayout_file_search_option->setContentsMargins(0,0,0,0);
    vboxLayout_file_search_option->setSpacing(0);

    vboxLayout_search_and_cancel = new QVBoxLayout;
    vboxLayout_search_and_cancel->setContentsMargins(0,0,0,0);
    vboxLayout_search_and_cancel->setSpacing(0);

    set_ui_for_file_name();

    set_ui_for_file_size();

    set_ui_for_all_dates();

    set_ui_mode_selection_radio_button();


    set_ui_for_search_option();

    set_ui_for_search_and_cancel();

    pushbutton_search->hide();
    pushbutton_cancel->hide();
    label_file_search->hide();
    lineEdit_file_search->hide();

    vbox_layout_main->addLayout(vbox_layout_file_name_and_size);
    vbox_layout_main->addLayout(vboxlayout_all_dates);
    vbox_layout_main->addLayout(vbox_mode_selection_radio_button);
    vbox_layout_main->addLayout(vboxLayout_file_search_option);
    vbox_layout_main->addLayout(vboxLayout_search_and_cancel);

    vbox_layout_main->setSpacing(10);
    vbox_layout_main->setContentsMargins(12,12,12,12);

    setLayout(vbox_layout_main);

    all_ui_connections();

    setFixedWidth(770);
    setFixedHeight(190);
}


void regular_filters::set_ui_for_file_name()
{
    QHBoxLayout *hboxlayout = new QHBoxLayout;

    QFont myfont;
    myfont.setPointSize(MACRO_QMenu_Font_Size_int);

    label_file_name = new QLabel(this);
    label_file_name->setText("File name");
    label_file_name->setFixedWidth(60);
    label_file_name->setFont(myfont);

    QStringList list_combox_entry;
    list_combox_entry << MACRO_Generic_Filter_Condition_Contains << MACRO_Generic_Filter_Condition_Matches
                      << MACRO_Generic_Filter_Condition_Begins_With << MACRO_Generic_Filter_Condition_Ends_With;
                      //<< MACRO_Generic_Filter_Condition_Is << MACRO_Generic_Filter_Condition_Is_Not;

    combobox_file_name_filters = new QComboBox(this);
    combobox_file_name_filters->addItems(list_combox_entry);
    combobox_file_name_filters->setFont(myfont);

    lineEdit_file_name = new QLineEdit(this);
    lineEdit_file_name->setAttribute(Qt::WA_MacShowFocusRect,0);
    lineEdit_file_name->setFont(myfont);

    hboxlayout->addWidget(label_file_name);
    hboxlayout->addWidget(lineEdit_file_name);
    hboxlayout->addWidget(combobox_file_name_filters);
    hboxlayout->setSpacing(10);
    hboxlayout->setContentsMargins(0,0,0,0);

    vbox_layout_file_name_and_size->addLayout(hboxlayout);
}

void regular_filters::set_ui_for_file_size()
{
    QHBoxLayout *hboxlayout = new QHBoxLayout;

    QFont myfont;
    myfont.setPointSize(MACRO_QMenu_Font_Size_int);

    label_file_size = new QLabel(this);
    label_file_size->setText("File size");
    label_file_size->setFixedWidth(60);
    label_file_size->setFont(myfont);

    combobox_file_size_type = new QComboBox(this);
    QStringList combobox_type_items_list;
    combobox_type_items_list << MACRO_Generic_Filter_Condition_Is_Equal
                             << MACRO_Generic_Filter_Condition_Between
                             << MACRO_Generic_Filter_Condition_Is_Greater_Than
                             << MACRO_Generic_Filter_Condition_Is_Less_Than;
    combobox_file_size_type->addItems(combobox_type_items_list);
    combobox_file_size_type->setFont(myfont);

    lineEdit_file_size_1 = new QLineEdit(this);
    lineEdit_file_size_1->setAttribute(Qt::WA_MacShowFocusRect,0);
    lineEdit_file_size_1->setFont(myfont);

//    QRegularExpression rx1("\\d+");
//    QValidator *validator1 = new QRegularExpressionValidator(rx1, this);
//    lineEdit_file_size_1->setValidator(validator1);

    label_file_size_between_to = new QLabel(this);
    label_file_size_between_to->setText("and");
    label_file_size_between_to->setFont(myfont);

    lineEdit_file_size_2 = new QLineEdit(this);
    lineEdit_file_size_2->setAttribute(Qt::WA_MacShowFocusRect,0);
    lineEdit_file_size_2->setFont(myfont);

//    QRegularExpression rx2("\\d+");
//    QValidator *validator2 = new QRegularExpressionValidator(rx2, this);
//    lineEdit_file_size_2->setValidator(validator2);

    combobox_file_size_sufix = new QComboBox(this);
    QStringList combobox_suffix_items_list;
    combobox_suffix_items_list << MACRO_Generic_Filter_Condition_Bytes
                               << MACRO_Generic_Filter_Condition_KB
                               << MACRO_Generic_Filter_Condition_MB
                               << MACRO_Generic_Filter_Condition_GB
                               << MACRO_Generic_Filter_Condition_TB;
    combobox_file_size_sufix->addItems(combobox_suffix_items_list);
    combobox_file_size_sufix->setFont(myfont);

    hboxlayout->addWidget(label_file_size);
    hboxlayout->addWidget(combobox_file_size_type);
    hboxlayout->addWidget(lineEdit_file_size_1);
    hboxlayout->addWidget(label_file_size_between_to);
    hboxlayout->addWidget(lineEdit_file_size_2);
    hboxlayout->addWidget(combobox_file_size_sufix);
    hboxlayout->setSpacing(10);
    hboxlayout->setContentsMargins(0,0,0,0);

    label_file_size_between_to->hide();
    lineEdit_file_size_2->hide();

    vbox_layout_file_name_and_size->addLayout(hboxlayout);
}

void regular_filters::set_ui_for_all_dates()
{
    set_ui_for_create_date();
    set_ui_for_last_read_date();
    set_ui_for_last_modified_date();
}

void regular_filters::set_ui_for_create_date()
{
    /// Make UI for create Date
    QHBoxLayout *hboxlayout = new QHBoxLayout;

    QFont myfont;
    myfont.setPointSize(MACRO_QMenu_Font_Size_int);

    checkbox_create_date = new QCheckBox(this);
    checkbox_create_date->setFixedWidth(135);
    checkbox_create_date->setFont(myfont);
    checkbox_create_date->setText("Date Created");

    int size_1 = checkbox_create_date->height();

    comboBox_create_within = new QComboBox(this);
    QStringList item_list;
    item_list << MACRO_Generic_Filter_Condition_Between
              //<< MACRO_Generic_Filter_Condition_Within_Last
              << MACRO_Generic_Filter_Condition_Exactly
              << MACRO_Generic_Filter_Condition_Before
              << MACRO_Generic_Filter_Condition_After;
//              << MACRO_Generic_Filter_Condition_Today
//              << MACRO_Generic_Filter_Condition_Yesterday
//              << MACRO_Generic_Filter_Condition_This_Week
//              << MACRO_Generic_Filter_Condition_This_Month
//              << MACRO_Generic_Filter_Condition_This_Year;

    comboBox_create_within->addItems(item_list);
    comboBox_create_within->setFont(myfont);
    comboBox_create_within->setFixedHeight(size_1);

    lineEdit_create = new QLineEdit(this);
    lineEdit_create->setAttribute(Qt::WA_MacShowFocusRect,0);
    lineEdit_create->setFont(myfont);

//    QRegularExpression rx("\\d+");
//    QValidator *validator = new QRegularExpressionValidator(rx, this);
//    lineEdit_create->setValidator(validator);

    comboBox_create_days = new QComboBox(this);
    QStringList days_list;
    days_list << MACRO_Generic_Filter_Condition_Days
              << MACRO_Generic_Filter_Condition_Weeks
              << MACRO_Generic_Filter_Condition_Months
              << MACRO_Generic_Filter_Condition_Years;

    comboBox_create_days->addItems(days_list);
    comboBox_create_days->setFixedHeight(size_1);
    comboBox_create_days->setFont(myfont);

    date_edit_create_1 = new QDateEdit(this);
    date_edit_create_1->setDate(QDate::currentDate());
    date_edit_create_1->setDisplayFormat("dd/MM/yyyy");
    date_edit_create_1->setFont(myfont);

    date_edit_create_2 = new QDateEdit(this);
    date_edit_create_2->setDate(QDate::currentDate());
    date_edit_create_2->setDisplayFormat("dd/MM/yyyy");
    date_edit_create_2->setFont(myfont);

    label_create_between_to = new QLabel(this);
    label_create_between_to->setFont(myfont);
    label_create_between_to->setFixedHeight(size_1);
    label_create_between_to->setText("and");
    label_create_between_to->setContentsMargins(0,0,0,9);
    label_create_between_to->setAlignment(Qt::AlignCenter);
    label_create_between_to->setMaximumWidth(40);

    label_extra_create_1 = new QLabel(this);
    label_extra_create_1->setFont(myfont);
    label_extra_create_1->setFixedHeight(size_1);

    label_extra_create_2 = new QLabel(this);
    label_extra_create_2->setFont(myfont);
    label_extra_create_2->setFixedHeight(size_1);

    label_extra_create_3 = new QLabel(this);
    label_extra_create_3->setFont(myfont);
    label_extra_create_3->setFixedHeight(size_1);

    hboxlayout->addWidget(checkbox_create_date);
    hboxlayout->addWidget(comboBox_create_within);
    hboxlayout->addWidget(lineEdit_create);
    hboxlayout->addWidget(comboBox_create_days);
    hboxlayout->addWidget(date_edit_create_1);
    hboxlayout->addWidget(label_create_between_to);
    hboxlayout->addWidget(date_edit_create_2);
    hboxlayout->addWidget(label_extra_create_1);
    hboxlayout->addWidget(label_extra_create_2);
    hboxlayout->addWidget(label_extra_create_3);
    hboxlayout->setSpacing(10);
    hboxlayout->setContentsMargins(0,0,0,0);

    comboBox_create_within->setEnabled(false);
    lineEdit_create->setEnabled(false);
    comboBox_create_days->setEnabled(false);
    date_edit_create_1->setEnabled(false);
    label_create_between_to->setEnabled(false);
    date_edit_create_2->setEnabled(false);
    label_extra_create_1->setEnabled(false);
    label_extra_create_2->setEnabled(false);
    label_extra_create_3->setEnabled(false);

    lineEdit_create->hide();
    comboBox_create_days->hide();
    label_extra_create_1->hide();
    label_extra_create_2->hide();
    label_extra_create_3->hide();

    vboxlayout_all_dates->addLayout(hboxlayout);
}


void regular_filters::set_ui_for_last_read_date()
{
    /// Make UI for last opened Date
    QHBoxLayout *hboxlayout = new QHBoxLayout;

    QFont myfont;
    myfont.setPointSize(MACRO_QMenu_Font_Size_int);

    checkbox_last_read_date = new QCheckBox(this);
    checkbox_last_read_date->setFixedWidth(135);
    checkbox_last_read_date->setFont(myfont);
    checkbox_last_read_date->setText("Date Accessed");

    int size_1 = checkbox_last_read_date->height();

    comboBox_last_read_within = new QComboBox(this);
    QStringList item_list;
    item_list << MACRO_Generic_Filter_Condition_Between
              //<< MACRO_Generic_Filter_Condition_Within_Last
              << MACRO_Generic_Filter_Condition_Exactly
              << MACRO_Generic_Filter_Condition_Before
              << MACRO_Generic_Filter_Condition_After;

    comboBox_last_read_within->addItems(item_list);
    comboBox_last_read_within->setFont(myfont);
    comboBox_last_read_within->setFixedHeight(size_1);

    lineEdit_last_read = new QLineEdit(this);
    lineEdit_last_read->setAttribute(Qt::WA_MacShowFocusRect,0);
    lineEdit_last_read->setFont(myfont);

//    QRegularExpression rx("\\d+");
//    QValidator *validator = new QRegularExpressionValidator(rx, this);
//    lineEdit_last_read->setValidator(validator);

    comboBox_last_read_days = new QComboBox(this);
    QStringList days_list;
    days_list << MACRO_Generic_Filter_Condition_Days
              << MACRO_Generic_Filter_Condition_Weeks
              << MACRO_Generic_Filter_Condition_Months
              << MACRO_Generic_Filter_Condition_Years;

    comboBox_last_read_days->addItems(days_list);
    comboBox_last_read_days->setFont(myfont);

    date_edit_last_read_1 = new QDateEdit(this);
    date_edit_last_read_1->setDate(QDate::currentDate());
    date_edit_last_read_1->setDisplayFormat("dd/MM/yyyy");
    date_edit_last_read_1->setFont(myfont);

    date_edit_last_read_2 = new QDateEdit(this);
    date_edit_last_read_2->setDate(QDate::currentDate());
    date_edit_last_read_2->setDisplayFormat("dd/MM/yyyy");
    date_edit_last_read_2->setFont(myfont);

    label_last_read_between_to = new QLabel(this);
    label_last_read_between_to->setFont(myfont);
    label_last_read_between_to->setFixedHeight(size_1);
    label_last_read_between_to->setText("and");
    label_last_read_between_to->setContentsMargins(0,0,0,9);
    label_last_read_between_to->setAlignment(Qt::AlignCenter);
    label_last_read_between_to->setMaximumWidth(40);

    label_extra_read_1 = new QLabel(this);
    label_extra_read_1->setFont(myfont);
    label_extra_read_1->setFixedHeight(size_1);

    label_extra_read_2 = new QLabel(this);
    label_extra_read_2->setFont(myfont);
    label_extra_read_2->setFixedHeight(size_1);

    label_extra_read_3 = new QLabel(this);
    label_extra_read_3->setFont(myfont);
    label_extra_read_3->setFixedHeight(size_1);

    hboxlayout->addWidget(checkbox_last_read_date);
    hboxlayout->addWidget(comboBox_last_read_within);
    hboxlayout->addWidget(lineEdit_last_read);
    hboxlayout->addWidget(comboBox_last_read_days);
    hboxlayout->addWidget(date_edit_last_read_1);
    hboxlayout->addWidget(label_last_read_between_to);
    hboxlayout->addWidget(date_edit_last_read_2);
    hboxlayout->addWidget(label_extra_read_1);
    hboxlayout->addWidget(label_extra_read_2);
    hboxlayout->addWidget(label_extra_read_3);
    hboxlayout->setSpacing(10);
    hboxlayout->setContentsMargins(0,0,0,0);

    comboBox_last_read_within->setEnabled(false);
    lineEdit_last_read->setEnabled(false);
    comboBox_last_read_days->setEnabled(false);
    date_edit_last_read_1->setEnabled(false);
    label_last_read_between_to->setEnabled(false);
    date_edit_last_read_2->setEnabled(false);
    label_extra_read_1->setEnabled(false);
    label_extra_read_2->setEnabled(false);
    label_extra_read_3->setEnabled(false);

    lineEdit_last_read->hide();
    comboBox_last_read_days->hide();
    label_extra_read_1->hide();
    label_extra_read_2->hide();
    label_extra_read_3->hide();

    vboxlayout_all_dates->addLayout(hboxlayout);
}

void regular_filters::set_ui_for_last_modified_date()
{
    /// Make UI for last modified Date
    QHBoxLayout *hboxlayout = new QHBoxLayout;

    QFont myfont;
    myfont.setPointSize(MACRO_QMenu_Font_Size_int);

    checkbox_last_modified_date = new QCheckBox(this);
    checkbox_last_modified_date->setFixedWidth(135);
    checkbox_last_modified_date->setFont(myfont);
    checkbox_last_modified_date->setText("Date Modifed");

    int size_1 = checkbox_last_read_date->height();

    comboBox_last_modified_within = new QComboBox(this);
    QStringList item_list;
    item_list << MACRO_Generic_Filter_Condition_Between
              //<< MACRO_Generic_Filter_Condition_Within_Last
              << MACRO_Generic_Filter_Condition_Exactly
              << MACRO_Generic_Filter_Condition_Before
              << MACRO_Generic_Filter_Condition_After;


    comboBox_last_modified_within->addItems(item_list);
    comboBox_last_modified_within->setFont(myfont);
    comboBox_last_modified_within->setFixedHeight(size_1);

    lineEdit_last_modified = new QLineEdit(this);
    lineEdit_last_modified->setAttribute(Qt::WA_MacShowFocusRect,0);
    lineEdit_last_modified->setFont(myfont);

//    QRegularExpression rx("\\d+");
//    QValidator *validator = new QRegularExpressionValidator(rx, this);
//    lineEdit_last_modified->setValidator(validator);

    comboBox_last_modified_days = new QComboBox(this);
    QStringList days_list;
    days_list << MACRO_Generic_Filter_Condition_Days
              << MACRO_Generic_Filter_Condition_Weeks
              << MACRO_Generic_Filter_Condition_Months
              << MACRO_Generic_Filter_Condition_Years;

    comboBox_last_modified_days->addItems(days_list);
    comboBox_last_modified_days->setFont(myfont);

    date_edit_last_modified_1 = new QDateEdit(this);
    date_edit_last_modified_1->setDate(QDate::currentDate());
    date_edit_last_modified_1->setDisplayFormat("dd/MM/yyyy");
    date_edit_last_modified_1->setFont(myfont);

    date_edit_last_modified_2 = new QDateEdit(this);
    date_edit_last_modified_2->setDate(QDate::currentDate());
    date_edit_last_modified_2->setDisplayFormat("dd/MM/yyyy");
    date_edit_last_modified_2->setFont(myfont);

    label_last_modified_between_to = new QLabel(this);
    label_last_modified_between_to->setFont(myfont);
    label_last_modified_between_to->setFixedHeight(size_1);
    label_last_modified_between_to->setText("and");
    label_last_modified_between_to->setContentsMargins(0,0,0,9);
    label_last_modified_between_to->setAlignment(Qt::AlignCenter);
    label_last_modified_between_to->setMaximumWidth(40);

    label_extra_modified_1 = new QLabel(this);
    label_extra_modified_1->setFixedHeight(size_1);
    label_extra_modified_1->setFont(myfont);

    label_extra_modified_2 = new QLabel(this);
    label_extra_modified_2->setFixedHeight(size_1);
    label_extra_modified_2->setFont(myfont);

    label_extra_modified_3 = new QLabel(this);
    label_extra_modified_3->setFont(myfont);
    label_extra_modified_3->setFixedHeight(size_1);

    hboxlayout->addWidget(checkbox_last_modified_date);
    hboxlayout->addWidget(comboBox_last_modified_within);
    hboxlayout->addWidget(lineEdit_last_modified);
    hboxlayout->addWidget(comboBox_last_modified_days);
    hboxlayout->addWidget(date_edit_last_modified_1);
    hboxlayout->addWidget(label_last_modified_between_to);
    hboxlayout->addWidget(date_edit_last_modified_2);
    hboxlayout->addWidget(label_extra_modified_1);
    hboxlayout->addWidget(label_extra_modified_2);
    hboxlayout->addWidget(label_extra_modified_3);
    hboxlayout->setSpacing(10);
    hboxlayout->setContentsMargins(0,0,0,0);

    comboBox_last_modified_within->setEnabled(false);
    lineEdit_last_modified->setEnabled(false);
    comboBox_last_modified_days->setEnabled(false);
    date_edit_last_modified_1->setEnabled(false);
    label_last_modified_between_to->setEnabled(false);
    date_edit_last_modified_2->setEnabled(false);
    label_extra_modified_1->setEnabled(false);
    label_extra_modified_2->setEnabled(false);
    label_extra_modified_3->setEnabled(false);

    lineEdit_last_modified->hide();
    comboBox_last_modified_days->hide();
    label_extra_modified_1->hide();
    label_extra_modified_2->hide();
    label_extra_modified_3->hide();

    vboxlayout_all_dates->addLayout(hboxlayout);
}

void regular_filters::set_ui_mode_selection_radio_button()
{
    /// Make UI for Mode selection Radio Buttons
    QHBoxLayout *hlayout_radio_button = new QHBoxLayout;

    QFont myfont;
    myfont.setPointSize(MACRO_QMenu_Font_Size_int);

    radio_button_logical_or = new QRadioButton(this);
    radio_button_logical_or->setText("Logical OR");
    radio_button_logical_or->setFont(myfont);
    radio_button_logical_or->setChecked(true);

    radio_button_logical_and = new QRadioButton(this);
    radio_button_logical_and->setText("Logical AND");
    radio_button_logical_and->setFont(myfont);

    QSpacerItem *spacer_horrizontal = new QSpacerItem(60,10,QSizePolicy::Minimum,QSizePolicy::Expanding);

    hlayout_radio_button->addWidget(radio_button_logical_or);
    hlayout_radio_button->addSpacerItem(spacer_horrizontal);
    hlayout_radio_button->addWidget(radio_button_logical_and);
    hlayout_radio_button->setSpacing(10);
    hlayout_radio_button->setContentsMargins(0,0,0,0);


    radio_button_logical_and->hide();
    radio_button_logical_or->hide();
    // vbox_mode_selection_radio_button->addLayout(hlayout_radio_button);
}

void regular_filters::set_ui_for_search_option()
{
    QHBoxLayout *hlayout = new QHBoxLayout;

    QFont myfont;
    myfont.setPointSize(MACRO_QMenu_Font_Size_int);

    label_file_search = new QLabel(this);
    label_file_search->setText("Label");
    label_file_search->setFont(myfont);

    lineEdit_file_search = new QLineEdit(this);
    lineEdit_file_search->setAttribute(Qt::WA_MacShowFocusRect,0);
    lineEdit_file_search->setFont(myfont);

    hlayout->addWidget(label_file_search);
    hlayout->addWidget(lineEdit_file_search);
    hlayout->setSpacing(10);
    hlayout->setContentsMargins(0,0,0,0);

    vboxLayout_file_search_option->addLayout(hlayout);
}

void regular_filters::set_ui_for_search_and_cancel()
{
    QHBoxLayout *hlayout = new QHBoxLayout;

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

    QSpacerItem *spacer_horrizontal = new QSpacerItem(60,10,QSizePolicy::Expanding);

    hlayout->addSpacerItem(spacer_horrizontal);
    hlayout->addWidget(pushbutton_cancel);
    hlayout->addWidget(pushbutton_search);
    hlayout->setSpacing(10);
    hlayout->setContentsMargins(0,0,0,0);

    vboxLayout_search_and_cancel->addLayout(hlayout);
}

void regular_filters::reset_all_fields_of_ui()
{
    lineEdit_create->clear();
    lineEdit_file_name->clear();
    lineEdit_file_search->clear();
    lineEdit_file_size_1->clear();
    lineEdit_file_size_2->clear();
    lineEdit_last_modified->clear();
    lineEdit_last_read->clear();

    bool checked = false;

    checkbox_create_date->setChecked(checked);
    comboBox_create_days->setEnabled(checked);
    lineEdit_create->setEnabled(checked);
    comboBox_create_within->setEnabled(checked);
    date_edit_create_1->setEnabled(checked);

    checkbox_last_modified_date->setChecked(checked);
    comboBox_last_modified_days->setEnabled(checked);
    lineEdit_last_modified->setEnabled(checked);
    comboBox_last_modified_within->setEnabled(checked);
    date_edit_last_modified_1->setEnabled(checked);

    checkbox_last_read_date->setChecked(checked);
    comboBox_last_read_days->setEnabled(checked);
    lineEdit_last_read->setEnabled(checked);
    comboBox_last_read_within->setEnabled(checked);
    date_edit_last_read_1->setEnabled(checked);

    comboBox_create_days->setCurrentIndex(0);
    comboBox_last_modified_days->setCurrentIndex(0);
    comboBox_last_read_days->setCurrentIndex(0);

    comboBox_create_within->setCurrentIndex(0);
    comboBox_last_modified_within->setCurrentIndex(0);
    comboBox_last_read_within->setCurrentIndex(0);

    combobox_file_size_sufix->setCurrentIndex(0);
    combobox_file_size_type->setCurrentIndex(0);
    radio_button_logical_or->setChecked(true);

    combobox_file_name_filters->setCurrentIndex(0);

}

