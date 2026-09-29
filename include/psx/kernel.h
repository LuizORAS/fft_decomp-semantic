#ifndef PSX_KERNEL_H
#define PSX_KERNEL_H

#include "psx/types.h"

/* Event descriptors (DescHW|0x11, DescSW|0x01), event specs
 * and event modes, as passed to OpenEvent by the memory-card setup. */
#define HwCdRom    0xF0000003
#define HwCARD     0xF0000011
#define HwSPU      0xF0000009
#define SwCARD     0xF4000001
#define EvSpIOE    0x0004
#define EvSpCOMP   0x0020
#define EvSpDR     0x0040
#define EvSpTIMOUT 0x0100
#define EvSpNEW    0x2000
#define EvSpERROR  0x8000
#define EvMdNOINTR 0x2000

/* Memory-card directory entry (firstfile/nextfile). */
typedef struct DIRENTRY {
    char name[20];
    int attr;
    int size;
    struct DIRENTRY* next;
    int head;
    char system[4];
} DIRENTRY;

#endif
