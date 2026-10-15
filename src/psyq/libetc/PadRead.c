#include "psx/libetc.h"

/* Return both pads' buttons, a 1 bit for each pressed button: pad 1 in the low 16 bits, pad 2 in the high
 * 16. It refreshes the word with PAD_dr first; `id` is unused (the callers pass 0 or 1). */
u32 PadRead(s32 id) {
    PAD_dr();
    return ~g_psyq_etc_pad_buttons;
}
