#include "exif_metadata_filters.h"
#include "ui_exif_metadata_filters.h"

exif_metadata_filters::exif_metadata_filters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exif_metadata_filters)
{
    ui->setupUi(this);

    ui->radioButton_all_filters->setChecked(true);

    ui->doubleSpinBox_latitude_from->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->doubleSpinBox_latitude_to->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->doubleSpinBox_longitude_from->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->doubleSpinBox_longitude_to->setAttribute(Qt::WA_MacShowFocusRect,false);

    ui->lineEdit_author->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_make->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_model->setAttribute(Qt::WA_MacShowFocusRect, false);

    ui->lineEdit_search_label->setAttribute(Qt::WA_MacShowFocusRect, false);

    ui->pushButton_select_source_list->setIcon(QIcon("../icons/signs/check_list.png"));


    pub_load_defaults();

    select_source_interface_obj = new select_source_interface(this);
    connect(select_source_interface_obj,SIGNAL(signal_selected_source_list(QStringList)),this,SLOT(slot_selected_source_list(QStringList)));

    contains_str     = QString("Contains");
    matches_str      = QString("Matches");
    starts_with_str  = QString("Starts with");
    ends_with_str    = QString("Ends with");

}


exif_metadata_filters::~exif_metadata_filters()
{
    delete ui;
}

void exif_metadata_filters::pub_load_defaults()
{ // load default settings of exif metadata
    ui->doubleSpinBox_latitude_from->clear();
    ui->doubleSpinBox_latitude_to->clear();
    ui->doubleSpinBox_longitude_from->clear();
    ui->doubleSpinBox_longitude_to->clear();


    ui->lineEdit_author->clear();
    ui->lineEdit_make->clear();
    ui->lineEdit_model->clear();


    ui->lineEdit_search_label->clear();


    ui->checkBox_latitude->setChecked(false);
    ui->checkBox_longitude->setChecked(false);

    ui->radioButton_all_filters->setChecked(false);
    ui->radioButton_any_filter->setChecked(true);

    ui->radioButton_all_source->setChecked(true);
    ui->pushButton_select_source_list->setDisabled(true);

    ui->comboBox_author->setCurrentIndex(0);
    ui->comboBox_make->setCurrentIndex(0);
    ui->comboBox_model->setCurrentIndex(0);

    on_checkBox_latitude_clicked(false);
    on_checkBox_longitude_clicked(false);

    selected_source_cnt_nm_list.clear();
    prevoius_selected_source_cnt_nm_list.clear();

}

void exif_metadata_filters::pub_set_sources_in_ui()
{ // set ui
    QList<struct_GLOBAL_witness_info_source> sources_list =  global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    int row = 0;

    source_checkbox_list.clear();
    for(int i = 0; i < sources_list.size() ; i++)
    {
        if(i % 4 == 0)
        {
            row++;
        }

        QCheckBox *checkBox = new QCheckBox(this);

        checkBox->setText(sources_list.at(i).source_count_name);
        checkBox->setChecked(true);
        checkBox->setToolTip(sources_list.at(i).complete_source_name);


        source_checkbox_list.append(checkBox);

    }
}



void exif_metadata_filters::on_checkBox_latitude_clicked(bool checked)
{ // checkbox latitude clicked
    ui->doubleSpinBox_latitude_from->setEnabled(checked);
    ui->doubleSpinBox_latitude_to->setEnabled(checked);
    ui->doubleSpinBox_latitude_from->setValue(40.0000);
    ui->doubleSpinBox_latitude_to->setValue(50.0000);
}


void exif_metadata_filters::on_checkBox_longitude_clicked(bool checked)
{ // checkbox longitude clicked
    ui->doubleSpinBox_longitude_from->setEnabled(checked);
    ui->doubleSpinBox_longitude_to->setEnabled(checked);
    ui->doubleSpinBox_longitude_from->setValue(-70.0000);
    ui->doubleSpinBox_longitude_to->setValue(-60.0000);
}


void exif_metadata_filters::on_pushButton_search_clicked()
{ // search button clicked for metadata search
    QStringList where_items;

    if(ui->checkBox_latitude->isChecked())
    {
        QString latitude_from_str = ui->doubleSpinBox_latitude_from->text().trimmed();
        if(latitude_from_str.isEmpty())
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Latitude 'From' Empty.");
            return;
        }

        QString latitude_to_str = ui->doubleSpinBox_latitude_to->text().trimmed();
        if(latitude_to_str.isEmpty())
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Latitude 'To' Empty.");
            return;
        }


        where_items.append(latitude_sql_predicate());
    }

    if(ui->checkBox_longitude->isChecked())
    {
        QString longitude_from_str = ui->doubleSpinBox_longitude_from->text().trimmed();
        if(longitude_from_str.isEmpty())
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Longitude 'From' Empty.");
            return;
        }

        QString longitude_to_str = ui->doubleSpinBox_longitude_to->text().trimmed();
        if(longitude_to_str.isEmpty())
        {
            QMessageBox::warning(this, recon_static_functions::get_app_name(), "Longitude 'To' Empty.");
            return;
        }


        where_items.append(longitude_sql_predicate());
    }

    if(!(ui->lineEdit_author->text().trimmed().isEmpty()))
    {
        where_items.append(author_sql_predicate());
    }

    if(!(ui->lineEdit_make->text().trimmed().isEmpty()))
    {
        where_items.append(make_sql_predicate());
    }

    if(!(ui->lineEdit_model->text().trimmed().isEmpty()))
    {
        where_items.append(model_sql_predicate());
    }

    if(where_items.isEmpty())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please select atleast one filter for search.");
        return;
    }


    QString command = "select filesystem_record,make,model,author,latitude,longitude from files where ";

    QString where_combiner;

    if(ui->radioButton_all_filters->isChecked())
        where_combiner = " AND ";
    else
        where_combiner = " OR ";

    if(ui->radioButton_all_source->isChecked())
    {
        ui->pushButton_select_source_list->setEnabled(false);
        QList<struct_GLOBAL_witness_info_source> list_target_source_info =  global_witness_info_manager_class_obj->pub_get_source_structure_QList();
        selected_source_cnt_nm_list.clear();
        for(int ii = 0; ii < list_target_source_info.size() ; ii++)
        {
            selected_source_cnt_nm_list << list_target_source_info.at(ii).source_count_name;
        }
    }
    else
    {
        // selected_source_cnt_nm_list is filled in selected source slot
    }

    QStringList source_list_for_searching  = selected_source_cnt_nm_list;
    if(source_list_for_searching.isEmpty())
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Please select atleast one 'Source' for search.");
        return;
    }


    QString search_label = ui->lineEdit_search_label->text().trimmed();

    if(search_label.isEmpty())
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Please enter 'Search' label");
        return;
    }

    QString where_statement = where_items.join(where_combiner);

    command.append(where_statement);

    emit signal_exif_metadata_filter_work_done(command,where_statement, source_list_for_searching, search_label);

}

QString exif_metadata_filters::latitude_sql_predicate()
{ // prepare command for latitude filter
    if(ui->doubleSpinBox_latitude_from->text().trimmed().isEmpty() && ui->doubleSpinBox_latitude_to->text().trimmed().isEmpty())
        return QString("");


    QString sql_predicate;

    QString latitude_from = ui->doubleSpinBox_latitude_from->text();
    QString latitude_to = ui->doubleSpinBox_latitude_to->text();

    sql_predicate = QString("(latitude >= ") + latitude_from + " AND latitude <= " + latitude_to + ")";

    return  sql_predicate;
}


QString exif_metadata_filters::longitude_sql_predicate()
{ // prepare command for longitude filter

    if(ui->doubleSpinBox_longitude_from->text().trimmed().isEmpty() && ui->doubleSpinBox_longitude_to->text().trimmed().isEmpty())
        return QString("");

    QString sql_predicate;

    QString longitude_from = ui->doubleSpinBox_longitude_from->text();
    QString longitude_to = ui->doubleSpinBox_longitude_to->text();


    sql_predicate = QString("(longitude >= ") + longitude_from + " AND longitude <= " + longitude_to + ")";

    return  sql_predicate;
}

QString exif_metadata_filters::author_sql_predicate()
{ // prepare author command for sql to display

    if(ui->lineEdit_author->text().trimmed().isEmpty())
        return QString("");

    QStringList sql_predicate_list;

    QString author_name = ui->lineEdit_author->text().trimmed();
    QStringList author_name_list =  author_name.split(",", Qt::SkipEmptyParts);
    author_name_list.removeAll(" ");


    if(ui->comboBox_author->currentText() == contains_str)

    {
        for(int ii = 0 ; ii < author_name_list.size() ; ii++)
        {
            QString author_name = author_name_list.at(ii);
            sql_predicate_list.append("author LIKE '%" + author_name.trimmed() + "%'");
        }
    }

    else if(ui->comboBox_author->currentText() == matches_str)

    {
        for(int ii = 0 ; ii < author_name_list.size() ; ii++)
        {
            QString author_name = author_name_list.at(ii);
            sql_predicate_list.append("author LIKE '" + author_name.trimmed()  + "'");

        }
    }

    else if(ui->comboBox_author->currentText() == starts_with_str)

    {
        for(int ii = 0 ; ii < author_name_list.size() ; ii++)
        {
            QString author_name = author_name_list.at(ii);
            sql_predicate_list.append("author LIKE '" + author_name.trimmed()  + "%'");
        }
    }

    else if(ui->comboBox_author->currentText() == ends_with_str)

    {
        for(int ii = 0 ; ii < author_name_list.size() ; ii++)
        {
            QString author_name = author_name_list.at(ii);
            sql_predicate_list.append("author LIKE '%" + author_name.trimmed()  + "'");
        }
    }

    QString partial_cmd = sql_predicate_list.join(" OR ");
    partial_cmd.prepend("( ").append(" )");


    return partial_cmd;
}

QString exif_metadata_filters::make_sql_predicate()
{ // prepare make command to display for filters
    if(ui->lineEdit_make->text().trimmed().isEmpty())
        return QString("");


    QStringList sql_predicate_list;

    QString exif_make = ui->lineEdit_make->text().trimmed();
    QStringList exif_make_list =  exif_make.split(",", Qt::SkipEmptyParts);
    exif_make_list.removeAll(" ");


    if(ui->comboBox_make->currentText() == contains_str)
    {
        for(int ii = 0 ; ii < exif_make_list.size() ; ii++)
        {
            QString exif_make = exif_make_list.at(ii);
            sql_predicate_list.append("make LIKE '%" + exif_make.trimmed() + "%'");
        }
    }
    else if(ui->comboBox_make->currentText() == matches_str)
    {
        for(int ii = 0 ; ii < exif_make_list.size() ; ii++)
        {
            QString exif_make = exif_make_list.at(ii);

            sql_predicate_list.append("make LIKE '" + exif_make.trimmed()  + "'");
        }
    }
    else if(ui->comboBox_make->currentText() == starts_with_str)
    {
        for(int ii = 0 ; ii < exif_make_list.size() ; ii++)
        {
            QString exif_make = exif_make_list.at(ii);
            sql_predicate_list.append("make LIKE '" + exif_make.trimmed()  + "%'");
        }
    }
    else if(ui->comboBox_make->currentText() == ends_with_str)
    {
        for(int ii = 0 ; ii < exif_make_list.size() ; ii++)
        {
            QString exif_make = exif_make_list.at(ii);
            sql_predicate_list.append("make LIKE '%" + exif_make.trimmed()  + "'");
        }
    }

    QString partial_cmd = sql_predicate_list.join(" OR ");
    partial_cmd.prepend("( ").append(" )");


    return partial_cmd;
}

QString exif_metadata_filters::model_sql_predicate()
{ // prepare model command to display
    if(ui->lineEdit_model->text().trimmed().isEmpty())
        return QString("");

    QStringList sql_predicate_list;

    QString exif_model = ui->lineEdit_model->text().trimmed();
    QStringList exif_model_list =  exif_model.split(",", Qt::SkipEmptyParts);
    exif_model_list.removeAll(" ");


    if(ui->comboBox_model->currentText() == contains_str)
    {
        for(int ii = 0 ; ii < exif_model_list.size() ; ii++)
        {
            QString exif_model = exif_model_list.at(ii);
            sql_predicate_list.append("model LIKE '%" + exif_model.trimmed() + "%'");
        }
    }
    else if(ui->comboBox_model->currentText() == matches_str)
    {
        for(int ii = 0 ; ii < exif_model_list.size() ; ii++)
        {
            QString exif_model = exif_model_list.at(ii);

            sql_predicate_list.append("model LIKE '" + exif_model.trimmed()  + "'");
        }
    }
    else if(ui->comboBox_model->currentText() == starts_with_str)
    {
        for(int ii = 0 ; ii < exif_model_list.size() ; ii++)
        {
            QString exif_model = exif_model_list.at(ii);
            sql_predicate_list.append("model LIKE '" + exif_model.trimmed()  + "%'");
        }
    }
    else if(ui->comboBox_model->currentText() == ends_with_str)
    {
        for(int ii = 0 ; ii < exif_model_list.size() ; ii++)
        {
            QString exif_model = exif_model_list.at(ii);
            sql_predicate_list.append("model LIKE '%" + exif_model.trimmed()  + "'");
        }
    }

    QString partial_cmd = sql_predicate_list.join(" OR ");
    partial_cmd.prepend("( ").append(" )");


    return partial_cmd;
}


void exif_metadata_filters::on_radioButton_all_source_toggled(bool checked)
{ // radio button all source, search all sources
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    if(!checked)
        return ;

    ui->pushButton_select_source_list->setEnabled(false);

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);

}

void exif_metadata_filters::on_radioButton_select_source_toggled(bool checked)
{ // radio button select source, search from single source
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    if(!checked)
        return ;

    ui->pushButton_select_source_list->setEnabled(true);

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void exif_metadata_filters::on_pushButton_select_source_list_clicked()
{ // select source list, select any one source and search from them
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    select_source_interface_obj->pub_put_prevoius_selected_source_list(prevoius_selected_source_cnt_nm_list);
    select_source_interface_obj->pub_set_ui_essentials();
    select_source_interface_obj->pub_insert_data_in_table();
    select_source_interface_obj->show();


    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void exif_metadata_filters::slot_selected_source_list(QStringList selected_src_list)
{ // slot of selected source list for search filters
    selected_source_cnt_nm_list = selected_src_list;
    prevoius_selected_source_cnt_nm_list = selected_src_list;
}


