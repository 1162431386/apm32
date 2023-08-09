#include "main.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

#include "testCase.h"



CASE_STATE execTestCase_1(uint8_t *sigSampleList);
CASE_STATE execTestCase_2(uint8_t *sigSampleList);
CASE_STATE execTestCase_3(uint8_t *sigSampleList);
CASE_STATE execTestCase_4(uint8_t *sigSampleList);

void endTestCase_1(void);
void endTestCase_2(void);
void endTestCase_3(void);
void endTestCase_4(void);



USER_TESTCASE_CMD_T TestCaseFs[] =
{
    {1, execTestCase_1},
		{2, execTestCase_2},
		{3, execTestCase_3},
		{4, execTestCase_4},
};

USER_TESTCASE_RESULT_T TestCaseResultFs[] =
{
    {1, endTestCase_1},
    {2, endTestCase_2},
		{3, endTestCase_3},
    {4, endTestCase_4},				
};


CASE_STATE execTestCase(uint16_t caseNumble, uint8_t *sigSampleList)
{
    return (*TestCaseFs[caseNumble].func)(sigSampleList);
}

void endTestCase(uint16_t caseNumble, CASE_STATE *caseTotalState)
{
   (*TestCaseResultFs[caseNumble].func)();
        return;
}

/*******************testcase 1*************************/
CASE_STATE execTestCase_1(uint8_t *sigSampleList)
{
    static CASE_STATE caseState = {0};
    return caseState;
}

void endTestCase_1(void)
{
    CASE_FINAL_RESULT_PKT_T resultPkt = {0};

    if(TePwrSeqState != VCC_5V_NORMAL_STATE)
		{
        resultPkt.resultCode = stm32_htons(0x8000);
			  HAL_GPIO_WritePin(ERROR_GPIO_Port, ERROR_Pin, GPIO_PIN_SET);
		}
    else
		{
        resultPkt.resultCode = stm32_htons(0x0000);
		}

    resultPkt.pktLen = stm32_htons(sizeof(CASE_FINAL_RESULT_PKT_T));
    resultPkt.pktType = stm32_htons(0x8203);
    resultPkt.caseNum = stm32_htons(1);

    if(resultPkt.resultCode != 0)
        sprintf((char *)resultPkt.resultDesc, "terminal power on sequence current state[%d], pre-state[%d]", TePwrSeqState, TePreErrState);

		CDC_Transmit_FS((uint8_t *)&resultPkt, resultPkt.pktLen);
		
    return;
}

/*******************testcase 2*************************/
CASE_STATE execTestCase_2(uint8_t *sigSampleList)
{
    static CASE_STATE caseState = {0};
    return caseState;
}

void endTestCase_2()
{
   CASE_FINAL_RESULT_PKT_T resultPkt = {0};

    if(TePwrSeqState != VCC_3V3_NORMAL_STATE)
		{
        resultPkt.resultCode = stm32_htons(0x8000);
			  HAL_GPIO_WritePin(ERROR_GPIO_Port, ERROR_Pin, GPIO_PIN_SET);
		}
    else
		{
        resultPkt.resultCode = stm32_htons(0x0000);
		}

    resultPkt.pktLen = stm32_htons(sizeof(CASE_FINAL_RESULT_PKT_T));
    resultPkt.pktType = stm32_htons(0x8203);
    resultPkt.caseNum = stm32_htons(3);

    if(resultPkt.resultCode != 0)
        sprintf((char *)resultPkt.resultDesc, "terminal power on sequence current state[%d], pre-state[%d]", TePwrSeqState, TePreErrState);

		CDC_Transmit_FS((uint8_t *)&resultPkt, resultPkt.pktLen);
		
    return;
}

/*******************testcase 3*************************/
CASE_STATE execTestCase_3(uint8_t *sigSampleList)
{
    static CASE_STATE caseState = {0};
    return caseState;
}

void endTestCase_3()
{
    CASE_FINAL_RESULT_PKT_T resultPkt = {0};

    if(g_pwrDownState != VCC_2nd_5V_PWR_OFF_STABLE)
		{
        resultPkt.resultCode = stm32_htons(0x8000);
			  HAL_GPIO_WritePin(ERROR_GPIO_Port, ERROR_Pin, GPIO_PIN_SET);
		}
    else
		{
        resultPkt.resultCode = stm32_htons(0x0000);
		}

    resultPkt.pktLen = stm32_htons(sizeof(CASE_FINAL_RESULT_PKT_T));
    resultPkt.pktType = stm32_htons(0x8203);
    resultPkt.caseNum = stm32_htons(3);

    if(resultPkt.resultCode != 0)
        sprintf((char *)resultPkt.resultDesc, "terminal power off state[%d], pre-state[%d]", TePwrSeqState, TePreErrState);

		CDC_Transmit_FS((uint8_t *)&resultPkt, resultPkt.pktLen);
		
    return;
}

/*******************testcase 4*************************/
CASE_STATE execTestCase_4(uint8_t *sigSampleList)
{
    static CASE_STATE caseState = {0};
    return caseState;
}

void endTestCase_4()
{
   CASE_FINAL_RESULT_PKT_T resultPkt = {0};

    if((g_pwrDownState != VCC_2nd_3V3_PWR_OFF_STABLE))
		{
        resultPkt.resultCode = stm32_htons(0x8000);
			  HAL_GPIO_WritePin(ERROR_GPIO_Port, ERROR_Pin, GPIO_PIN_SET);
		}
    else
		{
        resultPkt.resultCode = stm32_htons(0x0000);
		}

    resultPkt.pktLen = stm32_htons(sizeof(CASE_FINAL_RESULT_PKT_T));
    resultPkt.pktType = stm32_htons(0x8203);
    resultPkt.caseNum = stm32_htons(4);

    if(resultPkt.resultCode != 0)
        sprintf((char *)resultPkt.resultDesc, "terminal  power off state[%d]", g_pwrDownState);
		
		CDC_Transmit_FS((uint8_t *)&resultPkt, resultPkt.pktLen);
		
    return;
}
