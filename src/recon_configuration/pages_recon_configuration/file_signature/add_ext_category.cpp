#include "add_ext_category.h"
#include "ui_add_ext_category.h"

add_ext_category::add_ext_category(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_ext_category)
{
    ui->setupUi(this);

    //  ui->lineEdit_case_no->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->lineEdit_category->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->pushButton_add_category->setIcon(QIcon("../icons/signs/add.png"));

    message_dialog_object = new message_dialog;

}

add_ext_category::~add_ext_category()
{
    delete ui;
}

void add_ext_category::set_list_signature_category(QStringList list)
{
    received_list_signature_category = list;
}

void add_ext_category::clear_lineedit()
{
    ui->lineEdit_category->clear();
}

void add_ext_category::pub_set_destination_db_file(QString db_path)
{
    destination_db_file = db_path;
}

void add_ext_category::on_pushButton_add_category_clicked()
{
    if(ui->lineEdit_category->text().trimmed() == QString(""))
        return;


    QString mtext = ui->lineEdit_category->text().trimmed();
    for(int i = 0; i < received_list_signature_category.count(); i++)
    {
        if(mtext == received_list_signature_category.at(i))
        {
            message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,"Category Already Exist");
            message_dialog_object->show();
            return;
        }
    }

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination_db ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    query.prepare("insert into 'tbl_signature_category' (category, category_status) values(?, ?)");
    query.addBindValue(ui->lineEdit_category->text());
    query.addBindValue("1");
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(query.executedQuery(),Q_FUNC_INFO);

        this->hide();
        return;
    }

    ui->lineEdit_category->clear();
    this->hide();

    emit signal_category_added();
}
