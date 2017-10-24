/*
*   @brief
*  R U X I : redondant unary Xor integer
*
*  @type
*  tRUXI_aField a stream of 1024 bits (128 bytes) in a RAM buffer which are the image of a flash part
*  this perform and XOR across all bits
*  even parity return 0
*  odd parity return 1
*  far west bit in RAM is reserved to keep overflow (allways 1 in flash)
*   
*  @functions
*  # RUXIi_XorDigest
*  # RUXI_Check
*  # RUXI_reset
*  # RUXI_Setup
*  # RUXI_Set
*  # RUXI_Get
*/

typedef int bool;
typedef char tRUXI_aField[128];

/*
*   @brief
*   local routine to parse axField
*
*   @param[in] axField
*
*   @param[out] *pi8xFareast
*   index of far east not null byte
*
*   @ret_val
*   stored value (bool)
*   
*/
static bool RUXIi_XorDigest(tRUXI_aField axField, int *pi8xFareast)
{
    int i=0;
    char u8digest;
    
     u8digest= (char) axField[0] | 0x80; /* far west byte always 1 in flash, it is used for overflow in RAM */
   
    /* digesting across each non null byte */
    for (i=1; !!(axField[i]) && (i<sizeof(tRUXI_aField)); i++) 
    {
        u8digest ^= axField[i];
    }
    if (pi8xFareast) *pi8xFareast=i-1;
   
    /* digesting accross bit */
    /* (remark u8digest value will change according the char type is signed or not, but result (right bit) does not change) */
   u8digest ^= u8digest>>4;
   u8digest ^= u8digest>>2;
   u8digest ^= u8digest>>1;
   
   /* return last digested bit */
    return !!(u8digest&0x1);        
}

/*
*   @brief
*   check overflow axField
*
*   @param[in] axField
*
*   @ret_val
*   1 as long no need to erase target
*   0 else
*   
*/
bool RUXI_Check(tRUXI_aField axField)
{
    return !!(axField[0]&0x80);
}

/*
*   @brief
*   reset overflow flag
*
*   @param[in/out] axField
*
*  @remark
*  # call before programming flash
*/
void RUXI_reset(tRUXI_aField axField)
{
   axField[0] = (char) axField[0] | 0x80; /* reset the overflow flag */
}

/*
*   @brief
*   initialize axField
*
*   @param[out] axField
*
*   @param[in] xiInit
*/
void RUXI_Setup(tRUXI_aField axField, bool xiInit)
{
    int i;
    for (i=0; i<sizeof(tRUXI_aField); i++) 
		axField[i]= (char)0xFF;
    axField[i-1]^= (char)xiInit;
}

/*
*   @brief
*   codes xiNew into axField
*
*   @param[in/out] axField
*
*   @param[in] xiNew
*
*   @ret_val
*   1 as long no need to erase target
*   0 else (overflow)
*/
bool RUXI_Set(tRUXI_aField axField, bool xiNew)
{
   int iNdex;
   if (RUXI_Check(axField))
   {
      /* no over flow */
       if (RUXIi_XorDigest(axField, &iNdex) == xiNew) 
         return 1; /* No change */
      /* store xiNew by shifting left fareast bit so drop letf bit which is 1 */
      axField[iNdex] = (char) axField[iNdex] << 1; 
   }
   if (RUXI_Check(axField))
      return 1; /* Well set no over flow*/

   /* overflow */
   RUXI_Setup(axField, xiNew); /* drop the history */
   axField[0] ^= 0x80; /* keep the overflow flag */
   return 0;
}
/*
*   @brief
*   decodes axField
*
*   @param[in] axField
*
*   @ret_val
*   stored value (bool)
*   
*/
bool RUXI_Get(tRUXI_aField axField)
{
    return RUXIi_XorDigest(axField, 0);        
}
