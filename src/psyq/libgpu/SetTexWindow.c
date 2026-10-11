/* LIBGPU 800253a0-800253dc. */
#include "psx/libgpu.h"

void SetTexWindow(DR_TWIN* packet, RECT* window) {
    ((P_TAG*)packet)->len = PSYQ_GPU_PACKET_WORDS(DR_MODE);
    packet->code[0] = get_tw(window);
    packet->code[1] = 0;
}
