#include "fft/battle_script.h"
#include "fft/battle_unit.h"
#include "fft/script.h"
#include "psx/types.h"

/*
 * Event instruction: set or clear movement-effect suppression on the addressed unit(s).
 * Parameters: 0x00 unit id (16-bit value), 0x02 nonzero to set, zero to clear.
 * A unit id above 0xff selects a group, so the unit-state loop runs over all
 * 21 unit indices. WORLD twin: world_script_set_units_movement_effect_suppression.
 */
void script_set_units_movement_effect_suppression(const event_movement_effect_parameters_t* parameters) {
    u16 unit_id;
    s32 state;
    s32 unit_index;

    unit_id = script_read_s16(parameters->unit_id_le);
    if (script_prepare_unit_filter(&unit_id, &state) != 0) {
        unit_index = 0;
        do {
            if (script_filter_unit_id_by_mode(&unit_id, (u16*)&unit_index, &state) != 0) {
                if (parameters->suppressed != 0) {
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
