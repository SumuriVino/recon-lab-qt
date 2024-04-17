#include "ichat_viewer.h"
#include "ui_ichat_viewer.h"


ichat_parser::ichat_parser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ichat_parser)
{
    ui->setupUi(this);

    mainMenu = new QMenu(this);

    export_rsync_obj = new export_rsync(this);
    recon_helper_standard_obj = new recon_helper_standard(this);


    export_dialog_box_obj = new export_dialog_box(this);
    connect(export_dialog_box_obj, SIGNAL(signal_save_results_by_export_window(QString , QString)), this, SLOT(slot_export_ichat_ok_clicked(QString , QString)));

}

ichat_parser::~ichat_parser()
{

}

//void ichat_parser::mousePressEvent(QMouseEvent *event)
//{
//    if(event->button() == Qt::RightButton)
//    {
//        //mainMenu->exec(QCursor::pos());
//    }
//}


void ichat_parser::pub_parse_ichat()
{
    extract_xcode_archieved_chat(source_file_path_ichat);
}


void ichat_parser::pub_set_global_recon_file_info_structure_obj(struct_global_recon_file_info obj)
{
    struct_global_recon_file_info_obj = obj;
}

void ichat_parser::pub_clear_data()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void ichat_parser::pub_set_essentials()
{

    source_count_name_str = struct_global_recon_file_info_obj.source_count_name;
    source_file_path_ichat = struct_global_recon_file_info_obj.complete_file_path;

    create_right_clicks();
}



void ichat_parser::create_right_clicks()
{
    mainMenu->clear();

    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    mainMenu->setFont(m_font);

    //    QAction *action_select_all = new QAction(mainMenu);
    //    action_select_all = mainMenu->addAction(tr("Select All"));
    //    action_select_all->setFont(m_font);

    //    QAction *action_copy = new QAction(mainMenu);
    //    action_copy = mainMenu->addAction(tr("Copy"));
    //    action_copy->setFont(m_font);

    action_export = new QAction(mainMenu);
    action_export = mainMenu->addAction(tr(MACRO_Generic_Right_Click_Export_iChat));
    action_export->setFont(m_font);

    connect(mainMenu, SIGNAL(triggered(QAction*)), this, SLOT(slot_right_click_main_menu_triggered(QAction*)));

}

//void ichat_parser::act_select_all_ichat()
//{
//}

//void ichat_parser::act_copy_selected_text_ichat()
//{
//}

void ichat_parser::act_export_ichat()
{
    QString exported_file_path_str = source_file_path_ichat;
    QString suffix = recon_helper_standard_obj->mining_suffix(exported_file_path_str, Q_FUNC_INFO);
    exported_file_path_str.chop(suffix.size());
    exported_file_path_str.append("html");
    QString file_name_str = QFileInfo(exported_file_path_str).fileName();
    QString defualt_export_ichat_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_iChat_In_Result_QString).toString();


    export_dialog_box_obj->pub_set_name_and_dir_path(file_name_str,defualt_export_ichat_dir_path);
    export_dialog_box_obj->show();

}



