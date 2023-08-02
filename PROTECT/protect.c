#include "protect.h"
#include "main.h"
#include "arm_math.h"

#define MAX_SAFE_CURRENT 8
extern float32_t ADC1_Buf_f32[4];


void PROTECT_GUARD(void)
{
    if(ADC1_Buf_f32[1]>MAX_SAFE_CURRENT) 
    {
        HOLD_MOS();
        while (1)
        {
            HAL_Delay(500-1);
        } 
    }
}