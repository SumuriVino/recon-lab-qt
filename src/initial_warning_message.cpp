#include "initial_warning_message.h"
#include "ui_initial_warning_message.h"

initial_warning_message::initial_warning_message(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::initial_warning_message)
{
    ui->setupUi(this);
    setWindowTitle(recon_static_functions::get_app_name());
}

initial_warning_message::~initial_warning_message()
{
    delete ui;
}

void initial_warning_message::pub_show()
{

#ifdef MACRO_RECON_STATIC_APP_RUN_BY_FORENSO
    return;
#endif

    QString filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Parent_QString).toString() + "empty_file";

    QFile file(filepath);

    if(file.exists())
        hide();
    else
        show();
}

void initial_warning_message::on_pushButton_ok_clicked()
{
    if(ui->checkBox_dont_show->checkState() == Qt::Checked)
    {
        QString filepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Parent_QString).toString() + "empty_file";

        QProcess proc;
        QStringList args;
        args << filepath;

        proc.start("touch", args);

        proc.waitForFinished(3000);
    }

    hide();

}
