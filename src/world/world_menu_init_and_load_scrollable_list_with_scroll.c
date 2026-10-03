#include "fft/world.h"

/* Built at -O1: the unscheduled prologue (sw ra, sw s0, lw s0) is the -O1 signature shared by the
 * neighbouring menu-script handlers. */

void world_menu_init_and_load_scrollable_list_with_scroll(
    s16* values, s32 value, s32 selected, s32 state, world_menu_list_record_t* list_record) {
    world_menu_init_scrollable_list(values, value, selected, state);
    world_menu_load_scrollable_list_layout(list_record);
}
