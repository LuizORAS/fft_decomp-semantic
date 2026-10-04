/* LIBGPU 80025a04-80025a88. */
#include "psx/libgpu.h"
u32 get_tw(RECT* rect) {
    int fields[4];
    register RECT* rectangle __asm__("$4");
    int x;
    register int width __asm__("$6");
    int y;
    int height;
    register u32 command __asm__("$4");
    register u32 result __asm__("$2");
    /* Pins retain the original input and independently shifted command fields. */
    if (!rect)
        result = 0;
    else {
        rectangle = rect;
        x = ((u8)rectangle->x) >> 3;
        fields[0] = x;
        width = -rectangle->w & 0xff;
        __asm__("" : "=r"(width) : "0"(width)); /* Preserve the original signed shift after masking. */
        width >>= 3;
        fields[2] = width;
        y = ((u8)rectangle->y) >> 3;
        fields[1] = y;
        height = -rectangle->h & 0xff;
        height >>= 3;
        fields[3] = height;
        x <<= 10;
        y <<= 15;
        command = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_TEXTURE_WINDOW);
        x |= command;
        result = y | x;
        result |= height << 5;
        result |= width;
    }
    return result;
}
