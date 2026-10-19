#include "fft/wldcore.h"
#include "psx/types.h"

/* Each queued record starts with the shared 32-bit display flags. */
static inline s32* wldcore_display_object_flags(void* object) {
    return object;
}

/* Show (nonzero `clear`) or blank every display object of the auxiliary and main render-object
 * queues by clearing or setting WLDCORE_DISPLAY_OBJECT_BLANK; each object's own HIDDEN flag stays. */
void wldcore_window_set_render_objects_visible(s32 clear) {
    s32 i;

    if (clear) {
        for (i = 0; i < g_wldcore_window_aux_render_object_count; i++) {
            *wldcore_display_object_flags(g_wldcore_window_aux_render_object_queue[i]) &= ~WLDCORE_DISPLAY_OBJECT_BLANK;
        }
        for (i = 0; i < g_wldcore_window_render_object_count; i++) {
            *wldcore_display_object_flags(g_wldcore_window_render_object_queue[i]) &= ~WLDCORE_DISPLAY_OBJECT_BLANK;
        }
    } else {
        for (i = 0; i < g_wldcore_window_aux_render_object_count; i++) {
            *wldcore_display_object_flags(g_wldcore_window_aux_render_object_queue[i]) |= WLDCORE_DISPLAY_OBJECT_BLANK;
        }
        for (i = 0; i < g_wldcore_window_render_object_count; i++) {
            *wldcore_display_object_flags(g_wldcore_window_render_object_queue[i]) |= WLDCORE_DISPLAY_OBJECT_BLANK;
        }
    }
}
