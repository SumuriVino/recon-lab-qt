#ifndef FACEFINDER_H
#define FACEFINDER_H

#include <QObject>
#include <QtCore>

#include <dlib/dnn.h>
#include <dlib/gui_widgets.h>
#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include "cropped_image_display.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "face_extractor/thread_face_find.h"
#include "task_status/task_status_window.h"
#include "header_global_variables_1.h"
#include "header_global_structures.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include <QMessageBox>


using namespace dlib;
using namespace std;


template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N, BN, 1, tag1<SUBNET>>>;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2, 2, 2, 2, skip1<tag2<block<N, BN, 2, tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block = BN<con<N, 3, 3, 1, 1, relu<BN<con<N, 3, 3, stride, stride, SUBNET>>>>>;

template <int N, typename SUBNET> using ares = relu<residual<block, N, affine, SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block, N, affine, SUBNET>>;


template <typename SUBNET> using alevel0 = ares_down<256, SUBNET>;
template <typename SUBNET> using alevel1 = ares<256, ares<256, ares_down<256, SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128, ares<128, ares_down<128, SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64, ares<64, ares<64, ares_down<64, SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32, ares<32, ares<32, SUBNET>>>;


using anet_type = loss_metric<fc_no_bias<128, avg_pool_everything<
    alevel0<
    alevel1<
    alevel2<
    alevel3<
    alevel4<
    max_pool<3, 3, 2, 2, relu<affine<con<32, 7, 7, 2, 2,
    input_rgb_image_sized<150>
    >>>>>>>>>>>>;


class facefinder : public QObject
{
    Q_OBJECT
public:
    explicit facefinder(QObject *parent = nullptr);


    bool check_is_image_have_face(QString file_path, QString fs_record_number, QString source_cnt_name);

    bool extract_cropped_faces(QString file_path);

    void pub_set_display_loading_progress_bar_obj(display_loading_progress_bar *obj);

    void pub_prepare_display();
    QString pub_get_cropped_faces_dir_path();
    void pub_set_essentials();
    void pub_start_search_clicked(QString src_path, QString search_label_name);

    void pub_clear_fields();

    void pub_set_task_status_window(task_status_window *obj);

    void pub_stop_face_finder_thread();


signals:
    void signal_add_matched_faces_to_case_tree(QString);

    void signal_task_status_cancel_clicked(QString job_type);


private slots:
    void slot_start_search_clicked(QString s_path, QString search_lbl_name);
    void slot_face_found(QString path);
    void slot_thread_face_find_finished();

    void slot_add_all_matched_faces_to_case_tree();

    void slot_face_search_cancelled(QString job_type);

private:


    task_status_window *task_status_window_obj;
    struct struct_record_info
    {
      int record_no;
      QString filepath;
      QString source_count_name;
    };

    QList<struct_record_info> struct_record_info_list;
    shape_predictor shape_predictor_obj;

    void SaveCropImage(matrix<rgb_pixel> &matCrop, int nCropIndex, QString path);

    QList<struct_GLOBAL_witness_info_source> list_target_source_info;

    display_loading_progress_bar *display_loading_progress_bar_obj;

    thread_face_find *thread_face_find_obj;

    cropped_image_display *cropped_image_display_obj;

    QMap<int, QString> map;

    QStringList found_path_list;

    bool open_and_create_face_search_db(QString path);

    QSqlDatabase face_search_db;

    QString db_path;

    QString cropped_faces_dir_path;

    QString search_name;

    int count = -1;

    bool bool_thread_face_search_running = false;
    recon_helper_standard *recon_helper_standard_obj;


    string str_cropped_image;
    void save_data_in_db(QString distinct_face_file_path, QString rec_number, QString source_count_name, bool bool_is_face_already_present, QString matched_face_path_str, QSqlDatabase *faces_db);
    float       m_fThreshold = 0.0f;
    int count_distinct_faces = 0;

    void copy_picture_searched_face_and_insert_filter_table(QString searched_face_picture_path_adhoc, QString searched_face_directory_path);

};

#endif // FACEFINDER_H
