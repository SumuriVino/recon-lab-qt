#include "screenshots.h"
#include "ui_screenshots.h"

screenshots::screenshots(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::screenshots)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    process_generic = new QProcess(this);
    connect(process_generic,SIGNAL(finished(int)),this,SLOT(slot_generic_process_finished(int)));

    setWindowTitle("Screenshots");
    // m_clipboard = new QClipboard(this);

    image_previewe_obj = new image_preview(this);


    set_gui();
}

screenshots::~screenshots()
{
    delete ui;
}

void screenshots::closeEvent(QCloseEvent *)
{
    emit signal_screenshots_canceled();
}

void screenshots::clear_fields()
{
    ui->lineEdit_file_name->clear();
    ui->lineEdit_add_note->clear();
    m_clipboard->clear();
}

bool screenshots::pub_screenshot_show()
{
    if(!m_clipboard->image(QClipboard::Clipboard).isNull())
        return true;
    else
        return false;
}

void screenshots::set_screenshot()
{
    QString file_name = QString("Screenshot-") + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss"));
    ui->lineEdit_file_name->setPlaceholderText(file_name);

    if(images_dir.trimmed().isEmpty())
    {
        images_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_Images_Directory_In_Result_QString).toString();
    }

    ui->lineEdit_file_path->setPlaceholderText(images_dir);

    QStringList args;

    // args << "-i" << file_path;
    args << "-ic";

    run_command_generic("screencapture",args);

    m_clipboard = QApplication::clipboard();

    image_previewe_obj->set_image_by_pixmap(m_clipboard->pixmap(QClipboard::Clipboard));

}

QString screenshots::run_command_generic(QString command, QStringList arguments)
{
    bool_process_generic = false;
    process_generic->start(command,arguments);

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_process_generic)
            break;
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    return QString::fromLocal8Bit(process_generic->readAll().trimmed());
}

void screenshots::set_gui()
{
    image_previewe_obj->setMaximumWidth(9999);
    image_previewe_obj->setMaximumHeight(9999);

    QWidget *m_widget = new QWidget(this);

    QHBoxLayout *h_layout = new QHBoxLayout(m_widget);
    h_layout->addWidget(image_previewe_obj);
    h_layout->setContentsMargins(0,0,7,5);

    m_widget->setLayout(h_layout);

    ui->stackedWidget_screenshot->addWidget(m_widget);
    ui->stackedWidget_screenshot->setCurrentIndex(2);


    ui->lineEdit_file_name->setAttribute(Qt::WA_MacShowFocusRect,0);
    ui->lineEdit_file_path->setAttribute(Qt::WA_MacShowFocusRect,0);

}

void screenshots::slot_generic_process_finished(int)
{
    bool_process_generic = true;
}

void screenshots::on_pushButton_dir_path_clicked()
{
    QString dir_path =  QFileDialog::getExistingDirectory(this, "Select Dir", images_dir, QFileDialog::ShowDirsOnly);

    if(!dir_path.trimmed().isEmpty())
    {
        images_dir = dir_path;
        ui->lineEdit_file_path->setText(images_dir);
    }
}

void screenshots::on_pushButton_done_clicked()
{
    QString file_name = ui->lineEdit_file_name->text();
    QString note = ui->lineEdit_add_note->text();


    if(file_name.trimmed().isEmpty())
    {
        file_name = ui->lineEdit_file_name->placeholderText();
    }

    file_name.append(".png");

    file_name = recon_static_functions::get_available_filename(file_name,images_dir,Q_FUNC_INFO);

    QString file_path;
    if(images_dir.endsWith("/"))
        file_path = images_dir + file_name;
    else
        file_path = images_dir + QString("/") + file_name;

    m_clipboard->image(QClipboard::Clipboard).save(file_path);

    emit signal_screenshots_work_complete(file_path, note.trimmed(), "");



}

void screenshots::on_pushButton_cancel_clicked()
{
    emit signal_screenshots_canceled();
}
