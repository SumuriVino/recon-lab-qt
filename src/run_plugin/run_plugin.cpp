#include "run_plugin.h"
#include "ui_run_plugin.h"

run_plugin::run_plugin(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::run_plugin)
{
    ui->setupUi(this);

    plugin_selector_obj = new plugin_selector(this);
    plugin_selector_obj->pub_set_widget_content_margin(0,0,0,0);

    setWindowFlags(Qt::Dialog);
    setWindowTitle(recon_static_functions::get_app_name() + " - Plugin Selector");

    QVBoxLayout *vbox_mainlayout = new QVBoxLayout(this);
    vbox_mainlayout->addWidget(plugin_selector_obj);

    QHBoxLayout *hbox_layout = new QHBoxLayout(vbox_mainlayout->widget());
    QSpacerItem *spacer = new QSpacerItem(130,10,QSizePolicy::Expanding,QSizePolicy::Minimum);

    pushbutton_start = new QPushButton("Start",this);
    pushbutton_start->setMinimumWidth(100);
    connect(pushbutton_start,SIGNAL(clicked()),this,SLOT(on_pushButton_start_clicked()));
    hbox_layout->addSpacerItem(spacer);
    hbox_layout->addWidget(pushbutton_start);
    hbox_layout->setContentsMargins(0,0,12,0);

    vbox_mainlayout->addLayout(hbox_layout);
    vbox_mainlayout->setContentsMargins(4,4,4,8);
    vbox_mainlayout->setSpacing(0);
    setLayout(vbox_mainlayout);
}

run_plugin::~run_plugin()
{
    delete ui;
}

void run_plugin::start_work()
{
    plugin_selector_obj->set_narad_muni_for_report(global_narad_muni_class_obj);
    plugin_selector_obj->pub_set_launcher_type(MACRO_Launcher_Plugin_Selection_Run_Plugin);
    plugin_selector_obj->pub_initialise_plugin_info_for_right_click_run_plugin();
}

void run_plugin::on_pushButton_start_clicked()
{
    if(plugin_selector_obj->pub_get_selected_plugins_from_launcher_for_run_plugin())
        emit signal_plugin_list_from_run_plugin();
}

