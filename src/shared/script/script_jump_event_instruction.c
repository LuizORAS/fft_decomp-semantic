#include "fft/battle_unit.h"
#include "fft/script.h"

/* Jump: cancel the unit's accumulated event offset, then build its one-step path
 * of length_plus_one - 1 tiles in the encoded direction through
 * battle_unit_prepare_rider_dismount. The unit's Jump stat is 6 while that runs
 * and is restored afterwards. A unit id that does not resolve does nothing. */
void script_jump_event_instruction(const event_jump_parameters_t* parameters) {
    SVECTOR vec;
    s32 runtime_id;
    SVECTOR* coords;
    unit_stats_t* stats;
    u8 saved;

    runtime_id = script_resolve_runtime_id(script_read_s16(parameters->unit_id_le));
    if (runtime_id != EVENT_RUNTIME_ID_NONE) {
        coords = battle_unit_get_event_offset_by_runtime_id(runtime_id);
        setVector(&vec, -coords->vx, -coords->vy, -coords->vz);
        battle_unit_add_event_offset_by_runtime_id(runtime_id, &vec);
        stats = battle_unit_get_stats_from_battle_id(battle_unit_get_battle_id_by_runtime_id(runtime_id));
        saved = stats->jump;
        stats->jump = 6;
        battle_unit_prepare_rider_dismount(runtime_id, parameters->direction, parameters->length_plus_one - 1);
        stats->jump = saved;
    }
}
