#include "fft/world.h"

/* Run the shop's roster-count display script, after writing the party unit
 * count into it, with menu value callback 0 removed, then restore the
 * callback. */
void world_menu_run_script_with_callback_suppressed(void) {
    s32 (*previous_callback)(s32);

    previous_callback = g_world_menu_script_callbacks[0].value;
    g_world_menu_script_callbacks[0].value = 0;
    g_world_shop_roster_count_menu_script.unit_count.value = g_world_shop_party_unit_count;
    /* The serialized script prefix shares the bytecode interpreter's instruction view. */
    world_menu_run_script_with_palette_mode((menu_instruction_t*)&g_world_shop_roster_count_menu_script, 0, 0);
    g_world_menu_script_callbacks[0].value = previous_callback;
}
