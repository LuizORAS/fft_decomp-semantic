#ifndef THREAD_H
#define THREAD_H

/* Native cooperative threads shared by the BATTLE and WORLD schedulers. */

#include "psx/types.h"

/* thread */
/* The thread record both schedulers use, and the task ids threads publish. */

/* Native cooperative thread slot, 0x400 bytes. *_thread_start zeroes the task words and sets the
 * stack to the slot's top (0x3f0), leaving about 900 bytes of stack; each *_thread_yield saves
 * s0-s7, k0 and k1 (saved_registers), gp, sp, fp and ra (code_pointer) here. The parameters and
 * task words are the thread's inputs: menu blink/icon cursor threads use [0] timer and [1] state,
 * event unit/sprite-move threads keep the misc id in [0], DisplayMessage text threads use [2] x
 * offset, [3] y offset, [4] arrow offset, [5] opening type and [6] width override, and the
 * scrolling text page reads [4] as a redraw request. */
typedef struct native_thread {
    /* The four parameters are s32 but often carry pointers (QUIRKS.md). */
    s32 function_parameter_1;   /* 0x00 */
    s32 function_parameter_2;   /* 0x04 */
    s32 function_parameter_3;   /* 0x08 */
    s32 function_parameter_4;   /* 0x0c; set by WORLD's four-parameter helper */
    u32 saved_registers[10];    /* 0x10: s0-s7, k0, k1 */
    void* global_pointer;       /* 0x38 */
    u32* stack_pointer;         /* 0x3c */
    u32* frame_pointer;         /* 0x40 */
    void (*code_pointer)(void); /* 0x44: resume address: the start function, then each yield's ra */
    s32 is_running;             /* 0x48 */
    s32 task_id;                /* 0x4c */
    s32 task_words[7];          /* 0x50: per-task words, see above */
    u8 _unused_06c[0x3f0 - 0x6c];
    u32 stack_top[4]; /* 0x3f0: initial SP/FP; the stack grows down from here */
} native_thread_t;
typedef char native_thread_size_must_be_0x400[(sizeof(native_thread_t) == 0x400) ? 1 : -1];
typedef char native_thread_running_must_be_0x48[((unsigned long)&((native_thread_t*)0)->is_running == 0x48) ? 1 : -1];
typedef char native_thread_task_must_be_0x4c[((unsigned long)&((native_thread_t*)0)->task_id == 0x4c) ? 1 : -1];

/* Reset loops cover sixteen 0x400-byte slots. WORLD's scheduler also scans
 * slot 16, which lies beyond this reset span. */
enum {
    NATIVE_THREAD_SLOT_COUNT = 16,
    NATIVE_THREAD_STRIDE = 0x400,
    NATIVE_THREAD_ARRAY_BYTES = 0x4000,
    NATIVE_THREAD_LAST_WORD_OFFSET = 0x3ffc,
};

/* task_id values shared by the BATTLE and WORLD schedulers. Each named id is
 * set by one family of twin thread functions (set_current_task_id or a
 * direct task_id store) and is looked up with *_thread_find_running_by_task.
 * 1/2/3 are the dialogue text threads' handshake: a text thread runs as 1,
 * sets 2 to wait until another thread stores 1 to resume it, and stops when
 * another thread stores 3. Ids without a known setter stay literal. */
typedef enum native_thread_task {
    NATIVE_THREAD_TASK_RESUME = 1,
    NATIVE_THREAD_TASK_WAIT_FOR_RESUME = 2,
    NATIVE_THREAD_TASK_STOP_REQUEST = 3,
    NATIVE_THREAD_TASK_CAMERA = 4,
    NATIVE_THREAD_TASK_MAP_DARKNESS = 6,
    NATIVE_THREAD_TASK_MAP_LIGHT = 7,
    NATIVE_THREAD_TASK_EVENT_BLOCK = 8,
    NATIVE_THREAD_TASK_SPRITE_MOVE = 0xb,
    NATIVE_THREAD_TASK_COLOR_SCREEN = 0xc,
    NATIVE_THREAD_TASK_EARTHQUAKE = 0xd,
    NATIVE_THREAD_TASK_ADD_UNIT = 0xe,
    NATIVE_THREAD_TASK_UNIT_EDITOR_PANEL = 0x15,
    NATIVE_THREAD_TASK_NUMERIC_DISPLAY_PANEL = 0x23,
    NATIVE_THREAD_TASK_MESSAGE_BOX = 0x26,
    NATIVE_THREAD_TASK_MENU_WINDOW_BUILD = 0x31,
    /* Text thread waiting for its next message; Change Dialog and
     * world_text_resume_printing swap in new text and store 1. */
    NATIVE_THREAD_TASK_DIALOG_AWAIT_TEXT = 0x33,
    NATIVE_THREAD_TASK_LOAD_EVTCHR = 0x34,
    NATIVE_THREAD_TASK_BG_SOUND = 0x35,
    /* Dark Screen thread start; Remove Dark Screen stores it into the
     * 0x37 thread to end the hold. */
    NATIVE_THREAD_TASK_DARK_SCREEN = 0x36,
    NATIVE_THREAD_TASK_DARK_SCREEN_HOLD = 0x37,
    NATIVE_THREAD_TASK_CONDITION_DISPATCH = 0x38,
    NATIVE_THREAD_TASK_UNIT_STATUS_BANNER = 0x39,
    NATIVE_THREAD_TASK_STATUS_PANEL = 0x3b,
    NATIVE_THREAD_TASK_SHOW_GRAPHIC = 0x3d,
    NATIVE_THREAD_TASK_DEBUG_UNIT_EDITOR = 0x3e,
    NATIVE_THREAD_TASK_HELP_MENU = 0x40,
    NATIVE_THREAD_TASK_PLAY_EFFECT = 0x41,
    NATIVE_THREAD_TASK_MENU_SOUND_DELAY = 0x42,
    NATIVE_THREAD_TASK_INFLICT_STATUS = 0x43,
    NATIVE_THREAD_TASK_EXECUTE_EVENT = 0x44,
    NATIVE_THREAD_TASK_UNIT_MESSAGE = 0x46,
} native_thread_task_e;

#endif
