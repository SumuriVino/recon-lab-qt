#include "artifacts_keyword_search_interface.h"
#include "ui_artifacts_keyword_search_interface.h"

artifacts_keyword_search_interface::artifacts_keyword_search_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::artifacts_keyword_search_interface)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->lineEdit_keywords->setAttribute(Qt::WA_MacShowFocusRect,0);
    ui->lineEdit_search_name->setAttribute(Qt::WA_MacShowFocusRect,0);

    layout = new QVBoxLayout;
    plugin_selector_obj = new plugin_selector(this);
    plugin_selector_obj->pub_set_widget_content_margin(0,0,0,0);
    layout->addWidget(plugin_selector_obj);
    layout->setContentsMargins(0,0,0,0);

    ui->groupBox_plugion_selection->setLayout(layout);

    keyword_list_selector_obj = new keywords_list_selector(this);

    //    ui->label_keyword_list_active->setText("<a href = "+ QString("Keywords List") + " > " + QString("Keywords List") + " </a>");
    //    connect(ui->label_keyword_list_active, SIGNAL(linkActivated(QString)), this, SLOT(slot_label_keyword_list_clicked(QString)),Qt::QueuedConnection);

    message_dialog_object = new message_dialog(this);
    ui->checkBox_files->setChecked(false);
    ui->checkBox_files->setHidden(true);
    ui->checkBox_plugins->setChecked(true);
    setWindowTitle(QString(MACRO_CASE_TREE_Artifacts_Keyword_Search));
    ui->checkBox_plugins->setHidden(true);

}

artifacts_keyword_search_interface::~artifacts_keyword_search_interface()
{
    if(plugin_selector_obj != NULL)
        delete plugin_selector_obj;


    delete ui;

}


void artifacts_keyword_search_interface::slot_label_keyword_list_clicked(QString) // function not in use
{
    keyword_list_selector_obj->show();
}


void artifacts_keyword_search_interface::set_keywordsearch_default_name() // set keyword search default name in label
{
    QString name = "Search " + QDateTime::currentDateTime().toString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString).toString());
    ui->lineEdit_search_name->setPlaceholderText(name);
}

void artifacts_keyword_search_interface::on_pushButton_search_clicked()
{ // peroforms when we click on search button after entring keyword, and it will display the result accordingly
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QStringList keyword_list;

    if(ui->lineEdit_keywords->text().isEmpty())
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Please provide keywords for search.");
        message_dialog_object->show();

        return;
    }

    QStringList list_selected_plugins_with_tab = plugin_selector_obj->pub_get_seleted_tabs_and_plugins_for_Keyword_Search();

    QStringList list_selected_plugins;


    for(int i = 0 ; i < list_selected_plugins_with_tab.size(); i++)
    {
        QString plugin_tab_entry = list_selected_plugins_with_tab.at(i);

        QStringList temp_list = plugin_tab_entry.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);
        list_selected_plugins << temp_list.at(0);
    }

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Keyword_Search_Selected_Plugins_And_Tabs_QStringList,list_selected_plugins_with_tab);

    if(list_selected_plugins.isEmpty() && !ui->checkBox_files->checkState() && ui->checkBox_plugins->checkState())
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Please select plugins for search.");
        message_dialog_object->show();

        return;
    }


    if(ui->lineEdit_search_name->text().trimmed().isEmpty())
    {
        search_name = ui->lineEdit_search_name->placeholderText().trimmed();
    }
    else
    {
        search_name = ui->lineEdit_search_name->text().trimmed();
    }

    QStringList list_saved_search = recon_helper_standard_obj->get_keyword_search_namelist_from_result_db(Q_FUNC_INFO);
    if(list_saved_search.contains(search_name))
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Entered search name already exists.");
        message_dialog_object->show();
        return;
    }

    if(!ui->lineEdit_keywords->text().isEmpty())
    {
        keyword_list = ui->lineEdit_keywords->text().split(",");
    }

    ///- Assign Plugins and Keywords to the Search engine.

    bool bool_plugins_selected = ui->checkBox_plugins->checkState();
    bool bool_files_selected = ui->checkBox_files->checkState();

    this->hide();
    emit signal_keyword_search_plugin_selection_workdone(search_name,keyword_list, list_selected_plugins, bool_plugins_selected, bool_files_selected);

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}


void artifacts_keyword_search_interface::set_initialisation() // set some of essenstials things
{
    ui->lineEdit_keywords->clear();
    ui->lineEdit_search_name->clear();
    plugin_selector_obj->set_narad_muni_for_report(global_narad_muni_class_obj);
    plugin_selector_obj->pub_set_launcher_type(MACRO_Launcher_Plugin_Selection_K_Search);
    plugin_selector_obj->pub_initialise_plugin_info_for_keyword_search();
    set_keywordsearch_default_name();
    plugin_selector_obj->show();
}
