#ifndef REMOVE_MULTIPLE_TAG_SELECOTOR_H
#define REMOVE_MULTIPLE_TAG_SELECOTOR_H

#include <QWidget>
#include <QtCore>
#include <QTableWidgetItem>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class remove_multiple_tag_selecotor;
}

class  remove_multiple_tag_selecotor : public QWidget
{
    Q_OBJECT

public:
    explicit remove_multiple_tag_selecotor(QWidget *parent = 0);
    ~remove_multiple_tag_selecotor();

    void pub_set_initialisation(QStringList tag_list,QStringList icons_list, QString table_name, QString record_INT);

signals:
    void signal_remaining_tags_list(QStringList, QString table_name, QString record);
    void signal_tag_list_to_be_removed(QStringList, QString table_name, QString record);

private slots:
    void on_pushButton_remove_tag_clicked();


    void on_tableWidget_remove_tag_list_cellClicked(int row, int column);

private:
    Ui::remove_multiple_tag_selecotor *ui;

    void display_tag_table();
    QStringList recived_tag_list;
    QStringList recived_icons_list;
    QString table_name;
    QString record;
};

#endif // REMOVE_MULTIPLE_TAG_SELECOTOR_H
