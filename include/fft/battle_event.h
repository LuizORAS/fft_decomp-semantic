#ifndef BATTLE_EVENT_H
#define BATTLE_EVENT_H

/* Battle scenario interpreter, event state and deferred commands. */

#include "fft/map.h"
#include "fft/menu.h"
#include "fft/script.h"
#include "fft/unit.h"

/* event */
typedef struct battle_script_weather_environment {
    u8 environment_id;
    u8 _unused_01;
} battle_script_weather_environment_t;
typedef char
    battle_script_weather_environment_size_must_be_2[(sizeof(battle_script_weather_environment_t) == 2) ? 1 : -1];

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

extern map_tile_coordinates_t g_battle_script_warp_coords;
extern u16 g_battle_script_attack_entry_mode; /* script_attack_entry_mode_e */
extern s32 g_battle_script_condition_unit_battle_id;
extern s32 g_battle_script_rand16_state;
extern u32 g_battle_script_unfiltered_controller_input;
extern const char g_battle_script_pause_message[];
extern s16 g_battle_script_debug_paused;
extern s16 g_battle_event_effect_target_x;
extern s16 g_battle_event_effect_target_y;
extern u8 g_battle_script_condition_param_lengths[];
extern u16 g_battle_script_map_darkness_durations[];
extern u16 g_battle_script_unit_face_slot_order[];
extern u32 g_battle_script_event_input;
extern u32 g_battle_script_saved_event_input;
extern s16 g_battle_event_map_jump_in_request;
extern s16 g_battle_script_threads_idle;
extern s32 g_battle_next_event_id;
extern u16 g_battle_script_frame_result_override;
extern VECTOR g_battle_script_light_scale;
extern s32 g_battle_event_pending_loaded_evtchr_slot;
extern MATRIX g_battle_script_light_base_direction_matrix;
struct battle_gfx_packed_evtchr_record;
extern struct battle_gfx_packed_evtchr_record* g_battle_event_loaded_evtchr_buffer;
extern SVECTOR g_battle_script_light_angles;
extern MATRIX g_battle_script_light_base_color_matrix;
extern u8 g_battle_event_status_masks[BATTLE_EVENT_STATUS_MASK_COUNT][8];

extern u8
    g_battle_event_thread_status_snapshot[]; /* per-thread running flags sampled before the yield, indexed 1..15 */

extern s16 g_battle_scenario_event_active;
extern u32 g_battle_script_event_current_input;
extern u32 g_battle_script_event_initial_repeat_counter;
extern u32 g_battle_script_event_previous_input;
extern u32 g_battle_script_event_repeat_counter;
extern u32 g_battle_script_event_secondary_repeat_counter;
/* g_main_script_variables: a variable id below EVENT_SCRIPT_VAR_PACKED_BIT_FIRST
 * (event_script_variable_id_e) is its word index. */
extern s32* g_battle_script_variables;
extern u16 g_battle_scenario_music_1_id;
extern u16 g_battle_scenario_music_2_id;
extern u16 g_battle_event_input_suppression_frames;
extern s32 g_battle_menu_input_lock_mode;

/* BATTLE pointer slot 0x80173ca4; WORLD pointer slot 0x801cd75c. */
extern event_file_block_t* g_battle_event_block;
extern u16 g_battle_event_effect_target_misc_id; /* 0x80165ff8 */

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
extern s16 g_battle_event_pending_map_jump_out_id;            /* 0x80174054 */
extern s16 g_battle_event_pending_map_state;                  /* 0x80173f64; script variable 0x24 */
extern s32 g_battle_event_pending_reserved_vram_release_slot; /* 0x80173f48 */
extern s32 g_battle_event_pending_saved_evtchr_clear_slot;    /* 0x80173f50 */

/* 0x80173cac: packed misc ID << 8 | reserved VRAM slot; -1 absent. */
extern s32 g_battle_event_pending_unit_vram_copy;
extern s32 g_battle_event_speed;
extern s16 g_battle_event_unit_slots[4];

/* 0x80173f68: packed weather flags; consumer replaces it with a table index. */
extern s16 g_battle_event_weather_request;
extern battle_script_weather_environment_t g_battle_event_weather_commands[];

/* Operand-byte counts, excluding the opcode; not per-operand width schemas.
 * BATTLE 0x8014d170, WORLD 0x8013a454. The tables end after opcode 0xf3. */
extern u8 g_battle_script_event_instruction_sizes[0xf4];

/* 0x801660a3: suppress recursive write filtering while getters use operand 0. */
extern u8 g_battle_script_variable_write_guard;
extern s32 g_battle_event_frame_counter;
extern s32 g_battle_script_loaded_event_id;
extern s16 g_battle_script_opcode_dc_value; /* EVENT_OPCODE_UNKNOWN_DC stores 13; nothing reads it */

extern map_background_gradient_colors_t g_battle_event_background_colors;

void battle_script_advance_date_by_one_year(void);
void battle_script_apply_input_overrides(u32* input);
void battle_script_assign_bitset_flag(u32* bitset, s32 bit_index, s32 value);
void battle_script_block_start_thread(void);
void battle_script_blue_remove_unit(s32 unit_id);
void battle_script_blue_remove_all_enemy_units(void);
void battle_script_change_stats(const event_change_stats_parameters_t* parameters);
void battle_script_clamp_s32_to_range(s32* value, s32 minimum, s32 maximum);
void battle_script_earthquake_start(void);
void battle_script_face_tile(const event_face_tile_parameters_t* parameters);
void battle_script_face_unit(const event_face_unit_parameters_t* parameters, s32 second_only);
void battle_script_filter_controller_input(u32* input);
void battle_script_focus(event_focus_parameters_t* parameters);
/* A typed operand plus a local Camera cursor reorders the original argument saves. */
void battle_script_focus_speed(u8* parameters, s32* position, s32* rotation);
u32* battle_script_get_controller_input_pointer(s32 use_current);
s32 battle_script_get_current_scenario_finish_operation(void);
s32 battle_script_get_variable(s32 variable_id);
s32 battle_script_interpolate_range_fixed12(s32 start, s32 end, s32 fraction, s32 value);
void battle_script_join_units_silently(void);
void battle_script_jump_event_instruction(const event_jump_parameters_t* parameters);
void battle_script_load_event(s32 id);

/* The byte-stream reader at 0x80146078 preserves signed halfwords. Some
 * callers forward their incoming a0 without an additional argument move.
 * unit_t lookup at 0x80133158 accepts and returns full words; narrowing is
 * caller-specific, not part of its interface. */
s16 battle_script_load_halfword(const u8* data);
s32 battle_script_load_next_event(void);
void battle_script_map_darkness(void);
void battle_script_march_units(const event_march_parameters_t* parameters);
void battle_script_print_debug_message(void);
void battle_script_reset_event_state(void);
void battle_script_run_next_event(void);
void battle_script_seed_random_from_vsync(void);
void battle_script_set_variable(s32 variable_id, s32 value);
void battle_script_show_graphic(void);
void battle_script_sprite_move(void);
void battle_script_sprite_move_beta(void);
void battle_script_start_current_event_thread(void);
void battle_script_unit_anim(const event_unit_anim_parameters_t* parameters);
u32 battle_script_get_event_finish_operation(void);
void battle_script_store_halfword(u8* destination, s16 value);
void battle_script_update_event_input_state(u32 state);
void battle_script_set_current_otag_entry_and_update_input(u32* entry, s32 input);
void battle_script_wait_for_unit_ready(s32 misc_id);
void battle_script_warp_unit(const event_warp_unit_parameters_t* parameters);
void battle_script_set_units_movement_effect_suppression(const event_movement_effect_parameters_t* parameters);
s32 battle_script_run_scenario_conditions(void);
s32 battle_script_check_scenario_condition(s32 type, s32 id, s32 value, s32 tile_y, s32 elevation);
void battle_script_run_sprite_move(const event_sprite_move_parameters_t* parameters, s32 use_speed);

/* Returns the event result that the battle state handlers switch on;
 * proven by 0x80078fb4 and 0x800785ac, which both consume it. */
s32 battle_script_run_event_frame(u32* ot, u32 buttons);
void battle_script_load_event_input_state(const menu_input_state_t* saved);
void battle_script_copy_matrix(MATRIX* destination, const MATRIX* source);
s32 battle_script_filter_unit_id_by_mode(u16* out_id, u16* in_id, s32* mode);
u32 battle_script_get_event_initial_repeat_counter(void);
s32 battle_script_is_deployment_running(u32* ot, u32 buttons);
void battle_script_process_pending_requests(void);
s32 battle_script_scale_music_volume(s32 x);
void battle_script_pulse_tutorial_wait_value(s32 value);
void battle_script_set_specialized_map_destroyed(void);
void battle_script_dismiss_unit_event_instruction(s32 unit_id);

s32 battle_script_add_ghost_unit_event_instruction(
    s32 map_x, s32 map_y, s32 map_level, s16 facing, s16 spritesheet_id, s32 misc_id, s32 flags);

void battle_script_add_unit_start_thread(void);
void battle_script_color_screen_thread(void);
void battle_script_color_unit_event_instruction(const event_color_unit_parameters_t* parameters);
void battle_script_toggle_message_portrait_flip(u8* ptr);
void battle_script_set_units_palette_update_suppression(s32 unit_id, s32 enable);
void battle_script_apply_relative_camera(event_camera_values_t* parameters, s32* src);
void battle_script_load_attack_graphics_event_instruction(void);
void battle_script_execute_display_conditions_instruction(void);

/* Whole matching interpreter ranges:
 * BATTLE 0x80143bd8..0x80145f78, WORLD 0x800f6f20..0x800f92a0 (exclusive). */
void battle_script_execute_event(void);
s32 battle_script_find_instruction_byte_offset(s32 offset, s32 opcode);

/* 0x80149d6c..0x80149ebc: returns offset after the matching target marker;
 * missing targets stop the current thread. alternate_opcode accepts -1. */
s32 battle_script_find_jump_target(s32 limit_offset, event_opcode_e target_opcode, s32 alternate_opcode, s32 target_id);
s32 battle_script_get_rand16(void);
s32 battle_script_get_variable_bit_position(s32 variable_id);

/* Encoded variable IDs select words, bits, or nibbles. The command runner's
 * fourth argument is supplied by existing callers but is not read. */
s32* battle_script_get_variable_word_pointer_from_id(s32 variable_id);
void battle_script_inflict_status_thread(void);
s32 battle_script_is_tutorial_event_slot(void);
void battle_script_load_portrait_colors_event_instruction(s32 portrait_id);
void battle_script_mirrorsprite_event_instruction(const event_mirror_sprite_parameters_t* parameters);
void battle_script_pause_event_instruction(void);
void battle_script_play_effect_thread(void);
void battle_script_resetpalette_event_instruction(const event_unit_parameters_t* parameters);
void battle_script_run_condition(event_opcode_e opcode);
void battle_script_run_variable_command(s32 opcode, s32 destination_id, s32 source, s32 unused);
void battle_script_set_event_speed(s32 speed);
void battle_script_teleportin_event_instruction(s32 unit_id, s32 unused);
void battle_script_teleportout_event_instruction(s32 arg, s32 remove);
void battle_script_unit_animation_rotate_event_instruction(const event_unit_anim_rotate_parameters_t* parameters);
void battle_script_unlockdate_event_instruction(u32* bitset, s32 date_index, s32 month, s32 day);
void battle_script_wait_value_event_instruction(const event_wait_value_parameters_t* parameters);
void battle_script_waitrotateunit_and_waitrotateall_event_instruction(s32 unit_id);
void battle_script_waitspritemove_event_instruction(s32 unit_id);
void battle_script_waitwalk_event_instruction(s32 unit_id);
void battle_script_walkto_event_instruction(const event_walk_to_parameters_t* parameters);
void battle_script_warp_unit_display_to_paired_unit(s32 misc_id);

void battle_script_add_unit_stub(void);
void battle_script_frame_after_threads_stub(void);
void battle_script_frame_after_requests_stub(void);
void battle_script_report_missing_unit_stub(s32 unused_unit_id);
void battle_script_opcode_74_event_instruction(const u8* unused_parameters);
void battle_script_dark_screen_thread(void);
void battle_script_process_inflict_status_commands(void);
void battle_script_rotate_unit(const event_rotate_unit_parameters_t* parameters);
s32 battle_script_get_misc_id(s32 unit_id);
void battle_script_play_effect_and_wait(void);

#endif
