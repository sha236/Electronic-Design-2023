#include "current.h"
#include "arm_math.h"
#include "tim.h"
#include "sogi.h"
#include "oled.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define A_phase_correction 0 //填补上电压电流相位做差和电感造成的误差

#define KP_1 1
#define KI_1 0
#define KD_1 0
#define I_REF 4
#define RATO 1
arm_pid_instance_f32 S_3 = {0};
SPLL_1PH_SOGI sineA = {0};
int count_1 = 0;
float32_t angle_1 = 0;
float32_t I_ref = 0;
float32_t rato = 0;
float32_t percentage = 0;
float32_t real_I_ref = 0; //经过计算单电路的目标电流
float32_t I_target = 0; //实时的目标电流
float32_t I_error = 0;
float32_t current_pid_result = 0;

extern u32 pid_3[3][5];
extern float32_t ADC1_Buf_f32[4];
extern float32_t ADC3_Buf_f32[4];
extern u32 current_num[5];
extern u32 rato_num[3];
extern u32 chip_select; //0: Chip A, 1: Chip B
extern u32 chip_b_mode; //0: RL, 1: Network

#define NETWORK_VOTAGE ADC3_Buf_f32[2]
#define OUTPUT_CURRENT ADC3_Buf_f32[0]
#define OUTPUT_VOTAGE ADC1_Buf_f32[0]
#define CHIPA_CURRENT ADC3_Buf_f32[2]

void CURRENT_PID_INIT(void)
{
    S_3.Kp = KP_1;
    S_3.Ki = KI_1;
    S_3.Kd = KD_1;
    arm_pid_init_f32(&S_3, 1);

    SPLL_1PH_SOGI_reset(&sineA);
    SPLL_1PH_SOGI_config(&sineA,50,20000,20, -20);

    I_ref = I_REF;
    rato = RATO;
}

void UPDATE_CURRENT_NUM(void)
{
    u32 temp[3];
    temp[0] = KP_1*100;
    temp[1] = KI_1*1000;
    temp[2] = KD_1*10000;
    for(int i=0;i<3;i++)
    {
        pid_3[i][4] = temp[i]%10;
        pid_3[i][3] = ((temp[i]- pid_3[i][4])%100)/10;
        pid_3[i][2] = ((temp[i]- pid_3[i][4]- pid_3[i][3]*10)%1000)/100;
        pid_3[i][1] = ((temp[i]- pid_3[i][4]- pid_3[i][3]*10-pid_3[i][2]*100)%10000)/1000;
        pid_3[i][0] = (temp[i]- pid_3[i][4]- pid_3[i][3]*10-pid_3[i][2]*100-pid_3[i][1]*1000)/10000;
    }
}

//将上面define的值赋给按键的存储值
void UPDATE_IREF_RATO_NUM(void)
{
    u32 temp = 0;
    temp = I_REF*10000;
    current_num[4] = temp%10;
    current_num[3] = ((temp- current_num[4])%100)/10;
    current_num[2] = ((temp- current_num[4]- current_num[3]*10)%1000)/100;
    current_num[1] = ((temp- current_num[4]- current_num[3]*10-current_num[2]*100)%10000)/1000;
    current_num[0] = (temp- current_num[4]- current_num[3]*10-current_num[2]*100-current_num[1]*1000)/10000;

    temp = RATO*100;
    rato_num[2] = temp%10;
    rato_num[1] = ((temp-rato_num[2])%100)/10;
    rato_num[0] = (temp-rato_num[2]-rato_num[1]*10)/100;
}

//将按键的存储值赋给计算用的值
void UPDATE_IREF_RATO_PID(void)
{
    rato = (float32_t)(rato_num[0]) + (float32_t)(rato_num[1])*0.1 + (float32_t)(rato_num[2])*0.01;
    I_ref = (float32_t)(current_num[0]) + (float32_t)(current_num[1])*0.1 + (float32_t)(current_num[2])*0.01 + (float32_t)(current_num[3])*0.001 + (float32_t)current_num[4]*0.0001;
}

void UPDATE_CURRENT_PID(void)
{
    S_3.Kp = (float32_t)(pid_3[0][0])*100 + (float32_t)(pid_3[0][1])*10 + (float32_t)(pid_3[0][2]) + (float32_t)(pid_3[0][3])*0.1 + (float32_t)(pid_3[0][4])*0.01;
    S_3.Ki = (float32_t)(pid_3[1][0])*10 + (float32_t)(pid_3[1][1]) + (float32_t)(pid_3[1][2])*0.1 + (float32_t)(pid_3[1][3])*0.01 + (float32_t)(pid_3[1][4])*0.001;
    S_3.Kd = (float32_t)(pid_3[2][0]) + (float32_t)(pid_3[2][1])*0.1 + (float32_t)(pid_3[2][2])*0.01 + (float32_t)(pid_3[2][3])*0.001 + (float32_t)(pid_3[2][4])*0.0001;
    arm_pid_init_f32(&S_3, 0);
}

void COUNT_CURRENT_PID(void)
{
    if (chip_select == 0)
    {
        //单片机1并网工作代码
        SPLL_1PH_SOGI_run(&sineA, NETWORK_VOTAGE);
        angle_1 = (float32_t)(count_1);
        angle_1 *= 0.01570795;
        angle_1 += sineA.theta;
        angle_1 += A_phase_correction;
        percentage = rato/(rato+1);
        real_I_ref = percentage * I_ref;
        I_target = arm_sin_f32(angle_1);
        I_target = I_target * real_I_ref * 1.414;
        I_error = I_target;
        // OLED_ShowNum(0,0,I_error*100, 7,16);
        // OLED_Refresh_Gram();
        I_error -= OUTPUT_CURRENT;
        current_pid_result = arm_pid_f32(&S_3,I_error);
        //过调制保护
        if(current_pid_result*10>1372)TIM1->CCR1 = 1372;
        else if(current_pid_result*10<28) TIM1->CCR1 = 1372;
        else TIM1->CCR1 = current_pid_result*10;        
    }
    else if(chip_b_mode == 0)
    {
        //单片机2带负载工作代码
        SPLL_1PH_SOGI_run(&sineA, NETWORK_VOTAGE);
        angle_1 = (float32_t)(count_1);
        angle_1 *= 0.01570795;
        angle_1 += sineA.theta;
        percentage = rato/(rato+1);
        real_I_ref = I_ref;
        I_target = arm_sin_f32(angle_1);
        I_target = I_target * real_I_ref * 1.414;
        I_error = I_target;
        // OLED_ShowNum(0,0,I_error*100, 7,16);
        // OLED_Refresh_Gram();
        I_error -= OUTPUT_CURRENT;
        I_error -= CHIPA_CURRENT;
        current_pid_result = arm_pid_f32(&S_3,I_error);
        //过调制保护
        if(current_pid_result*10>1372)TIM1->CCR1 = 1372;
        else if(current_pid_result*10<28) TIM1->CCR1 = 1372;
        else TIM1->CCR1 = current_pid_result*10;
    }
    else
    {
        //单片机2并网工作代码
        SPLL_1PH_SOGI_run(&sineA, NETWORK_VOTAGE);
        angle_1 = (float32_t)(count_1);
        angle_1 *= 0.01570795;
        angle_1 += sineA.theta;
        percentage = 1/(rato+1);
        real_I_ref = percentage * I_ref;
        I_target = arm_sin_f32(angle_1);
        I_target = I_target * real_I_ref * 1.414;
        I_error = I_target;
        // OLED_ShowNum(0,0,I_error*100, 7,16);
        // OLED_Refresh_Gram();
        I_error -= OUTPUT_CURRENT;
        current_pid_result = arm_pid_f32(&S_3,I_error);
        //过调制保护
        if(current_pid_result*10>1372)TIM1->CCR1 = 1372;
        else if(current_pid_result*10<28) TIM1->CCR1 = 1372;
        else TIM1->CCR1 = current_pid_result*10;       
    }

    if(count_1==399) count_1 = 0;
    else count_1++;
}

void UPDATE_PHASE(void)
{
    SPLL_1PH_SOGI_run(&sineA, NETWORK_VOTAGE);
}