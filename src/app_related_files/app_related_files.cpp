#include "app_related_files.h"
#include "ui_app_related_files.h"

app_related_files::app_related_files(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::app_related_files)
{
    ui->setupUi(this);
    ui->tableWidget_related_files->setColumnWidth(enum_table_column_record_no,100);
    ui->tableWidget_related_files->setColumnWidth(enum_table_column_filepath,500);

    ui->tableWidget_related_files->hideColumn(enum_table_column_INT);
    ui->tableWidget_related_files->hideColumn(enum_table_column_record_no);

    ui->tableWidget_related_files->horizontalHeader()->setStretchLastSection(true);

    ui->tableWidget_related_files->setAlternatingRowColors(true);

    m_menu = new QMenu;
    action_goto_record = new QAction;
    connect(action_goto_record, SIGNAL(triggered(bool)),this, SLOT(slot_action_goto_record_triggered(bool)));
    action_goto_record->setText("Go to Record");

    m_menu->addAction(action_goto_record);
    connect(ui->tableWidget_related_files, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(slot_action_goto_record_double_clicked(QTableWidgetItem*)));

}

app_related_files::~app_related_files()
{
    delete ui;
}

void app_related_files::contextMenuEvent(QContextMenuEvent *event)
{
    if(event == NULL)
        return;

    QModelIndexList selection_model_list = ui->tableWidget_related_files->selectionModel()->selectedRows();

    if(selection_model_list.size() < 1)
        return;

    m_menu->exec(QCursor::pos());


}

bool app_related_files::pub_set_data_in_table(QString db_path, QString searched_app_identifier,QString app_name)
{
    //searched_app _identifier = Chrome or Google Chrome.app
    //app_name = Google Chrome.app (Right_clicked for search)

    setWindowTitle(app_name + ": Related Files");

    fs_db_path = db_path;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
    source_db.setDatabaseName(db_path);

    if(!source_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
        return false;
    }

    QStringList app_name_list;

    QString cmd = "SELECT filepath, is_dir,int from files where ";

    QString tmp_app_name = searched_app_identifier;
    if(tmp_app_name.endsWith(".app"))
        tmp_app_name.chop(QString(".app").size());


    //This is to set more filter to get more no of files for the searched app, for eg. .
    //Google Chrome -> Google and Chrome two values in a list
    //Google-Chrome -> Google and Chrome two values in a list
    if(tmp_app_name.contains(" "))
    {
        app_name_list = tmp_app_name.split(" ");
    }
    else if(tmp_app_name.contains("-"))
    {
        app_name_list = tmp_app_name.split("-");
    }
    else
        app_name_list = QStringList(tmp_app_name);

    //The query will search the filepath which contain the app name that have '/' in just before its name or '.' just after its name
    //eg /Google or Google. and /Chrome or Chrome.
    for(int ii = 0; ii < app_name_list.size(); ii++)
    {
        cmd.append("(filepath like '%/" + app_name_list.at(ii) + "%' OR filepath like '%" + app_name_list.at(ii) + ".%') AND ");
    }

    if(cmd.endsWith(" AND "))
    {
        cmd.chop(QString(" AND ").size());
    }

    //The query will Search the filename with the exact searched appname
    //eg filename like Google Chrome.app
    cmd.append(" OR filename like '" + app_name +"'");

    QSqlQuery query_select(source_db);
    source_db.transaction();

    query_select.prepare(cmd);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(" select query ----FAILED---- " + query_select.executedQuery() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_select.lastError().text() , Q_FUNC_INFO);
        source_db.commit();
        source_db.close();
        return false;
    }

    QString dir_path = "-1";

    int row_cnt = 0;
    ui->tableWidget_related_files->setRowCount(row_cnt);

    while(query_select.next())
    {
        QString file_path = query_select.value("filepath").toString();

        if(file_path.startsWith(dir_path) )
        {
            continue;
        }

        if(query_select.value("is_dir").toString() == "1")
        {
            dir_path = file_path;
        }

        //The Given path are the whitelisted path where application files/folders are found
        if(file_path.startsWith("/var")
                || (file_path.startsWith("/Library/Logs"))
                || (file_path.startsWith("/Library/Cookies"))
                || (file_path.startsWith("/Applications") && file_path.endsWith(app_name))
                || (file_path.startsWith("/System") && file_path.contains("/Library/Logs"))
                || (file_path.startsWith("/System") && file_path.contains("/Library/Cookies"))
                || (file_path.startsWith("/Users") && file_path.contains("/Library"))
                || (file_path.startsWith("/Users/Shared"))
                )
        {
            ui->tableWidget_related_files->insertRow(row_cnt);
            QString rec_no = query_select.value("int").toString();
            QTableWidgetItem *item_rec = new QTableWidgetItem;
            item_rec->setText(rec_no);
            ui->tableWidget_related_files->setItem(row_cnt,enum_table_column_record_no,item_rec);

            QTableWidgetItem *item_filepath = new QTableWidgetItem;
            item_filepath->setText(file_path);
            ui->tableWidget_related_files->setItem(row_cnt,enum_table_column_filepath,item_filepath);
            row_cnt++;

        }
    }

    source_db.commit();
    source_db.close();

    return true;
}

void app_related_files::slot_action_goto_record_triggered(bool status)
{

    int selected_row_no = ui->tableWidget_related_files->currentRow();

    if(selected_row_no < 0 && selected_row_no >= ui->tableWidget_related_files->rowCount())
        return;

    if(ui->tableWidget_related_files->isRowHidden(selected_row_no))
        return;


    QString filepath = ui->tableWidget_related_files->item(selected_row_no,enum_table_column_filepath)->text();

    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_cnt_name);

    emit signal_related_files_tablewidget_goto_record(filepath,"" , source_cnt_name);


}

void app_related_files::slot_action_goto_record_double_clicked(QTableWidgetItem *item)
{
    slot_action_goto_record_triggered(true);
}

bool app_related_files::pub_set_source_count_name(QString src_cnt_name)
{
    source_cnt_name = src_cnt_name;
}
