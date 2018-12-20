#include <stdio.h>
#include <reg51.h>
#include <absacc.h>
#define uchar unsigned char
#define uint unsigned int
#define CMDER XBYTE[0x8003]
#define PA XBYTE[0x0000]
char i=0,m=0,n,x,y,flag;
code char tab[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
sbit P1_0=P1^0;/*K2*/
sbit P1_1=P1^1;/*LED warning:0 on;1 off*/ 
sbit P1_2=P1^2;/*speaker*/
sbit P1_3=P1^3;/*LED working condition:0 on;1 off*/

void delay_1ms(uint q)/*delay 1ms experienced function*/
{
	uint a,b;
	for(a=0;a<q;a++)
	for(b=0;b<120;b++);
}

void warning_1Hz()
{
	P1_1=0;
	P1_2=1;
	delay_1ms(500);
	P1_1=1;
	P1_2=0;
	delay_1ms(500);
}

void K1_INT0() interrupt 0/*K1 interruption*/
{
	m++;
	n=m%2;
	switch(n)
	{
		case 0:
		{
			P1_3=1;
			while(1);
		}/*no count*/
		case 1:P1_3=0;break;/*count*/
	}
}
		
void II_INT1() interrupt 2/*infrared interruption*/
{
	i++;
	PA=tab[i];
	P1_2=1;
	P1_1=0;
	delay_1ms(1000);/*delay 1s*/
	P1_2=0;
	P1_1=1;
}

void judge_K2()
{
	x=P1_0;
	if(x==y)
	{
		flag=0;
	}
	else
	{
		y=x;
		flag=1;
	}
}

main()
{
	EA=1;
	EX0=1;
	IT0=1;/*int0 settings*/
	EX1=1;
	IT1=1;
	flag=0;
	x=0;
	y=0;
	CMDER=0x80;
	P1_1=1;
	P1_2=0;
	P1_3=0;
	countcircle:
	PA=tab[0];
	for(i=0;i<9;);/*when i<9,wait to infrared interruption.if i=9,1Hz warning,and wait to K2 interruption */
	while(flag==1)
	{
		P1_1=0;
		warning_1Hz();
		judge_K2();
	}
	P1_1=1;
	goto countcircle;
}