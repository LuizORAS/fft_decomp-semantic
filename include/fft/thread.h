#ifndef THREAD_H
#define THREAD_H

/* Native cooperative threads shared by the BATTLE and WORLD schedulers. */

#include "psx/types.h"

/* Native cooperative scheduler context, not scenario bytecode or its operand
 * buffer. BATTLE 0x8014c8a0 and WORLD 0x800ffd70 initialize the same layout;
 * both index slots with thread_id << 10 and start SP/FP at slot + 0x3f0.
 * The parameter stores at BATTLE 0x8014ca38 and WORLD's corresponding
 * three/four-parameter helpers establish full 32-bit payload values.
 * The seven 32-bit task values at 0x50 are zeroed by battle_thread_start /
 * world_thread_start; each task gives them its own meaning: menu blink/icon
 * cursor threads use [0] timer and [1] state, event unit/sprite-move threads
 * keep the misc id in [0], and DisplayMessage text threads use [2] x offset,
 * [3] y offset, [4] arrow offset, [5] opening type and [6] width override. The
 * remaining saved context/stack stays provisional. */
typedef struct native_thread {
    s32 function_parameter_1; /* 0x00 */
    s32 function_parameter_2; /* 0x04 */
    s32 function_parameter_3; /* 0x08 */
    s32 function_parameter_4; /* 0x0c; set by WORLD's four-parameter helper */
    u8 _unused_010[0x28];
    void* global_pointer;         /* 0x38 */
    u32* stack_pointer;           /* 0x3c */
    u32* frame_pointer;           /* 0x40 */
    void (*code_pointer)(void);   /* 0x44 */
    s32 is_running;               /* 0x48 */
    s32 task_id;                  /* 0x4c: native_thread_task_e */
    s32 task_words[7];            /* 0x50: per-task 32-bit values, see above */
    u8 _unused_06c[0x3f0 - 0x6c]; /* 0x6c: the thread's stack */
    u32 stack_top[4];             /* 0x3f0: initial SP/FP; the stack grows down from here */
} native_thread_t;
typedef char native_thread_size_must_be_1024[(sizeof(native_thread_t) == 1024) ? 1 : -1];
typedef char native_thread_running_must_be_0x48[((unsigned long)&((native_thread_t*)0)->is_running == 0x48) ? 1 : -1];
typedef char native_thread_task_must_be_0x4c[((unsigned long)&((native_thread_t*)0)->task_id == 0x4c) ? 1 : -1];

/* Reset loops cover sixteen 0x400-byte slots. WORLD's scheduler also scans
 * slot 16, which lies beyond this reset span. */
enum {
    NATIVE_THREAD_SLOT_COUNT = 16,
    NATIVE_THREAD_STRIDE = 1024,
    NATIVE_THREAD_ARRAY_BYTES = 16384,
    NATIVE_THREAD_LAST_WORD_OFFSET = 0x3ffc,
};

/* Slot numbers of the cooperative threads: the index into g_battle_threads /
 * g_world_threads and the first argument of the thread calls. Slot 0 is the
 * program's main loop. No slot belongs to one thread for good: the battle and
 * world programs run their threads in fixed slots, every menu screen runs its
 * own in the same slots while it is up, and a menu thread starts its child
 * menu one slot below itself. Each enum below maps the slots of one of them;
 * world.h, wldcore.h and the overlay headers map the slots their own screens
 * use (world_thread_slot_e, wldcore_thread_slot_e, attack_thread_slot_e,
 * bunit_thread_slot_e, equip_thread_slot_e, require_thread_slot_e). */

/* The battle and world programs (twin code). */
typedef enum native_thread_slot {
    NATIVE_THREAD_SLOT_MAIN = 0,
    NATIVE_THREAD_SLOT_EVENT = 1,           /* the event script: script_run_next_event and its twin */
    NATIVE_THREAD_SLOT_UNIT_VIEW = 2,       /* the unit view supervisor that starts the four panels below */
    NATIVE_THREAD_SLOT_MENU_BUILDING = 3,   /* the menu entry starter and the help menu thread */
    NATIVE_THREAD_SLOT_SYSTEM_FUNCTION = 4, /* the system function thread and the spell quote it dispatches */
    NATIVE_THREAD_SLOT_MENU_GRANDCHILD = 6, /* the action slot list under the skillset list */
    NATIVE_THREAD_SLOT_MENU_CHILD = 7,      /* the skillset list under the action menu */
    NATIVE_THREAD_SLOT_MENU = 8,            /* the open entry of the menu table: action menu, mini menu, a
                                             * system function's menu, the spell quote text */
    /* The unit view's panel slots, named for the thread each usually runs. */
    NATIVE_THREAD_SLOT_TILE_INFO_PANEL = 10,    /* tile info or numeric display */
    NATIVE_THREAD_SLOT_UNIT_SUMMARY_PANEL = 11, /* unit summary */
    NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS = 12,   /* right unit stats or attack caster stats */
    NATIVE_THREAD_SLOT_HOVERED_UNIT_STATS = 13, /* hovered unit stats or attack target stats */
    /* Passed to the resolve_id functions, which return a slot below 16
     * unchanged and otherwise the first idle slot. */
    NATIVE_THREAD_SLOT_FIRST_IDLE = 16,
} native_thread_slot_e;

/* The menu screens: formation and unit list, shop, deployment, post-battle,
 * save, debug, bar. */
typedef enum native_screen_thread_slot {
    NATIVE_SCREEN_THREAD_SLOT_TEXT = 1,              /* help, description and message text of a screen */
    NATIVE_SCREEN_THREAD_SLOT_MESSAGE = 2,           /* a message box or text window */
    NATIVE_SCREEN_THREAD_SLOT_MENU_CHILD = 5,        /* the child of the menu in slot 6 */
    NATIVE_SCREEN_THREAD_SLOT_MENU = 6,              /* the menu or notice a WORLD screen or the save screen runs */
    NATIVE_SCREEN_THREAD_SLOT_COMPARISON_STATUS = 7, /* the second (comparison) unit status banner */
    NATIVE_SCREEN_THREAD_SLOT_UNIT_STATUS = 8,       /* the unit status banner */
    NATIVE_SCREEN_THREAD_SLOT_CHARACTER_STATUS = 9,  /* the character status or equipment-and-ability panel */
    NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR = 12,   /* the numeric editor (stats) panel */
} native_screen_thread_slot_e;

/* Slots one routine takes for a thread of its own while the slot's usual
 * thread is not running. */
typedef enum native_borrowed_thread_slot {
    NATIVE_BORROWED_THREAD_SLOT_CARD_BACKDROP = 2, /* the memory card screen's zooming backdrop */
    NATIVE_BORROWED_THREAD_SLOT_DEPLOYMENT = 6,    /* attack_deploy_run_screen, or the map-data-load-complete thread
                                                    * when a battle has no deployment */
    /* The AT list of the dead-unit menu, once the unit view's panels have stopped. */
    NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU_CHILD = 9, /* a wait-forever thread while the AT list's child runs */
    NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU = 10,
    NATIVE_BORROWED_THREAD_SLOT_FADE_OUT = 13, /* the fade-out thread that follows the unit list overlay */
} native_borrowed_thread_slot_e;

/* task_id values shared by the BATTLE and WORLD schedulers. Each named id is
 * set by one family of twin thread functions (set_current_task_id or a
 * direct task_id store) and is looked up with *_thread_find_running_by_task.
 * 1/2/3 are the dialogue text threads' handshake: a text thread runs as 1,
 * sets 2 to wait until another thread stores 1 to resume it, and stops when
 * another thread stores 3. Ids without a known setter stay literal. */
typedef enum native_thread_task {
    /* No task: *_thread_start and *_thread_exit_current store it, and
     * world_menu_start_thread_group stores it into thread 12 when the location changes. */
    NATIVE_THREAD_TASK_NONE = 0,
    NATIVE_THREAD_TASK_RESUME = 1,
    NATIVE_THREAD_TASK_WAIT_FOR_RESUME = 2,
    NATIVE_THREAD_TASK_STOP_REQUEST = 3,
    NATIVE_THREAD_TASK_CAMERA = 4,
    NATIVE_THREAD_TASK_MAP_DARKNESS = 6,
    NATIVE_THREAD_TASK_MAP_LIGHT = 7,
    NATIVE_THREAD_TASK_EVENT_BLOCK = 8,
    NATIVE_THREAD_TASK_SPRITE_MOVE = 11,
    NATIVE_THREAD_TASK_COLOR_SCREEN = 12,
    NATIVE_THREAD_TASK_EARTHQUAKE = 13,
    NATIVE_THREAD_TASK_ADD_UNIT = 14,
    /* Unit-view panels: the ids the view thread sets pair with each panel thread. */
    NATIVE_THREAD_TASK_TILE_INFO_PANEL = 16,
    NATIVE_THREAD_TASK_HOVERED_UNIT_STATS = 17,
    NATIVE_THREAD_TASK_ATTACK_TARGET_STATS = 18,
    NATIVE_THREAD_TASK_ATTACK_CASTER_STATS = 19,
    NATIVE_THREAD_TASK_UNIT_SUMMARY_PANEL = 20,
    NATIVE_THREAD_TASK_UNIT_EDITOR_PANEL = 21,
    /* g_world_menu_thread_groups gives its three text windows the ids 0x20,
     * 0x21 and 0x22 (threads 9, 10 and 11); the window thread slides only the
     * second one with the main scroll. */
    NATIVE_THREAD_TASK_MENU_GROUP_SECOND_WINDOW = 33,
    NATIVE_THREAD_TASK_NUMERIC_DISPLAY_PANEL = 35,
    NATIVE_THREAD_TASK_MESSAGE_BOX = 38,
    NATIVE_THREAD_TASK_MENU_WINDOW_BUILD = 49,
    /* Text thread waiting for its next message; Change Dialog and
     * world_text_resume_printing swap in new text and store 1. */
    NATIVE_THREAD_TASK_DIALOG_AWAIT_TEXT = 51,
    NATIVE_THREAD_TASK_LOAD_EVTCHR = 52,
    NATIVE_THREAD_TASK_BG_SOUND = 53,
    /* Dark Screen thread start; Remove Dark Screen stores it into the
     * 0x37 thread to end the hold. */
    NATIVE_THREAD_TASK_DARK_SCREEN = 54,
    NATIVE_THREAD_TASK_DARK_SCREEN_HOLD = 55,
    NATIVE_THREAD_TASK_CONDITION_DISPATCH = 56,
    NATIVE_THREAD_TASK_UNIT_STATUS_BANNER = 57,
    NATIVE_THREAD_TASK_RIGHT_UNIT_STATS = 58,
    NATIVE_THREAD_TASK_STATUS_PANEL = 59,
    NATIVE_THREAD_TASK_SHOW_GRAPHIC = 61,
    NATIVE_THREAD_TASK_DEBUG_UNIT_EDITOR = 62,
    NATIVE_THREAD_TASK_HELP_MENU = 64,
    NATIVE_THREAD_TASK_PLAY_EFFECT = 65,
    NATIVE_THREAD_TASK_MINI_MENU = 66,
    NATIVE_THREAD_TASK_INFLICT_STATUS = 67,
    NATIVE_THREAD_TASK_EXECUTE_EVENT = 68,
    NATIVE_THREAD_TASK_UNIT_MESSAGE = 70,
} native_thread_task_e;

#endif
