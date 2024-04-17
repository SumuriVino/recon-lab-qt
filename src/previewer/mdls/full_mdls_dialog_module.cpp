#include "full_mdls_dialog_module.h"
#include "ui_full_mdls_dialog_module.h"
//#include "global_header.h"


full_mdls_dialog_module::full_mdls_dialog_module(const QString &title, QWidget *parent) :
    QDialog(parent), w_title(title),
    ui(new Ui::full_mdls_dialog_module)
{
    ui->setupUi(this);

    QString set_title = w_title + "  - Full [Apple Metadata]";
    setWindowTitle(QObject::tr(set_title.toLocal8Bit().data()));
}
full_mdls_dialog_module::~full_mdls_dialog_module()
{
    delete ui;
}


void full_mdls_dialog_module::set_db_file_loaction(QString file_location)
{
    db_file_location_recieved = file_location;
}

void full_mdls_dialog_module::set_INT_value(QString int_value)
{
    INT_value_recieved = int_value;
}

void full_mdls_dialog_module::set_category_name(QString category)
{
    category_name_recieved = category;
}

void full_mdls_dialog_module::set_module_name(QString module_naam)
{
    module_name_recieved = module_naam;
}

void full_mdls_dialog_module::set_table_name(QString my_table)
{
    table_name_recieved = my_table;
}

void full_mdls_dialog_module::display_full_metadata(QString received_path)
{

    QString file_path = received_path;

    ui->treeWidget_full_apple_metadata->clear();
    ui->textEdit_filepath->clear();

    ui->textEdit_filepath->setText(QString("<b>") +QObject::tr("Plugin") + "</b>: " + module_name_recieved);
    ui->textEdit_filepath->append(QString("<b>") +QObject::tr("Category") + "</b>: " + category_name_recieved);
    ui->textEdit_filepath->append(QString("<b>") +QObject::tr("Record No.") + "</b>: " + INT_value_recieved);
    QFile file(file_path);
    if(!file.exists())
    {
        ui->textEdit_filepath->append(QString("<b>") + QObject::tr("File Path") + "</b>: "+ file_path + QString("<font color=red>")  +QObject::tr(" [File does not exist]") + QString("</font>"));
        return;
    }
    else
        ui->textEdit_filepath->append(QString("<b>") +QObject::tr("File Path") + "</b>: " + file_path);

    QStringList metadata_list;
    // metadata_list = global_recon_helper_singular_class_obj->get_apple_metadata_from_api(file_path);
    //    QStringList ext_attribute_list = global_recon_helper_singular_class_obj->get_apple_metadata_from_api(file_path);
    //    if(ext_attribute_list.size() <= 0)
    //    {
    //        //This check is specially for mojave and catalina sources and for Recon Logical Images too.
    //        QString disp_filepath = st_globl_recon_file_info_obj.display_file_path;
    //        ext_attribute_list = recon_helper_standard_obj->get_apple_metadata_from_result_apple_metadata_db(disp_filepath,struct_source_info.source_count_name);
    //    }
    //    if(ext_attribute_list.size() == 0)

    {
        recon_static_functions::app_debug(" metadata_list.size() == 0 -----FAILED---",Q_FUNC_INFO);
        return;
    }

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(db_file_location_recieved);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" db open -----FAILED--- " + db_file_location_recieved,Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    QString command,data;
    command = QString("select bookmarked_extended_attributes from " + table_name_recieved + " where INT = ") + INT_value_recieved;

    if(!query.exec(command))
    {
        recon_static_functions::app_debug(" query.exec -----FAILED--- path " + db_file_location_recieved,Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(command,Q_FUNC_INFO);
        destination_db.close();
        return;
    }
    if(query.next())
    {
        data = query.value(0).toString();
    }

    QFont*	font	= new QFont("Helvetica",15,10,true);

    for(int i = 0 ; i < metadata_list.size() ; i++)
    {
        QString tmp = metadata_list.at(i);
        QStringList single_attribute_list = tmp.split("::==::", Qt::SkipEmptyParts);

        QString field,value,desc;
        field.clear();value.clear();desc.clear();

        if(single_attribute_list.size() > 0)
            field = single_attribute_list.at(0);

        if(single_attribute_list.size() > 1)
            value = single_attribute_list.at(1);

        if(single_attribute_list.size() > 2)
            desc = single_attribute_list.at(2);

        QString tmp_to_check = field + " : " + value + "\n";

        QTreeWidgetItem *parent_item = new QTreeWidgetItem;
        if(data.contains(tmp_to_check))
            parent_item->setCheckState(enum_full_mdls_dialog_module_META_BOOKMARK,Qt::Checked);
        else
            parent_item->setCheckState(enum_full_mdls_dialog_module_META_BOOKMARK,Qt::Unchecked);

        parent_item->setText(enum_full_mdls_dialog_module_META_FIELD, field);

        value = value.trimmed();
        if(value.contains("\n"))
        {
            QStringList mlist = value.split("\n", Qt::SkipEmptyParts);

            int ccount = 0;
            while(ccount < mlist.size())
            {
                QString val = mlist.at(ccount);

                if(ccount == 0)
                {
                    parent_item->setText(enum_full_mdls_dialog_module_META_VALUE, val);
                }
                else
                {
                    QTreeWidgetItem *item_cchils = new QTreeWidgetItem;
                    item_cchils->setText(enum_full_mdls_dialog_module_META_VALUE, val);
                    item_cchils->setToolTip(enum_full_mdls_dialog_module_META_VALUE, desc);
                    parent_item->addChild(item_cchils);
                }
                ccount++;
            }
        }
        else
        {
            parent_item->setText(enum_full_mdls_dialog_module_META_VALUE, value);
        }

        QToolTip::setFont(*font);

        parent_item->setToolTip(enum_full_mdls_dialog_module_META_BOOKMARK, desc);
        parent_item->setToolTip(enum_full_mdls_dialog_module_META_FIELD, desc);
        parent_item->setToolTip(enum_full_mdls_dialog_module_META_VALUE, desc);

        ui->treeWidget_full_apple_metadata->addTopLevelItem(parent_item);

        ui->treeWidget_full_apple_metadata->setColumnWidth(0,53);
        ui->treeWidget_full_apple_metadata->setColumnWidth(1,208);
    }

    destination_db.close();

}

void full_mdls_dialog_module::on_treeWidget_full_apple_metadata_itemClicked(QTreeWidgetItem *item, int column)
{

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(db_file_location_recieved);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" db open -----FAILED--- path " + db_file_location_recieved,Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    QString command,data;
    command = QString("select bookmarked_extended_attributes from " + table_name_recieved + " where INT = ") + INT_value_recieved;
    if(!query.exec(command))
    {
        recon_static_functions::app_debug(" query -----FAILED--- path " + db_file_location_recieved,Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(command,Q_FUNC_INFO);
        destination_db.close();
        return;

    }
    if(query.next())
    {
        data = query.value(0).toString();
    }
    query.clear();

    if(column == enum_full_mdls_dialog_module_META_BOOKMARK)
    {
        if(item->checkState(0))
        {
            QString cmd;
            QString field = item->text(enum_full_mdls_dialog_module_META_FIELD);
            QString value = item->text(enum_full_mdls_dialog_module_META_VALUE);
            data += field + " : " + value + "\n";

            qint64 child_count = item->childCount();
            for(int mm = 0; mm < child_count; mm++)
            {
                data += item->child(mm)->text(enum_full_mdls_dialog_module_META_VALUE) + "\n";
            }

            cmd = QString("update " + table_name_recieved + " set bookmarked_extended_attributes = ") +  "\"" + data + "\"" + " where INT = " + INT_value_recieved;
            query.clear();
            if(!query.exec(cmd))
            {
                destination_db.close();
                return;
            }
            cmd.clear();query.clear();
        }
        else
        {
            QString cmd;
            QString field = item->text(enum_full_mdls_dialog_module_META_FIELD);
            QString value = item->text(enum_full_mdls_dialog_module_META_VALUE);

            QString tmp = field + " : " + value  + "\n";

            qint64 child_count = item->childCount();
            for(int mm = 0; mm < child_count; mm++)
            {
                tmp += item->child(mm)->text(enum_full_mdls_dialog_module_META_VALUE) + "\n";
            }

            data = data.remove(tmp);

            cmd = QString("update " + table_name_recieved + " set bookmarked_extended_attributes = ") +  "\"" + data + "\"" + " where INT = " + INT_value_recieved;
            query.clear();
            if(!query.exec(cmd))
            {
                recon_static_functions::app_debug(" query 1111 -----FAILED--- path ",Q_FUNC_INFO);
                recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug(cmd,Q_FUNC_INFO);
                destination_db.close();
                return;
            } // if(!query.exec(cmd))

            cmd.clear();query.clear();
        }   //else close

    } //if(column == 0)

    destination_db.close();


}
