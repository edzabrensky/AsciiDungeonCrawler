#ifndef _VICTORY_H__
#define _VICTORY_H__
#include "common.h"
#include "nokia5110.h"

enum victoryStates { victoryWait,
					 victoryScreen
					};

//mode = 0x06
//Victory!
//Choose Reward
// d1 a2 p4 skip
//Progress Saved 
//   After Reward
card rewardCard[3];
int victoryTick(int state) {
	switch(state) {
		case victoryWait:
			if(mode == 0x06) {
				state = victoryScreen;
				nokia_lcd_clearM(0x06);
				nokia_lcd_write_stringM("   Victory!", 1, 0x06);
				nokia_lcd_set_cursorM(0,10, 0x06);
				nokia_lcd_write_stringM("Select Reward:", 1, 0x06);
				nokia_lcd_set_cursorM(0,20, 0x06);
				unsigned char i = 0;
				unsigned char randSymbol;
				for(i = 0; i < 3; ++i) {
					randSymbol = rand() % 3;
					if(randSymbol == 0) {
						rewardCard[i].symbol    = 'a';
					}
					else if(randSymbol == 1) {
						rewardCard[i].symbol    = 'd';
					}
					else {
						rewardCard[i].symbol    = 'p';
					}
					if(rewardCard[i].symbol == 'p') {
						rewardCard[i].magnitude = rand() % 2 + 1;
					}
					else {
						rewardCard[i].magnitude = rand() % 3 + 2;
					}
				} 
				for(i = 0; i < 3; ++i) {
					nokia_lcd_write_stringM(" ", 1, 0x06);
					nokia_lcd_write_charM(rewardCard[i].symbol, 1, 0x06);
					char buff[5];
					itoa(rewardCard[i].magnitude, buff, 10);
					nokia_lcd_write_stringM(buff, 1, 0x06);
				}
				nokia_lcd_write_stringM(" skip", 1, 0x06);
				nokia_lcd_set_cursorM(0, 30, 0x06);
				nokia_lcd_write_stringM("Progress Saves", 1, 0x06);
				nokia_lcd_set_cursorM(0, 40, 0x06);
				nokia_lcd_write_stringM("  After Reward", 1, 0x06);
			}
			else {
				state = victoryWait;
			}
		break;
		case victoryScreen:
			if(mode == 0x06) {
				state = victoryScreen;
			}
			else {
				state = victoryWait;
			}
		break;
		default:
			state = victoryWait;
		break;
	}
	switch(state) {
		case victoryWait:

		break;
		case victoryScreen:
			nokia_lcd_set_cursorM(0, 20, 0x06);
			nokia_lcd_write_charM(' ', 1, 0x06);
			nokia_lcd_set_cursorM(18, 20, 0x06);
			nokia_lcd_write_charM(' ', 1, 0x06);
			nokia_lcd_set_cursorM(36, 20, 0x06);
			nokia_lcd_write_charM(' ', 1, 0x06);
			nokia_lcd_set_cursorM(54, 20, 0x06);
			nokia_lcd_write_charM(' ', 1, 0x06);
			if(choice > 0) {
				nokia_lcd_set_cursorM(choice*18, 20, 0x06);
			}
			else {
				nokia_lcd_set_cursorM(0,20, 0x06);
			}
			nokia_lcd_write_charM('>', 1, 0x06);
		break;

	}

	return state;
}


#endif