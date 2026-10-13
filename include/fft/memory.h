#ifndef MEMORY_H
#define MEMORY_H

/* Game memory loops; each entry keeps its original argument order and bounds. */

#include "psx/types.h"

/* Copy forward; count must be positive. */
void copy_bytes_nonempty(void* destination, const void* source, s32 count);

/* Source-first byte copies; nonpositive counts leave the destination unchanged. */
void main_util_copy_bytes(const void* source, void* destination, int count);
void main_util_copy_byte_data(const void* source, void* destination, s32 count);

/* Clear count bytes; nonpositive counts leave the destination unchanged. */
void main_util_clear_byte_data(void* destination, s32 count);

/* ai buffers */
/* Copy forward using a signed destination-end address; nonpositive counts do nothing. */
void ai_copy_bytes(void* destination, const void* source, s32 byte_count);

/* Copy byte_count / 2 unsigned 16-bit values; leave an odd final byte unchanged. */
void ai_copy_u16_values(void* destination, const void* source, s32 byte_count);

/* Clear byte_count / 4 signed 32-bit values; leave any trailing bytes unchanged. */
void ai_clear_words(void* destination, s32 byte_count);

#endif /* MEMORY_H */
