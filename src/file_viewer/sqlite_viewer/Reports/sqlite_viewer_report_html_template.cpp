#include "sqlite_viewer_report_html_template.h"

sqlite_viewer_report_html_template::sqlite_viewer_report_html_template()
{
}

QString sqlite_viewer_report_html_template::html_header1()
{
    QString header1 = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">  <html xmlns=\"http://www.w3.org/1999/xhtml\"> <head> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />";
    return header1;
}

QString sqlite_viewer_report_html_template::html_title(QString title)
{
    return (QString("<title>") + title + "</title>");
}

QString sqlite_viewer_report_html_template::html_header_after_title()
{
    QString header_after_title = "<link rel=\"stylesheet\" href=\"./resources/style.css\" type=\"text/css\" /><style type=\"text/css\"> .c3{color:RGB(255,255,255)} .c4{color:RGB(128,128,128)}  .c6{color:RGB(255,0,0)}  .g1, tr.g1 td{background:RGB(51,113,163)} .g2, tr.g2 td{background:RGB(255,255,255)}  .g5, tr.g5 td{background:RGB(236,236,236)}     .g7, tr.g7 td{background:RGB(200,209,219)}    .g8, tr.g8 td{background:RGB(192,192,192)}    .s10{font-size:10pt} .s22{font-size:22pt} .s18{font-size:18pt}  .s14{font-size:14pt}    .s12{font-size:12pt}    .s11{font-size:11pt}    .p20{padding:20px}    .p10{padding:10px}  .wrapped { white-space: pre; white-space: pre-wrap; white-space: pre-line; white-space: -pre-wrap; white-space: -o-pre-wrap; white-space: -moz-pre-wrap; white-space: -hp-pre-wrap; word-wrap: break-word; width: 300px; } body { margin:0; padding:0 0 0 150px; } div#left-sidebar { position:absolute; top:20; left:0; width:150px; height:80%; } @media screen { body>div#left-sidebar { position:fixed; } } * html body { overflow:hidden; } * html div#content { height:60%; overflow:auto; } </style> </head>  <body> <div id=\"left-sidebar\"> <div class = \"stack\"> <h2 id =\"\"> <span class = \" s14\" id=\"\">" + QObject::tr("Contents") + "</span> </h2> </div>";
    return header_after_title;
}

QString sqlite_viewer_report_html_template::html_sidebar_start()
{
    QString sidebar_start = "<table width = \"100%\" border=\"0\" align=\"center\" cellpadding=\"1\" cellspacing=\"0\">  <tr>  <td> ";
    return sidebar_start;

}

QString sqlite_viewer_report_html_template::html_sidebar_link(QString module_name)
{
    QString temp = "<div align = \"left\"><a href = \"#";
    temp += module_name + "\">";
    temp+= module_name;
    temp+= "</a><br />";

    return temp;
    //<div align = "left"><a href = "#Address Book">Address Book </a> <br />
}

QString sqlite_viewer_report_html_template::html_report_main_title(QString icon, QString title_name)
{

    QString temp = "</div> <a name = \"" + QObject::tr("Top") + "\"> </a> <div id = \"page\"> <div id = \"content\"> <div> <div class = \"border g2\"><table class = \"grid\"><thead><th width=\"12%\" /><th width=\"1%\" /><th width=\"\" /></thead> <tbody>  <tr> <td class=\"td-\" style=\"word-wrap:break-word\"><div class=\"border g2 p10\">";
    temp += "<img src=";
    temp += "\"" + icon + "\"";
    temp += "height=\"100\" width=\"100\" /></div></td><td class=\"td-\" style=\"word-wrap:break-word\"> <span class=\" s10\" id=\"\"></span></td> <td class=\"td-\" style=\"word-wrap:break-word\"> <br /> <div class=\"stack\"> <h4 class=\"caption\" id=\"\"><span class=\" s18\" id=\"\">";
    temp += title_name + "</span>  </h4> </div>  </td> </tr>  </tbody>  </table>";

    return temp;

}

QString sqlite_viewer_report_html_template::html_module_header(QString icon_path, QString report_content, QString module_name)
{

    QString temp = "<br /><a name=\"";
    temp += module_name + "\"> </a>  <div> <h4 class=\"caption\" id=\"\"><span class=\" s14\" id=\"\">";
    temp += module_name;

    temp += " (<font size = \"3\" >" + report_content + "</font>)";
    temp += "</span>  </h4> <img src = " + QString("\"") + icon_path + "\"";
    temp += "width=\"60\" height=\"60\" /><br /> </div>";
    return temp;


}

QString sqlite_viewer_report_html_template::html_sub_module_header(QString module_name)
{
    QString temp = "<br /><a name=\"";
    temp += module_name + "\"> </a>  <div> <h4 class=\"caption\" id=\"\"><span class=\" s14\" id=\"\">";
    temp +=module_name +  " </span>  </h2>  </div>";
    return temp;
}

QString sqlite_viewer_report_html_template::html_top_link()
{
    QString temp = "<div align = \"right\"> <a href = \"#Top\">" + QObject::tr("Top") +  "</a> </div>";
    return temp;

}

QString sqlite_viewer_report_html_template::html_top_module_header(QString module_name)
{
    QString temp = " <a href = \"#Top\">" + QObject::tr("Top") + "</a> </div>";
    return temp;
}

QString sqlite_viewer_report_html_template::html_table_start()
{
    return QString("<table width = \"100%\" border=\"1\" align=\"center\" cellpadding=\"1\" cellspacing=\"0\" style=\"border-bottom: 0;border-top: 0;border-left: 0;border-right: 0;\">");
}

QString sqlite_viewer_report_html_template::html_table_headers(QStringList list, int value)
{
    QString temp = "<tr>";

    if(value == 2)
    {
        temp += "<th width=\"50\" >" + list.at(0) + "</th>";
        temp += "<th width=\"100%\" style=\"text-align:center\" >" + list.at(1) + "</th>";

        temp += "</tr>";
        return temp;

    }

    if(value == 3)
    {
        temp += "<th width=\"50\" >" + list.at(0) + "</th>";
        temp += "<th width=\"100%\" style=\"text-align:center\" >" + list.at(1) + "</th>";
        temp += "<th width=\"150\" style=\"text-align:center\" >" + list.at(2) + "</th>";

        temp += "</tr>";
        return temp;

    }


    if(list.size() <= 6  && value == 1)
    {

        temp += "<th width=\"50\" >" + list.at(0) + "</th>";

        for(int i = 1 ; i < list.size(); i ++)
        {
            temp += "<th width=\"50%\" >" + list.at(i) + "</th>";
        }
        temp += "</tr>";
        return temp;
    }
    else
    {
        temp += "<th width=\"50\" >" + list.at(0) + "</th>";
        temp += "<th width=\"50\" >" + list.at(1) + "</th>";
        temp += "<th width=\"90\" >" + list.at(2) + "</th>";
        for(int i =3; i < list.size(); i ++)
        {
            temp += "<th width=\"50%\" >" + list.at(i) + "</th>";
        }
        temp += "</tr>";
    }
    return temp;

}

QString sqlite_viewer_report_html_template::html_table_row_data(QStringList list, int value)
{
    QString temp;
    temp = "<tr>";

    for(int i = 0; i < list.size(); i++ )
    {
        if((i == list.size() - 1) && (value == 3))
        {
            temp += "<td class = \"wrapped\" align=\"center\">" + list.at(i) + "</td>";
        }
        else
        {
            temp += "<td class = \"wrapped\">" + list.at(i) + "</td>";
        }
    }
    temp += "</tr>";

    return temp;
}


QString sqlite_viewer_report_html_template::html_table_end()
{
    return QString("</table>");
}


QString sqlite_viewer_report_html_template::html_sidebar_end()
{
    QString sidebar_end = "</td>  </tr>  </table>";
    return sidebar_end;
}



