#include "fft/world.h"

/* Point the scrollable list at a new entry list with its cursor row and scroll base
 * (world_menu_init_scrollable_list), then load its layout from the list record.
 *
 * Built at -O1: the unscheduled prologue (sw ra, sw s0, lw s0) is the -O1 signature shared by the
 * neighbouring menu-script handlers. */
void world_menu_init_and_load_scrollable_list_with_scroll(
    const s16* values, s32 value, s32 selected, u8* text_table, menu_instruction_t* script) {
    world_menu_init_scrollable_list(values, value, selected, text_table);
    world_menu_load_scrollable_list_layout(script);
}
