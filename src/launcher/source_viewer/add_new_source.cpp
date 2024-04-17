#include "add_new_source.h"
#include "ui_add_new_source.h"

add_new_source::add_new_source(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_new_source)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    setWindowTitle(recon_static_functions::get_app_name() + " - Add New Source");

    source_selector_obj = new source_selector(this);
    ui->stackedWidget_multi_source_selection->insertWidget(enum_comman_multi_source_page_multi_source_selection,source_selector_obj);

    connect(source_selector_obj,SIGNAL(signal_source_selected(bool)),this, SLOT(slot_source_selected(bool)));
    //connect(source_selector_obj,SIGNAL(signal_added_sources_details_list(QList<struct_GLOBAL_derived_source_info>)),this, SLOT(slot_added_source_details_list(QList<struct_GLOBAL_derived_source_info>)));
    //connect(source_selector_obj,SIGNAL(signal_added_roots_details_list(QList<struct_GLOBAL_witness_info_root>)),this, SLOT(slot_added_root_details_list(QList<struct_GLOBAL_witness_info_root>)));

    connect(source_selector_obj,SIGNAL(signal_hide_source_selector_parent_window()),this,SLOT(slot_hide_source_selector_parent_window()));
    connect(source_selector_obj,SIGNAL(signal_qml_button_initial_back_clicked()),this,SLOT(slot_qml_button_initial_back_clicked()));

    ui->stackedWidget_multi_source_selection->setCurrentIndex(enum_comman_multi_source_page_multi_source_selection);

    bool_source_selected = false;

    ui->pushButton_next->setText(("Next"));
    ui->pushButton_previous->setText(("Back"));

    ui->pushButton_next->setEnabled(false);
    ui->pushButton_next->setText(("Add Source"));
    ui->pushButton_previous->hide();

    list_root_info.clear();
}

add_new_source::~add_new_source()
{
    delete ui;
}


void add_new_source::on_pushButton_next_clicked()
{
    int index = ui->stackedWidget_multi_source_selection->currentIndex();

    bool bool_finish_launcher = false;
    //-1.
    if(index == enum_comman_multi_source_page_multi_source_selection)
    {
        ui->pushButton_previous->show();


        bool_finish_launcher = true;
    }

    if(bool_finish_launcher)
    {
        get_data_from_the_multisource_launcher_pages();

        this->hide();

        emit signal_update_recon_case(list_root_info);

    }
}

void add_new_source::get_data_from_the_multisource_launcher_pages()
{
    source_selector_obj->pub_get_essential_value_from_all_roots();
    list_root_info = source_selector_obj->pub_get_root_info_list_from_gui();
}


void add_new_source::slot_source_selected(bool is_source_selected)
{
    this->show();

    if(is_source_selected)
    {
        bool_source_selected = true;
        ui->pushButton_next->setEnabled(true);
    }
    else
    {
        bool_source_selected = false;
        ui->pushButton_next->setEnabled(false);

    }

}


void add_new_source::slot_added_root_details_list(QList<struct_GLOBAL_witness_info_root> target_list)
{
    list_root_info = target_list;
}

void add_new_source::slot_hide_source_selector_parent_window()
{
    this->hide();
}

void add_new_source::slot_qml_button_initial_back_clicked()
{
    this->show();
}
