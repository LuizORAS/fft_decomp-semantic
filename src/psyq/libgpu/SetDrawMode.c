/* LIBGPU 800254cc-80025524. */
#include "psx/libgpu.h"

DR_MODE* SetDrawMode(DR_MODE* packet, int dfe, int dtd, int tpage, RECT* window) {
    ((P_TAG*)packet)->len = PSYQ_GPU_PACKET_WORDS(DR_MODE);
    packet->code[0] = get_mode(dfe, dtd, (u16)tpage);
    packet->code[1] = get_tw(window);
    return (DR_MODE*)packet->code[1];
}
