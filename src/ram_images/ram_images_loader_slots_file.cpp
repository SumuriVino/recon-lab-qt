#include "ram_images_loader.h"

void ram_images_loader::slot_right_click_main_menu_triggered(QAction *current_clicked_action)
{
    if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Carve_Data))
    {
        action_extract_carved_data_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Carve_Files))
    {
        action_extract_carved_files_triggered();
    }
}
