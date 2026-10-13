#ifndef BATTLE_SCRIPT_H
#define BATTLE_SCRIPT_H

/* Battle scenario bytecode execution, variables, input and instruction handlers. */

#include "fft/map.h"
#include "fft/menu.h"
#include "fft/script.h"

/* interpreter */
extern u16 g_script_attack_entry_mode; /* script_attack_entry_mode_e */
/* Operand-byte counts, excluding the opcode; not per-operand width schemas.
 * BATTLE 0x8014d170, WORLD 0x8013a454. The tables end after opcode 0xf3. */
extern u8 g_script_event_instruction_sizes[0xf4];
extern u16 g_script_frame_result_override;
extern s32 g_script_loaded_event_id;
extern s16 g_script_opcode_dc_value; /* EVENT_OPCODE_UNKNOWN_DC stores 13; nothing reads it */
extern s16 g_script_threads_idle;

void script_block_start_thread(void);
/* Whole matching interpreter ranges:
 * BATTLE 0x80143bd8..0x80145f78, WORLD 0x800f6f20..0x800f92a0 (exclusive). */
void script_execute_event(void);
s32 script_filter_unit_id_by_mode(u16* out_id, u16* in_id, s32* mode);
s32 script_find_instruction_byte_offset(s32 offset, s32 opcode);
/* 0x80149d6c..0x80149ebc: returns offset after the matching target marker;
 * missing targets stop the current thread. alternate_opcode accepts -1. */
s32 script_find_jump_target(s32 limit_offset, event_opcode_e target_opcode, s32 alternate_opcode, s32 target_id);
s32 script_get_current_scenario_finish_operation(void);
u32 script_get_event_finish_operation(void);
void script_jump_event_instruction(const event_jump_parameters_t* parameters);
void script_load_event(s32 id);
s32 script_load_next_event(void);
void script_process_pending_requests(void);
void script_reset_event_state(void);
/* Returns the event result that the battle state handlers switch on;
 * proven by 0x80078fb4 and 0x800785ac, which both consume it. */
s32 script_run_event_frame(u32* ot, u32 buttons);
void script_run_next_event(void);
void script_run_next_event_unused(void);
void script_set_event_speed(s32 speed);
void script_start_current_event_thread(void);
void script_wait_value_event_instruction(const event_wait_value_parameters_t* parameters);

/* variables */
/* 0x801660a3: suppress recursive write filtering while getters use operand 0. */
extern u8 g_script_variable_write_guard;
/* g_main_script_variables: a variable id below EVENT_SCRIPT_VAR_PACKED_BIT_FIRST
 * (event_script_variable_id_e) is its index into the 32-bit array. */
extern s32* g_script_variables;

void script_assign_bitset_flag(u32* bitset, s32 bit_index, s32 value);
s32 script_get_variable_bit_position(s32 variable_id);
/* Encoded variable IDs select 32-bit values, bits, or nibbles. The command runner's
 * fourth argument is supplied by existing callers but is not read. */
s32* script_get_variable_word_pointer_from_id(s32 variable_id);
void script_run_variable_command(s32 opcode, s32 destination_id, s32 source, s32 unused);
void script_set_variable(s32 variable_id, s32 value);

/* input */
extern u32 g_script_event_current_input;
extern u32 g_script_event_initial_repeat_counter;
extern u32 g_script_event_input;
extern u32 g_script_event_previous_input;
extern u32 g_script_event_repeat_counter;
extern u32 g_script_event_secondary_repeat_counter;
extern u32 g_script_saved_event_input;
extern u32 g_script_unfiltered_controller_input;

void script_apply_input_overrides(u32* input);
void script_filter_controller_input(u32* input);
u32* script_get_controller_input_pointer(s32 use_current);
u32 script_get_event_initial_repeat_counter(void);
void script_load_event_input_state(const menu_input_state_t* saved);
void script_route_event_input(void);
void script_set_current_otag_entry_and_update_input(u32* entry, s32 input);
void script_update_event_input_state(u32 state);

/* conditions */
extern u8 g_script_condition_param_lengths[];
extern s32 g_script_condition_unit_battle_id;

s32 script_check_scenario_condition(s32 type, s32 id, s32 value, s32 tile_y, s32 elevation);
void script_execute_display_conditions_instruction(void);
s32 script_is_deployment_running(u32* ot, u32 buttons);
s32 script_is_tutorial_event_slot(void);
void script_pulse_tutorial_wait_value(s32 value);
void script_run_condition(event_opcode_e opcode);
s32 script_run_scenario_conditions(void);

/* unit creation and removal */
s32 script_add_ghost_unit_event_instruction(
    s32 map_x, s32 map_y, s32 map_level, s16 facing, s16 spritesheet_id, s32 misc_id, s32 flags);
void script_add_unit_start_thread(void);
void script_add_unit_stub(void);
void script_blue_remove_all_enemy_units(void);
void script_blue_remove_unit(s32 unit_id);
void script_teleportout_event_instruction(s32 arg, s32 remove);

/* unit movement */
extern map_tile_coordinates_t g_script_warp_coords;

void script_march_units(const event_march_parameters_t* parameters);
void script_run_sprite_move(const event_sprite_move_parameters_t* parameters, s32 use_speed);
void script_set_units_movement_effect_suppression(const event_movement_effect_parameters_t* parameters);
void script_sprite_move(void);
void script_sprite_move_beta(void);
void script_wait_for_unit_ready(s32 misc_id);
void script_waitspritemove_event_instruction(s32 unit_id);
void script_waitwalk_event_instruction(s32 unit_id);
void script_walkto_event_instruction(const event_walk_to_parameters_t* parameters);
void script_warp_unit(const event_warp_unit_parameters_t* parameters);
void script_warp_unit_display_to_paired_unit(s32 misc_id);

/* unit animation */
extern u16 g_script_unit_face_slot_order[];

void script_face_tile(const event_face_tile_parameters_t* parameters);
void script_face_unit(const event_face_unit_parameters_t* parameters, s32 second_only);
void script_rotate_unit(const event_rotate_unit_parameters_t* parameters);
void script_unit_anim(const event_unit_anim_parameters_t* parameters);
void script_unit_animation_rotate_event_instruction(const event_unit_anim_rotate_parameters_t* parameters);
void script_waitrotateunit_and_waitrotateall_event_instruction(s32 unit_id);

/* unit statuses */
void script_change_stats(const event_change_stats_parameters_t* parameters);
void script_inflict_status_thread(void);
void script_process_inflict_status_commands(void);

/* camera */
void script_apply_relative_camera(event_camera_values_t* parameters, s32* src);
void script_earthquake_start(void);
void script_focus(event_focus_parameters_t* parameters);
/* A typed operand plus a local Camera cursor reorders the original argument saves. */
void script_focus_speed(u8* parameters, s32* position, s32* rotation);

/* map and lighting */
extern SVECTOR g_script_light_angles;
extern MATRIX g_script_light_base_color_matrix;
extern MATRIX g_script_light_base_direction_matrix;
extern VECTOR g_script_light_scale;
extern u16 g_script_map_darkness_durations[];

void script_dark_screen_thread(void);
void script_map_darkness(void);
void script_set_specialized_map_destroyed(void);

/* graphics and messages */
extern const char g_script_pause_message[];

void script_color_screen_thread(void);
void script_color_unit_event_instruction(const event_color_unit_parameters_t* parameters);
void script_load_attack_graphics_event_instruction(void);
void script_load_portrait_colors_event_instruction(s32 portrait_id);
void script_mirrorsprite_event_instruction(const event_mirror_sprite_parameters_t* parameters);
void script_play_effect_and_wait(void);
void script_play_effect_thread(void);
void script_print_debug_message(void);
void script_resetpalette_event_instruction(const event_unit_parameters_t* parameters);
void script_set_units_palette_update_suppression(s32 unit_id, s32 enable);
void script_show_graphic(void);
void script_toggle_message_portrait_flip(const u8* parameters);

/* date and random */
extern s32 g_script_rand16_state;

void script_advance_date_by_one_year(void);
s32 script_get_rand16(void);
void script_seed_random_from_vsync(void);
void script_unlockdate_event_instruction(u32* bitset, s32 date_index, s32 month, s32 day);

/* operand arithmetic */
void script_clamp_s32_to_range(s32* value, s32 minimum, s32 maximum);
void script_copy_matrix(MATRIX* destination, const MATRIX* source);
s32 script_interpolate_range_fixed12(s32 start, s32 end, s32 fraction, s32 value);
s32 script_scale_music_volume(s32 x);

/* debug */
extern s16 g_script_debug_paused;

void script_pause_event_instruction(void);

/* frame hooks */
void script_frame_after_requests_stub(void);
void script_frame_after_threads_stub(void);
void script_opcode_74_event_instruction(const u8* unused_parameters);

#endif
