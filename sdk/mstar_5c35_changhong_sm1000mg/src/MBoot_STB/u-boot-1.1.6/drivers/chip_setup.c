/*
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2000 MIPS Technologies, Inc.  All rights reserved.
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */
#include "config.h"
#include "chip_setup.h"

#ifdef __MIPS__ //The functions in this file are only for MIPS not for AeonR2.

#define _BITMASK(loc_msb, loc_lsb) ((1U << (loc_msb)) - (1U << (loc_lsb)) + (1U << (loc_msb)))
#define BITMASK(x) _BITMASK(1?x, 0?x)
#define _BIT(x)                      (1<<(x))


#define BIT0							0x01
#define BIT1							0x02
#define BIT2							0x04
#define BIT3							0x08
#define BIT4							0x10
#define BIT5							0x20
#define BIT6							0x40
#define BIT7							0x80

#define MIU1_BUS_BASE				(0x60000000)
#define HIGH_MEM_BUS_BASE				(0x50000000)
#define HIGH_MEM_PHY_BASE				(0x10000000)
#define HIGH_MEM_MAX_BUS_ADDR           (0x80000000)

#define MIU_INTERVAL   0x10000000
#define MIU_DRAM_LEN0  0x10000000
#define MIU_DRAM_LEN1  0x10000000

#define NULL 0
#define spin_lock_irq(x)
#define spin_unlock_irq(x)

#if 0
inline void* Chip_mphy_noncachevirt( unsigned long mphyaddr )
{
#if defined(CONFIG_TITANIA8) || defined(CONFIG_JANUS2)
    if( mphyaddr < (MIU_INTERVAL>>1)  )
    {
        return (void*)(mphyaddr|0xA0000000);
    }
    #if 0
    else if( mphyaddr < MIU_INTERVAL  )
    {
        return (void*)((mphyaddr -(MIU_INTERVAL>>1) ) |0xC0000000);
    }
    else if( mphyaddr < (MIU_INTERVAL +  (MIU_INTERVAL>>1)   ) )
    {
        return (void*)((mphyaddr -MIU_INTERVAL) |0xD0000000);
    }
    #endif
    else
    {
        return (void*)(NULL);
    }
#elif defined(CONFIG_TITANIA9) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1)
    if( mphyaddr < MIU_INTERVAL )
    {
        return (void*)(mphyaddr|0xA0000000);
    }
    #if 0
    else if( mphyaddr < MIU_INTERVAL  )
    {
        return (void*)((mphyaddr -MIU_INTERVAL ) |0xC0000000);
    }
    #endif
    else
    {
        return (void*)(NULL);
    }
#endif
        return (void*)(NULL);
}
#endif

inline unsigned long Chip_mphy_bus( unsigned long phyaddress )
{
#if defined(CONFIG_TITANIA8) || defined(CONFIG_JANUS2) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA12)
    if( phyaddress >= (MIU_INTERVAL<<1)  )
    {
        return -1;
    }
    #if 0
    else if( phyaddress >= MIU_INTERVAL  )
    {
        return (MIU1_BUS_BASE+phyaddress-MIU_INTERVAL);
    }
    else if( phyaddress >= HIGH_MEM_PHY_BASE  )
    {
        return ( HIGH_MEM_BUS_BASE+phyaddress-HIGH_MEM_PHY_BASE );
    }
    #endif
    else
    {
        return phyaddress;
    }
#elif defined(CONFIG_TITANIA9) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1)
    if( phyaddress >= (MIU_INTERVAL<<1)  )
    {
        return -1;
    }
    #if 0
    else if( phyaddress >= MIU_INTERVAL  )
    {
        return (MIU1_BUS_BASE+phyaddress-MIU_INTERVAL);
    }
    #endif
    else
    {
        return phyaddress;
    }
#endif
	return 0;
}

inline unsigned long Chip_bus_mphy( unsigned long busaddress )
{
#if defined(CONFIG_TITANIA8) || defined(CONFIG_JANUS2) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA12)
    if( busaddress >= HIGH_MEM_MAX_BUS_ADDR  )
    {
        return -1;
    }
    #if 0
    else if( busaddress >= MIU1_BUS_BASE  )
    {
        return ( MIU_INTERVAL+busaddress-MIU1_BUS_BASE);
    }
    else if( busaddress >= HIGH_MEM_BUS_BASE )
    {
        return (HIGH_MEM_PHY_BASE +busaddress- HIGH_MEM_BUS_BASE);
    }
    #endif
    else
    {
        return busaddress;
    }
#elif defined(CONFIG_TITANIA9) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1)
    if( busaddress >= HIGH_MEM_MAX_BUS_ADDR  )
    {
        return -1;
    }
    #if 0
    else if( busaddress >= MIU1_BUS_BASE  )
    {
        return ( MIU_INTERVAL+busaddress-MIU1_BUS_BASE);
    }
    #endif
    else
    {
        return busaddress;
    }
#endif
	return 0;
}


inline void _chip_flush_miu_pipe(void)
{
    static unsigned int u8_4Lines[1];
    volatile unsigned int *pu8;
    volatile unsigned int tmp ;
    // Transfer the memory to noncache memory
    pu8 = ((volatile unsigned int *)(((unsigned int)u8_4Lines) | 0xa0000000));
    pu8[0] = 1;
    //mb();
    asm volatile (
        "sync;"
    );
    // final read back
    tmp = pu8[0];
}


void Chip_L2_cache_wback_inv( unsigned long addr, unsigned long size)
{
#if defined(CONFIG_TITANIA8) || defined(CONFIG_JANUS2) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA12)
    volatile unsigned int temp=0;
    static unsigned int CacheLineSize=0;
    if( !CacheLineSize )
    {
        CacheLineSize=((*(volatile unsigned int *)(0xbf000000+0x203100))&(_BIT(4)|_BIT(5)))>>4;  // H40 L2 cache line size 00: 32-bytes    01: 64-bytes    1x: 128-bytes
        //printf("Read_Memory_Range: %lx \n"  , CacheLineSize);
        switch(CacheLineSize  )
        {
         case 0:
            CacheLineSize=~(32-1);
            break;
         case 1:
            CacheLineSize=~(64-1);
            break;
         case 2:
         case 3:
            CacheLineSize=~(128-1);
            break;
        default:
            //printf("Chip_L2_cache_wback_inv Failed: Unknown cache line size:%x \n"  , CacheLineSize);
            return;
            break;
        }
    }

    if( addr > 0x40000000)
    {
        return;
    }

    // TODO: U01 only
    size = 0x20000;

    // L2 disabled
    if( (*(volatile unsigned int *)(0xbf000000+0x203100)) & _BIT(1) )  // H40
    {
        // flush pipe
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) =  0x2; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x2; // H41 all
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(9);//pipe
        }while(temp == 0);
        // ^JL^
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        spin_unlock_irq(&l2prefetch_lock);
    }
    else if( size >= 0x20000 )
    {
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = 0x4; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x4; // H41 all
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(10);//all
        }while(temp == 0);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = 0; // H41
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        // invalid all
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4); // H41
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(8);//all
        }while(temp == 0);
        // ^JL^ necessary read
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        spin_unlock_irq(&l2prefetch_lock);
    }
    else // range flush
    {
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203110) = (addr&CacheLineSize) & 0xFFFF; // H44 range start Lo
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203114) = ((addr>>16)) & 0xFFFF; // H44 range start hi
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203118) = ((addr+size)&CacheLineSize) & 0xFFFF; // H44 range start Lo
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x20311C) = ((addr+size)>>16) & 0xFFFF; // H44 range start hi
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = 0x5; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x5; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(11);//all
        }while(temp == 0);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        // TODO: add invalid range
        spin_unlock_irq(&l2prefetch_lock);
    }

#elif defined(CONFIG_TITANIA9) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1)

    volatile unsigned int temp=0;
    if( (*(volatile unsigned int *)(0xbf000000+0x203100)) & _BIT(1) )  // H40
    {
    // L2 disabled
        // flush pipe
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) =  0x2; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x2; // H41 all

        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(9);//pipe
        }while(temp == 0);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        spin_unlock_irq(&l2prefetch_lock);
    }
    else
    {
    // L2 enabled
        // flush all and keep valid
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) =  0x2; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x2; // H41 all
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(9);//pipe
        }while(temp == 0);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) =  0x0; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x0; // H41 all
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(8);//all
        }while(temp == 0);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        spin_unlock_irq(&l2prefetch_lock);
    }
#elif defined(CONFIG_KAISERIN)

        unsigned int    dwTempData = 0;
        static unsigned int CacheLineSize=0;

        if(((*(volatile unsigned int *)(0xbf203100)) & BIT1) != 0)
        {
            //L2 Cache Disable
            spin_lock_irq(&l2prefetch_lock);

            //Flush Pipe Data
            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData &= ~(0x0F);
            dwTempData |= 0x02;                                     //Set cache_op_mode
            *(volatile unsigned int *)(0xbf203104) = dwTempData;

            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData &= ~_BIT(4);
            *(volatile unsigned int *)(0xbf203104) = dwTempData;

            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData |= _BIT(4);
            *(volatile unsigned int *)(0xbf203104) = dwTempData;

            do
            {
                dwTempData = *(volatile unsigned int *)(0xbf203104);
                dwTempData &= _BIT(9);  //Check Status of Flush Pipe Finish

            } while(dwTempData == 0);

            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData &= ~(0x0F);
            dwTempData &= ~_BIT(4);
            *(volatile unsigned int *)(0xbf203104) = dwTempData;

            spin_unlock_irq(&l2prefetch_lock);

        }
        else //if(((*(volatile unsigned int *)(0xbf203100)) & BIT1) == 0)
        {
            if( !CacheLineSize )
            {
                CacheLineSize=((*(volatile unsigned int *)(0xbf000000+0x203100))&(_BIT(4)|_BIT(5)))>>4;  // H40 L2 cache line size 00: 32-bytes    01: 64-bytes    1x: 128-bytes
                //printf("Read_Memory_Range: %lx \n"  , CacheLineSize);
                switch(CacheLineSize  )
                {
                 case 0:
                    CacheLineSize=~(32-1);
                    break;
                 case 1:
                    CacheLineSize=~(64-1);
                    break;
                 case 2:
                 case 3:
                    CacheLineSize=~(128-1);
                    break;
                default:
                    //printf("Chip_L2_cache_wback_inv Failed: Unknown cache line size:%x \n"  , CacheLineSize);
                    return;
                    break;
                }
            }


            spin_lock_irq(&l2prefetch_lock);

            dwTempData = *(volatile unsigned int *)(0xbf201600);    //Dummy Read

            //Flush All and Invalid
            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData &= ~(0x0F);
            dwTempData |= 0x04;                                     //Set cache_op_mode
            *(volatile unsigned int *)(0xbf203104) = dwTempData;

            dwTempData = *(volatile unsigned int *)(0xbf201600);    //Dummy Read

            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData &= ~_BIT(4);
            *(volatile unsigned int *)(0xbf203104) = dwTempData;

            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData |= _BIT(4);
            *(volatile unsigned int *)(0xbf203104) = dwTempData;

            do
            {
                dwTempData = *(volatile unsigned int *)(0xbf203104);
                dwTempData &= _BIT(10);  //Check Status of Flush All Finish

            } while(dwTempData == 0);

            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData &= ~(0x0F);
            dwTempData &= ~_BIT(4);
            *(volatile unsigned int *)(0xbf203104) = dwTempData;

            spin_unlock_irq(&l2prefetch_lock);
        }

#endif

    // flush MIU pipe
    _chip_flush_miu_pipe();
}

void Chip_L2_cache_wback( unsigned long addr, unsigned long size)
{
#if defined(CONFIG_TITANIA8) || defined(CONFIG_JANUS2) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA12)
    volatile unsigned int temp=0;
    static unsigned int CacheLineSize=0;
    if( !CacheLineSize )
    {
        CacheLineSize=((*(volatile unsigned int *)(0xbf000000+0x203100))&(_BIT(4)|_BIT(5)))>>4;  // H40 L2 cache line size 00: 32-bytes    01: 64-bytes    1x: 128-bytes
        //printf("Read_Memory_Range: %lx \n"  , CacheLineSize);
        switch(CacheLineSize  )
        {
         case 0:
            CacheLineSize=~(32-1);
            break;
         case 1:
            CacheLineSize=~(64-1);
            break;
         case 2:
         case 3:
            CacheLineSize=~(128-1);
            break;
        default:
            //printf("Chip_L2_cache_wback Failed: Unknown cache line size:%x \n"  , CacheLineSize);
            return;
            break;
        }
    }

    if( addr > 0x40000000)
    {
        return;
    }

    // L2 disabled
    if( (*(volatile unsigned int *)(0xbf000000+0x203100)) & _BIT(1) )  // H40
    {
        // flush pipe
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) =  0x2; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x2; // H41 all
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(9);//pipe
        }while(temp == 0);
        // ^JL^
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        spin_unlock_irq(&l2prefetch_lock);
    }
    else if( size >= 0x20000 )
    {
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) =  0x3; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x3; // H41 all
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(10);//all
        }while(temp == 0);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        spin_unlock_irq(&l2prefetch_lock);
    }
    else // range flush
    {
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203110) = (addr&CacheLineSize) & 0xFFFF; // H44 range start Lo
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203114) = ((addr>>16)) & 0xFFFF; // H44 range start hi
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203118) = ((addr+size)&CacheLineSize) & 0xFFFF; // H44 range start Lo
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x20311C) = ((addr+size)>>16) & 0xFFFF; // H44 range start hi
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = 0x5; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x5; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(11);//all
        }while(temp == 0);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        spin_unlock_irq(&l2prefetch_lock);
    }
#elif defined(CONFIG_TITANIA9) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1)

    volatile unsigned int temp=0;
    if( (*(volatile unsigned int *)(0xbf000000+0x203100)) & _BIT(1) )  // H40
    {
    // L2 disabled
        // flush pipe
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) =  0x2; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x2; // H41 all

        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(9);//pipe
        }while(temp == 0);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        spin_unlock_irq(&l2prefetch_lock);
    }
    else
    {
    // L2 enabled
        // flush all and keep valid
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) =  0x3; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x3; // H41 all
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(10);//all
        }while(temp == 0);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        spin_unlock_irq(&l2prefetch_lock);
    }

#elif defined(CONFIG_KAISERIN)

        unsigned int    dwTempData = 0;
        static unsigned int CacheLineSize=0;

        if(((*(volatile unsigned int *)(0xbf203100)) & BIT1) != 0)
        {
            //L2 Cache Disable
            spin_lock_irq(&l2prefetch_lock);

            //Flush Pipe Data
            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData &= ~(0x0F);
            dwTempData |= 0x02;       //Flush Pipe Data
            *(volatile unsigned int *)(0xbf203104) = dwTempData;

            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData &= ~_BIT(4);
            *(volatile unsigned int *)(0xbf203104) = dwTempData;

            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData |= _BIT(4);
            *(volatile unsigned int *)(0xbf203104) = dwTempData;

            do
            {
                dwTempData = *(volatile unsigned int *)(0xbf203104);
                dwTempData &= _BIT(9);  //Check Status of Flush Pipe Finish

            } while(dwTempData == 0);

            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData &= ~(0x0F);
            dwTempData &= ~_BIT(4);
            *(volatile unsigned int *)(0xbf203104) = dwTempData;

            spin_unlock_irq(&l2prefetch_lock);

        }
        else //if(((*(volatile unsigned int *)(0xbf203100)) & BIT1) == 0)
        {
            if( !CacheLineSize )
            {
                CacheLineSize=((*(volatile unsigned int *)(0xbf000000+0x203100))&(_BIT(4)|_BIT(5)))>>4;  // H40 L2 cache line size 00: 32-bytes    01: 64-bytes    1x: 128-bytes
                //printf("Read_Memory_Range: %lx \n"  , CacheLineSize);
                switch(CacheLineSize  )
                {
                 case 0:
                    CacheLineSize=~(32-1);
                    break;
                 case 1:
                    CacheLineSize=~(64-1);
                    break;
                 case 2:
                 case 3:
                    CacheLineSize=~(128-1);
                    break;
                default:
                    //printf("Chip_L2_cache_wback Failed: Unknown cache line size:%x \n"  , CacheLineSize);
                    return;
                    break;
                }
            }


            //printf("Chip_L2_cache_wback \n");

            spin_lock_irq(&l2prefetch_lock);

            //Flush All and Keep Valid
            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData &= ~(0x0F);
            dwTempData |= 0x03;     //Flush All and Keep Valid
            dwTempData &= ~_BIT(4);
            *(volatile unsigned int *)(0xbf203104) = dwTempData;


            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData |= 0x03;     //Flush All and Keep Valid
            dwTempData |= _BIT(4);
            *(volatile unsigned int *)(0xbf203104) = dwTempData;
            //printf("*(volatile unsigned int *)(0xbf203104) = 0x%x \n",*(volatile unsigned int *)(0xbf203104));

            do
            {
                dwTempData = *(volatile unsigned int *)(0xbf203104);
                dwTempData &= _BIT(10); //Check Status of Flush All Finish

            } while(dwTempData == 0);

            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData &= ~(0x0F);
            dwTempData &= ~_BIT(4);
            *(volatile unsigned int *)(0xbf203104) = dwTempData;

            spin_unlock_irq(&l2prefetch_lock);
        }

#endif

    // flush MIU pipe
    _chip_flush_miu_pipe();

}

void Chip_L2_cache_inv( unsigned long addr, unsigned long size)
{
#if defined(CONFIG_TITANIA8) || defined(CONFIG_JANUS2) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA12)
    volatile unsigned int temp=0;
    static unsigned int CacheLineSize=0;
    if( !CacheLineSize )
    {
        CacheLineSize=((*(volatile unsigned int *)(0xbf000000+0x203100))&(_BIT(4)|_BIT(5)))>>4;  // H40 L2 cache line size 00: 32-bytes    01: 64-bytes    1x: 128-bytes
        //printf("Read_Memory_Range: %lx \n"  , CacheLineSize);
        switch(CacheLineSize  )
        {
         case 0:
            CacheLineSize=~(32-1);
            break;
         case 1:
            CacheLineSize=~(64-1);
            break;
         case 2:
         case 3:
            CacheLineSize=~(128-1);
            break;
        default:
            //printf("Chip_L2_cache_wback_inv Failed: Unknown cache line size:%x \n"  , CacheLineSize);
            return;
            break;
        }
    }

    if( addr > 0x40000000)
    {
        return;
    }

    // TODO: U01 only
    size = 0x20000;

    // L2 disabled
    if( (*(volatile unsigned int *)(0xbf000000+0x203100)) & _BIT(1) )  // H40
    {
        // flush pipe
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) =  0x2; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x2; // H41 all
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(9);//pipe
        }while(temp == 0);
        // ^JL^
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        spin_unlock_irq(&l2prefetch_lock);
    }
    else if( size >= 0x20000 )
    {
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) =  0x3; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x3; // H41 all
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(10);//all
        }while(temp == 0);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = 0; // H41
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        // invalid all
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4); // H41
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(8);//all
        }while(temp == 0);
        // ^JL^ necessary read
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        spin_unlock_irq(&l2prefetch_lock);
    }
    else // range flush
    {
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203110) = (addr&CacheLineSize) & 0xFFFF; // H44 range start Lo
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203114) = ((addr>>16)) & 0xFFFF; // H44 range start hi
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203118) = ((addr+size)&CacheLineSize) & 0xFFFF; // H44 range start Lo
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x20311C) = ((addr+size)>>16) & 0xFFFF; // H44 range start hi
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        // TODO: add invalid range
        /*
        *(volatile unsigned int *)(0xbf000000+0x203104) = 0x5; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x5; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(11);//all
        }while(temp == 0);
        */
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        spin_unlock_irq(&l2prefetch_lock);
    }

#elif defined(CONFIG_TITANIA9) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1)

    volatile unsigned int temp=0;
    if( (*(volatile unsigned int *)(0xbf000000+0x203100)) & _BIT(1) )  // H40
    {
    // L2 disabled
        // flush pipe
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) =  0x2; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x2; // H41 all

        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(9);//pipe
        }while(temp == 0);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        spin_unlock_irq(&l2prefetch_lock);
    }
    else
    {
    // L2 enabled
        // flush all and keep valid
        spin_lock_irq(&l2prefetch_lock);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) =  0x2; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x2; // H41 all
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(9);//pipe
        }while(temp == 0);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) =  0x0; // H41 all
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        *(volatile unsigned int *)(0xbf000000+0x203104) = _BIT(4) | 0x0; // H41 all
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= _BIT(8);//all
        }while(temp == 0);
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        spin_unlock_irq(&l2prefetch_lock);
    }
#elif defined(CONFIG_KAISERIN)
    volatile unsigned int dwTempData=0;
    static unsigned int CacheLineSize=0;

    if( !CacheLineSize )
    {
        CacheLineSize=((*(volatile unsigned int *)(0xbf000000+0x203100))&(_BIT(4)|_BIT(5)))>>4;  // H40 L2 cache line size 00: 32-bytes    01: 64-bytes    1x: 128-bytes
        //printf("Read_Memory_Range: %lx \n"  , CacheLineSize);
        switch(CacheLineSize  )
        {
         case 0:
            CacheLineSize=~(32-1);
            break;
         case 1:
            CacheLineSize=~(64-1);
            break;
         case 2:
         case 3:
            CacheLineSize=~(128-1);
            break;
        default:
            //printf("Chip_L2_cache_wback_inv Failed: Unknown cache line size:%x \n"  , CacheLineSize);
            return;
            break;
        }
    }


    if(((*(volatile unsigned int *)(0xbf203100)) & BIT1) != 0)
    {
        //L2 Cache Disable
        spin_lock_irq(&l2prefetch_lock);

        //Flush Pipe Data
        dwTempData = *(volatile unsigned int *)(0xbf203104);
        dwTempData &= ~(0x0F);
        dwTempData |= 0x02;     //Flush Pipe Data
        *(volatile unsigned int *)(0xbf203104) = dwTempData;

        dwTempData = *(volatile unsigned int *)(0xbf203104);
        dwTempData &= ~_BIT(4);
        *(volatile unsigned int *)(0xbf203104) = dwTempData;

        dwTempData = *(volatile unsigned int *)(0xbf203104);
        dwTempData |= _BIT(4);
        *(volatile unsigned int *)(0xbf203104) = dwTempData;

        do
        {
            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData &= _BIT(9);  //Check Status of Flush Pipe Finish

        } while(dwTempData == 0);

        dwTempData = *(volatile unsigned int *)(0xbf203104);
        dwTempData &= ~(0x0F);
        dwTempData &= ~_BIT(4);
        *(volatile unsigned int *)(0xbf203104) = dwTempData;

        spin_unlock_irq(&l2prefetch_lock);

    }
    else //if(((*(volatile unsigned int *)(0xbf203100)) & BIT1) == 0)
    {
        spin_lock_irq(&l2prefetch_lock);

        //Flush All and Invalid
        dwTempData = *(volatile unsigned int *)(0xbf203104);
        dwTempData &= ~(0x0F);
        dwTempData |= 0x04;     //Flush All and Invalid
        *(volatile unsigned int *)(0xbf203104) = dwTempData;

        dwTempData = *(volatile unsigned int *)(0xbf203104);
        dwTempData &= ~_BIT(4);
        *(volatile unsigned int *)(0xbf203104) = dwTempData;

        dwTempData = *(volatile unsigned int *)(0xbf203104);
        dwTempData |= _BIT(4);
        *(volatile unsigned int *)(0xbf203104) = dwTempData;

        do
        {
            dwTempData = *(volatile unsigned int *)(0xbf203104);
            dwTempData &= _BIT(10);  //Check Status of Flush All Finish

        } while(dwTempData == 0);

        dwTempData = *(volatile unsigned int *)(0xbf203104);
        dwTempData &= ~(0x0F);
        dwTempData &= ~_BIT(4);
        *(volatile unsigned int *)(0xbf203104) = dwTempData;

        spin_unlock_irq(&l2prefetch_lock);
    }
#endif

    // flush MIU pipe
    _chip_flush_miu_pipe();

    return;
}

void Chip_Flush_Memory_Range(unsigned long pAddress , unsigned long  size)
{
#if defined(CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1)
    volatile unsigned int temp=0;
    if( *(volatile unsigned int*)(0xbf000000+0x203100) & _BIT(2)  )
    {
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        Chip_L2_cache_wback( pAddress , size );
    }
    else
    {
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        // flush MIU pipe
        _chip_flush_miu_pipe();
    }
#endif

#if defined(CONFIG_KAISERIN)
    volatile unsigned int temp=0;
     temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
    Chip_L2_cache_wback( pAddress , size );
#endif


}
/*
void Chip_Flush_Memory(void)
{
#if defined(CONFIG_TITANIA8) || defined(CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1)
    Chip_Flush_Memory_Range( 0 , 0xFFFFFFFF );
#endif
}
*/
void Chip_Read_Memory_Range(unsigned long pAddress , unsigned long  size)
{
#if defined(CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1)
    volatile unsigned int temp=0;
    if( *(volatile unsigned int*)(0xbf000000+0x203100) & _BIT(2)  )
    {
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        Chip_L2_cache_inv( pAddress , size );
    }
    else
    {
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        // flush MIU pipe
        _chip_flush_miu_pipe();
    }
#endif

#if defined(CONFIG_KAISERIN)
    volatile unsigned int temp=0;
    temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
    Chip_L2_cache_inv( pAddress , size );
#endif

}
/*
void Chip_Read_Memory(void)
{
#if defined(CONFIG_TITANIA8) || defined(CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1)
    Chip_Read_Memory_Range(0, 0x8000); //32KB
#endif
}
*/

#endif //__MIPS__
