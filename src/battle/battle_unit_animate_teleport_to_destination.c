#include "fft/battle.h"
#include "psx/types.h"

/* Distortion 0xe (BATTLE_DISTORTION_ANIMATION_TELEPORT): the teleport move, one phase step per frame with
 * distortion_timer counting frames up. It starts the departure effect
 * (battle_effect_start_teleport_departure), brightens the unit (for Altima once her effect and the
 * camera finish), darkens it after 17 frames, and once g_battle_state_animation_continue_check is
 * clear places it on its movement destination and starts the arrival effect; it then darkens it,
 * brightens it after 17 frames, restores its palette after 33, and ends when the check clears again.
 *
 * Jump table: .rodata 0x80068208, 8 entries.
 *
 * The return type is s32 with no value ever returned: keeping $v0 live at the
 * epilogue is what leaves the switch range check's delay slot empty, matching
 * the target.  Nothing reads the result: the distortion dispatcher calls its
 * handlers as void. */
s32 battle_unit_animate_teleport_to_destination(battle_unit_t* unit) {
    unit->distortion_timer++;
    switch (unit->distortion_phase) {
    case 0:
        if (battle_effect_start_teleport_departure(unit) == 0) {
            battle_effect_start_teleport_white_flash(unit);
        }
        unit->distortion_phase++;
        break;
    case 1:
        /* The target passes no argument here. */
        if (battle_effect_update_file_load() == 0 && battle_camera_is_active() == 0) {
            battle_effect_start_teleport_white_flash(unit);
        }
        break;
    case 2:
        if (unit->distortion_timer >= 0x11) {
            battle_gfx_apply_unit_palette_modulation_to_units(MAP_COLOR_MODE_BASE, 4, unit->runtime_id, -31, -31, -31);
            unit->distortion_timer = 0;
            unit->distortion_phase++;
        }
        break;
    case 3:
        if (g_battle_state_animation_continue_check == 0) {
            unit->map_x = unit->movement.destination_x;
            unit->map_y = unit->movement.destination_y;
            unit->map_z = unit->movement.destination_z;
            battle_unit_set_real_coords_from_map_coords(unit);
            battle_unit_set_world_coords_from_real_coords(unit);
            if (battle_effect_start_teleport_arrival(unit) == 0) {
                battle_effect_start_teleport_fade_out(unit);
            }
            unit->distortion_phase++;
        }
        break;
    case 4:
        if (battle_effect_update_file_load() == 0 && battle_camera_is_active() == 0) {
            battle_effect_start_teleport_fade_out(unit);
        }
        break;
    case 5:
        if (unit->distortion_timer >= 0x11) {
            battle_gfx_apply_unit_palette_modulation_to_units(MAP_COLOR_MODE_BASE, 4, unit->runtime_id, 31, 31, 31);
            unit->distortion_phase++;
        }
        break;
    case 6:
        if (unit->distortion_timer >= 0x21) {
            battle_gfx_apply_unit_palette_modulation_to_units(MAP_COLOR_MODE_RESTORE, 2, unit->runtime_id, 0, 0, 0);
            unit->distortion_timer = 0;
            unit->distortion_phase++;
        }
        break;
    case 7:
        if (g_battle_state_animation_continue_check == 0) {
            unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
            unit->sprite_display_flags &= ~BATTLE_SPRITE_DISPLAY_SEMI_TRANSPARENT;
        }
        break;
    }
}
