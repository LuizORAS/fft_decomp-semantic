#ifndef MEMORY_H
#define MEMORY_H

/* Game memory loops; each entry keeps its original argument order and bounds. */

#include "psx/types.h"

/* util */
/* MAIN's small helpers: the pass/fail roll, the vector setters, and byte and action-record copies and
 * clears. */

/* Clear count bytes; nonpositive counts leave the destination unchanged. */
void main_mem_zero(void* destination, s32 count);

void main_mem_copy_byte_data_source_first(const void* source, void* destination, s32 count);

/* Source-first byte copies; nonpositive counts leave the destination unchanged. */
void main_mem_copy_bytes_source_first(const void* source, void* destination, int count);

/* ai */
/* The AI that plays computer, guest and Auto-Battle units: its workspace, the per-unit ability lists
 * and decision records, the movement scenarios and tile grids, the ability search, and the
 * simulation, scoring and ranking that choose the turn's command (docs/mechanics/AI.md). */

/* Clear byte_count / 4 signed 32-bit values; leave any trailing bytes unchanged. */
void battle_mem_zero_32bit_values(void* destination, s32 byte_count);

/* Copy forward using a signed destination-end address; nonpositive counts do nothing. */
void battle_mem_copy_bytes(void* destination, const void* source, s32 byte_count);

/* Copy byte_count / 2 unsigned 16-bit values; leave an odd final byte unchanged. */
void battle_mem_copy_u16_values(void* destination, const void* source, s32 byte_count);

/* util */
/* BATTLE's hand-written helpers: a forward byte copy, a signed multiply-divide through a 64-bit product
 * and a Q12 cross product that flags overflow (the camera tracks' crossing points). */

/* Copy forward; count must be positive. */
void mem_copy_bytes_nonempty(void* destination, const void* source, s32 count);

#endif /* MEMORY_H */
