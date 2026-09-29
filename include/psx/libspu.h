#ifndef PSX_LIBSPU_H
#define PSX_LIBSPU_H

#include "psx/types.h"

/* Clean-room declarations of the Psy-Q LIBSPU interface that the linked
 * library (0x800186c4-0x8001bb5c) exports to the Suzuki sound driver. Names,
 * layouts and signatures follow the Psy-Q library reference; the offsets
 * noted on each type are the ones the driver's callers use. */

/* Psy-Q SpuVolume: a left/right volume pair. */
typedef struct {
    s16 left;  /* 0x00 */
    s16 right; /* 0x02 */
} SpuVolume;

/* Psy-Q SpuReverbAttr (0x14 bytes). The driver keeps one at 0x80037008 for
 * SpuSetReverbModeParam and SpuSetReverbDepth. */
typedef struct {
    u32 mask;        /* 0x00 */
    s32 mode;        /* 0x04 */
    SpuVolume depth; /* 0x08 */
    s32 delay;       /* 0x0c */
    s32 feedback;    /* 0x10 */
} SpuReverbAttr;

/* Psy-Q SpuExtAttr: CD or external input settings of SpuCommonAttr. */
typedef struct {
    SpuVolume volume; /* 0x00 */
    s32 reverb;       /* 0x04 */
    s32 mix;          /* 0x08 */
} SpuExtAttr;

/* Psy-Q SpuCommonAttr (0x28 bytes); the driver's copy is at 0x80037020. */
typedef struct {
    u32 mask;           /* 0x00 */
    SpuVolume mvol;     /* 0x04 */
    SpuVolume mvolmode; /* 0x08 */
    SpuVolume mvolx;    /* 0x0c */
    SpuExtAttr cd;      /* 0x10 */
    SpuExtAttr ext;     /* 0x1c */
} SpuCommonAttr;

/* Psy-Q SpuVoiceAttr (0x40 bytes). 0x80014180 fills addr and the envelope
 * fields from a WAVESET instrument entry. */
typedef struct {
    u32 voice;         /* 0x00 */
    u32 mask;          /* 0x04 */
    SpuVolume volume;  /* 0x08 */
    SpuVolume volmode; /* 0x0c */
    SpuVolume volumex; /* 0x10 */
    u16 pitch;         /* 0x14 */
    u16 note;          /* 0x16 */
    u16 sample_note;   /* 0x18 */
    s16 envx;          /* 0x1a */
    u32 addr;          /* 0x1c */
    u32 loop_addr;     /* 0x20 */
    s32 a_mode;        /* 0x24 */
    s32 s_mode;        /* 0x28 */
    s32 r_mode;        /* 0x2c */
    u16 ar;            /* 0x30 */
    u16 dr;            /* 0x32 */
    u16 sr;            /* 0x34 */
    u16 rr;            /* 0x36 */
    u16 sl;            /* 0x38 */
    u16 adsr1;         /* 0x3a */
    u16 adsr2;         /* 0x3c */
} SpuVoiceAttr;

/* Psy-Q SpuDecodedData: the SPU's decoded CD and voice 1/3 capture buffers.
 * SpuReadDecodedData (0x8001ac7c) offsets voice1 by 0x800 bytes. */
typedef struct {
    s16 cd_left[0x200];
    s16 cd_right[0x200];
    s16 voice1[0x200];
    s16 voice3[0x200];
} SpuDecodedData;

typedef void (*SpuTransferCallbackProc)(void);

extern void SpuInit(void);
extern void SsUtReverbOn(void);

void SpuFree(u32 addr);
s32 SpuMalloc(s32 size);
void SpuQuit(void);
u32 _spu_Fr(void* addr, u32 size);

/* libspu internals. */
extern s32 _spu_inTransfer;
/* Sound interrupt callbacks read and update these reverb attributes. */
extern volatile s32 _spu_rev_attr_delay;
extern volatile u16 _spu_rev_attr_depth_left;
extern volatile u16 _spu_rev_attr_depth_right;
extern volatile s32 _spu_rev_attr_feedback;
extern volatile s32 _spu_rev_attr_mode;
extern void (*_spu_transferCallback)(void);
void _SpuInit(s32 hot);
u32 _SpuSetAnyVoice(s32 on_off, u32 voice_bit, s32 reg_index_low, s32 reg_index_high);

/* 0x800194c4-0x8001b4b0: entry points used by the Suzuki driver. */
s32 SpuInitMalloc(s32 num, u8* top);
u32 SpuSetNoiseVoice(s32 on_off, u32 voice_bit);
s32 SpuSetNoiseClock(s32 n_clock);
u32 SpuRead(u8* addr, u32 size);
s32 SpuSetReverb(s32 on_off);
s32 SpuSetReverbModeParam(SpuReverbAttr* attr);
void SpuGetReverbModeParam(SpuReverbAttr* attr);
s32 SpuSetReverbDepth(SpuReverbAttr* attr);
s32 SpuReadDecodedData(SpuDecodedData* d_data, s32 flag);
void SpuSetKey(s32 on_off, u32 voice_bit);
/* The target stores the key status with `sh`; the documented pointer type
 * is long. */
void SpuGetVoiceEnvelopeAttr(s32 v_num, s32* key_stat, s16* envx);
u32 SpuWrite(u8* addr, u32 size);
u32 SpuSetTransferStartAddr(u32 addr);
s32 SpuSetTransferMode(s32 mode);
SpuTransferCallbackProc SpuSetTransferCallback(SpuTransferCallbackProc func);
u32 SpuSetPitchLFOVoice(s32 on_off, u32 voice_bit);
void SpuSetCommonAttr(SpuCommonAttr* attr);
u32 SpuSetReverbVoice(s32 on_off, u32 voice_bit);

/* Individual voice attribute setters (0x8001b428-0x8001bb58). */
void SpuSetVoiceVolume(s32 v_num, s16 vol_l, s16 vol_r);
void SpuSetVoiceVolumeAttr(s32 v_num, s16 vol_l, s16 vol_r, s16 vol_mode_l, s16 vol_mode_r);
void SpuSetVoicePitch(s32 v_num, u16 pitch);
void SpuSetVoiceStartAddr(s32 v_num, u32 start_addr);
void SpuSetVoiceLoopStartAddr(s32 v_num, u32 loop_start_addr);
void SpuSetVoiceDR(s32 v_num, u16 dr);
void SpuSetVoiceRR(s32 v_num, u16 rr);
void SpuSetVoiceSL(s32 v_num, u16 sl);
void SpuSetVoiceARAttr(s32 v_num, u16 ar, s32 ar_mode);
void SpuSetVoiceSRAttr(s32 v_num, u16 sr, s32 sr_mode);
void SpuSetVoiceRRAttr(s32 v_num, u16 rr, s32 rr_mode);

void SpuStart(void);
void SpuInitHot(void);
s32 SpuClearReverbWorkArea(s32 mode);

/* driver internals */

/* DMA/FIFO control values and polling units in the linked driver. */
#define PSYQ_SPU_POLL_LIMIT              3840
#define PSYQ_SPU_FIFO_CHUNK_BYTES        64
#define PSYQ_SPU_STATUS_MASK             0x7ff
#define PSYQ_SPU_STATUS_TRANSFER_BUSY    0x400
#define PSYQ_SPU_TRANSFER_MODE_MASK      0x30
#define PSYQ_SPU_TRANSFER_MODE_CLEAR     0xffcf
#define PSYQ_SPU_TRANSFER_MODE_IO        0x10
#define PSYQ_SPU_TRANSFER_MODE_DMA_WRITE 0x20
#define PSYQ_SPU_TRANSFER_MODE_DMA_READ  0x30

/* Private DMA operation selectors and halfword register indices. */
typedef enum {
    PSYQ_SPU_DMA_READ = 0,
    PSYQ_SPU_DMA_WRITE = 1,
    PSYQ_SPU_DMA_SET_ADDRESS = 2,
    PSYQ_SPU_DMA_START_TRANSFER = 3,
} psyq_spu_dma_operation_e;

typedef enum {
    PSYQ_SPU_REG_KEY_ON_LOW = 0xc4,
    PSYQ_SPU_REG_KEY_ON_HIGH = 0xc5,
    PSYQ_SPU_REG_KEY_OFF_LOW = 0xc6,
    PSYQ_SPU_REG_KEY_OFF_HIGH = 0xc7,
    PSYQ_SPU_REG_PITCH_LFO_LOW = 0xc8,
    PSYQ_SPU_REG_PITCH_LFO_HIGH = 0xc9,
    PSYQ_SPU_REG_NOISE_LOW = 0xca,
    PSYQ_SPU_REG_NOISE_HIGH = 0xcb,
    PSYQ_SPU_REG_REVERB_LOW = 0xcc,
    PSYQ_SPU_REG_REVERB_HIGH = 0xcd,
    PSYQ_SPU_REG_REVERB_START = 0xd1,
} psyq_spu_register_index_e;

typedef enum {
    PSYQ_SPU_ADDRESS_AS_UNITS = -1,
    PSYQ_SPU_ADDRESS_ALIGNED_BYTES = -2,
    PSYQ_SPU_REGISTER_RAW = 0,
    PSYQ_SPU_REGISTER_BYTE_ADDRESS = 1,
} psyq_spu_address_format_e;

#define PSYQ_SPU_HEAP_FREE         0x80000000U
#define PSYQ_SPU_HEAP_TAIL         0x40000000U
#define PSYQ_SPU_HEAP_DELETED      0x2fffffffU
#define PSYQ_SPU_HEAP_ADDRESS_MASK 0x0fffffffU
#define PSYQ_SPU_HEAP_START        0x1010U

/* Register layout reached through LIBSPU's pointer at 0x8002ad44. */
typedef struct psyq_spu_voice {
    u16 volume_left;
    u16 volume_right;
    u16 pitch;
    u16 start_address;
    u16 adsr1;
    u16 adsr2;
    u16 envelope;
    u16 loop_address;
} psyq_spu_voice_t;

typedef struct psyq_spu_registers {
    psyq_spu_voice_t voices[24]; /* 0x000 */
    u16 main_volume_left;        /* 0x180 */
    u16 main_volume_right;
    u16 reverb_depth_left;
    u16 reverb_depth_right;
    u16 key_on_low; /* 0x188 */
    u16 key_on_high;
    u16 key_off_low;
    u16 key_off_high;
    u16 pitch_lfo_low; /* 0x190 */
    u16 pitch_lfo_high;
    u16 noise_low;
    u16 noise_high;
    u16 reverb_low;
    u16 reverb_high;
    u16 voice_status_low;
    u16 voice_status_high;
    u16 _unknown_1a0;
    u16 reverb_start;
    u16 irq_address;
    u16 transfer_address;
    u16 transfer_fifo;
    u16 control;
    u16 transfer_control;
    u16 status;
    u16 cd_volume_left; /* 0x1b0 */
    u16 cd_volume_right;
    u16 external_volume_left;
    u16 external_volume_right;
    u16 current_main_volume_left;
    u16 current_main_volume_right;
    u16 _unknown_1bc;
    u16 _unknown_1be;
    u16 reverb_parameters[32]; /* 0x1c0 */
} psyq_spu_registers_t;

typedef char assert_psyq_spu_voice_size[(sizeof(psyq_spu_voice_t) == 0x10) ? 1 : -1];
typedef char assert_psyq_spu_register_size[(sizeof(psyq_spu_registers_t) == 0x200) ? 1 : -1];

extern volatile psyq_spu_registers_t* _spu_RXX;

extern volatile u32* g_psyq_spu_dma_address_register;  /* DMA4 MADR */
extern volatile u32* g_psyq_spu_dma_block_register;    /* DMA4 BCR */
extern volatile u32* g_psyq_spu_dma_control_register;  /* DMA4 CHCR */
extern volatile u32* g_psyq_spu_dma_priority_register; /* DMA priority: DPCR */
extern volatile u32* g_psyq_spu_delay_register;
extern u16 _spu_tsa;       /* transfer address, in eight-byte SPU units */
extern s32 _spu_transMode; /* normalized transfer mode: 0 DMA, 1 programmed I/O */
extern s32 _spu_addrMode;
extern s32 _spu_mem_mode;       /* address alignment enabled */
extern s32 _spu_mem_mode_plus;  /* byte-address shift */
extern s32 _spu_mem_mode_unit;  /* address alignment */
extern s32 _spu_mem_mode_unitM; /* alignment mask */
extern void (*_spu_IRQCallback)(void);
extern u16 g_psyq_spu_init_fifo_data[8]; /* initialization block sent through the FIFO */
extern s32 g_psyq_spu_dma_direction;     /* transfer direction: 0 to SPU, 1 from SPU */
extern void* g_psyq_spu_dma_buffer;
extern u32 g_psyq_spu_dma_blocks;
extern s32 _spu_isCalled;   /* event/callback setup flag */
extern u32 _spu_env;        /* queued operation mask */
extern s32 _spu_EVdma;      /* DMA completion BIOS event */
extern u32 _spu_keystat;    /* keyed-on voices */
extern s32 _spu_trans_mode; /* caller's transfer-mode value */
extern s32 _spu_rev_flag;
extern s32 _spu_rev_reserve_wa; /* reserve reverb work RAM */
extern u32 _spu_rev_offsetaddr; /* reverb work start, in eight-byte units */
extern u32 _spu_RQvoice;
extern u32 _spu_RQmask;
extern volatile u16 _spu_RQ[10]; /* deferred key, LFO, noise and reverb masks */

typedef struct psyq_spu_heap_record {
    u32 address;
    u32 size;
} psyq_spu_heap_record_t;

typedef struct psyq_spu_reverb_parameters {
    u32 mask;
    u16 values[32];
} psyq_spu_reverb_parameters_t;
extern psyq_spu_reverb_parameters_t _spu_rev_param[10];
void _spu_setReverbAttr(psyq_spu_reverb_parameters_t* parameters);
extern s32 _spu_AllocBlockNum;
extern s32 _spu_AllocLastNum;
extern psyq_spu_heap_record_t* _spu_memList;
extern u32 _spu_rev_startaddr[10]; /* reverb work starts, in eight-byte units */
extern u8 _spu_zerobuf[0x400];     /* zero block used to clear reverb work RAM */
extern u16 _spu_voice_centerNote[24];
extern volatile u16 g_psyq_spu_queued_register_base[]; /* shifted alias: indices 0xc4..0xcd reach _spu_RQ */
extern const char g_psyq_spu_timeout_format[];
extern const char g_psyq_spu_init_timeout_name[];
extern const char g_psyq_spu_fifo_ready_timeout_name[];
extern const char g_psyq_spu_status_restore_timeout_name[];

void _spu_gcSPU(void);
s32 _SpuIsInAllocateArea(u32 address);
s32 _SpuIsInAllocateArea_(u32 address);

s32 _spu_init(s32 hot);
void _spu_writeByIO(const u16* source, u32 size);
void _spu_FiDMA(void);
void _spu_Fr_(void* destination, u16 address, u32 blocks);
s32 _spu_t(s32 operation, ...);
u32 _spu_Fw(void* source, u32 size);
void _spu_FsetRXX(s32 reg_index, u32 value, s32 is_address);
u32 _spu_FgetRXXa(s32 reg_index, s32 raw);
void _spu_FsetPCR(s32 enabled);
void _spu_FsetDelayR(void);
void _spu_FsetDelayW(void);
void _spu_Fw1ts(void);
void _SpuDataCallback(void (*callback)(void));

/* This repeated arithmetic is a real hardware-settling delay in retail. */
#define PSYQ_SPU_REGISTER_DELAY()                                                                                      \
    {                                                                                                                  \
        volatile s32 count;                                                                                            \
        volatile s32 value;                                                                                            \
        value = 1;                                                                                                     \
        for (count = 0; count < 2; count++) {                                                                          \
            value *= 13;                                                                                               \
        }                                                                                                              \
    }

u32 _spu_FsetRXXa(s32 reg_index, u32 address);

#endif
