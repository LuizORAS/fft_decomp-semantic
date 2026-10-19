#include "fft/battle.h"
#include "psx/types.h"

/* Distortion 0x11 (BATTLE_DISTORTION_ANIMATION_JUMP_RISE): the rise of battle_move_animate_jump_start
 * with sound 0x27: a rise speed of sqrt(2 * gravity * the height left to 0x120 above the ground), slowed
 * by g_battle_move_jump_gravity each frame. It sets the jump-height status and ends at once when the
 * unit is already 0x120 above the ground; otherwise it rises, and the end only clears the id (no shadow
 * or animation change). */
void battle_move_animate_jump_rise_with_sfx(battle_unit_t* unit) {
    VECTOR unused_10;
    VECTOR velocity;
    s32 timer;
    s32 height;
    s32 speed;

    timer = unit->distortion_timer;
    velocity = unit->velocity;
    switch (unit->distortion_phase) {
    case 0:
        height = -battle_gfx_calculate_world_height_from_world_coords(unit) + 0x120;
        unit->status_flags_5_6 |= BATTLE_UNIT_STATUS_JUMP_HEIGHT_ACTIVE;
        if (-unit->world_coords.vy >= height) {
            unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
            break;
        }
        speed = SquareRoot12(height * g_battle_move_jump_gravity * 2);
        timer = speed / g_battle_move_jump_gravity;
        velocity.vy = -speed;
        battle_gfx_init_position_vector_copies(unit);
        battle_sound_play_movement_sfx(unit, MAIN_SFX_JUMP);
        unit->distortion_phase++;
    case 1:
        if (timer > 0) {
            unit->real.vy += velocity.vy;
            velocity.vy += g_battle_move_jump_gravity;
        } else {
            unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
        }
        timer--;
        break;
    }
    unit->velocity = velocity;
    battle_unit_set_world_coords_from_real_coords(unit);
    unit->distortion_timer = timer;
}
