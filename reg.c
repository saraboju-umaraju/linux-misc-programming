#define  RST_GENERAL_TIMER          0x18060000 
#define  RST_GENERAL_TIMER_RELOADx  0x18060004 
#define  RST_WATCHDOG_TIMER_CONTROL 0x18060008 
#define  RST_WATCHDOG_TIMER         0x1806000C 
#define  RST_GENERAL_TIMER2         0x18060094 
#define  RST_GENERAL_TIMER2_RELOAD  0x18060098 
#define  RST_GENERAL_TIMER3         0x1806009C 
#define  RST_GENERAL_TIMER3_RELOAD  0x180600A0 
#define  RST_GENERAL_TIMER4         0x180600A4 
#define  RST_GENERAL_TIMER4_RELOAD  0x180600A8 

ath_reg_wr(RST_GENERAL_TIMER        , 0);
ath_reg_wr(RST_GENERAL_TIMER_RELOADx, 0);
ath_reg_wr(RST_WATCHDOG_TIMER_CONTROL,0);
ath_reg_wr(RST_WATCHDOG_TIMER       , 0);
ath_reg_wr(RST_GENERAL_TIMER3       , 0);
ath_reg_wr(RST_GENERAL_TIMER3_RELOAD, 0);
ath_reg_wr(RST_GENERAL_TIMER4       , 0);
ath_reg_wr(RST_GENERAL_TIMER4_RELOAD, 0);
ath_reg_wr(RST_GENERAL_TIMER5       , 0);
ath_reg_wr(RST_GENERAL_TIMER5_RELOAD, 0);
