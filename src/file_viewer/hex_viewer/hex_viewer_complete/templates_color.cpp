#include "hex_viewer_complete.h"

void hex_viewer_complete::slot_action_template_fat_directory_entry_color_triggered(bool)
{

    struct_hex_viewer_tag tag_file_name;
    tag_file_name.start_address = current_cursor_position;
    tag_file_name.end_address = current_cursor_position + 7;
    tag_file_name.color_hex = "#01ff67";
    tag_file_name.description = "File Name";
    hex_viewer_tags_list.append(tag_file_name);


    struct_hex_viewer_tag tag_file_ext;
    tag_file_ext.start_address = current_cursor_position + 8;
    tag_file_ext.end_address = tag_file_ext.start_address + 2;
    tag_file_ext.color_hex = "#bb1067";
    tag_file_ext.description = "File Extension";
    hex_viewer_tags_list.append(tag_file_ext);

    struct_hex_viewer_tag tag_file_attr;
    tag_file_attr.start_address = current_cursor_position + 11;
    tag_file_attr.end_address = tag_file_attr.start_address;
    tag_file_attr.color_hex = "#017667";
    tag_file_attr.description = "Attribute";
    hex_viewer_tags_list.append(tag_file_attr);

    struct_hex_viewer_tag tag_reserved;
    tag_reserved.start_address = current_cursor_position + 0xC;
    tag_reserved.end_address = tag_reserved.start_address;
    tag_reserved.color_hex = "#0BB000";
    tag_reserved.description = "Reserved";
    hex_viewer_tags_list.append(tag_reserved);

    struct_hex_viewer_tag tag_file_mili;
    tag_file_mili.start_address = current_cursor_position + 0xD;
    tag_file_mili.end_address = tag_file_mili.start_address;
    tag_file_mili.color_hex = "#C07Cff";
    tag_file_mili.description = "Millisecond";
    hex_viewer_tags_list.append(tag_file_mili);

    struct_hex_viewer_tag tag_file_creation_time;
    tag_file_creation_time.start_address = current_cursor_position + 0xE;
    tag_file_creation_time.end_address = tag_file_creation_time.start_address + 1;
    tag_file_creation_time.color_hex = "#0070ff";
    tag_file_creation_time.description = "File Creation Time";
    hex_viewer_tags_list.append(tag_file_creation_time);

    struct_hex_viewer_tag tag_file_creation_date;
    tag_file_creation_date.start_address = current_cursor_position + 0x10;
    tag_file_creation_date.end_address = tag_file_creation_date.start_address + 1;
    tag_file_creation_date.color_hex = "#510667";
    tag_file_creation_date.description = "File Creation Date";
    hex_viewer_tags_list.append(tag_file_creation_date);

    struct_hex_viewer_tag tag_file_access_date;
    tag_file_access_date.start_address = current_cursor_position + 0x12;
    tag_file_access_date.end_address = tag_file_access_date.start_address + 1;
    tag_file_access_date.color_hex = "#AA09F7";
    tag_file_access_date.description = "File Access Date";
    hex_viewer_tags_list.append(tag_file_access_date);

    struct_hex_viewer_tag tag_high_word;
    tag_high_word.start_address = current_cursor_position + 0x14;
    tag_high_word.end_address = tag_high_word.start_address + 1;
    tag_high_word.color_hex = "#22A399";
    tag_high_word.description = "High Word";
    hex_viewer_tags_list.append(tag_high_word);

    struct_hex_viewer_tag tag_file_write_time;
    tag_file_write_time.start_address = current_cursor_position + 0x16;
    tag_file_write_time.end_address = tag_file_write_time.start_address + 1;
    tag_file_write_time.color_hex = "#111111";
    tag_file_write_time.description = "Write Time";
    hex_viewer_tags_list.append(tag_file_write_time);

    struct_hex_viewer_tag tag_file_write_date;
    tag_file_write_date.start_address = current_cursor_position + 0x18;
    tag_file_write_date.end_address = tag_file_write_date.start_address + 1;
    tag_file_write_date.color_hex = "#FFCC77";
    tag_file_write_date.description = "Write Date";
    hex_viewer_tags_list.append(tag_file_write_date);

    struct_hex_viewer_tag tag_file_low_word;
    tag_file_low_word.start_address = current_cursor_position + 0x1A;
    tag_file_low_word.end_address = tag_file_low_word.start_address + 1;
    tag_file_low_word.color_hex = "#C0C000";
    tag_file_low_word.description = "Low Word";
    hex_viewer_tags_list.append(tag_file_low_word);

    struct_hex_viewer_tag tag_file_size;
    tag_file_size.start_address = current_cursor_position + 0x1C;
    tag_file_size.end_address = tag_file_size.start_address + 3;
    tag_file_size.color_hex = "#F0CFFF";
    tag_file_size.description = "File Size";
    hex_viewer_tags_list.append(tag_file_size);


    create_tag_display();
}

void hex_viewer_complete::slot_action_template_manual_triggered(bool)
{
    QString file_path = QFileDialog::getOpenFileName(this, "RECON", "C:\\");

    QFile file(file_path);

    if(!file.open(QIODevice::ReadOnly))
        return;

    QStringList all_lines;

    while(!file.atEnd())
        all_lines.append(file.readLine().trimmed());

    file.close();

    foreach (QString line, all_lines) {

        line = line.trimmed();
        QStringList lines_parts = line.split(",");

        if(lines_parts.size() < 4)
            continue;

        QString start_adr = lines_parts.at(0);
        QString size = lines_parts.at(1);
        QString description = lines_parts.at(2);
        QString color_name = lines_parts.at(3);

        struct_hex_viewer_tag tag_file_name;
        tag_file_name.start_address = current_cursor_position + start_adr.toLongLong();
        tag_file_name.end_address = tag_file_name.start_address + size.toLongLong() - 1;
        tag_file_name.color_hex = color_name;
        tag_file_name.description = description;
        hex_viewer_tags_list.append(tag_file_name);
    }

    create_tag_display();

    return;
}

void hex_viewer_complete::slot_action_template_mbr_color_triggered(bool)
{

    struct_hex_viewer_tag t1;
    t1.start_address = current_cursor_position;
    t1.end_address = t1.start_address + 439;
    t1.color_hex = "#01ff67";
    t1.description = "Boot Code";
    hex_viewer_tags_list.append(t1);

    struct_hex_viewer_tag t2;
    t2.start_address = current_cursor_position + 440;
    t2.end_address = t2.start_address + 3;
    t2.color_hex = "#bb1067";
    t2.description = "Disk Signature";
    hex_viewer_tags_list.append(t2);

    struct_hex_viewer_tag t3;
    t3.start_address = current_cursor_position + 446;
    t3.end_address = t3.start_address + 16;
    t3.color_hex = "#017667";
    t3.description = "Partition 1";
    hex_viewer_tags_list.append(t3);

    struct_hex_viewer_tag t4;
    t4.start_address = current_cursor_position + 462;
    t4.end_address = t4.start_address + 16;
    t4.color_hex = "#0bb000";
    t4.description = "Partition 2";
    hex_viewer_tags_list.append(t4);

    struct_hex_viewer_tag t5;
    t5.start_address = current_cursor_position + 478;
    t5.end_address = t5.start_address + 16;
    t5.color_hex = "#c07cff";
    t5.description = "Partition 3";
    hex_viewer_tags_list.append(t5);

    struct_hex_viewer_tag t6;
    t6.start_address = current_cursor_position + 494;
    t6.end_address = t6.start_address + 16;
    t6.color_hex = "#0070ff";
    t6.description = "Partition 4";
    hex_viewer_tags_list.append(t6);

    struct_hex_viewer_tag t7;
    t7.start_address = current_cursor_position + 510;
    t7.end_address = t7.start_address + 1;
    t7.color_hex = "#510667";
    t7.description = "MBR Signature";
    hex_viewer_tags_list.append(t7);

    create_tag_display();
}



