#ifndef PSX_GTE_REGS_H
#define PSX_GTE_REGS_H

/* COP2 control and data registers have independent numeric namespaces. */
#define PSYQ_GTE_CTRL_R11_R12         0
#define PSYQ_GTE_CTRL_R13_R21         1
#define PSYQ_GTE_CTRL_R22_R23         2
#define PSYQ_GTE_CTRL_R31_R32         3
#define PSYQ_GTE_CTRL_R33             4
#define PSYQ_GTE_CTRL_TRX             5
#define PSYQ_GTE_CTRL_TRY             6
#define PSYQ_GTE_CTRL_TRZ             7
#define PSYQ_GTE_CTRL_L11_L12         8
#define PSYQ_GTE_CTRL_L13_L21         9
#define PSYQ_GTE_CTRL_L22_L23         10
#define PSYQ_GTE_CTRL_L31_L32         11
#define PSYQ_GTE_CTRL_L33             12
#define PSYQ_GTE_CTRL_RBK             13
#define PSYQ_GTE_CTRL_GBK             14
#define PSYQ_GTE_CTRL_BBK             15
#define PSYQ_GTE_CTRL_LR1_LR2         16
#define PSYQ_GTE_CTRL_LR3_LG1         17
#define PSYQ_GTE_CTRL_LG2_LG3         18
#define PSYQ_GTE_CTRL_LB1_LB2         19
#define PSYQ_GTE_CTRL_LB3             20
#define PSYQ_GTE_CTRL_RFC             21
#define PSYQ_GTE_CTRL_GFC             22
#define PSYQ_GTE_CTRL_BFC             23
#define PSYQ_GTE_CTRL_OFX             24
#define PSYQ_GTE_CTRL_OFY             25
#define PSYQ_GTE_CTRL_H               26
#define PSYQ_GTE_CTRL_DQA             27
#define PSYQ_GTE_CTRL_DQB             28
#define PSYQ_GTE_CTRL_ZSF3            29
#define PSYQ_GTE_CTRL_ZSF4            30
#define PSYQ_GTE_CTRL_FLAG            31
#define PSYQ_GTE_DATA_IR1             9
#define PSYQ_GTE_DATA_IR2             10
#define PSYQ_GTE_DATA_IR3             11
#define PSYQ_GTE_DATA_MAC1            25
#define PSYQ_GTE_DATA_MAC2            26
#define PSYQ_GTE_DATA_MAC3            27
#define PSYQ_GTE_DATA_LZCS            30
#define PSYQ_GTE_DATA_LZCR            31
#define PSYQ_GTE_CMD_SQUARE_0         0x4aa00428
#define PSYQ_GTE_CMD_SQUARE_12        0x4aa80428
#define PSYQ_GTE_CMD_OUTER_PRODUCT_0  0x4b70000c
#define PSYQ_GTE_CMD_OUTER_PRODUCT_12 0x4b78000c
#define PSYQ_GTE_CMD_LIGHT_COLOR      0x4a4da412

#define PSYQ_GTE_COP2_ENABLE 0x40000000

#endif
