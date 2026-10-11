#ifndef BATTLE_CAMERA_H
#define BATTLE_CAMERA_H

/* Battle camera state, input and scripted motion. */

#include "fft/battle_unit.h"
#include "fft/gfx.h"
#include "fft/script.h"

/* camera */
enum {
    BATTLE_CAMERA_PITCH_STEEP_MASK_THRESHOLD = 0x178,
};

/* g_battle_camera_rotation_action. */
typedef enum battle_camera_rotation_action {
    BATTLE_CAMERA_ROTATION_NONE = 0,
    BATTLE_CAMERA_ROTATION_LEFT = 1,  /* the yaw falls to its destination */
    BATTLE_CAMERA_ROTATION_RIGHT = 2, /* the yaw rises to its destination */
} battle_camera_rotation_action_e;

/* g_battle_camera_tilt_action and g_battle_camera_zoom_action: the move in progress towards
 * g_battle_map_tilt_target or g_battle_map_zoom_target. */
typedef enum battle_camera_step_action {
    BATTLE_CAMERA_STEP_NONE = 0,
    BATTLE_CAMERA_STEP_INCREASE = 2, /* tilt up; zoom in */
    BATTLE_CAMERA_STEP_DECREASE = 4, /* tilt down; zoom out */
} battle_camera_step_action_e;

/* g_battle_map_tilt_target values the tilt code tests; the target is a tilt band plus one, and
 * bands 3 and 4 have no pitch here. */
enum {
    BATTLE_MAP_TILT_LOW = 1,  /* pitch 0x12e */
    BATTLE_MAP_TILT_HIGH = 2, /* pitch 0x1c0 */
};

/* g_battle_map_zoom_target. */
enum {
    BATTLE_MAP_ZOOM_IN = 1,  /* scale ONE */
    BATTLE_MAP_ZOOM_OUT = 4, /* scale 0xc00 */
};

/* battle_camera_get_input_direction modes: which button state it reads. */
typedef enum battle_camera_input_source {
    BATTLE_CAMERA_INPUT_HELD = 0,
    BATTLE_CAMERA_INPUT_PRESSED = 1, /* newly pressed, repeating at the cursor repeat speed while held */
    BATTLE_CAMERA_INPUT_RELEASED = 2,
} battle_camera_input_source_e;

/* g_battle_cursor_input_mode: how the directional buttons map to map directions. The first two are
 * the Cursor Movement option's settings; FREE_CURSOR sends the four buttons along the screen's axes. */
typedef enum battle_cursor_input_mode {
    BATTLE_CURSOR_INPUT_OPTION_A = 0,
    BATTLE_CURSOR_INPUT_OPTION_B = 1,
    BATTLE_CURSOR_INPUT_FREE_CURSOR = 2,
} battle_cursor_input_mode_e;

extern u16 g_battle_camera_shake_z_offset;
extern s32 g_battle_previous_camera_yaw;
extern u16 g_battle_camera_script_variable_indices[EVENT_CAMERA_COMPONENT_COUNT];
extern u16 g_battle_camera_position_locked;
extern s32 g_battle_camera_yaw_band;
extern s32 g_battle_camera_wrap_yaw_pending;
extern VECTOR g_battle_camera_script_vector;
extern VECTOR* g_battle_camera_published_coords_ptr;
extern SVECTOR g_battle_camera_script_rotation;
extern SVECTOR* g_battle_camera_published_rotation_ptr;
/* Maps a camera quadrant index to its map-tile camera-block direction. */
extern u8 g_battle_camera_quadrant_block_directions[4];
extern VECTOR g_battle_camera_current_real_coords;
extern SVECTOR g_battle_camera_integer_coords;
extern MATRIX g_battle_camera_matrix;
extern u16 g_battle_camera_offset_screen_coord_countdown;
extern VECTOR g_battle_camera_offset_screen_coords;
extern VECTOR g_battle_camera_offset_screen_coords_fixed;
extern u16 g_battle_camera_real_coord_countdown;
extern VECTOR g_battle_camera_real_coord_delta;
extern s32 g_battle_camera_rotation_action;
extern SVECTOR g_battle_camera_rotation_backup;
extern u16 g_battle_camera_rotation_countdown;
extern SVECTOR g_battle_camera_rotation_delta;
extern s32 g_battle_camera_rotation_increment;
extern VECTOR g_battle_camera_saved_real_coords;
extern s32 g_battle_camera_tilt_action;
extern s32 g_battle_camera_tilt_increment;
extern VECTOR g_battle_camera_zoom;
extern s32 g_battle_camera_zoom_action;
extern VECTOR g_battle_camera_zoom_backup;
extern u16 g_battle_camera_zoom_countdown;
extern VECTOR g_battle_camera_zoom_delta;
extern s32 g_battle_camera_zoom_increment;
extern s32 g_battle_camera_cross_product_overflow; /* set by battle_fixed_cross_product_q12 on Q12 overflow */
extern s16 g_battle_camera_diagonal_yaws[4];

/* Base of the large render-state records; low offsets overlap camera/GPU fields.
 * .vx is the camera pitch and .vy its yaw. */
extern SVECTOR g_battle_camera_render_state;

/* 0x80166054: camera consumer unpacks two 2-bit fields and a high nibble. */
extern s32 g_battle_camera_speed_curve;
void battle_camera_convert_screen_coords_by_rotation(const SVECTOR* input, SVECTOR* output, s32 offset);
void battle_camera_convert_screen_coords_modify_by_1(const SVECTOR* screen, SVECTOR* destination);
void battle_camera_convert_screen_coords_by_1_from_misc_id(u32 misc_id, SVECTOR* coords);
void battle_camera_toggle_tilt(void);
s16 battle_camera_pick_nearest_unblocked_yaw(s32 misc_id_a, s32 misc_id_b);
void battle_camera_step_focus_toward_cursor_tile(s16 height);
void battle_camera_execute_angle_command(s32 command, s32 target, s32 duration, SVECTOR* angles);
void battle_camera_execute_position_command(s32 command, s32 target, s32 duration, VECTOR* offset);
s32 battle_camera_get_input_direction(s32 mode);
void battle_camera_update_effect_rotation_and_zoom(void);

/* Some routines address these components as separate linker symbols. */
void battle_camera_step_real_coords_toward_target(battle_unit_misc_data_t* target);
void battle_camera_update_vector_from_game_state(void);
void battle_camera_calculate_acting_unit_vector_normal(void);
s32 battle_camera_calculate_cursor_tile_vector_normal(void);
void battle_camera_call_rotate_left(s32 target_angle);
void battle_camera_call_rotate_right(s32 target_angle);
void battle_camera_call_toggle_tilt(void);
void battle_camera_call_zoom_map(void);
void battle_camera_copy_current_real_coords(void);
void battle_camera_copy_rotation(void);
void battle_camera_copy_zoom(void);
void battle_camera_execute_zoom_command(s32 command, s32 unused, s32 duration, VECTOR* zoom);
VECTOR* battle_camera_get_current_real_coords(void);
SVECTOR* battle_camera_get_rotation(void);
VECTOR* battle_camera_get_zoom(void);
void battle_camera_handle_rotation_input(void);
void battle_camera_init_defaults(void);
void battle_camera_init_tilt_and_zoom(void);
void battle_camera_normalize_yaw_angle(void);
s32 battle_camera_is_active(void);
void battle_camera_project_world_coords_to_screen(SVECTOR* world_coords, SVECTOR* screen_coords);
void battle_camera_request_rotation(s32 angle);
void battle_camera_restore_current_real_coords(void);
void battle_camera_restore_rotation(void);
void battle_camera_restore_zoom(void);
void battle_camera_rotate_left(s32 target_angle);
void battle_camera_rotate_right(s32 target_angle);
void battle_camera_rotate_when_unit_tile_not_visible(struct battle_unit_misc_data* unit);
s32 battle_camera_scale_cursor_tile_vector(void);
void battle_camera_set_current_real_coords(const VECTOR* coords);
void battle_camera_set_rotation(const SVECTOR* rotation);
void battle_camera_set_zoom(const VECTOR* zoom);
s32 battle_camera_step_track(camera_track_t* track, s32 component);
void battle_camera_store_state_to_script_variables(void);
void battle_camera_store_yaw_band(s32 value);
void battle_camera_toggle_zoom(void);
void battle_camera_update_map_pan(void);

void battle_camera_update_matrices(
    MATRIX* camera_matrix, SVECTOR* rotation, VECTOR* screen_offset, VECTOR* camera_zoom);

void battle_camera_update_offset_screen_coord_animation(void);
void battle_camera_update_real_coord_animation(void);
void battle_camera_update_rotation(void);
void battle_camera_update_rotation_animation(void);
void battle_camera_update_tilt(void);
void battle_camera_update_zoom(void);
void battle_camera_update_zoom_animation(void);
s32 battle_camera_wrap_yaw_angle(void);
void battle_camera_advance_effect_tracks(s16 frame);
void battle_camera_calculate_relative_offset(const SVECTOR* input, SVECTOR* output, s32 offset);
void battle_camera_calculate_relative_offset_by_1(const SVECTOR* input, SVECTOR* output);
void battle_camera_calculate_relative_offset_by_5(const SVECTOR* input, SVECTOR* output);

void battle_camera_choose_unblocked_rotation_for_tile(const SVECTOR* tile_coords, SVECTOR* rotation_out);

void battle_camera_convert_screen_coords_modify_by_5(const SVECTOR* screen, SVECTOR* destination);
s32 battle_camera_find_active_keyframe(s16 frame, s16 mask, s32* out_index, s32* out_frame);
void battle_camera_load_state_from_script_variables(void);

void battle_camera_set_focus_and_cursor_tile(const SVECTOR* coords, const SVECTOR* rotation);

void battle_camera_start_effect_keyframe_moves(s16 frame);
void battle_camera_update_cursor_tile_vector(void);
void battle_camera_shift_acting_unit_vector(void);
void battle_camera_fusion_thread(void);
void battle_camera_wait_until_idle(void);
void battle_camera_thread(void);

void battle_camera_clear_zoom_variable_low_bit(void);

#endif
