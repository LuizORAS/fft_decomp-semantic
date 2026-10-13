#include "fft/battle_gfx.h"
#include "psx/types.h"

/* Hides a unit's shadow (UnitShadow event instruction). Returns 0 when no unit
 * has the misc id. */
s32 battle_gfx_clear_shadow_graphic_trigger_by_misc_id(u32 misc_id) {
    battle_unit_t* unit = battle_unit_get_misc_data_by_misc_id(misc_id);

    if (unit != 0) {
        unit->shadow_graphic_trigger = 0;
        return 1;
    }
    return 0;
}
