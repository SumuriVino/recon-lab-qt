#include "story_board_loader.h"

QString story_board_loader::create_html_table_start()
{
    QString data = QString("<html>")
            + "<head>"
            + "<link rel=\"stylesheet\" href=\"./resources/style.css\" type=\"text/css\" />"
            + "</head>"

            + "<body>"
            + "<div id = \"page\"> <div id = \"content\">"
            + "<table width = \"98%\" border=\"1\" align=\"centre\" cellpadding=\"1\" cellspacing=\"1\" style=\"border-bottom: 0;border-top: 0;border-left: 0;border-right: 0;\">";


    return data;
}


QString story_board_loader::create_html_table_startfor_record()
{
    QString data = QString("<html>")
            + "<head>"
            + "<link rel=\"stylesheet\" href=\"./resources/style.css\" type=\"text/css\" />"
            + "</head>"

            + "<body>"
            + "<div id = \"page\"> <div id = \"content\">"
            + "<table width = \"98%\" border=\"0\" align=\"centre\" cellpadding=\"1\" cellspacing=\"1\" style=\"border-bottom: 0;border-top: 0;border-left: 0;border-right: 0;\">";


    return data;
}


QString story_board_loader::create_html_table_header(QStringList header_list)
{
    QString data = "<tr>";

    for(int h = 0; h < header_list.size(); h++)
    {
        if((header_list.size() == 1) )
        {
            data += "<th width=\"100%\">" + header_list.at(h) + "</th>";
            continue;
        }

        if((header_list.size() > 1) && (h == (header_list.size() - 1)))
            data += "<th width=\"20%\">" + header_list.at(h) + "</th>";
        else
            data += "<th width=\"80%\">" + header_list.at(h) + "</th>";
    }
    data += "</tr>";

    return data;
}


QString story_board_loader::create_html_table_rows_data_for_general_files(QStringList column_data_list , QString base_64_data)
{

    QString data = "<tr>";
    for(int col = 0; col < column_data_list.size(); col++)
    {
        if(col == 0)
        {
            data += "<td class = \"wrapped\">" +  column_data_list.at(col) + "</td>";
        }
        else if(col == 1)
        {

            //QString file_name = column_data_list.at(col);
            QString file_name = QFileInfo(column_data_list.at(col)).fileName();
            if(file_name.isEmpty())
                continue;

            //file_name.replace(" ", "%20");
            QString formatted_in_std_html_str = recon_helper_standard_obj->pub_get_html_formated_filename(file_name,Q_FUNC_INFO);
            QString relative_path = "./Resources/" + formatted_in_std_html_str;

            if(recon_helper_standard_obj->check_is_it_image_file(column_data_list.at(col),file_name,Q_FUNC_INFO))
            {
                QString image = QString("<img src=data:image/png;base64,") + base_64_data  + " width=\"140\" height=\"150\" >";
                //QString hyperlink = QString("<a href=") +  file_name + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + file_name.remove(0, QString("./Resources").size()) + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10 scrollbars=yes');return false\">");
                QString hyperlink = QString("<a href=") +  relative_path + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + relative_path + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10 scrollbars=yes');return false\">");

                data += QString("<td align=\"center\">") +  QString(hyperlink + image) + QString("</td>");
            }
            else
            {
                //data += QString("<td align=\"center\"><a href=") +   file_name + "  target=\"_blank\" >" + file_name.remove(0, QString("./Resources").size()) + "</a></td>";
                data += QString("<td align=\"center\"><a href=") +   relative_path + "  target=\"_blank\" >" + file_name + "</a></td>";
            }

        }
    }
    data += "</tr>";

    return data;
}

QString story_board_loader::create_html_table_rows_data_for_for_multiple_files(QStringList column_data_list ,QStringList display_file_path_list)
{
    recon_static_functions::app_debug(" -Start"  , Q_FUNC_INFO);

    QString data = "<tr>";
    for(int col = 0; col < column_data_list.size(); col++)
    {
        if(col == 0)
        {
            QString metadata = column_data_list.at(col);
            data += "<td class = \"wrapped\">" +  metadata + "</td>";
        }
        else if(col == 1)
        {

            QString all_file_path_str = column_data_list.at(col);
            //            if(all_file_path_str.isEmpty())
            //                continue;
            if(all_file_path_str.isEmpty())
            {
                data += "<td>";
                data += QString("<b> <font color = \"red\">" + QObject::tr("No File Available") + "</font></b> <br><br>");
                data += "</td>";
                continue;
            }

            QStringList all_file_list = all_file_path_str.split(MACRO_RECON_Splitter_1_multiple);

            data += "<td>";
            for(int pp = 0 ; pp < all_file_list.size() ; pp++)
            {
                QString complete_file_path = all_file_list.at(pp);
                QString display_file_path = display_file_path_list.at(pp);
                if(!QFileInfo(complete_file_path).exists())
                {
                    data += QString("<b> <font color = \"red\">" + QObject::tr(" File does not exist::")+ "</font></b>"+"<br>" +display_file_path + "</font></b> <br><br>");
                }
                else
                {
                    QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";
                    QString filename = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(display_file_path);
                    export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(complete_file_path,resource_dir_path,filename,Q_FUNC_INFO);

                    //QString relative_path = "./Resources/" + filename;
                    QString relative_path = "./Resources/" + recon_helper_standard_obj->pub_get_html_formated_filename(filename,Q_FUNC_INFO);
                    if(recon_helper_standard_obj->check_is_it_image_file(complete_file_path,display_file_path,Q_FUNC_INFO))
                    {
                        QString  base_64_data = get_image_hex_data(complete_file_path);
                        QString image = QString("<img src=data:image/png;base64,") + base_64_data  + " width=\"140\" height=\"150\" >";
                        QString hyperlink = QString("<a href=") +  relative_path  + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + relative_path + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");
                        data += QString(hyperlink + image) + "</a>";

                        data += QString("<br> <br> <br>");
                    }
                    else
                    {
                        data += QString("<a href=") +   relative_path + "  target=\"_blank\" >" + filename + "</a><br> <br> <br>";
                    }
                }
            }
            data += "</td>";
        }
    }
    data += "</tr>";

    recon_static_functions::app_debug(" -End"  , Q_FUNC_INFO);
    return data;
}

QString story_board_loader::create_html_table_rows_data_for_our_created_files(QString metadata_str , QString map_filepath, QString filepath , QString plugin_name_str)
{

    QString data = "<tr>";

    map_filepath = map_filepath.trimmed();
    filepath = filepath.trimmed();



    if(plugin_name_str == MACRO_Plugin_Name_Saved_Maps)
    {

        if(QFileInfo(map_filepath).exists())
        {
            QString raw_data_1 = get_image_hex_data_for_maps(map_filepath);
            QString file_name_1 =  QFileInfo(map_filepath).fileName();

            QString relative_path = "./Resources/" + recon_helper_standard_obj->pub_get_html_formated_filename(file_name_1,Q_FUNC_INFO);
            metadata_str += "<tr>";
            QString link_click1 = QString("<a href=")  + relative_path + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + file_name_1 + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10 scrollbars=yes');return false\">");

            metadata_str += "<td class = \"wrapped\">" + link_click1 +  QString("<img src=data:image/png;base64,") + raw_data_1 + ">"  + "</td>";
        }
        if(QFileInfo(filepath).exists())
        {
            QString raw_data_2 = get_image_hex_data_for_maps(filepath);
            QString file_name_2 =  QFileInfo(filepath).fileName();

            QString relative_path_2 = "./Resources/" + recon_helper_standard_obj->pub_get_html_formated_filename(file_name_2,Q_FUNC_INFO);

            metadata_str += "<tr>";
            QString link_click2 = QString("<a href=")  + relative_path_2 + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + file_name_2 + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10 scrollbars=yes');return false\">");
            metadata_str += "<td class = \"wrapped\">" + link_click2 +  QString("<img src=data:image/png;base64,") + raw_data_2 + ">"  + "</td>";

        }
    }
    else
    {
        QString file_name_3 =  QFileInfo(filepath).fileName();
        QString raw_data_3 = get_image_hex_data_for_maps(filepath);
        QString relative_path_3 = "./Resources/" + recon_helper_standard_obj->pub_get_html_formated_filename(file_name_3,Q_FUNC_INFO);

        metadata_str += "<tr>";
        QString link_click = QString("<a href=")  + relative_path_3 + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + file_name_3 + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10 scrollbars=yes');return false\">");
        metadata_str += "<td class = \"wrapped\">" + link_click +  QString("<img src=data:image/png;base64,") + raw_data_3 + ">"  + "</td>";
    }


    data += "<td class = \"wrapped\">" +  metadata_str + "</td>";

    data += "</tr>";

    return data;







    //=======================Older than 13 Aug=============///

    //    QString data = "<tr>";

    //    map_filepath = map_filepath.trimmed();
    //    filepath = filepath.trimmed();

    //    if(!map_filepath.isEmpty())
    //    {
    //        QString image_hex_data = get_image_hex_data_for_maps(map_filepath);

    //        QString file_name =  QFileInfo(map_filepath).fileName();
    //        file_name.replace(" ", "%20");

    //        QString link_click = QString("<a href=")  + "./Resources/" + file_name + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + file_name + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10 scrollbars=yes');return false\">");

    //        QString hyperlink_blue = QString("<a href='") + "./Resources/" + file_name + QString("' target=\"_blank\"> ") + file_name + QString("</a>");

    //        metadata_str += "<tr>";
    //        metadata_str += "<td class = \"wrapped\">"  + link_click +  QString("<img src=data:image/png;base64,")  +  image_hex_data + ">"  + "</td>" + hyperlink_blue;
    //    }

    //    if(!filepath.isEmpty())
    //    {
    //        QString image_hex_data = get_image_hex_data_for_maps(filepath);

    //        if(map_filepath.isEmpty())
    //        {
    //            QString file_name =  QFileInfo(filepath).fileName();

    //            QString hyperlink = QString("<a href=")  + "./Resources/" + file_name  + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + file_name + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10 scrollbars=yes');return false\">");
    //            QString hyperlink_blue = QString("<a href='") + "./Resources/" + file_name + QString("' target=\"_blank\"> ") + file_name + QString("</a>");

    //            metadata_str += "<tr>";
    //            metadata_str += "<td class = \"wrapped\">"  + hyperlink +  QString("<img src=data:image/png;base64,") + image_hex_data + ">"  + "</td>" + hyperlink_blue;
    //        }
    //        else
    //        {
    //            metadata_str += "<tr>";
    //            metadata_str += "<td class = \"wrapped\">"  +  QString("<img src=data:image/png;base64,") + image_hex_data + ">"  + "</td>";
    //        }
    //    }

    //    data += "<td class = \"wrapped\">" +  metadata_str + "</td>";

    //    data += "</tr>";

    //    return data;
}

QString story_board_loader::create_html_table_end()
{
    QString data = QString("</table>")    //-Table End
            +"</div></div>"
            + "</body>"
            + "</html>";
    return data;
}


QByteArray story_board_loader::get_image_hex_data_for_maps(QString filepath)
{
    QByteArray data_arr;

    if(filepath.trimmed().isEmpty())
        return data_arr;

    QImage img(filepath);

    img = img.scaled(500, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if(!bool_checkbox_blurr_status)
    {
        QByteArray arr;
        QBuffer buf(&arr);
        img.save(&buf, "PNG" );
        data_arr = arr.toBase64();
        return data_arr;
    }
    else
    {

        QGraphicsBlurEffect *effect = new QGraphicsBlurEffect;

        effect->setBlurRadius(8);
        int extent = 0;

        QGraphicsScene scene;
        QGraphicsPixmapItem item;
        item.setPixmap(QPixmap::fromImage(img));
        item.setGraphicsEffect(effect);
        scene.addItem(&item);
        QImage res(img.size()+QSize(extent*2, extent*2), QImage::Format_ARGB32);
        res.fill(Qt::transparent);
        QPainter ptr(&res);
        scene.render(&ptr, QRectF(), QRectF(-extent, -extent, img.width()+extent*2, img.height()+extent*2));

        QByteArray arr;
        QBuffer buf(&arr);
        res.save(&buf, "JPG" );
        data_arr = arr.toBase64();

        delete effect;

        return data_arr;
    }

}

