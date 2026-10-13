#ifndef BATTLE_SCRIPT_H
#define BATTLE_SCRIPT_H

/* Battle scene actions and orchestration invoked by scenario bytecode. */

#include "fft/map.h"
#include "fft/menu.h"
#include "fft/script.h"

/* interpreter */
extern s16 g_script_threads_idle;

void script_block_start_thread(void);
s32 script_filter_unit_id_by_mode(u16* out_id, u16* in_id, s32* mode);
s32 script_get_current_scenario_finish_operation(void);
u32 script_get_event_finish_operation(void);
void script_process_pending_requests(void);
void script_reset_event_state(void);
/* Returns the event result that the battle state handlers switch on;
 * proven by 0x80078fb4 and 0x800785ac, which both consume it. */
s32 script_run_event_frame(u32* ot, u32 buttons);
void script_set_event_speed(s32 speed);
void script_start_current_event_thread(void);

/* input */
extern u32 g_script_saved_event_input;

void script_route_event_input(void);
void script_set_current_otag_entry_and_update_input(u32* entry, s32 input);
void script_update_event_input_state(u32 state);

/* conditions */

s32 script_is_deployment_running(u32* ot, u32 buttons);
s32 script_is_tutorial_event_slot(void);

/* unit creation and removal */
s32 script_add_ghost_unit_event_instruction(
    s32 map_x, s32 map_y, s32 map_level, s16 facing, s16 spritesheet_id, s32 runtime_id, s32 flags);
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
void script_waitspritemove_event_instruction(s32 unit_id);
void script_warp_unit(const event_warp_unit_parameters_t* parameters);
void script_warp_unit_display_to_paired_unit(s32 runtime_id);

/* unit animation */
extern u16 g_script_unit_face_slot_order[];

void script_face_tile(const event_face_tile_parameters_t* parameters);
void script_face_unit(const event_face_unit_parameters_t* parameters, s32 second_only);
void script_rotate_unit(const event_rotate_unit_parameters_t* parameters);
void script_unit_anim(const event_unit_anim_parameters_t* parameters);

/* unit statuses */
void script_change_stats(const event_change_stats_parameters_t* parameters);
void script_inflict_status_thread(void);
void script_process_inflict_status_commands(void);

/* camera */
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

void script_color_screen_thread(void);
void script_color_unit_event_instruction(const event_color_unit_parameters_t* parameters);
void script_load_portrait_colors_event_instruction(s32 portrait_id);
void script_set_units_palette_update_suppression(s32 unit_id, s32 enable);
void script_toggle_message_portrait_flip(const u8* parameters);

/* frame hooks */
void script_frame_after_requests_stub(void);
void script_frame_after_threads_stub(void);
void script_opcode_74_event_instruction(const u8* unused_parameters);

#endif
