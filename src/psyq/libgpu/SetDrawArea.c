/* LIBGPU 800253dc-80025460. */
#include "psx/libgpu.h"

DR_AREA* SetDrawArea(DR_AREA* packet, RECT* rect) {
    ((P_TAG*)packet)->len = PSYQ_GPU_PACKET_WORDS(DR_AREA);
    packet->code[0] = get_cs(rect->x, rect->y);
    packet->code[1] = get_ce((s16)(rect->x + rect->w - 1), (s16)(rect->y + rect->h - 1));
    /* The current public declaration exposes the incidental final v0 value. */
    return (DR_AREA*)packet->code[1];
}
