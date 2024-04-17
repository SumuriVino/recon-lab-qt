#ifndef FILE_OPEN_CELL_WIDGET_H
#define FILE_OPEN_CELL_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QProcess>
#include <QtCore>
#include <QFileDialog>
#include <QPixmap>
#include <QBitmap>

class file_open_cell_widget :public QWidget
{
    Q_OBJECT
public:
    explicit file_open_cell_widget(QWidget *parent = 0);
    file_open_cell_widget(QString path);

    ~file_open_cell_widget();

    void pub_set_global_report_dir_name(QString path);
signals:

public slots:
    void slot_pushbutton_clicked();
    void slot_label_file_linkactivated(QString m_type);

private:
  QPushButton *pushbutton_open;
  QLabel *label_file;
  QString file_path;
};

#endif // FILE_OPEN_CELL_WIDGET_H
