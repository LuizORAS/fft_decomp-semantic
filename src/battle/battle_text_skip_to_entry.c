#include "psx/types.h"

/* Return entry `entry_index` of a text section: the byte after the `entry_index`th 0xFE or 0xFF. The
 * scan decodes nothing, because in the retail files no byte inside an entry takes those values: glyph indexes
 * stay below 0xD0, and a back-reference's operand bytes below 0xEF and 0xFE.
 *
 * The retail helper is hand-written: trapping addi/add, $t0-$t2 scratch registers and empty branch delay
 * slots, which the asm statement keeps. */
const u8* battle_text_skip_to_entry(const u8* text, s32 entry_index) {
    const u8* result;
    const u8* cursor = text;
    s32 index = entry_index;

    __asm__ volatile("addi $9,$0,0\n"
                     "addi $10,$0,0xfe\n"
                     "1:\n"
                     "lbu $8,0($4)\n"
                     "beq $5,$9,2f\n"
                     "andi $8,$8,0xfe\n"
                     "bne $8,$10,3f\n"
                     "addi $9,$9,1\n"
                     "3:\n"
                     "addi $4,$4,1\n"
                     "bgez $0,1b\n"
                     "2:\n"
                     "add $2,$0,$4"
        : "=r"(result), "=r"(cursor)
        : "1"(cursor), "r"(index)
        : "$8", "$9", "$10");
    return result;
}
