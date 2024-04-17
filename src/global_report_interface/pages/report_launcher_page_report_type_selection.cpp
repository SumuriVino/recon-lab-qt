#include "report_launcher_page_report_type_selection.h"
#include "ui_report_launcher_page_report_type_selection.h"

page_report_type_selection::page_report_type_selection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::page_report_type_selection)
{
    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->tableWidget_tags->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_global_report_name->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEdit_save_report_path->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->checkBox_advance_html->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->checkBox_standard_html->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->checkBox_pdf->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->checkBox_csv->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->checkBox_xml->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->checkBox_tag_export->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->checkBox_censor_skin_tone_image->setAttribute(Qt::WA_MacShowFocusRect,false);

    ui->lineEdit_save_report_path->setEnabled(false);

    ///------Tags-----
    horizontal_header_select_tags = new QTableWidgetItem();
    horizontal_header_select_tags->setText("Tags");

    horizontal_header_select_tags->setCheckState(Qt::Unchecked);
    horizontal_header_select_tags->setIcon(QIcon("../icons/unchecked.png"));

    ui->tableWidget_tags->setHorizontalHeaderItem(0,horizontal_header_select_tags);
    //    ui->pushButton_report_save_path->setIcon(QIcon("../icons/open_dir.png"));

    select_all_tags = true;
    QObject::connect((QObject*)ui->tableWidget_tags->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_tags_horizontal_header_clicked(int)));

    bool_report_type_is_selected = false;
    bool_is_tags_selected = false;

    file_dialog_obj.setOption(QFileDialog::DontUseNativeDialog);
    QList<QUrl> drives;
    drives << QUrl::fromLocalFile(QDir("/Volumes/").absolutePath());
    drives << QUrl::fromLocalFile(QDir("/").absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Downloads")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Desktop")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Documents")).absolutePath());
    file_dialog_obj.setSidebarUrls(drives);
    file_dialog_obj.setDirectory(getenv("HOME"));

}

page_report_type_selection::~page_report_type_selection()
{
    delete ui;
}

void page_report_type_selection::set_narad_muni_for_report(narad_muni *obj)
{
    narad_muni_for_report_obj = obj;
}

void page_report_type_selection::pub_initialise_report_type_page()
{
    report_dir = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Location_Global_Report_In_Result_QString).toString();

    ui->lineEdit_save_report_path->setText(report_dir);
    ui->lineEdit_global_report_name->clear();

    ui->checkBox_advance_html->setChecked(false);
    ui->checkBox_csv->setChecked(false);
    ui->checkBox_pdf->setChecked(false);
    ui->checkBox_standard_html->setChecked(false);
    ui->checkBox_xml->setChecked(false);

    ui->radioButton_full_plugin->setChecked(false);
    ui->radioButton_tags->setChecked(true);
    if(horizontal_header_select_tags->checkState() == Qt::Checked)
    {
        horizontal_header_select_tags->setIcon(QIcon("../icons/unchecked.png"));
        horizontal_header_select_tags->setCheckState(Qt::Unchecked);
    }

    ui->checkBox_censor_skin_tone_image->setDisabled(true);
    ui->checkBox_censor_skin_tone_image->setChecked(false);

    ui->checkBox_tag_export->setChecked(false);
    set_report_default_name();
    display_tags_table();


}

void page_report_type_selection::display_tags_table()
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QString command = "SELECT tag_name FROM tags";
    QString tags_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QStringList list_tags_from_db = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,tags_db_path,Q_FUNC_INFO);

    QString command_2 = "SELECT selected_colour FROM tags";
    QStringList list_tag_colour_from_db = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_2,0,tags_db_path,Q_FUNC_INFO);

    list_tags_from_db.removeAll("");
    list_tags_from_db.removeDuplicates();

    list_tag_colour_from_db.removeAll("");
    //  list_tag_colour_from_db.removeDuplicates();

    if(list_tags_from_db.size() != list_tag_colour_from_db.size())
    {
        recon_static_functions::app_debug("---FAILED--- 'list_tags_from_db' not equal with 'list_tag_colour_from_db' ", Q_FUNC_INFO);
        return;
    }

    //-Insert one bookmark item in list on top
    list_tags_from_db.prepend(QString(MACRO_Tag_Name_Bookmarks));
    list_tag_colour_from_db.prepend(""); //To make both list count equal.
    list_tags_from_db.append(QString(MACRO_Tag_Name_Screenshots));
    list_tags_from_db.append(QString(MACRO_Tag_Name_Saved_graphs));
    list_tags_from_db.append(QString(MACRO_Tag_Name_Saved_Maps));
    list_tags_from_db.append(QString(MACRO_Tag_Name_Examiner_Comments));

    tags_list_from_database.clear();
    tags_list_from_database = list_tags_from_db;

    ///clearing previous tag entries
    while(ui->tableWidget_tags->rowCount())
        ui->tableWidget_tags->removeRow(0);

    for(int i = 0; i < list_tags_from_db.size(); i++)
    {
        if(list_tags_from_db.at(i) == QString(""))
            continue;

        QTableWidgetItem *item_tags = new QTableWidgetItem();

        if(list_tags_from_db.at(i) == QString(MACRO_Tag_Name_Bookmarks))
        {
            item_tags->setText(list_tags_from_db.at(i));
            item_tags->setCheckState(Qt::Unchecked);
            item_tags->setIcon(QIcon("../icons/tag_icons/bookmarks.png"));
            // bool_is_tags_selected = true;
        }
        else if(list_tags_from_db.at(i) == QString(MACRO_Tag_Name_Screenshots))
        {
            item_tags->setText(list_tags_from_db.at(i));
            item_tags->setCheckState(Qt::Unchecked);
            item_tags->setIcon(QIcon("../icons/Features/screenshots.png"));
        }
        else if(list_tags_from_db.at(i) == QString(MACRO_Tag_Name_Saved_graphs))
        {
            item_tags->setText(list_tags_from_db.at(i));
            item_tags->setCheckState(Qt::Unchecked);
            item_tags->setIcon(QIcon("../icons/Features/graphs.png"));
        }
        else if(list_tags_from_db.at(i) == QString(MACRO_Tag_Name_Saved_Maps))
        {
            item_tags->setText(list_tags_from_db.at(i));
            item_tags->setCheckState(Qt::Unchecked);
            item_tags->setIcon(QIcon("../icons/Features/locations.png"));
        }
        else if(list_tags_from_db.at(i) == QString(MACRO_Tag_Name_Examiner_Comments))
        {
            item_tags->setText(list_tags_from_db.at(i));
            item_tags->setCheckState(Qt::Unchecked);
            item_tags->setIcon(QIcon("../icons/Features/examiner_comments.png"));
        }
        else
        {
            item_tags->setText(list_tags_from_db.at(i));
            item_tags->setCheckState(Qt::Unchecked);
            item_tags->setIcon(QIcon("../icons/tag_icons/" +list_tag_colour_from_db.at(i).toLower().replace(" ","_")));
        }

        ui->tableWidget_tags->insertRow(i);
        ui->tableWidget_tags->setRowHeight(i,25);
        ui->tableWidget_tags->setItem(i,0,item_tags);
    }

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void page_report_type_selection::slot_tags_horizontal_header_clicked(int index)
{

    if(horizontal_header_select_tags->checkState())
    {
        select_all_tags = false;
        bool_is_tags_selected = false;

        horizontal_header_select_tags->setIcon(QIcon("../icons/unchecked.png"));
        horizontal_header_select_tags->setCheckState(Qt::Unchecked);

        for(int i = 0; i < tags_list_from_database.size(); i++)
        {
            ui->tableWidget_tags->item(i,0)->setCheckState(Qt::Unchecked);
        }
    }
    else
    {
        select_all_tags = true;
        bool_is_tags_selected = true;

        horizontal_header_select_tags->setIcon(QIcon("../icons/checked.png"));
        horizontal_header_select_tags->setCheckState(Qt::Checked);

        for(int i = 0; i < tags_list_from_database.size(); i++)
        {
            ui->tableWidget_tags->item(i,0)->setCheckState(Qt::Checked);
        }
    }


    if(bool_report_type_is_selected && bool_is_tags_selected)
        emit signal_report_type_selected(true);
    else
        emit signal_report_type_selected(false);
}

void page_report_type_selection::set_report_default_name()
{
    QString name = "Report " + QDateTime::currentDateTime().toString(narad_muni_for_report_obj->get_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString).toString()) ;
    ui->lineEdit_global_report_name->setPlaceholderText(name);
}


void page_report_type_selection::on_pushButton_report_save_path_clicked()
{
    // QString path_1 = global_narad_muni_class_obj->get_field(MACRO_NARAD_Report_Location_Global_Report_In_Result_QString).toString();

    QString path_1 = ui->lineEdit_save_report_path->text();
    QString path;// = QFileDialog::getExistingDirectory(this, "GLobal Reports", path_1, QFileDialog::ShowDirsOnly);

    file_dialog_obj.setFileMode(QFileDialog::Directory);

    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            path = selected_files.at(0);
        }
    }
    if(path.trimmed() == "")
        return;

    if(!path.trimmed().endsWith("/"))
        path.append("/");

    ui->lineEdit_save_report_path->setText(path);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Location_Global_Report_In_Result_QString, path);

}


void page_report_type_selection::on_radioButton_full_plugin_toggled(bool checked)
{
    if(checked)
        ui->tableWidget_tags->setEnabled(false);

    emit signal_report_type_selected(bool_report_type_is_selected);
}

void page_report_type_selection::on_radioButton_tags_toggled(bool checked)
{
    if(checked)
        ui->tableWidget_tags->setEnabled(true);

    if(bool_report_type_is_selected && bool_is_tags_selected)
        emit signal_report_type_selected(true);
    else
        emit signal_report_type_selected(false);

}

void page_report_type_selection::on_radioButton_tags_clicked()
{
    // ui->checkBox_tag_export->setEnabled(true);

    emit signal_report_scope_selected(MACRO_REPORT_SCOPE_TAGS_QString);
}

void page_report_type_selection::on_radioButton_full_plugin_clicked()
{
    //   ui->checkBox_tag_export->setEnabled(false);
    emit signal_report_scope_selected(MACRO_REPORT_SCOPE_FULL_QString);
}

void page_report_type_selection::on_tableWidget_tags_cellClicked(int row, int column)
{
    ui->tableWidget_tags->selectRow(row);


    bool_is_tags_selected = false;
    for(int i = 0; i < ui->tableWidget_tags->rowCount(); i++)
    {
        if(ui->tableWidget_tags->item(i,0)->checkState() == Qt::Checked)
        {
            bool_is_tags_selected = true;
            break;
        }
        else
        {
            bool_is_tags_selected = false;
        }
    }

    if(bool_report_type_is_selected && bool_is_tags_selected)
        emit signal_report_type_selected(true);
    else
        emit signal_report_type_selected(false);


}

void page_report_type_selection::on_checkBox_advance_html_toggled(bool checked)
{
    if(checked)
    {
        //-Emit Signal true.
        bool_report_type_is_selected = true;
    }
    else
    {
        if(ui->checkBox_standard_html->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_pdf->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_csv->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_xml->isChecked())
            bool_report_type_is_selected = true;
        else
            bool_report_type_is_selected = false;

    }

    //- To combine check for both Tags and Report Type
    if(ui->radioButton_tags->isChecked())
    {
        if(bool_report_type_is_selected && bool_is_tags_selected)
            emit signal_report_type_selected(true);
        else
            emit signal_report_type_selected(false);
    }
    else
    {
        emit signal_report_type_selected(bool_report_type_is_selected);
    }
}

void page_report_type_selection::on_checkBox_standard_html_toggled(bool checked)
{
    if(checked)
    {
        //-Emit Signal true.
        bool_report_type_is_selected = true;
    }
    else
    {
        if(ui->checkBox_advance_html->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_pdf->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_csv->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_xml->isChecked())
            bool_report_type_is_selected = true;
        else
            bool_report_type_is_selected = false;

    }

    //- To combine check for both Tags and Report Type
    if(ui->radioButton_tags->isChecked())
    {
        if(bool_report_type_is_selected && bool_is_tags_selected)
            emit signal_report_type_selected(true);
        else
            emit signal_report_type_selected(false);
    }
    else
    {
        emit signal_report_type_selected(bool_report_type_is_selected);
    }

}

void page_report_type_selection::on_checkBox_pdf_toggled(bool checked)
{
    if(checked)
    {
        //-Emit Signal true.
        bool_report_type_is_selected = true;
    }
    else
    {
        if(ui->checkBox_advance_html->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_standard_html->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_csv->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_xml->isChecked())
            bool_report_type_is_selected = true;
        else
            bool_report_type_is_selected = false;
    }

    //- To combine check for both Tags and Report Type
    if(ui->radioButton_tags->isChecked())
    {
        if(bool_report_type_is_selected && bool_is_tags_selected)
            emit signal_report_type_selected(true);
        else
            emit signal_report_type_selected(false);
    }
    else
    {
        emit signal_report_type_selected(bool_report_type_is_selected);
    }

}

void page_report_type_selection::on_checkBox_csv_toggled(bool checked)
{
    if(checked)
    {
        //-Emit Signal true.
        bool_report_type_is_selected = true;
    }
    else
    {
        if(ui->checkBox_advance_html->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_standard_html->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_pdf->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_xml->isChecked())
            bool_report_type_is_selected = true;
        else
            bool_report_type_is_selected = false;

    }
    //- To combine check for both Tags and Report Type
    if(ui->radioButton_tags->isChecked())
    {
        if(bool_report_type_is_selected && bool_is_tags_selected)
            emit signal_report_type_selected(true);
        else
            emit signal_report_type_selected(false);
    }
    else
    {
        emit signal_report_type_selected(bool_report_type_is_selected);
    }

}

void page_report_type_selection::on_checkBox_xml_toggled(bool checked)
{
    if(checked)
    {
        //-Emit Signal true.
        bool_report_type_is_selected = true;
    }
    else
    {
        if(ui->checkBox_advance_html->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_standard_html->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_pdf->isChecked())
            bool_report_type_is_selected = true;
        else if(ui->checkBox_csv->isChecked())
            bool_report_type_is_selected = true;
        else
            bool_report_type_is_selected = false;

    }
    //- To combine check for both Tags and Report Type
    if(ui->radioButton_tags->isChecked())
    {
        if(bool_report_type_is_selected && bool_is_tags_selected)
            emit signal_report_type_selected(true);
        else
            emit signal_report_type_selected(false);
    }
    else
    {
        emit signal_report_type_selected(bool_report_type_is_selected);
    }
}


void page_report_type_selection::pub_set_report_type_narad_muni()
{
    QStringList selected_tags_list;

    if(ui->checkBox_advance_html->isChecked())
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Advance_Html_QString,"1");
    else
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Advance_Html_QString,"0");

    if(ui->checkBox_csv->isChecked())
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_CSV_QString,"1");
    else
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_CSV_QString,"0");

    if(ui->checkBox_pdf->isChecked())
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_PDF_QString,"1");
    else
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_PDF_QString,"0");

    if(ui->checkBox_standard_html->isChecked())
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Standard_Html_QString,"1");
    else
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Standard_Html_QString,"0");

    if(ui->checkBox_xml->isChecked())
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_XML_QString,"1");
    else
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_XML_QString,"0");

    if(ui->checkBox_tag_export->isChecked())
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Is_Export_Plugins_for_Tag_Checked_QString,"1");
    else
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Is_Export_Plugins_for_Tag_Checked_QString,"0");

    if(ui->checkBox_censor_skin_tone_image->isChecked())
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Censor_Skin_Tone_Image_Checked_QString,"1");
    else
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Censor_Skin_Tone_Image_Checked_QString,"0");


    if(ui->radioButton_full_plugin->isChecked())
    {
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Scope_Full_QString,ui->radioButton_full_plugin->text().trimmed());
    }
    else if(ui->radioButton_tags->isChecked())
    {
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Scope_Tags_QString,ui->radioButton_tags->text().trimmed());

        for(int i = 0; i < ui->tableWidget_tags->rowCount(); i++)
        {
            if(ui->tableWidget_tags->item(i,0)->checkState() == 2)
            {
                selected_tags_list << ui->tableWidget_tags->item(i,0)->text();
            }
        }

        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Selected_Tags_QStringList,selected_tags_list);
    }

    if(ui->lineEdit_global_report_name->text().trimmed().isEmpty())
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Name_QString, ui->lineEdit_global_report_name->placeholderText());
    else
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Global_Report_Name_QString, ui->lineEdit_global_report_name->text());

    if(ui->lineEdit_save_report_path->text().trimmed().isEmpty())
        narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Location_Global_Report_In_Result_QString, ui->lineEdit_save_report_path->placeholderText());
    else
    {
        QString m_path = ui->lineEdit_save_report_path->text().trimmed();
        if(!m_path.endsWith("/"))
            m_path.append("/");

        narad_muni_for_report_obj->set_field(MACRO_NARAD_Report_Location_Global_Report_In_Result_QString, m_path);
    }
}

void page_report_type_selection::on_checkBox_tag_export_toggled(bool checked)
{
    ui->checkBox_censor_skin_tone_image->setDisabled(!checked);
    ui->checkBox_censor_skin_tone_image->setChecked(checked);
}

