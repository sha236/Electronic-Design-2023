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
#include "current.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

extern uint16_t ADC1_Buf[4];
extern float32_t ADC1_Buf_f32[4];
extern float32_t pwm_compare;
extern u32 mode_select;

void USER_INIT(void);
void ADC_BUF_TO_F32(void);
void ADC_BUF_TO_F32_1(void);
void ADC_BUF_TO_F32_2(void);
void ADC_BUF_TO_F32_3(void);

void StartDefaultTask(void *argument)
{
    USER_INIT();

    while(1)
    {
        READ_BUTTON_A();
        READ_BUTTON_B();
        UPDATE_CURRENT_VOOTAGE_PID();
        UPDATE_CURRENT_PID();
        UPDATE_IREF_RATO_PID();
        DISPLAY_PID();
        OLED_Refresh_Gram();
        HAL_Delay(50-1);
    }
}

void USER_INIT(void)
{
    OLED_Init();
    DISPLAY_PID_SYMBLE();
    CURRENT_VOTAGE_PID_INIT();
    CURRENT_PID_INIT();
    UPDATE_CURRENT_VOTAGE_NUM();
    UPDATE_CURRENT_NUM();
    UPDATE_IREF_RATO_NUM();
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
        // Version.Test_Receive
        // OLED_ShowNum(0,0,ADC1_Buf[0],4,16);
        // OLED_ShowNum(0,13,ADC1_Buf[1],4,16);
        // OLED_ShowNum(0,26,ADC1_Buf[2],4,16);
        // OLED_ShowNum(0,39,ADC1_Buf[3],4,16);
        
        // OLED_ShowNum(50,0,ADC3_Buf[0],4,16);
        // OLED_ShowNum(50,13,ADC3_Buf[1],4,16);
        // OLED_ShowNum(50,26,ADC3_Buf[2],4,16);
        // OLED_ShowNum(50,39,ADC3_Buf[3],4,16);
        // OLED_Refresh_Gram();
        
        ADC_BUF_TO_F32();
        PROTECT_GUARD();
        switch (mode_select)
        {
        case 0:
            COUNT_CURRENT_VOTAGE_PID();
            break;
        case 1:
            COUNT_CURRENT_PID();
            break;
        }
    }
}

void ADC_BUF_TO_F32(void)
{
    ADC_BUF_TO_F32_1();
    ADC_BUF_TO_F32_2();
    ADC_BUF_TO_F32_3();
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
    ADC3_Buf_f32[0] = (float32_t)(ADC3_Buf[0]);
    ADC3_Buf_f32[0] *= 0.00293;
}

//采集输出电压
void ADC_BUF_TO_F32_3(void)
{
    ADC3_Buf_f32[2] = (float32_t)(ADC3_Buf[2]);
    ADC3_Buf_f32[2] *= 0.0007326;
}
