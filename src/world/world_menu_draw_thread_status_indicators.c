#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Build and submit the two translucent letterbox panels of the event dialogue
 * overlay.
 *
 * Banner tasks in slots 8 and 7 request a panel when display->x is zero;
 * display->y selects which of the two opens. Each panel fades its
 * gradient in by 0xc per frame up to 0x78 and back out again, alternating
 * between two primitive buffers so the previous frame stays queued. The
 * EQUIP twin watches threads 13 and 14.
 */
void world_menu_draw_thread_status_indicators(void) {
    battle_menu_status_panel_indicator_prims_t* prims;
    world_status_thread_t* display;
    world_status_thread_t* descriptor;
    s32 active;
    s32 i;
    s32 j;
    s32 k;
    s32 flip;
    s16 y;

    active = 0;
    if (world_thread_is_running_by_id(NATIVE_SCREEN_THREAD_SLOT_UNIT_STATUS) != 0) {
        if (g_world_threads[NATIVE_SCREEN_THREAD_SLOT_UNIT_STATUS].task_id == NATIVE_THREAD_TASK_UNIT_STATUS_BANNER) {
            display
                = (world_status_thread_t*)g_world_threads[NATIVE_SCREEN_THREAD_SLOT_UNIT_STATUS].function_parameter_1;
            active = (display->x == 0) * 8;
        }
    }
    if (world_thread_is_running_by_id(NATIVE_SCREEN_THREAD_SLOT_COMPARISON_STATUS) != 0) {
        if (g_world_threads[NATIVE_SCREEN_THREAD_SLOT_COMPARISON_STATUS].task_id
            == NATIVE_THREAD_TASK_UNIT_STATUS_BANNER) {
            descriptor = (world_status_thread_t*)g_world_threads[NATIVE_SCREEN_THREAD_SLOT_COMPARISON_STATUS]
                             .function_parameter_1;
            if (descriptor->x == 0) {
                active = 7;
                display = descriptor;
            }
        }
    }
    if (active != 0) {
        if ((display->y == 0x90) && !(display->flags & MENU_STATUS_PANEL_HIDDEN)) {
            g_world_menu_indicator_state[0] = MENU_INDICATOR_STATE_FADING_OUT;
            g_world_menu_indicator_state[1] = MENU_INDICATOR_STATE_FADING_IN;
            goto draw_panels;
        } else if ((display->y == 0) && !(display->flags & MENU_STATUS_PANEL_HIDDEN)) {
            g_world_menu_indicator_state[0] = MENU_INDICATOR_STATE_FADING_IN;
            g_world_menu_indicator_state[1] = MENU_INDICATOR_STATE_FADING_OUT;
            goto draw_panels;
        }
    }
    if (g_world_menu_indicator_state[0] == MENU_INDICATOR_STATE_FADING_IN) {
        g_world_menu_indicator_state[0] = MENU_INDICATOR_STATE_FADING_OUT;
    }
    if (g_world_menu_indicator_state[1] == MENU_INDICATOR_STATE_FADING_IN) {
        g_world_menu_indicator_state[1] = MENU_INDICATOR_STATE_FADING_OUT;
    }
draw_panels:
    for (i = 0; i < 2; i++) {
        flip = g_world_menu_indicator_packet_index[i] ^ 1;
        g_world_menu_indicator_packet_index[i] = flip;
        prims = &g_world_thread_indicator_packets[i][flip];
        SetDrawMode(&prims->mode, 0, 0, GetTPage(0, 2, 0x3c0, 0x100) & 0xffff, &g_world_gfx_draw_area_template);
        SetTile(&prims->frame);
        setRGB0(&prims->frame, 0, 0, 0);
        SetSemiTrans(&prims->frame, 1);
        y = 0xb2;
        if (i == 0) {
            y = 0x23;
        }
        setXY0(&prims->frame, 0x80, y);
        setWH(&prims->frame, 0x100, 0x32);
        for (j = 0; j < 9; j++) {
            SetTile(&prims->top[j]);
            setRGB0(&prims->top[j], 0, 0, 0);
            SetSemiTrans(&prims->top[j], 1);
            y = 0xa9;
            setWH(&prims->top[j], 0x100, 1);
            if (i == 0) {
                y = 0x1a;
            }
            setXY0(&prims->top[j], 0x80, y + j);
            SetTile(&prims->bottom[j]);
            setRGB0(&prims->bottom[j], 0, 0, 0);
            SetSemiTrans(&prims->bottom[j], 1);
            y = 0xe4;
            setWH(&prims->bottom[j], 0x100, 1);
            if (i == 0) {
                y = 0x55;
            }
            setXY0(&prims->bottom[j], 0x80, y + j);
        }
        if (g_world_menu_indicator_state[i] == MENU_INDICATOR_STATE_HIDDEN) {
            setRGB0(&prims->frame, 0, 0, 0);
        } else {
            if (g_world_menu_indicator_state[i] == MENU_INDICATOR_STATE_FADING_IN) {
                if (g_world_menu_indicator_brightness[i] < 0x78) {
                    g_world_menu_indicator_brightness[i] += 0xc;
                }
            }
            if (g_world_menu_indicator_state[i] == MENU_INDICATOR_STATE_FADING_OUT) {
                if (g_world_menu_indicator_brightness[i] != 0) {
                    g_world_menu_indicator_brightness[i] -= 0xc;
                } else {
                    g_world_menu_indicator_state[i] = MENU_INDICATOR_STATE_HIDDEN;
                }
            }
            setRGB0(&prims->frame, g_world_menu_indicator_brightness[i], g_world_menu_indicator_brightness[i],
                g_world_menu_indicator_brightness[i]);
            /* fade_work holds the edge brightness that steps down across the gradient. */
            g_world_menu_indicator_fade_work[i] = g_world_menu_indicator_brightness[i];
            for (j = 0; j < 9; j++) {
                if (g_world_menu_indicator_fade_work[i] != 0) {
                    g_world_menu_indicator_fade_work[i] -= 0xc;
                }
                k = 8 - j;
                setRGB0((k + prims->top), g_world_menu_indicator_fade_work[i], g_world_menu_indicator_fade_work[i],
                    g_world_menu_indicator_fade_work[i]);
                setRGB0(&prims->bottom[j], g_world_menu_indicator_fade_work[i], g_world_menu_indicator_fade_work[i],
                    g_world_menu_indicator_fade_work[i]);
                world_gfx_draw_or_append_gpu_primitive((k + prims->top));
                world_gfx_draw_or_append_gpu_primitive(&prims->bottom[j]);
            }
            world_gfx_draw_or_append_gpu_primitive(&prims->frame);
            world_gfx_draw_or_append_gpu_primitive(&prims->mode);
        }
    }
}
