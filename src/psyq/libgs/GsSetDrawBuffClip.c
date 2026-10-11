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
    setRECT(&GsDRAWENV.clip, x, y, CLIP2.w, CLIP2.h);
    PutDrawEnv(&GsDRAWENV);
}
