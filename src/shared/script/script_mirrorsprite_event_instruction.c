#include "fft/battle_unit.h"
#include "fft/script.h"

/* Mirror the unit sprite for mode 1 and restore it for every other mode. */
void script_mirrorsprite_event_instruction(const event_mirror_sprite_parameters_t* parameters) {
    s32 mode;
    s32 runtime_id;

    runtime_id = script_read_s16(parameters->unit_id_le);
    mode = parameters->mode;
    runtime_id = script_resolve_runtime_id(runtime_id);
    if (runtime_id != EVENT_RUNTIME_ID_NONE) {
        if (mode == 1) {
            battle_unit_set_horizontal_flip_flag(runtime_id);
        } else {
            battle_unit_clear_horizontal_flip_flag(runtime_id);
        }
    }
}
