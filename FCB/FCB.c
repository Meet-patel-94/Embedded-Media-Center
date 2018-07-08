#include "LPC17xx.h"
#include "type.h"
//#include "usb.h"
//#include "usbcfg.h"
//#include "usbhw.h"
//#include "usbcore.h"
//#include "usbaudio.h"
#include <stdio.h>
#include "cmsis_os.h"
#include "RTL.H"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include <stdbool.h>
#include "mainmenu320.c"
#include "legends320.c"
#include "fcbsquad320.c"
#include "msn320.c"
#include "messi5320.c"
#define __FI        1                    /* Font index 16x24                */
#define __USE_LCD   0                   /* Uncomment to use the LCD                 */

#define MAIN_MENU   0                   /* Font index 16x24                */
#define MYGAME      1   
#define PHOTOGALLERY  3                 /* Font index 16x24                */
#define MUSICPLAYER   2  
extern unsigned char mainmenu320_pixel_data[];
extern unsigned char legends320_pixel_data [];
extern unsigned char fcbsquad320_pixel_data[];
extern unsigned char msn320_pixel_data[];
extern unsigned char msn5320_pixel_data[];
extern void music(void);

int arrow=1;
bool BACK=false;
uint32_t joyDirection;
char text[10];

void delay(int a){
 long k, count=0;
 for (k=0;k<=a;k++){
  count++;
 }
}

void  DisplayMenu(int pointer){ 
    GLCD_SetBackColor(Red);
    GLCD_SetTextColor(White);
    GLCD_DisplayString(0, 0, __FI, "    MEDIA CENTER            ");
    GLCD_SetTextColor(White);
    GLCD_DisplayString(1, 0, __FI, "    MAIN MENU               ");
    GLCD_SetBackColor(Black);
    GLCD_SetTextColor(White);
    GLCD_DisplayString(4, 0, __FI, "  PHOTO GALLERY");
    GLCD_DisplayString(5, 0, __FI, "  MUSIC PLAYER");
    GLCD_DisplayString(6, 0, __FI, "  MY GAME");
   
    switch(pointer) {

		case PHOTOGALLERY:
           GLCD_DisplayString(4, 0, __FI, "->PHOTO GALLERY");
    break;
		case MUSICPLAYER:
           GLCD_DisplayString(5, 0, __FI, "->MUSIC PLAYER");
    break;
    case MYGAME:
           GLCD_DisplayString(6, 0, __FI, "->MY GAME");
    break;  
		default :
            break;
    }   
    delay(5000000);
}
int imagenum=0;
void ChangeImage(int x){
			switch(x) {  
			case 0:
							GLCD_Clear(Black);
							GLCD_Bitmap(0,0,320,240, GIMP_IMAGE_MAINMENU320_PIXEL_DATA);
      break;
      case 1:
              GLCD_Clear(Black);
							GLCD_Bitmap(50,50,120,90,GIMP_IMAGE_LEGENDS320_PIXEL_DATA);
			break;
			case 2:
              GLCD_Clear(Black);
              GLCD_Bitmap(50,50,100,75,GIMP_IMAGE_MSN320_PIXEL_DATA);
			break;
			case 3:
              GLCD_Clear(Black);
						//	GLCD_Bitmap(50,50,320,240, IMAGE0_PIXEL_DATA);
             GLCD_Bitmap(50,50,120,90,GIMP_IMAGE_MESSI5320_PIXEL_DATA);
			break;
			case 4:
              GLCD_Clear(Black);
              GLCD_Bitmap(50,50,120,90,GIMP_IMAGE_FCBSQUAD320_PIXEL_DATA);
      break;
			default :
			break;
 }   
}

void MusicLoop(){
{
		switch(joyDirection) 
				case KBD_SELECT:
   	    BACK=true;
				GLCD_Clear(Black);
				GLCD_SetBackColor(Blue);
				GLCD_SetTextColor(White);
				GLCD_DisplayString(2, 0, __FI, "Music Now Playing");
				GLCD_SetBackColor(Red);
				GLCD_SetTextColor(White);
				GLCD_DisplayString(5, 0, __FI, "COE718 Media Center");		
}
	music();
}
void GalleryLoop(){
    GLCD_Clear(Black); 
    ChangeImage(imagenum);
    while(!BACK){
 joyDirection = get_button();
    switch(joyDirection) {
		case KBD_LEFT:
    imagenum--;
    if(imagenum<0)
    imagenum=4;
    ChangeImage(imagenum);
    break;
		case KBD_RIGHT:
    imagenum++;
    if(imagenum>4)
		imagenum=0;
		ChangeImage(imagenum);
		break;
		case KBD_SELECT:
    BACK=true;
    GLCD_Clear(Black);
    DisplayMenu(arrow);
    break;                     
    default :
    break;
    }
    }
BACK=false;   
}
void SelectMenu(int pointer){
switch(pointer) {
	 case KBD_DOWN:
	 case KBD_UP:
	 case KBD_LEFT:
	 case KBD_RIGHT: 
			
case PHOTOGALLERY:
GalleryLoop();
break;
      case MUSICPLAYER:
			MusicLoop();
			music();
      break;
      case MYGAME:
      GLCD_Clear(Black);
   		 GLCD_DisplayString(5,  0, __FI, "Press [SELECT] to START");
   		 GLCD_DisplayString(6,  0, __FI, "Press [UP] to EXIT");    
    }
}
#define GALLARY     1   
#define GAME        2                    /* Font index 16x24                */
#define MUSIC       3   
int main (void) {

    //osTimerId timer_0 = osTimerCreate(osTimer(timer0_handle), osTimerPeriodic, (void *)0);   
   // osTimerId timer_1 = osTimerCreate(osTimer(timer1_handle), osTimerPeriodic, (void *)1);   
    //osTimerId timer_2 = osTimerCreate(osTimer(timer2_handle), osTimerPeriodic, (void *)2);   
		KBD_Init();
    GLCD_Init();   
    LED_Init();
    //osTimerStart(timer_0, 40000);
   // osTimerStart(timer_1, 40000);
    //osTimerStart(timer_2, 20000);
   
    GLCD_Clear(Black);
    DisplayMenu(arrow);
   // music();
while(1){
 joyDirection = get_button();
      switch(joyDirection) {
      case KBD_LEFT:
      break;
      case KBD_RIGHT:
      break;
      case KBD_DOWN:
      arrow++;
      if(arrow > 3)
      arrow = 1;
      DisplayMenu(arrow);
      break;
      case KBD_UP:
      arrow--;
      if(arrow < 1)
      arrow = 3;
      DisplayMenu(arrow);
      break;
      case KBD_SELECT:
 SelectMenu(arrow);
			music();
      break;
      default :
      break;
    }   
}
}
