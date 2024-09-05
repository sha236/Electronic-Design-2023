#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "main.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

//oled的显存 存放格式如下
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 	

u8 OLED_GRAM[128][8];

//更新显存到OLED
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址0~7
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置-列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置-列高地址  
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

//向显存写入一个字节
//dat：要写入的数据/命令
//cmd：数据/命令标志，0表示命令，1表示数据
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	OLED_RS=cmd; 
	OLED_CS=0;		  
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK=0;
		if(dat&0x80)OLED_SDIN=1;
		else OLED_SDIN=0;
		OLED_SCLK=1;
		dat<<=1;   
	}				 
	OLED_CS=1;		  
	OLED_RS=1;   	  
} 

//开启oled显示 
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

//关闭oled显示
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	

//清屏函数，效果和没点亮一致
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();
}

//画点
//x:0~127
//y:0~63
//t为1时显示，为0时消灭
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

//填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0，清空；1，填充；
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();
}

//在指定位置显示一个字符，包括部分字符
//x:0~127
//y:0~63
//mode:0, 反白显示；1，正常显示；	
//size:选择字体 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		
	chr=chr-' ';//µÃµ½Æ«ÒÆºóµÄÖµ		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	
		else if(size==16)temp=asc2_1608[chr][t];	
		else if(size==24)temp=asc2_2412[chr][t];	
		else return;								
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
 			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}		
//显示数字
//x,y :起点坐标
//len :数字位数
//size:字体大小
//mode:Ä£Ê½	0,填充模式；1，叠加模式
//num:ÊýÖµ(0~4294967295);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 

//显示字符串
//x,y: 起点坐标
//size: 字体大小
// *p：字符串起始地址
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	
}	

//初始化OLED

void OLED_Init(void)
{ 	 		 
    OLED_SDIN=1;
    OLED_SCLK=1;

    OLED_CS=1;
    OLED_RS=1;	 

    OLED_RST=0;
    HAL_Delay(100);
    OLED_RST=1; 
    HAL_Delay(100);

    OLED_WR_Byte(0xAE,OLED_CMD); 
    OLED_WR_Byte(0xD5,OLED_CMD);
    OLED_WR_Byte(80,OLED_CMD);   
    OLED_WR_Byte(0xA8,OLED_CMD); 
    OLED_WR_Byte(0X3F,OLED_CMD); 
    OLED_WR_Byte(0xD3,OLED_CMD); 
    OLED_WR_Byte(0X00,OLED_CMD); 

    OLED_WR_Byte(0x40,OLED_CMD);

    OLED_WR_Byte(0x8D,OLED_CMD); 
    OLED_WR_Byte(0x14,OLED_CMD); 
    OLED_WR_Byte(0x20,OLED_CMD); 
    OLED_WR_Byte(0x02,OLED_CMD); 
    OLED_WR_Byte(0xA1,OLED_CMD); 
    OLED_WR_Byte(0xC0,OLED_CMD);
    OLED_WR_Byte(0xDA,OLED_CMD);
    OLED_WR_Byte(0x12,OLED_CMD); 

    OLED_WR_Byte(0x81,OLED_CMD); 
    OLED_WR_Byte(0xEF,OLED_CMD); 
    OLED_WR_Byte(0xD9,OLED_CMD);
    OLED_WR_Byte(0xf1,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD); 
    OLED_WR_Byte(0x30,OLED_CMD);

    OLED_WR_Byte(0xA4,OLED_CMD);
    OLED_WR_Byte(0xA6,OLED_CMD);
    OLED_WR_Byte(0xAF,OLED_CMD);


    OLED_Clear();


}