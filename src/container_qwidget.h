#ifndef CONTAINER_QWIDGET_H
#define CONTAINER_QWIDGET_H

#include <QWidget>

class Container_QWidget : public QWidget
{
    Q_OBJECT
public:
    explicit Container_QWidget(QWidget *parent = 0);
    ~Container_QWidget();
    void pub_set_feature_widget_name(QString);
    void pub_set_category_name(QString cat_nm);
    void pub_set_case_name(QString case_nm);
    void pub_set_widget_unique_name(QString);

    QString pub_get_feature_widget_name(void);
    QString pub_get_category_name(void);
    QString pub_get_case_name(void);
    QString pub_get_widget_unique_name(void);
signals:

public slots:

private:
    QString widget_name;
    QString unique_name;
    QString category_name;
    QString case_name;

};

#endif // CONTAINER_QWIDGET_H
