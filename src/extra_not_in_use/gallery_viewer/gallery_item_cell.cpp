#include "gallery_item_cell.h"
#include "ui_gallery_item_cell.h"

gallery_item_cell::gallery_item_cell(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gallery_item_cell)
{
    ui->setupUi(this);
    ui->label_img->setAlignment(Qt::AlignCenter);
    installEventFilter(this);
    setFocusPolicy(Qt::TabFocus);

}

gallery_item_cell::~gallery_item_cell()
{
    delete ui;
}

void gallery_item_cell::pub_set_file_info(struct_global_gallery_view_file_info f1)
{

    file_info = f1;

    QString label_text_str;
    int char_in_one_line = 15;
    int tmp_count = 0;
    for(int count = 0; count < file_info.file_name.size(); count++)
    {
        label_text_str.append(file_info.file_name.at(count));
        if(tmp_count == char_in_one_line)
        {
            label_text_str.append("\n");
            tmp_count = 0;
        }
        tmp_count++;
    }
    ui->label_file_name->setText(label_text_str);


    if(file_info.bookmark == "1")
        ui->checkBox_bookmark->setCheckState(Qt::Checked);

}

bool gallery_item_cell::eventFilter(QObject *obj, QEvent *event)
{

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        custom_keypress_event(keyEvent);

        return true;
    }

    return false;
}

void gallery_item_cell::custom_keypress_event(QKeyEvent *keyEvent)
{

    switch(keyEvent->key())
    {

    case Qt::Key_Space:
    {
        emit signal_spacebar_pressed(file_info);
        break;
    }
    case Qt::Key_B:
    {
        QString bookmark_value;
        if(ui->checkBox_bookmark->checkState() == Qt::Checked)
        {
            ui->checkBox_bookmark->setCheckState(Qt::Unchecked);
            bookmark_value = "0";
        }
        else if(ui->checkBox_bookmark->checkState() == Qt::Unchecked)
        {
            ui->checkBox_bookmark->setCheckState(Qt::Checked);
            bookmark_value = "1";
        }
        else
        {
            break;
        }
        file_info.bookmark = bookmark_value;
        emit signal_bookmark_value_changed(file_info);
        break;
    }
    case Qt::Key_E:
    {
        if(keyEvent->modifiers() & Qt::ControlModifier)
        {
            emit signal_control_e_key_pressed(file_info);
        }
    }
    case Qt::Key_M:
    {
        if(keyEvent->modifiers() & Qt::ControlModifier)
        {
            emit signal_control_m_key_pressed(file_info);
        }

    }
    case Qt::Key_N:
    {

        if(keyEvent->modifiers() & Qt::ControlModifier)
        {
            emit signal_control_n_key_pressed(file_info);
        }

    }
    default:
        keyPressEvent(keyEvent);

    }

}

void gallery_item_cell::pub_load_file_data()
{
    is_file_data_loaded = true;


    if(QFileInfo(file_info.full_file_path).size() < 100 * 1024) // Less than 100 KB file
    {

        QImage img(file_info.full_file_path);
        img = img.scaled(140, 140, Qt::KeepAspectRatio, Qt::FastTransformation);
        ui->label_img->setPixmap(QPixmap::fromImage(img));
        return;
    }


    QString cache_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Caches_In_Result_QString).toString();
    QString cache_file = cache_path  + file_info.source_count_name + "_" + file_info.INT + ".png";


    QFile mfile(cache_file);

    if(mfile.exists())
    {
        QImage img(cache_file);
        img = img.scaled(140, 140, Qt::KeepAspectRatio, Qt::FastTransformation);
        ui->label_img->setPixmap(QPixmap::fromImage(img));
        return;
    }

    QImage img(file_info.full_file_path);
    img = img.scaled(140, 140, Qt::KeepAspectRatio, Qt::FastTransformation);

    img.save(cache_file);

    ui->label_img->setPixmap(QPixmap::fromImage(img));
}

bool gallery_item_cell::pub_is_2file_data_loaded()
{
    return is_file_data_loaded;
}

QString gallery_item_cell::pub_file_path()
{
    return file_info.file_path;
}

void gallery_item_cell::pub_set_bookmark(bool checked_status)
{
    if(checked_status)
    {
        ui->checkBox_bookmark->setCheckState(Qt::Checked);
    }
    else
    {
        ui->checkBox_bookmark->setCheckState(Qt::Unchecked);
    }

}

struct_global_gallery_view_file_info gallery_item_cell::pub_file_info()
{
    return file_info;
}

void gallery_item_cell::on_checkBox_bookmark_clicked()
{
    QString bookmark_value;

    if(ui->checkBox_bookmark->checkState() == Qt::Checked)
        bookmark_value = "1";
    else
        bookmark_value = "0";

    file_info.bookmark = bookmark_value;
    emit signal_bookmark_value_changed(file_info);
}
