#include "fft/world.h"
#include "psx/types.h"

/* Set the ordering-table priority the display scripts draw at. */
void world_menu_set_draw_priority(s32 priority) {
    g_world_menu_draw_priority = priority;
}
