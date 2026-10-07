#include "fft/main.h"

/* Start a quiet read of size bytes (whole sectors, rounded down) into destination on the
 * shared descriptor g_main_file_cd_state; the overlay's frame loop advances it with
 * main_file_poll_load and tests main_file_is_still_loading. Returns 1 while a read is
 * already in flight. */
s32 main_file_request_read_bytes(s32 sector, s32 size, void* destination) {
    return main_file_request_read_quiet(&g_main_file_cd_state, sector, (u32)size >> 11, destination);
}
