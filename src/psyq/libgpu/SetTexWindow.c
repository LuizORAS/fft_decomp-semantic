/* LIBGPU 800253a0-800253dc. */
#include "psx/libgpu.h"

void SetTexWindow(void* packet, RECT* window) {
    ((P_TAG*)packet)->len = PSYQ_GPU_PACKET_WORDS(DR_MODE);
    ((DR_MODE*)packet)->code[0] = get_tw(window);
    ((DR_MODE*)packet)->code[1] = 0;
}
