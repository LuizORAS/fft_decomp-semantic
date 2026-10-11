#include "fft/battle.h"
#include "psx/types.h"

/*
 * Parameters: 0x00 unit id (halfword), 0x02 rotation, 0x03 animation id
 * (halfword), 0x04 move-flag mode (0 unset, else flip).
 */
void battle_script_unit_animation_rotate_event_instruction(const event_unit_anim_rotate_parameters_t* parameters) {
    const event_unit_anim_rotate_parameters_t* data = parameters;
    u8 rotation;
    s16 animation;
    s16 call_misc_id;
    s16 misc_id;
    s32 index;
    unit_animation_state_t* rotation_state;

    misc_id = battle_script_load_halfword(data->unit_id_le);
    rotation = data->rotation;
    animation = battle_script_load_halfword(data->animation_le);
    call_misc_id = battle_script_get_misc_id(misc_id);
    misc_id = call_misc_id;
    if (data->animation_le[1] == 0) {
        battle_unit_clear_facing_update_suppression(call_misc_id);
    } else {
        battle_unit_set_facing_update_suppression(call_misc_id);
    }
    index = misc_id;
    rotation_state = &g_battle_unit_misc_rotation_data[index];
    rotation_state->target_facing = rotation;
    rotation_state->delay = 0;
    rotation_state->rotating = 0;
    battle_unit_set_facing(index, rotation);
    battle_unit_set_animation(index, animation);
}
