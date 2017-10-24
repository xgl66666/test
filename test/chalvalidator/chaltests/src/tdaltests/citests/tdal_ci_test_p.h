/*****************************************************************************
 *                         COPYRIGHT 2004 IWEDIA TECHNOLOGIES                *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_CI_TESTS
 *
 * FILE NAME: $URL  : $
 *            $Rev  : $
 *            $Date : $
 *
 * Description : Private Header of TDAL CI Tests
 * Target : All
 *
 ****************************************************************************/

#ifndef _TDAL_CI_TEST_P_H
#define _TDAL_CI_TEST_P_H

/*******************************************************/
/*              Defines                                */
/*******************************************************/
#define kTESTi_FAILED 0
#define kTESTi_PASSED 1

#define kTDAL_NB_FAMILY 2
#define NB_TEST_UNITAIRE 3
#define NB_TEST_TYPICAL_SCENARIO 2

/*******************************************************/
/*              Macros                                 */
/*******************************************************/

/*******************************************************/
/*              Typedefs                               */
/*******************************************************/
typedef uint32_t (*pFct) (void);

typedef struct{
    pFct  pFunc;
    char* test_name;
    char* test_result;
}t_testEntry;

typedef struct {
    t_testEntry *testList;
    int nbTest;
    char *Numero;
    char *libelle;
    char *liblong;
}t_testFamily;

typedef struct
{
    t_testFamily *test;
    int nombre;
    char *chaine1;
    char *chaine2;
}t_testModule;

#endif /* _TDAL_CI_TEST_P_H */

