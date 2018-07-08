#include "LPC17xx.h"                    	/* LPC17xx definitions */
#include "type.h"
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbaudio.h"
#include <stdbool.h>
#include <stdio.h>  
#include <stdlib.h>
#include "GLCD.h"
#include "LED.h"
#include "ADC.h"
#include "KBD.h"
#include "RTL.h"
#define wait 5000000
#define blocksize 16
#define xBoardLeft 80
#define xBoardRight 240
#define screenX 320
#define screenY 240
#define IMAGE 1
#define MAIN_MENU   0               	/* Font index 16x24            	*/
#define GAME 	1    
#define GALLERY    	2                	/* Font index 16x24            	*/
#define MUSIC   	3    
int arrow=1;
bool BACK=false;
uint32_t joyDirection;
void delay(int a){
 long k, count=0;
 for (k=0;k<=a;k++){
  count++;
 }
}

extern  void SystemClockUpdate(void);
extern uint32_t SystemFrequency;  

int dir=0;
int leveltimer=1;

extern unsigned char IMAGE0_pixel_data[];
extern unsigned char IMAGE1_pixel_data[];
extern unsigned char IMAGE2_pixel_data[];
extern unsigned char IMAGE3_pixel_data[];

extern unsigned char NEON_BLUE_pixel_data[];
extern unsigned char NEON_RED_pixel_data[];
extern unsigned char NEON_YELLOW_pixel_data[];
extern unsigned char NEON_GREEN_pixel_data[];

extern unsigned char YELLOW_BLOCK_pixel_data[];
extern unsigned char BLUE_BLOCK_pixel_data[];
extern unsigned char RED_BLOCK_pixel_data[];
extern unsigned char GREEN_BLOCK_pixel_data[];
extern unsigned char TETRISLOGO_pixel_data[];

extern unsigned char 	BACK_pixel_data[];
extern unsigned char 	BLOCK_WHITE_pixel_data[];
extern unsigned char  BACKGREY1_pixel_data[];
extern unsigned char  BACK_GREY_pixel_data[];
extern void play_music (void);
char text[10];   	 

void startMenu(void);
void game(void);

void square();
void linefull();
void ChangeImage();
                        

uint8_t board[10][15];

#define __FI    	1         /* Font index 16x24*/
#define __USE_LCD   0   		/* Uncomment to use the LCD */
#define NEON_MODE   1

                             	/* Time Tick */

int rightedge=11;
int leftedge=1;
int selector=0;
uint32_t currentDir = 0;
int score;
void  DisplayMenu(int pointer);
int grid[10][16];
int prev_grid[10][16];
void blank(){
int x=0;
int y=0;    
for(y=0;y<16;y++){
    for(x=0;x<10;x++){
    grid[x][y]=0;
    if(y==15)
   	 grid[x][y]=1;
    }
}
GLCD_Bitmap(10*blocksize, 0, 160, 99, TETRISLOGO_pixel_data);
   	 GLCD_DisplayString(6,  10, __FI, "SCORE:");
}
bool NEON=true;


void drawgrid(int grid2[10][16]){
int x=0;
int y=0;


for(y=0;y<16;y++){
    for(x=0;x<10;x++){
   	 if(grid2[x][y]==0){
   		GLCD_Bitmap((x)*blocksize, y*blocksize, blocksize, blocksize, BACK_GREY_pixel_data);
   	}else if(grid2[x][y]==1){
   			if(NEON)
   				 GLCD_Bitmap((x)*blocksize, y*blocksize, blocksize, blocksize, NEON_GREEN_pixel_data);
   			else
   					GLCD_Bitmap((x)*blocksize, y*blocksize, blocksize, blocksize, GREEN_BLOCK_pixel_data);

    }else if(grid2[x][y]==2){
   			if(NEON)
   						GLCD_Bitmap((x)*blocksize, y*blocksize, blocksize, blocksize,NEON_BLUE_pixel_data);
				else
   						 GLCD_Bitmap((x)*blocksize, y*blocksize, blocksize, blocksize, BLUE_BLOCK_pixel_data);
   		 
   	 }else if(grid2[x][y]==3){
   			if(NEON)
   					 GLCD_Bitmap((x)*blocksize, y*blocksize, blocksize, blocksize, NEON_RED_pixel_data);
   			else
   					GLCD_Bitmap((x)*blocksize, y*blocksize, blocksize, blocksize, RED_BLOCK_pixel_data);
   		 
   	}else if(grid2[x][y]==4){
   			if(NEON)
   				GLCD_Bitmap((x)*blocksize, y*blocksize, blocksize, blocksize, NEON_YELLOW_pixel_data);
				else
   				GLCD_Bitmap((x)*blocksize, y*blocksize, blocksize, blocksize, YELLOW_BLOCK_pixel_data);
   		 
		}else if(grid2[x][y]==5){
					GLCD_Bitmap((x)*blocksize, y*blocksize, blocksize, blocksize, BLOCK_WHITE_pixel_data);    
  	}
   }
 }
}
void square(){
int x=5;
int y=0;    
uint32_t gameButton = 0;
int ON = 1;    
int OFF = 0;
long k, count=0;
int delayint=0;
bool exit=false;    
    
for(y=1;y<16;y++){
    
delay(6000000/leveltimer);

gameButton=get_button();
    if(grid[x][y+1]==0 &&
   	 grid[x+1][y+1]==0){
   	 grid[x][y]=ON;
   	 grid[x+1][y]=ON;
   	 grid[x][y+1]=ON;
   	 grid[x+1][y+1]=ON;
   	 grid[x][y-1]=OFF;
   	 grid[x+1][y-1]=OFF;
if (gameButton==(KBD_RIGHT&KBD_MASK)){
    if(grid[x+1+1][y]==0 && grid[x+1+1][y+1]==0 && x < 8){
   	 grid[x][y]=OFF;
   	 grid[x+1][y]=OFF;
   	 grid[x][y+1]=OFF;
   	 grid[x+1][y+1]=OFF;
   	 x++;
   	 grid[x][y]=ON;
   	 grid[x+1][y]=ON;
   	 grid[x][y+1]=ON;
   	 grid[x+1][y+1]=ON;}
    }
    if (gameButton==(KBD_LEFT&KBD_MASK)){
    if(grid[x-1][y]==0 && grid[x-1][y+1]==0 && x > 0){
   	 grid[x][y]=OFF;
   	 grid[x+1][y]=OFF;
   	 grid[x][y+1]=OFF;
   	 grid[x+1][y+1]=OFF;
   	 x--;
   	 grid[x][y]=ON;
   	 grid[x+1][y]=ON;
   	 grid[x][y+1]=ON;
   	 grid[x+1][y+1]=ON;
   		 }
    }
    drawgrid(grid);
    }
    else{
   	 exit = true;
   	 break;   	 
    }
 }
}
void backupgrid(){
int x=0;
int y=0;
    
for(y=0;y<16;y++){
    for(x=0;x<10;x++){
   	 prev_grid[x][y]= grid[x][y];
    }
}
}

void restoregrid(){
int x=0;
int y=0;    
for(y=0;y<16;y++){
    for(x=0;x<10;x++){
   	  grid[x][y]=prev_grid[x][y];
    }
}
}


int totalscore=0;
int levelup=0;
void linefull(){
char text[10];
int x=0;
int y=0;
    int indexy=0;
   		 int b;
   		 int a=0;
   	 int i;
int counter=0;


    
for(y=0;y<15;y++){

counter = 0;
    for(x=0;x<10;x++){
   	 if(grid[x][y]>=1){
   	 counter++;
   	 }else{
   	 counter = 0;
   	 }
    }
    if(counter >= 10){
   		 backupgrid();    
   	 //GLCD_Bitmap(11*blocksize, 14*blocksize, blocksize, blocksize, block_white);
   	 for(a=0;a<10;a++){
   		 grid[a][y]=0;
   		 drawgrid(grid);
   	 }
   	 for(a=0;a<10;a++){
   		 grid[a][y]=5;
   	 }
   	 drawgrid(grid);
   	 delay(1000000);
   	 restoregrid();
   	 drawgrid(grid);
   	 delay(1000000);
   	 for(a=0;a<10;a++){
   		 grid[a][y]=5;
   	 }
   	 drawgrid(grid);
   	 for(a=0;a<10;a++){
   		 grid[a][y]=0;
   		 drawgrid(grid);
   	 }
   	 levelup++;
   	 if(levelup==2){
   	 leveltimer++;
   	 levelup=0;
   	 }   	 
   	 GLCD_SetTextColor(White);
   	 totalscore++;
   	 GLCD_DisplayString(6,  10, __FI, "SCORE:");
   	 sprintf(text,"%d",totalscore);
   	 GLCD_DisplayString(7,  10, __FI, (unsigned char *)text);
   	 GLCD_DisplayString(8,  10, __FI, "LEVEL:");
   	 sprintf(text,"%d",leveltimer);
   	 GLCD_DisplayString(9,  10, __FI, (unsigned char *)text);
   	 counter= 0;
   	 indexy = y;   	 
    for(b=indexy;b>0;b--){
   	 for(i=0;i<10;i++){
   	 grid[i][b]=grid[i][b-1];
   	 }
   	 }
   	 NEON=!NEON;   	 
    }
}
}

void lshape1(int rot){
int x=5;
int y=0;
int ON = 2;    
int OFF = 0;
uint32_t gameButton = 0;
    
backupgrid();
for(y=1;y<15;y++){
delay(6000000/leveltimer);
   	 gameButton=get_button();
    if(rot==0){
    gameButton=get_button();    
    if(grid[x][y+1]==0 && grid[x+1][y]==0 && grid[x+2][y]==0){
   	 grid[x][y]=ON;
   	 grid[x+1][y]=ON;
   	 grid[x+2][y]=ON;
   	 grid[x][y+1]=ON;   	 
   	 grid[x][y-1]=OFF;
   	 grid[x+1][y-1]=OFF;
   	 grid[x+2][y-1]=OFF;
   	 if (gameButton==(KBD_RIGHT&KBD_MASK) && x < 7){
   	 if(grid[x+2+1][y]==0 && grid[x+1][y+1]==0){
   	 grid[x][y]=OFF;
   	 grid[x+1][y]=OFF;
   	 grid[x+2][y]=OFF;
   	 grid[x][y+1]=OFF;
   	 x++;
   	 grid[x][y]=ON;
   	 grid[x+1][y]=ON;
   	 grid[x+2][y]=ON;
   	 grid[x][y+1]=ON;
   		 }                                      	 
    }
if (gameButton==(KBD_SELECT&KBD_MASK)){
    rot++;
    if(rot>3){
   	 rot=0;}
    restoregrid();
    }
    if (gameButton==(KBD_LEFT&KBD_MASK) && x > 0){
    if(grid[x-1][y]==0 && grid[x-1][y+1]==0){
   	 grid[x][y]=OFF;
   	 grid[x+1][y]=OFF;
   	 grid[x+2][y]=OFF;
   	 grid[x][y+1]=OFF;
   	 x--;
   	 grid[x][y]=ON;
   	 grid[x+1][y]=ON;
   	 grid[x+2][y]=ON;
   	 grid[x][y+1]=ON;
    }}
    drawgrid(grid);
    }
    else{
   	 break;
    }
    }
    if(rot==1){
    gameButton=get_button();
    if(grid[x][y+2]==0 && grid[x+1][y+2]==0){
   	 grid[x][y]=ON;
   	 grid[x][y+1]=ON;
   	 grid[x][y+2]=ON;
   	 grid[x+1][y+2]=ON;
   	 grid[x][y-1]=OFF;
   	 grid[x+1][y+2-1]=OFF;
   	 if (gameButton==(KBD_RIGHT&KBD_MASK) && x < 8){
   	 if(grid[x+1][y]==0 &&
   			 grid[x+1][y+1]==0 &&
   			 grid[x+1+1][y+2]==0){    
   	 grid[x][y]=OFF;
   	 grid[x][y+1]=OFF;
   	 grid[x][y+2]=OFF;
   	 grid[x+1][y+2]=OFF;
   	 x++;
   	 grid[x][y]=ON;
   	 grid[x][y+1]=ON;
   	 grid[x][y+2]=ON;
   	 grid[x+1][y+2]=ON;
   	 }}
if (gameButton==(KBD_SELECT&KBD_MASK)){
    rot++;
    if(rot>3){
   	 rot=0;}
   	 restoregrid();
    }
    if (gameButton==(KBD_LEFT&KBD_MASK)&& x > 0){
   	 if(grid[x-1][y]==0 &&
   		  grid[x-1][y+1]==0 &&
   		  grid[x-1][y+2]==0 ){    
   	 grid[x][y]=OFF;
   	 grid[x][y+1]=OFF;
   	 grid[x][y+2]=OFF;
   	 grid[x+1][y+2]=OFF;
   	 x--;
   	 grid[x][y]=ON;
   	 grid[x][y+1]=ON;
   	 grid[x][y+2]=ON;
   	 grid[x+1][y+2]=ON;
    }}
    drawgrid(grid);
    }
    else{
   	 break;   	 
    }
    }
    if(rot==2){
   	 gameButton=get_button();    
    if(grid[x][y+1]==0 &&
   	 grid[x+1][y+1]==0&&
   	 grid[x+2][y+1]==0){	 
   	 grid[x][y+1]=ON;
   	 grid[x+1][y+1]=ON;
   	 grid[x+2][y+1]=ON;
   	 grid[x+2][y]=ON;   	 
   	 grid[x][y+1-1]=OFF;
   	 grid[x+1][y+1-1]=OFF;
   	 grid[x+2][y-1]=OFF;   	 
   	 if (gameButton==(KBD_RIGHT&KBD_MASK) && x < 7){
   	 if(grid[x+1+2][y+1]==0 &&
   	 grid[x+1+2][y]==0){    
   	 grid[x][y+1]=OFF;
   	 grid[x+1][y+1]=OFF;
   	 grid[x+2][y+1]=OFF;
   	 grid[x+2][y]=OFF;
   	 x++;
   	 grid[x][y+1]=ON;
   	 grid[x+1][y+1]=ON;
   	 grid[x+2][y+1]=ON;
   	 grid[x+2][y]=ON;
   	 }}
if (gameButton==(KBD_SELECT&KBD_MASK)){
    rot++;
    if(rot>3){
   	 rot=0;}
   	 restoregrid();
    }
    if (gameButton==(KBD_LEFT&KBD_MASK) && x > 0){
   	 if(grid[x-1][y+1]==0 && grid[x-1+2][y]==0){    
   	 grid[x][y+1]=OFF;
   	 grid[x+1][y+1]=OFF;
   	 grid[x+2][y+1]=OFF;
   	 grid[x+2][y]=OFF;
   	 x--;
   	 grid[x][y+1]=ON;
   	 grid[x+1][y+1]=ON;
   	 grid[x+2][y+1]=ON;
   	 grid[x+2][y]=ON;
    }}
    drawgrid(grid);
    }
    else{
   	 break; 	 
}
}
    if(rot==3){
   	gameButton=get_button();    
    if(grid[x][y]==0 &&    grid[x+1][y+2]==0){   		 
   	 grid[x][y]=ON;
   	 grid[x+1][y]=ON;
   	 grid[x+1][y+1]=ON;
   	 grid[x+1][y+2]=ON;   	 
   	 grid[x][y-1]=OFF;
   	 grid[x+1][y-1]=OFF;   	 
   	 if (gameButton==(KBD_RIGHT&KBD_MASK) && x < 8){
   	 if(grid[x+1+1][y]==0 &&
   	 grid[x+1+1][y+1]==0 &&
   	 grid[x+1+1][y+2]==0){    
   	 grid[x][y]=OFF;
   	 grid[x+1][y]=OFF;
   	 grid[x+1][y+1]=OFF;
   	 grid[x+1][y+2]=OFF;
   	 x++;
   	 grid[x][y]=ON;
   	 grid[x+1][y]=ON;
   	 grid[x+1][y+1]=ON;
   	 grid[x+1][y+2]=ON;
   	 }}
if (gameButton==(KBD_SELECT&KBD_MASK)){
    rot++;
    if(rot>3){
   	 rot=0;}
   	 restoregrid();
    }
    if (gameButton==(KBD_LEFT&KBD_MASK) && x > 0){
   	 if(grid[x+1-1][y+1]==0 &&
   	 grid[x+1-1][y+2]==0 && grid[x-1][y]==0){    
   	 grid[x][y]=OFF;
   	 grid[x+1][y]=OFF;
   	 grid[x+1][y+1]=OFF;
   	 grid[x+1][y+2]=OFF;
   	 x--;
   	 grid[x][y]=ON;
   	 grid[x+1][y]=ON;
   	 grid[x+1][y+1]=ON;
   	 grid[x+1][y+2]=ON;
}}
    drawgrid(grid);
    }
    else{
   	 break;
    }
    }
}
}
void lineshape(int rot){   	     
int x=5;
int y=0;
int ON = 3;    
int OFF = 0;
uint32_t gameButton = 0;
    
backupgrid();
for(y=1;y<16;y++){
delay(6000000/leveltimer);
    if(rot==0){
    gameButton=get_button();
    if(grid[x][y]==0 &&
   	 grid[x+1][y]==0 &&
   	 grid[x+2][y]==0 &&
   	 grid[x+3][y]==0 ){
   	 grid[x][y]=ON;
   	 grid[x+1][y]=ON;
   	 grid[x+2][y]=ON;
   	 grid[x+3][y]=ON;   	 
   	 grid[x][y-1]=OFF;
   	 grid[x+1][y-1]=OFF;
   	 grid[x+2][y-1]=OFF;
   	 grid[x+3][y-1]=OFF;
   	 
   	 if (gameButton==(KBD_RIGHT&KBD_MASK) && x < 6){
   		 if(grid[x+3+1][y]==0){
   	 grid[x][y]=OFF;
   	 grid[x+1][y]=OFF;
   	 grid[x+2][y]=OFF;
   	 grid[x+3][y]=OFF;
   	 x++;
   	 grid[x][y]=ON;
   	 grid[x+1][y]=ON;
   	 grid[x+2][y]=ON;
   	 grid[x+3][y]=ON;
   		 }                                      	 
    }
if (gameButton==(KBD_SELECT&KBD_MASK)){
   		 rot++;
    if(rot>1){
   	 rot =0;
    }
    restoregrid();
    }
    if (gameButton==(KBD_LEFT&KBD_MASK) && x > 0){
    if(grid[x-1][y]==0){
   	 grid[x][y]=OFF;
   	 grid[x+1][y]=OFF;
   	 grid[x+2][y]=OFF;
   	 grid[x+3][y]=OFF;
   	 x--;
   	 grid[x][y]=ON;
   	 grid[x+1][y]=ON;
   	 grid[x+2][y]=ON;
   	 grid[x+3][y]=ON;
    }}
    drawgrid(grid);
    }
    else{
   	 break;
    }
    }
    if(rot==1){
    gameButton=get_button();
    if(grid[x][y+3]==0){
   	 grid[x][y]=ON;
   	 grid[x][y+1]=ON;
   	 grid[x][y+2]=ON;
   	 grid[x][y+3]=ON;
   	 grid[x][y-1]=OFF;
   	 if (gameButton==(KBD_RIGHT&KBD_MASK) && x < 9){
   	 if(grid[x+1][y]==0 &&
   	 grid[x+1][y+1]==0 &&
   	 grid[x+1][y+2]==0 &&
   	 grid[x+1][y+3]==0){    
   		 
   	 grid[x][y]=OFF;
   	 grid[x][y+1]=OFF;
   	 grid[x][y+2]=OFF;
   	 grid[x][y+3]=OFF;
   	 x++;
   	 grid[x][y]=ON;
   	 grid[x][y+1]=ON;
   	 grid[x][y+2]=ON;
   	 grid[x][y+3]=ON;
   	 }}
if (gameButton==(KBD_SELECT&KBD_MASK)){
    rot++;
    if(rot>1){
   	 rot=0;
    }
   	 restoregrid();
    }
    if (gameButton==(KBD_LEFT&KBD_MASK)&& x > 0){
   	 if(grid[x-1][y]==0 &&
   	 grid[x-1][y+1]==0 &&
   	 grid[x-1][y+2]==0 &&
   	 grid[x-1][y+3]==0){    
   	 grid[x][y]=OFF;
   	 grid[x][y+1]=OFF;
   	 grid[x][y+2]=OFF;
   	 grid[x][y+3]=OFF;
   	 x--;
   	 grid[x][y]=ON;
   	 grid[x][y+1]=ON;
   	 grid[x][y+2]=ON;
   	 grid[x][y+3]=ON;
    }}
    drawgrid(grid);
    }
    else{
   	 break;
   	 
    }
    }
}
}
bool game_over(){
    int x;
    bool result= false;
    for(x=0;x<10;x++){
if(grid[x][3]>=1){
    result = true;
    GLCD_Clear(Black);
    GLCD_DisplayString(6,  0, __FI, "GAME OVER");		
    sprintf(text,"SCORE:%d",totalscore);
    GLCD_DisplayString(7,  0, __FI,  (unsigned char *)text);
    GLCD_DisplayString(8,  0, __FI, "Press UP to END GAME");
}

}
    return result;
}

void random_block(){
int x= rand()%3;
if(x==0)
    lshape1(1);
else if(x==1)
    square();
else
    lineshape(0);

}
void game(void){
   	 bool run = true;
    GLCD_Clear(Black);
    GLCD_DisplayString(6,  10, __FI, "SCORE:");
    sprintf(text,"%d",totalscore);
    GLCD_DisplayString(7,  10, __FI,(unsigned char *)text);
    blank();
    totalscore=0;
     
    while(run){
   	 if(game_over()!=true){
   	 random_block();
   	 linefull();
   	 }
   	 joyDirection = get_button();
   	 if(KBD_UP == joyDirection){
   		 break;
   	 }   	 
    }
    GLCD_Clear(Black);
    DisplayMenu(arrow);    
}

void  DisplayMenu(int pointer){

  GLCD_SetBackColor(Red);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, " COE718 MEDIA CENTER   ");
  GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "  FINAL PROJECT      	 ");
	GLCD_SetTextColor(White);
  GLCD_SetBackColor(Black);
  GLCD_DisplayString(4, 0, __FI, "  PHOTO GALLERY         ");
  GLCD_DisplayString(5, 0, __FI, "  MUSIC PLAYER          ");
  GLCD_DisplayString(6, 0, __FI, "  MY GAME            	  ");
  GLCD_SetTextColor(Green);
    switch(pointer) {
   case GALLERY:
   	      GLCD_DisplayString(4, 0, __FI, "->PHOTO GALLERY        ");
  	break;
   case MUSIC:
   	      GLCD_DisplayString(5, 0, __FI, "->MUSIC PLAYER         ");
					
  	break;
     case GAME:
		      GLCD_DisplayString(6, 0, __FI, "->MY GAME        	     ");
  	break;
   default :
   		 break;
}
    GLCD_SetTextColor(White);
    delay(5000000);

}

int imagenum=0;
void ChangeImage(int x){
switch(x) {	 
case 0:
	GLCD_Clear(Black);
	GLCD_Bitmap(0,0,320,240,IMAGE0_pixel_data);
break;
case 1:
	GLCD_Clear(Black);       	 
   //GLCD_Bitmap((320/2)-(172/2), (240/2)-(172/2),180,180,IMAGE1_pixel_data);
break;
case 2:
	//GLCD_Clear(White);
	//GLCD_Bitmap((320/2)-(172/2), (240/2)-(172/2),180,180,IMAGE2_pixel_data);
break;
case 3:
	//GLCD_Clear(Black);
  //GLCD_Bitmap(0,0,(180),(180),IMAGE3_pixel_data);
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
				while(!BACK){
				joyDirection= get_button();
				switch(joyDirection){
				case KBD_SELECT:
				BACK=true;
				DisplayMenu(arrow);
			}
		}	
  play_music();
}
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
   	  imagenum=3;
     ChangeImage(imagenum);
  	break;
   case KBD_RIGHT:
   	  imagenum++;
          if(imagenum>3)
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
bool start = true;
   	 switch(pointer) {
   	 case GALLERY:
   	  GalleryLoop();
  	break;
   	 case MUSIC:
   	      MusicLoop();
		 if(get_button() == KBD_LEFT){
		NVIC_DisableIRQ(USB_IRQn);
		NVIC_DisableIRQ(TIMER0_IRQn);
		USB_Connect(0);
		USB_Reset();
		USB_Connect(1);
	}
  	break;
     case GAME:
   		 GLCD_Clear(Black);
   		 GLCD_DisplayString(5,  0, __FI, "Press [SELECT] to START");
   		 GLCD_DisplayString(6,  0, __FI, "Press [UP] to EXIT");    
   	 while(1){
   		 joyDirection = get_button();
   	 if(KBD_UP == joyDirection){
   			 start = false;
   		 break;
   	 }
   	 if(KBD_SELECT == joyDirection){
   		 start = true;
   		 break;}
   	 }
   	 if(start)    
		      game();
   	 else
   	   DisplayMenu(arrow);
  	break;    
   default :
   		 break;
    }
}
//#define GALLARY 	1    
//#define GAME    	2                	/* Font index 16x24            	*/
//#define MUSIC   	3    
int main (void) {
  LED_Init ();
    GLCD_Init();    
    LED_Init();
    GLCD_Clear(Black);
    DisplayMenu(arrow);
		
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
  	  break;    		 
      default :
   		break;
    }    
}
}

	