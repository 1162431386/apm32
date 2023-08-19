#include "main.h"
#include "testCase.h"
#include "common.h"

#include "tim.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

uint8_t TePwrSeqState = INIT;
uint8_t TePreErrState = INIT;

uint8_t nextCourseAmpMeasure = 0;
uint8_t pre_nextCourseAmpMeasure = 0;

uint32_t g_uClockCnt = 0;
uint32_t g_uEndSampleClkCnt = 0;
uint32_t g_uStartSampleClkCnt = 0;

uint32_t uiDutyCycle;
uint32_t uiCycle;
uint32_t uiFrequency;

void get_duty_cycle(uint32_t uiDutyCycle, uint32_t uiCycle, uint8_t *HRadio, uint8_t *LRadio)
{
#if 0
    int i;
    for (i = uiDutyCycle; i >= 1; i--) {
        if ((uiDutyCycle % i == 0) && (uiCycle % i == 0)) {
            uiDutyCycle = uiDutyCycle / i;
            uiCycle = uiCycle / i;
        }
    }
#endif
    uint32_t a = uiDutyCycle;
    uint32_t b = uiCycle;
    while ((a > 10) && (b > 10)) {
        a = a / 10;
        b = b / 10;
    }
    *HRadio = a;
    *LRadio = b;
}

float vcc_amp_count(uint16_t pc4, uint16_t pc5)
{
    return ((VOL(pc4) - VOL(pc5)) / 15);
}

float vcc_amp_get(void)
{
    return (curVccAmp * 1000000);  /* uA */
}


void send_clock_sample_frame(void)
{
    uint32_t tmp = 0;
    uint8_t a,b = 0;

    SAMPLING_CLK clkFrame = {0};
    clkFrame.dataLen = stm32_htons(14);
    clkFrame.dataType = stm32_htons(0x8102);
    tmp = 1000000/uiCycle;
    clkFrame.clkFreq = stm32_htonl(tmp);
    tmp = HAL_GetTick();
    clkFrame.StartTimeStamp = stm32_htonl(tmp);

    get_duty_cycle(uiDutyCycle, uiCycle, &a, &b);
    clkFrame.HRadio = a & 0xf;
    clkFrame.LRadio = b & 0xf;

    CDC_Transmit_FS((uint8_t *)&clkFrame, sizeof(clkFrame));

    return;
}

void startup_info_report(void)
{
    SYS_START_UP_INFO_T sysInfoPkt = {0};

    sysInfoPkt.pktLen = stm32_htons(sizeof(SYS_START_UP_INFO_T));
    sysInfoPkt.pktType = stm32_htons(0x8001);
    sysInfoPkt.uidLen = stm32_htons(6);
    sysInfoPkt.version = stm32_htons(0x10);
    *(uint16_t *)&sysInfoPkt.uid = stm32_htons(HAL_GetUIDw0() & 0xffff);
    *((uint16_t *)(&sysInfoPkt.uid)+1) = stm32_htons(HAL_GetUIDw1() & 0xffff);
    *((uint16_t *)(&sysInfoPkt.uid)+2) = stm32_htons(HAL_GetUIDw2() & 0xffff);
    CDC_Transmit_FS((uint8_t *)&sysInfoPkt, sysInfoPkt.pktLen);
    return;
}


void sys_exception_info_report(char *errInfo)
{
     SYS_EXCEPTION_INFO_T errInfoPkt = {0};

     errInfoPkt.pktLen = stm32_htons(sizeof(SYS_START_UP_INFO_T));
     errInfoPkt.pktType = stm32_htons(0x8001);
     errInfoPkt.errInfoLen = stm32_htons(6);
     errInfoPkt.version = stm32_htons(0x10);
     strcpy(errInfoPkt.errInfo, errInfo);

     CDC_Transmit_FS((uint8_t *)&errInfoPkt, errInfoPkt.pktLen);
     return;
}
void te_init_pwr_seq(uint8_t *TePwrSeqState, float curVcc_v, float curClk_v, float curIo_v, float curRst_v)
{
    switch(*TePwrSeqState) {
        case INIT:
            if ((curTeVccState(curVcc_v) == CLASS_POWER_OFF_DEV) && (ClkLevel(curClk_v) == 'L') &&
                (IoLevel(curIo_v) == 'L') && (TeRstLevel(curRst_v) == 'L'))
                *TePwrSeqState = PWR_OFF;
            else {
                TePreErrState = INIT;
                *TePwrSeqState = PWR_FAIL;
            }
            break;
        case PWR_OFF:
            if ((curTeVccState(curVcc_v) == CLASS_B_3V3_DEV) || (curTeVccState(curVcc_v) == CLASS_C_1V8_DEV))
            {
								 /* 使能定时TIM4的PWM输入捕获 */
								HAL_GPIO_WritePin(CLK_CNT_EN_GPIO_Port, CLK_CNT_EN_Pin, GPIO_PIN_SET);
								HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);  
								__HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
				
                if (curTeVccState(curVcc_v) == CLASS_B_3V3_DEV)
                    *TePwrSeqState = VCC_3V3_STABLE;
                if (curTeVccState(curVcc_v) == CLASS_C_1V8_DEV)
                    *TePwrSeqState = VCC_1V8_STABLE;
            }
            break;
        default:
            break;
    }
}               
uint32_t g_pro_clk_cnt = 0;
/*3v3 terminal start course*/   
void te_3v3_pwr_seq(uint8_t *TePwrSeqState, uint32_t curClock, float curVcc_v, float curClk_v, float curIo_v, float curRst_v)
{
   switch(*TePwrSeqState){
        case VCC_3V3_STABLE:
            if (clkStableFlag == 1) {
                g_pro_clk_cnt = g_uClockCnt;
                send_clock_sample_frame();
                *TePwrSeqState = VCC_3V3_CLK_STABLE;
            }
            break;
        case VCC_3V3_CLK_STABLE:
					  usb_printf("IO >>>>>>>> [%u, %u] %u %u\r\n", g_uClockCnt, g_pro_clk_cnt, curVcc_v, curIo_v);
            if(isHighZ(curVcc_v, curIo_v) != 'Z'){
						    if ((g_uClockCnt - g_pro_clk_cnt) <= 200){
                    *TePwrSeqState = VCC_3V3_IO_STABLE;
                } else {
                    TePreErrState = VCC_3V3_IO_STABLE;
                   *TePwrSeqState = PWR_FAIL;
                }						
						}
            break;
        case VCC_3V3_IO_STABLE:
					 usb_printf("RST >>>>>>>> [%u, %u] %u %u\r\n", g_uClockCnt, g_pro_clk_cnt, curVcc_v, curRst_v);
            if (TeRstLevel(curRst_v) != 'L') {
                if ((g_uClockCnt - g_pro_clk_cnt) >= 400) {
                    *TePwrSeqState = VCC_3V3_NORMAL_STATE;
                } else {
                    TePreErrState = VCC_3V3_IO_STABLE;
                    *TePwrSeqState = PWR_FAIL;
                } 
					  }
            break;

        case VCC_3V3_NORMAL_STATE:
            *TePwrSeqState = VCC_SWTICH_TO_5V;
            clkStableFlag = 0;
            break;
        default:
            break;
    }
}

/*1v8 terminal start course*/   
void te_1v8_pwr_seq(uint8_t *TePwrSeqState, uint32_t curClock, float curVcc_v, float curClk_v, float curIo_v, float curRst_v)
{
	  uint8_t high_z = 0;
    switch(*TePwrSeqState){
        case VCC_1V8_STABLE:
            if (clkStableFlag == 1) {
                g_pro_clk_cnt = g_uClockCnt;
                send_clock_sample_frame();  
                *TePwrSeqState = VCC_1V8_CLK_STABLE;
            }
            break;
        case VCC_1V8_CLK_STABLE:
					  usb_printf("IO >>>>>>>> [%u,%u] %u %u\r\n", g_uClockCnt, g_pro_clk_cnt, curVcc_v, curIo_v);
            if(isHighZ(curVcc_v, curIo_v) != 'Z'){
						    if ((g_uClockCnt - g_pro_clk_cnt) <= 200){
                    *TePwrSeqState = VCC_1V8_IO_STABLE;
                } else {
                    TePreErrState = VCC_1V8_CLK_STABLE;
                   *TePwrSeqState = PWR_FAIL;
                }						
						}
            break;
        case VCC_1V8_IO_STABLE:
					 usb_printf("RST >>>>>>>> [%u,%u] %u %u\r\n", g_uClockCnt, g_pro_clk_cnt, curVcc_v, curRst_v);
            if (TeRstLevel(curRst_v) != 'L') {
                if ((g_uClockCnt - g_pro_clk_cnt) >= 400) {
                    *TePwrSeqState = VCC_1V8_NORMAL_STATE;
                } else {
                    TePreErrState = VCC_1V8_IO_STABLE;
                    *TePwrSeqState = PWR_FAIL;
                } 
					  }
            break;
        case VCC_1V8_NORMAL_STATE:
            *TePwrSeqState = VCC_SWTICH_TO_3V3;
            clkStableFlag = 0;
            break;
        default:
            break;
    }
}

/*stage 2 5v power seq*/
void te_2nd_5v_pwr_seq(uint8_t *TePwrSeqState, uint32_t curClock, float curVcc_v, float curClk_v, float curIo_v, float curRst_v)
{
    switch(*TePwrSeqState){
        case VCC_SWTICH_TO_5V:
            if (clkStableFlag == 1) {
                send_clock_sample_frame();
                *TePwrSeqState = VCC_5V_CLK_STABLE;
            }
            break;
						
        case VCC_5V_CLK_STABLE:
					  usb_printf("IO >>>>>>>> [%u,%u] %u %u\r\n", g_uClockCnt, g_pro_clk_cnt, curVcc_v, curIo_v);
            if(isHighZ(curVcc_v, curIo_v) != 'Z'){
						    if ((g_uClockCnt - g_pro_clk_cnt) <= 200){
                    *TePwrSeqState = VCC_5V_IO_STABLE;
                } else {
                    TePreErrState = VCC_5V_IO_STABLE;
                   *TePwrSeqState = PWR_FAIL;
                }						
						}
            break;
        case VCC_5V_IO_STABLE:
					 usb_printf("RST >>>>>>>> [%u,%u] %u %u\r\n", g_uClockCnt, g_pro_clk_cnt, curVcc_v, curRst_v);
            if (TeRstLevel(curRst_v) != 'L') {
                if ((g_uClockCnt - g_pro_clk_cnt) >= 400) {
                    *TePwrSeqState = VCC_5V_NORMAL_STATE;
                } else {
                    TePreErrState = VCC_5V_IO_STABLE;
                    *TePwrSeqState = PWR_FAIL;
                } 
					  }
            break;

        default:
            break;
    }
}

/*stage 2 3v3 power seq*/
void te_2nd_3v3_pwr_seq(uint8_t *TePwrSeqState, uint32_t curClock, float curVcc_v, float curClk_v, float curIo_v, float curRst_v)
{
    switch (*TePwrSeqState) {
        case VCC_SWTICH_TO_3V3:
            if (clkStableFlag == 1) {
                send_clock_sample_frame();
                *TePwrSeqState = VCC_3V3_CLK_STABLE;
            }
            break;
        case VCC_2nd_3V3_CLK_STABLE:
					  usb_printf("IO >>>>>>>> [%u,%u] %u %u\r\n", g_uClockCnt, g_pro_clk_cnt, curVcc_v, curIo_v);
            if(isHighZ(curVcc_v, curIo_v) != 'Z'){
						    if ((g_uClockCnt - g_pro_clk_cnt) <= 200){
                    *TePwrSeqState = VCC_3V3_IO_STABLE;
                } else {
                    TePreErrState = VCC_2nd_3V3_CLK_STABLE;
                   *TePwrSeqState = PWR_FAIL;
                }						
						}
            break;
        case VCC_2nd_3V3_IO_STABLE:
					 usb_printf("RST >>>>>>>> [%u,%u] %u %u\r\n", g_uClockCnt, g_pro_clk_cnt, curVcc_v, curRst_v);
            if (TeRstLevel(curRst_v) != 'L') {
                if ((g_uClockCnt - g_pro_clk_cnt) >= 400) {
                    *TePwrSeqState = VCC_3V3_NORMAL_STATE;
                } else {
                    TePreErrState = VCC_2nd_3V3_IO_STABLE;
                    *TePwrSeqState = PWR_FAIL;
                } 
					  }
            break;

        default:
            break;
    }
}

uint8_t g_pwrDownState = START;
void te_3v3_pwr_down_seq(uint8_t *TePwrSeqState, uint32_t curClock, float curVcc_v, float curClk_v, float curIo_v, float curRst_v)
{
    switch(*TePwrSeqState) {
        case START:
            if (curTeVccState(curVcc_v) == CLASS_B_3V3_DEV) {
                *TePwrSeqState = VCC_3V3_PWR_PRE_NORMAL;
            }
            break;
        case VCC_3V3_PWR_PRE_NORMAL:
            if (curTeVccState(curVcc_v) == CLASS_POWER_OFF_DEV) {
                *TePwrSeqState = VCC_3V3_PWR_CUR_OFF;
            }
            break;
        case VCC_3V3_PWR_CUR_OFF:
            if((ClkLevel(curClk_v) == 'L') && (TeRstLevel(curRst_v) == 'L') && (isStateA(curVcc_v, curIo_v) == 'A')) {
                *TePwrSeqState = VCC_2nd_5V_START;
            }
            break;
        default:
            break;
    }
    return;
}

void te_1v8_pwr_down_seq(uint8_t *TePwrSeqState, uint32_t curClock, float curVcc_v, float curClk_v, float curIo_v, float curRst_v)
{
    switch (*TePwrSeqState){
        case START:
            if (curTeVccState(curVcc_v) == CLASS_C_1V8_DEV) {
                *TePwrSeqState = VCC_1V8_PWR_PRE_NORMAL;
            }
            break;
        case VCC_1V8_PWR_PRE_NORMAL:
            if (curTeVccState(curVcc_v) == CLASS_POWER_OFF_DEV) {
                *TePwrSeqState = VCC_1V8_PWR_CUR_OFF;
            }
            break;
        case VCC_1V8_PWR_CUR_OFF:
            if ((ClkLevel(curClk_v) == 'L') && (TeRstLevel(curRst_v) == 'L') && (isStateA(curVcc_v, curIo_v) == 'A')) {
                *TePwrSeqState = VCC_2nd_3V3_START;
            }
            break;
        default:
            break;
    }
    return;
}

void te_2nd_3v3_pwr_down_seq(uint8_t *TePwrSeqState, uint32_t curClock, float curVcc_v, float curClk_v, float curIo_v, float curRst_v)
{
    switch (*TePwrSeqState) {
        case VCC_2nd_3V3_START:
            if (curTeVccState(curVcc_v) == CLASS_B_3V3_DEV) {
                *TePwrSeqState = VCC_2nd_3V3_PWR_PRE_NORMAL;
            }
            break;
        case VCC_2nd_3V3_PWR_PRE_NORMAL:
            if (curTeVccState(curVcc_v) == CLASS_POWER_OFF_DEV) {
                *TePwrSeqState = VCC_2nd_3V3_PWR_CUR_OFF;
            }
            break;
        case VCC_2nd_3V3_PWR_CUR_OFF:
            if((ClkLevel(curClk_v) == 'L') && (TeRstLevel(curRst_v) == 'L') && (isStateA(curVcc_v, curIo_v) == 'A')) {
                *TePwrSeqState = VCC_2nd_3V3_PWR_OFF_STABLE;
            }
            break;
        default:
            break;
    }

    return;
}

void te_2nd_5v_pwr_down_seq(uint8_t *TePwrSeqState, uint32_t curClock, float curVcc_v, float curClk_v, float curIo_v, float curRst_v)
{
    switch(*TePwrSeqState){
        case VCC_2nd_5V_START:
            if(curTeVccState(curVcc_v) == CLASS_A_5V_DEV)
                *TePwrSeqState = VCC_2nd_5V_PWR_PRE_NORMAL;
            break;
        case VCC_2nd_5V_PWR_PRE_NORMAL:
            if(curTeVccState(curVcc_v) == CLASS_POWER_OFF_DEV)
                *TePwrSeqState = VCC_2nd_5V_PWR_CUR_OFF;
            break;
        case VCC_2nd_5V_PWR_CUR_OFF:
            if ((ClkLevel(curClk_v) == 'L') && (TeRstLevel(curRst_v) == 'L') && (isStateA(curVcc_v, curIo_v) == 'A')) {
                *TePwrSeqState = VCC_2nd_5V_PWR_OFF_STABLE;
            }
            break;
        default:
            break;
    }

    return;
}
