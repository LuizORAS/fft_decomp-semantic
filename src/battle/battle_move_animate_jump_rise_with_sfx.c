#include "fft/battle.h"
#include "psx/types.h"

/* Distortion 0x11 (BATTLE_DISTORTION_JUMP_UP_WITH_SOUND): the rise of battle_move_animate_jump_start
 * with sound 0x27. It sets the jump-height status and ends at once when the unit is already 0x120
 * above the ground; otherwise it rises, and the end only clears the id (no shadow or animation
 * change). */
void battle_move_animate_jump_rise_with_sfx(battle_unit_misc_data_t* unit) {
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
        unit->status_flags_5_6 |= BATTLE_MISC_STATUS_JUMP_HEIGHT_ACTIVE;
        if (-unit->screen.vy >= height) {
            unit->distortion_animation_id = 0;
            break;
        }
        speed = SquareRoot12(height * g_battle_move_jump_gravity * 2);
        timer = speed / g_battle_move_jump_gravity;
        velocity.vy = -speed;
        battle_gfx_init_position_vector_copies(unit);
        battle_sound_play_movement_sfx(unit, 0x27);
        unit->distortion_phase++;
    case 1:
        if (timer > 0) {
            unit->real.vy += velocity.vy;
            velocity.vy += g_battle_move_jump_gravity;
        } else {
            unit->distortion_animation_id = 0;
        }
        timer--;
        break;
    }
    unit->velocity = velocity;
    battle_unit_set_screen_coords_from_real_coords(unit);
    unit->distortion_timer = timer;
}
