#include "votage.h"
#include "arm_math.h"
#include "tim.h"
#include "oled.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define KP 6.95
#define KI 6
#define KD 0
#define VREF 20.97385
#define OUTPUT_VOTAGE ADC1_Buf_f32[0]
#define DEFAULT_RATO 0

arm_pid_instance_f32 S_4 = {0};
int count_3 = 0;
float32_t angle_3 = 0;
float32_t V_ref_3 = 0;
float V_error = 0;
float32_t PID_output = 0;

float32_t debug_votage = 0;
float32_t debug_rate = 0;
float32_t temp_rato = 0;

int spwm_group[400]={
	33,99,165,231,297,362,428,494,559,624,690,755,819,884,948,1013,1076,
1140,1203,1266,1329,1392,1454,1515,1577,1638,1698,1758,1818,1877,1936,1994,2052,
2110,2166,2223,2278,2333,2388,2442,2495,2548,2600,2652,2703,2753,2802,2851,2899,
2946,2993,3039,3084,3129,3172,3215,3257,3298,3339,3378,3417,3455,3492,3528,3564,
3598,3632,3664,3696,3727,3757,3786,3814,3841,3868,3893,3917,3940,3963,3984,4005,
4024,4042,4060,4076,4092,4106,4119,4132,4143,4153,4163,4171,4178,4184,4190,4194,
4197,4199,4200,4200,4199,4197,4194,4190,4184,4178,4171,4163,4153,4143,4132,4119,
4106,4092,4076,4060,4042,4024,4005,3984,3963,3940,3917,3893,3868,3841,3814,3786,
3757,3727,3696,3664,3632,3598,3564,3528,3492,3455,3417,3378,3339,3298,3257,3215,
3172,3129,3084,3039,2993,2946,2899,2851,2802,2753,2703,2652,2600,2548,2495,2442,
2388,2333,2278,2223,2166,2110,2052,1994,1936,1877,1818,1758,1698,1638,1577,1515,
1454,1392,1329,1266,1203,1140,1076,1013,948,884,819,755,690,624,559,494,
428,362,297,231,165,99,33,-33,-99,-165,-231,-297,-362,-428,-494,-559,
-624,-690,-755,-819,-884,-948,-1013,-1076,-1140,-1203,-1266,-1329,-1392,-1454,-1515,-1577,
-1638,-1698,-1758,-1818,-1877,-1936,-1994,-2052,-2110,-2166,-2223,-2278,-2333,-2388,-2442,-2495,
-2548,-2600,-2652,-2703,-2753,-2802,-2851,-2899,-2946,-2993,-3039,-3084,-3129,-3172,-3215,-3257,
-3298,-3339,-3378,-3417,-3455,-3492,-3528,-3564,-3598,-3632,-3664,-3696,-3727,-3757,-3786,-3814,
-3841,-3868,-3893,-3917,-3940,-3963,-3984,-4005,-4024,-4042,-4060,-4076,-4092,-4106,-4119,-4132,
-4143,-4153,-4163,-4171,-4178,-4184,-4190,-4194,-4197,-4199,-4200,-4200,-4199,-4197,-4194,-4190,
-4184,-4178,-4171,-4163,-4153,-4143,-4132,-4119,-4106,-4092,-4076,-4060,-4042,-4024,-4005,-3984,
-3963,-3940,-3917,-3893,-3868,-3841,-3814,-3786,-3757,-3727,-3696,-3664,-3632,-3598,-3564,-3528,
-3492,-3455,-3417,-3378,-3339,-3298,-3257,-3215,-3172,-3129,-3084,-3039,-2993,-2946,-2899,-2851,
-2802,-2753,-2703,-2652,-2600,-2548,-2495,-2442,-2388,-2333,-2278,-2223,-2166,-2110,-2052,-1994,
-1936,-1877,-1818,-1758,-1698,-1638,-1577,-1515,-1454,-1392,-1329,-1266,-1203,-1140,-1076,-1013,
-948,-884,-819,-755,-690,-624,-559,-494,-428,-362,-297,-231,-165,-99,-33,
};
float32_t square_buffer = 0;
float32_t votage_rms = 0;
float32_t clip_result = 0;

extern u32 pid_4[3][5];
extern u32 pid_6[3][5];
extern float32_t ADC1_Buf_f32[4];
extern float32_t ADC3_Buf_f32[4];

void VOTAGE_PID_INIT(void)
{
    S_4.Kp = KP;
    S_4.Ki = KI*0.01;
    S_4.Kd = KD;
    arm_pid_init_f32(&S_4, 1);
}

void UPDATE_RATO_NUM(void)
{
    u32 temp[3];
    temp[0] = 0;
    temp[1] = 0;
    temp[2] = DEFAULT_RATO;
    for(int i=0;i<3;i++)
    {
        pid_6[i][4] = temp[i]%10;
        pid_6[i][3] = ((temp[i]- pid_6[i][4])%100)/10;
        pid_6[i][2] = ((temp[i]- pid_6[i][4]- pid_6[i][3]*10)%1000)/100;
        pid_6[i][1] = ((temp[i]- pid_6[i][4]- pid_6[i][3]*10-pid_6[i][2]*100)%10000)/1000;
        pid_6[i][0] = (temp[i]- pid_6[i][4]- pid_6[i][3]*10-pid_6[i][2]*100-pid_6[i][1]*1000)/10000;
    }
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
    S_4.Ki *= 0.01;
    S_4.Kd = (float32_t)(pid_4[2][0]) + (float32_t)(pid_4[2][1]*0.1) + (float32_t)(pid_4[2][2])*0.01 + (float32_t)(pid_4[2][3])*0.001 + (float32_t)(pid_4[2][4])*0.0001;
    arm_pid_init_f32(&S_4, 0);
}

void UPDATE_RATO_PID(void)
{
    temp_rato = (float32_t)(pid_6[2][0]) + (float32_t)(pid_6[2][1]*0.1) + (float32_t)(pid_6[2][2])*0.01 + (float32_t)(pid_6[2][3])*0.001 + (float32_t)(pid_6[2][4])*0.0001;
}

void COUNT_VOTAGE_PID(void)
{
    /*
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
    */
    if(count_3!=399)
    {
        //SQUARE BUFFER
        square_buffer += OUTPUT_VOTAGE*OUTPUT_VOTAGE; 
    }
    else
    {
        //SQUARE BUFFER
        square_buffer += OUTPUT_VOTAGE*OUTPUT_VOTAGE; 
        square_buffer = square_buffer/400;
        
        //Debug1
        debug_votage = square_buffer;
        
        //PID COUNT
        V_error = VREF*VREF - square_buffer;
        V_error *=0.05;
        PID_output = arm_pid_f32(&S_4, V_error);
        PID_output*=0.001;

        //TEMP RATO INPUT
        temp_rato = PID_output*100;

        //Debug1
        debug_rate = PID_output*100;



        if(PID_output>=0.99) PID_output = 0.99;
        else if(PID_output<=0.01) PID_output = 0.01;

        square_buffer = 0;
    }
    TIM1->CCR1 = 700.f + 0.16666667* PID_output * (float32_t)(spwm_group[count_3]);
    TIM1->CCR3 = 700.f + 0.16666667* PID_output * (float32_t)(spwm_group[count_3]);

    // TIM1->CCR1 = 700.f + 0.16666667* 0.8 * (float32_t)(spwm_group[count_3]);
    // TIM1->CCR3 = 700.f + 0.16666667* 0.8 * (float32_t)(spwm_group[count_3]);

    if(count_3==399) count_3 = 0;
    else count_3++; 
}