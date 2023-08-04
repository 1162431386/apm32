#ifndef COMMON_H
#define COMMON_H
#include "main.h"
#include "testCase.h"

#define SAMPLING_COUNT 32

#define CLASS_POWER_OFF_DEV         0    /*invaild pre-define terminal class*/
#define CLASS_A_5V_DEV              1    /*5v terminal*/
#define CLASS_B_3V3_DEV             2    /*3.3v terminal*/
#define CLASS_C_1V8_DEV             3    /*1.8v terminal*/
#define CLASS_POWER_INVAILD_DEV     4

#define VOL(x) (((x) * 3.3000 / 4095.0) * 3.300 / 1.7000)

#define stm32_htons(A)  ((((uint16_t)(A) & 0xff00) >> 8) | \
                         (((uint16_t)(A) & 0x00ff) << 8))

#define stm32_htonl(A)  ((((uint32_t)(A) & 0xff000000) >> 24) | \
                         (((uint32_t)(A) & 0x00ff0000) >> 8) | \
                         (((uint32_t)(A) & 0x0000ff00) << 8)  | \
                         (((uint32_t)(A) & 0x000000ff) << 24))

#define stm32_ntohs(A)  ((((uint16_t)(A) & 0xff00) >> 8) | \
                         (((uint16_t)(A) & 0x00ff) << 8))
#define stm32_ntohl(A)  ((((uint32_t)(A) & 0xff000000) >> 24) | \
                         (((uint32_t)(A) & 0x00ff0000) >> 8) | \
                         (((uint32_t)(A) & 0x0000ff00) << 8) | \
                         (((uint32_t)(A) & 0x000000ff) << 24))

enum { 
    INIT,
    PWR_OFF,
    PWR_FAIL,
    VCC_1V8_STABLE,
    VCC_3V3_STABLE,
    VCC_5V_STABLE,	
    VCC_1V8_CLK_STABLE,
    VCC_3V3_CLK_STABLE,
    VCC_5V_CLK_STABLE,
    VCC_1V8_IO_STABLE,
    VCC_3V3_IO_STABLE,
    VCC_5V_IO_STABLE,
    VCC_1V8_NORMAL_STATE,
    VCC_3V3_NORMAL_STATE,
    VCC_5V_NORMAL_STATE,
    VCC_SWTICH_TO_5V,
    VCC_SWTICH_TO_3V3,
    VCC_2nd_3V3_CLK_STABLE,
    VCC_2nd_3V3_IO_STABLE,
    VCC_2nd_3V3_NORMAL_STATE,
};


enum { 
    START,
    VCC_1V8_PWR_PRE_NORMAL,
    VCC_3V3_PWR_PRE_NORMAL,	
    VCC_5V_PWR_PRE_NORMAL,
    VCC_1V8_PWR_CUR_OFF,
    VCC_3V3_PWR_CUR_OFF,
    VCC_5V_PWR_CUR_OFF,
    VCC_1V8_PWR_OFF_STABLE,
    VCC_3V3_PWR_OFF_STABLE,
    VCC_5V_PWR_OFF_STABLE,
    VCC_2nd_3V3_START,
    VCC_2nd_5V_START,
    VCC_2nd_3V3_PWR_PRE_NORMAL,	
    VCC_2nd_5V_PWR_PRE_NORMAL,
    VCC_2nd_3V3_PWR_CUR_OFF,
    VCC_2nd_5V_PWR_CUR_OFF,
    VCC_2nd_3V3_PWR_OFF_STABLE,
    VCC_2nd_5V_PWR_OFF_STABLE,
};


/*MAIN machine state*/
enum { 
    INIT_STATE,
    IDLE_STATE,
    SAMPLE_STATE,
    TEST_PROC_STATE,
    TEST_END_STATE
};

__packed typedef struct
{
    uint16_t dataLen;
    uint16_t dataType;
    uint32_t StartTimeStamp;
    uint32_t clkFreq;
    uint8_t HRadio;
    uint8_t LRadio;
} SAMPLING_CLK;

__packed typedef struct
{
    uint32_t Clock;
    uint8_t  Pin;
    uint32_t Data0:12;
    uint32_t Data1:12;
    uint32_t Data2:12;
} SAMPLING_DATA;


extern uint8_t workState;
extern uint8_t TePwrSeqState;
extern uint8_t TePreErrState;
extern float curVccAmp;

extern uint8_t TePwrSeqState;
extern uint8_t TePreErrState;

extern uint8_t TeInitErrState;

extern uint8_t TE3V3Stage1EndState;
extern uint8_t TE3V3Stage2EndState;

extern uint8_t TE1V8Stage1EndState;
extern uint8_t TE1V8Stage2EndState;

extern uint8_t nextCourseAmpMeasure;
extern uint8_t pre_nextCourseAmpMeasure;

extern uint32_t g_uClockCnt;
extern uint32_t g_uEndSampleClkCnt;
extern uint32_t g_uStartSampleClkCnt;

extern uint32_t uiDutyCycle;
extern uint32_t uiCycle;
extern uint32_t uiFrequency;

extern void startup_info_report(void);
extern int clockIsStable(void);
extern int curTeVccState(float vcc);
extern uint8_t TePwrState(void);
extern uint8_t TeCurVcc(void);
extern char TeRstLevel(float x);
extern char IoLevel(float x);
extern char ClkLevel(float x);
extern char VppLevel(float x);
extern char isHighZ(float curVcc_v,  float x);
extern char isStateA(float curVcc_v, float x);

extern void te_init_pwr_seq(uint8_t *TePwrSeqState, int curVcc_v, char curClk_v, char curIo_v, char curRst_v);
/*3v3 terminal start course*/
extern void te_3v3_pwr_seq(uint8_t *TePwrSeqState, uint32_t curClock, int curVcc_v, char curClk_v, char curIo_v, char curRst_v);
/*1v8 terminal start course*/
extern void te_1v8_pwr_seq(uint8_t *TePwrSeqState, uint32_t curClock, int curVcc_v, char curClk_v, char curIo_v, char curRst_v);
/*stage 2 5v power seq*/
extern void te_2nd_5v_pwr_seq(uint8_t *TePwrSeqState, uint32_t curClock, int curVcc_v, char curClk_v, char curIo_v, char curRst_v);
/*stage 2 3v3 power seq*/
extern void te_2nd_3v3_pwr_seq(uint8_t *TePwrSeqState, uint32_t curClock, int curVcc_v, char curClk_v, char curIo_v, char curRst_v);

extern float vcc_amp_get(void);
extern void get_duty_cycle(uint32_t uiDutyCycle, uint32_t uiCycle, uint8_t *HRadio, uint8_t *LRadio);
extern float vcc_amp_count(uint16_t pc4, uint16_t pc5);
extern float vcc_amp_get(void);
extern void send_clock_sample_frame(void);
extern void startup_info_report(void);
extern void sys_exception_info_report(char *errInfo);

extern uint8_t g_pwrDownState;
extern void te_3v3_pwr_down_seq(uint8_t *TePwrSeqState, uint32_t curClock, int curVcc_v, char curClk_v, char curIo_v, char curRst_v);
extern void te_1v8_pwr_down_seq(uint8_t *TePwrSeqState, uint32_t curClock, int curVcc_v, char curClk_v, char curIo_v, char curRst_v);
extern void te_2nd_3v3_pwr_down_seq(uint8_t *TePwrSeqState, uint32_t curClock, int curVcc_v, char curClk_v, char curIo_v, char curRst_v);
extern void te_2nd_5v_pwr_down_seq(uint8_t *TePwrSeqState, uint32_t curClock, int curVcc_v, char curClk_v, char curIo_v, char curRst_v);

#endif
