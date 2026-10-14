#include "fft/battle_effect.h"
#include "fft/battle_gfx.h"
#include "fft/battle_unit.h"
#include "psx/libgpu.h"
#include "psx/libgte.h"

/*
 * Trace a projectile path from one unit's upper body to another's.
 *
 * Both endpoints are raised by two thirds of the unit's sprite height (12 when
 * the target has none). `unused_20` and `unused_38` reproduce unreferenced
 * frame slots the target reserves between the vectors.
 */
s32 effect_check_direct_trajectory_between_units(u8 source_id, u8 target_id) {
    SVECTOR origin;
    SVECTOR destination;
    SVECTOR unused_20;
    VECTOR delta;
    s32 unused_38[2];
    effect_obstacle_unit_list_t* obstacles;
    s32 distance;
    s32 source_height;
    s32 target_height;

    source_height = battle_gfx_get_unit_spritesheet_height_by_unit_id(source_id) * 2 / 3;
    target_height = battle_gfx_get_unit_spritesheet_height_by_unit_id(target_id) * 2 / 3;
    if (target_height == 0) {
        target_height = 12;
    }
    effect_list_valid_unit_targeting_data(source_id, target_id, &obstacles);
    battle_unit_get_tile_center_and_height_by_battle_id(source_id, &origin);
    origin.vy -= source_height;
    battle_unit_get_tile_center_and_height_by_battle_id(target_id, &destination);
    destination.vy -= target_height;
    setVector(&delta, destination.vx - origin.vx, destination.vy - origin.vy, destination.vz - origin.vz);
    distance = SquareRoot12((delta.vx * delta.vx + delta.vy * delta.vy + delta.vz * delta.vz) * ONE);
    return effect_trace_projectile_path(&delta, &origin, &distance, obstacles);
}
