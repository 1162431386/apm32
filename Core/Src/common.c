#include "main.h"
#include "stdlib.h"
#include "common.h"

uint32_t g_terminalClass  = CLASS_POWER_OFF_DEV;
float g_vccValue = 0.0;
uint8_t clkStableFlag = 0;

void init_vcc_terminal_R(uint8_t RV)
{
    if(RV == 50)
		{
		    HAL_GPIO_WritePin(VCC_T50K_GPIO_Port, VCC_T50K_Pin, GPIO_PIN_SET);
		} else if (RV == 30) {
		    HAL_GPIO_WritePin(VCC_T30K_GPIO_Port, VCC_T30K_Pin, GPIO_PIN_SET);
		} else if (RV == 20) {
        HAL_GPIO_WritePin(VCC_T20K_GPIO_Port, VCC_T20K_Pin, GPIO_PIN_SET);		
		} else{
        HAL_GPIO_WritePin(VCC_T10K_GPIO_Port, VCC_T10K_Pin, GPIO_PIN_SET);		
		}
		
     return;
}

int clockIsStable(void)
{
    return clkStableFlag;
}

int curTeVccState(float vcc)
{
    if((vcc >= 4.5) && (vcc <= 5.5))
			  g_terminalClass = CLASS_A_5V_DEV;
		else if((vcc >= 2.7) && (vcc <= 3.3))
			  g_terminalClass = CLASS_B_3V3_DEV;
		else if ((vcc >= 1.62) && (vcc <= 1.98))
			  g_terminalClass = CLASS_C_1V8_DEV;
		else
			  if (vcc < 0.4)
				    g_terminalClass = CLASS_POWER_OFF_DEV;
				else
					  g_terminalClass = CLASS_POWER_INVAILD_DEV;
		g_vccValue = vcc;
		return g_terminalClass;
}

uint8_t TePwrState(void)
{
    return g_terminalClass;
}

uint8_t TeCurVcc(void)
{
    return g_vccValue;
}

char TeRstLevel(float x)
{
    switch(TePwrState()){
			case CLASS_A_5V_DEV:
				if((x >= TeCurVcc() - 0.7) && 
					 (x <= TeCurVcc()))
				   return 'H';
				else if(x <= 0.6)
					 return 'L';
				return 'E';
			case CLASS_B_3V3_DEV:
			case CLASS_C_1V8_DEV:
				if((x >= TeCurVcc() * 0.8) && 
					 (x <= TeCurVcc()))
				   return 'H';
				else if(x <= TeCurVcc() * 0.2)
					 return 'L';
				return 'E';	
			case CLASS_POWER_OFF_DEV:
				if(x <= 0.4)
					return 'L';
				else
					return 'E';
			default:
				break;			
		}
		return 'E' ;
}

char IoLevel(float x)
{
    switch(TePwrState()){
			case CLASS_A_5V_DEV:
				if((x >= 3.8) && 
					 (x <= TeCurVcc()))
				   return 'H';
				else if(x <= 0.4)
					 return 'L';
				return 'E';
			case CLASS_B_3V3_DEV:
				if((x >= TeCurVcc() * 0.7) && 
					 (x <= TeCurVcc()))
				   return 'H';
				else if(x <= 0.4)
					 return 'L';
				return 'E';					
			case CLASS_C_1V8_DEV:
				if((x >= TeCurVcc() * 0.7) && 
					 (x <= TeCurVcc()))
				   return 'H';
				else if(x <= 0.3)
					 return 'L';
				return 'E';	
			case CLASS_POWER_OFF_DEV:
				if(x <= 0.4)
					return 'L';
				else
					return 'E';
			default:
				return 'E';		
		}
}

char ClkLevel(float x)
{
    switch(TePwrState()){
			case CLASS_A_5V_DEV:
				if((x >= TeCurVcc() * 0.7) && 
					 (x <= TeCurVcc()))
				   return 'H';
				else if(x <= 0.5)
					 return 'L';
				return 'E';
			case CLASS_B_3V3_DEV:
			case CLASS_C_1V8_DEV:
				if((x >= TeCurVcc() * 0.7) && 
					 (x <= TeCurVcc()))
				   return 'H';
				else if(x <= TeCurVcc() * 0.2)
					 return 'L';
				return 'E';			
			case CLASS_POWER_OFF_DEV:
				if(x <= 0.4)
					return 'L';
				else
					return 'E';				
			default:
				return 'E';		
		}
}

char VppLevel(float x)  /*programing vpp/C6, see <ts_102221v15 chap 5.1.3>*/
{
    if((x >= 4.5) && (x <= 5.5))
			  return 'H';
    return 'E';		
}


char isHighZ(float curVcc_v,  float x)
{
	  float Amp = (VOL(x)/3300)*1000000;
	  if(TePwrState() == CLASS_A_5V_DEV){
			if(((VOL(x) > 3.8 * 0.7 * curVcc_v) && (VOL(x) < (curVcc_v + 0.3))) && (Amp < 20))
				return 'Z';
		}else if((TePwrState() == CLASS_B_3V3_DEV) || (TePwrState() == CLASS_C_1V8_DEV)){
			if(((VOL(x) > 0.7 * curVcc_v) && (VOL(x) < (curVcc_v + 0.3))) && (Amp < 20))
				return 'Z';
		}

	  return 'E';
}
		
char isStateA(float curVcc_v, float x)
{
	  float Amp = (VOL(x)/3300)*1000000;
	  if((TePwrState() == CLASS_A_5V_DEV) || (TePwrState() == CLASS_B_3V3_DEV) || (TePwrState() == CLASS_C_1V8_DEV)){
			if((VOL(x) <= curVcc_v * 0.15 ) && (Amp <= 1000))
				return 'A';
		}
	  return 'E';
}
		
