#include "examiner_space.h"
#include "ui_examiner_space.h"

examiner_space::examiner_space(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::examiner_space)
{
    ui->setupUi(this);

    setWindowFlag(Qt::Window);

    setWindowModality(Qt::WindowModality());

    setWindowTitle("Examiner Space");

    connect(ui->textEdit_examiner_notes,SIGNAL(textChanged()),this,SLOT(slot_textedit_examiner_notes_textchanged()));

    create_examiner_notes_custom_ui();

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->pushButton_to_do_list_add_task->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_to_do_list_remove_task->setIcon(QIcon("../icons/signs/remove.png"));
    ui->pushButton_to_do_list_edit_task->setIcon(QIcon("../icons/signs/edit.png"));

    ui->tableWidget_to_do_list->verticalHeader()->setVisible(true);
    ui->tableWidget_to_do_list->verticalHeader()->setFixedWidth(25);

    ui->tableWidget_to_do_list->hideColumn(enum_to_do_list_table_column_record_no);

    message_dialog_obj = new message_dialog(this);
    connect(message_dialog_obj,SIGNAL(signal_yes_no_check(bool)),this,SLOT(slot_message_box_yes_no_clicked(bool)));

    htmltopdf_wkhtmltopdf_obj = new htmltopdf_wkhtmltopdf(this);
    connect(this,SIGNAL(signal_create_pdf_wkhtmltopdf(QString,QString)),htmltopdf_wkhtmltopdf_obj,SLOT(slot_create_pdf_wkhtmltopdf(QString,QString)));
    connect(htmltopdf_wkhtmltopdf_obj,SIGNAL(signal_htmltopdf_finished_wkhtmltopdf(QString,bool)),this,SLOT(slot_html_to_pdf_finished(QString,bool)));


    display_loading_progress_bar_obj = new display_loading_progress_bar();
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));

    display_loading_progress_bar_obj->pub_set_progress_bar_title("Examiner Space");
    display_loading_progress_bar_obj->pub_set_label_messsge("Building Report......");

    bool_cancel_operation = false;

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


examiner_space::~examiner_space()
{
    examiner_notes_html_QFile.close();

    delete ui;
}


void examiner_space::closeEvent(QCloseEvent *event)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    examiner_notes_html_QFile.close();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void examiner_space::pub_set_essential()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString examiner_notes_html_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Examiner_Space_In_Result_QString).toString() + "Examiner_Notes.html";
    examiner_notes_html_QFile.setFileName(examiner_notes_html_file_path);


    to_do_list_database_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Examiner_Space_In_Result_QString).toString() + "examiner_space.sqlite";

    if(!examiner_notes_html_QFile.open(QFile::ReadWrite))
    {
        recon_static_functions::app_debug(" File Open Examiner Notes Html -----FAILED-----" + examiner_notes_html_QFile.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error:" + examiner_notes_html_QFile.errorString(), Q_FUNC_INFO);
        return;
    }


    create_table_to_do_list();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void examiner_space::pub_prepare_display()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    display_examiner_notes();
    display_to_do_list();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void examiner_space::display_examiner_notes()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    examiner_notes_html_QFile.seek(0);

    QTextStream m_txt_stream(&examiner_notes_html_QFile);

    ui->textEdit_examiner_notes->setHtml(m_txt_stream.readAll());

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void examiner_space::display_to_do_list()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    ui->tableWidget_to_do_list->setRowCount(0);

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase to_do_list_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    to_do_list_db.setDatabaseName(to_do_list_database_path);
    if(!to_do_list_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + to_do_list_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + to_do_list_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QString command_select_task = QString("select INT,bookmark,task from to_do_list order by bookmark ASC");

    QSqlQuery query_select(to_do_list_db);

    query_select.prepare(command_select_task);
    if(!query_select.exec())
    {
        recon_static_functions::app_debug(" select query ----FAILED---- " + query_select.executedQuery(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error" + query_select.lastError().text(), Q_FUNC_INFO);
        to_do_list_db.close();
        return;
    }

    while(query_select.next())
    {
        QString record_no = query_select.value("INT").toString();
        QString task = query_select.value("task").toString();
        QString m_bookmark = query_select.value("bookmark").toString();

        int row_count = ui->tableWidget_to_do_list->rowCount();

        ui->tableWidget_to_do_list->insertRow(row_count);


        QTableWidgetItem *item_task = new QTableWidgetItem;
        QTableWidgetItem *item_record = new QTableWidgetItem;

        if(m_bookmark == "1")
        {
            item_task->setCheckState(Qt::Checked);
        }
        else if(m_bookmark == "0")
        {
            item_task->setCheckState(Qt::Unchecked);
        }

        item_record->setText(record_no);
        item_task->setText(task);

        item_task->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
        item_record->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);

        ui->tableWidget_to_do_list->setItem(row_count, enum_to_do_list_table_column_record_no, item_record);
        ui->tableWidget_to_do_list->setItem(row_count, enum_to_do_list_table_column_task, item_task);
    }

    to_do_list_db.close();

    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}

void examiner_space::slot_textedit_examiner_notes_textchanged()
{
    recon_static_functions::app_debug("Starts", Q_FUNC_INFO);

    examiner_notes_html_QFile.seek(0);
    examiner_notes_html_QFile.resize(0);

    QTextStream m_stream(&examiner_notes_html_QFile);
    m_stream << ui->textEdit_examiner_notes->toHtml().toLocal8Bit();

    if(ui->textEdit_examiner_notes->toPlainText().isEmpty())
    {
        examiner_notes_html_QFile.remove();
        if(!examiner_notes_html_QFile.open(QFile::ReadWrite))
        {
            recon_static_functions::app_debug(" File Open Examiner Notes Html -----FAILED-----" + examiner_notes_html_QFile.fileName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error:" + examiner_notes_html_QFile.errorString(), Q_FUNC_INFO);
            return;
        }
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}


void examiner_space::create_examiner_notes_custom_ui()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString examiner_space_icons_dir_path = QString("../icons/signs/");

    QFont font_s;
    font_s.setPixelSize(15);

    groupbox_text_formatting_tool = new QGroupBox(this);
    groupbox_text_formatting_tool->setParent(ui->frame_examiner_notes);
    groupbox_text_formatting_tool->setGeometry(4,5,156,35);
    groupbox_text_formatting_tool->setContentsMargins(4,4,4,4);


    QPushButton *pushbutton_examiner_notes_bold = new QPushButton(QString(""),this);
    pushbutton_examiner_notes_bold->setParent(groupbox_text_formatting_tool);
    pushbutton_examiner_notes_bold->setFixedWidth(30);
    pushbutton_examiner_notes_bold->setFixedHeight(30);
    pushbutton_examiner_notes_bold->move(3,2);
    pushbutton_examiner_notes_bold->setFont(font_s);
    pushbutton_examiner_notes_bold->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_examiner_notes_bold->setIcon(QIcon(examiner_space_icons_dir_path + QString("bold.png")));
    pushbutton_examiner_notes_bold->setToolTip(QString("Bold"));
    pushbutton_examiner_notes_bold->setFocusPolicy(Qt::NoFocus);
    connect(pushbutton_examiner_notes_bold, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_examiner_notes_bold_clicked()));


    QPushButton *pushbutton_examiner_notes_italic = new QPushButton(QString(""),this);
    pushbutton_examiner_notes_italic->setParent(groupbox_text_formatting_tool);
    pushbutton_examiner_notes_italic->setFixedWidth(30);
    pushbutton_examiner_notes_italic->setFixedHeight(30);
    pushbutton_examiner_notes_italic->move(43,2);
    pushbutton_examiner_notes_italic->setFont(font_s);
    pushbutton_examiner_notes_italic->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_examiner_notes_italic->setIcon(QIcon(examiner_space_icons_dir_path + QString("italic.png")));
    pushbutton_examiner_notes_italic->setToolTip(QString("Italic"));
    pushbutton_examiner_notes_italic->setFocusPolicy(Qt::NoFocus);
    connect(pushbutton_examiner_notes_italic, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_examiner_notes_italic_clicked()));


    QPushButton *pushbutton_examiner_notes_underline = new QPushButton(QString(""),this);
    pushbutton_examiner_notes_underline->setParent(groupbox_text_formatting_tool);
    pushbutton_examiner_notes_underline->setFixedWidth(30);
    pushbutton_examiner_notes_underline->setFixedHeight(30);
    pushbutton_examiner_notes_underline->move(83,2);
    pushbutton_examiner_notes_underline->setFont(font_s);
    pushbutton_examiner_notes_underline->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_examiner_notes_underline->setIcon(QIcon(examiner_space_icons_dir_path + QString("underline.png")));
    pushbutton_examiner_notes_underline->setToolTip(QString("Underline"));
    pushbutton_examiner_notes_underline->setFocusPolicy(Qt::NoFocus);
    connect(pushbutton_examiner_notes_underline, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_examiner_notes_underline_clicked()));


    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    pushbutton_examiner_notes_color = new QPushButton(pix, QString(""),this);
    pushbutton_examiner_notes_color->setParent(groupbox_text_formatting_tool);
    pushbutton_examiner_notes_color->setFixedWidth(30);
    pushbutton_examiner_notes_color->setFixedHeight(24);
    pushbutton_examiner_notes_color->move(123,2);
    pushbutton_examiner_notes_color->setFont(font_s);
    pushbutton_examiner_notes_color->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_examiner_notes_color->setToolTip(QString("Color"));
    pushbutton_examiner_notes_color->setFocusPolicy(Qt::NoFocus);
    connect(pushbutton_examiner_notes_color, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_examiner_notes_color_clicked()));


    ui->checkBox_use_utc->setParent(ui->groupBox_timestamp);
    ui->pushButton_add_timestamp->setParent(ui->groupBox_timestamp);

    QPushButton *pushbutton_examiner_notes_report = new QPushButton(QString(""),this);
    pushbutton_examiner_notes_report->setParent(ui->frame_examiner_notes);
    pushbutton_examiner_notes_report->setFixedWidth(65);
    pushbutton_examiner_notes_report->setFixedHeight(24);
    pushbutton_examiner_notes_report->move(350,10);
    pushbutton_examiner_notes_report->setFont(font_s);
    pushbutton_examiner_notes_report->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_examiner_notes_report->setIcon(QIcon(examiner_space_icons_dir_path + QString("export.png")));
    pushbutton_examiner_notes_report->setToolTip(QString("Report"));
    pushbutton_examiner_notes_underline->setFocusPolicy(Qt::NoFocus);

    QMenu *menu_export = new QMenu(this);

    QAction *action_report_html  =  new QAction(QString("Report HTML") , this);
    QAction *action_report_pdf   =  new QAction(QString("Report PDF") , this);


    connect(action_report_html, SIGNAL(triggered()), this, SLOT(slot_action_report_html_triggered()));
    connect(action_report_pdf, SIGNAL(triggered()), this, SLOT(slot_action_report_pdf_triggered()));

    menu_export->addAction(action_report_html);
    menu_export->addAction(action_report_pdf);

    pushbutton_examiner_notes_report->setMenu(menu_export);


    //    Note :- Removed ODT report - manoj sir Jan 05, 2022
    //    QAction *action_report_odt   =  new QAction(QString("Report ODT") , this);
    //    connect(action_report_odt, SIGNAL(triggered()), this, SLOT(slot_action_report_odt_triggered()));
    //    menu_export->addAction(action_report_odt);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}


void examiner_space::mergeFormatOnWordOrSelection_examiner_notes(const QTextCharFormat &format)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QTextCursor cursor = ui->textEdit_examiner_notes->textCursor();

    cursor.mergeCharFormat(format);

    ui->textEdit_examiner_notes->mergeCurrentCharFormat(format);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}


void examiner_space::colorChanged_examiner_notes(const QColor &color)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QPixmap pix(16, 16);
    pix.fill(color);

    pushbutton_examiner_notes_color->setIcon(pix);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}


void examiner_space::slot_pushbutton_examiner_notes_bold_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QTextCursor cursor = ui->textEdit_examiner_notes->textCursor();

    if(!cursor.charFormat().font().bold())
    {
        QTextCharFormat fmt;
        fmt.setFontWeight(QFont::Bold);
        mergeFormatOnWordOrSelection_examiner_notes(fmt);
    }
    else
    {
        QTextCharFormat fmt;
        fmt.setFontWeight(QFont::Normal);
        mergeFormatOnWordOrSelection_examiner_notes(fmt);
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}


void examiner_space::slot_pushbutton_examiner_notes_italic_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QTextCursor cursor = ui->textEdit_examiner_notes->textCursor();

    if(!cursor.charFormat().font().italic())
    {
        QTextCharFormat fmt;
        fmt.setFontItalic(true);
        mergeFormatOnWordOrSelection_examiner_notes(fmt);
    }
    else
    {
        QTextCharFormat fmt;
        fmt.setFontItalic(false);
        mergeFormatOnWordOrSelection_examiner_notes(fmt);
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}


void examiner_space::slot_pushbutton_examiner_notes_underline_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QTextCursor cursor = ui->textEdit_examiner_notes->textCursor();

    if(!cursor.charFormat().font().underline())
    {
        QTextCharFormat fmt;
        fmt.setFontUnderline(true);
        mergeFormatOnWordOrSelection_examiner_notes(fmt);
    }
    else
    {
        QTextCharFormat fmt;
        fmt.setFontUnderline(false);
        mergeFormatOnWordOrSelection_examiner_notes(fmt);
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}


void examiner_space::slot_pushbutton_examiner_notes_color_clicked()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    QColor col = QColorDialog::getColor(ui->textEdit_examiner_notes->textColor(), this);

    if(!col.isValid())
        return;

    QTextCharFormat fmt;
    fmt.setForeground(col);

    mergeFormatOnWordOrSelection_examiner_notes(fmt);
    colorChanged_examiner_notes(col);


    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void examiner_space::create_table_to_do_list()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString command = "CREATE TABLE IF NOT EXISTS to_do_list(INT INTEGER PRIMARY KEY,bookmark VARCHAR(10),task VARCHAR(1024))";

    recon_helper_standard_obj->execute_db_command_by_dbpath(command, to_do_list_database_path, Q_FUNC_INFO);

    recon_static_functions::app_debug(" Ends ", Q_FUNC_INFO);
}

void examiner_space::on_tableWidget_to_do_list_itemClicked(QTableWidgetItem *item)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    if(item == NULL)
    {
        return;
    }

    int select_row = item->row();

    int row_count = ui->tableWidget_to_do_list->rowCount();

    if(select_row < 0 || row_count <= 0)
        return;

    if(ui->tableWidget_to_do_list->item(select_row, enum_to_do_list_table_column_record_no) == NULL)
    {
        return;
    }

    QString m_bookmark = "0";

    if(item->checkState() == Qt::Unchecked)
    {
        m_bookmark = "0";
    }
    else if(item->checkState() == Qt::Checked)
    {
        m_bookmark = "1";
    }

    QString record_no = ui->tableWidget_to_do_list->item(select_row, enum_to_do_list_table_column_record_no)->text();

    QString command_update_bookmark = QString("UPDATE to_do_list SET bookmark = ? WHERE INT = ?");
    QStringList list;
    list << m_bookmark << record_no;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update_bookmark, list, to_do_list_database_path, Q_FUNC_INFO);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void examiner_space::on_pushButton_to_do_list_add_task_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    int row_count = ui->tableWidget_to_do_list->rowCount();

    if(ui->tableWidget_to_do_list->item(row_count - 1, enum_to_do_list_table_column_record_no) == NULL && row_count > 0)
        return;

    ui->tableWidget_to_do_list->insertRow(row_count);
    ui->tableWidget_to_do_list->setRowHeight(row_count, 23);

    line_edit_task_add = new QLineEdit(this);
    line_edit_task_add->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(line_edit_task_add, SIGNAL(returnPressed()), this, SLOT(slot_line_edit_task_add_returnpressed()));

    ui->tableWidget_to_do_list->setCellWidget(row_count, enum_to_do_list_table_column_task, line_edit_task_add);

    ui->tableWidget_to_do_list->setCurrentCell(row_count, enum_to_do_list_table_column_task);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void examiner_space::slot_line_edit_task_add_returnpressed()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    if(ui->tableWidget_to_do_list->currentRow() < 0)
        return;

    QString new_task = line_edit_task_add->text();

    if(!new_task.trimmed().isEmpty())
    {
        QString m_bookmark = "0";

        QString command_insert = "INSERT into to_do_list(task, bookmark) VALUES(?,?)";
        QStringList value_list;
        value_list << new_task << m_bookmark;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert , value_list , to_do_list_database_path, Q_FUNC_INFO);

        ui->pushButton_to_do_list_edit_task->setEnabled(true);
    }

    display_to_do_list();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void examiner_space::on_pushButton_to_do_list_remove_task_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    int current_row = ui->tableWidget_to_do_list->currentRow();

    if(current_row < 0)
        return;

    if(ui->tableWidget_to_do_list->item(current_row, enum_to_do_list_table_column_record_no) == NULL)
    {
        ui->tableWidget_to_do_list->removeRow(current_row);
        ui->tableWidget_to_do_list->setCurrentCell( -1, enum_to_do_list_table_column_record_no);

        return;
    }

    QString record_no = ui->tableWidget_to_do_list->item(current_row, enum_to_do_list_table_column_record_no)->text();

    QString command_remove_task = QString("DELETE FROM to_do_list where INT = ?");
    QStringList value_list;
    value_list << record_no;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_remove_task , value_list , to_do_list_database_path , Q_FUNC_INFO);

    ui->tableWidget_to_do_list->removeRow(current_row);

    ui->pushButton_to_do_list_edit_task->setEnabled(true);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void examiner_space::on_pushButton_to_do_list_edit_task_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    ui->pushButton_to_do_list_edit_task->setEnabled(false);

    int current_row = ui->tableWidget_to_do_list->currentRow();
    int row_count = ui->tableWidget_to_do_list->rowCount();

    if(current_row < 0 || row_count <= 0)
    {
        ui->pushButton_to_do_list_edit_task->setEnabled(true);
        return;
    }

    if(ui->tableWidget_to_do_list->item(current_row, enum_to_do_list_table_column_record_no) == NULL)
    {
        ui->tableWidget_to_do_list->removeRow(current_row);
        ui->tableWidget_to_do_list->setCurrentCell( -1, enum_to_do_list_table_column_record_no);
        ui->pushButton_to_do_list_edit_task->setEnabled(true);

        return;
    }

    if(ui->tableWidget_to_do_list->item(current_row, enum_to_do_list_table_column_task) == NULL)
    {
        ui->tableWidget_to_do_list->removeRow(current_row);
        ui->tableWidget_to_do_list->setCurrentCell( -1, enum_to_do_list_table_column_task);
        ui->pushButton_to_do_list_edit_task->setEnabled(true);

        return;
    }

    QString task = ui->tableWidget_to_do_list->item(current_row, enum_to_do_list_table_column_task)->text();
    record_no = ui->tableWidget_to_do_list->item(current_row, enum_to_do_list_table_column_record_no)->text();


    line_edit_task_edit = new QLineEdit(this);
    line_edit_task_edit->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(line_edit_task_edit, SIGNAL(returnPressed()), this, SLOT(slot_line_edit_task_edit_returnpressed()));

    line_edit_task_edit->setText(task);
    line_edit_task_edit->selectAll();

    ui->tableWidget_to_do_list->setCellWidget(current_row, enum_to_do_list_table_column_task,line_edit_task_edit);
    ui->tableWidget_to_do_list->setCurrentCell(current_row, enum_to_do_list_table_column_task);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void examiner_space::slot_line_edit_task_edit_returnpressed()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString edit_task_data = line_edit_task_edit->text();

    line_edit_task_edit->close();

    QString command_update_task = QString("UPDATE to_do_list SET task = ? WHERE INT = ?");
    QStringList list;
    list << edit_task_data << record_no;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update_task, list, to_do_list_database_path, Q_FUNC_INFO);

    ui->pushButton_to_do_list_edit_task->setEnabled(true);

    display_to_do_list();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void examiner_space::on_tabWidget_examiner_space_currentChanged(int index)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    if(index == 0)
    {
        display_examiner_notes();
    }
    else if(index == 1)
    {
        display_to_do_list();
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void examiner_space::on_pushButton_add_timestamp_clicked()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString timestamp;

    if(ui->checkBox_use_utc->isChecked())
    {
        timestamp = QDateTime::currentDateTimeUtc().toString(("MMM-dd-yyyy hh:mm") + QString(" UTC+00:00") + QString(" "));

    }
    else
    {
        struct_global_desktop_info strct_exm_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);
        QString current_machine_timezone = QString("GMT") + strct_exm_obj.timezone_offset_readable_QString;

        timestamp = QDateTime::currentDateTime().toString(("MMM-dd-yyyy hh:mm ")) + current_machine_timezone + QString(" ");

    }

    ui->textEdit_examiner_notes->insertPlainText(timestamp);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

QString examiner_space::write_report_initials()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString case_details;

    case_details += report_template_html_standard::html_header1().toLocal8Bit().data();
    case_details += report_template_html_standard::html_title("Eaminer Space").toLocal8Bit().data();
    case_details += html_header_after_title().toLocal8Bit().data();

    case_details += report_template_html_standard::html_report_main_title("./resources/agency_logo.png",global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Agency_Name_QString).toString()).toLocal8Bit().data();
    case_details += global_recon_helper_singular_class_obj->write_html_only_case_info("Eaminer Space", global_narad_muni_class_obj).toLocal8Bit().data();


    recon_static_functions::app_debug("- End",Q_FUNC_INFO);

    return case_details;
}

QString examiner_space::html_header_after_title()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString header_after_title = "<link rel=\"stylesheet\" href=\"./resources/style.css\" type=\"text/css\" /><style type=\"text/css\"> .c3{color:RGB(255,255,255)} .c4{color:RGB(128,128,128)}  .c6{color:RGB(255,0,0)}  .g1, tr.g1 td{background:RGB(51,113,163)} .g2, tr.g2 td{background:RGB(255,255,255)}  .g5, tr.g5 td{background:RGB(236,236,236)}     .g7, tr.g7 td{background:RGB(200,209,219)}    .g8, tr.g8 td{background:RGB(192,192,192)}    .s10{font-size:10pt} .s22{font-size:22pt} .s18{font-size:18pt}  .s14{font-size:14pt}    .s12{font-size:12pt}    .s11{font-size:11pt}    .p20{padding:20px}    .p10{padding:10px}  .wrapped { white-space: pre; white-space: pre-wrap; white-space: pre-line; white-space: -pre-wrap; white-space: -o-pre-wrap; white-space: -moz-pre-wrap; white-space: -hp-pre-wrap; word-wrap: break-word; width: 300px; } body { margin:0; padding:0 0 0 150px; } div#left-sidebar { position:absolute; top:20; left:0; width:150px; height:80%; } @media screen { body>div#left-sidebar { position:fixed; } } * html body { overflow:hidden; } * html div#content { height:60%; overflow:auto; } </style> </head>  <body> <div id=\"left-sidebar\"> <div class = \"stack\"> <h2 id =\"\"> <span class = \" s14\" id=\"\">" +QObject::tr(" ") + "</span> </h2> </div>";
    return header_after_title;

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

QString examiner_space::create_examiner_notes_dest_and_copy_essentials(QString dest_path)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString dir_name = QString(MACRO_Tag_Name_Examiner_Comments);
    dir_name.replace(" ", "_");

    QString report_currently_dir_name = dir_name + "_" + QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss");
    QString reports_dir_path = dest_path + report_currently_dir_name + "/";

    QDir dir;
    dir.mkpath(reports_dir_path);

    QString new_resource_path = reports_dir_path + "resources";
    dir.mkpath(new_resource_path);

    QFile file;

    QString agency_logo_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "agency_logo.png";

    file.copy(agency_logo_path, new_resource_path + "/agency_logo.png");
    file.copy("../Resources/graphics/html_reports/style.css", new_resource_path + "/style.css");

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

    return reports_dir_path;
}

QString examiner_space::create_html_report_for_examiner_notes()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString select_output_dir;

    file_dialog_obj.setNameFilter(("*"));
    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();
        if(selected_files.size() > 0)
        {
            select_output_dir = selected_files.at(0);
        }
    }

    select_output_dir = select_output_dir.trimmed();



    QString examiner_export_html_file_path;

    if(select_output_dir.isEmpty())
    {
        return "";
    }

    display_loading_progress_bar_obj->show();
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();

    if(!select_output_dir.endsWith("/"))
        select_output_dir.append("/");



    QString dest_dir = create_examiner_notes_dest_and_copy_essentials(select_output_dir);

    if(bool_cancel_operation)
        return "";

    QString examiner_export_file = recon_static_functions::get_available_filename("examiner_comments_report.html", dest_dir, Q_FUNC_INFO);
    examiner_export_html_file_path = dest_dir + examiner_export_file;

    QFile examiner_notes_html_report_QFile;

    examiner_notes_html_report_QFile.setFileName(examiner_export_html_file_path);

    if(!examiner_notes_html_report_QFile.open(QFile::ReadWrite))
    {
        recon_static_functions::app_debug(" File Open Examiner Notes Html Report -----FAILED-----" + examiner_notes_html_report_QFile.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error:- " + examiner_notes_html_report_QFile.errorString(), Q_FUNC_INFO);
        return "";
    }

    examiner_notes_html_report_QFile.write(write_report_initials().toLocal8Bit().data());

    examiner_notes_html_report_QFile.write(QString("<br>").toLocal8Bit().data());
    examiner_notes_html_report_QFile.write(report_template_html_standard::html_top_link().toLocal8Bit().data());
    examiner_notes_html_report_QFile.write(report_template_html_standard::html_table_start().toLocal8Bit().data());

    examiner_notes_html_report_QFile.write(QString("<tr><th width=200 > " + QString(MACRO_Tag_Name_Examiner_Comments) +  " </th></tr>").toLocal8Bit().data());

    if(bool_cancel_operation)
    {
        examiner_notes_html_report_QFile.close();
        return "";
    }

    QString report_data;


    QString examiner_comment = ui->textEdit_examiner_notes->toHtml().toLocal8Bit().trimmed();

    if(examiner_comment.contains("\n"))
        examiner_comment.remove("\n");

    report_data += "<tr><td class = \"wrapped\"> <strong> " + examiner_comment + " </strong> </td>";

    examiner_notes_html_report_QFile.write(report_data.toLocal8Bit().data());

    examiner_notes_html_report_QFile.flush();
    examiner_notes_html_report_QFile.close();


    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);

    return examiner_export_html_file_path;
}

void examiner_space::slot_action_report_html_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);


    QString examiner_export_html_file_path = create_html_report_for_examiner_notes();

    display_loading_progress_bar_obj->hide();
    if(bool_cancel_operation)
    {
        return;
    }



    if(QFileInfo(examiner_export_html_file_path).exists() && examiner_export_html_file_path != "")
    {
        QProcess proc;
        QStringList args;
        args << "-R" << "0777" << examiner_export_html_file_path;
        proc.execute("chmod", args);

        bool_message_box_yes_button_clicked = false;
        bool_message_box_clicked = false;
        message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO, ( "Report has been generated. Do you want to open it?"));
        message_dialog_obj->show();

        recon_static_functions::debug_conditional(" : infinite loop start", Q_FUNC_INFO);

        int proc_counter = 0;
        while(1)
        {
            proc_counter++;
            if(proc_counter % 100 == 0)
            {
                proc_counter = 0;
                QCoreApplication::processEvents();
            }

            if(bool_message_box_clicked)
                break ;
        }

        recon_static_functions::debug_conditional(" : infinite loop end", Q_FUNC_INFO);

        if(bool_message_box_yes_button_clicked)
        {
            QString html_file_path;
            html_file_path = examiner_export_html_file_path;

            QProcess myProcess;

            myProcess.startDetached("open",QStringList(html_file_path));
        }

    }
    else
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, ( "Report not exported."));
        message_dialog_obj->show();

        return;
    }


    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void examiner_space::slot_action_report_odt_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString select_output_dir;

    file_dialog_obj.setNameFilter(("*"));
    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();
        if(selected_files.size() > 0)
        {
            select_output_dir = selected_files.at(0);
        }
    }

    select_output_dir = select_output_dir.trimmed();


    QString examiner_export_odt_file_path;

    if(select_output_dir.isEmpty())
    {
        return;
    }

    if(!select_output_dir.endsWith("/"))
        select_output_dir.append("/");

    display_loading_progress_bar_obj->show();
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();

    QString dest_dir = create_examiner_notes_dest_and_copy_essentials(select_output_dir);

    QString examiner_export_file = recon_static_functions::get_available_filename("examiner_notes_report.odt", dest_dir, Q_FUNC_INFO);
    examiner_export_odt_file_path = dest_dir + examiner_export_file;

    QFile examiner_notes_odt_report_QFile;

    examiner_notes_odt_report_QFile.setFileName(examiner_export_odt_file_path);

    if(!examiner_notes_odt_report_QFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        recon_static_functions::app_debug(" File Open Examiner Notes odt Report -----FAILED-----" + examiner_notes_odt_report_QFile.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error:" + examiner_notes_odt_report_QFile.errorString(), Q_FUNC_INFO);
        return ;
    }


    examiner_notes_odt_report_QFile.write(write_odt_report_initials().toLocal8Bit().data());

    examiner_notes_odt_report_QFile.write(QString("<br><br>").toLocal8Bit().data());
    examiner_notes_odt_report_QFile.write(report_template_html_standard::html_table_start().toLocal8Bit().data());

    examiner_notes_odt_report_QFile.write(QString("<tr><th width=10> " + QString(MACRO_Tag_Name_Examiner_Comments)  +  " </th></tr>").toLocal8Bit().data());

    QString report_data;

    QString examiner_comment = ui->textEdit_examiner_notes->toHtml().toLocal8Bit().trimmed();

    if(examiner_comment.contains("\n"))
        examiner_comment.remove("\n");

    report_data += "<tr><td class = \"wrapped\"> <strong> " + examiner_comment + " </strong> </td>";

    if(bool_cancel_operation)
    {
        examiner_notes_odt_report_QFile.close();
        return;
    }

    examiner_notes_odt_report_QFile.write(report_data.toLocal8Bit().data());

    examiner_notes_odt_report_QFile.flush();
    examiner_notes_odt_report_QFile.close();

    display_loading_progress_bar_obj->hide();

    QFileInfo info(examiner_export_odt_file_path);
    if(info.exists())
    {
        if(bool_cancel_operation)
        {
            examiner_notes_odt_report_QFile.close();
            return;
        }

        QProcess proc;
        QStringList args;
        args << "-R" << "0777" << examiner_export_odt_file_path;
        proc.execute("chmod", args);

        bool_message_box_yes_button_clicked = false;
        bool_message_box_clicked = false;
        message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO, ( "File(s) has been exported. Do you want to open it?"));
        message_dialog_obj->show();

        recon_static_functions::debug_conditional(" : infinite loop start", Q_FUNC_INFO);
        int proc_counter = 0;
        while(1)
        {
            proc_counter++;
            if(proc_counter % 100 == 0)
            {
                proc_counter = 0;
                QCoreApplication::processEvents();
            }

            if(bool_message_box_clicked)
                break ;
        }
        recon_static_functions::debug_conditional(" : infinite loop end", Q_FUNC_INFO);

        if(bool_message_box_yes_button_clicked)
        {
            QString html_file_path;
            html_file_path = examiner_export_odt_file_path;

            QProcess myProcess;

            myProcess.startDetached("open",QStringList(html_file_path));

        }

    }
    else
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, ( "Report not exported."));
        message_dialog_obj->show();

        return;
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}


void examiner_space::slot_action_report_pdf_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);


    QString examiner_export_html_file_path = create_html_report_for_examiner_notes();

    if(QFileInfo(examiner_export_html_file_path).exists() && examiner_export_html_file_path != "")
    {
        QString html_file_path = examiner_export_html_file_path;
        html_file_path.chop(QString("html").size());

        QString dest_pdf_path = html_file_path.append("pdf");

        emit signal_create_pdf_wkhtmltopdf(examiner_export_html_file_path, dest_pdf_path);
    }
    else
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, ("Report not exported."));
        message_dialog_obj->show();

        return;
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void examiner_space::slot_html_to_pdf_finished(QString pdf_file_path, bool stauts)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);


    display_loading_progress_bar_obj->hide();

    if(!stauts)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION, ( "Report not generated."));
        message_dialog_obj->show();

        return;
    }

    if(bool_cancel_operation)
    {
        return;
    }


    QString examiner_export_pdf_file_path = pdf_file_path;

    QProcess proc;
    QStringList args;
    args << "-R" << "0777" << examiner_export_pdf_file_path;
    proc.execute("chmod", args);

    bool_message_box_yes_button_clicked = false;
    bool_message_box_clicked = false;
    message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO, ( "Report has been generated. Do you want to open it?"));
    message_dialog_obj->show();

    recon_static_functions::debug_conditional(" : infinite loop start", Q_FUNC_INFO);

    int proc_counter = 0;
    while(1)
    {
        proc_counter++;
        if(proc_counter % 100 == 0)
        {
            proc_counter = 0;
            QCoreApplication::processEvents();
        }

        if(bool_message_box_clicked)
            break ;
    }

    recon_static_functions::debug_conditional(" : infinite loop end", Q_FUNC_INFO);

    if(bool_message_box_yes_button_clicked)
    {
        QString html_file_path;
        html_file_path = examiner_export_pdf_file_path;

        QProcess myProcess;

        myProcess.startDetached("open",QStringList(html_file_path));
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void examiner_space::slot_message_box_yes_no_clicked(bool yes_no_clicked)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    bool_message_box_yes_button_clicked = yes_no_clicked;
    bool_message_box_clicked = true;

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}


QString examiner_space::write_odt_report_initials()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString case_details;

    QString icon = "./resources/agency_logo.png";
    case_details += "<img src=";
    case_details += "\"" + icon + "\"";
    case_details += "height=\"150\" width=\"150\"";

    case_details += global_recon_helper_singular_class_obj->write_html_only_case_info("Examiner Space", global_narad_muni_class_obj).toLocal8Bit().data();

    recon_static_functions::app_debug("- End",Q_FUNC_INFO);

    return case_details;
}

void examiner_space::slot_hide_loading_display_dialog_box(bool cancel_status)
{
    bool_cancel_operation = true;

    display_loading_progress_bar_obj->hide();
}

