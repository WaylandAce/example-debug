Based on:

ID DLC Data 
5C1 1 06 Vol up 
5C1 1 07 Vol Down 
5C1 1 0A Menu 
5C1 1 1A Phone 
5C1 1 22 Arrow Up 
5C1 1 23 Arrow Down 
5C1 1 28 OK 
5C1 1 2B Mute 
5C1 1 00 Button is released 
*/
#define AUDI_MULTI_RUL 0x5C3
#define MFW_none 0x3900
#define MFW_prev 0x3902
#define MFW_next 0x3903
#define MFW_vol_up 0x3906
#define MFW_vol_down 0x3907
#define MFW_scan_up 0x3904
#define MFW_scan_down 0x3905

/*
5C3 2 39 00 none 
5C3 2 39 02 Prev 
5C3 2 39 03 Next 
5C3 2 39 04 Scan Up 
5C3 2 39 05 Scan down 
5C3 2 39 06 Volume+ 
5C3 2 39 07 Volume- 
5C3 2 3C 2A Phone button pressed 
5C3 2 3C 00 Phone button released, changes to 39 00 after a second or so 
5C3 2 3A 1C Mode button pressed 
5C3 2 3A 00 Mode button released, changes to 39 00 after a second or so 
5C3 2 39 01 Mode button pressed before it turns back to 39 00 
5C3 2 3A 1A up/down button pressed within 2 seconds after pressing Mode button 
5C3 2 3A 02 up within 2 seconds after pressing Mode 
5C3 2 3A 03 down within 2 seconds after pressing Mode 
5C3 2 3B A7 vol up/down pressed 
5C3 2 3C 07 vol down after pressing phone button 
5C3 2 3C 06 vol up after pressing phone bu


CAN-Infotainment (100kbit/s)


I've checked the code generated from Standard Id CAN Bus demo of the Waijung Blockset and found some interesting settings. Since APB1 Prescaler is 4, AHB Prescaler is 1 (defined under system_stm32f4xx.c), the CAN bus clock is 42Mhz. Moreover, bitrate is calculated as Bitrate= CAN_CLK/(CAN_Prescaler * (1 + CAN_BS1 + CAN_BS2+CAN_SJW)).

When the generated code is examined, under waijung_hwdrvlib.c, CAN bus timing settings for 125kbit/s as follows:

/* CAN Baudrate = 125000 bps */
CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;//CAN_BS1_6tq=0x05
CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;//CAN_BS2_8tq=0x07
CAN_InitStructure.CAN_Prescaler = 21;
CAN_Init(CAN1, &CAN_InitStructure);

But, the above time settings defined for 125000 bps actually correspond to 42000000/(21*(1+5+7+0))=153846. Hence, I think the baudrate settings are wrong under Waijung CAN blockset. 

After googling about CAN bus standards, I found the recommendations given in the attached table copied from the book "A Comprehensible Guide to Controller Area Network"

Hence, I think the correct baudrate settings should be as follows,

CAN_BS1 = 14 and CAN_BS2 = 6 

Bitrate= 42000000/(CAN_Prescaler*(1+14+6+0))

CAN_Prescaler=2 // 1000 kbit/s
CAN_Prescaler=4 // 500 kbit/s
CAN_Prescaler=5 // 400 kbit/s
CAN_Prescaler=8 // 250 kbit/s
CAN_Prescaler=10 // 200 kbit/s
CAN_Prescaler=16 // 125 kbit/s
CAN_Prescaler=20 // 100 kbit/s
CAN_Prescaler=40 // 50 kbit/s
CAN_Prescaler=80 // 40 kbit/s
CAN_Prescaler=200 // 10 kbit/s
