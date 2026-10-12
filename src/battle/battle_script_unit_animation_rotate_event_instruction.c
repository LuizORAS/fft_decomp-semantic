#include "fft/battle.h"
#include "psx/types.h"

/* UnitAnimRotate (EVENT_OPCODE_UNIT_ANIM_ROTATE): set or clear the unit's facing-update suppression
 * by the byte at +4 (0 clears it), which is the animation id's high byte rather than an operand of its
 * own; turn the unit at once to the facing operand and start the animation. The unit id is not
 * checked, so an absent unit indexes the rotation table with EVENT_MISC_ID_NONE. */
void battle_script_unit_animation_rotate_event_instruction(const u8* parameters) {
    const u8* data = parameters;
    u8 rotation;
    s16 animation;
    s16 call_misc_id;
    s16 misc_id;
    s32 index;
    world_unit_animation_state_t* rotation_state;

    misc_id = battle_script_load_halfword(data);
    rotation = data[2];
    animation = battle_script_load_halfword(data + 3);
    call_misc_id = battle_script_get_misc_id(misc_id);
    misc_id = call_misc_id;
    if (data[4] == 0) {
        battle_unit_clear_facing_update_suppression(call_misc_id);
    } else {
        battle_unit_set_facing_update_suppression(call_misc_id);
    }
    index = misc_id;
    rotation_state = &g_battle_unit_animation_states[index];
    rotation_state->target_facing = rotation;
    rotation_state->delay = 0;
    rotation_state->rotating = 0;
    battle_unit_set_facing(index, rotation);
    battle_unit_set_animation(index, animation);
}
