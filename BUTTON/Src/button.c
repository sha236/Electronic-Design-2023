#include "button.h"
#include "main.h"
#include "oled.h"


//B组按键管脚逻辑映射
#define RESET_BR1() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET)
#define SET_BR1() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET)
#define RESET_BR2() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET)
#define SET_BR2() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET)
#define RESET_BR3() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET)
#define SET_BR3() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET)
#define RESET_BR4() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET)
#define SET_BR4() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET)

#define READ_BC1() HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)
#define READ_BC2() HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)
#define READ_BC3() HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)
#define READ_BC4() HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11)

//A组按键管脚逻辑映射
#define RESET_AR1() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET)
#define SET_AR1() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET)
#define RESET_AR2() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET)
#define SET_AR2() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET)
#define RESET_AR3() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET)
#define SET_AR3() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET)
#define RESET_AR4() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET)
#define SET_AR4() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET)

#define READ_AC1() HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)
#define READ_AC2() HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)
#define READ_AC3() HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8)
#define READ_AC4() HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0)

//要求是新状态X_state减去原来的状态temp_state
#define DOWN -1
#define UP 1

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t


int temp_state = 0;//存储读取前按键状态的Buffer
int A_state[4][4] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int B_state[4][4] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

//PID控制
u32 pid_1[3][5] = {0}; //kp:pid_1[0]999.99~0.01, ki:pid_1[1]99.999~0.001, kd:pid_1[2]9.9999~0.0001
u32 pid_2[3][5] = {0};
u32 pid_3[3][5] = {0};
u32 pid_4[3][5] = {0};
u32 pid_5[3][5] = {0};
u32 pid_6[3][5] = {0};

u8 ctrl_pid[6] = {0};
u8 ctrl_num[6] = {0};
u32 ctrl_display = 3;
u32 mode_select = 2;    //0:Single 1:Double 2:单环 3:PR控制
u32 current_num_select = 0; //0~5:检测
u32 current_num[5] = {2,0,0,0,0};
u32 rato_num_select = 0; //0~3:检测
u32 rato_num[3] = {1,0,0};
u32 chip_select = 0; //0: Chip A, 1: Chip B
u32 chip_b_mode = 0; //0: RL, 1: Network

float kp_1 = 0;
float ki_1 = 0;
float kd_1 = 0;

float kp_2 = 0;
float ki_2 = 0;
float kd_2 = 0;

float kp_3 = 0;
float ki_3 = 0;
float kd_3 = 0;

float kp_4 = 0;
float ki_4 = 0;
float kd_4 = 0;

void PID_CONVERT(int pid[3][5], float *kp, float *ki, float *kd)
{
    *kp = pid[0][0]*100+pid[0][1]*10+pid[0][2]+pid[0][3]*0.1+pid[0][4]*0.01;
    *ki = pid[1][0]*10+pid[1][1]+pid[1][2]*0.1+pid[1][3]*0.01+pid[1][4]*0.001;
    *kd = pid[2][0]+pid[2][1]*0.1+pid[2][2]*0.01+pid[2][3]*0.001+pid[2][4]*0.0001;
}




//按键按下后触发的回调函数

    //AR1
    void AR1_AC1(void)
    {
        if(ctrl_pid[ctrl_display]!=0) ctrl_pid[ctrl_display]--;        
    }

    void AR1_AC2(void)
    {
        if(ctrl_num[ctrl_display]!=5) ctrl_num[ctrl_display]++;         
    }

    void AR1_AC3(void)
    {
        switch (ctrl_display)
        {
        case 0:
            if(pid_1[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]!=9) pid_1[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]++;        
            break;
        case 1:
            if(pid_2[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]!=9) pid_2[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]++;        
            break;
        case 2:
            if(pid_3[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]!=9) pid_3[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]++;        
            break;
        case 3:
            if(pid_4[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]!=9) pid_4[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]++;        
            break;
        case 4:
            if(pid_5[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]!=9) pid_5[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]++;        
            break;
        case 5:
            if(pid_6[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]!=9) pid_6[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]++;        
            break;
        }
        
    }

    void AR1_AC4(void)
    {
        if(ctrl_display!=5) ctrl_display++;         
    }

    //AR2
    void AR2_AC1(void)
    {
        if(ctrl_pid[ctrl_display]!=2) ctrl_pid[ctrl_display]++;        
    }

    void AR2_AC2(void)
    {
        if(ctrl_num[ctrl_display]!=0) ctrl_num[ctrl_display]--;        
    }

    void AR2_AC3(void)
    {
        switch (ctrl_display)
        {
        case 0:
            if(pid_1[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]!=0) pid_1[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]--;        
            break;
        case 1:
            if(pid_2[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]!=0) pid_2[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]--;        
            break;
        case 2:
            if(pid_3[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]!=0) pid_3[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]--;        
            break;
        case 3:
            if(pid_4[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]!=0) pid_4[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]--;        
            break;
        case 4:
            if(pid_5[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]!=0) pid_5[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]--;        
            break;
        case 5:
            if(pid_6[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]!=0) pid_6[ctrl_pid[ctrl_display]][ctrl_num[ctrl_display]]--;        
            break;
        }    
    }

    void AR2_AC4(void)
    {
        if(ctrl_display!=0) ctrl_display--;
       
    }

    //AR3
    void AR3_AC1(void)
    {
        if(mode_select!=3) mode_select++;
    }

    void AR3_AC2(void)
    {
        if(current_num_select!=4) current_num_select++;
                
    }

    void AR3_AC3(void)
    {
        if(current_num[current_num_select]!=9) current_num[current_num_select]++;
                
    }

    void AR3_AC4(void)
    {
        if(rato_num_select!=2) rato_num_select++;
        
    }

    //AR4
    void AR4_AC1(void)
    {
        if(mode_select!=0) mode_select--;
    }

    void AR4_AC2(void)
    {
        if(current_num_select!=0) current_num_select--; 
    }

    void AR4_AC3(void)
    {
        if(current_num[current_num_select]!=0) current_num[current_num_select]--;
       
    }

    void AR4_AC4(void)
    {
        if(rato_num_select!=2) rato_num_select--;
       
    }

void READ_BUTTON_A_R1(void)
{
    RESET_AR1();
    temp_state = A_state[0][0];
    A_state[0][0] = READ_AC1();
    if(A_state[0][0] - temp_state == UP) AR1_AC1();

    temp_state = A_state[0][1];
    A_state[0][1] = READ_AC2();
    if(A_state[0][1] - temp_state == UP) AR1_AC2();

    temp_state = A_state[0][2];
    A_state[0][2] = READ_AC3();
    if(A_state[0][2] - temp_state == UP) AR1_AC3();

    temp_state = A_state[0][3];
    A_state[0][3] = READ_AC4();
    if(A_state[0][3] - temp_state == UP) AR1_AC4();
    SET_AR1();


}

void READ_BUTTON_A_R2(void)
{
    RESET_AR2();
    temp_state = A_state[1][0];
    A_state[1][0] = READ_AC1();
    if(A_state[1][0] - temp_state == UP) AR2_AC1();

    temp_state = A_state[1][1];
    A_state[1][1] = READ_AC2();
    if(A_state[1][1] - temp_state == UP) AR2_AC2();

    temp_state = A_state[1][2];
    A_state[1][2] = READ_AC3();
    if(A_state[1][2] - temp_state == UP) AR2_AC3();

    temp_state = A_state[1][3];
    A_state[1][3] = READ_AC4();
    if(A_state[1][3] - temp_state == UP) AR2_AC4();
    SET_AR2();
}

void READ_BUTTON_A_R3(void)
{
    RESET_AR3();
    temp_state = A_state[2][0];
    A_state[2][0] = READ_AC1();
    if(A_state[2][0] - temp_state == UP) AR3_AC1();

    temp_state = A_state[2][1];
    A_state[2][1] = READ_AC2();
    if(A_state[2][1] - temp_state == UP) AR3_AC2();

    temp_state = A_state[2][2];
    A_state[2][2] = READ_AC3();
    if(A_state[2][2] - temp_state == UP) AR3_AC3();

    temp_state = A_state[2][3];
    A_state[2][3] = READ_AC4();
    if(A_state[2][3] - temp_state == UP) AR3_AC4();
    SET_AR3();
}

void READ_BUTTON_A_R4(void)
{
    RESET_AR4();
    temp_state = A_state[3][0];
    A_state[3][0] = READ_AC1();
    if(A_state[3][0] - temp_state == UP) AR4_AC1();

    temp_state = A_state[3][1];
    A_state[3][1] = READ_AC2();
    if(A_state[3][1] - temp_state == UP) AR4_AC2();

    temp_state = A_state[3][2];
    A_state[3][2] = READ_AC3();
    if(A_state[3][2] - temp_state == UP) AR4_AC3();

    temp_state = A_state[3][3];
    A_state[3][3] = READ_AC4();
    if(A_state[3][3] - temp_state == UP) AR4_AC4();
    SET_AR4();
}

void READ_BUTTON_A(void)
{
    READ_BUTTON_A_R1();
    READ_BUTTON_A_R2();
    READ_BUTTON_A_R3();
    READ_BUTTON_A_R4();
}

//按键按下后触发的回调函数
    //BR1
    void BR1_BC1(void)
    {
        if(rato_num[rato_num_select]!=9) rato_num[rato_num_select]++;
       
    }

    void BR1_BC2(void)
    {
        if(chip_select!=1) chip_select++;
        
    }

    void BR1_BC3(void)
    {
        if(chip_b_mode!=1) chip_b_mode++;
        
    }

    void BR1_BC4(void)
    {
        OLED_ShowString(0,0,"BR1_BC4",16);         
    }

    //BR2
    void BR2_BC1(void)
    {
        if(rato_num[rato_num_select]!=0) rato_num[rato_num_select]--;
        
    }

    void BR2_BC2(void)
    {
        if(chip_select!=0) chip_select--;
        
    }

    void BR2_BC3(void)
    {
        if(chip_b_mode!=0) chip_b_mode--;
             
    }

    void BR2_BC4(void)
    {
        OLED_ShowString(0,0,"BR2_BC4",16);       
    }

    //BR3
    void BR3_BC1(void)
    {
        OLED_ShowString(0,0,"BR3_BC1",16);
        
    }

    void BR3_BC2(void)
    {
        OLED_ShowString(0,0,"BR3_BC2",16);
        
    }

    void BR3_BC3(void)
    {
        OLED_ShowString(0,0,"BR3_BC3",16);
        
    }

    void BR3_BC4(void)
    {
        OLED_ShowString(0,0,"BR3_BC4",16);         
    }

    //BR4
    void BR4_BC1(void)
    {
        OLED_ShowString(0,0,"BR4_BC1",16);
       
    }

    void BR4_BC2(void)
    {
        OLED_ShowString(0,0,"BR4_BC2",16);
       
    }

    void BR4_BC3(void)
    {
        OLED_ShowString(0,0,"BR4_BC3",16);
         
    }

    void BR4_BC4(void)
    {
        OLED_ShowString(0,0,"BR4_BC4",16);        
    }

void READ_BUTTON_B_R1(void)
{
    RESET_BR1();
    temp_state = B_state[0][0];
    B_state[0][0] = READ_BC1();
    if(B_state[0][0] - temp_state == UP) BR1_BC1();

    temp_state = B_state[0][1];
    B_state[0][1] = READ_BC2();
    if(B_state[0][1] - temp_state == UP) BR1_BC2();

    temp_state = B_state[0][2];
    B_state[0][2] = READ_BC3();
    if(B_state[0][2] - temp_state == UP) BR1_BC3();

    temp_state = B_state[0][3];
    B_state[0][3] = READ_BC4();
    if(B_state[0][3] - temp_state == UP) BR1_BC4();
    SET_BR1();


}

void READ_BUTTON_B_R2(void)
{
    RESET_BR2();
    temp_state = B_state[1][0];
    B_state[1][0] = READ_BC1();
    if(B_state[1][0] - temp_state == UP) BR2_BC1();

    temp_state = B_state[1][1];
    B_state[1][1] = READ_BC2();
    if(B_state[1][1] - temp_state == UP) BR2_BC2();

    temp_state = B_state[1][2];
    B_state[1][2] = READ_BC3();
    if(B_state[1][2] - temp_state == UP) BR2_BC3();

    temp_state = B_state[1][3];
    B_state[1][3] = READ_BC4();
    if(B_state[1][3] - temp_state == UP) BR2_BC4();
    SET_BR2();
}

void READ_BUTTON_B_R3(void)
{
    RESET_BR3();
    temp_state = B_state[2][0];
    B_state[2][0] = READ_BC1();
    if(B_state[2][0] - temp_state == UP) BR3_BC1();

    temp_state = B_state[2][1];
    B_state[2][1] = READ_BC2();
    if(B_state[2][1] - temp_state == UP) BR3_BC2();

    temp_state = B_state[2][2];
    B_state[2][2] = READ_BC3();
    if(B_state[2][2] - temp_state == UP) BR3_BC3();

    temp_state = B_state[2][3];
    B_state[2][3] = READ_BC4();
    if(B_state[2][3] - temp_state == UP) BR3_BC4();
    SET_BR3();
}

void READ_BUTTON_B_R4(void)
{
    RESET_BR4();
    temp_state = B_state[3][0];
    B_state[3][0] = READ_BC1();
    if(B_state[3][0] - temp_state == UP) BR4_BC1();

    temp_state = B_state[3][1];
    B_state[3][1] = READ_BC2();
    if(B_state[3][1] - temp_state == UP) BR4_BC2();

    temp_state = B_state[3][2];
    B_state[3][2] = READ_BC3();
    if(B_state[3][2] - temp_state == UP) BR4_BC3();

    temp_state = B_state[3][3];
    B_state[3][3] = READ_BC4();
    if(B_state[3][3] - temp_state == UP) BR4_BC4();
    SET_BR4();
}

void READ_BUTTON_B(void)
{
    READ_BUTTON_B_R1();
    READ_BUTTON_B_R2();
    READ_BUTTON_B_R3();
    READ_BUTTON_B_R4();
}
