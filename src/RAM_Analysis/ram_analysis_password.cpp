#include "ram_analysis.h"
#include "ui_ram_analysis.h"


void ram_analysis::password_first_method(QString file_path, struct_GLOBAL_witness_info_source src_obj)
{

    if(bool_cancel_loading)
        return;

    display_loading_progress_bar_obj->pub_set_label_messsge("First Pass: ");
    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(0);

    //   ui->progressBar->setValue(0);

    QFile mfile(file_path);
    if(!mfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File Open ---FAILED---: "  + mfile.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + mfile.errorString() , Q_FUNC_INFO);
        return;
    }

    qint64 file_size = mfile.size();

    int step_size = file_size / 100;

    qint64 bytes_read = 0;


    QByteArray master_acct;
    master_acct.append(12);
    master_acct.append(4);
    master_acct.append('a');
    master_acct.append('c');
    master_acct.append('c');
    master_acct.append('t');

    while(!mfile.atEnd())
    {
        if(bool_cancel_loading)
        {
            mfile.close();
            return;
        }

        QByteArray mar = mfile.read(10240);
        QCoreApplication::processEvents();


        bytes_read += mar.size();

        int current_progress = bytes_read / step_size;

        //   ui->progressBar->setValue(current_progress);
        display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(current_progress);



        char cch = 6;
        int index = 0;
        while(1)
        {
            if(bool_cancel_loading)
            {
                mfile.close();
                return;
            }

            index = mar.indexOf("acct", index);
            // index = mar.indexOf("acct", index);

            if(index < 0)
                break;

            if(index < 2)
            {
                index++;
                continue;
            }

            // 6

            if((int)mar.at(index - 1) != 4 || (int)mar.at(index - 2) != 12)
            {
                index++;
                continue;
            }


            if(mar.size() < index + 6)
            {
                index++;
                continue;
            }

            int account_length = mar.at(index + 5);

            if(mar.size() < index + 5 + account_length)
            {
                index++;
                continue;
            }

            QByteArray account_name = sub_array(mar, account_length, index + 6);

            //            for(int i = 0 ; i < account_length; i++)
            //            {

            //                if(mar.size() <= index + 6 + i)
            //                    break;

            //                account_name.append(mar.at(index + 6 + i));
            //            }

            if(account_name.size() < 1 || account_name.contains("com.apple.continuity.auto-unlock.sync"))
            {
                index++;
                continue;
            }


            int offset_length_of_server = index + 5 + account_length + 10;
            QByteArray server_name;
            QByteArray password;


            if(mar.size() >= offset_length_of_server)
            {

                if(mar.size() >= index + 5 + account_length + 10 + (int)mar.at(offset_length_of_server))
                {

                    server_name = sub_array(mar, (int)mar.at(offset_length_of_server),index + 5 + account_length + 11 );

                }


                QByteArray vdata = "v_Data";

                if(mar.size() >= index + 5 + account_length + 10 + (int)mar.at(offset_length_of_server) + 5)
                {
                    if(check_for_sub_array(mar, vdata, index + 5 + account_length + 10 + (int)mar.at(offset_length_of_server) + 5 ))
                    {
                        int offset_length_of_password = index + 5 + account_length + 10 + (int)mar.at(offset_length_of_server) + 12;

                        if(mar.size() >= offset_length_of_password)
                        {
                            if(mar.size() >= offset_length_of_password + (int)mar.at(offset_length_of_password))
                                password = sub_array(mar, (int)mar.at(offset_length_of_password),offset_length_of_password + 1);
                        }
                    }
                    else
                    {
                        index++;
                        continue;
                    }
                }

            }

            if(password.size() < 1)
            {
                index++;
                continue;
            }

            password_count++;


            display_loading_progress_bar_obj->pub_set_label_messsge("First Pass: " + QString::number(password_count));

            // ui->label_password_count->setText(QString::number(password_count));

            //            ui->textEdit->append("--------------------------");
            //            ui->textEdit->append("Account = " + account_name);
            //            ui->textEdit->append("Server = " + server_name);
            //            ui->textEdit->append("Password = " + password);

            //            //    ui->textEdit->append(account_name + "\t\t\t" + server_name +"\t\t\t" + password);
            //            ui->textEdit->append("--------------------------\n\n");


            struct_password_info m_obj;
            m_obj.m_account = account_name;
            m_obj.m_password = password;
            m_obj.m_server = server_name;
            m_obj.m_description = "";
            m_obj.m_source_count_name = src_obj.source_count_name;
            m_obj.m_os_scheme_display = src_obj.os_scheme_display;

            insert_password_info_in_db(m_obj);


            index++;

        }



    }


    mfile.close();
}

void ram_analysis::password_second_method(QString file_path, struct_GLOBAL_witness_info_source src_obj)
{

    if(bool_cancel_loading)
        return;

    //  ui->label_method->setText("password_fourth_method");

    //    ui->progressBar->setValue(0);

    display_loading_progress_bar_obj->pub_set_label_messsge("Second Pass: " + QString::number(password_count));
    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(0);


    QFile mfile(file_path);
    if(!mfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File Open ---FAILED---: "  + mfile.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + mfile.errorString() , Q_FUNC_INFO);
        return;
    }

    qint64 file_size = mfile.size();

    int step_size = file_size / 100;

    qint64 bytes_read = 0;


    while(!mfile.atEnd())
    {
        if(bool_cancel_loading)
        {
            mfile.close();
            return;
        }

        QByteArray mar = mfile.read(10240);
        QCoreApplication::processEvents();

        bytes_read += mar.size();

        int current_progress = bytes_read / step_size;

        //     ui->progressBar->setValue(current_progress);


        display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(current_progress);

        char cch = 6;
        int index = 0;
        while(1)
        {
            if(bool_cancel_loading)
            {
                mfile.close();
                return;
            }

            index = mar.indexOf("v_Data", index);

            if(index < 0)
                break;

            if((int)mar.at(index - 1) != 6 || (int)mar.at(index - 2) != 12)
            {
                index++;
                continue;
            }

            int password_length = mar.at(index + 7);

            QByteArray password = sub_array(mar, password_length, index + 8);

            if(password.size() < 1 )
            {
                index++;
                continue;
            }

            if(check_if_array_contains_non_readable_characters(password))
            {
                index++;
                continue;
            }


            QByteArray desc_str = "desc";

            if(!check_for_sub_array(mar, desc_str, index + 7 + password_length + 5))
            {

                password_count++;

                //                ui->textEdit->append("--------------------------");
                //                ui->textEdit->append("Password = " + password);
                //                ui->textEdit->append("--------------------------\n\n");

                display_loading_progress_bar_obj->pub_set_label_messsge("Second Pass: " + QString::number(password_count));

                struct_password_info m_obj;
                m_obj.m_account = "";
                m_obj.m_password = password;
                m_obj.m_server = "";
                m_obj.m_description = "";
                m_obj.m_source_count_name = src_obj.source_count_name;
                m_obj.m_os_scheme_display = src_obj.os_scheme_display;

                insert_password_info_in_db(m_obj);

                index++;
                continue;
            }


            int offset_length_of_desc = index + 7 + password_length + 10;
            int desc_name_length = (int)mar.at(offset_length_of_desc);
            QByteArray desc_name = sub_array(mar, desc_name_length, index + 7 + password_length + 11);



            QByteArray acct_str = "acct";

            if(!check_for_sub_array(mar, acct_str, index + 7 + password_length + 10 + desc_name_length + 5))
            {
                password_count++;

                display_loading_progress_bar_obj->pub_set_label_messsge("Second Pass: " + QString::number(password_count));

                //                ui->label_password_count->setText(QString::number(password_count));

                //                ui->textEdit->append("--------------------------");
                //                ui->textEdit->append("Desc = " + desc_name);
                //                ui->textEdit->append("Password = " + password);
                //                ui->textEdit->append("--------------------------\n\n");

                struct_password_info m_obj;
                m_obj.m_account = "";
                m_obj.m_password = password;
                m_obj.m_server = "";
                m_obj.m_description = desc_name;
                m_obj.m_source_count_name = src_obj.source_count_name;
                m_obj.m_os_scheme_display = src_obj.os_scheme_display;

                insert_password_info_in_db(m_obj);

                index++;
                continue;
            }

            int offset_length_of_acct = index + 7 + password_length + 10 + desc_name_length + 10;
            int acct_length = (int)mar.at(offset_length_of_acct);
            QByteArray account_name = sub_array(mar, acct_length, index + 7 + password_length + 10 + desc_name_length + 11);


            password_count++;


            display_loading_progress_bar_obj->pub_set_label_messsge("Second Pass: " + QString::number(password_count));

            //            ui->label_password_count->setText(QString::number(password_count));

            //            ui->textEdit->append("--------------------------");
            //            ui->textEdit->append("Account = " + account_name);
            //            ui->textEdit->append("Desc = " + desc_name);
            //            ui->textEdit->append("Password = " + password);

            //            ui->textEdit->append("--------------------------\n\n");



            struct_password_info m_obj;
            m_obj.m_account = account_name;
            m_obj.m_password = password;
            m_obj.m_server = "";
            m_obj.m_description = desc_name;
            m_obj.m_source_count_name = src_obj.source_count_name;
            m_obj.m_os_scheme_display = src_obj.os_scheme_display;

            insert_password_info_in_db(m_obj);

            index++;

        }



    }


    mfile.close();

}

void ram_analysis::password_third_method(QString file_path, struct_GLOBAL_witness_info_source src_obj)
{

    if(bool_cancel_loading)
        return;

    //  ui->label_method->setText("password_third_method");

    // ui->progressBar->setValue(0);
    display_loading_progress_bar_obj->pub_set_label_messsge("Third Pass: " + QString::number(password_count));
    display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(0);

    QFile mfile(file_path);
    if(!mfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File Open ---FAILED---: "  + mfile.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + mfile.errorString() , Q_FUNC_INFO);
        return;
    }

    qint64 file_size = mfile.size();

    int step_size = file_size / 100;

    qint64 bytes_read = 0;


    while(!mfile.atEnd())
    {

        if(bool_cancel_loading)
        {
            mfile.close();
            return;
        }

        QByteArray mar = mfile.read(10240);
        QCoreApplication::processEvents();

        bytes_read += mar.size();

        int current_progress = bytes_read / step_size;

        //    ui->progressBar->setValue(current_progress);

        display_loading_progress_bar_obj->pub_set_progress_bar_definite_value(current_progress);


        char cch = 6;
        int index = 0;
        while(1)
        {
            if(bool_cancel_loading)
            {
                mfile.close();
                return;
            }

            index = mar.indexOf("v_Data", index);

            if(index < 0)
                break;

            if((int)mar.at(index - 1) != 6 || (int)mar.at(index - 2) != 12)
            {
                index++;
                continue;
            }

            int password_length = mar.at(index + 7);

            QByteArray password = sub_array(mar, password_length, index + 8);

            if(password.size() < 1 )
            {
                index++;
                continue;
            }

            if(check_if_array_contains_non_readable_characters(password))
            {
                index++;
                continue;
            }


            QByteArray acct_str = "acct";

            if(!check_for_sub_array(mar, acct_str, index + 7 + password_length + 5))
            {
                password_count++;

                display_loading_progress_bar_obj->pub_set_label_messsge("Third Pass: " + QString::number(password_count));

                //                ui->label_password_count->setText(QString::number(password_count));


                //                ui->textEdit->append("--------------------------");
                //                ui->textEdit->append("Password = " + password);
                //                ui->textEdit->append("--------------------------\n\n");

                struct_password_info m_obj;
                m_obj.m_account = "";
                m_obj.m_password = password;
                m_obj.m_server = "";
                m_obj.m_description = "";
                m_obj.m_source_count_name = src_obj.source_count_name;
                m_obj.m_os_scheme_display = src_obj.os_scheme_display;

                insert_password_info_in_db(m_obj);

                index++;
                continue;
            }


            int offset_length_of_account = index + 7 + password_length + 10;
            int account_name_length = (int)mar.at(offset_length_of_account);
            QByteArray account_name = sub_array(mar, (int)mar.at(offset_length_of_account), index + 7 + password_length + 11);

            //            if(account_name.endsWith("-AuthToken"))
            //            {
            //                index++;
            //                continue;
            //            }


            QByteArray desc_str = "desc";

            if(!check_for_sub_array(mar, desc_str, index + 7 + password_length + 10 + account_name_length + 5))
            {

                password_count++;

                display_loading_progress_bar_obj->pub_set_label_messsge("Third Pass: " + QString::number(password_count));

                //                ui->textEdit->append("--------------------------");
                //                ui->textEdit->append("Account = " + account_name);
                //                ui->textEdit->append("Password = " + password);
                //                ui->textEdit->append("--------------------------\n\n");


                struct_password_info m_obj;
                m_obj.m_account = account_name;
                m_obj.m_password = password;
                m_obj.m_server = "";
                m_obj.m_description = "";
                m_obj.m_source_count_name = src_obj.source_count_name;
                m_obj.m_os_scheme_display = src_obj.os_scheme_display;

                insert_password_info_in_db(m_obj);
                index++;
                continue;
            }

            int offset_length_of_desc = index + 7 + password_length + 10 + account_name_length + 10;
            int desc_length = (int)mar.at(offset_length_of_desc);
            QByteArray desc_name = sub_array(mar, desc_length, index + 7 + password_length + 10 + account_name_length + 11);

            password_count++;

            //            ui->label_password_count->setText(QString::number(password_count));

            //            ui->textEdit->append("--------------------------");
            //            ui->textEdit->append("Account = " + account_name);
            //            ui->textEdit->append("Desc = " + desc_name);
            //            ui->textEdit->append("Password = " + password);

            //            ui->textEdit->append("--------------------------\n\n");


            display_loading_progress_bar_obj->pub_set_label_messsge("Third Pass: " + QString::number(password_count));

            struct_password_info m_obj;
            m_obj.m_account = account_name;
            m_obj.m_password = password;
            m_obj.m_server = "";
            m_obj.m_description = desc_name;
            m_obj.m_source_count_name = src_obj.source_count_name;
            m_obj.m_os_scheme_display = src_obj.os_scheme_display;

            insert_password_info_in_db(m_obj);


            index++;

        }



    }


    mfile.close();
}





QByteArray ram_analysis::sub_array(QByteArray ar, int length, int offset)
{
    QByteArray mar;

    if(ar.size() < offset + length)
        return mar;

    for(int i = 0; i < length; i++)
    {
        mar.append(ar.at(offset + i));
    }

    return mar;
}

bool ram_analysis::check_for_sub_array(QByteArray source, QByteArray to_check, int offset)
{
    QByteArray arr;

    bool ok = false;

    if(source.size() < offset + to_check.size())
    {
        return ok;
    }


    for(int i = 0; i < to_check.size(); i++)
    {

        arr.append(source.at(offset + i));

    }


    if(arr == to_check)
        return true;

    return ok;

}

bool ram_analysis::check_if_array_contains_non_readable_characters(QByteArray ar)
{

    bool ok = false;

    for(int i = 0 ; i < ar.size(); i++)
    {

        if((int)ar.at(i) < 0 || (int)ar.at(i) > 127)
        {
            ok = true;
            return ok;
        }

    }
    return ok;
}

void ram_analysis::insert_password_info_in_db(ram_analysis::struct_password_info m_obj)
{
    QString command1 = QString("INSERT INTO carved_password"
                               "(bookmark ,plugin_name, tab_name ,m_password ,m_server,m_account, m_description, source_count_name,os_scheme_display)"
                               "VALUES(?,?,?,?,?,?,?, ?,?)");
    QStringList arg_value_list;
    arg_value_list << "0" << MACRO_Plugin_Name_Carved_Password << MACRO_Plugin_Carved_Passwords_TAB_Name_Passwords << m_obj.m_password << m_obj.m_server << m_obj.m_account << m_obj.m_description << m_obj.m_source_count_name << m_obj.m_os_scheme_display;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command1,arg_value_list,carved_password_db_path,Q_FUNC_INFO);
}
