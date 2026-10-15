#include "psx/libetc.h"

/* Set the video mode GetVideoMode returns and return the old one. No code refers to it, so the game keeps
 * mode 0 (NTSC). */
s32 SetVideoMode(s32 mode) {
    s32 previous = g_psyq_etc_video_mode;
    g_psyq_etc_video_mode = mode;
    return previous;
}
