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
