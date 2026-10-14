#include "fft/battle_effect.h"
#include "fft/battle_gfx.h"
#include "fft/battle_map.h"
#include "fft/battle_unit.h"
#include "fft/map.h"
#include "fft/unit.h"
#include "psx/libgte.h"
#include "psx/types.h"

/*
 * Set up an arcing projectile from source_id toward target_id (or the tile at
 * tile_position when target_id is -1) and pick the arc to fly.
 *
 * Both candidate arcs from effect_calculate_arcing_trajectory are
 * traced, arc_b first; a clear arc returns target_id. Otherwise an arc blocked
 * by a unit of another team than the source wins (arc_b's first) and returns
 * that unit. Failing that, arc_a's trace is kept and its blocker (or -1) is
 * returned, except when only arc_a was blocked by a unit: then arc_b's
 * terrain-blocked result is restored and -1 returned. g_effect_arc_trajectory_height receives the
 * chosen arc height. `unused` reserves the target's 16 otherwise unused frame bytes.
 */
s32 effect_set_and_validate_arc_trajectory(s32 source_id, SVECTOR* tile_position, s32 target_id) {
    s32 unused[4];
    SVECTOR arc;
    VECTOR second_step;
    VECTOR first_step;
    SVECTOR second_position;
    SVECTOR first_position;
    battle_arc_query_t query;
    effect_obstacle_unit_list_t* obstacles;
    s32 arc_a;
    s32 arc_b;
    map_tile_t* tile;
    unit_stats_t* stats;
    unit_stats_t* other;
    s32 hit;
    s32 second_hit;
    s32 first_unit;
    s32 first_other;
    s32 first_count;
    s32 second_unit;
    s32 second_other;
    s32 second_count;
    s32 result;
    s32 height;

    g_effect_trajectory_source_id = source_id;
    query.source.unit_id = source_id;
    query.target.unit_id = target_id;
    query.source.height = battle_gfx_get_unit_spritesheet_height_by_unit_id(source_id) * 2 / 3;
    if (target_id != -1) {
        query.target.height = battle_gfx_get_unit_spritesheet_height_by_unit_id(target_id);
    } else {
        query.target.height = 0;
    }
    effect_list_valid_unit_targeting_data(source_id, -1, &obstacles);
    battle_unit_get_tile_center_and_height_by_battle_id(source_id, &g_effect_trajectory_origin);
    g_effect_trajectory_origin.vy -= query.source.height;
    if (target_id == -1) {
        tile = map_get_tile_data_pointer(tile_position->vx, tile_position->vz, tile_position->vy);
        g_effect_trajectory_destination.vx = tile_position->vx * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
        g_effect_trajectory_destination.vz = tile_position->vz * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
        g_effect_trajectory_destination.vy = -((tile->height + tile->depth_half_height.depth) * 12);
    } else {
        battle_unit_get_tile_center_and_height_by_battle_id(target_id, &g_effect_trajectory_destination);
    }
    query.delta.x = g_effect_trajectory_destination.vx - g_effect_trajectory_origin.vx;
    query.delta.z = g_effect_trajectory_destination.vz - g_effect_trajectory_origin.vz;
    query.delta.y = g_effect_trajectory_origin.vy - g_effect_trajectory_destination.vy;
    query.delta.distance = SquareRoot12((query.delta.x * query.delta.x + query.delta.z * query.delta.z) * ONE);
    arc.vy = ratan2(-query.delta.z, query.delta.x);
    if (effect_calculate_arcing_trajectory(&query, &arc_a, &arc_b) == 0) {
        arc_b = ONE;
        arc_a = ONE;
    }

    hit = effect_trace_arc_trajectory_path(&g_effect_trajectory_origin, &arc_b, &query.delta.distance, &arc, obstacles);
    first_unit = g_effect_trajectory_hit_unit_id;
    first_step = g_effect_trajectory_step;
    first_position = g_effect_trajectory_position;
    first_count = g_effect_trajectory_step_count;
    first_other = -1;
    if (g_effect_trajectory_hit_unit_id == -1) {
        first_other = g_effect_trajectory_tile_flags;
    }
    if (hit != 0) {
        height = arc_b;
        result = target_id;
    } else {
        second_hit = effect_trace_arc_trajectory_path(
            &g_effect_trajectory_origin, &arc_a, &query.delta.distance, &arc, obstacles);
        second_unit = g_effect_trajectory_hit_unit_id;
        second_step = g_effect_trajectory_step;
        second_position = g_effect_trajectory_position;
        second_count = g_effect_trajectory_step_count;
        second_other = -1;
        if (g_effect_trajectory_hit_unit_id == -1) {
            second_other = g_effect_trajectory_tile_flags;
        }
        if (second_hit != 0) {
            height = arc_a;
            result = target_id;
        } else {
            stats = battle_unit_get_stats_from_battle_id(source_id);
            if (first_unit != -1
                && (other = battle_unit_get_stats_from_battle_id(first_unit),
                    (stats->team_flags & BATTLE_TEAM_MASK) != (other->initial_team_flags & BATTLE_TEAM_MASK))) {
                g_effect_trajectory_step = first_step;
                g_effect_trajectory_position = first_position;
                height = arc_b;
                g_effect_trajectory_hit_unit_id = first_unit;
                result = first_unit;
                g_effect_trajectory_step_count = first_count;
            } else if (second_unit != -1
                && (other = battle_unit_get_stats_from_battle_id(second_unit),
                    (stats->team_flags & BATTLE_TEAM_MASK) != (other->initial_team_flags & BATTLE_TEAM_MASK))) {
                height = arc_a;
                g_effect_trajectory_step_count = second_count;
                g_effect_trajectory_hit_unit_id = second_unit;
                result = second_unit;
            } else if (second_unit == -1) {
                height = arc_a;
                g_effect_trajectory_step_count = second_count;
                g_effect_trajectory_hit_unit_id = second_unit;
                g_effect_trajectory_tile_flags = second_other;
                result = -1;
            } else if (first_unit != -1) {
                height = arc_a;
                g_effect_trajectory_step_count = second_count;
                g_effect_trajectory_tile_flags = second_other;
                g_effect_trajectory_hit_unit_id = second_unit;
                result = second_unit;
            } else {
                g_effect_trajectory_step = first_step;
                g_effect_trajectory_position = first_position;
                height = arc_b;
                g_effect_trajectory_hit_unit_id = first_unit;
                result = -1;
                g_effect_trajectory_tile_flags = first_other;
                g_effect_trajectory_step_count = first_count;
            }
        }
    }
    g_effect_arc_trajectory_height = height;
    return result;
}
