#include "psx/libgpu.h"
#include "psx/libgs.h"
#include "psx/types.h"

/* Program the active draw buffer's clip rectangle: the shared clip rectangle
 * shifted by that buffer's offsets. */
void GsSetDrawBuffClip(void) {
    s32 x;
    s32 y;

    x = CLIP2.x + PSDBASEX[PSDIDX];
    y = CLIP2.y + PSDBASEY[PSDIDX];
    GsDRAWENV.clip.x = x;
    GsDRAWENV.clip.y = y;
    GsDRAWENV.clip.w = CLIP2.w;
    GsDRAWENV.clip.h = CLIP2.h;
    PutDrawEnv(&GsDRAWENV);
}
