#ifndef MAIN_H
#define MAIN_H

/* SCUS_942.21: the resident executable, its data tables and the Suzuki sound driver. */

#include "fft/data.h"
#include "fft/gfx.h"
#include "fft/map.h"
#include "fft/menu.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "fft/unit.h"
#include "psx/crt.h"
#include "psx/libapi.h"
#include "psx/libcd.h"
#include "psx/libspu.h"
#include "psx/pad.h"
#include "psx/suzuki.h"

typedef struct suzuki_smd_header suzuki_smd_header_t;

/* ability */
/* Jump ability record, 12 of them at g_main_jump_ability_data: horizontal
 * range and vertical reach. */
typedef struct battle_loader_jump_record {
    u8 range;    /* 0x00 */
    u8 vertical; /* 0x01 */
} battle_loader_jump_record_t;

/* Charge ability record, 8 of them at g_main_charge_ability_data. */
typedef struct battle_loader_charge_record {
    u8 ct;    /* 0x00 */
    u8 power; /* 0x01 */
} battle_loader_charge_record_t;

/* Pointer returned for ability_type_e: the record extent depends on the type. */
typedef union main_ability_secondary_pointer {
    ability_secondary_data_t* range;
    u8* item_id;
    u8* throw_item_type;
    battle_loader_jump_record_t* jump;
    battle_loader_charge_record_t* charge;
    u8* math_rsm;
} main_ability_secondary_pointer_t;

typedef char main_ability_secondary_pointer_size_must_be_4[(sizeof(main_ability_secondary_pointer_t) == 4) ? 1 : -1];
typedef char battle_loader_jump_record_size_must_be_2[(sizeof(battle_loader_jump_record_t) == 2) ? 1 : -1];

typedef struct main_ability_human_skillset {
    u8 high_id_bits[3];
    u8 ability_ids[HUMAN_SKILLSET_ABILITY_COUNT];
} main_ability_human_skillset_t;

typedef struct main_ability_monster_skillset {
    u8 high_id_bits;
    u8 ability_ids[MONSTER_SKILLSET_ABILITY_COUNT];
} main_ability_monster_skillset_t;

extern u8 g_main_ability_aoe_data[];
extern u8 g_main_ability_ct_data[];
extern ability_secondary_data_t g_main_ability_range_data[];

/* One secondary-data byte per ability from ABILITY_ID_MATH_FIRST on (math,
 * reaction, support, movement). */
extern u8 g_main_math_rsm_ability_data[ABILITY_ID_COUNT - ABILITY_ID_MATH_FIRST];

/* The item_type_e each Throw ability throws, indexed by ability id - ABILITY_ID_THROW_FIRST. */
extern u8 g_main_throw_ability_item_types[];
extern battle_loader_jump_record_t g_main_jump_ability_data[12];
extern battle_loader_charge_record_t g_main_charge_ability_data[8];
extern ability_data_t g_main_ability_data[];

/* Indexed by reaction ability id - ABILITY_ID_REACTION_FIRST. */
extern const u8 g_main_reaction_behavior_flags[ABILITY_ID_SUPPORT_FIRST - ABILITY_ID_REACTION_FIRST];
extern main_ability_human_skillset_t g_main_ability_human_skillset_abilities[];
extern main_ability_monster_skillset_t g_main_ability_monster_skillset_abilities[];
extern u16 g_main_ability_temp_list[SKILLSET_ABILITY_LIST_COUNT];
extern u32 g_main_secondary_saved_data_bits[2];
s32 main_ability_get_id_from_skillset(s32 skillset_id, s32 ability_index);
u16* main_ability_store_skillset_abilities(s32 skillset, s32 flags);
s32 main_ability_calculate_pointers_and_type(
    s32 id, ability_data_t** out_ability_data, main_ability_secondary_pointer_t* out_secondary_data);
u32 main_ability_get_known(battle_stats_t* unit, u32 skillset_id);

/* card */
/* Memory-card BIOS event handles opened by main_card_init_events; the CARD,
 * OPEN and WORLD pollers test them by these indices. */
enum {
    MAIN_CARD_EVENT_BIOS_DONE,
    MAIN_CARD_EVENT_BIOS_ERROR,
    MAIN_CARD_EVENT_BIOS_TIMEOUT,
    MAIN_CARD_EVENT_BIOS_NEW_CARD,
    MAIN_CARD_EVENT_HARDWARE_DONE,
    MAIN_CARD_EVENT_HARDWARE_ERROR,
    MAIN_CARD_EVENT_HARDWARE_TIMEOUT,
    MAIN_CARD_EVENT_HARDWARE_NEW_CARD,
    MAIN_CARD_EVENT_COUNT
};

/* The scanner initializes 15 file states, then allocates available card
 * blocks to usable new-save entries. State 0 means a readable save header. */
typedef enum card_save_slot_state {
    CARD_SAVE_SLOT_UNSCANNED = -2,
    CARD_SAVE_SLOT_UNAVAILABLE = -1,
    CARD_SAVE_SLOT_OCCUPIED = 0,
    CARD_SAVE_SLOT_AVAILABLE = 1
} card_save_slot_state_e;

/* The software- and hardware-card pollers return the first event observed in
 * the IOE, ERROR, TIMOUT, NEW order installed by main_card_init_events. */
typedef enum card_io_result {
    CARD_IO_RESULT_PENDING = -1,
    CARD_IO_RESULT_COMPLETE = 0,
    CARD_IO_RESULT_ERROR = 1,
    CARD_IO_RESULT_TIMEOUT = 2,
    CARD_IO_RESULT_NEW_CARD = 3,
} card_io_result_e;

enum { CARD_SAVE_SLOT_COUNT = 15 };

/* g_main_card_save_last_used_slot when no slot was loaded or written. */
enum {
    CARD_SAVE_LAST_USED_SLOT_NONE = 0xff,
};

/* PlayStation memory-card save-file header block (256 bytes), as built by
 * card_build_save_file_header (0x801c2ea8) and its WORLD twin
 * world_card_build_save_file_header (0x801322e4). */
typedef struct card_save_header {
    u8 magic0;           /* 0x00: 'S' */
    u8 magic1;           /* 0x01: 'C' */
    u8 icon_flags;       /* 0x02: 0x11, one icon frame */
    u8 block_count;      /* 0x03 */
    u8 title[0x40];      /* 0x04: Shift-JIS save-file title */
    u8 reserved[0x1C];   /* 0x44 */
    u8 icon_clut[0x20];  /* 0x60 */
    u8 icon_image[0x80]; /* 0x80 */
} card_save_header_t;

/* The fixed leading bytes of card_save_header_t. */
enum {
    CARD_SAVE_HEADER_MAGIC_S = 'S',
    CARD_SAVE_HEADER_MAGIC_C = 'C',
    CARD_SAVE_HEADER_ICON_ONE_FRAME = 0x11,
    CARD_SAVE_HEADER_BLOCK_COUNT_ONE = 1,
};

/* What a save slot's line on the save screen shows: the first 0x18 bytes of a
 * save's data block. g_main_card_save_slot_metadata keeps one per slot. */
typedef struct card_save_summary {
    u8 slot;            /* 0x00; save slot, 0xff while the buffer is incomplete */
    u8 name[0x10];      /* 0x01; save-description leader name, 0xfe-terminated */
    u8 name_terminator; /* 0x11 */
    u8 job_id;          /* 0x12 */
    u8 level;           /* 0x13 */
    u8 month;           /* 0x14; script variable 0x2e */
    u8 day;             /* 0x15; script variable 0x2f */
    u8 location;        /* 0x16; script variable 0x31, location text index */
    u8 format_version;  /* 0x17; CARD writes 4 and WORLD load requires 4 */
} card_save_summary_t;
typedef char card_save_summary_size_must_be_0x18[(sizeof(card_save_summary_t) == 0x18) ? 1 : -1];

/* The 0x1e00-byte memory-card save image, parity-covered per 128-byte block.
 * WORLD holds it in g_world_load_work_buffer (0x801cd1ec) and its serializer
 * and loader establish the fields below; CARD fills the same layout from the
 * SCUS globals in card_save_build_buffer (0x801c01ac) through
 * g_card_save_buffer_pointer. */
typedef struct card_save_buffer {
    card_save_header_t header;   /* 0x000 */
    card_save_summary_t summary; /* 0x100 */
    u8 parity_bits[8];           /* 0x118; one parity bit per 128-byte block */
    s32 elapsed_seconds;         /* 0x120 */
    /* 0x124-0x1dbe: filled by world_card_build_save_image and
     * card_save_build_buffer from the SCUS globals named in their bcopy calls. */
    u8 treasure_acquisition_date_bits[0x35];     /* 0x124; 47 packed 9-bit dates and 1 spare bit */
    u8 land_discovery_date_bits[0x12];           /* 0x159; packed 9-bit discovery dates */
    u8 proposition_last_attempt_date_bits[0x6c]; /* 0x16b; packed 9-bit dates */
    u8 proposition_states[0x60];                 /* 0x1d7; low 6 bits are a remaining-day counter */
    u8 _unused_0237[1];                          /* 0x237; serialized and parity-covered */
    u8 saved_records[5][0x38];                   /* 0x238; wldcore_saved_record_t: one sound-novel reading state each */
    u32 saved_data_bits[40];                     /* 0x350 */
    u32 secondary_saved_data_bits[2];            /* 0x3f0 */
    u8 brave_story_character_ages[0x40];         /* 0x3f8 */
    u8 proposition_count;                        /* 0x438 */
    u8 active_propositions[8][9];                /* 0x439 */
    u8 _unused_0481[3];                          /* 0x481; serialized and parity-covered */
    u8 party_records[20][0xe0];                  /* 0x484; party_data_t records truncated to 0xe0 bytes */
    u8 item_quantities[ITEM_ID_COUNT];           /* 0x1604; g_main_item_quantities */
    u8 poached_item_quantities[ITEM_ID_COUNT];   /* 0x1704; g_main_item_poached_quantities */
    /* 0x1804; first 0x40 bytes are Move-Find-Item flags, second half unknown. */
    u8 item_location_flags[0x80];
    s32 script_variables[0x100];   /* 0x1884 */
    game_options_fields_t options; /* 0x1c84; copied to g_main_game_options by world_card_init_screen */
    u8 _unknown_1c88;              /* 0x1c88 */
    s8 item_type_order_0[0xc];     /* 0x1c89; signed, -1-terminated */
    u8 _unused_1c95[1];            /* 0x1c95; serialized and parity-covered */
    s8 item_type_order_1[8];       /* 0x1c96; signed, -1-terminated */
    u8 _unused_1c9e[1];            /* 0x1c9e; serialized and parity-covered */
    s8 item_type_order_2[7];       /* 0x1c9f; signed, -1-terminated */
    s8 item_type_order_3[5];       /* 0x1ca6; signed, -1-terminated */
    s8 item_type_order_4[5];       /* 0x1cab; signed, -1-terminated */
    s8 item_type_order_5[7];       /* 0x1cb0; signed, -1-terminated */
    s8 item_type_order_6[5];       /* 0x1cb7; signed, -1-terminated */
    u8 weapon_page_order[0x8a];    /* 0x1cbc */
    u8 helmet_page_order[0x1d];    /* 0x1d46 */
    u8 armor_page_order[0x25];     /* 0x1d63 */
    u8 accessory_page_order[0x21]; /* 0x1d88 */
    u8 item_page_order[0x15];      /* 0x1da9 */
    u8 _unused_1dbe[0x42];         /* 0x1dbe; parity-covered tail */
} card_save_buffer_t;
typedef char
    card_save_buffer_options_offset_must_be_0x1c84[((unsigned long)&((card_save_buffer_t*)0)->options == 0x1c84) ? 1
                                                                                                                 : -1];
typedef char card_save_buffer_size_must_be_0x1e00[(sizeof(card_save_buffer_t) == 0x1e00) ? 1 : -1];

extern s32 g_main_card_bios_events[MAIN_CARD_EVENT_COUNT];
extern u8 g_main_card_bios_events_opened;
extern u8 g_main_card_save_last_used_slot; /* the save slot last loaded or written */
extern u8 g_card_save_selected_slot;
extern s8 g_card_save_slot_file_states[];
extern card_save_summary_t g_main_card_save_slot_metadata[CARD_SAVE_SLOT_COUNT];
extern u8 g_main_card_save_slot_playtimes[][3];
void main_card_init_events(void);

/* file */
/* First sector of each disc file the code reads at a fixed sector, named as
 * the disc directory names the file. The other files are found through
 * sector tables in the data. */
typedef enum disc_sector {
    DISC_SECTOR_SCEAP_DAT = 0xc6,
    DISC_SECTOR_BATTLE_BIN = 0x3e8,
    DISC_SECTOR_EVENT_ATTACK_OUT = 0x990,
    DISC_SECTOR_EVENT_MAPTITLE_BIN = 0xdac, /* five sectors for each four maps */
    DISC_SECTOR_EVENT_FRAME_BIN = 0xe68,
    DISC_SECTOR_EVENT_TEST_EVT = 0xe7b,     /* EVENT_BLOCK_SECTORS per event_id_e */
    DISC_SECTOR_EVENT_EVTFACE_BIN = 0x164b, /* four sectors per portrait */
    DISC_SECTOR_EVENT_UNIT_BIN = 0x166b,
    DISC_SECTOR_EVENT_BONUS_BIN = 0x16c0, /* thirteen sectors per image */
    DISC_SECTOR_EVENT_ITEM_BIN = 0x1899,
    DISC_SECTOR_EVENT_WLDFACE_BIN = 0x18ba,
    DISC_SECTOR_EVENT_ATCHELP_LZW = 0x1a3a,
    DISC_SECTOR_EVENT_WORLD_LZW = 0x1bd8,
    DISC_SECTOR_EVENT_OPEN_LZW = 0x1c18,
    DISC_SECTOR_EVENT_EVTCHR_BIN = 0x1d4c, /* fifteen sectors per block */
    DISC_SECTOR_BATTLE_EFC_FNT_TIM = 0xdea8,
    DISC_SECTOR_BATTLE_WEP_SPR = 0xdefa,
    DISC_SECTOR_BATTLE_OTHER_SPR = 0xdf24,
    DISC_SECTOR_BATTLE_ENTD1_ENT = 0xebc1,
    DISC_SECTOR_BATTLE_ENTD2_ENT = 0xebe9,
    DISC_SECTOR_BATTLE_ENTD3_ENT = 0xec11,
    DISC_SECTOR_BATTLE_ENTD4_ENT = 0xec39,
    DISC_SECTOR_BATTLE_ZODIAC_BIN = 0xec61,
    DISC_SECTOR_MENU_FFTSAVE_DAT = 0x11a7f,
    DISC_SECTOR_WORLD_WLDTEX_TM2 = 0x11d28,
    DISC_SECTOR_WORLD_WLDPIC_BIN = 0x11dae,  /* by g_wldcore_picture_sector_offsets */
    DISC_SECTOR_WORLD_WLDMES_BIN = 0x11f59,  /* by g_wldcore_message_block_sector_offsets */
    DISC_SECTOR_WORLD_SNPLBIN_BIN = 0x128f9, /* by g_wldcore_sound_novel_script_sector_offsets */
    DISC_SECTOR_WORLD_SNPLMES_BIN = 0x1291d, /* by g_wldcore_sound_novel_text_sector_offsets */
    DISC_SECTOR_WORLD_WLDBK_BIN = 0x12995,   /* by g_wldcore_background_sector_offsets */
    DISC_SECTOR_WORLD_WLDCORE_BIN = 0x14849,
    DISC_SECTOR_WORLD_WORLD_BIN = 0x14925,
    DISC_SECTOR_SOUND_ENV_SED = 0x14c08,
    DISC_SECTOR_SOUND_SYSTEM_SED = 0x14c0a,
    DISC_SECTOR_SOUND_WAVESET_WD = 0x14c0f,
    DISC_SECTOR_OPEN_OPEN_BIN = 0x14ff0,
    DISC_SECTOR_OPEN_OPNTEX_BIN = 0x1505d,
    DISC_SECTOR_OPEN_OPNBK_BIN = 0x15243, /* the title backgrounds, by sector offset */
    DISC_SECTOR_OPEN_FFTST_STR = 0x153d6,
    DISC_SECTOR_OPEN_FFTOP_STR = 0x17b60,
    DISC_SECTOR_OPEN_FFTPRE_STR = 0x1e038,
    DISC_SECTOR_OPEN_FFTEND_STR = 0x21908,
    DISC_SECTOR_OPEN_FFTPR_STR = 0x25160,
    DISC_SECTOR_OPEN_FFTUNIT_STR = 0x26ec0,
    DISC_SECTOR_OPEN_ENDING_XA = 0x2f551,
} disc_sector_e;

/* One poll advances at most one phase; recoverable CD errors restart setup. */
typedef enum main_file_load_state {
    MAIN_FILE_LOAD_STATE_IDLE = 0,
    MAIN_FILE_LOAD_STATE_SET_DOUBLE_SPEED = 1,
    MAIN_FILE_LOAD_STATE_WAIT_AFTER_SET_MODE = 2,
    MAIN_FILE_LOAD_STATE_START_SEEK = 3,
    MAIN_FILE_LOAD_STATE_POLL_SEEK = 4,
    MAIN_FILE_LOAD_STATE_START_READ = 5,
    MAIN_FILE_LOAD_STATE_POLL_READ = 6,
    MAIN_FILE_LOAD_STATE_UPLOAD_VRAM = 7, /* WLDCORE image stream only */
} main_file_load_state_e;

/* Asynchronous CD read state advanced by main_file_poll_load. */
typedef struct main_file_load_descriptor {
    s32 _unknown_00;
    s32 state; /* MAIN_FILE_LOAD_STATE_* */
    s32 error_count;
    s32 wait_frames;
    s32 sector_count;
    s32 lba;
    s32 sector_index; /* 0x18; sector being read by wldcore_file_poll_vram_image_stream */
    CdlLOC position;
    u8* destination;
} main_file_load_descriptor_t;
typedef char main_file_load_state_size_must_be_36[sizeof(main_file_load_descriptor_t) == 36 ? 1 : -1];

extern main_file_load_descriptor_t g_main_file_cd_state;
extern u32 g_main_file_still_loading;

/* File-header wrapper at 0x80011bd0 forwards build's result. */
s32 main_file_build_header_nnl(main_file_load_descriptor_t* header, s32 sector, s32 sectors, void* destination);
s32 main_file_call_build_header(s32 sector, u32 size, void* destination);
int main_file_is_still_loading(void);
void main_file_reset_cd_subsystems(void);
void main_file_handle_cd_read_callback(u8 event, u8* result);
void main_file_handle_cd_ready_callback(u8 event, u8* result);
void main_file_reset_cdrom_cpu_ram(void);

int main_file_build_header(
    main_file_load_descriptor_t* state, int lba, int sector_count, void* destination, int loading_display_mode);

void* main_file_load_checked_to_heap(int sector, int size);
void* main_file_load_checked_to_address(int sector, int size, void* destination);
suzuki_smd_header_t* main_file_get_smd(int sector, u32 size);
void* main_file_load_to_heap(s32 sector, u32 size);
void* main_file_load_to_address(int sector, u32 size, void* destination);
void main_file_load_data_from_disc(int sector, int sectors, void* destination, int suppress_loading_display);
void main_file_poll_load(main_file_load_descriptor_t* state);
void main_file_reset_pause_cdrom(main_file_load_descriptor_t* state);

/* input */
/* L1 + R1 + Select + Start restarts the game. */
enum {
    MAIN_INPUT_RESET_BUTTONS = PSX_PAD_L1 | PSX_PAD_R1 | PSX_PAD_SELECT | PSX_PAD_START,
};

extern u8 g_main_input_repeat_initial_delay;
extern u8 g_main_input_repeat_period;
extern u8 g_main_input_secondary_repeat_period;

/* item */
extern u8 g_main_item_quantities[ITEM_ID_COUNT];
extern item_attribute_t g_main_item_attributes[];
extern u8 g_main_item_page_order[0x15];

/* The first 0x40 bytes are Move-Find-Item flags; the second half is not yet
 * identified. Both halves are initialized and serialized as one block. */
extern u8 g_main_item_location_flags[0x80];
extern u8 g_main_item_poached_quantities[ITEM_ID_COUNT];
extern item_secondary_data_t g_main_item_secondary_data[];

/* &g_main_item_primary_data[0].secondary_data_id viewed as a stride-12 byte
 * array. main_unit_set_equipment_stats reads the secondary id through its own
 * %lo(g_main_item_primary_data+4) address while the type_flags test uses a
 * record pointer; reading both through item_data_t folds them onto one base
 * register and misses the byte-exact match. */
extern u8 g_main_item_secondary_data_ids[];
extern item_type_order_tables_t g_main_item_type_order_tables;

/* Views of g_main_item_type_order_tables.order_1..6 for the WORLD save-image
 * copies (world_card_build_save_image, world_card_load_globals_from_save_image),
 * whose code the member spelling changes. */
extern s8 g_main_item_type_order_1[8];
extern u8 g_main_item_type_order_2[7];
extern u8 g_main_item_type_order_3[5];
extern u8 g_main_item_type_order_4[5];
extern u8 g_main_item_type_order_5[7];
extern u8 g_main_item_type_order_6[5];
extern weapon_data_t g_main_item_weapon_data[]; /* 0x80063ab8 */
extern u8 g_main_weapon_page_order[0x8a];
extern u8 g_main_item_ability_item_ids[14];
extern u8 g_main_item_poach_table[48][2];
extern item_evade_t g_main_item_accessory_data[]; /* 0x80063f58; unit accessory evade slots */
extern armor_data_t g_main_item_helm_armor_secondary_data[];
extern item_data_t g_main_item_primary_data[];
extern item_evade_t g_main_item_shield_data[]; /* 0x80063eb8; unit right/left shield evade slots */
void main_item_init_new_game_inventory(void);
void main_item_init_order_tables(void);
item_data_t* main_item_get_data_pointer(s32 item_id);
s32 main_item_get_total_equipment_quantity(u8 item_id, s32 include_battle);

/* job */
extern job_data_t g_main_job_data[];
extern const u16 g_main_job_level_jp_requirements[8];
extern u16 g_main_job_jp_requirements_by_level[];
extern const u8 g_main_job_unlock_requirements[UNIT_CAREER_JOB_COUNT - 1][UNIT_JOB_LEVEL_BYTE_COUNT];
s32 main_job_add_proposition_jp(u32 party_index, s32 jp);
u32 main_job_calculate_level(u16 job_jp);
u32 main_job_calculate_unlocked(const u8* packed_job_levels, u32 gender_flags);
job_data_t* main_job_get_data_pointer(s32 job_id);
s32 main_job_get_random_unlocked(const battle_stats_t* unit);
void main_job_store_unlock_bitset(u8 destination[UNIT_UNLOCKED_JOB_BYTE_COUNT], u32 unlocked_jobs);

/* save */
/* Nine-byte active-proposition records at 0x80057eec. Daily update 0x80074b30
 * advances elapsed_days and sets completion bit 0x04. The assignment at
 * 0x80074a34 stores the current world-map location at +0x04; 0x80074c40
 * filters completed records by that byte. */
typedef struct wldcore_proposition_progress {
    u8 flags;          /* 0x00; wldcore_proposition_progress_flag_e */
    u8 proposition_id; /* 0x01; text 0xd000 | id names it */
    u8 elapsed_days;
    u8 assigned_days;
    u8 location;
    u8 participant_count;      /* 0x05; speaker selection at 0x800795c4 trusts this count */
    u8 participant_indices[3]; /* 0x06; 0x80074a8c passes each index to party lookup before 0x80074ab4 stores it */
} wldcore_proposition_progress_t;
typedef char wldcore_proposition_progress_size_must_be_9[sizeof(wldcore_proposition_progress_t) == 9 ? 1 : -1];

/* Saved-data records at 0x80059594. The exact caller at 0x8008aba8 establishes
 * the 0x38 stride; 0x8008abe4 selectively resets these fields. Field meanings
 * beyond those stores remain provisional. */
struct wldcore_sound_novel_script_file;

typedef struct wldcore_saved_record {
    /* 0x00: loaded sound-novel script, including its section-offset table. */
    struct wldcore_sound_novel_script_file* buffer;
    u32* script_words; /* 0x04 */
    u32 instruction;   /* 0x08 */
    /* 0x0c-0x37 mirror the live sound-novel state at 0x800d484c. */
    u16 state_flags; /* 0x0c; wldcore_opcode_state_flag_e */
    s16 section;     /* 0x0e */
    s16 word_index;  /* 0x10; instruction word index in the section; cleared by wldcore_sound_novel_restore_saved_state
                      */
    s16 counter_delta;        /* 0x12 */
    s16 text_id;              /* 0x14 */
    s16 brightness;           /* 0x16 */
    s16 _unknown_18;          /* 0x18 */
    s16 background_set;       /* 0x1a */
    s16 picture_render_index; /* 0x1c */
    s16 picture_id;           /* 0x1e */
    s16 text_history_0;       /* 0x20; four-entry text history, newest last */
    s16 text_history_1;       /* 0x22 */
    s16 text_history_2;       /* 0x24 */
    s16 text_history_3;       /* 0x26 */
    s16 value_display_mode;   /* 0x28 */
    s16 music_selection;      /* 0x2a */
    s16 sound_mode;           /* 0x2c */
    s16 picture_x;            /* 0x2e */
    s16 picture_y;            /* 0x30 */
    s16 sound_id;             /* 0x32 */
    s32 counter;              /* 0x34 */
} wldcore_saved_record_t;
typedef char wldcore_saved_record_size_must_be_0x38[sizeof(wldcore_saved_record_t) == 0x38 ? 1 : -1];

/* Provisional: an adjacent word pair copied as one structure. The paired
 * type reproduces the target's two-load/two-store schedule. */
typedef struct wldcore_word_pair {
    s32 first;
    s32 second;
} wldcore_word_pair_t;

extern u8 g_main_save_proposition_count;
extern s32 g_main_saved_animation_speed;
extern s32 g_main_saved_weather_variable;

/* Word-indexed RAM bitmaps; the save serializer copies only their packed data
 * bytes and omits the padding at the end of each allocation. */
extern u32 g_main_proposition_last_attempt_date_bits[27];
extern u32 g_main_treasure_acquisition_date_bits[14];
extern u32 g_main_land_discovery_date_bits[5];
extern u8 g_main_proposition_states[96]; /* wldcore_proposition_state_e */
extern struct wldcore_sound_novel_script_file g_main_save_slot_buffer;
extern u32 g_main_saved_data_bits[40];
extern wldcore_word_pair_t g_main_saved_list_cursor_state;
extern wldcore_proposition_progress_t g_main_active_propositions[8];
extern wldcore_saved_record_t g_main_saved_records[];
void main_save_init_data_tables(void);
void main_save_init_state_and_options(void);

/* script */
extern u16 g_main_scenario_event_finish_operations[];
/* The script variable words: whole-word variables first (event_script_variable_id_e), then the packed flags. */
extern s32 g_main_script_variables[0x100];
extern u8* g_main_heap_companion_overlay_load_address;
extern u16 g_main_scenario_condition_offsets[];
extern s16 g_main_scenario_condition_data[];
extern event_file_block_t g_main_script_event_block[];

/* sound */
/* Suzuki sound driver (SCUS_942.21 0x800120f4-0x800186c4) records, tables and
 * globals. Offsets and widths are taken from the driver code named beside
 * each field; the driver writes its _unknown_XX members and never reads them. */
/* Linear ramp stepped once per tick by main_smd_step_ramp (0x80014f18):
 * value moves by step until count reaches zero, then snaps to target << 16.
 * The root-counter handler tests count with `lh` before stepping MUS
 * +0x88/+0x94/+0xa0/+0xac and the volume ramps of g_main_sound_spu_state. */
typedef struct suzuki_ramp {
    s32 value;  /* 0x00 */
    s32 step;   /* 0x04 */
    s16 count;  /* 0x08 */
    s16 target; /* 0x0a */
} suzuki_ramp_t;

/* WAVESET instrument entry (0x10 bytes). main_smd_set_instrument
 * (0x80016fb4) and main_smd_get_instrument_attr copy it into a channel or an
 * SpuVoiceAttr. The rates, the level and the modes are LIBSPU envelope
 * arguments. Modes: 1 linear increase, 3 linear decrease, 5 exponential
 * increase, 7 exponential decrease (attack: 5 or linear; sustain: 1, 5, 7 or
 * linear decrease; release: 7 or linear). */
typedef struct suzuki_instrument {
    u32 start;        /* 0x00; sample offset from the waveset's SPU address */
    u16 loop_offset;  /* 0x04; loop start relative to start */
    u16 fine_tune;    /* 0x06; copied to the channel fine_tune */
    u8 attack_rate;   /* 0x08 */
    u8 decay_rate;    /* 0x09 */
    u8 sustain_rate;  /* 0x0a */
    u8 release_rate;  /* 0x0b */
    u8 sustain_level; /* 0x0c */
    u8 attack_mode;   /* 0x0d */
    u8 sustain_mode;  /* 0x0e */
    u8 release_mode;  /* 0x0f */
} suzuki_instrument_t;

/* WAVESET.WD header as loaded. main_sound_load_waveset
 * (0x80017c8c) uploads wave_size bytes from wave_offset to SPU RAM and keeps
 * header_size bytes on the Suzuki heap, linked from g_main_sound_waveset_list
 * through next. Select Sound Font (0x80016e48) matches id. */
typedef struct suzuki_waveset {
    u8 _unused_00[0x10];                /* 0x00; "dwdsP" magic, file size at 0x08 */
    u32 header_size;                    /* 0x10 */
    u32 wave_size;                      /* 0x14 */
    u32 wave_offset;                    /* 0x18 */
    u8 _unused_1c[4];                   /* 0x1c; index of the last instrument (0xaf in WAVESET.WD) */
    u16 id;                             /* 0x20 */
    u8 _unused_22[6];                   /* 0x22 */
    u32 spu_address;                    /* 0x28; SpuMalloc result, freed by main_sound_free_waveset */
    struct suzuki_waveset* next;        /* 0x2c */
    suzuki_instrument_t instruments[0]; /* 0x30 */
} suzuki_waveset_t;

/* SMD music sequence header ("smds"), read by main_smd_transfer_music_data,
 * main_smd_init_channels, main_smd_init_channel_streams and
 * main_smd_get_music_filename. The values noted as "on the disc" hold for
 * all 100 MUSIC_NN.SMD files. */
struct suzuki_smd_header {
    u8 magic[4];            /* 0x00; "smds" */
    u8 _unused_04[4];       /* 0x04; differs in every file */
    u32 size;               /* 0x08; file size in bytes */
    u8 _unused_0c[4];       /* 0x0c */
    u16 id;                 /* 0x10; copied to MUS id and channel sound_id; 0 on the disc */
    u8 _unknown_12;         /* 0x12; copied to MUS _unknown_014; 2 on the disc */
    u8 tick_divisor;        /* 0x13; divides the 48 ticks of a beat for the bar and beat counters; 1 on the disc */
    u8 channel_count;       /* 0x14 */
    u8 _unknown_15;         /* 0x15; copied to MUS _unknown_017; 0 on the disc */
    u16 waveset_id;         /* 0x16; 0 on the disc */
    u16 _unknown_18;        /* 0x18; copied to MUS _unknown_01a; 0x28-0x7f on the disc */
    s8 reverb_mode;         /* 0x1a; 4 on the disc */
    u8 reverb_depth;        /* 0x1b */
    u8 reverb_delay;        /* 0x1c */
    u8 reverb_feedback;     /* 0x1d */
    u16 filename_offset;    /* 0x1e; offset of the zero-terminated name the sequence was built from */
    u16 data_offset;        /* 0x20 */
    u16 channel_offsets[0]; /* 0x22; one stream offset per channel (0: no stream), then a zero */
};

/* Suzuki sequencer modulator (0x20 bytes). Each channel has four, at
 * channel+0xe0. step is one of the g_main_smd_modulator_waveforms functions,
 * called by main_smd_update_modulators (0x8001749c), which adds the scaled
 * result to the channel output selected by target. delay delays the
 * start; while fade_level is below 0x100 the output is scaled by
 * fade_level / 0x100 and fade_level grows by fade_step (fade-in). */
typedef struct suzuki_modulator {
    s32 (*step)(struct suzuki_modulator*); /* 0x00 */
    s32 value;                             /* 0x04 */
    s32 increment;                         /* 0x08 */
    s32 amplitude;                         /* 0x0c; from main_smd_modulator_calculate_step */
    u16 timer;                             /* 0x10 */
    u16 timer_reset;                       /* 0x12 */
    u16 delay;                             /* 0x14; ticks before the start */
    u16 delay_reset;                       /* 0x16; reload of delay */
    s16 fade_level;                        /* 0x18; output scale, 0x100 = full */
    u16 fade_step;                         /* 0x1a; per-tick fade-in increment */
    u8 target;                             /* 0x1c; suzuki_modulator_target_e */
    u8 waveform;                           /* 0x1d; suzuki_modulator_waveform_e */
    s16 flags;                             /* 0x1e; suzuki_modulator_flag_e */
} suzuki_modulator_t;
typedef char suzuki_modulator_size_must_be_0x20[(sizeof(suzuki_modulator_t) == 0x20) ? 1 : -1];

/* suzuki_modulator_t.target: the channel output main_smd_update_modulators
 * adds the modulator to. */
typedef enum suzuki_modulator_target {
    SUZUKI_MODULATOR_TARGET_PITCH = 0,
    SUZUKI_MODULATOR_TARGET_VOLUME = 1,
    SUZUKI_MODULATOR_TARGET_BALANCE = 2,
} suzuki_modulator_target_e;

/* suzuki_modulator_t.waveform: index into g_main_smd_modulator_waveforms,
 * the low nibble of the waveform byte of the modulator opcodes. Indices
 * 8-15 hold main_smd_modulator_deactivate, which stops the modulator on its
 * first step. main_smd_modulator_calculate_step scales the amplitude of the
 * triangles and of sawtooth 4 only. */
typedef enum suzuki_modulator_waveform {
    SUZUKI_MODULATOR_WAVEFORM_SQUARE = 0,            /* main_smd_modulator_step_square: 0 and +amplitude */
    SUZUKI_MODULATOR_WAVEFORM_ALTERNATING = 1,       /* main_smd_modulator_step_alternating: +/-amplitude */
    SUZUKI_MODULATOR_WAVEFORM_TRIANGLE = 2,          /* main_smd_modulator_step_triangle */
    SUZUKI_MODULATOR_WAVEFORM_TRIANGLE_CENTERED = 3, /* main_smd_modulator_step_triangle_centered */
    SUZUKI_MODULATOR_WAVEFORM_SAWTOOTH = 4,          /* main_smd_modulator_step_sawtooth */
    SUZUKI_MODULATOR_WAVEFORM_SAWTOOTH_UNSCALED = 5, /* the same step function, amplitude not divided */
    SUZUKI_MODULATOR_WAVEFORM_RANDOM = 6,            /* main_smd_modulator_step_random: sample and hold */
    SUZUKI_MODULATOR_WAVEFORM_RANDOM_BIPOLAR = 7,    /* main_smd_modulator_step_random_bipolar */
    SUZUKI_MODULATOR_WAVEFORM_OFF = 8,               /* main_smd_modulator_deactivate, as are 9-15 */
} suzuki_modulator_waveform_e;

/* suzuki_modulator_t.flags. */
typedef enum suzuki_modulator_flag {
    SUZUKI_MODULATOR_ACTIVE = 0x1,
    SUZUKI_MODULATOR_RESTART_ON_KEY_ON = 0x2, /* main_smd_read_instructions restarts it at each key-on */
    SUZUKI_MODULATOR_HALF_PERIOD_DONE = 0x4,  /* centred triangle: later half-periods last twice as long */
    SUZUKI_MODULATOR_NEGATED = 0x8,           /* the next step or amplitude is negated */
} suzuki_modulator_flag_e;

typedef s32 (*suzuki_modulator_step_t)(suzuki_modulator_t* modulator);

/* Repeat stack entry (0xc bytes) pushed by Repeat and consumed by Coda and
 * To Coda. */
typedef struct suzuki_repeat {
    u8 count;       /* 0x00; passes left after the current one */
    u8 _unused_01;  /* 0x01 */
    u8 octave_base; /* 0x02; octave_base at the section start */
    u8 coda_octave; /* 0x03; octave_base at the Coda */
    u8* start;      /* 0x04; first byte of the section */
    u8* coda;       /* 0x08; the byte after the Coda */
} suzuki_repeat_t;

/* Suzuki sequencer channel record (0x160 bytes), the third argument of every
 * SMD opcode handler (note data, music, channel). Field names follow the
 * opcode that writes them. voice_attr at 0x34 is the SDK SpuVoiceAttr; the driver passes its
 * members to LIBSPU one by one.
 *
 * flags is the channel flag word (suzuki_channel_flag_e); a channel is
 * allocated while it is non-zero, and main_smd_set_flags_all_channels ORs a
 * mask into every non-zero word. voice_update_flags (suzuki_voice_update_e)
 * queues work for main_smd_update_voices, voice_attr_flags
 * (suzuki_voice_attr_e) marks the SPU voice attributes
 * main_sound_update_voice_attrs pushes, and ramp_flags (suzuki_ramp_flag_e)
 * marks the ramps stepped by main_smd_update_channel_ramps.
 *
 * octave_base is written at halfword width (Octave, Raise/Lower Octave, End
 * Bar) but saved into the u8 loop_octave_base by Loop and Loop Select, which
 * read it with `lbu`: GCC narrows the u16 load itself.
 *
 * sound_id is the SMD id for music and a MAIN_SOUND_ID (`resource id << 16 |
 * sound`) for sound effects; Play VFX reads the resource id half with `lh`,
 * the SFX API compares the whole word. */
typedef struct suzuki_music_channel {
    u16 flags;              /* 0x00; suzuki_channel_flag_e */
    u16 voice_update_flags; /* 0x02; suzuki_voice_update_e */
    u16 voice_attr_flags;   /* 0x04; suzuki_voice_attr_e */
    u16 ramp_flags;         /* 0x06; suzuki_ramp_flag_e */
    s32 sound_id;           /* 0x08; sound resource id in the high halfword, sound number in the low halfword */
    u8 channel_number;      /* 0x0c; 0xff + channel index (SFX: the index); write-only */
    u8 priority;            /* 0x0d; SFX steal priority (channel id >> 8; stolen when <= 0x20); music: 0x10 */
    u8 _padding_00e[2];     /* 0x0e; aligns start_tick */
    u32 start_tick;         /* 0x10; g_main_sound_tick_count when main_sound_start_sfx started the channel */
    u8* note_data_start;    /* 0x14 */
    u8* note_data;          /* 0x18; read position */
    u8* loop_note_data;     /* 0x1c; loop point set by Loop and Loop Select; End Bar jumps to it */
    u8* restart_note_data;  /* 0x20; set by main_smd_init_channel_streams */
    u32 _unknown_024;       /* 0x24; cleared by both channel initialisers, never read */
    u16 loop_count;         /* 0x28; times End Bar has jumped to the loop point */
    u8 _unknown_02a;        /* 0x2a; cleared by both channel initialisers, never read */
    u8 loop_octave_base;    /* 0x2b; octave_base saved with loop_note_data */
    u8 instrument;          /* 0x2c; index into waveset->instruments */
    u8 voice;               /* 0x2d; SPU voice (Set Voice); only an odd voice takes Enable LFO */
    u16 note_release_rate;  /* 0x2e; the instrument's or Release Rate's value; each note reloads release_rate from it */
    suzuki_waveset_t* waveset; /* 0x30; Select Sound Font */
    SpuVoiceAttr voice_attr;   /* 0x34; SPU voice, volume, sample addresses and envelope */
    s16 event_ticks;           /* 0x74; ticks left of the current note, Rest or Fermata; 0 reads the next event */
    s8 length_adjust;          /* 0x76; ticks added to each event's length (Length Adjust) */
    u8 _padding_077;           /* 0x77; aligns note_ticks */
    u16 note_ticks;            /* 0x78; ticks until the note is keyed off (SUZUKI_CHANNEL_KEYED_OFF) */
    u16 gate_time;             /* 0x7a; key-on time in sixteenths of the length (Gate Time); initialised to 0xf */
    u8 previous_key;           /* 0x7c; previous note's key, the portamento start; cleared by the channel initialiser */
    u8 key;                    /* 0x7d; key number of the current note */
    u16 octave_base;           /* 0x7e; key number of the current octave, octave * 12 */
    s32 pitch;              /* 0x80; 1/256 semitones << 16: each note's key, moved by the pitch slide and portamento */
    s16 fine_tune;          /* 0x84; instrument fine_tune, added to key << 8 (1/256 semitone) */
    s16 pitch_offset;       /* 0x86; detune in 1/256 semitone (the Pitch Offset opcodes), added to each note */
    s16 pitch_modulation;   /* 0x88; modulator target 0 output */
    s16 volume_modulation;  /* 0x8a; modulator target 1 output */
    s16 balance_modulation; /* 0x8c; modulator target 2 output */
    u8 _unused_08e[2];      /* 0x8e */
    u16 portamento_ticks;   /* 0x90; Portamento length; non-zero sets SUZUKI_RAMP_PORTAMENTO */
    s16 balance;            /* 0x92; balance byte << 8 */
    s16 velocity;           /* 0x94; velocity << 8, 0x6000 by default */
    u8 _padding_096[2];     /* 0x96; aligns volume */
    s32 volume;             /* 0x98; volume byte << 24 */
    s32 pitch_slide_step;   /* 0x9c; per-tick pitch delta of Pitch Slide or the portamento */
    s32 volume_step;        /* 0xa0; per-tick volume delta of Volume Ramp */
    s16 balance_step;       /* 0xa4; per-tick balance delta of Balance Ramp */
    u16 pitch_slide_count;  /* 0xa6 */
    u16 volume_step_count;  /* 0xa8 */
    u16 balance_step_count; /* 0xaa */
    u16 repeat_depth;       /* 0xac; -1 when the repeat stack is empty */
    u16 modulator_index;    /* 0xae; modulator selected by Modulator Select */
    suzuki_repeat_t repeats[4];       /* 0xb0 */
    suzuki_modulator_t modulators[4]; /* 0xe0 */
} suzuki_music_channel_t;
typedef char suzuki_music_channel_size_must_be_0x160[(sizeof(suzuki_music_channel_t) == 0x160) ? 1 : -1];
typedef char suzuki_channel_voice_attr_must_be_at_offset_0x34
    [((unsigned long)&((suzuki_music_channel_t*)0)->voice_attr == 0x34) ? 1 : -1];
typedef char suzuki_channel_voice_attr_size_must_be_0x40[(sizeof(SpuVoiceAttr) == 0x40) ? 1 : -1];

/* suzuki_music_channel_t.flags. */
typedef enum suzuki_channel_flag {
    SUZUKI_CHANNEL_IN_USE = 0x0001,
    /* Set at initialisation from SUZUKI_MUSIC_STATUS_IMMEDIATE_INSTRUMENT:
     * main_smd_set_instrument marks the voice attributes at once. */
    SUZUKI_CHANNEL_IMMEDIATE_INSTRUMENT = 0x0004,
    /* Sound-effect channel (main_sound_start_sfx): notes keep the channel
     * velocity, and main_smd_set_instrument marks the voice attributes at once. */
    SUZUKI_CHANNEL_SFX = 0x0008,
    SUZUKI_CHANNEL_NO_RETRIGGER = 0x0010,       /* never set by the driver itself; a held note is not keyed on again */
    SUZUKI_CHANNEL_MUTED = 0x0020,              /* by music->mute_mask; blocks key-on */
    SUZUKI_CHANNEL_NOTE_READ = 0x0080,          /* set with SOUNDING by every note; never tested */
    SUZUKI_CHANNEL_SOUNDING = 0x0100,           /* a note or Fermata holds the channel; ends the tick's opcode reads */
    SUZUKI_CHANNEL_TIED = 0x0200,               /* the gate time does not key the note off */
    SUZUKI_CHANNEL_KEYED_OFF = 0x0400,          /* resting or past the gate time; the next note keys on */
    SUZUKI_CHANNEL_SLUR = 0x0800,               /* Slur On/Off: every note is tied */
    SUZUKI_CHANNEL_NOTE_FOLLOWS = 0x1000,       /* the look-ahead found a note as the next event */
    SUZUKI_CHANNEL_STREAMS_SET = 0x4000,        /* main_smd_init_channel_streams; never tested */
    SUZUKI_CHANNEL_INSTRUMENT_PENDING = 0x8000, /* the next note marks the voice attributes */
} suzuki_channel_flag_e;

/* suzuki_music_channel_t.voice_update_flags: work queued for
 * main_smd_update_voices. */
typedef enum suzuki_voice_update {
    SUZUKI_VOICE_UPDATE_KEY_ON = 0x0001,
    SUZUKI_VOICE_UPDATE_KEY_OFF = 0x0002,
    SUZUKI_VOICE_UPDATE_VOLUME = 0x0100, /* volume and balance */
    SUZUKI_VOICE_UPDATE_PITCH = 0x0200,
} suzuki_voice_update_e;

/* suzuki_music_channel_t.voice_attr_flags: the SPU voice attributes
 * main_sound_update_voice_attrs pushes. PITCH, ATTACK and SUSTAIN double as
 * the marks of the LFO, noise and reverb opcodes, so they also request the
 * voice-mask updates of suzuki_voice_mask_request_e. Callers also set bits
 * 0x7000, which nothing tests. */
typedef enum suzuki_voice_attr {
    SUZUKI_VOICE_ATTR_VOLUME = 0x0001,
    SUZUKI_VOICE_ATTR_VOLUME_MODE = 0x0002, /* volume and volume mode */
    SUZUKI_VOICE_ATTR_PITCH = 0x0004,
    SUZUKI_VOICE_ATTR_ADDRESS = 0x0008, /* start and loop addresses */
    SUZUKI_VOICE_ATTR_ATTACK = 0x0010,  /* time and mode */
    SUZUKI_VOICE_ATTR_DECAY = 0x0020,
    SUZUKI_VOICE_ATTR_SUSTAIN = 0x0040, /* time and mode */
    SUZUKI_VOICE_ATTR_RELEASE = 0x0080, /* time and mode */
    SUZUKI_VOICE_ATTR_SUSTAIN_LEVEL = 0x0100,
    SUZUKI_VOICE_ATTR_ALL = 0x01ff,
} suzuki_voice_attr_e;

/* suzuki_music_channel_t.ramp_flags. */
typedef enum suzuki_ramp_flag {
    SUZUKI_RAMP_PITCH_SLIDE = 0x01,
    SUZUKI_RAMP_PITCH_SLIDE_HOLD = 0x02, /* toggled by Pitch Slide Hold: the slide count does not run down */
    SUZUKI_RAMP_PORTAMENTO = 0x04,       /* Portamento: a key-on slides from the previous key */
    SUZUKI_RAMP_VOLUME = 0x08,
    SUZUKI_RAMP_BALANCE = 0x10,
} suzuki_ramp_flag_e;

/* g_main_sound_voice_update_requests: the voice masks the root-counter
 * handler recalculates. */
typedef enum suzuki_voice_mask_request {
    SUZUKI_VOICE_MASK_REQUEST_PITCH_LFO = 0x1,
    SUZUKI_VOICE_MASK_REQUEST_NOISE = 0x2,
    SUZUKI_VOICE_MASK_REQUEST_REVERB = 0x4,
} suzuki_voice_mask_request_e;

/* SMD instruction bytes. A byte below SUZUKI_SMD_OPCODE_FIRST starts a note:
 * it is the velocity, the next byte indexes g_main_smd_note_semitones and
 * g_main_smd_note_durations, and a length byte follows when that duration
 * is 0. Every other byte is an opcode; g_main_smd_opcode_handlers and
 * g_main_smd_opcode_lengths are indexed by opcode - SUZUKI_SMD_OPCODE_FIRST.
 *
 * Each comment lists the parameter bytes after the opcode. The handler of
 * SUZUKI_SMD_OPCODE_X is main_smd_x unless the comment names another. The 37
 * opcodes without an enumerator share main_smd_no_instruction and have
 * length 0 in g_main_smd_opcode_lengths, so the look-ahead of
 * main_smd_read_instructions would never get past one. */
typedef enum suzuki_smd_opcode {
    SUZUKI_SMD_OPCODE_FIRST = 0x80,
    SUZUKI_SMD_OPCODE_REST = 0x80,    /* ticks */
    SUZUKI_SMD_OPCODE_FERMATA = 0x81, /* ticks: holds the sounding note that much longer */
    SUZUKI_SMD_OPCODE_NOP_8A = 0x8a,  /* nothing; main_smd_opcode_8a */
    SUZUKI_SMD_OPCODE_LOOP_SELECT
    = 0x8d, /* selector: a Loop when it equals music->loop_select; main_smd_loop_point_if_selected */
    SUZUKI_SMD_OPCODE_SKIP_3 = 0x8e,  /* three ignored bytes */
    SUZUKI_SMD_OPCODE_NOP_8F = 0x8f,  /* nothing; main_smd_opcode_8f */
    SUZUKI_SMD_OPCODE_END_BAR = 0x90, /* jumps to the loop point; ends the channel when none is set */
    SUZUKI_SMD_OPCODE_LOOP = 0x91,    /* sets the loop point */
    SUZUKI_SMD_OPCODE_OCTAVE = 0x94,  /* octave */
    SUZUKI_SMD_OPCODE_RAISE_OCTAVE = 0x95,
    SUZUKI_SMD_OPCODE_LOWER_OCTAVE = 0x96,
    SUZUKI_SMD_OPCODE_TIME_SIGNATURE = 0x97, /* beats per bar, beat unit */
    SUZUKI_SMD_OPCODE_REPEAT = 0x98,         /* pass count: opens a repeated section */
    SUZUKI_SMD_OPCODE_CODA = 0x99,           /* closes the section: jumps back while passes remain */
    SUZUKI_SMD_OPCODE_TO_CODA = 0x9a,        /* on the last pass, jumps past the section's Coda */
    SUZUKI_SMD_OPCODE_PLAY_SFX = 0x9c,       /* sound id low, high, volume */
    /* Sound id low, high. g_main_smd_opcode_lengths counts three parameter
     * bytes, one more than the handler consumes. */
    SUZUKI_SMD_OPCODE_STOP_SFX = 0x9d,
    SUZUKI_SMD_OPCODE_PLAY_VFX = 0x9e,           /* sound low, high, stream: continues in a loaded sound resource */
    SUZUKI_SMD_OPCODE_TEMPO = 0xa0,              /* tempo: 75/64 quarter notes a minute per unit */
    SUZUKI_SMD_OPCODE_ACCELERANDO = 0xa1,        /* signed tempo delta */
    SUZUKI_SMD_OPCODE_TEMPO_RAMP = 0xa2,         /* ticks, target tempo */
    SUZUKI_SMD_OPCODE_UNKNOWN_A4 = 0xa4,         /* value: sets music->_unknown_01c; main_smd_opcode_a4 */
    SUZUKI_SMD_OPCODE_UNKNOWN_A5 = 0xa5,         /* delta: adds to music->_unknown_01c; main_smd_opcode_a5 */
    SUZUKI_SMD_OPCODE_MASTER_VOLUME = 0xa6,      /* volume of the whole music */
    SUZUKI_SMD_OPCODE_MASTER_VOLUME_RAMP = 0xa7, /* time in 32/240 s units, target volume */
    SUZUKI_SMD_OPCODE_GATE_TIME = 0xa9,  /* sixteenths of each note's length that stay keyed on; main_smd_gate_time */
    SUZUKI_SMD_OPCODE_SET_VOICE = 0xaa,  /* SPU voice number */
    SUZUKI_SMD_OPCODE_INSTRUMENT = 0xac, /* instrument of the channel's waveset */
    SUZUKI_SMD_OPCODE_LENGTH_ADJUST
    = 0xad,                              /* signed ticks added to length_adjust, 0 clears it; main_smd_length_adjust */
    SUZUKI_SMD_OPCODE_NOP_AE = 0xae,     /* nothing; main_smd_opcode_ae */
    SUZUKI_SMD_OPCODE_NOP_AF = 0xaf,     /* nothing; main_smd_opcode_af */
    SUZUKI_SMD_OPCODE_SLUR_ON = 0xb0,    /* every note is tied to the next; main_smd_slur_on */
    SUZUKI_SMD_OPCODE_SLUR_OFF = 0xb1,   /* main_smd_slur_off */
    SUZUKI_SMD_OPCODE_ENABLE_LFO = 0xb2, /* SPU pitch LFO on the channel's voice (odd voices only) */
    SUZUKI_SMD_OPCODE_DISABLE_LFO = 0xb3,
    SUZUKI_SMD_OPCODE_ENABLE_NOISE = 0xb4,   /* noise clock */
    SUZUKI_SMD_OPCODE_INCREASE_NOISE = 0xb5, /* noise clock delta, modulo 64 */
    SUZUKI_SMD_OPCODE_TURN_ON_NOISE = 0xb6,  /* noise with the clock left as it is */
    SUZUKI_SMD_OPCODE_DISABLE_NOISE = 0xb7,
    SUZUKI_SMD_OPCODE_ACOUSTICS = 0xb8, /* reverb depth, delay, feedback */
    SUZUKI_SMD_OPCODE_ENABLE_REVERB = 0xba,
    SUZUKI_SMD_OPCODE_DISABLE_REVERB = 0xbb,
    SUZUKI_SMD_OPCODE_NATURALE = 0xc0,                /* reloads the channel's instrument */
    SUZUKI_SMD_OPCODE_ENVELOPE_SHAPE = 0xc1,          /* attack, sustain and release modes */
    SUZUKI_SMD_OPCODE_ATTACK_RATE = 0xc2,             /* rate; main_smd_attack_time */
    SUZUKI_SMD_OPCODE_DECAY_RATE = 0xc3,              /* rate; main_smd_decay_time */
    SUZUKI_SMD_OPCODE_SUSTAIN_RATE = 0xc4,            /* rate; main_smd_sustain_time */
    SUZUKI_SMD_OPCODE_RELEASE_RATE = 0xc5,            /* rate; main_smd_release */
    SUZUKI_SMD_OPCODE_SUSTAIN_LEVEL = 0xc6,           /* level */
    SUZUKI_SMD_OPCODE_ENVELOPE_TAIL = 0xc7,           /* decay rate, sustain level */
    SUZUKI_SMD_OPCODE_ATTACK_MODE = 0xc8,             /* SPU envelope mode */
    SUZUKI_SMD_OPCODE_SUSTAIN_MODE = 0xc9,            /* SPU envelope mode */
    SUZUKI_SMD_OPCODE_RELEASE_MODE = 0xca,            /* SPU envelope mode */
    SUZUKI_SMD_OPCODE_PITCH_OFFSET = 0xd0,            /* signed, 1/8 semitone; main_smd_pitch_offset */
    SUZUKI_SMD_OPCODE_SHIFT_PITCH_OFFSET = 0xd1,      /* signed delta, 1/8 semitone; main_smd_add_pitch_offset_coarse */
    SUZUKI_SMD_OPCODE_SHIFT_PITCH_OFFSET_FINE = 0xd2, /* signed delta, 1/32 semitone; main_smd_add_pitch_offset_fine */
    /* Signed 16-bit delta in 1/256 semitone, high byte first; main_smd_add_pitch_offset_wide. */
    SUZUKI_SMD_OPCODE_SHIFT_PITCH_OFFSET_WORD = 0xd3,
    SUZUKI_SMD_OPCODE_PITCH_SLIDE = 0xd4, /* ticks, signed semitones */
    SUZUKI_SMD_OPCODE_PITCH_SLIDE_HOLD
    = 0xd5,                              /* toggles SUZUKI_RAMP_PITCH_SLIDE_HOLD; main_smd_toggle_pitch_slide_hold */
    SUZUKI_SMD_OPCODE_PORTAMENTO = 0xd6, /* ticks a key-on slides from the previous key, 0 off; main_smd_portamento */
    /* The vibrato opcodes program modulator 0 on the pitch; their handlers
     * are main_smd_pitch_shift_fade_in, main_smd_pitch_shift,
     * main_smd_pitch_shift_waveform, main_smd_pitch_shift_on and
     * main_smd_pitch_shift_off. */
    SUZUKI_SMD_OPCODE_VIBRATO_FADE_IN = 0xd7,  /* fade-in length in ticks, less one */
    SUZUKI_SMD_OPCODE_VIBRATO = 0xd8,          /* speed, signed depth, delay: a centred triangle */
    SUZUKI_SMD_OPCODE_VIBRATO_WAVEFORM = 0xd9, /* speed, signed depth, waveform (bit 4: no restart on key-on) */
    SUZUKI_SMD_OPCODE_VIBRATO_ON = 0xda,
    SUZUKI_SMD_OPCODE_VIBRATO_OFF = 0xdb,
    SUZUKI_SMD_OPCODE_PITCH_SLIDE_OFF = 0xdc, /* main_smd_pitch_slide_off */
    SUZUKI_SMD_OPCODE_DYNAMIC = 0xe0,         /* channel volume */
    SUZUKI_SMD_OPCODE_CRESCENDO = 0xe1,       /* signed volume delta */
    SUZUKI_SMD_OPCODE_VOLUME_RAMP = 0xe2,     /* ticks, target volume; main_smd_fermata_ramp */
    /* The tremolo opcodes program modulator 1 on the volume. */
    SUZUKI_SMD_OPCODE_TREMOLO_FADE_IN = 0xe3,  /* fade-in length in ticks, less one */
    SUZUKI_SMD_OPCODE_TREMOLO = 0xe4,          /* speed, signed depth, delay: a triangle */
    SUZUKI_SMD_OPCODE_TREMOLO_WAVEFORM = 0xe5, /* speed, signed depth, waveform (bit 4: no restart on key-on) */
    SUZUKI_SMD_OPCODE_TREMOLO_ON = 0xe6,
    SUZUKI_SMD_OPCODE_TREMOLO_OFF = 0xe7,
    SUZUKI_SMD_OPCODE_BALANCE = 0xe8,       /* balance: 0 left, 0x40 centre, 0x7f right */
    SUZUKI_SMD_OPCODE_SHIFT_BALANCE = 0xe9, /* signed balance delta */
    SUZUKI_SMD_OPCODE_BALANCE_RAMP = 0xea,  /* ticks, target balance */
    /* The pan LFO opcodes program modulator 2 on the balance. */
    SUZUKI_SMD_OPCODE_PAN_LFO_FADE_IN = 0xeb,  /* fade-in length in ticks, less one */
    SUZUKI_SMD_OPCODE_PAN_LFO = 0xec,          /* speed, signed depth, delay: a centred triangle */
    SUZUKI_SMD_OPCODE_PAN_LFO_WAVEFORM = 0xed, /* speed, signed depth, waveform (bit 4: no restart on key-on) */
    SUZUKI_SMD_OPCODE_PAN_LFO_ON = 0xee,
    SUZUKI_SMD_OPCODE_PAN_LFO_OFF = 0xef,
    /* Modulator index, waveform (bit 4: no restart on key-on), target: selects
     * the modulator the next two opcodes program. */
    SUZUKI_SMD_OPCODE_MODULATOR_SELECT = 0xf0,
    SUZUKI_SMD_OPCODE_MODULATOR_SET_DEPTH = 0xf1,   /* speed, signed depth high, depth low */
    SUZUKI_SMD_OPCODE_MODULATOR_SET_FADE_IN = 0xf2, /* delay, fade-in length in ticks less one */
    SUZUKI_SMD_OPCODE_MODULATOR_ENABLE_MASK = 0xf5, /* bit n: modulator n restarts and runs, clear stops it */
    SUZUKI_SMD_OPCODE_MODULATOR_ENABLE = 0xf6,      /* modulator index */
    SUZUKI_SMD_OPCODE_MODULATOR_DISABLE = 0xf7,     /* modulator index */
    SUZUKI_SMD_OPCODE_SCALE_TEMPO = 0xfd,           /* tempo multiplier, 0 ignored */
    SUZUKI_SMD_OPCODE_SELECT_SOUND_FONT = 0xfe,     /* waveset id */
    SUZUKI_SMD_OPCODE_END_TRACK = 0xff,             /* ends the channel once its voice's envelope has run out */
} suzuki_smd_opcode_e;

/* Suzuki MUS (music sequence) record: 0xb8 bytes followed by channel_count
 * channels, allocated by SuzukiPutPlaySMD. It is the second argument of every
 * SMD opcode handler. Queued records form a linked list through next, headed
 * by g_main_sound_active_music_list; snapshot chains saved copies of the
 * whole record (main_smd_save_snapshot) through their own snapshot fields.
 *
 * status (suzuki_music_status_e) has bit 15 set while the music plays
 * (SuzukiGetMusicPlaying); main_sound_calculate_reverb_voices tests that bit
 * as a sign (`lh` and `bgez`), and bit 0 there marks the reverb voices as
 * excludable.
 *
 * tempo is a signed 16.16 value. Scale Tempo reads its integer part; Tempo,
 * Accelerando and Tempo Ramp update the whole word. */
typedef struct suzuki_music {
    struct suzuki_music* next;     /* 0x00 */
    struct suzuki_music* snapshot; /* 0x04; freed by main_smd_free_snapshots */
    suzuki_smd_header_t* smd;      /* 0x08 */
    u8 _unused_00c[4];             /* 0x0c */
    s16 status;                    /* 0x10; suzuki_music_status_e */
    u16 id;                        /* 0x12; SMD id */
    u8 _unknown_014;               /* 0x14; SMD _unknown_12, 1 for the SFX record */
    u8 tick_divisor;               /* 0x15 */
    u8 channel_count;              /* 0x16 */
    u8 _unknown_017;               /* 0x17; SMD _unknown_15, 0 for the SFX record */
    s16 waveset_id;                /* 0x18; sound font id */
    u16 _unknown_01a;              /* 0x1a; SMD _unknown_18, 0x7f for SFX */
    u8 _unknown_01c;               /* 0x1c; set by opcode 0xA4, added to by 0xA5, cleared by the header reset */
    u8 loop_select;                /* 0x1d; the Loop Select opcode whose byte equals it sets the loop point */
    u16 noise_clock;               /* 0x1e; SpuSetNoiseClock value (Enable Noise, Increase Noise) */
    s32 timer_ticks;               /* 0x20; incremented every tick */
    u32 sequence_ticks;            /* 0x24; ticks while the record owns voices */
    u32 ticks;                     /* 0x28; root-counter ticks played, in 1/256 units (tempo scale) */
    u32 snapshot_restore_tick;     /* 0x2c; restore the snapshot when sequence_ticks reaches it (0: never) */
    u16 min_loop_count;            /* 0x30; main_smd_get_min_loop_count */
    u16 bar;                       /* 0x32; 1-based bar counter */
    u16 beat;                      /* 0x34; beat within the bar */
    u16 beat_ticks_remaining;      /* 0x36; ticks left in the beat */
    u16 beats_per_bar;             /* 0x38 */
    u16 ticks_per_beat;            /* 0x3a */
    u16 beat_unit;                 /* 0x3c */
    u16 beats_per_bar_copy;        /* 0x3e; write-only */
    u8 _unused_040[4];             /* 0x40 */
    s32 reverb_mode;               /* 0x44 */
    s16 reverb_depth;              /* 0x48; depth byte << 8 */
    u8 _padding_04a[2];            /* 0x4a; aligns reverb_delay */
    s32 reverb_delay;              /* 0x4c */
    s32 reverb_feedback;           /* 0x50 */
    u16 stop_bar;                  /* 0x54; main_smd_set_stop_bar: the music stops when bar reaches it */
    u8 _padding_056[2];            /* 0x56; aligns channel_mask */
    u32 channel_mask;              /* 0x58; bit n: channel n has note data and is still running */
    u32 mute_mask;                 /* 0x5c; muted channels */
    u32 key_on_mask;               /* 0x60 */
    u32 key_off_mask;              /* 0x64 */
    u32 lfo_voice_mask;            /* 0x68 */
    u32 noise_voice_mask;          /* 0x6c */
    u32 reverb_voice_mask;         /* 0x70 */
    s32 tempo_accumulator;         /* 0x74; one sequencer tick per 0x10000 of scaled_tempo; initialised to 0x10000 */
    s32 scaled_tempo;              /* 0x78; tempo * tempo_scale */
    s32 tempo;                     /* 0x7c; 16.16 tempo advanced by Tempo Ramp */
    s32 tempo_step;                /* 0x80; per-tick tempo delta of Tempo Ramp */
    u16 tempo_step_count;          /* 0x84 */
    u16 tempo_target;              /* 0x86; tempo byte Tempo Ramp ends on */
    suzuki_ramp_t tempo_scale;     /* 0x88; value is scale byte << 24, its high halfword the multiplier */
    suzuki_ramp_t master_volume;   /* 0x94; value is volume byte << 24 */
    suzuki_ramp_t pitch_shift;     /* 0xa0 */
    suzuki_ramp_t balance_shift;   /* 0xac */
    suzuki_music_channel_t channels[0]; /* 0xb8; channel_count entries */
} suzuki_music_t;
typedef char suzuki_music_header_size_must_be_0xb8[(sizeof(suzuki_music_t) == 0xb8) ? 1 : -1];

/* suzuki_music_t.status. */
typedef enum suzuki_music_status {
    SUZUKI_MUSIC_STATUS_SMD = 0x0001,            /* the record plays SMD music */
    SUZUKI_MUSIC_STATUS_SFX = 0x0002,            /* the sound-effect record */
    SUZUKI_MUSIC_STATUS_SNAPSHOT = 0x0010,       /* a saved copy exists */
    SUZUKI_MUSIC_STATUS_NO_VOICE_ATTRS = 0x0020, /* never set; main_sound_update_voice_attrs skips the record */
    SUZUKI_MUSIC_STATUS_PAUSED = 0x0100,         /* main_smd_pause_music; cleared on resume */
    /* Never set; main_smd_init_channels gives every channel
     * SUZUKI_CHANNEL_IMMEDIATE_INSTRUMENT. */
    SUZUKI_MUSIC_STATUS_IMMEDIATE_INSTRUMENT = 0x2000,
    SUZUKI_MUSIC_STATUS_FULL_VOICE_UPDATE = 0x4000, /* push every voice attribute once */
    SUZUKI_MUSIC_STATUS_PLAYING = 0x8000,
} suzuki_music_status_e;

/* Loaded sound resource (a "feds" block: SYSTEM.SED, ENV.SED, or the block
 * inside an effect file or WLDCORE.BIN), kept by the Suzuki driver in a
 * singly linked list through next (SuzukiAppendVFXSMD). main_sound_start_sfx
 * and Play VFX (0x80015c38) match id and index channel_offsets. Each sound
 * has two streams, so one sound plays on a pair of SFX channels. */
typedef struct main_sound_resource {
    u8 _unused_00[0xa];               /* 0x00; "feds", the size at 0x04, the sound count at 0x08 */
    u16 id;                           /* 0x0a; main_sound_resource_id_e, the high half of a sound id */
    u16 volume_offset;                /* 0x0c; offset of the per-sound u8 volume table (0x80 = full) */
    u8 _padding_0e[2];                /* 0x0e; aligns next */
    struct main_sound_resource* next; /* 0x10 */
    u16 channel_offsets[0];           /* 0x14; two stream offsets per sound, 0 for no stream */
} main_sound_resource_t;

/* Driver SPU state at 0x80037020: the common attributes passed to LIBSPU,
 * the volumes they and g_main_sound_reverb_attr (0x80037008) are built from,
 * and two volume ramps stepped by the root-counter handler (0x800149dc). The
 * reverb attributes are a separate object: every user addresses them from
 * their own base, not from this record's. */
typedef struct suzuki_spu_state {
    SpuCommonAttr common;     /* 0x00 0x80037020 */
    s16 music_volume;         /* 0x28 0x80037048; applied to common.mvol */
    s16 cd_volume;            /* 0x2a 0x8003704a; applied to common.cd.volume */
    s16 reverb_depth;         /* 0x2c 0x8003704c; applied to g_main_sound_reverb_attr.depth */
    s16 cd_mix_level;         /* 0x2e 0x8003704e; the CdMix level last given to main_sound_put_type */
    suzuki_ramp_t music_ramp; /* 0x30 0x80037050 */
    suzuki_ramp_t cd_ramp;    /* 0x3c 0x8003705c */
} suzuki_spu_state_t;
typedef char suzuki_spu_state_size_must_be_0x48[(sizeof(suzuki_spu_state_t) == 0x48) ? 1 : -1];

/* g_main_sound_driver_flags. */
typedef enum main_sound_driver_flag {
    MAIN_SOUND_DRIVER_ROOT_COUNTER_EVENT = 0x0001, /* the root-counter-2 event is enabled */
    /* An SPU RAM transfer is in progress; the transfer callback clears it
     * together with SPU_WRITE and SPU_READ. */
    MAIN_SOUND_DRIVER_SPU_TRANSFER = 0x0010,
    MAIN_SOUND_DRIVER_SPU_WRITE = 0x0020,
    MAIN_SOUND_DRIVER_SPU_READ = 0x0040,
    MAIN_SOUND_DRIVER_STEREO = 0x0100,        /* every sound type but mono */
    MAIN_SOUND_DRIVER_WIDE = 0x0200,          /* MAIN_SOUND_TYPE_WIDE */
    MAIN_SOUND_DRIVER_WIDE_OPPOSITE = 0x0400, /* MAIN_SOUND_TYPE_WIDE_OPPOSITE */
    MAIN_SOUND_DRIVER_SFX_ENABLED = 0x1000,   /* main_sound_set_sfx_enabled */
    /* Only the flags argument of SuzukiSPUInitialiser can set it (the game
     * passes 0): start-up and main_sound_set_type also call main_sound_put_type. */
    MAIN_SOUND_DRIVER_PUT_TYPE = 0x2000,
    MAIN_SOUND_DRIVER_INITIALISED = 0x8000,
} main_sound_driver_flag_e;

/* Output sound type of main_sound_set_type and main_sound_get_type (which
 * reports both wide types as WIDE). */
typedef enum main_sound_type {
    MAIN_SOUND_TYPE_MONO = 0,
    MAIN_SOUND_TYPE_STEREO = 1,
    MAIN_SOUND_TYPE_WIDE = 2,          /* one side phase-inverted */
    MAIN_SOUND_TYPE_WIDE_OPPOSITE = 3, /* the other side inverted; no menu offers it */
} main_sound_type_e;

/* Low nibble of the mode argument of main_sound_transfer_spu_data. */
typedef enum main_sound_spu_transfer {
    MAIN_SOUND_SPU_TRANSFER_WRITE = 1,
    MAIN_SOUND_SPU_TRANSFER_READ = 2,
    MAIN_SOUND_SPU_TRANSFER_READ_DECODED = 3,    /* SpuReadDecodedData, every buffer */
    MAIN_SOUND_SPU_TRANSFER_READ_DECODED_CD = 4, /* SpuReadDecodedData, CD buffers only */
    MAIN_SOUND_SPU_TRANSFER_NONE = 5,
    /* Flag of main_sound_wait_for_spu_transfer: wait for the running transfer
     * to end. main_sound_load_waveset also sets it in its transfer mode,
     * where only the low nibble is read. */
    MAIN_SOUND_SPU_TRANSFER_WAIT = 0x10,
} main_sound_spu_transfer_e;

/* Mode argument of main_smd_dispatch_snapshot. */
typedef enum suzuki_snapshot_mode {
    SUZUKI_SNAPSHOT_DISCARD = 0,
    SUZUKI_SNAPSHOT_SAVE = 1,
    SUZUKI_SNAPSHOT_RESTORE = 2,
} suzuki_snapshot_mode_e;

/* SMD opcode handler, called through g_main_smd_opcode_handlers by
 * main_smd_read_instructions with the byte after the opcode; it returns the
 * next read position. Handlers that ignore an argument may declare it
 * void*; the ABI is the same. */
typedef u8* (*suzuki_smd_handler_t)(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel);

/* Scenario-music playback state at 0x8004d95c (SCUS_942.21). current_music
 * is the Suzuki music record of the playing scenario music:
 * main_sound_stop_current_music hands it straight to
 * SuzukiDeallocateMUSChannels, which takes a suzuki_music_t*. */
typedef struct main_sound_music_state {
    s32 current_slot;                 /* 0x00 0x8004d95c slot of the playing scenario music */
    suzuki_music_t* current_music;    /* 0x04 0x8004d960 */
    suzuki_smd_header_t* current_smd; /* 0x08 0x8004d964 SMD data of the playing music */
    s32 volume;                       /* 0x0c 0x8004d968 */
} main_sound_music_state_t;

/* Music slots 1..2 hold scenario music; slots 3..7 hold the five permanent
 * tunes. Both arrays store slot n at index n - 1. */
typedef struct main_sound_music_slots {
    s32 tune;                    /* 0x00: pending tune (id | 0x80) or playing tune id */
    s32 loading_slot;            /* 0x04: slot whose SMD load is in flight, or zero */
    suzuki_music_t* music[7];    /* 0x08: music records returned by SuzukiPutPlaySMD */
    suzuki_smd_header_t* smd[7]; /* 0x24: loaded SMD data */
} main_sound_music_slots_t;

/* The music state and slot table are one object: main_sound_open_generic_sfx
 * derives the slot words from the &state.current_smd base register, and the
 * tune/slot helpers reach across tune, handles and smd from single bases. */
typedef struct main_sound_music {
    main_sound_music_state_t state; /* 0x00 0x8004d95c */
    main_sound_music_slots_t slots; /* 0x10 0x8004d96c */
} main_sound_music_t;

/* Music ids: the index of a MUSIC_NN.SMD file in
 * g_main_sound_scenario_smd_files (MAIN_MUSIC_COUNT entries). Only the ids
 * the code passes by number are named, after the screen that plays them;
 * event scripts and scenario data choose the others. The music test offers
 * ids 1-96 (g_open_music_test_music_ids). */
typedef enum main_music {
    MAIN_MUSIC_NONE = 0,         /* the *_sound_play_music_tracks functions and the WLDCORE audio queue skip it */
    MAIN_MUSIC_ALTIMA = 0x13,    /* EVENT_CALL_FUNCTION_PLAY_ALTIMA_MUSIC */
    MAIN_MUSIC_MILESTONE = 0x14, /* wldcore_map_step_discovery_announcement, in the bar */
    MAIN_MUSIC_TUTORIAL = 0x1a,  /* the world menu's tutorial screen */
    MAIN_MUSIC_WORLD_MAP = 0x1b,
    MAIN_MUSIC_SHOP = 0x1c,                         /* world_shop_run_screen: the item shop */
    MAIN_MUSIC_SOLDIER_OFFICE = 0x1d,               /* world_shop_run_screen */
    MAIN_MUSIC_FUR_SHOP = 0x1e,                     /* world_shop_run_screen */
    MAIN_MUSIC_FORMATION = 0x1f,                    /* world_formation_run_screen */
    MAIN_MUSIC_BRAVE_STORY = 0x20,                  /* the Brave Story screens */
    MAIN_MUSIC_BAR = 0x21,                          /* the bar of a town */
    MAIN_MUSIC_NEW_GAME = 0x22,                     /* name and birthday entry of a new game */
    MAIN_MUSIC_PROPOSITION_REPORT = 0x23,           /* a finished proposition's report */
    MAIN_MUSIC_SAVE = 0x29,                         /* the memory card screens */
    MAIN_MUSIC_DEPLOYMENT = 0x2a,                   /* the ATTACK unit placement screen */
    MAIN_MUSIC_CONGRATULATIONS = 0x2b,              /* require_text_show_battle_congratulations */
    MAIN_MUSIC_CONGRATULATIONS_FINAL_BATTLE = 0x2c, /* the same screen after event 0x145 */
    MAIN_MUSIC_GAME_OVER = 0x32,                    /* etc_graphic_show_game_over */
    MAIN_MUSIC_COUNT = 100,
} main_music_e;

/* Tunes of main_sound_play_tune: short pieces that fade the scenario music
 * out, play once and fade it back in (main_sound_update_tunes). Tune t is
 * MUSIC_(44 + t).SMD, kept loaded in music slot 2 + t. Tune 4 has no caller
 * in the code; EVENT_OPCODE_PLAY_TUNE passes its operand. */
typedef enum main_sound_tune {
    MAIN_SOUND_TUNE_NONE = 0,
    MAIN_SOUND_TUNE_LEVEL_UP = 1,
    MAIN_SOUND_TUNE_ITEM_FOUND = 2, /* an item found by moving, a poach, a trained monster */
    MAIN_SOUND_TUNE_JOB_LEVEL_UP = 3,
    MAIN_SOUND_TUNE_JOB_CHANGE = 5,
    MAIN_SOUND_TUNE_ID_MASK = 0x3f, /* the id in g_main_sound_music.slots.tune */
    MAIN_SOUND_TUNE_PENDING = 0x80, /* set there until the scenario music has faded out */
} main_sound_tune_e;

/* Steal priority of an SFX channel, the high byte of a main_sound_start_sfx
 * channel id. When no channel pair is free, main_sound_find_sfx_channel takes
 * the oldest busy one whose priority is at most STEALABLE. */
typedef enum main_sound_sfx_priority {
    MAIN_SOUND_SFX_PRIORITY_STEALABLE = 0x20,  /* a sound on a searched or caller-named channel pair */
    MAIN_SOUND_SFX_PRIORITY_BACKGROUND = 0x60, /* SuzukiPlaySound2 (main_sound_play_bg_sfx), channels 4-5 */
    MAIN_SOUND_SFX_PRIORITY_DIRECT = 0x80,     /* SuzukiPlaySound1 (main_sound_play_sfx), channels 6-7 */
} main_sound_sfx_priority_e;

#define MAIN_SOUND_SFX_CHANNEL_ID(priority, channel) ((s16)(((priority) << 8) | (channel)))

/* Sound resources. A sound id is MAIN_SOUND_ID(resource, sound): the id of a
 * loaded main_sound_resource_t in the high halfword and the index of a
 * sound in it in the low. Each battle effect file brings a resource of its
 * own (battle_effect_load_sound_section). */
typedef enum main_sound_resource_id {
    MAIN_SOUND_RESOURCE_SYSTEM = 0,         /* SOUND/SYSTEM.SED: main_sfx_e */
    MAIN_SOUND_RESOURCE_ENV = 1,            /* SOUND/ENV.SED: main_env_sfx_e */
    MAIN_SOUND_RESOURCE_SOUND_NOVEL = 0xe1, /* g_wldcore_sound_novel_sound_resource, 81 sounds */
} main_sound_resource_id_e;

#define MAIN_SOUND_ID(resource, sound) (((resource) << 16) | (sound))

/* Sound-effect ids passed to main_sound_play_sfx and the module queues that
 * feed it (wldcore_sound_play_effect, world_sound_dispatch_effect and the
 * g_*_sound_*effect_id globals); every path reaches main_sound_start_sfx with
 * MAIN_SOUND_RESOURCE_SYSTEM in the high halfword, so this is one id space.
 * SYSTEM.SED holds MAIN_SFX_COUNT sounds; the ones named here are named
 * from their users in the code, the others are played only by event and
 * animation script data. The event frames store -1 in the pending globals
 * for "no request"; the dispatchers turn a negative id into MAIN_SFX_NONE. */
typedef enum main_sfx {
    /* Nothing queued. SYSTEM.SED entry 0 has no streams, so playing it stops
     * the channel pair main_sound_play_sfx uses;
     * wldcore_menu_step_map_path_level plays it when the path animation ends. */
    MAIN_SFX_NONE = 0,
    MAIN_SFX_CONFIRM = 1,
    MAIN_SFX_CANCEL = 2,
    MAIN_SFX_CURSOR_MOVE = 3,
    MAIN_SFX_GIL_TICKER = 4, /* looping count-up of the REQUIRE gil reward */
    MAIN_SFX_INVALID = 5,
    MAIN_SFX_PAGE_SWITCH = 6, /* L1/R1 unit and page switches */
    MAIN_SFX_UNEQUIP = 7,     /* remove equipment/abilities */
    MAIN_SFX_RED_ORBS = 8,    /* battle_effect_red_orbs_secondary_handler starts */
    MAIN_SFX_DISMISS = 9,     /* dismiss unit */
    MAIN_SFX_EQUIP = 0xa,     /* equip, set ability, best fit */
    MAIN_SFX_CAMERA_ROTATE_LEFT = 0xb,
    MAIN_SFX_CAMERA_ROTATE_RIGHT = 0xc,
    MAIN_SFX_CAMERA_ROTATE_STOP = 0xd, /* the rotation reached its target */
    /* Paired zoom sounds: battle_camera_toggle_zoom and battle_camera_zoom_map
     * play 0x11 with zoom action 4 and 0xe with action 2; the in/out reading
     * follows the zoom-target comment in battle_camera_toggle_zoom. The
     * ATTACK deploy screen also plays 0x11 when a placed unit is moved. */
    MAIN_SFX_CAMERA_ZOOM_IN = 0xe,
    /* L1/R1 on the world map (wldcore_menu_step_map_cursor_level): 0x10 when
     * the toggle sets the zoomed-out bit of g_wldcore_map_zoom_motion, 0xf
     * when it clears it. */
    MAIN_SFX_WORLD_MAP_ZOOM_IN = 0xf,
    MAIN_SFX_WORLD_MAP_ZOOM_OUT = 0x10,
    MAIN_SFX_CAMERA_ZOOM_OUT = 0x11,
    MAIN_SFX_WINDOW_OPEN = 0x12, /* help, message or text window opens */
    /* The weapon sounds of battle_sound_select_weapon_hit, by the weapon's
     * item_type_e: g_battle_sound_weapon_miss_sfx_ids (the swing, played
     * before the hit is known), g_battle_sound_weapon_hit_sfx_ids and
     * g_battle_sound_shield_deflect_sfx_ids. */
    MAIN_SFX_HIT_BLADE = 0x13,    /* knife, ninja blade, sword, knight sword, katana */
    MAIN_SFX_HIT_HEAVY = 0x15,    /* axe, flail; also the deflect of a fist, bag or cloth */
    MAIN_SFX_HIT_UNARMED = 0x18,  /* fist, bag, cloth */
    MAIN_SFX_SWING_LIGHT = 0x19,  /* fist, knife, ninja blade, instrument, book, bag, cloth */
    MAIN_SFX_SWING_MEDIUM = 0x1a, /* sword, knight sword, katana, rod, staff */
    MAIN_SFX_SWING_HEAVY = 0x1b,  /* axe, flail, polearm, pole */
    /* Started by the spell-charge and summon-charge secondary effects, which
     * stop them with main_sound_stop_sfx_channels when they finish. */
    MAIN_SFX_SPELL_CHARGE = 0x1f,
    MAIN_SFX_SUMMON_CHARGE = 0x21,
    MAIN_SFX_VENOM_TRAP = 0x22, /* battle_effect_venom_trap_secondary_handler starts */
    /* battle_sound_play_landing_sfx_by_surface_type: water surfaces play
     * 0x23, grassland, thicket and rug 0x28, wooden floor, tree and box 0x3a,
     * every other surface 0x29. A step that ends on another unit also plays
     * 0x28, and mounting a unit named by battle id plays 0x29. */
    MAIN_SFX_LAND_WATER = 0x23,
    MAIN_SFX_JUMP = 0x27, /* a jump rise starts; the fall that ends a mount ride starts */
    MAIN_SFX_LAND_SOFT = 0x28,
    MAIN_SFX_LAND_DEFAULT = 0x29,
    MAIN_SFX_TEXT_PAGE = 0x2d,            /* dialogue/scroll window advances a page */
    MAIN_SFX_SHIELD_DEFLECT_LIGHT = 0x2e, /* blades, rod, staff, polearm, pole */
    MAIN_SFX_SHIELD_DEFLECT_HEAVY = 0x2f, /* axe, flail */
    /* An error message opens: a memory-card load, save or scan failure, or a
     * refused equip, dismissal, job change or ability.
     * battle_sound_select_weapon_hit plays it for an attack stopped by an
     * accessory or class evade, a forced failure, a blade grasp or a catch,
     * and as the shield deflect of an instrument or book. */
    MAIN_SFX_ERROR_MESSAGE = 0x30,
    MAIN_SFX_CAMERA_TILT = 0x31, /* battle_camera_handle_tilt_request */
    MAIN_SFX_HIT_ROD = 0x38,     /* rod, staff */
    MAIN_SFX_LAND_WOOD = 0x3a,
    /* Circle on a name-entry character key, or on an occupied tile of the
     * ATTACK deploy screen. */
    MAIN_SFX_SELECT = 0x3c,
    MAIN_SFX_HIT_POLE = 0x3e,         /* polearm, pole */
    MAIN_SFX_EVENT_REVIVE = 0x41,     /* the event script's revive status change (*_script_inflict_status_thread) */
    MAIN_SFX_WORLD_MAP_ARRIVE = 0x43, /* a route leg ends at a location with no event or encounter */
    /* Dead status added outside an event
     * (battle_unit_update_animation_for_status_changes), chosen by the unit's
     * male, female or monster flag; the male sound is 0x9a. */
    MAIN_SFX_DEATH_FEMALE = 0x45,
    MAIN_SFX_DEATH_MONSTER = 0x46,
    MAIN_SFX_SHIELD_DEFLECT_BULLET = 0x47, /* gun */
    MAIN_SFX_SHIELD_DEFLECT_ARROW = 0x49,  /* crossbow, bow */
    /* Proposition result (g_wldcore_job_selection.result): 0 plays the
     * 0x4d/0x4e pair through main_sound_play_2_sfx, 1 the 0x4f/0x50 pair,
     * 2 plays 0x51. */
    MAIN_SFX_PROPOSITION_SUCCESS_A = 0x4d,
    MAIN_SFX_PROPOSITION_SUCCESS_B = 0x4e,
    MAIN_SFX_PROPOSITION_PARTIAL_A = 0x4f,
    MAIN_SFX_PROPOSITION_PARTIAL_B = 0x50,
    MAIN_SFX_PROPOSITION_FAILURE = 0x51,
    MAIN_SFX_SWING_BOW = 0x52,
    MAIN_SFX_SWING_CROSSBOW = 0x56,
    MAIN_SFX_SWING_GUN = 0x5a,
    MAIN_SFX_HIT_GUN = 0x5b,
    MAIN_SFX_TELEPORT_OUT = 0x6a,
    MAIN_SFX_TELEPORT_IN = 0x6b,
    /* Animation step 4 of the world event-transition indicator
     * (wldcore_menu_step_event_transition_level): 0x6c in mode 0, which a
     * random encounter pushes, 0x6d in the other spinning modes. */
    MAIN_SFX_EVENT_TRANSITION_MODE_0 = 0x6c,
    MAIN_SFX_EVENT_TRANSITION_MODE_1 = 0x6d,
    MAIN_SFX_STATUS_MENU_REQUEST
    = 0x71,                       /* Triangle queues the status menu on another unit's turn; again when it opens */
    MAIN_SFX_CRITICAL_HIT = 0x72, /* a weapon hit whose action is critical (battle_sound_select_weapon_hit) */
    MAIN_SFX_TEXT_GLYPH = 0x73,   /* typewriter tick after a dialogue glyph */
    MAIN_SFX_HIT_ARROW = 0x74,    /* crossbow, bow */
    /* World map: 0x76 when a path animation starts, drawing or erasing; 0x77
     * when a hidden location is made visible. */
    MAIN_SFX_WORLD_MAP_PATH = 0x76,
    MAIN_SFX_WORLD_MAP_LOCATION_APPEAR = 0x77,
    /* A save or load finished, Start pressed in the opening's new-game
     * transition or to end deployment, a crystal or treasure picked up, or a
     * proposition report showing its reward picture. */
    MAIN_SFX_COMPLETE = 0x85,
    MAIN_SFX_SHOP_TRANSACTION = 0x97, /* shop buy/sell confirmed */
    MAIN_SFX_ABILITY_LEARNED = 0x98,  /* an ability bought in the learn list */
    MAIN_SFX_DEATH_MALE = 0x9a,
    MAIN_SFX_HIT_BOOK = 0x9c, /* instrument, book */
    MAIN_SFX_COUNT = 0xa8,    /* the sound count in the SYSTEM.SED header */
} main_sfx_e;

/* Sounds of MAIN_SOUND_RESOURCE_ENV that the code plays by number. ENV.SED
 * holds 27 (0, 0x15, 0x16 and 0x17 are empty); event scripts choose the
 * others through EVENT_OPCODE_BG_SOUND. */
typedef enum main_env_sfx {
    MAIN_ENV_SFX_RAIN = 2,      /* battle_map_run_weather_command: rain without snow, and the thunder command */
    MAIN_ENV_SFX_THUNDER_A = 3, /* battle_map_draw_mesh_and_weather picks one of the two for a lightning flash */
    MAIN_ENV_SFX_THUNDER_B = 0xe,
} main_env_sfx_e;

/* Sector/size pairs for scenario and permanently loaded music SMD files. */
typedef struct main_sound_smd_file {
    s32 sector;
    s32 size;
} main_sound_smd_file_t;

/* SMD and WD resources store their total byte size at offset eight. */
typedef struct suzuki_resource_prefix {
    u8 _unknown_00[8];
    u32 size_bytes; /* 0x08 */
} suzuki_resource_prefix_t;

/* The transfer controller indexes these records in 16-byte steps. */
typedef struct suzuki_spu_transfer_status {
    s16 status; /* 0x00 */
    u8 _unknown_02[14];
} suzuki_spu_transfer_status_t;

/* Elapsed play time filled by main_smd_get_play_time. The root counter runs
 * at 240 Hz; music->ticks counts those ticks in 1/256 units. */
typedef struct suzuki_play_time {
    u32 sequence_ticks; /* 0x00; music->sequence_ticks */
    u16 fraction;       /* 0x04; 1/240 s */
    u16 seconds;        /* 0x06 */
    u16 minutes;        /* 0x08 */
} suzuki_play_time_t;

extern u8 g_main_sound_env_sed_data[];
extern u8 g_main_sound_system_sed_data[];

/* Driver tables (read-only data 0x80028b0c-0x8002a8d8). */
extern suzuki_smd_handler_t g_main_smd_opcode_handlers[0x80]; /* 0x80028b0c; opcodes 0x80-0xff */
extern u8 g_main_smd_opcode_lengths[0x80];                    /* 0x80028d0c; opcode plus parameters */
/* 0x80028d8c; ticks per note byte. Each of the twelve semitones has the same
 * 19 lengths: 0 (a length byte follows), then 192 (a whole note), 144, 96,
 * 72, 64, 48, 36, 32, 24, 18, 16, 12, 9, 8, 6, 4, 3 and 2. */
extern u8 g_main_smd_note_durations[228];
extern u8 g_main_smd_note_semitones[228];                          /* 0x80028e70; note byte / 19 */
extern suzuki_modulator_step_t g_main_smd_modulator_waveforms[16]; /* 0x80028f54 */
extern u8 g_main_smd_key_octaves[120];                             /* 0x80028fe8; key / 12 */
extern u8 g_main_smd_key_semitones[120];                           /* 0x80029060; key % 12 */

/* 0x800290d8: SPU pitch for semitone s and fine step f at index (s << 8) + f
 * (octave 6 reference); main_smd_calculate_pitch indexes it flat. */
extern u16 g_main_smd_pitch_table[12 * 256];

/* Driver globals (0x800329f0-0x80032a68, gp = 0x800329bc). The heap globals
 * are only ever reached $gp-relative, by 0x8001423c-0x8001442c. */
extern s16
    g_main_sound_sfx_channel_count; /* channels main_sound_start_sfx fills, one per stream of a sound: always 2 */
extern main_sound_resource_t* g_main_sound_resource_list; /* 0x80032a00 */
extern suzuki_spu_transfer_status_t* g_main_sound_spu_transfer_status_records;
extern u32 g_main_sound_music_key_off_voices;      /* voices queued for key-off by stopped music */
extern u32 g_main_mask_exclusion;                  /* 0x80032a0c; voices reserved by SFX */
extern u32 g_main_sound_sfx_restart_channels;      /* last SFX voice search: channel mask */
extern u32 g_main_sound_sfx_restart_voices;        /* last SFX voice search: voice mask */
extern s32 g_main_smd_random_state;                /* 0x80032a18 */
extern s16 g_main_sound_sfx_instrument;            /* instrument every started SFX channel begins with; always 0 */
extern u32 g_main_sound_sfx_key_off_voices;        /* SFX voices pending key-off */
extern u32 g_main_sound_tick_count;                /* root-counter tick count; odd ticks step the ramps */
extern u16 g_main_sound_spu_transfer_status_index; /* index into g_main_sound_spu_transfer_status_records */
extern CdlATV g_main_sound_cd_mix;                 /* 0x80032a3c; written by Put Sound Type */

extern s16 g_main_sound_decoded_data_result;        /* SPU transfer controller state */
extern suzuki_waveset_t* g_main_sound_waveset_list; /* 0x80032a44 */
extern s32 g_main_sound_spu_event;                  /* 0x80032a48; OpenEvent handle of main_sound_spu_event_handler */
extern u16 g_main_sound_voice_update_requests;      /* suzuki_voice_mask_request_e */
extern suzuki_music_t* g_main_sound_active_music_list; /* 0x80032a50 */

/* Driver status (main_sound_driver_flag_e). */
extern s16 g_main_sound_driver_flags;
extern s32 g_main_root_counter_2_event;        /* 0x80032a5c; OpenEvent handle of main_sound_root_counter_2_handler */
extern suzuki_music_t* g_main_sound_sfx_music; /* 0x80032a60; eight channels on SPU voices 16-23 */
extern SpuReverbAttr g_main_sound_reverb_attr; /* 0x80037008 */
extern suzuki_spu_state_t g_main_sound_spu_state;              /* 0x80037020 */
extern psyq_spu_heap_record_t g_main_sound_spu_malloc_table[]; /* 0x800408e0; SpuInitMalloc(6, ...) records */
extern main_sound_music_t g_main_sound_music;
extern main_sound_smd_file_t g_main_sound_scenario_smd_files[];
extern main_sound_smd_file_t g_main_sound_permanent_smd_files[5];
extern s32 g_main_sound_weather_sfx_id; /* the background sound main_sound_play_bg_sfx started, 0 for none */
void main_sound_set_sfx_echo(int sound_id, int volume);
void main_sound_stop_weather_sfx_music(void);
void main_sound_pause_tracked_sfx(void);
s32 main_sound_get_current_scenario_music(void);
suzuki_music_t* main_sound_get_music_handle(s32 slot);

/* Verified SCUS scenario-music and tune wrappers. Include this header in
 * their definitions as well as callers so ABI declarations cannot drift. */
s32 main_sound_open_and_play_music(s32 music_id, s32 slot);
void main_sound_open_generic_sfx(void);
s32 main_sound_open_music_into_free_slot(s32 music_id);
s32 main_sound_open_music_into_slot(s32 music_id, s32 slot);
s32 main_sound_open_scenario_smd_files(s32 music_id);
void main_sound_play_sfx(s32 sound_id);
void main_sound_play_bg_sfx(s32 sound_id);
void main_sound_play_sfx_find_channel(s32 sound_id);
s32 main_sound_play_tune(s32 tune);
s32 main_sound_poll_scenario_smd_load(void);
s32 main_sound_set_current_music_balance_shift_timed(s32 value, s32 time);
s32 main_sound_set_current_music_pitch_shift_timed(s32 value, s32 time);
s32 main_sound_set_current_music_target(s32 volume, s32 time);
s32 main_sound_set_current_music_tempo_scale_timed(s32 value, s32 time);
void main_sound_set_current_music_track(s32 music_id);
s32 main_sound_set_current_music_volume(s32 volume, s32 time);
s32 main_sound_set_current_music_volume_timed(s32 time);
s32 main_sound_set_tune_volume(s32 time);
s32 main_sound_stop_current_music(void);

/* battle_map_step_init_sequence calls this without loading $a0, but the body reads sound_id. */
void main_sound_stop_sfx(int sound_id);
suzuki_music_t* main_sound_switch_music_track(s32 slot, s32 volume, s32 fade);
void main_sound_unload_current_scenario_music(void);
s32 main_sound_unload_scenario_mus(s32 slot);
void main_sound_unload_scenario_music_and_tunes(void);
s32 main_sound_update_tunes(void);
s32 main_sound_wait_for_scenario_smd_load(s32 music_id);

/* Suzuki driver entry points. The music handle passed by the game wrappers
 * is the suzuki_music_t* that SuzukiPutPlaySMD returns. */
void SuzukiAppendVFXSMD(main_sound_resource_t* resource);
void main_sound_remove_vfx_resource(main_sound_resource_t* resource);
void SuzukiCalcMusVolChange(suzuki_music_t* music, s16 volume, s16 time);
void SuzukiDeallocateMUSChannels(suzuki_music_t* music);
u32 SuzukiGetActiveChannels(suzuki_music_t* music);
u32 SuzukiGetMusicPlaying(suzuki_music_t* music);
suzuki_music_t* SuzukiPutPlaySMD(suzuki_smd_header_t* smd);
void SuzukiPlaySound1(int sound_id);
void SuzukiPlaySound2(int sound_id);
void SuzukiPlaySoundFindChannel(s32 sound_id);
void SuzukiSetSfxEcho(int sound_id, s16 volume);
void SuzukiSPUInitialiser(s32 flags);
void SuzukiTurnOffAllMusic(void);
void SuzukiUnloadMUS(suzuki_music_t* music);

/* MUS lifecycle, playback control and queue (0x800120f4-0x80014200). */
void main_smd_reset_music(suzuki_music_t* music, s16 volume, s16 fade);
void main_smd_resume_music(suzuki_music_t* music, s16 volume, s16 fade);
void main_smd_pause_music(suzuki_music_t* music);
void main_smd_stop_marked_music(void);
void main_smd_init_channel_streams(suzuki_music_t* music, suzuki_smd_header_t* smd);
void main_smd_set_tempo_scale(suzuki_music_t* music, s16 value, s16 time);
void main_smd_set_pitch_shift(suzuki_music_t* music, s16 value, s16 time);
void main_smd_set_balance_shift(suzuki_music_t* music, s16 value, s16 time);
void main_smd_set_channel_mute_mask(suzuki_music_t* music, u32 mask);
void main_smd_set_channel_select(suzuki_music_t* music, u8 value);
void main_smd_get_play_time(suzuki_music_t* music, suzuki_play_time_t* time);
u16* main_smd_get_min_loop_count(suzuki_music_t* music);
char* main_smd_get_music_filename(suzuki_music_t* music);
void main_smd_dispatch_snapshot(suzuki_music_t* music, s32 mode);
void main_smd_discard_snapshot(suzuki_music_t* music);
void main_smd_save_snapshot(suzuki_music_t* music);
void main_smd_restore_snapshot(suzuki_music_t* music);
void main_smd_set_stop_bar(suzuki_music_t* music, u16 value);
u32 main_smd_retrigger_held_voices(suzuki_music_t* music);
u32 main_smd_get_held_voices(suzuki_music_t* music);
void main_smd_transfer_music_data(suzuki_music_t* music);
void main_smd_init_music_header(suzuki_music_t* music);
void main_smd_init_channels(suzuki_music_t* music);
void main_smd_free_snapshots(suzuki_music_t* music);
void main_smd_copy_snapshot(suzuki_music_t* music, suzuki_music_t* snapshot);
void main_smd_insert_music(suzuki_music_t* music);
s32 main_smd_remove_music(suzuki_music_t* music);
void main_smd_get_instrument_attr(SpuVoiceAttr* attr, s16 index);
void main_smd_set_flags_all_channels(u16 mask, suzuki_music_t* music);

/* SFX (0x800124cc-0x80013f74). The SFX music record has eight channels on
 * SPU voices 16-23; the low byte of a start_sfx channel id is the first
 * channel and its high byte the steal priority
 * (MAIN_SOUND_SFX_CHANNEL_ID). */
void main_sound_set_sfx_enabled(s32 enabled);
void main_sound_play_sfx_in_channel(s32 sound_id, s32 channel);
void main_sound_play_sfx_with_settings_find_channel(s32 sound_id, s32 volume, s32 balance);
void main_sound_play_sfx_with_settings_in_channel(s32 sound_id, s32 channel, s32 volume, s32 balance);
void main_sound_play_2_sfx(s32 first, s32 second);
void main_sound_play_4_sfx(s32 first, s32 second, s32 third, s32 fourth);
void main_sound_stop_resource_sfx(main_sound_resource_t* resource);
void main_sound_stop_sfx_channels(s32 sound_id);
void main_sound_stop_sfx_channel_pair(s32 index);
void main_sound_release_sfx_voices(suzuki_music_t* music, u32 channel_mask, u32 voice_mask);
void main_sound_set_sfx_balance(s32 sound_id, s32 balance);
u32 main_sound_get_sfx_channels(s32 sound_id);
s32 main_sound_find_sfx_channel(s32 sound_id, s32 voice_count);
suzuki_music_t* main_sound_init_sfx_music(void);
void main_smd_init_sfx_music_header(suzuki_music_t* music);
void main_sound_start_sfx(s16 channel_id, s32 sound_id, s16 volume, s16 balance);
void main_sound_stop_all(void);

/* Suzuki heap (0x8001423c-0x80014544) and SPU RAM wrappers. */
void main_sound_copy_memory(void* destination, const void* source, s32 size);
s32 main_sound_alloc_spu_ram(s32 size);
s32 main_sound_free_spu_ram(u32 addr);

/* Root-counter tick (0x80014590-0x80015324, 0x80017118-0x800178e4). The
 * per-channel passes take (music, first channel, channel count). */
s32 main_sound_root_counter_2_handler(void);
void main_sound_update_voice_attrs(void);
void main_sound_flush_key_on(void);
void main_sound_flush_key_off(void);
void main_smd_step_ramp(suzuki_ramp_t* ramp);
void main_sound_calculate_pitch_lfo_voices(void);
void main_sound_calculate_noise_voices(void);
void main_sound_calculate_reverb_voices(void);
void main_smd_update_channel_ramps(suzuki_music_t* music, suzuki_music_channel_t* channels, s16 channel_count);
void main_smd_read_instructions(suzuki_music_t* music, suzuki_music_channel_t* channels, s16 channel_count);
void main_smd_update_modulators(suzuki_music_t* music, suzuki_music_channel_t* channels, s16 channel_count);
void main_smd_update_voices(suzuki_music_t* music, suzuki_music_channel_t* channels, s16 channel_count);
s16 main_smd_calculate_pitch(s16 key);
void main_smd_modulator_reset(suzuki_modulator_t* modulator);
void main_smd_modulator_deactivate(suzuki_modulator_t* modulator);
s32 main_smd_modulator_calculate_step(s32 depth, s16 speed, s16 waveform);
s32 main_smd_modulator_step_square(suzuki_modulator_t* modulator);
s32 main_smd_modulator_step_alternating(suzuki_modulator_t* modulator);
s32 main_smd_modulator_step_triangle(suzuki_modulator_t* modulator);
s32 main_smd_modulator_step_triangle_centered(suzuki_modulator_t* modulator);
s32 main_smd_modulator_step_sawtooth(suzuki_modulator_t* modulator);
s32 main_smd_modulator_step_random(suzuki_modulator_t* modulator);
s32 main_smd_modulator_step_random_bipolar(suzuki_modulator_t* modulator);
void main_smd_seed_random(s32 value);
s32 main_smd_random(void);

/* Reprograms an SMD channel with an instrument of its waveset. */
void main_smd_set_instrument(s16 instrument, suzuki_music_channel_t* channel);
void main_smd_set_voice_update_flags_all_channels(u16 mask, suzuki_music_t* music);
void main_smd_set_voice_attr_flags_all_channels(suzuki_music_t* music, u16 mask);

/* Driver set-up, wavesets, volume and SPU transfers (0x80017920-0x800186c4). */
void main_sound_quit(void);
suzuki_waveset_t* main_sound_load_waveset(suzuki_waveset_t* file);
void main_sound_free_wavesets(void);
void main_sound_free_waveset(suzuki_waveset_t* waveset);
suzuki_waveset_t* main_sound_find_waveset(s16 id);
void main_sound_set_type(s32 type);
s32 main_sound_get_type(void);
void main_sound_put_type(s16 level);
void main_sound_set_reverb_mode(s32 mode, s16 depth, s32 delay, s32 feedback);
void main_sound_set_master_volume(s16 volume, s16 time);
void main_sound_set_cd_volume(s16 volume, s16 time);
void main_sound_set_cd_reverb(s32 reverb, s32 mix);
void main_sound_commit_volume_change(void);
void main_sound_set_vol_balance(s32 volume, SpuVolume* volume_out, u8 mode);
void main_sound_transfer_spu_data(u32 spu_address, void* data, u32 size, s32 mode);
void main_suzuki_spu_callback_func(void);
s32 main_sound_spu_event_handler(void);
s32 main_sound_sum_resource_words(suzuki_resource_prefix_t* resource);
s16 main_sound_wait_for_spu_transfer(s32 flags);
void main_sound_replay_bg_sfx(void);

/* Suzuki key-on/key-off flushes. The driver compiled them both as functions
 * and inline in its root-counter handler, so their bodies live here once. */
/* Keys on the voices queued in each playing MUS record's key_on_mask; voices
 * reserved or pending release by SFX (g_main_mask_exclusion, g_main_sound_sfx_key_off_voices) only
 * for the SFX record (status bit 1). main_sound_flush_key_on (0x80014818) is
 * this body alone; the root-counter handler (0x800149dc) contains it inline. */
static inline void main_sound_flush_key_on_inline(void) {
    suzuki_music_t* music;
    s16 status;
    u32 allowed;
    u32 voices;

    voices = 0;
    allowed = ~(g_main_mask_exclusion | g_main_sound_sfx_key_off_voices);
    music = g_main_sound_active_music_list;
    while (music != 0) {
        status = music->status;
        if (status < 0) {
            if ((status & SUZUKI_MUSIC_STATUS_SFX) != 0)
                voices |= music->key_on_mask;
            else
                voices |= allowed & music->key_on_mask;
            music->key_on_mask = 0;
        }
        music = music->next;
    }

    if (voices != 0)
        SpuSetKey(SPU_ON, voices);
}

/* Keys off the voices queued in each playing MUS record's key_off_mask (same
 * SFX filter as the key-on flush) plus the voices of stopped music
 * (g_main_sound_music_key_off_voices) and the SFX voices pending release (g_main_sound_sfx_key_off_voices), which first
 * get a fast release (rate 6, mode 3). main_sound_flush_key_off (0x800148b4)
 * is this body alone; the root-counter handler contains it inline. */
static inline void main_sound_flush_key_off_inline(void) {
    suzuki_music_t* music;
    s16 status;
    u32 allowed;
    u32 voices;
    u32 pending;
    s32 voice;

    allowed = ~(g_main_mask_exclusion | g_main_sound_sfx_key_off_voices);
    voices = (allowed & g_main_sound_music_key_off_voices) | g_main_sound_sfx_key_off_voices;
    music = g_main_sound_active_music_list;
    while (music != 0) {
        status = music->status;
        if (status < 0) {
            if ((status & SUZUKI_MUSIC_STATUS_SFX) != 0)
                voices |= music->key_off_mask;
            else
                voices |= allowed & music->key_off_mask;
            music->key_off_mask = 0;
        }
        music = music->next;
    }

    pending = (allowed & g_main_sound_music_key_off_voices) | g_main_sound_sfx_key_off_voices;
    if (pending != 0) {
        for (voice = 23; voice >= 0; voice--) {
            if ((pending & (1 << voice)) != 0)
                SpuSetVoiceRRAttr(voice, 6, 3);
        }
        g_main_sound_sfx_key_off_voices = 0;
        g_main_sound_music_key_off_voices = 0;
    }

    if (voices != 0)
        SpuSetKey(SPU_OFF, voices);
}

/* status */
/* main_status_change_unit, 0x8005e6cc: OR, AND-not, or replacement of
 * inflicted_status, followed by merging status_sets.innate into status_sets.current. */
typedef enum main_status_change {
    MAIN_STATUS_ADD = 0,
    MAIN_STATUS_REMOVE = 1,
    MAIN_STATUS_REPLACE = 2
} main_status_change_e;

extern status_infliction_data_t g_main_status_infliction_data[];
extern const status_effect_data_t g_main_status_effect_data[BATTLE_STATUS_COUNT];

/* Byte-stride alias of g_main_status_effect_data[0].flags_1. The target uses
 * this address directly while walking the 0x10-byte records. */
extern u8 g_main_status_check_sets[MAIN_STATUS_CHECK_SET_COUNT][BATTLE_STATUS_BYTE_COUNT];
void main_status_init_check_data(void);
void main_status_change_unit(battle_stats_t* unit, s32 status_set, u8 status_flag, s32 change_type);
void main_status_init_ct(battle_stats_t* unit);
void main_status_init_unit(battle_stats_t* unit);
void main_status_set_action_state(battle_stats_t* unit, u8 action_state);
s32 main_status_set_ct(battle_stats_t* unit, s32 status_id, s32 removing);
void main_status_store_current(battle_stats_t* unit);
void main_status_update_unit_flags_and_ct(battle_stats_t* unit);
s32 main_status_find_action_highest_order_effect(const battle_action_data_t* action);

/* system */
/* Bits of g_main_system_flags, the control word of the WLDCORE world-map
 * loop: frame state, what the frame leaves out, requests left by WORLD and
 * OPENING, and the reason the loop ends. */
typedef enum main_system_flags {
    /* wldcore_entrypoint runs frames while set; wldcore_init_world_map_state sets it, a level clears it to leave. */
    MAIN_SYSTEM_FLAG_RUNNING = 0x00000001,
    /* Draw this frame with the 3D map; wldcore_gfx_draw_world_frame_with_map clears it after drawing. */
    MAIN_SYSTEM_FLAG_DRAW_MAP = 0x00000002,
    /* A level waits for a thread or a fade: wldcore_reset_game_if_special_keycode_is_pressed publishes no pad input. */
    MAIN_SYSTEM_FLAG_INPUT_BLOCKED = 0x00000004,
    /* Set by wldcore_fade_start_screen, cleared by wldcore_fade_step_screen one step before the fade ends. */
    MAIN_SYSTEM_FLAG_FADING = 0x00000008,
    /* wldcore_map_project_and_cull_dots hides every map dot. */
    MAIN_SYSTEM_FLAG_HIDE_MAP_DOTS = 0x00000010,
    /* wldcore_map_draw_visible_routes draws no route. */
    MAIN_SYSTEM_FLAG_HIDE_MAP_ROUTES = 0x00000020,
    /* wldcore_entrypoint masks the display for the frame. */
    MAIN_SYSTEM_FLAG_DISPLAY_OFF = 0x00000040,
    /* wldcore_menu_run_world_frame returns without running the menu frame; no code sets it. */
    MAIN_SYSTEM_FLAG_SKIP_MENU_FRAME = 0x00000080,
    /* Set by world_formation_update_world_map_root_request: the return level opens the Brave Story screen. */
    MAIN_SYSTEM_FLAG_REQUEST_BRAVE_STORY = 0x00000100,
    /* wldcore_gfx_draw_world_frame does not load the scratch image into the display buffer. */
    MAIN_SYSTEM_FLAG_NO_BACKGROUND_IMAGE = 0x00000200,
    /* wldcore_dispatch_display_object_list draws nothing and wldcore_map_pulse_dot_colors returns. */
    MAIN_SYSTEM_FLAG_HIDE_DISPLAY_OBJECTS = 0x00000400,
    /* wldcore_gfx_draw_context_value_display draws nothing. */
    MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE = 0x00000800,
    /* wldcore_gfx_draw_calendar_date draws nothing. */
    MAIN_SYSTEM_FLAG_HIDE_CALENDAR_DATE = 0x00001000,
    /* wldcore_proposition_submit_marker_request draws no marker; set while a level moves or covers the map view. */
    MAIN_SYSTEM_FLAG_HIDE_LOCATION_MARKER = 0x00002000,
    /* WORLD's menu chose a map (script variable CURRENT_MAP): the return level restarts the core and travels there. */
    MAIN_SYSTEM_FLAG_REQUEST_DESTINATION = 0x00004000,
    /* wldcore_map_step_route_travel rolled an encounter: wldcore_entrypoint returns 1. */
    MAIN_SYSTEM_FLAG_RANDOM_BATTLE = 0x00008000,
    /* Set after the card menu returns 1: wldcore_entrypoint restarts its loop instead of returning. */
    MAIN_SYSTEM_FLAG_RESTART = 0x00010000,
    /* The unit-panel threads (8, 9 and 12) run: the menu frame draws their windows, not the formation menu. */
    MAIN_SYSTEM_FLAG_UNIT_PANEL_THREADS = 0x00020000,
    /* A story event replays: wldcore_entrypoint returns 2 and the next entry reopens the story-event list. */
    MAIN_SYSTEM_FLAG_STORY_EVENT_REPLAY = 0x00040000,
    /* Set by open_title_update_menu: the world entry opens the tutorial menu, which leaves through RETURN_TO_TITLE. */
    MAIN_SYSTEM_FLAG_TITLE_TUTORIAL = 0x00080000,
    /* Set by world_formation_update_tutorial_categories_request: the return level opens the tutorial menu. */
    MAIN_SYSTEM_FLAG_REQUEST_TUTORIAL = 0x00100000,
    /* A tutorial event plays: wldcore_entrypoint returns 2 with the music kept; the next entry reopens its list. */
    MAIN_SYSTEM_FLAG_TUTORIAL_EVENT = 0x00200000,
    /* wldcore_init_world_map_state sets the rumor, path, location and story-event variables; no code sets it. */
    MAIN_SYSTEM_FLAG_UNLOCK_ALL = 0x00400000,
    /* Set by world_formation_update_debug_variable_list_request: the return level opens the variable list. */
    MAIN_SYSTEM_FLAG_REQUEST_VARIABLE_LIST = 0x00800000,
    /* Set by world_formation_update_fixed_number_panel_request: the return level opens the fixed number panel. */
    MAIN_SYSTEM_FLAG_REQUEST_FIXED_NUMBER_PANEL = 0x01000000,
    /* The location menu chose a deep-dungeon battle: wldcore_entrypoint returns 4. */
    MAIN_SYSTEM_FLAG_DEEP_DUNGEON_BATTLE = 0x02000000,
    /* Set by wldcore_clear_screen_and_set_map_clip_rect; the return level then recaptures the world frame. */
    MAIN_SYSTEM_FLAG_SCREEN_CLEARED = 0x04000000,
    /* Map dots stop pulsing and wldcore_gfx_draw_animated_display_object stops advancing frames. */
    MAIN_SYSTEM_FLAG_FREEZE_ANIMATION = 0x08000000,
    /* wldcore_entrypoint returns 5: main_system_run_game_loop goes back to OPENING. */
    MAIN_SYSTEM_FLAG_RETURN_TO_TITLE = 0x10000000,
    /* The world entry opens the Brave Story screen, which leaves through RETURN_TO_TITLE; no code sets it. */
    MAIN_SYSTEM_FLAG_TITLE_BRAVE_STORY = 0x20000000,
} main_system_flags_e;

/* 64 allocation tags, one per 0x800-byte block in the game arena. */
extern u8 g_main_heap_game_allocator_table[64];
extern u8 g_main_heap_smd_allocator_table[16];
extern u8 g_main_heap_smd_base[];
extern game_options_t g_main_game_options;
extern game_options_t g_main_game_options_defaults;

/* g_main_system_game_flow_state: what follows when the BATTLE.BIN loop
 * closes a battle or event. */
typedef enum main_game_flow_state {
    MAIN_GAME_FLOW_BATTLE = 0, /* stay in battle_state_run_game_loop for the next event */
    MAIN_GAME_FLOW_WORLD = 1,  /* back to the world map */
    MAIN_GAME_FLOW_RESET = 2,  /* stop display and audio and restart from the opening */
    MAIN_GAME_FLOW_ENDING = 3, /* run the ending, then restart */
} main_game_flow_state_e;

/* Error codes passed to main_system_handle_error, whose report
 * hook is empty in retail. */
typedef enum main_system_error {
    MAIN_SYSTEM_ERROR_HEAP_ALLOC = 1,              /* main_file_load_to_heap got no block */
    MAIN_SYSTEM_ERROR_FILE_LOAD = 2,               /* a checked file load returned 0 */
    MAIN_SYSTEM_ERROR_NO_MISC_UNIT_SLOT = 6,       /* battle_unit_claim_misc_slot found none free */
    MAIN_SYSTEM_ERROR_SHP_FRAME = 8,               /* frame index above 0x200 or frame pointer -1 */
    MAIN_SYSTEM_ERROR_SEQ_ANIMATION = 9,           /* animation index above 0x100 or sequence pointer -1 */
    MAIN_SYSTEM_ERROR_FRAME_PART_COUNT = 10,       /* more than eight parts in a unit frame */
    MAIN_SYSTEM_ERROR_SEQ_OPCODE = 0xd,            /* unknown sequence opcode, or a queued animation for slot 0 */
    MAIN_SYSTEM_ERROR_SMD_LOAD = 0x11,             /* no SMD buffer or the SMD load could not start */
    MAIN_SYSTEM_ERROR_DISTORTION_ANIMATION = 0x17, /* distortion animation id without a handler */
} main_system_error_e;

extern s32 g_main_system_frontend_world_result;
extern u32 g_main_system_game_flow_state; /* main_game_flow_state_e */
extern u32 g_main_system_game_loop_stack_pointer;
extern u8 g_main_system_go_straight_to_battle;
extern int g_main_system_graphics_error;
extern s32 g_main_system_play_time_frames;
extern s32 g_main_system_play_time_hours;
extern s32 g_main_system_play_time_minutes;
extern s32 g_main_system_play_time_seconds;
/* main_system_flags_e bits; cleared by main_save_init_data_tables. */
extern u32 g_main_system_flags;
extern s32 g_main_system_session_frames;

/* The packed-word view of the options record; g_main_game_options names the
 * same address under its union type. The scalar name is retained because GCC
 * otherwise coalesces the saved-word load with the following bitfield writes
 * and changes the target instruction order. */

extern s32 g_main_boot_startup2_has_run;
extern u32 g_main_boot_squaresoft_logo_image_words[];
extern u8* g_main_heap_high_overlay_load_address;
extern u8* g_main_heap_low_overlay_load_address;
extern u8* g_main_heap_world_overlay_load_address;
/* 0x800449ec: empty (nop, jr ra). Every main loop and every blocking wait calls it once per frame. */
void main_system_frame_stub(void);

/* 0x800449f8: a return-only hook. Main and overlay callers supply two words,
 * but neither their original meaning nor the hook's original name is proven. */
void main_system_report_stub(s32 category, s32 code);
void main_overlay_call_battle_entrypoint(void);
void main_overlay_exec_battle_bin(void);
int main_overlay_exec_open_bin_main_loop(int mode);
void main_overlay_exec_open_bin_ending(void);
int main_overlay_open_world_and_wldcore(int load_world);
void main_overlay_open_world_bin(s32 mode);
void main_restore_game_loop_stack_pointer(u32* source);
void main_system_handle_error(int error_code);
void main_system_handle_battle_load_exception(s32 value);
void main_system_handle_malloc_exception(s32 allocator_id, s32 error_code);
void main_system_handle_pointer_exception(int error_code);
void main_system_reset_game(void);
void main_system_run_game_loop(void);
void main_system_stop_display_and_audio_for_game_reset(void);
void main_system_store_stack_pointer(u32* destination);
s32 main_util_roll_pass_fail(s32 maximum, s32 threshold);
void main_boot_run_startup(void);
void main_boot_reset_game_state(void);
void main_system_handle_draw_sync_callback(void);
void main_system_handle_vsync_callback(void);
void main_boot_build_and_draw_sceap_logo(void);
void main_boot_build_and_draw_squaresoft_logo(void);
int main_boot_draw_sceap_logo(u32* otag, u32* image);
int main_boot_draw_squaresoft_logo(u32* otag, u32* image);
void main_boot_fade_out_squaresoft_logo(void);
void main_util_set_svector(SVECTOR* vector, s32 x, s32 y, s32 z);
void main_util_set_vector(VECTOR* vector, s32 x, s32 y, s32 z);
void* main_heap_alloc(u32 size);
void* main_heap_alloc_smd(u32 size);
void main_heap_call_free(void* allocation);
void main_heap_clear_game_allocator_table(void);
s32 main_heap_free(void* allocation);
u8* main_heap_reserve_at(u8* allocation, u32 size);
s32 main_heap_free_smd(void* allocation);
void main_util_clear_byte_data(void* destination, s32 count);

/* SCUS byte loops at 0x8005e254 (copy) and 0x8005e644 (clear), not SDK
 * memcpy/memset. Copy takes source first;
 * both routines do nothing for a nonpositive signed byte count. */
void main_util_copy_byte_data(const void* source, void* destination, s32 count);
void* game_malloc(s32 byte_length);
void main_util_copy_action_data(const battle_action_command_t* source, battle_action_command_t* destination);
void main_heap_clear_smd_allocator_table(void);
void main_sound_init_stub(void);
void main_system_enable_root_counter_2_evcb(void);
void main_system_disable_root_counter_2_evcb(void);
void main_util_copy_bytes(const void* source, void* destination, int count);

/* unit */
/* Selects rows in the base-data (0x8005e90c) and variance (0x8005e93c)
 * tables; these are not job IDs or gender bit flags. */
typedef enum main_unit_generation_type {
    MAIN_UNIT_TYPE_MALE = 0,
    MAIN_UNIT_TYPE_FEMALE = 1,
    MAIN_UNIT_TYPE_RAMZA = 2,
    MAIN_UNIT_TYPE_MONSTER = 3,
    MAIN_UNIT_TYPE_COUNT = 4
} main_unit_generation_type_e;

/* One row of the generation tables at 0x8005e90c and 0x8005e93c. The
 * records are selected by main_unit_generation_type_e, not by job id. */
typedef struct main_unit_generation_base_data {
    u8 raw_stat_seeds[UNIT_RAW_STAT_COUNT];
    u8 equipment[UNIT_EQUIPMENT_SLOT_COUNT];
} main_unit_generation_base_data_t;
typedef char main_unit_generation_base_data_size_must_be_12[(sizeof(main_unit_generation_base_data_t) == 12) ? 1 : -1];

typedef struct main_unit_generation_raw_stat_variance {
    u8 raw_stats[UNIT_RAW_STAT_COUNT];
} main_unit_generation_raw_stat_variance_t;
typedef char
    main_unit_generation_raw_stat_variance_size_must_be_5[(sizeof(main_unit_generation_raw_stat_variance_t) == 5) ? 1
                                                                                                                  : -1];

/* Values accepted by main_status_set_action_state. */
typedef enum main_unit_action_state {
    MAIN_UNIT_ACTION_STATE_NONE = 0,
    MAIN_UNIT_ACTION_STATE_CHARGING = 5,
    MAIN_UNIT_ACTION_STATE_JUMPING = 6,
    MAIN_UNIT_ACTION_STATE_DEFENDING = 7,
    MAIN_UNIT_ACTION_STATE_PERFORMING = 8,
    MAIN_UNIT_ACTION_STATE_KEEP_PERFORMING = 0xff
} main_unit_action_state_e;

extern u8 g_main_party_highest_level;
extern s32 g_main_entd_current_event_id;
extern entd_file_t* g_main_entd_set;
extern party_data_t g_main_party_data[PARTY_ROSTER_SLOT_COUNT];
extern main_unit_generation_base_data_t g_main_unit_generation_base_data[MAIN_UNIT_TYPE_COUNT];
extern main_unit_generation_raw_stat_variance_t g_main_unit_generation_raw_stat_variance[MAIN_UNIT_TYPE_COUNT];
extern u32 g_main_unit_name_uses_world_text;
void main_unit_update_formation_stats(battle_stats_t* unit);

/* Current encounter, NULL while loading, or (entd_encounter_t *)-1 when
 * no encounter is selected. */
entd_encounter_t* main_entd_get_encounter(void);
int main_entd_open_file(void);
void main_entd_init_event_unit_data(int event_id);
void main_party_clear_all(void);
s32 main_party_create_monster_egg(s32 monster_job, s32 egg_modifier, s32 egg_color);
s32 main_party_find_free_slot(s32 save_formation, u8* palette);
s32 main_party_find_slot_by_sprite_set(s32 sprite_set);

/* Out-of-battle generation normalizes every value except male, female and
 * monster to Ramza. The raw-stat generator indexes the tables directly. */
s32 main_party_generate_unit(s32 unit_type);
party_data_t* main_party_get_data_pointer(s32 party_index);
void main_party_init_new_game(s32 mode);
u32 main_party_level_unit_to_target(party_data_t* party_unit, s32 level_delta);
void main_party_remove_unit(u32 index);
s32 main_party_save_unit(battle_stats_t* unit, s32 allow_guest);
void main_unit_apply_equipment_move_jump_and_name(battle_stats_t* unit);
void main_unit_apply_level_growth(battle_stats_t* unit, s32 level_down);
void main_unit_calculate_abilities(battle_stats_t* unit, entd_unit_t* entd);
void main_unit_calculate_actual_stats(battle_stats_t* unit, s32 mode);
s32 main_unit_calculate_entd_data(battle_stats_t* unit, entd_unit_t* entd);
void main_unit_calculate_entd_equipment(battle_stats_t* unit, entd_unit_t* entd);
void main_unit_calculate_jobs_and_skillsets_from_entd(battle_stats_t* unit, const entd_unit_t* entd);
void main_unit_calculate_move_jump(battle_stats_t* unit, s32 keep_position);
u8 main_unit_calculate_palette_spritesheet(battle_stats_t* unit, u8* palette);

s32 main_unit_calculate_random_equipment(
    battle_stats_t* unit, u8 item_type_flag, u8 weapon_flag_mask, u8 required_item_type);

u16 main_unit_calculate_rsm(battle_stats_t* unit, u16 ability_id, s32 flags, entd_unit_t* entd);
s32 main_unit_calculate_zodiac_symbol(u16 birthday);
s32 main_unit_check_level_up(battle_stats_t* unit);
void main_unit_copy_job_data(battle_stats_t* unit);
void main_unit_copy_job_growths_and_multipliers(battle_stats_t* unit);
void main_unit_copy_last_ability_ct(battle_stats_t* unit);
void main_unit_enable_rsm_flags(battle_stats_t* unit);
void main_unit_generate_base_raw_stats(unit_raw_stats_t* raw_stats, s32 unit_type);
void main_unit_generate_out_of_battle(party_data_t* party, s32 unit_type);
void main_unit_generate_party_base_raw_stats(party_data_t* party, s32 unit_type);
void main_unit_generate_raw_stats(battle_stats_t* unit);
u8 main_unit_get_spritesheet_palette(battle_stats_t* unit, u8* palette);
s32 main_unit_has_status_in_set(const battle_stats_t* unit, main_status_check_set_e status_set);
s32 main_unit_init(entd_encounter_t* entd, s32 unit_id, s32 guest_id, s32 initialize_for_battle);
void main_unit_init_battle_data(battle_stats_t* unit, const party_data_t* party_unit);
s32 main_unit_init_for_battle(battle_stats_t* unit, entd_unit_t* entd, u32 use_world_text, s32 kind);
s32 main_unit_init_job_data(battle_stats_t* unit, s32 slot, s32 use_world_text);
s32 main_unit_init_job_data_from_entd(battle_stats_t* unit, entd_unit_t* entd);
void main_unit_init_job_levels(unit_job_point_table_t* unit_job_jp, u8* unit_job_levels);
void main_unit_init_status_and_rewards(battle_stats_t* unit, s32 clear_rewards);
void main_unit_learn_job_abilities(battle_stats_t* unit, s32 job_id, const entd_unit_t* entd);
void main_unit_refresh_stats_and_statuses(battle_stats_t* unit);
void main_unit_reset_battle_state(battle_stats_t* unit);
void main_unit_set_equipment_attributes(battle_stats_t* unit, s32 level_up_check);

/* SCUS_942.21 unit equipment/name pipeline, 0x8005ab00 and
 * 0x8005c398..0x8005cbd0. Signatures are shared with the matching definitions. */
void main_unit_set_equipment_stats(battle_stats_t* unit);
void main_unit_set_equippable_items(battle_stats_t* unit);
void main_unit_set_rsm_flag(battle_stats_t* unit, u32 ability_id);
void main_unit_store_character_names(battle_stats_t* unit);
void main_unit_store_monster_equipment(battle_stats_t* unit, const entd_unit_t* entd);
void main_unit_store_ramza_name_birthday_zodiac(battle_stats_t* unit);
void main_unit_update_stats_statuses_and_equipment(battle_stats_t* unit, s32 skip_status_check, s32 initializing);
u8 main_party_calculate_highest_level(void);
void main_unit_increase_casualty_counters(battle_stats_t* unit);
void main_unit_init_position_and_rewards(battle_stats_t* unit, entd_unit_t* entd);

/* zodiac */
/* ZODIAC.BIN sprite-table records shared by the builder and polygon helper. */
typedef struct main_zodiac_sprite_frame {
    u8 u;
    u8 _unused_01;
    u8 v;
    u8 _padding_03; /* aligns width */
    s16 width;
    s16 height;
    s16 offset_x;
    s16 offset_y;
} main_zodiac_sprite_frame_t;

typedef struct main_zodiac_draw_offset {
    u8 _unused_00[8];
    u16 x;
    u16 y;
} main_zodiac_draw_offset_t;

/* Draw state handed to SCUS Build ZODIAC.BIN (provisional layout). */
typedef struct zodiac_draw_context {
    u32* ot;             /* 0x00: ordering table entry the primitives link into */
    s32 brightness;      /* 0x04: low byte is r0/g0/b0 of every sprite; WORLD stores g_world_menu_color_red */
    s32 link_primitives; /* 0x08: nonzero: link each sprite into the OT */
    s16 scale_x;         /* 0x0c: 12.12 */
    s16 scale_y;         /* 0x0e: 12.12 */
    u8 _unused_10[8];
    main_zodiac_draw_offset_t offset; /* 0x18 */
} zodiac_draw_context_t;

extern const u16 g_main_zodiac_day_limits[ZODIAC_SIGN_ORDINARY_COUNT];
extern POLY_FT4 g_main_zodiac_primitives[2][30];

/* gfx */

/* EVENT/FRAME.BIN as loaded: 0x92c0 bytes of file in a sector-rounded
 * 0x9800-byte buffer. Its loaders upload the three regions after the first
 * 0x1000 bytes; the arrays are words for LoadImage. */
typedef struct main_gfx_frame_bin {
    u8 _unused_0000[0x1000];            /* 0x0000; not uploaded */
    u32 pixels[0x8000 / sizeof(u32)];   /* 0x1000; 256x256 at 4 bits per pixel */
    u32 cluts[0x200 / sizeof(u32)];     /* 0x9000; 16 CLUTs */
    u32 cluts_tail[0xc0 / sizeof(u32)]; /* 0x9200; 6 CLUTs */
    u8 _padding_92c0[0x540];            /* 0x92c0; past the end of the file, to the sector boundary */
} main_gfx_frame_bin_t;
typedef char main_gfx_frame_bin_size_must_be_0x9800[(sizeof(main_gfx_frame_bin_t) == 0x9800) ? 1 : -1];

/* EVENT/WLDFACE.BIN is four of these sheets: forty 32x48 portraits at 4 bits
 * per pixel, eight across and five down, then sixteen rows of four palettes.
 * Palette n is portrait n's; rows 10-15 hold the other palettes of the monster
 * portraits, one row per monster with the palette number as the column. */
typedef struct main_gfx_wldface_sheet {
    u8 pixels[240][128];       /* 0x0000; 4bpp, 256 pixels per row */
    palette_t palettes[16][4]; /* 0x7800; four 16-color palettes per row */
} main_gfx_wldface_sheet_t;

enum {
    MAIN_GFX_WLDFACE_SHEET_COUNT = 4,
    MAIN_GFX_WLDFACE_SHEET_SIZE = 0x8000,
    MAIN_GFX_WLDFACE_FILE_SIZE = MAIN_GFX_WLDFACE_SHEET_COUNT * MAIN_GFX_WLDFACE_SHEET_SIZE,
    MAIN_GFX_WLDFACE_PORTRAITS_PER_SHEET = 40,
    MAIN_GFX_WLDFACE_FIRST_MONSTER_PORTRAIT = 0x3f,
    MAIN_GFX_WLDFACE_MONSTER_CLUT_ROW = 10, /* first CLUT row of the monsters' other palettes */
    MAIN_GFX_WLDFACE_MONSTER_CLUT_ROW_COUNT = 6,
};
typedef char main_gfx_wldface_sheet_size_must_be_0x8000[(sizeof(main_gfx_wldface_sheet_t) == 0x8000) ? 1 : -1];

extern RECT g_main_gfx_frame_image_rect;
extern RECT g_main_gfx_frame_palette_rect;
extern RECT g_main_gfx_frame_palette_tail_rect;
extern u32 g_main_gfx_now_loading_clut[];
extern u32 g_main_gfx_now_loading_frame_counter;
extern u32 g_main_gfx_now_loading_otags[2][2];

/* Seven textured quads per screen polarity; main_gfx_add_now_loading_to_otag
 * walks the same table as a [7][sizeof(POLY_FT4)] byte array. */
extern POLY_FT4 g_main_gfx_now_loading_primitives[2][7];
extern u32 g_main_gfx_now_loading_visible;
extern upoint16_t g_main_gfx_zodiac_screen_position;
extern u8 g_main_gfx_zodiac_sprite_count;
extern main_zodiac_sprite_frame_t g_main_gfx_zodiac_sprite_frames[];
extern point16_t g_main_gfx_zodiac_sprite_scales[];
extern point16_t g_main_gfx_zodiac_texture_position;
extern DISPENV g_main_gfx_display_envs[2];
extern DRAWENV g_main_gfx_draw_envs[2];
extern s32 g_main_gfx_loading_display_frame_counter;
extern s32 g_main_gfx_screen_polarity;
void main_gfx_add_now_loading_to_otag(u32* otag);
void main_gfx_build_now_loading(u32 visible, s32 x, s32 y);
void main_gfx_draw_now_loading_message(void);
u32* main_gfx_get_otag(void);
void main_gfx_load_efc_fnt(void);
void main_gfx_load_frame_bin_into_vram(void);
void main_gfx_load_zodiac_frame(void);
void main_gfx_reset_display(int width, int height, int projection, u8 red, u8 green, u8 blue);
void main_gfx_swap_and_clear_otag(void);
int main_gfx_swap_display_area(u32* otag);
void main_set_display_draw(int width, int height, int projection, u8 red, u8 green, u8 blue);
void main_set_display_draw_new_game(int width, int height, int projection, u8 red, u8 green, u8 blue);
void main_gfx_build_now_loading_center(u32 visible);
void main_gfx_build_zodiac_primitives(zodiac_draw_context_t* context, POLY_FT4* poly);
void main_gfx_call_build_zodiac_bin(zodiac_draw_context_t* context, POLY_FT4* poly);

void main_gfx_build_zodiac_sprite_polygon(POLY_FT4* poly, point16_t* texture, upoint16_t* base,
    main_zodiac_sprite_frame_t* frame, point16_t* scale, main_zodiac_draw_offset_t* offset);

/* other */
/* g_main_terrain_status_flags has one byte per map_surface_type_e; only
 * MAP_SURFACE_POISONED_MARSH has a bit. */
enum {
    MAIN_TERRAIN_STATUS_FLAG_POISON = 0x80, /* battle_status_prepare_terrain_poison poisons a unit standing there */
};

/* Rows of g_main_terrain_movement_cost_tables, chosen from the unit's movement
 * abilities by battle_move_store_unit_movement_to_scratchpad; each has one
 * cost per map_surface_type_e. */
typedef enum main_terrain_cost_table {
    MAIN_TERRAIN_COST_TABLE_DEFAULT = 0,
    MAIN_TERRAIN_COST_TABLE_FLY_TELEPORT = 1, /* every surface costs 1 */
    MAIN_TERRAIN_COST_TABLE_FLOAT = 2,        /* every surface costs 1 */
    MAIN_TERRAIN_COST_TABLE_WATER = 3,        /* Move in Water and Walk on Water */
    MAIN_TERRAIN_COST_TABLE_UNDERWATER = 4,
    MAIN_TERRAIN_COST_TABLE_LAVA = 5,
} main_terrain_cost_table_e;

/* Costs in those rows that are not a number of Move points. */
enum {
    MAIN_TERRAIN_COST_WEATHER = 0,    /* replaced by battle_move_get_weather_terrain_cost */
    MAIN_TERRAIN_COST_BLOCKED = 0xff, /* stays so under Any Ground */
};

extern volatile u8 g_fntload_clut_y_offset_immediate;
extern s32 g_main_font_print_enabled;
extern u8* g_main_brave_story_ages_source;
extern u8 g_main_menu_scroll_accel_delay;
extern u8 g_main_menu_scroll_slow_step;
extern u8 g_main_menu_scroll_fast_step;
extern u8 g_main_accessory_page_order[0x21];
extern u8 g_main_armor_page_order[0x25];
extern u8 g_main_helmet_page_order[0x1d];
extern u8 g_main_action_menu_types_by_skillset[0xe0];
extern s32 g_main_battle_bin_sector;
extern u8 g_main_common_menu_graphics_loaded;
extern u32 g_main_tutorial_progress_masks[];
extern u8 g_main_special_portrait_unit_id;
extern u8 g_main_calculator_primes[CALCULATOR_PRIME_COUNT];
extern u16 g_main_month_start_day_offsets[13];
extern unit_deployment_coordinates_t* g_main_unit_deployed_coords;
extern u8 g_main_crystal_learnable_abilities[];
extern crystal_pickup_result_t g_main_crystal_pickup_result;
extern u8 g_main_crystal_treasure_item_id;
extern u8 g_main_brave_story_character_ages[0x40];
extern s32 g_main_debug_display_enabled;
extern s32 g_main_replay_story_event_index;
extern u8 g_main_special_portrait_wldface_id;
extern unit_deployment_coordinates_t g_main_deployed_unit_map_coordinates[6];
extern u8 g_main_terrain_movement_cost_tables[][64];
extern const u8 g_main_terrain_status_flags[64];
s32 main_sound_is_music_loading_stub(void);

/* unnamed */
void func_800184e0(u16 value);
extern u16 D_80032A28; /* only ever stored */
extern u32 D_80032A34; /* only ever stored */
extern s16 D_80032A58; /* only ever stored */
extern s32 D_800459D8;
extern u8 D_800473A7;
extern u8 D_80057b1c;
/* Two bytes per map_surface_type_e; all 64 pairs are 0x19, 0x19. */
extern u8 g_main_terrain_tile_info_pairs[64][2];

/* entry */

#endif
