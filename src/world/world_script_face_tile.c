#include "fft/battle_unit.h"
#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/* FaceTile event instruction: turn every unit record of a unit toward a map tile.
 *
 * Parameters: 16-bit unit id, tile x, tile y, unused byte, rotate mode, speed index
 * into g_world_rotation_speed_frames, and a delay multiplier that staggers successive units by a quarter
 * of its value each. The second argument to world_script_filter_unit_id_by_mode is
 * taken from the g_world_unit_face_slot_order 16-bit table rather than a local counter. */
void world_script_face_tile(const event_face_tile_parameters_t* parameters) {
    SVECTOR position;
    SVECTOR target;
    u16 unit_id;
    s32 filter_mode;
    s32 i;
    s32 order;
    s32 facing;
    s32 dx;
    s32 dy;
    unit_animation_state_t* state;

    order = 1;
    target.vx = parameters->x * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
    target.vz = parameters->y * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
    unit_id = script_read_s16(parameters->unit_id_le);
    if (world_unit_prepare_event_filter(&unit_id, &filter_mode) != 0) {
        for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
            if (world_script_filter_unit_id_by_mode(&unit_id, &g_world_unit_face_slot_order[i], &filter_mode) != 0) {
                battle_unit_copy_screen_coords(unit_id, &position);
                dx = target.vx - position.vx;
                dy = target.vz - position.vz;
                facing = ((-ratan2(dy, dx) + 0xC00) & 0xF00) >> 8;
                state = &g_world_unit_animation_states[unit_id];
                state->target_facing = facing;
                state->rotate_mode = parameters->mode;
                state->frames_per_step = g_world_rotation_speed_frames[parameters->speed];
                state->step_counter = 0;
                state->rotating = 1;
                state->delay = parameters->delay * order++ / 4;
                if (filter_mode == 0) {
                    break;
                }
            }
        }
    }
}
