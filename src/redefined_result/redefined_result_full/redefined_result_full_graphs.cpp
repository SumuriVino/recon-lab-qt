#include "redefined_result_full_loader.h"

void redefined_result_full_loader::create_graphs_messenger()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    radiobutton_create_graph_yearly->setChecked(true);
    set_graphs_essentials_for_data_only();

    int count_stacks = stackwidget_obj->count();
    if(count_stacks > 0)
    {
        for(int i = (count_stacks-1); i >= 0; i--)
        {
            stackwidget_obj->removeWidget(stackwidget_obj->widget(i));
            delete stackwidget_obj->widget(i);
        }
    }

    label_show_date_time->clear();
    label_show_pages->clear();
    stackwidget_index = 0;

    radiobutton_create_graph_yearly->setDisabled(true);
    radiobutton_create_graph_monthly->setDisabled(true);
    radiobutton_create_graph_daywise->setDisabled(true);
    radiobutton_create_graph_hourly->setDisabled(true);
    tablewidget_accounts_configure_list->setDisabled(true);
    tablewidget_participants_list->setDisabled(true);

    map_of_date_lables_map.clear();
    map_of_chartview.clear();
    list_of_charview.clear();

    create_graph_messenger_yearly();
    map_of_chartview.insert(enum_redefined_result_graphs_yearly,list_of_charview);
    map_of_date_lables_map.insert(enum_redefined_result_graphs_yearly,map_dates_labels);
    if(radiobutton_create_graph_yearly->isChecked())
    {
        list_of_charview = map_of_chartview.value(enum_redefined_result_graphs_yearly);
        map_dates_labels = map_of_date_lables_map.value(enum_redefined_result_graphs_yearly);

        for(int i = 0; i < list_of_charview.size();i++)
        {
            stackwidget_obj->addWidget(list_of_charview.at(i));

            if(stackwidget_obj->count() > 1)
                pushbutton_next->setEnabled(true);

            label_show_date_time->setText(map_dates_labels.value(stackwidget_index));
            label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));
            stackwidget_obj->setCurrentIndex(stackwidget_index);

        }

    }


    list_of_charview.clear();
    create_graph_messenger_monthly();
    map_of_chartview.insert(enum_redefined_result_graphs_monthly,list_of_charview);
    map_of_date_lables_map.insert(enum_redefined_result_graphs_monthly,map_dates_labels);
    if(radiobutton_create_graph_monthly->isChecked())
    {

        list_of_charview = map_of_chartview.value(enum_redefined_result_graphs_monthly);
        map_dates_labels = map_of_date_lables_map.value(enum_redefined_result_graphs_monthly);
        for(int i = 0; i < list_of_charview.size();i++)
        {
            stackwidget_obj->addWidget(list_of_charview.at(i));

            if(stackwidget_obj->count() > 1)
                pushbutton_next->setEnabled(true);

            label_show_date_time->setText(map_dates_labels.value(stackwidget_index));
            label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));
            stackwidget_obj->setCurrentIndex(stackwidget_index);

        }

    }


    list_of_charview.clear();
    create_graph_messenger_daywise();
    map_of_chartview.insert(enum_redefined_result_graphs_daywise,list_of_charview);
    map_of_date_lables_map.insert(enum_redefined_result_graphs_daywise,map_dates_labels);
    if(radiobutton_create_graph_daywise->isChecked())
    {
        map_dates_labels = map_of_date_lables_map.value(enum_redefined_result_graphs_daywise);
        list_of_charview = map_of_chartview.value(enum_redefined_result_graphs_daywise);
        for(int i = 0; i < list_of_charview.size();i++)
        {
            stackwidget_obj->addWidget(list_of_charview.at(i));

            if(stackwidget_obj->count() > 1)
                pushbutton_next->setEnabled(true);

            label_show_date_time->setText(map_dates_labels.value(stackwidget_index));
            label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));
            stackwidget_obj->setCurrentIndex(stackwidget_index);
        }
    }


    list_of_charview.clear();

    radiobutton_create_graph_yearly->setDisabled(false);
    radiobutton_create_graph_monthly->setDisabled(false);
    radiobutton_create_graph_daywise->setDisabled(false);
    radiobutton_create_graph_hourly->setDisabled(false);

    if(stackwidget_obj->count() <= 1)
    {
        pushbutton_previous->setEnabled(false);
        pushbutton_next->setEnabled(false);
    }else
    {
        pushbutton_previous->setEnabled(false);
        pushbutton_next->setEnabled(true);
    }

    tablewidget_accounts_configure_list->setDisabled(false);
    tablewidget_participants_list->setDisabled(false);

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_full_loader::create_graph_messenger_yearly()
{
    if(years_list.isEmpty())
        return;

    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    if(selected_configured_account_name == QString(Macro_Redefined_Result_Messenger_All_Accounts))
    {
        create_graph_yearly_common_function();
        return;
    }


    map_dates_labels.clear();

    ///create pages to split years if year list is greater than 5
    int no_of_years_on_single_page = 5;
    int divednd_result = 0;
    if(years_list.size() > no_of_years_on_single_page)
    {
        qint64 size = years_list.size();
        int remainder = size % no_of_years_on_single_page;
        divednd_result = size/no_of_years_on_single_page;
        if(remainder != 0)
            divednd_result++;
    }

    int no_of_pages = 1;//By Default No of Pages are 1

    if(divednd_result > 0)
        no_of_pages = divednd_result;


    stackwidget_index = 0;
    map_dates_labels.clear();
    int tmp_year_count = 0;
    for(int page_i = 0; page_i < no_of_pages; page_i++)
    {
        QStringList categories_list;
        qint64 max_range_for_yaxis = 0;

        ///Create List of Barset for single page and save all barset in a map with 'key is a participant' and 'value is barset' related to that participant.
        QMap<QString, m_barset*> map_list_of_barsets;
        QMap<QString,QString>::iterator iter;


        for(iter = map_of_partipants_with_color.begin() ; iter != map_of_partipants_with_color.end(); iter++)
        {
            m_barset *barset_obj = new m_barset(iter.key());
            QColor color(iter.value());
            barset_obj->setColor(color);
            map_list_of_barsets.insert(iter.key(), barset_obj);
        }

        QStringList listofyears_for_single_page;

        if(tmp_year_count < years_list.size())
        {
            listofyears_for_single_page= years_list.mid(tmp_year_count,no_of_years_on_single_page);
            tmp_year_count+=no_of_years_on_single_page;
        }
        else
        {
            listofyears_for_single_page= years_list.mid(tmp_year_count,years_list.size());
        }


        if(listofyears_for_single_page.size() > 1)
        {
            QString year_label = listofyears_for_single_page.at(0) + "-" + listofyears_for_single_page.at(listofyears_for_single_page.size() - 1);
            map_dates_labels.insert(page_i, year_label);
        }

        ///set data in barset list of single page
        int index_in_x_axis = 0;
        for(int k = 0; k < listofyears_for_single_page.size(); k++)
        {
            QString yr_name = listofyears_for_single_page.at(k);
            categories_list << yr_name;
            QMap<QString,QString>::iterator iter;

            int max_count_in_y_axis = 0;


            for(iter = map_of_partipants_with_color.begin() ; iter != map_of_partipants_with_color.end(); iter++)
            {
                QCoreApplication::processEvents();

                m_barset *barset_obj = map_list_of_barsets.value(iter.key());

                qint64 count = 0;
                QList<struct_graphs_essentials> list_of_struct =  map_of_struct_graph_data.values(yr_name);
                for(int jj = 0;  jj < list_of_struct.size(); jj++)
                {
                    struct_graphs_essentials struct_data_database_obj = list_of_struct.at(jj);

                    if(((struct_data_database_obj.sender == selected_configured_account_name) && (struct_data_database_obj.receiver == iter.key()) ) ||
                            ((struct_data_database_obj.sender == iter.key()) && (struct_data_database_obj.receiver == selected_configured_account_name) ))
                    {
                        count++;
                    }
                }

                max_count_in_y_axis += count;
                *barset_obj << count;

                QString tooltip = prepare_and_get_tooltip_string_for_barset(count, yr_name, barset_obj->label());

                QMap<int, QString> map_insert_index_with_category = barset_obj->get_tooltip();
                map_insert_index_with_category.insert(index_in_x_axis,tooltip);
                barset_obj->set_tooltip(map_insert_index_with_category);

                map_list_of_barsets.insert(iter.key(), barset_obj);

            }

            if(max_range_for_yaxis <= max_count_in_y_axis)
                max_range_for_yaxis = max_count_in_y_axis;

            index_in_x_axis++;

        }

        ///--------

        ///Create a QStackedBarSeries and insert all barset for single page
        QStackedBarSeries *stacked_bar_series = new QStackedBarSeries(this);
        connect(stacked_bar_series, SIGNAL(hovered(bool, int, QBarSet *)), this, SLOT(slot_hovered_toolTip_on_barsets(bool, int, QBarSet *)));

        QMap<QString,QString>::iterator iter2;

        for(iter2 = map_of_partipants_with_color.begin() ; iter2 != map_of_partipants_with_color.end(); iter2++)
        {
            m_barset *barset_obj = map_list_of_barsets.value(iter2.key());
            stacked_bar_series->append(barset_obj);
        }

        qint64 roundup_value_yaxis = recon_static_functions::get_round_up_value_near_hundred_or_thousand(max_range_for_yaxis, Q_FUNC_INFO);
        struct_redefined_result_single_chart_detail struct_chart_detail_obj;
        struct_chart_detail_obj.category_list = categories_list;
        struct_chart_detail_obj.roundup_value_axis = roundup_value_yaxis;
        struct_chart_detail_obj.stacked_bar_series = stacked_bar_series;
        set_chart_in_stackwidget_after_setting_of_all_specification(struct_chart_detail_obj);
    }


    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_full_loader::create_graph_messenger_monthly()
{
    if(selected_configured_account_name == QString(Macro_Redefined_Result_Messenger_All_Accounts))
    {
        create_graph_monthly_common_function();
        return;
    }

    recon_static_functions::app_debug("Start",Q_FUNC_INFO);


    progressbar_increment_step = years_list.size() / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    stackwidget_index = 0;
    map_dates_labels.clear();

    ///create pages to according to number of years(if no of years = 5 then 5 pages will get create)
    for(int i = 0; i < years_list.size(); i++)
    {
        if(i % 100 == 0)
        {
            QCoreApplication::processEvents();
        }


        QString year = years_list.at(i);

        map_dates_labels.insert(i, year);

        QStringList categories_list;
        qint64 max_range_for_yaxis = 0;
        QMap<QString, m_barset*> map_list_of_barsets;
        QMap<QString,QString>::iterator iter;
        for(iter = map_of_partipants_with_color.begin() ; iter != map_of_partipants_with_color.end(); iter++)
        {
            m_barset *barset_obj = new m_barset(iter.key());
            barset_obj->setColor(QColor(iter.value()));
            map_list_of_barsets.insert(iter.key(), barset_obj);
        }

        int index_of_x_axis = 0;
        for(int k = 0; k < month_year_list.size(); k++)
        {
            QString mnth_name = month_year_list.at(k);

            if(mnth_name.endsWith(year))
            {

                QString tmp_name = mnth_name;
                tmp_name.remove(tmp_name.indexOf("-"),tmp_name.size());

                categories_list << tmp_name;

                QMap<QString,QString>::iterator iter;

                int max_count_for_y_axis = 0;
                for(iter = map_of_partipants_with_color.begin() ; iter != map_of_partipants_with_color.end(); iter++)
                {
                    QCoreApplication::processEvents();

                    m_barset *barset_obj = map_list_of_barsets.value(iter.key());

                    qint64 count = 0;

                    QList<struct_graphs_essentials> list_of_struct =  map_of_struct_graph_data.values(mnth_name);
                    for(int jj = 0;  jj < list_of_struct.size(); jj++)
                    {
                        struct_graphs_essentials struct_data_database_obj = list_of_struct.at(jj);

                        if(((struct_data_database_obj.sender == selected_configured_account_name) && (struct_data_database_obj.receiver == iter.key()) ) ||
                                ((struct_data_database_obj.sender == iter.key()) && (struct_data_database_obj.receiver == selected_configured_account_name) ))
                        {
                            count++;
                        }
                    }


                    max_count_for_y_axis += count;

                    *barset_obj << count;

                    QString tooltip = prepare_and_get_tooltip_string_for_barset(count, mnth_name, barset_obj->label());
                    QMap<int, QString> map_insert_index_with_category = barset_obj->get_tooltip();
                    map_insert_index_with_category.insert(index_of_x_axis,tooltip);
                    barset_obj->set_tooltip(map_insert_index_with_category);
                    map_list_of_barsets.insert(iter.key(), barset_obj);

                }

                if(max_range_for_yaxis <= max_count_for_y_axis)
                    max_range_for_yaxis = max_count_for_y_axis;

                index_of_x_axis++;
            }


        }

        QStackedBarSeries *stacked_bar_series = new QStackedBarSeries(this);
        connect(stacked_bar_series, SIGNAL(hovered(bool, int, QBarSet *)), this, SLOT(slot_hovered_toolTip_on_barsets(bool, int, QBarSet *)));

        QMap<QString,QString>::iterator iter2;
        for(iter2 = map_of_partipants_with_color.begin() ; iter2 != map_of_partipants_with_color.end(); iter2++)
        {
            m_barset *barset_obj = map_list_of_barsets.value(iter2.key());
            stacked_bar_series->append(barset_obj);
        }

        qint64 roundup_value_yaxis = recon_static_functions::get_round_up_value_near_hundred_or_thousand(max_range_for_yaxis, Q_FUNC_INFO);

        struct_redefined_result_single_chart_detail struct_chart_detail_obj;
        struct_chart_detail_obj.category_list = categories_list;
        struct_chart_detail_obj.roundup_value_axis = roundup_value_yaxis;
        struct_chart_detail_obj.stacked_bar_series = stacked_bar_series;

        set_chart_in_stackwidget_after_setting_of_all_specification(struct_chart_detail_obj);
    }


    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_full_loader::create_graph_messenger_daywise()
{
    if(selected_configured_account_name == QString(Macro_Redefined_Result_Messenger_All_Accounts))
    {
        create_graph_daywise_common_function();
        return;
    }

    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    progressbar_increment_step = month_year_list.size() / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    stackwidget_index = 0;
    map_dates_labels.clear();
    for(int i = 0; i < month_year_list.size(); i++)
    {
        if(i % 100 == 0)
        {
            QCoreApplication::processEvents();
        }


        QString mnth_year = month_year_list.at(i);

        map_dates_labels.insert(i, mnth_year);

        QMap<QString, m_barset*> map_list_of_barsets;
        QMap<QString,QString>::iterator iter;
        for(iter = map_of_partipants_with_color.begin() ; iter != map_of_partipants_with_color.end(); iter++)
        {
            m_barset *barset_obj = new m_barset(iter.key());
            barset_obj->setColor(QColor(iter.value()));
            map_list_of_barsets.insert(iter.key(), barset_obj);
        }

        QStringList categories_list;
        qint64 max_range_for_yaxis = 0;
        int index_for_x_axis = 0;
        for(int k = 0; k < day_month_year_list.size(); k++)
        {
            QString day_mnth_name = day_month_year_list.at(k);
            if(day_mnth_name.endsWith(mnth_year))
            {
                QString tmp_name = day_mnth_name;
                tmp_name.remove(tmp_name.indexOf("-"),tmp_name.size());
                tmp_name.remove(0,tmp_name.lastIndexOf(" ") + 1);

                categories_list << tmp_name;

                QMap<QString,QString>::iterator iter;

                int max_count_for_yaxis = 0;
                for(iter = map_of_partipants_with_color.begin() ; iter != map_of_partipants_with_color.end(); iter++)
                {
                    QCoreApplication::processEvents();

                    m_barset *barset_obj = map_list_of_barsets.value(iter.key());

                    qint64 count = 0;

                    QList<struct_graphs_essentials> list_of_struct =  map_of_struct_graph_data.values(day_mnth_name);
                    for(int jj = 0;  jj < list_of_struct.size(); jj++)
                    {
                        struct_graphs_essentials struct_data_database_obj = list_of_struct.at(jj);

                        if(((struct_data_database_obj.sender == selected_configured_account_name) && (struct_data_database_obj.receiver == iter.key()) ) ||
                                ((struct_data_database_obj.sender == iter.key()) && (struct_data_database_obj.receiver == selected_configured_account_name) ))
                        {
                            count++;
                        }
                    }

                    max_count_for_yaxis += count;

                    *barset_obj << count;

                    QString tooltip = prepare_and_get_tooltip_string_for_barset(count, day_mnth_name, barset_obj->label());

                    QMap<int, QString> map_insert_index_with_category = barset_obj->get_tooltip();
                    map_insert_index_with_category.insert(index_for_x_axis,tooltip);
                    barset_obj->set_tooltip(map_insert_index_with_category);
                    map_list_of_barsets.insert(iter.key(), barset_obj);

                }

                if(max_range_for_yaxis <= max_count_for_yaxis)
                    max_range_for_yaxis = max_count_for_yaxis;

                index_for_x_axis++;
            }
        }

        QStackedBarSeries *stacked_bar_series = new QStackedBarSeries(this);
        connect(stacked_bar_series, SIGNAL(hovered(bool, int, QBarSet *)), this, SLOT(slot_hovered_toolTip_on_barsets(bool, int, QBarSet *)));

        QMap<QString,QString>::iterator iter2;
        for(iter2 = map_of_partipants_with_color.begin() ; iter2 != map_of_partipants_with_color.end(); iter2++)
        {
            m_barset *barset_obj = map_list_of_barsets.value(iter2.key());
            stacked_bar_series->append(barset_obj);
        }

        qint64 roundup_value_yaxis = recon_static_functions::get_round_up_value_near_hundred_or_thousand(max_range_for_yaxis, Q_FUNC_INFO);
        struct_redefined_result_single_chart_detail struct_chart_detail_obj;
        struct_chart_detail_obj.category_list = categories_list;
        struct_chart_detail_obj.roundup_value_axis = roundup_value_yaxis;
        struct_chart_detail_obj.stacked_bar_series = stacked_bar_series;

        set_chart_in_stackwidget_after_setting_of_all_specification(struct_chart_detail_obj);

    }


    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void redefined_result_full_loader::create_graph_messenger_hourly()
{
    if(selected_configured_account_name == QString(Macro_Redefined_Result_Messenger_All_Accounts))
    {
        create_graph_hourly_common_function();
        return;
    }

    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    progressbar_increment_step = day_month_year_list.size() / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    stackwidget_index = 0;
    map_dates_labels.clear();
    for(int i = 0; i < day_month_year_list.size(); i++)
    {
        if(i % 100 == 0)
        {
            QCoreApplication::processEvents();
        }
        if(bool_cancel_loading)
        {
            return;
        }

        QString day_mnth_year = day_month_year_list.at(i);

        map_dates_labels.insert(i, day_mnth_year);

        if(stackwidget_obj->count() >=1)
            pushbutton_next->setEnabled(true);

        QStringList categories_list;
        qint64 max_range_for_yaxis = 0;
        int index_in_x_axis = 0;
        QMap<QString, m_barset*> map_list_of_barsets;
        QMap<QString,QString>::iterator iter;
        for(iter = map_of_partipants_with_color.begin() ; iter != map_of_partipants_with_color.end(); iter++)
        {
            m_barset *barset_obj = new m_barset(iter.key());
            barset_obj->setColor(QColor(iter.value()));
            map_list_of_barsets.insert(iter.key(), barset_obj);
        }

        for(int k = 0; k < hour_day_month_year_list.size(); k++)
        {
            if(bool_cancel_loading)
            {
                return;
            }

            QString hr_day_mnth_name = hour_day_month_year_list.at(k);

            if(hr_day_mnth_name.startsWith(day_mnth_year))
            {
                QString tmp_name = hr_day_mnth_name;
                tmp_name.remove(0,tmp_name.lastIndexOf(" ") + 1);

                categories_list << tmp_name;

                QMap<QString,QString>::iterator iter;

                int max_count = 0;
                for(iter = map_of_partipants_with_color.begin() ; iter != map_of_partipants_with_color.end(); iter++)
                {
                    if(bool_cancel_loading)
                    {
                        return;
                    }
                    QCoreApplication::processEvents();

                    qint64 count = 0;


                    QList<struct_graphs_essentials> list_of_struct =  map_of_struct_graph_data.values(hr_day_mnth_name);
                    for(int jj = 0;  jj < list_of_struct.size(); jj++)
                    {
                        if(bool_cancel_loading)
                        {
                            return;
                        }

                        struct_graphs_essentials struct_data_database_obj = list_of_struct.at(jj);

                        if(((struct_data_database_obj.sender == selected_configured_account_name) && (struct_data_database_obj.receiver == iter.key()) ) ||
                                ((struct_data_database_obj.sender == iter.key()) && (struct_data_database_obj.receiver == selected_configured_account_name) ))
                        {
                            count++;
                        }
                    }


                    m_barset *barset_obj = map_list_of_barsets.value(iter.key());
                    *barset_obj << count;

                    QMap<int, QString> map_insert_index_with_category = barset_obj->get_tooltip();

                    QString tooltip = prepare_and_get_tooltip_string_for_barset(count, hr_day_mnth_name, barset_obj->label());
                    map_insert_index_with_category.insert(index_in_x_axis,tooltip);
                    barset_obj->set_tooltip(map_insert_index_with_category);
                    map_list_of_barsets.insert(iter.key(), barset_obj);

                    max_count += count;
                }

                if(max_range_for_yaxis <= max_count)
                    max_range_for_yaxis = max_count;

                index_in_x_axis++;
            }


        }

        QStackedBarSeries *stacked_bar_series = new QStackedBarSeries(this);
        connect(stacked_bar_series, SIGNAL(hovered(bool, int, QBarSet *)), this, SLOT(slot_hovered_toolTip_on_barsets(bool, int, QBarSet *)));

        QMap<QString,QString>::iterator iter2;
        for(iter2 = map_of_partipants_with_color.begin() ; iter2 != map_of_partipants_with_color.end(); iter2++)
        {
            m_barset *barset_obj = map_list_of_barsets.value(iter2.key());
            stacked_bar_series->append(barset_obj);
        }

        qint64 roundup_value_yaxis = recon_static_functions::get_round_up_value_near_hundred_or_thousand(max_range_for_yaxis, Q_FUNC_INFO);

        struct_redefined_result_single_chart_detail struct_chart_detail_obj;
        struct_chart_detail_obj.category_list = categories_list;
        struct_chart_detail_obj.roundup_value_axis = roundup_value_yaxis;
        struct_chart_detail_obj.stacked_bar_series = stacked_bar_series;

        if(bool_cancel_loading)
        {
            return;
        }

        set_chart_in_stackwidget_after_setting_of_all_specification(struct_chart_detail_obj);

        label_show_date_time->setText(map_dates_labels.value(stackwidget_index));
        label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));

    }



    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void redefined_result_full_loader::create_piechart_for_selected_account_for_messenger()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);


    QChart *chart_pie_obj = new QChart();
    chart_pie_obj->setAnimationOptions(QChart::SeriesAnimations);

    if(selected_configured_account_name == Macro_Redefined_Result_Messenger_All_Accounts)
    {
        chartView_for_pie_or_visit_count_obj->setChart(chart_pie_obj);
        chart_pie_obj->setTitle(QString("Please select an account"));

        QBrush bursh = QBrush(Qt::red);
        chart_pie_obj->setTitleBrush(bursh);

        return;
    }

    QSqlDatabase::removeDatabase(Q_FUNC_INFO );
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(destination_db_redefined_result);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("----FAILED--- to open database:" + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("--Error--:" + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QPieSeries *pie_series = new QPieSeries(this);
    connect(pie_series, SIGNAL(hovered(QPieSlice *, bool)), this, SLOT(slot_hovered_toolTip_on_pieslices(QPieSlice *, bool)));
    pie_series->setPieSize(0.9);


    for(int i = 0; i < tablewidget_participants_list->rowCount() ; i++)
    {
        if(i%10 == 0)
            QCoreApplication::processEvents();

        QString participant_name = tablewidget_participants_list->item(i,0)->text();
        QString color_name = tablewidget_participants_list->item(i,0)->toolTip();

        if(participant_name == selected_configured_account_name)
            continue;

        QString command =  "select COUNT(*) from redefined_result where ((sender = ? AND receiver LIKE ?) OR (sender = ? AND receiver LIKE ?))";
        command.append(" AND timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC");

        QStringList values;
        values << selected_configured_account_name << participant_name << participant_name <<selected_configured_account_name <<QString::number( start_timestamp_int) << QString::number(end_timestamp_int);

        qint64 count = recon_helper_standard_obj->get_intvalue_from_db_with_addbindvalues_by_dbreference(command, source_db, 0, values, Q_FUNC_INFO);

        if(count == 0)
            continue;


        QFont font("Arial", 8);
        QPieSlice *slice = new QPieSlice(this);
        connect(slice, SIGNAL(doubleClicked()), this, SLOT(slot_pie_slice_doubleClicked()));
        connect(slice, SIGNAL(pressed()), this, SLOT(slot_pie_slice_pressed()));

        slice->setValue(count);

        slice->setBorderColor(QColor("transparent"));

        slice->setLabelFont(font);
        slice->setColor(QColor(color_name));
        slice->setLabel(participant_name);

        pie_series->append(slice);
    }

    QList<QPieSlice *>list = pie_series->slices();

    for(int i = 0; i < list.size(); i++)
    {
        QPieSlice *tmp_slice = list.at(i);
        QString label = tmp_slice->label();
        label += " , ";
        label += QString::number(tmp_slice->percentage() * 100, 'f', 2);
        label += "%";

        tmp_slice->setLabel(label);
    }


    chart_pie_obj->addSeries(pie_series);
    chart_pie_obj->setTitle(QString("Chat of Account '" + selected_configured_account_name + "'"));
    //chart_pie_obj->legend()->setVisible(false);
    chart_pie_obj->legend()->setAlignment(Qt::AlignRight);


    chartView_for_pie_or_visit_count_obj->setChart(chart_pie_obj);
    source_db.close();


    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void redefined_result_full_loader::create_graphs_browsers_history()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    create_graph_browser_history_top_visit_count_domains();

    radiobutton_create_graph_yearly->setChecked(true);

    int count_stacks = stackwidget_obj->count();

    if(count_stacks > 0)
    {
        for(int i = (count_stacks-1); i >= 0; i--)
        {
            stackwidget_obj->removeWidget(stackwidget_obj->widget(i));
            delete stackwidget_obj->widget(i);
        }
    }

    label_show_date_time->clear();
    label_show_pages->clear();
    stackwidget_index = 0;

    radiobutton_create_graph_yearly->setDisabled(true);
    radiobutton_create_graph_monthly->setDisabled(true);
    radiobutton_create_graph_daywise->setDisabled(true);
    radiobutton_create_graph_hourly->setDisabled(true);
    tablewidget_accounts_configure_list->setDisabled(true);
    tablewidget_participants_list->setDisabled(true);

    map_of_date_lables_map.clear();
    map_of_chartview.clear();
    list_of_charview.clear();

    create_graph_yearly_common_function();
    map_of_chartview.insert(enum_redefined_result_graphs_yearly,list_of_charview);
    map_of_date_lables_map.insert(enum_redefined_result_graphs_yearly,map_dates_labels);
    if(radiobutton_create_graph_yearly->isChecked())
    {
        list_of_charview = map_of_chartview.value(enum_redefined_result_graphs_yearly);
        map_dates_labels = map_of_date_lables_map.value(enum_redefined_result_graphs_yearly);

        for(int i = 0; i < list_of_charview.size();i++)
        {
            stackwidget_obj->addWidget(list_of_charview.at(i));

            if(stackwidget_obj->count() > 1)
                pushbutton_next->setEnabled(true);

            label_show_date_time->setText(map_dates_labels.value(stackwidget_index));
            label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));
            stackwidget_obj->setCurrentIndex(stackwidget_index);

        }

    }

    list_of_charview.clear();
    create_graph_monthly_common_function();
    map_of_chartview.insert(enum_redefined_result_graphs_monthly,list_of_charview);
    map_of_date_lables_map.insert(enum_redefined_result_graphs_monthly,map_dates_labels);
    if(radiobutton_create_graph_monthly->isChecked())
    {

        list_of_charview = map_of_chartview.value(enum_redefined_result_graphs_monthly);
        map_dates_labels = map_of_date_lables_map.value(enum_redefined_result_graphs_monthly);
        for(int i = 0; i < list_of_charview.size();i++)
        {
            stackwidget_obj->addWidget(list_of_charview.at(i));

            if(stackwidget_obj->count() > 1)
                pushbutton_next->setEnabled(true);

            label_show_date_time->setText(map_dates_labels.value(stackwidget_index));
            label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));
            stackwidget_obj->setCurrentIndex(stackwidget_index);

        }

    }

    list_of_charview.clear();
    create_graph_daywise_common_function();
    map_of_chartview.insert(enum_redefined_result_graphs_daywise,list_of_charview);
    map_of_date_lables_map.insert(enum_redefined_result_graphs_daywise,map_dates_labels);
    if(radiobutton_create_graph_daywise->isChecked())
    {

        map_dates_labels = map_of_date_lables_map.value(enum_redefined_result_graphs_daywise);
        list_of_charview = map_of_chartview.value(enum_redefined_result_graphs_daywise);
        for(int i = 0; i < list_of_charview.size();i++)
        {
            stackwidget_obj->addWidget(list_of_charview.at(i));

            if(stackwidget_obj->count() > 1)
                pushbutton_next->setEnabled(true);

            label_show_date_time->setText(map_dates_labels.value(stackwidget_index));
            label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));
            stackwidget_obj->setCurrentIndex(stackwidget_index);

        }

    }

    list_of_charview.clear();

    radiobutton_create_graph_yearly->setDisabled(false);
    radiobutton_create_graph_monthly->setDisabled(false);
    radiobutton_create_graph_daywise->setDisabled(false);
    radiobutton_create_graph_hourly->setDisabled(false);

    // stackwidget->setCurrentIndex(stackwidget_index);
    if(stackwidget_obj->count() <= 1)
    {
        pushbutton_previous->setEnabled(false);
        pushbutton_next->setEnabled(false);

    }else
    {
        pushbutton_previous->setEnabled(false);
        pushbutton_next->setEnabled(true);

    }


    tablewidget_accounts_configure_list->setDisabled(false);
    tablewidget_participants_list->setDisabled(false);

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_full_loader::create_graph_browser_history_top_visit_count_domains()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    int index_in_y_axis = 0;

    if(table_name_list.isEmpty())
        return;

    QString table = table_name_list.at(0);

    QString command = "select url From '" + table + "'";
    command.append(" where timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC");

    QStringList list;
    list << QString::number(start_timestamp_int) << QString::number(end_timestamp_int);

    ///get all urls list from database
    QStringList url_strlist = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command,list, 0, destination_db_redefined_result,Q_FUNC_INFO);
    QStringList domain_list;
    QStringList unique_domain_list;

    for(int i = 0; i < url_strlist.size(); i++)
    {
        QString str = url_strlist.at(i);
        if(str.trimmed().isEmpty())
            continue;

        QUrl url(str);
        if(url.host().trimmed().isEmpty())
            continue;

        if(!unique_domain_list.contains(url.host()))
            unique_domain_list << url.host();

        domain_list << url.host();

    }

    QMultiMap<int,QString> map_of_all_visit_count_and_domain;
    for(int i = 0; i < unique_domain_list.size(); i++)
    {
        QString domain_str = unique_domain_list.at(i);
        int count = domain_list.count(domain_str);

        map_of_all_visit_count_and_domain.insert(count,domain_str);
    }


    int top_visit_count = 0;
    QMultiMap<int,QString> map_of_top_20_visit_count;
    QList<int> list_of_all_visit_count_keys = map_of_all_visit_count_and_domain.uniqueKeys();///4,2,8,7,3
    std::sort(list_of_all_visit_count_keys.begin(), list_of_all_visit_count_keys.end());///2,3,4,7,8,9 - sorted order
    for(int i = list_of_all_visit_count_keys.size() - 1; i > 0; i--)
    {
        QList<QString> list = map_of_all_visit_count_and_domain.values(list_of_all_visit_count_keys.at(i));

        ///key = 120
        /// values = www.google.com, www.gmail.com,www.linkedin.com
        if(top_visit_count == 20)
            break;


        for(int j = 0; j < list.size(); j++)
        {
            map_of_top_20_visit_count.insert(list_of_all_visit_count_keys.at(i),list.at(j));

            ///insert in map like this
            ///key = 120,value = www.google.com
            ///key = 120,value = www.gmail.com
            ///key = 120,value = www.linkedin.com

            if(top_visit_count == 20)
                break;

            top_visit_count++;
        }

    }


    m_barset *barset_obj = new m_barset(QString("URL"));
    connect(barset_obj, SIGNAL(doubleClicked(int)), this, SLOT(slot_barset_doubleClicked(int)));
    connect(barset_obj, SIGNAL(pressed(int)), this, SLOT(slot_barset_pressed(int)));

    barset_obj->setColor(QColor("purple"));
    QStringList categories_list;

    QMap<int, QString> map_insert_index_with_category;
    int max_range_for_x_axis = 0;

    QMultiMap<int,QString>::iterator iter_map1;
    for(iter_map1 = map_of_top_20_visit_count.begin(); iter_map1 != map_of_top_20_visit_count.end(); iter_map1++)
    {

        categories_list << iter_map1.value();

        int count = iter_map1.key();

        *barset_obj << count;

        map_insert_index_with_category.insert(index_in_y_axis,iter_map1.value());

        if(max_range_for_x_axis <= count)
            max_range_for_x_axis = count;

        index_in_y_axis++;

    }

    QHorizontalBarSeries* horizontal_bar_series_obj = new QHorizontalBarSeries(this);
    connect(horizontal_bar_series_obj, SIGNAL(hovered(bool, int, QBarSet *)), this, SLOT(slot_hovered_toolTip_on_barsets(bool, int, QBarSet *)));
    barset_obj->set_tooltip(map_insert_index_with_category);
    horizontal_bar_series_obj->append(barset_obj);
    qint64 roundup_value_x_axis = recon_static_functions::get_round_up_value_near_hundred_or_thousand(max_range_for_x_axis, Q_FUNC_INFO);


    QBarCategoryAxis *barcategory_axis_obj = new QBarCategoryAxis(this);
    barcategory_axis_obj->append(categories_list);
    barcategory_axis_obj->setLabelsColor(Qt::darkGray);

    QFont font;
    font.setPixelSize(14);

    barcategory_axis_obj->setLabelsFont(font);

    // barcategory_axis_obj->hide();

    horizontal_bar_series_obj->setBarWidth(0.70);

    QChart *chart_obj = new QChart();
    chart_obj->setTitle("Top Visited Domains");
    chart_obj->setAnimationOptions(QChart::SeriesAnimations);

    chart_obj->addSeries(horizontal_bar_series_obj);

    chart_obj->setAxisY(barcategory_axis_obj, horizontal_bar_series_obj);
    chart_obj->legend()->setVisible(false);

    QValueAxis *value_axis_x = new QValueAxis;
    value_axis_x->setRange(0,roundup_value_x_axis);
    value_axis_x->setLabelFormat("%i");
    value_axis_x->setTitleText("Visit Count");

    chart_obj->setAxisX(value_axis_x, horizontal_bar_series_obj);
    chart_obj->legend()->setAlignment(Qt::AlignBottom);
    chartView_for_pie_or_visit_count_obj->setChart(chart_obj);


    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_full_loader::display_data_of_graph_in_tablewidget()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    redefined_result_bookmark_index = 0;
    record_no_index = 3;
    plugin_name_index = 4;
    tab_name_index = 5;
    os_name_index = 6;
    source_count_name_index = 7;
    visit_count_index = 11;

    redefined_result_examiner_notes_index = 12;

    m_tablewidget_obj->hideColumn(tab_name_index);
    m_tablewidget_obj->hideColumn(os_name_index);
    m_tablewidget_obj->hideColumn(source_count_name_index);
    m_tablewidget_obj->hideColumn(redefined_result_examiner_notes_index);




    destination_db_path = destination_db_redefined_result;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(destination_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        bool_display_table_data_loaded = false;
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    QSqlQuery query_index(*destination_db);

    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR && tabwidget_redefined_result->currentIndex() == enum_redefined_result_tab_pie_or_visit_count_graph)
    {
        bool_save_data_for_graphs_tablewidet = false;
        prepare_sqlquery_for_visit_count_tablewidget(query_index);
    }
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR && tabwidget_redefined_result->currentIndex() == enum_redefined_result_tab_pie_or_visit_count_graph)
    {
        bool_save_data_for_graphs_tablewidet = false;
        prepare_sqlquery_for_piegraph_tablewidget(query_index);
    }
    else
    {
        bool_save_data_for_graphs_tablewidet = true;
        prepare_sqlquery_for_all_graphs_tablewidget(query_index);
    }

    populate_data_of_graph_in_tablewidget(query_index);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

void redefined_result_full_loader::prepare_sqlquery_for_visit_count_tablewidget(QSqlQuery &query_index)
{

    /// prepairing Query from CSV Parsed data (Select "tabs" from "table")
    QString db_cmd;

    if(!db_columns_list.isEmpty())
        db_cmd = QString("Select ");

    for(int i = 0; i < db_columns_list.size(); i++)
    {
        db_cmd.append(db_columns_list.at(i));
        db_cmd.append(QString(","));
    }

    if(db_cmd.endsWith(QString(",")))
        db_cmd.chop(1);

    if(!table_name_list.isEmpty())
    {
        db_cmd.append(QString(" From '"));
        db_cmd.append(table_name_list.at(0) + "'");
    }


    db_cmd.append("where url LIKE ?");

    QString command_time = " AND timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC";

    db_cmd.append(command_time);
    command_for_table_display = db_cmd;

    query_index.prepare(command_for_table_display);

    query_index.addBindValue(QString("%//") + current_clicked_domain + QString("%"));

    query_index.addBindValue(QString::number(start_timestamp_int));
    query_index.addBindValue(QString::number(end_timestamp_int));



}

void redefined_result_full_loader::prepare_sqlquery_for_piegraph_tablewidget(QSqlQuery &query_index)
{

    QString db_cmd;

    if(!db_columns_list.isEmpty())
        db_cmd = QString("Select ");

    for(int i = 0; i < db_columns_list.size(); i++)
    {
        db_cmd.append(db_columns_list.at(i));
        db_cmd.append(QString(","));
    }


    if(db_cmd.endsWith(QString(",")))
        db_cmd.chop(1);

    if(!table_name_list.isEmpty())
    {
        db_cmd.append(QString(" From '"));
        db_cmd.append(table_name_list.at(0) + "'");
    }

    db_cmd.append("where ((sender = ? AND receiver = ?) OR (sender = ? AND receiver = ?)) AND ");

    QString command_time = " timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC";

    db_cmd.append(command_time);
    command_for_table_display = db_cmd;


    query_index.prepare(command_for_table_display);


    query_index.addBindValue(selected_configured_account_name);
    query_index.addBindValue(current_barset_participant);
    query_index.addBindValue(current_barset_participant);
    query_index.addBindValue(selected_configured_account_name);

    query_index.addBindValue(QString::number(start_timestamp_int));
    query_index.addBindValue(QString::number(end_timestamp_int));


}

void redefined_result_full_loader::prepare_sqlquery_for_all_graphs_tablewidget(QSqlQuery &query_index)
{
    QString db_cmd;


    if(!db_columns_list.isEmpty())
        db_cmd = QString("Select ");

    for(int i = 0; i < db_columns_list.size(); i++)
    {
        db_cmd.append(db_columns_list.at(i));
        db_cmd.append(QString(","));
    }

    if(db_cmd.endsWith(QString(",")))
        db_cmd.chop(1);

    if(!table_name_list.isEmpty())
    {
        db_cmd.append(QString(" From '"));
        db_cmd.append(table_name_list.at(0) + "'");
    }


    if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
    {

        if(tablewidget_accounts_configure_list->currentItem()->text() == Macro_Redefined_Result_Messenger_All_Accounts)
        {
            db_cmd.append("where ");
        }else
        {
            db_cmd.append("where ((sender = ? AND receiver = ?) OR (sender = ? AND receiver = ?)) AND ");
        }
    }else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
    {
        db_cmd.append("where ");
    }

    QString graph_type;///get column name for selected graph type.so that we can extract particular data from database
    if(radiobutton_create_graph_yearly->isChecked())
        graph_type = "year_str";

    if(radiobutton_create_graph_monthly->isChecked())
        graph_type = "month_year";

    if(radiobutton_create_graph_daywise->isChecked())
        graph_type = "day_month_year";

    if(radiobutton_create_graph_hourly->isChecked())
        graph_type = "hour_day_month_year";

    QString command_type =  graph_type + " = ? ";
    db_cmd.append(command_type) ;

    QString command_time = " AND timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC";

    db_cmd.append(command_time);
    command_for_table_display = db_cmd;

    query_index.prepare(command_for_table_display);


    if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
    {
        if(tablewidget_accounts_configure_list->currentItem()->text() != Macro_Redefined_Result_Messenger_All_Accounts)
        {
            query_index.addBindValue(selected_configured_account_name);
            query_index.addBindValue(current_barset_participant);
            query_index.addBindValue(current_barset_participant);
            query_index.addBindValue(selected_configured_account_name);
        }
    }

    QString category_str = current_barset_category_str;

    if(category_str.endsWith(" hr"))
        category_str.chop(3);

    query_index.addBindValue(category_str);

    query_index.addBindValue(QString::number(start_timestamp_int));
    query_index.addBindValue(QString::number(end_timestamp_int));


}

int redefined_result_full_loader::populate_data_of_graph_in_tablewidget(QSqlQuery &query_index)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        populate_data_of_graph_in_tablewidget_browser_history(query_index);
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        populate_data_of_graph_in_tablewidget_messenger(query_index);

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}


void redefined_result_full_loader::set_data_in_configured_accounts_list()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    QString online_accounts_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + "Online_Accounts/Online_Accounts.sqlite";

    while (tablewidget_accounts_configure_list->rowCount() > 0)
    {
        tablewidget_accounts_configure_list->removeRow(0);
    }

    tablewidget_accounts_configure_list->setColumnCount(1);
    tablewidget_accounts_configure_list->verticalHeader()->hide();
    QStringList header_receiver;
    header_receiver << "Accounts";
    tablewidget_accounts_configure_list->setHorizontalHeaderLabels(header_receiver);

    QStringList configured_accounts_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath("SELECT account_ID FROM accounts",0,online_accounts_db_path,Q_FUNC_INFO);
    configured_accounts_list.removeDuplicates();


    QStringList all_participants_list;
    QString cmm = "Select distinct sender,receiver from redefined_result where timestamp != '' AND timestamp not NULL";
    all_participants_list <<  recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmm,0,destination_db_redefined_result,Q_FUNC_INFO);
    all_participants_list <<  recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmm,1,destination_db_redefined_result,Q_FUNC_INFO);
    all_participants_list.removeDuplicates();
    all_participants_list.removeAll("");
    QStringList account_list_only_for_messenger;
    for(int ii = 0 ; ii < all_participants_list.size() ; ii++)
    {
        QString participants_str = all_participants_list.at(ii);
        for(int jj = 0 ; jj < configured_accounts_list.size() ; jj++)
        {
            QString account_id_str = configured_accounts_list.at(jj);
            if(participants_str == account_id_str)
            {
                account_list_only_for_messenger << account_id_str;
            }
        }
    }
    configured_accounts_list = account_list_only_for_messenger;
    configured_accounts_list.removeDuplicates();


    map_of_accounts_with_particiapnts.clear();

    int j = 0;
    for(int i = 0; i < configured_accounts_list.size() ; i++)
    {
        if(configured_accounts_list.at(i).trimmed().isEmpty())
            continue;

        QString name = configured_accounts_list.at(i);
        map_of_accounts_with_particiapnts.insert(name, get_participant_list_of_account(name));

        if(j == 0)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            item->setText(QString(Macro_Redefined_Result_Messenger_All_Accounts));

            // item->setCheckState(Qt::Unchecked);
            tablewidget_accounts_configure_list->insertRow(j);
            tablewidget_accounts_configure_list->setRowHeight(j,21);
            tablewidget_accounts_configure_list->setItem(j,0,item);

        }
        j++;

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        item->setText(name);

        // item->setCheckState(Qt::Unchecked);
        tablewidget_accounts_configure_list->insertRow(j);
        tablewidget_accounts_configure_list->setRowHeight(j,21);
        tablewidget_accounts_configure_list->setItem(j,0,item);


    }

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

QStringList redefined_result_full_loader::get_participant_list_of_account(QString account_name)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QStringList listof_all_participants;
    QSqlDatabase::removeDatabase(Q_FUNC_INFO );
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(destination_db_redefined_result);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("----FAILED--- to open database:" + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("--Error--:" + source_db.lastError().text(),Q_FUNC_INFO);
        return listof_all_participants;
    }

    QSqlQuery query(source_db);
    query.prepare("SELECT sender, receiver FROM redefined_result where sender = ? OR receiver = ? ORDER BY timestamp ASC");
    query.addBindValue(account_name);
    query.addBindValue(account_name);

    if(!query.exec())
    {
        recon_static_functions::app_debug("query.exec()  ------FAILED-----" + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("--Error---" + query.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug("Command - " +  query.executedQuery(),Q_FUNC_INFO);
        source_db.close();
        return listof_all_participants;

    }

    map_of_partipants_with_color.clear();
    listof_all_participants.clear();
    while(query.next())
    {
        QString sender = query.value(0).toString().trimmed();
        QString receiver = query.value(1).toString().trimmed();

        if(sender.trimmed().isEmpty() && receiver.trimmed().isEmpty())
            continue;

        if(!listof_all_participants.contains(sender))
            listof_all_participants << sender;

        if(receiver.contains(","))
        {
            QStringList list = receiver.split(",", Qt::SkipEmptyParts);
            for(int i = 0; i < list.size(); i++)
            {
                QString tmp_str = list.at(i);
                tmp_str = tmp_str.trimmed();
                if(!listof_all_participants.contains(tmp_str))
                    listof_all_participants << tmp_str;
            }
        }
        else
        {
            if(!listof_all_participants.contains(receiver))
                listof_all_participants << receiver;
        }
    }


    listof_all_participants.removeOne(account_name);

    source_db.close();
    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
    return listof_all_participants;
}

void redefined_result_full_loader::create_graph_yearly_common_function()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    map_dates_labels.clear();
    ///create pages to split years if year list is greater than 5
    int no_of_years_on_single_page = 5;
    int divednd_result = 0;
    if(years_list.size() > no_of_years_on_single_page)
    {
        qint64 size = years_list.size();
        int remainder = size % no_of_years_on_single_page;
        divednd_result = size/no_of_years_on_single_page;
        if(remainder != 0)
            divednd_result++;
    }

    int no_of_pages = 1;//By Default No of Pages are 1

    if(divednd_result > 0)
        no_of_pages = divednd_result;

    int tmp_year_count = 0;

    map_dates_labels.clear();
    for(int page_i = 0; page_i < no_of_pages; page_i++)
    {
        QStringList categories_list;
        qint64 max_range_for_yaxis = 0;
        m_barset *barset_obj = new m_barset(QString(Macro_Redefined_Result_Messenger_All_Accounts));
        barset_obj->setColor(Qt::red);
        QMap<int, QString> map_insert_index_with_category;
        QStringList list_of_years_in_single_page;

        if(tmp_year_count < years_list.size())
        {
            list_of_years_in_single_page= years_list.mid(tmp_year_count,no_of_years_on_single_page);
            tmp_year_count+=no_of_years_on_single_page;
        }
        else
        {
            list_of_years_in_single_page= years_list.mid(tmp_year_count,years_list.size());
        }

        if(list_of_years_in_single_page.size() > 1)
        {
            QString year_label = list_of_years_in_single_page.at(0) + "-" + list_of_years_in_single_page.at(list_of_years_in_single_page.size() - 1);
            map_dates_labels.insert(page_i, year_label);
        }

        int index_in_x_axis = 0;
        for(int k = 0; k < list_of_years_in_single_page.size(); k++)
        {
            QString yr_name = list_of_years_in_single_page.at(k);
            categories_list << yr_name;

            QCoreApplication::processEvents();

            QList<struct_graphs_essentials> list_of_struct =  map_of_struct_graph_data.values(yr_name);
            qint64 count = list_of_struct.size();

            *barset_obj << count;

            QString tooltip = prepare_and_get_tooltip_string_for_barset(count, yr_name, barset_obj->label());
            map_insert_index_with_category.insert(index_in_x_axis,tooltip);

            if(max_range_for_yaxis <= count)
                max_range_for_yaxis = count;

            index_in_x_axis++;

        }

        QStackedBarSeries *stacked_bar_series = new QStackedBarSeries(this);
        connect(stacked_bar_series, SIGNAL(hovered(bool, int, QBarSet *)), this, SLOT(slot_hovered_toolTip_on_barsets(bool, int, QBarSet *)));
        barset_obj->set_tooltip(map_insert_index_with_category);
        stacked_bar_series->append(barset_obj);
        qint64 roundup_value_yaxis = recon_static_functions::get_round_up_value_near_hundred_or_thousand(max_range_for_yaxis, Q_FUNC_INFO);

        struct_redefined_result_single_chart_detail struct_chart_detail_obj;
        struct_chart_detail_obj.category_list = categories_list;
        struct_chart_detail_obj.roundup_value_axis = roundup_value_yaxis;
        struct_chart_detail_obj.stacked_bar_series = stacked_bar_series;

        set_chart_in_stackwidget_after_setting_of_all_specification(struct_chart_detail_obj);

    }



    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_full_loader::create_graph_monthly_common_function()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);


    progressbar_increment_step = years_list.size() / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    map_dates_labels.clear();
    for(int i = 0; i < years_list.size(); i++)
    {
        if(i % 100 == 0)
        {
            QCoreApplication::processEvents();
        }

        QString year = years_list.at(i);

        map_dates_labels.insert(i, year);

        int index_in_x_axis = 0;

        QStringList categories_list;
        qint64 max_range_for_yaxis = 0;

        m_barset *barset_obj = new m_barset(QString(Macro_Redefined_Result_Messenger_All_Accounts));
        barset_obj->setColor(Qt::red);
        QMap<int, QString> map_insert_index_with_category;
        for(int k = 0; k < month_year_list.size(); k++)
        {
            QString mnth_name = month_year_list.at(k);

            if(mnth_name.endsWith(year))
            {
                QString tmp_name = mnth_name;
                tmp_name.remove(tmp_name.indexOf("-"),tmp_name.size());
                categories_list << tmp_name;

                QCoreApplication::processEvents();


                QList<struct_graphs_essentials> list_of_struct =  map_of_struct_graph_data.values(mnth_name);
                qint64 count = list_of_struct.size();
                *barset_obj << count;

                QString tooltip = prepare_and_get_tooltip_string_for_barset(count, mnth_name, barset_obj->label());
                map_insert_index_with_category.insert(index_in_x_axis,tooltip);

                if(max_range_for_yaxis <= count)
                    max_range_for_yaxis = count;

                index_in_x_axis++;
            }


        }

        barset_obj->set_tooltip(map_insert_index_with_category);

        QStackedBarSeries *stacked_bar_series = new QStackedBarSeries(this);
        connect(stacked_bar_series, SIGNAL(hovered(bool, int, QBarSet *)), this, SLOT(slot_hovered_toolTip_on_barsets(bool, int, QBarSet *)));
        stacked_bar_series->append(barset_obj);

        qint64 roundup_value_yaxis = recon_static_functions::get_round_up_value_near_hundred_or_thousand(max_range_for_yaxis, Q_FUNC_INFO);
        struct_redefined_result_single_chart_detail struct_chart_detail_obj;
        struct_chart_detail_obj.category_list = categories_list;
        struct_chart_detail_obj.roundup_value_axis = roundup_value_yaxis;
        struct_chart_detail_obj.stacked_bar_series = stacked_bar_series;

        set_chart_in_stackwidget_after_setting_of_all_specification(struct_chart_detail_obj);
    }



    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_full_loader::create_graph_daywise_common_function()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);


    progressbar_increment_step = month_year_list.size() / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    map_dates_labels.clear();
    for(int i = 0; i < month_year_list.size(); i++)
    {
        if(i % 100 == 0)
        {
            QCoreApplication::processEvents();
        }


        QString mnth_year = month_year_list.at(i);

        map_dates_labels.insert(i, mnth_year);

        QStringList categories_list;
        qint64 max_range_for_yaxis = 0;
        m_barset *barset_obj = new m_barset(Macro_Redefined_Result_Messenger_All_Accounts);
        barset_obj->setColor(Qt::red);

        QMap<int, QString> map_insert_index_with_category;

        int index_in_x_axis = 0;
        for(int k = 0; k < day_month_year_list.size(); k++)
        {
            QCoreApplication::processEvents();
            QString day_mnth_name = day_month_year_list.at(k);

            if(day_mnth_name.endsWith(mnth_year))
            {
                QString tmp_name = day_mnth_name;
                tmp_name.remove(tmp_name.indexOf("-"),tmp_name.size());
                tmp_name.remove(0,tmp_name.lastIndexOf(" ") + 1);

                categories_list << tmp_name;

                QCoreApplication::processEvents();

                QList<struct_graphs_essentials> list_of_struct =  map_of_struct_graph_data.values(day_mnth_name);
                qint64 count = list_of_struct.size();

                *barset_obj << count;

                QString tooltip = prepare_and_get_tooltip_string_for_barset(count, day_mnth_name, barset_obj->label());
                map_insert_index_with_category.insert(index_in_x_axis,tooltip);

                if(max_range_for_yaxis <= count)
                    max_range_for_yaxis = count;

                index_in_x_axis++;
            }


        }

        barset_obj->set_tooltip(map_insert_index_with_category);

        QStackedBarSeries *stacked_bar_series = new QStackedBarSeries(this);
        connect(stacked_bar_series, SIGNAL(hovered(bool, int, QBarSet *)), this, SLOT(slot_hovered_toolTip_on_barsets(bool, int, QBarSet *)));
        stacked_bar_series->append(barset_obj);

        qint64 roundup_value_yaxis = recon_static_functions::get_round_up_value_near_hundred_or_thousand(max_range_for_yaxis, Q_FUNC_INFO);

        struct_redefined_result_single_chart_detail struct_chart_detail_obj;
        struct_chart_detail_obj.category_list = categories_list;
        struct_chart_detail_obj.roundup_value_axis = roundup_value_yaxis;
        struct_chart_detail_obj.stacked_bar_series = stacked_bar_series;

        set_chart_in_stackwidget_after_setting_of_all_specification(struct_chart_detail_obj);

    }



    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void redefined_result_full_loader::create_graph_hourly_common_function()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);


    progressbar_increment_step = day_month_year_list.size() / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;

    map_dates_labels.clear();
    for(int i = 0; i < day_month_year_list.size(); i++)
    {
        if(bool_cancel_loading)
        {
            return;
        }

        if(i % 100 == 0)
        {
            QCoreApplication::processEvents();
        }


        QString day_mnth_year = day_month_year_list.at(i);

        map_dates_labels.insert(i, day_mnth_year);

        if(stackwidget_obj->count() >=1)
            pushbutton_next->setEnabled(true);

        QStringList categories_list;
        qint64 max_range_for_yaxis = 0;

        m_barset *barset_obj = new m_barset(Macro_Redefined_Result_Messenger_All_Accounts);
        barset_obj->setColor(Qt::red);

        QMap<int, QString> map_insert_index_with_category;

        int index_in_x_axis = 0;
        for(int k = 0; k < hour_day_month_year_list.size(); k++)
        {
            if(bool_cancel_loading)
            {
                return;
            }
            QString hr_day_mnth_name = hour_day_month_year_list.at(k);

            if(hr_day_mnth_name.startsWith(day_mnth_year))
            {
                QString tmp_name = hr_day_mnth_name;
                tmp_name.remove(0,tmp_name.lastIndexOf(" ") + 1);
                categories_list << tmp_name;

                QCoreApplication::processEvents();

                QList<struct_graphs_essentials> list_of_struct =  map_of_struct_graph_data.values(hr_day_mnth_name);
                qint64 count = list_of_struct.size();


                *barset_obj << count;

                QString tooltip = prepare_and_get_tooltip_string_for_barset(count, QString(hr_day_mnth_name + " hr") , barset_obj->label());
                map_insert_index_with_category.insert(index_in_x_axis,tooltip);


                if(max_range_for_yaxis <= count)
                    max_range_for_yaxis = count;

                index_in_x_axis++;
            }

        }

        QStackedBarSeries *stacked_bar_series = new QStackedBarSeries(this);
        connect(stacked_bar_series, SIGNAL(hovered(bool, int, QBarSet *)), this, SLOT(slot_hovered_toolTip_on_barsets(bool, int, QBarSet *)));
        stacked_bar_series->append(barset_obj);
        barset_obj->set_tooltip(map_insert_index_with_category);

        qint64 roundup_value_yaxis = recon_static_functions::get_round_up_value_near_hundred_or_thousand(max_range_for_yaxis, Q_FUNC_INFO);

        struct_redefined_result_single_chart_detail struct_chart_detail_obj;
        struct_chart_detail_obj.category_list = categories_list;
        struct_chart_detail_obj.roundup_value_axis = roundup_value_yaxis;
        struct_chart_detail_obj.stacked_bar_series = stacked_bar_series;

        if(bool_cancel_loading)
        {
            return;
        }
        set_chart_in_stackwidget_after_setting_of_all_specification(struct_chart_detail_obj);

        label_show_date_time->setText(map_dates_labels.value(stackwidget_index));
        label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));

    }


    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

QString redefined_result_full_loader::prepare_and_get_tooltip_string_for_barset(qint64 count, QString category, QString label)
{
    QString tooltip = "Events - " +  QString::number(count) + "\n";
    tooltip.append("Category - "+ category + "\n");
    if(label != Macro_Redefined_Result_Messenger_All_Accounts)
        tooltip.append("Participant - "+ label);

    return tooltip;

}

void redefined_result_full_loader::set_chart_in_stackwidget_after_setting_of_all_specification(redefined_result_full_loader::struct_redefined_result_single_chart_detail &struct_chart_detail_obj)
{
    ///Added functionality on barset
    QList<QBarSet*> list = struct_chart_detail_obj.stacked_bar_series->barSets();
    for(int i = 0; i < list.size() ; i++)
    {
        m_barset *barset = dynamic_cast<m_barset*> (list.at(i));
        barset->setBorderColor(QColor("transparent"));
        connect(barset, SIGNAL(doubleClicked(int)), this, SLOT(slot_barset_doubleClicked(int)));
        connect(barset, SIGNAL(pressed(int)), this, SLOT(slot_barset_pressed(int)));
    }

    QChart *chart_obj = new QChart();

    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        chart_obj->setTitle(QString(MACRO_Redefined_Result_Browser_History));
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        chart_obj->setTitle(QString(MACRO_Redefined_Result_Messenger));

    chart_obj->setAnimationOptions(QChart::SeriesAnimations);
    chart_obj->legend()->setVisible(false);
    chart_obj->legend()->setAlignment(Qt::AlignBottom);

    ///Add series in chart
    chart_obj->addSeries(struct_chart_detail_obj.stacked_bar_series);

    ///Create category Axis- it can be create for x-axis or y-axis
    QBarCategoryAxis *barcategory_axis_obj = new QBarCategoryAxis(this);
    barcategory_axis_obj->append(struct_chart_detail_obj.category_list);
    struct_chart_detail_obj.stacked_bar_series->setBarWidth(0.40);

    ///Set axis x for chart
    chart_obj->setAxisX(barcategory_axis_obj, struct_chart_detail_obj.stacked_bar_series);
    // chart_obj->axisX()->setTitleText(struct_chart_detail_obj.x_axis_title);

    ///Set axis y for chart
    QValueAxis *value_axisy = new QValueAxis;
    value_axisy->setRange(0,struct_chart_detail_obj.roundup_value_axis);
    value_axisy->setLabelFormat("%i");
    // value_axisy->setTitleText(struct_chart_detail_obj.y_axis_title);
    chart_obj->setAxisY(value_axisy, struct_chart_detail_obj.stacked_bar_series);

    QChartView *chartView_common_obj = new QChartView(this);

    chartView_common_obj->setChart(chart_obj);

    list_of_charview << chartView_common_obj;

    if(radiobutton_create_graph_hourly->isChecked())
        stackwidget_obj->addWidget(chartView_common_obj);


}

int redefined_result_full_loader::set_graphs_data_tablewidget_header()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    if(table_headers_width_list.isEmpty())
        return -1;

    if(display_header_name_list.size() != table_headers_width_list.size())
        return -1;

    custom_header_widgets_QList.clear();

    m_tablewidget_obj->setColumnCount(display_header_name_list.size());

    QStringList empty_header_stringlist;

    for(int column_count = 0; column_count < display_header_name_list.size(); column_count++)
    {
        QString column_name = display_header_name_list.at(column_count);

        custom_table_header_widget_obj = new custom_table_header_widget;

        custom_table_header_widget_obj->pub_set_column_name(column_name);
        custom_table_header_widget_obj->pub_set_column_seq_number(column_count);

        m_tablewidget_obj->pub_set_horizontal_header_item_widget(column_count, custom_table_header_widget_obj);

        empty_header_stringlist << "";

        connect(custom_table_header_widget_obj, SIGNAL(search_field_return_pressed()), this, SLOT(slot_custom_table_header_widget_search_field_return_pressed()));
        connect(custom_table_header_widget_obj, SIGNAL(search_field_text_changed()), this, SLOT(slot_custom_table_header_widget_search_field_text_changed()));
        connect(custom_table_header_widget_obj, SIGNAL(signal_sorting_clicked(int,int)), this, SLOT(slot_custom_table_header_widget_sorting_clicked(int,int)));

        if(column_count ==  enum_tablewidget_column_bookmark && column_name.isEmpty())
        {
            custom_table_header_widget_obj->pushbutton_sort->show();
        }
        else if(column_count ==  enum_tablewidget_column_notes && column_name.isEmpty())
        {
            custom_table_header_widget_obj->pushbutton_sort->show();
        }
        else if(column_count ==  enum_tablewidget_column_tags && column_name.isEmpty())
        {
            custom_table_header_widget_obj->pushbutton_sort->show();
        }
        custom_header_widgets_QList.append(custom_table_header_widget_obj);

    }

    m_tablewidget_obj->setHorizontalHeaderLabels(empty_header_stringlist);

    if(feature_name == MACRO_CASE_TREE_Redefined_Result_Parent)
    {
        if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        {
            tab_custom_header_widgets_for_redefine_result_Messenger_QMap.insert(new_tab_name,custom_header_widgets_QList);
        }

        if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        {
            tab_custom_header_widgets_for_redefine_result_browser_history_QMap.insert(new_tab_name,custom_header_widgets_QList);
        }
    }

    for(int i = 0; i < table_headers_width_list.size(); i++)
    {
        if(((custom_table_header_widget *)custom_header_widgets_QList.at(i))->pub_get_column_name().isEmpty())
        {
            m_tablewidget_obj->setColumnWidth(i, table_headers_width_list.at(i).toInt());
        }
        else
        {
            int column_width = table_headers_width_list.at(i).toInt();
            column_width = column_width + 25;
            m_tablewidget_obj->setColumnWidth(i, column_width);
        }

        if(table_headers_width_list.at(i).size() == 0)
            m_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

    return 0;
}

QStringList redefined_result_full_loader::get_colors_stringlist_from_db()
{
    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Redefined_Result_Filter_In_Result_QString).toString() + "participants_emblem.sqlite";

    return(recon_helper_standard_obj->get_stringlist_from_db_by_dbpath("select color_name from colors", 0, db_path, Q_FUNC_INFO));
}

void redefined_result_full_loader::set_timestamp_from_db_min_max()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    QSqlDatabase::removeDatabase(Q_FUNC_INFO );
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(destination_db_redefined_result);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("----FAILED--- to open database:" + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("--Error--:" + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString command_min = "SELECT MIN(timestamp)  FROM redefined_result where timestamp is not ''";
    QString command_max = "SELECT MAX(timestamp)  FROM redefined_result where timestamp is not ''";

    quint64 start_temp_epch = recon_helper_standard_obj->get_string_from_db_by_dbreference(command_min,0, source_db, Q_FUNC_INFO).toInt();
    quint64 endtemp_epch = recon_helper_standard_obj->get_string_from_db_by_dbreference(command_max,0, source_db, Q_FUNC_INFO).toInt();

    start_timestamp_int = start_temp_epch;
    end_timestamp_int = endtemp_epch;

    source_db.close();

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void redefined_result_full_loader::set_graphs_essentail()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);


    int count_stacks = stackwidget_obj->count();

    if(count_stacks > 0)
    {
        for(int i = (count_stacks-1); i >= 0; i--)
        {
            stackwidget_obj->removeWidget(stackwidget_obj->widget(i));
            delete stackwidget_obj->widget(i);
        }
    }

    label_show_date_time->clear();
    label_show_pages->clear();


    years_list.clear();
    month_year_list.clear();
    day_month_year_list.clear();
    hour_day_month_year_list.clear();


    quint64 temp_start_epchtime = start_timestamp_int;
    quint64 temp_end_epchtime = end_timestamp_int;

    if(temp_start_epchtime > temp_end_epchtime)
    {
        return;
    }

    qint64 time_between = temp_end_epchtime - temp_start_epchtime;

    qint64 no_of_hours = time_between/3600;

    quint64 start_time_add_an_hour = 0;

    start_time_add_an_hour = temp_start_epchtime;

    progressbar_increment_step = no_of_hours / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;


    for(qint64 hours = 0; hours < no_of_hours; hours++)
    {

        if(start_time_add_an_hour > temp_end_epchtime)
            break;

        if(hours % 100 == 0)
        {
            QCoreApplication::processEvents();
        }


        QDateTime dttime;
        dttime.setTimeSpec(Qt::UTC);
        dttime = dttime.fromSecsSinceEpoch(start_time_add_an_hour);


        QString yearr = QString::number(dttime.date().year());

        QString day_mnth_year = dttime.toString("ddd dd-MMM-yyyy");
        QString hour_day_mnth_year = dttime.toString("ddd dd-MMM-yyyy hh");

        QString mnth_year = QString(QString(dttime.date().toString("MMM")) + "-" + QString::number(dttime.date().year()));

        if(!hour_day_month_year_list.contains(hour_day_mnth_year))
        {
            hour_day_month_year_list << hour_day_mnth_year;
        }

        if(!day_month_year_list.contains(day_mnth_year))
        {
            day_month_year_list << day_mnth_year;
        }

        if(!years_list.contains(yearr))
        {
            years_list << yearr;
        }


        if(!month_year_list.contains(mnth_year))
        {
            month_year_list << mnth_year;
        }


        start_time_add_an_hour += 3600;

    }


    QSqlDatabase::removeDatabase(Q_FUNC_INFO );
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(destination_db_redefined_result);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("----FAILED--- to open database:" + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("--Error--:" + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);

    if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
    {
        query.prepare("select INT,timestamp,year_str, month_year, day_month_year,hour_day_month_year,sender, receiver from redefined_result where timestamp >= ?"
                      " AND timestamp <= ? ORDER BY timestamp ASC");
    }else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
    {
        query.prepare("select INT,timestamp,year_str, month_year, day_month_year,hour_day_month_year from redefined_result where timestamp >= ?"
                      " AND timestamp <= ? ORDER BY timestamp ASC");

    }

    query.addBindValue(QString::number(start_timestamp_int));
    query.addBindValue(QString::number(end_timestamp_int));

    if(!query.exec())
    {
        recon_static_functions::app_debug("query --------FAILED------ "+ destination_db_redefined_result, Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        source_db.close();
        return;
    }

    map_of_struct_graph_data.clear();
    while(query.next())
    {
        QCoreApplication::processEvents();

        struct_graphs_essentials struct_data_database_obj;

        struct_data_database_obj.int_value = query.value(0).toString();
        struct_data_database_obj.timestamp = query.value(1).toString();
        struct_data_database_obj.year_value = query.value(2).toString();
        struct_data_database_obj.month_year_value = query.value(3).toString();
        struct_data_database_obj.day_month_year_value = query.value(4).toString();
        struct_data_database_obj.hour_day_month_year_value = query.value(5).toString();

        if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        {
            struct_data_database_obj.sender = query.value(6).toString();
            struct_data_database_obj.receiver = query.value(7).toString();
        }

        map_of_struct_graph_data.insert(struct_data_database_obj.year_value,struct_data_database_obj);
        map_of_struct_graph_data.insert(struct_data_database_obj.month_year_value,struct_data_database_obj);
        map_of_struct_graph_data.insert(struct_data_database_obj.day_month_year_value,struct_data_database_obj);
        map_of_struct_graph_data.insert(struct_data_database_obj.hour_day_month_year_value,struct_data_database_obj);
    }


    source_db.close();

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void redefined_result_full_loader::set_graphs_essentials_for_data_only()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);


    if(start_timestamp_int > end_timestamp_int)
    {
        return;
    }


    QSqlDatabase::removeDatabase(Q_FUNC_INFO );
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(destination_db_redefined_result);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("----FAILED--- to open database:" + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("--Error--:" + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }



    QSqlQuery query(source_db);

    years_list.clear();
    month_year_list.clear();
    day_month_year_list.clear();
    hour_day_month_year_list.clear();

    QString db_cmd = "select COUNT(*) From redefined_result";
    db_cmd.append(" where timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC");

    QStringList list;
    list << QString::number(start_timestamp_int) << QString::number(end_timestamp_int);

    total_record_count = recon_helper_standard_obj->get_intvalue_from_db_with_addbindvalues_by_dbreference(db_cmd, source_db,0,list, Q_FUNC_INFO);

    if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
    {

        if(selected_configured_account_name == Macro_Redefined_Result_Messenger_All_Accounts)
        {
            query.prepare("select INT,timestamp,year_str, month_year, day_month_year,hour_day_month_year,sender, receiver from redefined_result where timestamp >= ?"
                          " AND timestamp <= ? ORDER BY timestamp ASC");

            query.addBindValue(QString::number(start_timestamp_int));
            query.addBindValue(QString::number(end_timestamp_int));


        }else
        {

            QString participant_name = tablewidget_participants_list->item(tablewidget_participants_list->currentRow(),0)->text();

            if(participant_name == Macro_Redefined_Result_Messenger_All_Participants)
            {


                query.prepare("select INT,timestamp,year_str, month_year, day_month_year,hour_day_month_year,sender, receiver from redefined_result where (sender = ? OR receiver = ?) AND timestamp >= ?"
                              " AND timestamp <= ? ORDER BY timestamp ASC");

                query.addBindValue(selected_configured_account_name);
                query.addBindValue(selected_configured_account_name);

                query.addBindValue(QString::number(start_timestamp_int));
                query.addBindValue(QString::number(end_timestamp_int));

            }else
            {

                query.prepare("select INT,timestamp,year_str, month_year, day_month_year,hour_day_month_year,sender, receiver from redefined_result where ((sender = ? AND receiver = ?) OR (sender = ? AND receiver = ?)) AND timestamp >= ?"
                              " AND timestamp <= ? ORDER BY timestamp ASC");

                query.addBindValue(selected_configured_account_name);
                query.addBindValue(participant_name);
                query.addBindValue(participant_name);
                query.addBindValue(selected_configured_account_name);

                query.addBindValue(QString::number(start_timestamp_int));
                query.addBindValue(QString::number(end_timestamp_int));


            }




        }

    }else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
    {
        query.prepare("select INT,timestamp,year_str, month_year, day_month_year,hour_day_month_year from redefined_result where timestamp >= ?"
                      " AND timestamp <= ? ORDER BY timestamp ASC");

        query.addBindValue(QString::number(start_timestamp_int));
        query.addBindValue(QString::number(end_timestamp_int));


    }


    if(!query.exec())
    {
        recon_static_functions::app_debug("query --------FAILED------ ", Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        source_db.close();
        return;
    }


    progressbar_increment_step = total_record_count / 10;

    if(progressbar_increment_step <= 0)
        progressbar_increment_step = 1;


    map_of_struct_graph_data.clear();
    int count = 0;
    while(query.next())
    {
        if(count % 100 == 0)
        {
            QCoreApplication::processEvents();
        }


        count++;

        QString yearr = query.value(2).toString();
        QString mnth_year = query.value(3).toString();
        QString day_mnth_year = query.value(4).toString();
        QString hour_day_mnth_year = query.value(5).toString();

        if(!hour_day_month_year_list.contains(hour_day_mnth_year))
        {
            hour_day_month_year_list << hour_day_mnth_year;
        }

        if(!day_month_year_list.contains(day_mnth_year))
        {
            day_month_year_list << day_mnth_year;
        }

        if(!years_list.contains(yearr))
        {
            years_list << yearr;
        }

        if(!month_year_list.contains(mnth_year))
        {
            month_year_list << mnth_year;
        }


        QCoreApplication::processEvents();


        struct_graphs_essentials struct_data_database_obj;

        struct_data_database_obj.int_value = query.value(0).toString();
        struct_data_database_obj.timestamp = query.value(1).toString();
        struct_data_database_obj.year_value = query.value(2).toString();
        struct_data_database_obj.month_year_value = query.value(3).toString();
        struct_data_database_obj.day_month_year_value = query.value(4).toString();
        struct_data_database_obj.hour_day_month_year_value = query.value(5).toString();

        if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        {
            struct_data_database_obj.sender = query.value(6).toString();
            struct_data_database_obj.receiver = query.value(7).toString();
        }

        map_of_struct_graph_data.insert(struct_data_database_obj.year_value,struct_data_database_obj);
        map_of_struct_graph_data.insert(struct_data_database_obj.month_year_value,struct_data_database_obj);
        map_of_struct_graph_data.insert(struct_data_database_obj.day_month_year_value,struct_data_database_obj);
        map_of_struct_graph_data.insert(struct_data_database_obj.hour_day_month_year_value,struct_data_database_obj);
    }


    source_db.close();
    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}





QString redefined_result_full_loader::create_db_for_redefined_saved_result(QString db_name)
{
    QString saved_path;
    QString create_cmd;
    QString insert_cmd;
    QString saved_index_tab_db_path;

    if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
    {
        saved_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Messenger_In_Result_QString).toString() + db_name + ".sqlite";
        if(QFile::exists(saved_path))
            return QString("");

        create_cmd = "create table redefined_result_saved(INT INTEGER PRIMARY KEY,bookmark varchar(30),plugin varchar(30),tab_name varchar(30), "
                     "record_no INTEGER, sender varchar(30), receiver varchar(30),message INTEGER,timestamp varchar(300),os_scheme_display varchar(300),"
                     "recon_tag_value varchar(30), source_count_name varchar(30),notes varchar(100),notes_icon varchar(30),result_name varchar(30),blank_column varchar(300) ,source_file varchar(100))";

        recon_helper_standard_obj->execute_db_command_by_dbpath(create_cmd,saved_path,Q_FUNC_INFO);
        insert_cmd  = QString("insert into saved_index(db_name,display_name) values(?,?)");

        saved_index_tab_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Messenger_In_Result_QString).toString() + "index_messenger_saved.sqlite";

    }
    else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
    {
        saved_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Browser_History_In_Result_QString).toString() + db_name + ".sqlite";
        if(QFile::exists(saved_path))
            return QString("");

        create_cmd = "create table redefined_result_saved(INT INTEGER PRIMARY KEY,bookmark varchar(30),plugin varchar(30),tab_name varchar(30), "
                     "record_no INTEGER, url varchar(30), title varchar(30),visit_count INTEGER,timestamp varchar(300),os_scheme_display varchar(300),"
                     "recon_tag_value varchar(30), source_count_name varchar(30),notes varchar(100),notes_icon varchar(30),result_name varchar(30),blank_column varchar(300) ,source_file varchar(100))";

        recon_helper_standard_obj->execute_db_command_by_dbpath(create_cmd,saved_path,Q_FUNC_INFO);

        insert_cmd  = QString("insert into saved_index(db_name,display_name) values(?,?)");

        saved_index_tab_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Browser_History_In_Result_QString).toString() + "index_browsers_history_saved.sqlite";

    }

    saved_db_info(db_name,insert_cmd,saved_index_tab_db_path);
    return saved_path;
}



void redefined_result_full_loader::extract_and_save_result(QString saved_db_path, QString result_name)
{

    QSqlDatabase source_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(destination_db_redefined_result);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("destination db open ---FAILED--- " + source_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + source_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_index(source_db);

    QString added_tab_name_str = tabwidget_redefined_result->tabText(tabwidget_redefined_result->currentIndex());
    current_clicked_domain = qmap_domians_str.value(added_tab_name_str);
    current_barset_participant = qmap_barset_participants_str.value(added_tab_name_str);
    current_barset_category_str = qmap_barset_catgories_str.value(added_tab_name_str);
    bool bool_save_data_other_than_pie_and_visit_count = qmap_bool_save_data_other_than_pie_and_visit_count_bool.value(added_tab_name_str);
    QString account_name_str = qmap_configured_account_str.value(added_tab_name_str);
    QString graph_type_str = qmap_bar_graph_radiobutton_type_str.value(added_tab_name_str);

    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR && bool_save_data_other_than_pie_and_visit_count == false)
    {
        prepare_sqlquery_for_visit_count_tablewidget(query_index);
    }
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR && bool_save_data_other_than_pie_and_visit_count == false)
    {
        prepare_sqlquery_for_piegraph_tablewidget_to_save_data(query_index , account_name_str);
    }
    else if(bool_save_data_other_than_pie_and_visit_count)
    {
        prepare_sqlquery_for_all_graphs_tablewidget_to_save_data(query_index,account_name_str,graph_type_str);
    }


    insert_values_in_save_result_db(query_index,saved_db_path,result_name);

    source_db.close();
}


void redefined_result_full_loader::insert_values_in_save_result_db(QSqlQuery &query_index , QString db_path, QString result_name)
{



    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("destination db open ---FAILED--- " + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }
    QSqlQuery insert_query(destination_db);

    if(!query_index.exec())
    {
        recon_static_functions::app_debug("query_index.exec() ---FAILED---- " + destination_db.databaseName() ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_index.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query_index.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return ;
    }

    qint64 counter = 0;

    while(query_index.next())
    {
        counter++;

        if(counter % 100 == 0)
            QCoreApplication::processEvents();

        if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        {
            insert_query.prepare("insert into redefined_result_saved(bookmark,recon_tag_value,"
                                 "INT,plugin,tab_name,os_scheme_display,source_count_name,"
                                 "sender,receiver,message,timestamp,notes,result_name,source_file)"
                                 "values(?,?,?,?,?,?,?,?,?,?,?,?,?,?)");


            insert_query.addBindValue(query_index.value("bookmark").toString());
            insert_query.addBindValue(query_index.value("recon_tag_value").toString());
            insert_query.addBindValue(query_index.value("INT").toString());
            insert_query.addBindValue(query_index.value("plugin").toString());
            insert_query.addBindValue(query_index.value("tab_name").toString());
            insert_query.addBindValue(query_index.value("os_scheme_display").toString());
            insert_query.addBindValue(query_index.value("source_count_name").toString());
            insert_query.addBindValue(query_index.value("sender").toString());
            insert_query.addBindValue(query_index.value("receiver").toString());
            insert_query.addBindValue(query_index.value("message").toString());
            insert_query.addBindValue(query_index.value("timestamp").toString());
            insert_query.addBindValue(query_index.value("notes").toString());
            insert_query.addBindValue(result_name);
            insert_query.addBindValue("");

        }
        else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        {
            insert_query.prepare("insert into redefined_result_saved(bookmark,recon_tag_value,"
                                 "INT,plugin,tab_name,os_scheme_display,source_count_name,"
                                 "url,title,timestamp,visit_count,notes,result_name,source_file)"
                                 "values(?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

            insert_query.addBindValue(query_index.value("bookmark").toString());
            insert_query.addBindValue(query_index.value("recon_tag_value").toString());
            insert_query.addBindValue(query_index.value("INT").toString());
            insert_query.addBindValue(query_index.value("plugin").toString());
            insert_query.addBindValue(query_index.value("tab_name").toString());
            insert_query.addBindValue(query_index.value("os_scheme_display").toString());
            insert_query.addBindValue(query_index.value("source_count_name").toString());
            insert_query.addBindValue(query_index.value("url").toString());
            insert_query.addBindValue(query_index.value("title").toString());
            insert_query.addBindValue(query_index.value("timestamp").toString());
            insert_query.addBindValue(query_index.value("visit_count").toString());
            insert_query.addBindValue(query_index.value("notes").toString());
            insert_query.addBindValue(result_name);
            insert_query.addBindValue("");

        }

        if(!insert_query.exec())
        {
            recon_static_functions::app_debug("insert_query  ----FAILED---- ",Q_FUNC_INFO);
            recon_static_functions::app_debug(" error- " + insert_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" query- " + insert_query.executedQuery(),Q_FUNC_INFO);
            continue ;
        }

    }
    destination_db.close();
}


void redefined_result_full_loader::save_redefined_result_data_in_case_treewidget(QString tab_to_be_saved)
{

    QString saved_db_path = create_db_for_redefined_saved_result(tab_to_be_saved);
    if(bool_to_save_all_data_with_different_colour_barset)
    {
        current_barset_participant.clear();
        QMap<QString, QString>::iterator iter;
        for(iter = map_of_partipants_with_color.begin() ; iter != map_of_partipants_with_color.end(); iter++)
        {
            current_barset_participant = iter.key();
            extract_and_save_result(saved_db_path,tab_to_be_saved);
        }
    }
    else
    {
        extract_and_save_result(saved_db_path,tab_to_be_saved);
    }

    emit signal_save_tabs_from_redefined_result(tab_to_be_saved , plugin_name);
}

void redefined_result_full_loader::saved_db_info(QString db_name, QString command, QString db_path)
{
    QSqlDatabase saved_db_index;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    saved_db_index = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    saved_db_index.setDatabaseName(db_path);
    if(!saved_db_index.open())
    {
        recon_static_functions::app_debug("db open ---FAILED--- " + saved_db_index.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + saved_db_index.lastError().text(), Q_FUNC_INFO);
        return;
    }
    QString db_file_name = db_name;
    db_file_name = db_file_name + ".sqlite";

    QSqlQuery insert_qry(saved_db_index);
    insert_qry.prepare(command);
    insert_qry.addBindValue(db_name);///db name
    insert_qry.addBindValue(db_name);///display name

    if(!insert_qry.exec())
    {
        recon_static_functions::app_debug("insert_qry.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + insert_qry.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + insert_qry.executedQuery(),Q_FUNC_INFO);
        saved_db_index.close();
        return ;
    }

    saved_db_index.close();
}

