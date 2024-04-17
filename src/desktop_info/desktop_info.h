#ifndef DESKTOP_INFO
#define DESKTOP_INFO

#include <QWidget>
#include <QtCore>
#include "header_global_structures.h"
#include "recon_generic_functions/recon_helper_process.h"
#include "recon_generic_functions/recon_helper_standard.h"

class desktop_info : public QObject
{
    Q_OBJECT
public:
    desktop_info(QWidget *parent = nullptr);
    ~desktop_info();


    void pub_fill_desktop_info_general(QString caller_func);
    struct_global_desktop_info pub_get_desktop_info(QString caller_func);

    void pub_update_desktop_password(bool mstatus, QString caller_func);


signals:


private:

    struct_global_desktop_info st_desktop_info;

    bool bool_consistency_requirements_satisfied();

    void update_python_info(struct_global_desktop_info &mobj);

    recon_helper_standard *recon_helper_standard_obj;

    QStringList consistency_requirements_failed_list;
};

#endif // DESKTOP_INFO
