#include "fft/battle_unit.h"
#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/*
 * Event instruction: face a unit at once and set its animation.
 *
 * Parameters: 0x00 unit id (16-bit value), 0x02 rotation, 0x03 animation id
 * (16-bit value; its high byte is also read as 0x04, the facing-update mode:
 * 0 clears the suppression, anything else sets it).
 */
void world_script_unit_animation_rotate_event_instruction(const event_unit_anim_rotate_parameters_t* parameters) {
    const event_unit_anim_rotate_parameters_t* data = parameters;
    u8 rotation;
    s16 animation;
    s16 call_runtime_id;
    s16 runtime_id;
    s32 index;
    unit_animation_state_t* rotation_state;

    runtime_id = script_read_s16(data->unit_id_le);
    rotation = data->rotation;
    animation = script_read_s16(data->animation_le);
    call_runtime_id = script_resolve_runtime_id(runtime_id);
    runtime_id = call_runtime_id;
    if (data->animation_le[1] == 0) {
        battle_unit_clear_facing_update_suppression(call_runtime_id);
    } else {
        battle_unit_set_facing_update_suppression(call_runtime_id);
    }
    index = runtime_id;
    rotation_state = &g_world_unit_animation_states[index];
    rotation_state->target_facing = rotation;
    rotation_state->delay = 0;
    rotation_state->rotating = 0;
    world_unit_set_facing(index, rotation);
    world_unit_set_animation(index, animation);
}
