#include "fft/event_card.h"
#include "psx/types.h"

/* Opcode 0x0f (MENU_SCRIPT_OP_DRAW_PRIORITY): byte 3 becomes the ordering-table index of the text
 * primitives (g_card_text_otag_index). Returns the next command. Only CARD's handler table holds it, and
 * no CARD code reads that table: CARD calls its handlers directly. */
const u8* card_menu_script_set_draw_priority(const u8* data) {
    g_card_text_otag_index = data[3];
    return data + data[1];
}
