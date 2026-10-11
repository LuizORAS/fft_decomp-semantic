#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/* Event instruction 0x2f MirrorSprite: flip one unit's sprite horizontally.
 *
 * Ported from the byte-identical BATTLE twin at 0x8013e65c. */

void world_script_mirrorsprite_event_instruction(const event_mirror_sprite_parameters_t* parameters) {
    s16 unit_id;
    s32 mode;
    s32 misc_id;

    unit_id = world_script_load_halfword(parameters->unit_id_le);
    mode = parameters->mode;
    misc_id = world_script_get_misc_id(unit_id);
    if (misc_id != EVENT_MISC_ID_NONE) {
        if (mode == 1) {
            battle_unit_set_horizontal_flip_flag(misc_id);
        } else {
            battle_unit_clear_horizontal_flip_flag(misc_id);
        }
    }
}
