#ifndef PSX_SUZUKI_H
#define PSX_SUZUKI_H

#include "psx/types.h"

/* Suzuki CPU arena allocator; shared with the sound driver. */
/* Suzuki heap block header (0x10 bytes; payload follows). The $gp-relative
 * allocator (0x8001423c-0x8001442c) rounds requests to 16 bytes and chains
 * blocks in address order from g_main_sound_heap_blocks. */
typedef struct suzuki_heap_block {
    u16 flags;                      /* 0x00; 0x8000 list head, 0x2 in use */
    u16 _unknown_02;                /* 0x02; cleared on allocation; never read */
    u32 _unknown_04;                /* 0x04; cleared on allocation; never read */
    u8* end;                        /* 0x08; end of the payload */
    struct suzuki_heap_block* next; /* 0x0c */
} suzuki_heap_block_t;

extern suzuki_heap_block_t* g_main_sound_heap_arena;  /* 0x800329f8 */
extern suzuki_heap_block_t* g_main_sound_heap_blocks; /* 0x800329fc */
extern u32 g_main_sound_heap_size;                    /* 0x80032a38 */
extern u8* g_main_sound_heap_end;                     /* 0x80032a64 */
extern u8 g_main_sound_heap_memory[];                 /* 0x800370bc; the Suzuki heap arena */

void main_sound_init_heap(void* arena, u32 size);
void* main_sound_alloc(u32 size);
void main_sound_free(void* payload);
s32 main_sound_get_largest_free_block(void);
void main_sound_clear_memory(void* destination, s32 size);

#endif
