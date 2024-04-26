#include "source_widgets.h"
#include "ui_source_widgets.h"

source_widgets::source_widgets(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::source_widgets)
{
    ui->setupUi(this);
    received_root_type = "";
    password_string = "";
    username_string = "";

    ui->pushButton_repoint->setIcon(QIcon("../icons/signs/browse.png"));
    ui->pushButton_repoint_2->setIcon(QIcon("../icons/signs/browse.png"));

    ui->pushButton_repoint->setHidden(true);
    ui->pushButton_repoint_2->setHidden(true);

    bool_is_load_case = false;


    source_evidencer_obj = new source_evidencer();

    file_dialog_obj.setOption(QFileDialog::DontUseNativeDialog);
    QList<QUrl> drives;
    drives << QUrl::fromLocalFile(QDir("/Volumes/").absolutePath());
    drives << QUrl::fromLocalFile(QDir("/").absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Downloads")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Desktop")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Documents")).absolutePath());
    file_dialog_obj.setSidebarUrls(drives);
    file_dialog_obj.setDirectory(getenv("HOME"));

}

source_widgets::~source_widgets()
{
    delete ui;
}

void source_widgets::pub_set_widget_id(int id) //set received id/ source id
{
    received_root_count_name.clear();

    received_id = id;
}

void source_widgets::pub_set_root_type(QString name) // set root type/root name of Source type
{
    received_root_type = name;
}

QString source_widgets::pub_get_root_type() //get root type
{
    return received_root_type;
}

void source_widgets::pub_set_source_type_internal(QString source_type_internal) //what type of source type
{
    received_source_type_internal = source_type_internal;
}

QString source_widgets::pub_get_source_type_internal() // get type of source
{
    return received_source_type_internal;
}

void source_widgets::pub_set_source_type_display(QString source_type_display) // source type, which display to user
{
    received_source_type_display = source_type_display;
}

QString source_widgets::pub_get_source_type_display() // get source type which display to user
{
    return received_source_type_display;
}
QString source_widgets::pub_get_root_count_name() // root count name eg root1, root2
{
    return received_root_count_name;
}

void source_widgets::pub_set_is_load_case(bool status) //status of load case
{
    bool_is_load_case = status;

    if(status)
    {
        ui->pushButton_close->setHidden(status);
        ui->pushButton_close_2->setHidden(status);

    }

}


void source_widgets::pub_set_media_path(QString path) // filepath of source which we select display to the exaimer initially
{
    ui->label_media_type->setText(global_lang_tr_obj.get_translated_string(received_root_type));

    if(received_root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage)
    {
        QStringList list = path.split(MACRO_RECON_Splitter_6_comma, Qt::SkipEmptyParts);

        if(list.size() < 2)
            return;

        ui->lineEdit_media_path->setText(list.at(0).trimmed());
        ui->lineEdit_media_path_2->setText(list.at(1).trimmed());
        this->setMinimumHeight(75);

        QPalette *palette = new QPalette();
        if(!QFileInfo(list.at(0).trimmed()).exists())
        {
            palette->setColor(QPalette::Text,Qt::red);
            ui->lineEdit_media_path->setPalette(*palette);

            if(bool_is_load_case)
                ui->pushButton_repoint->setHidden(false);

        }

        if(!QFileInfo(list.at(1).trimmed()).exists())
        {
            palette->setColor(QPalette::Text,Qt::red);
            ui->lineEdit_media_path_2->setPalette(*palette);

            if(bool_is_load_case)
                ui->pushButton_repoint_2->setHidden(false);

        }
    }
    else
    {
        ui->lineEdit_media_path->setText(path);
        ui->lineEdit_media_path_2->hide();
        ui->pushButton_close_2->hide();
        this->setMaximumHeight(52);

        ui->pushButton_repoint_2->hide();

        if(bool_is_load_case)
            ui->pushButton_repoint->setHidden(false);

        QPalette *palette = new QPalette();

        if(!QFileInfo(path.trimmed()).exists())
        {
            palette->setColor(QPalette::Text,Qt::red);
            ui->lineEdit_media_path->setPalette(*palette);
        }

    }

}

void source_widgets::pub_ask_for_eveidence_no(QString source_path) // ask evidence number after adding source
{
    if(received_root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_ForensicsImage
            || received_root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FileVaultImage
            || received_root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONFSBlockImage
            || received_root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_RECONLogicalImage)
    {
        source_evidencer_obj->set_source_message(source_path);
        source_evidencer_obj->pub_set_evidence_number(source_path);
    }
    else if(received_root_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage)
    {
        QString tmp_source_path = source_path;
        if(source_path.contains(MACRO_RECON_Splitter_1_multiple))
        {
            QStringList source_path_for_fusion_drive_list = tmp_source_path.split(MACRO_RECON_Splitter_1_multiple);
            //Extract only SSD path here for parse evidence number from source image's case detail text file
            if(!source_path_for_fusion_drive_list.at(0).isEmpty())
            {
                tmp_source_path = source_path_for_fusion_drive_list.at(0);
            }
            //this is for display file paths with comma seperator
            source_path = source_path.replace(MACRO_RECON_Splitter_1_multiple,MACRO_RECON_Splitter_6_comma);
        }
        source_evidencer_obj->set_source_message(source_path);
        source_evidencer_obj->pub_set_evidence_number(tmp_source_path);
    }
    else
    {
        source_evidencer_obj->set_source_message(source_path);
    }

    source_evidencer_obj->exec();
}

int source_widgets::pub_get_id() // get receive id
{
    return received_id;
}

int source_widgets::pub_get_window_height() //get minimum height of window
{
    return this->minimumHeight();
}

void source_widgets::pub_set_password_string(QString passwd) // set password
{
    password_string = passwd;
}

QString source_widgets::pub_get_password_string() //get password
{
    return password_string;
}

void source_widgets::pub_set_username_string(QString usr_nm) // set username
{
    username_string = usr_nm;
}
void source_widgets::pub_set_evidence_type(QString evidence_type) // evidence type
{
    recevied_evidence_type_string = evidence_type;
}

QString source_widgets::pub_get_evidence_type() // evidence type
{
    return recevied_evidence_type_string;
}
void source_widgets::pub_set_root_count_name(QString rt_count_name) //root count name
{
    received_root_count_name = rt_count_name;
}

QString source_widgets::pub_get_user_name_string() //username
{
    return username_string;
}

QString source_widgets::pub_get_path_1_string() //source path 1
{
    return ui->lineEdit_media_path->text().trimmed();
}

QString source_widgets::pub_get_path_2_string() // source path 2 in case of fusion drive
{
    return ui->lineEdit_media_path_2->text().trimmed();
}

struct_global_source_evidencer source_widgets::get_source_evidence_info() // evidence info
{
    return source_evidencer_obj->get_evicencer_info();
}

void source_widgets::on_pushButton_close_clicked() // close button
{
    emit signal_remove_button_clicked(received_id,received_source_type_internal);

}

void source_widgets::on_pushButton_repoint_clicked() // repoint the image path if changes incase of load case
{
    QString previous_file_path = ui->lineEdit_media_path->text().trimmed();

    QString file_path;
    if(received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal)
    {
        file_dialog_obj.setDirectory("/Volumes/");
        file_dialog_obj.setFileMode(QFileDialog::Directory);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                file_path = selected_files.at(0);
            }
        }
    }
    else if(received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal)
    {
        file_dialog_obj.setNameFilter(("*.dd *.DD *.E01 *.e01 *.dmg *.DMG *.sparsebundle *.sparseimage *.Ex01 *.ex01 *.S01 *.s01 *.000 *.001 *.00001 *.raw *.RAW *.vmdk *.vhd *.VHD *.aff4 *.AFF4"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                file_path = selected_files.at(0);
            }
        }
    }
    else if(received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal)
    {
        file_dialog_obj.setNameFilter(("*.dd *.DD *.E01 *.e01 *.dmg *.DMG *.sparsebundle *.sparseimage *.Ex01 *.ex01 *.S01 *.s01 *.000 *.001 *.00001 *.raw *.RAW *.vmdk *.vhd *.VHD"));

        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                file_path = selected_files.at(0);
            }
        }
    }
    else if(received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
    {

        file_dialog_obj.setNameFilter(("*.dmg *.DMG *.raw *.RAW *.dd *.DD *.e01 *.E01 *.ex01 *.Ex01 *.s01 *.S01 *.000 *.001 *.00001"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                file_path = selected_files.at(0);
            }
        }

    }
    else if(received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal
            || received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal
            || received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal)
    {
        file_dialog_obj.setNameFilter(("*.sparseimage *.SPARSEIMAGE "));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                file_path = selected_files.at(0);
            }
        }
    }
    else if(received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal)
        {
            file_dialog_obj.setNameFilter(("*.dmg *.DMG *.Sparseimage *.sparseimage"));
            file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
            if(file_dialog_obj.exec())
            {
                QStringList selected_files = file_dialog_obj.selectedFiles();

                if(selected_files.size() > 0)
                {
                    file_path = selected_files.at(0);
                }
            }
        }
    else if(received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal)
    {
        file_dialog_obj.setNameFilter(("*.l01 *.L01 "));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                file_path = selected_files.at(0);
            }
        }
    }
    else if(received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal
            || received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
            || received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal)
    {
        file_dialog_obj.setNameFilter(("*.dmg *.DMG "));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                file_path = selected_files.at(0);
            }
        }
    }
    else if(received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
    {
        QString mbdb_file_path;

        file_dialog_obj.setNameFilter(("Manifest.db Manifest.mbdb"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                mbdb_file_path = selected_files.at(0);
            }
        }

        if(mbdb_file_path.trimmed().isEmpty())
            return;

        if(QFileInfo(mbdb_file_path).fileName() == QString("Manifest.db") || QFileInfo(mbdb_file_path).fileName() == QString("Manifest.mbdb"))
        {
            file_path = QFileInfo(mbdb_file_path).absoluteDir().absolutePath();
        }
    }
    else if(received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal
            || received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Internal
            || received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal
            || received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal
            || received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal
            || received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal
            || received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GoogleTakeout_Internal)
    {

        file_dialog_obj.setFileMode(QFileDialog::Directory);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                file_path = selected_files.at(0);
            }
        }

    }
    else if(received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal
            || received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal)
    {
        file_dialog_obj.setNameFilter((QFileInfo(previous_file_path).fileName()));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                file_path = selected_files.at(0);
            }
        }

    }
    else if(received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal)
    {
        file_dialog_obj.setNameFilter(("*.iso *.ISO *.cdr *.CDR"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                file_path = selected_files.at(0);
            }
        }

    }


    file_path = file_path.trimmed();

    if(file_path.isEmpty())
        return;


    struct_GLOBAL_witness_info_root struct_rt_info = global_witness_info_manager_class_obj->pub_get_root_structure_according_root_count_name(received_root_count_name);

    if(QFileInfo(file_path).fileName() != QFileInfo(struct_rt_info.root_path_1).fileName())
    {
        QMessageBox::critical(this,windowTitle() , "It seems you have selected a different source. Source should likely be '" + QFileInfo(struct_rt_info.root_path_1).fileName() + "'.");
        return;
    }

    ui->lineEdit_media_path->setText(file_path);
}

void source_widgets::on_pushButton_repoint_2_clicked() // point the source path in case of changes in load case (fusion drive)
{

    QString file_path;
    if(received_source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal)
    {
        file_dialog_obj.setNameFilter(("*.dmg *.DMG *.raw *.RAW *.dd *.DD *.e01 *.E01 *.ex01 *.Ex01 *.s01 *.S01 *.000 *.001 *.00001"));
        file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
        if(file_dialog_obj.exec())
        {
            QStringList selected_files = file_dialog_obj.selectedFiles();

            if(selected_files.size() > 0)
            {
                file_path = selected_files.at(0);
            }
        }

    }

    file_path = file_path.trimmed();

    if(file_path.isEmpty())
        return;


    struct_GLOBAL_witness_info_root struct_rt_info = global_witness_info_manager_class_obj->pub_get_root_structure_according_root_count_name(received_root_count_name);

    if(QFileInfo(file_path).fileName() != QFileInfo(struct_rt_info.root_path_2_for_fusion).fileName())
    {
        QMessageBox::critical(this,windowTitle() , "It seems you have selected a different source. Source should likely be '" + QFileInfo(struct_rt_info.root_path_2_for_fusion).fileName() + "'.");
        return;
    }


    ui->lineEdit_media_path_2->setText(file_path);

}
