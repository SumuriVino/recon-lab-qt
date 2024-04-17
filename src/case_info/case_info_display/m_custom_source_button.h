#ifndef M_CUSTOM_SOURCE_BUTTON_H
#define M_CUSTOM_SOURCE_BUTTON_H

#include <QObject>
#include <QPushButton>
#include <QtCore>

class m_custom_source_button : public QPushButton
{
    Q_OBJECT
public:
    m_custom_source_button(QWidget *parent = 0);

    void pub_set_source_id(int id);
    void pub_set_source_name(QString name);
    void pub_set_os_scheme(QString scheme);

signals:
    void signal_source_button_clicked(bool open_close,int id,QString source_name,QString os_scheme);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
private:

    int source_id;
    QString source_name;
    QString os_scheme;

    bool bool_expandded;


};

#endif // M_CUSTOM_SOURCE_BUTTON_H
