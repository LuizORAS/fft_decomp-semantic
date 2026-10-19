#ifndef BATTLE_SCRIPT_H
#define BATTLE_SCRIPT_H

/* Battle scene actions and orchestration invoked by scenario bytecode. */

#include "fft/map.h"
#include "fft/menu.h"
#include "fft/script.h"

/* script */
/* The event-script interpreter (battle_script_*): loading and running an event, its frame, the
 * script variables and comparisons, the scenario conditions that pick the next event, and the event
 * input. */

extern u16 g_battle_script_map_darkness_durations[];
extern u32 g_battle_script_saved_event_input;
void battle_script_route_event_input(void);
void battle_script_set_current_otag_entry_and_update_input(u32* entry, s32 input);
extern s16 g_script_finish_check_pending;
extern VECTOR g_battle_script_light_scale;
extern MATRIX g_battle_script_light_base_direction_matrix;
extern SVECTOR g_battle_script_light_angles;
extern MATRIX g_battle_script_light_base_color_matrix;
void battle_script_blue_remove_unit(s32 unit_id);
void battle_script_blue_remove_all_enemy_units(void);
void battle_script_earthquake_thread(void);
s32 battle_script_get_current_scenario_finish_operation(void);
void battle_script_map_darkness_thread(void);
void script_sprite_move_thread(void);
void script_sprite_move_beta_thread(void);
u32 battle_script_get_event_finish_operation(void);
void battle_script_update_event_input_state(u32 state);
void script_warp_unit(const event_warp_unit_parameters_t* parameters);
void battle_script_run_sprite_move(const event_sprite_move_parameters_t* parameters, s32 use_speed);

/* Returns the event result that the battle state handlers switch on;
 * proven by 0x80078fb4 and 0x800785ac, which both consume it. */
s32 battle_script_run_event_frame(u32* ot, u32 buttons);

s32 script_filter_unit_id_by_mode(u16* out_id, u16* in_id, s32* mode);
s32 battle_script_is_deployment_running(u32* ot, u32 buttons);
void battle_script_process_pending_requests(void);
void battle_script_set_specialized_map_destroyed(void);

s32 battle_script_add_ghost_unit_event_instruction(
    s32 map_x, s32 map_y, s32 map_level, s16 facing, s16 spritesheet_id, s32 runtime_id, s32 flags);

void battle_script_add_unit_start_thread(void);
void battle_script_toggle_message_portrait_flip(const u8* parameters);
void battle_script_set_units_palette_update_suppression(s32 unit_id, s32 enable);
s32 script_is_tutorial_event_slot(void);
void battle_script_load_portrait_colors_event_instruction(s32 portrait_id);
void battle_script_set_event_speed(s32 speed);
void battle_script_teleport_out_event_instruction(s32 arg, s32 remove);
extern map_tile_coordinates_t g_battle_script_warp_unit_coords;
void script_wait_sprite_move_event_instruction(s32 unit_id);
void battle_script_warp_unit_display_to_paired_unit(s32 runtime_id);
void battle_script_add_unit_stub(void);
void battle_script_frame_after_threads_stub(void);
void battle_script_frame_after_requests_stub(void);
void battle_script_opcode_74_event_instruction(const u8* unused_parameters);
void battle_script_dark_screen_thread(void);

#endif
