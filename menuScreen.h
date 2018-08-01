#ifndef __MENU_SCREEN_H__
#define __MENU_SCREEN_H__
#include "common.h"
#include "nokia5110.h"
enum menuStates { menuScreenWait, 
				  menuScreenPlay,      //00 in choice 
				  menuScreenHigh,      //01
				  menuScreenHelp,	   //02
				  menuScreenContinue };//03

int menuScreenTick(int state) {
	switch(state) {
		case menuScreenWait:
			if(mode == 0x01) {
				nokia_lcd_clearM(0x01);
    			nokia_lcd_write_stringM(" play new game  ", 1, 0x01);
    			nokia_lcd_set_cursorM(0, 10, 0x01);
    			nokia_lcd_write_stringM(" high score ", 1, 0x01);
    			nokia_lcd_set_cursorM(0, 20, 0x01);
    			nokia_lcd_write_stringM(" help ", 1, 0x01);
    			nokia_lcd_set_cursorM(0, 30, 0x01);
    			if(HP > 0 && HP <= 30) {
    				nokia_lcd_write_stringM(" continue ", 1, 0x01);
    			}
    			nokia_lcd_set_cursorM(30, 40, 0x01);
    			nokia_lcd_write_stringM("menu", 1, 0x01);
				state = menuScreenPlay;
			}
			else {
				state = menuScreenWait;
			}
		break;
		case menuScreenPlay:
			if(mode != 0x01) {
				state = menuScreenWait;
			}
			else if(choice == 0x00) {
				state = menuScreenPlay;
				nokia_lcd_set_cursorM(0, 0, 0x01);
    		nokia_lcd_write_stringM(" play new game ", 1, 0x01);
			}
			else if(choice == 0x01) {
				state = menuScreenHigh;
				nokia_lcd_set_cursorM(0, 0, 0x01);
    		nokia_lcd_write_stringM(" play new game ", 1, 0x01);
			}
			else if(choice == 0x02) {
				state = menuScreenHelp;
				nokia_lcd_set_cursorM(0, 0, 0x01);
    		nokia_lcd_write_stringM(" play new game ", 1, 0x01);
			}
			else if(choice == 0x03) {
				state = menuScreenContinue;
				nokia_lcd_set_cursorM(0, 0, 0x01);
    		nokia_lcd_write_stringM(" play new game ", 1, 0x01);
			}
			else {
				state = menuScreenWait;
			}
		break;
		case menuScreenHigh:
			if(mode != 0x01) {
				state = menuScreenWait;
			}
			else if(choice == 0x00) {
				state = menuScreenPlay;
				nokia_lcd_set_cursorM(0, 10, 0x01);
    		nokia_lcd_write_stringM(" high score ", 1, 0x01);
			}
			else if(choice == 0x01) {
				state = menuScreenHigh;
				nokia_lcd_set_cursorM(0, 10, 0x01);
    		nokia_lcd_write_stringM(" high score ", 1, 0x01);
			}
			else if(choice == 0x02) {
				state = menuScreenHelp;
				nokia_lcd_set_cursorM(0, 10, 0x01);
    		nokia_lcd_write_stringM(" high score ", 1, 0x01);
			}
			else if(choice == 0x03) {
				state = menuScreenContinue;
				nokia_lcd_set_cursorM(0, 10, 0x01);
    		nokia_lcd_write_stringM(" high score ", 1,0x01);
			}
			else {
				state = menuScreenWait;
			}
		break;
		case menuScreenHelp:
			if(mode != 0x01) {
				state = menuScreenWait;
			}
			else if(choice == 0x00) {
				state = menuScreenPlay;
				nokia_lcd_set_cursorM(0, 20, 0x01);
    		nokia_lcd_write_stringM(" help ", 1, 0x01);
			}
			else if(choice == 0x01) {
				state = menuScreenHigh;
				nokia_lcd_set_cursorM(0, 20, 0x01);
    		nokia_lcd_write_stringM(" help ", 1);
			}
			else if(choice == 0x02) {
				state = menuScreenHelp;
				nokia_lcd_set_cursorM(0, 20, 0x01);
    		nokia_lcd_write_stringM(" help ", 1, 0x01);
			}
			else if(choice == 0x03) {
				state = menuScreenContinue;
				nokia_lcd_set_cursorM(0, 20, 0x01);
    		nokia_lcd_write_stringM(" help ", 1, 0x01);
			}
			else {
				state = menuScreenWait;
			}
		break;
		case menuScreenContinue:
			if(mode != 0x01) {
				state = menuScreenWait;
			}
			else if(choice == 0x00) {
				state = menuScreenPlay;
				nokia_lcd_set_cursorM(0, 30, 0x01);
    		nokia_lcd_write_stringM(" continue ", 1, 0x01);
			}
			else if(choice == 0x01) {
				state = menuScreenHigh;
				nokia_lcd_set_cursorM(0, 30, 0x01);
    		nokia_lcd_write_stringM(" continue ", 1, 0x01);
			}
			else if(choice == 0x02) {
				state = menuScreenHelp;
				nokia_lcd_set_cursorM(0, 30, 0x01);
    		nokia_lcd_write_stringM(" continue ", 1, 0x01);
			}
			else if(choice == 0x03) {
				state = menuScreenContinue;
				nokia_lcd_set_cursorM(0, 30, 0x01);
    		nokia_lcd_write_stringM(" continue ", 1, 0x01);
			}
			else {
				state = menuScreenWait;
			}
		break;
		default:
			state = menuScreenWait;
		break;
	}
	switch(state) {
		case menuScreenWait:
		break;
		case menuScreenPlay:
			nokia_lcd_set_cursorM(0, 0, 0x01);
    		nokia_lcd_write_stringM(">play new game ", 1, 0x01);
    	break;
    	case menuScreenHigh:
    		nokia_lcd_set_cursorM(0, 10, 0x01);
    		nokia_lcd_write_stringM(">high score ", 1, 0x01);
    	break;
    	case menuScreenHelp:
    		nokia_lcd_set_cursorM(0, 20, 0x01);
    		nokia_lcd_write_stringM(">help ", 1, 0x01);
    	break;
    	case menuScreenContinue:
    		nokia_lcd_set_cursorM(0, 30, 0x01);
    		nokia_lcd_write_stringM(">continue ", 1, 0x01);
    	break;
		default:
		break;
	}

	return state;
}




#endif