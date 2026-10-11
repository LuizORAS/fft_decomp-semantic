#include "fft/battle.h"
#include "psx/types.h"

/* Start reading one map file into `destination`. Returns `destination` when
 * the read was started, 0 when the row names no file or the start failed. */
s32 battle_map_start_file_load(
    void* unused_directory_path, gns_load_info_t* load_info, s32 destination, s32 unused_resource_type) {
    s32 result;

    if (load_info->fields.file_present_marker == 0) {
        result = 0;
    } else {
        /* These words are aligned within the file row; retail loads both with lw. */
        if (main_file_call_build_header(
                *(s32*)load_info->fields.disc_lba_le, *(s32*)load_info->fields.byte_length_le, (void*)destination)
            == 0) {
            return destination;
        }
        result = 0;
    }
    return result;
}
