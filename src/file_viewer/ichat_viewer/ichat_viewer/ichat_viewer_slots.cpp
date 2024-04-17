#include "ichat_viewer.h"

void ichat_parser::on_textBrowser_ichat_anchorClicked(const QUrl &arg1)
{
    QString filepath = arg1.toString();
    if(!QFileInfo(filepath).exists())
        return;

    QStringList arg;
    arg << "-R"<< filepath;

    QProcess *myProcess = new QProcess(this);
    myProcess->startDetached("open",arg);
}



void ichat_parser::slot_right_click_main_menu_triggered(QAction *current_clicked_action)
{
    if(current_clicked_action->text() == MACRO_Generic_Right_Click_Export_iChat)
    {
        act_export_ichat();
    }
    //    else if(current_clicked_action->text() == "Select All")
    //    {
    //        //act_select_all_ichat();
    //    }
    //    else if(current_clicked_action->text() == "Copy")
    //    {
    //        //act_copy_selected_text_ichat();
    //    }

}

void ichat_parser::slot_export_ichat_ok_clicked(QString given_name, QString selected_dir_path)
{
    export_dialog_box_obj->hide();


    given_name = given_name.trimmed();
    selected_dir_path = selected_dir_path.trimmed();
    if(given_name.isEmpty() || selected_dir_path.isEmpty())
        return;

    if(!selected_dir_path.endsWith("/"))
        selected_dir_path.append("/");


    QString file_nm_with_extn_html_str = given_name;


    QString tm_dir_nm_with_file_nm = file_nm_with_extn_html_str;
    if(tm_dir_nm_with_file_nm.endsWith(".html"))
        tm_dir_nm_with_file_nm.chop(QString(".html").size());

    tm_dir_nm_with_file_nm.replace(".","_");
    tm_dir_nm_with_file_nm =  "Exported Chat " + tm_dir_nm_with_file_nm;

    QString new_dir_nm_str = recon_static_functions::get_available_filename(tm_dir_nm_with_file_nm,selected_dir_path,Q_FUNC_INFO);
    QString export_chat_dir_path = selected_dir_path + new_dir_nm_str;
    QDir mk_dir;
    mk_dir.mkpath(export_chat_dir_path);

    exported_files_dir = export_chat_dir_path + "/files";
    mk_dir.mkpath(exported_files_dir);




    QString html_filepath_str = export_chat_dir_path + "/" + file_nm_with_extn_html_str;
    QFile m_file(html_filepath_str);
    if(!m_file.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug(" -- FILE Open ------FAILED----- " + m_file.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("--error-- " + m_file.errorString(),Q_FUNC_INFO);
        return;
    }



    file_name_list.clear();

    export_ichat_file_in_html_format(m_file);


    m_file.flush();
    m_file.close();

    QString m_message = "iChat has been exported. Do you want to open it?";
    report_status obj;
    obj.set_label(m_message);
    if(obj.exec())
    {
        QStringList arg;
        arg << html_filepath_str;

        QProcess *myProcess = new QProcess(this);
        myProcess->startDetached("open",arg);
    }

}


void ichat_parser::onCustomContextMenuRequested(const QPoint &pos)
{
    mainMenu->exec(QCursor::pos());
}
