#include "fft/battle_unit.h"
#include "fft/script.h"

/* MirrorSprite (EVENT_OPCODE_MIRROR_SPRITE): mirror the unit's sprite when the operand byte is 1 and
 * restore it otherwise. BATTLE and WORLD compile this text. */
void script_mirror_sprite_event_instruction(const event_mirror_sprite_parameters_t* parameters) {
    s32 mode;
    s32 runtime_id;

    runtime_id = script_load_halfword(parameters->unit_id_le);
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
