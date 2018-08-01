#ifndef __HIGH_SCORE_H__
#define __HIGH_SCORE_H__
#include "common.h"
#include "nokia5110.h"


enum highScoreScreenStates { highScoreScreenWait, highScoreScreenStart};

int highScoreTick(int state) {
	switch(state) {
		case highScoreScreenWait:
			if(mode == 0x02) {
				state = highScoreScreenStart;
				nokia_lcd_clearM(0x02);
			}
			else {
				state = highScoreScreenWait;
			}
		break;
		case highScoreScreenStart:
			if(mode == 0x02) {
				state = highScoreScreenStart;
			}
			else {
				state = highScoreScreenWait;
			}
		break;
		default:
			state = highScoreScreenWait;
		break;
	}
	switch(state) {
		case highScoreScreenWait:
		break;
		case highScoreScreenStart:
			nokia_lcd_clearM(0x02);
			nokia_lcd_write_stringM(" HIGHSCORE:", 1, 0x02);
			char buff[5];
			itoa(highScore, buff, 10);
			nokia_lcd_set_cursorM(0, 10, 0x02);
			nokia_lcd_write_stringM(buff, 2, 0x02);
		break;
	}

	return state;
}

















#endif