#ifndef RECON_CONFIGURATION_PAGE_VOLATILITY_H
#define RECON_CONFIGURATION_PAGE_VOLATILITY_H

#include <QWidget>

#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
                                   
#include "recon_generic_functions/recon_static_functions.h"
#include <QFileDialog>
#include "recon_generic_functions/recon_helper_standard.h"      


namespace Ui {
class recon_configuration_page_volatility;
}

class recon_configuration_page_volatility : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration_page_volatility(QWidget *parent = 0);
    ~recon_configuration_page_volatility();

    void pub_set_essentials();
private slots:
    void on_pushButton_open_clicked();

private:
    Ui::recon_configuration_page_volatility *ui;

    QString destination_db_file;
    QString volatility_path;

    void prepare_display();

    QFileDialog file_dialog_obj;
    recon_helper_standard *recon_helper_standard_obj;

};

#endif // RECON_CONFIGURATION_PAGE_VOLATILITY_H
