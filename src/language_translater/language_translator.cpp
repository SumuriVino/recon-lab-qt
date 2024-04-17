#include "language_translator.h"

language_translator::language_translator()
{

#ifdef MACRO_RECON_STATIC_APP_RUN_BY_FORENSO
    remaining_keyword_csv_file = getenv("HOME") + QString("/Desktop/remaining.csv");
    QFile::remove(remaining_keyword_csv_file) ;
#endif

}


void language_translator::set_essentials()
{

    map_keyword.clear();

    QString m_path = "../Resources/all_keyword.sqlite";
    if(!QFileInfo(m_path).exists())
        return;

    QString translated_db_path = recon_static_functions::decrypt_file_1(m_path, Q_FUNC_INFO);
    if(!QFileInfo(translated_db_path).exists())
        return;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(translated_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("destination_db open ---FAILED---" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("error" + destination_db.lastError().text(), Q_FUNC_INFO);
        QFile(translated_db_path).remove();
        return;
    }

    QSqlQuery query_select(destination_db);
    QString command = "select original_keyword,english_keyword,chinese_keyword from keywords";
    if (!query_select.exec(command))
    {
        recon_static_functions::app_debug("query_select ---FAILED---" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("error" + query_select.executedQuery(), Q_FUNC_INFO);
        destination_db.close();
        QFile(translated_db_path).remove();
        return;
    }

    int language_position = global_narad_muni_class_obj->get_field(MACRO_NARAD_User_Selected_Language_int).toInt();

    while(query_select.next())
    {
        map_keyword.insert(query_select.value("original_keyword").toString().trimmed(), query_select.value(language_position).toString().trimmed());
    }

    destination_db.close();
    QFile(translated_db_path).remove();
}

QString language_translator::get_translated_string(QString actual_key)
{
    return actual_key;


    QString m_space = " ";
    QString prpnd_val;
    QString apnd_val;
    if(actual_key.startsWith(m_space))
        prpnd_val = m_space;
    if(actual_key.endsWith(m_space))
        apnd_val = m_space;

    QString m_key = actual_key;
    m_key = m_key.trimmed();
    if(m_key.isEmpty() || global_narad_muni_class_obj->get_field(MACRO_NARAD_User_Selected_Language_int).toInt() == enum_global_language_translater_original)
        return (prpnd_val + m_key + apnd_val);

    QString m_value = map_keyword.value(m_key);
    m_value= m_value.trimmed();

    if(m_value.isEmpty())
    {

#ifdef MACRO_RECON_STATIC_APP_RUN_BY_FORENSO
        QString sel_lang = "ORIGINAL";
        if(global_narad_muni_class_obj->get_field(MACRO_NARAD_User_Selected_Language_int).toInt() == enum_global_language_translater_english)
            sel_lang = "ENGLISH";
        else if(global_narad_muni_class_obj->get_field(MACRO_NARAD_User_Selected_Language_int).toInt() == enum_global_language_translater_chinese)
            sel_lang = "CHINESE";

        //recon_static_functions::app_debug( "---FAILED--- Selected Language:" + sel_lang + ", Untranslated keyword found: " + m_key, Q_FUNC_INFO);
        fill_remaining_keyword_csv_file(m_key, Q_FUNC_INFO);
#endif

        // return (m_key + apnd_val);
        // return (m_key + "_愛所有人" + apnd_val);
        return (prpnd_val + m_key + "_愛" + apnd_val);
    }

    if(actual_key.startsWith(m_space))
    {
        if(!m_value.startsWith(m_space))
            m_value = (prpnd_val + m_value);
    }

    if(actual_key.endsWith(m_space))
    {
        if(!m_value.endsWith(m_space))
            m_value = (m_value + apnd_val);
    }


    return m_value;
}


QStringList language_translator::get_translated_stringlist(QStringList key_list)
{
    return key_list;


    if(global_narad_muni_class_obj->get_field(MACRO_NARAD_User_Selected_Language_int).toInt() == enum_global_language_translater_original)
        return key_list;


    QStringList value_list;

    for(int ii = 0; ii < key_list.size(); ii++)
    {
        QString m_key = key_list.at(ii);
        QString actual_key = m_key;

        QString m_space = " ";
        QString prpnd_val;
        QString apnd_val;
        if(actual_key.startsWith(m_space))
            prpnd_val = m_space;
        if(actual_key.endsWith(m_space))
            apnd_val = m_space;

        m_key = m_key.trimmed();
        if(m_key.isEmpty())
        {
            value_list << (prpnd_val + m_key + apnd_val);
        }
        else
        {
            QString m_value = map_keyword.value(m_key);
            m_value = m_value.trimmed();
            if(m_value.isEmpty())
            {

#ifdef MACRO_RECON_STATIC_APP_RUN_BY_FORENSO
                QString sel_lang = "ORIGINAL";
                if(global_narad_muni_class_obj->get_field(MACRO_NARAD_User_Selected_Language_int).toInt() == enum_global_language_translater_english)
                    sel_lang = "ENGLISH";
                else if(global_narad_muni_class_obj->get_field(MACRO_NARAD_User_Selected_Language_int).toInt() == enum_global_language_translater_chinese)
                    sel_lang = "CHINESE";

                // recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "---FAILED--- Selected Language:" + sel_lang + ", Untranslated keyword found: " + m_key, Q_FUNC_INFO);
                fill_remaining_keyword_csv_file(m_key, Q_FUNC_INFO);
#endif

                // value_list << (m_key + apnd_val);
                // value_list << (prpnd_val + m_key + "_愛所有人" + apnd_val);
                value_list << (prpnd_val + m_key + "_愛" + apnd_val);
            }
            else
            {
                if(actual_key.startsWith(m_space))
                {
                    if(!m_value.startsWith(m_space))
                        m_value = (prpnd_val + m_value);
                }

                if(actual_key.endsWith(m_space))
                {
                    if(!m_value.endsWith(m_space))
                        m_value = (m_value + apnd_val);
                }

                value_list << m_value;
            }
        }
    }

    return value_list;
}

QString language_translator::get_original_string(QString translated_value)
{
    //return translated_value;

    //    return translated_value.remove("_愛"); // prpnd_val + m_key + "_愛" + apnd_val);

    translated_value = translated_value.trimmed();
    if(translated_value.isEmpty() || global_narad_muni_class_obj->get_field(MACRO_NARAD_User_Selected_Language_int).toInt() == enum_global_language_translater_original)
        return translated_value;

    QString m_key = map_keyword.key(translated_value);

    if(m_key.trimmed().isEmpty())
    {
        return translated_value;
    }

    return m_key;
}

void language_translator::fill_remaining_keyword_csv_file(QString m_keyword, QString caller_func)
{
    QFile mfile(remaining_keyword_csv_file);
    if(!mfile.open(QIODevice::Append))
    {
        recon_static_functions::app_debug(" opening remaining csv file ---FAILED---" + mfile.fileName() + " keyword: " + m_keyword, Q_FUNC_INFO);
        recon_static_functions::app_debug("error - " + mfile.errorString() , Q_FUNC_INFO);
        return;
    }
    QTextStream out(&mfile);
    out << m_keyword << "\n";
    mfile.flush();
    mfile.close();

}


