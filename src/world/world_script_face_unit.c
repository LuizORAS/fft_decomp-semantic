#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/* FACE_UNIT and FACE_EACH_OTHER event instructions: turn two units toward each other.
 *
 * Parameters: first 16-bit unit id, second 16-bit unit id, rotate mode, speed index
 * into g_world_rotation_speed_frames, and a delay multiplier. Every misc record of the second unit turns
 * toward the first; unless second_only is set (FaceUnit), the first unit also turns
 * toward the second. Successive second-unit records are staggered like FaceTile.
 * The angle is built in place (negate, then add) so combine cannot fold the two
 * constant additions into one. */
void world_script_face_unit(const event_face_unit_parameters_t* parameters, s32 second_only) {
    SVECTOR position;
    SVECTOR target;
    u16 unit_id;
    s32 misc_index;
    u16 misc_id;
    s32 i;
    s32 order;
    s32 facing;
    s32 reverse;
    s32 dx;
    s32 dy;
    unit_animation_state_t* state;

    order = 1;
    misc_id = script_read_s16(parameters->first_unit_id_le);
    unit_id = script_read_s16(parameters->second_unit_id_le);
    if (world_unit_try_get_misc_data_by_unit_id(&unit_id, &misc_index) != 0) {
        misc_id = script_get_misc_id(misc_id);
        if (misc_id != EVENT_MISC_ID_NONE) {
            for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
                if (world_script_filter_unit_id_by_mode(&unit_id, &g_world_unit_face_slot_order[i], &misc_index) != 0
                    && misc_id != unit_id) {
                    world_unit_copy_misc_screen_coords(misc_id, &position);
                    world_unit_copy_misc_screen_coords(unit_id, &target);
                    dx = target.vx - position.vx;
                    dy = target.vz - position.vz;
                    facing = -ratan2(dy, dx);
                    facing += 0xC00;
                    reverse = facing + 0x800;
                    facing &= 0xF00;
                    reverse &= 0xF00;
                    facing >>= 8;
                    reverse >>= 8;
                    if (second_only == 0) {
                        state = &g_world_unit_animation_states[misc_id];
                        state->target_facing = facing;
                        state->rotate_mode = parameters->mode;
                        state->frames_per_step = g_world_rotation_speed_frames[parameters->speed];
                        state->step_counter = 0;
                        state->rotating = 1;
                        state->delay = parameters->delay >> 2;
                    }
                    state = &g_world_unit_animation_states[unit_id];
                    state->target_facing = reverse;
                    state->rotate_mode = parameters->mode;
                    state->frames_per_step = g_world_rotation_speed_frames[parameters->speed];
                    state->step_counter = 0;
                    state->rotating = 1;
                    state->delay = parameters->delay * order++ / 4;
                    if (misc_index == 0) {
                        break;
                    }
                }
            }
        }
    }
}
