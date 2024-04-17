#include "chat_template_class.h"

chat_template_class::chat_template_class()
{

}

chat_template_class::~chat_template_class()
{

}

QString chat_template_class::chat_page_start()
{
    QString data;

    ///reference from ./resources/chat_template.css

    //right align
    QString triangle_style_tag_value =   ".triangle-obtuse {"
                                         "position:relative;"
                                         "float: right;"
                                         "max-width: 60%;"
                                         "margin: 0.50;"
                                         "margin-right: 10;"
                                         "color:#ffffff;"
                                         "background:#10c553 ;"
                                         "padding:5px 8px;"
                                         "font-size: 12;"


            /* css3 */

            "-webkit-border-radius:10px;"
            "-moz-border-radius:10px;"
            "border-radius:10px;"
            "}";


    //left align
    QString obtuse_style_tag_value =  ".example-obtuse {"
                                      "position:relative;"
                                      "float: left;"
                                      "max-width: 60%;"
                                      "margin: 0.50;"
                                      "margin-left: 10;"
                                      "color:#ffffff;"
                                      "background:#008ee6;" /* default background for browsers without gradient support */
            "padding:5px 8px;"
            "font-size: 12;"

            /* css3 */

            "-webkit-border-radius:10px;"
            "-moz-border-radius:10px;"
            "border-radius:10px;"


            "};"
            "</style>";


    data += "<html>";
    data += "<head>";
    data += "<style>";
    data += triangle_style_tag_value + obtuse_style_tag_value;
    data += "</style>";
    data += "</head>";
    data += "<body>";

    return data;
}

QString chat_template_class::html_left_align(QString data_str)
{
    return  QString("<div class=\"example-obtuse\">" + data_str  + "</div>");
}

QString chat_template_class::html_right_align(QString data_str)
{
    return QString("<div class=\"triangle-obtuse\">" + data_str  + "</div>");
}



QString chat_template_class::html_table_start()
{
    return QString("<table width = \"100%\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" style=\"border-bottom: 0;border-top: 0;border-left: 0;border-right: 0;\">");
}

QString chat_template_class::html_table_row_data(QString data)
{
    QString temp;
    temp = "<tr>";

    temp += "<td class = \"wrapped\">" + data + "</td>";

    temp += "</tr>";

    return temp;
}

QString chat_template_class::html_table_end()
{
    return QString("</table>");
}

QString chat_template_class::chat_page_end()
{
    QString data;
    data += "</body>";
    data += "</html>";

    return data;
}


