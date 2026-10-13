#include "fft/battle_script.h"
#include "fft/battle_unit.h"
#include "fft/script.h"
#include "psx/types.h"

/* MirrorSprite (event instruction 0x68): mirrors the unit's sprite when the
 * operand byte is 1 and restores it otherwise. */
void script_mirrorsprite_event_instruction(const event_mirror_sprite_parameters_t* parameters) {
    s16 unit_id;
    s32 mirror;
    s32 runtime_id;

    unit_id = script_read_s16(parameters->unit_id_le);
    mirror = parameters->mode;
    runtime_id = script_resolve_runtime_id(unit_id);
    if (runtime_id != EVENT_RUNTIME_ID_NONE) {
        if (mirror == 1) {
            battle_unit_set_horizontal_flip_flag(runtime_id);
        } else {
            battle_unit_clear_horizontal_flip_flag(runtime_id);
        }
    }
}
