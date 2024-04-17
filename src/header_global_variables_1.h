#ifndef HEADER_GLOBAL_VARIABLES
#define HEADER_GLOBAL_VARIABLES

#include <QString>
#include "witness_info/witness_info_manager/witness_info_manager.h"
#include "csv_reader/csv_reader.h"

extern bool bool_Global_hide_seen_files;
const int Global_Variable_hex_viewer_BYTES_PER_LINE_int = 16;
extern int global_variable_license_validation_value_int;


extern QString global_variable_quick_look_preview_filepath_QString;
extern bool global_variable_quick_look_preview_close_status_bool;


extern witness_info_manager *global_witness_info_manager_class_obj;
extern csv_reader *global_csv_reader_class_selected_obj; // Deault contains all/full values of all csv... Change values at artifacts tab selection (for parsing) or somewhere else
extern csv_reader *global_csv_reader_class_full_obj; // Always contains all/full values of all csv



#endif // HEADER_GLOBAL_VARIABLES

