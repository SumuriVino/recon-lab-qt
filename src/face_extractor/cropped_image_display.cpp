#include "cropped_image_display.h"
#include "ui_cropped_image_display.h"

cropped_image_display::cropped_image_display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cropped_image_display)
{
    ui->setupUi(this);

    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qGuiApp->primaryScreen()->availableGeometry()));

    setWindowModality(Qt::ApplicationModal);
    ui->lineEdit_save_result_name->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->pushButton_next->setHidden(true);
    ui->pushButton_prev->setHidden(true);

}

cropped_image_display::~cropped_image_display()
{
    delete ui;
}

void cropped_image_display::prepare_image_display(QString path)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    map_found_face_info.clear();

    if(ui->stackedWidget->count() > 0)
    {
        for(int ii = ui->stackedWidget->count(); ii >= 0; ii--)
        {
            QWidget* widget = ui->stackedWidget->widget(ii);
            ui->stackedWidget->removeWidget(widget);
            widget->deleteLater();
        }
    }

    QDir dir(path);

    if(!dir.exists())
        return;

    QDirIterator itr(path, QDir::Files | QDir::NoDotAndDotDot);

    int count = 0;

    while(itr.hasNext())
    {
        QString f_path = itr.next();

        if(QFileInfo(f_path).isFile())
        {
            QLabel *label = new QLabel();

            QPixmap pic(f_path);

            label->setPixmap(pic);

            QHBoxLayout *hbox = new QHBoxLayout;

            QSpacerItem *item = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);

            hbox->addSpacerItem(item);

            hbox->addWidget(label);

            QSpacerItem *item_2 = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);

            hbox->addSpacerItem(item_2);

            QWidget *wid = new QWidget();
            wid->setLayout(hbox);

            ui->stackedWidget->insertWidget(count, wid);

            map_found_face_info.insert(count, f_path);

            count++;
        }
    }
    ui->stackedWidget->setCurrentIndex(0);

    if(ui->stackedWidget->count() > 1)
    {
        ui->pushButton_next->setHidden(false);
        ui->pushButton_prev->setHidden(false);

    }
    else
    {
        ui->pushButton_next->setHidden(true);
        ui->pushButton_prev->setHidden(true);

    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}

void cropped_image_display::pub_clear_fields()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    ui->lineEdit_save_result_name->clear();

    int count = ui->stackedWidget->count();

    for(int ii = count-1; ii >= 0; ii--)
    {
        QWidget *wid = ui->stackedWidget->widget(ii);

        ui->stackedWidget->removeWidget(wid);
        delete wid;
    }

    ui->pushButton_next->setHidden(true);
    ui->pushButton_prev->setHidden(true);

    set_linedit_search_label_placeholder_text();
    add_dummy_face_img();


    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void cropped_image_display::on_pushButton_prev_clicked()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    int index = ui->stackedWidget->currentIndex();

    if(index <= 0)
        return;

    ui->stackedWidget->setCurrentIndex(index - 1);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void cropped_image_display::on_pushButton_next_clicked()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    int index = ui->stackedWidget->currentIndex();

    if(index >= ui->stackedWidget->count() - 1)
        return;

    ui->stackedWidget->setCurrentIndex(index + 1);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void cropped_image_display::on_pushButton_start_clicked()
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString val_file_path = map_found_face_info.value(ui->stackedWidget->currentIndex());

    QString search_label_name = ui->lineEdit_save_result_name->text().trimmed();

    if(search_label_name.trimmed().isEmpty())
        search_label_name = ui->lineEdit_save_result_name->placeholderText();

    emit signal_start_search_clicked(val_file_path, search_label_name);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void cropped_image_display::set_linedit_search_label_placeholder_text()
{
    QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Face_Search_In_Result_QString).toString() + "index_face_search.sqlite";
    recon_helper_standard *recon_helper_standard_obj = new recon_helper_standard(this);

    QString command = "SELECT COUNT(*) FROM face_search_index";
    QString value = recon_helper_standard_obj->get_string_from_db_by_dbpath(command, 0, index_db_path, Q_FUNC_INFO);

    ui->lineEdit_save_result_name->clear();
//    ui->lineEdit_save_result_name->setPlaceholderText("Search_Result_" + QString::number(value.toInt() + 1));
    ui->lineEdit_save_result_name->setPlaceholderText("Search_Result");
}

void cropped_image_display::add_dummy_face_img()
{
    QString f_path = "../icons/person2.png";

    if(QFileInfo(f_path).isFile())
    {
        QLabel *label = new QLabel();

        QPixmap pic(f_path);

        label->setPixmap(pic);

        QHBoxLayout *hbox = new QHBoxLayout;

        QSpacerItem *item = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);

        hbox->addSpacerItem(item);

        hbox->addWidget(label);

        QSpacerItem *item_2 = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);

        hbox->addSpacerItem(item_2);

        QWidget *wid = new QWidget();
        wid->setLayout(hbox);

        ui->stackedWidget->insertWidget(0, wid);
    }
}

