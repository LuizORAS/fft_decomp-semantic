#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* This function's target access is a signed halfword load of the shared u16
 * cell. Keep a local linker alias instead of changing the global declaration. */
extern s16 g_battle_screen_fade_intensity_signed;

/* EVENT state: animate the units and camera (unless the status screen is up), run one frame of
 * the event script and draw the screen fade. When the script ends the battle resumes; a
 * scenario finish closes the battle with the next game flow: 9 returns to the world map (1),
 * 0xA sets up the next battle (0), 0xB resets the game (2) and 0x13 plays the ending (3).
 *
 * The target loads the shared u16 fade cell with `lh`; the local signed view
 * preserves that use-specific access. Keeping the loaded intensity and the
 * clamped display level as separate s16 values retains the target's two moves
 * before its unsigned 0x100 comparison. The otherwise-unused 24-byte local
 * preserves the original 64-byte frame. */
void battle_state_handle_event_state(void) {
    s16 intensity;
    s16 level;
    char unused[24];

    battle_gfx_update_all_unit_rotation_and_vectors();
    if (g_battle_menu_status_screen_selected != 1) {
        battle_unit_update_and_animate_units();
        battle_gfx_update_screen_color_modulation_fade();
        battle_camera_update_real_coord_animation();
        battle_camera_update_offset_screen_coord_animation();
        battle_camera_update_zoom_animation();
        battle_camera_update_rotation_animation();
        battle_move_update_unit_by_misc_id();
    }
    battle_gfx_update_status_bubbles_and_graphics();
    battle_gfx_draw_screen_color_modulation_overlay();
    switch (battle_script_run_event_frame(main_gfx_get_otag(), g_controller_input_raw)) {
    case 0:
        battle_gfx_init_evtchr_vram_slots();
        g_battle_menu_status_enabled = 1;
        battle_ai_init_workspace();
        battle_action_check_between_turn_events();
        break;
    case 9:
        battle_gfx_init_evtchr_vram_slots();
        battle_state_enter_close_battle(8, 1);
        break;
    case 10:
        battle_gfx_init_evtchr_vram_slots();
        battle_state_enter_close_battle(8, 0);
        break;
    case 11:
        battle_gfx_init_evtchr_vram_slots();
        battle_state_enter_close_battle(8, 2);
        break;
    case 19:
        battle_gfx_init_evtchr_vram_slots();
        battle_state_enter_close_battle(8, 3);
        break;
    }

    if (g_battle_screen_fade_intensity_signed != 0) {
        intensity = g_battle_screen_fade_intensity_signed;
        level = intensity;
        if ((u32)level >= 0x100) {
            level = 0xff;
        }
        g_battle_screen_fade_polygons[g_main_gfx_screen_polarity].r0 = level;
        g_battle_screen_fade_polygons[g_main_gfx_screen_polarity].g0 = level;
        g_battle_screen_fade_polygons[g_main_gfx_screen_polarity].b0 = level;

        AddPrim(main_gfx_get_otag(), &g_battle_screen_fade_polygons[g_main_gfx_screen_polarity]);
        AddPrim(main_gfx_get_otag(), (u8*)g_battle_screen_fade_draw_modes + g_main_gfx_screen_polarity * 0xc);
    }
}
