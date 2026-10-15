#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Count the leading bits of `value` equal to its sign bit (LZCS; 1-32, psx-spx). csqrt calls it. */
s32 Lzc(s32 value) {
    s32 count;
    PSYQ_GTE_LZCS(value, count);
    return count;
}
