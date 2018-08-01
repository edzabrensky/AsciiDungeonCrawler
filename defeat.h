#ifndef _DEFEAT_H__
#define _DEFEAT_H__
#include "common.h"
#include "nokia5110.h"


enum defeatScreenStates {
		defeatWaitScreen,
		defeatStartScreen
};

//unsigned long k;
int defeatTick(int state) {
	switch(state) {
		case defeatWaitScreen:
			if(mode == 0x05) {
				state = defeatStartScreen;
			}
			else {
				state = defeatWaitScreen;
			}
		break;
		case defeatStartScreen:
			if(mode == 0x05) {
				state = defeatStartScreen;
			}
			else {
				state = defeatWaitScreen;
			}
		break;
		default:
			state = defeatWaitScreen;
		break;
	}
	switch(state) {
		case defeatWaitScreen:
		break;
		case defeatStartScreen:
				nokia_lcd_clearM(0x05);
				nokia_lcd_write_stringM(" DEFEAT", 2, 0x05);
				if(newHighScoreBool == 0x01) {
					nokia_lcd_set_cursorM(0,30, 0x05);
					nokia_lcd_write_stringM("New", 1, 0x05);
					nokia_lcd_set_cursorM(0, 40, 0x05);
					nokia_lcd_write_stringM("Highscore: ", 1, 0x05);
					char buff[5];
					itoa(score, buff, 10);
					nokia_lcd_write_stringM(buff, 1, 0x05);
				}
		break;
	}
	return state;
}





#endif