#include "fft/wldcore.h"
#include "psx/types.h"

/* Dispatch a list of display objects to one of two handlers by object kind.
 * Objects with flags 0x18 are skipped, and the whole pass is suppressed while
 * system flag 0x400 is set. */
void wldcore_dispatch_display_object_list(GsOT* ot, s32** list, s32 count) {
    s32* object;
    s32 i;

    if (g_main_system_flags & MAIN_SYSTEM_FLAG_HIDE_DISPLAY_OBJECTS) {
        return;
    }
    for (i = 0; i < count; i++) {
        object = list[i];
        if (*object & (WLDCORE_DISPLAY_OBJECT_BLANK | WLDCORE_DISPLAY_OBJECT_HIDDEN)) {
            continue;
        }
        if (*object & WLDCORE_DISPLAY_OBJECT_ANIMATED) {
            wldcore_gfx_draw_animated_display_object((world_animated_render_record_t*)object, ot);
        } else if (*object & WLDCORE_DISPLAY_OBJECT_IMAGE) {
            wldcore_gfx_draw_display_object_sprite((wldcore_window_render_record_t*)object, ot);
        }
    }
}
