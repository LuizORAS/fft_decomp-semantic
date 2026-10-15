/* LIBGPU 80023ebc-80023f70. */
#include "psx/libgpu.h"

/* Build a one-word draw-mode packet (texture page, dithering, drawing to the displayed area, no texture
 * window). No code refers to it. */
void SetDrawTPage(DR_TPAGE* p, int dfe, int dtd, int tpage) {
    setlen(p, 1);
    p->code[0] = _get_mode(dfe, dtd, tpage);
}
