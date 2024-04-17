#ifndef GPT_PARSER_H
#define GPT_PARSER_H

#include <QtCore>
#include "file_viewer/hex_viewer/hex_viewer_complete/utilities_function.h"

class gpt_parser
{
public:
    gpt_parser();

    void parse(QByteArray data);

    QList<common_structure_for_data_table> gpt_protective_mbr_data(void);
    QList<common_structure_for_data_table> gpt_header(void);

    QList<QList<common_structure_for_data_table>> gpt_partition_tables(void);


private:
    QByteArray gpt_data;

    utilities_function utility_obj;

    int gpt_start_sector;

    int partition_table_start_sector_int;

    bool is_partition_entry_blank(QByteArray ar);

};

#endif // GPT_PARSER_H
