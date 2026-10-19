/* LIBGPU 80023ebc-80023f70. */
#include "psx/libgpu.h"

/* Build a draw-mode packet of one command word (texture page, dithering, drawing to the displayed area, no texture
 * window). No code refers to it. */
void SetDrawTPage(DR_TPAGE* p, int dfe, int dtd, int tpage) {
    setlen(p, PSYQ_GPU_PACKET_WORDS(DR_TPAGE));
    p->code[0] = _get_mode(dfe, dtd, tpage);
}
