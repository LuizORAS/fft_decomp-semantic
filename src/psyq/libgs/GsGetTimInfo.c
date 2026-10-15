#include "psx/libgs.h"
#include "psx/types.h"

/* Read the header of the TIM image at `tim` into `im`: the pixel mode, the CLUT position, size and data when
 * the TIM has one, and the image position, size and pixels. WLDCORE's pictures (locations, sound novels,
 * treasures, reports) are placed this way. */
void GsGetTimInfo(u32* tim, GsIMAGE* im) {
    u32* image;

    im->pmode = *tim;
    if ((im->pmode >> 3) & 1) {
        tim++;
        image = tim + (*tim >> 2);
        tim++;
        im->cx = tim[0];
        im->cy = ((u16*)tim)[1];
        tim++;
        im->cw = tim[0];
        im->ch = ((u16*)tim)[1];
        tim++;
        im->clut = tim;
        image++;
        im->px = image[0];
        im->py = ((u16*)image)[1];
        image++;
        im->pw = image[0];
        im->ph = ((u16*)image)[1];
        image++;
        im->pixel = image;
    } else {
        tim += 2;
        im->px = tim[0];
        im->py = ((u16*)tim)[1];
        tim++;
        im->pw = tim[0];
        im->ph = ((u16*)tim)[1];
        tim++;
        im->pixel = tim;
    }
}
