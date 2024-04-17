#ifndef M_BARSET_H
#define M_BARSET_H

#include <QtCharts>
#include <QBarSet>
#include <QtCore>

class Q_CHARTS_EXPORT m_barset : public QBarSet
{
    Q_OBJECT
public:
    explicit m_barset(const QString label, QObject *parent = Q_NULLPTR);
    virtual ~m_barset();

    QMap<int , QString> m_map;

    void set_tooltip(QMap<int , QString> map);

   // QString m_tooltip(void);

    QMap<int , QString> get_tooltip(void);

};

#endif // M_BARSET_H
