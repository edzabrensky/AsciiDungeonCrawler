#ifndef __COMBAT_CHOICE_SCREEN_H__
#define __COMBAT_CHOICE_SCREEN_H__
#include "common.h"
#include "nokia5110.h"


enum combatChoiceStates { ccScreenWait, 
						  cc1,
						  cc2,
						  cc3};

int ccTick(int state) {
	switch(state) {
		case ccScreenWait:
			if(mode == 0x04) {
				nokia_lcd_clearM(0x04);
				nokia_lcd_write_stringM(" 1boss|Point+1", 1, 0x04);
    			nokia_lcd_set_cursorM(0, 10, 0x04);
    			nokia_lcd_write_stringM(" 2boss|Point+3", 1, 0x04);
    			nokia_lcd_set_cursorM(0, 20, 0x04);
    			nokia_lcd_write_stringM(" 1boss|HP+8", 1, 0x04);
    			nokia_lcd_set_cursorM(0, 30, 0x04);
  				char buff[5];
  				itoa(HP, buff, 10);
    			nokia_lcd_write_stringM("HP",1, 0x04);
    			nokia_lcd_write_stringM(buff, 1, 0x04);
    			nokia_lcd_write_stringM("/30|Pts:", 1, 0x04);
    			itoa(score, buff, 10);
    			nokia_lcd_write_stringM(buff, 1, 0x04);
    			nokia_lcd_set_cursorM(0, 40, 0x04);
    			nokia_lcd_write_stringM(" Select Battle", 1, 0x04);
				state = cc1;
			} 
			else {
				state = ccScreenWait;
			}
		break;
		case cc1:
			if(mode != 0x04) {
				state = ccScreenWait;
			}
			else {
				if(choice == 0x00) {
					state = cc1;
				}
				else if(choice == 0x01) {
					state = cc2;
				}
				else if(choice == 0x02) {
					state = cc3;
				}
				else {
					state = ccScreenWait;
				}
				nokia_lcd_set_cursorM(0, 0, 0x04);
				nokia_lcd_write_stringM(" 1boss|Point+1",1, 0x04);
			}
		break;
		case cc2:
			if(mode != 0x04) {
				state = ccScreenWait;
			}
			else {
				if(choice == 0x00) {
					state = cc1;
				}
				else if(choice == 0x01) {
					state = cc2;
				}
				else if(choice == 0x02) {
					state = cc3;
				}
				else {
					state = ccScreenWait;
				}
				nokia_lcd_set_cursorM(0, 10, 0x04);
				nokia_lcd_write_stringM(" 2boss|Point+3",1, 0x04);
			}
		break;
		case cc3:
			if(mode != 0x04) {
				state = ccScreenWait;
			}
			else {
				if(choice == 0x00) {
					state = cc1;
				}
				else if(choice == 0x01) {
					state = cc2;
				}
				else if(choice == 0x02) {
					state = cc3;
				}
				else {
					state = ccScreenWait;
				}
				nokia_lcd_set_cursorM(0, 20, 0x04);
				nokia_lcd_write_stringM(" 1boss|HP+8",1, 0x04);
			}
		break;
		default:
			state = ccScreenWait;
		break;
	}
	switch(state) {
		case ccScreenWait:
		break;
		case cc1:
			nokia_lcd_set_cursorM(0, 0, 0x04);
			nokia_lcd_write_stringM(">1boss|Point+1",1, 0x04);
		break;
		case cc2:
			nokia_lcd_set_cursorM(0, 10, 0x04);
			nokia_lcd_write_stringM(">2boss|Point+3",1, 0x04);
		break;
		case cc3:
			nokia_lcd_set_cursorM(0, 20, 0x04);
			nokia_lcd_write_stringM(">1boss|HP+8",1, 0x04);
		break;
	}

	return state;
}


#endif