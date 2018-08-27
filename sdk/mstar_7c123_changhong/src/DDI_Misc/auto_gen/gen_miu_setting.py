import sys
import re

VIVALDI9 = re.compile('^(VIVALDI9)[\s]*(vivaldi9 DSP Cache)?', re.I)
VIVALDI_AU_R2 = re.compile('^(VIVALDI[\d]?[\s]+AU_R2_[\d]){1}|^(VIVALDI[\d]?[\s]?R2 ARB){1}|^(AU_R2){1}', re.I)
VD_R2 = re.compile('^(VD_R2[.]*){1}', re.I)
SEC_R2 = re.compile('^(SEC_R2[_]?[\d]?[.]*){1}|^(Secure_R2){1}', re.I)
TSP_ALL = re.compile('^(TSP([_])?[\d]){1}', re.I)
TSIO_ALL = re.compile('^(TSIO).*', re.I)

MVD_BBU = re.compile('^(MVD[\s]*(\(bbu\))?(bbu)?){1}', re.I)
MVD_ENG = re.compile('^(MVD)$|^(MVD)[\s]*[(]?ENG[\d]?[)]?$', re.I)

EVD_BBU = re.compile('^(EVD).*(bbu)', re.I)
EVD_ENG0 = re.compile('^(EVD).*(eng0)', re.I)
EVD_ENG1 = re.compile('^(EVD).*(eng1)', re.I)

HVD_BBU = re.compile('^(HVD).*(bbu)', re.I)
HVD_ENG = re.compile('^(HVD).*(eng)', re.I)

BDMA = re.compile('.*(BDMA){1}.*', re.I)
URDMA = re.compile('.*((URDMA)|(UART DMA)){1}.*', re.I)
HSUART = re.compile('.*(high-speed UART){1}.*', re.I)
XD2MIU = re.compile('.*(xd2miu){1}.*', re.I)

PCIE_ALL = re.compile('^(PCIE)+?', re.I)
USB_ALL = re.compile('^(USB)+?', re.I)
SDIO_ALL = re.compile('^(SDIO)+?', re.I)
GMAC_ALL = re.compile('^((.)*(GMAC))+?', re.I)
EMAC_ALL = re.compile('^((.)*(EMAC))+?', re.I)

ZDEC_ALL = re.compile('^(ZDEC)+?', re.I)
MFE_ALL = re.compile('^(MFE)+?', re.I)
NAND_ALL = re.compile('^((.)*(NAND))+?|^(FCIE)+?', re.I)
JPD_ALL = re.compile('^(JPD)+?', re.I)
GPD_ALL = re.compile('^(GPD)+?', re.I)
CMDQ_ALL = re.compile('^(CMDQ)+?', re.I)
SATA_ALL = re.compile('^(SATA)+?', re.I)

MVOP_ALL = re.compile('^(MVOP)+?', re.I)
GE_ALL = re.compile('^(GE)+?', re.I)
VE_ALL = re.compile('^(VE)+?', re.I)
SC1_OPM = re.compile('^(SC1)+?[\s]*[_]?[(]*(OPM){1}[)]*', re.I)
SC1_IPM = re.compile('^(SC1)+?[\s]*[_]?[(]*(IPM){1}[)]*', re.I)
OPM = re.compile('^(OPM)', re.I)
IPM = re.compile('^(IPM)', re.I)
IPS = re.compile('^(IPS)', re.I) #Sub window

GOP_ALL = re.compile('^(GOP[_]?[\d]+)', re.I)

MFDEC_ALL = re.compile('^(MFDEC)', re.I)
AUTO_DOWNLOAD = re.compile('^(auto download)', re.I)
DIP_ALL = re.compile('^(DIP)|DDI_[\d]', re.I)
DYN_SCL = re.compile('^(DYN_SCL)')

GPU_ALL = re.compile('.*(GPU){1}.*', re.I)
DEMOD_ALL = re.compile('^(DEMOD){1}[_]?[\d]?', re.I)

CLIENT_ID = re.compile('g[0-9a-f]{2}', re.I)
CLIENT_NAME = re.compile('^(.(?!(g[0-9a-f]{2}))*$)', re.I)
TSO_ALL = re.compile('^(TSO)+', re.I)
CROSSBAR_ALL = re.compile('.*(MIU){1}[_]?(CROSSBAR){1}', re.I)

found = 0
index = 0
NAME = 0
HAS_PRESELECT = 1
MMAP_NAME = 3
MI_MMAP_NAME = 4
miu_num = 0
hw_sel = 0
miu0_sel = 0
miu1_sel = 0

miu0_group_selmiu = "setenv MIU0_GROUP_SELMIU "
miu1_group_selmiu = "setenv MIU1_GROUP_SELMIU "

IP_LIST = [
          [VIVALDI9, 0, 0, 'MAD_ADV_BUF', 'MI_MAD_ADV_BUF'],
          [VIVALDI_AU_R2, 0, 0, 'MAD_ADV_BUF', 'MI_MAD_ADV_BUF'],
          [VD_R2, 0, 0, 'VDEC_AEON', 'MI_VDEC_AEON'],
          [SEC_R2, 1, 1, '', ''],
          [TSP_ALL, 1, 1, '', ''],
          [TSIO_ALL, 1, 0, 'TSIO_TEMP', ''],
          [BDMA, 1, 1, '', ''],
          [URDMA, 1, 1, '', ''],
          [XD2MIU, 1, 1, '', ''],
          [HSUART, 1, 1, '', ''],
          [PCIE_ALL, 1, 1, '', ''],
          [USB_ALL, 1, 1, '', ''],
          [SDIO_ALL, 1, 1, '', ''],
          [GMAC_ALL, 0, 0, 'GMAC_MEM', 'GMAC_MEM'],
          [EMAC_ALL, 0, 0, 'EMAC_MEM', 'EMAC_MEM'],
          [ZDEC_ALL, 1, 1, '', ''],
          [MFE_ALL, 0, 1, '', ''],
          [NAND_ALL, 1, 1, '', ''],
          [JPD_ALL, 1, 1, '', ''],
          [GPD_ALL, 1, 1, '', ''],
          [CMDQ_ALL, 1, 1, '', ''],
          [SATA_ALL, 1, 1, '', ''],
          [MVOP_ALL, 1, 1, '', ''],
          [GE_ALL, 1, 1, '', ''],
          [MVD_ENG, 0, 0, 'VDEC_FRAME_BUF','MI_VDEC_FRAME_BUF'],
          [MVD_BBU, 0, 0, 'VDEC_BIT_STREAM','MI_VDEC_BITSTREAM'],
          [EVD_ENG0, 0, 0, 'VDEC_FRAME_BUF','MI_VDEC_FRAME_BUF'],
          [EVD_ENG1, 0, 0, 'VDEC_FRAME_BUF','MI_VDEC_FRAME_BUF'],
          [EVD_BBU, 0, 0, 'VDEC_BIT_STREAM','MI_VDEC_BITSTREAM'],
          [HVD_ENG, 0, 0, 'VDEC_FRAME_BUF','MI_VDEC_FRAME_BUF'],
          [HVD_BBU, 0, 0, 'VDEC_BIT_STREAM','MI_VDEC_BITSTREAM'],
          [OPM, 0, 0, 'SC0_MAIN_FB', 'MI_XC_MAIN_FRAME_BUF'],
          [IPM, 0, 0, 'SC0_MAIN_FB', 'MI_XC_MAIN_FRAME_BUF'],
          [IPS, 0, 0, 'SC0_SUB_FB', 'MI_XC_SUB_FRAME_BUF'],
          [SC1_OPM, 0, 0, 'SC1_MAIN_FB', 'MI_XC1_MAIN_FRAME_BUF'],
          [SC1_IPM, 0, 0, 'SC1_MAIN_FB', 'MI_XC1_MAIN_FRAME_BUF'],
          [VE_ALL, 0, 0, 'VE_FRAME_BUF', 'MI_VE_FRAME_BUF'],
          [MFDEC_ALL, 1, 1, '', ''],
          [AUTO_DOWNLOAD, 1, 1, '', ''],
          [GOP_ALL, 0, 0, 'GOP_BASE_TAG', 'MI_BOOTLOGO_SHOW_BUF'],
          [DYN_SCL, 1, 1, '', ''],
          [GPU_ALL, 1, 1, '', ''],
          [DIP_ALL, 1, 1, '', ''],
          [TSO_ALL, 1, 0, 'TSO_SVQ_BUF', ''],
          [CROSSBAR_ALL, 1, 0, '', ''],
          [DEMOD_ALL, 1, 0, 'INTERNAL_DVBS2_DJB', ''],
         ]

def get_sel(mem):
    mem_str = mem + "_MEMORY_TYPE"
    miu_str = ""
    miu_num = ""
    sel = ""
    mmap.seek(0)
    for line in mmap:
        if re.search(mem_str, line):
            miu_str = line.split()[2][1:]
            sel = line.split()[4]
            #sanity check
            if re.match("(HW)|(SW)", sel) and re.match("MIU", miu_str):
                miu_num = miu_str[3:]
                if sel == "SW":
                    return 0, int(miu_num)
                else:
                    return 1, 0
            else:
                print "check fail"
                sys.exit()

    print "-----> Cannot find", mem_str, "in memory map, Force IP sel"
    return 1, 0

def update_group(miu1_sel, miu0_sel):
    global miu0_group_selmiu
    global miu1_group_selmiu
    miu0_group_selmiu = miu0_group_selmiu + '{0:0{1}X}'.format(miu0_sel, 4) + ':'
    miu1_group_selmiu = miu1_group_selmiu + '{0:0{1}X}'.format(miu1_sel, 4) + ':'
    print miu0_group_selmiu
    print miu1_group_selmiu + "\n"

# Check MI_BSP enable or not for parsing MI MMAP FILE.
def check_mi_bsp():
  AutoGenVarList = open(sys.argv[3], "r")
  for line in AutoGenVarList:
    MI_CONFIG = line.split("=")[0]
    if MI_CONFIG=="MI_BSP":
      return "enable"

MI_BSP = check_mi_bsp()

if len(sys.argv) < 4:
    print "please enter MMAP file name."
    sys.exit()

print sys.argv[1]
print sys.argv[2]
print sys.argv[3]

mmap = open(sys.argv[2], "r")
with open(sys.argv[1]) as client_table:
    for line in client_table:
        if index >= 16:
           index = 0
           update_group(miu1_sel, miu0_sel)
           miu1_sel = 0
           miu0_sel = 0

        client_id = re.findall(CLIENT_ID, line)[0]
        client = re.split(CLIENT_ID, line)[1].lstrip().strip() #remove leading and trailing spaces
        print index,

        # no client, skip
        if client == '':
            index = index + 1
            print ""
            continue

        for ip in IP_LIST:
            if ip[NAME].match(client):
                found = 1
                print client,
                if ip[HAS_PRESELECT]:
                    # miu num is irrevalent if it is IP select
                    print "is IP sel"
                    hw_sel = 1
                    miu_num = 0
                else:
                    if MI_BSP=="enable":
                      print "based on", ip[MI_MMAP_NAME],
                      result = get_sel(ip[MI_MMAP_NAME])
                    else:
                      print "based on", ip[MMAP_NAME],
                      result = get_sel(ip[MMAP_NAME])
                    hw_sel = result[0]
                    miu_num = result[1]
                    print "hw" if hw_sel else "sw", "sel", miu_num

                miu1_sel |= hw_sel << index
                miu0_sel |= miu_num << index
                index = index + 1
                break; # once we found a match there is no need to check the rest of the list

        if found == 1:
            found = 0
        else:
            print client, "----------------> no matching IP"
            index = index + 1
    if index <= 16:
        update_group(miu1_sel, miu0_sel)

miu_setting_txt = open("miu_setting.txt", "w+")
miu_setting_txt.write(miu0_group_selmiu[:-1] + "\n" + miu1_group_selmiu[:-1] + "\nsaveenv\n%<- this is end of file symbol")
miu_setting_txt.close()

client_table.close()
mmap.close()
