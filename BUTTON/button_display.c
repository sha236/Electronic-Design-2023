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
extern u32 pid_5[3][5];
extern u32 pid_6[3][5];
extern u32 ctrl_display;
extern u32 mode_select;

extern u32 current_num[5];
extern u32 rato_num[3];
extern u32 chip_select; //0: Chip A, 1: Chip B
extern u32 chip_b_mode; //0: RL, 1: Network


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
    OLED_ShowChar(60,17,'I',16,1);
    OLED_ShowChar(67,17,'r',16,1);
    OLED_ShowChar(60,32,'R',16,1);
    OLED_ShowChar(67,32,'a',16,1);
    OLED_ShowChar(74,32,'t',16,1);
    OLED_ShowChar(81,32,'o',16,1);
    OLED_ShowChar(60,47,'C',16,1);
    OLED_ShowChar(67,47,'h',16,1);
    OLED_ShowChar(74,47,'i',16,1);
    OLED_ShowChar(81,47,'p',16,1);
    OLED_ShowChar(88,47,'.',16,1);
    OLED_ShowChar(104,47,'.',16,1);

    OLED_Refresh_Gram();
}

void DISPLAY_PID(void)
{
    //Display Chip
    if(chip_select == 0) OLED_ShowChar(95,47,'A',16,1);
    else OLED_ShowChar(95,47,'B',16,1);
    //Display CHip.B Mode
    if(chip_b_mode==0) OLED_ShowChar(110,47,'0',16,1);
    else OLED_ShowChar(110,47,'1',16,1);
    //Display Mode
    switch(mode_select)
    {
        case 0:
            OLED_ShowChar(70,0,'S',16,1);
            OLED_ShowChar(77,0,'i',16,1);
            OLED_ShowChar(84,0,'n',16,1);
            OLED_ShowChar(91,0,'g',16,1);
            OLED_ShowChar(97,0,'l',16,1);
            OLED_ShowChar(104,0,'e',16,1);
        break;

        case 1:
            OLED_ShowChar(70,0,'D',16,1);
            OLED_ShowChar(77,0,'o',16,1);
            OLED_ShowChar(84,0,'u',16,1);
            OLED_ShowChar(91,0,'b',16,1);
            OLED_ShowChar(101,0,'l',16,1);
            OLED_ShowChar(107,0,'e',16,1);
        break;

        case 2:
            OLED_ShowChar(70,0,'V',16,1);
            OLED_ShowChar(77,0,'o',16,1);
            OLED_ShowChar(84,0,'t',16,1);
            OLED_ShowChar(91,0,'a',16,1);
            OLED_ShowChar(101,0,'g',16,1);
            OLED_ShowChar(107,0,'e',16,1);
        break;

        case 3:
            OLED_ShowChar(70,0,'P',16,1);
            OLED_ShowChar(77,0,'R',16,1);
            OLED_ShowChar(84,0,'C',16,1);
            OLED_ShowChar(91,0,'t',16,1);
            OLED_ShowChar(101,0,'r',16,1);
            OLED_ShowChar(107,0,'l',16,1);
        break;       
    } 

    //Display PID
    switch (ctrl_display)
    {
    case 0:
        OLED_ShowChar(25,0,'1',16,1);
        PID_TO_OLED(pid_1);
        break;
    
    case 1:
        OLED_ShowChar(25,0,'2',16,1);
        PID_TO_OLED(pid_2);
        break;
    case 2:
    OLED_ShowChar(25,0,'3',16,1);
        PID_TO_OLED(pid_3);
        break;
    case 3:
    OLED_ShowChar(25,0,'4',16,1);
        PID_TO_OLED(pid_4);
        break;
    case 4:
    OLED_ShowChar(25,0,'5',16,1);
        PID_TO_OLED(pid_5);
        break;   
    case 5:
    OLED_ShowChar(25,0,'6',16,1);
        PID_TO_OLED(pid_6);
        break;    
    }
    //Display Current
    //CURRENT_TO_OLED(current_num);
    //Display Rato
    //RATO_TO_OLED(rato_num);

    OLED_Refresh_Gram();
}