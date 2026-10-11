#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Set the sprite CLUT from the command's first coordinate triple (bytes 2-4)
 * in palette mode 0, or its second (bytes 5-7) in the alternate palette
 * mode, and return the next command. */
u8* jobstts_cmd_set_layout_clut_handler(u8* cmd) {
    if (g_jobstts_menu_use_alternate_palette != 0) {
        g_jobstts_gfx_clut_id = GetClut(cmd[6] << 4, cmd[7] | (cmd[5] << 8));
    } else {
        g_jobstts_gfx_clut_id = GetClut(cmd[3] << 4, cmd[4] | (cmd[2] << 8));
    }
    return cmd + cmd[1];
}
