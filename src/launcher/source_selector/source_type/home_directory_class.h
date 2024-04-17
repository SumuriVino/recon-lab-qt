#ifndef HOME_DIRECTORY_H
#define HOME_DIRECTORY_H

#include <QFrame>

namespace Ui {
class Home_Directory_Class;
}

class Home_Directory_Class : public QFrame
{
    Q_OBJECT

public:
    explicit Home_Directory_Class(QWidget *parent = 0);
    ~Home_Directory_Class();
    void pub_set_id_and_name(int id,QString name);
    int pub_get_id();
    int pub_get_window_height();
signals:
    void signal_home_directory_user_name_inserted(QString user_name);

private slots:
    void on_pushButton_add_username_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::Home_Directory_Class *ui;
    int received_id;
    QString received_name;

};

#endif // HOME_DIRECTORY_H
