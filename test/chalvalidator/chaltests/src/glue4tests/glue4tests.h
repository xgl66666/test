/*****************************************************************************
 * COPYRIGHT 2008 IWEDIA                                                     *
 *****************************************************************************
 *
 * MODULE NAME: Glue for CI+ stack
 *
 * FILE NAME: $URL:  $
 *            $Rev:  $
 *            $Date: $
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
#ifndef __GLUE4TESTS_H__
#define __GLUE4TESTS_H__

/*****************************************************************************
 *                                  DEFINES                                  *
 *****************************************************************************/

/*****************************************************************************
 *                                 TYPEDEFS                                  *
 *****************************************************************************/
typedef enum
{
    eTDAL_TESTCIPLUS_NVM=30,
    eTDAL_TESTCIPLUS_DESC
} tGLUE4TESTS_ModuleTraceID;

typedef enum
{
    NOT_SCRAMBLED_CHANNEL,
    SCRAMBLED_CHANNEL_CIV1_1,
    SCRAMBLED_CHANNEL_CIV1_2,
    SCRAMBLED_CHANNEL_CIPLUS,
    GLUE4TEST_CIplus_Cert_Test_Stream_Terpid32=0x100,
    GLUE4TEST_CIplus_Cert_Test_Stream_Terpid35,
    GLUE4TEST_CIplus_Cert_Test_Stream_Terpid38,
    GLUE4TEST_CIplus_Cert_Test_Stream_Terpid41,
    GLUE4TEST_CIplus_Cert_Test_Stream_Terpid44,
    GLUE4TEST_CIplus_Cert_Test_Stream_Terpid47,
    GLUE4TEST_CIplus_Cert_Test_Stream_Terpid50,
    GLUE4TEST_Stream_CIplus_1_CLSCRv2_0pid24,
    GLUE4TEST_Stream_CIplus_1_CLSCRv2_0pid32,
    GLUE4TEST_Stream_CIplus_1_CLSCRv2_0pid40,
    GLUE4TEST_Stream_CIplus_2_CCv2_0pid24,
    GLUE4TEST_Stream_CIplus_2_CCv2_0pid32,
    GLUE4TEST_Stream_CIplus_2_CCv2_0pid40,
    GLUE4TEST_Stream_CIplus_3_DOTAv2_0pid24,
    GLUE4TEST_Stream_CIplus_3_DOTAv2_0pid32,
    GLUE4TEST_Stream_CIplus_3_DOTAv2_0pid40,
    GLUE4TEST_Stream_VIACCESS_SWE_RF27_service1,
    GLUE4TEST_Stream_VIACCESS_SWE_RF27_service2,
    GLUE4TEST_Stream_VIACCESS_SWE_RF27_service3,
    GLUE4TEST_Stream_VIACCESS_SWE_RF27_service4,
    GLUE4TEST_Stream_VIACCESS_SWE_RF27_service5,
    GLUE4TEST_Stream_VIACCESS_SWE_RF27_service6,
    GLUE4TEST_Stream_VIACCESS_SWE_RF27_service7,
    GLUE4TEST_Stream_CONAX_FIN_RF53_service1,
    GLUE4TEST_Stream_CONAX_FIN_RF53_service2,
    GLUE4TEST_Stream_CONAX_FIN_RF53_service3,
    GLUE4TEST_Stream_CONAX_FIN_RF53_service4,
    GLUE4TEST_Stream_CONAX_FIN_RF53_service5,
    GLUE4TEST_Stream_CONAX_FIN_RF53_service6,
    GLUE4TEST_Stream_CONAX_FIN_RF53_service7,
    GLUE4TEST_Stream_CONAX_FIN_RF53_service8,
    GLUE4TEST_Stream_CONAX_FIN_RF46_service1,
    GLUE4TEST_Stream_CONAX_FIN_RF46_service2,
    GLUE4TEST_Stream_CONAX_FIN_RF46_service3,
    GLUE4TEST_Stream_CONAX_FIN_RF46_service4,
    GLUE4TEST_Stream_CONAX_FIN_RF46_service5,
    GLUE4TEST_Stream_CONAX_FIN_RF46_service6,
    GLUE4TEST_Stream_CONAX_FIN_RF46_service7,
    GLUE4TEST_Stream_CONAX_FIN_RF46_service8,
    GLUE4TEST_Stream_CONAX_FIN_RF46_service9,
    GLUE4TEST_Stream_CONAX_FIN_RF46_service10,
    GLUE4TEST_Stream_CONAX_FIN_RF46_service11,
    GLUE4TEST_Stream_CONAX_FIN_RF46_service12,
    GLUE4TEST_Stream_CONAX_FIN_RF46_service13,
    GLUE4TEST_Stream_Setanta_Prog_12866_Five,
    GLUE4TEST_Stream_Setanta_Prog_12928_FIVER,
    GLUE4TEST_Stream_Setanta_Prog_12992_Five_US,
    GLUE4TEST_Stream_Setanta_Prog_13120_QVC,
    GLUE4TEST_Stream_Setanta_Prog_14976_UKTV_STYLE,
    GLUE4TEST_Stream_Setanta_Prog_16096_SETANTA_SPORTS,
    GLUE4TEST_Stream_Setanta_Prog_20000_SETANTA_SPORTS,
} tGLUE4TESTS_Channel;

/*****************************************************************************
 *                       EXPORTED FUNCTIONS PROTOTYPES                       *
 *****************************************************************************/

void GLUE4TESTS_Init(void);
void GLUE4TESTS_Terminate(void);

void GLUE4TESTS_StartAV(tGLUE4TESTS_Channel ChannelId);
void GLUE4TESTS_StopAV(void);

int GLUE4TESTS_GetCurrentPMT(unsigned char * pPmt, unsigned short *pPmtSize);

#endif /* __GLUE4TESTS_H__ */
