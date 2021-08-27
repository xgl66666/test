/* *************************** */
/*      Author:   mastar Allright Reserved */
/* *************************** */
#include "MsCommon.h"
#include "Board.h"
#include "drvFD650.h"
#include "string.h"

#define PKEY_VALUE_UP              0x5F
#define PKEY_VALUE_DOWN            0x57
#define PKEY_VALUE_LEFT            0x67
#define PKEY_VALUE_RIGHT           0x6F
#define PKEY_VALUE_MENU            0x47
#define PKEY_VALUE_SELECT          0x4F
#define PKEY_VALUE_EXIT            0x66


const led_bitmap BCD_decode_tab[LEDMAPNUM] =
{
    {'0', 0x3F}, {'1', 0x06}, {'2', 0x5B}, {'3', 0x4F},
    {'4', 0x66}, {'5', 0x6D}, {'6', 0x7D}, {'7', 0x07},
    {'8', 0x7F}, {'9', 0x6F}, {'a', 0x77}, {'A', 0x77},
    {'b', 0x7C}, {'B', 0x7C}, {'c', 0x58}, {'C', 0x39},
    {'d', 0x5E}, {'D', 0x5E}, {'e', 0x79}, {'E', 0x79},
    {'f', 0x71}, {'F', 0x71}, {'o', 0x5C}, {'t', 0x78},
    {'l', 0x30}, {'L', 0x38}, {'n', 0x37}, {'p', 0x73},
    {'P', 0x73}, {'O', 0x3F}, {'u', 0x1C}, {'U', 0x3E},
    {'-', 0x40}, {'I', 0x06}, {' ', 0x00},
};//BCD decode

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_S32 stFD650Mutex = -1;


/*************************************************************************************/
void Delay1(void)
{
    int iCount = 300;//100;
    while( iCount )
    {
        iCount--;
        if(iCount == 0)
            return;
    }
}

/****************************************************************
 *
 *  Function Name:FD650_Start
 *
 *  Description:
 *
 *  Parameter:
 *
 *  return：
****************************************************************/
void FD650_Start( void )
{
    FD650_SDA_SET;
    FD650_SDA_D_OUT;
    FD650_SCL_SET;
    FD650_SCL_D_OUT;
    DELAY1;
    FD650_SDA_CLR;
    DELAY1;
    FD650_SCL_CLR;
}
/****************************************************************
 *
 *  Function Name:FD650_Stop
 *
 *  Description:
 *
 *  Parameter:
 *
 *  return：
****************************************************************/
void FD650_Stop( void )
{
    FD650_SDA_CLR;
    FD650_SDA_D_OUT;
    DELAY1;
    FD650_SCL_SET;
    DELAY1;
    FD650_SDA_SET;
    DELAY1;
    FD650_SDA_D_IN;
}

/****************************************************************
 *
 *  Function Name:FD650_WrByte
 *
 *  Description: Write One Byte data
 *
 *  Parameter: data
 *
 *  return：
****************************************************************/
void FD650_WrByte( MS_U8 dat )
{
    MS_U8 i;
    FD650_SDA_D_OUT;
    for( i = 0; i != 8; i++ )
    {
        if( dat & 0x80 )
        {
            FD650_SDA_SET;
        }
        else
        {
            FD650_SDA_CLR;
        }
        DELAY1;
        FD650_SCL_SET;
        dat <<= 1;
        DELAY1;  // choose delay
        FD650_SCL_CLR;
    }
    FD650_SDA_D_IN;
    FD650_SDA_SET;
    DELAY1;
    FD650_SCL_SET;
    DELAY1;
    FD650_SCL_CLR;
}
/****************************************************************
 *
 *  Function Name:FD650_WrByte
 *
 *  Description: Read One Byte data
 *
 *  Parameter:
 *
 *  return：data
****************************************************************/
MS_U8 FD650_RdByte( void )
{
    MS_U8 dat,i;
    FD650_SDA_SET;
    FD650_SDA_D_IN;
    dat = 0;
    for( i = 0; i != 8; i++ )
    {
        DELAY1;  //choose delay
        FD650_SCL_SET;
        DELAY1;  // choose delay
        dat <<= 1;
        if( FD650_SDA_IN ) dat++;
        FD650_SCL_CLR;
    }
    FD650_SDA_SET;
    DELAY1;
    FD650_SCL_SET;
    DELAY1;
    FD650_SCL_CLR;
    return dat;
}

/****************************************FD650 function**********************/

/****************************************************************
 *
 *  Function Name:FD650_Write
 *
 *  Description:by cmd
 *
 *  Parameter: cmd FD650.H
 *
 *  return：
****************************************************************/
void FD650_Write( MS_U16 cmd )  //write cmd
{
    FD650_Start();
    FD650_WrByte(((MS_U8)(cmd>>7)&0x3E)|0x40);
    FD650_WrByte((MS_U8)cmd);
    FD650_Stop();

    return;
}

/****************************************************************
 *
 *  Function Name:FD650_Read
 *
 *  Description:read key value
 *
 *  Parameter:
 *
 *  return：key value
****************************************************************/
MS_U8 FD650_Read( void )
{
    MS_U8 keycode = 0;

    FD650_Start();
    FD650_WrByte((MS_U8)((((FD650_GET_KEY>>7)&0x3E)|0x01)|0x40));
    keycode=FD650_RdByte();
    FD650_Stop();

    if((keycode&0x00000040)==0)
        keycode = 0xff;

    return keycode;
}

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL  MDrv_FD650_Input_Key_Convert(MS_U8 u8key, MS_U32 *key)
{
    MS_U32 u32Key=0;
    //printf("**********key=%x**********\n",u8key);
    switch (u8key)
    {

        case PKEY_VALUE_UP:
            printf("**********UP*********\n");
            break;

        case PKEY_VALUE_DOWN:
            printf("**********DOWN*********\n");
            break;

        case PKEY_VALUE_LEFT:
            printf("**********LEFT*********\n");
            break;

        case PKEY_VALUE_RIGHT:
            printf("**********RIGHT*********\n");
            break;

        case PKEY_VALUE_SELECT:
            printf("**********SELECT********\n");
            break;

        case PKEY_VALUE_MENU:
            printf("**********MENU*********\n");
            break ;
            
        case PKEY_VALUE_EXIT:
            printf("**********EXIT*********\n");
            break;
            
        default:
            break;
    }

    *key=u32Key;

    return 1;

}

MS_BOOL MDrv_FD650_Init(void)
{

    if(stFD650Mutex < 0)
    {
        stFD650Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "MUTEX_FrontPnl", MSOS_PROCESS_PRIVATE);
    }
    FD650_Write(FD650_SYSON_8);

    return TRUE;
}

MS_BOOL MDrv_FD650_Term(void)
{

    if(FALSE == MsOS_ObtainMutex(stFD650Mutex, MSOS_WAIT_FOREVER))
    {
        return 0;
    }
    
    //FD650_Write(FD650_SYSOFF);
    FD650_Write(FD650_SLEEPON); 
    
    MsOS_ReleaseMutex(stFD650Mutex);

    return TRUE;
}

/****************************************************************
 *
 *  Function Name:Led_Get_Code
 *
 *  Description:转换字符为数码管的显示码，如遇到无法转换的字符返回0
 *
 *  Parameter: cTemp  待转换为显示码的字符
 *
 *  return：显示码值
****************************************************************/
static MS_U8 Led_Get_Code(char cTemp)
{
    MS_U8 i, bitmap = 0x00;

    for(i = 0; i < LEDMAPNUM; i++)
    {
        if(BCD_decode_tab[i].character == cTemp)
        {
            bitmap = BCD_decode_tab[i].bitmap;
            break;
        }
    }

    return bitmap;
}

//-------------------------------------------------------------------------------------------------
/// initialze Front Panel display control.
/// Function Name:MDrv_FrontPnl_DisPlay1
///
///  Description:简单的数码管显示应用示例
///
/// Parameter:  FPStr          显示的字符串；
///                        sec_flag      开启小数点标志位；
/// return：无

//-------------------------------------------------------------------------------------------------
static MS_U8 sec_vel=0;
static int data[4] = {0x00, 0x00, 0x00, 0x00};
static MS_U8 display_time =5;
void MDrv_FD650_Display(char *FPStr, MS_U8 sec_flag)
{
    int i, iLength;

    if(FALSE == MsOS_ObtainMutex(stFD650Mutex, MSOS_WAIT_FOREVER))
    {
        return;
    }

    if(!FPStr)
    {
        return;
    }

    first_time:
        
    iLength = strlen(FPStr);
    if(iLength <4)
    {
        data[iLength] =0;
        data[3] =0;
    }
    if(iLength > 4)
        iLength = 4;

    for(i = 0; i < iLength; i++)
    {
        data[i] = Led_Get_Code(FPStr[i]);
    }

    FD650_Write(FD650_SYSON_8);// 开启显示和键盘，8段显示方式
    //发显示数据
    FD650_Write( FD650_DIG0 | (MS_U8)data[0] ); //点亮第一个数码管

    if(sec_flag)
        FD650_Write( FD650_DIG1 | (MS_U8)data[1] | FD650_DOT ); //点亮第二个数码管
    else
        FD650_Write( FD650_DIG1 | (MS_U8)data[1] );

    FD650_Write( FD650_DIG2 | (MS_U8)data[2]|FD650_DOT); //点亮第三个数码管

    FD650_Write( FD650_DIG3 | (MS_U8)data[3] ); //点亮第四个数码管

    FD650_Write(FD650_SYSON_8);// 开启显示和键盘，8段显示方式
    sec_vel = (MS_U8)data[3];

    if(display_time != 0)
    {
        display_time -- ;
        goto first_time;
    }
    
    MsOS_ReleaseMutex(stFD650Mutex);

    return ;
}



MS_U8 MDrv_FD650_GetKeyValue(void)
{
    MS_U8 KeyValue = 0;

    if(FALSE == MsOS_ObtainMutex(stFD650Mutex, MSOS_WAIT_FOREVER))
    {
        printf("[%s][%d]Obtain fd650 Mutex ++++ fail!\n",__FUNCTION__,__LINE__);
        return 0xFF;
    }
    KeyValue =  FD650_Read();       //read key value   
    FD650_Write(FD650_SYSON_8);// 开启显示和键盘，8段显示方式
    if(FALSE == MsOS_ReleaseMutex(stFD650Mutex))
    {        
        printf("[%s][%d]Release fd650 Mutex ---- fail!\n",__FUNCTION__,__LINE__);
        return 0xFF;
    }

    return KeyValue;
}
void Mdrv_FD650_LockLed_OnOff(MS_U8 bLightOn)
{
    if(FALSE == MsOS_ObtainMutex(stFD650Mutex, MSOS_WAIT_FOREVER))
    {
        return ;
    }

    FD650_Write(FD650_SYSON_8);// 开启显示和键盘，8段显示方式
    if(bLightOn)
        FD650_Write( FD650_DIG3 | sec_vel | FD650_DOT ); //点亮第四个数码管
    else
        FD650_Write( FD650_DIG3 | sec_vel  ); //点亮第四个数码管
   
    FD650_Write(FD650_SYSON_8);// 开启显示和键盘，8段显示方式
    MsOS_ReleaseMutex(stFD650Mutex);

}

void Mdrv_FD650_PowerLed_OnOff(MS_U8 bLightOn)
{
    if(FALSE == MsOS_ObtainMutex(stFD650Mutex, MSOS_WAIT_FOREVER))
    {
        return ;
    }

    FD650_Write(FD650_SYSON_8);// 开启显示和键盘，8段显示方式
    if(bLightOn)
    FD650_Write( FD650_DIG2 | data[2] | FD650_DOT ); 
    else
    FD650_Write( FD650_DIG0 | data[0] | FD650_DOT ); 
    
    FD650_Write(FD650_SYSON_8);// 开启显示和键盘，8段显示方式
    MsOS_ReleaseMutex(stFD650Mutex);
}
