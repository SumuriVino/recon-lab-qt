#include "thread_recon_lab_initial_settings.h"

thread_recon_lab_initial_settings::thread_recon_lab_initial_settings()
{

}

void thread_recon_lab_initial_settings::run()
{
    QString home_dir;
    home_dir = getenv("HOME");

    QString cal_dir = home_dir + "/Library/Calendars";
    QString contacts_dir =  home_dir + "/Library/Application\ Support/AddressBook/";

    QDirIterator iter_1(cal_dir);

    while(iter_1.hasNext())
    {
        QString file_path = iter_1.next();

        global_recon_helper_singular_class_obj->get_apple_metadata_from_api(file_path);
    }



    QDirIterator iter_2(contacts_dir);

    while(iter_2.hasNext())
    {
        QString file_path = iter_2.next();

        global_recon_helper_singular_class_obj->get_apple_metadata_from_api(file_path);
    }

}

