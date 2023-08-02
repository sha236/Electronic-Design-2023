#include "usercode.h"
#include "main.h"
#include "oled.h"
#include "adc.h"
#include "tim.h"
#include "button.h"
#include "button_display.h"
#include "current_votage.h"
#include "arm_math.h"
#include "protect.h"

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
        //DISPLAY_PID();
        OLED_Refresh_Gram();
        HAL_Delay(50-1);
    }
}

void USER_INIT(void)
{
    OLED_Init();
    DISPLAY_PID_SYMBLE();
    UPDATE_CURRENT_VOTAGE_NUM();
    HOLD_MOS();
    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC1_Buf,4);
    HAL_ADC_Start_DMA(&hadc3,(uint32_t *)ADC3_Buf,4);
    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    ENABLE_MOS();
    
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance == ADC3)
    {
        ADC_BUF_TO_F32();
        OLED_ShowNum(0,0,ADC1_Buf_f32[0]*1000,4,16);
        OLED_ShowNum(0,20,ADC1_Buf_f32[1]*1000,4,16);
        OLED_Refresh_Gram();
        PROTECT_GUARD();
        COUNT_CURRENT_VOTAGE_PID();
    }
}

void ADC_BUF_TO_F32(void)
{
    ADC_BUF_TO_F32_1();
    ADC_BUF_TO_F32_2();
}

//采集电压
void ADC_BUF_TO_F32_1(void)
{
    ADC1_Buf_f32[0] = (float32_t)(ADC1_Buf[0]);
    ADC1_Buf_f32[0] *= 0.0007326; 
}

//采集电流
void ADC_BUF_TO_F32_2(void)
{
    ADC1_Buf_f32[1] = (float32_t)(ADC3_Buf[0]);
    ADC1_Buf_f32[1] *= 0.00293;
}

