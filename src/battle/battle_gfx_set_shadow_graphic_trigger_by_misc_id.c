#include "fft/battle_gfx.h"
#include "psx/types.h"

/* Shows a unit's shadow (UnitShadow event instruction). Returns 0 when no unit
 * has the misc id. */
s32 battle_gfx_set_shadow_graphic_trigger_by_misc_id(u32 misc_id) {
    battle_unit_misc_data_t* unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);

    if (unit != 0) {
        unit->shadow_graphic_trigger = 1;
        return 1;
    }
    return 0;
}
