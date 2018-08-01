#ifndef __START_SCREEN_H__
#define __START_SCREEN_H__ 
#include "common.h"
#include "nokia5110.h"
unsigned char startScreenMessageStatus = 0x00; //blinks the select button message on and off


enum startScreenStates { startScreenWait,
							startScreen };
int startScreenTick(int state) {
	
	switch(state) {
		case startScreenWait:
			if(mode == 0x00) { //go to start screen
				state = startScreen;
				nokia_lcd_clearM(0x00);
				nokia_lcd_write_stringM("ASCII ",1, 0x00);//nokia_lcd_write_string(">1boss|Score+1",1);
    			nokia_lcd_set_cursorM(0, 10, 0x00);
    			nokia_lcd_write_stringM("    DUNGEON", 1, 0x00);
    			nokia_lcd_set_cursorM(0, 20, 0x00);
    			nokia_lcd_write_stringM("        RUN!@#", 1,0x00);
			}
			else {
				state = startScreenWait;
			}
		break;
		case startScreen:
			if(mode == 0x00) {
				state = startScreen;
			}
			else {
				state = startScreenWait;
			} 
		break;
		default:
			state = startScreenWait;
		break;
	}
	switch(state) {
		case startScreenWait:
		break;
		case startScreen:
			startScreenMessageStatus = ~startScreenMessageStatus; 
			if(startScreenMessageStatus) {
				nokia_lcd_set_cursorM(0, 30, 0x00);
				nokia_lcd_write_stringM("'o' Select to ", 1, 0x00);
				nokia_lcd_set_cursorM(0, 40, 0x00);
				nokia_lcd_write_stringM("   Start", 1, 0x00);
			}
			else {
				nokia_lcd_set_cursorM(0, 30, 0x00);
				nokia_lcd_write_stringM("                                    ", 1, 0x00);
			}
		break;
		default:
		break;
	}



	return state;
}










#endif