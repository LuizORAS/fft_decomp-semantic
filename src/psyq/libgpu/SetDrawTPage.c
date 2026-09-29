/* LIBGPU 80023ebc-80023f70. */
#include "psx/libgpu.h"

void SetDrawTPage(void* primitive, int dfe, int dtd, int tpage) {
    void* packet = primitive;
    int draw_enable = dfe;
    int dithering = dtd;
    int page = tpage;
    /* Fixed registers retain the original callee-save order across type queries. */
    u32 command;
    register u32 flags __asm__("$2");         /* The original returns each branch result through v0. */
    register u32 other_command __asm__("$7"); /* The standard branch uses a3 for the command. */
    __asm__(""
        : "=r"(packet), "=r"(draw_enable), "=r"(dithering), "=r"(page)
        : "0"(packet), "1"(draw_enable), "2"(dithering),
        "3"(page)); /* Keep argument copies ahead of the tag constant. */
    ((P_TAG*)packet)->len = PSYQ_GPU_PACKET_WORDS(DR_TPAGE);
    if (GetGraphType() == 1 || GetGraphType() == 2) {
        command = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MODE);
        if (dithering)
            command |= 0x800;
        flags = page & 0x27ff;
        if (draw_enable)
            flags |= 0x1000;
        ((DR_TPAGE*)packet)->code[0] = command | flags;
    } else {
        other_command = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MODE);
        if (dithering)
            other_command |= 0x200;
        flags = page & 0x9ff;
        if (draw_enable)
            flags |= 0x400;
        ((DR_TPAGE*)packet)->code[0] = other_command | flags;
    }
}
