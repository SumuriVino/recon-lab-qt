#include <QtCore>
#include <QtSql>
#include "ichat_viewer.h"
#include "ui_ichat_viewer.h"

#include "InstantMessage.m"
#include "Presentity.m"
#include "StubCoder.m"

#import <Foundation/Foundation.h>
#import <Foundation/NSAutoreleasePool.h>


void ichat_parser::extract_xcode_archieved_chat(QString source_file)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);



    if(!source_file.endsWith(".ichat"))
    {
        ui->stackedWidget->setCurrentIndex(0);
        return;
    }


    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSString *filePath = source_file.toNSString();
    [NSKeyedUnarchiver setClass:[InstantMessage class] forClassName:@"InstantMessage"];
    [NSKeyedUnarchiver setClass:[Presentity class] forClassName:@"Presentity"];

    [NSKeyedUnarchiver setClass:[StubCoder class] forClassName:@"NSFont"];
    [NSKeyedUnarchiver setClass:[StubCoder class] forClassName:@"NSMutableParagraphStyle"];
    [NSKeyedUnarchiver setClass:[StubCoder class] forClassName:@"NSTextAttachment"];
    [NSKeyedUnarchiver setClass:[StubCoder class] forClassName:@"NSColor"];

    NSData *data = [NSData dataWithContentsOfFile:filePath];
    NSKeyedUnarchiver* unarchiver = [[NSKeyedUnarchiver alloc] initForReadingWithData:data];
    NSArray* root = [unarchiver decodeObjectForKey:@"$root"];
    NSMutableArray *ns_mutable_array = [NSMutableArray array];
    NSMutableSet *people = [NSMutableSet set];


    QString account_str , participants_str ;
    for (id object in root)
    {
        if ([object isKindOfClass:[NSArray class]])
        {
            for (id sub in object)
            {

                if ([sub isKindOfClass:[Presentity class]])
                {
                    Presentity *prs = (Presentity *) sub;
                    [people addObject:prs.accountName];
                    account_str = QString::fromNSString(prs.accountName);
                }

                if ([sub isKindOfClass:[InstantMessage class]])
                {
                    InstantMessage *im = (InstantMessage *) sub;
                    if ([im subject] != nil)
                    {
                        [people addObject:im.subject.accountName];
                    }
                    if ([im sender] != nil)
                    {
                        [people addObject:im.sender.accountName];
                    }
                    [ns_mutable_array addObject:im];


                    QString part_str = QString::fromNSString(im.sender.accountName);
                    if(account_str != part_str)
                    {
                        participants_str = part_str;
                    }
                }

            }
        }
    }

    QString ichat_html_data_for_display_str = chat_template_class::chat_page_start();
    ichat_html_data_for_display_str += chat_template_class::html_table_start();

    QString message_str;
    for (InstantMessage* instant_message_obj in ns_mutable_array)
    {
        if([people count] > 2)
        {
            [instant_message_obj setIsMultiParty:true];
        }

        [instant_message_obj setParticipantIds:people];
        [instant_message_obj setChatId: [root lastObject]];

        NSDictionary *ns_dictionary_obj = [instant_message_obj get_message_detail];


        QString message_sender_str;
        id id_sndr = [ns_dictionary_obj objectForKey:@"sender"];
        if([id_sndr isKindOfClass:[NSString class]])
        {
            message_sender_str = QString::fromNSString(id_sndr);
            if(message_sender_str.startsWith("e:"))
                message_sender_str.remove(0,2);
        }


        QString date_str;
        NSString *NS_string_Date = [ns_dictionary_obj objectForKey:@"date"];
        QString date_time = QString::fromNSString(NS_string_Date);
        date_str = global_recon_helper_singular_class_obj->convert_epoch_to_readable_in_selected_timezone(global_recon_helper_singular_class_obj->convert_T_Z_time_to_epoch(date_time),Q_FUNC_INFO);

        QString message_sender = QString("<font size=\"3\" color=\"#000000\"> <b>") + message_sender_str + "</b> </font> <br>";
        QString message_str;
        id id_msg = [ns_dictionary_obj objectForKey:@"message"];
        if([id_msg isKindOfClass:[NSString class]])
        {
            QString temp_message_str;
            QString message_data_str = QString::fromNSString(id_msg);
            temp_message_str = QString("<font size=\"3\" color=\"#000000\"> <b>") + date_str + "<br>" + message_sender + "<br>"  + message_data_str + "</b> </font> <br>";

            if(message_sender_str.trimmed() == account_str.trimmed())
            {
                temp_message_str = chat_template_class::html_right_align(temp_message_str);
            }
            else
            {
                temp_message_str = chat_template_class::html_left_align(temp_message_str);
            }

            ichat_html_data_for_display_str += chat_template_class::html_table_row_data(temp_message_str);
            ichat_html_data_for_display_str += "<br>";
        }


        QString all_files_name_str, all_file_path_str,relative_file_path;
        QString attachment_file_detail = "";
        id id_file = [ns_dictionary_obj objectForKey:@"files"];
        if([id_file isKindOfClass:[NSArray class]])
        {
            for(id dict_file in id_file)
            {

                if([dict_file isKindOfClass:[NSDictionary class]])
                {
                    QString attchemnt_file_name;


                    if([[dict_file objectForKey:@"id"] isKindOfClass:[NSString class]])
                    {
                        NSString *p12 = [dict_file objectForKey:@"id"];
                        attchemnt_file_name = QString::fromNSString(p12);

                    }
                    if([[dict_file objectForKey:@"name"] isKindOfClass:[NSString class]])
                    {
                        NSString *p12 = [dict_file objectForKey:@"name"];
                        QString file_id_str = QString::fromNSString(p12);
                        QString attchment_dir_path = recon_helper_standard_obj->pub_get_absolute_dir_path_without_fileinfo(recon_helper_standard_obj->pub_get_absolute_dir_path_without_fileinfo(recon_helper_standard_obj->pub_get_absolute_dir_path_without_fileinfo(source_file)));
                        if(!attchment_dir_path.endsWith("/"))
                        {
                            attchment_dir_path.append("/");
                        }
                        attchment_dir_path.append("Attachments/");
                        if(QFileInfo(attchment_dir_path).exists())
                        {
                            QDirIterator iter(attchment_dir_path, QDirIterator::Subdirectories);
                            while(iter.hasNext())
                            {
                                QString main_file_link_str;
                                QFileInfo info(iter.next());
                                if(info.fileName() == file_id_str)
                                {
                                    QString data_str;
                                    QString attchment_info_data_str;
                                    QString attchmnt_file_path = (info.filePath() + "/" + attchemnt_file_name);
                                    if(!attchmnt_file_path.isEmpty())
                                    {
                                        if(recon_helper_standard_obj->check_is_it_image_file(attchmnt_file_path,attchmnt_file_path,Q_FUNC_INFO))
                                        {
                                            QImage img(attchmnt_file_path);

                                            QByteArray arr;
                                            QBuffer buf(&arr);
                                            img.save(&buf, "JPG" );

                                            QString base_64_data = QString::fromLocal8Bit(arr.toBase64());
                                            QString  source_image_data = QString("<img src=data:image/png;base64,") + base_64_data  + " width=\"400\" height=\"300\" >";

                                            QString hyperlink = QString("<a href = \""+ attchmnt_file_path + "\" Link "+ QString(" target=\"_blank\"") +" > " + attchemnt_file_name + "</a>") ;
                                            data_str = "<font size=\"3\" color=\"#000000\"> <b>" + date_str + "<br>" + message_sender + source_image_data + "<br>" + hyperlink +  + "</b> </font> <br>";

                                        }
                                        else
                                        {
                                            QFileInfo info(attchmnt_file_path);
                                            QString file_name = info.fileName();
                                            main_file_link_str = QString("<a href = \""+ attchmnt_file_path + "\" Link "+ QString(" target=\"_blank\"") +" > " + file_name + "</a>") ;
                                            data_str = "<font size=\"3\" color=\"#000000\"> <b>" + date_str + "<br>" + message_sender + main_file_link_str + "</b> </font> <br>";
                                        }
                                    }
                                    if(message_sender_str.trimmed() == account_str.trimmed())
                                    {
                                        attchment_info_data_str = chat_template_class::html_right_align(data_str);
                                    }
                                    else
                                    {
                                        attchment_info_data_str = chat_template_class::html_left_align(data_str);
                                    }

                                    ichat_html_data_for_display_str += chat_template_class::html_table_row_data(attchment_info_data_str);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    [pool drain];

    ichat_html_data_for_display_str += chat_template_class::chat_page_end();
    QWebEngineView *view = new QWebEngineView(ui->stackedWidget);
    view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(view,SIGNAL(customContextMenuRequested(const QPoint&)),SLOT(onCustomContextMenuRequested(const QPoint&)));
    view->setHtml(ichat_html_data_for_display_str);

    ui->stackedWidget->insertWidget(1,view);
    ui->stackedWidget->setCurrentIndex(1);

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
    return;
}


void ichat_parser::export_ichat_file_in_html_format(QFile &m_file)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);


    if(!source_file_path_ichat.endsWith(".ichat"))
        return;

    QString ichat_html_data_for_report_str = chat_template_class::chat_page_start();
    ichat_html_data_for_report_str += chat_template_class::html_table_start();

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSString *filePath = source_file_path_ichat.toNSString();
    [NSKeyedUnarchiver setClass:[InstantMessage class] forClassName:@"InstantMessage"];
    [NSKeyedUnarchiver setClass:[Presentity class] forClassName:@"Presentity"];

    [NSKeyedUnarchiver setClass:[StubCoder class] forClassName:@"NSFont"];
    [NSKeyedUnarchiver setClass:[StubCoder class] forClassName:@"NSMutableParagraphStyle"];
    [NSKeyedUnarchiver setClass:[StubCoder class] forClassName:@"NSTextAttachment"];
    [NSKeyedUnarchiver setClass:[StubCoder class] forClassName:@"NSColor"];

    NSData *data = [NSData dataWithContentsOfFile:filePath];
    NSKeyedUnarchiver* unarchiver = [[NSKeyedUnarchiver alloc] initForReadingWithData:data];
    NSArray* root = [unarchiver decodeObjectForKey:@"$root"];
    NSMutableArray *ns_mutable_array = [NSMutableArray array];
    NSMutableSet *people = [NSMutableSet set];


    QString account_str , participants_str ;
    for (id object in root)
    {
        if ([object isKindOfClass:[NSArray class]])
        {
            for (id sub in object)
            {

                if ([sub isKindOfClass:[Presentity class]])
                {
                    Presentity *prs = (Presentity *) sub;
                    [people addObject:prs.accountName];
                    account_str = QString::fromNSString(prs.accountName);
                }

                if ([sub isKindOfClass:[InstantMessage class]])
                {
                    InstantMessage *im = (InstantMessage *) sub;
                    if ([im subject] != nil)
                    {
                        [people addObject:im.subject.accountName];
                    }
                    if ([im sender] != nil)
                    {
                        [people addObject:im.sender.accountName];
                    }
                    [ns_mutable_array addObject:im];


                    QString part_str = QString::fromNSString(im.sender.accountName);
                    if(account_str != part_str)
                    {
                        participants_str = part_str;
                    }
                }

            }
        }
    }

    QString message_str;
    for (InstantMessage* instant_message_obj in ns_mutable_array)
    {
        if([people count] > 2)
        {
            [instant_message_obj setIsMultiParty:true];
        }

        [instant_message_obj setParticipantIds:people];
        [instant_message_obj setChatId: [root lastObject]];

        NSDictionary *ns_dictionary_obj = [instant_message_obj get_message_detail];


        QString message_sender_str;
        id id_sndr = [ns_dictionary_obj objectForKey:@"sender"];
        if([id_sndr isKindOfClass:[NSString class]])
        {
            message_sender_str = QString::fromNSString(id_sndr);
            if(message_sender_str.startsWith("e:"))
                message_sender_str.remove(0,2);
        }

        QString date_str;
        NSString *NS_string_Date = [ns_dictionary_obj objectForKey:@"date"];
        QString date_time = QString::fromNSString(NS_string_Date);
        date_str = global_recon_helper_singular_class_obj->convert_epoch_to_readable_in_selected_timezone(global_recon_helper_singular_class_obj->convert_T_Z_time_to_epoch(date_time),Q_FUNC_INFO);



        QString message_sender = QString("<font size=\"3\" color=\"#000000\"> <b>") + message_sender_str + "</b> </font> <br>";
        QString message_str;
        id id_msg = [ns_dictionary_obj objectForKey:@"message"];
        if([id_msg isKindOfClass:[NSString class]])
        {
            QString temp_message_str;
            QString message_data_str = QString::fromNSString(id_msg);
            temp_message_str = QString("<font size=\"3\" color=\"#000000\"> <b>") + date_str + "<br>" + message_sender + "<br>"  + message_data_str + "</b> </font> <br>";
            if(message_sender_str.trimmed() == account_str.trimmed())
            {
                temp_message_str = chat_template_class::html_right_align(temp_message_str);
            }
            else
            {
                temp_message_str = chat_template_class::html_left_align(temp_message_str);
            }

            ichat_html_data_for_report_str.append(chat_template_class::html_table_row_data(temp_message_str));
            ichat_html_data_for_report_str.append("<br>");
        }

        QString all_files_name_str, all_file_path_str,relative_file_path;
        QString attachment_file_detail = "";
        QString main_file_link_str;
        id id_file = [ns_dictionary_obj objectForKey:@"files"];
        if([id_file isKindOfClass:[NSArray class]])
        {
            for(id dict_file in id_file)
            {
                if([dict_file isKindOfClass:[NSDictionary class]])
                {
                    QString attchmnt_file_path,attchment_file_name;
                    if([[dict_file objectForKey:@"id"] isKindOfClass:[NSString class]])
                    {
                        NSString *p12 = [dict_file objectForKey:@"id"];
                        attchment_file_name = QString::fromNSString(p12);

                    }
                    if([[dict_file objectForKey:@"name"] isKindOfClass:[NSString class]])
                    {
                        NSString *p12 = [dict_file objectForKey:@"name"];
                        QString file_id_str = QString::fromNSString(p12);
                        QString attchment_dir_path = recon_helper_standard_obj->pub_get_absolute_dir_path_without_fileinfo(recon_helper_standard_obj->pub_get_absolute_dir_path_without_fileinfo(recon_helper_standard_obj->pub_get_absolute_dir_path_without_fileinfo(source_file_path_ichat)));
                        if(!attchment_dir_path.endsWith("/"))
                        {
                            attchment_dir_path.append("/");
                        }
                        attchment_dir_path.append("Attachments/");
                        if(QFileInfo(attchment_dir_path).exists())
                        {
                            QDirIterator iter(attchment_dir_path, QDirIterator::Subdirectories);
                            while(iter.hasNext())
                            {
                                QFileInfo info(iter.next());
                                if(info.fileName() == file_id_str)
                                {
                                    QString data_str;
                                    QString report_data_str;
                                    attchmnt_file_path = (info.filePath() + "/" + attchment_file_name);
                                    if(!attchmnt_file_path.isEmpty())
                                    {

                                        //start if more than one file with same name
                                        QString  new_name = recon_static_functions::get_available_stringname(attchment_file_name, file_name_list, Q_FUNC_INFO);
                                        QString dest_file_path = exported_files_dir + "/" + new_name;
                                        //end


                                        QString exported_attchmnt_file_path = export_rsync_obj->pub_copy_file_using_rsync(attchmnt_file_path,dest_file_path);
                                        exported_attchmnt_file_path =  recon_helper_standard_obj->pub_get_html_formated_filename(exported_attchmnt_file_path,Q_FUNC_INFO);

                                        QFileInfo exported_file_info(exported_attchmnt_file_path);
                                        QString exported_file_name = exported_file_info.fileName();
                                        file_name_list <<  exported_file_name;


                                        QString relative_file_path = "./files/" + exported_file_name;
                                        QString display_file_nm_str = attchment_file_name;
                                        if(recon_helper_standard_obj->check_is_it_image_file(attchmnt_file_path,attchmnt_file_path,Q_FUNC_INFO))
                                        {
                                            QImage img(attchmnt_file_path);

                                            QByteArray arr;
                                            QBuffer buf(&arr);
                                            img.save(&buf, "JPG" );

                                            QString base_64_data = QString::fromLocal8Bit(arr.toBase64());
                                            QString  source_image_data = QString("<img src=data:image/png;base64,") + base_64_data  + " width=\"400\" height=\"300\" >";
                                            QString hyperlink = QString("<a href=") +  relative_file_path + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + relative_file_path + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10 scrollbars=yes');return false\">");
                                            data_str = "<font size=\"3\" color=\"#000000\"> <b>" + date_str + "<br>" + message_sender + hyperlink + source_image_data + "</b> </font> <br>";

                                        }
                                        else
                                        {
                                            main_file_link_str = QString("<a href = \""+ relative_file_path + "\" Link "+ QString(" target=\"_blank\"") +" > " + display_file_nm_str + "</a>") ;
                                            data_str = "<font size=\"3\" color=\"#000000\"> <b>" + date_str + "<br>" + message_sender + main_file_link_str + "</b> </font> <br>";
                                        }
                                    }

                                    if(message_sender_str.trimmed() == account_str.trimmed())
                                    {
                                        report_data_str = chat_template_class::html_right_align(data_str);
                                    }
                                    else
                                    {
                                        report_data_str = chat_template_class::html_left_align(data_str);
                                    }

                                    ichat_html_data_for_report_str.append(chat_template_class::html_table_row_data(report_data_str));
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    ichat_html_data_for_report_str += chat_template_class::html_table_end();
    ichat_html_data_for_report_str += chat_template_class::chat_page_end();


    m_file.write(ichat_html_data_for_report_str.toLocal8Bit());

    [pool drain];
    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
    return;
}
