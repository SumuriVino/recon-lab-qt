#ifndef EDIT_CATEGORY_DIALOG_H
#define EDIT_CATEGORY_DIALOG_H

#include <QDialog>
#include <QtCore>


namespace Ui {
class edit_category_dialog;
}

class edit_category_dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit edit_category_dialog(QWidget *parent = 0);
    ~edit_category_dialog();
    void set_category_name(QString cat);
    
signals:
    void signal_edit_category(QString cat);

private slots:
    void on_pushButton_save_category_clicked();

private:
    Ui::edit_category_dialog *ui;
};

#endif // EDIT_CATEGORY_DIALOG_H
