#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/* MirrorSprite (EVENT_OPCODE_MIRROR_SPRITE): mirror the unit's sprite when the operand byte is 1 and
 * restore it otherwise; byte for byte battle_script_mirror_sprite_event_instruction. */
void world_script_mirror_sprite_event_instruction(const u8* parameters) {
    s16 unit_id;
    s32 mode;
    s32 misc_id;

    unit_id = world_script_load_halfword(parameters);
    mode = parameters[2];
    misc_id = world_script_get_misc_id(unit_id);
    if (misc_id != EVENT_MISC_ID_NONE) {
        if (mode == 1) {
            battle_unit_set_horizontal_flip_flag(misc_id);
        } else {
            battle_unit_clear_horizontal_flip_flag(misc_id);
        }
    }
}
