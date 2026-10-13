#include "fft/battle.h"
#include "psx/types.h"

/*
 * Distortion animation 0x11: launch a unit upward during a jump landing.
 *
 * Like move_animate_jump_start, phase 0 picks a rise speed of
 * sqrt(2 * gravity * (0x120 - screen z)) and a frame count, and phase 1
 * applies it to the height, decelerating by g_move_jump_gravity. Here phase 0 also
 * plays sound 0x27 and ends the animation at once when the unit is already
 * high enough; the end of phase 1 only clears the animation id.
 */
void move_animate_jump_rise_with_sfx(battle_unit_t* unit) {
    VECTOR unused_10;
    VECTOR velocity;
    s32 timer;
    s32 height;
    s32 speed;

    timer = unit->distortion_timer;
    velocity = unit->velocity;
    switch (unit->distortion_phase) {
    case 0:
        height = -battle_gfx_calculate_screen_z_from_misc_screen_data(unit) + 0x120;
        unit->status_flags_5_6 |= BATTLE_UNIT_STATUS_JUMP_HEIGHT_ACTIVE;
        if (-unit->screen.vy >= height) {
            unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
            break;
        }
        speed = SquareRoot12(height * g_move_jump_gravity * 2);
        timer = speed / g_move_jump_gravity;
        velocity.vy = -speed;
        battle_gfx_init_position_vector_copies(unit);
        sound_play_movement_sfx(unit, MAIN_SFX_JUMP);
        unit->distortion_phase++;
    case 1:
        if (timer > 0) {
            unit->real.vy += velocity.vy;
            velocity.vy += g_move_jump_gravity;
        } else {
            unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
        }
        timer--;
        break;
    }
    unit->velocity = velocity;
    battle_unit_set_screen_coords_from_real_coords(unit);
    unit->distortion_timer = timer;
}
