//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Verify EMAC Compiler Switches
//need to open VER_EMAC_ENABLE for any verify
#define VER_EMAC_ENABLE                     0

#define VER_EMAC_IFCONFIG                   0
#define VER_EMAC_LOOPBACK                   0
#define VER_EMAC_INTERNAL_LOOPBACK          0

//need to open RX_CHKSUM_SUPPORT too
#define VER_EMAC_RX_CHKSUM                  0

//VER_EMAC_FORCE_TX can't be use in VER_EMAC_LOOPBACK
#define VER_EMAC_FORCE_TX                   0
#define VER_EMAC_DUMP_REG                   0
#define VER_EMAC_CAM                        0
#define VER_EMAC_THROUGHPUT                 0
#define VER_EMAC_DUMP_COUNTER               0
#define VER_EMAC_CHANGE_SPEED_TEST          0
#define VER_EMAC_MIU_WRITE_PROTECT          0
#define VER_EMAC_CRAZY_REG_ACESS            0
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define VER_EMAC_MIU_WRITE_PROTECT_UP_BASE      RX_BUFFER_BASE
#define VER_EMAC_MIU_WRITE_PROTECT_UP_BASE_LEN  0x10
#define VER_EMAC_MIU_WRITE_PROTECT_DUMP_SIZE    0x20
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WritMAC_Address(MS_U32 addr,MS_U8 m0,MS_U8 m1,MS_U8 m2,MS_U8 m3,MS_U8 m4,MS_U8 m5);
void WritCAM_Address(MS_U32 addr,MS_U8 m0,MS_U8 m1,MS_U8 m2,MS_U8 m3,MS_U8 m4,MS_U8 m5);
MS_U32 EMAC_CableConnection(void);
#if VER_EMAC_DUMP_COUNTER
MS_U32 ver_emac_counter[2]={0,0};//0:tx. 1:rx
MS_U32 ver_emac_system_time2=0;
void ver_emac_dump_cnt(void)
{
    MS_U32 current_time=MsOS_GetSystemTime();
    if(0 == ver_emac_system_time2)
    {
        ver_emac_system_time2=current_time;
    }
    printf("%d counters: tx=%d, rx=%d, CTL=%x, phy=%x\n",(current_time-ver_emac_system_time2)/60000,ver_emac_counter[0],ver_emac_counter[1],EMAC_ReadReg32(REG_ETH_CTL),phy_status);
}
#endif//VER_EMAC_DUMP_COUNTER

//------------------------------------------------------------------------------
#if VER_EMAC_LOOPBACK
MS_U8 _ver_emac_tx_buf_idx=0;
static MS_U8 _ver_emac_tx_buf[2][1600];
MS_U32 _ver_emac_tx_buf_ncache;
struct eth_drv_sgX _ver_emac_sg_list0;
MS_U32 _ver_emac_current_rx_desc[2];
void ver_emac_rev_mac_addr(MS_U8* pPacket)
{
    MS_U8 tmp,i;
    for(i=0;i<6;i++)
    {
        tmp=pPacket[i];
        pPacket[i]=pPacket[i+6];
        pPacket[i+6]=tmp;
    }
}
//----------------------------
MS_U8 ver_emac_chk_mac2me(MS_U8* pPacket)
{
    MS_U8 i;
    for(i=0;i<6;i++)
    {
        if(pPacket[i]!=MY_MAC[i])break;
    }
    return (6==i);
}
//----------------------------
void ver_emac_rx_chksum(MS_U8* pPacket,MS_U32* rx_desc)
{
    //4: IP_PROTOCOL
    //3: checksum ok
    //2: checksum ok
    //1: TCP_Protocol
    //0: UDP_PROTOCOL
    pPacket[22]=((rx_desc[1]>>18)&0x0f) | ((rx_desc[1]>>7)&0x10);
}
//----------------------------
void ver_emac_loopback(void)
{
    MS_U8* pPacket;
    MS_U32 timeout=100;
    while((EMAC_ReadReg32(REG_ETH_TCR)&0x7ff)!=0);
    {
        if(0 == timeout--)
        {
            printf("hw busy: drop this tx packet\n");
            return;
        }
    }
    _ver_emac_tx_buf_ncache = (MS_U32)KSEG02KSEG1(_ver_emac_tx_buf[_ver_emac_tx_buf_idx]);
    _ver_emac_tx_buf_idx+=1;
    if(_ver_emac_tx_buf_idx > 1)_ver_emac_tx_buf_idx=0;
    _ver_emac_tx_buf_ncache = ALIGN_16(_ver_emac_tx_buf_ncache);
    _ver_emac_sg_list0.buf = _ver_emac_tx_buf_ncache;
    pPacket=(MS_U8*)_ver_emac_tx_buf_ncache;
    EMAC_Sync();
    drvEMAC_RxEvent(&_ver_emac_sg_list0.len);
    drvEMAC_recv(&_ver_emac_sg_list0,1);

    if(ver_emac_chk_mac2me(pPacket)==0)return;

    ver_emac_rev_mac_addr(pPacket);

#if VER_EMAC_RX_CHKSUM
#if (RX_CHKSUM_SUPPORT==0)
    "ERROR: Please Enable RX_CHKSUM_SUPPORT."
#endif
    ver_emac_rx_chksum(pPacket,_ver_emac_current_rx_desc);
#endif

    EMAC_Sync();
#if VER_EMAC_THROUGHPUT
    //when test throughput, disable loopback tx
#else
    EMAC_WritReg32(REG_ETH_TAR,_ver_emac_tx_buf_ncache&0x0FFFFFFF);
    EMAC_WritReg32(REG_ETH_TCR,_ver_emac_sg_list0.len);
    #if VER_EMAC_DUMP_COUNTER
        ver_emac_counter[0]++;//0:tx. 1:rx
    #endif
#endif
}
#endif//#if VER_EMAC_LOOPBACK

//------------------------------------------------------------------------------
#if VER_EMAC_CAM
void ver_emac_cam_test(void)
{
    int i;
    MS_U32 word_ETH_CFG = 0x00000000;
    // Specific 2,3,4
    WritMAC_Address(0x000000A0,0x00,0x30,0x1B,0xBA,0x02,0x03);
    WritMAC_Address(0x000000A8,0x00,0x30,0x1B,0xBA,0x02,0x04);
    WritMAC_Address(0x000000B0,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7);//0x00,0x30,0x1B,0xBA,0x02,0x02);

    // CAM Address:

    for(i=0;i<64;i++)
    {
        // IMPORTANT: CAM memory must be sorted DECEND. Fill the non-data as 0x00
        // The hardware uses binary search.
        WritCAM_Address(0x00000200 + i * 8,0x00,0x00,0x00,0x00,0x00,0x7F - i);
    } // for

    word_ETH_CFG = EMAC_ReadReg32(REG_ETH_CFG);
    word_ETH_CFG &= 0xFFDFFFEF;//clear rx all
    word_ETH_CFG |= 0x00000200;
    //word_ETH_CFG |= 0x00002000;//for CAM refuse
    EMAC_WritReg32(REG_ETH_CFG,word_ETH_CFG);
    printf("enable cam test %x\n",EMAC_ReadReg32(REG_ETH_CFG));
}
#endif//VER_EMAC_CAM

//------------------------------------------------------------------------------
#if VER_EMAC_DUMP_REG
void ver_emac_dump_reg(void)
{
    MS_U32 i,j=0;
    MS_U32 tmp;
    printf("\ndump_reg:");
    //for(i=0;i<0x84;i+=4)
    for(i=0;i<0xB8;i+=4)
    {
        if((j++%8)==0)printf("\n");
        tmp=EMAC_ReadReg32(i);
        if(tmp)printf("%08x ",tmp);
        else   printf("__[%02x]__ ",i);
    }
}
#endif//VER_EMAC_DUMP_REG
//------------------------------------------------------------------------------
#if VER_EMAC_MIU_WRITE_PROTECT
void ver_emac_miu_write_protect(void)
{
        MS_U32 i;
        MS_U8* ptr=(MS_U8*)(VER_EMAC_MIU_WRITE_PROTECT_UP_BASE|0xa0000000);
        printf("miu protect test:\n(1)EMAC can write:\n");
        for(i=0;i<VER_EMAC_MIU_WRITE_PROTECT_UP_BASE_LEN;i++)
        {
            if(ptr[i]!=0x5a) printf("[%d]=%x ",i,ptr[i]);
        }
        printf("\n(2)EMAC can't write:\n");
        for(i=VER_EMAC_MIU_WRITE_PROTECT_UP_BASE_LEN;i<VER_EMAC_MIU_WRITE_PROTECT_DUMP_SIZE;i++)
        {
            if(ptr[i]!=0x5a) printf("[%d]=%x ",i,ptr[i]);
        }
        printf("\nend of miu protect.\n");
}
#endif//VER_EMAC_MIU_WRITE_PROTECT
//------------------------------------------------------------------------------
#if VER_EMAC_THROUGHPUT
MS_U32 ver_emac_byte_cnt[2];//0:tx, 1:rx
MS_U32 ver_emac_system_time=0;
#define VER_EMAC_CACULATE_TIME 30000
void ver_emac_caculate_through_put(void)
{
    MS_U32 current_time=MsOS_GetSystemTime();
    float tx_tput, rx_tput;
    if(ver_emac_system_time)
    {
        if((current_time-ver_emac_system_time)>VER_EMAC_CACULATE_TIME)
        {
            tx_tput=ver_emac_byte_cnt[0]/((current_time-ver_emac_system_time)/8000.0);
            tx_tput=tx_tput/(1024*1024);
            rx_tput=ver_emac_byte_cnt[1]/((current_time-ver_emac_system_time)/8000.0);
            rx_tput=rx_tput/(1024*1024);
            printf("ver_emac_caculate_through_put(ms, tx byte, rx): %d, tx=%d, rx=%d\n",current_time-ver_emac_system_time,ver_emac_byte_cnt[0],ver_emac_byte_cnt[1]);
            printf("ver_emac_caculate_through_put(Mbits/s): tx=%f, rx=%f\n",tx_tput,rx_tput);
            ver_emac_system_time=current_time;
            ver_emac_byte_cnt[0]=0;
            ver_emac_byte_cnt[1]=0;
        }
    }
    else
    {
        printf("ver_emac_caculate_through_put initial\n");
        ver_emac_system_time=MsOS_GetSystemTime();
        ver_emac_byte_cnt[0]=0;
        ver_emac_byte_cnt[1]=0;
    }
}
#endif//VER_EMAC_THROUGHPUT

//------------------------------------------------------------------------------
#if VER_EMAC_CHANGE_SPEED_TEST
MS_U32 speed_step=0;
MS_U32 speed_timeer=0;
void EMAC_ChangeSpeed(MS_U32 speed);
void ver_emac_chg_spd_test(void)
{
    MS_U32 speed_array[4]=
    {
        EMAC_SPEED_100|EMAC_DUPLEX_FULL,
        EMAC_SPEED_100|EMAC_DUPLEX_HALF,
        EMAC_SPEED_10|EMAC_DUPLEX_FULL,
        EMAC_SPEED_10|EMAC_DUPLEX_HALF
    };
    if(speed_timeer++>5)
    {
        speed_timeer=0;
        printf("change speed test to %d",speed_array[speed_step]);
        EMAC_ChangeSpeed(speed_array[speed_step++]);
        if(speed_step>3)speed_step=0;
    }
}
#endif//VER_EMAC_CHANGE_SPEED_TEST
//------------------------------------------------------------------------------
#if VER_EMAC_ENABLE
void ver_emac_in_EMAC_DRIVER(void)
{
#if VER_EMAC_CRAZY_REG_ACESS
while(1)
{
    MS_U32 val_ETH_CTL=EMAC_ReadReg32(REG_ETH_CTL);
    EMAC_WritReg32(REG_ETH_CTL,0x10|val_ETH_CTL);
    EMAC_WritReg32(REG_ETH_CTL,val_ETH_CTL);
    val_ETH_CTL=EMAC_ReadReg32(REG_ETH_CTL);
    if(val_ETH_CTL!=0xc)printf("[%x]",val_ETH_CTL);
}
#endif//VER_EMAC_CRAZY_REG_ACESS
#if VER_EMAC_CHANGE_SPEED_TEST
        ver_emac_chg_spd_test();
#endif

#if VER_EMAC_FORCE_TX
        MS_U8* pu8Data;
        MS_U32 i;
        if(EMAC_CableConnection())
        {
            pu8Data=(MS_U8*)RAM_ADDR_BASE + TX_BUFFER_BASE;
            for(i=0;i<6;i++)
            {
                pu8Data[i]=0xff;
                pu8Data[i+6]=i;
            }
            pu8Data[12]=0x08;
            pu8Data[13]=0x06;
            while(1)
            {
                SendPacket((MS_U32)pu8Data,1514);
            }
        }
#endif

#if VER_EMAC_DUMP_COUNTER
    ver_emac_dump_cnt();
#endif

#if VER_EMAC_DUMP_REG
    ver_emac_dump_reg();
#endif
}
//----------------------------
MS_U8 ver_emac_in_SendPacket(MS_U32 xframeLeng)
{
    MS_U8 result=1;
#if VER_EMAC_DUMP_COUNTER
    ver_emac_counter[0]++;//0:tx. 1:rx
#endif

#if VER_EMAC_THROUGHPUT
    if(EMAC_ReadReg32(REG_ETH_TSR)&0x01)
    {
        printf("****************TX OVER FLOW***************\n");
        result=0;
    }
    if(EMAC_ReadReg32(REG_ETH_TCR)&0x7ff)
    {
        result=0;
    }
    else
    {
        ver_emac_byte_cnt[0]+=xframeLeng;
    }
#endif
    return result;
}
MS_U8 ver_emac_task_stack[4096];
MS_U32 ver_retETH_ISR = 0;
void ver_emac_dump_reg(void);
void ver_emac_task(MS_U32 argc, VOID *argv)
{
    printf("Verify pramater:======================================================\n");
    printf("EMAC Memory map:\n");
    printf("alloRAM_ADDR_BASE=%x\n",alloRAM_ADDR_BASE);
    printf("RAM_ADDR_BASE=%x\n",RAM_ADDR_BASE);
    printf("RX_BUFFER_BASE=%x\n",RX_BUFFER_BASE);
    printf("RBQP_BASE=%x\n",RBQP_BASE);
    printf("TX_BUFFER_BASE=%x\n",TX_BUFFER_BASE);
    printf("RX_FRAME_ADDR=%x\n",RX_FRAME_ADDR);
    printf("RAM_ADDR_BASE=%x\n",RAM_ADDR_BASE);
    printf("==========================This is seperate line=======================\n");
    printf("SW_DESC_SUPPORT=%x\n",SW_DESC_SUPPORT);
    printf("RX_CHKSUM_SUPPORT=%x\n",RX_CHKSUM_SUPPORT);
    printf("DELAY_INTR=%x\n",DELAY_INTR);
    printf("MIU_WRITE_PROTECT=%x\n",MIU_WRITE_PROTECT);
    printf("SW_DESC_RX_BUFFER_SIZE=%d KByte\n",SW_DESC_RX_BUFFER_SIZE/1024);
    printf("==========================This is seperate line=======================\n");
    printf("VER_EMAC_ENABLE=%x\n",VER_EMAC_ENABLE);
    printf("VER_EMAC_IFCONFIG=%x\n",VER_EMAC_IFCONFIG);
    printf("VER_EMAC_LOOPBACK=%x\n",VER_EMAC_LOOPBACK);
    printf("VER_EMAC_INTERNAL_LOOPBACK=%x\n",VER_EMAC_INTERNAL_LOOPBACK);
    printf("VER_EMAC_RX_CHKSUM=%x\n",VER_EMAC_RX_CHKSUM);
    printf("VER_EMAC_FORCE_TX=%x\n",VER_EMAC_FORCE_TX);
    printf("VER_EMAC_DUMP_REG=%x\n",VER_EMAC_DUMP_REG);
    printf("VER_EMAC_CAM=%x\n",VER_EMAC_CAM);
    printf("VER_EMAC_THROUGHPUT=%x\n",VER_EMAC_THROUGHPUT);
    printf("VER_EMAC_DUMP_COUNTER=%x\n",VER_EMAC_DUMP_COUNTER);
    printf("VER_EMAC_CHANGE_SPEED_TEST=%x\n",VER_EMAC_CHANGE_SPEED_TEST);
    printf("VER_EMAC_MIU_WRITE_PROTECT=%x\n",VER_EMAC_MIU_WRITE_PROTECT);
    printf("VER_EMAC_CRAZY_REG_ACESS=%x\n",VER_EMAC_CRAZY_REG_ACESS);

    while(1)
    {
        if(ver_retETH_ISR & INT_RCOM) {
            ver_retETH_ISR = 0;
#if VER_EMAC_LOOPBACK
            while(1)
            {
                MS_U32 len;
                drvEMAC_RxEvent(&len);
                if(len==0)break;
                ver_emac_loopback();
#if VER_EMAC_THROUGHPUT
                ver_emac_caculate_through_put();
#endif
            }
#endif
        }
#if VER_EMAC_THROUGHPUT
               ver_emac_caculate_through_put();
#endif
        MsOS_DelayTask(1);
    }
}
void ver_emac_in__DrvEMAC_Task0(void)
{
#if VER_EMAC_IFCONFIG
    MS_U32 u32IP;
    MS_U8* pIP;
    MApi_LAN_IntfReset();
    MsOS_DelayTask(1000);
    MApi_LAN_MacConfig(MY_MAC);
    MsOS_DelayTask(2000);
    MApi_LAN_SubnetConfig("255.255.0.0");
    MApi_LAN_IpConfig("172.16.90.253");
    MApi_LAN_GatewaySet("172.16.90.254");
    MsOS_DelayTask(1000);
    u32IP = MApi_LAN_GetIp("eth0");
    pIP=(MS_U8*)&u32IP;
    printf("ip=%d.%d.%d.%d\n",pIP[0],pIP[1],pIP[2],pIP[3]);
#endif
    MsOS_CreateTask((TaskEntry)ver_emac_task,
                    0,
                    E_TASK_PRI_HIGH,
                    TRUE,
                    ver_emac_task_stack,
                    4096,
                    "ver_emac_task");
}

void ver_emac_in__ReadONE_RX_Frame_recv(void *sg_list0, MS_U32 xframeLeng)
{
#if VER_EMAC_CAM
    struct eth_drv_sgX *sg_list = (struct eth_drv_sgX *)sg_list0;
    MS_U8* da=(MS_U8*)sg_list[0].buf;
    if(da[0]!=0xff)printf("rx da=%02x:%02x:%02x:%02x:%02x:%02x [%08x,%08x]\n",da[0],da[1],da[2],da[3],da[4],da[5],_ver_emac_current_rx_desc[0],_ver_emac_current_rx_desc[1]);
#endif

#if VER_EMAC_THROUGHPUT
    ver_emac_byte_cnt[1]+=xframeLeng;
#endif

#if VER_EMAC_DUMP_COUNTER
    ver_emac_counter[1]++;//0:tx. 1:rx
#endif
}
#endif//VER_EMAC_ENABLE
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*******************************************************************************
Issue:
(1)driver use one buffer, but verify ver_emac_loopback can send two buffer at same
   time. Need to improve driver to two bufer mode. (When TCR len!=0, it is
   safe to [1]write to EMAC, [2]reuse old buffer)

(2)
[1]problem
371 counters: tx=711477, rx=714563, CTL=c, phy=6086786d
371 counters: tx=711542, rx=714628, CTL=c, phy=6086786d
371 counters: tx=711607, rx=714693, CTL=c, phy=6086786d
371 counters: tx=711672, rx=714758, CTL=c, phy=6086786d
371 counters: tx=711736, rx=714822, CTL=c, phy=6086786d
371 counters: tx=711801, rx=714888, CTL=c, phy=6086786d
371 counters: tx=711866, rx=714953, CTL=c, phy=6086786d
371 counters: tx=711931, rx=715018, CTL=c, phy=6086786d
371 counters: tx=711996, rx=715083, CTL=c, phy=6086786d
371 counters: tx=711997, rx=715084, CTL=8f, phy=6086786d
371 counters: tx=711997, rx=715084, CTL=8f, phy=6086786d
371 counters: tx=711997, rx=715084, CTL=8f, phy=6086786d
371 counters: tx=711997, rx=715084, CTL=8f, phy=6086786d
371 counters: tx=711997, rx=715084, CTL=8f, phy=6086786d
[2]readreg  can't be distrube by readreg
   writereg can't be distrube by writereg
[3]ISR usb those reg                      : RSR, RPTR, TCP
   DriveTask use those reg(for linkstatus): CTL, MAN

*******************************************************************************/




