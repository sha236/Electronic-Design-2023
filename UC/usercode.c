#include "usercode.h"
#include "main.h"
#include "oled.h"
#include "adc.h"
#include "tim.h"
#include "button.h"
#include "button_display.h"
#include "current_votage.h"
#include "arm_math.h"

extern uint16_t ADC1_Buf[4];
extern float32_t ADC1_Buf_f32[4];
extern float32_t pwm_compare;

void USER_INIT(void);
void ADC_BUF_TO_F32(void);
void ADC_BUF_TO_F32_1(void);
void ADC_BUF_TO_F32_2(void);

void StartDefaultTask(void *argument)
{
    USER_INIT();

    while(1)
    {
        READ_BUTTON_A();
        READ_BUTTON_B();
        UPDATE_CURRENT_VOOTAGE_PID();
        DISPLAY_PID();
        OLED_Refresh_Gram();
        HAL_Delay(50-1);
    }
}

void USER_INIT(void)
{
    OLED_Init();
    DISPLAY_PID_SYMBLE();
    UPDATE_CURRENT_VOTAGE_NUM();
    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC1_Buf,4);
    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    ADC_BUF_TO_F32();
    COUNT_CURRENT_VOTAGE_PID();
}

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)  //中断回调函数 20k
// {
//     if (htim->Instance == TIM14) {
//         HAL_IncTick();
//     }

//     if(htim==&htim2)
//     {
//         TIM1->CCR1 = pwm_compare*10;
//     }

// }

void ADC_BUF_TO_F32(void)
{
    ADC_BUF_TO_F32_1();
    ADC_BUF_TO_F32_2();
}

//采集电压
void ADC_BUF_TO_F32_1(void)
{
    ADC1_Buf_f32[0] = (ADC1_Buf[0]/4095)*3;
}

//采集电流
void ADC_BUF_TO_F32_2(void)
{
    ADC1_Buf_f32[1] = (ADC1_Buf[1]/4095)*3;
}

