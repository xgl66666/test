
#include "../include/hal_suspend.h"

#define PMU_WAKEUP_ADDR_REGL          (0xBF000000 + (0x0E78 << 1))
#define PMU_WAKEUP_ADDR_LMASK           0x000000FF
#define PMU_WAKEUP_ADDR_REGH            (0xBF000000 + (0x0E78 << 1))
#define PMU_WAKEUP_ADDR_HMASK           0x0000FF00

typedef unsigned long               MS_U32;                             // 4 bytes

//MBX
typedef enum
{
    /// House Keeping Identifier
    E_MBX_ROLE_HK = 0,
    /// Co-Processer Identifier
    E_MBX_ROLE_CP = 1,
    /// PM Identifier
    E_MBX_ROLE_PM = 2,
    E_MBX_ROLE_AEON = 3,
    E_MBX_ROLE_MAX
}MBX_ROLE_ID;

#define BIT(x) (1UL << (x))

#define RIU_MAP 0xBF200000

#define RIU8    ((unsigned char  volatile *) RIU_MAP)

#define REG_MBX_BASE                (0x19C0)
#define MBX_REG8(gid, addr)         RIU8[((gid+REG_MBX_BASE)<<2) + ((addr) * 2) - ((addr) & 1)]

//Reg8 defines:
#define REG8_MBX_CTRL               0x0000
    #define MBX_CTRL_FIRE             BIT(0)
    #define MBX_CTRL_READBACK         BIT(1)
    #define MBX_CTRL_INSTANT          BIT(2)
#define REG8_MBX_MAIL_CLASS          0x0001
#define REG8_MBX_MAIL_IDX            0x0002
#define REG8_MBX_PARAMETER_CNT       0x0003
#define REG8_MBX_PARAMETER_S         0x0004
#define REG8_MBX_PARAMETER_E         0x000D
#define REG8_MBX_STATE_0              0x000E
#define REG8_MBX_STATE_1              0x000F
    #define MBX_STATE1_DISABLED          BIT(4)
    #define MBX_STATE1_OVERFLOW           BIT(5)
    #define MBX_STATE1_ERROR              BIT(6)
    #define MBX_STATE1_BUSY               BIT(7)

#define REG_MBX_GROUP0      0x00
#define REG_MBX_GROUP1      0x08
#define REG_MBX_GROUP2      0x10
#define REG_MBX_GROUP3      0x18
#define REG_MBX_GROUP4      0x20
#define REG_MBX_GROUP5      0x28

#define REG8_MBX_GROUP(gid, addr) MBX_REG8(gid, addr)

#define REG8_MBX_MIPS(addr)        MBX_REG8(REG_MBX_GROUP1, addr) //group0 allocated for mips
#define REG8_MBX_AEON(addr)        MBX_REG8(REG_MBX_GROUP0, addr) //group1 allocated for aeon
#define REG8_MBX_PM(addr)          MBX_REG8(REG_MBX_GROUP3, addr) //group3 allocated for pm

//  for MDrv_PM_Get_BrickTerminator_Info  (pm bank dummy register)
// 0x0E h0053 BIT[15:8]  
#define RIU_MAP_MBX     0xBF000000
#define REG_PM_BASE   (0x0EA7)
#define MHAL_PM_REG() ((volatile unsigned char*)(RIU_MAP_MBX))[(REG_PM_BASE << 1) - (REG_PM_BASE & 1)]

int mstar_pm_init(void);


