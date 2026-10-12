#ifndef PSX_KERNEL_H
#define PSX_KERNEL_H

/* Event descriptors, specs and modes used by the linked drivers. */
#define HwCdRom    0xF0000003
#define HwCARD     0xF0000011
#define HwSPU      0xF0000009
#define SwCARD     0xF4000001
#define RCntCNT2   0xF2000002
#define RCntMdINTR 0x1000
#define EvSpINT    0x0002
#define EvSpIOE    0x0004
#define EvSpCOMP   0x0020
#define EvSpDR     0x0040
#define EvSpTIMOUT 0x0100
#define EvSpTRAP   0x1000
#define EvSpNEW    0x2000
#define EvSpERROR  0x8000
#define EvMdINTR   0x1000
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
