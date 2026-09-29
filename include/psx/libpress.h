#ifndef PSX_LIBPRESS_H
#define PSX_LIBPRESS_H

#include "psx/types.h"

s32 DecDCTvlcSize(s32 words);
void DecDCTvlc(void* frame_data, void* decode_buffer);

/* driver internals */

enum { PSYQ_PRESS_VLC_DEFAULT_HALFWORDS = 0x00ffffff };

/* Resume(NULL) restores this handwritten VLC context; offset0x18 is untouched. */
typedef struct {
    u16* input;
    u16* output;
    u32 bit_window;
    s32 consumed_bits;
    s32 quant_scale_shifted;
    s32 component;
    s32 _unknown_18;
    s32 predictor_cr;
    s32 predictor_cb;
    s32 predictor_y;
} psyq_press_vlc_state_t;

typedef struct {
    u16 prefix_bits;
    u16 magnitude_bits;
} psyq_press_dc_entry_t;

typedef struct {
    u32 first_code_and_bits;
    u32 extra_codes;
} psyq_press_ac_entry_t;

extern u32 g_psyq_press_vlc_limit_halfwords;
/* The last word aliases g_open_birthday_month_lengths[0..3] in retail. */
extern psyq_press_vlc_state_t g_psyq_press_vlc_state;
extern const psyq_press_dc_entry_t g_psyq_press_dc_luma_table[];
extern const psyq_press_dc_entry_t g_psyq_press_dc_chroma_table[];
extern const psyq_press_ac_entry_t g_psyq_press_ac_primary_table[];
extern const u32 g_psyq_press_ac_secondary_table[];

#endif
