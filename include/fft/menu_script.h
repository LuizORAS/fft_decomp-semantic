#ifndef MENU_SCRIPT_H
#define MENU_SCRIPT_H

/* Byte-aligned display instructions shared by WORLD, BUNIT, EQUIP and JOBSTTS. */

#include "fft/menu.h"

/* menu */
/* WORLD's menus (world_menu_*): its copy of BATTLE's menu engine, the world menu's rows, the display
 * scripts, scrollable lists and saved positions of the full-screen menus, and the copies of HELPMENU's
 * code. WORLD's copy of OPTION's code is world_option_*. */

/* Ten-byte menu-script "draw number" command: an
 * opcode/length prefix followed by the value getter (a row-callback index),
 * layout and byte position operands. world_menu_script_draw_formatted_number
 * and the overlay copies advance by `length`. */
typedef struct menu_draw_number_command {
    u8 opcode;
    u8 length;
    u8 style;
    u8 getter;
    u8 width;
    u8 spacing;
    u8 alternate_palette;
    u8 x;
    u8 y;
    u8 value;
} menu_draw_number_command_t;

/* Variable-length list command (0x10) in the menu display stream: the layout
 * world_menu_load_scrollable_list_layout reads and the placement
 * world_menu_script_draw_scrollable_list draws with. */
typedef struct world_menu_list_record {
    u8 opcode;          /* 0x00: 0x10 = list layout */
    u8 length;          /* 0x01: byte length of this record */
    u8 show_cursor;     /* 0x02 */
    u8 _unused_03;      /* 0x03 */
    u8 skip_count;      /* 0x04: row records that follow (g_world_menu_list_row_record_count) */
    u8 row_height;      /* 0x05 */
    u8 visible_rows;    /* 0x06 */
    u8 x;               /* 0x07 */
    u8 y;               /* 0x08 */
    u8 width_a;         /* 0x09 */
    u8 width_b;         /* 0x0a */
    u8 cursor_x_offset; /* 0x0b */
    u8 arrow_up_y;      /* 0x0c */
    u8 arrow_down_y;    /* 0x0d */
} world_menu_list_record_t;

/* The menu records BATTLE, WORLD and the event overlays share: window buffers, menu entries with their
 * text bindings and layouts, sprite, list and quad pages, icons, panels and frames, the AT list, the
 * crystal pickup menu, help requests and navigation, the system functions and menu ids, the companion
 * overlays, number entries, and the menu enums. */

/* Byte-aligned commands in the variable-length menu display stream. */
typedef struct menu_sprite_command {
    u8 opcode;
    u8 length;
    u8 _unknown_02;
    battle_menu_sprite_cell_t sprite;
} menu_sprite_command_t;
typedef char menu_sprite_command_size_must_be_9[sizeof(menu_sprite_command_t) == 9 ? 1 : -1];

typedef struct menu_callback_sprite_command {
    u8 opcode;
    u8 length;
    u8 callback_index;
    u8 row;
    u8 x;
    u8 y;
} menu_callback_sprite_command_t;

typedef struct menu_tile_command {
    u8 opcode;
    u8 length;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    rgb24_t color;
} menu_tile_command_t;
typedef char menu_tile_command_size_must_be_9[sizeof(menu_tile_command_t) == 9 ? 1 : -1];

/* LINE_F2 uses endpoint coordinates rather than a rectangle's extents. */
typedef struct menu_line_command {
    u8 opcode;
    u8 length;
    u8 x0;
    u8 y0;
    u8 x1;
    u8 y1;
    rgb24_t color;
} menu_line_command_t;
typedef char menu_line_command_size_must_be_9[sizeof(menu_line_command_t) == 9 ? 1 : -1];

typedef struct menu_draw_area_command {
    u8 opcode;
    u8 length;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} menu_draw_area_command_t;

typedef struct menu_sprite_color_command {
    u8 opcode;
    u8 length;
    rgb24_t normal;
    rgb24_t alternate;
} menu_sprite_color_command_t;
typedef char menu_sprite_color_command_size_must_be_8[sizeof(menu_sprite_color_command_t) == 8 ? 1 : -1];

/* Four-byte menu state commands, including draw priority and blend state. */
typedef struct menu_draw_state_command {
    u8 opcode;
    u8 length;
    u8 _unused_02;
    u8 value;
} menu_draw_state_command_t;

typedef struct menu_fixed_clut_command {
    u8 opcode;
    u8 length;
    u8 y_high;
    u8 x_div_16;
    u8 y_low;
} menu_fixed_clut_command_t;

typedef struct menu_texture_page_command {
    u8 opcode;
    u8 length;
    u8 y_blend; /* bits 0-1: y/256; bits 4-5: blend rate */
    u8 x_div_16;
    u8 depth;
} menu_texture_page_command_t;

/* Seven-byte window command in the menu script streams. The length field
 * advances to the next command, which may have a different size. */
typedef struct world_menu_window_command {
    u8 opcode;
    u8 length;
    u8 rectangle_source;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} world_menu_window_command_t;

/* A view of the current byte-aligned instruction, not a fixed-stride array.
 * Commands advance by their encoded byte length; opcode 0x16 and the module's
 * terminator are single bytes without a length operand. */
typedef union menu_instruction {
    struct {
        u8 opcode;
        u8 length;
    } header;
    struct {
        u8 opcode;
        u8 length;
        u8 _unused_02;
        u8 predicate;
        u8 count;
        u8 value;
    } conditional;
    struct {
        u8 opcode;
        u8 length;
        u8 _unused_02;
        u8 predicate;
        u8 count_true;
        u8 count_false;
        u8 value;
    } if_else;
    world_menu_window_command_t window;
    menu_draw_number_command_t number;
    world_menu_list_record_t list;
    menu_sprite_command_t sprite;
    menu_callback_sprite_command_t callback_sprite;
    menu_tile_command_t tile;
    menu_line_command_t line;
    menu_draw_area_command_t draw_area;
    menu_sprite_color_command_t sprite_color;
    menu_fixed_clut_command_t fixed_clut;
    struct {
        u8 opcode;
        u8 length;
        u8 normal_y_high;
        u8 normal_x_div_16;
        u8 normal_y_low;
        u8 alternate_y_high;
        u8 alternate_x_div_16;
        u8 alternate_y_low;
    } clut;
    menu_texture_page_command_t texture_page;
    menu_draw_state_command_t draw_state;
    struct {
        u8 opcode;
        u8 length;
        u8 blend_rate;
    } blend;
} menu_instruction_t;
typedef char menu_instruction_size_must_be_14[sizeof(menu_instruction_t) == 14 ? 1 : -1];

menu_instruction_t* menu_script_set_draw_color(menu_instruction_t* command);

/* Opcode byte of a menu script command, the index of its handler in the
 * handler table of WORLD, BUNIT, EQUIP and JOBSTTS (CARD calls the same
 * handlers directly). Only the opcodes the code tests are named. Each
 * module ends a stream with its own opcode, the first one past its table
 * (MENU_SCRIPT_OP_WORLD_END, BUNIT_MENU_SCRIPT_END, EQUIP_MENU_SCRIPT_END, JOBSTTS_MENU_SCRIPT_END). */
typedef enum menu_script_opcode {
    MENU_SCRIPT_OP_WINDOW_FRAME = 0,            /* world_menu_script_draw_window_frame */
    MENU_SCRIPT_OP_IF_GROUP = 0x01,             /* *_cmd_run_if_group_handler */
    MENU_SCRIPT_OP_IF_ELSE_GROUP = 0x02,        /* *_cmd_run_if_else_group_handler */
    MENU_SCRIPT_OP_TILE = 3,                    /* world_menu_script_draw_tile */
    MENU_SCRIPT_OP_UNCOLOURED_QUAD = 0x04,      /* *_cmd_draw_textured_quad_handler; opcode 5 applies the draw color */
    MENU_SCRIPT_OP_COLOURED_QUAD = 5,           /* world_menu_script_draw_textured_quad, with the draw color */
    MENU_SCRIPT_OP_CALLBACK_QUAD = 6,           /* world_menu_script_draw_callback_quad */
    MENU_SCRIPT_OP_LINE = 7,                    /* world_menu_script_draw_line */
    MENU_SCRIPT_OP_FIXED_CLUT = 9,              /* world_menu_script_set_fixed_clut */
    MENU_SCRIPT_OP_CLUT = 0x0a,                 /* world_menu_script_set_clut */
    MENU_SCRIPT_OP_DRAW_COLOUR = 0x0b,          /* world_menu_script_set_draw_color */
    MENU_SCRIPT_OP_TEXTURE_PAGE = 0x0c,         /* world_menu_script_set_texture_page */
    MENU_SCRIPT_OP_NUMBER = 0x0d,               /* world_menu_script_draw_formatted_number */
    MENU_SCRIPT_OP_SEMI_TRANS = 0x0e,           /* world_menu_script_set_semi_trans */
    MENU_SCRIPT_OP_DRAW_PRIORITY = 0x0f,        /* world_menu_script_set_draw_priority */
    MENU_SCRIPT_OP_SCROLLABLE_LIST = 0x10,      /* the list geometry *_menu_draw_scrollable_list reads */
    MENU_SCRIPT_OP_TILED_RECTANGLE = 0x11,      /* world_menu_script_draw_tiled_rectangle */
    MENU_SCRIPT_OP_OPENING_WINDOW_FRAME = 0x12, /* world_menu_script_draw_opening_window_frame */
    MENU_SCRIPT_OP_CLEAR_DRAW_AREA = 0x13,      /* world_menu_script_clear_temporary_draw_area */
    MENU_SCRIPT_OP_DRAW_AREA = 0x14,            /* world_menu_script_set_draw_area */
    MENU_SCRIPT_OP_REPEAT_DRAW_AREA = 0x15,     /* world_menu_script_reregister_temporary_draw_area */
    MENU_SCRIPT_OP_ADVANCE_WINDOW_SCALE = 0x16, /* world_menu_script_advance_window_scale_step */
    MENU_SCRIPT_OP_PLAIN_NUMBER = 0x17,         /* world_menu_script_draw_plain_number */
    MENU_SCRIPT_OP_BLEND_DRAW_MODE = 0x18,      /* world_menu_script_add_blend_draw_mode */
    /* The "draw number" opcodes of the WORLD and BUNIT scripts that
     * *_menu_script_draw_formatted_number tells apart from its plain form (0x0d). */
    MENU_SCRIPT_OP_NUMBER_TINTED_NO_LEADING_ZEROS = 0x19, /* sprite color; leading zeros not drawn */
    MENU_SCRIPT_OP_NUMBER_DIM_LEADING_ZEROS = 0x1a,       /* leading zeros in the disabled clut */
    MENU_SCRIPT_OP_NUMBER_TINTED = 0x1b,                  /* sprite color */
} menu_script_opcode_e;

/* Flags a menu-script number getter (a row callback) returns above its
 * value, read by *_menu_script_draw_formatted_number and the overlays'
 * *_cmd_draw_formatted_number_handler. */
typedef enum menu_number_value_flags {
    MENU_NUMBER_VALUE_DASHES = 0x20000000,   /* a row of dashes instead of the digits */
    MENU_NUMBER_VALUE_DISABLED = 0x40000000, /* drawn with the disabled number clut */
} menu_number_value_flags_e;

/* script */
/* BUNIT's handlers of the menu display scripts (menu_script_opcode_e), named after the opcode like
 * WORLD's: bunit_menu_run_display_script runs a script through menu_script_handlers. */

/* The display opcode selects the row callback's result: an icon source
 * for opcode 0x06, or an integer for predicates and number renderers. */
typedef union menu_callback {
    s32 (*value)(s32 row);
    battle_menu_icon_source_t* (*icon)(s32 row);
} menu_callback_t;
typedef char menu_callback_size_must_be_4[sizeof(menu_callback_t) == 4 ? 1 : -1];

/* Each display stream has its own handler table and end opcode. */
extern menu_instruction_t* (*menu_script_handlers[])(menu_instruction_t* instruction);

extern world_menu_window_command_t g_menu_window_interior_command;
menu_instruction_t* menu_script_draw_window_frame(menu_instruction_t* command);
menu_instruction_t* menu_script_draw_textured_quad(menu_instruction_t* instruction);
menu_instruction_t* menu_script_reregister_temporary_draw_area(menu_instruction_t* instruction);
menu_instruction_t* menu_script_set_draw_area(menu_instruction_t* instruction);
menu_instruction_t* menu_script_draw_opening_window_frame(menu_instruction_t* instruction);
menu_instruction_t* menu_script_skip_group(menu_instruction_t* command);

static __inline__ menu_instruction_t* menu_next_instruction(menu_instruction_t* instruction) {
    return (menu_instruction_t*)((u8*)instruction + instruction->header.length);
}

static __inline__ const menu_instruction_t* menu_next_const_instruction(const menu_instruction_t* instruction) {
    return (const menu_instruction_t*)((const u8*)instruction + instruction->header.length);
}

/* menu */
/* The screen's menus: the unit list frame and its grid, the sort-order list, the job page, the
 * scrollable lists with their saved positions, the selectors, the cursors, the palette mode and the menu
 * threads. */

extern s32 g_menu_list_row_index;
void menu_run_script_with_palette_mode(menu_instruction_t* script, s32 input, s32 mode);
menu_instruction_t* menu_script_if_group(menu_instruction_t* command);
menu_instruction_t* menu_script_if_else_group(menu_instruction_t* command);
menu_instruction_t* menu_script_set_fixed_clut(menu_instruction_t* command);
menu_instruction_t* menu_script_set_clut(menu_instruction_t* command);
menu_instruction_t* menu_script_set_texture_page(menu_instruction_t* command);
extern menu_callback_t g_menu_row_callbacks[16];
extern s16 g_menu_scroll_base_index;
extern s16 g_menu_scroll_list_active;
extern s16 g_menu_list_scroll_offset;
extern s16 g_menu_window_right_x;
menu_instruction_t* menu_script_draw_tiled_rectangle(menu_instruction_t* instruction);

#endif /* MENU_SCRIPT_H */
