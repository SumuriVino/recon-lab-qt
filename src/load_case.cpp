#include "load_case.h"
#include "ui_load_case.h"

load_case::load_case(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::load_case)
{
    ui->setupUi(this);

    message_dialog_object = new message_dialog(this);
}

load_case::~load_case()
{
    delete ui;
}

void load_case::slot_load_case_button_clicked()
{


    QString home_dir = getenv("HOME");
    QString path = QFileDialog::getExistingDirectory(this, "Load Case", home_dir, QFileDialog::ShowDirsOnly);

    if(path.trimmed() == "")
        return;


    QFile file(path + "/Result.recon/caseresult");
    if(!file.exists())
    {
        recon_static_functions::app_debug(" : ---FAILED--- file not exist " + path + "/Result.recon/caseresult",Q_FUNC_INFO);
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Selected directory does not contain RECON case result.");
        message_dialog_object->show();

        return;
    }

    QFileInfo info(path);
    QString tmp_path = path.trimmed();
    tmp_path = tmp_path.toLower();
    QString dir_name = "Result.recon";
    dir_name = dir_name.toLower();
    QString realpath;
    if(tmp_path.endsWith(dir_name))
    {
        realpath = (info.absolutePath().trimmed());
    }
    else
    {
        realpath = (path.trimmed());
    }

    emit signal_loadcase_launched(realpath);

}

void load_case::slot_load_case_from_launcher_clicked(QString path)
{
    //    if(path.trimmed() == "")
    //        return;

    //    QFile file(path + "/Result.recon/caseresult");
    //    if(!file.exists())
    //    {
    //        recon_static_functions::app_debug(" : ---FAILED--- file not exist " + path + "/Result.recon/caseresult",Q_FUNC_INFO);
    //    message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Selected directory does not contain RECON case result.");
    //    message_dialog_object->show();

    //        return;
    //    }


    QFileInfo info(path);
    QString tmp_path = path.trimmed();
    tmp_path = tmp_path.toLower();
    QString dir_name = "Result.recon";
    dir_name = dir_name.toLower();
    QString realpath;
    if(tmp_path.endsWith(dir_name))
    {
        realpath = (info.absolutePath().trimmed());
    }
    else
    {
        realpath = (path.trimmed());
    }


    emit signal_loadcase_launched(realpath);

}
