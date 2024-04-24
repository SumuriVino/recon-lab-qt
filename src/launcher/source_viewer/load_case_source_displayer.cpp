#include "load_case_source_displayer.h"
#include "ui_load_case_source_displayer.h"

load_case_source_displayer::load_case_source_displayer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::load_case_source_displayer)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    source_selector_obj = new source_selector(this);
    ui->verticalLayout_for_media_list_page->addWidget(source_selector_obj);


    recon_helper_standard_obj = new recon_helper_standard(this);

    source_selector_obj->pub_hide_widgets_for_load_case();
}

load_case_source_displayer::~load_case_source_displayer()
{
    delete ui;
}

void load_case_source_displayer::pub_set_list_of_source_path_and_type_in_page(QStringList source_path_with_type_list) //Set source path and type for source selection if load case button clicked
{
    source_selector_obj->pub_set_root_info_in_page(source_path_with_type_list);
}


void load_case_source_displayer::on_pushButton_ok_clicked() //Prepare all sources list and asks if any changes needed
{
    source_selector_obj->pub_get_essential_value_from_all_roots();

    prepare_all_sources_list_from_root_source();

}



void load_case_source_displayer::on_pushButton_cancel_clicked() // Hit when ok clicked
{
    QList<struct_GLOBAL_witness_info_source> list_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    emit signal_load_case_set_sources_path(list_target_info, false);
}


void load_case_source_displayer::prepare_all_sources_list_from_root_source() // Prepare all sources list from databases and update them
{
    //==============Root Update Start =================//

    QList<struct_GLOBAL_witness_info_root> list_exmnr_slctd_root_info = source_selector_obj->pub_get_root_info_list_from_gui();

    QList<struct_GLOBAL_witness_info_root> list_saved_and_updated_rt_info = global_witness_info_manager_class_obj->pub_get_root_structure_QList();

    for(int jj= 0 ; jj < list_exmnr_slctd_root_info.size(); jj++)
    {
        struct_GLOBAL_witness_info_root struct_exm_root_info = list_exmnr_slctd_root_info.at(jj);

        QString root_cnt_name = struct_exm_root_info.root_count_name;

        for(int kk = 0 ; kk < list_saved_and_updated_rt_info.size(); kk++)
        {
            struct_GLOBAL_witness_info_root strct_rt_info = list_saved_and_updated_rt_info.at(kk);

            if(root_cnt_name != strct_rt_info.root_count_name)
                continue;

            struct_GLOBAL_witness_info_root struct_to_update_root_info = list_saved_and_updated_rt_info.takeAt(kk);

            struct_to_update_root_info.root_path_1            = struct_exm_root_info.root_path_1;
            struct_to_update_root_info.root_path_2_for_fusion = struct_exm_root_info.root_path_2_for_fusion;

            list_saved_and_updated_rt_info.insert(kk , struct_to_update_root_info);
            break;
        }
    }

    global_witness_info_manager_class_obj->pub_set_updated_root_structure_QList(list_saved_and_updated_rt_info);

    //==============Root Update End =================//


    //==============Source Update Start =================//

    QList<struct_GLOBAL_witness_info_source> prepared_list_source_info;

    for(int kk = 0 ; kk < list_saved_and_updated_rt_info.size(); kk++)
    {
        struct_GLOBAL_witness_info_root st_root_info = list_saved_and_updated_rt_info.at(kk);

        QStringList source_count_name_prime_QStringList = st_root_info.source_count_name_prime_QStringList;
        for(int ii = 0 ; ii < source_count_name_prime_QStringList.size(); ii++)
        {
            QString source_count_name = source_count_name_prime_QStringList.at(ii);

            struct_GLOBAL_witness_info_source  struct_source_info_prime =  global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

            struct_source_info_prime.root_path_1 = st_root_info.root_path_1;
            struct_source_info_prime.root_path_2_for_fusion = st_root_info.root_path_2_for_fusion;

            if(st_root_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal
                    || st_root_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal
                    || st_root_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal
                    || st_root_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal
                    || st_root_info.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal)
            {
                // no need to update
                // becoz these sources are stored in result directory.
            }
            else
            {
                struct_source_info_prime.source_path_1 = st_root_info.root_path_1;
                struct_source_info_prime.source_path_2_for_fusion = st_root_info.root_path_2_for_fusion;

                struct_source_info_prime.source_main_path = st_root_info.root_path_1;


            }

            prepared_list_source_info.append(struct_source_info_prime);
        }

        QStringList source_count_name_nested_QStringList = st_root_info.source_count_name_nested_QStringList;

        for(int ii = 0 ; ii < source_count_name_nested_QStringList.size(); ii++)
        {
            QString source_count_name = source_count_name_nested_QStringList.at(ii);

            struct_GLOBAL_witness_info_source  struct_source_info_nested =  global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
            struct_source_info_nested.root_path_1 = st_root_info.root_path_1;
            struct_source_info_nested.root_path_2_for_fusion = st_root_info.root_path_2_for_fusion;

            prepared_list_source_info.append(struct_source_info_nested);
        }

    }


    emit signal_load_case_set_sources_path(prepared_list_source_info, true);

    //==============Source Update End =================//

}


QStringList load_case_source_displayer::get_source_count_name_list_from_root_source(QString root_count_name) // Get source count name list from databases
{

    QStringList src_cnt_list;
    root_count_name = root_count_name.trimmed();
    if(root_count_name.isEmpty())
        return src_cnt_list;

    QString sources_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";
    src_cnt_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(QString("Select table_name From source_index where root_count_name = ?"), QStringList(root_count_name), 0, sources_db_path, Q_FUNC_INFO);

    if(!src_cnt_list.isEmpty())
        return src_cnt_list;


    QString src_index_txt_filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_index.txt";


    ///get all source counts
    QFile file_index(src_index_txt_filepath);
    if(!file_index.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("-----FAILED----- file open:" + file_index.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + file_index.errorString(), Q_FUNC_INFO);
        return src_cnt_list;
    }

    QStringList all_src_cnt_list;

    while(!file_index.atEnd())
    {
        all_src_cnt_list << QString::fromLocal8Bit(file_index.readLine()).trimmed();
    }
    file_index.close();


    ///open each source and check if root count name matches
    for(int count = 0; count < all_src_cnt_list.size(); count++)
    {
        QString src_cnt_str = all_src_cnt_list.at(count);
        QString src_txt_filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + src_cnt_str;

        QFile file_src(src_txt_filepath);
        if(!file_src.open(QIODevice::ReadOnly))
        {
            recon_static_functions::app_debug("-----FAILED----- file open:" + file_src.fileName(), Q_FUNC_INFO);
            recon_static_functions::app_debug("Error:" + file_src.errorString(), Q_FUNC_INFO);
            continue;
        }

        while(!file_src.atEnd())
        {
            QString line_str = QString::fromLocal8Bit(file_src.readLine()).trimmed();

            if(line_str.startsWith(MACRO_NARAD_Root_Count_Name_QString))
            {
                if(line_str.contains("="))
                {
                    QStringList tmp_splt_list = line_str.split("=");
                    QString tmp_root_count = tmp_splt_list.at(tmp_splt_list.size() -1);
                    tmp_root_count = tmp_root_count.trimmed();


                    if(tmp_root_count == root_count_name)
                    {
                        src_cnt_str.remove(".txt");
                        src_cnt_str = src_cnt_str.trimmed();

                        src_cnt_list << src_cnt_str;
                    }
                }
                break;
            }
        }
        file_src.close();
    }


    return src_cnt_list;
}
