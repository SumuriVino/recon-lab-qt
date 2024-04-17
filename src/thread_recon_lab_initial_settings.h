#ifndef THREAD_RECON_LAB_INITIAL_SETTINGS_H
#define THREAD_RECON_LAB_INITIAL_SETTINGS_H

#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"

class thread_recon_lab_initial_settings : public QThread
{
public:
    thread_recon_lab_initial_settings();

    void run();

private:

};

#endif // THREAD_RECON_LAB_INITIAL_SETTINGS_H
