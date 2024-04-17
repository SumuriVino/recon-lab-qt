#include "recognize_face.h"
#include "ui_recognize_face.h"

recognize_face::recognize_face(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recognize_face)
{
    ui->setupUi(this);

    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qGuiApp->primaryScreen()->availableGeometry()));
    setWindowTitle("Recognize Face");

    display_loading_progress_bar_obj = new display_loading_progress_bar();

    cropped_image_display_obj = new cropped_image_display(this);
    connect(cropped_image_display_obj, SIGNAL(signal_start_search_clicked(QString,QString)), this, SLOT(slot_start_search_clicked(QString, QString)));

    setWindowFlags(Qt::Window);
    activateWindow();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(cropped_image_display_obj);
    ui->frame_crop_faces_window->setLayout(layout);
    bool_status_face_found = false;

    face_finder_obj = new facefinder(this);
    face_finder_obj->pub_set_display_loading_progress_bar_obj(display_loading_progress_bar_obj);
    connect(face_finder_obj, SIGNAL(signal_add_matched_faces_to_case_tree(QString)), this, SIGNAL(signal_add_search_result_in_case_tree(QString)));
}

recognize_face::~recognize_face()
{
    delete ui;
}

void recognize_face::pub_set_task_status_window(task_status_window *obj)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    task_status_window_obj = obj;
    face_finder_obj->pub_set_task_status_window(task_status_window_obj);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void recognize_face::slot_start_search_clicked(QString s_path,QString lbl_name)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    recon_helper_standard_obj = new recon_helper_standard;

    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    int cnt_rec = 0;
    for(int count = 0; count < list_target_source_info.size(); count++)
    {
        struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);

        QString face_analysis_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + obj.source_count_name + QString("/face_analysis.sqlite");

        QString count_cmd = "SELECT COUNT(*) from files";

        QString str_rec_cnt = recon_helper_standard_obj->get_string_from_db_by_dbpath(count_cmd, 0, face_analysis_db_path, Q_FUNC_INFO);

        cnt_rec += str_rec_cnt.toInt();
    }

    if(s_path.trimmed().isEmpty() && lbl_name.trimmed().isEmpty())
    {
        QString msg_str = "Please select the Picture and Extract Faces.";
        QMessageBox::information(this, recon_static_functions::get_app_name(), msg_str, QMessageBox::Ok);
        return;
    }

    if(cnt_rec == 0)
    {
        QString msg_str = "Please process Face Analysis first.";
        QMessageBox::information(this, recon_static_functions::get_app_name(), msg_str, QMessageBox::Ok);
        hide();
        return;
    }

    if(bool_status_face_found)
    {
        hide();
        face_finder_obj->pub_start_search_clicked(s_path, lbl_name);
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void recognize_face::pub_clear_fields()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    ui->lineEdit_source_path->clear();

    cropped_image_display_obj->pub_clear_fields();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void recognize_face::on_pushButton_import_clicked()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString file_path = QFileDialog::getOpenFileName(this, recon_static_functions::get_app_name(), "/Volumes" , ("*.jpg *.JPG *.png *.PNG *.jpeg *.JPEG *.bmp *.BMP *.dng *.DNG"));

    ui->lineEdit_source_path->setText(file_path.trimmed());

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void recognize_face::on_pushButton_crop_faces_clicked()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    bool_status_face_found = false;

    QString path = ui->lineEdit_source_path->text().trimmed();

    if(path.isEmpty() || path.isNull())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please import the image file for searching.", QMessageBox::Ok);
        return;
    }

    bool_status_face_found = face_finder_obj->extract_cropped_faces(path);

    if(bool_status_face_found)
    {
        QString dir_path = face_finder_obj->pub_get_cropped_faces_dir_path();

        cropped_image_display_obj->prepare_image_display(dir_path);
    }
    else
    {
        cropped_image_display_obj->pub_clear_fields();
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}


