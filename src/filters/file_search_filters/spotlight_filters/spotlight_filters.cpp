#include "spotlight_filters.h"

spotlight_filters::spotlight_filters(QWidget *parent) : QWidget(parent)
{
    list_search_types << MACRO_Generic_Filter_Condition_Name << MACRO_Generic_Filter_Condition_Contents
                      << MACRO_Generic_Filter_Condition_Kind << MACRO_Generic_Filter_Condition_Last_Opened_Date
                      << MACRO_Generic_Filter_Condition_Last_Modified_Date << MACRO_Generic_Filter_Condition_Others;

    list_search_type_names_search_items << MACRO_Generic_Filter_Condition_Matches << MACRO_Generic_Filter_Condition_Contains
                                        << MACRO_Generic_Filter_Condition_Begins_With << MACRO_Generic_Filter_Condition_Ends_With;
                                       // << MACRO_Generic_Filter_Condition_Is << MACRO_Generic_Filter_Condition_Is_Not;

    list_search_type_kind_search_items  << MACRO_Generic_Filter_Condition_Any << MACRO_Generic_Filter_Condition_Application
                                        << MACRO_Generic_Filter_Condition_Archive << MACRO_Generic_Filter_Condition_Document
                                        << MACRO_Generic_Filter_Condition_Executable << MACRO_Generic_Filter_Condition_Folder
                                        << MACRO_Generic_Filter_Condition_Image << MACRO_Generic_Filter_Condition_Movie
                                        << MACRO_Generic_Filter_Condition_Music << MACRO_Generic_Filter_Condition_Pdf
                                        << MACRO_Generic_Filter_Condition_Presentation << MACRO_Generic_Filter_Condition_Text
                                        << MACRO_Generic_Filter_Condition_Other;

    list_search_type_kind_images_search_items << MACRO_Generic_Filter_Condition_All << MACRO_Generic_Filter_Condition_Jpeg
                                              << MACRO_Generic_Filter_Condition_Tiff << MACRO_Generic_Filter_Condition_Gif
                                              << MACRO_Generic_Filter_Condition_Png << MACRO_Generic_Filter_Condition_Bmp;

    list_search_type_kind_music_search_items << MACRO_Generic_Filter_Condition_All << MACRO_Generic_Filter_Condition_Mp3
                                             << MACRO_Generic_Filter_Condition_Aac << MACRO_Generic_Filter_Condition_Purchased;

    list_search_type_kind_textfiles_search_items << MACRO_Generic_Filter_Condition_All << MACRO_Generic_Filter_Condition_Plain_Text
                                                 << MACRO_Generic_Filter_Condition_Rich_Text << MACRO_Generic_Filter_Condition_Html
                                                 << MACRO_Generic_Filter_Condition_Xml << MACRO_Generic_Filter_Condition_Source_Code;

    list_search_type_date_search_items  << MACRO_Generic_Filter_Condition_Within_Last << MACRO_Generic_Filter_Condition_Exactly
                                        << MACRO_Generic_Filter_Condition_Before << MACRO_Generic_Filter_Condition_After
                                        << MACRO_Generic_Filter_Condition_Today << MACRO_Generic_Filter_Condition_Yesterday
                                        << MACRO_Generic_Filter_Condition_This_Week << MACRO_Generic_Filter_Condition_This_Month
                                        << MACRO_Generic_Filter_Condition_This_Year;

    list_search_type_date_days_search_items  << MACRO_Generic_Filter_Condition_Days << MACRO_Generic_Filter_Condition_Weeks
                                             << MACRO_Generic_Filter_Condition_Months << MACRO_Generic_Filter_Condition_Years;

    list_search_type_options_search_items << MACRO_Generic_Filter_Condition_Any << MACRO_Generic_Filter_Condition_Yes
                                          << MACRO_Generic_Filter_Condition_No;

    list_search_type_size_filters_items << MACRO_Generic_Filter_Condition_KB << MACRO_Generic_Filter_Condition_MB
                                        << MACRO_Generic_Filter_Condition_GB;

    list_search_type_compare_search_items << MACRO_Generic_Filter_Condition_Equal << MACRO_Generic_Filter_Condition_Is_Greater_Than
                                          << MACRO_Generic_Filter_Condition_Is_Less_Than << MACRO_Generic_Filter_Condition_Is_Not;

    list_search_type_system_search_items << MACRO_Generic_Filter_Condition_Are_Not_Included << MACRO_Generic_Filter_Condition_Are_Included;

    list_search_type_visible_search_items << MACRO_Generic_Filter_Condition_Visible_Items << MACRO_Generic_Filter_Condition_Invisible_Items
                                          << MACRO_Generic_Filter_Condition_Visible_Or_Invisible;

    //model_filters = new QStandardItemModel(0, 5);
    vlayout_main = new QVBoxLayout(this);
    vlayout_filters_and_pushbutton_back_next_combobox = new QVBoxLayout;
    vlayout_filters = new QVBoxLayout;
    search_count = 0;
    last_save_index = 0;

    ///Menulist when click on other
    apple_attributes_obj = new apple_attributes(this);

    font_size_of_spotlight_search_int = 11;
    stylesheet_pushbutton_str = "QPushButton { margin: 1px; border-color: #D0D0D0;  border-style: solid; border-radius: 1px; border-width: 1px; color: black;"
                                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FFFFFF, stop: 1 #FFFFFF); }"
                                "QPushButton:pressed {  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FFFFFF, stop: 1 #FFFFFF);  border-color: #000000;}"
                                "QPushButton:checked {  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FFFFFF, stop: 1 #FFFFFF);  border-color: #484848;}";

    pub_create_ui();

    vlayout_main->addLayout(vlayout_filters_and_pushbutton_back_next_combobox);
    vlayout_main->setSpacing(0);
    vlayout_main->setContentsMargins(1,0,1,0);
    setLayout(vlayout_main);
}

void spotlight_filters::pub_set_essentials()
{
    db_path_case_confg = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
}

void spotlight_filters::pub_create_ui()
{
    if(search_count > enum_File_System_MAX_Filter_Count_int)
        return;

    QFont font;
    //    font.setPointSize(MACRO_QMenu_Font_Size_int);
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
    lineedit_name_condition->setMinimumWidth(70);
    lineedit_name_condition->setMaximumWidth(150);
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

    QSpacerItem *spaceitem = new QSpacerItem(enum_File_System_Spacer_Width_int , enum_File_System_Spacer_Height_int , QSizePolicy::Expanding);

    QHBoxLayout *hchild_lay = new QHBoxLayout;
    hchild_lay->addWidget(combobox_search_type_filter);
    hchild_lay->addWidget(combobox_search_filters_2);
    hchild_lay->addWidget(lineedit_name_condition);
    hchild_lay->addSpacerItem(spaceitem);
    hchild_lay->addWidget(pushbutton_delete);
    hchild_lay->addWidget(pushbutton_add);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setContentsMargins(0,0,0,0);
    hchild_lay->setSpacing(enum_File_System_Spacing_int);

    //    vlayout_filters->setAlignment(Qt::AlignLeft);
    vlayout_filters->addLayout(hchild_lay);
    vlayout_filters->setSizeConstraint(QLayout::SetMinAndMaxSize);
    vlayout_filters->setSpacing(0);
    vlayout_filters->setContentsMargins(0,0,0,0);
    vlayout_filters->setContentsMargins(0,0,0,0);

    QSpacerItem *spaceitem_search = new QSpacerItem(enum_File_System_Spacer_Width_int , enum_File_System_Spacer_Height_int , QSizePolicy::Expanding);

    QHBoxLayout *hlayout_search = new QHBoxLayout;
    hlayout_search->insertSpacerItem(hlayout_search->count() - 2,spaceitem_search);
    hlayout_search->setContentsMargins(0,0,0,0);
    hlayout_search->setContentsMargins(0,0,0,0);

    vlayout_filters_and_pushbutton_back_next_combobox->addLayout(vlayout_filters);

    connect(combobox_search_type_filter,SIGNAL(currentIndexChanged(int)), this, SLOT(slot_on_combobox_search_type_current_index_changed(int)));
    connect(pushbutton_delete, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_delete_clicked()));
    connect(pushbutton_add, SIGNAL(clicked()), this,SLOT(slot_on_pushbutton_add_clicked()));

    search_count++;
}

void spotlight_filters::on_appl_attribute_ok_button_click()
{
    list_search_types =  get_main_list_from_appl_attribute_database();

    list_search_types << MACRO_Generic_Filter_Condition_Others;

    sub_set_all_index();
}

QStringList spotlight_filters::get_main_list_from_appl_attribute_database()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";;

    QStringList list;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Open -------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(db_path , Q_FUNC_INFO);
        return list;
    }

    QSqlQuery query_select(destination_db);

    query_select.prepare("select attribute_name from tbl_apple_attributes where bookmark_filter = 1");

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(" query_select execution ---------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug(db_path , Q_FUNC_INFO);
        return list;
    }

    while (query_select.next())
    {
        QString name = query_select.value(0).toString();
        list << name;
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    destination_db.close();

    return list;
}


void spotlight_filters::on_appl_attribute_cancel_button_click()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(db_path_case_confg);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + db_path_case_confg,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    for(int p = 0; p < list_search_types.size(); p++)
    {
        QString name = list_search_types.at(p);
        name = name.trimmed();

        query.prepare("update tbl_apple_attributes set bookmark_filter = 1 where attribute_name = ?");
        query.addBindValue(name);
        if(!query.exec())
        {
            recon_static_functions::app_debug(" query execution for filter_menu-------FAILED------ " ,Q_FUNC_INFO);
            recon_static_functions::app_debug(query.lastError().text() , Q_FUNC_INFO);
            recon_static_functions::app_debug(query.executedQuery(), Q_FUNC_INFO);
        }
    }

    destination_db.close();

    sub_set_all_index();

    recon_static_functions::app_debug(" ends " , Q_FUNC_INFO);
}

int spotlight_filters::sub_set_all_index()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    for(int i = 0; i < vlayout_filters->count() ; i++)
    {
        QLayoutItem * item_combo = vlayout_filters->itemAt(i);

        for(int j = 0 ; j < item_combo->layout()->count() ; j++)
        {
            QLayoutItem *item2 = item_combo->layout()->itemAt(j);

            if(item2->spacerItem())
            {
                continue;
            }

            QString str_classname = item2->widget()->metaObject()->className();

            if(str_classname == "QComboBox" )
            {
                QComboBox *box = new QComboBox(this);
                box = (QComboBox*) item2->widget();
                box->setInsertPolicy(QComboBox::InsertAtBottom);

                QString text = box->currentText();

                QStringList list2;

                box->blockSignals(true);


                for (int i = box->count(); i >= 0; --i)
                {
                    if (i != box->currentIndex())
                    {
                        box->removeItem(i);
                    }
                }

                std::vector< QString > list( box->count() );

                for( auto l = 0; l < list.size(); l++ )
                {

                    list[l] = box->itemText( l );

                    list2 << list[l];
                }

                for(int k = 0 ; k < list_search_types.size() ; k++)
                {

                    if(!list2.contains(list_search_types.at(k)))
                    {
                        box->addItem(list_search_types.at(k));
                    }
                }

                box->blockSignals(false);
                break;
            }
        }
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return 0;
}

int spotlight_filters::get_main_combobox_index(QLayoutItem *item_combobox)
{
    int index_main = 0;

    for(int j = 0 ; j < item_combobox->layout()->count() ; j++)
    {
        QLayoutItem *item2 = item_combobox->layout()->itemAt(j);

        if(item2->spacerItem())
        {
            continue;
        }

        QString str_classname = item2->widget()->metaObject()->className();

        if(str_classname == "QComboBox")
        {
            QComboBox *box = new QComboBox(this);
            box =(QComboBox*) item2->widget();
            index_main = box->currentIndex();
            break;
        }
    }

    return index_main;
}

QString spotlight_filters::pub_collect_all_spotlight_search_filters_information()
{
    QStringList list;

    for(int i = 0 ; i < vlayout_filters->count() ; i++)
    {
        QString namestr = "";

        QLayoutItem *item = vlayout_filters->itemAt(i);
        for(int j = 0 ; j < item->layout()->count() ; j++)
        {
            QLayoutItem *item2 = item->layout()->itemAt(j);

            if(item2->spacerItem())
            {
                continue;
            }

            QString str_classname = item2->widget()->metaObject()->className();

            if(str_classname == "QComboBox")
            {
                QComboBox *box = new QComboBox(this);
                box =(QComboBox*) item2->widget();

                namestr.append(box->currentText() + MACRO_RECON_Splitter_3_dollar_in_brace);
            }

            if(str_classname == "QDateTimeEdit")
            {
                QDateTimeEdit *wid = new QDateTimeEdit(this);
                wid =(QDateTimeEdit*) item2->widget();

                namestr.append(wid->text() + MACRO_RECON_Splitter_3_dollar_in_brace);
            }

            if(str_classname == "QLineEdit")
            {
                QLineEdit *wid = new QLineEdit(this);
                wid =(QLineEdit*) item2->widget();

                namestr.append(wid->text() + MACRO_RECON_Splitter_3_dollar_in_brace);
            }

            if(str_classname == "QLabel")
            {
                QLabel *wid = new QLabel(this);
                wid =(QLabel*) item2->widget();

                namestr.append(wid->text() + MACRO_RECON_Splitter_3_dollar_in_brace);
            }
        }

        if(namestr.endsWith(MACRO_RECON_Splitter_3_dollar_in_brace))
            namestr.chop(QString(MACRO_RECON_Splitter_3_dollar_in_brace).size());

        if(namestr != "")
            list << namestr;
    }

    QString mdfind_condition;

    mdfind_condition = make_strings_for_mdfind_search_condition(list);

    return mdfind_condition;
}

QString spotlight_filters::make_strings_for_mdfind_search_condition(QStringList list)
{
    recon_static_functions::app_debug(" Starts" , Q_FUNC_INFO);

    QString mdfind_conditions;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(db_path_case_confg);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("destination_db Open -------FAILED------ " + db_path_case_confg , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() , Q_FUNC_INFO);
        return mdfind_conditions;
    }

    QSqlQuery query(destination_db);

    for(int i = 0 ; i < list.size() ; i++)
    {
        QString name_str = list.at(i);

        QStringList sub_list = name_str.split(MACRO_RECON_Splitter_3_dollar_in_brace, Qt::SkipEmptyParts);

        QString name;
        if(sub_list.size() > 0)
        {
            name = sub_list.at(0);
            name = name.trimmed();
        }

        query.prepare("select widget_type, metadata_attribute_name from tbl_apple_attributes where attribute_name = ?");
        query.addBindValue(name);
        if(!query.exec())
        {
            recon_static_functions::app_debug(" query execution ------FAILED----- " , Q_FUNC_INFO);
            recon_static_functions::app_debug(destination_db.lastError().text() , Q_FUNC_INFO);
            recon_static_functions::app_debug(db_path_case_confg , Q_FUNC_INFO);
            continue;
        }

        int type;
        QString command;
        if(query.next())
        {
            type = query.value(0).toInt();
            command = query.value(1).toString();
        }

        if(type == enum_File_System_Widget_Type_FILESYSTEM_NAME)
        {
            mdfind_conditions += get_mdkind_condition_str_for_widget_type_name(sub_list,command);
        }

        if(type == enum_File_System_Widget_Type_FILESYSTEM_OPTIONS)
        {
            mdfind_conditions += get_mdkind_condition_str_for_widget_type_options(sub_list,command);
        }

        if(type == enum_File_System_Widget_Type_FILESYSTEM_RANGE)
        {
            mdfind_conditions += get_mdkind_condition_str_for_widget_type_range(sub_list,command);
        }

        if(type == enum_File_System_Widget_Type_FILESYSTEM_CONTENT)
        {
            QStringList sub_list = name_str.split(MACRO_RECON_Splitter_3_dollar_in_brace, Qt::SkipEmptyParts);

            mdfind_conditions += get_mdkind_condition_str_for_widget_type_content(sub_list,command);
        }

        if(type == enum_File_System_Widget_Type_FILESYSTEM_KIND)
        {
            QStringList sub_list = name_str.split(MACRO_RECON_Splitter_3_dollar_in_brace, Qt::SkipEmptyParts);

            mdfind_conditions += get_mdkind_condition_str_for_widget_type_kind(sub_list);
        }

        if(type == enum_File_System_Widget_Type_FILESYSTEM_DATE)
        {
            QStringList sub_list = name_str.split(MACRO_RECON_Splitter_3_dollar_in_brace, Qt::SkipEmptyParts);

            mdfind_conditions += get_mdkind_condition_str_for_widget_type_date(sub_list, command);
        }

        if(type == enum_File_System_Widget_Type_FILESYSTEM_DURATION)
        {
            mdfind_conditions += get_mdkind_condition_str_for_widget_type_duration(sub_list, command);
        }

        if(type == enum_File_System_Widget_Type_FILESYSTEM_SIZE)
        {
            mdfind_conditions += get_mdkind_condition_str_for_widget_type_size(sub_list, command);
        }

        if(type == enum_File_System_Widget_Type_FILESYSTEM_SIMPLE_LINEEDIT)
        {
            mdfind_conditions += get_mdkind_condition_str_for_widget_type_simple_lineedit(sub_list, command);
        }

        if(type == enum_File_System_Widget_Type_FILESYSTEM_SIMPLE_LABEL_N_LINEEDIT)
        {
            mdfind_conditions += get_mdkind_condition_str_for_widget_type_simple_label_and_lineedit(sub_list, command);
        }

        if(type == enum_File_System_Widget_Type_FILESYSTEM_SINGLE_SYSTEM_COMBOBX)
        {
            mdfind_conditions += get_mdkind_condition_str_for_widget_type_single_system_combobox(sub_list, command);
        }

        if(type == enum_File_System_Widget_Type_FILESYSTEM_SINGLE_VISIBLE_COMBOBX)
        {
            mdfind_conditions += get_mdkind_condition_str_for_widget_type_single_visible_combobox(sub_list, command);
        }
    }

    if(mdfind_conditions.endsWith("&& "))
        mdfind_conditions.chop(3);

    mdfind_conditions = mdfind_conditions.trimmed();

    destination_db.close();
    return mdfind_conditions;
}

QString spotlight_filters::get_mdkind_condition_str_for_widget_type_name(QStringList sub_name_list, QString command)
{
    QString mdkind_conditions;

    QString match1;
    QString match_str;
    if(sub_name_list.size() > 1)
    {
        match1 = sub_name_list.at(1);
    }

    if(sub_name_list.size() > 2)
    {
        match_str = sub_name_list.at(2);
    }

    match_str = match_str.trimmed();

    if(match_str.isEmpty())
        return mdkind_conditions;

    if(match1 == MACRO_Generic_Filter_Condition_Is)
    {
        mdkind_conditions.append(QString( command + " == " + match_str) + " && ");
    }

    ///check it again
    if(match1 == MACRO_Generic_Filter_Condition_Matches)
    {
        mdkind_conditions.append(QString( command + " == " + match_str ) + " && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Contains)
    {
        mdkind_conditions.append(QString( command + " == *" + match_str )+ "* && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Begins_With)
    {
        mdkind_conditions.append(QString( command + " == " + match_str) + "* && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Ends_With)
    {
        mdkind_conditions.append(QString( command + " == *" + match_str) + " && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Is_Not)
    {
        mdkind_conditions.append(QString( command + " != " + match_str) + " && ");
    }

    return mdkind_conditions;
}

QString spotlight_filters::get_mdkind_condition_str_for_widget_type_options(QStringList sub_name_list, QString command)
{
    QString mdkind_conditions;

    QString match1;
    QString match_str;

    if(sub_name_list.size() > 1)
    {
        match1 = sub_name_list.at(1);
    }

    if(sub_name_list.size() > 2)
    {
        match_str = sub_name_list.at(2);
    }

    if(match1 == MACRO_Generic_Filter_Condition_Any)
    {
        mdkind_conditions.append(QString( command + " == -1") + " && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Yes)
    {
        mdkind_conditions.append(QString( command + " == 1") + " && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_No)
    {
        mdkind_conditions.append(QString( command + " == 0") + " && ");
    }

    return mdkind_conditions;
}

QString spotlight_filters::get_mdkind_condition_str_for_widget_type_range(QStringList sub_name_list, QString command)
{
    QString mdkind_conditions;

    QString match1;
    QString match_str;

    if(sub_name_list.size() > 1)
    {
        match1 = sub_name_list.at(1);
    }

    if(sub_name_list.size() > 2)
    {
        match_str = sub_name_list.at(2);
    }

    if(match1 == MACRO_Generic_Filter_Condition_Equal && match_str != "")
    {
        mdkind_conditions.append(QString( command + " == "  + match_str ) + " && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Is_Greater_Than && match_str != "")
    {
        mdkind_conditions.append(QString( command + " > "  + match_str ) + " && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Is_Less_Than && match_str != "")
    {
        mdkind_conditions.append(QString( command + " < "  + match_str ) + " && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Is_Not && match_str != "")
    {
        mdkind_conditions.append(QString( command + " != "  + match_str ) + " && ");
    }

    return mdkind_conditions;
}

QString spotlight_filters::get_mdkind_condition_str_for_widget_type_content(QStringList sub_name_list, QString command)
{
    QString mdkind_conditions;

    QString match1;
    QString match_str;

    if(sub_name_list.size() > 1)
    {
        match1 = sub_name_list.at(1);
    }
    if(sub_name_list.size() > 2)
    {
        match_str = sub_name_list.at(2);
    }

    match_str = match_str.trimmed();

    if(match_str.isEmpty())
        return mdkind_conditions;

    if(match1 == "contains")
    {
        mdkind_conditions.append(QString(command + " == *" + match_str) + "* && ");
    }

    return mdkind_conditions;
}

QString spotlight_filters::get_mdkind_condition_str_for_widget_type_kind(QStringList sub_name_list)
{
    QString mdkind_conditions;

    QString match1;
    QString match_str, match_third;

    if(sub_name_list.size() > 1)
    {
        match1 = sub_name_list.at(1);
    }

    if(sub_name_list.size() > 2)
    {
        match_str = sub_name_list.at(2);
    }

    if(sub_name_list.size() > 3)
    {
        match_third = sub_name_list.at(3);
    }


    if(match_str == MACRO_Generic_Filter_Condition_Any)
    {
        mdkind_conditions.append(QString( "kMDItemContentTypeTree == public.item") + " && ");
    }
    if(match_str == MACRO_Generic_Filter_Condition_Pdf)
    {
        mdkind_conditions.append(QString( "kMDItemKind == *PDF*") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Document || match_str == MACRO_Generic_Filter_Condition_Application || match_str == MACRO_Generic_Filter_Condition_Folder)
    {
        mdkind_conditions.append(QString( "kMDItemKind == " + match_str) + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Archive)
    {
        mdkind_conditions.append(QString( "kMDItemKind == *archive") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Executable)
    {
        mdkind_conditions.append(QString( "kMDItemContentTypeTree == public.executable") + " && ");
        mdkind_conditions.append(QString( "kMDItemContentTypeTree != public.object-code") + " && ");
        mdkind_conditions.append(QString( "kMDItemContentTypeTree != com.apple.mach-o-binary") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Presentation)
    {
        mdkind_conditions.append(QString( "kMDItemKind == *presentation") + " && ");
        // mystr.append(QString( "kMDItemContentType == org.openxmlformats.presentationml.presentation") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Movie)
    {
        mdkind_conditions.append(QString( "kMDItemKind == *movie ") + " && ");
    }

    //************************Images Starts****************************
    if(match_str == MACRO_Generic_Filter_Condition_Image && match_third == MACRO_Generic_Filter_Condition_All)
    {
        mdkind_conditions.append(QString( "kMDItemKind == *image") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Image && match_third == MACRO_Generic_Filter_Condition_Jpeg)
    {
        mdkind_conditions.append(QString( "kMDItemKind == \"JPEG image\"") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Image && match_third == MACRO_Generic_Filter_Condition_Tiff)
    {
        mdkind_conditions.append(QString( "kMDItemKind == \"TIFF image\"") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Image && match_third == MACRO_Generic_Filter_Condition_Png)
    {
        mdkind_conditions.append(QString( "kMDItemKind == \"Portable Network Graphics image\"") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Image && match_third == MACRO_Generic_Filter_Condition_Gif)
    {
        mdkind_conditions.append(QString( "kMDItemKind == \"Graphics Interchange Format (GIF)\"") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Image && match_third == MACRO_Generic_Filter_Condition_Bmp)
    {
        mdkind_conditions.append(QString( "kMDItemKind == \"Windows bitmap image\"") + " && ");
    }
    //************************Images End****************************

    //************************Music Start****************************
    if(match_str == MACRO_Generic_Filter_Condition_Music && match_third == MACRO_Generic_Filter_Condition_All)
    {
        mdkind_conditions.append(QString( "kMDItemKind == *audio") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Music && match_third == MACRO_Generic_Filter_Condition_Mp3)
    {
        mdkind_conditions.append(QString("kMDItemKind == \"MP3 audio\"") + " && ");
    }

    ///AAC Audio-------
    if(match_str == MACRO_Generic_Filter_Condition_Music && match_third == MACRO_Generic_Filter_Condition_Aac)
    {
        mdkind_conditions.append(QString("kMDItemKind == \"AAC audio\"") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Music && match_third == MACRO_Generic_Filter_Condition_Purchased)
    {

        mdkind_conditions.append(QString("kMDItemKind == purchased*") + " && ");

    }

    //************************Music Ends****************************



    //************************TEXT Starts****************************
    if(match_str == MACRO_Generic_Filter_Condition_Text && match_third == MACRO_Generic_Filter_Condition_All)
    {
        mdkind_conditions.append(QString( "kMDItemContentTypeTree == public.text") + " && ");
        mdkind_conditions.append(QString( "kMDItemContentTypeTree != public.object-code") + " && ");

    }

    if(match_str == MACRO_Generic_Filter_Condition_Text && match_third == MACRO_Generic_Filter_Condition_Plain_Text)
    {
        mdkind_conditions.append(QString( "kMDItemContentTypeTree == public.plain-text") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Text && match_third == MACRO_Generic_Filter_Condition_Html)
    {
        mdkind_conditions.append(QString("kMDItemKind == \"HTML document\"") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Text && match_third == MACRO_Generic_Filter_Condition_Rich_Text)
    {
        mdkind_conditions.append(QString("kMDItemKind == \"Rich Text Document\"") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Text && match_third == MACRO_Generic_Filter_Condition_Xml)
    {
        mdkind_conditions.append(QString("kMDItemContentTypeTree == public.xml") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Text && match_third == MACRO_Generic_Filter_Condition_Source_Code)
    {
        mdkind_conditions.append(QString("kMDItemContentTypeTree == public.source-code") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Other)
    {
        if(match_third.contains(" "))
            mdkind_conditions.append(QString("kMDItemKind == \""  + match_third) + "\" && ");
        else
            mdkind_conditions.append(QString("kMDItemKind == *" + match_third) + "* && ");

    }

    //************************TEXT Ends****************************

    return mdkind_conditions;
}

QString spotlight_filters::get_mdkind_condition_str_for_widget_type_date(QStringList sub_name_list, QString command)
{
    QString mdkind_conditions;

    QString match1;
    QString match_str, match_third, match_withindays_day;

    if(sub_name_list.size() > 1)
    {
        match1 = sub_name_list.at(1);
    }
    if(sub_name_list.size() > 2)
    {
        match_str = sub_name_list.at(2);
    }

    if(sub_name_list.size() > 3)
    {
        match_third = sub_name_list.at(3);
    }

    if(sub_name_list.size() > 4)
    {
        match_withindays_day = sub_name_list.at(4);
    }


    match_str = match_str.trimmed();
    match_third = match_third.trimmed();
    match_withindays_day = match_withindays_day.trimmed();
    //            if(match_str == "" || match_third == "" || match_withindays_day == "")
    //                continue;

    if(match_str == MACRO_Generic_Filter_Condition_Exactly)
    {
        if(match_third == "")
            return mdkind_conditions;

        mdkind_conditions.append(QString( command + " == $time.iso(") + match_third + ") && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Within_Last && match_withindays_day == MACRO_Generic_Filter_Condition_Days)
    {
        if(match_third == "")
            return mdkind_conditions;

        mdkind_conditions.append(QString( command + " >= $time.today(-") + match_third + ") && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Within_Last && match_withindays_day == MACRO_Generic_Filter_Condition_Weeks)
    {
        if(match_third == "")
            return mdkind_conditions;

        mdkind_conditions.append(QString( command + " >= $time.today(-7") + match_third + ") && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Within_Last && match_withindays_day == MACRO_Generic_Filter_Condition_Months)
    {
        if(match_third == "")
            return mdkind_conditions;

        mdkind_conditions.append(QString( command + " >= $time.this_month(-") + match_third + ") && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Within_Last && match_withindays_day == MACRO_Generic_Filter_Condition_Years)
    {
        if(match_third == "")
            return mdkind_conditions;

        mdkind_conditions.append(QString( command + " >= $time.this_year(-") + match_third + ") && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Before)
    {
        if(match_third == "")
            return mdkind_conditions;

        mdkind_conditions.append(QString( command + " <= $time.iso(") + match_third + ") && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_After)
    {
        if(match_third == "")
            return mdkind_conditions;

        mdkind_conditions.append(QString( command + " >= $time.iso(") + match_third + ") && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Today)
    {
        mdkind_conditions.append(QString( command + " <= $time.today") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_Yesterday)
    {
        mdkind_conditions.append(QString( command + " <= $time.yesterday") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_This_Week)
    {
        mdkind_conditions.append(QString( command + " <= $time.this_week") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_This_Month)
    {
        mdkind_conditions.append(QString( command + " <= $time.this_month") + " && ");
    }

    if(match_str == MACRO_Generic_Filter_Condition_This_Year)
    {
        mdkind_conditions.append(QString( command + " <= $time.this_year") + " && ");
    }

    return mdkind_conditions;
}

QString spotlight_filters::get_mdkind_condition_str_for_widget_type_duration(QStringList sub_list, QString command)
{
    QString mdkind_conditions;

    QString match1;
    QString hr_str, min_str, sec_str;

    if(sub_list.size() > 1)
    {
        match1 = sub_list.at(1);
    }
    if(sub_list.size() > 2)
    {
        hr_str = sub_list.at(2);
    }

    if(sub_list.size() > 4)
    {
        min_str = sub_list.at(4);
    }

    if(sub_list.size() > 6)
    {
        sec_str = sub_list.at(6);
    }

    quint64  Attach_with_units =  (hr_str.toInt() * 3600) + (min_str.toInt() * 60) + sec_str.toInt();


    if(match1 == MACRO_Generic_Filter_Condition_Equal && (hr_str != "" || min_str != "" || sec_str != ""))
    {
        mdkind_conditions.append(QString( command + " == "  + QString::number( Attach_with_units)) + " && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Is_Greater_Than  && (hr_str != "" || min_str != "" || sec_str != ""))
    {
        mdkind_conditions.append(QString( command + " > " + QString::number( Attach_with_units)) + " && ");
    }
    if(match1 == MACRO_Generic_Filter_Condition_Is_Less_Than  && (hr_str != "" || min_str != "" || sec_str != ""))
    {
        mdkind_conditions.append(QString( command + " < " + QString::number( Attach_with_units)) + " && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Is_Not  && (hr_str != "" || min_str != "" || sec_str != ""))
    {
        mdkind_conditions.append(QString( command + " != " + QString::number( Attach_with_units)) + " && ");
    }

    return mdkind_conditions;
}

QString spotlight_filters::get_mdkind_condition_str_for_widget_type_size(QStringList sub_list, QString command)
{
    QString mdkind_conditions;

    QString match1;
    QString match_str;
    QString match_third;

    if(sub_list.size() > 1)
    {
        match1 = sub_list.at(1);
    }
    if(sub_list.size() > 2)
    {
        match_str = sub_list.at(2);
    }

    if(sub_list.size() > 3)
    {
        match_third = sub_list.at(3);
    }


    quint64 attach_with_units = 0;

    if(match_third == MACRO_Generic_Filter_Condition_KB)
    {
        attach_with_units =  (match_str.toInt() * 1024);
    }

    if(match_third == MACRO_Generic_Filter_Condition_MB)
    {
        attach_with_units =  (match_str.toInt() * 1024 *1024);
    }

    if(match_third == MACRO_Generic_Filter_Condition_GB)
    {
        attach_with_units =  (match_str.toInt() * 1024 * 1024 * 1024);
    }


    if(match1 == MACRO_Generic_Filter_Condition_Equal && match_str != "")
    {
        mdkind_conditions.append(QString( command + " == " + QString::number( attach_with_units)) + " && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Is_Greater_Than && match_str != "")
    {

        mdkind_conditions.append(QString( command + " > "  + QString::number( attach_with_units)) + " && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Is_Less_Than && match_str != "")
    {
        mdkind_conditions.append(QString( command + " < " + QString::number( attach_with_units)) + " && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Is_Not && match_str != "")
    {
        mdkind_conditions.append(QString( command + " != " + QString::number( attach_with_units)) + " && ");
    }

    return mdkind_conditions;
}

QString spotlight_filters::get_mdkind_condition_str_for_widget_type_simple_lineedit(QStringList sub_list, QString command)
{
    QString mdkind_conditions;

    QString match1;
    if(sub_list.size() > 1)
    {
        match1 = sub_list.at(1);
    }

    if((command == "TYPE YOUR OWN QUERY") && (match1 != ""))
    {
        mdkind_conditions.append(QString( match1 + " && "));
    }

    return mdkind_conditions;
}

QString spotlight_filters::get_mdkind_condition_str_for_widget_type_simple_label_and_lineedit(QStringList sub_list, QString command)
{
    QString mdkind_conditions;

    QString match1;
    QString match_str;

    if(sub_list.size() > 1)
    {
        match1 = sub_list.at(1);
    }

    if(sub_list.size() > 2)
    {
        match_str = sub_list.at(2);
        match_str = match_str.trimmed();
    }

    if(match1 == "is" && match_str != "")
    {
        mdkind_conditions.append(QString( command + " == *." + match_str)+ " && ");
    }

    return mdkind_conditions;
}

QString spotlight_filters::get_mdkind_condition_str_for_widget_type_single_system_combobox(QStringList sub_list, QString command)
{
    QString mdkind_conditions;

    QString match1;
    QString match_str;

    if(sub_list.size() > 1)
    {
        match1 = sub_list.at(1);
    }
    if(sub_list.size() > 2)
    {
        match_str = sub_list.at(2);
    }
    if(match1 == MACRO_Generic_Filter_Condition_Are_Included)
    {
        mdkind_conditions.append(QString( command + " == 1") + " && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Are_Not_Included)
    {
        mdkind_conditions.append(QString( command + " == 0") + " && ");
    }

    return mdkind_conditions;
}

QString spotlight_filters::get_mdkind_condition_str_for_widget_type_single_visible_combobox(QStringList sub_list, QString command)
{
    QString mdkind_conditions;

    QString match1;

    if(sub_list.size() > 1)
    {
        match1 = sub_list.at(1);
    }

    if(match1 == MACRO_Generic_Filter_Condition_Visible_Items)
    {
        mdkind_conditions.append(QString( command + " == 1") + " && ");
    }

    if(match1 == MACRO_Generic_Filter_Condition_Invisible_Items)
    {
        mdkind_conditions.append(QString( command + " == 0") + " && ");
    }

    ///pending
    if(match1 == MACRO_Generic_Filter_Condition_Visible_Or_Invisible)
    {
        mdkind_conditions.append(QString( command + " == 0 || " +  command + " == 1 " ) + " && ");
    }

    return mdkind_conditions;
}
