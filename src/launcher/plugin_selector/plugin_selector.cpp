#include "plugin_selector.h"
#include "ui_plugin_selector.h"

plugin_selector::plugin_selector(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::plugin_selector)
{
    ui->setupUi(this);

    recon_static_functions::app_debug(" plugin_selector - 1 start ", Q_FUNC_INFO);

    recon_helper_standard_obj = new recon_helper_standard(this);

    recon_static_functions::app_debug(" plugin_selector - 2", Q_FUNC_INFO);

    M_VAR_PLUGIN_TEMPLATE_Plugin_selection_QString = global_lang_tr_obj.get_translated_string("<Plugin Selection>");
    M_VAR_PLUGIN_TEMPLATE_Select_All_QString = global_lang_tr_obj.get_translated_string("Select All");
    M_VAR_PLUGIN_TEMPLATE_Select_All_With_Export_QString = global_lang_tr_obj.get_translated_string("Select All With Export");
    M_VAR_PLUGIN_TEMPLATE_Select_All_Without_Export_QString = global_lang_tr_obj.get_translated_string("Select All without Export");
    M_VAR_PLUGIN_TEMPLATE_Remove_All_QString = global_lang_tr_obj.get_translated_string("Remove All");

    M_VAR_PLUGIN_Parent_All_Plugins_QString = "All Plugins";
    M_VAR_FUNCTIONALITY_Not_Available_QString = "NA";

    recon_static_functions::app_debug(" plugin_selector - 3", Q_FUNC_INFO);

    ui->lineEdit_plugin_search->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_save_template->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_save_template->setEnabled(false);
    ui->pushButton_add_template->setEnabled(false);
    ui->treeWidget_plugins->setAttribute(Qt::WA_MacShowFocusRect,false);

    ui->comboBox_template_plugins_selection->setAttribute(Qt::WA_MacShowFocusRect,false);

    QFont font;
    font.setItalic(true);

    recon_static_functions::app_debug(" plugin_selector - 4", Q_FUNC_INFO);

    //- Disabled Items in Device Combobox.
    //    ui->comboBox_device_selection->setItemData(2,0,Qt::UserRole - 1);

    total_plugin_selected = 0;
    bool_plugin_is_checked = false;

    set_hide_widgets(true);

    setWindowTitle(recon_static_functions::get_app_name() + " - Plugin Selector");
    recon_static_functions::app_debug(" plugin_selector - 5", Q_FUNC_INFO);

    bool_combobox_updating = false;
    ui->pushButton_remove_template->setEnabled(false);

    message_dialog_object = new message_dialog(this);

    bool_plugins_tree_updated = false;


    supported_parent_macOS = (MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display + QString(" Plugins"));
    supported_parent_iOS = (MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display + QString(" Plugins"));
    supported_parent_winOS = (MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display + QString(" Plugins"));
    supported_parent_gtakeoutOS = ("Google Takeout" + QString(" Plugins"));
    supported_parent_androidOS = (MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Display + QString(" Plugins"));

    ui->lineEdit_plugin_search->setPlaceholderText(global_lang_tr_obj.get_translated_string(ui->lineEdit_plugin_search->placeholderText()));
    ui->lineEdit_save_template->setPlaceholderText(global_lang_tr_obj.get_translated_string(ui->lineEdit_save_template->placeholderText()));
    ui->checkBox_save_template->setText(global_lang_tr_obj.get_translated_string(ui->checkBox_save_template->text()));
    ui->pushButton_add_template->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_add_template->text()));
    ui->pushButton_remove_template->setText(global_lang_tr_obj.get_translated_string(ui->pushButton_remove_template->text()));


    ui->tableWidget_parent_list->hideColumn(enum_parent_tablewidget_column_original_name);
    ui->treeWidget_plugins->hideColumn(enum_plugin_tree_column_plugin_original_name);

    recon_static_functions::app_debug(" plugin_selector - 6 end", Q_FUNC_INFO);

}

plugin_selector::~plugin_selector()
{
    delete ui;
}



void plugin_selector::set_narad_muni_for_report(narad_muni *obj)
{
    narad_muni_for_report_obj = obj;
}

void plugin_selector::pub_set_launcher_type(QString launcher)
{
    launcher_type = launcher;
}

void plugin_selector::pub_set_widget_content_margin(int left, int top, int right, int bottom)
{
    this->setContentsMargins(left,top,right,bottom);
}

void plugin_selector::fill_template_combobox()
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    bool_combobox_updating = true;

    ui->comboBox_template_plugins_selection->clear();

    QStringList list_combo_template_type;


    if(launcher_type == MACRO_Launcher_Plugin_Selection_G_Report)
        list_combo_template_type << M_VAR_PLUGIN_TEMPLATE_Plugin_selection_QString << M_VAR_PLUGIN_TEMPLATE_Select_All_With_Export_QString << M_VAR_PLUGIN_TEMPLATE_Select_All_Without_Export_QString << M_VAR_PLUGIN_TEMPLATE_Remove_All_QString;
    else
        list_combo_template_type << M_VAR_PLUGIN_TEMPLATE_Plugin_selection_QString << M_VAR_PLUGIN_TEMPLATE_Select_All_QString << M_VAR_PLUGIN_TEMPLATE_Remove_All_QString;



    QString command = "select template_name from plugin_selection_template";

    QStringList list_template = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,destination_db_file,Q_FUNC_INFO);
    /// Remove duplicate entries if order of elements is not an issue
    list_template.removeDuplicates();

    for(int i = 0; i < list_template.size(); i++)
    {
        list_combo_template_type << list_template.at(i);
    }

    ui->comboBox_template_plugins_selection->addItems(list_combo_template_type);

    bool_combobox_updating = false;

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);

}

void plugin_selector::set_hide_widgets(bool value)
{
    //    ui->comboBox_device_selection->setHidden(value);
    ui->pushButton_remove_template->setHidden(value);
    ui->checkBox_save_template->setHidden(value);
    ui->lineEdit_save_template->setHidden(value);
    ui->pushButton_add_template->setHidden(value);
}

void plugin_selector::on_lineEdit_plugin_search_textChanged(const QString &arg1)
{
    lineEdit_plugin_search_textChanged_common(arg1);

}


void plugin_selector::on_comboBox_template_plugins_selection_currentTextChanged(const QString &arg1)
{
    if(bool_combobox_updating)
        return;

    ui->lineEdit_save_template->clear();
    ui->pushButton_remove_template->setEnabled(false);

    if(arg1 == M_VAR_PLUGIN_TEMPLATE_Plugin_selection_QString)
        return;

    if(arg1 == M_VAR_PLUGIN_TEMPLATE_Select_All_QString || arg1 == M_VAR_PLUGIN_TEMPLATE_Select_All_With_Export_QString)
    {
        select_all_with_export_clicked_common();
    }
    else if(arg1 == M_VAR_PLUGIN_TEMPLATE_Select_All_Without_Export_QString)
    {
        select_all_without_export();
    }

    else if(arg1 == M_VAR_PLUGIN_TEMPLATE_Remove_All_QString)
    {
        remove_all_clicked_common();
    }
    else
    {
        ui->pushButton_remove_template->setEnabled(true);
        ui->lineEdit_save_template->setText(arg1);

        ui->tableWidget_parent_list->setCurrentCell(0,enum_parent_tablewidget_column_translated_name);

        select_template_plugins_for_common(arg1);

    }

}

void plugin_selector::on_checkBox_save_template_clicked(bool checked)
{
    if(launcher_type != MACRO_Launcher_Plugin_Selection_RECON_Configuration)
        return;

    if(checked)
    {
        ui->pushButton_add_template->setEnabled(true);
        ui->lineEdit_save_template->setEnabled(true);
    }
    else
    {
        ui->pushButton_add_template->setEnabled(false);
        ui->lineEdit_save_template->setEnabled(false);
    }
}

void plugin_selector::on_lineEdit_save_template_returnPressed()
{
    on_pushButton_add_template_clicked();
}

void plugin_selector::on_pushButton_remove_template_clicked()
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    if(launcher_type != MACRO_Launcher_Plugin_Selection_RECON_Configuration)
        return;

    QString combo_current_text = ui->comboBox_template_plugins_selection->currentText();
    if(combo_current_text == M_VAR_PLUGIN_TEMPLATE_Plugin_selection_QString || combo_current_text == M_VAR_PLUGIN_TEMPLATE_Select_All_QString || combo_current_text == M_VAR_PLUGIN_TEMPLATE_Remove_All_QString)
        return;

    QString combo_text;
    for(int i = 2; i < ui->comboBox_template_plugins_selection->count(); i++)
    {
        combo_text = ui->comboBox_template_plugins_selection->itemText(i);
        if(ui->comboBox_template_plugins_selection->currentText() == combo_text)
        {
            ui->comboBox_template_plugins_selection->removeItem(i);
            break;
        }
    }

    QString command = "DELETE FROM plugin_selection_template WHERE template_name = '" + combo_text + "'";

    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command,destination_db_file,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" DELETE FROM plugin_selection_template  --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
    }

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);

}

void plugin_selector::on_lineEdit_save_template_textChanged(const QString &arg1)
{
    if(arg1 != "")
    {
        ui->lineEdit_save_template->setStyleSheet(styleSheet());
    }
}



void plugin_selector::on_treeWidget_plugins_itemClicked(QTreeWidgetItem *item, int column)
{
    if(launcher_type == MACRO_Launcher_Plugin_Selection_New_Case || launcher_type == MACRO_Launcher_Plugin_Selection_G_Report
            || launcher_type == MACRO_Launcher_Plugin_Selection_G_Timeline  || launcher_type == MACRO_Launcher_Plugin_Selection_K_Search){
        on_treeWidget_plugins_itemClicked_new_case_common(item,column);
    }
    else if(launcher_type == MACRO_Launcher_Plugin_Selection_Run_Plugin || launcher_type == MACRO_Launcher_Plugin_Selection_RECON_Configuration){
        on_treeWidget_plugins_itemClicked_run_plugin_common(item,column);
    }

}

void plugin_selector::on_tableWidget_parent_list_cellClicked(int row, int column)
{

    ui->comboBox_template_plugins_selection->setCurrentText(M_VAR_PLUGIN_TEMPLATE_Plugin_selection_QString);

    ui->lineEdit_plugin_search->clear();
    ui->treeWidget_plugins->collapseAll();

    for(int i = 0 ; i < ui->treeWidget_plugins->topLevelItemCount(); i++)
    {
        ui->treeWidget_plugins->topLevelItem(i)->setSelected(false);

        bool bool_hide_row = false;
        //-Compare value with the list filled at time of initialisation.

        if(list_plugins_info_common.at(i).plugin_name == ui->treeWidget_plugins->topLevelItem(i)->text(enum_plugin_tree_column_plugin_original_name))
        {
            int mrow = ui->tableWidget_parent_list->currentRow();
            if(mrow < 0)
                continue;

            QString crnt_orgnl_txt =  ui->tableWidget_parent_list->item(mrow, enum_parent_tablewidget_column_original_name)->text();

            if(crnt_orgnl_txt == M_VAR_PLUGIN_Parent_All_Plugins_QString)
            {
                bool_hide_row = false;
            }
            else if(crnt_orgnl_txt == supported_parent_macOS
                    || crnt_orgnl_txt == supported_parent_iOS
                    || crnt_orgnl_txt == supported_parent_winOS
                    || crnt_orgnl_txt == supported_parent_androidOS
                    || crnt_orgnl_txt == supported_parent_gtakeoutOS)
            {
                QString os_schme_dsply2 = crnt_orgnl_txt;
                if(os_schme_dsply2.endsWith(" Plugins"))
                    os_schme_dsply2.chop(QString(" Plugins").size());

                os_schme_dsply2 = os_schme_dsply2.trimmed();
                if(crnt_orgnl_txt == supported_parent_gtakeoutOS)
                    os_schme_dsply2 = MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_gTakeoutOS_Display;


                if(!list_plugins_info_common.at(i).supported_os_scheme_display_QStringList.contains(os_schme_dsply2))
                {
                    bool_hide_row = true;
                }
                else
                {
                    bool_hide_row = false;
                }
            }
            else if(list_plugins_info_common.at(i).parent_name != crnt_orgnl_txt)
            {
                bool_hide_row = true;
            }
            else
            {
                bool_hide_row = false;
            }

            if(bool_hide_row)
                ui->treeWidget_plugins->topLevelItem(i)->setHidden(true);
            else
                ui->treeWidget_plugins->topLevelItem(i)->setHidden(false);
        }
    }

    for(int j = 0; j< ui->treeWidget_plugins->topLevelItemCount(); j++)
    {
        if(ui->treeWidget_plugins->topLevelItem(j)->isHidden())
            continue;

        ui->treeWidget_plugins->topLevelItem(j)->setSelected(true);
        break;
    }

}

void plugin_selector::on_treeWidget_plugins_itemChanged(QTreeWidgetItem *item, int column)
{
    bool_plugins_tree_updated = true;
}

void plugin_selector::on_pushButton_add_template_clicked()
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    if(launcher_type != MACRO_Launcher_Plugin_Selection_RECON_Configuration)
        return;

    //-Check Entry exist or not.
    QString lineedit_text = ui->lineEdit_save_template->text().trimmed();

    if(lineedit_text.trimmed().isEmpty())
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Template name should not be blank");
        message_dialog_object->show();
        return;
    }


    bool bool_template_exist = false;
    for(int com = 0; com < ui->comboBox_template_plugins_selection->count(); com++)
    {
        if(ui->comboBox_template_plugins_selection->itemText(com) == lineedit_text)
        {
            bool_template_exist = true;
            break;
        }
    }

    if(bool_template_exist)
    {
        ui->lineEdit_save_template->setStyleSheet("QLineEdit{color : red;}");
        return;
    }


    QString commnad = "insert into 'plugin_selection_template' (template_name,plugin_name,selected_tabs_list) values(?,?,?)";

    QStringList list_values;


    ///=======Prepare Selected Plugins List - Start========///

    QStringList list_tmp;

    for(int i = 0; i < ui->treeWidget_plugins->topLevelItemCount(); i++)
    {
        if(ui->treeWidget_plugins->topLevelItem(i)->checkState(enum_plugin_tree_column_plugin_translated_name) == Qt::Checked)
        {
            list_tmp <<  ui->treeWidget_plugins->topLevelItem(i)->text(enum_plugin_tree_column_plugin_translated_name);

            QString selected_plugin_name = ui->treeWidget_plugins->topLevelItem(i)->text(enum_plugin_tree_column_plugin_original_name);

            QTreeWidgetItem *plugin_tree_item = ui->treeWidget_plugins->topLevelItem(i);

            QStringList seleted_tabs_list;
            for(int count = 0; count < plugin_tree_item->childCount(); count++)
            {
                if(plugin_tree_item->child(count)->checkState(enum_plugin_tree_column_plugin_translated_name) == Qt::Checked)
                {
                    seleted_tabs_list << plugin_tree_item->child(count)->text(enum_plugin_tree_column_plugin_original_name);
                }
            }

            list_values.clear();
            list_values << lineedit_text << selected_plugin_name << seleted_tabs_list.join(MACRO_RECON_Splitter_1_multiple);

            if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(commnad,list_values,destination_db_file,Q_FUNC_INFO))
            {
                recon_static_functions::app_debug(" insert into plugin_selection_template  --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
            }
        }
    }

    if(list_tmp.isEmpty())
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Please select at least one plugin");
        message_dialog_object->show();
        return;
    }

    ///=======Prepare Selected Plugins List - End========///




    ui->lineEdit_save_template->setEnabled(false);
    ui->pushButton_add_template->setEnabled(false);
    ui->checkBox_save_template->setChecked(false);

    fill_template_combobox();

    ui->comboBox_template_plugins_selection->setCurrentText(lineedit_text);
    ui->lineEdit_save_template->clear();

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);

}
