#ifndef TESTCASE_H
#define TESTCASE_H
#include "main.h"
#include "common.h"

#define CASE_INIT      0
#define CASE_SUCCESS   1
#define CASE_FAIL    (-1)


__packed typedef struct{
  int clockState;
  int vccState;
  int rstState;
  int ioZStae;
}CASE_STATE;

__packed typedef struct
{
    uint16_t type;  /* case number */
    CASE_STATE (*func)(uint8_t *sigSampleList); /* exec func*/
} USER_TESTCASE_CMD_T;

__packed typedef struct
{
    uint16_t type;  /* case number */
    void (*func)(void); /* exec func*/
} USER_TESTCASE_RESULT_T;

__packed typedef struct
{
    uint16_t pktLen;
    uint16_t pktType;
    uint32_t version;
    uint16_t uidLen;
    uint8_t uid[6];
}SYS_START_UP_INFO_T;

__packed typedef struct
{
    uint16_t pktLen;
    uint16_t pktType;
    uint32_t version;
    uint16_t errInfoLen;
    char errInfo[256];
}SYS_EXCEPTION_INFO_T;

__packed typedef struct
{
    uint16_t pktLen;
    uint16_t pktType;
    uint16_t caseNo;
}START_TEST_CASE_CMD_T;

typedef struct{
    uint16_t pktLen;
    uint16_t pktType;
    uint16_t caseNum;
    uint16_t resultCode;
    char resultDesc[256];
}CASE_FINAL_RESULT_PKT_T;

extern CASE_STATE execTestCase(uint16_t caseNumble, uint8_t *sigSampleList);
extern void endTestCase(uint16_t caseNumble, CASE_STATE *caseTotalState);
#endif /*TESTCASE_H*/
