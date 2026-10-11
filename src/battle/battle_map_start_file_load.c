#include "fft/battle.h"
#include "psx/types.h"

/* `load_info` points to +0x06 of a raw GNS file row. Relative offsets
 * 0x02/0x06 hold its 32-bit absolute disc LBA and byte length; +0x0d is the
 * nonzero file marker at row offset 0x13. */
enum {
    MAP_FILE_LOAD_LBA_OFFSET = 0x02,
    MAP_FILE_LOAD_BYTE_LENGTH_OFFSET = 0x06,
    MAP_FILE_LOAD_PRESENT_OFFSET = 0x0d,
};

/* Start reading one map file into `destination`. Returns `destination` when
 * the read was started, 0 when the row names no file or the start failed. */
s32 battle_map_start_file_load(void* unused_directory_path, u8* load_info, s32 destination, s32 unused_resource_type) {
    s32 result;

    if (load_info[MAP_FILE_LOAD_PRESENT_OFFSET] == 0) {
        result = 0;
    } else {
        if (main_file_request_read_bytes(*(s32*)(load_info + MAP_FILE_LOAD_LBA_OFFSET),
                *(s32*)(load_info + MAP_FILE_LOAD_BYTE_LENGTH_OFFSET), (void*)destination)
            == 0) {
            return destination;
        }
        result = 0;
    }
    return result;
}
