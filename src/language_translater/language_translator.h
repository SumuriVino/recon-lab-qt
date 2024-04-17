#ifndef LANGUAGE_TRANSLATOR_H
#define LANGUAGE_TRANSLATOR_H

#include <QObject>
#include <QWidget>
#include "recon_generic_functions/recon_static_functions.h"
#include <QtSql>
#include <QtCore>
#include <QMap>
#include <QApplication>
#include <header_global_enum.h>
#include "header_global_variables_2.h"

class language_translator
{
public:
    language_translator();


    void set_essentials();
    QString get_translated_string(QString actual_key);
    QString get_original_string(QString translated_value);

    QStringList get_translated_stringlist(QStringList m_key);
signals:

public slots:

private:

    QString remaining_keyword_csv_file;
    QMap <QString, QString> map_keyword;

    void fill_remaining_keyword_csv_file(QString m_keyword, QString caller_func);

};

#endif // LANGUAGE_TRANSLATOR_H
