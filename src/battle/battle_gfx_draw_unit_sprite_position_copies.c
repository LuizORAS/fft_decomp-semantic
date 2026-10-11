#include "fft/battle.h"
#include "psx/types.h"

/*
 * Draw the unit's sprite once at each of its six position_copies.
 *
 * Counterpart of battle_gfx_init_position_vector_copies, which seeds the
 * copies; each copy is projected with the battle camera and queued at its own
 * ordering-table depth using the current camera zoom.
 */
void battle_gfx_draw_unit_sprite_position_copies(battle_unit_misc_data_t* unit) {
    SVECTOR position;
    SVECTOR zoom;
    VECTOR transformed;
    long flag;
    battle_gfx_sprite_display_data_t* display;
    s32 depth;
    s32 i;
    s16 flags;

    for (i = 0; i < 6; i++) {
        battle_camera_update_matrices(&g_battle_camera_matrix, &g_battle_camera_render_state,
            &g_battle_offset_screen_coords, &g_battle_camera_zoom);
        SetRotMatrix(&g_battle_camera_matrix);
        SetTransMatrix(&g_battle_camera_matrix);
        RotTrans(&unit->position_copies[i], &transformed, &flag);
        depth = transformed.vz / 4;
        position.vx = transformed.vx;
        position.vy = transformed.vy;
        flags = ((unit->horizontal_flip_flag ^ unit->sprite_display_flags) & ~BATTLE_MISC_DISPLAY_BLEND_MASK)
            | BATTLE_MISC_DISPLAY_SEMI_TRANSPARENT;
        copyVector(&zoom, &g_battle_camera_zoom);
        display = unit->animation.display;
        battle_gfx_construct_polygon_data_for_units(
            display, display->part_count, 0, &position, 0, flags, &zoom, main_gfx_get_otag() + depth);
    }
}
