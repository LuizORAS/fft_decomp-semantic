#include "psx/types.h"

/* Four 8-byte slots starting at +0xc, each with a pair of bytes at +0/+1.
 * The function is reached only through a function pointer, and no other code
 * names the owning type. */
typedef struct {
    u8 x;
    u8 y;
    u8 _unused_02[6];
} world_800e6f64_slot_t;

typedef struct {
    u8 _unused_00[0xc];
    world_800e6f64_slot_t slots[4];
} world_800e6f64_record_t;

/* When flag is negative, swap slots 0<->1 and 2<->3: the x bytes if slot 0
 * and 1 already differ in x, otherwise the y bytes. */
void world_gfx_flip_poly_ft4_uv_if_negative(world_800e6f64_record_t* record, s32 flag) {
    u8 swap;

    if (flag < 0) {
        if (record->slots[0].x != record->slots[1].x) {
            swap = record->slots[0].x;
            record->slots[0].x = record->slots[1].x;
            record->slots[1].x = swap;
            swap = record->slots[2].x;
            record->slots[2].x = record->slots[3].x;
            record->slots[3].x = swap;
        } else {
            swap = record->slots[0].y;
            record->slots[0].y = record->slots[1].y;
            record->slots[1].y = swap;
            swap = record->slots[2].y;
            record->slots[2].y = record->slots[3].y;
            record->slots[3].y = swap;
        }
    }
}
