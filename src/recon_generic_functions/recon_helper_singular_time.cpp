#include "recon_helper_singular.h"


QString recon_helper_singular::convert_webkit_chrome_time_to_human_readable(QString received_time, QString caller_func)
{
    //Input  : 13264753944000000
    //Output : In user selected format date time


    if(received_time == "0")
        return("");

    QString temp_time_str = received_time.split(".").at(0);

    if(temp_time_str.size() < 10)
    {
        return("");
    }

    if(received_time.size() == 10)
        return("");

    bool bbl;

    qint64 date_time_int = received_time.toULongLong(&bbl,10);

    date_time_int = (date_time_int / 1000000) - 11644473600;
    QString human_readable_time = convert_numeric_to_readable_in_selected_timezone(QString::number(date_time_int), Q_FUNC_INFO);

    return human_readable_time;

}

QString recon_helper_singular::convert_safari_time_to_human_readable(QString received_time, QString caller_func)
{
    //Input  : 621752966
    //Output : In user selected format date time


    received_time = received_time.trimmed();
    if(received_time == "0" || received_time.isEmpty())
    {
        return("");
    }


    QString tmp_rcvd_time;
    for(int count = 0; count < 9; count++)
    {
        if(received_time.size() > count)
            tmp_rcvd_time.append(received_time.at(count));
    }

    received_time.clear();
    received_time = tmp_rcvd_time;

    QStringList list = received_time.split(".", Qt::SkipEmptyParts);
    received_time = list.at(0);

    bool v;
    qint64 date_time_int = received_time.toULongLong(&v,10);

    date_time_int = (date_time_int + 978307200);

    QString human_readable_time =  convert_numeric_to_readable_in_selected_timezone(QString::number(date_time_int), Q_FUNC_INFO);

    return human_readable_time;

}

qint64 recon_helper_singular::convert_timestamp_readable_to_numeric_1(QString m_datetime)
{
    //Input : 2018-09-04 10:53:32
    //Output : 1536038613

    //Input : 2018-09-04 10:53:32.153872
    //Output : 1536038613

    m_datetime = m_datetime.trimmed();
    if(m_datetime.contains("."))
    {
        QStringList value =   m_datetime.split(".");
        m_datetime = value.at(0);
    }

    QString format  = QString("yyyy-MM-dd hh:mm:ss");
    QDateTime valid = QDateTime::fromString(m_datetime, format);
    valid.setTimeSpec(Qt::UTC);

    qint64 epch_qint64 = valid.toSecsSinceEpoch();

    return epch_qint64;
}

qint64 recon_helper_singular::convert_timestamp_readable_to_numeric_2(QString timestamp_str , QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    //Input : 2016-11-26 06:29:50 +0000
    //Output : 1480121990

    timestamp_str = timestamp_str.trimmed();

    if(timestamp_str.isEmpty())
        return 0;

    if(timestamp_str.endsWith("+0000"))
        timestamp_str.chop(5);
    else
        return 0;

    timestamp_str = timestamp_str.trimmed();

    QString format  = QString("yyyy-MM-dd hh:mm:ss");
    QDateTime valid = QDateTime::fromString(timestamp_str, format);
    valid.setTimeSpec(Qt::UTC);

    qint64 epch_qint64 =  valid.toSecsSinceEpoch();

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return epch_qint64;
}


qint64 recon_helper_singular::convert_timestamp_readable_to_numeric_3(QString timestamp_str , QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    //Input : 1970-01-03T03:33:19.000+02:00
    //Output : 1480121990

    timestamp_str = timestamp_str.trimmed();

    if(timestamp_str.isEmpty())
        return 0;

    QStringList date_time_list;
    if(timestamp_str.contains("T"))
    {
        date_time_list = timestamp_str.split("T");
    }
    else
        return 0;

    QString date_str , time_str;
    if(date_time_list.size() == 2)
    {
        date_str = date_time_list.at(0);
        time_str = date_time_list.at(1);
    }

    bool is_negative_timezone = false;

    QStringList temp_list;
    QString timezone;
    QString hr_min_ss_str;
    if(time_str.contains("-"))
    {
        is_negative_timezone = true;
        temp_list = time_str.split("-");
        hr_min_ss_str = temp_list.at(0);
        timezone = temp_list.at(1);
    }
    else
    {
        is_negative_timezone = false;
        temp_list = time_str.split("+");
        hr_min_ss_str = temp_list.at(0);
        timezone = temp_list.at(1);
    }

    hr_min_ss_str = hr_min_ss_str.trimmed();
    if(hr_min_ss_str.contains("."))
    {
        QStringList value = hr_min_ss_str.split(".");
        hr_min_ss_str = value.at(0);
    }

    timestamp_str = date_str + " " + hr_min_ss_str;

    QString format  = QString("yyyy-MM-dd hh:mm:ss");
    QDateTime valid;
    valid = QDateTime::fromString(timestamp_str, format);
    valid.setTimeSpec(Qt::UTC);

    QStringList zone_list = timezone.split(":");

    int hh = zone_list.at(0).toInt();
    int mm = zone_list.at(1).toInt();

    qint64 offset = (3600 * hh) + (60 * mm);

    qint64 epch_qint64 = valid.toSecsSinceEpoch();

    if(is_negative_timezone)
    {
        epch_qint64 =  epch_qint64 + offset;
    }
    else
    {
        epch_qint64 =  epch_qint64 - offset;
    }

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return epch_qint64;
}

qint64 recon_helper_singular::convert_timestamp_readable_to_numeric_4(QString timestamp_str , QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    //Input : 09-Jan-21 01:47:02 PM +05:30
    //Output : 1480121990

    timestamp_str = timestamp_str.trimmed();

    if(timestamp_str.isEmpty())
        return -1;

    QStringList list = timestamp_str.split(" " , Qt::SkipEmptyParts);

    QString date_string , time_string, hour_format, timezone_string;
    if(list.size() == 4)
    {
        date_string     = list.at(0).trimmed();
        time_string     = list.at(1).trimmed();
        hour_format     = list.at(2).trimmed();
        timezone_string = list.at(3).trimmed();
    }
    else
        return -1;

    QStringList date_list = date_string.split("-");

    QString year_str = date_list.at(2).trimmed();
    if(year_str.size() == 2)
        year_str.prepend("20");

    date_string = date_list.at(0) + "-" + date_list.at(1) + "-" + year_str;

    timestamp_str = date_string + " " + time_string + " " + hour_format;

    timestamp_str = timestamp_str.trimmed();

    QString format = "dd-MMM-yyyy hh:mm:ss AP";
    QDateTime valid ;
    valid = QDateTime::fromString(timestamp_str, format);
    valid.setTimeSpec(Qt::UTC);

    bool bool_is_positive_tmezone = true;
    if(timezone_string.startsWith("-"))
    {
        bool_is_positive_tmezone = false;
        timezone_string.remove("-");
    }
    else if(timezone_string.startsWith("+"))
    {
        timezone_string.remove("+");
    }

    QStringList timezone_list;
    if(timezone_string.contains(":"))
    {
        timezone_list = timezone_string.split(":");
    }

    int timezone_offset = 0;
    if(timezone_list.size() == 2)
    {
        int hh = timezone_list.at(0).toInt();
        int mm = timezone_list.at(1).toInt();

        int hh1 = hh * 3600;
        int mm1 = mm * 60;

        timezone_offset = hh1 + mm1;
    }

    qint64 epch_qint64;
    if(bool_is_positive_tmezone)
    {
        epch_qint64 = valid.toSecsSinceEpoch() - timezone_offset;
    }
    else
    {
        epch_qint64 = valid.toSecsSinceEpoch() + timezone_offset;
    }

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return epch_qint64;
}

qint64 recon_helper_singular::convert_timestamp_readable_to_numeric_5(QString timestamp_str , QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    //Input : 4/6/2021 01:47:02 PM
    //Output : 1480121990

    //Input : 04/06/2021 01:47:02 PM
    //Output : 1480121990

    //Input : 4/6/2015 13:47:02
    //Output : 1480121990

    //Input : 04/06/2015 13:47:02
    //Output : 1480121990

    //Input : 04/06/2015 1:47:02
    //Output : 1480121990

    timestamp_str = timestamp_str.trimmed();

    if(timestamp_str.isEmpty())
        return 0;

    QString format;

    QStringList list = timestamp_str.split(" " , Qt::SkipEmptyParts);

    QString date_string , time_string, hour_format;
    if(list.size() == 3)
    {
        date_string     = list.at(0).trimmed();
        time_string     = list.at(1).trimmed();
        hour_format     = list.at(2).trimmed();

        format = "dd/MM/yyyy hh:mm:ss AP";
    }
    else if(list.size() == 2)
    {
        date_string     = list.at(0).trimmed();
        time_string     = list.at(1).trimmed();
        hour_format     = "";
        format = "dd/MM/yyyy hh:mm:ss";
    }
    else
        return 0;

    QStringList date_list = date_string.split("/");

    QString day_str = date_list.at(0).trimmed();
    if(day_str.size() == 1)
        day_str.prepend("0");

    QString month_str = date_list.at(1).trimmed();
    if(month_str.size() == 1)
        month_str.prepend("0");

    QString year_str = date_list.at(2).trimmed();
    if(year_str.size() == 2)
        year_str.prepend("20");

    date_string = day_str + "/" + month_str + "/" + year_str;


    QStringList time_list = time_string.split(":");

    QString hh_str = time_list.at(0).trimmed();
    if(hh_str.size() == 1)
        hh_str.prepend("0");

    QString mm_str = time_list.at(1).trimmed();
    if(mm_str.size() == 1)
        mm_str.prepend("0");

    QString ss_str = time_list.at(2).trimmed();
    if(ss_str.size() == 1)
        ss_str.prepend("0");

    time_string = hh_str + ":" + mm_str + ":" + ss_str;

    timestamp_str = date_string + " " + time_string + " " + hour_format;

    timestamp_str = timestamp_str.trimmed();

    QDateTime valid ;
    valid = QDateTime::fromString(timestamp_str, format);
    valid.setTimeSpec(Qt::UTC);

    qint64 epch_qint64 = valid.toSecsSinceEpoch();

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return epch_qint64;
}

qint64 recon_helper_singular::convert_timestamp_readable_to_numeric_6(QString timestamp_str , QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    //Input : 2/1/2021 7:37:27 PM(UTC+0)
    //Output : 1480121990

    timestamp_str = timestamp_str.trimmed();

    if(timestamp_str.isEmpty())
        return 0;

    QString format = "dd/MM/yyyy hh:mm:ss AP";

    QStringList list = timestamp_str.split(" " , Qt::SkipEmptyParts);

    QString date_string , time_string, hour_format;
    if(list.size() == 3)
    {
        date_string     = list.at(0).trimmed();
        time_string     = list.at(1).trimmed();
        hour_format     = list.at(2).trimmed();

        QStringList temp;
        if(hour_format.contains("(UTC"))
        {
            temp = hour_format.split("(");
            hour_format = temp.at(0);
        }
    }
    else
        return 0;

    QStringList date_list = date_string.split("/");

    QString day_str = date_list.at(0).trimmed();
    if(day_str.size() == 1)
        day_str.prepend("0");

    QString month_str = date_list.at(1).trimmed();
    if(month_str.size() == 1)
        month_str.prepend("0");

    QString year_str = date_list.at(2).trimmed();
    if(year_str.size() == 2)
        year_str.prepend("20");

    date_string = day_str + "/" + month_str + "/" + year_str;

    QStringList time_list = time_string.split(":");

    QString hh_str = time_list.at(0).trimmed();
    if(hh_str.size() == 1)
        hh_str.prepend("0");

    QString mm_str = time_list.at(1).trimmed();
    if(mm_str.size() == 1)
        mm_str.prepend("0");

    QString ss_str = time_list.at(2).trimmed();
    if(ss_str.size() == 1)
        ss_str.prepend("0");

    time_string = hh_str + ":" + mm_str + ":" + ss_str;

    timestamp_str = date_string + " " + time_string + " " + hour_format;

    timestamp_str = timestamp_str.trimmed();

    QDateTime valid ;
    valid = QDateTime::fromString(timestamp_str, format);
    valid.setTimeSpec(Qt::UTC);

    qint64 epch_qint64 = valid.toSecsSinceEpoch();

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    return epch_qint64;
}

QString recon_helper_singular::convert_timestamp_readable_to_numeric_7(QString date_time)
{
    // received value example  ==>>  2019-04-01 22:17:57.484591+0200
    // returned value example  ==>>  1554157077


    QString dt_time = date_time;

    dt_time = dt_time.remove(date_time.indexOf("."), date_time.size());
    dt_time = dt_time.trimmed();
    if(dt_time.isEmpty() || dt_time == QString::number(0))
        return QString("");



    QString tzone = date_time;
    if(tzone.contains("+"))
        tzone = tzone.remove(0 , tzone.lastIndexOf("+") +1);
    else if(tzone.contains("-"))
        tzone = tzone.remove(0 , tzone.lastIndexOf("-") +1);
    else
        return "";


    QString format = "yyyy-MM-dd hh:mm:ss";

    QDateTime valid = QDateTime::fromString(dt_time, format);
    valid.setTimeSpec(Qt::UTC);

    qint64 epoch_time = (valid.toMSecsSinceEpoch())/1000;


    int total_seconds = convert_timezone_to_seconds(tzone);

    if(date_time.contains("+"))
        epoch_time = epoch_time - total_seconds;
    else
        epoch_time = epoch_time + total_seconds;


    return QString::number(epoch_time);
}

int recon_helper_singular::convert_timezone_to_seconds(QString tm_zone)
{
    // Input : 0530
    // Output : 19800


    QString hours = tm_zone;
    hours = hours.remove(2 , hours.size());

    QString mins = tm_zone;
    mins = mins.remove(0, 2);


    int mins_frm_hrs = hours.toInt();
    mins_frm_hrs = mins_frm_hrs * 60;

    int total_mintues = mins_frm_hrs + mins.toInt();
    int total_seconds = total_mintues * 60;


    return total_seconds;
}

QString recon_helper_singular::convert_numeric_to_readable_in_utc_timezone(QString received_date_time, QString caller_func)
{

    //Input  : 1614528701
    //Output : In user selected format date time

    if(received_date_time.contains("."))
    {
        QStringList list = received_date_time.split(".", Qt::SkipEmptyParts);
        received_date_time = list.at(0);
    }


    QString tmp_rcvd_time_str;
    for(int count  = 0; count < 10; count++)
    {
        if(received_date_time.size() > count)
            tmp_rcvd_time_str.append(received_date_time.at(count));
    }
    received_date_time = tmp_rcvd_time_str;

    bool ok = true;
    qint64 received_date_time_int = received_date_time.toLongLong(&ok,10);

    if(received_date_time_int == 0)
        return "";


    QString date_format = struct_default_timezone_details_obj.d_examiner_selected_date_format_readable_QString;
    qint64 date_format_int = struct_default_timezone_details_obj.d_examiner_selected_date_format_numeric_qint64;
    QString display_time = "+00:00";
    if(received_date_time_int < 0)
    {
        time_t t_of_day = received_date_time_int;

        struct tm *tss =  gmtime(&t_of_day);
        char       buf[80];

        if(date_format_int == 0)
            strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", tss);
        if(date_format_int == 1)
            strftime(buf, sizeof(buf), "%m/%d/%Y %H:%M:%S", tss);
        if(date_format_int == 2)
            strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", tss);

        QString final_time = QString(buf) + " " + display_time;
        return final_time;
    }

    QDateTime ddt;
    ddt.setTimeSpec(Qt::UTC);
    ddt = ddt.fromSecsSinceEpoch(received_date_time_int, Qt::UTC);

    QString final_date_time_str = ddt.toString(date_format) + " " + display_time;

    return final_date_time_str;
}

QString recon_helper_singular::convert_numeric_to_readable_in_selected_timezone(QString received_date_time, QString caller_func)
{
    //Input  : 1614528701
    //Output : In user selected format date time

    if(received_date_time.contains("."))
    {
        QStringList list = received_date_time.split(".", Qt::SkipEmptyParts);
        received_date_time = list.at(0);
    }

    if(received_date_time.startsWith("-"))
    {
        QString tmp_rcvd_time_str;
        for(int count  = 0; count < 11; count++)
        {
            if(received_date_time.size() > count)
                tmp_rcvd_time_str.append(received_date_time.at(count));
        }

        received_date_time = tmp_rcvd_time_str;

        bool ok = true;
        qint64 received_date_time_qint64 = received_date_time.toLongLong(&ok,10);

        if(received_date_time_qint64 == 0)
            return "";

        qint64 offset_value_qint64 = 0;
        qint64 date_format_qint64 = 0;
        QString display_time , date_format;
        bool bool_is_time_in_timezones = false;

        for(int ii = 0 ; ii < added_timezone_details_qlist.size() ; ii++)
        {
            qint64 start_date_qint64 = added_timezone_details_qlist.at(ii).start_date_numeric_qint64;
            qint64 end_date_qint64 = added_timezone_details_qlist.at(ii).end_date_numeric_qint64;

            if((received_date_time_qint64 >= start_date_qint64) && received_date_time_qint64 <= end_date_qint64)
            {
                offset_value_qint64 = added_timezone_details_qlist.at(ii).offset.toLongLong();
                display_time = added_timezone_details_qlist.at(ii).display_timezone_offset_name;
                bool_is_time_in_timezones = true;
                break ;
            }
        }

        date_format = struct_default_timezone_details_obj.d_examiner_selected_date_format_readable_QString;
        if(!bool_is_time_in_timezones)
        {
            offset_value_qint64 = struct_default_timezone_details_obj.d_examiner_selected_timezone_numeric_qint64;
            date_format = struct_default_timezone_details_obj.d_examiner_selected_date_format_readable_QString;
            display_time = struct_default_timezone_details_obj.timezone_offset_name;
        }

        received_date_time_qint64 += offset_value_qint64;

        date_format_qint64 = struct_default_timezone_details_obj.d_examiner_selected_date_format_numeric_qint64;
        QDateTime ddt;
        ddt.setTimeSpec(Qt::UTC);
        ddt = ddt.fromSecsSinceEpoch(received_date_time_qint64, Qt::UTC);
        QString final_date_time_str = ddt.toString(date_format) + " " + display_time +"";

        return final_date_time_str;
    }
    else
    {
        QString tmp_rcvd_time_str;
        for(int count  = 0; count < 10; count++)
        {
            if(received_date_time.size() > count)
                tmp_rcvd_time_str.append(received_date_time.at(count));
        }
        received_date_time = tmp_rcvd_time_str;

        bool ok = true;
        qint64 received_date_time_qint64 = received_date_time.toLongLong(&ok,10);

        if(received_date_time_qint64 == 0)
            return "";

        qint64 offset_value_qint64 = 0;
        qint64 date_format_qint64 = 0;
        QString display_time , date_format;

        bool bool_is_time_in_timezones = false;
        for(int ii = 0 ; ii < added_timezone_details_qlist.size() ; ii++)
        {
            qint64 start_date_qint64 = added_timezone_details_qlist.at(ii).start_date_numeric_qint64;
            qint64 end_date_qint64 = added_timezone_details_qlist.at(ii).end_date_numeric_qint64;

            if((received_date_time_qint64 >= start_date_qint64) && received_date_time_qint64 <= end_date_qint64)
            {
                offset_value_qint64 = added_timezone_details_qlist.at(ii).offset.toLongLong();
                display_time = added_timezone_details_qlist.at(ii).display_timezone_offset_name;
                bool_is_time_in_timezones = true;
                break ;
            }
        }
        date_format = struct_default_timezone_details_obj.d_examiner_selected_date_format_readable_QString;

        if(!bool_is_time_in_timezones)
        {
            offset_value_qint64 = struct_default_timezone_details_obj.d_examiner_selected_timezone_numeric_qint64;
            date_format = struct_default_timezone_details_obj.d_examiner_selected_date_format_readable_QString;
            display_time = struct_default_timezone_details_obj.timezone_offset_name;
        }

        received_date_time_qint64 += offset_value_qint64;
        date_format_qint64 = struct_default_timezone_details_obj.d_examiner_selected_date_format_numeric_qint64;
        if(received_date_time_qint64 < 0)
        {
            time_t t_of_day = received_date_time_qint64;


            struct tm *tss =  gmtime(&t_of_day);
            char       buf[80];

            if(date_format_qint64 == 0)
                strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", tss);
            if(date_format_qint64 == 1)
                strftime(buf, sizeof(buf), "%m/%d/%Y %H:%M:%S", tss);
            if(date_format_qint64 == 2)
                strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", tss);

            QString final_time = QString(buf) + " " + display_time + "";
            return final_time;
        }

        QDateTime ddt;
        ddt.setTimeSpec(Qt::UTC);
        ddt = ddt.fromSecsSinceEpoch(received_date_time_qint64, Qt::UTC);

        QString final_date_time_str = ddt.toString(date_format) + " " + display_time +"";

        return final_date_time_str;
    }
}

QString recon_helper_singular::convert_numeric_to_readable_export_time(QString received_date_time, QString caller_func)
{
    //Input  : 1614528701
    //Output : In user selected format date time

    if(received_date_time.contains("."))
    {
        QStringList list = received_date_time.split(".", Qt::SkipEmptyParts);
        received_date_time = list.at(0);
    }



    QString tmp_rcvd_time_str;
    for(int count  = 0; count < 10; count++)
    {
        if(received_date_time.size() > count)
            tmp_rcvd_time_str.append(received_date_time.at(count));
    }
    received_date_time = tmp_rcvd_time_str;

    bool ok = true;
    qint64 received_date_time_int = received_date_time.toLongLong(&ok,10);

    if(received_date_time_int == 0)
        return "";

    if(received_date_time_int < 0)
    {
        time_t t_of_day = received_date_time_int;

        struct tm *tss =  gmtime(&t_of_day);
        char       buf[80];

        if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Date_Format_int).toInt() == 0)
            strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", tss);
        if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Date_Format_int).toInt() == 1)
            strftime(buf, sizeof(buf), "%m/%d/%Y %H:%M:%S", tss);
        if(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Date_Format_int).toInt() == 2)
            strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", tss);

        return QString(buf);
    }

    QDateTime ddt;
    ddt.setTimeSpec(Qt::UTC);
    ddt = ddt.fromSecsSinceEpoch(received_date_time_int, Qt::UTC);

    return ddt.toString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString).toString());

}

QString recon_helper_singular::convert_numeric_to_readable_for_tablewidget(QString received_date_time, QString caller_func)
{
    //Input  : 1614528701
    //Output : In user selected format date time  Like 2022/01/05 12:58:54

    if(received_date_time == "0")
    {
        return "";
    }

    if(received_date_time.contains("."))
    {
        QStringList list = received_date_time.split(".", Qt::SkipEmptyParts);
        received_date_time = list.at(0);
    }

    QString tmp_rcvd_time_str;
    for(int count  = 0; count < 10; count++)
    {
        if(received_date_time.size() > count)
            tmp_rcvd_time_str.append(received_date_time.at(count));
    }
    received_date_time = tmp_rcvd_time_str;


    bool ok = true;
    qint64 received_date_time_int = received_date_time.toLongLong(&ok,10);

    if(received_date_time_int == 0)
        return "";

    qint64 offset_value = 0;
    QString display_time , date_format;

    bool bool_is_time_in_timezones = false;
    for(int i = 0 ; i < added_timezone_details_qlist.size() ; i++)
    {
        qint64 start_date_qint64 = added_timezone_details_qlist.at(i).start_date_numeric_qint64;
        qint64 end_date_qint64 = added_timezone_details_qlist.at(i).end_date_numeric_qint64;

        if((received_date_time_int >= start_date_qint64) && received_date_time_int <= end_date_qint64)
        {
            offset_value = added_timezone_details_qlist.at(i).offset.toLongLong();
            display_time = added_timezone_details_qlist.at(i).display_timezone_offset_name;
            bool_is_time_in_timezones = true;
            break ;
        }
    }
    date_format = "yyyy/MM/dd hh:mm:ss";

    if(!bool_is_time_in_timezones)
    {
        offset_value = struct_default_timezone_details_obj.d_examiner_selected_timezone_numeric_qint64;
        display_time = struct_default_timezone_details_obj.timezone_offset_name;
    }

    received_date_time_int += offset_value;
    if(received_date_time_int < 0)
    {
        time_t t_of_day = received_date_time_int;


        struct tm *tss =  gmtime(&t_of_day);
        char       buf[80];

        strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", tss);

        QString final_time = QString(buf) + " " + display_time + "";
        return final_time;
    }

    QDateTime ddt;
    ddt.setTimeSpec(Qt::UTC);
    ddt = ddt.fromSecsSinceEpoch(received_date_time_int, Qt::UTC);

    QString final_date_time_str = ddt.toString(date_format) + " " + display_time +"";

    return final_date_time_str;
}

qint64 recon_helper_singular::convert_UTC_numeric_to_case_timezone_numeric(qint64 epch)
{
    epch = epch - struct_default_timezone_details_obj.d_examiner_selected_timezone_numeric_qint64;

    return epch;
}

QString recon_helper_singular::convert_user_formatted_date_to_numeric(QString timestamp_str, QString format_str, QString caller_fun)
{
    QString created_date = QDateTime::fromString(timestamp_str, format_str).toString("yyyy/MM/dd");
    QDateTime datetime = QDateTime::fromString(created_date, "yyyy/MM/dd");
    datetime.setTimeSpec(Qt::UTC);
    return QString::number(datetime.toSecsSinceEpoch());
}

QString recon_helper_singular::convert_user_formatted_datetime_to_numeric(QString timestamp_str, QString format_str, QString caller_fun)
{
    QDateTime datetime = QDateTime::fromString(timestamp_str, format_str);
    datetime.setTimeSpec(Qt::UTC);

    return QString::number(datetime.toSecsSinceEpoch());
}

QString recon_helper_singular::extract_date_from_datetime(qint64 m_epch)
{
    QString date_epch;

    if(m_epch == 0)
        return "0";

    qint64 round_off_value =  m_epch % (24*60*60); // Make it at 12:00

    date_epch = QString::number(m_epch - round_off_value);

    return date_epch;
}


