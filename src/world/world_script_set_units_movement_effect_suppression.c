#include "fft/battle_unit.h"
#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/* Set or clear movement-effect suppression for units selected by an event operand. */
void world_script_set_units_movement_effect_suppression(const event_movement_effect_parameters_t* parameters) {
    u16 unit_id;
    s32 filter_mode;
    s32 unit_index;

    unit_id = script_read_s16(parameters->unit_id_le);
    if (script_prepare_unit_filter(&unit_id, &filter_mode) != 0) {
        for (unit_index = 0; unit_index < BATTLE_UNIT_SLOT_COUNT; unit_index++) {
            if (world_script_filter_unit_id_by_mode(&unit_id, &unit_index, &filter_mode) != 0) {
                if (parameters->suppressed != 0) {
                    battle_unit_set_movement_effect_suppression(unit_id);
                } else {
                    battle_unit_clear_movement_effect_suppression(unit_id);
                }
                if (filter_mode == 0) {
                    break;
                }
            }
        }
    }
}
