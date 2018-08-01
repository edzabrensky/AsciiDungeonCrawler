#ifndef _HELP_H_
#define _HELP_H_
#include "common.h"
#include "nokia5110.h"
enum helpStates { helpScreenWait, helpScreenStart};



char message[778] = "These are the bosses. Points are earned by beating bosses. The goal is to get the highest amount of points.The game is turned based. During your turn you draw 5 cards. The type of cards are attack, defend, and power.Attack(X):Attacks enemy for X amount of HP. Defend(X): Gain a *shield for X amount. *Shield falls off after enemy's turn. PowerUp(X): Increases attack damage by X amount.You can only play up to 3 cards per turn.After your turn it is the enemy's turn.Above the enemy is their intent.! or d are the intents. (!x) means the enemy will attack for x amount. (dx) means they will defend for x amount. You can defeat enemies by bringing their hp down to 0.You start with 5 attack cards, 5 defend, and 2 powerup cards. You can gain cards after you win a combat.\0";
unsigned char helpPageNumber;
int helpTick(int state) {
	switch(state) {
		case helpScreenWait:
			if(mode == 0x08) {
				nokia_lcd_clearM(0x08);
				state = helpScreenStart; 
				helpPageNumber = 0x00;
			}
			else {
				state = helpScreenWait;
			}
		break;
		case helpScreenStart:
			if(mode == 0x08) {
				state = helpScreenStart; 
			}
			else {
				state = helpScreenWait;
			}
		break;
		default:
			state = helpScreenWait;
		break;

	}
	switch(state) {
		case helpScreenWait:
		break;
		case helpScreenStart:
			nokia_lcd_clearM(0x08);
			unsigned char i = 0; 
			unsigned char j = 0;
			if(choice == 0) {
				nokia_lcd_write_stringM("     ", 1, 0x08);
				nokia_lcd_write_charM(0x80, 1, 0x08);//output custom characters
				nokia_lcd_write_charM(0x81, 1, 0x08);
				nokia_lcd_write_charM(0x82, 1, 0x08);
				for(i = 0; i < 3; ++i) {
					nokia_lcd_set_cursorM(0, 10*(i+1), 0x08);
					for(j = 0; j < 14; ++j) {
						nokia_lcd_write_charM(message[j+(i*14)], 1, 0x08);
					}
				}
			}
			else if(choice == 1) {
				for(i = 0; i < 4; ++i) {
					nokia_lcd_set_cursorM(0, 10*(i), 0x08);
					for(j = 0; j < 14; ++j) {
						nokia_lcd_write_charM(message[j+(i*14)+(43*choice)], 1, 0x08);
					}
				}
			}
			else {
				for(i = 0; i < 4; ++i) {
					nokia_lcd_set_cursorM(0, 10*(i), 0x08);
					for(j = 14; j > 0; --j) {
						if(message[(i*14)+(56*choice) - j] != '\0') {
							nokia_lcd_write_charM(message[(i*14)+(56*choice) - j], 1, 0x08);
						}
					}
				}
			}
			nokia_lcd_set_cursorM(0, 40, 0x08);
			char buff[5];
			itoa(choice+1, buff, 10);
			nokia_lcd_write_stringM("    (", 1, 0x08);
			nokia_lcd_write_stringM(buff, 1, 0x08);
			nokia_lcd_write_stringM("/", 1, 0x08);
			itoa(NUM_HELP_PAGES, buff, 10);
			nokia_lcd_write_stringM(buff, 1, 0x08);
			nokia_lcd_write_stringM(")", 1, 0x08);
		break;
		default:
		break;
	}
	return state;
}







#endif