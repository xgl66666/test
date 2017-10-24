For Dmx DSMCC stress tests (0 1 1 3 2)
DDBouput.bin must be multiplex using genius 1.3.2.
Use the following profile.ini to configure genius.

Profile.ini:

[Config]
[Config/PROFILE_LIST]
[Config/PROFILE_LIST/Default]
SoftVersion=0x00000001
BLTYPE=Boot Loader
BLTYPE_DC_OC_IWC=Data Carousel
TriggerType_UK_SSU_SSUE=SSU Simple
AUTO_SIGN=true
InputKeyFile=Y:\\build\\src\\iwedia.key
InputSignFile=
OutputDir=C:\\IwediaStreamOutput
N_IMAGES=1
InputFile=Y:\\test\\chal\\chaltests\\data\\DDBoutput.bin
FlashOffset1=0x00000000
InputFile2=
FlashOffset2=0x00000000
InputFile3=
FlashOffset3=0x00000000
InputFile4=
FlashOffset4=0x00000000
InputFile5=
FlashOffset5=0x00000000
InputFile6=
FlashOffset6=0x00000000
InputFile7=
FlashOffset7=0x00000000
InputFile8=
FlashOffset8=0x00000000
Protocol=0x0001
OUI=0x000000
ManName=
HW_Model=0x0000
HW_Version=0x0000
SW_Model=0x0000
SW_Version=0x0000
GroupIDInputFile=
ONID=0x0011
TSID=0x0022
SVCID=0x0033
NO_NIT_BAT=None
PMT_PID=0x0055
DATA_PID=0x0044
DiffusionDate=2008/06/10
INTERNAL_SIG=true
SSUE_StartDiffusionDate=2008/06/10
SSUE_StartHourUser=01
SSUE_StopHourUser=01
SSUE_StartHourAmPm=am
SSUE_StopHourAmPm=am
SSUE_NbDayOfDiffsuion=0
SSUE_UNT_PID=0x0000
SSUE_AdditionMessage=
SAT_CABLE_TERR=Terrestrial
SAT_Frequency=000,00000
SAT_OrbitalPosition=000,0
SAT_WestEastFlag=western
SAT_Polarization=linear - horizontal
SAT_Modulation=Not defined
SAT_SymbolRate=000,0000
SAT_FECInner=Not defined
CABLE_Frequency=000,00000
CABLE_FECOuter=No FEC coding
CABLE_Modulation=16-QAM
CABLE_SymbolRate=000,0000
CABLE_FECInner=1/2
TERR_CentreFrequency=10000000
TERR_Bandwidth=8 MHz
TERR_Priority=Low
TERR_TimeSlicingIndicator=Used
TERR_MPE_FEC_Indicator=Used
TERR_Constellation=16-QAM
TERR_HierarchyInformation=Non-hierarchical
TERR_CodeRateHPStream=7/8
TERR_CodeRateLPStream=7/8
TERR_GuardInterval=1/32
TERR_TransmissionMode=8k
TERR_OtherFrequencyFlag=no other used
DataRate=1000
ModuleSize=64 KBytes
DoMultiplex=true
MODE_EXPERT=true
DEBUG_REQUIRED=true
