#pragma once

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

//将32bitADC读取到的数字转化成电压显示在OLED屏幕上
//x,y :起点坐标
//len :数字位数
//size:字体大小
//mode:Ä£Ê½	0,填充模式；1，叠加模式
//num:(0~4095);
void UINT32_TO_OLED(u8 x,u8 y,u32 num,u8 size);

void PID_TO_OLED(u32 pid[3][5]);

void CURRENT_TO_OLED(u32 currrent[5]);

void RATO_TO_OLED(u32 rato[3]);
