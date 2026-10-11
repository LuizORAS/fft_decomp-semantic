#include "psx/libgpu.h"
#include "psx/libgs.h"

/* Copy the clip rectangle that the draw environment setup reads. */
void GsSetClip2D(RECT* rectp) {
    CLIP2.x = rectp->x;
    CLIP2.y = rectp->y;
    CLIP2.w = rectp->w;
    CLIP2.h = rectp->h;
}
