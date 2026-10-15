/* LIBGPU 800254cc-80025524. */
#include "psx/libgpu.h"

/* Build a draw-mode packet: texture page `tpage`, dithering `dtd`, drawing to the displayed area `dfe` and the
 * texture window `window`. The return value is the second command word. About 60 functions use it. */
DR_MODE* SetDrawMode(DR_MODE* packet, int dfe, int dtd, int tpage, RECT* window) {
    ((P_TAG*)packet)->len = PSYQ_GPU_PACKET_WORDS(DR_MODE);
    packet->code[0] = get_mode(dfe, dtd, (u16)tpage);
    packet->code[1] = get_tw(window);
    return (DR_MODE*)packet->code[1];
}
