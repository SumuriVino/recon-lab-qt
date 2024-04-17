#include "manage_plist_key.h"
#include "ui_manage_plist_key.h"

manage_plist_key::manage_plist_key(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::manage_plist_key)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
    setWindowModality(Qt::ApplicationModal);

}

manage_plist_key::~manage_plist_key()
{
    delete ui;
}

void manage_plist_key::draw_table_manage_plist_key(QStringList &keys_list)
{
    ///Creating Right Click actions
    all_key_QStringList_pointer = &keys_list;

    menu_manage_keys = new QMenu(this);
    connect(menu_manage_keys, SIGNAL(triggered(QAction*)), this, SLOT(slot_manage_keys_menu_clicked(QAction*)));

    QStringList menuList;
    menuList << QString(MACRO_Generic_Right_Click_Remove);
    create_right_click_actions(menu_manage_keys, menuList);

    QStringList timestamp_list;
    QStringList headers_list;
    headers_list << "Keys" << "Data Conversion";

    QStringList key_list;
    for(int ii = 0; ii < all_key_QStringList_pointer->size(); ii++)
    {
        QString keys_name = all_key_QStringList_pointer->at(ii);
        key_list << keys_name;
        timestamp_list << get_conversion_type_of_the_current_selected_key(keys_name);
    }

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(headers_list);

    QTableWidgetItem *itemcolName;

    int count = 0;

    for(int kk = 0; kk < key_list.size(); kk++)
    {
        itemcolName = new QTableWidgetItem;
        itemcolName->setText(key_list.at(kk));

        if(kk % 200 == 0)
        {
           QCoreApplication::processEvents();
        }

        ui->tableWidget->insertRow(count);
        ui->tableWidget->setRowHeight(count,25);
        ui->tableWidget->setItem(count,enum_keys_column,itemcolName);

        QStringList combo_list;

        //        combo_list << "Select Conversion" << "Unix Timestamp" << "Cocoa Core Data Timestamp" << "WebKit/Chrome Timestamp";
        combo_list << "Select Conversion" << "Unix Timestamp" << "Cocoa Core Data Timestamp" << "WebKit/Chrome Timestamp" << "ASCII" << "Unicode";


        QComboBox* comboBox = new QComboBox;

        QFont font;
        font.setPixelSize(12);
        comboBox->setFont(font);
        comboBox->setParent(ui->tableWidget);

        comboBox->addItems(combo_list);
        comboBox->setMaximumWidth(180);

        ui->tableWidget->setCellWidget(count,enum_data_conversion_column,comboBox);

        connect(comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(slot_current_index_changed_data_conversion_combobox(QString)));

        // No crash point will come.
        QString conversion_type = timestamp_list.at(kk);

        if(conversion_type.isEmpty())
        {
            comboBox->setCurrentIndex(0); //Select Conversion
        }
        else
        {
            comboBox->setCurrentText(conversion_type);
        }

        count++;
    }

    ui->tableWidget->setColumnWidth(enum_keys_column,180);
    ui->tableWidget->setColumnWidth(enum_data_conversion_column,150);
}

void manage_plist_key::slot_current_index_changed_data_conversion_combobox(QString text)
{
    QString data_conversion_combobox_text;
    QString key_at_combobox;

    for(int ii = 0; ii < ui->tableWidget->rowCount(); ii++)
    {
        data_conversion_combobox_text = (dynamic_cast<QComboBox *>(ui->tableWidget->cellWidget((ii) ,enum_data_conversion_column))->currentText());
        key_at_combobox = ui->tableWidget->item(ii, enum_keys_column)->text();

        emit signal_data_conversion_timestamp_type_change(data_conversion_combobox_text, key_at_combobox);
    }
}

QString manage_plist_key::get_conversion_type_of_the_current_selected_key(QString current_key)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QString command_str = "SELECT data_conversion_type FROM '" + plist_db_table_name + "' Where key = '" + current_key + "'";
    QString conversion_timestamp_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(command_str,0,plist_viewer_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

    return conversion_timestamp_str;
}

void manage_plist_key::on_pushButton_clicked()
{
    this->hide();
}

void manage_plist_key::pub_set_plist_table_name(QString db_name)
{
    plist_db_table_name = db_name;
}

void manage_plist_key::pub_set_plist_db_path(QString db_path)
{
    plist_viewer_db_path = db_path;
}

void manage_plist_key::slot_manage_keys_menu_clicked(QAction *action)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    if(action->text() == QString(MACRO_Generic_Right_Click_Remove))
    {
        QModelIndexList selection_model_list = ui->tableWidget->selectionModel()->selectedRows();

        ui->tableWidget->blockSignals(true);
        QStringList selected_row_list;
        QStringList temp_list;
        for(int ii = selection_model_list.size() - 1; ii >= 0; ii--)
        {
            if(ii % 100 == 0)
                QCoreApplication::processEvents();

            QModelIndex model_index = selection_model_list.at(ii);

            int selected_row_no = model_index.row();

            if(selected_row_no < 0 && selected_row_no >= ui->tableWidget->rowCount())
                continue;

            if(ui->tableWidget->isRowHidden(selected_row_no))
                continue;

            selected_row_list << ui->tableWidget->item(selected_row_no, enum_keys_column)->text();

            ui->tableWidget->removeRow(selected_row_no);

            temp_list = *all_key_QStringList_pointer;

            for(int xx = 0; xx < selected_row_list.size(); xx++)
            {
                QString key_name = selected_row_list.at(xx);
                QString selected_key_name;

                for(int yy = 0; yy < temp_list.size(); yy++)
                {
                    selected_key_name = temp_list.at(yy);
                    if(selected_key_name == key_name)
                    {
                        temp_list.removeAt(yy);
                        update_conversion_type_value_in_db_for_removed_key(key_name);
                    }
                }
            }
        }

        temp_list.removeDuplicates();
        all_key_QStringList_pointer->clear();
        all_key_QStringList_pointer->append(temp_list);
    }

    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void manage_plist_key::create_right_click_actions(QMenu *mainMenu, QStringList submenuList)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    for(int ii = 0; ii < submenuList.size(); ii++)
    {
        QAction *act_submenu = new QAction(submenuList.at(ii).toLocal8Bit(), this);

        if(submenuList.at(ii).isEmpty())
            act_submenu->setSeparator(true);

        mainMenu->addAction(act_submenu);
    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void manage_plist_key::contextMenuEvent(QContextMenuEvent *)
{
    if(all_key_QStringList_pointer->size() < 1)
        return;

    QMenu context_menu(this);
    context_menu.addMenu(menu_manage_keys);
    menu_manage_keys->exec(QCursor::pos());
}

void manage_plist_key::update_conversion_type_value_in_db_for_removed_key(QString key_name)
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    QStringList arg_list;
    QString command_update = "UPDATE '" + plist_db_table_name + "' SET data_conversion_type = ? WHERE key = ?";
    arg_list << "" << key_name;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,arg_list,plist_viewer_db_path,Q_FUNC_INFO);
    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}
