#include "fft/battle_gfx.h"

/* Hides a unit: clears draw_enabled and its sprite graphic trigger. Returns 0
 * when no unit has the misc id. */
s32 battle_gfx_reset_unit_graphic_trigger(u32 misc_id) {
    battle_unit_misc_data_t* unit = battle_unit_get_misc_data_by_misc_id(misc_id);

    if (unit != 0) {
        unit->draw_enabled = 0;
        unit->animation.trigger = 0;
        return 1;
    }
    return 0;
}
