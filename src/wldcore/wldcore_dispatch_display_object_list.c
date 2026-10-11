#include "fft/wldcore.h"
#include "psx/types.h"

/* Dispatch a list of display objects to one of two handlers by object kind.
 * Objects with flags 0x18 are skipped, and the whole pass is suppressed while
 * system flag 0x400 is set. */
void wldcore_dispatch_display_object_list(GsOT* ot, void** list, s32 count) {
    void* object;
    s32* flags;
    s32 i;

    if (g_main_system_flags & MAIN_SYSTEM_FLAG_HIDE_DISPLAY_OBJECTS) {
        return;
    }
    for (i = 0; i < count; i++) {
        object = list[i];
        flags = object;
        if (*flags & (WLDCORE_DISPLAY_OBJECT_BLANK | WLDCORE_DISPLAY_OBJECT_HIDDEN)) {
            continue;
        }
        if (*flags & WLDCORE_DISPLAY_OBJECT_ANIMATED) {
            world_animated_render_record_t* animated = object;
            wldcore_gfx_draw_animated_display_object(animated, ot);
        } else if (*flags & WLDCORE_DISPLAY_OBJECT_IMAGE) {
            wldcore_window_render_record_t* image = object;
            wldcore_gfx_draw_display_object_sprite(image, ot);
        }
    }
}
