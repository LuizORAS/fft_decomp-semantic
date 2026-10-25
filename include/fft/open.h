#ifndef OPEN_H
#define OPEN_H

/* OPEN/OPEN.BIN: opening and title. */

#include "fft/world.h"
#include "psx/libpress.h"

/* thread */
/* BATTLE's cooperative threads: slot 0 is the main loop, and one scheduler pass a frame runs slots 1-15. */

/* Serialized ending-script opcodes, in the order of g_open_opcode_handlers. */
typedef enum open_script_opcode {
    OPEN_SCRIPT_OPCODE_END = 0x00,
    OPEN_SCRIPT_OPCODE_POSITIONED_TEXT = 0x01,
    OPEN_SCRIPT_OPCODE_SCROLL_SPEED = 0x02,
    OPEN_SCRIPT_OPCODE_REVEAL_SCREEN = 0x03,
    OPEN_SCRIPT_OPCODE_HIDE_SCREEN = 0x04,
    OPEN_SCRIPT_OPCODE_WAIT_FRAMES = 0x05,
    OPEN_SCRIPT_OPCODE_WAIT_SCROLL = 0x06,
    OPEN_SCRIPT_OPCODE_START_XA_AUDIO = 0x07,
    OPEN_SCRIPT_OPCODE_FADE_IN_AUDIO = 0x08,
    OPEN_SCRIPT_OPCODE_FADE_OUT_AUDIO = 0x09,
    OPEN_SCRIPT_OPCODE_STOP_XA_AUDIO = 0x0a,
    OPEN_SCRIPT_OPCODE_LOAD_OPNTEX_FRAME = 0x0b,
    OPEN_SCRIPT_OPCODE_FADE_IN_PICTURE = 0x0c,
    OPEN_SCRIPT_OPCODE_FADE_OUT_PICTURE = 0x0d,
    OPEN_SCRIPT_OPCODE_WAIT_XA_AUDIO_IDLE = 0x0e,
    OPEN_SCRIPT_OPCODE_START_OPNTEX_SEQUENCE = 0x0f,
} open_script_opcode_e;

/* The ending-script dispatcher passes a serialized 16-bit cursor. */
typedef void (*open_script_entry_handler_t)(s16* entry);

/* text */
/* WORLD's text engine (world_text_*), BATTLE's with WORLD's additions: the text files WORLD.BIN carries
 * (SAMPLE.LZW, HELP.LZW, WLDHELP.LZW) and its resident text blocks, the message box thread of the shops and
 * the name entry, and the text image renderers its screens share with the overlays. */

extern s32 g_world_text_wldhelp_section_offsets[];

/* system */
/* OPEN.BIN's own state: the runtime flags, the result it returns to MAIN, the VSync mode, the two work
 * buffers the title and the prologue copy pictures into, and the setup of its two entry points
 * (open_system_run_main_loop for the title sequence, open_system_run_ending_cutscene for the ending). */

/* g_open_system_runtime_flags. */
typedef enum open_system_runtime_flag {
    OPEN_SYSTEM_RUNTIME_FLAG_RUNNING = 0x00001,          /* the OPEN main loop runs while set */
    OPEN_SYSTEM_RUNTIME_FLAG_MOVIE_STREAMING = 0x00002,  /* cleared by open_movie_stop_stream */
    OPEN_SYSTEM_RUNTIME_FLAG_CD_AUDIO_PLAYING = 0x00004, /* open_movie_pause_cd_audio pauses the drive and clears it */
    OPEN_SYSTEM_RUNTIME_FLAG_RENDER_RECORDS_HIDDEN = 0x00008,
    OPEN_SYSTEM_RUNTIME_FLAG_INPUT_DISABLED = 0x00010,
    OPEN_SYSTEM_RUNTIME_FLAG_DISPLAY_BLANKED = 0x00040,
    OPEN_SYSTEM_RUNTIME_FLAG_WORLD_OVERLAY_LOADED = 0x00080,
    OPEN_SYSTEM_RUNTIME_FLAG_PARTY_CHOSEN = 0x00100,
    OPEN_SYSTEM_RUNTIME_FLAG_WORLD_MENU_FRAME = 0x00400, /* the WORLD menu frame runs every loop */
    OPEN_SYSTEM_RUNTIME_FLAG_WIDE_SCREEN = 0x01000,
    OPEN_SYSTEM_RUNTIME_FLAG_RGB24 = 0x02000,
    OPEN_PARTY_MODE_FLAG_BASE = 0x04000, /* shifted left by the party mode: which new-game party was built */
    OPEN_PARTY_MODE_FLAG_MASK = 0x1c000,
    /* open_movie_present_frame: clears the frame, or copies it from VRAM (512, 256), instead of
     * uploading the file image. */
    OPEN_SYSTEM_RUNTIME_FLAG_CLEAR_FRAME = 0x20000,
    OPEN_SYSTEM_RUNTIME_FLAG_FRAME_FROM_VRAM = 0x40000,
    OPEN_SYSTEM_RUNTIME_FLAG_SOFT_RESET_DISABLED = 0x80000,
} open_system_runtime_flag_e;

extern s32 g_open_system_result;
extern u32 g_open_system_runtime_flags;
extern s32 g_open_system_vsync_mode;
extern u8 g_open_work_buffer_0[];
extern u8 g_open_work_buffer_1[];
s32 open_system_run_main_loop(s32 skip_movie);

/* input */
/* The pad as OPEN reads it each frame: the held and newly pressed buttons (none while input is disabled),
 * the Up and Down auto-repeat, and the L1 + R1 + Select + Start soft reset. */

extern u32 g_open_input_current_buttons;
extern s32 g_open_input_left_repeat_counter;
extern s32 g_open_input_right_repeat_counter;
extern s32 g_open_input_directional_repeat_initial_delay;
extern s32 g_open_input_directional_repeat_period;
extern s32 g_open_input_down_repeat_counter;

/* Volatile: open_menu_handle_sound_type_input reloads it (lui/lw) for every
 * test; readers that test it twice from one load take a local copy. */
extern volatile u32 g_open_input_new_button_presses;

extern u32 g_open_input_polled_buttons;
extern u32 g_open_input_previous_buttons;
extern s32 g_open_input_up_repeat_counter;
u32 open_input_check_repeating_directional(u32 buttons);

/* file */
/* OPEN's disc reads through MAIN's loader, one at a time: the OPNTEX.BIN textures and the OPNBK.BIN
 * pictures (the title's frame, the prologue's pictures and the ending's). */

extern s32 g_open_file_current_openbk_image_id;
extern const s32 g_open_file_openbk_end_sector_offsets[];
extern const s32 g_open_file_openbk_start_sector_offsets[];

/* card */
/* The title's memory card check: whether a card answers in either slot, through the card events and
 * _card_info. */

extern u8 g_open_card_info_pending;
extern u8 g_open_card_slot;
s32 open_card_get_info_with_retries(s32 port, s32 retry_limit);
s32 open_card_poll_info_until_stable(s32 retry_limit);

/* gfx */
/* OPEN's drawing: the double-buffered 256- or 320-pixel screen, the render records (36-byte animated
 * sprites and 56-byte window images, drawn each frame from a pointer list) with their shared primitive pool,
 * and the full-screen fade overlay. */

/* world_animated_render_record_t.palette of the selected menu entry. */
enum { OPEN_MENU_ENTRY_HIGHLIGHTED_PALETTE = 9 };

enum { OPEN_GFX_RENDER_RECORD_CAPACITY = 16 };

/* Render records. open_gfx_draw_render_record_pointer_list dispatches on flags
 * (world_render_record_flag_e): ANIMATED is a 36-byte record, IMAGE a
 * 56-byte record, and a record with BLANK or HIDDEN set is skipped.
 * Each pool holds OPEN_GFX_RENDER_RECORD_CAPACITY entries. */
/* The render queue holds either record format; its leading 32-bit flags value selects
 * the concrete layout. */

/* CLUT position and source rectangle at +0x24 of a 56-byte record.
 * open_gfx_init_window_frame_record takes its address as one pointer. */
typedef struct open_render_record_56_tail {
    /* 0x00 */ open_point32_t clut; /* open_gfx_draw_window_record passes it to GetClut */
    /* 0x08 */ RECT quad;           /* quarter-pixel source rectangle */
} open_render_record_56_tail_t;

typedef struct open_render_record_56 {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 vram_x; /* quad.x / 4 + 64 * depth + 0x180 */
    /* 0x08 */ s32 vram_y;
    /* 0x0c */ s32 ot_layer;
    /* 0x10 */ s32 depth;
    /* 0x14 */ s32 pop_in_step;
    /* 0x18 */ s32 palette;
    /* 0x1c */ s32 x; /* quad.x - 0x80 */
    /* 0x20 */ s32 y; /* quad.y - 0x78 */
    /* 0x24 */ open_render_record_56_tail_t tail;
    /* 0x34: a sibling of `tail`, not part of it. open_gfx_init_window_frame_record
     * computes its address as records + index * 0x38 + 0x34, which a nested
     * `&rec[i].tail.r` cannot give (CSE rewrites that as tail + 0x10). */
    CVECTOR color;
} open_render_record_56_t;
typedef char open_render_record_56_size_must_be_56[(sizeof(open_render_record_56_t) == 56) ? 1 : -1];

/* x/y view of the 56-byte render records, based at records_56[0].x; the
 * retail code addresses that pair through its own base register, loaded
 * before the index is scaled, which &records_56[i].x does not reproduce. */
typedef struct open_render_record_56_position {
    /* 0x00 */ s32 x; /* == g_open_gfx_render_records_56[i].x */
    /* 0x04 */ s32 y;
    /* 0x08 */ u8 _unused_08[0x38 - 8];
} open_render_record_56_position_t;

/* open_overlay_fade_t.flags. */
typedef enum open_overlay_fade_flag {
    OPEN_OVERLAY_FADE_OUT = 0x01,    /* the overlay clears */
    OPEN_OVERLAY_FADE_IN = 0x02,     /* the overlay darkens the screen */
    OPEN_OVERLAY_FADE_ACTIVE = 0x04, /* the overlay is drawn; cleared when either fade ends */
} open_overlay_fade_flag_e;

/* One DR_MODE + SPRT pair of the shared 32-entry per-buffer sprite pool. */
typedef struct open_sprite_prim {
    /* 0x00 */ DR_MODE mode;
    /* 0x0c */ SPRT sprt;
} open_sprite_prim_t;
typedef char open_sprite_prim_size_must_be_32[(sizeof(open_sprite_prim_t) == 32) ? 1 : -1];

/* Double-buffered draw/display environments at 0x800851c0, one pair per frame buffer (0x70 bytes each);
 * open_gfx_init_screen_environments fills both and the movie presenter flips between them. */
typedef struct open_screen_environment {
    DRAWENV draw; /* 0x00 */
    DISPENV disp; /* 0x5c */
} open_screen_environment_t;

/* OPEN screen overlay fade at 0x80085cac..0x80085d03: one record, since
 * open_gfx_update_fade_overlay reaches flags..rect and the tiles/modes from a
 * single base. */
typedef struct open_overlay_fade {
    /* 0x00 */ u32 flags; /* open_overlay_fade_flag_e */
    /* 0x04 */ s32 frame;
    /* 0x08 */ s32 duration;
    /* 0x0c */ s32 level;
    /* 0x10 */ s32 abr;
    /* 0x14 */ s32 ot_index;
    /* 0x18 */ RECT rect;
    /* 0x20 */ TILE tiles[2];
    /* 0x40 */ DR_MODE modes[2];
} open_overlay_fade_t;

/* Title menu controller (handler 2), pushed into the next controller record by open_title_push_menu_controller and run
 * by open_title_update_menu. The sound type menu sees the same record as the logo's render record and the four entries'
 * records, which it hides and shows again (open_gfx_hide_four_render_records, open_gfx_show_four_render_records). */
typedef struct open_title_controller {
    /* 0x00 */ s32 records_36[5]; /* logo, then the four menu items */
    /* 0x14 */ s32 idle_timer;
    /* 0x18 */ s32 cd_end_position;
    /* 0x1c */ s32 cursor; /* open_title_menu_entry_e */
    /* 0x20 */ s32 state;  /* open_title_state_e */
    /* 0x24 */ s32 cursor_record_36;
    /* 0x28 */ s32 _unused_28[2];
    /* 0x30 */ s32 exit_timer;
    /* 0x34 */ s32 exiting;
    /* 0x38 */ u8 _unused_38[0x64 - 0x38];
} open_title_controller_t;

extern POLY_FT4 g_open_gfx_fade_primitives[2];
extern s32 g_open_gfx_next_render_record_36;
extern s32 g_open_gfx_next_render_record_56;
extern u32* g_open_gfx_opntex_data;
extern u32 g_open_gfx_otags[2][16];
extern s32 g_open_gfx_primitive_count;
extern open_render_record_56_position_t g_open_gfx_record_translation[OPEN_GFX_RENDER_RECORD_CAPACITY];
extern s32 g_open_gfx_render_record_pointer_count;
extern world_animated_render_record_t g_open_gfx_render_records_36[OPEN_GFX_RENDER_RECORD_CAPACITY];
extern open_render_record_56_t g_open_gfx_render_records_56[OPEN_GFX_RENDER_RECORD_CAPACITY];
extern open_screen_environment_t g_open_gfx_screen_environments[2];
extern open_overlay_fade_t g_open_gfx_overlay_fade;
extern open_sprite_prim_t g_open_gfx_sprite_primitive_pool[2][32];
extern POLY_FT4 g_open_gfx_credit_band_primitives[2];
extern wldcore_sprite_animation_sequence_t** g_open_gfx_record_36_animations;
extern world_sprite_cel_header_t** g_open_gfx_record_36_sprites;

/* movie */
/* The STR movies, each run by a controller: FFTST.STR (whose audio is the title music), the title demos
 * FFTPRE, FFTPR and FFTUNIT, FFTOP after the birthday and FFTEND at the ending, streamed through a CD ring
 * into the MDEC. */

/* Movie controllers' view (handlers 0, 1, 9 and 11): the stream the FFTST and title demo movies play. */
typedef struct open_movie_stream_controller_state {
    s32 start_sector;
    s32 stream_length;
} open_movie_stream_controller_state_t;

/* Shared VLC/MDEC state used by OPEN.BIN's movie-stream pipeline. */
typedef struct open_movie_mdec_stream_state {
    u32* vlc_buffers[2];     /* 0x00; decoded run-level data in 32-bit entries */
    s32 vlc_buffer_index;    /* 0x08 */
    u32* output_buffers[2];  /* 0x0c; decoded image data in 32-bit entries */
    s32 output_buffer_index; /* 0x14 */
    RECT frame_rects[2];     /* 0x18 */
    s32 frame_rect_index;    /* 0x28 */
    RECT output_rect;        /* 0x2c */
    s32 output_ready;        /* 0x34 */
} open_movie_mdec_stream_state_t;
typedef char open_movie_mdec_stream_state_size_must_be_56[(sizeof(open_movie_mdec_stream_state_t) == 56) ? 1 : -1];

/* g_open_movie_stream_status, set as each frame header arrives. */
typedef enum open_movie_stream_status {
    OPEN_MOVIE_STREAM_PLAYING = 0,
    OPEN_MOVIE_STREAM_STOP_AND_PAUSE_AUDIO = 1, /* reached g_open_movie_end_frame */
    OPEN_MOVIE_STREAM_STOP = 2,                 /* reached g_open_movie_video_end_frame; the CD audio plays on */
} open_movie_stream_status_e;

extern s32 g_open_movie_aligned_height;
extern u32 g_open_movie_cd_stream_ring_buffer[];
extern u32 g_open_movie_current_frame;
extern u32 g_open_movie_end_frame;
extern s32 g_open_movie_height;
extern s32 g_open_movie_video_end_frame; /* -1: none */
extern u32 g_open_movie_mdec_output_buffer_0[];
extern u32 g_open_movie_mdec_output_buffer_1[];
extern open_movie_mdec_stream_state_t g_open_movie_mdec_stream_state;
extern open_movie_stream_status_e g_open_movie_stream_status;
extern s32 g_open_movie_width;

/* OPEN.BIN interfaces shared across its reconstructed translation units. */
u32* open_movie_acquire_next_frame(open_movie_mdec_stream_state_t* state);

s32 open_movie_decode_next_frame(open_movie_mdec_stream_state_t* state);

/* title */
/* The title menu (New Game, Continue, Tutorial, Sound), the memory card warning before a new game
 * without a card, and the New Game controller that runs the prologue, the name entry and the birthday. */

/* New-game start controller (handler 4). */
typedef struct open_title_new_game_state {
    /* 0x00 */ u8 _unused_00[0x10];
    /* 0x10 */ s32 step;
    /* 0x14 */ u8 _unused_14[0x0c];
    /* 0x20 */ s32 name_text_id;
} open_title_new_game_state_t;

/* Title exit controller (handler 12). */
typedef struct open_title_card_warning {
    s32 state;
} open_title_card_warning_t;

/* open_title_controller_t.cursor: the four title menu entries. */
typedef enum open_title_menu_entry {
    OPEN_TITLE_MENU_NEW_GAME = 0,
    OPEN_TITLE_MENU_CONTINUE = 1,
    OPEN_TITLE_MENU_TUTORIAL = 2,
    OPEN_TITLE_MENU_SOUND = 3,
} open_title_menu_entry_e;

/* open_title_controller_t.state. */
typedef enum open_title_state {
    OPEN_TITLE_STATE_MENU = 0,
    OPEN_TITLE_STATE_SOUND_MENU = 2, /* pushes the sound type menu on the next update */
    OPEN_TITLE_STATE_DEMO_MOVIE = 3, /* idle timeout: starts the next demo movie once the screen has faded */
} open_title_state_e;

/* 32-bit state of the title-exit controller (open_title_update_card_warning). */
typedef enum open_title_card_warning_step {
    OPEN_TITLE_CARD_WARNING_INPUT = 0,
    OPEN_TITLE_CARD_WARNING_BACK = 1,     /* cross: back to the title menu */
    OPEN_TITLE_CARD_WARNING_CONTINUE = 2, /* circle or start: the new-game transition */
} open_title_card_warning_step_e;

extern s32 g_open_title_demo_movie_index;

/* menu */
/* The title's sub-menus: the Sound type menu (mono, stereo or wide) and the hidden Music Test, whose
 * track list runs on WORLD's menu frame. */

/* The slot of the music test's list thread. */
enum {
    OPEN_THREAD_SLOT_MUSIC_TEST_LIST = 12,
};

/* Controller 7 (hidden Music Test menu) state, pushed by open_menu_start_music_test_controller. */
typedef struct open_menu_sound_test_state {
    /* 0x00 */ s32 music_id;     /* scenario music selected for playback */
    /* 0x04 */ s32 delay;        /* frames before loading music_id */
    /* 0x08 */ s32 music_slot;   /* slot from main_sound_load_music_into_free_slot */
    /* 0x0c */ s32 step;         /* open_music_test_step_e */
    /* 0x10 */ s32 blink_frames; /* counts ten frames after a selection, while Up and Down stay blocked */
} open_menu_sound_test_state_t;

/* Step of the music test controller (open_menu_update_music_test). */
typedef enum open_music_test_step {
    OPEN_MUSIC_TEST_STEP_IDLE = 0,
    OPEN_MUSIC_TEST_STEP_CLOSE = 1,       /* waits for the list thread, then fades the screen */
    OPEN_MUSIC_TEST_STEP_FADE_OUT = 2,    /* waits for the fade, then returns to the title menu */
    OPEN_MUSIC_TEST_STEP_START_TRACK = 3, /* waits for the music to load, then plays it */
    OPEN_MUSIC_TEST_STEP_OPEN = 4,        /* waits for the fade, then starts the list thread */
} open_music_test_step_e;

typedef enum game_sound_mode {
    GAME_SOUND_MODE_MONO = 0,
    GAME_SOUND_MODE_STEREO = 1,
    GAME_SOUND_MODE_WIDE = 2,
} game_sound_mode_e;

/* Pad buttons withheld from the WORLD menu frame (open_menu_run_world_menu_frame); -1 blocks all. */
extern s32 g_open_menu_blocked_buttons;

extern s32 g_open_menu_world_menu_otag_index;
extern s16 g_open_music_test_row_actions[16];
extern world_menu_entry_t g_open_music_test_list_params;
extern world_menu_text_layout_t g_open_music_test_list_state;
extern s16 g_open_music_test_list_entry_flags[96];
extern s16 g_open_music_test_list_entries[96];
extern s16 g_open_music_test_selected_option;
extern u8 g_open_music_test_music_ids[];               /* music test entry -> scenario music id */
extern suzuki_music_t* g_open_music_test_sound_handle; /* handle of the playing music test track */

/* prologue */
/* Alazlam's prologue after New Game: pages of narration lines revealed from two OPNTEX text pages over
 * OPNBK pictures that crossfade from the bottom up, before the name entry. */

/* New-game prologue entry: a packed line count, texture-page position and
 * image operation, followed by four-byte texture-row/width records. */
typedef struct open_gfx_transition_line {
    u8 texture_row;
    u8 width;
    u8 _unused_02[2];
} open_gfx_transition_line_t;
typedef char open_gfx_transition_line_size_must_be_4[sizeof(open_gfx_transition_line_t) == 4 ? 1 : -1];

/* One page of the prologue: a header word (bits 0-7 the line count, bits 8-12 the text page's texture-page position,
 * bits 16-23 the image operation) followed by one four-byte step per line, whose value byte is the line's 16-pixel row
 * in the text page and duration byte its width. */
typedef struct open_prologue_entry {
    u32 header;
    u8 line_data[0]; /* variable number of encoded open_gfx_transition_line_t records */
} open_prologue_entry_t;
typedef char open_prologue_entry_size_must_be_4[sizeof(open_prologue_entry_t) == 4 ? 1 : -1];

enum {
    OPEN_GFX_TRANSITION_IMAGE_OPERATION_MASK = 0xf0,
    OPEN_GFX_TRANSITION_IMAGE_INDEX_MASK = 0x0f,
};

/* High nibble of the image command byte in open_prologue_entry_t.header. */
typedef enum open_gfx_transition_image_operation {
    OPEN_GFX_TRANSITION_PREPARE_IMAGE = 0x10,
    OPEN_GFX_TRANSITION_START_CROSSFADE = 0x20,
} open_gfx_transition_image_operation_e;

/* open_prologue_t.flags. */
typedef enum open_prologue_flag {
    OPEN_PROLOGUE_NEXT_ENTRY = 0x01,  /* starts the next entry of the table, or ends at the target */
    OPEN_PROLOGUE_NEXT_STEP = 0x02,   /* starts the next step of the entry */
    OPEN_PROLOGUE_WAIT_CIRCLE = 0x08, /* the entry is complete: waits for circle */
    OPEN_PROLOGUE_DIM = 0x10,         /* the line strips fade out before the next entry */
    OPEN_PROLOGUE_SKIP = 0x20,        /* start was pressed: fades out and ends */
} open_prologue_flag_e;

/* The prologue's state at 0x800855b4: the pages to show, the page and line in progress and the brightness of its text.
 */
typedef struct open_prologue {
    /* 0x00 */ u16 flags;    /* open_prologue_flag_e */
    /* 0x02 */ s16 ot_layer; /* text and shadow occupy this layer and the next */
    /* 0x04 */ s16 current_index;
    /* 0x06 */ s16 target_index;
    /* 0x08 */ s16 step;
    /* 0x0a */ s16 step_count;
    /* 0x0c */ s16 tpage_x; /* (header & 0xf00) >> 2; GetTPage x */
    /* 0x0e */ s16 tpage_y; /* (header >> 4) & 0x100; GetTPage y */
    /* 0x10 */ s16 line_texture_rows[7];
    /* 0x1e */ s16 line_widths[7];
    /* 0x2c */ s16 timer;
    /* 0x2e */ s16 intensity;
    /* 0x30 */ open_prologue_entry_t** entry_table;
    /* 0x34 */ open_prologue_entry_t* entry;
} open_prologue_t;
typedef char open_prologue_size_must_be_56[(sizeof(open_prologue_t) == 56) ? 1 : -1];

extern u16* g_open_prologue_crossfade_destination;
extern u16* g_open_prologue_crossfade_file_pixels;

/* Volatile: open_prologue_step_crossfade reloads it every column. */
extern u16* volatile g_open_prologue_crossfade_source;

extern u8 g_open_prologue_crossfade_progress;
extern u8 g_open_prologue_crossfade_row_flags[];
extern open_prologue_t g_open_prologue;
extern POLY_GT4 g_open_prologue_line_primitives[2][14];
extern open_prologue_entry_t** g_open_prologue_entries;

/* birthday */
/* The birthday entry after the name: the date controller, the Yes/No confirmation, and the day of the
 * year and zodiac sign stored in Ramza's record. */

/* Birthday date-selection controller state shared with its redraw routine. */
typedef struct open_birthday_date_state {
    s32 cursor_record_36; /* 0x00: cursor moved between month and day */
    s32 label_record;
    s32 title_record_56;  /* 0x08: birthday title image (0xb800); palette 2 on
                             confirmation, 0 on restore */
    s32 window_record_56; /* 0x0c: flags bit 8 blocks input */
    s32 _unused_10[4];
    s32 month;           /* 0x20: 1..12 */
    s32 day;             /* 0x24 */
    s32 selecting_month; /* 0x28: 0 edits the day, 1 edits the month */
} open_birthday_date_state_t;

/* Final birthday-confirmation controller state. */
typedef struct open_birthday_confirmation_state {
    s32 cursor_record_36; /* 0x00 */
    s32 label_record;     /* 0x04 */
    s32 menu_record_56;   /* 0x08 */
    s32 selected_option;  /* 0x0C: 0 confirms, 1 returns to date entry */
    s32 _unused_10[4];
    s32 month; /* 0x20 */
    s32 day;   /* 0x24 */
    u8 _unused_28[0x64 - 0x28];
} open_birthday_confirmation_state_t;
typedef char
    open_birthday_confirmation_state_size_must_be_100[(sizeof(open_birthday_confirmation_state_t) == 100) ? 1 : -1];

/* Sound-type menu controller state (handler 3). */
enum { OPEN_SOUND_MENU_ENTRY_COUNT = 3 };

/* menu */
/* More of the menu block: its types, after the declarations above that they use. */

typedef struct open_sound_menu_state {
    /* 0x00 */ s32 entry_render_indices[OPEN_SOUND_MENU_ENTRY_COUNT]; /* Mono, Stereo and Wide labels */
    u8 _unused_0c[16];
    /* 0x1c */ s32 sound_mode_selection; /* Signed so decrement can wrap below Mono. */
    /* 0x20 */ s32 step;
} open_sound_menu_state_t;
typedef char open_sound_menu_state_size_must_be_36[sizeof(open_sound_menu_state_t) == 36 ? 1 : -1];

/* birthday */
/* More of the birthday block: its globals and functions, after the declarations above that they use. */

/* Days in each month of the birthday entry. February has 28, so 29 February cannot be entered. */
extern u8 g_open_birthday_month_lengths[12];

extern u8 g_open_birthday_title_image[];
extern u8 g_open_birthday_window_image[];

/* Start date of each ordinary sign as {month, day} byte pairs, Aries first
 * ({3, 21}, {4, 20}, ...). open_birthday_to_zodiac_date indexes
 * it flat ([i * 2], [i * 2 + 1]), and it is not const: that routine reloads a
 * day after storing through its month pointer. */
extern u8 g_open_birthday_zodiac_start_dates[ZODIAC_SIGN_ORDINARY_COUNT * 2];

s32 open_birthday_to_day_of_year(s32 month, s32 day);

/* controller */
/* The controller stack that runs every OPEN screen: each frame the current controller's handler
 * (open_controller_handler_e) runs on its 100-byte record, and a screen pushes the next controller or pops
 * back to the one below. */

/* Text-message controller (handler 8): whether to restore the text section pointers afterwards, and the formation entry
 * mask to restore. */
typedef struct open_controller_thread_completion {
    s32 run_followup;
    s32 saved_blocked_buttons; /* g_open_menu_blocked_buttons when the message started */
} open_controller_thread_completion_t;

/* One 100-byte state record per controller. The current controller c runs
 * g_open_controller_handlers[g_open_controller_handler_indices[c - 1]] on g_open_controller_records[c]. A push made
 * while the current index is c fills g_open_controller_records_next[c] (which is records[c + 1]) and stores the new
 * handler id at handler_indices[c]; that id picks the view. */
typedef union open_controller_record {
    open_movie_stream_controller_state_t movie;               /* handlers 0, 1, 9, 11 */
    open_title_controller_t title;                            /* 2 */
    open_sound_menu_state_t sound_menu;                       /* 3 */
    open_title_new_game_state_t new_game;                     /* 4 */
    open_birthday_date_state_t birthday_date;                 /* 5 */
    open_birthday_confirmation_state_t birthday_confirmation; /* 6 */
    open_menu_sound_test_state_t music_test;                  /* 7 */
    open_controller_thread_completion_t text_message;         /* 8 */
    open_title_card_warning_t card_warning;                   /* 12 */
} open_controller_record_t;
typedef char open_controller_record_size_must_be_100[(sizeof(open_controller_record_t) == 100) ? 1 : -1];

typedef void (*open_controller_handler_t)(open_controller_record_t* record);

/* system */
/* More of the system block: its functions, after the declarations above that they use. */

void open_system_init_runtime_state(void);
void open_system_run_ending_cutscene(void);
void open_system_init_ending(void);
void open_system_init_title_sequence(s32 skip_movie);

/* input */
/* More of the input block: its functions, after the declarations above that they use. */

void open_input_init_directional_state(void);
void open_input_update_buttons_and_check_game_reset(void);

/* file */
/* More of the file block: its globals and functions, after the declarations above that they use. */

extern void* g_open_file_destination;
void open_file_build_openbk_header(s32 image_id, void* destination);
void open_file_load_end_movie_graphics(void);
void open_file_load_main_menu_graphics(void);
void open_file_load_prologue_text_pages(void);
void open_file_wait_for_pending(void);

void open_file_wait_then_build_header(
    main_file_load_descriptor_t* header, s32 sector, s32 sector_count, void* destination);

s32 open_file_get_cd_sync_state_delta(void);

/* card */
/* More of the card block: its functions, after the declarations above that they use. */

s32 open_card_check_slots(void);
s32 open_card_poll_info_result(void);
void open_card_set_slot(u8 slot);
s32 open_card_wait_for_bios_event(void);
s32 open_card_wait_for_hardware_event(void);

/* gfx */
/* More of the gfx block: its globals and functions, after the declarations above that they use. */

/* Both render pools publish record pointers to this queue. */
extern void* g_open_gfx_render_record_pointers[];

void open_gfx_draw_animated_render_record(world_animated_render_record_t* actor, u32* ot);
void open_gfx_draw_window_record(open_render_record_56_t* record, u32* ot);
s32 open_gfx_append_animated_render_record(void** list, s32* count);
s32 open_gfx_append_window_record(void** list, s32* count);
void open_gfx_restore_frame_vram_page(void);
void open_gfx_clear_and_draw_current_frame(void);
void open_gfx_clear_current_otag(void);
void open_gfx_draw_render_record_pointer_list(u32* otag, void** records, s32 count);
void open_gfx_show_four_render_records(open_title_controller_t* title);
void open_gfx_init_render_buffers(void);
void open_gfx_init_screen_environments(s32 clear_first);
void open_gfx_init_window_frame_record(s32 record_index, RECT quad, s32 depth, u16* image);
void open_gfx_load_opntex_into_frame_buffer(u32* tim);
void open_gfx_hide_four_render_records(open_title_controller_t* title);
void open_gfx_start_overlay_fade_in(u32 duration);
void open_gfx_start_overlay_fade_out(u32 duration);
void open_gfx_update_fade_overlay(u32* otag);
void open_gfx_update_opntex_sequence(void);

/* sound */
/* The CD audio fade shared by the movies, the title music and the ending's XA tracks. */

/* Fades the CD audio in at the given mix level (suzuki_sound_put_type), or out when it is 0. */
void open_sound_set_cd_mix_and_volume(s32 cd_mix_level, s32 duration);

/* movie */
/* More of the movie block: its functions, after the declarations above that they use. */

void open_movie_handle_mdec_output_slice(void);
void open_movie_init_mdec_stream_state(open_movie_mdec_stream_state_t* state, s32 x0, s32 y0, s32 x1, s32 y1);
void open_movie_init_stream(s32 sector, psyq_interrupt_callback_t output_callback);
void open_movie_pause_cd_audio(void);
void open_movie_start_fftend(void);
void open_movie_present_frame(void);
void open_movie_start_cd_stream_read(const CdlLOC* location);
void open_movie_start_fftst_or_title_menu(s32 use_alternate);
void open_movie_update_demo_controller(open_controller_record_t* unused_record);
void open_title_update_card_warning(open_controller_record_t* record);
void open_movie_start_stream(s32 sector, s32 end_frame, s32 video_end_frame, s32 cd_mix_level);
void open_movie_stop_stream(s32 pause_cd_audio);
void open_movie_push_fftop_controller(s32 start_sector, s32 unused_stream_length, s32 end_frame);
void open_movie_push_fftend_controller(s32 start_sector, s32 unused, s32 end_frame);

void open_movie_push_fftst_controller(s32 start_sector, s32 stream_length, s32 end_frame, s32 video_end_frame);

void open_movie_push_demo_controller(s32 start_sector, s32 stream_length, s32 end_frame, s32 cd_mix_level);

void open_movie_update_stream_frame(void);
void open_movie_wait_for_mdec_output(open_movie_mdec_stream_state_t* state, s32 unused);

/* title */
/* More of the title block: its functions, after the declarations above that they use. */

void open_title_push_menu_controller(s32 argument);
void open_title_init_new_game_party(s32 party_mode, s32 world_load_mode);
void open_title_start_new_game(void);

/* menu */
/* More of the menu block: its functions, after the declarations above that they use. */

void open_menu_start_music_test_list_thread(s32 thread_id, s32 limit, open_point32_t* extent, open_point32_t* origin);
void open_menu_start_music_test_controller(void);
void open_menu_run_world_menu_frame(void);
void open_menu_push_sound_type_controller(void);

void open_menu_init_render_record_fields(
    s32 record_index, point16_t vram_position, point16_t image_size, open_point32_t clut_position, u32 flags);

/* prologue */
/* More of the prologue block: its functions, after the declarations above that they use. */

void open_prologue_start(void);
s32 open_prologue_update(void);
void open_prologue_draw_text_lines(s32 intensity);
void open_prologue_prepare_crossfade(s32 file_index);
void open_prologue_request(s32 target, s32 ot_layer, open_prologue_entry_t** entry_table);
void open_prologue_step_crossfade(void);
void open_prologue_finished_hook(void);

/* birthday */
/* More of the birthday block: its functions, after the declarations above that they use. */

void open_birthday_push_date_controller(void);
void open_birthday_build_confirmation_menu(open_birthday_date_state_t* state);
void open_birthday_build_menu_text(open_birthday_date_state_t* menu);
void open_birthday_to_zodiac_date(s32* month, s32* day);
void open_birthday_draw_menu_text_entry(s32 record_index, s32 combined_text_index, open_point32_t position, u8* image);
void open_birthday_handle_confirmation_input(open_controller_record_t* record);
void open_menu_update_music_test(open_controller_record_t* record);
void open_controller_wait_text_message(open_controller_record_t* record);
void open_movie_update_fftend_controller(open_controller_record_t* unused_record);
void open_birthday_load_window_image(s32 record_index, u32* image);
void open_birthday_push_confirmation_controller(void);
void open_birthday_restore_date_menu(open_birthday_date_state_t* menu);

/* controller */
/* More of the controller block: its types, globals and functions, after the declarations above that they use. */

/* Rows of g_open_controller_handlers: the value a push stores in
 * g_open_controller_handler_indices. */
typedef enum open_controller_handler {
    OPEN_CONTROLLER_FFTST_MOVIE = 0,           /* open_movie_update_fftst_controller */
    OPEN_CONTROLLER_FFTOP_MOVIE = 1,           /* open_movie_update_fftop_controller */
    OPEN_CONTROLLER_TITLE_MENU = 2,            /* open_title_update_menu */
    OPEN_CONTROLLER_SOUND_TYPE_MENU = 3,       /* open_menu_handle_sound_type_input */
    OPEN_CONTROLLER_NEW_GAME_START = 4,        /* open_title_step_new_game_start */
    OPEN_CONTROLLER_BIRTHDAY_DATE = 5,         /* open_birthday_handle_menu_input */
    OPEN_CONTROLLER_BIRTHDAY_CONFIRMATION = 6, /* open_birthday_handle_confirmation_input */
    OPEN_CONTROLLER_MUSIC_TEST = 7,            /* open_menu_update_music_test */
    OPEN_CONTROLLER_TEXT_MESSAGE = 8,          /* open_controller_wait_text_message */
    OPEN_CONTROLLER_FFTEND_MOVIE = 9,          /* open_movie_update_fftend_controller */
    OPEN_CONTROLLER_SCRIPT = 10,               /* open_script_update_controller */
    OPEN_CONTROLLER_TITLE_DEMO_MOVIE = 11,     /* open_movie_update_demo_controller */
    OPEN_CONTROLLER_CARD_WARNING = 12,         /* open_title_update_card_warning */
} open_controller_handler_e;

extern s32 g_open_controller_handler_indices[]; /* open_controller_handler_e */
extern open_controller_handler_t g_open_controller_handlers[];
extern open_controller_record_t g_open_controller_records[];
extern open_controller_record_t g_open_controller_records_next[];
extern s32 g_open_controller_selected_options[];
extern s32 g_open_current_controller_index;
void open_controller_dispatch_current(void);
void open_controller_start_text_message(s32 parameter, s32 run_followup);
void open_movie_update_fftst_controller(open_controller_record_t* record);
void open_movie_update_fftop_controller(open_controller_record_t* unused_record);
void open_title_update_menu(open_controller_record_t* record);
void open_menu_handle_sound_type_input(open_controller_record_t* record);
void open_title_step_new_game_start(open_controller_record_t* record);
void open_birthday_handle_menu_input(open_controller_record_t* record);

/* text */
/* Text drawn into OPEN's window images, and the hex digits of the credits script's escapes. */

s32 open_text_decode_hex_digit(s32 character);
void open_text_measure_menu_image(s32 combined_text_index, wldcore_text_dimensions_t* dimensions);
void open_text_init_menu_render_record(s32 record_index, RECT quad, s32 depth, s32 combined_text_index, u8* image);

/* script */
/* The ending's credits script (open_script_*, opcodes open_opcode_*): text lines that scroll over the
 * OPNTEX pictures, the ENDING.XA music and the fades, run by the OPEN_CONTROLLER_SCRIPT controller. */

typedef enum open_script_dispatch_flag {
    OPEN_SCRIPT_DISPATCH_RUNNING = 0x01,  /* cleared to stop the dispatcher and hide the text records */
    OPEN_SCRIPT_DISPATCH_CONTINUE = 0x02, /* runs another entry this frame */
    OPEN_SCRIPT_DISPATCH_WAITING = 0x04,  /* a wait opcode is in progress */
    OPEN_SCRIPT_DISPATCH_XA_PLAYING = 0x08,
    OPEN_SCRIPT_DISPATCH_XA_STOPPING = 0x10, /* the stop sequence of open_script_update_xa_audio runs */
    OPEN_SCRIPT_DISPATCH_XA_STARTING = 0x20, /* its start sequence runs */
    OPEN_SCRIPT_DISPATCH_INCREASE_FADE_INTENSITY = 0x40,
    OPEN_SCRIPT_DISPATCH_DECREASE_FADE_INTENSITY = 0x80,
} open_script_dispatch_flag_e;

/* open_script_record_t.flags. */
typedef enum open_script_record_flag {
    OPEN_SCRIPT_RECORD_ACTIVE = 0x01, /* the record holds a text being shown */
} open_script_record_flag_e;

typedef struct open_script_record {
    u32 flags; /* open_script_record_flag_e */
    s32 glyph_count;
    s32 skew; /* 0x08: italic skew added to the top edge of each glyph quad */
    s32 x;
    s32 y;
    RECT glyphs[42];
} open_script_record_t;
typedef char open_script_record_size_must_be_356[(sizeof(open_script_record_t) == 356) ? 1 : -1];

/* Byte-addressed ending-script cursor. */
typedef struct open_script_cursor {
    u8* data_base;
    s32 byte_offset;
} open_script_cursor_t;
typedef char open_script_cursor_size_must_be_8[(sizeof(open_script_cursor_t) == 8) ? 1 : -1];

/* OPEN script interpreter state at 0x8008e548. */
typedef struct open_script_dispatch_state {
    /* 0x00 */ u32 flags; /* open_script_dispatch_flag_e */
    /* 0x04 */ s32 tpage;
    /* 0x08 */ s32 clut;
    /* 0x0c */ s32 font_index; /* each font has 0x380 bytes of glyph metrics */
    /* 0x10 */ s32 skew;       /* copied into each text record's skew */
    /* 0x14 */ s32 wide_glyphs;
    /* 0x18 */ s32 timing_step;
    /* 0x1c */ s32 timing_fraction;
    /* 0x20 */ s32 external_counter;
    /* 0x24 */ s32 wait_counter;
    /* 0x28 */ open_script_cursor_t cursor;
} open_script_dispatch_state_t;
typedef char open_script_dispatch_state_size_must_be_48[(sizeof(open_script_dispatch_state_t) == 48) ? 1 : -1];

/* OPNTEX frame sequence and the script screen fade it starts. */
typedef struct open_opntex_sequence_state {
    /* 0x00 */ s32 fade_elapsed_frames;
    /* 0x04 */ s32 fade_duration_frames;
    /* 0x08 */ s32 fade_intensity;
    /* 0x0c */ s32 first_frame;
    /* 0x10 */ s32 last_frame;
    /* 0x14 */ s32 frame_duration;
    /* 0x18 */ s32 frame_countdown;
    /* 0x1c */ s32 delay;
} open_opntex_sequence_state_t;

/* The dispatcher state, the 16 text records and the control/OPNTEX tail
 * (0x8008e548..0x8008fbef) are one object: open_gfx_update_opntex_sequence
 * reaches dispatch.flags at -0x1698 from its &opntex.frame_duration base. */
typedef struct open_script_state {
    /* 0x0000 */ open_script_dispatch_state_t dispatch;
    /* 0x0030 */ open_script_record_t records[16];
    /* 0x1670 */ s32 _unused_1670;
    /* 0x1674 */ s32 _unknown_1674;     /* cleared by open_script_init_state */
    /* 0x1678 */ s32 control_parameter; /* XA channel of ENDING.XA to play */
    /* 0x167c */ s32 control_state;     /* step of the XA start or stop sequence (open_script_update_xa_audio) */
    /* 0x1680 */ s32 xa_wait_frames;    /* frames since the XA read started */
    /* 0x1684 */ open_opntex_sequence_state_t opntex;
    /* 0x16a4 */ s32 opntex_control_value;
} open_script_state_t;
typedef char open_script_state_size_must_be_5800[(sizeof(open_script_state_t) == 5800) ? 1 : -1];

extern open_script_entry_handler_t g_open_opcode_handlers[16];

/* Eight-byte glyph metrics record addressed by OPEN script text. */
typedef struct open_font_metrics {
    u8 u;
    u8 v;
    u8 flags;
    u8 bearing;
    u8 height;
    u8 width;
    u8 descender;
    u8 advance;
} open_font_metrics_t;

extern open_script_state_t g_open_script_state;
extern open_font_metrics_t* g_open_script_font_metrics;

/* Scalar views of 32-bit fields of g_open_script_state. open_opcode_set_timing_step,
 * open_script_scroll_text and
 * open_gfx_update_opntex_sequence load and store each of these by absolute
 * address; the member spelling lets cse keep the address in a register. */
extern s32 g_open_script_byte_offset; /* dispatch.cursor.byte_offset */

extern s32 g_open_script_timing_fraction;  /* dispatch.timing_fraction */
extern s32 g_open_script_external_counter; /* dispatch.external_counter */
extern POLY_FT4 g_open_script_glyph_primitives[2][672];
extern u8* g_open_script_initial_data_base;
void open_opcode_end_script(s16* unused_entry);
void open_opcode_create_positioned_text_record(s16* command);
void open_opcode_set_timing_step(s16* entry);
void open_opcode_start_overlay_fade_out(s16* command);
void open_opcode_start_overlay_fade_in(s16* command);
void open_opcode_wait_counted(s16* entry);
void open_opcode_wait_for_external_counter(s16* entry);
void open_opcode_start_xa_audio(s16* command);
void open_opcode_start_sound_fade_in(s16* command);
void open_opcode_start_sound_fade_out(s16* command);
void open_opcode_stop_xa_audio(s16* unused_entry);
void open_opcode_load_opntex_frame(s16* command);
void open_opcode_start_fade_intensity_increase(s16* command);
void open_opcode_start_fade_intensity_decrease(s16* command);
void open_opcode_wait_for_xa_audio_idle(s16* unused_entry);
void open_opcode_start_opntex_sequence(s16* command);
void open_script_init_state(void);
void open_script_draw_text_records(u32* otag, s32 graphics_buffer_index);
void open_script_update_xa_audio(void);
u8* open_script_add_pointer_offset(u8* base, s32 byte_offset);
s32 open_script_build_text_record(u8* text);
s32 open_script_run_frame(void);
void open_script_push_controller(void);
void open_script_update_controller(open_controller_record_t* unused_record);
void open_script_update_screen_fade(void);
void open_script_scroll_text(void);

#endif
