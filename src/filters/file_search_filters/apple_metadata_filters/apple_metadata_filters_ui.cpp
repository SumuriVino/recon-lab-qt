#include "apple_metadata_filters.h"

void apple_metadata_filters::set_options_interface(int indexofitem,QString current_text)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(MACRO_QMenu_Font_Size_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFixedSize(170,22);
    combobox_search_type_filter->setFont(font);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_options_search_items);
    combobox_search_filters_2->setFixedSize(130,22);
    combobox_search_filters_2->setFont(font);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;

    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);

        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }
    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_text);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_options_interface_add_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(MACRO_QMenu_Font_Size_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFixedSize(170,22);
    combobox_search_type_filter->setFont(font);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_options_search_items);
    combobox_search_filters_2->setFixedSize(130,22);
    combobox_search_filters_2->setFont(font);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);


    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;

    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_str);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_file_labels_interface(int indexofitem,QString current_text)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(MACRO_QMenu_Font_Size_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QPushButton *pushbutton_cross = new QPushButton(this);
    pushbutton_cross->setText("cross");
    pushbutton_cross->setFixedWidth(80);
    pushbutton_cross->setFixedSize(26,18);
    pushbutton_cross->setStyleSheet(stylesheet_pushbutton_str);
    pushbutton_cross->setFont(font);

    QPushButton *pushbutton_red = new QPushButton(this);
    pushbutton_red->setText("red");
    pushbutton_red->setFixedWidth(80);
    pushbutton_red->setFont(font);

    QPushButton *pushbutton_orange = new QPushButton(this);
    pushbutton_orange->setText("orange");
    pushbutton_orange->setFixedWidth(80);
    pushbutton_orange->setFont(font);

    QPushButton *pushbutton_yellow = new QPushButton(this);
    pushbutton_yellow->setText("yellow");
    pushbutton_yellow->setFixedWidth(80);
    pushbutton_yellow->setFont(font);

    QPushButton *pushbutton_green = new QPushButton(this);
    pushbutton_green->setText("green");
    pushbutton_green->setFixedWidth(80);
    pushbutton_green->setFont(font);

    QPushButton *pushbutton_blue = new QPushButton(this);
    pushbutton_blue->setText("blue");
    pushbutton_blue->setFixedWidth(80);
    pushbutton_blue->setFont(font);

    QPushButton *pushbutton_pink = new QPushButton(this);
    pushbutton_pink->setText("pink");
    pushbutton_pink->setFixedWidth(80);
    pushbutton_pink->setFont(font);

    QPushButton *pushbutton_grey = new QPushButton(this);
    pushbutton_grey->setText("grey");
    pushbutton_grey->setFixedWidth(80);
    pushbutton_grey->setFont(font);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedWidth(50);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedWidth(50);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(pushbutton_cross);
    hchild_lay->addWidget(pushbutton_red);
    hchild_lay->addWidget(pushbutton_orange);
    hchild_lay->addWidget(pushbutton_yellow);
    hchild_lay->addWidget(pushbutton_green);
    hchild_lay->addWidget(pushbutton_blue);
    hchild_lay->addWidget(pushbutton_pink);
    hchild_lay->addWidget(pushbutton_grey);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);

        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }
    }


    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_text);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_file_labels_interface_add_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(MACRO_QMenu_Font_Size_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);

    QPushButton *pushbutton_cross = new QPushButton(this);
    pushbutton_cross->setText("cross");
    pushbutton_cross->setFixedWidth(80);
    pushbutton_cross->setFont(font);

    QPushButton *pushbutton_red = new QPushButton(this);
    pushbutton_red->setText("red");
    pushbutton_red->setFixedWidth(80);
    pushbutton_red->setFont(font);

    QPushButton *pushbutton_orange = new QPushButton(this);
    pushbutton_orange->setText("orange");
    pushbutton_orange->setFixedWidth(80);
    pushbutton_orange->setFont(font);

    QPushButton *pushbutton_yellow = new QPushButton(this);
    pushbutton_yellow->setText("yellow");
    pushbutton_yellow->setFixedWidth(80);
    pushbutton_yellow->setFont(font);

    QPushButton *pushbutton_green = new QPushButton(this);
    pushbutton_green->setText("green");
    pushbutton_green->setFixedWidth(80);
    pushbutton_green->setFont(font);

    QPushButton *pushbutton_blue = new QPushButton(this);
    pushbutton_blue->setText("blue");
    pushbutton_blue->setFixedWidth(80);
    pushbutton_blue->setFont(font);

    QPushButton *pushbutton_pink = new QPushButton(this);
    pushbutton_pink->setText("pink");
    pushbutton_pink->setFixedWidth(80);
    pushbutton_pink->setFont(font);

    QPushButton *pushbutton_grey = new QPushButton(this);
    pushbutton_grey->setText("grey");
    pushbutton_grey->setFixedWidth(80);
    pushbutton_grey->setFont(font);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedWidth(50);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedWidth(50);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(pushbutton_cross);
    hchild_lay->addWidget(pushbutton_red);
    hchild_lay->addWidget(pushbutton_orange);
    hchild_lay->addWidget(pushbutton_yellow);
    hchild_lay->addWidget(pushbutton_green);
    hchild_lay->addWidget(pushbutton_blue);
    hchild_lay->addWidget(pushbutton_pink);
    hchild_lay->addWidget(pushbutton_grey);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_str);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " ,Q_FUNC_INFO);

}

void apple_metadata_filters::set_range_interface(int indexofitem,QString current_text)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(MACRO_QMenu_Font_Size_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_compare_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QLineEdit *lineedit_range_condition = new QLineEdit(this);
    lineedit_range_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_range_condition->setFont(font);
    lineedit_range_condition->setFixedSize(70,18);


    //    QRegularExpression rx("\\d+");
    //    QValidator *validator = new QRegularExpressionValidator(rx, this);
    //    lineedit_range_condition->setValidator(validator);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addWidget(lineedit_range_condition);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem* child;
    if(vlayout_filters->count() > indexofitem)
    {
        while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
        {
            child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);

            if(child->layout() != 0)
            {
                child->layout()->setParent(NULL);
            }
            else if(child->widget() != 0)
            {
                child->widget()->setParent(NULL);
                vlayout_filters->removeWidget(child->widget());
            }
        }
    }

    if(vlayout_filters->itemAt(indexofitem) != NULL)
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_text);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void apple_metadata_filters::set_range_interface_add_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Starts " ,Q_FUNC_INFO);

    QFont font;
    font.setPointSize(MACRO_QMenu_Font_Size_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_compare_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QLineEdit *lineedit_range_condition = new QLineEdit(this);
    lineedit_range_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_range_condition->setFont(font);
    lineedit_range_condition->setFixedSize(150,18);

    //    QRegularExpression rx("\\d+");
    //    QValidator *validator = new QRegularExpressionValidator(rx, this);
    //    lineedit_range_condition->setValidator(validator);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);


    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addWidget(lineedit_range_condition);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);


    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_str);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_lineedit_interface(int indexofitem , QString current_text)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(MACRO_QMenu_Font_Size_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QLineEdit *lineedit_filter = new QLineEdit(this);
    lineedit_filter->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_filter->setFont(font);
    lineedit_filter->setFixedHeight(18);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(lineedit_filter);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);

        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }

    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_text);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_lineedit_interface_add_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(MACRO_QMenu_Font_Size_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QLineEdit *lineedit_filter = new QLineEdit(this);
    lineedit_filter->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_filter->setFont(font);
    lineedit_filter->setFixedHeight(18);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(lineedit_filter);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_str);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_label_with_lineedit_interface(int indexofitem,QString current_text)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(MACRO_QMenu_Font_Size_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QLineEdit *lineedit_label_filter = new QLineEdit(this);
    lineedit_label_filter->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_label_filter->setFont(font);
    lineedit_label_filter->setFixedHeight(18);

    QLabel *label_filter = new QLabel(this);
    label_filter->setText("is");
    label_filter->setFont(font);
    label_filter->setFixedHeight(18);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(label_filter);
    hchild_lay->addWidget(lineedit_label_filter);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);

        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }
    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_text);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_label_with_lineedit_interface_add_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(MACRO_QMenu_Font_Size_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QLineEdit *lineedit_label_filter = new QLineEdit(this);
    lineedit_label_filter->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_label_filter->setFont(font);
    lineedit_label_filter->setFixedHeight(18);

    QLabel *label_filter = new QLabel(this);
    label_filter->setText("is");
    label_filter->setFont(font);
    label_filter->setFixedHeight(22);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(label_filter);
    hchild_lay->addWidget(lineedit_label_filter);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_str);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_size_interface(int indexofitem,QString current_text)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_compare_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QLineEdit *linesdit_size_condition = new QLineEdit(this);
    linesdit_size_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    linesdit_size_condition->setFont(font);
    linesdit_size_condition->setFixedSize(80,18);

    //    QRegularExpression rx("\\d+");
    //    QValidator *validator = new QRegularExpressionValidator(rx, this);
    //    linesdit_size_condition->setValidator(validator);

    QComboBox *combobox_search_filter_3 = new QComboBox(this);
    combobox_search_filter_3->addItems(list_search_type_size_filters_items);
    combobox_search_filter_3->setFont(font);
    combobox_search_filter_3->setFixedSize(100,22);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addWidget(linesdit_size_condition);
    hchild_lay->addWidget(combobox_search_filter_3);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);

        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }
    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_text);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void apple_metadata_filters::set_size_interface_add_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_compare_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QLineEdit *linesdit_size_condition = new QLineEdit(this);
    linesdit_size_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    linesdit_size_condition->setFont(font);
    linesdit_size_condition->setFixedSize(80,18);

    //    QRegularExpression rx("\\d+");
    //    QValidator *validator = new QRegularExpressionValidator(rx, this);
    //    linesdit_size_condition->setValidator(validator);

    QComboBox *combobox_search_filter_3 = new QComboBox(this);
    combobox_search_filter_3->addItems(list_search_type_size_filters_items);
    combobox_search_filter_3->setFont(font);
    combobox_search_filter_3->setFixedSize(100,22);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addWidget(linesdit_size_condition);
    hchild_lay->addWidget(combobox_search_filter_3);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_str);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_duration_interface(int indexofitem,QString current_text)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QRegularExpression rx("\\d+");
    QValidator *validator = new QRegularExpressionValidator(rx, this);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_compare_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);


    QLineEdit *lineEdit_duration_hr_condition = new QLineEdit(this);
    lineEdit_duration_hr_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineEdit_duration_hr_condition->setFixedSize(50,18);
    lineEdit_duration_hr_condition->setFont(font);
    // lineEdit_duration_hr_condition->setValidator(validator);

    QLabel *label_duration_hr_condition = new QLabel(this);
    label_duration_hr_condition->setText("hours");
    label_duration_hr_condition->setFont(font);
    label_duration_hr_condition->setFixedHeight(22);

    QLineEdit *lineedit_duration_min_condition = new QLineEdit(this);
    lineedit_duration_min_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_duration_min_condition->setFixedSize(50,18);
    lineedit_duration_min_condition->setFont(font);
    //  lineedit_duration_min_condition->setValidator(validator);

    QLabel *label_duration_min_condition = new QLabel(this);
    label_duration_min_condition->setFixedHeight(22);
    label_duration_min_condition->setText("minutes");
    label_duration_min_condition->setFont(font);

    QLineEdit *lineedit_duration_sec_condition = new QLineEdit(this);
    lineedit_duration_sec_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_duration_sec_condition->setFixedSize(50,18);
    lineedit_duration_sec_condition->setFont(font);
    // lineedit_duration_sec_condition->setValidator(validator);

    QLabel *label_duration_sec_condition = new QLabel(this);
    label_duration_sec_condition->setText("seconds");
    label_duration_sec_condition->setFont(font);
    label_duration_sec_condition->setFixedHeight(22);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addWidget(lineEdit_duration_hr_condition);
    hchild_lay->addWidget(label_duration_hr_condition);
    hchild_lay->addWidget(lineedit_duration_min_condition);
    hchild_lay->addWidget(label_duration_min_condition);
    hchild_lay->addWidget(lineedit_duration_sec_condition);
    hchild_lay->addWidget(label_duration_sec_condition);
    hchild_lay->addItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);

        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }
    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_text);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_duration_interface_add_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QRegularExpression rx("\\d+");
    QValidator *validator = new QRegularExpressionValidator(rx, this);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_compare_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QLineEdit *lineEdit_duration_hr_condition = new QLineEdit(this);
    lineEdit_duration_hr_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineEdit_duration_hr_condition->setFixedSize(50,18);
    lineEdit_duration_hr_condition->setFont(font);
    //  lineEdit_duration_hr_condition->setValidator(validator);

    QLabel *label_duration_hr_condition = new QLabel(this);
    label_duration_hr_condition->setText("hours");
    label_duration_hr_condition->setFont(font);
    label_duration_hr_condition->setFixedHeight(18);

    QLineEdit *lineedit_duration_min_condition = new QLineEdit(this);
    lineedit_duration_min_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_duration_min_condition->setFixedSize(50,18);
    lineedit_duration_min_condition->setFont(font);
    // lineedit_duration_min_condition->setValidator(validator);

    QLabel *label_duration_min_condition = new QLabel(this);
    label_duration_min_condition->setText("minutes");
    label_duration_min_condition->setFont(font);
    label_duration_min_condition->setFixedHeight(18);

    QLineEdit *lineedit_duration_sec_condition = new QLineEdit(this);
    lineedit_duration_sec_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_duration_sec_condition->setFixedSize(50,18);
    lineedit_duration_sec_condition->setFont(font);
    // lineedit_duration_sec_condition->setValidator(validator);

    QLabel *label_duration_sec_condition = new QLabel(this);
    label_duration_sec_condition->setText("seconds");
    label_duration_sec_condition->setFont(font);
    label_duration_sec_condition->setFixedHeight(18);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addWidget(lineEdit_duration_hr_condition);
    hchild_lay->addWidget(label_duration_hr_condition);
    hchild_lay->addWidget(lineedit_duration_min_condition);
    hchild_lay->addWidget(label_duration_min_condition);
    hchild_lay->addWidget(lineedit_duration_sec_condition);
    hchild_lay->addWidget(label_duration_sec_condition);
    hchild_lay->addItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_str);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    //    connect(lineEdit_duration_hr_condition, SIGNAL(textChanged(QString)), this,SLOT(on_lineEdit_textChanged(QString)));
    //    connect(lineedit_duration_min_condition, SIGNAL(textChanged(QString)), this,SLOT(on_lineEdit_textChanged(QString)));
    //    connect(lineedit_duration_sec_condition, SIGNAL(textChanged(QString)), this,SLOT(on_lineEdit_textChanged(QString)));

    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_single_combobox_interface(int indexofitem,QString current_text)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_system_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;

    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);

        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }

    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_text);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_single_combobox_interface_add_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_system_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;

    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_str);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_single_visible_combobox_interface(int indexofitem,QString current_text)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_visible_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);

        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }
    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_text);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_single_visible_combobox_interface_add_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_visible_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_str);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_names_interface(int indexofitem, QString current_text)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_names_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QLineEdit *lineedit_name_condition = new QLineEdit(this);
    lineedit_name_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_name_condition->setFixedSize(150,18);
    lineedit_name_condition->setFont(font);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addWidget(lineedit_name_condition);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    hchild_lay->setStretchFactor(lineedit_name_condition,1);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);

        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }
    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    vlayout_filters->insertLayout(indexofitem,hchild_lay);

    combobox_search_type_filter->setCurrentText(current_text);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void apple_metadata_filters::set_names_interface_add_filter(int indexofitem,QString current_str)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_names_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QLineEdit *lineedit_name_condition = new QLineEdit(this);
    lineedit_name_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_name_condition->setFont(font);
    lineedit_name_condition->setFixedSize(150,18);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addWidget(lineedit_name_condition);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    hchild_lay->setStretchFactor(lineedit_name_condition,1);

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_str);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_contents_interface(int indexofitem, QString current_text)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFixedSize(170,22);
    combobox_search_type_filter->setFont(font);

    QLabel *label_contents_ext_attr = new QLabel(this);
    label_contents_ext_attr->setText("contains");
    label_contents_ext_attr->setFont(font);
    label_contents_ext_attr->setFixedHeight(22);

    QLineEdit *lineedit_contents_condition = new QLineEdit(this);
    lineedit_contents_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_contents_condition->setMinimumWidth(150);
    lineedit_contents_condition->setMaximumWidth(200);
    lineedit_contents_condition->setFont(font);
    lineedit_contents_condition->setFixedHeight(18);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(label_contents_ext_attr);
    hchild_lay->addWidget(lineedit_contents_condition);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);

        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);

            vlayout_filters->removeWidget(child->widget());
        }
    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_text);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_contents_interface_add_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFixedSize(170,22);
    combobox_search_type_filter->setFont(font);

    QLabel *label_contents_ext_attr = new QLabel(this);
    label_contents_ext_attr->setText("contains");
    label_contents_ext_attr->setFont(font);
    label_contents_ext_attr->setFixedHeight(22);

    QLineEdit *lineedit_contents_condition = new QLineEdit(this);
    lineedit_contents_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_contents_condition->setFixedSize(180,18);
    lineedit_contents_condition->setFont(font);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(label_contents_ext_attr);
    hchild_lay->addWidget(lineedit_contents_condition);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    combobox_search_type_filter->setCurrentText(current_str);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_kind_interface(int indexofitem, QString current_text)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QLabel *label_kind_ext_attr = new QLabel(this);
    label_kind_ext_attr->setText("is");
    label_kind_ext_attr->setFont(font);
    label_kind_ext_attr->setFixedHeight(22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_kind_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int + 100,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(label_kind_ext_attr);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);
        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }
    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    combobox_search_type_filter->setCurrentText(current_text);

    vlayout_filters->insertLayout(indexofitem,hchild_lay);
    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(combobox_search_filters_2,SIGNAL(currentTextChanged(QString)), this, SLOT(slot_on_combobox_ext_attr_kind_current_index_changed(QString)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void apple_metadata_filters::set_kind_interface_add_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QLabel *label_kind_ext_attr = new QLabel(this);
    label_kind_ext_attr->setText("is");
    label_kind_ext_attr->setFont(font);
    label_kind_ext_attr->setMaximumHeight(22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_kind_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int + 100,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(label_kind_ext_attr);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    combobox_search_type_filter->setCurrentText(current_str);
    vlayout_filters->insertLayout(indexofitem,hchild_lay);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(combobox_search_filters_2,SIGNAL(currentTextChanged(QString)), this, SLOT(slot_on_combobox_ext_attr_kind_current_index_changed(QString)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_date_interface(int indexofitem, QString current_text)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QLabel *label_is = new QLabel(this);
    label_is->setText("is");
    label_is->setFont(font);
    label_is->setFixedHeight(22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_date_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QDateTimeEdit *datetime_date_ext_attr_condition = new QDateTimeEdit(this);
    datetime_date_ext_attr_condition->setDate(QDate::currentDate());
    QDate min_date  = QDate::fromString(QString("1970/01/01"), "yyyy/MM/dd");
    QDate max_date  = QDate::currentDate();
    datetime_date_ext_attr_condition->setMinimumDate(min_date);
    datetime_date_ext_attr_condition->setMaximumDate(max_date);
    datetime_date_ext_attr_condition->setDisplayFormat("yyyy/MM/dd");
    datetime_date_ext_attr_condition->setFont(font);
    datetime_date_ext_attr_condition->setFixedHeight(20);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(label_is);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addWidget(datetime_date_ext_attr_condition);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);
        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }

    }
    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    combobox_search_type_filter->setCurrentText(current_text);

    vlayout_filters->insertLayout(indexofitem,hchild_lay);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(combobox_search_filters_2,SIGNAL(currentTextChanged(QString)), this, SLOT(slot_on_combobox_ext_attr_dates_current_index_changed(QString)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void apple_metadata_filters::set_date_interface_add_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QLabel *label_is = new QLabel(this);
    label_is->setText("is");
    label_is->setFont(font);
    label_is->setFixedHeight(22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_date_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

//    QComboBox *combobox_search_filters_3 = new QComboBox(this);
//    combobox_search_filters_3->addItems(list_search_type_date_days_search_items);
//    combobox_search_filters_3->setFont(font);
//    combobox_search_filters_3->setFixedSize(100,22);


//    QLineEdit *lineedit_date_ext_attr_condition = new QLineEdit(this);
//    lineedit_date_ext_attr_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
//    lineedit_date_ext_attr_condition->setFont(font);
//    lineedit_date_ext_attr_condition->setFixedWidth(70);
//    lineedit_date_ext_attr_condition->setFixedSize(70,18);

    QDateTimeEdit *datetime_date_ext_attr_condition = new QDateTimeEdit(this);
    datetime_date_ext_attr_condition->setDate(QDate::currentDate());
    QDate min_date  = QDate::fromString(QString("1970/01/01"), "yyyy/MM/dd");
    QDate max_date  = QDate::currentDate();

    datetime_date_ext_attr_condition->setMinimumDate(min_date);
    datetime_date_ext_attr_condition->setMaximumDate(max_date);
    datetime_date_ext_attr_condition->setDisplayFormat("yyyy/MM/dd");
    datetime_date_ext_attr_condition->setFont(font);
    datetime_date_ext_attr_condition->setFixedHeight(20);

    //    QRegularExpression rx("\\d+");
    //    QValidator *validator = new QRegularExpressionValidator(rx, this);
    //    lineedit_date_ext_attr_condition->setValidator(validator);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(label_is);
    hchild_lay->addWidget(combobox_search_filters_2);
//    hchild_lay->addWidget(lineedit_date_ext_attr_condition);
//    hchild_lay->addWidget(combobox_search_filters_3);
    hchild_lay->addWidget(datetime_date_ext_attr_condition);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    combobox_search_type_filter->setCurrentText(current_str);
    vlayout_filters->insertLayout(indexofitem,hchild_lay);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(combobox_search_filters_2,SIGNAL(currentTextChanged(QString)), this, SLOT(slot_on_combobox_ext_attr_dates_current_index_changed(QString)));

    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_kind_images_filter(int indexofitem, QString current_str, QStringList listof_kind_data)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QLabel *label_kind_ext_attr = new QLabel(this);
    label_kind_ext_attr->setText("is");
    label_kind_ext_attr->setFont(font);
    label_kind_ext_attr->setFixedHeight(22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_kind_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QComboBox *combobox_search_filters_3 = new QComboBox(this);
    combobox_search_filters_3->addItems(listof_kind_data);
    combobox_search_filters_3->setFont(font);
    combobox_search_filters_3->setFixedSize(100,22);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(label_kind_ext_attr);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addWidget(combobox_search_filters_3);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem *item_combo = vlayout_filters->itemAt(indexofitem);
    int index_main = get_main_combobox_index(item_combo);


    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);
        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }
    }
    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    combobox_search_type_filter->setCurrentIndex(index_main);
    combobox_search_filters_2->setCurrentText(current_str);
    vlayout_filters->insertLayout(indexofitem,hchild_lay);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(combobox_search_filters_2,SIGNAL(currentTextChanged(QString)), this, SLOT(slot_on_combobox_ext_attr_kind_current_index_changed(QString)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void apple_metadata_filters::set_kind_others_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QLabel *label_kind_ext_attr = new QLabel(this);
    label_kind_ext_attr->setText("is");
    label_kind_ext_attr->setFont(font);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_kind_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QLineEdit *lineedit_kind_ext_attr_filter = new QLineEdit(this);
    lineedit_kind_ext_attr_filter->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_kind_ext_attr_filter->setFont(font);
    lineedit_kind_ext_attr_filter->setFixedSize(150,18);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(label_kind_ext_attr);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addWidget(lineedit_kind_ext_attr_filter);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem *item_combo = vlayout_filters->itemAt(indexofitem);
    int index_main = get_main_combobox_index(item_combo);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);
        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }
    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    combobox_search_type_filter->setCurrentIndex(index_main);
    combobox_search_filters_2->setCurrentText(current_str);
    vlayout_filters->insertLayout(indexofitem,hchild_lay);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(combobox_search_filters_2,SIGNAL(currentTextChanged(QString)), this, SLOT(slot_on_combobox_ext_attr_kind_current_index_changed(QString)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

}

void apple_metadata_filters::set_kind_interface_all_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QLabel *label_kind_ext_attr = new QLabel(this);
    label_kind_ext_attr->setText("is");
    label_kind_ext_attr->setFont(font);
    label_kind_ext_attr->setFixedHeight(22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_kind_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);


    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(label_kind_ext_attr);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem *item_combo = vlayout_filters->itemAt(indexofitem);
    int index_main = get_main_combobox_index(item_combo);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);
        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }
    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    combobox_search_type_filter->setCurrentIndex(index_main);
    combobox_search_filters_2->setCurrentText(current_str);
    vlayout_filters->insertLayout(indexofitem,hchild_lay);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(combobox_search_filters_2,SIGNAL(currentTextChanged(QString)), this, SLOT(slot_on_combobox_ext_attr_kind_current_index_changed(QString)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_dates_interface_with_display_format(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QLabel *label_is = new QLabel(this);
    label_is->setText("is");
    label_is->setFont(font);
    label_is->setFixedHeight(22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_date_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QDateTimeEdit *datetime_date_ext_attr_condition = new QDateTimeEdit(this);
    datetime_date_ext_attr_condition->setDate(QDate::currentDate());
//    datetime_date_ext_attr_condition->setMinimumDate(QDate::currentDate().addDays(-20000));
//    datetime_date_ext_attr_condition->setMaximumDate(QDate::currentDate().addDays(1000));
    QDate min_date  = QDate::fromString(QString("1970/01/01"), "yyyy/MM/dd");
    QDate max_date  = QDate::currentDate();
    datetime_date_ext_attr_condition->setMinimumDate(min_date);
    datetime_date_ext_attr_condition->setMaximumDate(max_date);
    datetime_date_ext_attr_condition->setDisplayFormat("yyyy/MM/dd");
    datetime_date_ext_attr_condition->setFont(font);
    datetime_date_ext_attr_condition->setFixedHeight(20);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(label_is);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addWidget(datetime_date_ext_attr_condition);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem *item_combo = vlayout_filters->itemAt(indexofitem);
    int index_main = get_main_combobox_index(item_combo);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);
        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }
    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    combobox_search_type_filter->setCurrentIndex(index_main);
    combobox_search_filters_2->setCurrentText(current_str);
    vlayout_filters->insertLayout(indexofitem,hchild_lay);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(combobox_search_filters_2,SIGNAL(currentTextChanged(QString)), this, SLOT(slot_on_combobox_ext_attr_dates_current_index_changed(QString)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_dates_interface_others_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QLabel *label_is = new QLabel(this);
    label_is->setText("is");
    label_is->setFont(font);
    label_is->setFixedHeight(22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_date_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(label_is);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem *item_combo = vlayout_filters->itemAt(indexofitem);
    int index_main = get_main_combobox_index(item_combo);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);
        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }
    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    combobox_search_type_filter->setCurrentIndex(index_main);
    combobox_search_filters_2->setCurrentText(current_str);

    vlayout_filters->insertLayout(indexofitem,hchild_lay);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(combobox_search_filters_2,SIGNAL(currentTextChanged(QString)), this, SLOT(slot_on_combobox_ext_attr_dates_current_index_changed(QString)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void apple_metadata_filters::set_date_interface_within_filter(int indexofitem, QString current_str)
{
    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);

    QFont font;
    font.setPointSize(font_size_of_spotlight_search_int);

    QComboBox *combobox_search_type_filter = new QComboBox(this);
    combobox_search_type_filter->addItems(list_search_types);
    combobox_search_type_filter->setFont(font);
    combobox_search_type_filter->setFixedSize(170,22);

    QLabel *label_is = new QLabel(this);
    label_is->setText("is");
    label_is->setFont(font);
    label_is->setFixedHeight(22);

    QComboBox *combobox_search_filters_2 = new QComboBox(this);
    combobox_search_filters_2->addItems(list_search_type_date_search_items);
    combobox_search_filters_2->setFont(font);
    combobox_search_filters_2->setFixedSize(130,22);

    QComboBox *combobox_search_filters_3 = new QComboBox(this);
    combobox_search_filters_3->addItems(list_search_type_date_days_search_items);
    combobox_search_filters_3->setFont(font);
    combobox_search_filters_3->setFixedSize(100,22);

    QLineEdit *lineedit_date_ext_attr_condition = new QLineEdit(this);
    lineedit_date_ext_attr_condition->setAttribute(Qt::WA_MacShowFocusRect,false);
    lineedit_date_ext_attr_condition->setFont(font);
    lineedit_date_ext_attr_condition->setFixedSize(70,18);

    //    QRegularExpression rx("\\d+");
    //    QValidator *validator = new QRegularExpressionValidator(rx, this);
    //    lineedit_date_ext_attr_condition->setValidator(validator);

    QPushButton *pushbutton_delete = new QPushButton(this);
    pushbutton_delete->setFont(font);
    pushbutton_delete->setText("-");
    pushbutton_delete->setFixedSize(26,18);
    pushbutton_delete->setStyleSheet(stylesheet_pushbutton_str);

    QPushButton *pushbutton_add = new QPushButton(this);
    pushbutton_add->setFont(font);
    pushbutton_add->setText("+");
    pushbutton_add->setFixedSize(26,18);
    pushbutton_add->setStyleSheet(stylesheet_pushbutton_str);

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int,enum_File_System_Spacer_Height_int,QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(label_is);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addWidget(lineedit_date_ext_attr_condition);
    hchild_lay->addWidget(combobox_search_filters_3);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);

    QLayoutItem *item_combo = vlayout_filters->itemAt(indexofitem);
    int index_main = get_main_combobox_index(item_combo);

    QLayoutItem* child;
    while(vlayout_filters->itemAt(indexofitem)->layout()->count()!=0)
    {
        child = vlayout_filters->itemAt(indexofitem)->layout()->takeAt(0);
        if(child->layout() != 0)
        {
            child->layout()->setParent(NULL);
        }
        else if(child->widget() != 0)
        {
            child->widget()->setParent(NULL);
            vlayout_filters->removeWidget(child->widget());
        }
    }

    if(vlayout_filters->itemAt(indexofitem)->isEmpty())
    {
        vlayout_filters->removeItem(vlayout_filters->itemAt(indexofitem));
    }

    combobox_search_type_filter->setCurrentIndex(index_main);
    combobox_search_filters_2->setCurrentText(current_str);

    vlayout_filters->insertLayout(indexofitem,hchild_lay);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(combobox_search_filters_2,SIGNAL(currentTextChanged(QString)), this, SLOT(slot_on_combobox_ext_attr_dates_current_index_changed(QString)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    recon_static_functions::app_debug(" Ends " ,Q_FUNC_INFO);
}
