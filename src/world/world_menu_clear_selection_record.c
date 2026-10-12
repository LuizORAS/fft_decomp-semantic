#include "fft/world.h"
#include "psx/libc.h"
#include "psx/types.h"

/* Forget the saved position of list `index` (world_menu_set_selection_record). */
void world_menu_clear_selection_record(s32 index) {
    memset(&g_world_menu_selection_records[index], 0, sizeof(world_menu_selection_record_t));
}
