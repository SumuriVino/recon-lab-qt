#include "facefinder.h"

facefinder::facefinder(QObject *parent) : QObject(parent)
{


}

void facefinder::pub_set_essentials()
{ // set essntionls things
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    cropped_image_display_obj = new cropped_image_display();
    connect(cropped_image_display_obj, SIGNAL(signal_start_search_clicked(QString,QString)), this, SLOT(slot_start_search_clicked(QString, QString)));

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

bool facefinder::check_is_image_have_face(QString file_path, QString fs_record_number, QString source_cnt_name)
{ // check if image have face or not for search
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    //    deserialize("Models/dlib/shape_predictor_68_face_landmarks.dat") >> shape_predictor_obj;
    deserialize("../Resources/features_resources/face_analysis/Models/dlib/shape_predictor_68_face_landmarks.dat") >> shape_predictor_obj;

    frontal_face_detector detector = get_frontal_face_detector();

    matrix<rgb_pixel> imgSrc;
    load_image(imgSrc, file_path.toStdString());

    std::vector<matrix<rgb_pixel>> facesSrc;
    for (auto face : detector(imgSrc))
    {
        auto shape = shape_predictor_obj(imgSrc, face);
        matrix<rgb_pixel> face_chip;
        extract_image_chip(imgSrc, get_face_chip_details(shape, 150, 0.25), face_chip);
        facesSrc.push_back(face_chip);
        //        break;
        // Also put some boxes on the faces so we can see that the detector is finding them.
        //winTarget.add_overlay(face);
    }
    if (facesSrc.size() == 0)
    {
        return false;
    }

    QString rslt_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString distinct_faces_cmplt_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Distinct_Faces_In_Result_QString).toString();

    std::vector<string> vStrTargetFileList;

    QStringList target_file_path_list;

    QDirIterator itr(distinct_faces_cmplt_dir_path , QDirIterator::Subdirectories);

    while (itr.hasNext())
    {
        QString file_path = itr.next();

        if(file_path.endsWith(".") || file_path.endsWith(".."))
            continue;

        target_file_path_list.append(file_path);

    }

    anet_type net;
    //    deserialize("Models/dlib/dlib_face_recognition_resnet_model_v1.dat") >> net;
    deserialize("../Resources/features_resources/face_analysis/Models/dlib/dlib_face_recognition_resnet_model_v1.dat") >> net;

    std::vector<matrix<float, 0, 1>> descriptorsSrc = net(facesSrc);

    if(!target_file_path_list.isEmpty())
    {
        for(int i = 0; i < target_file_path_list.size(); i++)
        {

            QString path = target_file_path_list.at(i);
            string strPath = QFileInfo(path).filePath().toStdString();
            vStrTargetFileList.push_back(strPath);
        }
    }

    int nRepeat;

    matrix<rgb_pixel> imgTemp, imgTarget;

    nRepeat = 0;


    QString cropped_face_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Faces_Group_In_Result_QString).toString() + "face_info.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase cropped_faces_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);

    cropped_faces_db.setDatabaseName(cropped_face_db_path);

    if(!cropped_faces_db.open())
        return false;

    cropped_faces_db.transaction();


    for(size_t ii = 0; ii < descriptorsSrc.size(); ii++)
    {

        bool bool_face_already_present = false;

        QString matched_face_path;

        for (uint k = 0; k < vStrTargetFileList.size(); k++)
        {

            load_image(imgTarget, vStrTargetFileList[k]);
            std::vector<matrix<rgb_pixel>> facesTarget;
            std::vector<matrix<rgb_pixel>> vCropImage;
            nRepeat = 0;

            while( nRepeat < 3 )
            {

                if( nRepeat == 1 )
                    resize_image(2.0, imgTarget);
                else if( nRepeat == 2 )
                    resize_image(4.0, imgTarget);

                for (auto face : detector(imgTarget))
                {

                    auto shape = shape_predictor_obj(imgTarget, face);
                    matrix<rgb_pixel> face_chip;
                    extract_image_chip(imgTarget, get_face_chip_details(shape, 150, 0.25), face_chip);
                    facesTarget.push_back(face_chip);
                    set_image_size(face_chip, 150, 150);
                    if( nRepeat == 1 )
                        resize_image(0.5, face_chip);
                    else if( nRepeat == 2 )
                        resize_image(0.25, face_chip);
                    vCropImage.push_back(face_chip);
                    // Also put some boxes on the faces so we can see that the detector is finding them.
                    //win.add_overlay(face);
                }
                if (facesTarget.size() > 0)
                    break;
                nRepeat++;
            }

            if( nRepeat == 3 )
                continue;


            std::vector<matrix<float, 0, 1>> descriptorsTarget = net(facesTarget);

            if( m_fThreshold == 0.0f )
                m_fThreshold = 0.5f;


            for (size_t i = 0; i < descriptorsTarget.size(); ++i)
            {
                if (length(descriptorsTarget[i] - descriptorsSrc[ii]) < m_fThreshold )
                {
                    matched_face_path = target_file_path_list.at(k);

                    bool_face_already_present = true;

                    break;
                }

            }

            if(bool_face_already_present)
                break;

        }

        if(!bool_face_already_present)
        {
            SaveCropImage(facesSrc[ii], ii * 100, distinct_faces_cmplt_dir_path);
        }

        if(matched_face_path.startsWith(rslt_dir_path))
            matched_face_path.remove(0,rslt_dir_path.size());

        save_data_in_db(QString::fromStdString(str_cropped_image), fs_record_number, source_cnt_name, bool_face_already_present, matched_face_path, &cropped_faces_db);
    }

    cropped_faces_db.commit();
    cropped_faces_db.close();
    return true;

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void facefinder::save_data_in_db(QString distinct_face_file_path, QString rec_number, QString source_count_name, bool bool_is_face_already_present, QString matched_face_path_str, QSqlDatabase *faces_db)
{ // save data in database
    QString unique_face_id;

    if(!distinct_face_file_path.startsWith("/"))
        distinct_face_file_path = distinct_face_file_path.prepend("/");

    if(!matched_face_path_str.startsWith("/"))
        matched_face_path_str = matched_face_path_str.prepend("/");

    QStringList values;
    if(!bool_is_face_already_present)
    {
        QString insert_cmd = "insert into distinct_faces(fs_record, source_count_name, file_path, face_id) VALUES(?,?,?,(SELECT ifnull(max(face_id),0) + 1 from distinct_faces))";

        values.clear();
        values << rec_number << source_count_name << distinct_face_file_path;


        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_cmd, values, *faces_db, Q_FUNC_INFO);

        QString select_cmd = "SELECT face_id from distinct_faces WHERE fs_record = ? AND source_count_name LIKE ? AND file_path LIKE ?";

        values.clear();
        values << rec_number << source_count_name << distinct_face_file_path;

        unique_face_id = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(select_cmd, values, 0, *faces_db, Q_FUNC_INFO);

    }

    else
    {
        QString select_cmd = "select face_id from distinct_faces where file_path LIKE ?";
        values.clear();
        values << matched_face_path_str;

        QString face_id_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(select_cmd, values, 0, *faces_db, Q_FUNC_INFO);

        unique_face_id = face_id_str;

    }

    QString insert_into_all_faces_cmd = "insert into all_faces(fs_record, source_count_name, face_id) VALUES(?,?,?)";

    values.clear();
    values << rec_number << source_count_name << unique_face_id;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(insert_into_all_faces_cmd, values, *faces_db, Q_FUNC_INFO);

}

bool facefinder::extract_cropped_faces(QString file_path)
{ // extract cropped faces
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QMimeDatabase obj;
    QString mime_type_for_file = obj.mimeTypeForFile(file_path, QMimeDatabase::MatchContent).name();
    if(!(mime_type_for_file.startsWith("image/") && (mime_type_for_file.contains("jpeg", Qt::CaseInsensitive) || mime_type_for_file.contains("jpg", Qt::CaseInsensitive) || mime_type_for_file.contains("png", Qt::CaseInsensitive)) && !QFileInfo(file_path).fileName().startsWith(".")))
    {
        return false;
    }

    //    deserialize("Models/dlib/shape_predictor_68_face_landmarks.dat") >> shape_predictor_obj;
    deserialize("../Resources/features_resources/face_analysis/Models/dlib/shape_predictor_68_face_landmarks.dat") >> shape_predictor_obj;

    frontal_face_detector detector = get_frontal_face_detector();
    matrix<rgb_pixel> imgSrc;
    load_image(imgSrc, file_path.toStdString());
    std::vector<matrix<rgb_pixel>> facesSrc;
    for (auto face : detector(imgSrc))
    {
        auto shape = shape_predictor_obj(imgSrc, face);
        matrix<rgb_pixel> face_chip;
        extract_image_chip(imgSrc, get_face_chip_details(shape, 150, 0.25), face_chip);
        facesSrc.push_back(face_chip);

        // Also put some boxes on the faces so we can see that the detector is finding them.
        //winTarget.add_overlay(face);
    }

    QString faces_adhoc_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Recognize_Faces_In_Result_QString).toString();

    if(QDir(faces_adhoc_dir_path).exists())
        QDir(faces_adhoc_dir_path).removeRecursively();

    QDir dir_faces;
    dir_faces.mkpath(faces_adhoc_dir_path);

    if (facesSrc.size() == 0)
    {
        QMessageBox::information(cropped_image_display_obj, recon_static_functions::get_app_name(), "No face(s) found", QMessageBox::Ok);
        return false;
    }

    anet_type net_type_obj;

    //    deserialize("Models/dlib/dlib_face_recognition_resnet_model_v1.dat") >> net_type_obj;
    deserialize("../Resources/features_resources/face_analysis/Models/dlib/dlib_face_recognition_resnet_model_v1.dat") >> net_type_obj;

    std::vector<matrix<float, 0, 1>> descriptorsSrc = net_type_obj(facesSrc);

    QString tmp_face_path = QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Recognize_Faces_In_Result_QString).toString());

    for(size_t ii = 0; ii < descriptorsSrc.size(); ii++)
    {
        SaveCropImage(facesSrc[ii], ii * 100, tmp_face_path);
    }

    cropped_faces_dir_path = faces_adhoc_dir_path;

    return true;

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

QString facefinder::pub_get_cropped_faces_dir_path()
{ // cropped face directory path
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    return cropped_faces_dir_path;

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void facefinder::pub_prepare_display()
{ // prepare display the face search result in table
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(cropped_faces_dir_path.isEmpty() || cropped_faces_dir_path.isNull())
        return;

    cropped_image_display_obj->prepare_image_display(cropped_faces_dir_path);

    cropped_image_display_obj->show();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}


void facefinder::pub_set_display_loading_progress_bar_obj(display_loading_progress_bar *obj)
{ // progress bar
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    display_loading_progress_bar_obj = obj;

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void facefinder::pub_start_search_clicked(QString src_path, QString search_label_name)
{ // start search button clicked after importing any face image
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString searched_face_picture_path_adhoc = src_path;

    QStringList filepath_list;

    struct_record_info_list.clear();

    int cnt_rec = -1;

    QString face_info_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Faces_Group_In_Result_QString).toString()
            + QString("face_info.sqlite");

    QString count_cmd = "SELECT COUNT(*) from distinct_faces";
    QString str = recon_helper_standard_obj->get_string_from_db_by_dbpath(count_cmd, 0, face_info_db_path, Q_FUNC_INFO);
    cnt_rec += str.toInt();

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString connection_naam = QString(Q_FUNC_INFO);

    QSqlDatabase::removeDatabase(connection_naam);

    QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    dest_db.setDatabaseName(face_info_db_path);

    if(!dest_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + dest_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + dest_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QString slct_command_face;

    slct_command_face = QString("select fs_record ,file_path , source_count_name from distinct_faces");

    QSqlQuery query(dest_db);
    query.prepare(slct_command_face);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" select query ----FAILED---- " + query.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query.lastError().text() , Q_FUNC_INFO);
        dest_db.close();
        return;
    }

    int countt = 0;
    while(query.next())
    {

        if(countt % 20 == 0)
        {
            QCoreApplication::processEvents();
            countt = 0;
        }
        countt++;

        QString rec_int = query.value(0).toString();
        QString file_path = query.value(1).toString();
        QString source_count_name = query.value(2).toString();

        QString distinct_face_file_path = result_dir_path + file_path;

        if(recon_helper_standard_obj->bool_is_permission_restricted_file(distinct_face_file_path, Q_FUNC_INFO))
            continue;

        distinct_face_file_path = distinct_face_file_path.trimmed();

        struct_record_info obj_info;
        obj_info.record_no = rec_int.toInt();
        obj_info.filepath = distinct_face_file_path;
        obj_info.source_count_name = source_count_name;

        struct_record_info_list.append(obj_info);

        filepath_list.append(distinct_face_file_path);
    }
    dest_db.close();

    found_path_list.clear();

    QString face_search_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Face_Search_In_Result_QString).toString() + "index_face_search.sqlite";
    QStringList search_label_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("Select search_label_name from face_search_index"), 0, face_search_index_db, Q_FUNC_INFO);
    search_label_name = recon_static_functions::get_available_stringname(search_label_name, search_label_name_list, Q_FUNC_INFO);
    search_label_name = search_label_name.trimmed();
    search_name = search_label_name;

    QString searched_face_dir_name = search_label_name;
    searched_face_dir_name = searched_face_dir_name.replace(" ", "_");

    QString searched_face_directory_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Face_Search_In_Result_QString).toString() + searched_face_dir_name;

    QDir dir_fs;
    if(!dir_fs.mkdir(searched_face_directory_path))
    {
        recon_static_functions::app_debug("face_search_directory_path  ----FAILED---- " + searched_face_directory_path, Q_FUNC_INFO);
        return;
    }

    QString searched_face_db_path = searched_face_directory_path + QString("/") + ("record.sqlite");

    if(!open_and_create_face_search_db(searched_face_db_path))
        return;

    copy_picture_searched_face_and_insert_filter_table(searched_face_picture_path_adhoc, searched_face_directory_path);

    if(bool_thread_face_search_running)
    {
        QMessageBox::information(cropped_image_display_obj, recon_static_functions::get_app_name(), "Face Search module already running.");

        return;
    }

    thread_face_find_obj = new thread_face_find;
    thread_face_find_obj->m_strSrcFilePath = src_path;
    thread_face_find_obj->target_path_QStringList = filepath_list;
    thread_face_find_obj->total_rec_count = cnt_rec;

    connect(thread_face_find_obj, SIGNAL(FaceFound(QString)), this, SLOT(slot_face_found(QString)));
    connect(thread_face_find_obj, SIGNAL(signal_finished()), this, SLOT(slot_thread_face_find_finished()));
    connect(thread_face_find_obj, SIGNAL(signal_add_all_matches_to_case_tree()), this, SLOT(slot_add_all_matched_faces_to_case_tree()));

    connect(thread_face_find_obj, SIGNAL(signal_PBAR_thread_file_system_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_face_find_obj, SIGNAL(signal_PBAR_thread_file_system_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_face_find_obj, SIGNAL(signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);

    disconnect(task_status_window_obj, SIGNAL(signal_cancel_face_search(QString)), this, SLOT(slot_face_search_cancelled(QString)));
    connect(task_status_window_obj, SIGNAL(signal_cancel_face_search(QString)), this, SLOT(slot_face_search_cancelled(QString)));

    disconnect(this, SIGNAL(signal_task_status_cancel_clicked(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(this, SIGNAL(signal_task_status_cancel_clicked(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));


    bool_thread_face_search_running = true;

    thread_face_find_obj->start();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void facefinder::pub_clear_fields()
{ // clear previous values
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    cropped_image_display_obj->pub_clear_fields();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void facefinder::pub_set_task_status_window(task_status_window *obj)
{ // set task status for counter
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    task_status_window_obj = obj;

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void facefinder::pub_stop_face_finder_thread()
{ // stop face finder
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(thread_face_find_obj != NULL && !thread_face_find_obj->isRunning())
    {
        thread_face_find_obj->m_bThreadExit = true;
        thread_face_find_obj->quit();
        delete thread_face_find_obj;
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void facefinder::slot_start_search_clicked(QString s_path, QString search_lbl_name)
{ // slot of start face search clicked
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    cropped_image_display_obj->hide();

    pub_start_search_clicked(s_path, search_lbl_name);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

bool facefinder::open_and_create_face_search_db(QString path)
{ // open and create face search database
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QFile file(path);

    if(file.exists())
    {
        QString file_name = QFileInfo(path).fileName();

        QString file_path = QFileInfo(path).filePath();

        file_path.chop(file_name.size());

        QString fnl_file_name = recon_static_functions::get_available_filename(file_name,file_path, Q_FUNC_INFO);

        file_path.append(fnl_file_name);

        path = file_path;
    }

    db_path = path;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    face_search_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    face_search_db.setDatabaseName(db_path);

    if(!face_search_db.open())
        return false;


    QString cmd = "create table files(INT INTEGER PRIMARY KEY, fs_record_no INTEGER, source_count_name varchar(200))";

    recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, face_search_db, Q_FUNC_INFO);

    QString cmd_create = "create table filter(INT INTEGER PRIMARY KEY, searched_face_pic_path varchar(200))";

    recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_create, face_search_db, Q_FUNC_INFO);

    return true;

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void facefinder::copy_picture_searched_face_and_insert_filter_table(QString searched_face_picture_path_adhoc, QString searched_face_directory_path)
{ // copy picture searched face and insert the info in database
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString searched_face_picture_ext_type = recon_helper_standard_obj->mining_suffix(searched_face_picture_path_adhoc, Q_FUNC_INFO);

    QString searched_face_picture_name = "face." + searched_face_picture_ext_type;

    QString destination_searched_face_picture_path = searched_face_directory_path + QString("/") + searched_face_picture_name;

    if (!QFile::copy(searched_face_picture_path_adhoc, destination_searched_face_picture_path))
    {
        recon_static_functions::app_debug("searched_face_picture_copy  --FAILED-- " , Q_FUNC_INFO);
        return;
    }

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    if(destination_searched_face_picture_path.startsWith(result_dir_path))
        destination_searched_face_picture_path = destination_searched_face_picture_path.remove(0, result_dir_path.size());

    if(!destination_searched_face_picture_path.startsWith("/"))
        destination_searched_face_picture_path.prepend("/");

    QString command_insert = "INSERT into filter(searched_face_pic_path) VALUES (?)";
    QStringList value_list;
    value_list << destination_searched_face_picture_path;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert, value_list, face_search_db, Q_FUNC_INFO);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void facefinder::slot_face_found(QString path)
{ // slot of when face found while searching, store these all info in database also
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString cmd = "INSERT into files (fs_record_no, source_count_name) VALUES(?,?)";

    QSqlQuery query_insert(face_search_db);

    for(int ii = 0; ii < struct_record_info_list.size(); ii++)
    {
        struct_record_info obj = struct_record_info_list.at(ii);

        query_insert.prepare(cmd);

        if(obj.filepath == path.trimmed())
        {
            QString fs_record_no = QString::number(obj.record_no);
            QString source_count_name = obj.source_count_name;
            QStringList values;
            values << fs_record_no << source_count_name;

            QString face_info_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Faces_Group_In_Result_QString).toString()
                    + QString("face_info.sqlite");

            QString select_face_id_command = "Select face_id from distinct_faces Where fs_record = ? And source_count_name = ? ";

            QString face_id = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_face_id_command, values, 0 , face_info_db_path, Q_FUNC_INFO);

            QString command_all_faces = "SELECT fs_record, source_count_name from all_faces where face_id = ?";

            QStringList fs_record_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command_all_faces,QStringList() << face_id, 0, face_info_db_path, Q_FUNC_INFO);

            QStringList source_count_name_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command_all_faces, QStringList() << face_id, 1, face_info_db_path, Q_FUNC_INFO);

            if(fs_record_list.size() == source_count_name_list.size())
            {
                for(int jj = 0 ; jj < fs_record_list.size() ; jj++)
                {
                    query_insert.addBindValue(fs_record_list.at(jj));

                    query_insert.addBindValue(source_count_name_list.at(jj));

                    if(!query_insert.exec())
                    {
                        recon_static_functions::app_debug(" select query ----FAILED---- " + query_insert.executedQuery() , Q_FUNC_INFO);

                        recon_static_functions::app_debug("Error " + query_insert.lastError().text() , Q_FUNC_INFO);

                        return;
                    }
                }
            }
            else
            {
                break;
            }
        }
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void facefinder::slot_thread_face_find_finished()
{ // face finder finished
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString select_cmd = "SELECT COUNT(*) from files";

    QString count_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(select_cmd, 0, face_search_db, Q_FUNC_INFO);


    QString search_label = search_name;
    QString face_search_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Face_Search_In_Result_QString).toString() + "index_face_search.sqlite";

    QString insert_cmd = "INSERT into face_search_index (search_label_name, search_db_file_name, total_records_extracted) VALUES(?,?,?)";
    QStringList values;
    QString search_db_name = search_label;

    search_db_name = "record.sqlite";

    values << search_label << search_db_name << count_str;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd, values, face_search_index_db, Q_FUNC_INFO);

    face_search_db.close();


    bool_thread_face_search_running = false;

    thread_face_find_obj->m_bThreadExit = true;
    thread_face_find_obj->quit();

    if(thread_face_find_obj != NULL && !thread_face_find_obj->isRunning())
        delete thread_face_find_obj;


    if(task_status_window_obj->get_running_tasks_count() < 1)
        task_status_window_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void facefinder::slot_add_all_matched_faces_to_case_tree()
{ // add matched faces to case tree to display
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    emit signal_add_matched_faces_to_case_tree(search_name);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void facefinder::slot_face_search_cancelled(QString job_type)
{ // face search canceled
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    thread_face_find_obj->pub_set_bool_cancel_extraction(true);

    emit signal_task_status_cancel_clicked(job_type);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void facefinder::SaveCropImage(matrix<rgb_pixel>& matCrop, int nCropIndex,QString path)
{ // save crop images in desrtination dir
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    time_t now;
    struct tm *tm;
    now = time(nullptr);
    tm = localtime (&now);
    string strTime = to_string(tm->tm_year+1900) +
            to_string(tm->tm_mon+1) +
            to_string(tm->tm_mday) +
            to_string(tm->tm_hour) +
            to_string(tm->tm_min) +
            to_string(tm->tm_sec) +
            to_string(nCropIndex);

    QString rslt_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString distinct_faces_result_dir = path;

    if(distinct_faces_result_dir.startsWith(rslt_dir_path))
        distinct_faces_result_dir = distinct_faces_result_dir.remove(0, rslt_dir_path.size());

    string cropped_image_path = path.toStdString() + strTime + ".jpg";
    str_cropped_image = distinct_faces_result_dir.toStdString() + strTime + ".jpg";

    save_jpeg(matCrop, cropped_image_path);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}
