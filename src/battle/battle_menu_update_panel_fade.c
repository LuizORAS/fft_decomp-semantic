#include "fft/battle_gfx.h"
#include "fft/battle_menu.h"
#include "fft/menu.h"

/*
 * Update the menu panel's fade and submit its center and edge tiles.
 *
 * Mode 1 raises intensity toward 80; mode 2 lowers it toward 0. The nine
 * tile pairs taper away from the center in increments of 8.
 * BATTLE twin of world_menu_update_panel_fade.
 */
void battle_menu_update_panel_fade(void) {
    menu_gradient_panel_prims_t* frame;
    s32 i;
    s32 intensity;

    if (g_battle_menu_panel_fade_mode != MENU_PANEL_FADE_NONE) {
        frame = &g_battle_menu_panel_fade_frames[g_menu_packet_buffer_index];
        if (g_battle_menu_panel_fade_mode == MENU_PANEL_FADE_IN && g_battle_menu_panel_fade_intensity < 80) {
            g_battle_menu_panel_fade_intensity += 8;
        }
        if (g_battle_menu_panel_fade_mode == MENU_PANEL_FADE_OUT) {
            if (g_battle_menu_panel_fade_intensity != 0) {
                g_battle_menu_panel_fade_intensity -= 8;
            } else {
                g_battle_menu_panel_fade_mode = MENU_PANEL_FADE_NONE;
            }
        }
        setRGB0(&frame->center, g_battle_menu_panel_fade_intensity, g_battle_menu_panel_fade_intensity,
            g_battle_menu_panel_fade_intensity);
        intensity = g_battle_menu_panel_fade_intensity;
        for (i = 0; i < 9; i++) {
            if (intensity != 0) {
                intensity -= 8;
            }
            /* The pointer walk preserves the target's operand order for the biased index. */
            (frame->top + (8 - i))->r0 = intensity;
            (frame->top + (8 - i))->g0 = intensity;
            (frame->top + (8 - i))->b0 = intensity;
            setRGB0(&frame->bottom[i], intensity, intensity, intensity);
            battle_gfx_draw_or_append_gpu_primitive(&frame->top[8 - i]);
            battle_gfx_draw_or_append_gpu_primitive(&frame->bottom[i]);
        }
        battle_gfx_draw_or_append_gpu_primitive(&g_battle_menu_panel_fade_frames[g_menu_packet_buffer_index].center);
        battle_gfx_draw_or_append_gpu_primitive(&g_battle_menu_panel_fade_frames[g_menu_packet_buffer_index].draw_mode);
    }
}
