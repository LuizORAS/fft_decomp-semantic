/* LIBGPU 80024e84-80025334. */
#include "psx/libgpu.h"

DISPENV* PutDispEnv(DISPENV* env) {
    u32 mode = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_DISPLAY_MODE);
    int horizontal_start;
    int horizontal_end;
    int vertical_start, vertical_end;
    /* Preserve the retail clamp temporaries and GP1 argument registers. */
    int value;
    register int compare __asm__("$3");
    register int coordinate __asm__("$4");
    register u32 first __asm__("$2");
    u32 second;
    u32 command;
    u16* cached;
    register psyq_gpu_dispatch_t* start_dispatch __asm__("$2");
    psyq_gpu_dispatch_t* timing_dispatch;
    register void (*control)(u32) __asm__("$3");
    u8* debug = &g_psyq_gpu_environment.debug_level;
    /* This address is materialized before the byte load in the retail. */
    __asm__("" : "=r"(debug) : "0"(debug));
    if (*debug >= 2)
        g_psyq_gpu_printf(g_psyq_gpu_put_disp_env_format, env);
    if ((unsigned)(g_psyq_gpu_graph_type - 1) < 2) {
        first = get_dx(&env->disp);
        second = *(u16*)&env->disp.y;
        first &= 0xfff;
        second &= 0xfff;
        second <<= 12;
        second |= first;
        first = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_DISPLAY_START);
    } else {
        first = *(u16*)&env->disp.y;
        second = *(u16*)&env->disp.x;
        first = (first & 0x3ff) << 10;
        second &= 0x3ff;
        first |= second;
        second = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_DISPLAY_START);
    }
    command = first | second;
    start_dispatch = g_psyq_gpu_dispatch;
    start_dispatch->control(command);
    cached = &g_psyq_gpu_cached_screen_x;
    __asm__("" : "=r"(cached) : "0"(cached));
    /* Cached unsigned halfwords retain their separate signed-narrow sequence. */
    value = *cached;
    compare = env->screen.x;
    __asm__("" : "=r"(value), "=r"(compare) : "0"(value), "1"(compare));
    value = (s16)value;
    if (value != compare)
        goto timings_changed;
    value = g_psyq_gpu_cached_screen_y;
    compare = env->screen.y;
    __asm__("" : "=r"(value), "=r"(compare) : "0"(value), "1"(compare));
    value = (s16)value;
    if (value != compare)
        goto timings_changed;
    value = g_psyq_gpu_cached_screen_width;
    compare = env->screen.w;
    __asm__("" : "=r"(value), "=r"(compare) : "0"(value), "1"(compare));
    value = (s16)value;
    if (value != compare)
        goto timings_changed;
    value = g_psyq_gpu_cached_screen_height;
    compare = env->screen.h;
    __asm__("" : "=r"(value), "=r"(compare) : "0"(value), "1"(compare));
    value = (s16)value;
    if (value == compare)
        goto check_mode;
timings_changed:
    /* The reserved byte carries the current video mode in this SDK version. */
    env->pad0 = GetVideoMode();
    compare = env->screen.x;
    value = compare * 10;
    horizontal_start = value + 608;
    value = env->pad0;
    __asm__("" : "=r"(value) : "0"(value) : "$4");
    coordinate = env->screen.y;
    vertical_start = coordinate + 19;
    if (!value)
        vertical_start = coordinate + 16;
    compare = env->screen.w;
    __asm__("" : "=r"(compare) : "0"(compare));
    if (compare) {
        value = compare * 10;
        horizontal_end = horizontal_start + value;
    } else {
        horizontal_end = horizontal_start + 2560;
    }
    vertical_end = vertical_start + env->screen.h;
    if (!env->screen.h)
        vertical_end = vertical_start + 240;
    coordinate = 500;
    if (horizontal_start >= 500) {
        coordinate = horizontal_start;
        if (coordinate > 3290)
            coordinate = 3290;
    }
    horizontal_start = coordinate;
    compare = horizontal_start + 80;
    if (horizontal_end >= compare) {
        compare = horizontal_end;
        if (compare > 3290)
            compare = 3290;
    }
    horizontal_end = compare;
    if (vertical_start < 16)
        goto lower_start;
    if (env->pad0) {
        if (vertical_start >= 311)
            goto upper_start;
        coordinate = vertical_start;
        goto start_clamped;
    }
    if (vertical_start < 257)
        goto keep_start;
upper_start:
    coordinate = 256;
    if (!env->pad0)
        goto start_clamped;
    coordinate = 310;
    goto start_clamped;
keep_start:
    /* Merging this copy with the PAL branch changes the original jumps. */
    __asm__ volatile("" : : : "$4");
    coordinate = vertical_start;
    goto start_clamped;
lower_start:
    coordinate = 16;
start_clamped:
    vertical_start = coordinate;
    compare = vertical_start + 2;
    if (vertical_end < compare)
        goto end_clamped;
    if (env->pad0) {
        if (vertical_end >= 313)
            goto upper_end;
        compare = vertical_end;
        goto end_clamped;
    }
    if (vertical_end < 259)
        goto keep_end;
upper_end:
    compare = 258;
    if (!env->pad0)
        goto end_clamped;
    compare = 312;
    goto end_clamped;
keep_end:
    compare = vertical_end;
end_clamped:
    vertical_end = compare;
    /* Keep timing bits in v0 while the control function is loaded through v1. */
    first = (horizontal_end & 0xfff) << 12;
    command = horizontal_start & 0xfff;
    second = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_HORIZONTAL_RANGE);
    timing_dispatch = g_psyq_gpu_dispatch;
    __asm__("" : "=r"(timing_dispatch) : "0"(timing_dispatch));
    command |= second;
    control = timing_dispatch->control;
    control(first | command);
    first = (vertical_end & 0x3ff) << 10;
    command = vertical_start & 0x3ff;
    second = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_VERTICAL_RANGE);
    timing_dispatch = g_psyq_gpu_dispatch;
    __asm__("" : "=r"(timing_dispatch) : "0"(timing_dispatch));
    command |= second;
    control = timing_dispatch->control;
    control(first | command);
check_mode:
    if (g_psyq_gpu_cached_display_flags != *(u32*)&env->isinter)
        goto mode_changed;
    value = g_psyq_gpu_cached_display_x;
    compare = env->disp.x;
    __asm__("" : "=r"(value), "=r"(compare) : "0"(value), "1"(compare));
    value = (s16)value;
    if (value != compare)
        goto mode_changed;
    value = g_psyq_gpu_cached_display_y;
    compare = env->disp.y;
    __asm__("" : "=r"(value), "=r"(compare) : "0"(value), "1"(compare));
    value = (s16)value;
    if (value != compare)
        goto mode_changed;
    value = g_psyq_gpu_cached_display_width;
    compare = env->disp.w;
    __asm__("" : "=r"(value), "=r"(compare) : "0"(value), "1"(compare));
    value = (s16)value;
    if (value != compare)
        goto mode_changed;
    value = g_psyq_gpu_cached_display_height;
    compare = env->disp.h;
    __asm__("" : "=r"(value), "=r"(compare) : "0"(value), "1"(compare));
    value = (s16)value;
    if (value == compare)
        goto copy_environment;
mode_changed: {
    env->pad0 = GetVideoMode();
    if (env->pad0 == 1)
        mode |= PSYQ_GPU_DISPLAY_PAL;
    if (env->isrgb24)
        mode |= PSYQ_GPU_DISPLAY_RGB24;
    if (env->isinter)
        mode |= PSYQ_GPU_DISPLAY_INTERLACE;
    debug = &g_psyq_gpu_environment.reversed;
    __asm__("" : "=r"(debug) : "0"(debug));
    if (*debug)
        mode |= PSYQ_GPU_DISPLAY_REVERSE;
    if (env->disp.w > 280) {
        if (env->disp.w <= 352)
            mode |= PSYQ_GPU_DISPLAY_WIDTH_320;
        else if (env->disp.w <= 400)
            mode |= PSYQ_GPU_DISPLAY_WIDTH_368;
        else if (env->disp.w <= 560)
            mode |= PSYQ_GPU_DISPLAY_WIDTH_512;
        else
            mode |= PSYQ_GPU_DISPLAY_WIDTH_640;
    }
    value = env->pad0;
    compare = env->disp.h;
    if (value)
        value = compare < 289;
    else
        value = compare < 257;
    if (!value)
        mode |= PSYQ_GPU_DISPLAY_HEIGHT_480 | PSYQ_GPU_DISPLAY_INTERLACE;
    g_psyq_gpu_dispatch->control(mode);
}
copy_environment:
    psyq_api_memcpy(&g_psyq_gpu_display_environment, env, sizeof(DISPENV));
    return env;
}
