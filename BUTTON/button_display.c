#include "button_display.h"
#include "oled.h"
#include "customoled.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

extern u32 pid_1[3][5];
extern u32 pid_2[3][5];
extern u32 pid_3[3][5];
extern u32 pid_4[3][5];
extern u32 ctrl_display;
extern u32 mode_select;

extern u32 current_num[5];
extern u32 rato_num[3];


void DISPLAY_PID_SYMBLE(void)
{
    OLED_ShowChar(0,0,'P',16,1);
    OLED_ShowChar(7,0,'I',16,1);
    OLED_ShowChar(14,0,'D',16,1);
    OLED_ShowChar(40,0,'M',16,1);
    OLED_ShowChar(47,0,'O',16,1);
    OLED_ShowChar(54,0,'D',16,1);
    OLED_ShowChar(61,0,'E',16,1);
    OLED_ShowChar(0,17,'K',16,1);
    OLED_ShowChar(8,17,'p',16,1);
    OLED_ShowChar(0,32,'K',16,1);
    OLED_ShowChar(8,32,'i',16,1);
    OLED_ShowChar(0,47,'K',16,1);
    OLED_ShowChar(8,47,'d',16,1);
    OLED_Refresh_Gram();
    OLED_ShowChar(60,17,'I',16,1);
    OLED_ShowChar(67,17,'r',16,1);
    OLED_ShowChar(60,32,'R',16,1);
    OLED_ShowChar(67,32,'a',16,1);
    OLED_ShowChar(74,32,'t',16,1);
    OLED_ShowChar(81,32,'o',16,1);
}

void DISPLAY_PID(void)
{
    //Display Mode
    if(mode_select==0)
    {
        OLED_ShowChar(70,0,'S',16,1);
        OLED_ShowChar(77,0,'i',16,1);
        OLED_ShowChar(84,0,'n',16,1);
        OLED_ShowChar(91,0,'g',16,1);
        OLED_ShowChar(97,0,'l',16,1);
        OLED_ShowChar(104,0,'e',16,1);
    } 
    else{
        OLED_ShowChar(70,0,'D',16,1);
        OLED_ShowChar(77,0,'o',16,1);
        OLED_ShowChar(84,0,'u',16,1);
        OLED_ShowChar(91,0,'b',16,1);
        OLED_ShowChar(101,0,'l',16,1);
        OLED_ShowChar(107,0,'e',16,1);
    }
    //Display PID
    switch (ctrl_display)
    {
    case 0:
        OLED_ShowChar(25,0,'0',16,1);
        PID_TO_OLED(pid_1);
        break;
    
    case 1:
        OLED_ShowChar(25,0,'1',16,1);
        PID_TO_OLED(pid_2);
        break;
    case 2:
    OLED_ShowChar(25,0,'2',16,1);
        PID_TO_OLED(pid_3);
        break;
    case 3:
    OLED_ShowChar(25,0,'3',16,1);
        PID_TO_OLED(pid_4);
        break;
    }
    //Display Current
    CURRENT_TO_OLED(current_num);
    //Display Rato
    RATO_TO_OLED(rato_num);
}