#include "report_template_html_advance.h"

report_template_html_advance::report_template_html_advance(QObject *parent)
{

}

report_template_html_advance::~report_template_html_advance()
{

}


void report_template_html_advance::set_essential()
{
    clear_fields();

    fill_variable_from_narad_muni();
}

void report_template_html_advance::fill_variable_from_narad_muni()
{

}

void report_template_html_advance::clear_fields()
{

}

QString report_template_html_advance::report_set_html_header()
{
    QString report_data;
    report_data = QString("<!DOCTYPE html>"
                          "<html lang=\"en\">"
                          "<head>"
                          "<meta charset=\"utf-8\">"
                          "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">"
                          "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                          "<!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->"
                          "<meta name=\"description\" content=\"\">"
                          "<meta name=\"author\" content=\"\">"
                          "<link rel=\"icon\" href=\"../../resources/favicon.ico\">"

                          "<title>Advance HTML Report</title>"

                          "<!-- Bootstrap core CSS -->"
                          "<!--  <link href=\"../../resources/css/bootstrap.min.css\" rel=\"stylesheet\"> -->"
                          "<link href=\"../../resources/css/bootstrap.min.css\" rel=\"stylesheet\">"

                          "<!--To remove below file -->"
                          "<link href=\"../../resources/css/bootstrap.css\" rel=\"stylesheet\">"

                          "<!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->"
                          "<link href=\"../../resources/css/ie10-viewport-bug-workaround.css\" rel=\"stylesheet\">"

                          "<!-- Custom styles for this template -->"
                          "<link href=\"../../resources/css/dashboard.css\" rel=\"stylesheet\">"

                          "</head>"

                          "<body> <font color=\"#000000\">"


                          "<!-- START-1 .Below Code is for the Black Topbar with company name and navigaion-->"
                          "<nav class=\"navbar navbar-inverse navbar-fixed-top  navbar-xs\">"
                          "<div class=\"container-fluid\">"
                          "<div class=\"navbar-header\">"
                          "<ul class=\"nav navbar-nav\">");

    return report_data;

}

QString report_template_html_advance::report_drop_down_plugin_list(QStringList plugins_info_list)
{

    ///- Up to first Navigation bar compete
    QString temp_data;
    temp_data = QString( "<li class=\"dropdown\">"
                         "<a href=\"#\" class=\"dropdown-toggle \" data-toggle=\"dropdown\" role=\"button\" aria-haspopup=\"true\" aria-expanded=\"false\"><font size=\"3\"><b>Plugins </b> </font><span class=\"caret\"></span></a>"
                         "<ul class=\"dropdown-menu scrollable-menu \">");


    for(int i = 0 ; i < plugins_info_list.size() ; i++)
    {
        QString value_str = plugins_info_list.at(i);

        QStringList tmp_list = value_str.split(MACRO_RECON_Splitter_1_multiple);

        if(tmp_list.size() < 2)
            continue;

        QString plugin_name = tmp_list.at(0);
        QStringList selected_tab_names = recon_static_functions::get_associated_tabs(plugin_name,QStringList(value_str),Q_FUNC_INFO);

        QString tmp_first_tab_nm;

        if(selected_tab_names.size() >= 0)
            tmp_first_tab_nm = selected_tab_names.at(0);

        QString tmp_plugin_nm = plugin_name;
        tmp_plugin_nm.replace(" ","_");

        temp_data += "<li><a href=\"../"+ tmp_plugin_nm + "/" + tmp_first_tab_nm.replace(" ","_") + "0.html\"" + "><img src=\"../../resources/plugins/" + tmp_plugin_nm + ".png\" style=\"border: 0px solid #000; max-width:18px; max-height:18px;\">" +" "+ plugin_name + "</a></li>";

    }

    temp_data += QString( "</ul>"
                          "</li>");


    return temp_data;

}

QString report_template_html_advance::report_set_plugin_name_and_icon_on_no_data_available(QString plugin_name, QString parent_category)
{
    QString icon_path = QString("../../resources/plugins/") +  plugin_name.replace(" ","_") + ".png";

    plugin_name.replace("_"," ");

    QString report_data = "<br /><a name=\"";
    report_data += plugin_name + "\"> </a>  <div> <h4 class=\"caption\" id=\"\"><span class=\" s14\" id=\"\">";
    report_data += plugin_name;

    if(!parent_category.isEmpty())
        report_data += " (<font size = \"3\" >" + parent_category + "</font>)";

    report_data += "</span>  </h4> <img src = " + QString("\"") + icon_path + "\"";
    report_data += "width=\"60\" height=\"60\" /><br /> </div>";

    report_data += QString("<\div> <font color=\"red\">" +  QObject::tr("No Data Available") + "</font></span></div></padding-left:1cm >");
    return report_data;
}

QString report_template_html_advance::report_set_agency_name_and_logo()
{
    QString report_data = QString(

                ///- Below line is done to create space before HOme icon button.

                "<li class=\"about\" ><a></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>"

                "<li class=\"about\"><a href=\"../../index.html\"><img src=\"../../resources/home_green.png\" style=\"border: 0px solid #000; max-width:38px; max-height:38px;\"> </a></li>"

                "</ul>"
                "</div>"

                "<div class=\"navbar-collapse collapse pull-right \">"
                "<ul class=\"nav navbar-nav\">"

                "<li class=\"about\"><a class=\"navbar-brand\"><b>" + recon_static_functions::get_app_name()  + "</b></a></li>"

                                                                                                                        "</ul>"
                                                                                                                        "</div><!--/.nav-collapse -->"
                                                                                                                        "</div>"
                                                                                                                        "</nav><br>"
                                                                                                                        "<!-- END-1 .Black Topbar END-->");
    return report_data;


}

QString report_template_html_advance::report_set_plugin_name_and_icon(QString plugin_name,QString parent_category)
{

    QString temp_data;



    QString image_name = plugin_name;
    image_name.replace(" ", "_");

    temp_data = QString("<!--SECTION FOR THE PLUGIN ICON , NAME, CATEGORY, and ARTIFACTS LIST [START]  color:#262626;text-decoration:none;background-color:#f5f5f5-->"

                        "<div class=\"row placeholders\" style=\"padding-left:1cm\">"
                        "<div class=\"col-md-2 col-md-1 placeholder\">"
                        "<img src=\"../../resources/plugins/" + QString(image_name + ".png").replace(" ","_") +"\" width=\"100\" height=\"100\" class=\"img-responsive\" alt=\"Generic placeholder thumbnail\">"
                                                                                                               "<h4>" + QString(plugin_name) + "</h4>"
                                                                                                                                               "<span class=\"text-muted\">" + QString(parent_category) + "</span>"
                                                                                                                                                                                                          "</div>");

    return temp_data;
}

QString report_template_html_advance::report_set_artifacts_source(QStringList list_artifacts_source)
{
    QString temp_data;

    temp_data = QString("<div class=\"container pull-left\">");


    //    if(!list_artifacts_source.isEmpty())
    //    {
    //        temp_data +=  "<h4 class=\"text-left\">Artifacts Source</h4><br>";
    //    }

    if(!list_artifacts_source.isEmpty())
    {
        temp_data +=  "<h4 class=\"text-left\"> </h4><br>";
    }


    for(int i =0; i < list_artifacts_source.size(); i++)
    {
        temp_data += "<p class=\"text-left\">" + QString("") + "</p>";
    }

    temp_data += QString("</div>"
                         "</div>");

    return temp_data;

}

QString report_template_html_advance::report_set_category_name_in_subnavigation_bar(QString category_name)
{

    QString temp_data;
    temp_data = QString("<nav class=\"navbar navbar-inverse navbar-df\">"
                        " <div class=\"container-fluid\">"
                        "<div class=\"navbar-header\">"
                        "<button type=\"button\" class=\"navbar-toggle collapsed\" data-toggle=\"collapse\" data-target=\"#navbar\" aria-expanded=\"false\" aria-controls=\"navbar\">"
                        "<span class=\"sr-only\">Toggle navigation</span>"
                        "<span class=\"icon-bar\"></span>"
                        "<span class=\"icon-bar\"></span>"
                        "<span class=\"icon-bar\"></span>"
                        "</button>"
                        "<a class=\"navbar-brand\"><font size=\"4\">" + category_name + "</font></a>"
                                                                                        " </div>"
                        ///-- Start Section for the Drop down button and next previous buttons
                        "<div id=\"navbar\" class=\"navbar-collapse collapse\">"
                        "<ul >"
                        );



    return temp_data;
}

QString report_template_html_advance::report_set_next_previous_pager_buttons(QString previous, QString next, QString first, QString last)
{
    QString temp_data;
    temp_data = QString("<div class=\"pager  pull-right \">");

    //-Very First Page
    temp_data += QString("<li class=\"previous \" data-toggle=\"tooltip\" data-placement=\"top\" title=\"First Page\"><a href=\"" + first +"\"><img src=\"../../resources/first_green.png\" style=\"border: 0px solid #000; max-width:30px; max-height:30px;\"> </a></li>");

    //-For previous page from current
    if(previous == "")
        temp_data += QString("<li class=\"previous disabled\"  data-toggle=\"tooltip\" data-placement=\"top\" title=\"Previous Page\"><a href=\"" + previous +"\"> <img src=\"../../resources/stop_previous.png\" style=\"border: 0px solid #000; max-width:31px; max-height:31px;\">  </a></li>");
    else
        temp_data += QString("<li class=\"previous\"  data-toggle=\"tooltip\" data-placement=\"top\" title=\"Previous Page\"><a href=\"" + previous +"\"> <img src=\"../../resources/previous_green.png\" style=\"border: 0px solid #000; max-width:30px; max-height:30px;\">  </a></li>");


    //-For Last Page
    temp_data += QString("<li class=\"next \" data-toggle=\"tooltip\" data-placement=\"top\" title=\"Last Page\"><a href=\"" + last +"\"> <img src=\"../../resources/last_green.png\" style=\"border: 0px solid #000; max-width:30px; max-height:30px;\"></a></li>");


    //- For next from current
    if(next == "")
        temp_data += QString("<li class=\"next disabled\" data-toggle=\"tooltip\" data-placement=\"top\" title=\"Next Page\"><a href=\"" + next +"\">  <img src=\"../../resources/stop_next.png\" style=\"border: 0px solid #000; max-width:31px; max-height:31px;\"></a></li>");
    else
        temp_data += QString("<li class=\"next \" data-toggle=\"tooltip\" data-placement=\"top\" title=\"Next Page\"><a href=\"" + next +"\">  <img src=\"../../resources/next_green.png\" style=\"border: 0px solid #000; max-width:30px; max-height:30px;\"> </a></li>");


    temp_data += QString("</div> "

                         " </ul>"
                         "</div><!--/.nav-collapse -->");

    return temp_data;
}

QString report_template_html_advance::report_set_category_dropdownlist(QStringList category_list, QString current_pg_of_total_pg)
{



    ///- Dropdown list independent section
    QString temp_data;
    temp_data = QString(    "<div class=\"nav navbar-nav  pull-right  right-padding\">"
                            //    temp_data = QString(    "</n><div class=\"nav navbar-nav  pull-right \">"
                            "<li><a style=\"border: 0px solid #000;  min-width:130px;\">" + current_pg_of_total_pg + "</a></li>"


                                                                                                                     "<li class=\"dropdown\">"
                                                                                                                     " <a href=\"#\" class=\"dropdown-toggle\"  data-toggle=\"dropdown\" role=\"button\" aria-haspopup=\"true\" aria-expanded=\"false\"><b>Categories </b><span class=\"caret\"></span></a>"
                                                                                                                     "<ul class=\"dropdown-menu\">");


    for(int i_lst = 0; i_lst < category_list.size(); i_lst++)
    {
        QString tmp_nm = category_list.at(i_lst);

        temp_data += QString("<li><a href=\"" + QString(tmp_nm +"0.html").replace(" ","_") + "\">" + tmp_nm + "</a></li>");

    }


    temp_data += QString("</ul>"
                         "</li>"
                         "<li>"
                         "</li>"
                         "</div>");

    return temp_data;
}

QString report_template_html_advance::report_set_table_header(QStringList table_header_list , QStringList coloum_width_list)
{
    QString temp_data;

    ///-Navigation closed of the sub navigation bar
    temp_data = QString("</div>"
                        "</nav>");

    ///- Table work start
    temp_data += QString("<div class=\"container-fluid\">"
                         "<div class=\"table-responsive scroll\">"
                         "<table class=\"table  table-hover table-bordered table-condensed table-striped \" width = \"100%\" >"
                         "<thead >"
                         "<tr>"
                         );

    for(int i = 0; i < table_header_list.size(); i++)
    {


        if(table_header_list.size() > 3)
        {
            if(i==1)
            {///-2nd Coloum
                temp_data += QString("<th width= 5%px>" + table_header_list.at(i) + "</th>" );
            }
            else if(i==2)
            {///- 3rd coloum

                if(table_header_list.at(i) == "System Account")
                    temp_data += QString("<th width= 10%px>" + table_header_list.at(i) + "</th>" );
                else
                    temp_data += QString("<th width="+ coloum_width_list.at(i)+"px>" + table_header_list.at(i) + "</th>" );

            }
            else if(i== (table_header_list.size() -1))
            { ///- Last Coloum

                if(table_header_list.at(i) == "Notes")
                {///-Last coloum is NOtes
                    temp_data += QString("<th width= 15%px>" + table_header_list.at(i) + "</th>" );
                }
                else
                {///-Last coloum is preview
                    temp_data += QString("<th width= 11%px>" + table_header_list.at(i) + "</th>" );
                }

            }
            else
                temp_data += QString("<th width="+ coloum_width_list.at(i)+"px>" + table_header_list.at(i) + "</th>" );

        }
        else if(table_header_list.size() == 3)
        {///- 3 coloum report for export tables
            if(i== table_header_list.size() -1)
                temp_data += QString("<th width= 11%px>" + table_header_list.at(i) + "</th>" );
            else
                temp_data += QString("<th width="+ coloum_width_list.at(i)+"px>" + table_header_list.at(i) + "</th>" );
        }
        else
            temp_data += QString("<th width="+ coloum_width_list.at(i)+"px>" + table_header_list.at(i) + "</th>" );


    }

    temp_data += QString("</tr>"
                         "</thead>"
                         "<tbody>"
                         "<tr>");
    return temp_data;
}

QString report_template_html_advance::report_insert_table_data(QStringList table_data_list, int report_preview)
{
    QString temp_data;

    for(int i = 0; i < table_data_list.size(); i++)
    {
        if( i == (table_data_list.size() - 1) && (report_preview == 1))
        {
            temp_data += "<td align=\"center\">" + table_data_list.at(i) + "</td>";
        }
        else if( i == (table_data_list.size() - 1) && (report_preview == QString(MACRO_CSV_TEMPLATE_DATATYPE_VIRTUAL_EXPORT_IMAGE_PATH_QString).toInt()))
        {
            temp_data += "<td align=\"center\">" + table_data_list.at(i) + "</td>";
        }
        else
        {
            temp_data +=  QString("<td class=\"table_data-break\">" + table_data_list.at(i) + "</td>");
        }
    }

    temp_data += QString("<tr>");
    table_data_list.clear();

    return temp_data;
}

QString report_template_html_advance::report_main_set_tool_agency_name()
{
    QString report_data = QString("<body> <font color=\"#000000\">"
                                  "<!-- START-1 .Below Code is for the Black Topbar with company name and navigaion-->"

                                  "<nav class=\"navbar navbar-inverse navbar-fixed-top  navbar-xs\">"
                                  "<div class=\"container-fluid\">"
                                  " <div class=\"navbar-header\">"
                                  " <ul class=\"nav navbar-nav\">"

                                  "<li class=\"about\"><a class=\"navbar-brand\"><b><img src=\"./resources/Report_Plugin.png\" style=\"border: 0px solid #000; max-width:24px; max-height:24px;\"> Contents </b></a></li>"
                                  ///- Below line is done to create space before HOme icon button.
                                  " <li class=\"about\"><a href=\"#\"></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>"

                                  "<li class=\"about\"><a href=\"index.html\"><img src=\"./resources/home_green.png\" style=\"border: 0px solid #000; max-width:38px; max-height:38px;\"> </a></a></li>"

                                  "</ul>"
                                  "</div>"

                                  "<div class=\"navbar-collapse collapse pull-right \">"
                                  "<ul class=\"nav navbar-nav\">");


    report_data += "<li class=\"about\"><a class=\"navbar-brand\"><b>" + recon_static_functions::get_app_name()  + "</b></a></li>";

    report_data +=      ("</ul>"
                         "</div><!--/.nav-collapse -->"
                         "</div>"
                         "</nav>");


    return report_data;

}

QString report_template_html_advance::report_main_set_sidebar_plugins_list(QStringList m_selected_plugins_info)
{

    QString report_data = QString("<div class=\"container-fluid\">"
                                  "<div class=\"row row-offcanvas row-offcanvas-left\">"
                                  "<!-- Sidebar -->"
                                  "<div id=\"sidebar-wrapper\">"
                                  "<ul class=\"sidebar-nav sidebar-recon\">");


    for(int i = 0; i < m_selected_plugins_info.size(); i++)
    {
        QString single_plugin_info = m_selected_plugins_info.at(i);
        QStringList tmp_list = single_plugin_info.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

        if(tmp_list.size() < 2)
            continue;

        QString plugin_name = tmp_list.at(0);

        QStringList selected_tablist = recon_static_functions::get_associated_tabs(plugin_name,QStringList(single_plugin_info),Q_FUNC_INFO);

        plugin_name = plugin_name.replace(" ","_");
        if(selected_tablist.size() > 0)
        {
            QString first_tab_nm = selected_tablist.at(0);
            report_data += "<li><a href=\"Plugins/"+ plugin_name + "/" + first_tab_nm.replace(" ","_") + "0.html\"" + "><img src=\"./resources/plugins/" + plugin_name + ".png\" style=\"border: 0px solid #000; max-width:22px; max-height:22px;\">" + "&nbsp;&nbsp;&nbsp;"+ QString(plugin_name).replace("_"," ") + "</a></li>";
        }
    }

    report_data += "</ul>"
                   "</div>"
                   "</div>";

    return report_data;

}

QString report_template_html_advance::report_main_set_agency_logo_and_name(QString logo_path, QString agency_name, QString tag_line)
{
    QString report_data = QString("<div class=\"col-sm-9 col-sm-offset-3 col-md-10 col-md-offset-2 main\">"

                                  "<div class=\"row \">"
                                  "<div class=\"col-sm-2 placeholders\">"

                                  "<img src=\"./resources/agency_logo.png\" class=\"img-thumbnail\" alt=\"Agency Logo\" width=\"150\" height=\"150\">"
                                  "<h5>" + agency_name + "</h5> </div>"
                                  "</div>"
                                  );

    return report_data;
}

QString report_template_html_advance::report_main_end()
{
    QString report_data = QString(
                "<script>window.jQuery || document.write('<script src=\"./resources/vendor/jquery.min.js\"></\\script>')</script>"
                "<script src=\"./resources/js/bootstrap.min.js\"></script>"
                "<!-- Just to make our placeholder images work. Don't actually copy the next line! -->"
                "<script src=\"./resources/vendor/holder.min.js\"></script>"
                "<!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->"
                "<script src=\"./resources/js/ie10-viewport-bug-workaround.js\"></script>"

                "</font>"
                "</body>"
                "</html>"

                );

    return report_data;
}

QString report_template_html_advance::report_end()
{
    QString temp_data;
    temp_data = QString("</tbody>"
                        "</table>"

                        "</div>"
                        "</div>"

                        "<script>window.jQuery || document.write('<script src=\"../../resources/vendor/jquery.min.js\"></\\script>')</script>"
                        "<script src=\"../../resources/js/bootstrap.min.js\"></script>"
                        "<!-- Just to make our placeholder images work. Don't actually copy the next line! -->"
                        "<script src=\"../../resources/vendor/holder.min.js\"></script>"
                        "<!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->"
                        "<script src=\"../../resources/js/ie10-viewport-bug-workaround.js\"></script>"
                        //                        "<script src=\"../../resources/js/tooltip.js\"></script>"



                        "</font>"
                        "</body>"
                        "</html>");

    return temp_data;
}

QString report_template_html_advance::report_main_set_html_header()
{
    QString report_data = QString("<!DOCTYPE html>"
                                  "<html lang=\"en\">"
                                  "<head>"
                                  "<meta charset=\"utf-8\">"
                                  "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">"
                                  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                                  "<!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->"
                                  "<meta name=\"description\" content=\"\">"
                                  "<meta name=\"author\" content=\"\">"
                                  "<link rel=\"icon\" href=\"favicon.ico\">"
                                  "<!--  <link rel=\"shortcut icon\" href=\"http://sstatic.net/stackoverflow/img/favicon.ico\">-->"
                                  "<title>Advance HTML Report</title>"

                                  "<!-- Bootstrap core CSS -->"
                                  "<!--  <link href=\"../../dist/css/bootstrap.min.css\" rel=\"stylesheet\"> -->"

                                  "<link href=\"./resources/css/bootstrap.min.css\" rel=\"stylesheet\">"
                                  "<link href=\"./resources/css/bootstrap.css\" rel=\"stylesheet\">"
                                  "<link href=\"./resources/css/simple-sidebar.css\" rel=\"stylesheet\">"

                                  "<link href=\"./resources/css/table-fixed.css\" rel=\"stylesheet\">"


                                  "<!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->"
                                  "<link href=\"./resources/css/ie10-viewport-bug-workaround.css\" rel=\"stylesheet\">"

                                  "<!-- Custom styles for this template -->"
                                  " <link href=\"./resources/css/dashboard.css\" rel=\"stylesheet\">"
                                  "</head>"
                                  );

    return report_data;
}

/*
 * QString report_html_template_plugin_advance::report_set_next_previous_pager_buttons(QString previous, QString next, QString first, QString last)
{

    QString temp_data;
    temp_data = QString("<div class=\"pager  pull-right \">");

    //-Very First Page
    temp_data += QString("<li class=\"previous \"><a href=\"" + first +"\"><span aria-hidden=\"true\">&larr;</span> First </a></li>");
    temp_data += QString("<li> </li>");

    //-For previous page from current
    if(previous == "")
        temp_data += QString("<li class=\"previous disabled\"><a href=\"" + previous +"\"><span aria-hidden=\"true\">&larr;</span> Prev </a></li>");
    else
        temp_data += QString("<li class=\"previous\"><a href=\"" + previous +"\"><span aria-hidden=\"true\">&larr;</span> Prev </a></li>");

    // temp_data += QString("<li> </li>");

    //-For Last Page
    temp_data += QString("<li class=\"next \"><a href=\"" + last +"\"> Last <span aria-hidden=\"true\">&rarr;</span></a></li>");

    temp_data += QString("<li> </li>");

    //- For next from current
    if(next == "")
        temp_data += QString("<li class=\"next disabled\"><a href=\"" + next +"\"> Next <span aria-hidden=\"true\">&rarr;</span></a></li>");
    else
        temp_data += QString("<li class=\"next \"><a href=\"" + next +"\"> Next <span aria-hidden=\"true\">&rarr;</span></a></li>");


    temp_data += QString("</div> "

                         " </ul>"
                         "</div><!--/.nav-collapse -->");



//<a href="http://www.w3schools.com/html/">Visit our HTML tutorial</a>

    return temp_data;
}

*/

