#ifndef PSX_LIBETC_H
#define PSX_LIBETC_H

#include "psx/libapi.h"

#include "psx/libc.h"
#include "psx/types.h"

typedef void (*psyq_interrupt_callback_t)(void);

extern int VSync(int);
extern int ResetCallback(void);
extern void VSyncCallback(void*);
extern void PadInit(int);
extern u32 PadRead(s32);

s32 GetVideoMode(void);
void PadStop(void);
void StopCallback(void);
psyq_interrupt_callback_t DMACallback(s32 dma_channel, psyq_interrupt_callback_t func);

void* InterruptCallback(s32 channel, psyq_interrupt_callback_t callback);
void* VSyncCallbacks(s32 slot, void* callback);
s32 RestartCallback(void);
s32 CheckCallback(void);
s32 GetIntrMask(void);
s32 SetIntrMask(s32 mask);
s32 SetVideoMode(s32 mode);

/* driver internals */

/* pad state */
extern s32 g_psyq_etc_pad_mode;
extern u32 g_psyq_etc_pad_buttons;

extern s32 g_psyq_etc_video_mode;

typedef psyq_interrupt_callback_t (*psyq_callback_setter_t)(s32, psyq_interrupt_callback_t);

typedef enum {
    PSYQ_IRQ_VBLANK = 0,
    PSYQ_IRQ_DMA = 3,
    PSYQ_ETC_CALLBACK_SLOTS = 8,
    PSYQ_ETC_VBLANK_COUNTER_MODE = 0x107,
    PSYQ_DMA_IRQ_MASTER_ENABLE = 0x00800000,
    PSYQ_DMA_IRQ_CONTROL_MASK = 0x00ffffff,
    PSYQ_DMA_IRQ_CHANNEL_ENABLE_SHIFT = 16,
    PSYQ_DMA_DEFAULT_PRIORITY = 0x33333333,
    PSYQ_ETC_INTERRUPT_STACK_RESERVED_WORDS = 20,
    PSYQ_ETC_VSYNC_TIMEOUT_SHIFT = 15,
} psyq_interrupt_control_e;
typedef struct {
    u16 initialized;
    u16 in_callback;
    psyq_interrupt_callback_t callbacks[11];
    u16 callback_mask;
    u16 saved_mask;
    u32 saved_dma_priority;
    u32 jump_buffer[12];
    u32 stack[1024];
} psyq_interrupt_state_t;
extern psyq_interrupt_state_t g_psyq_etc_interrupt_state;
extern u32 g_psyq_etc_saved_stack_pointer; /* Alias of interrupt_state.jump_buffer[1]. */
extern volatile u16* g_psyq_etc_irq_status;
extern volatile u32* g_psyq_etc_dma_dpcr;
extern u16 g_psyq_etc_callback_mask;
extern u16 g_psyq_etc_saved_irq_mask;
extern u32 g_psyq_etc_saved_dma_dpcr;
extern psyq_interrupt_callback_t g_psyq_etc_vblank_callbacks[8];
extern volatile u32* g_psyq_etc_timer1_mode;
extern volatile s32 g_psyq_etc_vblank_count; /* Updated by VBlank; retail also performs an unused reload. */
extern volatile u32* g_psyq_etc_dma_dicr;
extern psyq_interrupt_callback_t g_psyq_etc_dma_callbacks[8];
psyq_interrupt_state_t* startIntr(void);
psyq_interrupt_state_t* stopIntr(void);
psyq_interrupt_state_t* restartIntr(void);
void trapIntr(void);
void trapIntrVSync(void);
psyq_callback_setter_t startIntrVSync(void);
psyq_interrupt_callback_t setIntrVSync(s32 slot, psyq_interrupt_callback_t callback);
psyq_callback_setter_t startIntrDMA(void);
void trapIntrDMA(void);
psyq_interrupt_callback_t setIntrDMA(s32 channel, psyq_interrupt_callback_t callback);

typedef struct {
    const char* name;
    psyq_interrupt_callback_t (*dma_callback)(s32, psyq_interrupt_callback_t);
    void* (*irq_callback)(s32, psyq_interrupt_callback_t);
    s32 (*reset)(void);
    void (*stop)(void);
    void* (*vblank_callback)(s32, void*);
    s32 (*restart)(void);
    void* state;
} psyq_interrupt_dispatch_t;
extern psyq_interrupt_dispatch_t* g_psyq_etc_dispatch;
extern volatile u16* g_psyq_etc_irq_mask;
extern u16 g_psyq_etc_in_callback;
void psyq_etc_clear_interrupt_state_words(u32* words, s32 count);
void psyq_etc_clear_vblank_callback_words(u32* words, s32 count);
void psyq_etc_clear_dma_callback_words(u32* words, s32 count);

extern volatile u32* g_psyq_etc_gpu_status;
extern volatile u32* g_psyq_etc_timer1_count;
extern u32 g_psyq_etc_last_timer1_count;
extern s32 g_psyq_etc_last_vblank;
extern char g_psyq_etc_vsync_timeout_message[];
void v_wait(s32 vblank, s32 frames);

/* interrupt diagnostics */
extern s32 g_psyq_etc_unhandled_irq_count;
extern const char g_psyq_etc_uninitialized_irq_format[];
extern const char g_psyq_etc_unhandled_irq_format[];
extern const char g_psyq_etc_dma_error_format[];
extern const char g_psyq_etc_dma_channel_address_format[];

typedef struct {
    volatile u32 address;
    volatile u32 block_control;
    volatile u32 channel_control;
    u32 _padding_0c; /* register spacing: channels sit 0x10 apart and use 12 bytes */
} psyq_dma_channel_t;
extern psyq_dma_channel_t* g_psyq_etc_dma_channels;
psyq_interrupt_callback_t setIntr(s32 slot, psyq_interrupt_callback_t callback);

typedef enum {
    PSYQ_IRQ_TIMER0 = 4,
    PSYQ_IRQ_TIMER1 = 5,
    PSYQ_IRQ_TIMER2 = 6,
    PSYQ_IRQ_SOURCES = 11,
    PSYQ_ETC_UNHANDLED_IRQ_LIMIT = 2048,
    PSYQ_ETC_VBLANK_ROOT_COUNTER = 3,
    PSYQ_DMA_CHANNEL_COUNT = 7,
    PSYQ_DMA_IRQ_FLAG_SHIFT = 24
} psyq_interrupt_handler_control_e;

#define PSYQ_DMA_IRQ_CHANNEL_MASK 0x0000007fU
#define PSYQ_DMA_IRQ_FLAGS_MASK   0xff000000U
#define PSYQ_DMA_IRQ_MASTER_FLAG  0x80000000U
#define PSYQ_DMA_IRQ_BUS_ERROR    0x00008000U

#endif
