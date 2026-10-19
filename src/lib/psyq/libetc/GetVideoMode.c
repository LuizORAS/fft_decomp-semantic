#include "psx/libetc.h"

/* Return the video mode that LIBGPU and LIBGS build displays for: 1 is PAL, anything else NTSC. It stays 0,
 * its initial value, because nothing calls SetVideoMode. */
s32 GetVideoMode(void) {
    return g_psyq_etc_video_mode;
}
