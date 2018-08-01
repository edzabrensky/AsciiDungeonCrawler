#ifndef _OUTPUT_LCD_H__
#define _OUTPUT_LCD_H__
#include "common.h"
#include "nokia5110.h"


enum lcdState {lcdStart};


int outputTick(int state) {
	switch(state) {
		case lcdStart:
			state = lcdStart;
		break;
		default:
			state = lcdStart;
		break;
	}
	switch(state) {
		case lcdStart:
			nokia_lcd_renderM(mode);
		break;
	}
	return state;
}




#endif