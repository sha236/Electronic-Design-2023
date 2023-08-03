#include "current_votage.h"
#include "arm_math.h"
#include "tim.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define KP_1 1
#define KI_1 0
#define KD_1 0
#define KP_2 1
#define KI_2 0
#define KD_2 0
#define VREF 24
arm_pid_instance_f32 S_1 = {0};
arm_pid_instance_f32 S_2 = {0};
int count = 0;
float32_t V_ref = 0;
float32_t angle = 0;
float32_t v_error = 0;
float32_t iref = 0;
float32_t i_error = 0;
float32_t pwm_compare = 0;

extern u32 pid_1[3][5];
extern u32 pid_2[3][5];
extern float32_t ADC1_Buf_f32[4];
extern float32_t ADC3_Buf_f32[4];

void CURRENT_VOTAGE_PID_INIT(void)
{
    S_1.Kp = KP_1;
    S_1.Ki = KI_1;
    S_1.Kd = KD_1;
    arm_pid_init_f32(&S_1, 1);

    S_2.Kp = KP_2;
    S_2.Ki = KI_2;
    S_2.Kd = KD_2;
    arm_pid_init_f32(&S_2, 1); 
}


void UPDATE_CURRENT_VOTAGE_NUM(void)
{
    u32 temp[3];
    temp[0] = KP_1*100;
    temp[1] = KI_1*1000;
    temp[2] = KD_1*10000;
    for(int i=0;i<3;i++)
    {
        pid_1[i][4] = temp[i]%10;
        pid_1[i][3] = ((temp[i]- pid_1[i][4])%100)/10;
        pid_1[i][2] = ((temp[i]- pid_1[i][4]- pid_1[i][3]*10)%1000)/100;
        pid_1[i][1] = ((temp[i]- pid_1[i][4]- pid_1[i][3]*10-pid_1[i][2]*100)%10000)/1000;
        pid_1[i][0] = (temp[i]- pid_1[i][4]- pid_1[i][3]*10-pid_1[i][2]*100-pid_1[i][1]*1000)/10000;
    }

    temp[0] = KP_2*100;
    temp[1] = KI_2*1000;
    temp[2] = KD_2*10000;
    for(int i=0;i<3;i++)
    {
        pid_2[i][4] = temp[i]%10;
        pid_2[i][3] = ((temp[i]- pid_2[i][4])%100)/10;
        pid_2[i][2] = ((temp[i]- pid_2[i][4]- pid_2[i][3]*10)%1000)/100;
        pid_2[i][1] = ((temp[i]- pid_2[i][4]- pid_2[i][3]*10-pid_2[i][2]*100)%10000)/1000;
        pid_2[i][0] = (temp[i]- pid_2[i][4]- pid_2[i][3]*10-pid_2[i][2]*100-pid_2[i][1]*1000)/10000;
    }
}

void UPDATE_CURRENT_VOOTAGE_PID(void)
{
    S_1.Kp = pid_1[0][0]*100+pid_1[0][1]*10+pid_1[0][2]+pid_1[0][3]*0.1+pid_1[0][4]*0.01;
    S_1.Ki = pid_1[1][0]*10+pid_1[1][1]+pid_1[1][2]*0.1+pid_1[1][3]*0.01+pid_1[1][4]*0.001;
    S_1.Kd = pid_1[2][0]+pid_1[2][1]*0.1+pid_1[2][2]*0.01+pid_1[2][3]*0.001+pid_1[2][4]*0.0001;
    arm_pid_init_f32(&S_1, 0);

    S_2.Kp = pid_2[0][0]*100+pid_2[0][1]*10+pid_2[0][2]+pid_2[0][3]*0.1+pid_2[0][4]*0.01;
    S_2.Ki = pid_2[1][0]*10+pid_2[1][1]+pid_2[1][2]*0.1+pid_2[1][3]*0.01+pid_2[1][4]*0.001;
    S_2.Kd = pid_2[2][0]+pid_2[2][1]*0.1+pid_2[2][2]*0.01+pid_2[2][3]*0.001+pid_2[2][4]*0.0001;
    arm_pid_init_f32(&S_2, 0);   
}

void COUNT_CURRENT_VOTAGE_PID(void)
{
    angle = (float32_t)(count);
    angle = angle*0.01570795;
    V_ref = arm_sin_f32(angle);
    V_ref = V_ref*VREF*1.414;
    v_error = V_ref - ADC1_Buf_f32[0];
    iref = arm_pid_f32(&S_1, v_error);
    i_error = iref - ADC3_Buf_f32[0];
    pwm_compare = arm_pid_f32(&S_2, i_error);
    TIM1->CCR1 = pwm_compare*10;
    if(count==399) count = 0;
    else count++;
}