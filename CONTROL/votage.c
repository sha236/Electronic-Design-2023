#include "votage.h"
#include "arm_math.h"
#include "tim.h"
#include "oled.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define KP 1
#define KI 0
#define KD 0
#define VREF 24
#define OUTPUT_VOTAGE ADC1_Buf_f32[0]

arm_pid_instance_f32 S_4 = {0};
int count_3 = 0;
float32_t angle_3 = 0;
float32_t V_ref_3 = 0;
float V_error = 0;
float32_t PID_output = 0;

extern u32 pid_4[3][5];
extern float32_t ADC1_Buf_f32[4];
extern float32_t ADC3_Buf_f32[4];

void VOTAGE_PID_INIT(void)
{
    S_4.Kp = KP;
    S_4.Ki = KI;
    S_4.Kd = KD;
    arm_pid_init_f32(&S_4, 1);
}

void UPDATE_VOTAGE_NUM(void)
{
    u32 temp[3];
    temp[0] = KP*100;
    temp[1] = KI*1000;
    temp[2] = KD*10000;
    for(int i=0;i<3;i++)
    {
        pid_4[i][4] = temp[i]%10;
        pid_4[i][3] = ((temp[i]- pid_4[i][4])%100)/10;
        pid_4[i][2] = ((temp[i]- pid_4[i][4]- pid_4[i][3]*10)%1000)/100;
        pid_4[i][1] = ((temp[i]- pid_4[i][4]- pid_4[i][3]*10-pid_4[i][2]*100)%10000)/1000;
        pid_4[i][0] = (temp[i]- pid_4[i][4]- pid_4[i][3]*10-pid_4[i][2]*100-pid_4[i][1]*1000)/10000;
    }
}

void UPDATE_VOTAGE_PID(void)
{
    S_4.Kp = (float32_t)(pid_4[0][0])*100 + (float32_t)(pid_4[0][1])*100 + (float32_t)(pid_4[0][2]) + (float32_t)(pid_4[0][3])*0.1 + (float32_t)(pid_4[0][4])*0.01;
    S_4.Ki = (float32_t)(pid_4[1][0])*10 + (float32_t)(pid_4[1][1]) + (float32_t)(pid_4[1][2])*0.1 + (float32_t)(pid_4[1][3])*0.01 + (float32_t)(pid_4[1][4])*0.001;
    S_4.Kd = (float32_t)(pid_4[2][0]) + (float32_t)(pid_4[2][1]*0.1) + (float32_t)(pid_4[2][2])*0.01 + (float32_t)(pid_4[2][3])*0.001 + (float32_t)(pid_4[2][4])*0.0001;
    arm_pid_init_f32(&S_4, 0);
}

void COUNT_VOTAGE_PID(void)
{
    angle_3 = (float32_t)(count_3);
    angle_3 = angle_3*0.01570795;
    V_ref_3 = arm_sin_f32(angle_3);
    V_ref_3 = V_ref_3*VREF*1.414;
    V_error = V_ref_3 - OUTPUT_VOTAGE;
    PID_output = arm_pid_f32(&S_4, V_error);
    if(PID_output*10>1372) TIM1->CCR1=1372; 
    else if(PID_output*10<28) TIM1->CCR1=28;
    else TIM1->CCR1 = PID_output*10;

    if(count_3==399) count_3 = 0;
    else count_3++;
}