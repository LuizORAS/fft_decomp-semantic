#include "fft/battle_unit.h"
#include "psx/types.h"

void battle_unit_update_display_by_misc_id(s32 misc_id) {
    battle_unit_update_display(battle_unit_get_misc_data_by_misc_id(misc_id));
}
