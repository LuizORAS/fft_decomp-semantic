#include "fft/world.h"
#include "psx/libc.h"
#include "psx/types.h"

void world_menu_clear_selection_records(void) {
    memset(g_world_menu_selection_records, 0, 0x54);
}
