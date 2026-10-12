#include "fft/battle_effect.h"
#include "fft/main.h"

/* Run the handler of every active secondary effect once, newest first, and remove
 * each one whose handler returns 0. Returns 0 when none is active.
 *
 * Before the handlers run it takes this frame's ordering table and flips the
 * buffer index their double-buffered primitives use. */
s32 effect_update_secondary_effects(void) {
    effect_secondary_data_t* effect;
    u32 previous_slot_id;

    g_effect_polygon_depth_data = main_gfx_get_otag();
    g_effect_buffer_index = 1 - g_effect_buffer_index;
    g_effect_current_slot_id = g_effect_secondary_active_tail;
    if (g_effect_secondary_active_tail == 0) {
        return 0;
    }

    do {
        effect = &g_effect_secondary_data[g_effect_current_slot_id];
        g_effect_current_secondary = effect;
        previous_slot_id = effect->previous_slot_id;
        if (g_effect_secondary_handlers[effect->function_id]() == 0) {
            effect_remove_secondary(g_effect_current_slot_id);
        }
        g_effect_current_slot_id = previous_slot_id;
    } while (previous_slot_id != 0);

    return 1;
}
