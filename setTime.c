#include "settime.h"
#include "tft.h"
#include "text.h"
#include "key.h"
#include "malloc.h"
#include "string.h"
#include "menu.h"
#include "timecaculate.h"
#include "stm32l0xx_hal.h"
#include "sysdata.h"
#include "maintask.h"

TIME_STR realTime={
	.year = 18,
	.month = 03,
	.day = 19,
	.hour = 13,
	.minute = 50,
};


static void CursorChange(unsigned char Xcur,unsigned char Ycur,unsigned short FColor,unsigned char value,unsigned short WColor){
	HZS_T *b = mymalloc(16);
	unsigned char *pNum = mymalloc(8);
	LCD_Fill(Xcur,Ycur,Xcur+16,Ycur+16,FColor);
	b->x0 = Xcur;
	b->y0 = Ycur;
	b->color = WColor;
	b->mode = 1;
	b->size = 16;
	*pNum = value/10+'0';
	*(pNum+1) = value%10+'0';
	*(pNum+2) = 0;
	b->str = pNum;
	Show_Str(b);
	myfree(pNum);
	myfree(b);
}

int timeSetPage(void){
	TIME_STR *pTime;
	HZS_T b;
	char rev=1,menuInit=1,cursor=0,tmp1=0;
	char *data;
	data = (char *)mymalloc(24);
	if(data==NULL){
		return 0;
	}
	strcpy(data,"2018-03-04 12:00:00");
	pTime = &realTime;
	*(data+2) = pTime->year/10+'0';
	*(data+3) = pTime->year%10+'0';
	*(data+5) = pTime->month/10+'0';
	*(data+6) = pTime->month%10+'0';
	*(data+8) = pTime->day/10+'0';
	*(data+9) = pTime->day%10+'0';
	*(data+10) = 0;
	*(data+11) = pTime->hour/10+'0';
	*(data+12) = pTime->hour%10+'0';
	*(data+14) = pTime->minute/10+'0';
	*(data+15) = pTime->minute%10+'0';
	*(data+17) = pTime->second/10+'0';
	*(data+18) = pTime->second%10+'0';
	while(rev){
		if(menuInit){
			menuInit = 0;
			DispColor(WHITE);
			DrawLine(24,30,100,30,BLUE);
			DrawLine(24,50,100,50,BLUE);
			DrawLine(24,70,100,70,BLUE);
			b.x0 = 24;
			b.y0 = 12;
			b.color = BLUE;
			b.mode = 1;
			b.size = 16;
			switch(sysSet.LanguageType){
				case 0:b.str = (unsigned char *)Menu2[1].Ch_Simplified;break;
				case 1:b.str = (unsigned char *)Menu2[1].Ch_Traditional;break;
				case 2:b.str = (unsigned char *)Menu2[1].Ch_English;break;
				default:break;
			}
			Show_Str(&b);
			b.x0 = 24;
			b.y0 = 32;
			b.color = BLUE;
			b.mode = 1;
			b.size = 16;
			b.str = data;
			Show_Str(&b);
			b.x0 = 24;
			b.y0 = 52;
			b.color = BLUE;
			b.mode = 1;
			b.size = 16;
			b.str = data+11;
			Show_Str(&b);
			b.x0 = 0;
			b.y0 = 78;
			b.color = RED;
			b.mode = 1;
			b.size = 12;
			switch(sysSet.LanguageType){
				case 0:b.str = "(ÇëÌîÐ´¶ÔÓ¦µÄÊý×Ö)";break;
				case 1:b.str = "(ÕˆÌîŒ‘Œ¦‘ªµÄ”µ×Ö)";break;
				case 2:b.str = "(Please Input Number)";break;
				default:break;
			}
			Show_Str(&b);
			tmp1 = pTime->year;
			CursorChange(40,32,BLUE,tmp1,WHITE);
		}
		SYSTask();
		if(key.TrgFlag){
			key.TrgFlag = 0;
			switch(key.TrgValue){
				case KEY_UP:
					switch(cursor){
						case 0:
							tmp1++;
							if(tmp1>99){
								tmp1 = 0;
							}
							CursorChange(40,32,BLUE,tmp1,WHITE);
							pTime->year = tmp1;
						break;
						case 1:
							tmp1++;
							if(tmp1>12){
								tmp1 = 0;
							}
							CursorChange(64,32,BLUE,tmp1,WHITE);
							pTime->month = tmp1;
						break;
						case 2:
							switch(pTime->month){
								case 1:
								case 3:
								case 5:
								case 7:
									tmp1++;
									if(tmp1>31){
										tmp1 = 1;
									}	
								break;
								case 2:
									tmp1++;
									if((pTime->year%100)&&(pTime->year%4==0)){
										if(tmp1>29){
											tmp1 = 1;
										}
									}else{
										if(tmp1>28){
											tmp1 = 1;
										}
									}
								break;
								case 4:
								case 6:
								case 8:
								case 10:
								case 12:
									tmp1++;
									if(tmp1>30){
										tmp1 = 1;
									}
								break;
								default:break;
							}
							CursorChange(88,32,BLUE,tmp1,WHITE);
							pTime->day = tmp1;
						break;
						case 3:
							tmp1++;
							if(tmp1>23){
								tmp1 = 0;
							}
							CursorChange(24,52,BLUE,tmp1,WHITE);
							pTime->hour = tmp1;
						break;
						case 4:
							tmp1++;
							if(tmp1>59){
								tmp1 = 0;
							}
							CursorChange(48,52,BLUE,tmp1,WHITE);
							pTime->minute = tmp1;
						break;
						case 5:
							tmp1++;
							if(tmp1>59){
								tmp1 = 0;
							}
							CursorChange(72,52,BLUE,tmp1,WHITE);
							pTime->second = tmp1;
						break;
					}
				break;
				case KEY_DOWN:
					switch(cursor){
						case 0:
							if(tmp1==18){
								tmp1 = 100;
							}
							tmp1--;
							CursorChange(40,32,BLUE,tmp1,WHITE);
							pTime->year = tmp1;
						break;
						case 1:
							if(tmp1==1){
								tmp1 = 13;
							}
							tmp1--;
							CursorChange(64,32,BLUE,tmp1,WHITE);
							pTime->month = tmp1;
						break;
						case 2:
							switch(pTime->month){
								case 1:
								case 3:
								case 5:
								case 7:
									if(tmp1==1){
										tmp1 = 32;
									}
									tmp1--;
								break;
								case 2:
									if(tmp1==1){
										if((pTime->year%100)&&(pTime->year%4==0)){
											tmp1 = 30;
										}else{
											tmp1 = 29;
										}
									}
	                tmp1--;
								break;
								case 4:
								case 6:
								case 8:
								case 10:
								case 12:
									if(tmp1==1){
										tmp1 = 31;
									}
									tmp1--;
								break;
								default:break;
							}
							CursorChange(88,32,BLUE,tmp1,WHITE);
							pTime->day = tmp1;
						break;
						case 3:
							if(tmp1==0){
								tmp1 = 24;
							}
							tmp1--;
							CursorChange(24,52,BLUE,tmp1,WHITE);
							pTime->hour = tmp1;
						break;
						case 4:
							if(tmp1==0){
								tmp1 = 60;
							}
							tmp1--;
							CursorChange(48,52,BLUE,tmp1,WHITE);
							pTime->minute = tmp1;
						break;
						case 5:
							if(tmp1==0){
								tmp1 = 60;
							}
							tmp1--;
							CursorChange(72,52,BLUE,tmp1,WHITE);
							pTime->second = tmp1;
						break;
						default:break;
					}
				break;
				case KEY_LEFT:
					switch(cursor){
						case 0:
							CursorChange(40,32,WHITE,tmp1,BLUE);
							tmp1 = pTime->second;
							CursorChange(72,52,BLUE,tmp1,WHITE);
							cursor = 5;
						break;
						case 1:
							CursorChange(64,32,WHITE,tmp1,BLUE);
							tmp1 = pTime->year;
							CursorChange(40,32,BLUE,tmp1,WHITE);
							cursor = 0;
						break;
						case 2:
							CursorChange(88,32,WHITE,tmp1,BLUE);
							tmp1 = pTime->month;
							CursorChange(64,32,BLUE,tmp1,WHITE);
							cursor = 1;
						break;
						case 3:
							CursorChange(24,52,WHITE,tmp1,BLUE);
							tmp1 = pTime->day;
							CursorChange(88,32,BLUE,tmp1,WHITE);
							cursor = 2;
						break;
						case 4:
							CursorChange(48,52,WHITE,tmp1,BLUE);
							tmp1 = pTime->hour;
							CursorChange(24,52,BLUE,tmp1,WHITE);
							cursor = 3;
						break;
						case 5:
							CursorChange(72,52,WHITE,tmp1,BLUE);
							tmp1 = pTime->minute;
							CursorChange(48,52,BLUE,tmp1,WHITE);
							cursor = 4;
						break;
						default:break;
					}
				break;
				case KEY_RIGHT:
					switch(cursor){
						case 0:
							CursorChange(40,32,WHITE,tmp1,BLUE);
							tmp1 = pTime->month;
							CursorChange(64,32,BLUE,tmp1,WHITE);
							cursor = 1;
						break;
						case 1:
							CursorChange(64,32,WHITE,tmp1,BLUE);
							pTime->month = tmp1;
							tmp1 = pTime->day;
							CursorChange(88,32,BLUE,tmp1,WHITE);
							cursor = 2;
						break;
						case 2:
							CursorChange(88,32,WHITE,tmp1,BLUE);
							pTime->day = tmp1;
							tmp1 = pTime->hour;
							CursorChange(24,52,BLUE,tmp1,WHITE);
							cursor = 3;
						break;
						case 3:
							CursorChange(24,52,WHITE,tmp1,BLUE);
							pTime->hour = tmp1;
							tmp1 = pTime->minute;
							CursorChange(48,52,BLUE,tmp1,WHITE);
							cursor = 4;
						break;
						case 4:
							CursorChange(48,52,WHITE,tmp1,BLUE);
							pTime->minute = tmp1;
							tmp1 = pTime->second;
							CursorChange(72,52,BLUE,tmp1,WHITE);
							cursor = 5;
						break;
						case 5:
							CursorChange(72,52,WHITE,tmp1,BLUE);
							pTime->second = tmp1;
							tmp1 = pTime->year;
							CursorChange(40,32,BLUE,tmp1,WHITE);
							cursor = 0;
						break;
						default:break;
					}
				break;
				case KEY_BACK:
					rev = 0;
				break;
				case KEY_ENTER:
					rev = 0;
				break;
				default:break;
			}
		}
	}
	myfree(data);
	return 0;
}



















