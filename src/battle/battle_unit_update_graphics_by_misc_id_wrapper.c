#include "fft/battle_unit.h"
#include "psx/types.h"

void battle_unit_update_graphics_by_misc_id_wrapper(u32 misc_id) {
    battle_unit_update_display_by_misc_id(misc_id);
}
