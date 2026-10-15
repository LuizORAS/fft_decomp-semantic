#include "fft/battle.h"
#include "psx/types.h"

/* EVENT_OPCODE_SET_UNIT_MOVEMENT_EFFECT_SUPPRESSION: set (nonzero operand) or clear the
 * movement-effect suppression of the unit, or of each unit of a group.
 *
 * A unit id above 0xff selects a group, so the misc-state loop runs over all 21 unit indices. */
void battle_script_set_units_movement_effect_suppression(const u8* parameters) {
    u16 unit_id;
    s32 state;
    s32 unit_index;

    unit_id = battle_script_load_halfword(parameters);
    if (battle_unit_try_get_misc_data_by_unit_id(&unit_id, &state) != 0) {
        unit_index = 0;
        do {
            if (battle_script_filter_unit_id_by_mode(&unit_id, (u16*)&unit_index, &state) != 0) {
                if (parameters[2] != 0) {
                    battle_unit_set_movement_effect_suppression(unit_id);
                } else {
                    battle_unit_clear_movement_effect_suppression(unit_id);
                }
                if (state == 0) {
                    break;
                }
            }
            unit_index++;
        } while (unit_index < EVENT_UNIT_SLOT_COUNT);
    }
}
