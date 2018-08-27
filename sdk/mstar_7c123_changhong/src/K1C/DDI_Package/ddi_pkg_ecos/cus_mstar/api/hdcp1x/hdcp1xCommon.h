#ifndef DEF_HDCP1X_COMMON_H
#define DEF_HDCP1X_COMMON_H
/*********************************************************************/
/*                                                                                                                     */
/*                                                      Defines                                                    */
/*                                                                                                                     */
/*********************************************************************/
#define DEF_HDCP1X_BCAPS_SIZE           0x01
#define DEF_HDCP1X_BSTATUS_SIZE         0x02
#define DEF_HDCP1X_M0_SIZE              0x08
#define DEF_HDCP1X_RI_SIZE              0x02
#define DEF_HDCP1X_KSV_SIZE             0x05
#define DEF_HDCP1X_AN_SIZE              0x08
#define DEF_HDCP1X_AINFO_SIZE           0x01
#define DEF_HDCP1X_V_SIZE               0x14
#define DEF_HDCP1X_ENC_KEY_SIZE         304
#define DEF_HDCP1X_RX_KEY_SIZE          289
#define DEF_HDCP1X_VERIFY_RESULT_SIZE   4
#define DEF_HDCP1X_KSV_LENGTH_SIZE      4
#define DEF_HDCP1X_PORT_INDEX_SIZE      1
#define DEF_HDCP1X_CONTEXT_SIZE         4
#define DEF_SHA1_DIGEST_SIZE            20

#define DEF_HDCP1XTX_MAX_SUPPORT_DEV_NUM 2//127//16
#define DEF_HDCP1XTX_MAX_SUPPORT_DEV_DEPTH 7

/*********************************************************************/
/*                                                                                                                     */
/*                                                      Structure                                                  */
/*                                                                                                                     */
/*********************************************************************/
typedef unsigned char (*Fn_HDCP1X_AccessX74)(unsigned char u8PortIdx, unsigned char u8OffsetAddr, unsigned char u8OpCode, unsigned char *pu8RdBuf, unsigned short u16RdLen, unsigned char *pu8WRBuff, unsigned short u16WrLen);
typedef void (*Fn_HDCP1X_NotifyAuthStatus)(unsigned char u8PortIdx, unsigned char pu8Status);
typedef unsigned char (*Fn_HDCP1X_GetOutputMode)(unsigned char* pu8Mode);
typedef unsigned char (*Fn_HDCP1X_NotifyRepeaterInfo)(unsigned char u8PortIdx, unsigned char* pu8Bstatus, unsigned char* pu8KsvList, unsigned char* pu8Vprime, unsigned short pu16KsvCnt);
typedef unsigned char (*Fn_HDCP1X_CompareRi)(unsigned long u32Cmd, void* pBuf, unsigned long u32BufSize);

typedef enum
{
    EN_HDCP1X_OP_WRITE = 0x00,
    EN_HDCP1X_OP_READ = 0x01
} EN_HDCP1X_OPCODE;

typedef enum
{
    EN_HDCP1X_STATUS_IDLE          = 0x00,
    EN_HDCP1X_STATUS_PROCESSING    = 0x01,
    EN_HDCP1X_STATUS_FAIL          = 0x02,
    EN_HDCP1X_STATUS_SUCCESS       = 0x03,
} EN_HDCP1X_AUTH_STATUS;

typedef enum//typedef enum
{
    EN_HDCP1X_DDCADDR_BKSV_00        = 0x00,
    EN_HDCP1X_DDCADDR_RIPRIME_08     = 0x08,
    EN_HDCP1X_DDCADDR_PJPRIME_0A     = 0x0A,
    EN_HDCP1X_DDCADDR_AKSV_10        = 0x10,
    EN_HDCP1X_DDCADDR_AINFO_15       = 0x15,
    EN_HDCP1X_DDCADDR_AN_18          = 0x18,
    EN_HDCP1X_DDCADDR_VPRIMEH0_20    = 0x20,
    EN_HDCP1X_DDCADDR_BCAPS_40       = 0x40,
    EN_HDCP1X_DDCADDR_BSTATUS_41     = 0x41,
    EN_HDCP1X_DDCADDR_KSVFIFO_43     = 0x43,
    EN_HDCP1X_DDCADDR_DBG_C0         = 0xC0
} EN_HDCP1X_DDC_ADDR;

#endif //#ifndef DEF_HDCP1X_COMMON_H


