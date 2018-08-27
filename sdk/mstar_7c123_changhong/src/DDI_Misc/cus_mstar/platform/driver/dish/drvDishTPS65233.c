
#include "Board.h"
#include "MsCommon.h"
#include "drvDish.h"
#include "drvDemod.h"
#include "drvGPIO.h"
#include "drvDishTPS65233.h"
#include "MsTypes.h"
#include "drvIIC.h"

#if(DISH_TYPE == DISH_TPS65233) ||\
   (defined(DISH_TYPE1) && (DISH_TYPE1 == DISH_TPS65233)) || \
   (defined(DISH_TYPE2) && (DISH_TYPE2 == DISH_TPS65233)) || \
   (defined(DISH_TYPE3) && (DISH_TYPE3 == DISH_TPS65233))


#define HB_printf       printf


#ifdef DISH_IIC_PORT
#define TPS65233_IIC_PORT DISH_IIC_PORT
#else
#define TPS65233_IIC_PORT FRONTEND_TUNER_PORT
#endif

#ifdef DISH_IIC_PORT1
#define TPS65233_IIC_PORT1 DISH_IIC_PORT1
#else
#define TPS65233_IIC_PORT1 FRONTEND_TUNER_PORT1
#endif

#ifdef DISH_IIC_PORT2
#define TPS65233_IIC_PORT2 DISH_IIC_PORT2
#else
#define TPS65233_IIC_PORT2 FRONTEND_TUNER_PORT2
#endif

#ifdef DISH_IIC_PORT3
#define TPS65233_IIC_PORT3 DISH_IIC_PORT3
#else
#define TPS65233_IIC_PORT3 FRONTEND_TUNER_PORT3
#endif


//#define TPS65233_IIC_Write( a, b, c, d, e)   MDrv_IIC1_Write(a, b, c, d, e)
//#define TPS65233_IIC_Read( a, b, c, d, e)    MDrv_IIC1_Read(a, b, c, d, e)


#define TPS65233_SLAVE_ID					(0xC2)    //Addr En pull hight
//#define TPS65233_SLAVE_ID					(0xC0)    //Addr En pull low
#define TPS65233_CONTROL_REGISTER_1     (0x00)
#define TPS65233_I2C_CON_MASK               (0x7F)
#define TPS65233_TGATE_TMODE_MASK       (0xCF)
#define TPS65233_LNB_POWER_SET_MASK     (0xF0)

#define TPS65233_CONTROL_REGISTER_2     (0x01)
#define TPS65233_CL1_CL0_MASK               (0xF9)
#define TPS65233_TONE_POS_MASK          (0xE7)
#define TPS65233_CL_EXT_MASK                (0xFE)

#define TPS65233_STATUS_REGISTER_1      (0x02)

static DISH_MS_INIT_PARAM DishInitParam[MAX_FRONTEND_NUM];

static BYTE g_ucDevSlaveID[MAX_TUNER_NUM] = {TPS65233_SLAVE_ID, TPS65233_SLAVE_ID};

static BOOL TPS65233_iReadReg(CSHDITunerIndex eTunerIndex, BYTE *pucRegAddress, BYTE *pucDatabuf);
static BOOL TPS65233_iWriteReg(CSHDITunerIndex eTunerIndex, BYTE *pucRegAddress, BYTE *pucDatabuf);
static BOOL TPS65233_iWriteRegBits(CSHDITunerIndex eTunerIndex, BYTE ucRegAddress, BYTE ucData, BYTE ucMask);
static BOOL TPS65233_iCurrentLimit(CSHDITunerIndex eTunerIndex, TPS65233_CurrentLimit_E eCurrentLimit);
static BOOL TPS65233_iSetCurrentControlMode(CSHDITunerIndex eTunerIndex, TPS65233_CLController_E eCLController);

/*************************************************
  Function:         Tps65233 I2C 寄存器读函数
  Description:      Tps65233 I2C 寄存器读函数
  Input:                eTunerIndex, pRegAddress(寄存器地址指针), pDatabuf(读取数据指针)
  Output:           NULL
  Return:           成功返回TRUE,否则返回FALSE
*************************************************/
static HWI2C_PORT _get_I2C_port(MS_U8 u8DishIndex)
{
    HWI2C_PORT ehwI2c_port;
    switch(u8DishIndex)
    {
        case 1:
            ehwI2c_port = TPS65233_IIC_PORT1;
            break;
        case 2:
            ehwI2c_port = TPS65233_IIC_PORT2;
            break;
        case 3:
            ehwI2c_port = TPS65233_IIC_PORT3;
            break;
        case 0:
        default:    
            ehwI2c_port = TPS65233_IIC_PORT;
            break;
    }
    return ehwI2c_port;
}


static BOOL TPS65233_iReadReg(CSHDITunerIndex eTunerIndex, BYTE *pucRegAddress, BYTE *pucDatabuf)
{
    BOOL bRet = TRUE;
    HWI2C_PORT ehwI2c_port;

    ehwI2c_port = _get_I2C_port((MS_U8)eTunerIndex);

    do
    {
        if ((NULL == pucRegAddress) || (NULL == pucDatabuf))
        {
            HB_printf("[%s]Bad parameter 'pucRegAddress' or 'pucDatabuf'!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }

        //设置Tps65233读寄存器模式

        if(ehwI2c_port > E_HWI2C_PORT_1)
            bRet = MDrv_HWI2C_SetReadModeP1(E_HWI2C_READ_MODE_DIRECTION_CHANGE);
        else
            bRet = MDrv_HWI2C_SetReadMode(E_HWI2C_READ_MODE_DIRECTION_CHANGE);

        if (!bRet)
        {
            HB_printf("[%s]MDrv_HWI2C_SetReadModeP1 FAIL!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }
        
        if(!MDrv_IIC_ReadBytes(ehwI2c_port,g_ucDevSlaveID[eTunerIndex], 1, pucRegAddress, 1, pucDatabuf))
        {
            HB_printf("[%s]TPS65233_IIC_Read FAIL!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }

        //设置Tps65233读寄存器模式
        if(ehwI2c_port > E_HWI2C_PORT_1)
            bRet = MDrv_HWI2C_SetReadModeP1(E_HWI2C_READ_MODE_DIRECT);
        else
            bRet = MDrv_HWI2C_SetReadMode(E_HWI2C_READ_MODE_DIRECT);
        
        if (!bRet)
        {
            HB_printf("[%s]MDrv_HWI2C_SetReadModeP1 FAIL!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }
    }
    while (0);

    return bRet;
}

/*************************************************
  Function:         Tps65233 I2C 寄存器写函数
  Description:      Tps65233 I2C 寄存器写函数
  Input:                eTunerIndex,  pRegAddress(寄存器地址指针), pDatabuf(写入数据指针)
  Output:           NULL
  Return:           成功返回TRUE,否则返回FALSE
*************************************************/
static BOOL TPS65233_iWriteReg(CSHDITunerIndex eTunerIndex, BYTE *pucRegAddress, BYTE *pucDatabuf)
{
    BOOL bRet = TRUE;
    HWI2C_PORT ehwI2c_port;

    ehwI2c_port = _get_I2C_port((MS_U8)eTunerIndex);

    do
    {
        if ((NULL == pucRegAddress) || (NULL == pucDatabuf))
        {
            HB_printf("[%s]Bad parameter 'pucRegAddress' or 'pucDatabuf'!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }
        
        if(!MDrv_IIC_WriteBytes(ehwI2c_port, g_ucDevSlaveID[eTunerIndex], 1, pucRegAddress, 1, pucDatabuf))
        {
            HB_printf("[%s]TPS65233_IIC_Write error!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }
    }
    while (0);

    return bRet;
}

/*************************************************
  Function:         Tps65233 I2C 位写入
  Description:      Tps65233 I2C 位写入
  Input:                eTunerIndex,  ucRegAddress(寄存器地址), ucData(写入数据), ucMask(写入位掩码)
  Output:           NULL
  Return:           成功返回TRUE,否则返回FALSE
*************************************************/
static BOOL TPS65233_iWriteRegBits(CSHDITunerIndex eTunerIndex, BYTE ucRegAddress, BYTE ucData, BYTE ucMask)
{
    BOOL bRet = TRUE;
    BYTE ucReadData = 0;

    do
    {
        if (!TPS65233_iReadReg(eTunerIndex, &ucRegAddress, &ucReadData))
        {
            bRet = FALSE;
            break;
        }

        ucReadData = (ucReadData & ucMask) | ucData;
        if (!TPS65233_iWriteReg(eTunerIndex, &ucRegAddress, &ucReadData))
        {
            bRet = FALSE;
            break;
        }

        if (!TPS65233_iReadReg(eTunerIndex, &ucRegAddress, &ucReadData))
        {
            bRet = FALSE;
            break;
        }

    }
    while (0);

    return bRet;
}

/*************************************************
  Function:         设置Tps65233 I2C控制使能函数
  Description:      设置Tps65233 I2C 控制是否使能
  Input:            eTunerIndex,  eI2CContrlMode(I2C使能枚举变量)
  Output:           NULL
  Return:           成功返回TRUE,否则返回FALSE
*************************************************/
static BOOL TPS65233_iSetI2CControlMode(CSHDITunerIndex eTunerIndex, TPS65233_I2CContrlMode_E eI2CContrlMode)
{
    BOOL bRet = TRUE;
    BYTE ucData = 0;

    do
    {
        if (eI2CContrlMode < EM_I2C_DISABLE || eI2CContrlMode > EM_I2C_ENABLE)
        {
            HB_printf("[%s]Bad parameter!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }

        //Register1:bit7: I2C control mode(I2C_CON) set bit
        ucData = (BYTE)((eI2CContrlMode & 0x01) << 7);
        if (!TPS65233_iWriteRegBits(eTunerIndex, TPS65233_CONTROL_REGISTER_1, ucData, TPS65233_I2C_CON_MASK))
        {
            HB_printf("[%s]Write Reg Bit Error!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }
    }
    while (0);

    return bRet;
}

/*************************************************
  Function:         设置 current limit(电流限制)
  Description:      设置 current limit(电流限制)
  Input:                eTunerIndex, eCurrentLimit(电流限制设置值)
  Output:           NULL
  Return:           成功返回TRUE,否则返回FALSE
*************************************************/
static BOOL TPS65233_iCurrentLimit(CSHDITunerIndex eTunerIndex, TPS65233_CurrentLimit_E eCurrentLimit)
{
    BOOL bRet = TRUE;
    BYTE ucData = 0;

    do
    {
        if ((eCurrentLimit < EM_CURRENT_LIMIT_400MA) || (eCurrentLimit > EM_CURRENT_LIMIT_1000MA))
        {
            HB_printf("[%s]Bad Parameter 'eCurrentLimit'!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }

        //Register2:bit2~1: the current limit(CL1&CL0) set bits
        ucData = (BYTE)((eCurrentLimit & 0x03) << 1);
        if (!TPS65233_iWriteRegBits(eTunerIndex, TPS65233_CONTROL_REGISTER_2, ucData, TPS65233_CL1_CL0_MASK))
        {
            bRet = FALSE;
            break;
        }
    }
    while (0);

    return bRet;
}

/*************************************************
  Function:         设置 current limit 的控制者
  Description:      deside what is the controller of current limit
  Input:                eTunerIndex, eCLController(电流限制控制者决定变量)
  Output:           NULL
  Return:           成功返回TRUE,否则返回FALSE
*************************************************/
static BOOL TPS65233_iSetCurrentControlMode(CSHDITunerIndex eTunerIndex, TPS65233_CLController_E eCLController)
{
    BOOL bRet = TRUE;
    BYTE ucData = 0;

    do
    {
        if ((eCLController < EM_CURRENT_LIMIT_SET_BY_REGISTER) || (eCLController > EM_CURRENT_LIMIT_SET_BY_EXTERNAL_RESISTOR))
        {
            HB_printf("[%s]Bad Parameter 'eCLController'!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }

        //Register2:bit0: the current limit control mode(CL_EXT) set bit
        ucData = (BYTE)(eCLController & 0x01);
        if (!TPS65233_iWriteRegBits(eTunerIndex, TPS65233_CONTROL_REGISTER_2, ucData, TPS65233_CL_EXT_MASK))
        {
            bRet = FALSE;
            break;
        }
    }
    while (0);

    return bRet;
}

/*************************************************
  Function:         设置 TPS65233 输出电压
  Description:      设置 TPS65233 输出电压
  Input:                eTunerIndex, eLNBPower(LNB芯片输出电压设置)
  Output:           NULL
  Return:           成功返回TRUE,否则返回FALSE
*************************************************/
BOOL TPS65233_SetLNBPower(CSHDITunerIndex eTunerIndex, TPS65233_LNBPower_E eLNBPower)
{
    BOOL bRet = TRUE;
    BYTE ucData = 0;

    do
    {
        if (eTunerIndex < Tuner_0 || eTunerIndex >= MAX_TUNER_NUM)
        {
            HB_printf("[%s]Bad Parameter 'eTunerIndex'!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }

        if ((eLNBPower < EM_TPS65233_LNB_DISABLE) || (eLNBPower > EM_TPS65233_LNB_V19_8))
        {
            HB_printf("[%s]Bad Parameter 'eLNBPower'!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }

        //Register1:bit3~0: the LNB power set bits
        ucData = (BYTE)(eLNBPower & 0x0f);
        if (!TPS65233_iWriteRegBits(eTunerIndex, TPS65233_CONTROL_REGISTER_1, ucData, TPS65233_LNB_POWER_SET_MASK))
        {
            bRet = FALSE;
            break;
        }
    }
    while (0);

    return bRet;
}

/*************************************************
  Function:         Tps652333 Tone Gate 模式设置
  Description:      Tps652333 Tone Gate 模式设置
  Input:                eTunerIndex,eToneGateMode(TONE GATE 开关和TONE MODE 设置)
  Output:           NULL
  Return:           成功返回TRUE,否则返回FALSE
*************************************************/
BOOL TPS65233_SetToneGateMode(CSHDITunerIndex eTunerIndex, TPS65233_ToneGateMode_E eToneGateMode)
{
    BOOL bRet = TRUE;
    BYTE ucData = 0;

    do
    {
        if (eTunerIndex < Tuner_0 || eTunerIndex >= MAX_TUNER_NUM)
        {
            HB_printf("[%s]Bad Parameter 'eTunerIndex'!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }

        if ((eToneGateMode < EM_TONE_GATE_OFF) || (eToneGateMode > EM_INTERNAL_TONE_GATE_ON))
        {
            HB_printf("[%s]Bad Parameter 'eToneGateMode'!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }

        //Register1:bit5~4: the TGATE&TMODE set bits
        ucData = (BYTE)((eToneGateMode & 0x03) << 4);
        if (!TPS65233_iWriteRegBits(eTunerIndex, TPS65233_CONTROL_REGISTER_1, ucData, TPS65233_TGATE_TMODE_MASK))
        {
            bRet = FALSE;
            break;
        }
    }
    while (0);

    return bRet;
}

/*************************************************
  Function:         Tps652333 TONE Position设置
  Description:      Tps652333Tone Gate 模式设置
  Input:            eTunerIndex,eTonePosition
  Output:           NULL
  Return:           成功返回TRUE,否则返回FALSE
*************************************************/
BOOL TPS65233_SetTonePosition(CSHDITunerIndex eTunerIndex, TPS65233_TonePosition_E eTonePosition)
{
    BOOL bRet = TRUE;
    BYTE ucData = 0;

    do
    {
        if (eTunerIndex < Tuner_0 || eTunerIndex >= MAX_TUNER_NUM)
        {
            HB_printf("[%s]Bad Parameter 'eTunerIndex'!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }

        if ((eTonePosition < EM_TONE_ABOVE_VOUT) || (eTonePosition > EM_TONE_BELOW_VOUT))
        {
            HB_printf("[%s]Bad Parameter 'eTonePosition'!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }

        //Register2:bit4~3: the TONE_POS set bits
        ucData = (BYTE)((eTonePosition & 0x03) << 3);
        if (!TPS65233_iWriteRegBits(eTunerIndex, TPS65233_CONTROL_REGISTER_2, ucData, TPS65233_TONE_POS_MASK))
        {
            bRet = FALSE;
            break;
        }
    }
    while (0);

    return bRet;
}

/*************************************************
  Function:         读取 TPS65233 运行状态值
  Description:      读取 TPS65233 运行状态值
  Input:            eTunerIndex,psStatus(状态存储指针)
  Output:           NULL
  Return:           成功返回TRUE,否则返回FALSE
*************************************************/
BOOL TPS65233_ReadStatus(CSHDITunerIndex eTunerIndex, TPS65233_Status_E *psStatus)
{
    BYTE ucRegAddress = 0;
    BYTE ucReadData = 0;
    BOOL bRet = TRUE;

    do
    {
        if (eTunerIndex < Tuner_0 || eTunerIndex >= MAX_TUNER_NUM || NULL == psStatus)
        {
            HB_printf("[%s]Bad parameter 'eTunerIndex' or 'psStatus'!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }

        ucRegAddress = TPS65233_STATUS_REGISTER_1;
        if (!TPS65233_iReadReg(eTunerIndex, &ucRegAddress, &ucReadData))
        {
            bRet = FALSE;
            break;
        }

        psStatus->m_ucTemperature = ((ucReadData & 0x40) >> 6);
        psStatus->m_ucLDO_ON = ((ucReadData & 0x20) >> 5);
        psStatus->m_ucTSD = ((ucReadData & 0x08) >> 3);
        psStatus->m_ucOCP = ((ucReadData & 0x04) >> 2);
        psStatus->m_ucCableGood = ((ucReadData & 0x02) >> 1);
        psStatus->m_ucVoutGood = (ucReadData & 0x01);
    }
    while (0);

    return bRet;
}


/*************************************************
  Function:         Tps652333初始化
  Description:      初始化时设置Tps652333为使能I2C 控制
  Input:            eTunerIndex
  Output:           NULL
  Return:           初始化成功返回TRUE,否则返回FALSE
*************************************************/
BOOL TPS65233_Init(CSHDITunerIndex eTunerIndex)
{
    BOOL bRet = TRUE;

    do
    {
        if (eTunerIndex < Tuner_0 || eTunerIndex >= MAX_TUNER_NUM)
        {
            HB_printf("[%s]Bad Parameter 'eTunerIndex'!\r\n", __FUNCTION__);
            bRet = FALSE;
            break;
        }

        bRet = TPS65233_iSetI2CControlMode(eTunerIndex, EM_I2C_ENABLE);
        bRet &= TPS65233_iSetCurrentControlMode(eTunerIndex, EM_CURRENT_LIMIT_SET_BY_REGISTER);
        bRet &= TPS65233_iCurrentLimit(eTunerIndex, EM_CURRENT_LIMIT_750MA);
        bRet &= TPS65233_SetToneGateMode(eTunerIndex, EM_EXTERNAL_TONE_GATE_ON);
        bRet &= TPS65233_SetTonePosition(eTunerIndex, EM_TONE_IN_THE_MIDDLE_OF_VOUT);
    }
    while (0);

    return bRet;
}

MS_BOOL MDrv_Dish_Init(MS_U8 u8DishIndex,DISH_MS_INIT_PARAM* pParam)
{
    MS_BOOL bRet = TRUE;

    if(pParam->pstDemodtab== NULL)
        return FALSE;
    else
        DishInitParam[u8DishIndex].pstDemodtab = pParam->pstDemodtab;

    #if defined(DISH_IIC_PORT)
        if(DISH_IIC_PORT!=FRONTEND_TUNER_PORT)
        {
            printf("init DISH_IIC_PORT\n");
            MDrv_IIC_Init(DISH_IIC_PORT);
        }  
    #endif

    bRet = TPS65233_Init(Tuner_0);
	printf("init TPS65233_Init ok, %d\n", bRet);

    return bRet;
}

MS_BOOL MDrv_Dish_SetTone(MS_U8 u8DishIndex, EN_TONEBURST_TYPE enToneType)
{
    MS_BOOL bRet = TRUE;

    switch (enToneType)
    {
        case EN_TONEBURST_TYPE_0:
            //bRet = MDrv_DiSEqC_SetTone(FALSE);
            return  DishInitParam[u8DishIndex].pstDemodtab->DiSEqCSetTone(u8DishIndex,FALSE);
            break;

        case EN_TONEBURST_TYPE_1:
            //bRet = MDrv_DiSEqC_SetTone(TRUE);
            return  DishInitParam[u8DishIndex].pstDemodtab->DiSEqCSetTone(u8DishIndex,TRUE);
            break;

        default:
            HB_printf("----%s error type %d----", __FUNCTION__, enToneType);
            break;
    }

    return bRet;
}

MS_BOOL MDrv_Dish_SetLNBPower(MS_U8 u8DishIndex, DISH_LNBPWR_TYPE enLNBPwr)
{
    MS_BOOL bRet = TRUE;

    switch (enLNBPwr)
    {
        case EN_LNBPWR_TYPE_OFF:
            bRet = TPS65233_SetLNBPower(Tuner_0, EM_TPS65233_LNB_DISABLE);
            break;

        case EN_LNBPWR_TYPE_13V:
        case EN_LNBPWR_TYPE_14V:
            bRet = TPS65233_SetLNBPower(Tuner_0, EM_TPS65233_LNB_V13_8);
            break;

        case EN_LNBPWR_TYPE_18V:
        case EN_LNBPWR_TYPE_19V:
            bRet = TPS65233_SetLNBPower(Tuner_0, EM_TPS65233_LNB_V19_2);
            break;

        default:
            HB_printf("----%s error type %d ----", __FUNCTION__, enLNBPwr);
            break;
    }

    return bRet;
}

MS_BOOL MDrv_Dish_Set22k(MS_U8 u8DishIndex, DISH_LNB22K_TYPE enLNB22k)
{
    MS_BOOL bRet = TRUE;

    switch (enLNB22k)
    {
        case EN_LNB22K_TYPE_OFF:
            //bRet = MDrv_DiSEqC_Set22kOnOff(FALSE);
            DishInitParam[u8DishIndex].pstDemodtab->DiSEqCSet22kOnOff(u8DishIndex, FALSE);

            break;

        case EN_LNB22K_TYPE_ON:
            //bRet = MDrv_DiSEqC_Set22kOnOff(TRUE);
            DishInitParam[u8DishIndex].pstDemodtab->DiSEqCSet22kOnOff(u8DishIndex, TRUE);

            break;

        default:
            HB_printf("----%s error type %d----", __FUNCTION__, enLNB22k);
            break;
    }

    return bRet;
}

MS_BOOL MDrv_Dish_SendCmd(MS_U8 u8DishIndex, MS_U8* pCmd, MS_U8 u8CmdSize)
{
    //return MDrv_DiSEqC_SendCmd(pCmd,u8CmdSize);
    return DishInitParam[u8DishIndex].pstDemodtab->DiSEqC_SendCmd(u8DishIndex, pCmd,u8CmdSize);

}

MS_BOOL MDrv_Dish_IsOverCurrent(MS_U8 u8DishIndex)
{
    /*mdrv_gpio_set_input(GPIO_ANT_OVERCURRENT);
    return IS_ANT_OVERCURRENT();*/
    return FALSE;
}

MS_BOOL MDrv_Dish_SetCable(MS_U8 u8DishIndex, EN_CABLE_SELECT eCableIndex)
{       
   return FALSE;
}

DISHTAB_ENTRY(dish_entry_DISH_TPS65233, "DISH_TPS65233", DISH_TPS65233,
              MDrv_Dish_Init,
              MDrv_Dish_SetTone,
              MDrv_Dish_SetLNBPower,
              MDrv_Dish_Set22k,
              MDrv_Dish_SendCmd,
              MDrv_Dish_IsOverCurrent,
              MDrv_Dish_SetCable
             );
#endif 


