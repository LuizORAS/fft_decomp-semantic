/* LIBGPU 800249c4-80024a88. */
#include "psx/libgpu.h"

s32 MoveImage(RECT* rect, s32 x, s32 y) {
    u32 destination;
    /* The packet base and source-coordinate words retain a1/v1/a0 scheduling. */
    register u32* body __asm__("$5");
    psyq_gpu_dispatch_t* dispatch;
    int copy_size;
    u32 argument;
    register u32 low __asm__("$3");
    register u32 source __asm__("$4");
    psyq_gpu_operation_t operation;
    int (*enqueue)(psyq_gpu_operation_t, void*, int, u32);
    checkRECT(g_psyq_gpu_move_image_name, rect);
    if (!rect->w || !rect->h)
        return -1;
    destination = (u32)y << 16;
    low = x & 0xffff;
    destination |= low;
    body = g_psyq_gpu_move_copy_words;
    source = ((u32*)rect)[0];
    /* The retail keeps the rectangle load ahead of its three packet stores. */
    __asm__("" : "=r"(destination), "=r"(source) : "0"(destination), "1"(source));
    dispatch = g_psyq_gpu_dispatch;
    __asm__("" : "=r"(dispatch) : "0"(dispatch) : "$6");
    copy_size = 20;
    __asm__ volatile(""
        : "=r"(destination), "=r"(source), "=r"(dispatch), "=r"(copy_size)
        : "0"(destination), "1"(source), "2"(dispatch), "3"(copy_size)
        : "memory");
    g_psyq_gpu_move_destination = destination;
    body[0] = source;
    destination = ((u32*)rect)[1];
    __asm__("" : "=r"(destination) : "0"(destination) : "$7");
    argument = 0;
    __asm__ volatile("" : "=r"(destination), "=r"(argument) : "0"(destination), "1"(argument) : "memory");
    g_psyq_gpu_move_dimensions = destination;
    __asm__ volatile("" : : : "memory");
    operation = dispatch->ordering_table;
    __asm__("" : "=r"(operation) : "0"(operation) : "$2");
    enqueue = dispatch->enqueue_four;
    return enqueue(operation, body - 2, copy_size, argument);
}
