#ifndef BATTLE_EVENT_H
#define BATTLE_EVENT_H

/* Battle scenario orchestration and deferred commands produced by scripts. */

#include "fft/map.h"
#include "fft/script.h"

/* script */
/* The event-script interpreter (battle_script_*): loading and running an event, its frame, the
 * script variables and comparisons, the scenario conditions that pick the next event, and the event
 * input. */

extern u16 g_battle_scenario_music_1_id;
extern u16 g_battle_scenario_music_2_id;

/* Separate triggers, command arguments, and completion status. Argument
 * domains remain provisional: map commands 0x80/0x83, polls 0x81/0x84.
 * The u16 requests and arguments are read with lhu by the consumer
 * battle_script_process_pending_requests; every other access is a store. */
extern u16 g_battle_3d_object_use_request; /* 0x80165fe2 */

extern u16 g_battle_3d_object_wait_status;    /* 0x8016606e */
extern u16 g_battle_field_object_use_request; /* 0x80165fe4 */
extern u16 g_battle_field_object_wait_status; /* 0x80166070 */

/* event */
/* What the event instructions share with the main loop: the requests
 * battle_script_process_pending_requests carries out, the event unit slots, the staged statuses and
 * the effect, music and background state. */

/* Per-byte status removal masks applied while an event stages a unit: three
 * eight-byte sets, of which the first BATTLE_STATUS_BYTE_COUNT bytes are used
 * (0x80169758). */
typedef enum battle_event_status_mask_set {
    BATTLE_EVENT_STATUS_MASK_BATTLE_STARTED = 0,
    BATTLE_EVENT_STATUS_MASK_PRE_BATTLE = 1,
    /* Monster skillsets, both Altima forms and the six undead jobs. */
    BATTLE_EVENT_STATUS_MASK_PRE_BATTLE_MONSTER_OR_UNDEAD = 2,
} battle_event_status_mask_set_e;

enum { BATTLE_EVENT_STATUS_MASK_COUNT = 3 };
extern s32 g_battle_next_event_id;
extern s16 g_battle_scenario_event_active;
extern u16 g_battle_event_input_suppression_frames;
extern s16 g_battle_event_effect_target_x;
extern s16 g_battle_event_effect_target_y;
extern s16 g_battle_event_map_jump_in_request;
extern s32 g_battle_event_pending_loaded_evtchr_slot;
extern evtchr_file_record_t* g_battle_event_loaded_evtchr_buffer;
extern u8 g_battle_event_status_masks[BATTLE_EVENT_STATUS_MASK_COUNT][8];

extern u8
    g_battle_event_thread_status_snapshot[]; /* per-thread running flags sampled before the yield, indexed 1..15 */

extern u16 g_battle_event_effect_target_runtime_id; /* 0x80165ff8 */

/* 0x80166000: consumer passes mode 2 when equal to 2, otherwise mode 0.
 * The last Effect operand is not a second Y coordinate. */
extern s16 g_battle_event_effect_target_mode;

extern u16 g_battle_event_3d_object_id;                     /* 0x80173c94 */
extern u16 g_battle_event_3d_object_state;                  /* 0x80173c96 */
extern u16 g_battle_event_map_command_83_arg1;              /* 0x80174058 */
extern u16 g_battle_event_map_command_83_arg2;              /* 0x8017405a */
extern u16 g_battle_event_music_switch_fade;                /* 0x80173f56; consumer multiplies by 4 */
extern u16 g_battle_event_music_switch_volume;              /* 0x80173f54; scaled 0..96 to 0..127 */
extern s16 g_battle_event_pending_effect_id;                /* 0x80173cb4 */
extern s32 g_battle_event_pending_evtchr_save_slot;         /* 0x80173f4c */
extern s32 g_battle_event_pending_loaded_evtchr_clear_slot; /* 0x80173ca0 */
extern s16 g_battle_event_pending_map_jump_out_2_id;        /* 0x8017405c */

/* Deferred BATTLE commands: producer 0x80143bd8, consumer
 * 0x80143418..0x801439c0. The outer update at 0x80142d58 clears one-shot
 * requests, schedules script threads, then applies commands on its normal
 * path. These are separate globals, not a contiguous struct. Signed pending
 * slots use -1; request bits and polled states do not. */
extern s16 g_battle_event_pending_map_jump_out_id; /* 0x80174054 */

extern s16 g_battle_event_pending_map_state;                  /* 0x80173f64; script variable 0x24 */
extern s32 g_battle_event_pending_reserved_vram_release_slot; /* 0x80173f48 */
extern s32 g_battle_event_pending_saved_evtchr_clear_slot;    /* 0x80173f50 */

/* 0x80173cac: packed unit ID << 8 | reserved VRAM slot; -1 absent. */
extern s32 g_battle_event_pending_unit_vram_copy;

/* 0x80173f68: packed weather flags; consumer replaces it with a table index. */
extern s16 g_battle_event_weather_request;

extern s32 g_battle_event_frame_counter;

typedef struct battle_script_weather_environment {
    u8 environment_id;
    u8 _unused_01;
} battle_script_weather_environment_t;
typedef char
    battle_script_weather_environment_size_must_be_2[(sizeof(battle_script_weather_environment_t) == 2) ? 1 : -1];

extern map_background_gradient_colors_t g_battle_event_background_colors;
extern battle_script_weather_environment_t g_battle_event_weather_commands[];

#endif
