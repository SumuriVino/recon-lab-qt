#include "strings_viewer.h"
#include "ui_strings_viewer.h"

strings_viewer::strings_viewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::strings_viewer)
{
    ui->setupUi(this);

    this->setMinimumHeight(40);
    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);


    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->lineEdit_search->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEdit_search->setClearButtonEnabled(true);


    proc_strings = new QProcess(this);
    connect(proc_strings, SIGNAL(readyRead()), this, SLOT(slot_proc_ready_read()));
    connect(proc_strings, SIGNAL(finished(int)), this, SLOT(slot_proc_strings_finished(int)));

    ui->textBrowser_strings->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textBrowser_strings, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(ShowContextMenu(const QPoint&)));

    //Tags
    add_tags_module_obj = new add_tags(this);

    connect(add_tags_module_obj, SIGNAL(tag_status(bool,QString,QString)), this, SLOT(slot_update_tags_submenu(bool,QString,QString)));
    connect(add_tags_module_obj, SIGNAL(signal_tags(QString,QString)), this, SLOT(slot_add_tags(QString,QString)));

    mainMenu = new QMenu(this);

    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    mainMenu->setFont(m_font);

    connect(mainMenu, SIGNAL(triggered(QAction*)), this, SLOT(slot_right_click_main_menu_triggered(QAction*)));

    act_select_all = new QAction(QString("Select All"), this);
    mainMenu->addAction(act_select_all);

    sub_menu_tags = mainMenu->addMenu(tr(QString(MACRO_Generic_Right_Click_Tags).toLocal8Bit().data()));
    sub_menu_tags->setFont(m_font);

    connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_tags_clicked(QAction*)));
    connect(sub_menu_tags,SIGNAL(aboutToShow()),this,SLOT(slot_abouttoshow_tags_in_submenu()));



}

strings_viewer::~strings_viewer()
{
    delete ui;
}

void strings_viewer::ShowContextMenu(const QPoint& pos)
{
    QPoint globalPos = ui->textBrowser_strings->mapToGlobal(pos);
    mainMenu->exec(globalPos);
}

void strings_viewer::pub_set_highlight_keyword(QString str)
{
    highlight_keyword = str;
}

void strings_viewer::pub_set_source_count_name(QString str)
{
    source_count_name = str;
}

void strings_viewer::pub_set_recon_file_info(struct_global_recon_file_info obj)
{
    st_globl_recon_file_info_obj = obj;
}

void strings_viewer::pub_hide_search_navigate_buttons(bool status)
{
    ui->pushButton_next->setHidden(status);
    ui->pushButton_prev->setHidden(status);
    ui->label_matches->setHidden(status);
}

void strings_viewer::pub_set_global_manager_obj(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
}

void strings_viewer::slot_proc_strings_finished(int state)
{
    ui->textBrowser_strings->moveCursor(QTextCursor::Start);

    // on_pushButton_search_clicked();
    pub_highlight_keyword_hit(highlight_keyword);
    highlight_keyword.clear();

}


void strings_viewer::slot_proc_ready_read()
{
    ui->textBrowser_strings->append(proc_strings->readAll());
    ui->textBrowser_strings->moveCursor(QTextCursor::Start);
}

void strings_viewer::pub_clear_data()
{

    if(global_variable_macOS_appearance_light_mode_bool)
    {
        ui->textBrowser_strings->setTextColor(QColor(Qt::black));
    }
    else {
        ui->textBrowser_strings->setTextColor(QColor(Qt::white));
    }
    ui->textBrowser_strings->clear();
}

void strings_viewer::pub_set_essentials()
{
    create_tags_submenu();
}

void strings_viewer::pub_show_file_data(QString file_path , QString record_no)
{

    current_filepath  = file_path;
    current_record_no = record_no;

    qint64 pref_min_file_size =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Prefrences_Text_View_Maximum_File_Size_Bytes_qint64).toLongLong();

    ui->textBrowser_strings->clear();

    if(QFileInfo(file_path).isDir())
        return;

    if(proc_strings->state() == QProcess::Running)
    {
        proc_strings->terminate();
        while(1)
        {
            QCoreApplication::processEvents();
            if(proc_strings->state() == QProcess::Running)
                continue;

            break;
        }

    }

    ui->textBrowser_strings->clear();

    if(QFileInfo(file_path).size() > pref_min_file_size)
    {
        return;
    }

    QStringList args;
    args << file_path;
    proc_strings->start("strings", args);

}

void strings_viewer::pub_highlight_keyword_hit(QString keyword_str)
{
    keyword_str = keyword_str.trimmed();

    if(keyword_str.isEmpty())
        return;

    list_highlight_cursor.clear();

    QTextDocument *document = ui->textBrowser_strings->document();

    if(document == NULL)
        return;

    document->undo();

    QTextCursor highlightCursor(document);
    QTextCursor cursor(document);

    cursor.beginEditBlock();

    QTextCharFormat plainFormat(highlightCursor.charFormat());
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setForeground(Qt::red);

    while (!highlightCursor.isNull() && !highlightCursor.atEnd())
    {

        highlightCursor = document->find(keyword_str, highlightCursor);


        if (!highlightCursor.isNull()) {
            highlightCursor.movePosition(QTextCursor::NoMove,
                                         QTextCursor::KeepAnchor);
            highlightCursor.mergeCharFormat(colorFormat);

            list_highlight_cursor.append(highlightCursor);

        }
    }

    if(!list_highlight_cursor.isEmpty())
    {
        QTextCursor scrollCursor = list_highlight_cursor.at(0);
        scrollCursor.mergeCharFormat(colorFormat);

        ui->textBrowser_strings->setTextCursor(scrollCursor);

        QPalette pallete = ui->textBrowser_strings->palette();
        pallete.setColor(QPalette::Highlight, QColor(Qt::yellow));
        pallete.setColor(QPalette::HighlightedText, QColor(Qt::red));
        ui->textBrowser_strings->setPalette(pallete);
    }

    cursor.endEditBlock();

}


void strings_viewer::create_tags_submenu()
{
    recon_static_functions::app_debug(" -Starts",Q_FUNC_INFO);

    if(sub_menu_tags == NULL)
        return;

    sub_menu_tags->clear();

    ///add new tag
    act_create_tag = new QAction(QString(MACRO_Generic_Right_Click_Create_Tag), this);
    QIcon icon_create("../icons/tag_icons/add.png");
    act_create_tag->setIcon(icon_create);
    sub_menu_tags->addAction(act_create_tag);

    disconnect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_act_create_tags()));
    connect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_act_create_tags()));


    ///remove tag
    //    act_remove_tag = new QAction(QString(MACRO_Generic_Right_Click_Remove_Tag), this);
    //    QIcon icon_remove("../icons/tag_icons/remove.png");
    //    act_remove_tag->setIcon(icon_remove);
    //    sub_menu_tags->addAction(act_remove_tag);

    //    disconnect(act_remove_tag, SIGNAL(triggered()), this, SLOT(slot_act_remove_tags()));
    //    connect(act_remove_tag, SIGNAL(triggered()), this, SLOT(slot_act_remove_tags()));

    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    sub_menu_tags->addAction(separator_main);

    add_tags_module_obj->pub_set_essentials();

    QString command_str = "SELECT tag_name,selected_colour FROM tags";

    QStringList tags_list = recon_helper_standard_obj->get_tags_and_colours_list_from_db(command_str , add_tags_module_obj->tags_db_path, Q_FUNC_INFO);

    for(int i = 0; i < tags_list.size(); i++)
    {
        QString temp_tag_str = tags_list.at(i);

        if(temp_tag_str.contains("::"))
        {
            if(i == MACRO_DEFAULT_TAGS_LIST_SIZE_int)
            {
                QAction *separator_default_tags = new QAction(this);
                separator_default_tags->setSeparator(true);
                sub_menu_tags->addAction(separator_default_tags);
            }

            QStringList tags_splitted_list = temp_tag_str.split("::", Qt::SkipEmptyParts);

            QString icon_path = "";
            QString tag_str  = tags_splitted_list.at(0);
            QString tag_colour = tags_splitted_list.at(1);

            icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);

            QIcon icon(icon_path);

            QAction *act_already_saved_tags = new QAction(QObject::tr(tag_str.toLocal8Bit().data()), this);
            act_already_saved_tags->setIcon(icon);
            sub_menu_tags->addAction(act_already_saved_tags);
        }
    }

    recon_static_functions::app_debug(" -Ends",Q_FUNC_INFO);
}

void strings_viewer::slot_act_create_tags()
{
    add_tags_module_obj->show();
}

void strings_viewer::slot_right_click_main_menu_triggered(QAction* current_clicked_action)
{
    if(current_clicked_action->text() == QString("Select All"))
    {
        ui->textBrowser_strings->selectAll();
    }
}

void strings_viewer::slot_sub_menu_tags_clicked(QAction* current_clicked_action)
{
    action_submenu_tag_triggered(current_clicked_action->text());
}

void strings_viewer::action_submenu_tag_triggered(QString tag_name)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(tag_name == QString(MACRO_Generic_Right_Click_Remove_Tag) || tag_name == QString(MACRO_Generic_Right_Click_Create_Tag))
        return;


    QString hex_colour = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("SELECT selected_colour FROM tags Where tag_name = ?",QStringList(tag_name) ,0,add_tags_module_obj->tags_db_path,Q_FUNC_INFO);

    update_tags_value(tag_name , hex_colour);

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void strings_viewer::slot_update_tags_submenu(bool status, QString new_tag, QString tag_colour)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        QAction *act_already_saved_tags = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags->setIcon(icon);
        sub_menu_tags->addAction(act_already_saved_tags);

    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void strings_viewer::slot_abouttoshow_tags_in_submenu()
{
    create_tags_submenu();
}

void strings_viewer::slot_add_tags(QString tag_data , QString colour)
{
    update_tags_value(tag_data,colour);
}

void strings_viewer::update_tags_value(QString tag_name,QString hex_colour)
{

    QTextCursor cursor = ui->textBrowser_strings->textCursor();
    QString selected_text = cursor.selectedText();
    if(selected_text.trimmed().isEmpty())
        return ;


    create_table_export_content();

    QString display_file_path = st_globl_recon_file_info_obj.display_file_path;
    QString recon_filefrom = st_globl_recon_file_info_obj.recon_file_from;
    QString recon_file_info_path =  st_globl_recon_file_info_obj.recon_file_info_path;

    QString export_file_path;
    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    if(struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
            || struct_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
    {
        export_file_path = st_globl_recon_file_info_obj.id_path;
    }
    else
        export_file_path = display_file_path;



    QFileInfo info_temp(current_filepath);
    QString file_name = QFileInfo(display_file_path).fileName();
    QString content_file_name_str = file_name;
    QFileInfo file_info(display_file_path);

    QString suffix_str = file_info.suffix();
    if(content_file_name_str.endsWith(suffix_str))
        content_file_name_str.chop(suffix_str.size());

    if(content_file_name_str.endsWith("."))
        content_file_name_str.chop(1);

    QString content_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString).toString();
    content_file_name_str = recon_static_functions::get_available_filename(content_file_name_str, content_file_path, Q_FUNC_INFO);
    content_file_path = content_file_path + content_file_name_str;


    QFile myfile(content_file_path);
    if(!myfile.open(QIODevice::ReadWrite))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + content_file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + myfile.errorString(),Q_FUNC_INFO);
        return;
    }
    myfile.write(selected_text.toLocal8Bit());
    myfile.flush();
    myfile.close();



    QList<struct_global_tagging_string_viewer_record> string_viewer_record_list;
    struct_global_tagging_string_viewer_record s1;
    s1.INT_record = current_record_no;
    s1.plugin_name = QString(MACRO_Plugin_Name_Strings_Viewer);
    s1.tab_name = QString(MACRO_Plugin_Name_Strings_Viewer);
    s1.item0 = file_name;
    s1.item1 = display_file_path;
    s1.item2 = selected_text;
    s1.item3 = content_file_name_str;
    s1.item4 = QString::number(info_temp.size());
    s1.source_count_name = source_count_name;
    s1.hex_color = hex_colour;
    s1.export_file_path = export_file_path;
    s1.recon_filefrom = recon_filefrom;
    s1.recon_file_infopath = recon_file_info_path;
    string_viewer_record_list << s1;


    global_connection_manager_obj->pub_add_data_in_export_content_string_viewer_and_text_viewer_db(string_viewer_record_list ,tag_name, MACRO_Plugin_Name_Strings_Viewer);
    global_connection_manager_obj->pub_add_tag_for_string_viewer_and_text_viewer(string_viewer_record_list , tag_name);

}


void strings_viewer::on_pushButton_search_clicked()
{
    QString searchString = ui->lineEdit_search->text().trimmed();

    if(searchString.isEmpty())
        return;

    list_highlight_cursor.clear();

    QTextDocument *document = ui->textBrowser_strings->document();

    bool found = false;

    document->undo();


    QTextCursor highlightCursor(document);
    QTextCursor cursor(document);

    cursor.beginEditBlock();

    QTextCharFormat plainFormat(highlightCursor.charFormat());
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setForeground(Qt::red);


    while (!highlightCursor.isNull() && !highlightCursor.atEnd())
    {

        highlightCursor = document->find(searchString, highlightCursor);


        if (!highlightCursor.isNull()) {
            found = true;
            highlightCursor.movePosition(QTextCursor::NoMove,
                                         QTextCursor::KeepAnchor);
            highlightCursor.mergeCharFormat(colorFormat);

            list_highlight_cursor.append(highlightCursor);

        }


    }

    if(!list_highlight_cursor.isEmpty())
    {
        QTextCursor scrollCursor = list_highlight_cursor.at(0);
        scrollCursor.mergeCharFormat(colorFormat);

        ui->textBrowser_strings->setTextCursor(scrollCursor);
    }

    cursor.endEditBlock();

}




void strings_viewer::on_lineEdit_search_returnPressed()
{
    on_pushButton_search_clicked();
}

void strings_viewer::on_pushButton_prev_clicked()
{
    current_search_index--;

    if(current_search_index < 0)
        current_search_index = 0;

    if(!list_highlight_cursor.isEmpty())
    {
        if(list_highlight_cursor.size() > current_search_index)
        {
            QTextCursor scrollCursor = list_highlight_cursor.at(current_search_index);
            scrollCursor.beginEditBlock();
            ui->textBrowser_strings->setTextCursor(scrollCursor);
            scrollCursor.endEditBlock();

        }
    }
}

void strings_viewer::on_pushButton_next_clicked()
{
    current_search_index++;

    if(current_search_index > list_highlight_cursor.size())
        current_search_index = list_highlight_cursor.size() -1;

    if(!list_highlight_cursor.isEmpty())
    {
        if(list_highlight_cursor.size() > current_search_index)
        {
            QTextCursor scrollCursor = list_highlight_cursor.at(current_search_index);
            scrollCursor.beginEditBlock();
            ui->textBrowser_strings->setTextCursor(scrollCursor);
            scrollCursor.endEditBlock();
        }
    }
}


void strings_viewer::create_table_export_content()
{

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString).toString() + "string_viewer_content_info.sqlite";
    QFileInfo info(exported_content_info_db);
    if(!info.exists())
    {

        QString command = "CREATE TABLE 'string_content_info'(INT VARCHAR(1000),bookmark VARCHAR(1000), file_name VARCHAR(1000), file_path VARCHAR(1000),"
                          "file_size VARCHAR(1000), string_data VARCHAR(1000), item2 VARCHAR(1000) , item3 VARCHAR(1000), item4 VARCHAR(1000) , item5 VARCHAR(1000),"
                          "recon_tag_value VARCHAR(1000),"
                          "color_hex VARCHAR(1000), content_file_path VARCHAR(1000), content_file_name VARCHAR(1000),export_file_path VARCHAR(1000) ,source_count_name VARCHAR(1000), notes VARCHAR(1000), LINKFILENAME VARCHAR(1000),"
                          "source_file VARCHAR(1000),recon_filefrom VARCHAR(10),recon_file_infopath VARCHAR(1000))";

        if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command,exported_content_info_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("CREATE table string_content_info in string viewer ---FAILED---" + exported_content_info_db,Q_FUNC_INFO);
        }

        QString export_file_cmd = recon_static_functions::get_exportfilelist_command();
        recon_helper_standard_obj->execute_db_command_by_dbpath(export_file_cmd , exported_content_info_db, Q_FUNC_INFO);

    }
}
