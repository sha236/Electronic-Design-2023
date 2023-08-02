#include "main.h"
#include "oled.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

void UINT32_TO_OLED(u8 x,u8 y,u32 num,u8 size)
{
    u32 temp1;
    u32 num1, num2, num3, num4;
    temp1 = (num*3300)/4095;
    num4 = temp1 % 10;
    num3 = ((temp1-num4)%100)/10;
    num2 = ((temp1-num4-num3*10)%1000)/100;
    num1 = (temp1-num4-num3*10-num2*100)/1000;

    OLED_ShowNum(x,y,num1,1,size);
    OLED_ShowChar(x+7,y,'.',size,1);
    OLED_ShowNum(x+10,y,num2,1,size);
    OLED_ShowNum(x+17,y,num3,1,size);
    OLED_ShowNum(x+24,y,num4,1,size);
}

void PID_TO_OLED(u32 pid[3][5])
{
    u32 temp1 = 0;
    temp1 = pid[0][0]*100+pid[0][1]*10+pid[0][2];
    OLED_ShowNum(15,17,temp1, 3,16);
    OLED_ShowChar(40,17,'.',16,1);
    OLED_ShowNum(44, 17,pid[0][3],1,16);
    OLED_ShowNum(51, 17,pid[0][4],1,16);
    temp1 = pid[1][0]*10+pid[1][1];
    OLED_ShowNum(15,32,temp1, 2,16);
    OLED_ShowChar(31,32,'.', 16, 1);
    OLED_ShowNum(35,32,pid[1][2], 1, 16);
    OLED_ShowNum(42,32,pid[1][3], 1, 16);
    OLED_ShowNum(49,32,pid[1][4], 1, 16);
    OLED_ShowNum(15,47,pid[2][0], 1,16);
    OLED_ShowChar(22,47,'.', 16, 1);
    OLED_ShowNum(29,47,pid[2][1], 1,16);
    OLED_ShowNum(36,47,pid[2][2], 1,16);
    OLED_ShowNum(43,47,pid[2][3], 1,16);
    OLED_ShowNum(50,47,pid[2][4], 1,16);
}

void CURRENT_TO_OLED(u32 current[5])
{
    OLED_ShowNum(74,17,current[0],1,16);
    OLED_ShowChar(81,17,'.',16,1);
    OLED_ShowNum(88,17,current[1],1,16);
    OLED_ShowNum(95,17,current[2],1,16);
    OLED_ShowNum(102,17,current[3],1,16);
    OLED_ShowNum(109,17,current[4],1,16);
}

void RATO_TO_OLED(u32 rato[3])
{
    OLED_ShowNum(90,32,rato[0],1,16);
    OLED_ShowChar(97,32,'.',16,1);
    OLED_ShowNum(104,32,rato[1],1,16);
    OLED_ShowNum(111,32,rato[2],1,16);    
}

