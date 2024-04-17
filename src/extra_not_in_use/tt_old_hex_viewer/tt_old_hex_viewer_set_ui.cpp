#include "tt_old_hex_viewer.h"

void hex_viewer::create_ui_full()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    QHBoxLayout *hlayout_hex_and_features = new QHBoxLayout(this);
    QVBoxLayout *vlayout_all_features = new QVBoxLayout;

    //GO offset and Pages next
    set_ui_go_to_page_next_and_pre();

    // Search
    set_ui_for_search_section();

    // Conversion
//    set_ui_for_conversion_section_without_tablewidget();
    set_ui_for_conversion_section_with_tablewidget();

    // Current Offset and selection
    set_ui_for_current_offset_and_seleted_bytes();

    // Go to Offset
    set_ui_for_go_to_offset();

    // Export Block Details
    set_ui_for_export_blocks_section();

    // Page_size
    set_ui_for_page_size_selection();

    //File Metadata
    set_ui_for_file_metadata_display();

    //Open in other recon viewers
    set_ui_for_open_in_other_recon_viewers();

    QSpacerItem *spacer_vertical = new QSpacerItem(10,60,QSizePolicy::Minimum,QSizePolicy::Expanding);
    groupbox_1_go_offset_to_page->setFixedWidth(300);
    groupBox_2_search->setFixedWidth(300);
    groupBox_3_conversion->setFixedWidth(300);
    groupBox_5_current_selection->setFixedWidth(300);
    groupBox_go_to_offset->setFixedWidth(300);
    groupBox_6_page_size->setFixedWidth(300);
    groupBox_blocks_list->setFixedWidth(300);
    groupBox_file_metadata->setFixedWidth(300);
    groupBox_open_in_other_recon_viewers->setFixedWidth(300);

    vlayout_all_features->addWidget(groupbox_1_go_offset_to_page);
    vlayout_all_features->addWidget(groupBox_go_to_offset);
    vlayout_all_features->addWidget(groupBox_2_search);
    vlayout_all_features->addWidget(groupBox_blocks_list);
    vlayout_all_features->addWidget(groupBox_6_page_size);
    vlayout_all_features->addWidget(groupBox_5_current_selection);
    vlayout_all_features->addWidget(groupBox_3_conversion);
    vlayout_all_features->addWidget(groupBox_file_metadata);
    vlayout_all_features->addWidget(groupBox_open_in_other_recon_viewers);

    vlayout_all_features->addSpacerItem(spacer_vertical);
    vlayout_all_features->setSpacing(4);

    // FRAME HEX
    set_ui_for_frame_hex();

    // SET UI ATTRIBUTES---
    set_converstion_to_default_attributes();

//    hlayout_hex_and_features->addSpacerItem(spacer_horrizontal_1);
    hlayout_hex_and_features->addWidget(frame_hex);
    hlayout_hex_and_features->addLayout(vlayout_all_features);
//    hlayout_hex_and_features->addSpacerItem(spacer_horrizontal_2);
    hlayout_hex_and_features->setSpacing(4);
    hlayout_hex_and_features->setContentsMargins(0,1,0,1);

    this->setLayout(hlayout_hex_and_features);

    viewer_connections();

    is_full_or_meta = true;
    emit signal_set_context_menu(is_full_or_meta);

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::pub_display_hex_viewer_metadata()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    QHBoxLayout *hlayout_all = new QHBoxLayout(this);
    QVBoxLayout *vlayout_hex_and_features = new QVBoxLayout;
    QHBoxLayout *hlayout_all_features = new QHBoxLayout;

    //GO offset and Pages next
    set_ui_go_to_page_next_and_pre_for_metadata();

    // Search
    set_ui_for_search_section();

    // Conversion
//    set_ui_for_conversion_section_without_tablewidget();
    set_ui_for_conversion_section_with_tablewidget();

    // Current Offset and selection
    set_ui_for_current_offset_and_seleted_bytes();

    // Go to Offset
    set_ui_for_go_to_offset();

    // Export Block Details
    set_ui_for_export_blocks_section();

    // Page_size
    set_ui_for_page_size_selection();

    //File Metadata
    set_ui_for_file_metadata_display();

    //Open in other recon viewers
    set_ui_for_open_in_other_recon_viewers();

    QSpacerItem *spacer_horrizontal = new QSpacerItem(60, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

    groupBox_2_search->hide();
    groupBox_3_conversion->hide();
    groupBox_5_current_selection->hide();
    groupBox_6_page_size->hide();
    groupBox_go_to_offset->hide();
    groupBox_blocks_list->hide();
    groupBox_file_metadata->hide();
    groupBox_open_in_other_recon_viewers->hide();

    hlayout_all_features->addWidget(groupbox_1_go_offset_to_page);
    hlayout_all_features->addWidget(groupBox_2_search);
    hlayout_all_features->addWidget(groupBox_3_conversion);
    hlayout_all_features->addWidget(groupBox_5_current_selection);
    hlayout_all_features->addWidget(groupBox_go_to_offset);
    hlayout_all_features->addWidget(groupBox_blocks_list);
    hlayout_all_features->addWidget(groupBox_6_page_size);
    hlayout_all_features->addWidget(groupBox_blocks_list);
    hlayout_all_features->addWidget(groupBox_file_metadata);
    hlayout_all_features->addWidget(groupBox_open_in_other_recon_viewers);

    hlayout_all_features->addSpacerItem(spacer_horrizontal);

    /// FRAME HEX
    set_ui_for_frame_hex();

    // SET UI ATTRIBUTES--
    set_converstion_to_default_attributes();


    vlayout_hex_and_features->addLayout(hlayout_all_features);
    vlayout_hex_and_features->addWidget(frame_hex);
    vlayout_hex_and_features->setStretchFactor(frame_hex, 1);
    vlayout_hex_and_features->setMargin(2);
    vlayout_hex_and_features->setSpacing(0);

//    hlayout_all->addSpacerItem(spacer_horrizontal_1);
    hlayout_all->addLayout(vlayout_hex_and_features);
//    hlayout_all->addSpacerItem(spacer_horrizontal_2);
    hlayout_all->setMargin(2);
    hlayout_all->setSpacing(0);

    this->setLayout(hlayout_all);

    viewer_connections();

    is_full_or_meta = false;
    emit signal_set_context_menu(is_full_or_meta);

    recon_static_functions::app_debug(" end ",Q_FUNC_INFO);
}

void hex_viewer::set_ui_for_open_in_other_recon_viewers()
{
    pushbutton_open_in_other_recon_viewers = new QPushButton(this);

    QSpacerItem *spacer_horrizontal_1 = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QHBoxLayout *hlayout_open_viewers = new QHBoxLayout;
    hlayout_open_viewers->addWidget(pushbutton_open_in_other_recon_viewers);
    hlayout_open_viewers->addSpacerItem(spacer_horrizontal_1);

    hlayout_open_viewers->setSpacing(0);
    hlayout_open_viewers->setMargin(0);

    groupBox_open_in_other_recon_viewers = new QGroupBox(this);
    groupBox_open_in_other_recon_viewers->setContentsMargins(0,0,0,0);
    groupBox_open_in_other_recon_viewers->setLayout(hlayout_open_viewers);
}

void hex_viewer::set_ui_go_to_page_next_and_pre()
{
    label_no_of_pages           = new QLabel(this);
    label_no_of_pages->setMinimumWidth(140);

    pushButton_next_goto_page             = new QPushButton(this);
    pushButton_next_goto_page->setText(">");
    pushButton_next_goto_page->setFixedWidth(60);

    pushButton_prev_goto_page             = new QPushButton(this);
    pushButton_prev_goto_page->setText("<");
    pushButton_prev_goto_page->setFixedWidth(60);


    lineedit_goto_page = new QLineEdit(this);
    lineedit_goto_page->setPlaceholderText("Go to page");
    lineedit_goto_page->setFixedHeight(20);
    lineedit_goto_page->setAttribute(Qt::WA_MacShowFocusRect,false);

    pushbutton_go_to_page = new QPushButton(this);
    pushbutton_go_to_page->setText("Go");

    QVBoxLayout *vlayout_pages_and_go = new QVBoxLayout;
    QHBoxLayout *hlayout_pages = new QHBoxLayout;
    hlayout_pages->addWidget(pushButton_prev_goto_page);
    hlayout_pages->addWidget(label_no_of_pages);
    hlayout_pages->addWidget(pushButton_next_goto_page);
    hlayout_pages->setSpacing(6);

    QHBoxLayout *hlayout_go_to_page = new QHBoxLayout;
    hlayout_go_to_page->addWidget(lineedit_goto_page);
    hlayout_go_to_page->addWidget(pushbutton_go_to_page);
    hlayout_go_to_page->setSpacing(6);

    vlayout_pages_and_go->addLayout(hlayout_pages);
    vlayout_pages_and_go->addLayout(hlayout_go_to_page);
    vlayout_pages_and_go->setMargin(0);
    vlayout_pages_and_go->setSpacing(0);

    groupbox_1_go_offset_to_page = new QGroupBox(this);
    groupbox_1_go_offset_to_page->setLayout(vlayout_pages_and_go);
}

void hex_viewer::set_ui_go_to_page_next_and_pre_for_metadata()
{
    label_no_of_pages = new QLabel(this);
    label_no_of_pages->setMinimumWidth(140);
    pushButton_next_goto_page = new QPushButton(this);
    pushButton_next_goto_page->setText(">");

    pushButton_prev_goto_page = new QPushButton(this);
    pushButton_prev_goto_page->setText("<");

    lineEdit_go_to_offset = new QLineEdit(this);
    lineEdit_go_to_offset->setPlaceholderText("Go to offset");
    lineEdit_go_to_offset->hide();

//    pushButton_goto_offset = new QPushButton(this);
//    pushButton_goto_offset->setText("Go");
//    pushButton_goto_offset->hide();

    lineedit_goto_page = new QLineEdit(this);
    lineedit_goto_page->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_goto_page->setPlaceholderText("Go to page");

    pushbutton_go_to_page = new QPushButton(this);
    pushbutton_go_to_page->setText("Go");

    QHBoxLayout *hlayout_pages_and_go = new QHBoxLayout;
    QHBoxLayout *hlayout_pages = new QHBoxLayout;
    hlayout_pages->addWidget(pushButton_prev_goto_page);
    hlayout_pages->addWidget(label_no_of_pages);
    hlayout_pages->addWidget(pushButton_next_goto_page);
    hlayout_pages->setSpacing(6);
    hlayout_pages->setMargin(0);

    QHBoxLayout *hlayout_go_to_page = new QHBoxLayout;
    hlayout_go_to_page->addWidget(lineedit_goto_page);
    hlayout_go_to_page->addWidget(pushbutton_go_to_page);
    hlayout_go_to_page->setSpacing(6);
    hlayout_go_to_page->setMargin(0);

    hlayout_pages_and_go->addLayout(hlayout_pages);
    hlayout_pages_and_go->addLayout(hlayout_go_to_page);
    hlayout_pages_and_go->setMargin(0);
    hlayout_pages_and_go->setSpacing(10);

    groupbox_1_go_offset_to_page = new QGroupBox(this);
    groupbox_1_go_offset_to_page->setLayout(hlayout_pages_and_go);
}

void hex_viewer::set_ui_for_search_section()
{
    comboBox_search_type        = new QComboBox(this);
    comboBox_search_type->addItem("HEX");
    comboBox_search_type->addItem("ASCII");

    lineEdit_search             = new QLineEdit(this);
    lineEdit_search->setPlaceholderText("Enter value to search");
    lineEdit_search->setFixedHeight(20);
    lineEdit_search->setAttribute(Qt::WA_MacShowFocusRect,false);


    pushButton_search           = new QPushButton(this);
    pushButton_search->setText("Search");
    pushButton_search->setFixedWidth(70);
    pushButton_search->setEnabled(false);


    label_search_results        = new QLabel(this);
    pushButton_search_next        = new QPushButton;
    pushButton_search_next->setText(">");
    pushButton_search_next->setFixedWidth(60);

    pushButton_search_previous    = new QPushButton(this);
    pushButton_search_previous->setText("<");
    pushButton_search_previous->setFixedWidth(60);

    QVBoxLayout *vlayout_search_all = new QVBoxLayout;
    QHBoxLayout *hlayout_search_lineedit = new QHBoxLayout;
    hlayout_search_lineedit->addWidget(lineEdit_search);
    hlayout_search_lineedit->addWidget(comboBox_search_type);
    hlayout_search_lineedit->setSpacing(6);


    hlayout_search_lineedit->setStretchFactor(lineEdit_search, 1);

    QHBoxLayout *hlayout_find_next = new QHBoxLayout;
    hlayout_find_next->addWidget(pushButton_search);
    hlayout_find_next->addWidget(pushButton_search_previous);
    hlayout_find_next->addWidget(label_search_results);
    hlayout_find_next->addWidget(pushButton_search_next);
    hlayout_find_next->setSpacing(6);


    vlayout_search_all->addLayout(hlayout_search_lineedit);
    vlayout_search_all->addLayout(hlayout_find_next);
    vlayout_search_all->setSpacing(0);
    vlayout_search_all->setMargin(0);

    groupBox_2_search = new QGroupBox(this);
    groupBox_2_search->setLayout(vlayout_search_all);
}

void hex_viewer::set_ui_for_conversion_section_with_tablewidget()
{
    checkBox_unsigned           = new QCheckBox(this);
    checkBox_unsigned->setText("Unsigned");

    checkBox_big_endian         = new QCheckBox(this);
    checkBox_big_endian->setText("Little Endian");

    QStringList headerlist;
    headerlist << "Key" << "Value";

    m_tablewidget_conversion_obj = new m_tablewidget(this);
    m_tablewidget_conversion_obj->setColumnCount(headerlist.size());
    m_tablewidget_conversion_obj->setHorizontalHeaderLabels(headerlist);
    m_tablewidget_conversion_obj->horizontalHeader()->setStretchLastSection(true);

    // Entry for Binary
    m_tablewidget_conversion_obj->insertRow(enum_binary);
    m_tablewidget_conversion_obj->setRowHeight(enum_binary,22);

    QTableWidgetItem *item_key_binary = new QTableWidgetItem;
    item_key_binary->setText("8 Bit Binary");

    QTableWidgetItem *item_value_binary = new QTableWidgetItem;
    item_value_binary->setText("");

    m_tablewidget_conversion_obj->setItem(enum_binary,0,item_key_binary);
    m_tablewidget_conversion_obj->setItem(enum_binary,1,item_value_binary);



    // Entry for 8 Bit
    m_tablewidget_conversion_obj->insertRow(enum_8_bit);
    m_tablewidget_conversion_obj->setRowHeight(enum_8_bit,22);

    QTableWidgetItem *item_key_8_bit = new QTableWidgetItem;
    item_key_8_bit->setText("8 Bit");

    QTableWidgetItem *item_value_8_bit = new QTableWidgetItem;
    item_value_8_bit->setText("");

    m_tablewidget_conversion_obj->setItem(enum_8_bit,0,item_key_8_bit);
    m_tablewidget_conversion_obj->setItem(enum_8_bit,1,item_value_8_bit);



    // Entry for 16 Bit
    m_tablewidget_conversion_obj->insertRow(enum_16_bit);
    m_tablewidget_conversion_obj->setRowHeight(enum_16_bit,22);

    QTableWidgetItem *item_key_16_bit = new QTableWidgetItem;
    item_key_16_bit->setText("16 Bit");

    QTableWidgetItem *item_value_16_bit = new QTableWidgetItem;
    item_value_16_bit->setText("");

    m_tablewidget_conversion_obj->setItem(enum_16_bit,0,item_key_16_bit);
    m_tablewidget_conversion_obj->setItem(enum_16_bit,1,item_value_16_bit);



    // Entry for 32 Bit
    m_tablewidget_conversion_obj->insertRow(enum_32_bit);
    m_tablewidget_conversion_obj->setRowHeight(enum_32_bit,22);

    QTableWidgetItem *item_key_32_bit = new QTableWidgetItem;
    item_key_32_bit->setText("32 Bit");

    QTableWidgetItem *item_value_32_bit = new QTableWidgetItem;
    item_value_32_bit->setText("");

    m_tablewidget_conversion_obj->setItem(enum_32_bit,0,item_key_32_bit);
    m_tablewidget_conversion_obj->setItem(enum_32_bit,1,item_value_32_bit);



    // Entry for MAC Timestamp
    m_tablewidget_conversion_obj->insertRow(enum_mac_timestamp);
    m_tablewidget_conversion_obj->setRowHeight(enum_mac_timestamp,22);

    QTableWidgetItem *item_key_mac_timestamp = new QTableWidgetItem;
    item_key_mac_timestamp->setText("MAC Timestamp");

    QTableWidgetItem *item_value_mac_timestamp = new QTableWidgetItem;
    item_value_mac_timestamp->setText("");

    m_tablewidget_conversion_obj->setItem(enum_mac_timestamp,0,item_key_mac_timestamp);
    m_tablewidget_conversion_obj->setItem(enum_mac_timestamp,1,item_value_mac_timestamp);



    // Entry for UNIX Timestamp
    m_tablewidget_conversion_obj->insertRow(enum_unix_timestamp);
    m_tablewidget_conversion_obj->setRowHeight(enum_unix_timestamp,22);

    QTableWidgetItem *item_key_unix_timestamp = new QTableWidgetItem;
    item_key_unix_timestamp->setText("UNIX Timestamp");

    QTableWidgetItem *item_value_unix_timestamp = new QTableWidgetItem;
    item_value_unix_timestamp->setText("");

    m_tablewidget_conversion_obj->setItem(enum_unix_timestamp,0,item_key_unix_timestamp);
    m_tablewidget_conversion_obj->setItem(enum_unix_timestamp,1,item_value_unix_timestamp);



    m_tablewidget_conversion_obj->setColumnWidth(0,120);
    m_tablewidget_conversion_obj->setColumnWidth(1,100);

    QVBoxLayout *vlayout_conversion = new QVBoxLayout;
    QHBoxLayout *hlayout_checkbox = new QHBoxLayout;
    QHBoxLayout *hlay_unsigned = new QHBoxLayout;
    hlay_unsigned->addWidget(checkBox_unsigned);
    hlay_unsigned->setSpacing(10);

    QHBoxLayout *hlay_big_endian = new QHBoxLayout;
    hlay_big_endian->addWidget(checkBox_big_endian);
    hlay_big_endian->setSpacing(10);

    hlayout_checkbox->addLayout(hlay_unsigned);
    hlayout_checkbox->addLayout(hlay_big_endian);
    hlayout_checkbox->setSpacing(0);

    vlayout_conversion->addLayout(hlayout_checkbox);
    vlayout_conversion->addWidget(m_tablewidget_conversion_obj);
    vlayout_conversion->setMargin(0);
    vlayout_conversion->setSpacing(0);

    groupBox_3_conversion = new QGroupBox(this);
    groupBox_3_conversion->setLayout(vlayout_conversion);
}

void hex_viewer::set_ui_for_conversion_section_without_tablewidget()
{
    checkBox_unsigned           = new QCheckBox(this);
    checkBox_unsigned->setText("Unsigned");

    checkBox_big_endian         = new QCheckBox(this);
    checkBox_big_endian->setText("Big Endian");

    label_binary                       = new QLabel(this);
    label_binary->setText("Binary");
    label_binary->setMinimumWidth(95);

    lineEdit_binary             = new QLineEdit(this);
    lineEdit_binary->setFixedHeight(20);
    lineEdit_binary->setAttribute(Qt::WA_MacShowFocusRect,false);

    label_8_bit                     = new QLabel(this);
    label_8_bit->setText("8 Bit");
    label_8_bit->setMinimumWidth(95);
    lineEdit_8_bit              = new QLineEdit(this);
    lineEdit_8_bit->setFixedHeight(20);
    lineEdit_8_bit->setAttribute(Qt::WA_MacShowFocusRect,false);


    label_16_bit                     = new QLabel(this);
    label_16_bit->setText("16 Bit");
    label_16_bit->setMinimumWidth(95);
    lineEdit_16_bit             = new QLineEdit(this);
    lineEdit_16_bit->setFixedHeight(20);
    lineEdit_16_bit->setAttribute(Qt::WA_MacShowFocusRect,false);


    label_32_bit                     = new QLabel(this);
    label_32_bit->setText("32 Bit");
    label_32_bit->setMinimumWidth(95);
    lineEdit_32_bit             = new QLineEdit(this);
    lineEdit_32_bit->setFixedHeight(20);
    lineEdit_32_bit->setAttribute(Qt::WA_MacShowFocusRect,false);

    label_mac_timestamp                     = new QLabel(this);
    label_mac_timestamp->setText("MAC Timestamp");
    label_mac_timestamp->setMinimumWidth(95);

    lineEdit_mac_timestamp      = new QLineEdit(this);
    lineEdit_mac_timestamp->setFixedHeight(20);
    lineEdit_mac_timestamp->setAttribute(Qt::WA_MacShowFocusRect,false);


    label_unix_timestamp                     = new QLabel(this);
    label_unix_timestamp->setText("UNIX Timestamp");
    label_unix_timestamp->setMinimumWidth(95);

    lineEdit_unix_timestamp     = new QLineEdit(this);
    lineEdit_unix_timestamp->setFixedHeight(20);
    lineEdit_unix_timestamp->setAttribute(Qt::WA_MacShowFocusRect,false);

    QVBoxLayout *vlayout_conversion = new QVBoxLayout;
    QHBoxLayout *hlayout_checkbox = new QHBoxLayout;
    QHBoxLayout *hlay_unsigned = new QHBoxLayout;
    hlay_unsigned->addWidget(checkBox_unsigned);
    hlay_unsigned->setSpacing(10);

    QHBoxLayout *hlay_big_endian = new QHBoxLayout;
    hlay_big_endian->addWidget(checkBox_big_endian);
    hlay_big_endian->setSpacing(10);

    hlayout_checkbox->addLayout(hlay_unsigned);
    hlayout_checkbox->addLayout(hlay_big_endian);
    hlayout_checkbox->setSpacing(0);


    QHBoxLayout *hlayout_binary = new QHBoxLayout;
    hlayout_binary->addWidget(label_binary);
    hlayout_binary->addWidget(lineEdit_binary);
    hlayout_binary->setStretchFactor(lineEdit_binary,1);

    QHBoxLayout *hlayout_8_bit = new QHBoxLayout;
    hlayout_8_bit->addWidget(label_8_bit);
    hlayout_8_bit->addWidget(lineEdit_8_bit);
    hlayout_8_bit->setStretchFactor(lineEdit_8_bit,1);

    QHBoxLayout *hlayout_16_bit = new QHBoxLayout;
    hlayout_16_bit->addWidget(label_16_bit);
    hlayout_16_bit->addWidget(lineEdit_16_bit);
    hlayout_16_bit->setStretchFactor(lineEdit_16_bit,1);

    QHBoxLayout *hlayout_32_bit = new QHBoxLayout;
    hlayout_32_bit->addWidget(label_32_bit);
    hlayout_32_bit->addWidget(lineEdit_32_bit);
    hlayout_32_bit->setStretchFactor(lineEdit_32_bit,1);

    QHBoxLayout *hlayout_mac_timestamp = new QHBoxLayout;
    hlayout_mac_timestamp->addWidget(label_mac_timestamp);
    hlayout_mac_timestamp->addWidget(lineEdit_mac_timestamp);
    hlayout_mac_timestamp->setStretchFactor(lineEdit_mac_timestamp,1);

    QHBoxLayout *hlayout_unix_timestamp = new QHBoxLayout;
    hlayout_unix_timestamp->addWidget(label_unix_timestamp);
    hlayout_unix_timestamp->addWidget(lineEdit_unix_timestamp);
    hlayout_unix_timestamp->setStretchFactor(lineEdit_unix_timestamp,1);


    vlayout_conversion->addLayout(hlayout_checkbox);
    vlayout_conversion->addLayout(hlayout_binary);
    vlayout_conversion->addLayout(hlayout_8_bit);
    vlayout_conversion->addLayout(hlayout_16_bit);
    vlayout_conversion->addLayout(hlayout_32_bit);
    vlayout_conversion->addLayout(hlayout_mac_timestamp);
    vlayout_conversion->addLayout(hlayout_unix_timestamp);
    vlayout_conversion->setSpacing(5);
    vlayout_conversion->setMargin(0);

    groupBox_3_conversion = new QGroupBox(this);
    groupBox_3_conversion->setLayout(vlayout_conversion);
}

void hex_viewer::set_ui_for_current_offset_and_seleted_bytes()
{
    label_selection_info_offset        = new QLabel(this);
    label_selection_info_offset->setText("Offset");
    label_selection_info_offset->setFixedWidth(70);

    pushButton_selection_info_offset           = new QPushButton(this);
    pushButton_selection_info_offset->setStyleSheet("");

    label_selection_info_byte_count     = new QLabel(this);
    label_selection_info_byte_count->setText("Selection");
    label_selection_info_byte_count->setFixedWidth(70);

    pushButton_selection_info_byte_count        = new QPushButton(this);

    QVBoxLayout *vlayout_curr_selection = new QVBoxLayout;

    QHBoxLayout *hlayout_curr_offset = new QHBoxLayout;
    hlayout_curr_offset->addWidget(label_selection_info_offset);
    hlayout_curr_offset->addWidget(pushButton_selection_info_offset);
    hlayout_curr_offset->setStretchFactor(pushButton_selection_info_offset, 1);

    QHBoxLayout *hlayout_selection = new QHBoxLayout;
    hlayout_selection->addWidget(label_selection_info_byte_count);
    hlayout_selection->addWidget(pushButton_selection_info_byte_count);
    hlayout_selection->setStretchFactor(pushButton_selection_info_byte_count, 1);

    vlayout_curr_selection->addLayout(hlayout_curr_offset);
    vlayout_curr_selection->addLayout(hlayout_selection);
    vlayout_curr_selection->setMargin(0);
    vlayout_curr_selection->setSpacing(5);

    groupBox_5_current_selection = new QGroupBox(this);
    groupBox_5_current_selection->setLayout(vlayout_curr_selection);
}

void hex_viewer::set_ui_for_go_to_offset()
{
    comboBox_go_to_offset_type = new QComboBox(this);
    comboBox_go_to_offset_type->addItem("HEX");
    comboBox_go_to_offset_type->addItem("DECIMAL");

    lineEdit_go_to_offset                    = new QLineEdit(this);
    lineEdit_go_to_offset->setPlaceholderText("Go to offset");
    lineEdit_go_to_offset->setFixedHeight(20);
    lineEdit_go_to_offset->setAttribute(Qt::WA_MacShowFocusRect,false);


    pushButton_goto_offset               = new QPushButton(this);
    pushButton_goto_offset->setText("Go");

    QHBoxLayout *hlayout_go_to_offset = new QHBoxLayout;
    hlayout_go_to_offset->addWidget(lineEdit_go_to_offset);
    hlayout_go_to_offset->addWidget(comboBox_go_to_offset_type);
    hlayout_go_to_offset->addWidget(pushButton_goto_offset);
    hlayout_go_to_offset->setSpacing(6);
    hlayout_go_to_offset->setMargin(0);

    groupBox_go_to_offset = new QGroupBox(this);
    groupBox_go_to_offset->setLayout(hlayout_go_to_offset);
}

void hex_viewer::set_ui_for_export_blocks_section()
{
    comboBox_blocks_list = new QComboBox(this);
//    comboBox_blocks_list->addItems(combobox_display_list);

    pushbutton_export_block = new QPushButton(this);
    pushbutton_export_block->setText("Export");
    pushbutton_export_block->setMinimumWidth(70);
    pushbutton_export_block->setMaximumWidth(70);

    pushButton_remove_block = new QPushButton(this);
    pushButton_remove_block->setText("Remove");
    pushButton_remove_block->setMinimumWidth(70);
    pushButton_remove_block->setMaximumWidth(70);

    label_block_info = new QLabel(this);
    label_block_info->setText("Tag Info");
    label_block_info->setFixedWidth(50);

    label_block_start_end_index = new QLabel(this);
    label_block_start_end_index->setText(first_label_display_string);

    QVBoxLayout *vbox_block_details = new QVBoxLayout;
    QHBoxLayout *hbox_layout_combo_box = new QHBoxLayout;
    hbox_layout_combo_box->addWidget(comboBox_blocks_list);
    hbox_layout_combo_box->addWidget(pushbutton_export_block);

    hbox_layout_combo_box->setSpacing(3);
    hbox_layout_combo_box->setMargin(0);

    QHBoxLayout *hbox_layout_remove_with_label = new QHBoxLayout;
    hbox_layout_remove_with_label->addWidget(label_block_info);
    hbox_layout_remove_with_label->addWidget(label_block_start_end_index);
    hbox_layout_remove_with_label->addWidget(pushButton_remove_block);

    hbox_layout_remove_with_label->setSpacing(3);
    hbox_layout_remove_with_label->setMargin(0);

    vbox_block_details->addLayout(hbox_layout_combo_box);
    vbox_block_details->addLayout(hbox_layout_remove_with_label);

    vbox_block_details->setSpacing(0);
    vbox_block_details->setMargin(0);

    groupBox_blocks_list = new QGroupBox(this);
    groupBox_blocks_list->setLayout(vbox_block_details);
}

void hex_viewer::set_ui_for_file_metadata_display()
{
    label_file_name = new QLabel(this);
    label_file_name->setWordWrap(true);

    label_file_path = new QLabel(this);
    label_file_path->setWordWrap(true);

    label_file_size = new QLabel(this);
    label_file_size->setWordWrap(true);

    QVBoxLayout *vlayout_file_metadata = new QVBoxLayout;
    vlayout_file_metadata->addWidget(label_file_name);
    vlayout_file_metadata->addWidget(label_file_path);
    vlayout_file_metadata->addWidget(label_file_size);
    vlayout_file_metadata->setSpacing(3);
    vlayout_file_metadata->setMargin(0);

    groupBox_file_metadata = new QGroupBox(this);
    groupBox_file_metadata->setLayout(vlayout_file_metadata);
}

void hex_viewer::set_ui_for_page_size_selection()
{
    label_page_size = new QLabel(this);
    label_page_size->setText("Page Size (Byte(s))");

    spinBox_page_size = new QSpinBox;

    QHBoxLayout *hlayout_page_size = new QHBoxLayout;
    hlayout_page_size->addWidget(label_page_size);
    hlayout_page_size->addWidget(spinBox_page_size);
    hlayout_page_size->setStretchFactor(spinBox_page_size, 1);
    hlayout_page_size->setMargin(0);
    hlayout_page_size->setSpacing(5);

    groupBox_6_page_size = new QGroupBox;
    groupBox_6_page_size->setLayout(hlayout_page_size);
}

void hex_viewer::set_ui_for_frame_hex()
{
    frame_hex = new QFrame(this);
    groupBox_6_page_size->hide();

    QVBoxLayout *vbox = new QVBoxLayout;

    vbox->addWidget(hex_edit_obj);
    vbox->setMargin(0);
    vbox->setSpacing(0);

    frame_hex->setLayout(vbox);
}

void hex_viewer::set_converstion_to_default_attributes()
{
    spinBox_page_size->setRange(512, 9999999);

    spinBox_page_size->setSingleStep(512);

    spinBox_page_size->setValue(MACRO_HEX_VIEWER_DEFAULT_PAGE_SIZE_int);

    pushButton_selection_info_offset->setCheckable(true);
    pushButton_selection_info_byte_count->setCheckable(true);

    pushButton_selection_info_offset->setStyleSheet("QPushButton { background: transparent; border: none;}");
    pushButton_selection_info_byte_count->setStyleSheet("QPushButton { background: transparent; border: none;}");

    //    lineEdit_8_bit->clear();
//    lineEdit_16_bit->clear();

//    lineEdit_binary->setReadOnly(true);
//    lineEdit_8_bit->setReadOnly(true);
//    lineEdit_16_bit->setReadOnly(true);
//    lineEdit_32_bit->setReadOnly(true);
//    lineEdit_mac_timestamp->setReadOnly(true);
//    lineEdit_unix_timestamp->setReadOnly(true);

    m_tablewidget_conversion_obj->item(enum_binary,1)->setText("");
    m_tablewidget_conversion_obj->item(enum_8_bit,1)->setText("");
    m_tablewidget_conversion_obj->item(enum_16_bit,1)->setText("");
    m_tablewidget_conversion_obj->item(enum_32_bit,1)->setText("");
    m_tablewidget_conversion_obj->item(enum_mac_timestamp,1)->setText("");
    m_tablewidget_conversion_obj->item(enum_unix_timestamp,1)->setText("");

    label_no_of_pages->setAlignment(Qt::AlignCenter);
    label_search_results->setAlignment(Qt::AlignCenter);
    label_block_start_end_index->setAlignment(Qt::AlignCenter);

    pushButton_search_next->setEnabled(false);
    pushButton_search_previous->setEnabled(false);
    pushButton_next_goto_page->setEnabled(false);
    pushButton_prev_goto_page->setEnabled(false);

    set_widget_fonts();
}

void hex_viewer::set_widget_fonts()
{
    QFont m_font = label_no_of_pages->font();
    m_font.setPointSize(12);
    label_no_of_pages->setFont(m_font);

    m_font = pushButton_next_goto_page->font();
    m_font.setPointSize(12);
    pushButton_next_goto_page->setFont(m_font);

    m_font = pushButton_prev_goto_page->font();
    m_font.setPointSize(12);
    pushButton_prev_goto_page->setFont(m_font);

    m_font = lineedit_goto_page->font();
    m_font.setPointSize(12);
    lineedit_goto_page->setFont(m_font);

    m_font = pushbutton_go_to_page->font();
    m_font.setPointSize(12);
    pushbutton_go_to_page->setFont(m_font);

    m_font = lineEdit_search->font();
    m_font.setPointSize(12);
    lineEdit_search->setFont(m_font);

    m_font = comboBox_search_type->font();
    m_font.setPointSize(12);
    comboBox_search_type->setFont(m_font);

    m_font = pushButton_search->font();
    m_font.setPointSize(12);
    pushButton_search->setFont(m_font);

    m_font = pushButton_search_next->font();
    m_font.setPointSize(12);
    pushButton_search_next->setFont(m_font);

    m_font = pushButton_search_previous->font();
    m_font.setPointSize(12);
    pushButton_search_previous->setFont(m_font);

    m_font = label_search_results->font();
    m_font.setPointSize(12);
    label_search_results->setFont(m_font);

    m_font = checkBox_big_endian->font();
    m_font.setPointSize(12);
    checkBox_big_endian->setFont(m_font);

    m_font = checkBox_unsigned->font();
    m_font.setPointSize(12);
    checkBox_unsigned->setFont(m_font);

    m_font = m_tablewidget_conversion_obj->font();
    m_font.setPointSize(12);
    m_tablewidget_conversion_obj->setFont(m_font);

//    m_font = label_binary->font();
//    m_font.setPointSize(12);
//    label_binary->setFont(m_font);

//    m_font = label_8_bit->font();
//    m_font.setPointSize(12);
//    label_8_bit->setFont(m_font);

//    m_font = label_16_bit->font();
//    m_font.setPointSize(12);
//    label_16_bit->setFont(m_font);

//    m_font = label_32_bit->font();
//    m_font.setPointSize(12);
//    label_32_bit->setFont(m_font);

//    m_font = label_mac_timestamp->font();
//    m_font.setPointSize(12);
//    label_mac_timestamp->setFont(m_font);

//    m_font = label_unix_timestamp->font();
//    m_font.setPointSize(12);
//    label_unix_timestamp->setFont(m_font);

    m_font = label_page_size->font();
    m_font.setPointSize(12);
    label_page_size->setFont(m_font);

//    m_font = lineEdit_8_bit->font();
//    m_font.setPointSize(12);
//    lineEdit_8_bit->setFont(m_font);

//    m_font = lineEdit_16_bit->font();
//    m_font.setPointSize(12);
//    lineEdit_16_bit->setFont(m_font);

//    m_font = lineEdit_32_bit->font();
//    m_font.setPointSize(12);
//    lineEdit_32_bit->setFont(m_font);

//    m_font = lineEdit_binary->font();
//    m_font.setPointSize(12);
//    lineEdit_binary->setFont(m_font);

    m_font = label_block_info->font();
    m_font.setPointSize(12);
    label_block_info->setFont(m_font);

    m_font = label_block_start_end_index->font();
    m_font.setPointSize(12);
    label_block_start_end_index->setFont(m_font);

    m_font = label_selection_info_offset->font();
    m_font.setPointSize(12);
    label_selection_info_offset->setFont(m_font);

    m_font = label_selection_info_byte_count->font();
    m_font.setPointSize(12);
    label_selection_info_byte_count->setFont(m_font);

    m_font = comboBox_blocks_list->font();
    m_font.setPointSize(12);
    comboBox_blocks_list->setFont(m_font);

    m_font = comboBox_go_to_offset_type->font();
    m_font.setPointSize(12);
    comboBox_go_to_offset_type->setFont(m_font);

//    m_font = lineEdit_mac_timestamp->font();
//    m_font.setPointSize(12);
//    lineEdit_mac_timestamp->setFont(m_font);

    m_font = lineEdit_go_to_offset->font();
    m_font.setPointSize(12);
    lineEdit_go_to_offset->setFont(m_font);

    m_font = pushButton_goto_offset->font();
    m_font.setPointSize(12);
    pushButton_goto_offset->setFont(m_font);

    m_font = pushbutton_export_block->font();
    m_font.setPointSize(12);
    pushbutton_export_block->setFont(m_font);

    m_font = pushButton_remove_block->font();
    m_font.setPointSize(12);
    pushButton_remove_block->setFont(m_font);

    m_font = pushButton_selection_info_offset->font();
    m_font.setPointSize(12);
    pushButton_selection_info_offset->setFont(m_font);

    m_font = pushButton_selection_info_byte_count->font();
    m_font.setPointSize(12);
    pushButton_selection_info_byte_count->setFont(m_font);

    m_font = label_file_name->font();
    m_font.setPointSize(12);
    label_file_name->setFont(m_font);

    m_font = label_file_path->font();
    m_font.setPointSize(12);
    label_file_path->setFont(m_font);

    m_font = label_file_size->font();
    m_font.setPointSize(12);
    label_file_size->setFont(m_font);

    m_font = pushbutton_open_in_other_recon_viewers->font();
    m_font.setPointSize(12);
    pushbutton_open_in_other_recon_viewers->setFont(m_font);
}

void hex_viewer::reset_all_the_widgets_data()
{
    label_no_of_pages->clear();
    lineedit_goto_page->clear();
    lineEdit_search->clear();
    comboBox_search_type->setCurrentText(0);
    label_search_results->clear();
//    lineEdit_binary->clear();
//    lineEdit_8_bit->clear();
//    lineEdit_16_bit->clear();
//    lineEdit_32_bit->clear();
    pushButton_selection_info_offset->setText("");
    pushButton_selection_info_byte_count->setText("");
//    lineEdit_mac_timestamp->clear();
//    lineEdit_unix_timestamp->clear();
    lineEdit_go_to_offset->clear();
    comboBox_go_to_offset_type->setCurrentIndex(0);
    comboBox_blocks_list->setCurrentIndex(0);
    label_block_start_end_index->clear();
    total_file_size = 0;
    pushButton_next_goto_page->setEnabled(false);
    pushButton_prev_goto_page->setEnabled(false);
    label_file_name->clear();
    label_file_path->clear();
    label_file_size->clear();
}
