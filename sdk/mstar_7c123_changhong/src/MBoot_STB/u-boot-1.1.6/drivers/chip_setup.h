#ifndef _CHIP_SETUP_H_
#define _CHIP_SETUP_H_

//inline void* Chip_mphy_noncachevirt( unsigned long mphyaddr );
inline unsigned long Chip_mphy_bus( unsigned long phyaddress );
inline unsigned long Chip_bus_mphy( unsigned long busaddress );

extern void Chip_Flush_Memory(void);
extern void Chip_Read_Memory(void) ;
extern void Chip_Read_Memory_Range(unsigned long pAddress , unsigned long  size);
extern void Chip_Flush_Memory_Range(unsigned long pAddress , unsigned long  size);

#endif
