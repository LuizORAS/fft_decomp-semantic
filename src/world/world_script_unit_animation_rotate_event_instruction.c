#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/*
 * Event instruction: face a unit at once and set its animation.
 *
 * Parameters: 0x00 unit id (halfword), 0x02 rotation, 0x03 animation id
 * (halfword; its high byte is also read as 0x04, the facing-update mode:
 * 0 clears the suppression, anything else sets it).
 */
void world_script_unit_animation_rotate_event_instruction(const u8* parameters) {
    const u8* data = parameters;
    u8 rotation;
    s16 animation;
    s16 call_misc_id;
    s16 misc_id;
    s32 index;
    world_unit_animation_state_t* rotation_state;

    misc_id = world_script_load_halfword(data);
    rotation = data[2];
    animation = world_script_load_halfword(data + 3);
    call_misc_id = world_script_get_misc_id(misc_id);
    misc_id = call_misc_id;
    if (data[4] == 0) {
        battle_unit_clear_facing_update_suppression(call_misc_id);
    } else {
        battle_unit_set_facing_update_suppression(call_misc_id);
    }
    index = misc_id;
    rotation_state = &g_world_unit_animation_states[index];
    rotation_state->target_facing = rotation;
    rotation_state->delay = 0;
    rotation_state->rotating = 0;
    world_unit_set_facing(index, rotation);
    world_unit_set_animation(index, animation);
}
