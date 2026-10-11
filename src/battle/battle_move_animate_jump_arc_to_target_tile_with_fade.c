#include "fft/battle.h"
#include "psx/types.h"

/* Distortion animation 0x10: jump along an arc to the centre of the unit's own
 * map tile, darkened for the first part of the flight.
 *
 * Phase 0 computes the arc as battle_move_animate_jump_arc_to_own_tile does,
 * hides the shadow, selects quarter-add blending and starts
 * a palette modulation of -31 on each colour component. Phase 1 flies until 4
 * frames remain (the threshold kept in distortion_target), then restores the
 * palette. Phase 2 flies the remaining frames and stops the horizontal motion.
 * Phase 3 falls to the ground, snaps the unit to the tile, shows the shadow,
 * clears BATTLE_MISC_STATUS_JUMP_HEIGHT_ACTIVE and the semi-transparency bit, and
 * ends the animation. */
void battle_move_animate_jump_arc_to_target_tile_with_fade(battle_unit_misc_data_t* unit) {
    /* Unreferenced 8-byte frame slot below the target point, like the one in
     * battle_move_animate_jump_arc_to_own_tile; it puts `target` at sp+0x20. */
    SVECTOR unused_18;
    SVECTOR target;
    VECTOR velocity;
    s32 timer;
    s32 threshold;

    timer = unit->distortion_timer;
    threshold = unit->distortion_target;
    velocity = unit->velocity;
    switch (unit->distortion_phase) {
    case 0:
        target.vx = unit->map_x * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
        target.vz = unit->map_y * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
        target.vy = battle_gfx_calculate_screen_z_from_misc_map_data(unit);
        timer = battle_move_calculate_jump_arc_velocity(&unit->screen, &target, &velocity);
        unit->shadow_graphic_trigger = 0;
        unit->sprite_display_flags |= BATTLE_MISC_DISPLAY_BLEND_ADD_QUARTER | BATTLE_MISC_DISPLAY_SEMI_TRANSPARENT;
        battle_gfx_start_misc_unit_palette_modulation(MAP_COLOR_MODE_BASE, 1, unit->unit_id, -31, -31, -31);
        threshold = 4;
        unit->distortion_phase++;
    case 1:
        if (timer > threshold) {
            unit->real.vx += velocity.vx;
            unit->real.vy += velocity.vy;
            unit->real.vz += velocity.vz;
            velocity.vy += g_battle_move_jump_gravity;
        } else {
            battle_gfx_start_misc_unit_palette_modulation(MAP_COLOR_MODE_RESTORE, 1, unit->unit_id, 0, 0, 0);
            unit->distortion_phase++;
        }
        timer--;
        break;
    case 2:
        if (timer > 0) {
            unit->real.vx += velocity.vx;
            unit->real.vy += velocity.vy;
            unit->real.vz += velocity.vz;
            velocity.vy += g_battle_move_jump_gravity;
        } else {
            velocity.vz = 0;
            velocity.vx = 0;
            unit->distortion_phase++;
        }
        timer--;
        break;
    case 3:
        main_util_set_svector(&target, unit->map_x * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2), 0,
            unit->map_y * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2));
        target.vy = battle_gfx_calculate_screen_z_from_misc_screen_data(unit);
        if (unit->screen.vy < target.vy) {
            unit->real.vy += velocity.vy;
            velocity.vy += g_battle_move_jump_gravity;
        } else {
            battle_unit_set_real_coords_from_map_coords(unit);
            unit->shadow_graphic_trigger = 1;
            unit->status_flags_5_6 &= ~BATTLE_MISC_STATUS_JUMP_HEIGHT_ACTIVE;
            unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
            unit->sprite_display_flags &= ~BATTLE_MISC_DISPLAY_SEMI_TRANSPARENT;
        }
        break;
    }
    unit->velocity = velocity;
    battle_unit_set_screen_coords_from_real_coords(unit);
    unit->distortion_timer = timer;
    unit->distortion_target = threshold;
}
