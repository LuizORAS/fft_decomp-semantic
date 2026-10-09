#include "fft/world.h"
#include "psx/libc.h"
#include "psx/types.h"

void world_menu_clear_selection_record(s32 index) {
    memset(&g_world_menu_selection_records[index], 0, sizeof(world_menu_selection_record_t));
}
